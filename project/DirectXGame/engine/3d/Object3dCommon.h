#pragma once
#include "DirectXGame/engine/base/DirectXCommon.h"
#include "DirectXGame/engine/base/Camera.h"

class Object3dCommon
{
public:
	static Object3dCommon* instance;

	static Object3dCommon* GetInstance();


	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	void DrawCommonSetting();

	void SetDefaltCamera(Camera* camera) { this->defaultCamera = camera; }

	Camera* GetDefaltCamera() const { return defaultCamera; }

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

