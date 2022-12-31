#pragma once
#include <memory>
#include <string>
#include "types.h"
#include "node.h"
#include "material.h"
#include "ray.h"
#include "image_texture.h"


namespace RT {
    namespace core {

        class Light;
        class AmbientLight;
        class PointLight;

        class PhongMaterial : public Material {
        public:
            RT_NODE(PhongMaterial)
                explicit PhongMaterial(const std::string& name = std::string());

            PhongMaterial(const Vec3r& ambient, const Vec3r& diffuse, const Vec3r& specular,
                Real shininess, const Vec3r& mirror = Vec3r{ 0, 0, 0 },
                const std::string& name = std::string());

            Vec3r Evaluate(const HitRecord& hit_record, const Vec3r& light_vec,
                const Vec3r& view_vec) const;

            void SetAmbient(const Vec3r& ambient) { ambient_ = ambient; }

            void SetDiffuse(const Vec3r& diffuse);

            void SetSpecular(const Vec3r& specular) { specular_ = specular; }

            void SetShininess(Real shininess) { shininess_ = shininess; }

            void SetMirror(const Vec3r& mirror) { mirror_ = mirror; }

            Vec3r GetAmbient() const { return ambient_; }

            Vec3r GetSpecular() const { return specular_; }

            Real GetShininess() const { return shininess_; }

            Vec3r GetMirror() const { return mirror_; }

            void SetDiffuse(Texture::Ptr diffuse);
        protected:
            Vec3r ambient_{ 0, 0, 0 };      //!< ambient coefficients
            Texture::Ptr diffuse_;      //!< diffuse coefficients
            Vec3r specular_{ 0, 0, 0 };     //!< specular coefficients
            Real shininess_{ 1 };           //!< shininess coefficient
            Vec3r mirror_{ 0, 0, 0 };       //!< mirror coefficients
        };

    }  // namespace core
}  // namespace RT