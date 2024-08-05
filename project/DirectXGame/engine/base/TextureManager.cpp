#include "TextureManager.h"
#include"StringUtility.h"

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvMane)
{
	dxCommon_ = dxCommon;
	srvManager = srvMane;
	// textureDatas.reserve(DirectXCommon::kMaxSRVCount);

}

//ImGuiで0番目を使用するため,1番から使用
uint32_t TextureManager::kSRVIndexTop = 1;



void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void TextureManager::LoadTexture(const std::string& filePath) {
	// 読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {
		return;
	}
	// テクスチャ枚数上限チェック
	//assert(textureDatas.size() + kSRVIndexTop < DirectXCommon::kMaxSRVCount);
	assert(srvManager->AllocateCheck());

	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	// filePathWが正しいか確認
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップ作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));



	// テクスチャデータを追加
	//textureDatas.resize(textureDatas.size() + 1);
	// 追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas[filePath];
	textureData.srvIndex = srvManager->Allocate();
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxCommon_->CreateTextureResource(textureData.metadata);

	textureData.intermediateResource = dxCommon_->UploadTextureData(textureData.resource.Get(), mipImages);


	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;
	// SRVを作成するDescriptorHeapの場所を決める
	textureData.srvHandleCPU = srvManager->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager->GetGPUDescriptorHandle(textureData.srvIndex);

	srvManager->CreateSRVforTexture2D(srvIndex, textureData.resource.Get(), textureData.metadata.format, UINT(textureData.metadata.mipLevels));

	textureDatas[filePath] = textureData;
	
	//// metadataを基にSRVの設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//srvDesc.Format = textureData.metadata.format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels);
	//// SRV
	//dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}


uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	auto it = textureDatas.find(filePath);
	if (it != textureDatas.end()) {
		// 読み込み済みなら要素番号を返す
		return static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
	}
	assert(false && "Texture not found");
	return 0; // エラー時の戻り値
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath) {
	auto it = textureDatas.find(filePath);
	if (it != textureDatas.end()) {
		return it->second.srvHandleGPU;
	}

	assert(false && "Texture not found");
	return D3D12_GPU_DESCRIPTOR_HANDLE(); // デフォルトハンドルを返す
}

const DirectX::TexMetadata& TextureManager::GetMetadata(const std::string& filePath) {
	auto it = textureDatas.find(filePath);
	if (it != textureDatas.end()) {
		return it->second.metadata;
	}

	assert(false && "Texture not found");
	throw std::runtime_error("Texture not found");
}

uint32_t TextureManager::GetSrvIndex(const std::string& filePath) {
	auto it = textureDatas.find(filePath);
	if (it != textureDatas.end()) {
		return static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
	}

	assert(false && "Texture not found");
	return 0; // エラー時の戻り値
}
