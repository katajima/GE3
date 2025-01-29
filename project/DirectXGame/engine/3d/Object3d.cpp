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
	//Collider::Initialize();

	transfomation = std::make_unique<Transfomation>();

	transfomation->Initialize(Object3dCommon::GetInstance()->GetDxCommon());

	worldtransform_.Initialize();
	worldtransform_.translate_.x = {0.00000001f};

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

	worldtransform_.Update();

	// トランスフォームデータ
	transfomation->Update(model, camera, localMatrix, worldtransform_.worldMat_);
}

void Object3d::UpdateSkinning()
{
	Matrix4x4 localMatrix = MakeIdentity4x4();
	// モデルが存在する場合
	if (model) {
		// アニメーションの更新
		if (model->animation.flag) {
			//ImGui::Begin("Joint Info");
			//ImGui::Checkbox("flagTime", &flag);
			if (flag) {
				model->animationTime += 1.0f / 60.0f; // フレームごとの時間経過を反映
			}
			model->animationTime = std::fmod(model->animationTime, model->animation.duration);
			//ImGui::SliderFloat("animationTime", &model->animationTime, 0.0f, model->animation.duration);

			//ImGui::End();

			localMatrix = model->skeleton.joints[0].skeletonSpaceMatrix;
			
			ApplyAnimation(model->skeleton, model->animation, model->animationTime);
			// スケルトンの更新
			UpdateSkeleton(model->skeleton);

			// スキニング更新
			UpdateSkinCluster(model->skinCluster, model->skeleton);

			// ボーンのライン描画
			UpdateLineSkeleton(model->skeleton.joints, model->line_, camera);

			// Imguiの表示
			//ImGuiJoint(model->skeleton.joints);
			
		}
		else {
			localMatrix = model->modelData.rootNode.localMatrix;
		}
		model->modelData.material[0]->GPUData();
	}


	worldtransform_.Update();

	// トランスフォームデータ
	transfomation->UpdateSkinning(model, camera, localMatrix, worldtransform_.worldMat_);
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

	worldtransform_.Update();

	// トランスフォームデータ
	transfomation->Update(model, camera, localMatrix, worldtransform_.worldMat_);
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


	DrawSkeleton(model->skeleton.joints, model->line_,worldtransform_.translate_,worldtransform_.scale_);
}

Vector2 Object3d::GetScreenPosition()
{
	Vector3 wPos = worldtransform_.worldMat_.GetWorldPosition();

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, 1280, 720, 0, 1);
	
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(camera->GetViewMatrix(), Multiply(camera->GetProjectionMatrix(), matViewport));

	Vector3 screenPos;

	if (Length(wPos) == 0) {
		wPos.z = 0.00000001f;
	}

	// ワールド→スクリーン座標変換（ここで3Dから2Dになる）
	screenPos = Transforms(wPos, matViewProjectionViewport);


	return Vector2{ screenPos.x,screenPos.y};
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




