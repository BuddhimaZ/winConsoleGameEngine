#pragma once

#include <cmath>
#include <memory>

namespace wcge {

	namespace Math {
		/// <summary>
	/// A template generic class to create a matrix with any number of rows and any number of columns
	/// </summary>
	/// <typeparam name="T"> : T can be any data type that support the addition, substraction, multiplication and division operators</typeparam>
	/// <param name="rows"> : number of rows </param>
	/// <param name="columns"> : number of columns </param>
		template<
			typename T,
			size_t rows,
			size_t columns
		>
		struct Matrix_generic {
			T elements[rows][columns]{ 0 };

			inline Matrix_generic& operator= ( const Matrix_generic& rhs ) {
				memcpy( this->elements, rhs.elements, sizeof( this->elements ) );
				return *this;
			}

			template<
				size_t _rows,
				size_t _columns
			>
			inline Matrix_generic& operator= ( const Matrix_generic<T, _rows, _columns>& rhs ) {
				static_assert(_rows < rows&& _columns < columns, "Operator Override failed! (operator =) The matrix at the rhs has to be smaller both in rows and column than matrix at lhs");
				memset( this->elements, 0, sizeof( this->elements ) );
				for ( size_t r = 0; r < _rows; r++ )
					memcpy( this->elements[r], rhs.elements[r], sizeof( T ) * _columns );
				return *this;
			}

			inline T* operator [] ( const size_t& _row ) {
				return this->elements[_row];
			}

			inline Matrix_generic operator + ( const Matrix_generic& rhs ) const {
				Matrix_generic ret = *this;
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ )
						ret.elements[r][c] += rhs.elements[r][c];
				return ret;
			}

			inline Matrix_generic operator - ( const Matrix_generic& rhs ) const {
				Matrix_generic ret = *this;
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ )
						ret.elements[r][c] -= rhs.elements[r][c];
				return ret;
			}

			template<size_t rhs_columns>
			inline Matrix_generic<T, rows, rhs_columns> operator * ( const Matrix_generic<T, columns, rhs_columns>& rhs ) const {
				Matrix_generic<T, rows, rhs_columns> ret;
				for ( size_t ret_r = 0; ret_r < rows; ret_r++ )
					for ( size_t ret_c = 0; ret_c < rhs_columns; ret_c++ )
						for ( size_t c = 0; c < columns; c++ )
							ret.elements[ret_r][ret_c] += (this->elements[ret_r][c] * rhs.elements[c][ret_c]);
				return ret;
			}

			inline Matrix_generic operator * ( const T& rhs ) const {
				Matrix_generic ret = *this;
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ )
						ret.elements[r][c] *= rhs;
				return ret;
			}

			inline Matrix_generic operator / ( const T& rhs ) const {
				Matrix_generic ret = *this;
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ )
						ret.elements[r][c] /= rhs;
				return ret;
			}

			inline Matrix_generic& operator += ( const Matrix_generic& rhs ) {
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ )
						this->elements[r][c] += rhs.elements[r][c];
				return *this;
			}

			inline Matrix_generic& operator -= ( const Matrix_generic& rhs ) {
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ )
						this->elements[r][c] -= rhs.elements[r][c];
				return *this;
			}

			inline Matrix_generic& operator *= ( const T& rhs ) {
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ )
						this->elements[r][c] *= rhs;
				return *this;
			}

			inline Matrix_generic& operator *= ( const Matrix_generic& rhs ) {
				static_assert(rows == columns, "Operator Override failed! (operator *=) This operation only available with squre matrices");
				Matrix_generic temp;
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < rows; c++ )
						for ( size_t i = 0; i < rows; i++ )
							temp.elements[r][c] += (this->elements[r][i] * rhs.elements[i][c]);
				*this = temp;
				return *this;
			}

			inline Matrix_generic& operator /= ( const T& rhs ) {
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ )
						this->elements[r][c] /= rhs;
				return *this;
			}

			inline bool operator == ( const Matrix_generic& rhs ) const {
				bool is_equal = true;
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ ) {
						is_equal = is_equal && (this->elements[r][c] == rhs.elements[r][c]);
					}
				return is_equal;
			}

			inline bool operator != ( const Matrix_generic& rhs ) const {
				bool is_equal = true;
				for ( size_t r = 0; r < rows; r++ )
					for ( size_t c = 0; c < columns; c++ ) {
						is_equal = is_equal && (this->elements[r][c] == rhs.elements[r][c]);
					}
				return !is_equal;
			}

			static Matrix_generic Identity() {
				static_assert(rows == columns, "Function failed! (Identity()) This operation only available with squre matrices");
				Matrix_generic ret;
				for ( size_t i = 0; i < rows; i++ ) {
					ret.elements[i][i] = 1;
				}
				return ret;
			}

		};
	}
}