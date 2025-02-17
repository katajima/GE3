#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/3d/Model.h"

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
#include<vector>
#include<format>


class Transfomation
{
public:
	// 初期化
	void Initialize(DirectXCommon* dxCommon);
	// 更新
	void Update(Model* model, Camera* camera,Matrix4x4& local,Matrix4x4& mat);
	// 更新
	void Update(Camera* camera,Matrix4x4& mat);
	// 更新
	void UpdateSkinning(Model* model, Camera* camera,Matrix4x4& local,Matrix4x4& mat);
	// 更新
	void UpdateSprite(Matrix4x4& mat);

	// データ
	void GetCommandList(int index);

	static void SetRootParameter(D3D12_ROOT_PARAMETER& parameter, int ShaderRegister);

	struct TransfomationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 worldInverseTranspose;
	};
	TransfomationMatrix* data_;

private:
	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr < ID3D12Resource> resource_;

};

