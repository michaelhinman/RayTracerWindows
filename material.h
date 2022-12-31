#pragma once
#include <memory>
#include <string>
#include "types.h"
#include "node.h"

namespace RT {
	namespace core {

		class Ray;
		class HitRecord;

		class Material : public Node {
		public:
			RT_NODE(Material)
				explicit Material(const std::string& name = std::string());
		protected:
		};

	}  // namespace core
}  // namespace RT