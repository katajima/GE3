#include "GamePlayScene.h"
#include <iostream>//用いるヘッダファイルが変わります。
#include <corecrt_math_defines.h>
#include <algorithm>

#pragma region Initialize

// 初期化
void GamePlayScene::Initialize()
{
	//オーディオの初期化
	audio_ = Audio::GetInstance();
	// 入力初期化
	input_ = Input::GetInstance();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* gropName = "controlPoint";
	// グループを追加する
	GlobalVariables::GetInstance()->CreateGroup(gropName);
	for (int i = 0; i < controlPointObjects_.size(); i++) {
		std::string label = "Translate " + std::to_string(i);
		globalVariables->AddItem(gropName, label, controlPoints2_[i]);
	}
	// カメラ
	InitializeCamera();
	// リソース
	InitializeResources();

	// 敵生成
	LoadEnemyPopData();;


	UpdateRail();
}

// レール初期化
void GamePlayScene::InitializeRail()
{
	
	controlPoints2_ = {
		{0.0f, 0.0f, 0.0f},// 視点
		{0.0f, 0.0f, 30.0f}, // 
		{0.0f, 0.0f, 60.0f}, //
		{0.0f, 10.0f, 80.0f},
		{0.0f, 12.0f, 90.0f},
		{0.0f, 12.0f, 100.0f},// 下り
		{5.0f, 12.0f, 105.0f},
		{10.0f, 10.0f, 110.0f},
		{20.0f, 5.0f, 120.0f},
		{30.0f, -0.0f, 110.0f},
		{40.0f, -5.0f, 100.0f},
		{45.0f, -7.5f, 95.0f},
		{30.0f, -10.0f, 90.0f},
		{20.0f, -15.0f, 100.0f},
		{10.0f, -20.0f, 110.0f},
		{20.0f, -25.0f, 120.0f},
		{30.0f, -30.0f, 130.0f},
		{40.0f, -33.0f, 140.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
	};

	ApplyGlobalVariables();

	float spacing = 1.0f; // オブジェクト間の距離（密度調整）
	float totalDistanceTraveled = 0.0f; // 累積距離

	for (size_t i = 0; i < controlPoints2_.size() - 1; ++i) {
		Vector3 start = controlPoints2_[i];
		Vector3 end = controlPoints2_[i + 1];

		float segmentLength = Length(Subtract(end, start));
		float distanceTraveled = 0.0f;

		// 各セグメント内でオブジェクトを配置
		while (distanceTraveled < segmentLength) {
			// セグメント内の `t` の計算（ローカルな範囲 [0, 1]）
			float tLocal = distanceTraveled / segmentLength;

			// グローバルな `t` を計算（Catmull-Rom 関数に渡す）
			float tGlobal = (i + tLocal) / (controlPoints2_.size() - 1);

			// Catmull-Rom 補間で位置を計算
			Vector3 pos = CatmullRom(controlPoints2_, tGlobal);

			// 進行方向のベクトルを計算
			Vector3 posNext = CatmullRom(controlPoints2_, tGlobal + 0.01f);
			Vector3 velocity = Normalize(Subtract(posNext, pos));

			// 回転の計算
			float rotateY = std::atan2(velocity.x, velocity.z);
			float length = Length(Vector3(velocity.x, 0, velocity.z));
			float rotateX = std::atan2(velocity.y, length);

			// オブジェクトの生成と配置
			auto object3d = std::make_unique<Object3d>();
			object3d->Initialize();
			object3d->SetModel("rail.obj");
			object3d->transform.translate = pos;
			object3d->transform.rotate = Vector3(rotateX, rotateY, 0);

			railObject2.push_back(std::move(object3d));

			// 次の配置位置に進める
			distanceTraveled += spacing;
		}

		// 累積距離の更新
		totalDistanceTraveled += segmentLength;
	}




	for (int i = 0; i < controlPoints2_.size(); ++i) {
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize();
		object3d->SetModel("Sphere.obj");
		object3d->transform.translate = controlPoints2_[i];
		controlPointObjects_.push_back(std::move(object3d));
	}

}

// カメラ初期化
void GamePlayScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	camera->transform_.rotate = { 0.36f,0,0 };
	camera->transform_.translate = { 5,32.5f,-59.2f };

	cameraDebugT = camera->transform_.translate;
	cameraDebugR = camera->transform_.rotate;

	cameraT.y = 1.0f;

	cameraObj_.Initialize();
}
// 各オブジェクトやスプライトなどの初期化
void GamePlayScene::InitializeResources()
{


	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

	// レールかんけい
	InitializeRail();


	// 列車オブジェクトを unique_ptr で作成
	train.Initialize();
	train.SetModel("train.obj");
	train.transform.translate = CatmullRom(controlPoints2_, 1);

	trainTemp.Initialize();
	trainTemp.transform.translate = train.transform.translate;

	// レチクル
	object3DReticle_.Initialize();
	object3DReticle_.SetModel("train.obj");

	// レーザー
	laser.Initialize();
	laser.SetModel("long.obj");

	// スプライト
	sprite2DReticle_ = std::make_unique<Sprite>();
	sprite2DReticle_->Initialize("resources/reticle.png");
	sprite2DReticle_->SetSize({ 10, 10 });
	sprite2DReticle_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	sprite2DReticle_->SetColor(Vector4{ 1,0,0,1 });

	spriteEnergy_ = std::make_unique<Sprite>();
	spriteEnergy_->Initialize("resources/white.png");
	spriteEnergy_->SetPosition(Vector2(20, 600));
	spriteEnergy_->SetSize({ 10, 10 });
	spriteEnergy_->SetAnchorPoint(Vector2{ 0.0f,0.0f });
	spriteEnergy_->SetColor(Vector4{ 1,0,0,1 });

	// 天球
	objectSkydome_.Initialize();
	objectSkydome_.SetModel("skydome.obj");

}
// 

