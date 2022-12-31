#pragma once
#include <memory>
#include <string>
#include <mutex>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include "types.h"
#include "node.h"
#include "texture.h"

namespace RT {
    namespace core {

        class ImageTexture : public Texture {
        public:
            RT_NODE(ImageTexture)

                //! \brief Constructor.
                //! \param[in] image_path Texture image path
                //! \param[in[ flipx Flip horizontally (around the y-axis)
                //! \param[in[ flipy Flip vertically (around the x-axis)
                //! \param[in] name Node name
                ImageTexture(const boost::filesystem::path& image_path,
                    bool flipx = false, bool flipy = false,
                    const std::string& name = std::string());
            ImageTexture(ImageTexture&& other) = delete;
            ImageTexture& operator=(const ImageTexture&) = delete;
            ImageTexture& operator=(ImageTexture&&) = delete;

            // Set texture image path
            void SetImagePath(const boost::filesystem::path& image_path,
                bool flipx = false, bool flipy = false);
            cv::Mat GetImage() const { return image_; }
            boost::filesystem::path GetImagePath() const { return image_path_; }
            Vec3r Value(const Vec2r& uv, const Vec3r& position) override;

            // Flip image horizontally, vertically, or both
            static cv::Mat FlipImage(const cv::Mat& in_image, bool flipx, bool flipy);
        protected:
            ImageTexture(const ImageTexture& other);
            bool LoadImageIfNeeded();

            boost::filesystem::path image_path_;
            bool flipx_{ false };
            bool flipy_{ false };
            cv::Mat image_;
            std::mutex image_mutex_;
            std::atomic<bool> needs_reload_{ false };
        };

    }  // namespace core
}  // namespace RT
