#pragma once

#include "wcgeWindowsHeader.hpp"

#include <cstdint>


namespace wcge {

	enum COLOUR : WORD {
		COL_FG_BLACK = 0x0000,				// 12, 12, 12
		COL_FG_DARK_BLUE = 0x0001,			// 0, 55, 218
		COL_FG_DARK_GREEN = 0x0002,			// 19, 161, 14
		COL_FG_DARK_CYAN = 0x0003,			// 58, 150, 221	
		COL_FG_DARK_RED = 0x0004,			// 197, 15, 31	
		COL_FG_DARK_MAGENTA = 0x0005,		// 136, 23, 152	
		COL_FG_DARK_YELLOW = 0x0006,		// 193, 156, 0
		COL_FG_GREY = 0x0007,				// 204, 204, 204
		COL_FG_DARK_GREY = 0x0008,			// 118, 118, 118
		COL_FG_BLUE = 0x0009,				// 59, 120, 255
		COL_FG_GREEN = 0x000A,				// 22, 198, 12
		COL_FG_CYAN = 0x000B,				// 97, 214, 214
		COL_FG_RED = 0x000C,				// 231, 72, 86
		COL_FG_MAGENTA = 0x000D,			// 180, 0, 158
		COL_FG_YELLOW = 0x000E,				// 249, 241, 165
		COL_FG_WHITE = 0x000F,				// 242, 242, 242

		COL_BG_BLACK = 0x0000,				// 12, 12, 12
		COL_BG_DARK_BLUE = 0x0010,			// 0, 55, 218
		COL_BG_DARK_GREEN = 0x0020,			// 19, 161, 14
		COL_BG_DARK_CYAN = 0x0030,			// 58, 150, 221	
		COL_BG_DARK_RED = 0x0040,			// 197, 15, 31	
		COL_BG_DARK_MAGENTA = 0x0050,		// 136, 23, 152	
		COL_BG_DARK_YELLOW = 0x0060,		// 193, 156, 0
		COL_BG_GREY = 0x0070,				// 204, 204, 204
		COL_BG_DARK_GREY = 0x0080,			// 118, 118, 118
		COL_BG_BLUE = 0x0090,				// 59, 120, 255
		COL_BG_GREEN = 0x00A0,				// 22, 198, 12
		COL_BG_CYAN = 0x00B0,				// 97, 214, 214
		COL_BG_RED = 0x00C0,				// 231, 72, 86
		COL_BG_MAGENTA = 0x00D0,			// 180, 0, 158
		COL_BG_YELLOW = 0x00E0,				// 249, 241, 165
		COL_BG_WHITE = 0x00F0,				// 242, 242, 242
	};

	enum SHADED_CHARACTERS : WCHAR {
		SC_SOLID = 0x2588,					// 1.0
		SC_THREEQUARTERS = 0x2593,			// 0.75
		SC_HALF = 0x2592,					// 0.5
		SC_QUARTER = 0x2591,
		SC_BLANK = 0x0020,
	};

	struct CB {
		CB( const WORD color = COLOUR::COL_BG_BLACK | COLOUR::COL_FG_WHITE, const WCHAR character = SHADED_CHARACTERS::SC_BLANK );

		WCHAR character;
		WORD color;
	};
}