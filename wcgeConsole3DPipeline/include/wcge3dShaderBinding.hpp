#pragma once

#include <wcgeCBImage.hpp>
#include "wcge3dConstantBuffer.hpp"

namespace wcge {
	namespace c3d {
		struct ShaderBinding {
			ConstantBuffer* pCbuffers[8];
			CBImage* pTextures[8];
		};
	}
}