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



	player_ = std::make_unique<Player>();
	player_->Initialize(Vector3(0, 2, -40), camera.get());


	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetObject3D());

	player_->SetCamera(camera.get());

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();


	for (int i = 0; i < 15; i++) {
		auto enemy = std::make_unique<Enemy>();
		Vector3 randPos = { float(rand() % 41 - 20),2,float(rand() % 40) };
		enemy->Initialize(randPos, 100, camera.get());
		enemy->SetPlayer(player_.get());
		enemy->SetFollowCamera(followCamera_.get());
		enemys_.push_back(std::move(enemy));
	}

	 

	tail.Initialize();
	tail.SetModel("renga.gltf");
	tail.SetCamera(camera.get());
	tail.worldtransform_.scale_ = { 10,10,10 };

	sky.Initialize();
	sky.SetModel("skydome.obj");
	sky.SetCamera(camera.get());
	sky.worldtransform_.scale_ = { 10,10,10 };
	sky.model->modelData.material[0]->enableLighting_ = false;

	//particleManager_ = ParticleManager::GetInstance();
	ParticleManager::GetInstance()->CreateParticleGroup("cc", "resources/Texture/Image.png", ModelManager::GetInstance()->FindModel("plane.obj"),true);
	ParticleManager::GetInstance()->SetPos("cc", { 0,0,0 });
	ParticleManager::GetInstance()->SetObject("cc", player_->GetObject3D().worldtransform_);
	
	ParticleManager::GetInstance()->CreateParticleGroup("Slash", "resources/Texture/aa.png", ModelManager::GetInstance()->FindModel("plane.obj"));
	ParticleManager::GetInstance()->SetPos("Slash", { 0,0,0 });
	ParticleManager::GetInstance()->SetObject("Slash", player_->GetWeapon()->GetObject3D().worldtransform_);
	
	

	for (int i = 0; i < 10; i++) {
		std::string strin = std::to_string(i) + "bullet";
		ParticleManager::GetInstance()->CreateParticleGroup(strin, "resources/Texture/aa.png", ModelManager::GetInstance()->FindModel("plane.obj"),true);
		ParticleManager::GetInstance()->SetPos(strin, { 0,0,0 });

		strin = std::to_string(i) + "exp";
		ParticleManager::GetInstance()->CreateParticleGroup(strin, "resources/Texture/aa.png", ModelManager::GetInstance()->FindModel("plane.obj"),true);
		ParticleManager::GetInstance()->SetPos(strin, { 0,0,0 });
		
		strin = std::to_string(i) + "exp2";
		ParticleManager::GetInstance()->CreateParticleGroup(strin, "resources/Texture/aa.png", ModelManager::GetInstance()->FindModel("plane.obj"),true);
		ParticleManager::GetInstance()->SetPos(strin, { 0,0,0 });
	}
	

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	InitializeResources();

	//ApplyGlobalVariables();


	LoadLevelData();
}


