#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <numbers>

#include <array>
#include <assert.h>
#include <algorithm>

template<typename T> class		Vector2;
template<typename T> class		Vector3;
template<typename T> class		Vector4;
template<typename T> class		Matrix4x4;

typedef Vector2<float>			vec2;
typedef Vector2<double>			dvec2;
typedef Vector2<int>			ivec2;
typedef Vector2<unsigned int>	uvec2;
typedef Vector2<bool>			bvec2;

typedef Vector3<float>			vec3;
typedef Vector3<double>			dvec3;
typedef Vector3<int>			ivec3;
typedef Vector3<unsigned int>	uvec3;
typedef Vector3<bool>			bvec3;

typedef Vector4<float>			vec4;
typedef Vector4<double>			dvec4;
typedef Vector4<int>			ivec4;
typedef Vector4<unsigned int>	uvec4;
typedef Vector4<bool>			bvec4;

typedef Matrix4x4<float>		mat4;



//-----------------------------------------------------------------------------
//	scaler functions
//-----------------------------------------------------------------------------
template <typename T>
inline T clamp(T v, T a, T b) {
	assert(a <= b);
	return std::max(std::min(v, b), a);
}

template <typename T>
inline T mod(T a, T b) {
	assert(b != 0);
	return a - b * floor(a / b);
}

inline float radians(float d) {
	return d * std::numbers::pi_v<float> / 180;
}

inline double radians(double d) {
	return d * std::numbers::pi / 180;
}


//-----------------------------------------------------------------------------
//	vec2
//-----------------------------------------------------------------------------

template<typename T>
class Vector2 {
public:
	T x, y;

	Vector2(T v = 0) :x(v), y(v) {/*...*/ };
	Vector2(T x, T y) :x(x), y(y) { /*...*/ };

	template <typename T2>
	Vector2(const Vector2<T2>& other) {
		x = other.x;
		y = other.y;
	}

	Vector2<T>& operator=(T s) { x = s; y = s; return *this; }

	T& operator[](uint8_t i) { assert(i >= 0 && i < 2); if (i == 0) return x; return y; }

	bool operator ==(const Vector2<T>& b)		const { return   x == b.x && y == b.y; }
	bool operator !=(const Vector2<T>& b)		const { return !(x == b.x && y == b.y); }

	Vector2<T>  operator-()						const { return Vector2<T>(-x, -y); }
	Vector2<T>  operator+()						const { return Vector2<T>(+x, +y); }

	Vector2<T>  operator+ (Vector2<T> const& v) const	{ return Vector2<T>(x + v.x, y + v.y); }
	Vector2<T>& operator+=(Vector2<T> const& v)			{ x += v.x; y += v.y; return *this; }

	Vector2<T>  operator- (Vector2<T> const& v) const { return Vector2(x - v.x, y - v.y); }
	Vector2<T>& operator-=(Vector2<T> const& v) { x -= v.x; y -= v.y; return *this; }

	Vector2<T>  operator* (Vector2<T> const& v)	const { return Vector2<T>(x * v.x, y * v.y); }
	Vector2<T>  operator* (T s)					const { return Vector2<T>(s * x, s * y); }
	Vector2<T>& operator*=(T s) { x *= s; y *= s; return *this; }

	Vector2<T>  operator/ (T f)					const { float inv = 1.0f / f; return Vector2<T>(x * inv, y * inv); }
	Vector2<T>& operator/=(T f) { float inv = 1.0f / f; x *= inv; y *= inv; return *this; }
	Vector2<T>  operator/ (Vector2<T> v)		const { return Vector2<T>(x / v.x, y / v.y); }
	Vector2<T>& operator/=(Vector2<T> v) { x /= v.x; y /= v.y; return *this; }

	float lengthSquared()						const { return x * x + y * y; }
	float length()								const { return std::sqrt(lengthSquared()); }
	bool  hasNaNs()								const { return std::isnan(x) || std::isnan(y); }
};




//-----------------------------------------------------------------------------
//	vec2 functions
//-----------------------------------------------------------------------------

template<typename T> Vector2<T> min(Vector2<T> const& a, Vector2<T> const& b) {
	return Vector2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
}

template<typename T> Vector2<T> max(Vector2<T> const& a, Vector2<T> const& b) {
	return Vector2<T>(std::max(a.x, b.x), std::max(a.y, b.y));
}

template <typename T> Vector2<T> abs(Vector2<T> const& v) {
	return Vector2<T>(std::abs(v.x), std::abs(v.y));
}

