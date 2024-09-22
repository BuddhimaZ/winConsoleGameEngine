#pragma once

#include <cstdint>

namespace wcge {
	namespace c3d {
		struct PipelineCoreDescriptor {
			uint32_t nBackBufferWidth;
			uint32_t nBackBufferHeight;
			void* pUserData;
		};
	}
}