// カメラ初期化
void GamePlayScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	camera->Initialize();
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


	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 10; i++) {
			numSprites[j][i] = std::make_unique<Sprite>();
			std::string str = "resources/Texture/num/" + std::to_string(i) + ".png";
			numSprites[j][i]->Initialize(str, false);
			//numSprites[j][i]->SetPosition(Vector2{ float(50 * i), 100 });
		}
	}

	float xpos = 1050;
	Vector2 scale{ 75,75 };

	icon_B = std::make_unique<Sprite>();
	icon_B->Initialize("resources/Texture/icon/B.png");
	icon_B->SetPosition({ xpos,500 });
	icon_B->SetSize(scale);

	icon_Y = std::make_unique<Sprite>();
	icon_Y->Initialize("resources/Texture/icon/Y.png");
	icon_Y->SetPosition({ xpos,550 });
	icon_Y->SetSize(scale);

	icon_X = std::make_unique<Sprite>();
	icon_X->Initialize("resources/Texture/icon/X.png");
	icon_X->SetPosition({ xpos,600 });
	icon_X->SetSize(scale);

	icon_RT = std::make_unique<Sprite>();
	icon_RT->Initialize("resources/Texture/icon/RB.png");
	icon_RT->SetPosition({ xpos,450 });
	icon_RT->SetSize(scale);

	scale = { 100,33 };
	xpos = 1120;

	text_normal = std::make_unique<Sprite>();
	text_normal->Initialize("resources/Texture/text/normalAttack.png");
	text_normal->SetPosition({ xpos,520 });
	text_normal->SetSize(scale);

	text_jump = std::make_unique<Sprite>();
	text_jump->Initialize("resources/Texture/text/JumpAttack.png");
	text_jump->SetPosition({ xpos,570 });
	text_jump->SetSize(scale);

	text_dash = std::make_unique<Sprite>();
	text_dash->Initialize("resources/Texture/text/DashAttack.png");
	text_dash->SetPosition({ xpos,620 });
	text_dash->SetSize(scale);

	text_special = std::make_unique<Sprite>();
	text_special->Initialize("resources/Texture/text/special.png");
	text_special->SetPosition({ xpos,470 });
	text_special->SetSize(scale);

	scale = { 100 * 1.5f,33 * 1.5f };

	text_hit = std::make_unique<Sprite>();
	text_hit->Initialize("resources/Texture/text/Hit.png");
	text_hit->SetPosition({ 1100,110 });
	text_hit->SetSize(scale);

	text_clera = std::make_unique<Sprite>();
	text_clera->Initialize("resources/Texture/text/clear.png");
	text_clera->SetPosition({ 300,200 });
	//text_clera->SetAnchorPoint({ 0.5f,0.5f });
	text_clera->SetSize(2);
	text_clera->SetColor({ 0,1,0,1 });
	text_over = std::make_unique<Sprite>();
	text_over->Initialize("resources/Texture/text/over.png");
	text_over->SetPosition({ 300,200 });
	//text_over->SetAnchorPoint({ 0.5f,0.5f });
	text_over->SetSize(2);
	text_over->SetColor({ 1,0,0,1 });


	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* gropName = "sprite";

	// グループを追加する 
	GlobalVariables::GetInstance()->CreateGroup(gropName);

	for (int j = 0; j < 3; j++) {
		std::string str = "posNum" + std::to_string(j);
		globalVariables->AddItem(gropName, str, numpos[j]);
	}

	xpos = { 950 };

	numpos[2] = { xpos,100 };
	numpos[1] = { xpos + (50 * 1),100 };
	numpos[0] = { xpos + (50 * 2),100 };

	emit_ = std::make_unique<ParticleEmitter>();
	emit_->Initialize("groundRtttight", "dustt", ParticleManager::EmitType::kRandom);
	emit_->GetFrequency() = 0.5f;
	emit_->SetCount(200);
	emit_->SetPos({ 200,40,200 });
	emit_->SetVelocityMinMax(-Vector3{0.2f,0.2f,0.2f }, { 0.2f, 0.2f, 0.2f });
	emit_->SetLifeTimeMinMax(10.4f, 10.7f);
	emit_->SetIsAlpha(true);
	emit_->SetSizeMinMax(Vector3{ 0.2f,0.2f,0.2f }, { 0.2f,0.2f,0.2f });
	emit_->SetColorMinMax({ 0.604f, 0.384f, 0.161f }, { 0.604f, 0.384f, 0.161f });
	emit_->SetRengeMinMax({-400,-100,-400}, { 400,100,400 });
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
			newObject->worldtransform_.translate_ = objectData.position;
			// 回転角 
			newObject->worldtransform_.rotate_ = objectData.rotation;
			// 大きさ
			newObject->worldtransform_.scale_ = objectData.scale;
			// 配列に登録
			objects.push_back(newObject);
		}
	}
}
// 調整項目
void GamePlayScene::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* gropName = "sprite";


	// グループを追加する 
	GlobalVariables::GetInstance()->CreateGroup(gropName);
	for (int j = 0; j < 3; j++) {
		std::string str = "posNum" + std::to_string(j);

		//numpos[j] = globalVariables->GetVector2Value(gropName, str);

	}

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 10; i++) {
			numSprites[j][i]->SetPosition(numpos[j]);
		}
	}

}

void GamePlayScene::CheckAllCollisions()
{
	// 衝突マネージャのリセット
	collisionManager_->Reset();
	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
	if (player_->GetBehavior() == Player::Behavior::kAttack) {
		// コライダーをリストに登録
		collisionManager_->AddCollider(player_->GetWeapon());
	}

	for (const auto& bullet : player_->GetBullets()) {
		collisionManager_->AddCollider(bullet.get());
	}

	//// 敵全てについて
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		collisionManager_->AddCollider(enemy.get());
	}

	// 衝突判定
	collisionManager_->CheckAllCollisions();
}
#pragma endregion 初期化関係


#pragma region UpDate

