#pragma once
#include <memory>
#include <string>
#include "types.h"
#include "node.h"

namespace RT {
    namespace core {
        class Texture : public Node {
        public:
            RT_NODE(Texture)

                // Default constructor and assignment oprators
                explicit Texture(const std::string& name = std::string());

            virtual void SetGain(const Vec3r& gain) { gain_ = gain; }
            virtual void SetBias(const Vec3r& bias) { bias_ = bias; }
            virtual Vec3r GetGain() const { return gain_; }
            virtual Vec3r GetBias() const { return bias_; }
            virtual Vec3r Value(const Vec2r&/*uv*/, const Vec3r&/*position*/) {
                return Vec3r{ 0, 0, 0 } + bias_;
            }
        protected:
            Vec3r gain_{ 1, 1, 1 };
            Vec3r bias_{ 0, 0, 0 };
        };


        // class SolidTexture
        // SolidTexture class
        class SolidTexture : public Texture {
        public:
            RT_NODE(SolidTexture)

                // Default constructor and assignment oprators
                explicit SolidTexture(const Vec3r& color, const std::string& name = std::string());

            void SetColor(const Vec3r& color) { color_ = color; }
            Vec3r GetColor() const { return color_; }
            Vec3r Value(const Vec2r&/*uv*/, const Vec3r&/*position*/) override {
                return color_.cwiseProduct(gain_) + bias_;
            }
        protected:
            Vec3r color_{ 0, 0, 0 };
        };

    }  // namespace core
}  // namespace RT
