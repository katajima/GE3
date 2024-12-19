#pragma once

//行列
struct Matrix4x4
{
	float m[4][4];
	// 行列のポインタを取得する関数
	const float* GetMatrixPointer() const {
		return &m[0][0];  // 行列データの最初の要素のポインタを返す
	}
};
