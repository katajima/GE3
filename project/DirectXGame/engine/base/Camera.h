#pragma once
#include "DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/math/MathFanctions.h"


class Object3d;

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4x4 view;       // ワールド → ビュー変換行列
	Matrix4x4 projection; // ビュー → プロジェクション変換行列
	Vector3 cameraPos;    // カメラ座標（ワールド座標）
};

// かめら
class Camera
{
public: // メンバ関数
	static Camera* GetInstance();	
	Camera();



	// 更新
	void UpdateMatrix();
	void UpdateMatrix(const Vector3& targetPosition);

	void TransferMatrix();

	//Matrix4x4 LookAt(const Vector3& cameraPosition, const Vector3& targetPosition, const Vector3& upVector);
	void LookAt(const Vector3& cameraPosition, const Vector3& targetPosition, const Vector3& upVector);
	// setter
	//void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	//void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	void SetFovY(const float fovY) { fovY_ = fovY; }
	void SetAspectRatio(const float aspect) { aspect_ = aspect; }
	void SetNearClip(const float nearC) { nearClip_ = nearC; }
	void SetFarClip(const float farC) { farClip_ = farC; }
	// getter
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
	const Vector3& GetRotate() const { return transform_.rotate; }
	const Vector3& GetTranslate() const { return transform_.translate; }


public:
	
	float fovY_ = 0.45f;
	float aspect_ = float(1280) / float(720);
	float nearClip_ = 0.1f;
	float farClip_ = 100.0f;
public:
	Transform transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
};


