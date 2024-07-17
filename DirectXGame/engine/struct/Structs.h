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
};

struct Vector3 final {
	float x, y, z;
};

struct Vector2 final {
	float x, y;
};
//Transform
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};