#pragma endregion 初期化関係


#pragma region UpDate

// ImGui更新
void GamePlayScene::UpdateImGui()
{

#ifdef _DEBUG

	if (Input::GetInstance()->PushKey(DIK_UP)) {
		cameraDebugT.z += 0.5f;
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		cameraDebugT.z -= 0.5f;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		cameraDebugT.x += 0.5f;
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		cameraDebugT.x -= 0.5f;
	}

	camera->transform_.rotate = cameraDebugR;
	camera->transform_.translate = cameraDebugT;
	camera->UpdateMatrix();
	ImGui::Begin("Camera");
	ImGui::DragFloat3("cameraDebugT", &cameraDebugT.x, 0.1f);
	ImGui::DragFloat3("cameraDebugR", &cameraDebugR.x, 0.01f);
	ImGui::InputFloat3("Translate", &camera->transform_.translate.x);
	ImGui::DragFloat3("cameraT", &cameraT.x, 0.1f);
	ImGui::InputFloat3("Rotate", &camera->transform_.rotate.x);
	ImGui::DragFloat3("CameraR", &cameraR.x, 0.01f);
	ImGui::InputFloat3("cameraCa", &ca.transform.translate.x);
	ImGui::Checkbox("flag", &flag);
	ImGui::End();

	ImGui::Begin("train");
	{
		ImGui::SliderFloat("t", &move_t, 0.0f, 1.0f);
		ImGui::DragFloat("moveSpeed", &moveSpeed, 0.0001f, 0.0f, 0.0f, "%.6f");
		Vector3 pos = train.GetWorldPosition();
		Vector3 pos2 = trainTemp.GetWorldPosition();
		Vector3 pos3 = object3DReticle_.GetWorldPosition();
		ImGui::InputFloat3("tanslateW", &pos.x);
		ImGui::InputFloat3("trainTempW", &pos2.x);
		ImGui::InputFloat3("object3DReticle_W", &pos3.x);
		ImGui::InputInt("Score", &Score_);
		ImGui::InputFloat("energy", &energy_);
	}
	ImGui::End();

	//
	ImGui::Begin("controlPoints2");
	for (int i = 0; i < controlPoints2_.size(); ++i) {
		// ラベルをユニークにするために制御点のインデックスを使う
		std::string label = "Translate " + std::to_string(i);
		ImGui::DragFloat3(label.c_str(), &controlPoints2_[i].x, 0.1f);
	}
	ImGui::End();
	ImGui::Begin("railObject2");
	// 制御点の個々の編集
	for (int i = 0; i < railObject2.size(); ++i) {
		// ラベルをユニークにするために制御点のインデックスを使う
		std::string label = "Translate " + std::to_string(i);
		ImGui::DragFloat3(label.c_str(), &railObject2[i]->transform.translate.x, 0.1f);
	}
	ImGui::End();
#endif
}

