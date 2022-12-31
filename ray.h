#pragma once
#include <memory>
#include <string>
#include "types.h"
#include "face_geouv.h"

namespace RT {
    namespace core {

        class Surface;

        class Ray {
        public:
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW

            Ray(const Vec3r& origin, const Vec3r& dir);

            // Default constructor
            Ray() = default;

            // Set ray origin
            inline void SetOrigin(const Vec3r& origin) { origin_ = origin; }

            // Set ray directionn
            inline void SetDirection(const Vec3r& dir) { dir_ = dir; }

            // Get ray origin
            inline Vec3r GetOrigin() const { return origin_; }

            // Get ray direction
            inline Vec3r GetDirection() const { return dir_; }

            // Evaluate ray at fractional distance t
            inline Vec3r At(Real t) const { return origin_ + t * dir_; }

            // Reflect incoming ray
            static Ray Reflect(const Ray& ray_in, const Vec3r& point, const Vec3r& normal);

            // Refract incoming ray
            static Ray Refract(const Ray& ray_in, const Vec3r& point, const Vec3r& normal,
                Real ior_in, Real ior_out);

            // Refract incoming ray
            static Ray Refract(const Ray& ray_in, const Vec3r& point, const Vec3r& normal,
                Real ior_ratio);
        protected:
            Vec3r origin_{ 0, 0, 0 };  //!< Ray origin
            Vec3r dir_{ 0, 0, 0 };     //!< Ray direction
        };

        class HitRecord {
        public:
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW

            HitRecord(const Ray& ray, Real ray_t, const Vec3r& point,
                    const Vec3r& face_normal);

            // Default constructor
            HitRecord() = default;

            // Set ray's t (fractional distance) at hit time
            inline void SetRayT(Real ray_t) { ray_t_ = ray_t; }

            // Set hit point position
            inline void SetPoint(const Vec3r& point) { point_ = point; }

            // Set surface normal at hit position
            inline void SetNormal(const Ray& ray, const Vec3r& face_normal) {
                front_face_ = ray.GetDirection().dot(face_normal) < 0;
                if (front_face_)
                    normal_ = face_normal;
                else
                    normal_ = -face_normal;
            }

            // Set surface normal at hit position.
            inline void SetNormal(const Vec3r& face_normal, bool front_face) {
                normal_ = face_normal;
                front_face_ = front_face;
            }

            // Set surface that was hit
            inline void SetSurface(std::shared_ptr<Surface> surface) { surface_ = surface; }

            // Get ray's fractional distance
            inline Real GetRayT() const { return ray_t_; }

            // Get hit position
            inline Vec3r GetPoint() const { return point_; }

            // Get surface normal at hit point
            inline Vec3r GetNormal() const { return normal_; }

            // Return whether the hit point was front or back facing
            inline bool IsFrontFace() const { return front_face_; }

            // Get hit surface
            inline std::shared_ptr<Surface> GetSurface() const { return surface_; }

            inline FaceGeoUV GetFaceGeoUV() const { return face_geouv_; }

            inline void SetFaceGeoUV(const FaceGeoUV& face_geouv) { face_geouv_ = face_geouv; }
        protected:
            Real ray_t_{ 0 }; //!< fractional distance along ray (t) that intersects surface
            Vec3r point_{ 0, 0, 0 };   //!< hit point
            Vec3r normal_{ 0, 0, 0 };  //!< surface normal at hit point
            bool front_face_{ true };  //!< whether hit point was front or back facing
            std::shared_ptr<Surface> surface_;  //!< pointer to the hit surface
            FaceGeoUV face_geouv_;  //!< track UV coordinates of intersected point
        };


        inline Ray
            Ray::Reflect(const Ray& ray_in, const Vec3r& point, const Vec3r& normal)
        {
            const Vec3r& dir = ray_in.GetDirection();
            return Ray{ point, dir - 2 * normal.dot(dir) * normal };
        }


        inline Ray
            Ray::Refract(const Ray& ray_in, const Vec3r& point, const Vec3r& normal,
                Real ior_in, Real ior_out)
        {
            const Vec3r& v = ray_in.GetDirection().normalized();
            auto cos_theta = fmin(normal.dot(-v), 1.0f);
            Vec3r r_out_perp = (ior_in / ior_out) * (v + cos_theta * normal);
            Vec3r r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.squaredNorm())) * normal;
            return Ray{ point, r_out_perp + r_out_parallel };
        }


        inline Ray
            Ray::Refract(const Ray& ray_in, const Vec3r& point, const Vec3r& normal,
                Real ior_ratio)
        {
            const Vec3r& v = ray_in.GetDirection().normalized();
            auto cos_theta = fmin(normal.dot(-v), 1.0f);
            Vec3r r_out_perp = ior_ratio * (v + cos_theta * normal);
            Vec3r r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.squaredNorm())) * normal;
            return Ray{ point, r_out_perp + r_out_parallel };
        }

    }  // namespace core
}  // namespace RT
