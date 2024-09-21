#pragma once

#include "wcgeMatrix.hpp"

namespace wcge {
	namespace Math {
		template <typename T> using Mat3x3_generic = Matrix_generic<T, 3, 3>;

		/// <summary>
		/// Mat3x3_generic templated on float
		/// </summary>
		typedef Mat3x3_generic<float> Mat3x3;

		/// <summary>
		/// Mat3x3_generic templated on double
		/// </summary>
		typedef Mat3x3_generic<double> Mat3x3d;

		/// <summary>
		/// Mat3x3_generic templated on int
		/// </summary>
		typedef Mat3x3_generic<int> Mat3x3i;

	}
}