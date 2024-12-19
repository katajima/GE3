#pragma once
#include <cmath>

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
	Vector3 Normalize() {
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

};
