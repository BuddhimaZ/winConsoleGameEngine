#pragma once

#include "wcgeMatrix.hpp"

namespace wcge {
	namespace Math {
		template <typename T> using Mat2x2_generic = Matrix_generic<T, 2, 2>;

		/// <summary>
		/// Mat2x2_generic templated on float
		/// </summary>
		typedef Mat2x2_generic<float> Mat2x2;

		/// <summary>
		/// Mat2x2_generic templated on double
		/// </summary>
		typedef Mat2x2_generic<double> Mat2x2d;

		/// <summary>
		/// Mat2x2_generic templated on int
		/// </summary>
		typedef Mat2x2_generic<int> Mat2x2i;
	}
}