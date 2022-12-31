#pragma once

#include <memory>
#include <string>

namespace RT {
	namespace core {
		namespace utils {

			void InstallSegfaultHandler();
			std::string Backtrace(int skip);

		}  // namespace utils
	}  // namespace core
}  // namespace RT