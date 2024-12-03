#include"Object3dCommon.h"
#include"Object3d.h"
#include<cstdint>
#include<string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include"DirectXGame/engine/base/TextureManager.h"
#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/math/MathFanctions.h"

#include <iostream>

#include"DirectXGame/engine/base/ImGuiManager.h"



void Object3d::Initialize()
{
	Object3dCommon::GetInstance();

	// 引数で受け取ってメンバ変数に記録する
	//this->object3dCommon_ = Object3dCommon::GetInstance();
	this->camera = Object3dCommon::GetInstance()->GetDefaltCamera();
	//トランスフォーム
	transformationMatrixResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(TransfomationMatrix));

	//書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transfomationMatrixData));

	//単位行列を書き込んでおく
	transfomationMatrixData->WVP = MakeIdentity4x4();
	transfomationMatrixData->World = MakeIdentity4x4();


	//平行光源用のリソースを作る
	directionalLightResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));
	directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	//今回は赤を書き込んで見る //白
	*directionalLightData = DirectionalLight({ 1.0f,1.0f,1.0f,1.0f }, { 0.0f,-1.0f,0.0f }, 1.0f);



	//transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
}

void Object3d::Update()
{
	//assert(model);
	mat_ = MakeAffineMatrixMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewMatrix = camera->GetViewMatrix();
		const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix();
		worldViewProjectionMatrix = Multiply(mat_, viewMatrix);
		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, projectionMatrix);
	
		
	}
	else {
		worldViewProjectionMatrix = mat_;
	}

	

	transfomationMatrixData->World = mat_;
	transfomationMatrixData->WVP = worldViewProjectionMatrix;

	/*if (parent_) {
		mat_ = Multiply(mat_, parent_->mat_);
	}*/

	
}

void Object3d::Draw()
{
	////------平行光源用------////
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	
	// 3Dモデルが割り当てれていれば描画する
	if (model) {
		model->Draw();
	}

}

void Object3d::InitializeInstance(size_t size)
{
	size_ = size;

	// Object3dCommon への参照を取得
	//this->object3dCommon_ = Object3dCommon::GetInstance();
	this->camera = Object3dCommon::GetInstance()->GetDefaltCamera();

	// transformationMatrixResource および transfomationMatrixData の配列を確保
	InstanseTransformationMatrixResource.resize(size_);
	InstanseTransfomationMatrixData.resize(size_);

	for (size_t i = 0; i < size_; i++) {
		// インスタンスごとのバッファを作成
		InstanseTransformationMatrixResource[i] = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(TransfomationMatrix));

		// バッファに書き込むためのアドレスを取得
		InstanseTransformationMatrixResource[i]->Map(0, nullptr, reinterpret_cast<void**>(&InstanseTransfomationMatrixData[i]));

		// 単位行列を書き込んでおく
		InstanseTransfomationMatrixData[i]->WVP = MakeIdentity4x4();
		InstanseTransfomationMatrixData[i]->World = MakeIdentity4x4();

		// 各インスタンスの transform を設定
		transforms.push_back({ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	}

	// 平行光源用のリソースを作成
	directionalLightResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));
	directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	// 平行光源データの初期化（白色光）
	*directionalLightData = DirectionalLight({ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, 1.0f);
}

void Object3d::UpdateInstance() {
	for (size_t i = 0; i < size_; i++) {
		// 各インスタンスの行列を計算
		Matrix4x4 worldMatrix = MakeAffineMatrixMatrix(transforms[i].scale, transforms[i].rotate, transforms[i].translate);
		Matrix4x4 worldViewProjectionMatrix;

		if (camera) {
			const Matrix4x4& viewMatrix = camera->GetViewMatrix();
			const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix();
			worldViewProjectionMatrix = Multiply(worldMatrix, viewMatrix);
			worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, projectionMatrix);
		}
		else {
			worldViewProjectionMatrix = worldMatrix;
		}

		// 行列をバッファに書き込む
		InstanseTransfomationMatrixData[i]->World = worldMatrix;
		InstanseTransfomationMatrixData[i]->WVP = worldViewProjectionMatrix;
	}
}

void Object3d::DrawInstance()
{
	////------平行光源用------////
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	
	// 3Dモデルが割り当てられている場合のみ描画
	if (model) {
		for (size_t i = 0; i < size_; i++) {
			// インスタンスごとの定数バッファを設定
			Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, InstanseTransformationMatrixResource[i]->GetGPUVirtualAddress());
		}
		model->DrawInstance(size_);
	}
}

void Object3d::SetModel(const std::string& filePath)
{
	//モデルを検索してセット
	model = ModelManager::GetInstance()->FindModel(filePath);
}



