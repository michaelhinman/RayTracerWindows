#pragma once

#include "types.h"

namespace RT {
    namespace core {

        class FaceGeoUV {
        public:
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW

            FaceGeoUV() = default;
            FaceGeoUV(int face_id, const Vec2r& uv, const Vec2r& global_uv);

            inline int GetFaceID() const { return face_id_; }

            inline Vec2r GetUV() const { return uv_; }

            inline Vec2r GetGlobalUV() const { return global_uv_; }

            inline void SetFaceID(int face_id) { face_id_ = face_id; }

            inline void SetUV(const Vec2r& uv) { uv_ = uv; }

            inline void SetGlobalUV(const Vec2r& global_uv) { global_uv_ = global_uv; }
            // ...

        protected:
            int face_id_;       // triangle id in mesh (-1 if not a triangle or TriMesj)
            Vec2r uv_;          // barycentric coordinates (Beta, gamma) of the point inside triangle, (-1,-1 if not tri)
            Vec2r global_uv_;   // uv coordinates of point, if surface has uv coordinates; (-1,-1) otherwise
        };

    }  //namespace core
} //namespace RT