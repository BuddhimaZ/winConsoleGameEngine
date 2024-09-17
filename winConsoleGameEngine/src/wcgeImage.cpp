#include <wcgeImage.hpp>
#include <wcgeWindowsHeader.hpp>

namespace Gdiplus {
	using std::min;
	using std::max;
}

#include <gdiplus.h>



class __GDIPlusManager {
public:
	__GDIPlusManager() {
		if ( m_nRefCount++ == 0 ) {
			Gdiplus::GdiplusStartupInput input;
			input.GdiplusVersion = 1;
			input.DebugEventCallback = nullptr;
			input.SuppressBackgroundThread = false;
			Gdiplus::GdiplusStartup( &m_pToken, &input, nullptr );
		}
	}

	~__GDIPlusManager() {
		if ( --m_nRefCount == 0 ) {
			Gdiplus::GdiplusShutdown( m_pToken );
		}
	}

private:
	static uint32_t m_nRefCount;
	static uint32_t m_nConstructorCallCount;
	static ULONG_PTR m_pToken;
};

uint32_t __GDIPlusManager::m_nRefCount = 0;
uint32_t __GDIPlusManager::m_nConstructorCallCount = 0;
ULONG_PTR __GDIPlusManager::m_pToken = 0;



wcge::Image::Image( const std::wstring& filename ) {
	__GDIPlusManager manager;

	Gdiplus::Bitmap bitmap( filename.c_str() );
	m_nWidth = bitmap.GetWidth();
	m_nHeight = bitmap.GetHeight();

	m_pBuffer = new Pixel[(uint64_t)m_nWidth * (uint64_t)m_nHeight];

	Gdiplus::Color pixel;
	if ( Gdiplus::IsAlphaPixelFormat( bitmap.GetPixelFormat() ) == TRUE ) {
		for ( uint32_t y = 0; y < m_nHeight; y++ ) {
			for ( uint32_t x = 0; x < m_nWidth; x++ ) {
				bitmap.GetPixel( x, y, &pixel );
				m_pBuffer[(uint64_t)x + (uint64_t)y * (uint64_t)m_nWidth] = Pixel( (uint8_t)pixel.GetR(), (uint8_t)pixel.GetG(), (uint8_t)pixel.GetB(), (uint8_t)pixel.GetA() );
			}
		}
	}
	else {
		for ( uint32_t y = 0; y < m_nHeight; y++ ) {
			for ( uint32_t x = 0; x < m_nWidth; x++ ) {
				bitmap.GetPixel( x, y, &pixel );
#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable : 6386 )
#endif // _MSC_VER
				m_pBuffer[(uint64_t)x + (uint64_t)y * (uint64_t)m_nWidth] = Pixel( (uint8_t)pixel.GetR(), (uint8_t)pixel.GetG(), (uint8_t)pixel.GetB() );
#if defined( _MSC_VER )
#pragma warning( pop )
#endif // _MSC_VER
			}
		}
	}
}

wcge::Image::Image(const Image& img) : Image(img.m_nWidth, img.m_nHeight) {
	for (uint32_t y = 0; y < m_nHeight; y++) {
		for (uint32_t x = 0; x < m_nWidth; x++) {
			SetPixel(x, y, img.GetPixel(x, y));
		}
	}
}

wcge::Image::Image( const uint32_t w, const uint32_t h ) : m_nWidth(w), m_nHeight(h) {
	m_pBuffer = new Pixel[(uint64_t)m_nWidth * (uint64_t)m_nHeight];
}

wcge::Image::~Image() {
	delete[] m_pBuffer;
	m_nWidth = 0;
	m_nHeight = 0;
}

void wcge::Image::SetPixel( uint32_t x, uint32_t y, const wcge::Pixel p ) {
	if ( x < m_nWidth && y < m_nHeight ) m_pBuffer[(uint64_t)x + (uint64_t)y * (uint64_t)m_nWidth] = p;
}

wcge::Pixel wcge::Image::GetPixel( uint32_t x, uint32_t y ) const {
	if ( x < m_nWidth && y < m_nHeight ) return m_pBuffer[(uint64_t)x + (uint64_t)y * (uint64_t)m_nWidth];
	return Pixel();
}