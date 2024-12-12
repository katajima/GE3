#include "GamePlayScene.h"
#include <iostream>//用いるヘッダファイルが変わります。
#include <corecrt_math_defines.h>
#include <algorithm>

#pragma region Initialize

// 初期化
void GamePlayScene::Initialize()
{
	// カメラ
	InitializeCamera();
	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

	
	//オーディオの初期化
	//audio_ = Audio::GetInstance();
	// 入力初期化
	//input_ = Input::GetInstance();

	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0, 0, 0), camera.get());


	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->SetTarget(&player_->GetObject3D());
	
	player_->SetCamera(camera.get());

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();


	for (int i = 0; i < 40; i++) {
		auto enemy = std::make_unique<Enemy>();
		enemy->Initialize({ 10, 0, 0 }, 100, camera.get());
		enemys_.push_back(std::move(enemy));
	}

	const char* gropName = "enemys";
	GlobalVariables::GetInstance()->CreateGroup(gropName);
	for (int i = 0; i < enemys_.size(); i++) {
		std::string label = "Translate " + std::to_string(i);
		globalVariables->AddItem(gropName, label, enemys_[i]->GetPostion());
	}


	tail.Initialize();
	tail.SetModel("grass.obj");
	tail.SetCamera(camera.get());
	tail.transform.scale = { 100,100,100 };

	


	LoadLevelData();
}


// カメラ初期化
void GamePlayScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	//camera = Camera::GetInstance();
	camera->transform_.rotate = { 0.36f,0,0 };
	camera->transform_.translate = { 5,32.5f,-59.2f };

	cameraDebugT = camera->transform_.translate;
	cameraDebugR = camera->transform_.rotate;

	cameraT.y = 1.0f;


	flag = true;
#ifdef _DEBUG

	//flag = false;

#endif // _DEBUG]


	cameraObj_.Initialize();
}
// 各オブジェクトやスプライトなどの初期化
void GamePlayScene::InitializeResources()
{
	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

	


}
// 

void GamePlayScene::LoadLevelData()
{
	const std::string kDefaultBaseDirectory = "resources/";
	const std::string fileName = "levelData/";
	const std::string kExtension = "untitled.json";
	// 凍結してフルパスを得る 
	const std::string fullpath = kDefaultBaseDirectory + fileName + kExtension;
	// ファイルストリーム 
	std::ifstream file; // ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗チェック 
	if (file.fail()) { assert(0); }
	// JSON文字列から解凍したデータ 
	nlohmann::json deserialized;
	// 解凍 
	file >> deserialized;
	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());
	// "name"を文字列として取得 
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック 
	assert(name.compare("scene") == 0);
	// レベルデータ格納用インスタンスを生成 
	LevelData* levelData = new LevelData();
	//std::map<std::string, std::unique_ptr <Model>> models;
	const auto& models = ModelManager::GetInstance()->GetModel();
	//models = ModelManager::GetInstance()->GetModel();
	// "objects"の全オブジェクトを走査 
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));
		// 種別を取得 
		std::string type = object["type"].get<std::string>();
		if (type.compare("MESH") == 0) {
			// 要素追加 
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る 
			LevelData::ObjectData& objectData = levelData->objects.back();
			if (object.contains("name")) {
				// ファイル名 
				objectData.fileName = object["name"];
			}
			// トランスフォームのパラメータ読み込み 
			nlohmann::json& transform = object["transform"];
			// 平行移動 
			objectData.position.x = (float)transform["translation"][0];
			objectData.position.y = (float)transform["translation"][1];
			objectData.position.z = (float)transform["translation"][2];
			// 回転 
			objectData.rotation.x = (float)transform["rotation"][0];
			objectData.rotation.y = (float)transform["rotation"][1];
			objectData.rotation.z = (float)transform["rotation"][2];
			// スケーリング 
			objectData.scale.x = (float)transform["scaling"][0];
			objectData.scale.y = (float)transform["scaling"][1];
			objectData.scale.z = (float)transform["scaling"][2];
			// 再帰関数にまとめ、再帰呼出で枝を走査する 
			if (object.contains("children")) {}
		} for (auto& objectData : levelData->objects) {
			// ファイル名から登録済みモデルを検索
			Model* model = nullptr;
			auto it = models.find(objectData.fileName + ".obj");
			if (it != models.end()) { model = it->second.get(); }
			// モデルを指定して3Dオブジェクトを生成 
			Object3d* newObject = new Object3d();
			//ModelManager::GetInstance()->LoadModel(objectData.fileName + ".obj");
			newObject->Initialize();
			newObject->SetModel(model);
			newObject->SetCamera(camera.get());
			// 座標 
			newObject->transform.translate = objectData.position;
			// 回転角 
			newObject->transform.rotate = objectData.rotation;
			// 大きさ
			newObject->transform.scale = objectData.scale;
			// 配列に登録
			objects.push_back(newObject);
		}
	}
}

