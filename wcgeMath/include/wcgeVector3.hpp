#pragma once

#include "wcgeMat2x2.hpp"
#include "wcgeMat3x3.hpp"
#include "wcgeVector2.hpp"

namespace wcge {
	namespace Math {
		/// <summary>
		/// A 3 dimensional vector that holds 3 values (x, y and z) templated on T. Vector3_generic also support some basic vector arithmatic
		/// via operator overloading.
		/// </summary>
		/// <typeparam name="T">T can be any data type that support the addition, substraction, multiplication and division operators</typeparam>
		template<typename T>
		struct Vector3_generic {

			union {
				T element[3];
				struct {
					T x;
					T y;
					T z;
				};
			};

			inline Vector3_generic()
				:
				x( 0 ),
				y( 0 ),
				z( 0 ) {
			}

			inline Vector3_generic(
				T x,
				T y
			)
				:
				x( x ),
				y( y ),
				z( 0 ) {
			}

			inline Vector3_generic(
				T x,
				T y,
				T z
			)
				:
				x( x ),
				y( y ),
				z( z ) {
			}

			inline Vector3_generic( const Vector2_generic<T>& v )
				:
				Vector3_generic( v.x, v.y, 0.0 ) {
			}

			inline Vector3_generic( const Vector3_generic& v )
				:
				Vector3_generic( v.x, v.y, v.z ) {
			}

			inline T Magnitude() const {
				return (T)sqrt( x * x + y * y + z * z );
			}

			inline Vector3_generic& Normalize() {
				this->x /= this->Magnitude();
				this->y /= this->Magnitude();
				this->z /= this->Magnitude();
				return *this;
			}

			inline Vector3_generic operator - () const {
				return Vector3_generic( -this->x, -this->y, -this->z );
			}

			inline Vector3_generic operator + ( const Vector3_generic& rhs ) const {
				return Vector3_generic( this->x + rhs.x, this->y + rhs.y, this->z + rhs.z );
			}

			inline Vector3_generic operator - ( const Vector3_generic& rhs ) const {
				return Vector3_generic( this->x - rhs.x, this->y - rhs.y, this->z - rhs.z );
			}

			inline Vector3_generic operator * ( const T& rhs ) const {
				return Vector3_generic( this->x * rhs, this->y * rhs, this->z * rhs );
			}

			inline Vector3_generic operator % ( const Vector3_generic& rhs ) const {
				return Vector3_generic(
					this->y * rhs.z - this->z * rhs.y,
					this->z * rhs.x - this->x * rhs.z,
					this->x * rhs.y - this->y * rhs.x
				);
			}

			inline Vector3_generic operator / ( const T& rhs ) const {
				return Vector3_generic( this->x / rhs, this->y / rhs, this->z / rhs );
			}

			inline T operator * ( const Vector3_generic& rhs ) const {
				return (T)(this->x * rhs.x + this->y * rhs.y + this->z * rhs.z);
			}

			inline Vector3_generic operator ^ ( const Vector3_generic& rhs ) {
				return Vector3_generic
				(
					this->y * rhs.z - this->z * rhs.y,
					this->z * rhs.x - this->x * rhs.z,
					this->x * rhs.y - this->y * rhs.x
				);
			}

			inline Vector3_generic& operator += ( const Vector3_generic& rhs ) {
				this->x += rhs.x;
				this->y += rhs.y;
				this->z += rhs.z;
				return *this;
			}

			inline Vector3_generic& operator -= ( const Vector3_generic& rhs ) {
				this->x -= rhs.x;
				this->y -= rhs.y;
				this->z -= rhs.z;
				return *this;
			}

			inline Vector3_generic& operator *= ( const T& rhs ) {
				this->x *= rhs;
				this->y *= rhs;
				this->z *= rhs;
				return *this;
			}

			inline Vector3_generic& operator /= ( const T& rhs ) {
				this->x /= rhs;
				this->y /= rhs;
				this->z /= rhs;
				return *this;
			}

			inline bool operator == ( const Vector3_generic& rhs ) const {
				return
					(this->x == rhs.x) &&
					(this->y == rhs.y) &&
					(this->z == rhs.z)
					;
			}

			inline bool operator != ( const Vector3_generic& rhs ) const {
				return
					(this->x != rhs.x) &&
					(this->y != rhs.y) &&
					(this->z != rhs.z)
					;
			}

			inline T& operator[]( size_t index ) const {
				return element[index];
			}

			inline Vector3_generic operator * ( const Mat3x3_generic<T>& rhs ) const {
				Vector3_generic result;
				for ( int colomn = 0; colomn < 3; colomn++ )
					for ( int row = 0; row < 3; row++ )
						result.element[colomn] += this->element[row] * rhs.elements[row][colomn];
				return result;
			}

			inline Vector3_generic& operator *= ( const Mat3x3_generic<T>& rhs ) {
				Vector3_generic temp = *this;
				this->x = 0; this->y = 0; this->z = 0;
				for ( int colomn = 0; colomn < 3; colomn++ )
					for ( int row = 0; row < 3; row++ )
						*this[colomn] += temp[row] * rhs[row][colomn];
				return *this;
			}
		};

		/// <summary>
		/// Vector3_generic templated on float
		/// </summary>
		typedef Vector3_generic<float> Vector3;

		/// <summary>
		/// Vector3_generic templated on double
		/// </summary>
		typedef Vector3_generic<double> Vector3d;

		/// <summary>
		/// Vector3_generic templated on int
		/// </summary>
		typedef Vector3_generic<int> Vector3i;

		static const Vector3 VEC3_ZERO = Vector3( 0.0, 0.0, 0.0 );
		static const Vector3 VEC3_UP = Vector3( 0.0, 1.0, 0.0 );
		static const Vector3 VEC3_DOWN = Vector3( 0.0, -1.0, 0.0 );
		static const Vector3 VEC3_RIGHT = Vector3( 1.0, 0.0, 0.0 );
		static const Vector3 VEC3_LEFT = Vector3( -1.0, 0.0, 0.0 );
		static const Vector3 VEC3_FORWARD = Vector3( 0.0, 0.0, 1.0 );
		static const Vector3 VEC3_BACKWORD = Vector3( 0.0, 0.0, -1.0 );
	}

}