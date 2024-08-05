#pragma once
#include "DirectXCommon.h"

// SrvManager
class SrvManager
{
public:
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	//確保関数
	uint32_t Allocate();
	// 最大確保関数
	bool AllocateCheck();

	// ディスクプリタハンドル計算
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	// SRV生成(テクスチャ用)
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	// SRV生成(Structred　Buffer用)
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	// ヒープセットコマンド
	void PreDraw();

	// SRVセットコマンド
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

private:
	DirectXCommon* directXCommon = nullptr;

	// 最大SRV数(最大テクスチャ枚数)
	static const uint32_t kMaxSRVCount;
	// SRV用のディスクプリタヒープ
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> descriptorHeap;
	// SRV用のディスクプリタサイズ
	uint32_t descriptorSize;

	// 次に確保するSRVインデックス
	uint32_t useIndex = 0;

};

