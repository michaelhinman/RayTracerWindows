#pragma once

#include <string>
#include "types.h"
#include "ray.h"
#include "node.h"

namespace RT {
    namespace core {

        class Camera : public Node {
        public:
            RT_NODE(Camera)

            explicit Camera(const std::string& name = std::string());

            Camera(const Vec3r& eye, const Vec3r& target, const Vec3r& up,
                Real fovy, Real aspect, const std::string& name = std::string());

            void LookAt(const Vec3r& eye, const Vec3r& target, const Vec3r& up,
                bool update_viewport = true);

            inline Ray GetRay(Real s, Real t) const {
                return Ray{ cop_,
                           lower_left_corner_ + s * horizontal_ + t * vertical_ - cop_ };
            }

            void SetFovy(Real fovy, bool update_viewport = true);

            void SetAspect(Real aspect, bool update_viewport = true);

            Vec3r GetEye() const { return eye_; }

            Vec3r GetTarget() const { return target_; }

            Vec3r GetUpVector() const { return up_; }

            Mat4r GetCameraXform() const { return camera_xform_; }

            Real GetFovy() const { return fovy_; }

            Real GetAspectRatio() const { return aspect_; }

            void UpdateViewport();
        protected:
            Vec3r eye_{ 0, 0, 0 };                      // eye location
            Vec3r target_{ 0, 0, 0 };                   // target location
            Vec3r up_{ 0, 1, 0 };                       // up vector

            Mat4r camera_xform_{ Mat4r::Identity() };
            Real fovy_{ 60 };                           // vertical field of view in degrees
            Real aspect_{ 1.77778f };                   // viewport aspect ratio

            // viewport related params
            Vec3r cop_{ 0, 0, 0 };                      // center of projection (eye position)

            // 3D coordinates of lower left point in viewport
            Vec3r lower_left_corner_{ -1.0264f, -0.5774f, -1.0f };

            // viewport horizontal axis (with length equal to viewport width)
            Vec3r horizontal_{ 2.0528f, 0.0f, 0.0f };

            // viewport vertical axis (with length equal to viewport height)
            Vec3r vertical_{ 0.0f, 1.1547f, 0.0f };
        private:
        };

    }  // namespace core
}  // namespace RT