// 調整項目
void GamePlayScene::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance(); 
	const char* gropName = "controlPoint"; 
	// グループを追加する 
	GlobalVariables::GetInstance()->CreateGroup(gropName); 
	for (int i = 0; i < controlPointObjects_.size(); i++) {
		std::string label = "Translate " + std::to_string(i); 
		controlPoints2_[i] = globalVariables->GetVector3Value(gropName, label);
		globalVariables->SetValue(gropName, label, controlPoints2_[i]);
	}

}
// レール更新
void GamePlayScene::UpdateRail()
{
	for (int i = 0; i < static_cast<int>(railObject2.size()); ++i) {
		float index = float(i) / float(railObject2.size());
		Vector3 pos = CatmullRom(controlPoints2_, index + 0.00001f);
		Vector3 pos2 = CatmullRom(controlPoints2_, index + 0.00002f);
		railObject2[i]->transform.translate = pos;

		// 進行方向のベクトルを計算
		Vector3 velocity = Subtract(pos2, pos);

		// カメラの回転を更新
		float rotateY = std::atan2(velocity.x, velocity.z);
		float length = Length(Vector3(velocity.x, 0, velocity.z));
		float rotateX = std::atan2(velocity.y, -length);

		// Z軸の回転を計算
		float rotateZ = 0.0f; // 初期値は0

		// Z軸を常に上方向に維持する
		railObject2[i]->transform.rotate = Vector3(rotateX, rotateY, rotateZ);
		railObject2[i]->Update();
	}

	// コントロールポイント位置
	for (int i = 0; i < static_cast<int>(controlPoints2_.size()); ++i) {
		controlPointObjects_[i]->transform.translate = controlPoints2_[i];
		controlPointObjects_[i]->Update();
	}
}
// トロッコ更新
void GamePlayScene::UpdateTrain()
{
	// スプライン上のカメラの移動
	move_t += moveSpeed;
	if (move_t >= 1.0f) {
		move_t = 1.0f;
	}
	else if (move_t <= 0.0f) {
		move_t = 0.0f;
	}

	// 列車の現在位置と進行方向を取得
	Vector3 pos = CatmullRom(controlPoints2_, move_t);
	Vector3 posNext = CatmullRom(controlPoints2_, move_t + 0.0001f);

	// 列車の進行方向ベクトルを計算
	Vector3 velocity = Normalize(Subtract(posNext, pos));

	// 目標速度を計算
	float targetSpeed;
	if (velocity.y > 0.01f) {
		// 上り坂（yが正の方向）
		targetSpeed = 0.0001f; // 上り坂での最小速度
	}
	else if (velocity.y < -0.01f) {
		// 下り坂（yが負の方向）
		targetSpeed = 0.0005f; // 下り坂での最大速度
	}
	else {
		// 平坦な場合（yの値がほぼゼロ）
		targetSpeed = 0.0002f; // 平坦なときの速度
	}

	// 現在の速度 `moveSpeed` を目標速度 `targetSpeed` に滑らかに補間
	float smoothingFactor = 0.01f; // 補間のスムーズさを調整（0に近いほど滑らかになる）
	moveSpeed = moveSpeed + (targetSpeed - moveSpeed) * smoothingFactor;


	// 列車の回転計算
	float rotateY = std::atan2(velocity.x, velocity.z);
	float horizontalLength = Length(Vector3(velocity.x, 0, velocity.z));
	float rotateX = std::atan2(-velocity.y, horizontalLength);

	// 列車の位置と回転を設定
	train.transform.translate = pos;
	train.transform.rotate = Vector3(rotateX, rotateY, 0);

	// 列車
	train.Update();
	// 位置だけトロッコと同じ
	trainTemp.Update();
}
// レティクル更新
void GamePlayScene::UpdateReticle()
{

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		reticlePos_.x -= 0.005f;
	}
	else if (input_->PushKey(DIK_D)) {
		reticlePos_.x += 0.005f;
	}
	if (input_->PushKey(DIK_W)) {
		reticlePos_.y -= 0.005f;
	}
	else if (input_->PushKey(DIK_S)) {
		reticlePos_.y += 0.005f;
	}

	if (reticlePos_.x >= 0.35f) {
		reticlePos_.x = 0.35f;
	}
	else if (reticlePos_.x <= -0.35f) {
		reticlePos_.x = -0.35f;
	}

	if (reticlePos_.y >= 0.17f) {
		reticlePos_.y = 0.17f;
	}
	else if (reticlePos_.y <= -0.2f) {
		reticlePos_.y = -0.2f;
	}

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	CalculationWorld3DReticlePosition();
	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	CalculationWorld2DReticlePosition();

	// スプライト
	sprite2DReticle_->Update();
}
// レーザー(エネルギー)更新
void GamePlayScene::UpdateLaser()
{
	// トロッコから出るレザーいち
	trainTemp.transform.translate = train.transform.translate;
	trainTemp.transform.rotate.x = train.transform.rotate.x + reticlePos_.y;
	trainTemp.transform.rotate.y = train.transform.rotate.y + reticlePos_.x;
	trainTemp.transform.rotate.z = train.transform.rotate.z;
	// レーザー
	laser.transform.translate = train.transform.translate;

	Vector3 velocityLaser = Subtract(train.transform.translate, object3DReticle_.transform.translate);
	// レーザーの回転を更新
	float rotateLY = std::atan2(velocityLaser.x, velocityLaser.z);
	float lengthL = Length(Vector3(velocityLaser.x, 0, velocityLaser.z));
	float rotateLX = std::atan2(velocityLaser.y, -lengthL);

	laser.transform.rotate = Vector3(rotateLX, rotateLY, laser.transform.rotate.z);


	// エネルギー
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		energy_ -= 0.5f;
	}
	else {
		energy_ += 0.5f;
	}
	if (energy_ > 100) {
		energy_ = 100;
	}
	else if (energy_ < 0) {
		energy_ = 0;
	}
	if (energy_ < 10) {
		laser.transform.scale.x = 0.2f;
		laser.transform.scale.y = 0.2f;
		spriteEnergy_->SetColor(Vector4(1, 0, 0, 1));
		damage_ = 1;
	}
	else {
		laser.transform.scale = { 1,1,2 };
		spriteEnergy_->SetColor(Vector4(0, 0, 1, 1));
		damage_ = 10;
	}
	spriteEnergy_->SetSize(Vector2(energy_ * 2, 30));

	// レーザー
	laser.Update();

	// スプライトエネルギー
	spriteEnergy_->Update();
}

