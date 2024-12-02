#pragma once

#include"DirectXGame/engine/base/DirectXCommon.h"

// SRV管理
class SrvManager {
public:
	// シングルトンインスタンス
	static SrvManager* GetInstance();

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	uint32_t Allocate();

	// デスクリプタハンドル計算
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	// SRV生成関数(テクスチャ用)
	// SRV生成(テクスチャ用)
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	// SRV生成(Structured Buffer用)
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	void PreDraw();

	// SRVセットコマンド
	void SetGraphicsRootdescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	bool IsMaxTextuer();

public:
	// 最大SRV数(最大テクスチャ枚数)
	static const uint32_t kMaxSRVCount;
private:
	static SrvManager* instance;

	DirectXCommon* directXCommon_ = nullptr;


	// SRV用のデスクリプタサイズ
	uint32_t descriptorSize;
	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	// 次に使用するSRVインデックス
	uint32_t useIndex = 0;

};
