#include <wcgeMath.hpp>

namespace wcge {
	namespace Math {
		Mat4x4 Mat4MakeProjectionMatrix(
			const float& AspectRatio,
			const float& FOV,
			const float& ZNear,
			const float& ZFar
		) {
			const float fFOVTan = 1.0f / tanf( fDegToRadian( FOV / 2.0f ) );
			const float fQ = (ZFar) / (ZFar - ZNear);

			return {
				fFOVTan,                  0.0f,         0.0f, 0.0f,
				   0.0f, fFOVTan * AspectRatio,         0.0f, 0.0f,
				   0.0f,                  0.0f,           fQ, 1.0f,
				   0.0f,                  0.0f,  -fQ * ZNear, 0.0f
			};
		}

		Mat4x4 Mat4MakeProjectionMatrixOrtho(
			const unsigned int& ScreenWidth,
			const unsigned int& ScreenHeight,
			const float& ZNear,
			const float& ZFar
		) {

			return {
				2.0f / (float)ScreenWidth,                  0.0f,         0.0f, 0.0f,
				   0.0f, 2.0f / (float)ScreenHeight,         0.0f, 0.0f,
				   0.0f,                  0.0f,           1.0f / (ZFar - ZNear), 0.0f,
				   0.0f,                  0.0f,  ZNear / (ZNear - ZFar), 1.0f
			};
		}

		Mat4x4 Mat4MakeProjectionMatrix(
			const unsigned int& ScreenWidth,
			const unsigned int& ScreenHeight,
			const float& FOV,
			const float& ZNear,
			const float& ZFar
		) {
			return Mat4MakeProjectionMatrix(
				(float)ScreenWidth / (float)ScreenHeight,
				FOV,
				ZNear,
				ZFar
			);
		}

		Mat4x4 Mat4MakeRotationZ( const float& theta ) {
			const float fSinTheta = sinf( theta );
			const float fCosTheta = cosf( theta );

			return {
					fCosTheta, fSinTheta, 0.0f, 0.0f,
				   -fSinTheta, fCosTheta, 0.0f, 0.0f,
						 0.0f,      0.0f, 1.0f, 0.0f,
						 0.0f,      0.0f, 0.0f, 1.0f
			};

		}

		Mat4x4 Mat4MakeRotationY( const float& theta ) {
			const float fSinTheta = sinf( theta );
			const float fCosTheta = cosf( theta );

			return {
					fCosTheta, 0.0f, -fSinTheta, 0.0f,
						 0.0f, 1.0f,	   0.0f, 0.0f,
					fSinTheta, 0.0f,  fCosTheta, 0.0f,
						 0.0f, 0.0f,       0.0f, 1.0f
			};

		}

		Mat4x4 Mat4MakeRotationX( const float& theta ) {
			const float fSinTheta = sinf( theta );
			const float fCosTheta = cosf( theta );

			return {
					1.0f,		0.0f,		0.0f, 0.0f,
					0.0f,  fCosTheta,  fSinTheta, 0.0f,
					0.0f, -fSinTheta,  fCosTheta, 0.0f,
					0.0f,		0.0f,       0.0f, 1.0f
			};

		}

		Mat4x4 Mat4MakeTranslation( const Vector3& offset ) {
			return {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				 offset.x,  offset.y,  offset.z, 1.0f
			};
		}

		Mat4x4 Mat4MakeTranslation(
			const float& x,
			const float& y,
			const float& z
		) {
			return Mat4MakeTranslation( { x,y,z } );
		}

		Mat4x4 Mat4MakeRotationZXY( const Vector3& rotation ) {
			return
				Mat4MakeRotationZ( rotation.z ) *
				Mat4MakeRotationX( rotation.x ) *
				Mat4MakeRotationY( rotation.y )
				;
		}

		Mat4x4 Mat4MakeRotationZXY( const float& x, const float& y, const float& z ) {
			return Mat4MakeRotationZXY( { x, y, z } );
		}

		Mat4x4 Mat4MakeRotationXYZ( const Vector3& rotation ) {
			return
				Mat4MakeRotationX( rotation.x ) *
				Mat4MakeRotationY( rotation.y ) *
				Mat4MakeRotationZ( rotation.z )
				;
		}

		Mat4x4 Mat4MakeRotationXYZ(
			const float& x,
			const float& y,
			const float& z
		) {
			return Mat4MakeRotationXYZ( { x, y, z } );
		}

		Mat4x4 Mat4MakeRotationYXZ( const Vector3& rotation ) {
			return
				Mat4MakeRotationY( rotation.y ) *
				Mat4MakeRotationX( rotation.x ) *
				Mat4MakeRotationZ( rotation.z )
				;
		}

		Mat4x4 Mat4MakeRotationYXZ(
			const float& x,
			const float& y,
			const float& z
		) {
			return Mat4MakeRotationYXZ( { x, y, z } );
		}


