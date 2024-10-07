#pragma once

//行列
struct Matrix4x4
{
	float m[4][4];
};
//行列
struct Matrix3x3
{
	float m[3][3];
};

struct Vector4 final {
	float x;
	float y;
	float z;
	float w;

	// == 演算子のオーバーロード
	bool operator==(const Vector4& other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}
};

struct Vector3 final {
	float x, y, z;

	// == 演算子のオーバーロード
	bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}
};

struct Vector2 final {
	float x, y;

	// == 演算子のオーバーロード
	bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}
};
//Transform
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

//AABB
struct AABB {
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};

