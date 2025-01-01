#include"Object3dCommon.h"
#include"Object3d.h"
#include"DirectXGame/engine/Skinning/Skinning.h"
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
#include"DirectXGame/engine/Line/Line.h"


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

#pragma region Update

void Object3d::Update()
{
	Matrix4x4 localMatrix = MakeIdentity4x4();
	// モデルが存在する場合
	if (model) {
		localMatrix = model->modelData.rootNode.localMatrix;
	}


	// ワールド行列の計算
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	//mat_ = localMatrix;
	Matrix4x4 worldViewProjectionMatrix{};

	if (camera) {

		const Matrix4x4& viewMatrix = camera->GetViewMatrix();
		const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix();


		// WVP計算
		Matrix4x4 worldViewProjectionMatrix{};

		worldViewProjectionMatrix = Multiply(localMatrix, mat_); // ワールド変換

		//worldViewProjectionMatrix = mat_;
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

void Object3d::UpdateSkinning()
{
	Matrix4x4 localMatrix = MakeIdentity4x4();
	// モデルが存在する場合
	if (model) {
		// アニメーションの更新
		if (model->animation.flag) {
			ImGui::Begin("Joint Info");
			ImGui::Checkbox("flagTime", &flag);
			if (flag) {
				model->animationTime += 1.0f / 60.0f; // フレームごとの時間経過を反映
			}
			model->animationTime = std::fmod(model->animationTime, model->animation.duration);
			ImGui::SliderFloat("animationTime", &model->animationTime, 0.0f, model->animation.duration);

			ImGui::End();

			localMatrix = model->skeleton.joints[0].skeletonSpaceMatrix;
			
			ApplyAnimation(model->skeleton, model->animation, model->animationTime);
			// スケルトンの更新
			UpdateSkeleton(model->skeleton);

			// スキニング更新
			UpdateSkinCluster(model->skinCluster, model->skeleton);

			// ボーンのライン描画
			UpdateLineSkeleton(model->skeleton.joints, model->line_, camera);

			// Imguiの表示
			ImGuiJoint(model->skeleton.joints);
			//
			//ImGuiNode(model->modelData.rootNode.children);

		}
		else {
			localMatrix = model->modelData.rootNode.localMatrix;
		}
	}


	// ワールド行列の計算
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	//mat_ = localMatrix;
	Matrix4x4 worldViewProjectionMatrix;

	if (camera) {

		const Matrix4x4& viewMatrix = camera->GetViewMatrix();
		const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix();


		// WVP計算
		Matrix4x4 worldViewProjectionMatrix{};
		
		worldViewProjectionMatrix = mat_;
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

void Object3d::UpdateAnimation()
{
	Matrix4x4 localMatrix = MakeIdentity4x4();
	// モデルが存在する場合
	if (model) {
		// アニメーションの更新
		if (model->animation.flag) {
			/*ImGui::Begin("Joint Info");
			ImGui::Checkbox("flagTime", &flag);
			*/if (flag) {
				model->animationTime += 1.0f / 60.0f; // フレームごとの時間経過を反映
			}
			model->animationTime = std::fmod(model->animationTime, model->animation.duration);
			//ImGui::SliderFloat("animationTime", &model->animationTime, 0.0f, model->animation.duration);

			//ImGui::End();

			// 単一のジョイントの場合
			const NodeAnimation& rootNodeAnimation = model->animation.nodeAnimations[model->modelData.rootNode.name];
			Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyframes, model->animationTime);
			Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, model->animationTime);
			Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyframes, model->animationTime);
			localMatrix = MakeAffineMatrix(scale, rotate, translate);
		}
		else {
			localMatrix = model->modelData.rootNode.localMatrix;
		}
	}


	// ワールド行列の計算
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	//mat_ = localMatrix;
	Matrix4x4 worldViewProjectionMatrix;

	if (camera) {

		const Matrix4x4& viewMatrix = camera->GetViewMatrix();
		const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix();


		// WVP計算
		Matrix4x4 worldViewProjectionMatrix{};

		worldViewProjectionMatrix = Multiply(localMatrix, mat_); // ワールド変換

		//worldViewProjectionMatrix = mat_;
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

#pragma endregion //更新系

void Object3d::Draw()
{
	Object3dCommon::GetInstance()->DrawCommonSetting();

	LightCommon::GetInstance()->DrawLight();


	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	// Cameraのバインド
	Object3dCommon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());


	// 3Dモデルが割り当てれていれば描画する
	if (model) {
		model->Draw();
	}

}

void Object3d::DrawSkinning()
{
	SkinningConmmon::GetInstance()->DrawCommonSetting();

	LightCommon::GetInstance()->DrawLight();


	SkinningConmmon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	// Cameraのバインド
	SkinningConmmon::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());


	// 3Dモデルが割り当てれていれば描画する
	if (model) {
		model->DrawSkinning();
	}
}

void Object3d::DrawLine()
{
	LineCommon::GetInstance()->DrawCommonSetting();


	DrawSkeleton(model->skeleton.joints, model->line_, transform.translate, transform.scale);
}



void Object3d::SetModel(const std::string& filePath)
{
	//モデルを検索してセット
	model = ModelManager::GetInstance()->FindModel(filePath);
}



