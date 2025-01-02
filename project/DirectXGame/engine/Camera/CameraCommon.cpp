#include "CameraCommon.h"

CameraCommon* CameraCommon::instance = nullptr;

CameraCommon* CameraCommon::GetInstance()
{
	if (instance == nullptr) {
		instance = new CameraCommon;
	}
	return instance;
}

void CameraCommon::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;
}

void CameraCommon::Finalize()
{
	delete instance;
	instance = nullptr;
}


