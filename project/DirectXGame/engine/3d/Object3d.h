#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include "DirectXGame/engine/base/Camera.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
#include<vector>
#include<format>
#include"Model.h"
#include"ModelManager.h"
using namespace Microsoft::WRL;

class Object3dCommon;

class Object3d
{
public:
	// 初期化
	void Initialize(/*Object3dCommon* object3dCommon*/);
	// 更新
	void Update();
	// 描画
	void Draw();

	// 初期化
	void InitializeInstance(size_t size);
	// 更新
	void UpdateInstance();

	void DrawInstance();

	// setter
	void SetModel(Model* model) { this->model = model; }
	Model* GetModel() { return model; }
	void SetModel(const std::string& filePath);
	//const Model& GetModel() const{ return model; }

	
	void SetCamera(Camera* camera) { this->camera = camera; }

	Vector3 GetWorldPosition() {
		// ワールド座標を入れる
		Vector3 worldPos;
		worldPos.x = mat_.m[3][0];
		worldPos.y = mat_.m[3][1];
		worldPos.z = mat_.m[3][2];
		return worldPos;
	};

private:
	Camera* camera = nullptr;

	Model* model = nullptr;

	//Object3dCommon* object3dCommon_ = nullptr;

	// 頂点データ
	struct VertexData {

		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	//マテリアルデータ
	struct MaterialData {
		std::string textuerFilePath;
		//テクスチャ番号
		uint32_t textureIndex = 0;
	};
	//モデルデータ
	struct ModelData
	{
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices; // 追加：インデックスデータ
		std::vector<VertexData> indicesPos;
		MaterialData material;
	};
	//マテリアルデータ
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};
	// 座標変換行列データ
	struct TransfomationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
	// 平行高原
	struct DirectionalLight {
		Vector4 color; //!< ライトの色
		Vector3 direction; //!< ライトの向き
		float intensity; //!< 輝度
	};
	

	TransfomationMatrix* transfomationMatrixData;
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource;

	//平行光源用のリソースを作る

	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	DirectionalLight* directionalLightData = nullptr;

	
	std::vector<TransfomationMatrix*> InstanseTransfomationMatrixData;
	std::vector<Microsoft::WRL::ComPtr < ID3D12Resource>> InstanseTransformationMatrixResource;

	size_t size_;
public:
	Transform transform;

	Matrix4x4 mat_;

	// 親となるワールド変換へのポインタ
	const Object3d* parent_ = nullptr;

	std::vector<Transform> transforms;
};


