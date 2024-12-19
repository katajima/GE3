#pragma once


struct Vector2 final {
	float x, y;

	// == 演算子のオーバーロード
	bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}
	// + 演算子のオーバーロード
	Vector2 operator+(const Vector2& other) const {
		return Vector2{ x + other.x, y + other.y};
	}

	// - 演算子のオーバーロード
	Vector2 operator-(const Vector2& other) const {
		return Vector2{ x - other.x, y - other.y};
	}

	// * 演算子のオーバーロード
	Vector2 operator*(const Vector2& other) const {
		return Vector2{ x * other.x, y * other.y};
	}
};
