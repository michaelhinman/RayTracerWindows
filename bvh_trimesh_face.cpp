#include <spdlog/spdlog.h>
#include "ray.h"
#include "material.h"
#include "triangle.h"
#include "face_geouv.h"
#include "bvh_trimesh_face.h"
#include "aabb.h"
#include "trimesh.h"

namespace RT {
    namespace core {

        using namespace std;

        BVHTriMeshFace::BVHTriMeshFace(const std::string& name) :
            Surface{ name }
        {
            name_ = name.size() ? name : "BVHTriMeshFace";
        }
        BVHTriMeshFace::BVHTriMeshFace(Surface::Ptr mesh_ptr, const TriMesh::FaceHandle fh,
            const std::string& name) :
            mesh_ptr_{ mesh_ptr }, fh_{ fh }
        {
        }
        bool BVHTriMeshFace::Hit(const Ray& ray, Real tmin, Real tmax,
            HitRecord& hit_record)
        {
            if (bbox_.Hit(ray, tmin, tmax)) {
                if (dynamic_pointer_cast<TriMesh>(mesh_ptr_)->RayFaceHit(fh_, ray, tmin, tmax, hit_record))
                {
                    //tmax = hit_record.GetRayT(); 
                    return true;
                }
            }
            return false;
        }
        AABB
            BVHTriMeshFace::GetBoundingBox(bool force_recompute)
        {
            // if bound is clean, just return existing bbox_
            if (!force_recompute && !IsBoundDirty())
                return bbox_;

            // compute triangle bbox
            bbox_.Reset();
            for (auto fcv = dynamic_pointer_cast<TriMesh>(mesh_ptr_)->fv_iter(fh_); fcv.is_valid(); ++fcv)
            {
                bbox_.ExpandBy(dynamic_pointer_cast<TriMesh>(mesh_ptr_)->point(*fcv));
            }
            bound_dirty_ = false;
            return bbox_;
        }

    }  // namespace core
}  // namespace RT
