#pragma once

#include "wcgeMat2x2.hpp"
#include "wcgeMat3x3.hpp"
#include "wcgeMat4x4.hpp"
#include "wcgeVector2.hpp"
#include "wcgeVector3.hpp"
#include "wcgeVector4.hpp"

namespace wcge {
	namespace Math {
		constexpr float fPi = 3.14159265358979323846264338f;
		constexpr double dPi = 3.1415926535897932384626433832795;

		/// <summary>
		/// Converts degrees to radians
		/// </summary>
		/// <param name="degrees"> : angle in degrees </param>
		/// <returns> angle in radians </returns>
		inline float fDegToRadian( const float& degrees ) { return degrees * fPi / 180.0f; }

		/// <summary>
		/// Converts degrees to radians (doubles version)
		/// </summary>
		/// <param name="degrees"> : angle in degrees (64 bit floating point) </param>
		/// <returns> angle in radians (64 bit floating point) </returns>
		inline double dDegToRadian( const double& degrees ) { return degrees * dPi / 180.0; }

		/// <summary>
		/// Returns the Mat4x4 that contains Perspective projection matrix for left hand coordinate system
		/// </summary>
		/// <param name="AspectRatio"> : Aspect ratio of the window / screen  (Aspect ratio = height / width) </param>
		/// <param name="FOV"> : Horizontal Field of view of the camera ( or viewport. Whatever you wanna call it )</param>
		/// <param name="ZNear"> : Distance to the near plane from the viewing point </param>
		/// <param name="ZFar"> : Distance to the far plane from the viewing point </param>
		Mat4x4 Mat4MakeProjectionMatrix(
			const float& AspectRatio,
			const float& FOV,
			const float& ZNear,
			const float& ZFar
		);

		/// <summary>
		/// Returns the Mat4x4 that contains Otthoganal projection matrix for left hand coordinate system
		/// </summary>
		/// <param name="ScreenWidth"> : Width of the screen (or window) in pixels </param>
		/// <param name="ScreenHeight"> : Height of the screen (or window) in pixels </param>
		/// <param name="ZNear"> : Distance to the near plane from the viewing point </param>
		/// <param name="ZFar"> : Distance to the far plane from the viewing point </param>
		Mat4x4 Mat4MakeProjectionMatrixOrtho(
			const unsigned int& ScreenWidth,
			const unsigned int& ScreenHeight,
			const float& ZNear,
			const float& ZFar
		);

		/// <summary>
		/// Returns the Mat4x4 that contains Perspective projection matrix for left hand coordinate system
		/// </summary>
		/// <param name="ScreenWidth"> : Width of the screen (or window) in pixels </param>
		/// <param name="ScreenHeight"> : Height of the screen (or window) in pixels </param>
		/// <param name="FOV"> : Horizontal Field of view of the camera ( or viewport. Whatever you wanna call it ) </param>
		/// <param name="ZNear"> : Distance to the near plane from the viewing point </param>
		/// <param name="ZFar"> : Distance to the far plane from the viewing point </param>
		Mat4x4 Mat4MakeProjectionMatrix(
			const unsigned int& ScreenWidth,
			const unsigned int& ScreenHeight,
			const float& FOV,
			const float& ZNear,
			const float& ZFar
		);

		//I think following functions are self documented functions.


		/// <param name="theta"> : Angle in radians </param>
		Mat4x4 Mat4MakeRotationZ( const float& theta );

		/// <param name="theta"> : Angle in radians </param>
		Mat4x4 Mat4MakeRotationY( const float& theta );

		/// <param name="theta"> : Angle in radians </param>
		Mat4x4 Mat4MakeRotationX( const float& theta );

		Mat4x4 Mat4MakeTranslation( const Vector3& offset );

		Mat4x4 Mat4MakeTranslation(
			const float& x,
			const float& y,
			const float& z
		);

		/// <param name="rotation"> : Rotation around x, y, and z axises (in radians) </param>
		Mat4x4 Mat4MakeRotationZXY( const Vector3& rotation );


		/// <param name="x"> : Rotation around x axis in radians </param>
		/// <param name="y"> : Rotation around y axis in radians </param>
		/// <param name="z"> : Rotation around z axis in radians </param>
		Mat4x4 Mat4MakeRotationZXY( const float& x, const float& y, const float& z );

