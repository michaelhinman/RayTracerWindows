#include "face_geouv.h"

namespace RT {
	namespace core {

		using namespace std;

		FaceGeoUV::FaceGeoUV(int face_id, const Vec2r& uv, const Vec2r& global_uv) :
			face_id_{ face_id }, uv_{ uv }, global_uv_{ global_uv }
		{
		}

	}
}