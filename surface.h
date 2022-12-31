#pragma once
#include <memory>
#include <string>

#include "node.h"
#include "aabb.h"

namespace RT {
    namespace core {

        class Ray;
        class HitRecord;
        class Material;

        class Surface : public Node {
        public:
            RT_NODE(Surface)

                explicit Surface(const std::string& name = std::string());

            virtual bool Hit(const Ray& ray, Real tmin, Real tmax,
                HitRecord& hit_record);

            virtual void SetMaterial(std::shared_ptr<Material> material);

            virtual std::shared_ptr<Material> GetMaterial();

            virtual void SetBoundingBox(const AABB& bbox) { bbox_ = bbox; }

            virtual void SetBoundDirty(bool dirty) { bound_dirty_ = dirty; }

            virtual AABB GetBoundingBox(bool force_recompute = false);

            virtual bool IsBoundDirty() const { return bound_dirty_; }
        protected:
            std::shared_ptr<Material> material_; // node material
            AABB bbox_;  // surface's axis-aligned bounding box
            bool bound_dirty_{ true };    // whether bounds need to be recomputed
        private:
        };

    }  // namespace core
}  // namespace RT
