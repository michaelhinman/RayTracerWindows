#include "material.h"
#include "ray.h"

namespace RT {
	namespace core {

		Material::Material(const std::string& name) :
			Node{}
		{
			name_ = name.size() ? name : "Material";
		}

	}  // namespace core
}  // namespace RT
