#pragma once
#include "DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/math/MathFanctions.h"

// かめら
class Camera
{
public: // メンバ関数
	Camera();

	// 更新
	void Update();

	// setter
	//void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	//void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	void SetFovY(const float fovY) { fovY_ = fovY; }
	void SetAspectRatio(const float aspect) { aspect_ = aspect; }
	void SetNearClip(const float nearC) { nearClip_ = nearC; }
	void SetFarClip(const float farC) { farClip_ = farC; }
	// getter
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }
	const Vector3& GetRotate() const { return transform.rotate; }
	const Vector3& GetTranslate() const { return transform.translate; }


private:
	//Transform transform;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewProjectionMatrix;
	float fovY_ = 0.45f;
	float aspect_ = float(1280) / float(720);
	float nearClip_ = 0.1f;
	float farClip_ = 100.0f;
public:
	Transform transform;
	//Transform transform;
	Matrix4x4 worldMatrix;
};


