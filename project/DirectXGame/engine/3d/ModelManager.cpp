#include "ModelManager.h"

ModelManager* ModelManager::instance = nullptr;

ModelManager* ModelManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ModelManager;
	}
	return instance;
}

void ModelManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void ModelManager::Initialize(DirectXCommon* dxCommon)
{
	
	//modelCommon = new ModelCommon;
	modelCommon = ModelCommon::GetInstance();
	modelCommon->Initialize(dxCommon);
}

void ModelManager::LoadModel(const std::string& filePath, const std::string& dire, const Vector2 texScale)
{

	std::string file = filePath;
	if (dire != "") {
		file = dire + filePath;
	}


	// 読み込み済みモデルを検索
	if (models.contains(file)) {
		return;
	}
	//モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize(modelCommon, "./resources/Models", filePath, dire, texScale);

	// モデルをmapコンテナに格納
	models.insert(std::make_pair(filePath, std::move(model)));
}

void ModelManager::LoadModelAmime(const std::string& filePath, const std::string& dire)
{

	std::string file = filePath;
	if (dire != "") {
		file = dire + filePath;
	}


	// 読み込み済みモデルを検索
	if (models.contains(file)) {
		return;
	}
	//モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->InitializeAnime(modelCommon, "./resources/Models", filePath, dire);

	// モデルをmapコンテナに格納
	models.insert(std::make_pair(filePath, std::move(model)));
}


Model* ModelManager::FindModel(const std::string& filePath)
{
	// 読み込み済みモデルを検索
	if (models.contains(filePath)) {
		// 読み込みモデルを戻り値としてreturn
		return models.at(filePath).get();
	}

	// ファイル名一致なし
	return nullptr;
}
