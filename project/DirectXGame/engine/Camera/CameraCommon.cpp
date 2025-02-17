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

void CameraCommon::SetRootParameter(D3D12_ROOT_PARAMETER& parameter, int ShaderRegister)
{
	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	parameter.Descriptor.ShaderRegister = ShaderRegister;
}


