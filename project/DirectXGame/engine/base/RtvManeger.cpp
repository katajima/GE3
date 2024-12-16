#include "RtvManeger.h"
#include "assert.h"

const uint32_t RtvManager::kMaxRTVCount = 10;

RtvManager* RtvManager::instance = nullptr; 
RtvManager* RtvManager::GetInstance() { 
	if (!instance) { 
		instance = new RtvManager();
	} 
	return instance; 
}

void RtvManager::Initialize(DirectXCommon* dxCommon)
{
	// 引数で受け取ってメンバ変数に記録する
	this->directXCommon_ = dxCommon;

	// デスクリプタヒープ
	descriptorHeap = directXCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxRTVCount, true);
	// デスクリプタ一個分のサイズを取得して記録
	descriptorSize = directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void RtvManager::Finalize()
{
	descriptorHeap.Reset();
	delete instance; 
	instance = nullptr;
}

uint32_t RtvManager::Allocate()
{
	assert(kMaxRTVCount > useIndex);

	// return する番号を一旦記録
	int index = useIndex;
	// 次回のために番号を1進める
	useIndex++;
	// 上で記録した番号をreturn
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE RtvManager::GetCPUDescriptorHandle(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart(); 
	handleCPU.ptr += (descriptorSize * index); 
	return handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE RtvManager::GetGPUDescriptorHandle(uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}


void RtvManager::CreateRTV(uint32_t rtvIndex, ID3D12Resource* pResource) { 
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {}; 
	rtvDesc.Format = pResource->GetDesc().Format; 
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; 
	rtvDesc.Texture2D.MipSlice = 0; 
	directXCommon_->GetDevice()->CreateRenderTargetView(pResource, &rtvDesc, GetCPUDescriptorHandle(rtvIndex)); 
}
