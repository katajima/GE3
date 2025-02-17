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
#include"DirectXGame/engine/MyGame/MyGame.h"

#include <iostream>

#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/Animation/Animation.h"
#include"DirectXGame/engine/Light/LightCommon.h"
#include"DirectXGame/engine/Line/Line.h"


void Object3d::Initialize()
{
	//Collider::Initialize();

	object3dCommon_ = Object3dCommon::GetInstance();

	transfomation = std::make_unique<Transfomation>();

	transfomation->Initialize(object3dCommon_->GetDxCommon());

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
				model->animationTime += MyGame::GameTime(); // フレームごとの時間経過を反映
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
				model->animationTime += MyGame::GameTime(); // フレームごとの時間経過を反映
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

void Object3d::Draw(ObjectType type)
{
	ObjectTypeDiscrimination(type);

	DrawSetting();

	// 3Dモデルが割り当てれていれば描画する
	if (model) {
		model->Draw();
	}

}

void Object3d::DrawSkinning(ObjectType type)
{
	ObjectTypeDiscrimination(type);

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

	// カメラのビュープロジェクション行列を取得
	Matrix4x4 matViewProjection = Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, 1280, 720, 0, 1);

	// 視錐台内にオブジェクトがあるかチェック (matViewProjection を渡す)
	if (!IsInFrustum(matViewProjection, wPos)) {
		return Vector2{ -100, -100 }; // 視錐台外にある場合、無効なスクリーン座標を返す
	}

	// ワールド座標をクリップ空間座標へ変換
	Vector4 clipSpacePos = Transforms(Vector4(wPos.x, wPos.y, wPos.z, 1.0f), matViewProjection);

	// 透視除算 (NDC へ変換)
	if (clipSpacePos.w == 0.0f) {
		return Vector2{ -100, -100 }; // 透視除算エラー
	}
	Vector3 ndcPos = {
		clipSpacePos.x / clipSpacePos.w,
		clipSpacePos.y / clipSpacePos.w,
		clipSpacePos.z / clipSpacePos.w
	};

	// NDC → スクリーン座標変換
	Vector4 screenPos = Transforms(Vector4(ndcPos.x, ndcPos.y, ndcPos.z, 1.0f), matViewport);

	return Vector2{ screenPos.x, screenPos.y };
}


bool Object3d::IsInFrustum(const Matrix4x4& viewProjectionMatrix, const Vector3& position)
{
	// クリップスペース座標を取得
	Vector4 clipSpacePosition = Transforms(Vector4(position.x, position.y, position.z, 1.0f), viewProjectionMatrix);

	// w が負の場合、カメラの後ろにあるため視錐台外
	if (clipSpacePosition.w <= 0.0f) {
		return false;
	}

	// 視錐台内にあるかチェック
	if (clipSpacePosition.x < -clipSpacePosition.w || clipSpacePosition.x > clipSpacePosition.w ||
		clipSpacePosition.y < -clipSpacePosition.w || clipSpacePosition.y > clipSpacePosition.w ||
		clipSpacePosition.z < 0 || clipSpacePosition.z > clipSpacePosition.w)
	{
		return false;
	}

	return true;
}


void Object3d::DrawSetting()
{
	LightCommon::GetInstance()->DrawLight();

	transfomation->GetCommandList(1);

	camera->GetCommandList(4);
}

void Object3d::ObjectTypeDiscrimination(ObjectType type)
{
	switch (type)
	{
	case Object3d::ObjectType::UvInterpolation_MODE_SOLID_BACK:
		Object3dCommon::GetInstance()->DrawCommonSetting(Object3dCommon::PSOType::UvInterpolation_MODE_SOLID_BACK);
		break;
	case Object3d::ObjectType::NoUvInterpolation_MODE_SOLID_BACK:
		Object3dCommon::GetInstance()->DrawCommonSetting(Object3dCommon::PSOType::NoUvInterpolation_MODE_SOLID_BACK);
		break;
	case Object3d::ObjectType::UvInterpolation_MODE_WIREFRAME_BACK:
		Object3dCommon::GetInstance()->DrawCommonSetting(Object3dCommon::PSOType::UvInterpolation_MODE_WIREFRAME_BACK);
		break;
	case Object3d::ObjectType::NoUvInterpolation_MODE_WIREFRAME_BACK:
		Object3dCommon::GetInstance()->DrawCommonSetting(Object3dCommon::PSOType::NoUvInterpolation_MODE_WIREFRAME_BACK);
		break;
	case Object3d::ObjectType::UvInterpolation_MODE_SOLID_NONE:
		Object3dCommon::GetInstance()->DrawCommonSetting(Object3dCommon::PSOType::UvInterpolation_MODE_SOLID_NONE);
		break;
	case Object3d::ObjectType::NoUvInterpolation_MODE_SOLID_NONE:
		Object3dCommon::GetInstance()->DrawCommonSetting(Object3dCommon::PSOType::NoUvInterpolation_MODE_SOLID_NONE);
		break;
	case Object3d::ObjectType::UvInterpolation_MODE_WIREFRAME_NONE:
		Object3dCommon::GetInstance()->DrawCommonSetting(Object3dCommon::PSOType::UvInterpolation_MODE_WIREFRAME_NONE);
		break;
	case Object3d::ObjectType::NoUvInterpolation_MODE_WIREFRAME_NONE:
		Object3dCommon::GetInstance()->DrawCommonSetting(Object3dCommon::PSOType::NoUvInterpolation_MODE_WIREFRAME_NONE);
		break;
	default:
		break;
	}


}

#pragma endregion // 描画系

void Object3d::SetModel(const std::string& filePath)
{
	//モデルを検索してセット
	model = ModelManager::GetInstance()->FindModel(filePath);
}




