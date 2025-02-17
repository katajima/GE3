#pragma once
#include "DirectXGame/engine/base/DirectXCommon.h"
#include "DirectXGame/engine/Camera/Camera.h"

class Object3dCommon
{
public:
	enum class PSOType {

		UvInterpolation_MODE_SOLID_BACK,
		NoUvInterpolation_MODE_SOLID_BACK,
		UvInterpolation_MODE_WIREFRAME_BACK,
		NoUvInterpolation_MODE_WIREFRAME_BACK,
		
		UvInterpolation_MODE_SOLID_NONE,
		NoUvInterpolation_MODE_SOLID_NONE,
		UvInterpolation_MODE_WIREFRAME_NONE,
		NoUvInterpolation_MODE_WIREFRAME_NONE,
	};

	static Object3dCommon* instance;

	static Object3dCommon* GetInstance();


	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	void DrawCommonSetting(PSOType type);

	void SetDefaltCamera(Camera* camera) { this->defaultCamera = camera; }

	Camera* GetDefaltCamera() const { 
		defaultCamera;
		return defaultCamera; }

private:
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの作成
	void CreateGraphicsPipeline();

	void Blob(DirectXCommon* dxCommon  ,D3D12_ROOT_SIGNATURE_DESC descriptionSignature, Microsoft::WRL::ComPtr < ID3D12RootSignature>& rootSignature);
	Microsoft::WRL::ComPtr < ID3D12RootSignature> Blob2(DirectXCommon* dxCommon  ,D3D12_ROOT_SIGNATURE_DESC descriptionSignature, Microsoft::WRL::ComPtr < ID3D12RootSignature>& rootSignature);

	void GraphicsPipelineState(Microsoft::WRL::ComPtr < ID3D12RootSignature>& rootSignature, Microsoft::WRL::ComPtr < ID3D12PipelineState>& graphicsPipelineState
		, D3D12_RASTERIZER_DESC rasterizerDesc, D3D12_BLEND_DESC blendDesc);

private:// メンバ変数
	DirectXCommon* dxCommon_;

	Camera* defaultCamera = nullptr;

	//ルートシグネチャデスク
	D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};
	//ルートシグネチャ
	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature[2];
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState[8];


	
	
};

