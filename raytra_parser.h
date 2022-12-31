#pragma once

#include <vector>
#include "node.h"
#include "surface.h"
#include "camera.h"
#include "light.h"

namespace RT {
    namespace core {

        class RaytraParser {
        public:
            static bool ParseFile(const std::string& filename, Surface::Ptr& scene,
                std::vector<Light::Ptr>& lights, Camera::Ptr& camera,
                Vec2i& image_size, int shadow_samples);
        };

    }  // namespace core
}  // namespace RT