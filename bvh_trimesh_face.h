#pragma once

#include <memory>
#include <string>

#include "surface.h"
#include "trimesh.h"

namespace RT {
    namespace core {

        // class For Holding Individual Trimesh Faces
        class BVHTriMeshFace : public Surface {
        public:
            RT_NODE(BVHTriMeshFace)

                explicit BVHTriMeshFace(const std::string& name = std::string());

            BVHTriMeshFace(Surface::Ptr mesh_ptr, const TriMesh::FaceHandle fh,
                const std::string& name = std::string());

            bool Hit(const Ray& ray, Real tmin, Real tmax,
                HitRecord& hit_record) override;

            inline void SetMeshPtr(Surface::Ptr mesh_ptr) { mesh_ptr_ = mesh_ptr; }

            inline void SetFaceHandle(TriMesh::FaceHandle fh) { fh_ = fh; }

            inline Surface::Ptr GetMeshPtr() { return mesh_ptr_; }

            AABB GetBoundingBox(bool force_recompute = false) override;
        protected:
            Surface::Ptr mesh_ptr_;
            TriMesh::FaceHandle fh_;
        private:
        };

    }  // namespace core
}  // namespace RT
