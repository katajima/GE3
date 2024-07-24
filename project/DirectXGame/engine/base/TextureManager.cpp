#include "TextureManager.h"
#include"StringUtility.h"
#include<unordered_map>

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

//ImGuiで0番目を使用するため,1番から使用
uint32_t TextureManager::kSRVIndexTop = 1;

void TextureManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
    dxCommon_ = dxCommon;
	textureDatas.reserve(DirectXCommon::kMaxSRVCount);
    srvManager_ = srvManager;
}

void TextureManager::Finalize()
{
    delete instance;
    instance = nullptr;
}

void TextureManager::LoadTexture(const std::string& filePath) {
    // 読み込み済みテクスチャを検索
    if (textureDatas.contains(filePath)) {

    }
    // テクスチャ枚数上限チェック
    assert(srvManager_->IsMaxTextuer());


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
    textureData.metadata = mipImages.GetMetadata();
    textureData.resource = dxCommon_->CreateTextureResource(textureData.metadata);
    
    textureData.intermediateResource = dxCommon_->UploadTextureData(textureData.resource.Get(), mipImages);


    //uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;
    textureData.srvIndex = srvManager_->Allocate();
    // SRVを作成するDescriptorHeapの場所を決める
    textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
    textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

    srvManager_->CreateSRVforTexture2D(textureData.srvIndex,textureData.resource.Get(),textureData.metadata.format,UINT(textureData.metadata.mipLevels));
   
}


uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	//読み込み済みテクスチャを検索
    if (textureDatas.contains(filePath)) {

    }
	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath)
{
    // 範囲外指定チェック
    auto it = textureDatas.find(filePath);
    if (it != textureDatas.end()) {
        return it->second.srvHandleGPU;
    }
    // エラー処理（例えば、ハンドルが見つからなかった場合）
    D3D12_GPU_DESCRIPTOR_HANDLE nullHandle = {};
    return nullHandle;
}

const DirectX::TexMetadata& TextureManager::GetMataData(const std::string& filePath)
{
    // 範囲外指定チェック
    auto it = textureDatas.find(filePath);
    assert(it != textureDatas.end() && "Texture file path not found");

    TextureData& textureData = it->second;
    return textureData.metadata;
}


