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
#include"LightCommon.h"


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




	cameraResource = Object3dCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(CameraGPU));
	//書き込むためのアドレスを取得
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));



	cameraData->worldPosition = Vector3{ 1.0f,1.0f,1.0f };
	cameraData->normal = { 0,0,0 };

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

		// カメラの法線方向
		Vector3 cameraFront = Vector3(viewMatrix.m[0][2], viewMatrix.m[1][2], viewMatrix.m[2][2]); // 法線ベクトルを正規化
		cameraFront = Normalize(cameraFront);
		
		cameraData->normal = cameraFront;
		cameraData->worldPosition = camera->transform_.translate;
	}
	else {
		worldViewProjectionMatrix = mat_;
	}


	//cameraData->worldPosition = camera->transform_.translate;

	transfomationMatrixData->World = mat_;
	transfomationMatrixData->WVP = worldViewProjectionMatrix;
	transfomationMatrixData->worldInverseTranspose = Transpose(Inverse(mat_));


}

void Object3d::Draw()
{


	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	// Cameraのバインド
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());


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

void Object3d::SetModel(const std::string& filePath)
{
	//モデルを検索してセット
	model = ModelManager::GetInstance()->FindModel(filePath);
}



