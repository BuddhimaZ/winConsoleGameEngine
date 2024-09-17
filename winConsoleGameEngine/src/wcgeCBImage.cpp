#include <wcgeCBImage.hpp>
#include <wcgeCB.hpp>
#include <wcgePixelConverter.hpp>

#include <algorithm>

wcge::CBImage::CBImage( const Image& img ) {
	m_nWidth = img.GetWidth();
	m_nHeight = img.GetHeight();

	m_pBuffer = new CB[(uint64_t)m_nWidth * (uint64_t)m_nHeight];

	Image ditheredImg(img);

	for (uint32_t y = 0; y < m_nHeight; y++) {
		for (uint32_t x = 0; x < m_nWidth; x++) {
			Pixel op = img.GetPixel(x, y);
			Pixel qp = utility::PixelConverter::GetIntstance().GetClosestPixel(op);

			int32_t error[3] = {
				op.r - qp.r,
				op.g - qp.g,
				op.b - qp.b
			};

			ditheredImg.SetPixel(x, y, qp);
#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable : 6386 )
#endif // _MSC_VER
			m_pBuffer[(uint64_t)x + (uint64_t)y * (uint64_t)m_nWidth] = utility::PixelConverter::GetIntstance().GetPixelToCBMap().at(qp);
#if defined( _MSC_VER )
#pragma warning( pop )
#endif // _MSC_VER

			auto UpdatePixel = [&x, &y, &ditheredImg, &error](const int32_t xOffset, const int32_t yOffset, const float fErrorBias) {
				Pixel p = ditheredImg.GetPixel(x + xOffset, y + yOffset);
				int32_t k[3] = { p.r, p.g, p.b };

				k[0] += (int32_t)((float)error[0] * fErrorBias);
				k[1] += (int32_t)((float)error[1] * fErrorBias);
				k[2] += (int32_t)((float)error[2] * fErrorBias);

				p.r = std::clamp(k[0], 0, 255);
				p.g = std::clamp(k[1], 0, 255);
				p.b = std::clamp(k[2], 0, 255);

				ditheredImg.SetPixel(x + xOffset, y + yOffset, p);
			};

			UpdatePixel( +1,  0, 7.0f / 16.0f);
			UpdatePixel( +1, +1, 1.0f / 16.0f);
			UpdatePixel(  0, +1, 5.0f / 16.0f);
			UpdatePixel( -1, +1, 3.0f / 16.0f);
		}
	}


//	for ( uint32_t y = 0; y < m_nHeight; y++ ) {
//		for ( uint32_t x = 0; x < m_nWidth; x++ ) {
//#if defined( _MSC_VER )
//#pragma warning( push )
//#pragma warning( disable : 6386 )
//#endif // _MSC_VER
//			m_pBuffer[(uint64_t)x + (uint64_t)y * (uint64_t)m_nWidth] = utility::PixelConverter::GetIntstance().ConvertPixelToCB( img.GetPixel( x, y ) );
//#if defined( _MSC_VER )
//#pragma warning( pop )
//#endif // _MSC_VER
//		}
//	}
}

wcge::CBImage::CBImage( const uint32_t w, const uint32_t h ) : m_nWidth(w), m_nHeight(h) {
	m_pBuffer = new CB[(uint64_t)m_nWidth * (uint64_t)m_nHeight];
}

wcge::CBImage::~CBImage() {
	m_nWidth = 0;
	m_nHeight = 0;
	delete[] m_pBuffer;
}

void wcge::CBImage::SetCB( const uint32_t x, const uint32_t y, const wcge::CB cb ) {
	if ( x < m_nWidth && y < m_nHeight ) m_pBuffer[(uint64_t)x + (uint64_t)y * (uint64_t)m_nWidth] = cb;
}

wcge::CB wcge::CBImage::GetCB( const uint32_t x, const uint32_t y ) const {
	if ( x < m_nWidth && y < m_nHeight ) return m_pBuffer[(uint64_t)x + (uint64_t)y * (uint64_t)m_nWidth];
	return CB();
}

void wcge::CBImage::Clear( const wcge::CB cb ) {
	for ( uint32_t y = 0; y < m_nHeight; y++ ) {
		for ( uint32_t x = 0; x < m_nWidth; x++ ) {
			m_pBuffer[(uint64_t)x + (uint64_t)y * (uint64_t)m_nWidth] = cb;
		}
	}
}