#pragma once
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>
#include "Vector2.h"

struct Vector3 final {
	float x, y, z;
	
	
	Vector2 xy() { return Vector2{ x,y }; }




	// == 演算子のオーバーロード
	bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}
	// = 演算子のオーバーロード
	Vector3 operator=(float other)  {
		return { x = other , y = other , z = other };
	}

	// + 演算子のオーバーロード
	Vector3 operator+(const Vector3& other) const {
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}
	// + 演算子のオーバーロード (スカラーとベクトル) 
	Vector3 operator+(float other) const {
		return Vector3{ x + other, y + other, z + other };;
	}
	// + 演算子のオーバーロード
	Vector3& operator+=(const Vector3& other)  {
		
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	// + 演算子のオーバーロード
	Vector3& operator-=(const Vector3& other)  {
		
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	// - 演算子のオーバーロード
	Vector3 operator-(const Vector3& other) const {
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}
	// - 演算子のオーバーロード
	Vector3 operator-(float other) const {
		return Vector3{ x - other, y - other, z - other };
	}

	// * 演算子のオーバーロード
	Vector3 operator*(const Vector3& other) const {
		return Vector3{ x * other.x, y * other.y, z * other.z };
	}

	// * 演算子のオーバーロード (スカラーとベクトル) 
	Vector3 operator*(float other) const {
		return Vector3{ x * other, y * other, z * other};;
	}
	 

	Vector3 operator-() const {
		return Vector3{ -x,-y,-z};
	};
	 


	// / 演算子のオーバーロード (スカラー除算) 
	Vector3 operator/(float s) const { 
		return Vector3{(x / s), (y / s), (z / s)}; 
	}

	// 正規化
	Vector3 Normalize() const{
		Vector3 result{};
		float length;

		length = sqrtf((x * x) + (y * y) + (z * z));

		if (length != 0.0f) {
			result.x = x / length;
			result.y = y / length;
			result.z = z / length;
		};

		return result;
	};
	// 内積
	float Dot(const Vector3& other) const {
		float result;

		result = x * other.x + y * other.y + z * other.z;


		return result;
	};
	// 長さ
	float Length() {
		float result;

		result = sqrtf((x * x) + (y * y) + (z * z));

		return result;
	};
	float LengthSq() const {
		return x * x + y * y + z * z;
	}
	// ベクトルの要素ごとの絶対値
	Vector3 Abs() const {
		return { std::abs(x), std::abs(y), std::abs(z) };
	}

	// ベクトルのクロス積（外積）
	Vector3 Cross(const Vector3& other) const {
		return {
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		};
	}

	/*Vector3 V3_V4(const Vector4& other) {
		return { x = other.x,y = other.y,z = other.z };
	}*/

};

static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
	Vector3 temp;
	t = std::clamp(t, 0.0f, 1.0f);
	temp.x = a.x * (1.0f - t) + b.x * t;
	temp.y = a.y * (1.0f - t) + b.y * t;
	temp.z = a.z * (1.0f - t) + b.z * t;

	return temp;
}
static Vector3 Normalize(const Vector3& v) {
	Vector3 result{};
	float length;

	length = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	if (length != 0.0f) {
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
	};

	return result;
};

static Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
};

static float Dot(const Vector3& v1, const Vector3& v2) {
	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;


	return result;
};

static Vector3 Max(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};


	result.x = (std::max)(v1.x, v2.x);
	result.y = (std::max)(v1.y, v2.y);
	result.z = (std::max)(v1.z, v2.z);

	return result;
}
static Vector3 Min(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};


	result.x = (std::min)(v1.x, v2.x);
	result.y = (std::min)(v1.y, v2.y);
	result.z = (std::min)(v1.z, v2.z);

	return result;
}

