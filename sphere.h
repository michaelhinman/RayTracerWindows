#pragma once
#include <memory>
#include <string>

#include "surface.h"

namespace RT {
    namespace core {

        class Sphere : public Surface {
        public:
            RT_NODE(Sphere)

                explicit Sphere(const std::string& name = std::string());

            Sphere(const Vec3r& center, Real radius,
                const std::string& name = std::string());

            bool Hit(const Ray& ray, Real tmin, Real tmax,
                HitRecord& hit_record) override;

            void SetCenter(const Vec3r& center);

            void SetRadius(Real radius);

            Vec3r GetCenter() const { return center_; }

            Real GetRadius() const { return radius_; }

            AABB GetBoundingBox(bool force_recompute = false) override;
        protected:
            Vec3r center_{ 0, 0, 0 };  //!< sphere position
            Real radius_{ 0 };         //!< sphere radius
        private:
        };

    }  // namespace core
}  // namespace RT
