#pragma once
#include <windows.h>
#include<cstdint>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
#include<wrl.h>
#include <array>
#include"Logger.h"
#include"StringUtility.h"
#include"WinApp.h"
#include<chrono>
#include <thread>
#include<vector>

#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"

class DirectXCommon
{
public: // メンバ関数
	// シングルトンインスタンス
	//static DirectXCommon* GetInstance();
	//static DirectXCommon* instance;
	// CPUHandle
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);

	// GPUHandle
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	// CPUHandle
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);

	// GPUHandle
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);

	// CPUHandle
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);

	// GPUHandle
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

	// 初期化
	void Intialize(WinApp* winApp);

	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();

	//終了処理
	void Finalize();

private:
	/// <summary>
	/// DXGIデバイス初期化
	/// </summary>
	void InitializeDXGIDevice();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// コマンド関連初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// レンダーターゲット生成
	/// </summary>
	void CreateRenderTargets();

	///
	///
	/// 
	void CreateDescriptorHeap();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// フェンス生成
	/// </summary>
	void CreateFence();

	/// <summary>
	/// DepthStencilView(DSV)初期化
	/// </summary>
	void InitializeDepthStencilView();

	/// <summary>
	/// Viewportの初期化
	/// </summary>
	void InitializeViewport();

	/// <summary>
	/// Scissorの初期化
	/// </summary>
	void InitializeScissor();

	/// <summary>
	/// DXCCompilerを初期化
	/// </summary>
	void CreateDXCCompiler();


	void InitializeImGui();

	//FPS固定初期化
	void InitializeFixFPS();
	//FPS固定更新
	void UpdateFixFPS();

public:

	// DescriptorHeapの作成関数
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	// Material用のResource作成関数
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	Microsoft::WRL::ComPtr <ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);


	//データを転送するUploadTextureData関数を作る
	[[nodiscard]]
	Microsoft::WRL::ComPtr < ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr < ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device; }

	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> GetCommandList() { return commandList; }

	//Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> GetSrvDescriptorHeap() { return srvDescriptorHeap; }

	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> GetRtvDescriptorHeap() { return rtvDescriptorHeap; }

	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> GetDsvDescriptorHeap() { return dsvDescriptorHeap; }
private:
	// WindowsAPI
	WinApp* winApp_ = nullptr;
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr < ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	Microsoft::WRL::ComPtr < ID3D12Resource> depthStencilResource;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvDescriptorHeap;
	//Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvDescriptorHeap;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvDescriptorHeap;
	Microsoft::WRL::ComPtr < ID3D12Fence> fence;
	HANDLE fenceEvent;
	uint64_t fenceValue;
	IDxcUtils* dxcUtils;
	IDxcIncludeHandler* includeHandler;
	IDxcCompiler3* dxcCompiler;
	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorRect;
	D3D12_RESOURCE_BARRIER barrier;
	//uint32_t desriptorSizeSRV;
	uint32_t desriptorSizeRTV;
	uint32_t desriptorSizeDSV;

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	
private:
	// CPUHandle
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	// GPUHandle
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);


public:
	//static const uint32_t kMaxSRVCount;

	
	////------CompileShader------////
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
		//CompileShaderするShaderファイルへのパス
		const std::wstring& filePach,
		//CompileShaderに使用するProfile
		const wchar_t* profile);

};

