#pragma once

#include "wcge3dConstantBuffer.hpp"

namespace wcge {
	namespace c3d {
		template <typename TextureType>
		struct ShaderBindingGeneric {
			ConstantBuffer* pCbuffers[8];
			TextureType* pTextures[8];
		};
	}
}