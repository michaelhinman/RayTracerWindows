#include "phong_material.h"
#include <spdlog/spdlog.h>
#include "light.h"

namespace RT {
    namespace core {

        using namespace std;

        PhongMaterial::PhongMaterial(const std::string& name) :
            Material{}
        {
            name_ = name.size() ? name : "PhongMaterial";
            SetDiffuse(Vec3r{ 0, 0, 0 });
        }


        PhongMaterial::PhongMaterial(const Vec3r& ambient, const Vec3r& diffuse,
            const Vec3r& specular, Real shininess,
            const Vec3r& mirror, const std::string& name) :
            Material{},
            ambient_{ ambient },
            specular_{ specular },
            shininess_{ shininess },
            mirror_{ mirror }
        {
            name_ = name.size() ? name : "PhongMaterial";
            SetDiffuse(diffuse);
        }


        void
            PhongMaterial::SetDiffuse(const Vec3r& diffuse)
        {
            diffuse_ = SolidTexture::Create(diffuse);
        }


        Vec3r
            PhongMaterial::Evaluate(const HitRecord& hit_record, const Vec3r& light_vec,
                const Vec3r& view_vec) const
        {
            // Blinn-Phong halfway vector formulation
            const Vec3r& normal = hit_record.GetNormal();
            Vec3r half = (view_vec + light_vec).normalized();
            auto half_dot = fmax(0, half.dot(normal));
            Real specular_falloff = pow(half_dot, shininess_);
            Vec3r specular_color = specular_falloff * specular_;

            // handle backfacing surfaces
            Vec3r diffuse_color{ 0,0,0 };
            if (diffuse_)
            {
                const auto& uv = hit_record.GetFaceGeoUV().GetGlobalUV();
                diffuse_color = diffuse_->Value(uv, hit_record.GetPoint());
                if (!hit_record.IsFrontFace()) {
                    diffuse_color = Vec3r{ 1, 1, 0 };
                    specular_color = Vec3r{ 0, 0, 0 };
                }
            }
            return specular_color + diffuse_color;
        }
        void
            PhongMaterial::SetDiffuse(Texture::Ptr diffuse)
        {
            diffuse_ = diffuse;
        }
    }  // namespace core
}  // namespace RT