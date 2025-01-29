#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"


class WorldTransform
{
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();

public:
	Vector3 scale_ = { 1,1,1 };		// 拡縮
	Vector3 rotate_ = {0,0,0};		// 回転
	Vector3 translate_ = { 0,0,0 }; // 位置

	// マトリックス(現在)
	Matrix4x4 worldMat_;
	// マトリックス(過去)
	Matrix4x4 worldPreMat_;

	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;
};