// 更新処理
void GamePlayScene::Update()
{
	// 調整項目
	ApplyGlobalVariables();
	
	// ImGuiの更新
	UpdateImGui();

	/// レールカメラ
	// カメラの回転を設定
	if (flag) {
		camera->transform_ = cameraObj_.transform;
		camera->transform_.rotate = train.transform.rotate;

		// カメラ
		CalculationWorldCameraPosition();

		// 必要に応じて行列を更新
		camera->UpdateMatrix();
	}
	else {
#ifdef _DEBUG

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			cameraDebugT.z += 0.5f;
		}
		else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
			cameraDebugT.z -= 0.5f;
		}
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			cameraDebugT.x += 0.5f;
		}
		else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			cameraDebugT.x -= 0.5f;
		}

		camera->transform_.rotate = cameraDebugR;
		camera->transform_.translate = cameraDebugT;
		camera->UpdateMatrix();
#endif // _DEBUG
	}


	// レール更新
	UpdateRail();
	// トロッコ更新
	UpdateTrain();

	// レーザー更新
	UpdateLaser();
	// レティクル更新
	UpdateReticle();

	// 敵PoP情報
	UpdateEnemyPopCommands();
	// 敵
	for (const auto& enemy : enemys_) {
		enemy->Update(); // 参照を通してアクセス
	}
	objectSkydome_.Update();
	// 当たり判定
	ChekAllCollisions();

	
}

