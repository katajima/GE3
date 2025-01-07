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
	Vector2 operator+(float other) const {
		return Vector2{ x + other, y + other };
	}

	// - 演算子のオーバーロード
	Vector2 operator-(const Vector2& other) const {
		return Vector2{ x - other.x, y - other.y};
	}
	// - 演算子のオーバーロード
	Vector2 operator-(float other) const {
		return Vector2{ x - other, y - other};
	}

	// * 演算子のオーバーロード
	Vector2 operator*(const Vector2& other) const {
		return Vector2{ x * other.x, y * other.y};
	}
	// * 演算子のオーバーロード
	Vector2 operator*(float other) const {
		return Vector2{ x * other, y * other};
	}


	// 長さ
	float Length() {
		float result;

		result = sqrtf((x * x) + (y * y));

		return result;
	};
};


