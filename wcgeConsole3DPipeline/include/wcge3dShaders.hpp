#pragma once

#include <wcgeCB.hpp>
#include "wcge3dVertex.hpp"
#include "wcge3dShaderBinding.hpp"

namespace wcge {
	namespace c3d {
		typedef void (*PFNVERTEXSHADER) (Vertex& out, const Vertex& in, const ShaderBinding& binding);
		typedef CB (*PFNPIXELSHADER) (const Vertex& in, const ShaderBinding& binding);

		struct VertexShader {
			inline VertexShader( PFNVERTEXSHADER pFnMain ) : main( pFnMain ), binding({ 0 }) {}
			PFNVERTEXSHADER main;
			ShaderBinding binding;
		};

		struct PixelShader {
			inline PixelShader(PFNPIXELSHADER pFnMain) : main(pFnMain), binding({ 0 }) {}
			PFNPIXELSHADER main;
			ShaderBinding binding;
		};
	}
}