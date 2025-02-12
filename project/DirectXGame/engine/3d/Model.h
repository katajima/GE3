#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/struct/Material.h"
#include "DirectXGame/engine/Animation/Animation.h"
#include"DirectXGame/engine/Line/Line.h"
#include"DirectXGame/engine/Mesh/Mesh.h"
#include"DirectXGame/engine/Material/Material.h"

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
#include<vector>
#include<format>
#include<span>
#include <iostream>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



class ModelCommon;



class Model
{
public:
	//モデルデータ
	struct ModelData
	{
		std::map<std::string, JointWeightData> skinClusterData;
		//MaterialData material;
		bool isNormalmap;
		Node rootNode;
		bool isAssimp;
		uint32_t skinningSrvindex;
		std::vector <std::unique_ptr<Mesh>> mesh;
		std::vector < std::unique_ptr<Material>> material;
	};
	std::vector<std::unique_ptr <LineDraw>> line_;

	// アニメーション
	Animation animation;
	float animationTime = 0.0f;
	Skeleton skeleton;
	SkinCluster skinCluster;


public:



	void Initialize(ModelCommon* modelCommon,const std::string& directorypath,const std::string& filename,const std::string& file = "", const Vector2 texScale = {1,1});

	void InitializeAnime(ModelCommon* modelCommon,const std::string& directorypath,const std::string& filename,const std::string& file = "");

	
	
	void Draw();
	
	void DrawSkinning();

	ModelData& GetModelData(){ return modelData; }

	void MoveVertices(const Vector3& offset);

	static Node ReadNode(aiNode* node);
private:

	ModelData LoadMesh(const aiScene* _scene);


public:


	// Objファイルのデータ
	ModelData modelData;
private:
	ModelCommon* modelCommon_ = nullptr;
	
	
	
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

	
	
	//
	static SkinCluster CreateSkinCluster(const Skeleton& skeleton, const ModelData& modelData);
public:

};


