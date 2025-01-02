#pragma once
#include "DirectXGame/engine/base/DirectXCommon.h"
#include "DirectXGame/engine/Camera/Camera.h"

class SkinningConmmon
{
public:
	static SkinningConmmon* instance;

	static SkinningConmmon* GetInstance();


	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	void DrawCommonSetting();

	void SetDefaltCamera(Camera* camera) { this->defaultCamera = camera; }

	Camera* GetDefaltCamera() const {
		defaultCamera;
		return defaultCamera;
	}

private:
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの作成
	void CreateGraphicsPipeline();
private:// メンバ変数
	DirectXCommon* dxCommon_;

	Camera* defaultCamera = nullptr;

	//ルートシグネチャデスク
	D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};
	//ルートシグネチャ
	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState = nullptr;

};

