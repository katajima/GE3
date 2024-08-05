
#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
#include<vector>
#include<format>

class ModelCommon;

class Model
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
		int32_t isHalfLambert;
	};

public:
	void Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename);

	void Draw();

	// setter
	void SetMaterial(Material* material) { materialData = material; };
	void SetUvTransformTranslate(const Vector3& translate) { uvTransform.translate = translate; };
	void SetUvTransformRotate(const Vector3& rotate) { uvTransform.rotate = rotate; };
	void SetUvTransformScale(const Vector3& scale) { uvTransform.scale = scale; };

	void SetLighting(const int32_t isLight) { materialData->enableLighting = isLight; };
	void SetHalfLambert(const int32_t isLight) { materialData->isHalfLambert = isLight; };

	//// getter
	const Vector3& GetUvTransformTranslate()const { return uvTransform.translate; };
	const Vector3& GetUvTransformRotate()const { return uvTransform.rotate; };
	const Vector3& GetUvTransformScale()const { return uvTransform.scale; };

	const int32_t& GetLighting()const { return materialData->enableLighting; };
	const int32_t& GetHalfLambert()const { return materialData->isHalfLambert; };

private:
	ModelCommon* modelCommon_ = nullptr;
	Transform transform;
	Transform uvTransform;


	// Objファイルのデータ
	ModelData modelData;

	// バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	Material* materialData;
public:
	//マテリアルデータを読み込む
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	//モデルデータ読み込み
	static ModelData LoadOdjFile(const std::string& directoryPath, const std::string& filename);


};
