#include <wcgePixel.hpp>

wcge::Pixel::Pixel( uint32_t n ) : n( n ) {}

wcge::Pixel::Pixel( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) 
	: r( r ), g( g ), b( b ), a( a ) {}