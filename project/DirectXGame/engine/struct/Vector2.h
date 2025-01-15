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



	// 正規化
	Vector2 Normalize() const {
		Vector2 result{};
		float length;

		length = sqrtf((x * x) + (y * y));

		if (length != 0.0f) {
			result.x = x / length;
			result.y = y / length;
			
		};

		return result;
	};
	// 長さ
	float Length() {
		float result;

		result = sqrtf((x * x) + (y * y));

		return result;
	};

	// 内積
	float Dot(const Vector2& other) const {
		float result;

		result = x * other.x + y * other.y ;


		return result;
	};

};

static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
	Vector2 temp;
	t = std::clamp(t, 0.0f, 1.0f);
	temp.x = a.x * (1.0f - t) + b.x * t;
	temp.y = a.y * (1.0f - t) + b.y * t;
	
	return temp;
}
static Vector2 Max(const Vector2& v1, const Vector2& v2) {
	Vector2 result{};


	result.x = (std::max)(v1.x, v2.x);
	result.y = (std::max)(v1.y, v2.y);
	
	return result;
}

static Vector2 Min(const Vector2& v1, const Vector2& v2) {
	Vector2 result{};


	result.x = (std::min)(v1.x, v2.x);
	result.y = (std::min)(v1.y, v2.y);
	
	return result;
}


