#pragma once
#include"DirectXGame/engine/base/DirectXCommon.h"

// 3Dモデル共通部分
class CameraCommon
{
public:
	static CameraCommon* instance;
	// インスタンス
	static CameraCommon* GetInstance();
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	static void SetRootParameter(D3D12_ROOT_PARAMETER& parameter, int ShaderRegister);

private:
	DirectXCommon* dxCommon_;

};