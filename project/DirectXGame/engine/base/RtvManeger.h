#pragma once
#include"DirectXGame/engine/base/DirectXCommon.h"


// RTV管理
class RtvManager {
public:
    // シングルトンインスタンス
    static RtvManager* GetInstance();

    // 初期化
    void Initialize(DirectXCommon* dxCommon);

    void Finalize();

    uint32_t Allocate();

    // デスクリプタハンドル計算
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

    // RTV生成関数
    void CreateRTV(uint32_t rtvIndex, ID3D12Resource* pResource);

    // 必要に応じて他のRTV関連の関数を追加
    // ...

public:
    // 最大RTV数
    static const uint32_t kMaxRTVCount;

private:
    static RtvManager* instance;

    DirectXCommon* directXCommon_ = nullptr;

    // RTV用のデスクリプタサイズ
    uint32_t descriptorSize;
    // RTV用デスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;

    // 次に使用するRTVインデックス
    uint32_t useIndex = 0;
};

