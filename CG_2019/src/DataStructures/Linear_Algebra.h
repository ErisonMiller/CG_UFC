#pragma once
#ifndef __LINEAR_ALGEBRA_H_
#define __LINEAR_ALGEBRA_H_

//
//		Linear Algebra Core
//
// This header contains the basic struct for:
//		- Vector4
//		- Matrix4
//
// All the critical components are using vector instructions for performance improvement
//


#include <math.h>
//vector instruction set include 
//#include <xmmintrin.h>
#include <immintrin.h>

namespace CRAB {

#define M_PI 3.14156265f
#define SMALL_NUMBER 0.000001f


	
	__declspec(align(16))//memory aling to speed up acess
	struct Vector4Df
	{
		union {//union btw array, floats to use the names and _m128 to use vetorial instructions
			struct { float x, y, z, w; };
			float _v[4];
			__m128 v128;
		};

		//_mm_dp_ps is the vetorial instruction for dot product
		__forceinline float lengthsq() const { return _mm_cvtss_f32(_mm_dp_ps(v128, v128, 0xff)); }
		__forceinline float length() const { return sqrtf(lengthsq()); }
		//inline void normalize() { float norm = length();  x /= norm; y /= norm; z /= norm; };
		inline void normalize() { v128 = _mm_div_ps(v128, _mm_sqrt_ps(_mm_dp_ps(v128, v128, 0xff))); };
		inline Vector4Df to_unitary() const { float norm = 1.0f/length(); return Vector4Df{ x *norm, y * norm, z * norm, w }; }
		inline void operator+=(const Vector4Df& v) { x += v.x; y += v.y; z += v.z; }
		inline void operator-=(const Vector4Df& v) { x -= v.x; y -= v.y; z -= v.z; }
		inline void operator*=(const float& a) { x *= a; y *= a; z *= a; }
		inline void operator*=(const Vector4Df& v) { x *= v.x; y *= v.y; z *= v.z; }
		inline void operator/=(const float& a) { x /= a; y /= a; z /= a; }
		//inline Vector4Df operator*(float a) const { return Vector4Df{ x*a, y*a, z*a, w }; }
		inline Vector4Df operator*(const float a) const { return *(Vector4Df*)&_mm_mul_ps(v128, _mm_set_ps1(a)); }
		//inline Vector4Df operator/(const float a) const { return Vector4Df{ x / a, y / a, z / a, w }; }
		inline Vector4Df operator/(const float a) const { return *(Vector4Df*)&_mm_div_ps(v128, _mm_set_ps1(a)); }
		//inline Vector4Df operator*(const Vector4Df& v) const { return Vector4Df{ x * v.x, y * v.y, z * v.z, w * v.w }; }
		__forceinline Vector4Df operator*(const Vector4Df& v) const { return *(Vector4Df*)&_mm_mul_ps(v128, v.v128); }
		//inline Vector4Df operator/(const Vector4Df& v) const { return Vector4Df{ x / v.x, y / v.y, z / v.z, w / v.w }; }
		inline Vector4Df operator/(const Vector4Df& v) const { return *(Vector4Df*)&_mm_div_ps(v128, v.v128); }
		//inline Vector4Df operator+(const Vector4Df& v) const { return Vector4Df{ x + v.x, y + v.y, z + v.z, w + v.w }; }
		inline Vector4Df operator+(const Vector4Df& v) const { return *(Vector4Df*)&_mm_add_ps(v128, v.v128); }
		//inline Vector4Df operator-(const Vector4Df& v) const { return Vector4Df{ x - v.x, y - v.y, z - v.z, w - v.w }; }
		inline Vector4Df operator-(const Vector4Df& v) const { return *(Vector4Df*)&_mm_sub_ps(v128, v.v128); }
		inline bool operator!=(const Vector4Df& v) const { return x != v.x || y != v.y || z != v.z; }
		inline bool operator==(const Vector4Df& v) const { return fabsf(x - v.x) < SMALL_NUMBER && fabsf(y - v.y) < SMALL_NUMBER && fabsf(z - v.z) < SMALL_NUMBER; }

		inline operator __m128() const { return v128; }
	};

	//normal cross product
	inline Vector4Df cross(const Vector4Df& v1, const Vector4Df& v2) { return { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x, 0.0f }; }
	//cross product using vetorial instructions

