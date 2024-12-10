#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/struct/Material.h"
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
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	//モデルデータ
	struct ModelData
	{
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices; // 追加：インデックスデータ
		std::vector<VertexData> indicesPos;
		MaterialData material;
		bool isNormalmap;
	};
	// カスタムハッシュ関数
	struct VertexHash {
		size_t operator()(const VertexData& vertex) const {
			// 簡単なハッシュ関数（例として位置のみを使用）
			return std::hash<float>()(vertex.position.x) ^
				std::hash<float>()(vertex.position.y) ^
				std::hash<float>()(vertex.position.z);
		}
	};

	
	size_t operator()(const VertexData& vertex) const {
		return std::hash<float>()(vertex.position.x) ^
			std::hash<float>()(vertex.position.y) ^
			std::hash<float>()(vertex.position.z) ^
			std::hash<float>()(vertex.normal.x) ^
			std::hash<float>()(vertex.texcoord.x);
	}


	Material* materialData;
public:



	void Initialize(ModelCommon* modelCommon,const std::string& directorypath,const std::string& filename, const std::string& normalMap = "", const std::string& specularMapTex = "");

	void Draw();

	void DrawInstance(size_t instanceCount);

	ModelData& GetModelData(){ return modelData; }

	void SetModelData(const ModelData& model) {
		modelData = model;
		UpdateVertexBuffer();
		UpdateIndexBuffer();
	}

	void MoveVertices(const Vector3& offset);

	
	// Objファイルのデータ
	ModelData modelData;
private:
	ModelCommon* modelCommon_ = nullptr;
	Transform transform;
	
	
	
	

	// バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	


	VertexData instanceData_;


	bool useNormalMap = false;
	bool useSpecularMap = false;
public:
	//マテリアルデータを読み込む
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	//モデルデータ読み込み
	static ModelData LoadOdjFile(const std::string& directoryPath, const std::string& filename);
	
	static ModelData LoadOdjFileAssimp(const std::string& directoryPath, const std::string& filename);

	static void GenerateIndices(ModelData& modelData);

	static void GenerateIndices2(ModelData& modelData);
		

	void UpdateVertexBuffer();

	void UpdateIndexBuffer();
};


