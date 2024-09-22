#pragma once

#include "wcge3dVertex.hpp"
#include "wcge3dShaderBindingGeneric.hpp"

namespace wcge {
	namespace c3d {
		template <typename TextureType>
		using PFNVERTEXSHADER = void(*) (Vertex& out, const Vertex& in, const ShaderBindingGeneric<TextureType>& binding);

		template <typename PixelType, typename TextureType>
		using PFNPIXELSHADER = PixelType(*) (const Vertex& in, const ShaderBindingGeneric<TextureType>& binding);

		template <typename TextureType>
		struct VertexShaderGeneric {
			inline VertexShaderGeneric(PFNVERTEXSHADER<TextureType> pFnMain) : main(pFnMain), binding({ 0 }) {}
			PFNVERTEXSHADER<TextureType> main;
			ShaderBindingGeneric<TextureType> binding;
		};

		template <typename PixelType, typename TextureType>
		struct PixelShaderGeneric {
			inline PixelShaderGeneric(PFNPIXELSHADER<PixelType, TextureType> pFnMain) : main(pFnMain), binding({ 0 }) {}
			PFNPIXELSHADER<PixelType, TextureType> main;
			ShaderBindingGeneric<TextureType> binding;
		};
	}
}