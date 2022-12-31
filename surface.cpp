#include "surface.h"
#include "ray.h"
#include "material.h"

namespace RT {
	namespace core {

		Surface::Surface(const std::string& name) :
			Node{ name }
		{
			name_ = name.size() ? name : "Surface";
		}


		void
			Surface::SetMaterial(std::shared_ptr<Material> material)
		{
			material_ = material;
		}


		Material::Ptr
			Surface::GetMaterial()
		{
			return material_;
		}


		bool
			Surface::Hit(const Ray&, Real, Real, HitRecord&)
		{
			return false;
		}


		AABB
			Surface::GetBoundingBox(bool /*force_recompute*/)
		{
			return bbox_;
		}

	}  // namespace core
}  // namespace RT
