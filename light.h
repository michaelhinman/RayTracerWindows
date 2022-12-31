#pragma once

#include <memory>
#include <string>
#include "types.h"
#include "node.h"
#include <random>

namespace RT {
    namespace core {

        class Ray;
        class HitRecord;
        class Surface;

        class Light : public Node {
        public:
            RT_NODE(Light)

                explicit Light(const std::string& name = std::string());

            // Illuminate a hit point by retrieving its Phong material
            // and evaluating it with a call to `Material::Eval()`
            // param[in] hit_record Hit record for the point
            // param[in] view_vec View vector (points away from the surface)
            // param[in] scene Pointer to the whole scene that's being rendered
            // return Total radiance leaving the point in the direction of
            //         view_vec
            virtual Vec3r Illuminate(const HitRecord& hit_record, const Vec3r& view_vec,
                std::shared_ptr<Surface> scene) const;
        protected:
        };

        class AmbientLight : public Light {
        public:
            RT_NODE(AmbientLight)

                explicit AmbientLight(const std::string& name = std::string());

            AmbientLight(const Vec3r& ambient, const std::string& name = std::string());

            // Illuminate a hit point by retrieving its Phong material
            // and evaluating it with a call to `Material::Eval()`
            // param[in] hit_record Hit record for the point
            // param[in] view_vec View vector (points away from the surface)
            // param[in] scene Pointer to the whole scene that's being rendered
            // return Total radiance leaving the point in the direction of
            // view_vec
            Vec3r Illuminate(const HitRecord& hit_record, const Vec3r& view_vec,
                std::shared_ptr<Surface> scene) const override;

            void SetAmbient(const Vec3r& ambient) { ambient_ = ambient; }

            Vec3r GetAmbient() const { return ambient_; }
        protected:
            Vec3r ambient_{ 0, 0, 0 };  //!< ambient intensity
        };

        class PointLight : public Light {
        public:
            RT_NODE(PointLight)
                
                explicit PointLight(const std::string& name = std::string());

            PointLight(const Vec3r& position, const Vec3r& intensity,
                const std::string& name = std::string());

            // Illuminate a hit point by retrieving its Phong material
            // and evaluating it with a call to `Material::Eval()`
            // param[in] hit_record Hit record for the point
            // param[in] view_vec View vector (points away from the surface)
            // param[in] scene Pointer to the whole scene that's being rendered
            // return Total radiance leaving the point in the direction of
            //        view_vec
            Vec3r Illuminate(const HitRecord& hit_record, const Vec3r& view_vec,
                std::shared_ptr<Surface> scene) const override;

            void SetPosition(const Vec3r& position) { position_ = position; }

            void SetIntensity(const Vec3r& intensity) { intensity_ = intensity; }

            Vec3r GetPosition() const { return position_; }

            Vec3r GetIntensity() const { return intensity_; }
        protected:
            Vec3r position_{ 0, 0, 0 };   //!< light position
            Vec3r intensity_{ 0, 0, 0 };  //!< light intensity
        };

        class AreaLight : public Light {
        public:
            RT_NODE(AreaLight)

                explicit AreaLight(const std::string& name = std::string());

            AreaLight(const Vec3r& center, const Vec3r& direction,
                const Vec3r& u, const Vec3r& rgb, Real len,
                const std::string& name = std::string());

            Vec3r Illuminate(const HitRecord& hit_record, const Vec3r& view_vec,
                std::shared_ptr<Surface> scene) const override;

            Vec3r ComputeV();

            void SetStratIncrements();

            inline void SetCenter(const Vec3r& center) { center_ = center; }

            inline void SetDirection(const Vec3r& direction) { direction_ = direction; }

            inline void SetU(const Vec3r& u) { u_ = u; }

            inline void SetRGB(const Vec3r& rgb) { rgb_ = rgb; }

            inline void SetLen(Real len) { len_ = len; }

            void SetStratSamples(int samples) {
                strat_samples_ = static_cast<int>(
                    std::sqrt(static_cast<Real>(samples)));
                SetStratIncrements();
            }

            void SetSamples(int samples) { samples_ = samples; SetStratSamples(samples); }

            inline Vec3r GetCenter() { return center_; }

            inline Vec3r GetDirection() { return direction_; }

            inline Vec3r GetU() { return u_; }

            inline Vec3r GetRGB() { return rgb_; }

            inline Real GetLen() { return len_; }

        protected:
            Vec3r center_{ 0,0,0 };
            Vec3r direction_{ 0,0,0 };
            Vec3r u_{ 0,0,0 };
            Vec3r v_{ 0,0,0 };
            Vec3r rgb_{ 0,0,0 };
            std::vector<Vec3r> strat_increments_;
            Real len_{ 0 };
            int samples_ = 1;
            int strat_samples_ = 1;
        };

    }  // namespace core
}  // namespace RT
