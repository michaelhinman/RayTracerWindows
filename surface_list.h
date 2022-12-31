#pragma once
#include <memory>
#include <string>
#include <set>
#include "surface.h"

namespace RT {
    namespace core {

        class Ray;
        class HitRecord;

        class SurfaceList : public Surface {
        public:
            RT_NODE(SurfaceList)

                SurfaceList(std::vector<Surface::Ptr> surfaces,
                    const std::string& name = std::string());

            bool Hit(const Ray& ray, Real tmin, Real tmax,
                HitRecord& hit_record) override;

            AABB GetBoundingBox(bool force_recompute = false) override;

            inline std::vector<Surface::Ptr> GetSurfaces() { return surfaces_; }
        protected:
            std::vector<Surface::Ptr> surfaces_;
        private:
        };

    }  // namespace core
}  // namespace RT
