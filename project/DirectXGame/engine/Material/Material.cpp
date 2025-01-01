#include "Material.h"
#include"DirectXGame/engine/base/TextureManager.h"

void Material::Initialize(DirectXCommon* dxcommon)
{
	dxCommon_ = dxcommon;


	// マテリアル
	resource_ = dxCommon_->CreateBufferResource(sizeof(Material::DataGPU));
	// 書き込むためのアドレスを取得
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));


	transform.scale = { 1.0f,1.0f,1.0f };
	transform.translate = { 0,0,0 };
	transform.rotate = { 0,0,0 };

	shininess_ = 20.0f;
	enableLighting_ = true;
	useNormalMap_ = false;
	useSpeculerMap_ = false;

	// GPUデータ
	GPUData();
}

void Material::GPUData()
{

	data_->enableLighting = enableLighting_;
	data_->shininess = shininess_;
	data_->useLig = useLig_;

	if (useNormalMap_) {
		data_->useNormalMap = useNormalMap_;
	}
	if (useSpeculerMap_) {
		data_->useSpeculerMap = useSpeculerMap_;
	}

	
	data_->uvTransform = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
}

void Material::LoadTex()
{
	//// .objの参照しているテクスチャファイル読み込み
	//TextureManager::GetInstance()->LoadTexture(modelData.material.textuerFilePath);
	//// 読み込んだテクスチャの番号を取得
	//modelData.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textuerFilePath);

	//if (modelData.material.textuerNormalFilePath == "") {
	//	useNormalMap = false;
	//}
	//else {
	//	useNormalMap = true;
	//}

	//if (useNormalMap) {
	//	TextureManager::GetInstance()->LoadTexture(modelData.material.textuerNormalFilePath);

	//	modelData.material.textureNormalIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textuerNormalFilePath);
	//}


	//if (modelData.material.textuerSpeculerFilePath == "") {
	//	useSpecularMap = false;
	//}
	//else {
	//	useSpecularMap = true;
	//}
	//if (useSpecularMap) {

	//	TextureManager::GetInstance()->LoadTexture(modelData.material.textuerSpeculerFilePath);

	//	modelData.material.textuerSpeculerIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textuerSpeculerFilePath);
	//}

}
