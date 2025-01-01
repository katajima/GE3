#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/struct/Material.h"
#include "DirectXGame/engine/Animation/Animation.h"
#include"DirectXGame/engine/Line/Line.h"

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
#include<vector>
#include<format>
#include<span>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



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
		std::map<std::string, JointWeightData> skinClusterData;
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices; // 追加：インデックスデータ
		std::vector<VertexData> indicesPos;
		MaterialData material;
		bool isNormalmap;
		Node rootNode;
		bool isAssimp;
		uint32_t skinningSrvindex;
		
	};
	std::vector<std::unique_ptr <LineDraw>> line_;

	// アニメーション
	Animation animation;
	float animationTime = 0.0f;
	Skeleton skeleton;
	SkinCluster skinCluster;

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
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;

public:



	void Initialize(ModelCommon* modelCommon,const std::string& directorypath,const std::string& filename,const std::string& file = "", const Vector2 texScale = {1,1});

	void InitializeAnime(ModelCommon* modelCommon,const std::string& directorypath,const std::string& filename,const std::string& file = "");

	
	
	void Draw();
	
	void DrawSkinning();

	//void DrawJoint();

	ModelData& GetModelData(){ return modelData; }

	void SetModelData(const ModelData& model) {
		modelData = model;
		//UpdateVertexBuffer();
		//UpdateIndexBuffer();
	}

	void MoveVertices(const Vector3& offset);

	static Node ReadNode(aiNode* node);
	
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
	


	VertexData instanceData_;


	bool useNormalMap = false;
	bool useSpecularMap = false;
public:
	//マテリアルデータを読み込む
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	//モデルデータ読み込み
	static ModelData LoadOdjFile(const std::string& directoryPath, const std::string& filename);
	
	static ModelData LoadOdjFileAssimp(const std::string& directoryPath, const std::string& filename, const Vector2 texScale = {1,1});
	
	static ModelData LoadOdjFileAssimpAmime(const std::string& directoryPath, const std::string& filename);

	static Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	static void GenerateIndices(ModelData& modelData);

	static void GenerateIndices2(ModelData& modelData);
	//	

	void UpdateVertexBuffer();

	void UpdateIndexBuffer();

	static SkinCluster CreateSkinCluster(const Skeleton& skeleton, const ModelData& modelData);

};


