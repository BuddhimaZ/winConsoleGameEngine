#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

#include <wcgeCBImage.hpp>

#include <algorithm>

class Dithering : public olc::PixelGameEngine {
public:
	Dithering() {
		sAppName = "Dithering";
	}

private:
	bool OnUserCreate() override {
		tv.Initialise({ ScreenWidth(), ScreenHeight() });

		pImg = new wcge::CBImage( wcge::Image(L"res/texture.jpg") );
		pActive = new olc::Sprite(pImg->GetWidth(), pImg->GetHeight());


		const wcge::Image& ditheredImg = pImg->GetDitheredImage();

		for (int y = 0; y < pActive->height; y++) {
			for (int x = 0; x < pActive->width; x++) {
				wcge::Pixel p = ditheredImg.GetPixel(x, y);
				pActive->SetPixel(x, y, olc::Pixel(p.r, p.g, p.b));
			}
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {


		tv.HandlePanAndZoom();

		Clear(olc::BLACK);

		tv.DrawSprite({ 0.0f, 0.0f }, pActive);

		return true;
	}

	bool OnUserDestroy() override {
		delete pActive;
		delete pImg;
		return true;
	}

	olc::Sprite* pActive = nullptr;
	wcge::CBImage* pImg = nullptr;

	olc::TransformedView tv;
};

int main() {
	Dithering demo;
	if (demo.Construct(1280, 720, 1, 1) == olc::rcode::OK) demo.Start();
}