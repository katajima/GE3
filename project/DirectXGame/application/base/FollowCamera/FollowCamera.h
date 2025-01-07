#pragma once
#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/input/Input.h"


class FollowCamera
{
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	//追従対象をポインタで持つ
	void SetTarget(const Object3d* target) { target_ = target; };

	Camera& GetViewProjection() { return camera_; };


	//void SetCameraRotate(Vector3& rotate) { return camera_.transform_.rotate = rotate; }

private:

	// ビュープロジェクション
	Camera camera_;

	//追従対象
	const Object3d* target_ = nullptr;

};

