#pragma once
#include "Vector3.h"

//行列
struct Matrix4x4
{
public:
	float m[4][4];

	static Matrix4x4 Identity() {
		return Matrix4x4{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	}
	// 行列のポインタを取得する関数
	const float* GetMatrixPointer() const {
		return &m[0][0];  // 行列データの最初の要素のポインタを返す
	}

	Matrix4x4 operator*(const Matrix4x4& mat);


	Matrix4x4 MakeRotateAxisAngle(const Vector3& axis,float angle) {

	};

private:
	
};


//行列の積
//static Matrix4x4 Multiply(const Matrix4x4& v1, const Matrix4x4& v2) {
//	Matrix4x4 result{};
//
//	for (int x = 0; x < 4; x++) {
//		for (int y = 0; y < 4; y++) {
//			for (int z = 0; z < 4; z++) {
//				result.m[y][x] += v1.m[y][z] * v2.m[z][x];
//			}
//		}
//	}
//
//	return result;
//};