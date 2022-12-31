#pragma once

#include <limits>
//#include <spdlog/spdlog.h>
//#include <spdlog/fmt/bundled/ostream.h>
#include "types.h"

namespace RT {
    namespace core {

        class Ray;

        class AABB {
        public:
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW

            AABB() = default;
            AABB(const Vec3r& bmin, const Vec3r& bmax);

            // Get min coordinates
            inline Vec3r GetMin() const { return min_; }

            // Get max coordinates
            inline Vec3r GetMax() const { return max_; }

            // Reset bounding box to uninitialized coordinates
            void Reset();

            // Return whether the bbox is valid (ie min/max coordinates are valid)
            bool IsValid() const;

            // Expand bounding box by point
            void ExpandBy(const Vec3r& point);

            // Expand bounding box by another box
            void ExpandBy(const AABB& other);

            // Compute intersection of this box another one
            AABB IntersectWith(const AABB& other) const;

            // Check if point is inside bbox (inclusive)
            bool IsPointInside(const Vec3r& point) const;

            // Check if ray intersects with aabb
            bool Hit(const Ray& ray, Real tmin, Real tmax) const;

            // Use * operator to transform min and max coordinates by a matrix
            friend AABB operator*(const Mat4r& xform, const AABB& bbox);

        protected:
            Vec3r min_{ kInfinity, kInfinity, kInfinity };    //!< min coordinates
            Vec3r max_{ -kInfinity, -kInfinity, -kInfinity }; //!< max coordinates
        };

        // Use * operator to transform min and max coordinates by a matrix
        AABB operator*(const Mat4r& xform, const AABB& bbox);

    }  // namespace core
}  // namespace RT

