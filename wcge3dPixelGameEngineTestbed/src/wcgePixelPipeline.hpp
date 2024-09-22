#pragma once

#include "olcPixelGameEngine.h"
#include <wcge3dPipelineGeneric.hpp>
#include <wcge3dVertexArray.hpp>
#include <wcgeImage.hpp>
#include <wcgePixel.hpp>

#include <vector>
#include <unordered_map>

namespace wcge {
	namespace c3d {
		class PixelPipeline : public PipelineGeneric<Pixel, Image> {
		public:
			PixelPipeline(olc::PixelGameEngine* pEngine);

			~PixelPipeline();

		private:
			void DrawPixel(const uint32_t x, const uint32_t y, Pixel pixel) override;
		};
	}
}