// 調整項目
void GamePlayScene::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* gropName = "controlPoint";
	
	gropName = "enemys";
	// グループを追加する 
	GlobalVariables::GetInstance()->CreateGroup(gropName);
	for (int i = 0; i < enemys_.size(); i++) {
		std::string label = "Translate " + std::to_string(i);
		enemys_[i]->SetPostion(globalVariables->GetVector3Value(gropName, label));
		//globalVariables->SetValue(gropName, label, controlPoints2_[i]);
	}

}
#pragma endregion 初期化関係


#pragma region UpDate

// ImGui更新
void GamePlayScene::UpdateImGui()
{


	
	
#ifdef _DEBUG
	


	ImGui::Begin("engine");	
	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::DragFloat3("Translate", &camera->transform_.translate.x, 0.1f);
		ImGui::DragFloat3("Rotate", &camera->transform_.rotate.x, 0.01f);
		ImGui::Checkbox("flag", &flag);
		if (ImGui::Button("cameraPos")) {
			camera->transform_.translate = { 0,20,-175 };
			camera->transform_.rotate = { 0,0,0 };
		}
		if (ImGui::Button("cameraPos2")) {
			camera->transform_.translate = { -30,10,-140 };
			camera->transform_.rotate = { 0,0,0 };
		}
		if (ImGui::Button("cameraPos3")) {
			camera->transform_.translate = { 0,500,0 };
			camera->transform_.rotate = { DegreesToRadians(90),0,0 };
		}
		if (ImGui::Button("cameraPos4")) {
			camera->transform_.translate = { 0,60,-220 };
			camera->transform_.rotate = { DegreesToRadians(10),0,0 };
		}


	}

	if (ImGui::TreeNode("Test")) {
		ImGui::Text("Camera1");
		ImGui::SliderFloat("値", &camera->transform_.rotate.x, 0.0f, 1.0f);
		//ImGui::TreePop();
		ImGui::Text("Camera2");
		ImGui::SliderFloat("値", &camera->transform_.rotate.x, 0.0f, 1.0f);
		ImGui::TreePop();
	}


	ImGui::End();

#endif
}

// 更新処理
void GamePlayScene::Update()
{
	// 調整項目
	ApplyGlobalVariables();

	// ImGuiの更新
	UpdateImGui();

	// プレイヤー
	player_->Update();



	/// レールカメラ
	// カメラの回転を設定
	if (flag) {
		
		followCamera_->Update();
		camera->viewMatrix_ = followCamera_->GetViewProjection().viewMatrix_;
		camera->projectionMatrix_ = followCamera_->GetViewProjection().projectionMatrix_;

		// 必要に応じて行列を更新
		//camera->UpdateMatrix();
	}
	else {
#ifdef _DEBUG

		/*if (Input::GetInstance()->IsPushKey(DIK_UP)) {
			cameraDebugT.z += 0.5f;
		}
		else if (Input::GetInstance()->IsPushKey(DIK_DOWN)) {
			cameraDebugT.z -= 0.5f;
		}
		if (Input::GetInstance()->IsPushKey(DIK_RIGHT)) {
			cameraDebugT.x += 0.5f;
		}
		else if (Input::GetInstance()->IsPushKey(DIK_LEFT)) {
			cameraDebugT.x -= 0.5f;
		}
		if (Input::GetInstance()->IsPushKey(DIK_O)) {
			cameraDebugT.y += 0.5f;
		}
		else if (Input::GetInstance()->IsPushKey(DIK_L)) {
			cameraDebugT.y -= 0.5f;
		}*/
#endif // _DEBUG
		camera->transform_.rotate = cameraDebugR;
		camera->transform_.translate = cameraDebugT;
		camera->UpdateMatrix();
	}


	

	
	
	// 敵
	for (int i = 0; i < enemys_.size(); i++) {
		//enemys_[i]->Update();
	}
	
	// タイル
	tail.Update();

	
	if (Input::GetInstance()->IsTriggerKey(DIK_P)) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

}

#pragma endregion //更新関係


#pragma region 


#pragma endregion その他

// 終了
void GamePlayScene::Finalize()
{
	for (auto& object : objects) {
		delete object;
	}
}

// 3D描画
void GamePlayScene::Draw3D()
{
	////3Dオブジェクトの描画

	player_->Draw();

	tail.Draw();
	
}

void GamePlayScene::DrawP3D()
{

}
void GamePlayScene::DrawLine3D()
{
}
// 2D描画
void GamePlayScene::Draw2D()
{
	//////////////--------スプライト-----------///////////////////

	
}

