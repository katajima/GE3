#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include "DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/struct/Light.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
#include<vector>
#include<format>
#include"Model.h"
#include"ModelManager.h"
#include"DirectXGame/engine/Transfomation/Transfomation.h"


using namespace Microsoft::WRL;

class Object3dCommon;

class Object3d
{
public:
	// 初期化
	void Initialize();
	// 更新(アニメーション無し)
	void Update();
	// 更新(スキニング有り)
	void UpdateSkinning();
	// 更新(アニメーション有り)
	void UpdateAnimation();
	
	// 描画通常
	void Draw();
	// 描画スキニング用
	void DrawSkinning();
	// 描画ライン
	void DrawLine();
	
	// setter
	 void SetModel(Model* model) { this->model = model; }
	 Model* GetModel() const { return model; }
	// void SetModelData(Model::ModelData modeldata);

	 void SetModel(const std::string& filePath);
	void SetCamera(Camera* camera) { this->camera = camera; }
	Vector3 GetWorldPosition() const {
		// ワールド座標を入れる
		Vector3 worldPos;
		worldPos.x = mat_.m[3][0];
		worldPos.y = mat_.m[3][1];
		worldPos.z = mat_.m[3][2];
		return worldPos;
	};
	Vector3 GetPreWorldPosition() const {
		// ワールド座標を入れる
		Vector3 worldPos;
		worldPos.x = preMat_.m[3][0];
		worldPos.y = preMat_.m[3][1];
		worldPos.z = preMat_.m[3][2];
		return worldPos;
	};

	Vector2 GetScreenPosition();
	
	Mesh* GetMesh(int index) { return model->modelData.mesh[index].get(); }
	Material* GetMaterial(int index) { return model->modelData.material[index].get(); }

private:
	// 各コマンドリスト
	void DrawSetting();

private:
	// カメラ
	Camera* camera = nullptr;
	// トランスフォームデータ
	std::unique_ptr<Transfomation> transfomation = nullptr;
	//
	// 
	bool flag = true;
	
public:
	// トランスフォーム(位置、回転、拡縮)
	Transform transform;
	// モデル
	Model* model = nullptr;
	// マトリックス(現在)
	Matrix4x4 mat_;
	// マトリックス(過去)
	Matrix4x4 preMat_;

	// 親となるワールド変換へのポインタ
	const Object3d* parent_ = nullptr;
};