	__forceinline __m128 cross_simd_mm128(const __m128& v1, const __m128& v2) {
		const __m128 &v1_yzx = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));
		const __m128 &v2_yzx = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1));
		const __m128 &mul1 = _mm_mul_ps(v1, v2_yzx);
		const __m128 &mul2 = _mm_mul_ps(v1_yzx, v2);
		const __m128 &c = _mm_sub_ps(mul1, mul2);
		return _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));
	}
	__forceinline Vector4Df cross_simd(const __m128& v1, const __m128& v2) {
		return *(Vector4Df*)&cross_simd_mm128(v1,v2);
	}


	//normal dot product
	inline float dot(const Vector4Df& v1, const Vector4Df& v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }
	//dot product using vetorial instructions
	__forceinline float dot_simd(const __m128& v1, const __m128& v2) {return _mm_cvtss_f32(_mm_dp_ps(v1, v2, 0xff));}
	__forceinline Vector4Df dot_simd_Vec(const __m128& v1, const __m128& v2) { return *(Vector4Df*)&_mm_dp_ps(v1, v2, 0xff); }

	//reflection vector
	inline Vector4Df reflection(const Vector4Df& v, const Vector4Df& mirror) { return (mirror * dot(mirror, v))*(2.0f) - v; }

	inline float distancesq(const Vector4Df& v1, const Vector4Df& v2) { return (v1-v2).lengthsq(); }
	inline float distance(const Vector4Df& v1, const Vector4Df& v2) { return sqrtf(distancesq(v1,v2)); }


	__declspec(align(32))
	struct Matrix4 {
		//using row-major order 
		union
		{
			float m[16];
			float _m[4][4];
			__m128 row[4];
		};

		__forceinline Vector4Df operator *(const Vector4Df &v) const {
			return Vector4Df{
				dot_simd(v, row[0]),
				dot_simd(v, row[1]),
				dot_simd(v, row[2]),
				dot_simd(v, row[3]) };
		}

		inline Matrix4 Transpose() const {
			Matrix4 mt = *this;
			_MM_TRANSPOSE4_PS(mt.row[0], mt.row[1], mt.row[2], mt.row[3]);
			return mt;
		}

		//matrix multiplication using vetorial instructions
		inline Matrix4 operator *(const Matrix4 &m1) const {
			Matrix4 mr;//answer matrix 
			//join the rows in two __m256 registers to make the calculations faster
			const register __m256 row_1_2 = _mm256_set_m128(m1.row[0], m1.row[1]), row_3_4 = _mm256_set_m128(m1.row[2], m1.row[3]);
			//_mm_broadcast_ss load one single float to the 4 floats of a __m128
			//_mm256_set_m128 joins 2 __m128 into a __m256
			//_mm256_add_ps adds float by float the floats in both __m256 
			//_mm256_mul_ps multiply float by float the floats in both __m256 
			const __m256 a{ _mm256_add_ps( 
				_mm256_mul_ps(_mm256_set_m128(_mm_broadcast_ss(&m[0]), _mm_broadcast_ss(&m[1])), row_1_2),
				_mm256_mul_ps(_mm256_set_m128(_mm_broadcast_ss(&m[2]), _mm_broadcast_ss(&m[3])), row_3_4)
			) };
			const __m256 b{ _mm256_add_ps(
				_mm256_mul_ps(_mm256_set_m128(_mm_broadcast_ss(&m[4]), _mm_broadcast_ss(&m[5])), row_1_2),
				_mm256_mul_ps(_mm256_set_m128(_mm_broadcast_ss(&m[6]), _mm_broadcast_ss(&m[7])), row_3_4)
			) };

			
			_mm256_store_ps((float*)&mr.m,
				_mm256_add_ps(//permutes a and b, after this we add then to discover the two firt rows of mr
					_mm256_permute2f128_ps(b, a, 0x02),
					_mm256_permute2f128_ps(b, a, 0x13)
				));

			//same of before to rows 3 and 4
			const __m256 a2{ _mm256_add_ps(
				_mm256_mul_ps(_mm256_set_m128(_mm_broadcast_ss(&m[ 8]), _mm_broadcast_ss(&m[ 9])), row_1_2),
				_mm256_mul_ps(_mm256_set_m128(_mm_broadcast_ss(&m[10]), _mm_broadcast_ss(&m[11])), row_3_4)
			) };
			const __m256 b2{ _mm256_add_ps(
				_mm256_mul_ps(_mm256_set_m128(_mm_broadcast_ss(&m[12]), _mm_broadcast_ss(&m[13])), row_1_2),
				_mm256_mul_ps(_mm256_set_m128(_mm_broadcast_ss(&m[14]), _mm_broadcast_ss(&m[15])), row_3_4)
			) };

			_mm256_store_ps((float*)&mr.row[2],
				_mm256_add_ps(
					_mm256_permute2f128_ps(b2, a2, 0x02),
					_mm256_permute2f128_ps(b2, a2, 0x13)
				));
			
			return mr;
		}
	};

	/*
	----------------- Create the transformation matrices----------------------
	*/
	inline
		Matrix4 Identity()
	{
		return Matrix4{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
	}

	inline
		Matrix4 translate(const Vector4Df &v)
	{
		return Matrix4{
			1, 0, 0, v.x,
			0, 1, 0, v.y,
			0, 0, 1, v.z,
			0, 0, 0, 1 };
	}

	inline
		Matrix4 resizeMatrix(float x, float y, float z)
	{
		return Matrix4{
		x,	0,	0,	0,
		0,	y,	0,	0,
		0,	0,	z,	0,
		0,	0,	0,	1 };

	}

	inline
		Matrix4 rotateX(float ang)
	{
		ang = (ang* M_PI) / 180.0f;

		return Matrix4{
		1,	0,	0,	0,
		0,	cosf(ang),	-sinf(ang),	0,
		0,	sinf(ang),	cosf(ang),	0,
		0,	0,	0,	1 };
	}

	inline
		Matrix4 rotateY(float ang)
	{
		ang = (ang* M_PI) / 180.0f;

		return Matrix4{
		cosf(ang),	0,	sinf(ang),	0,
		0,	1,	0,	0,
		-sinf(ang),	0,	cosf(ang),	0,
		0,	0,	0,	1 };
	}

	inline
		Matrix4 rotateZ(float ang)
	{
		ang = (ang* M_PI) / 180.0f;

		return Matrix4{
		cosf(ang),	-sinf(ang),	0,	0,
		sinf(ang),	cosf(ang),	0,	0,
		0,	0,	1,	0,
		0,	0,	0,	1 };

	}

	inline
		Matrix4 rotateArbitrary(float ang, Vector4Df &u)
	{
		u.normalize();

		ang = (ang* M_PI) / 180.0f;
		return Matrix4{
			cosf(ang) + u.x*u.x*(1 - cosf(ang)), u.y*u.x*(1 - cosf(ang)) - u.z*sinf(ang),
				   u.z*u.x*(1 - cosf(ang)) + u.y*sinf(ang),	0,

			u.y*u.x*(1 - cosf(ang)) + u.z*sinf(ang), cosf(ang) + u.y*u.y*(1 - cosf(ang)),
				   u.z*u.y*(1 - cosf(ang)) - u.x*sinf(ang),	0,

			u.z*u.x*(1 - cosf(ang)) - u.y*sinf(ang), u.z*u.y*(1 - cosf(ang)) + u.x*sinf(ang),
				   cosf(ang) + u.z*u.z*(1 - cosf(ang)),	0,

			0,0,0,1
		};
	}

	inline
		Matrix4 reflection_YZ()
	{

		return Matrix4{
	   -1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		0,	0,	0,	1 };
	}

	inline
		Matrix4 reflection_XZ()
	{

		return Matrix4{
	    1,	0,	0,	0,
		0, -1,	0,	0,
		0,	0,	1,	0,
		0,	0,	0,	1 };
	}

	inline
		Matrix4 reflection_XY()
	{

		return Matrix4{
		1,	0,	0,	0,
		0,  1,	0,	0,
		0,	0, -1,	0,
		0,	0,	0,	1 };
	}

	inline
		Matrix4 reflectionArbitrary(Vector4Df n)
	{

		return Matrix4{
		(1-(2*n.x*n.x)),    -2*n.x*n.y  ,    -2*n.x*n.z    ,	0,
		   -2*n.y*n.x  , (1-(2*n.y*n.y)),    -2*n.y*n.z    ,	0,
		   -2*n.z*n.x  ,    -2*n.z*n.y  , (1-(2*n.z*n.z))  ,	0,
		    0,	             0,	                 0,	            1 };
	}

}
#endif