		Mat4x4 Mat4RotateAroundAxis( const Vector3& axis, const float& theta ) {
			const float fSinTheta = sinf( theta );
			const float fCosTheta = cosf( theta );

			const float t = 1 - fCosTheta;

			return{
				t * powf( axis.x,2.0f ) + fCosTheta,		t * axis.x * axis.y - fSinTheta * axis.z,	t * axis.x * axis.z + fSinTheta * axis.y,	0.0f,
				t * axis.x * axis.y + fSinTheta * axis.z,	t * powf( axis.y,2 ) + fCosTheta,		t * axis.y * axis.z - fSinTheta * axis.x,	0.0f,
				t * axis.x * axis.z - fSinTheta * axis.y,	t * axis.y * axis.z + fSinTheta * axis.x,	t * powf( axis.z,2 ) + fCosTheta,		0.0f,
				0.0f,										0.0f,										0.0f,										1.0f

			};
		}

		Mat4x4 Mat4MakeScale( const Vector3& scale ) {
			return {
				scale.x, 0.0f, 0.0f, 0.0f,
				0.0f, scale.y, 0.0f, 0.0f,
				0.0f, 0.0f, scale.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		Mat3x3 Mat3MakeRotationZ( const float& theta ) {
			const float fSinTheta = sinf( theta );
			const float fCosTheta = cosf( theta );

			return {
					fCosTheta, fSinTheta, 0.0f,
				   -fSinTheta, fCosTheta, 0.0f,
						 0.0f,      0.0f, 1.0f

			};

		}

		Mat3x3 Mat3MakeRotationY( const float& theta ) {
			const float fSinTheta = sinf( theta );
			const float fCosTheta = cosf( theta );

			return {
					fCosTheta, 0.0f, -fSinTheta,
						 0.0f, 1.0f,	   0.0f,
					fSinTheta, 0.0f,  fCosTheta
			};

		}

		Mat3x3 Mat3MakeRotationX( const float& theta ) {
			const float fSinTheta = sinf( theta );
			const float fCosTheta = cosf( theta );

			return {
					1.0f,		0.0f,		0.0f,
					0.0f,  fCosTheta,  fSinTheta,
					0.0f, -fSinTheta,  fCosTheta
			};

		}

		Mat3x3 Mat3MakeRotationZXY( const Vector3& rotation ) {
			return Mat3MakeRotationZ( rotation.z ) * Mat3MakeRotationX( rotation.x ) * Mat3MakeRotationY( rotation.y );
		}

		Mat3x3 Mat3MakeRotationZXY(
			const float& x,
			const float& y,
			const float& z
		) {
			return Mat3MakeRotationZXY( { x, y, z } );
		}

		Mat3x3 Mat3MakeRotationXYZ( const Vector3& rotation ) {
			return
				Mat3MakeRotationX( rotation.x ) *
				Mat3MakeRotationY( rotation.y ) *
				Mat3MakeRotationZ( rotation.z )
				;
		}

		Mat3x3 Mat3MakeRotationXYZ(
			const float& x,
			const float& y,
			const float& z
		) {
			return Mat3MakeRotationXYZ( { x, y, z } );
		}

		Mat3x3 Mat3MakeRotationYXZ( const Vector3& rotation ) {
			return
				Mat3MakeRotationY( rotation.y ) *
				Mat3MakeRotationX( rotation.x ) *
				Mat3MakeRotationZ( rotation.z )
				;
		}

		Mat3x3 Mat3MakeRotationYXZ(
			const float& x,
			const float& y,
			const float& z
		) {
			return Mat3MakeRotationYXZ( { x, y, z } );
		}

		Mat3x3 Mat3MakeRotationAroundAxis(
			const float& theta,
			const Vector3& axis
		) {
			const float& fCosTheta = cosf( theta ); const float& fSinTheta = sinf( theta );


			return{
				fCosTheta + powf( axis.x, 2.0f ) * (1.0f - fCosTheta)	,		axis.x * axis.y * (1.0f - fCosTheta) - axis.z * fSinTheta	,		axis.x * axis.z * (1.0f - fCosTheta) + axis.y * fSinTheta,
				axis.y * axis.x * (1.0f - fCosTheta) + axis.z * fSinTheta	,		fCosTheta + powf( axis.y, 2.0f ) * (1.0f - fCosTheta)	,		axis.y * axis.z * (1.0f - fCosTheta) - axis.x * fSinTheta,
				axis.z * axis.x * (1.0f - fCosTheta) - axis.y * fSinTheta	,		axis.z * axis.y * (1.0f - fCosTheta) + axis.x * fSinTheta	,		fCosTheta + powf( axis.z, 2.0f ) * (1.0f - fCosTheta)
			};
		}

		Vector3 Vec3CrossProduct(
			const Vector3& a,
			const Vector3& b
		) {
			return Vector3(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
		}

		Vector4 Vec4CrossProduct(
			const Vector4& a,
			const Vector4& b
		) {
			return Vector4(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
		}

		float Vec2DotProduct(
			const Vector2& a,
			const Vector2& b
		) {
			return
				a.x * b.x + a.y * b.y;
			;
		}

		float Vec3DotProduct(
			const Vector3& a,
			const Vector3& b
		) {
			return
				a.x * b.x + a.y * b.y + a.z * b.z
				;
		}

		float Vec4DotProduct(
			const Vector4& a,
			const Vector4& b
		) {
			return
				a.x * b.x + a.y * b.y + a.z * b.z
				;
		}

	}
}