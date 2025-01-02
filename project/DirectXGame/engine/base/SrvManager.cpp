#include "SrvManager.h"

const uint32_t SrvManager::kMaxSRVCount = 512;

SrvManager* SrvManager::instance = nullptr;

SrvManager* SrvManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new SrvManager;
	}
	return instance;
}

void SrvManager::Initialize(DirectXCommon* dxCommon)
{
	// 引数で受け取ってメンバ変数に記録する
	this->directXCommon_ = dxCommon;

	// デスクリプタヒープ
	descriptorHeap = directXCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
	// デスクリプタ一個分のサイズを取得して記録
	descriptorSize = directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void SrvManager::Finalize()
{
	descriptorHeap.Reset();
	delete instance;
	instance = nullptr;
}

uint32_t SrvManager::Allocate()
{
	assert(kMaxSRVCount > useIndex);

	// return する番号を一旦記録
	int index = useIndex;
	// 次回のために番号を1進める
	useIndex++;
	// 上で記録した番号をreturn
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = MipLevels;
	
	directXCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride)
{
	// SRVの記述を設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER; // ビューの次元をバッファに設定
	srvDesc.Format = DXGI_FORMAT_UNKNOWN; // 構造化バッファの場合、フォーマットは未知
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // デフォルトのコンポーネントマッピング
	srvDesc.Buffer.FirstElement = 0; // 最初の要素のインデックス
	srvDesc.Buffer.NumElements = numElements; // バッファ内の要素数
	srvDesc.Buffer.StructureByteStride = structureByteStride; // 各要素のバイト単位のサイズ
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE; // フラグなし

	// デバイスの取得とSRVの作成
	directXCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::PreDraw()
{
	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap.Get() };
	directXCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);

}

void SrvManager::SetGraphicsRootdescriptorTable(UINT RootParameterIndex, uint32_t srvIndex)
{
	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}

bool SrvManager::IsMaxTextuer()
{
	if (kMaxSRVCount > useIndex) {
		return true;
	}
	return false;
}