#pragma endregion //更新関係


#pragma region 

// カメラ位置
void GamePlayScene::CalculationWorldCameraPosition()
{
	// 自機から3Dレティクルのへの距離
	const float kDistancePlayerTo3DReticle = 1.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0, 2.0f, 1.0f };
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, train.mat_);
	// ベクトルの長さを整える
	offset = Multiply(Normalize(offset), kDistancePlayerTo3DReticle);
	// 3Dレティクルの座標を設定
	cameraObj_.transform.translate = Add(train.GetWorldPosition(), offset);
	//
	cameraObj_.Update();
}
// レティクル3D関係
void GamePlayScene::CalculationWorld3DReticlePosition()
{
	{
		// 自機から3Dレティクルのへの距離
		const float kDistancePlayerTo3DReticle = 160.0f;
		// 自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = { 0, 0, 1.0f };
		// 自機のワールド行列の回転を反映
		offset = TransformNormal(offset, trainTemp.mat_);
		// ベクトルの長さを整える
		offset = Multiply(Normalize(offset), kDistancePlayerTo3DReticle);
		// 3Dレティクルの座標を設定
		object3DReticle_.transform.translate = Add(train.GetWorldPosition(), offset);
		//
		object3DReticle_.Update();
	}
}
// レティクル2D関係
void GamePlayScene::CalculationWorld2DReticlePosition() {

	Vector3 positionReticle = object3DReticle_.GetWorldPosition();

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, float(WinApp::GetClientWidth()), float(WinApp::GetClientHeight()), 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(camera->viewMatrix_, Multiply(camera->projectionMatrix_, matViewport));

	// ワールド→スクリーン座標変換（ここで3Dから2Dになる）
	positionReticle = Transforms(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}
// 当たり判定
void GamePlayScene::ChekAllCollisions()
{
	bool isCollisionDetected = false;
	// 敵とレーザー当たり判定
	for (const auto& enemy : enemys_) {
		Sphere sphere = { enemy->GetObjectTrans().GetWorldPosition(), 3 };
		Vector3 origin = train.GetWorldPosition();
		Vector3 target = object3DReticle_.GetWorldPosition();
		Vector3 diff = Subtract(target, origin); // 差分ベクトルの計算

		Segment segment = { origin, diff };

		if (Input::GetInstance()->PushKey(DIK_RETURN)) {
			if (enemy->GetAlive() && IsCollision(sphere, segment)) {
				// 衝突が検出されたらフラグを立てる


				enemy->addDamage(damage_);

				if (!enemy->GetAlive()) {
					Score_ += 10;
				}

				isCollisionDetected = true;
			}
		}
	}


	// 衝突した場合は黒に、していない場合は元の色に戻す
	if (isCollisionDetected) {
		if (Input::GetInstance()->PushKey(DIK_RETURN)) {
			sprite2DReticle_->SetColor(Vector4(0, 0, 0, 1)); // 黒
		}
	}
	else {
		sprite2DReticle_->SetColor(Vector4(1, 1, 1, 1)); // 白（元の色）
	}

}
// 敵生成
void GamePlayScene::EnemyGenerate(Vector3 position, float HP)
{
	// Enemy オブジェクトを生成
	auto enemy_ = std::make_unique<Enemy>();
	// 初期化
	// 位置とHP
	enemy_->Initialize(position, HP);

	// 所有権を移動してリストに追加
	enemys_.push_back(std::move(enemy_));
}
//　CSV読み込み
void GamePlayScene::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}
// 敵のPoP設定
void GamePlayScene::UpdateEnemyPopCommands()
{
	//待機処理
	if (isWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			//待機終了
			isWait_ = false;
		}
	}

	// 1桁分の文字列を入れる変数
	std::string line;
	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			// HP
			getline(line_stream, word, ',');
			float hp = (float)std::atof(word.c_str());
			//敵を発生させる
			EnemyGenerate(Vector3{ x, y, z }, hp);
		}
		else if (word.find("WAIT") == 0) {
			//,区切りで行の先頭文字列を取得
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait_ = true;
			waitTimer_ = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}
