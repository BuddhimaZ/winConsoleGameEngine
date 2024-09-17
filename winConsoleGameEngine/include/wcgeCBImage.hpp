#pragma once

#include "wcgeImage.hpp"
#include "wcgeCB.hpp"

#ifndef DITHERING_ENABLED
#define DITHERING_ENABLED 1
#endif // DITHERING_ENABLED

namespace wcge {
	class CBImage {
	public:
		CBImage( const Image& img );
		CBImage( const uint32_t w, const uint32_t h );
		~CBImage();

		void SetCB( const uint32_t x, const uint32_t y, const CB cb = CB( COLOUR::COL_BG_BLACK | COLOUR::COL_FG_WHITE , SHADED_CHARACTERS::SC_SOLID ) );
		CB GetCB( const uint32_t x, const uint32_t y ) const;
		void Clear( const CB cb );

		inline uint32_t GetWidth() const { return m_nWidth; }
		inline uint32_t GetHeight() const { return m_nHeight; }
		inline CB* const GetBufferPtr() const { return m_pBuffer; }
		inline size_t GetBufferSize() const { return sizeof(CB) * (size_t)m_nWidth * (size_t)m_nHeight; }
		inline const Image& GetDitheredImage() const { return *m_pImgDithered; }

	private:
		uint32_t m_nWidth = 0;
		uint32_t m_nHeight = 0;

		CB* m_pBuffer = nullptr;
		Image* m_pImgDithered = nullptr;
	};
}