		/// <param name="rotation"> : Rotation around x, y, and z axises (in radians) </param>
		Mat4x4 Mat4MakeRotationXYZ( const Vector3& rotation );

		/// <param name="x"> : Rotation around x axis in radians </param>
		/// <param name="y"> : Rotation around y axis in radians </param>
		/// <param name="z"> : Rotation around z axis in radians </param>
		Mat4x4 Mat4MakeRotationXYZ(
			const float& x,
			const float& y,
			const float& z
		);

		/// <param name="rotation"> : Rotation around x, y, and z axises (in radians) </param>
		Mat4x4 Mat4MakeRotationYXZ( const Vector3& rotation );

		/// <param name="x"> : Rotation around x axis in radians </param>
		/// <param name="y"> : Rotation around y axis in radians </param>
		/// <param name="z"> : Rotation around z axis in radians </param>
		Mat4x4 Mat4MakeRotationYXZ(
			const float& x,
			const float& y,
			const float& z
		);


		/// <param name="axis"> : A unit vector that represent the positive direction of the axis </param>
		/// <param name="theta"> : Angle (in radians) around the axis showed by axis parameter </param>
		Mat4x4 Mat4RotateAroundAxis( const Vector3& axis, const float& theta );

		Mat4x4 Mat4MakeScale( const Vector3& scale );

		/// <param name="theta"> : Angle in radians </param>
		Mat3x3 Mat3MakeRotationZ( const float& theta );

		/// <param name="theta"> : Angle in radians </param>
		Mat3x3 Mat3MakeRotationY( const float& theta );

		/// <param name="theta"> : Angle in radians </param>
		Mat3x3 Mat3MakeRotationX( const float& theta );

		/// <param name="rotation"> : Rotation around x, y, and z axises (in radians) </param>
		Mat3x3 Mat3MakeRotationZXY( const Vector3& rotation );

		/// <param name="x"> : Rotation around x axis in radians </param>
		/// <param name="y"> : Rotation around y axis in radians </param>
		/// <param name="z"> : Rotation around z axis in radians </param>
		Mat3x3 Mat3MakeRotationZXY(
			const float& x,
			const float& y,
			const float& z
		);

		/// <param name="rotation"> : Rotation around x, y, and z axises (in radians) </param>
		Mat3x3 Mat3MakeRotationXYZ( const Vector3& rotation );

		/// <param name="x"> : Rotation around x axis in radians </param>
		/// <param name="y"> : Rotation around y axis in radians </param>
		/// <param name="z"> : Rotation around z axis in radians </param>
		Mat3x3 Mat3MakeRotationXYZ(
			const float& x,
			const float& y,
			const float& z
		);

		/// <param name="rotation"> : Rotation around x, y, and z axises (in radians) </param>
		Mat3x3 Mat3MakeRotationYXZ( const Vector3& rotation );

		/// <param name="x"> : Rotation around x axis in radians </param>
		/// <param name="y"> : Rotation around y axis in radians </param>
		/// <param name="z"> : Rotation around z axis in radians </param>
		Mat3x3 Mat3MakeRotationYXZ(
			const float& x,
			const float& y,
			const float& z
		);

		/// <param name="axis"> : A unit vector that represent the positive direction of the axis </param>
		/// <param name="theta"> : Angle (in radians) around the axis showed by axis parameter </param>
		Mat3x3 Mat3MakeRotationAroundAxis(
			const float& theta,
			const Vector3& axis
		);

		Vector3 Vec3CrossProduct(
			const Vector3& a,
			const Vector3& b
		);

		Vector4 Vec4CrossProduct(
			const Vector4& a,
			const Vector4& b
		);

		float Vec2DotProduct(
			const Vector2& a,
			const Vector2& b
		);

		float Vec3DotProduct(
			const Vector3& a,
			const Vector3& b
		);

		float Vec4DotProduct(
			const Vector4& a,
			const Vector4& b
		);

		template<typename T>
		//A function for Inerpolating between src and dst based on alpha. This is a templated function so any object that support adding, substracting and multiplying with a constant operations will be able to interpolate.
		inline T Interpolate(
			const T& src,
			const T& dst,
			const float& alpha
		) {
			return src + (dst - src) * alpha;
		}
	}
}