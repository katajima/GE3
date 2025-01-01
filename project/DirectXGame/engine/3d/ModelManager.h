#pragma once
#include <windows.h>
#include<cstdint>
#include<string>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
using namespace Microsoft::WRL;
#include<vector>
#include<map>
#include"Model.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"ModelCommon.h"

// テクスチャマネージャー
class ModelManager {
public:
	// シングルトンインスタンスの取得
	static ModelManager* GetInstance();
	//
	void Finalize();

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// モデルファイルの読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadModel(const std::string& filePath, const std::string& dire = "",const Vector2 texScale = {1,1});
	
	void LoadModelAmime(const std::string& filePath, const std::string& dire = "");

	Model* FindModel(const std::string& filePath);

	// 

	const std::map<std::string, std::unique_ptr<Model>>& GetModel() const { return models; }

private:
	static ModelManager* instance;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = delete;
	ModelManager& operator=(ModelManager&) = delete;

	std::map<std::string, std::unique_ptr<Model>> models;

	ModelCommon* modelCommon = nullptr;
};
