#pragma once

struct Vector4 final {
	float x;
	float y;
	float z;
	float w;

	// == 演算子のオーバーロード
	bool operator==(const Vector4& other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	// + 演算子のオーバーロード
	Vector4 operator+(const Vector4& other) const {
		return Vector4{ x + other.x, y + other.y, z + other.z,w + other.w };
	}

	// - 演算子のオーバーロード
	Vector4 operator-(const Vector4& other) const {
		return Vector4{ x - other.x, y - other.y, z - other.z,w - other.w };
	}

	// * 演算子のオーバーロード
	Vector4 operator*(const Vector4& other) const {
		return Vector4{ x * other.x, y * other.y, z * other.z,w * other.w };
	}
};
