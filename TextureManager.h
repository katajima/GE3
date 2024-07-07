#pragma once
#include<cstdint>
#include<string>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
using namespace Microsoft::WRL;
#include<vector>
#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"
#include"DirectXCommon.h"
// テクスチャマネージャー
class TextureManager {
public:
	// シングルトンインスタンス
	static TextureManager* GetInstance();
	
	// 初期化
	void Initialize();
	
	// 終了
	void Finalize();

	//DirectTexを使ってTextureを読むためのLoadTextur関数
	void LoadTexture(const std::string& filePath);

	// テクスチャ番号取得
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	// テクスチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);


private:
	static TextureManager* instance;
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

	//テクスチャ一枚分のデータ
	struct TextureData {
		std::string filePath;
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	//テクスチャデータ
	std::vector<TextureData> textureDatas;
	DirectXCommon* dxCommon;
	static uint32_t kSRVIndexTop;

};