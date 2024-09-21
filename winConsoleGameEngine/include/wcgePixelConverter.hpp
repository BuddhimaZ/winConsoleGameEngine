#pragma once

#include <wcgePixel.hpp>
#include "wcgeCB.hpp"
#include <wcgeImage.hpp>

#include <unordered_map>
#include <vector>

namespace wcge {
	namespace utility {
		struct PixelKeyHasher {
			std::size_t operator()(const Pixel& p) const {
				return static_cast<size_t>(p.n);
			}
		};

		struct ShadedCharacterKeyHasher {
			std::size_t operator()(const SHADED_CHARACTERS& sc) const {
				return static_cast<size_t>(sc);
			}
		};

		float Distance(const Pixel& p1, const Pixel& p2);

		class PixelConverter {
		private:
			PixelConverter();

		public:
			static PixelConverter& GetIntstance();
			CB ConvertPixelToCB(const Pixel p);
			Pixel GetClosestPixel(const Pixel& p);
			void QuantizeImage(Image& img);
			inline const std::vector<Pixel>& GetPixelList() const { return m_pixelList; };
			inline const std::unordered_map<Pixel, CB, PixelKeyHasher>& GetPixelToCBMap() const { return m_pixelToCBMap; };

		private:
			static PixelConverter* m_pInstance;

			std::unordered_map<Pixel, CB, PixelKeyHasher> m_pixelToCBMap = {};
			std::vector<Pixel> m_pixelList = {
			};
		};
	}
}