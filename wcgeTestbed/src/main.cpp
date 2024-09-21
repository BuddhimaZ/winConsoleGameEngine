#include <wcgeConsoleGameEngine.hpp>


class TestBed : public wcge::ConsoleGameEngine {
public:
	using ConsoleGameEngine::ConsoleGameEngine;

	wcge::CBImage* m_pImage;

private:
	bool OnStart() override {
		SetApplicationName( L"TestBed" );

		m_pImage = new wcge::CBImage(wcge::Image(L"wcgeResources/texture.jpg"));

		//m_pImage->GetDitheredImage().Save(L"wcgeResources/texture_3_dithered.jpg");

		return true;
	}

	bool OnUpdate( const float dt ) override {
		
		//for ( uint32_t y = 0; y < GetBackBuffer().GetHeight(); y++ ) {
		//	for ( uint32_t x = 0; x < GetBackBuffer().GetWidth(); x++ ) {
		//		int n = rand() % 5;

		//		wcge::SHADED_CHARACTERS s;

		//		switch ( n ) {
		//		case 0: s = wcge::SC_BLANK; break;
		//		case 1: s = wcge::SC_QUARTER; break;
		//		case 2: s = wcge::SC_HALF; break;
		//		case 3: s = wcge::SC_THREEQUARTERS; break;
		//		case 4: s = wcge::SC_SOLID; break;
		//		default: s = wcge::SC_BLANK; break;
		//		}

		//		GetBackBuffer().SetCB( x, y, wcge::CB( wcge::COL_BG_BLACK | wcge::COL_FG_WHITE, s ) );
		//	}
		//}

		//DrawLine( 1, 1, 100, 1, wcge::CB( wcge::COL_BG_BLACK | wcge::COL_FG_WHITE, wcge::SC_SOLID ) );
		//DrawLine( 50, 50, 100, 1, wcge::CB( wcge::COL_BG_BLACK | wcge::COL_FG_WHITE, wcge::SC_SOLID ) );
		//DrawLine( 1, 1, 50, 50, wcge::CB( wcge::COL_BG_BLACK | wcge::COL_FG_WHITE, wcge::SC_SOLID ) );

		// DrawTriangle( 0, 0, 100, 0, 50, 50, wcge::CB( wcge::COL_BG_BLACK | wcge::COL_FG_WHITE, wcge::SC_SOLID ) );

		for (uint32_t y = 0; y < GetBackBuffer().GetHeight(); y++) {
			for (uint32_t x = 0; x < GetBackBuffer().GetWidth(); x++) {
				uint32_t xImage = static_cast<uint32_t>(static_cast<float>(x * m_pImage->GetWidth()) / static_cast<float>(GetBackBuffer().GetWidth()));
				uint32_t yImage = static_cast<uint32_t>(static_cast<float>(y * m_pImage->GetHeight()) / static_cast<float>(GetBackBuffer().GetHeight()));

				GetBackBuffer().SetCB(x, y, m_pImage->GetCB(xImage, yImage));
			}
		}


		return true;
	}

	bool OnDestroy() override {
		delete m_pImage;
		return true;
	}
};

int main() {
	TestBed().Run();
}