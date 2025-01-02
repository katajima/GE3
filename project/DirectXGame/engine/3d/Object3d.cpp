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

	transfomation = std::make_unique<Transfomation>();

	transfomation->Initialize(Object3dCommon::GetInstance()->GetDxCommon());

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
		model->modelData.material[0]->GPUData();
	}

	// ワールド行列の計算
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	
	// トランスフォームデータ
	transfomation->Update(model, camera, localMatrix, mat_);
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
			
		}
		else {
			localMatrix = model->modelData.rootNode.localMatrix;
		}
		model->modelData.material[0]->GPUData();
	}


	// ワールド行列の計算
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	// トランスフォームデータ
	transfomation->UpdateSkinning(model, camera, localMatrix, mat_);
}

void Object3d::UpdateAnimation()
{
	Matrix4x4 localMatrix = MakeIdentity4x4();
	// モデルが存在する場合
	if (model) {
		// アニメーションの更新
		if (model->animation.flag) {
			if (flag) {
				model->animationTime += 1.0f / 60.0f; // フレームごとの時間経過を反映
			}
			model->animationTime = std::fmod(model->animationTime, model->animation.duration);
			
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
		model->modelData.material[0]->GPUData();
	}


	// ワールド行列の計算
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	// トランスフォームデータ
	transfomation->Update(model, camera, localMatrix, mat_);
}

#pragma endregion //更新系

#pragma region Draw

void Object3d::Draw()
{
	Object3dCommon::GetInstance()->DrawCommonSetting();

	DrawSetting();

	// 3Dモデルが割り当てれていれば描画する
	if (model) {
		model->Draw();
	}

}

void Object3d::DrawSkinning()
{
	SkinningConmmon::GetInstance()->DrawCommonSetting();

	DrawSetting();

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

void Object3d::DrawSetting()
{
	LightCommon::GetInstance()->DrawLight();

	transfomation->GetCommandList(1);

	camera->GetCommandList(4);
}

#pragma endregion // 描画系

void Object3d::SetModel(const std::string& filePath)
{
	//モデルを検索してセット
	model = ModelManager::GetInstance()->FindModel(filePath);
}




