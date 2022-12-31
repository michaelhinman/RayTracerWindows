#include "trimesh.h"
#include <spdlog/spdlog.h>
#include "ray.h"
#include "material.h"
#include "triangle.h"
#include "face_geouv.h"
#include "bvh_trimesh_face.h"

namespace RT {
    namespace core {

        using namespace std;
        namespace fs = boost::filesystem;

        TriMesh::TriMesh(const std::string& name) :
            OMTriMesh{},
            Surface{ name }
        {
            name_ = name.size() ? name : "TriMesh";
        }

        bool TriMesh::Hit(const Ray& ray, Real tmin, Real tmax,
            HitRecord& hit_record)
        {
            bool had_hit = false;
            if (bvh_ != nullptr)
            {
                if (bvh_->Hit(ray, tmin, tmax, hit_record))
                {
                    return true;
                }
            }
            else
            {
                for (auto fit = faces_begin(); fit != faces_end(); ++fit)
                {
                    if (RayFaceHit(*fit, ray, tmin, tmax, hit_record))
                    {
                        tmax = hit_record.GetRayT();  //!< update tmax
                        had_hit = true;
                    }
                }
            }
            return had_hit;
        }

        bool TriMesh::RayFaceHit(TriMesh::FaceHandle fh, const Ray& ray, Real tmin,
            Real tmax, HitRecord& hit_record)
        {

            auto heh = halfedge_handle(fh);
            auto vh0 = from_vertex_handle(heh);
            auto vh1 = to_vertex_handle(heh);
            auto vh2 = to_vertex_handle(next_halfedge_handle(heh));
            Vec3r p0 = point(vh0);
            Vec3r p1 = point(vh1);
            Vec3r p2 = point(vh2);

            Vec2r uvfh;
            Real ray_t;
            if (!Triangle::RayTriangleHit(p0, p1, p2,
                ray, tmin, tmax, ray_t, uvfh)) return false;
            else
            {
                Real alpha = 1.0 - uvfh[0] - uvfh[1];
                Vec3r lerp_n{ alpha * normal(vh0) + uvfh[0] * normal(vh1) + uvfh[1] * normal(vh2) };
                const Vec3r& hit_point = ray.At(ray_t);
                hit_record.SetRayT(ray_t);
                hit_record.SetPoint(hit_point);
                hit_record.SetNormal(ray, lerp_n);
                hit_record.SetSurface(GetPtr());
                FaceGeoUV fguv;
                fguv.SetFaceID(fh.idx());
                fguv.SetUV(uvfh);
                if (!has_vertex_texcoords2D())
                {
                    //std::cout<<"No v textcoords!"<<std::endl;
                    fguv.SetGlobalUV(Vec2r(-1, -1));
                }
                else
                {
                    Vec2r one{ alpha * texcoord2D(vh0) };
                    Vec2r two{ uvfh[0] * texcoord2D(vh1) };
                    Vec2r three{ uvfh[1] * texcoord2D(vh2) };
                    Vec2r out{ one + two + three };
                    fguv.SetGlobalUV(out);
                    hit_record.SetFaceGeoUV(fguv);
                }

            }
            return true;
        }

        bool TriMesh::ComputeFaceNormals()
        {
            request_face_normals();
            if (!has_face_normals()) return false;
            for (auto fit = faces_begin(); fit != faces_end(); ++fit)
            {
                auto heh = halfedge_handle(*fit);
                auto vh0 = from_vertex_handle(heh);
                auto vh1 = to_vertex_handle(heh);
                auto vh2 = to_vertex_handle(next_halfedge_handle(heh));
                Vec3r p0 = point(vh0);
                Vec3r p1 = point(vh1);
                Vec3r p2 = point(vh2);
                Vec3r normal = (p1 - p2).cross(p2 - p0);
                normal.normalize();
                set_normal(*fit, normal);
            }
            return true;
        }

        bool TriMesh::ComputeVertexNormals()
        {
            request_vertex_normals();
            if (!has_vertex_normals()) return false;
            for (auto vit = vertices_begin(); vit != vertices_end(); ++vit)
            {
                Vec3r vertex_normal = VertexNormal(*vit);
                set_normal(*vit, vertex_normal);
            }
            return true;
        }

        Vec3r TriMesh::VertexNormal(TriMesh::VertexHandle vh, bool normalize)
        {
            Vec3r face_normals{ 0,0,0 };
            VertexFaceIter vfit, vfBegin;
            vfBegin = vf_iter(vh);
            for (vfit = vfBegin; vfit.is_valid(); ++vfit)
            {
                face_normals += normal(*vfit);
            }
            if (normalize) face_normals.normalize();
            return face_normals;
        }

        bool TriMesh::Load(const boost::filesystem::path& filepath)
        {

            request_face_normals();
            request_vertex_normals();
            request_vertex_texcoords2D();
            OpenMesh::IO::Options opts{ OpenMesh::IO::Options::FaceNormal |
                                       OpenMesh::IO::Options::VertexNormal |
                                      OpenMesh::IO::Options::VertexTexCoord };
            std::string filename = filepath.string();
            if (!OpenMesh::IO::read_mesh(*this, filename, opts))
            {
                spdlog::error("could not load mesh from { }", filepath.string());
                return false;
            }
            if (!opts.check(OpenMesh::IO::Options::FaceNormal))
                ComputeFaceNormals();
            if (!opts.check(OpenMesh::IO::Options::VertexNormal))
                ComputeVertexNormals();
            // delete vertex texcoord2d attribute if file did not have them
            if (opts.check(OpenMesh::IO::Options::VertexTexCoord))
                spdlog::info("mesh has texture coordinates");
            else
            {
                std::cout << "Mesh does not have texture coordinates" << std::endl;
                release_vertex_texcoords2D();
            }
            return true;
        }

        bool TriMesh::Save(const boost::filesystem::path& filepath,
            OpenMesh::IO::Options opts)
        {
            const std::string filename = filepath.string();
            if (!OpenMesh::IO::write_mesh(*this, filename, opts))
            {
                spdlog::error("could not write mesh to {}", filepath);
                return false;
            }
            return true;
        }

        AABB
            TriMesh::GetBoundingBox(bool force_recompute)
        {
            // if bound is clean, just return existing bbox_
            if (!force_recompute && !IsBoundDirty())
                return bbox_;

            // compute triangle bbox
            bbox_.Reset();
            for (auto vit = vertices_begin(); vit != vertices_end(); ++vit)
            {
                bbox_.ExpandBy(point(*vit));
            }
            bound_dirty_ = false;
            return bbox_;
        }

        void TriMesh::BuildBVH()
        {
            auto bvh_root = BVHNode::Create();
            std::vector<Surface::Ptr> bvh_faces(n_faces());
            int i = 0;
            for (auto fit = faces_begin(); fit != faces_end(); ++fit)
            {
                auto fptr = BVHTriMeshFace::Create(GetPtr(), *fit);
                bvh_faces[i] = fptr;
                ++i;
            }
            bvh_ = bvh_root->BuildBVH(bvh_faces);
        }

    }  // namespace core
}  // namespace RT