// ImGui更新
void GamePlayScene::UpdateImGui()
{




#ifdef _DEBUG
	if (Input::GetInstance()->IsTriggerKey(DIK_P)) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	Vector2 pos = player_->GetObject3D().GetScreenPosition();
	ImGui::Begin("engine");
	ImGui::DragFloat2("screenpos", &pos.x, 0.1f);
	ImGui::End();

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
	Camera::isShake_ = false;

	// 調整項目
	ApplyGlobalVariables();

	// ImGuiの更新
	UpdateImGui();



	if (behaviorRequest_) {
		// ふるまいを変更する
		behavior_ = behaviorRequest_.value();
		// 各ふるまいごとの初期化を実行
		switch (behavior_) {
		case Behavior::kPhase1: // フェーズ１
			BehaviorPhase1Initialize();
			break;
		case Behavior::kPhase2: // フェーズ２
			BehaviorPhase2Initialize();
			break;
		}
		// ふるまいリクエストリセット
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Behavior::kPhase1: // フェーズ１
		BehaviorPhase1Update();
		break;
	case Behavior::kPhase2: // フェーズ２
		BehaviorPhase2Update();
		break;
	}

	// プレイヤー
	//if (player_->GetAlive()) {
		player_->Update();
		player_->LockOn(enemys_);
	//}
	
		

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
#endif // _DEBUG
		camera->transform_.rotate = cameraDebugR;
		camera->transform_.translate = cameraDebugT;
		camera->UpdateMatrix();
	}



	ParticleManager::GetInstance()->SetCamera(&followCamera_->GetViewProjection());


	



	// タイル
	tail.Update();
	sky.Update();
	emit_->Update();
	// デバック表示用にワールドトランスフォームを更新
	collisionManager_->UpdateWorldTransform();


	player_->SetCamera(camera.get());

	LightCommon::GetInstance()->SetLineCamera(camera.get());

	CheckAllCollisions();
}

#pragma endregion //更新関係

#pragma region BehaviorPhase

void GamePlayScene::BehaviorPhase1Initialize()
{
}

void GamePlayScene::BehaviorPhase1Update()
{
	// 敵
	count = 0;
	for (int i = 0; i < enemys_.size(); i++) {
		enemys_[i]->Update();
		if (!enemys_[i]->GetAlive()) {
			count++;
		}
	}
}

void GamePlayScene::BehaviorPhase2Initialize()
{
}

void GamePlayScene::BehaviorPhase2Update()
{
}
#pragma endregion // フェーズ


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
	sky.Draw();
	tail.Draw();

	////3Dオブジェクトの描画

	
	player_->Draw();

	// 敵
	for (int i = 0; i < enemys_.size(); i++) {
		enemys_[i]->Draw();
	}


	// パーティクル
	player_->DrawP();

	ParticleManager::GetInstance()->GetInstance()->Draw();
	//ParticleManager::GetInstance()->GetInstance()->DrawAABB();


	// 当たり判定の表示
	collisionManager_->Draw();

}

// 2D描画
void GamePlayScene::Draw2D()
{
	player_->Draw2D();


	//////////////--------スプライト-----------///////////////////
	int adsbhads = player_->GetHitCount();
	if (adsbhads >= 999) {
		adsbhads = 999;
	}

	int numDigits = (adsbhads == 0) ? 1 : static_cast<int>(log10(adsbhads)) + 1;

	for (int j = 0; j < numDigits; ++j) {
		// j桁目の数字を取り出す（右から左へ）
		int digit = (static_cast<int>(adsbhads) / static_cast<int>(pow(10, j))) % 10;

		// 桁に対応する数字を描画
		numSprites[j][digit]->SetPosition(numpos[j]);
		numSprites[j][digit]->Update();
		numSprites[j][digit]->Draw();
	}


	icon_B->Update();
	icon_Y->Update();
	icon_X->Update();
	icon_RT->Update();
	text_normal->Update();
	text_jump->Update();
	text_dash->Update();
	text_special->Update();
	text_hit->Update();
	icon_B->Draw();
	icon_Y->Draw();
	icon_X->Draw();
	if (player_->GetIsSpecial()) {
		icon_RT->Draw();
		text_special->Draw();
	}
	text_normal->Draw();
	text_jump->Draw();
	text_dash->Draw();
	text_hit->Draw();

	for (int i = 0; i < enemys_.size(); i++) {
	
		enemys_[i]->Draw2D();
	}



	if (!player_->GetAlive()) {
		sceneCount++;
		if (clock == 1) {
			text_over->Update();
			text_over->Draw();
		}
	}
	else if (count >= enemys_.size()) {
		sceneCount++;
		if (clock == 1) {
			text_clera->Update();
			text_clera->Draw();
		}
	}
	if (sceneCount % 15 == 0) {
		clock *= -1;
	}

	if (sceneCount >= 240) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}




}

