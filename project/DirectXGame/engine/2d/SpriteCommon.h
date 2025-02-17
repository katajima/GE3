#pragma once
#include"DirectXGame/engine/base/DirectXCommon.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
using namespace Microsoft::WRL;
class SpriteCommon
{
public:// メンバ関数
	enum class PSOType {

		UvInterpolation_MODE_SOLID,
		NoUvInterpolation_MODE_SOLID,
		UvInterpolation_MODE_WIREFRAME,
		NoUvInterpolation_MODE_WIREFRAME,

	};


	static SpriteCommon* instance;

	static SpriteCommon* GetInstance();

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	void DrawCommonSetting(PSOType type);
	
private:// メンバ関数
	// ルートシグネチャの作成
	void CreateRootSignature();

	// グラフィックスパイプラインの作成
	void CreateGraphicsPipeline();
	

	static void Blob(DirectXCommon* dxCommon,D3D12_ROOT_SIGNATURE_DESC descriptionSignature, Microsoft::WRL::ComPtr < ID3D12RootSignature>& rootSignature);

	void GraphicsPipelineState(Microsoft::WRL::ComPtr < ID3D12RootSignature>& rootSignature,Microsoft::WRL::ComPtr < ID3D12PipelineState>& graphicsPipelineState
		,D3D12_RASTERIZER_DESC rasterizerDesc, D3D12_BLEND_DESC blendDesc);

private:// メンバ変数
	DirectXCommon* dxCommon_;

	//ルートシグネチャデスク
	D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};
	//ルートシグネチャ
	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature[2];
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState[4];
	

	D3D12_RASTERIZER_DESC rasterizerDesc{};
};


