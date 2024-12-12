#include "FollowCamera.h"

void FollowCamera::Initialize()
{

	camera_.farClip_ = 5000.0f;
	camera_.transform_.rotate.x = DegreesToRadians(90);
}

void FollowCamera::Update()
{
	//追従対象がいれば
	if (target_) {
		camera_.transform_.rotate.x = DegreesToRadians(20);


		if (Input::GetInstance()->IsPushKey(DIK_LEFT)) {
			camera_.transform_.rotate.y -= 0.01f;
		}
		if (Input::GetInstance()->IsPushKey(DIK_RIGHT)) {
			camera_.transform_.rotate.y += 0.01f;
		}

		// 追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f, 10.0f, -20.0f };

		Matrix4x4 matrix = MakeRotateYMatrix(camera_.transform_.rotate.y);

		// オフセットをカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, matrix);

		// 座標をコピーしてオフセット分ずらす
		camera_.transform_.translate = Add(target_->transform.translate, offset);
	}
	//ビュー行列の更新
	camera_.UpdateMatrix();
}
