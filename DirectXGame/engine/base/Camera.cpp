#include "Camera.h"
#include "DirectXGame//engine/base/WinApp.h"

Camera::Camera()

	:transform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} })
	, fovY_(0.45f)
	, aspect_(float(1280) / float(720))
	, nearClip_(0.1f)
	, farClip_(100.0f)
	, worldMatrix(MakeAffineMatrixMatrix(transform.scale, transform.rotate, transform.translate))
	, viewMatrix(Inverse(worldMatrix))
	, projectionMatrix(MakePerspectiveFovMatrix(fovY_, aspect_, nearClip_, farClip_))
	, viewProjectionMatrix(Multiply(viewMatrix, projectionMatrix))
{}

void Camera::Update()
{
	//transform = 
	worldMatrix = MakeAffineMatrixMatrix(transform.scale, transform.rotate, transform.translate);
	viewMatrix = Inverse(worldMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(fovY_, aspect_, nearClip_, farClip_);
	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
}
