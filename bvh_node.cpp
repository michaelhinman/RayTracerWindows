#include "bvh_node.h"
#include <spdlog/spdlog.h>
#include "ray.h"
#include "material.h"
#include <iostream>
namespace RT {
    namespace core {

        using namespace std;

        BVHNode::BVHNode(const std::string& name) :
            Surface{}
        {
            name_ = name.size() ? name : "BVHNode";
        }


        AABB
            BVHNode::GetBoundingBox(bool force_recompute)
        {
            // if bound is clean, just return existing bbox_
            if (!force_recompute && !IsBoundDirty())
                return bbox_;

            bbox_.Reset();
            if (left_)
                bbox_.ExpandBy(left_->GetBoundingBox(force_recompute));
            if (right_)
                bbox_.ExpandBy(right_->GetBoundingBox(force_recompute));
            // spdlog::info("{}: {}", GetName(), bbox_);
            bound_dirty_ = false;
            return bbox_;
        }


        bool
            BVHNode::Hit(const Ray& ray, Real tmin, Real tmax, HitRecord& hit_record)
        {
            if (bbox_.Hit(ray, tmin, tmax)) {
                HitRecord lrec, rrec;
                bool left_hit = (left_ != nullptr) and (left_->Hit(ray, tmin, tmax, lrec));
                bool right_hit = (right_ != nullptr) and (right_->Hit(ray, tmin, tmax, rrec));
                if (left_hit and right_hit) {
                    if (lrec.GetRayT() < rrec.GetRayT())
                        hit_record = lrec;
                    else
                        hit_record = rrec;
                    return true;
                }
                else if (left_hit) {
                    hit_record = lrec;
                    return true;
                }
                else if (right_hit)
                {
                    hit_record = rrec;
                    return true;
                }
            }
            return false;
        }

        BVHNode::Ptr
            BVHNode::BuildBVH(std::vector<Surface::Ptr> surfaces, const string& name)
        {
            spdlog::info("Building BVH ({})", name);

            // error checking
            auto surface_count = surfaces.size();
            if (!surface_count)
                return nullptr;

            // make sure we have valid bboxes for surfaces
            for (auto surface : surfaces) {
                if (surface)
                    surface->GetBoundingBox();
            }

            // build bvh
            uint split_axis = 0;
            auto bvh_node = BuildBVH(surfaces, 0, surface_count, split_axis, name);

            // compute bboxes
            if (bvh_node)
                bvh_node->GetBoundingBox();

            spdlog::info("Done building BVH ({})", name);
            return bvh_node;
        }

        AABB BVHNode::BBoxCombine(const AABB& left, const AABB& right)
        {
            Real minx = min(left.GetMin()[0], right.GetMin()[0]);
            Real miny = min(left.GetMin()[1], right.GetMin()[1]);
            Real minz = min(left.GetMin()[2], right.GetMin()[2]);
            Real maxx = max(left.GetMax()[0], right.GetMax()[0]);
            Real maxy = max(left.GetMax()[1], right.GetMax()[1]);
            Real maxz = max(left.GetMax()[2], right.GetMax()[2]);

            return AABB(Vec3r{ minx,miny,minz }, Vec3r{ maxx, maxy, maxz });
        }

        BVHNode::Ptr
            BVHNode::BuildBVH(std::vector<Surface::Ptr>& surfaces, size_t start,
                size_t end, uint split_axis, const std::string& name)
        {
            BVHNode::Ptr bvh_node = BVHNode::Create();
            size_t N = surfaces.size();
            if (N == 1)
            {
                bvh_node->left_ = surfaces[0];
                bvh_node->right_ = nullptr;
                bvh_node->bbox_ = surfaces[0]->GetBoundingBox();
            }
            else if (N == 2)
            {
                bvh_node->left_ = surfaces[0];
                bvh_node->right_ = surfaces[1];
                bvh_node->bbox_ = BBoxCombine(bvh_node->left_->GetBoundingBox(), bvh_node->right_->GetBoundingBox());
            }
            else
            {
                if (split_axis == 0)
                {
                    // Sort along x
                    std::sort(surfaces.begin(), surfaces.end(), [](const Surface::Ptr& lhs, const Surface::Ptr& rhs)
                        {
                            return lhs->GetBoundingBox().GetMin()[0] + ((lhs->GetBoundingBox().GetMax()[0] - lhs->GetBoundingBox().GetMin()[0]) / 2) >
                                rhs->GetBoundingBox().GetMax()[0] + ((rhs->GetBoundingBox().GetMax()[0] - rhs->GetBoundingBox().GetMin()[0]) / 2);
                        });
                }
                else if (split_axis == 1)
                {
                    // Sort along y
                    std::sort(surfaces.begin(), surfaces.end(), [](const Surface::Ptr& lhs, const Surface::Ptr& rhs)
                        {
                            return lhs->GetBoundingBox().GetMin()[1] + ((lhs->GetBoundingBox().GetMax()[1] - lhs->GetBoundingBox().GetMin()[1]) / 2) >
                                rhs->GetBoundingBox().GetMax()[1] + ((rhs->GetBoundingBox().GetMax()[1] - rhs->GetBoundingBox().GetMin()[1]) / 2);
                        });
                }
                else
                {
                    // Sort along z
                    std::sort(surfaces.begin(), surfaces.end(), [](const Surface::Ptr& lhs, const Surface::Ptr& rhs)
                        {
                            return lhs->GetBoundingBox().GetMin()[2] + ((lhs->GetBoundingBox().GetMax()[2] - lhs->GetBoundingBox().GetMin()[2]) / 2) >
                                rhs->GetBoundingBox().GetMax()[2] + ((rhs->GetBoundingBox().GetMax()[2] - rhs->GetBoundingBox().GetMin()[2]) / 2);
                        });
                }
                auto start_left = surfaces.begin();
                auto end_left = start_left + static_cast<int>(N / 2);
                std::vector<Surface::Ptr> left_sub(static_cast<int>(N / 2));
                std::copy(start_left, end_left, left_sub.begin());

                auto start_right = end_left;
                auto end_right = surfaces.end();
                std::vector<Surface::Ptr> right_sub(surfaces.size() - left_sub.size());
                std::copy(start_right, end_right, right_sub.begin());
                bvh_node->left_ = BuildBVH(left_sub, 0, (N / 2), (split_axis + 1) % 3);
                bvh_node->right_ = BuildBVH(right_sub, (N / 2), N - 1, (split_axis + 1) % 3);
                bvh_node->bbox_ = BBoxCombine(bvh_node->left_->GetBoundingBox(), bvh_node->right_->GetBoundingBox());
            }
            return bvh_node;
        }
    }  // namespace core
}  // namespace RT
