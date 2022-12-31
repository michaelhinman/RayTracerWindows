#pragma once
#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <set>
//#include <tbb.h>
#include <opencv2/opencv.hpp>
#include "tqdm.h"
#include "types.h"
#include "node.h"
#include "surface.h"
#include "camera.h"
#include "light.h"

namespace RT {
    namespace core {

        class RayTracer {
        public:
            // Default constructor
            RayTracer() = default;

            // The main render function responsible for generating a

            bool Render(Surface::Ptr scene, const std::vector<Light::Ptr>& lights,
                Camera::Ptr camera);

            // Set output image height. The image width will be
            inline void SetImageHeight(uint image_height) { image_height_ = image_height; }

            // Set maximum number of times a ray can bounce in the scene
            inline void SetMaxRayDepth(uint max_ray_depth) { max_ray_depth_ = max_ray_depth; }

            // Get output image height
            inline uint GetImageHeight() const { return image_height_; }

            // Get maximum number of times a ray can bounce in the scene
            inline uint GetMaxRayDepth() const { return max_ray_depth_; }

            // Write rendered image to file. If the image extension is
            bool WriteImage(const std::string& image_name, Real gamma = 1) const;

            // Set the number of rays per pixel for anti-aliasing
            inline void SetNumSamplesPerPixel(int per_pixel) { samples_per_pixel_ = per_pixel; }

            Real inline RandomReal()
            {
                return std::rand() / RAND_MAX;
            }
        protected:
            // Determine ray color by intersecting it with the scene
            bool RayColor(const Ray& ray, Surface::Ptr scene,
                const std::vector<Light::Ptr>& lights, uint ray_depth,
                uint max_ray_depth, Vec3r& ray_color);

            // Gamma correct input image
            cv::Mat GammaCorrectImage(const cv::Mat& in_image, Real gamma) const;

            // Convert an input image with double-precision floating
            cv::Mat RGBToBGRUChar(const cv::Mat& in_image) const;

            // Convert an input image with double-precision floating
            cv::Mat RGBToBGRFloat32(const cv::Mat& in_image) const;

            // Start the render progress bar
            void RenderProgressStart(size_t total_pixels);

            // Incremenet the number of rendered pixels in the progress bar
            void RenderProgressIncDonePixels();

            // Stop/end the render progress bar
            void RenderProgressEnd();

            uint image_height_{ 180 };  // output image height
            cv::Mat rendered_image_;  // output rendered image
            uint max_ray_depth_ = 5;  // max ray depth
            int samples_per_pixel_ = 1;   // samples per pixel (Anti-Aliasing)

            // progress bar related data members
            std::mutex progress_bar_mutex_;        // progress bar mutex
            std::shared_ptr<tqdm> progress_bar_;   // render progress bar
            size_t progress_bar_total_pixels_ = 0; // total number pixels to render
            size_t progress_bar_done_pixels_ = 0;  // number of pixels that have been rendered
        };

    }  // namespace core
}  // namespace RT
