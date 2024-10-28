#pragma once
#include <array>
#include <cstdint>
#include <d3d12.h>
#include <memory>
#include <string>
#include <wrl.h>
#include "DirectXGame/engine/struct/Structs.h"
#include "DirectXGame/engine/base/Camera.h"
#include "DirectXGame/engine/base/DirectXCommon.h"

class LineCommon
{
public:

public:
	static LineCommon* GetInstance();
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	void DrawCommonSetting();

	void SetDefaltCamera(Camera* camera) { this->defaultCamera = camera; }

	Camera* GetDefaltCamera() const { return defaultCamera; }

private:
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの作成
	void CreateGraphicsPipeline();


private:
	Camera* defaultCamera = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	DirectXCommon* dxCommon_;
};