// コントロールポイントの設置
std::vector<Vector3> GamePlayScene::GenerateSpiralControlPoints(float radius, float height, int numPoints, float turns)
{
	std::vector<Vector3> controlPoints;

	// 1回転あたりのラジアン（2π = 360度）
	float fullTurnRadians = 2.0f * 3.14159265359f;

	// 各制御点の座標を計算
	for (int i = 0; i < numPoints; ++i) {
		float t = float(i) / float(numPoints - 1);  // 0から1までの値を生成
		float angle = t * turns * fullTurnRadians;  // 回転角度
		float y = t * height;                       // Y軸の高さ
		float x = radius * std::cosf(angle);         // X座標
		float z = radius * std::sinf(angle);         // Z座標

		controlPoints.push_back({ x, y, z });
	}
	return controlPoints;
}
// コントロールポイントの設置
std::vector<Vector3> GamePlayScene::GenerateVerticalSpiralControlPoints(float radius, float height, int numPoints, float turns)
{
	std::vector<Vector3> controlPoints;

	// numPointsが1未満の場合は無効
	if (numPoints < 1) return controlPoints;

	// 1回転あたりのラジアン（2π = 360度）
	const float fullTurnRadians = 2.0f * 3.14159265359f;

	// 各制御点の座標を計算
	for (int i = 0; i < numPoints; ++i) {
		float t = static_cast<float>(i) / (numPoints - 1); // 0から1までの値を生成
		float angle = t * turns * fullTurnRadians;         // 回転角度
		float currentHeight = t * height;                   // Z軸の進み具合（高さ）

		// XとYで円形の動きを生成
		float x = radius * std::cosf(angle);                // X軸の円形回転
		float y = radius * std::sinf(angle);                // Y軸の円形回転

		// 新たな制御点をベクトルに追加
		controlPoints.emplace_back(x, y, currentHeight);
	}
	return controlPoints;
}

#pragma endregion その他

// 終了
void GamePlayScene::Finalize()
{

}

// 3D描画
void GamePlayScene::Draw3D()
{
	////3Dオブジェクトの描画

	// レール2
	for (int i = 0; i < railObject2.size(); ++i) {
		if (300 >= Distance(railObject2[i]->GetWorldPosition(), train.GetWorldPosition())) {
			railObject2[i]->Draw();
		}
	}

	// コントロールポイント位置描画
	for (int i = 0; i < static_cast<int>(controlPointObjects_.size()); ++i) {
		controlPointObjects_[i]->Draw();
	}

	// 敵
	for (const auto& enemy : enemys_) {
		if (enemy->GetAlive() && 300 >= Distance(enemy->GetObjectTrans().GetWorldPosition(), train.GetWorldPosition())) {
			enemy->Draw(); // 参照を通してアクセス
		}
	}

	// 列車
	train.Draw();

	// レチクル
	//object3DReticle_.Draw();

	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		// レーザー
		laser.Draw();
	}

	// 天球
	objectSkydome_.Draw();

}
// 2D描画
void GamePlayScene::Draw2D()
{
	//////////////--------スプライト-----------///////////////////

	sprite2DReticle_->Draw();


	spriteEnergy_->Draw();
}

