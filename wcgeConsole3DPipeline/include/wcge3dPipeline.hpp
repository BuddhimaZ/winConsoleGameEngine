#pragma once

#include <wcgeConsoleGameEngine.hpp>
#include <wcge3dPipelineGeneric.hpp>
#include <wcge3dVertexArray.hpp>

#include <vector>
#include <unordered_map>

namespace wcge {
	namespace c3d {
		class Pipeline : public PipelineGeneric<CB, CBImage> {
		public:
			Pipeline(ConsoleGameEngine* pEngine) 
				: 
				PipelineGeneric<CB, CBImage>(
					PipelineCoreDescriptor{ 
						pEngine->GetBackBuffer().GetWidth(), 
						pEngine->GetBackBuffer().GetHeight(),
						reinterpret_cast<void*>(pEngine)
					}
				) {}

			~Pipeline() {}

		private:
			void DrawPixel(const uint32_t x, const uint32_t y, CB pixel) override;
		};
	}
}