#include "ray.h"
#include <spdlog/spdlog.h>

namespace RT {
    namespace core {

        using namespace std;

        Ray::Ray(const Vec3r& origin, const Vec3r& dir) :
            origin_{ origin },
            dir_{ dir }
        {
        }


        HitRecord::HitRecord(const Ray& ray, Real ray_t, const Vec3r& point,
            const Vec3r& face_normal) :
            ray_t_{ ray_t },
            point_{ point }
        {
            SetNormal(ray, face_normal);
        }

    }  // namespace core
}  // namespace RT