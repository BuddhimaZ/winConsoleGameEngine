#pragma once

#include "wcgeMat2x2.hpp"

namespace wcge {
	namespace Math {
		/// <summary>
		/// A 2 dimensional vector that holds 2 values (x and y) templated on T. Vector2_generic also support some basic vector arithmatic
		/// via operator overloading.
		/// </summary>
		/// <typeparam name="T">T can be any data type that support the addition, substraction, multiplication and division operators</typeparam>
		template<typename T>
		struct Vector2_generic {

			union {
				T element[2];
				struct {
					T x;
					T y;
				};
			};

			inline Vector2_generic()
				:
				x( 0 ),
				y( 0 ) {
			}

			inline Vector2_generic(
				T x,
				T y
			)
				:
				x( x ),
				y( y ) {
			}

			inline Vector2_generic( const Vector2_generic& v )
				:
				Vector2_generic( v.x, v.y ) {
			}

			inline T Magnitude() const {
				return (T)sqrt( x * x + y * y );
			}

			inline Vector2_generic& Normalize() {
				this->x /= this->Magnitude();
				this->y /= this->Magnitude();
				return *this;
			}

			inline Vector2_generic operator - () const {
				return Vector2_generic( -this->x, -this->y );
			}

			inline Vector2_generic operator + ( const Vector2_generic& rhs ) const {
				return Vector2_generic( this->x + rhs.x, this->y + rhs.y );
			}

			inline Vector2_generic operator - ( const Vector2_generic& rhs ) const {
				return Vector2_generic( this->x - rhs.x, this->y - rhs.y );
			}

			inline Vector2_generic operator * ( const T& rhs ) const {
				return Vector2_generic( this->x * rhs, this->y * rhs );
			}

			inline Vector2_generic operator / ( const T& rhs ) const {
				return Vector2_generic( this->x / rhs, this->y / rhs );
			}

			inline T operator * ( const Vector2_generic& rhs ) const {
				return (T)(this->x * rhs.x + this->y * rhs.y);
			}

			inline Vector2_generic& operator += ( const Vector2_generic& rhs ) {
				this->x += rhs.x;
				this->y += rhs.y;
				return *this;
			}

			inline Vector2_generic& operator -= ( const Vector2_generic& rhs ) {
				this->x -= rhs.x;
				this->y -= rhs.y;
				return *this;
			}

			inline Vector2_generic& operator *= ( const T& rhs ) {
				this->x *= rhs;
				this->y *= rhs;
				return *this;
			}

			inline Vector2_generic& operator /= ( const T& rhs ) {
				this->x /= rhs;
				this->y /= rhs;
				return *this;
			}

			inline bool operator == ( const Vector2_generic& rhs ) const {
				return
					(this->x == rhs.x) &&
					(this->y == rhs.y)
					;
			}

			inline bool operator != ( const Vector2_generic& rhs ) const {
				return
					(this->x != rhs.x) &&
					(this->y != rhs.y)
					;
			}

			inline T& operator[] ( size_t index ) {
				return element[index];
			}

			inline Vector2_generic operator * ( const Mat2x2_generic<T>& rhs ) const {
				Vector2_generic result;
				for ( int colomn = 0; colomn < 2; colomn++ )
					for ( int row = 0; row < 2; row++ )
						result.element[colomn] += this->element[row] * rhs.elements[row][colomn];
				return result;
			}

			inline Vector2_generic& operator *= ( const Mat2x2_generic<T>& rhs ) {
				Vector2_generic temp = *this;
				this->x = 0; this->y = 0;
				for ( int colomn = 0; colomn < 2; colomn++ )
					for ( int row = 0; row < 2; row++ )
						*this[colomn] += temp[row] * rhs[row][colomn];
				return *this;
			}
		};

		/// <summary>
		/// Vector2_generic templated on float
		/// </summary>
		typedef Vector2_generic<float> Vector2;

		/// <summary>
		/// Vector2_generic templated on double
		/// </summary>
		typedef Vector2_generic<double> Vector2d;

		/// <summary>
		/// Vector2_generic templated on int
		/// </summary>
		typedef Vector2_generic<int> Vector2i;

		static const Vector2 VEC2_ZERO = Vector2( 0.0, 0.0 );
		static const Vector2 VEC2_UP = Vector2( 0.0, 1.0 );
		static const Vector2 VEC2_DOWN = Vector2( 0.0, -1.0 );
		static const Vector2 VEC2_RIGHT = Vector2( 1.0, 0.0 );
		static const Vector2 VEC2_LEFT = Vector2( -1.0, 0.0 );
	}

}