#include "light.h"
#include "surface.h"
#include "ray.h"
#include "phong_material.h"
#include <iostream>

namespace RT {
    namespace core {

        using namespace std;

        Light::Light(const std::string& name) :
            Node{ name }
        {
            name_ = name.size() ? name : "Light";
        }


        Vec3r
            Light::Illuminate(const HitRecord&/*hit_record*/, const Vec3r&/*view_vec*/,
                Surface::Ptr /*scene*/) const
        {
            return Vec3r{ 0, 0, 0 };
        }


        AmbientLight::AmbientLight(const std::string& name) :
            Light{ name }
        {
            name_ = name.size() ? name : "AmbientLight";
        }


        AmbientLight::AmbientLight(const Vec3r& ambient, const std::string& name) :
            Light{ name },
            ambient_{ ambient }
        {
            name_ = name.size() ? name : "AmbientLight";
        }


        Vec3r
            AmbientLight::Illuminate(const HitRecord& hit_record, const Vec3r&/*view_vec*/,
                Surface::Ptr /*scene*/) const
        {
            // only process phong materials
            auto surface = hit_record.GetSurface();
            if (!surface)
                return Vec3r{ 0, 0, 0 };
            auto phong_material = dynamic_pointer_cast<PhongMaterial>(surface->
                GetMaterial());
            if (!phong_material)
                return Vec3r{ 0, 0, 0 };
            return ambient_.cwiseProduct(phong_material->GetAmbient());
        }


        PointLight::PointLight(const std::string& name) :
            Light{ name }
        {
            name_ = name.size() ? name : "PointLight";
        }


        PointLight::PointLight(const Vec3r& position, const Vec3r& intensity,
            const std::string& name) :
            Light{ name },
            position_{ position },
            intensity_{ intensity }
        {
            name_ = name.size() ? name : "PointLight";
        }


        Vec3r
            PointLight::Illuminate(const HitRecord& hit_record, const Vec3r& view_vec,
                Surface::Ptr scene) const
        {
            // evaluate hit points material
            Vec3r black{ 0, 0, 0 };

            // create a shadow ray to the point light and check for occlusion
            const auto& hit_position = hit_record.GetPoint();
            Ray shadow_ray{ hit_position, GetPosition() - hit_position };
            HitRecord shadow_record;
            if (scene->Hit(shadow_ray, kEpsilon, 1, shadow_record)) {
                return black;
            }

            // only process phong materials
            auto surface = hit_record.GetSurface();
            if (!surface)
                return black;
            auto phong_material = dynamic_pointer_cast<PhongMaterial>(surface->
                GetMaterial());
            if (!phong_material)
                return black;

            // compute irradiance at hit point
            const Vec3r& normal = hit_record.GetNormal();
            Vec3r light_vec = position_ - hit_position;
            auto distance2 = light_vec.squaredNorm();
            light_vec.normalize();
            auto denominator = std::max(kEpsilon2, distance2);
            Vec3r irradiance = intensity_ * fmax(0.0f, normal.dot(light_vec)) / denominator;

            // compute how much the material absorts light
            const Vec3r& attenuation = phong_material->Evaluate(hit_record, light_vec,
                view_vec);
            return irradiance.cwiseProduct(attenuation);
        }
        //! \param[in] name Node name
        AreaLight::AreaLight(const std::string& name) :
            Light{ name }
        {
            name_ = name.size() ? name : "AreaLight";
        }

        AreaLight::AreaLight(const Vec3r& center, const Vec3r& direction,
            const Vec3r& u, const Vec3r& rgb, Real len,
            const std::string& name) :
            Light{ name },
            center_{ center },
            direction_{ direction },
            u_{ u },
            v_{ ComputeV() },
            rgb_{ rgb },
            len_{ len }
        {
            name_ = name.size() ? name : "AreaLight";
        }


        Vec3r AreaLight::Illuminate(const HitRecord& hit_record, const Vec3r& view_vec,
            std::shared_ptr<Surface> scene) const
        {
            Vec3r total_illumination{ 0,0,0, };

            for (auto i : strat_increments_)
            {
                Vec3r black{ 0, 0, 0 };
                // create a shadow ray to the point light and check for occlusion
                const auto& hit_position = hit_record.GetPoint();
                Real r = static_cast<Real>(std::rand());
                r /= static_cast<Real>(RAND_MAX);
                Real s = static_cast<Real>(std::rand());
                s /= static_cast<Real>(RAND_MAX);
                Real xoffset = r;
                Real yoffset = s;
                Vec3r sample_position = i + ((u_ * xoffset) / strat_samples_) +
                    ((v_ * yoffset) / strat_samples_);

                Ray shadow_ray{ hit_position, sample_position - hit_position };
                HitRecord shadow_record;
                if (scene->Hit(shadow_ray, kEpsilon, 1, shadow_record)) {
                    continue;
                }
                // only process phong materials
                auto surface = hit_record.GetSurface();
                if (!surface)
                    return black;
                auto phong_material = dynamic_pointer_cast<PhongMaterial>(surface->
                    GetMaterial());
                if (!phong_material)
                    return black;

                // compute irradiance at hit point
                const Vec3r& normal = hit_record.GetNormal();
                Vec3r light_vec = sample_position - hit_position;
                auto distance2 = light_vec.squaredNorm();
                light_vec.normalize();
                auto denominator = std::max(kEpsilon2, distance2);
                Vec3r intensity = rgb_ * (-light_vec.dot(direction_));
                Vec3r irradiance = intensity * fmax(0.0f, normal.dot(light_vec)) / denominator;
                irradiance *= (len_ * len_);
                // compute how much the material absorts light
                const Vec3r& attenuation = phong_material->Evaluate(hit_record, light_vec,
                    view_vec);
                total_illumination += (irradiance.cwiseProduct(attenuation));
            }
            return total_illumination / samples_;
        }

        Vec3r AreaLight::ComputeV()
        {
            return u_.cross(direction_);
        }

        void AreaLight::SetStratIncrements()
        {
            Vec3r upper_left = center_ - (0.5 * len_ * u_) - (0.5 * len_ * v_);
            for (int i = 0; i < strat_samples_; ++i)
            {
                for (int j = 0; j < strat_samples_; ++j)
                {
                    Vec3r current_corner = upper_left + (len_ * j * u_ / strat_samples_) +
                        (len_ * i * v_ / strat_samples_);
                    strat_increments_.push_back(current_corner);
                }
            }
        }

    }  // namespace core
}  // namespace RT


