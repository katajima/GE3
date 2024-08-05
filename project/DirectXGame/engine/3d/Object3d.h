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
private:
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

public:
	// 初期化
	void Initialize(Object3dCommon* object3dCommon);
	// 更新
	void Update();
	// 描画
	void Draw();


	Model* GetModel() { return model; };
	// setter
	void SetModel(Model* model) { this->model = model; }
	void SetModel(const std::string& filePath);
	//const Model& GetModel() const{ return model; }

	// setter
	void SetScale(const Vector3& scale) { transform.scale = scale; };
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; };
	void SetTranslate(const Vector3& translate) { transform.translate = translate; };
	void SetDirectionalLightColor(const Vector4 color) { directionalLightData->color = color; };
	void SetDirectionalLightDirection(const Vector3 direction) { directionalLightData->direction = direction; };
	void SetDirectionalLightIntensity(const float intensity) { directionalLightData->intensity = intensity; };
	void SetUvTranslate(const Vector3 translate) { model->SetUvTransformTranslate(translate); };
	void SetUvRotate(const Vector3 rotate) { model->SetUvTransformRotate(rotate); };
	void SetUvScale(const Vector3 scale) { model->SetUvTransformScale(scale); };

	void SetLighting(const int32_t isLight) { model->SetLighting(isLight); };
	void SetHalfLambert(const int32_t isLight) { model->SetHalfLambert(isLight); };
	// getter
	const Vector3& GetScale()const { return transform.scale; };
	const Vector3& GetRotate()const { return transform.rotate; };
	const Vector3& GetTranslate()const { return transform.translate; };
	const Vector4& GetDirectionalLightColor() { return directionalLightData->color; };
	const Vector3& GetDirectionalLightDirection() { return directionalLightData->direction; };
	const float& GetDirectionalLightIntensity() { return directionalLightData->intensity; };

	const Vector3& GetUvTranslate() { return model->GetUvTransformTranslate(); };
	const Vector3& GetUvRotate() { return model->GetUvTransformRotate(); };
	const Vector3& GetUvScale() { return model->GetUvTransformScale(); };
	
	const int32_t& GetLighting() { return model->GetLighting(); };
	const int32_t& GetHalfLambert() { return model->GetHalfLambert(); };




	void SetCamera(Camera* camera) { this->camera = camera; }

private:
	Camera* camera = nullptr;

	Model* model = nullptr;

	Object3dCommon* object3dCommon_ = nullptr;

	
	Transform uvTransform;

	

	TransfomationMatrix* transfomationMatrixData;
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource;

	//平行光源用のリソースを作る

	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	DirectionalLight* directionalLightData = nullptr;

	Transform transform;
	Transform cameraTransform;


public:
	
};


