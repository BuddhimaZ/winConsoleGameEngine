#pragma once

#include "wcgePixel.hpp"

#include <string>
#include <cstdint>

namespace wcge {
	class Image {
	public:
		Image( const std::wstring& filename );
		Image(const Image& img);
		Image( const uint32_t w, const uint32_t h );
		~Image();

		void SetPixel( uint32_t x, uint32_t y, const Pixel p );

		Pixel GetPixel( uint32_t x, uint32_t y ) const;
		inline uint32_t GetWidth() const { return m_nWidth; }
		inline uint32_t GetHeight() const { return m_nHeight; }

		inline Pixel* const GetBufferPtr() const { return m_pBuffer; }
		inline size_t GetBufferSize() const { return sizeof( Pixel ) * (size_t)m_nWidth * (size_t)m_nHeight; }

		void Save(const std::wstring& filename) const;

	private:
		uint32_t m_nWidth = 0;
		uint32_t m_nHeight = 0;

		Pixel* m_pBuffer = nullptr;
	};
}