#pragma once

#include "wcgeMat2x2.hpp"
#include "wcgeMat3x3.hpp"
#include "wcgeMat4x4.hpp"
#include "wcgeVector2.hpp"
#include "wcgeVector3.hpp"

namespace wcge {
	namespace Math {
		/// <summary>
		/// A 4 dimensional vector that holds 4 values (x, y, z and w) templated on T. Vector4_generic also support some basic vector arithmatic
		/// via operator overloading.
		/// </summary>
		/// <typeparam name="T">T can be any data type that support the addition, substraction, multiplication and division operators</typeparam>
		template<typename T>
		struct Vector4_generic {

			union {
				T element[4];
				struct {
					T x;
					T y;
					T z;
					T w;
				};
			};

			inline Vector4_generic()
				:
				x( 0 ),
				y( 0 ),
				z( 0 ),
				w( 1.0 ) {
			}

			inline Vector4_generic(
				T x,
				T y
			)
				:
				x( x ),
				y( y ),
				z( 0 ),
				w( 1.0 ) {
			}

			inline Vector4_generic(
				T x,
				T y,
				T z
			)
				:
				x( x ),
				y( y ),
				z( z ),
				w( 1.0 ) {
			}

			inline Vector4_generic(
				T x,
				T y,
				T z,
				T w
			)
				:
				x( x ),
				y( y ),
				z( z ),
				w( w ) {
			}

			inline Vector4_generic( const Vector2_generic<T>& v )
				:
				Vector4_generic( v.x, v.y, 0.0, 1.0 ) {
			}

			inline Vector4_generic( const Vector3_generic<T>& v )
				:
				Vector4_generic( v.x, v.y, v.z, 1.0 ) {
			}

			inline Vector4_generic( const Vector4_generic& v )
				:
				Vector4_generic( v.x, v.y, v.z, v.w ) {
			}

			inline T Magnitude() const {
				return (T)sqrt( x * x + y * y + z * z + w * w );
			}

			inline Vector4_generic& Normalize() {
				this->x /= this->Magnitude();
				this->y /= this->Magnitude();
				this->z /= this->Magnitude();
				this->w /= this->Magnitude();
				return *this;
			}

			inline Vector4_generic operator - () const {
				return Vector4_generic( -this->x, -this->y, -this->z, -this->w );
			}

			inline Vector4_generic operator + ( const Vector4_generic& rhs ) const {
				return Vector4_generic(
					this->x + rhs.x,
					this->y + rhs.y,
					this->z + rhs.z,
					this->w + rhs.w
				);
			}

			inline Vector4_generic operator - ( const Vector4_generic& rhs ) const {
				return Vector4_generic(
					this->x - rhs.x,
					this->y - rhs.y,
					this->z - rhs.z,
					this->w - rhs.w
				);
			}

			inline Vector4_generic operator * ( const T& rhs ) const {
				return Vector4_generic(
					this->x * rhs,
					this->y * rhs,
					this->z * rhs,
					this->w * rhs
				);
			}

			inline Vector4_generic operator / ( const T& rhs ) const {
				return Vector4_generic(
					this->x / rhs,
					this->y / rhs,
					this->z / rhs,
					this->w / rhs
				);
			}

			inline Vector4_generic& operator += ( const Vector4_generic& rhs ) {
				this->x += rhs.x;
				this->y += rhs.y;
				this->z += rhs.z;
				this->w += rhs.w;
				return *this;
			}

			inline Vector4_generic& operator -= ( const Vector4_generic& rhs ) {
				this->x -= rhs.x;
				this->y -= rhs.y;
				this->z -= rhs.z;
				this->w -= rhs.w;
				return *this;
			}

			inline Vector4_generic& operator *= ( const T& rhs ) {
				this->x *= rhs;
				this->y *= rhs;
				this->z *= rhs;
				this->w *= rhs;
				return *this;
			}

			inline Vector4_generic& operator /= ( const T& rhs ) {
				this->x /= rhs;
				this->y /= rhs;
				this->z /= rhs;
				this->w /= rhs;
				return *this;
			}

			inline bool operator == ( const Vector4_generic& rhs ) const {
				return
					(this->x == rhs.x) &&
					(this->y == rhs.y) &&
					(this->z == rhs.z) &&
					(this->w == rhs.w)
					;
			}

			inline bool operator != ( const Vector4_generic& rhs ) const {
				return
					(this->x != rhs.x) &&
					(this->y != rhs.y) &&
					(this->z != rhs.z) &&
					(this->w != rhs.w)
					;
			}

			inline T& operator [] ( size_t index ) {
				return element[index];
			}

			inline Vector4_generic operator * ( const Mat4x4_generic<T>& rhs ) const {
				Vector4_generic result( 0.0f, 0.0f, 0.0f, 0.0f );
				for ( int colomn = 0; colomn < 4; colomn++ )
					for ( int row = 0; row < 4; row++ )
						result.element[colomn] += (this->element[row] * rhs.elements[row][colomn]);
				return result;
			}

			inline Vector4_generic& operator *= ( const Mat4x4_generic<T>& rhs ) {
				Vector4_generic temp = *this;
				this->x = 0; this->y = 0; this->z = 0; this->w = 0;
				for ( int colomn = 0; colomn < 4; colomn++ )
					for ( int row = 0; row < 4; row++ )
						this->element[colomn] += temp.element[row] * rhs.element[row][colomn];
				return *this;
			}

		};

		/// <summary>
		/// Vector4_generic templated on float
		/// </summary>
		typedef Vector4_generic<float> Vector4;

		/// <summary>
		/// Vector4_generic templated on double
		/// </summary>
		typedef Vector4_generic<double> Vector4d;

		/// <summary>
		/// Vector4_generic templated on int
		/// </summary>
		typedef Vector4_generic<int> Vector4i;

		static const Vector4 VEC4_ZERO = Vector4( 0.0, 0.0, 0.0 );
		static const Vector4 VEC4_UP = Vector4( 0.0, 1.0, 0.0 );
		static const Vector4 VEC4_DOWN = Vector4( 0.0, -1.0, 0.0 );
		static const Vector4 VEC4_RIGHT = Vector4( 1.0, 0.0, 0.0 );
		static const Vector4 VEC4_LEFT = Vector4( -1.0, 0.0, 0.0 );
		static const Vector4 VEC4_FORWARD = Vector4( 0.0, 0.0, 1.0 );
		static const Vector4 VEC4_BACKWORD = Vector4( 0.0, 0.0, -1.0 );
	}
}