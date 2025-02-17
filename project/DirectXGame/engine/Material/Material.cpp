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
	color = { 1,1,1,1 };

	shininess_ = 20.0f;
	enableLighting_ = true;
	useNormalMap_ = false;
	useSpeculerMap_ = false;

	// GPUデータ
	GPUData();
}

void Material::GetCommandListMaterial(int index)
{
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_->GetGPUVirtualAddress());
}

void Material::GetCommandListTexture(int indexDiffuse, int normalIndex, int speculerIndex)
{
	// テクスチャのバインド
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(indexDiffuse, TextureManager::GetInstance()->GetSrvHandleGPU(tex_.diffuseFilePath));
	if (useNormalMap_) {
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(normalIndex, TextureManager::GetInstance()->GetSrvHandleGPU(tex_.normalFilePath));
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(9, TextureManager::GetInstance()->GetSrvHandleGPU(tex_.normalFilePath));
	}
	if (useSpeculerMap_) {
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(speculerIndex, TextureManager::GetInstance()->GetSrvHandleGPU(tex_.speculerFilePath));
	}
}

void Material::SetRootParameter(D3D12_ROOT_PARAMETER& parameter, int ShaderRegister)
{
	parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // CBVを使う　// b0のbと一致する
	parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	parameter.Descriptor.ShaderRegister = ShaderRegister;    // レジスタ番号0とバインド　　// b0の0と一致する。もしb11と紐づけたいなら11となる
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

	data_->color = color;
	
	data_->uvTransform = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
}

void Material::LoadTex()
{
	
	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(tex_.diffuseFilePath);
	// 読み込んだテクスチャの番号を取得
	tex_.diffuseIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(tex_.diffuseFilePath);

	if (tex_.normalFilePath == "") {
		useNormalMap_ = false;
	}
	else {
		useNormalMap_ = true;
	}

	if (useNormalMap_) {
		TextureManager::GetInstance()->LoadTexture(tex_.normalFilePath);

		tex_.normalIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(tex_.normalFilePath);
	}


	if (tex_.speculerFilePath == "") {
		useSpeculerMap_ = false;
	}
	else {
		useSpeculerMap_ = true;
	}
	if (useSpeculerMap_) {

		TextureManager::GetInstance()->LoadTexture(tex_.speculerFilePath);

		tex_.speculerIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(tex_.speculerFilePath);
	}

}
