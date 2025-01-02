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


	Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {

	};

	Vector3 GetWorldPosition() const {
		// ワールド座標を入れる
		Vector3 worldPos{};
		worldPos.x = m[3][0];
		worldPos.y = m[3][1];
		worldPos.z = m[3][2];
		return worldPos;
	};



private:

};


