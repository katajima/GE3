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
public:
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
public:



	void Initialize(ModelCommon* modelCommon,const std::string& directorypath,const std::string& filename);

	void Draw();

	ModelData& GetModelData(){ return modelData; }

	void SetModelData(const ModelData& model) {
		modelData = model;
		UpdateVertexBuffer();
	}

	

private:
	ModelCommon* modelCommon_ = nullptr;
	Transform transform;
	
	
	//マテリアルデータ
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};
	
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

	void UpdateVertexBuffer();
};


