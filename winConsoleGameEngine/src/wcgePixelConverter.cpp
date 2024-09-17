#include <wcgePixelConverter.hpp>

#include <wcgeCB.hpp>

namespace wcge {
	namespace utility {
		static std::unordered_map<Pixel, COLOUR, PixelKeyHasher> fgColorMap = {
			{ Pixel(12, 12, 12), COLOUR::COL_FG_BLACK },
			//{ Pixel(0, 55, 218), COLOUR::COL_FG_DARK_BLUE },
			{ Pixel(19, 161, 14), COLOUR::COL_FG_DARK_GREEN },
			//{ Pixel(58, 150, 221), COLOUR::COL_FG_DARK_CYAN },
			//{ Pixel(197, 15, 31), COLOUR::COL_FG_DARK_RED },
			//{ Pixel(136, 23, 152), COLOUR::COL_FG_DARK_MAGENTA },
			//{ Pixel(193, 156, 0), COLOUR::COL_FG_DARK_YELLOW },
			{ Pixel(204, 204, 204), COLOUR::COL_FG_GREY },
			{ Pixel(118, 118, 118), COLOUR::COL_FG_DARK_GREY },
			{ Pixel(59, 120, 255), COLOUR::COL_FG_BLUE },
			{ Pixel(22, 198, 12), COLOUR::COL_FG_GREEN },
			{ Pixel(97, 214, 214), COLOUR::COL_FG_CYAN },
			{ Pixel(231, 72, 86), COLOUR::COL_FG_RED },
			//{ Pixel(180, 0, 158), COLOUR::COL_FG_MAGENTA },
			{ Pixel(249, 241, 165), COLOUR::COL_FG_YELLOW },
			{ Pixel(242, 242, 242), COLOUR::COL_FG_WHITE }
		};

		static std::unordered_map<Pixel, COLOUR, PixelKeyHasher> bgColorMap = {
			{ Pixel(12, 12, 12), COLOUR::COL_BG_BLACK },
			//{ Pixel(0, 55, 218), COLOUR::COL_BG_DARK_BLUE },
			//{ Pixel(19, 161, 14), COLOUR::COL_BG_DARK_GREEN },
			//{ Pixel(58, 150, 221), COLOUR::COL_BG_DARK_CYAN },
			//{ Pixel(197, 15, 31), COLOUR::COL_BG_DARK_RED },
			//{ Pixel(136, 23, 152), COLOUR::COL_BG_DARK_MAGENTA },
			//{ Pixel(193, 156, 0), COLOUR::COL_BG_DARK_YELLOW },
			{ Pixel(204, 204, 204), COLOUR::COL_BG_GREY },
			{ Pixel(118, 118, 118), COLOUR::COL_BG_DARK_GREY },
			{ Pixel(59, 120, 255), COLOUR::COL_BG_BLUE },
			{ Pixel(22, 198, 12), COLOUR::COL_BG_GREEN },
			{ Pixel(97, 214, 214), COLOUR::COL_BG_CYAN },
			{ Pixel(231, 72, 86), COLOUR::COL_BG_RED },
			//{ Pixel(180, 0, 158), COLOUR::COL_BG_MAGENTA },
			{ Pixel(249, 241, 165), COLOUR::COL_BG_YELLOW },
			{ Pixel(242, 242, 242), COLOUR::COL_BG_WHITE }
		};

		static std::unordered_map<SHADED_CHARACTERS, float, ShadedCharacterKeyHasher> shadedCharacterMap = {
			{ SHADED_CHARACTERS::SC_SOLID, 1.0f },
			{ SHADED_CHARACTERS::SC_THREEQUARTERS, 0.75f },
			{ SHADED_CHARACTERS::SC_HALF, 0.5f },
		};
	}
}

float wcge::utility::Distance(const Pixel& p1, const Pixel& p2) {
	float r = static_cast<float>(p1.r) - static_cast<float>(p2.r);
	float g = static_cast<float>(p1.g) - static_cast<float>(p2.g);
	float b = static_cast<float>(p1.b) - static_cast<float>(p2.b);

	return sqrtf(r * r + g * g + b * b);
}

wcge::utility::PixelConverter::PixelConverter() {
	for (const auto& fg : fgColorMap) {
		for (const auto& bg : bgColorMap) {
			for (const auto& sc : shadedCharacterMap) {
				Pixel fgPxel = fg.first;
				Pixel bgPixel = bg.first;
				float scValue = sc.second;

				float finalR = static_cast<float>(fgPxel.r) * scValue + static_cast<float>(bgPixel.r) * (1.0f - scValue);
				float finalG = static_cast<float>(fgPxel.g) * scValue + static_cast<float>(bgPixel.g) * (1.0f - scValue);
				float finalB = static_cast<float>(fgPxel.b) * scValue + static_cast<float>(bgPixel.b) * (1.0f - scValue);

				finalR = std::min(255.0f, std::max(0.0f, finalR));
				finalG = std::min(255.0f, std::max(0.0f, finalG));
				finalB = std::min(255.0f, std::max(0.0f, finalB));

				Pixel finalPixel = Pixel(
					static_cast<uint8_t>(finalR),
					static_cast<uint8_t>(finalG),
					static_cast<uint8_t>(finalB)
				);

				COLOUR fgColor = fg.second;
				COLOUR bgColor = bg.second;

				CB cb = CB(
					static_cast<WORD>(bgColor) | static_cast<WORD>(fgColor),
					static_cast<WCHAR>(sc.first)
				);

				m_pixelToCBMap[finalPixel] = cb;
				m_pixelList.push_back(finalPixel);
			}
		}
	}
}

wcge::utility::PixelConverter& wcge::utility::PixelConverter::GetIntstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new PixelConverter();
	}

	return *m_pInstance;
}

wcge::CB wcge::utility::PixelConverter::ConvertPixelToCB(const Pixel p) {
	Pixel closestPixel = GetClosestPixel(p);
	return m_pixelToCBMap[closestPixel];
}

wcge::Pixel wcge::utility::PixelConverter::GetClosestPixel(const Pixel& p) {
	Pixel closestPixel = m_pixelList[0];
	float minDistance = Distance(p, closestPixel);

	for (const auto& pixel : m_pixelList) {
		float distance = Distance(p, pixel);
		if (distance < minDistance) {
			minDistance = distance;
			closestPixel = pixel;
		}
	}

	return closestPixel;
}

void wcge::utility::PixelConverter::QuantizeImage(Image& img) {
	for (size_t y = 0; y < img.GetHeight(); y++) {
		for (size_t x = 0; x < img.GetWidth(); x++) {
			Pixel p = img.GetPixel(x, y);
			Pixel closestPixel = GetClosestPixel(p);
			img.SetPixel(x, y, closestPixel);
		}
	}
}

wcge::utility::PixelConverter* wcge::utility::PixelConverter::m_pInstance = nullptr;
