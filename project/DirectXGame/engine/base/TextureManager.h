#pragma once
#include <windows.h>
#include<cstdint>
#include<string>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
using namespace Microsoft::WRL;
#include<vector>
#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"
#include"DirectXCommon.h"
#include"SrvManager.h"
// テクスチャマネージャー
class TextureManager {
public:
	// シングルトンインスタンス
	static TextureManager* GetInstance();
	
	// 初期化
	void Initialize(DirectXCommon* dxCommon/*,SrvManager* srvManager*/);
	
	// 終了
	void Finalize();

	//DirectTexを使ってTextureを読むためのLoadTextur関数
	void LoadTexture(const std::string& filePath);
	//void LoadTextureStruct(const std::string& filePath);

	// テクスチャ番号取得
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	// テクスチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePach);

	// メタデータを取得
	const DirectX::TexMetadata& GetMataData(const std::string& filePach);

	SrvManager* GetSrvManager(){ return SrvManager::GetInstance(); }

	static void SetRootParameter(D3D12_ROOT_PARAMETER& parameter,D3D12_DESCRIPTOR_RANGE& descriptorRange);


private:
	static TextureManager* instance;
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

	//テクスチャ一枚分のデータ
	struct TextureData {
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
	};

	//テクスチャデータ
	//std::vector<TextureData> textureDatas;
	std::unordered_map<std::string, TextureData> textureDatas;

	DirectXCommon* dxCommon_ = nullptr;
	static uint32_t kSRVIndexTop;
	//Microsoft::WRL::ComPtr <ID3D12Resource> intermediateResource;
	
	SrvManager* srvManager_ = nullptr;
};