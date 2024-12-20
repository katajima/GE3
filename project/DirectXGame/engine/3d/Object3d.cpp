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
#include"DirectXGame/engine/Animation/Animation.h"
#include"LightCommon.h"


void Object3d::Initialize()
{

	// 引数で受け取ってメンバ変数に記録する
	//this->object3dCommon_ = Object3dCommon::GetInstance();
	//this->camera = Object3dCommon::GetInstance()->GetDefaltCamera();
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
	Matrix4x4 localMatrix = MakeIdentity4x4();

	// モデルが存在する場合
	if (model) {
		// アニメーションの更新
		if (model->animation.flag) {
			model->animationTime += 1.0f / 60.0f; // フレームごとの時間経過を反映
			model->animationTime = std::fmod(model->animationTime, model->animation.duration);

			if (model->skeleton.joints.size() > 1) {
				// スケルトンの更新
				ApplyAnimation(model->skeleton, model->animation, model->animationTime);
				//UpdateSkeleton(model->skeleton);
				localMatrix = model->skeleton.joints[0].skeletonSpaceMatrix;

			}
			else {
				// 単一のジョイントの場合
				const NodeAnimation& rootNodeAnimation = model->animation.nodeAnimations[model->modelData.rootNode.name];
				Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyframes, model->animationTime);
				Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, model->animationTime);
				Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyframes, model->animationTime);
				localMatrix = MakeAffineMatrix(scale, rotate, translate);
			}
		}
		else {
			localMatrix = model->modelData.rootNode.localMatrix;
		}
	}

	// ワールド行列の計算
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix;

	if (camera) {

		const Matrix4x4& viewMatrix = camera->GetViewMatrix();
		const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix();


		// WVP計算
		Matrix4x4 worldViewProjectionMatrix = Multiply(localMatrix, mat_); // ワールド変換
		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetViewMatrix()); // ビュー変換
		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetProjectionMatrix()); // 射影変換


		// カメラデータの更新
		Vector3 cameraFront(viewMatrix.m[0][2], viewMatrix.m[1][2], viewMatrix.m[2][2]);
		cameraData->normal = Normalize(cameraFront); // 必要なら正規化
		cameraData->worldPosition = camera->transform_.translate;

		if (model) {
			transfomationMatrixData->WVP = worldViewProjectionMatrix;
			transfomationMatrixData->World = Multiply(localMatrix, mat_);
		}
		else {
			transfomationMatrixData->WVP = worldViewProjectionMatrix;
			transfomationMatrixData->World = mat_;
		}
	}
	else {
		worldViewProjectionMatrix = mat_;
		transfomationMatrixData->WVP = worldViewProjectionMatrix;
		transfomationMatrixData->World = mat_;
	}

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