template<typename T> inline Vector2<T> normalize(Vector2<T> const& v) {
	assert(v.length() != 0);
	return v / v.length();
}

template <typename T> inline T dot(Vector2<T> const& a, Vector2<T> const& b) {
	return a.x * b.x + a.y * b.y;
}

template <typename T> inline Vector2<T> clamp(Vector2<T> const& v, Vector2<T> const& a, Vector2<T> const& b) {
	return max(min(v, b), a);
}

template <typename T> inline float distance(Vector2<T> const& a, Vector2<T> const& b) {
	return (a - b).length();
}

template <typename T> inline Vector2<bool> equal(Vector2<T> const& a, Vector2<T> const& b) {
	return Vector2<bool>(a.x == b.x, a.y == b.y);
}

template <typename T> inline Vector2<bool> greaterThan(Vector2<T> const& a, Vector2<T> const& b) {
	return Vector2<bool>(a.x > b.x, a.y > b.y);
}

template <typename T> inline Vector2<bool> greaterThanEqual(Vector2<T> const& a, Vector2<T> const& b) {
	return Vector2<bool>(a.x >= b.x, a.y >= b.y);
}

template <typename T> inline Vector2<bool> lessThan(Vector2<T> const& a, Vector2<T> const& b) {
	return Vector2<bool>(a.x < b.x, a.y < b.y);
}

template <typename T> inline Vector2<bool> lessThanEqual(Vector2<T> const& a, Vector2<T> const& b) {
	return Vector2<bool>(a.x <= b.x, a.y <= b.y);
}

inline bool all(bvec2 const& a) {
	return a.x && a.y;
}

inline bool any(bvec2 const& a) {
	return a.x || a.y;
}

template <typename T> inline Vector2<T> floor(Vector2<T> const& a) {
	return Vector2<T>(floor(a.x), floor(a.y));
}

template <typename T> inline Vector2<bool> mod(Vector2<T> const& a, Vector2<T> const& b) {
	return Vector2<T>(mod(a.x, b.x), mod(a.y, b.y));
}

//-----------------------------------------------------------------------------
//	vec3
//-----------------------------------------------------------------------------

template<typename T>
class Vector3 {
public:
	T x, y, z;

	constexpr Vector3(T v = 0) :x(v), y(v), z(v) {/*...*/ };
	constexpr Vector3(T x, T y, T z) :x(x), y(y), z(z) { /*...*/ };


	template <typename T2>
	explicit Vector3(const Vector3<T2>& other) {
		x = static_cast<T>(other.x);
		y = static_cast<T>(other.y);
		z = static_cast<T>(other.z);
	}

	Vector3<T>& operator=(T s) { x = s; y = s; z = s; return *this; }

	T& operator[](uint8_t i) { assert(i >= 0 && i < 3); if (i == 0) return x; if (i == 1) return y; return z; }

	bool operator ==(const Vector3<T>& b)		const { return   x == b.x && y == b.y && z == b.z; }
	bool operator !=(const Vector3<T>& b)		const { return !(x == b.x && y == b.y && z == b.z); }

	Vector3<T>  operator-()						const { return Vector3<T>(-x, -y, -z); }
	Vector3<T>  operator+()						const { return Vector3<T>(+x, +y, +z); }

