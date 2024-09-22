#include <wcge3dPipeline.hpp>

void wcge::c3d::Pipeline::DrawPixel(const uint32_t x, const uint32_t y, CB pixel) {
	reinterpret_cast<ConsoleGameEngine*>(GetDescriptor().pUserData)->GetBackBuffer().SetCB(x, y, pixel);
}
