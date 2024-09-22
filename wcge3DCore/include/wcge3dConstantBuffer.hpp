#pragma once

#include <wcgeMath.hpp>

namespace wcge {
	namespace c3d {
		struct ConstantBuffer {
			inline ConstantBuffer() : mat4_00( Math::Mat4x4() ) {};
			inline ConstantBuffer( const ConstantBuffer& copy ) : mat4_00( copy.mat4_00 ) {};
			union {
				Math::Mat4x4 mat4_00;
				Math::Mat3x3 mat3_00;
				
				struct { Math::Mat2x2 mat2_00, mat2_01, mat2_02, mat2_03; };

				struct { Math::Vector4 v4_00, v4_01, v4_02, v4_03; };
				struct { Math::Vector3 v3_00, v3_01, v3_02, v3_03, v3_04; };
				struct { Math::Vector2 v2_00, v2_01, v2_02, v2_03, v2_04, v2_05, v2_06, v2_07; };
				struct { float f_00, f_01, f_02, f_03, f_04, f_05, f_06, f_07, f_08, f_09, f_10, f_11, f_12, f_13, f_14, f_15; };

				Math::Mat2x2 mat2_arr[4];
				Math::Vector4 v4_arr[4];
				Math::Vector3 v3_arr[5];
				Math::Vector2 v2_arr[8];
				float f_arr[16];
			};

			inline operator Math::Mat4x4& () { return this->mat4_00; }
			inline operator const Math::Mat4x4& const () { return this->mat4_00; }

			inline operator Math::Mat3x3& () { return this->mat3_00; }
			inline operator const Math::Mat3x3& const () { return this->mat3_00; }
		};
	}
}