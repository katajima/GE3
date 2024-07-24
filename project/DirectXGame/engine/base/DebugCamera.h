#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/base/WinApp.h"
#include"DirectXGame/engine/input/Input.h"
/// <summary>
/// デバッグカメラ
/// </summary>
class DebugCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input *input);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void UpdateMatrix();

	void InputKeyCamera();

private:
	// X,Y,Z回りのローカル回転角
	Vector3 rotation_ = { 0,0,0 };
	// ローカル座標
	Vector3 translation_ = { 0,0,-50 };
	// ビュー座標
	Matrix4x4 viewMatrix_;
	// 射影行列
	Matrix4x4 worldMatrix_;

	Input* input_;
};

