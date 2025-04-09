#pragma once

#include <functional>

namespace wcge {
	namespace js {
		void Init();
		void DeInit();

		void Execute(const std::function<void()>& job);
		void Dispatch(const size_t numGroups, const size_t groupSize, const std::function<void(size_t, size_t)>& job);
		
		void Wait();
	}
}