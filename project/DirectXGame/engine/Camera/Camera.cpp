#include "Camera.h"
#include "DirectXGame/engine/base/WinApp.h"
#include <DirectXGame/engine/3d/Object3d.h>
#include "DirectXGame/engine/MyGame/MyGame.h"

bool Camera::isShake_ = false;

Camera& Camera::GetInstance()
{
	static Camera instance;
	return instance;
}
Camera::Camera()

	:transform_({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} })
	, fovY_(0.45f)
	, aspect_(float(1280) / float(720))
	, nearClip_(0.1f)
	, farClip_(1000.0f)
	, worldMatrix_(MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate))
	, viewMatrix_(Inverse(worldMatrix_))
	, projectionMatrix_(MakePerspectiveFovMatrix(fovY_, aspect_, nearClip_, farClip_))
	, viewProjectionMatrix_(Multiply(viewMatrix_, projectionMatrix_))

{



}

void Camera::Initialize()
{
	dxCommon_ = CameraCommon::GetInstance()->GetDxCommon();

	resource = dxCommon_->CreateBufferResource(sizeof(DataGPU));
	//書き込むためのアドレスを取得
	resource->Map(0, nullptr, reinterpret_cast<void**>(&data));

	data->worldPosition = Vector3{ 1.0f,1.0f,1.0f };
	data->normal = { 0,0,0 };
}

void Camera::GetCommandList(int index)
{
	// Cameraのバインド
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(index, resource->GetGPUVirtualAddress());
}


void Camera::UpdateMatrix() {

#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat("debugShakeTime", &debugShakeTime_, 0.01f);
	ImGui::DragFloat3("debugShakeDirectionRange", &debugShakeDirectionRange_.x, 0.1f);
	ImGui::End();

	if (Input::GetInstance()->IsGamePadTriggered(GamePadButton::GAMEPAD_Up)) {
		SetShake(debugShakeTime_, debugShakeDirectionRange_);
	}
#endif // _DEBUG



	if (shakeTime_ > 0) {
		// Reduce shake time
		shakeTime_ -= MyGame::GameTime();

		// Generate random shake offset within the direction range
		float xOffset = static_cast<float>(rand()) / RAND_MAX * shakeDirectionRange_.x;
		float yOffset = static_cast<float>(rand()) / RAND_MAX * shakeDirectionRange_.y;
		float zOffset = static_cast<float>(rand()) / RAND_MAX * shakeDirectionRange_.z;

		// Create shake offset vector
		Vector3 shakeOffset(xOffset, yOffset, zOffset);

		Matrix4x4 cameraWorldMatrix = Inverse(viewMatrix_);

		// カメラの向きに基づいて移動方向をワールド座標系に変換
		Vector3 worldDirection = {
	   shakeOffset.x * cameraWorldMatrix.m[0][0] + shakeOffset.y * cameraWorldMatrix.m[1][0] + shakeOffset.z * cameraWorldMatrix.m[2][0],
	   shakeOffset.x * cameraWorldMatrix.m[0][1] + shakeOffset.y * cameraWorldMatrix.m[1][1] + shakeOffset.z * cameraWorldMatrix.m[2][1],
	   shakeOffset.x * cameraWorldMatrix.m[0][2] + shakeOffset.y * cameraWorldMatrix.m[1][2] + shakeOffset.z * cameraWorldMatrix.m[2][2]
		};

		// Apply shake offset to the camera's position
		transform_.translate += worldDirection;
	}

	// カメラのワールド行列を計算
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	// ビュー行列をカメラのワールド行列の逆行列として計算
	viewMatrix_ = Inverse(worldMatrix_);

	// 射影行列を計算
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspect_, nearClip_, farClip_);

	// ビュー・プロジェクション行列を更新
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);


	// カメラデータの更新
	Vector3 cameraFront(viewMatrix_.m[0][2], viewMatrix_.m[1][2], viewMatrix_.m[2][2]);
	data->normal = Normalize(cameraFront); // 必要なら正規化
	data->worldPosition = transform_.translate;
}


void Camera::UpdateMatrix(const Vector3& targetPosition)
{
	// カメラとターゲットの距離を設定
	float distanceFromTarget = 2.0f; // 適切な距離に調整
	Vector3 directionToTarget = Normalize(Subtract(targetPosition, transform_.translate));

	// カメラの位置をターゲットから一定距離後ろに配置
	transform_.translate = Subtract(targetPosition, Multiply(directionToTarget, distanceFromTarget));

	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);




	// 射影行列の再計算
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspect_, nearClip_, farClip_);

	// ビュー・プロジェクション行列を更新
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

void Camera::TransferMatrix()
{
	// ビュー行列の逆行列を計算してカメラの位置を抽出
	Matrix4x4 iView = Inverse(worldMatrix_);
	transform_.translate = { iView.m[3][0], iView.m[3][1], iView.m[3][2] };

	// ビュー・プロジェクション行列を計算
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}


void Camera::LookAt(const Vector3& cameraPosition, const Vector3& targetPosition, const Vector3& upVector) {
	Vector3 forward = Normalize(Subtract(targetPosition, cameraPosition)); // 前方向ベクトル
	Vector3 right = Normalize(Cross(upVector, forward)); // 右方向ベクトル
	Vector3 up = Cross(forward, right); // 上方向ベクトル

	// カメラの回転行列を設定
	transform_.rotate.x = std::atan2(forward.y, std::sqrt(forward.x * forward.x + forward.z * forward.z));
	transform_.rotate.y = std::atan2(forward.x, forward.z);

	// ワールド行列の更新
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, cameraPosition);

	// ビュー行列を更新
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspect_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

void Camera::SetShake(float time, Vector3 directionRange) {
	// Initialize shake timer
	shakeTime_ = time;
	// Save the direction range for shaking
	shakeDirectionRange_ = directionRange;
}


