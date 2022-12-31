#pragma once
#include <memory>
#include <string>

#include "surface.h"

namespace RT {
    namespace core {
        class Triangle : public Surface {
        public:
            RT_NODE(Triangle)

                explicit Triangle(const std::string& name = std::string());
            Triangle(const std::vector<Vec3r>& points,
                const std::string& name = std::string());

            // Static function to compute ray-triangle intersection
            static bool RayTriangleHit(const Vec3r& p0, const Vec3r& p1, const Vec3r& p2,
                const Ray& ray, Real tmin, Real tmax,
                Real& ray_t, Vec2r& uv);

            bool Hit(const Ray& ray, Real tmin, Real tmax,
                HitRecord& hit_record) override;

            bool SetPoints(const std::vector<Vec3r>& points);

            bool GetPoints(std::vector<Vec3r>& points) const;

            Vec3r GetNormal() const { return normal_; }

            AABB GetBoundingBox(bool force_recompute = false) override;
        protected:
            bool ComputeNormal();

            std::vector<Vec3r> points_;  //!< triangle points
            Vec3r normal_{ 0, 0, 0 };      //!< triangle normal
        private:
        };

    }  // namespace core
}  // namespace RT
