#pragma once

#include <cstdint>

namespace wcge {
	struct Pixel {
		union {
			uint32_t n;
			struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; };
		};

		Pixel( uint32_t n );
		Pixel( uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0xFF );

		inline bool operator==(const Pixel& p) const { return n == p.n; }
	};
}