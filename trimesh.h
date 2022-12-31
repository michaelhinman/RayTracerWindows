#pragma once
#define _USE_MATH_DEFINES
#include <memory>
#include <string>
#include <boost/filesystem.hpp>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Geometry/EigenVectorT.hh>
#include <eigen-3.4.0/Eigen/Core>
#include <eigen-3.4.0/Eigen/Dense>
#include <eigen-3.4.0/Eigen/Geometry>
#include "surface.h"
#include "bvh_node.h"

namespace RT {
    namespace core {
        class BVHNode;

        // use Eigen instead of OpenMesh's default structures for Point and
        // Normal
        struct EigenMeshTraits : OpenMesh::DefaultTraits {
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
                using Point = Vec3r;
            using Normal = Vec3r;
            using TexCoord2D = Vec2r;
        };
        using OMTriMesh = OpenMesh::TriMesh_ArrayKernelT<EigenMeshTraits>;

        class TriMesh : public OMTriMesh, public Surface {
        public:
            RT_NODE(TriMesh)

                explicit TriMesh(const std::string& name = std::string());

            bool Hit(const Ray& ray, Real tmin, Real tmax,
                HitRecord& hit_record) override;

            bool RayFaceHit(TriMesh::FaceHandle fh, const Ray& ray, Real tmin,
                Real tmax, HitRecord& hit_record);

            bool Load(const boost::filesystem::path& filepath);

            bool Save(const boost::filesystem::path& filepath,
                OpenMesh::IO::Options opts = OpenMesh::IO::Options::Default);

            AABB GetBoundingBox(bool force_recompute = false) override;

            bool ComputeFaceNormals();

            bool ComputeVertexNormals();

            void SetFilePath(const boost::filesystem::path& filepath)
            {
                filepath_ = filepath;
            }

            boost::filesystem::path GetFilePath() const { return filepath_; }

            Vec3r VertexNormal(TriMesh::VertexHandle fh, bool normalize = true);

            void BuildBVH();
        protected:
            boost::filesystem::path filepath_;
            BVHNode::Ptr bvh_{ nullptr };
        };


    }  // namespace core
}  // namespace RT