	Vector3<T>  operator+ (Vector3<T> const& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3<T>& operator+=(Vector3<T> const& v) { x += v.x; y += v.y; z += v.z; return *this; }

	Vector3<T>  operator- (Vector3<T> const& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	Vector3<T>& operator-=(Vector3<T> const& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

	Vector3<T>  operator* (Vector3<T> const& v)	const { return Vector3<T>(x * v.x, y * v.y, z * v.z); }
	Vector3<T>  operator* (T s)					const { return Vector3<T>(s * x, s * y, s * z); }
	Vector3<T>& operator*=(T s) { x *= s; y *= s; z *= s; return *this; }

	Vector3<T>  operator/ (T f)					const { float inv = 1.0f / f; return Vector3<T>(x * inv, y * inv, z * inv); }
	Vector3<T>& operator/=(T f) { float inv = 1.0f / f; x *= inv; y *= inv; z *= inv; return *this; }
	Vector3<T>  operator/ (Vector3<T> v)		const { return Vector3<T>(x / v.x, y / v.y, z / v.z); }
	Vector3<T>& operator/=(Vector3<T> v) { x /= v.x; y /= v.y;  z /= v.z; return *this; }

	float lengthSquared()						const { return x * x + y * y + z * z; }
	float length()								const { return std::sqrt(lengthSquared()); }
	bool  hasNaNs()								const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }
};






//-----------------------------------------------------------------------------
//	vec3 functions
//-----------------------------------------------------------------------------

template<typename T> inline Vector3<T> cross(Vector3<T> const& a, Vector3<T> const& b) {
	return Vector3<T>((a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x));
}



template<typename T> Vector3<T> min(Vector3<T> const& a, Vector3<T> const& b) {
	return Vector3<T>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}
template<typename T> Vector3<T> max(Vector3<T> const& a, Vector3<T> const& b) {
	return Vector3<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

template <typename T> Vector3<T> abs(Vector3<T> const& v) {
	return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template<typename T> inline Vector3<T> normalize(Vector3<T> const& v) {
	assert(v.length() != 0);
	return v / v.length();
}

template <typename T> inline T dot(Vector3<T> const& a, Vector3<T> const& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T> inline Vector3<T> clamp(Vector3<T> const& v, Vector3<T> const& a, Vector3<T> const& b) {
	return max(min(v, b), a);
}

template <typename T> inline float distance(Vector3<T> const& a, Vector3<T> const& b) {
	return (a - b).length();
}


template <typename T> inline Vector3<bool> equal(Vector3<T> const& a, Vector3<T> const& b) {
	return Vector3<bool>(a.x == b.x, a.y == b.y, a.z == b.z);
}

template <typename T> inline Vector3<bool> greaterThan(Vector3<T> const& a, Vector3<T> const& b) {
	return Vector3<bool>(a.x > b.x, a.y > b.y, a.z > b.z);
}

template <typename T> inline Vector3<bool> greaterThanEqual(Vector3<T> const& a, Vector3<T> const& b) {
	return Vector3<bool>(a.x >= b.x, a.y >= b.y, a.z >= b.z);
}

template <typename T> inline Vector3<bool> lessThan(Vector3<T> const& a, Vector3<T> const& b) {
	return Vector3<bool>(a.x < b.x, a.y < b.y, a.z < b.z);
}

template <typename T> inline Vector3<bool> lessThanEqual(Vector3<T> const& a, Vector3<T> const& b) {
	return Vector3<bool>(a.x <= b.x, a.y <= b.y, a.z <= b.z);
}

inline bool all(bvec3 const& a) {
	return a.x && a.y && a.z;
}

inline bool any(bvec3 const& a) {
	return a.x || a.y || a.z;
}

template <typename T> inline Vector3<T> floor(Vector3<T> const& a) {
	return Vector3<T>(floor(a.x), floor(a.y), floor(a.z));
}

template <typename T> inline Vector3<bool> mod(Vector3<T> const& a, Vector3<T> const& b) {
	return Vector3<T>(mod(a.x, b.x), mod(a.y, b.y), mod(a.z, b.z));
}

//-----------------------------------------------------------------------------
//	vec4
//-----------------------------------------------------------------------------

template<typename T>
class Vector4 {
public:
	T x, y, z, w;

	Vector4(T v = 0) :x(v), y(v), z(v), w(v) { /*.*/ };
	Vector4(T x, T y, T z, T w) :x(x), y(y), z(z), w(w) { /*.*/ };
	Vector4(Vector2<T> v, T z, T w) :x(v.x), y(v.y), z(z), w(w) { /*.*/ };
	Vector4(Vector3<T> v, T w) :x(v.x), y(v.y), z(v.z), w(w) { /*.*/ };

	template <typename T2>
	Vector4(const Vector4<T2>& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	Vector4<T>& operator=(T s) { x = s; y = s; z = s; w = s; return *this; }

	T& operator[](uint8_t i) { assert(i >= 0 && i < 4); if (i == 0) return x; if (i == 1) return y; if (i == 2) return z; return w; }

	bool operator ==(const Vector4<T>& b)		const { return   x == b.x && y == b.y && z == b.z && w == b.w; }
	bool operator !=(const Vector4<T>& b)		const { return !(x == b.x && y == b.y && z == b.z && w == b.w); }

	Vector4<T>  operator-()						const { return Vector4<T>(-x, -y, -z, -w); }
	Vector4<T>  operator+()						const { return Vector4<T>(+x, +y, +z, +w); }

	Vector4<T>  operator+ (Vector4<T> const& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
	Vector4<T>& operator+=(Vector4<T> const& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }

	Vector4<T>  operator- (Vector4<T> const& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vector4<T>& operator-=(Vector4<T> const& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

	Vector4<T>  operator* (T s)					const { return Vector4<T>(s * x, s * y, s * z, s * w); }
	Vector4<T>& operator*=(T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
	Vector4<T>  operator* (Vector4<T> const& v)	const { return Vector4<T>(x * v.x, y * v.y, z * v.z, w * v.w); }

	Vector4<T>  operator/ (T f)					const { float inv = 1.0f / f; return Vector4<T>(x * inv, y * inv, z * inv, w * inv); }
	Vector4<T>& operator/=(T f) { float inv = 1.0f / f; x *= inv; y *= inv; z *= inv; w *= inv; return *this; }
	Vector4<T>  operator/ (Vector4<T> v)		const { return Vector4<T>(x / v.x, y / v.y, z / v.z, w / v.w); }
	Vector4<T>& operator/=(Vector4<T> v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

	float lengthSquared()						const { return x * x + y * y + z * z + w * w; }
	float length()								const { return std::sqrt(lengthSquared()); }
	bool  hasNaNs()								const { return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w); }
};





//-----------------------------------------------------------------------------
//	vec4 functions
//-----------------------------------------------------------------------------

template<typename T> Vector4<T> min(Vector4<T> const& a, Vector4<T> const& b) {
	return Vector4<T>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
}
template<typename T> Vector4<T> max(Vector4<T> const& a, Vector4<T> const& b) {
	return Vector4<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
}

template <typename T> Vector4<T> abs(Vector4<T> const& v) {
	return Vector4<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z).std::abs(v.w));
}

template<typename T> inline Vector4<T> normalize(Vector4<T> const& v) {
	assert(v.length() != 0);
	return v / v.length();
}

template <typename T> inline T dot(Vector4<T> const& a, Vector4<T> const& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T> inline Vector4<T> clamp(Vector4<T> const& v, Vector4<T> const& a, Vector4<T> const& b) {
	return max(min(v, b), a);
}

template <typename T> inline float distance(Vector4<T> const& a, Vector4<T> const& b) {
	return (a - b).length();
}

template <typename T> inline Vector4<bool> equal(Vector4<T> const& a, Vector4<T> const& b) {
	return Vector4<bool>(a.x == b.x, a.y == b.y, a.z == b.z, a.w == b.w);
}

template <typename T> inline Vector4<bool> greaterThan(Vector4<T> const& a, Vector4<T> const& b) {
	return Vector4<bool>(a.x > b.x, a.y > b.y, a.z > b.z, a.w > b.w);
}

template <typename T> inline Vector4<bool> greaterThanEqual(Vector4<T> const& a, Vector4<T> const& b) {
	return Vector4<bool>(a.x >= b.x, a.y >= b.y, a.z >= b.z, a.w >= b.w);
}

template <typename T> inline Vector4<bool> lessThan(Vector4<T> const& a, Vector4<T> const& b) {
	return Vector4<bool>(a.x < b.x, a.y < b.y, a.z < b.z, a.w < b.w);
}

template <typename T> inline Vector4<bool> lessThanEqual(Vector4<T> const& a, Vector4<T> const& b) {
	return Vector4<bool>(a.x <= b.x, a.y <= b.y, a.z <= b.z, a.w <= b.w);
}

inline bool all(bvec4 const& a) {
	return a.x && a.y && a.z && a.w;
}

inline bool any(bvec4 const& a) {
	return a.x || a.y || a.z || a.w;
}

template <typename T> inline Vector4<T> floor(Vector4<T> const& a) {
	return Vector4<T>(floor(a.x), floor(a.y), floor(a.z), floor(a.w));
}

template <typename T> inline Vector4<bool> mod(Vector4<T> const& a, Vector4<T> const& b) {
	return Vector4<T>(mod(a.x, b.x), mod(a.y, b.y), mod(a.z, b.z), mod(a.w, b.w));
}



//-----------------------------------------------------------------------------
//	mat4
//-----------------------------------------------------------------------------

//	memory layout: row-major order
//		-> matrix[i][j]: i = row,  j = collumn
//
//	vectors are expressed as columns
//		->	columns of the matrix represent the bases of a coordinate system 
//		->	post-multiplication (M*v = v')
//		->	translation vector lies on indecies 3, 7, 11
//			openGL expects it to be at 12, 13, 14 post multiplying
//				-> calling glUniformMatrix4fv with transpose=true when 



template<typename T>
class Matrix4x4 {
public:

	Matrix4x4() :m{ {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}} } {
		//..
	}

	Matrix4x4(T t00, T t01, T t02, T t03,
		T t10, T t11, T t12, T t13,
		T t20, T t21, T t22, T t23,
		T t30, T t31, T t32, T t33)
		:m{ {{t00, t01, t02, t03},
			{t10, t11, t12, t13},
			{t20, t21, t22, t23},
			{t30, t31, t32, t33}} }
	{
		//...
	}

	//returns row i
	std::array<T, 4>& operator [] (uint8_t i) {
		return m[i];
	}

	//returns row i
	std::array<T, 4> const& operator [] (uint8_t i) const {
		return m[i];
	}


	bool operator ==(Matrix4x4 const& other) const {
		for (uint8_t i = 0; i < 4; i++)
			for (uint8_t j = 0; j < 4; j++)
				if (other.m[i][j] != m[i][j])
					return false;
		return true;
	}


	Matrix4x4 operator* (Matrix4x4 const& other) const {
		Matrix4x4 result;
		for (uint8_t i = 0; i < 4; i++)
			for (uint8_t j = 0; j < 4; j++)
				result[i][j] = m[i][0] * other.m[0][j]
				+ m[i][1] * other.m[1][j]
				+ m[i][2] * other.m[2][j]
				+ m[i][3] * other.m[3][j];
		return result;
	}


	//post multiplication with vector
	vec4 operator* (vec4 const& v) const {
		return vec4(
			v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + v.w * m[0][3],
			v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + v.w * m[1][3],
			v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + v.w * m[2][3],
			v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + v.w * m[3][3]
		);
	}



	std::array<std::array<T, 4>, 4> m;	// m[i][j]: i = row,  j = collumn
};


//-----------------------------------------------------------------------------
//	mat4 functions
//-----------------------------------------------------------------------------


template<typename T>
Matrix4x4<T> transpose(Matrix4x4<T> const& input) {
	Matrix4x4<T> result;
	for (uint8_t i = 0; i < 4; i++)
		for (uint8_t j = 0; j < 4; j++)
			result.m[i][j] = input.m[j][i];
	return result;
}


inline mat4 mat4_translate(float x, float y, float z) {
	return mat4(1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1);
}


inline mat4 mat4_translate(vec3 const& offset) {
	return mat4_translate(offset.x, offset.y, offset.z);
}


inline mat4 mat4_scale(vec3 const& scale) {
	return mat4(scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1);
}



inline mat4 mat4_rotate_axis(vec3 const& a, float theta) {
	float sinT = std::sin(theta);
	float cosT = std::cos(theta);
	float oneMinusCosT = 1.0f - cosT;

	return mat4(cosT + a.x * a.x * oneMinusCosT, a.x * a.y * oneMinusCosT - a.z * sinT, a.x * a.z * oneMinusCosT + a.y * sinT, 0,
		a.y * a.x * oneMinusCosT + a.z * sinT, cosT + a.y * a.y * oneMinusCosT, a.y * a.z * oneMinusCosT - a.x * sinT, 0,
		a.z * a.x * oneMinusCosT - a.y * sinT, a.z * a.y * oneMinusCosT + a.x * sinT, cosT + a.z * a.z * oneMinusCosT, 0,
		0, 0, 0, 1);
}


inline mat4 mat4_lookAt(vec3 const& pos, vec3 const& target, vec3 const& up) {
	vec3 dir = normalize(target - pos);
	vec3 right = normalize(cross(dir, up));
	vec3 newUp = cross(right, dir);

	return mat4(right.x, right.y, right.z, -dot(pos, right),
		newUp.x, newUp.y, newUp.z, -dot(pos, newUp),
		dir.x, dir.y, dir.z, -dot(pos, dir),
		0, 0, 0, 1);

}

inline mat4 mat4_lookAt_inv(vec3 const& pos, vec3 const& target, vec3 const& up) {
	vec3 dir = normalize(target - pos);
	vec3 right = normalize(cross(dir, up));
	vec3 newUp = cross(right, dir);

	return mat4(right.x, newUp.x, dir.x, pos.x,
		right.y, newUp.y, dir.y, pos.y,
		right.z, newUp.z, dir.z, pos.z,
		0, 0, 0, 1);
}




// z points forward
inline mat4 mat4_perspective(float fovRadians, float aspect, float near, float far) {

	float scale = 1.0f / std::tan(0.5f * fovRadians);
	return mat4(scale / aspect, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, far / (far - near), -far * near / (far - near),
		0, 0, 1, 1);
 }
