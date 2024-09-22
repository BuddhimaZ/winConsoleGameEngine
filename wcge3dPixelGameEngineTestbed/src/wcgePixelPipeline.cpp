#include "wcgePixelPipeline.hpp"

wcge::c3d::PixelPipeline::PixelPipeline(olc::PixelGameEngine* pEngine)
	:
	PipelineGeneric<Pixel, Image>(
		PipelineCoreDescriptor{
			static_cast<uint32_t>(pEngine->ScreenWidth()),
			static_cast<uint32_t>(pEngine->ScreenHeight()),
			reinterpret_cast<void*>(pEngine)
		}
	) {}

wcge::c3d::PixelPipeline::~PixelPipeline() {}

void wcge::c3d::PixelPipeline::DrawPixel(const uint32_t x, const uint32_t y, Pixel pixel) {
	olc::PixelGameEngine* pEngine = reinterpret_cast<olc::PixelGameEngine*>(GetDescriptor().pUserData);
	pEngine->Draw(x, y, olc::Pixel(pixel.r, pixel.g, pixel.b, pixel.a));
}
