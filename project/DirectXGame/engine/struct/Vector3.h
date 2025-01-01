#pragma once
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>

struct Vector3 final {
	float x, y, z;

	// == 演算子のオーバーロード
	bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	// + 演算子のオーバーロード
	Vector3 operator+(const Vector3& other) const {
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	// - 演算子のオーバーロード
	Vector3 operator-(const Vector3& other) const {
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}

	// * 演算子のオーバーロード
	Vector3 operator*(const Vector3& other) const {
		return Vector3{ x * other.x, y * other.y, z * other.z };
	}

	// * 演算子のオーバーロード (スカラーとベクトル) 
	Vector3 operator*(float other) const {
		return Vector3{ x * other, y * other, z * other};;
	} 
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


	//Vector3 SafeVector3() {
	//	const float epsilon = 1e-6f; // 非常に小さな値
	//	return {
	//		x != 0.0f ? x : epsilon,
	//		y != 0.0f ? y : epsilon,
	//		z != 0.0f ? z : epsilon
	//	};
	//}
};

static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
	Vector3 temp;
	t = std::clamp(t, 0.0f, 1.0f);
	temp.x = a.x * (1.0f - t) + b.x * t;
	temp.y = a.y * (1.0f - t) + b.y * t;
	temp.z = a.z * (1.0f - t) + b.z * t;

	return temp;
}



