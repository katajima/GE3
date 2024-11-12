#include "GamePlayScene.h"
#include <iostream>//用いるヘッダファイルが変わります。
#include <corecrt_math_defines.h>
#include <algorithm>

#pragma region Initialize

// 初期化
void GamePlayScene::Initialize()
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
		{50.0f, -33.0f, 130.0f},
		{50.0f, -33.0f, 130.0f},
	};
	// カメラ
	InitializeCamera();
	//オーディオの初期化
	audio_ = Audio::GetInstance();
	// 入力初期化
	input_ = Input::GetInstance();

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


	// レールかんけい
	InitializeRail();

	// リソース
	InitializeResources();

	UpdateRail();
}

// レール初期化
void GamePlayScene::InitializeRail()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "controlPoint";
	globalVariables->CreateGroup(groupName);
	for (int i = 0; i < controlPoints2_.size(); i++) {
		std::string label = "Translate " + std::to_string(i);
		globalVariables->AddItem(groupName, label, controlPoints2_[i]);
		controlPoints2_[i] = globalVariables->GetVector3Value(groupName, label);
	}


	//PlaceObjectsAlongSpline(controlPoints2_, 0.1f);

	PlaceObjectsOnCurve(controlPoints2_, 0.1f);



	for (int i = 0; i < controlPoints2_.size(); ++i) {
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize();
		object3d->SetModel("Sphere.obj");
		object3d->SetCamera(camera.get());
		object3d->transform.translate = controlPoints2_[i];
		controlPointObjects_.push_back(std::move(object3d));
	}

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

	flag = false;

#endif // _DEBUG]


	cameraObj_.Initialize();
}
// 各オブジェクトやスプライトなどの初期化
void GamePlayScene::InitializeResources()
{


	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

	// 列車オブジェクトを unique_ptr で作成
	train.Initialize();
	train.SetModel("train.obj");
	train.transform.translate = CatmullRom(controlPoints2_, 1);
	train.SetCamera(camera.get());
	trainTemp.Initialize();
	trainTemp.transform.translate = train.transform.translate;

	// レチクル
	object3DReticle_.Initialize();
	object3DReticle_.SetModel("train.obj");

	// レーザー
	laser.Initialize();
	laser.SetModel("long.obj");
	laser.SetCamera(camera.get());
	// スプライト
	sprite2DReticle_ = std::make_unique<Sprite>();
	sprite2DReticle_->Initialize("resources/reticle.png");
	sprite2DReticle_->SetSize({ 10, 10 });
	sprite2DReticle_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	sprite2DReticle_->SetColor(Vector4{ 1,0,0,1 });

	spriteEnergy_ = std::make_unique<Sprite>();
	spriteEnergy_->Initialize("resources/white.png");
	spriteEnergy_->SetPosition(Vector2(20, 550));
	spriteEnergy_->SetSize({ 10, 10 });
	spriteEnergy_->SetAnchorPoint(Vector2{ 0.0f,0.0f });
	spriteEnergy_->SetColor(Vector4{ 1,0,0,1 });

	// 天球
	objectSkydome_.Initialize();
	objectSkydome_.SetModel("skydome.obj");
	objectSkydome_.SetCamera(camera.get());

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 10; i++) {
			std::string label = "resources/num/" + std::to_string(i) + ".png";
			spriteScoreNum_[j][i] = std::make_unique<Sprite>();
			spriteScoreNum_[j][i]->Initialize(label);
			spriteScoreNum_[j][i]->SetSize({ 50, 50 });
			spriteScoreNum_[j][i]->SetPosition({ -10, -10 });
		}
	}


	spriteEnter_ = std::make_unique<Sprite>();
	spriteEnter_->Initialize("resources/enter.png");
	spriteEnter_->SetPosition(Vector2(20, 600));
	spriteEnter_->SetSize({ 400, 120 });
	spriteEnter_->SetAnchorPoint(Vector2{ 0.0f,0.0f });

}
// 

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
#ifdef _DEBUG
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
		ImGui::SliderFloat("t", &move_t, 0.0f, 1.0f, "%.6f");
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
		move_t = 0.9999f;
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
		reticlePos_.x -= 0.01f;
	}
	else if (input_->PushKey(DIK_D)) {
		reticlePos_.x += 0.01f;
	}
	if (input_->PushKey(DIK_W)) {
		reticlePos_.y -= 0.01f;
	}
	else if (input_->PushKey(DIK_S)) {
		reticlePos_.y += 0.01f;
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
		damage_ = 10;
	}
	else {
		laser.transform.scale = { 1,1,2 };
		spriteEnergy_->SetColor(Vector4(0, 0, 1, 1));
		damage_ = 100;
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
		if (Input::GetInstance()->PushKey(DIK_O)) {
			cameraDebugT.y += 0.5f;
		}
		else if (Input::GetInstance()->PushKey(DIK_L)) {
			cameraDebugT.y -= 0.5f;
		}
#endif // _DEBUG
		camera->transform_.rotate = cameraDebugR;
		camera->transform_.translate = cameraDebugT;
		camera->UpdateMatrix();
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
	//UpdateEnemyPopCommands();
	// 敵
	for (int i = 0; i < enemys_.size(); i++) {
		enemys_[i]->Update();
	}
	objectSkydome_.Update();
	// 当たり判定
	ChekAllCollisions();



	if(move_t >= 0.999f) {
		t--;
	}
	if (t <= 0) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

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
					Score_ += 40;
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
// 
void GamePlayScene::PlaceObjectsAlongSpline(const std::vector<Vector3>& controlPoints, float spacing)
{
	const int baseSamples = 200;
	auto samplePoints = AdaptiveSampling(controlPoints, baseSamples);

	// 累積距離を計算して均等な間隔で配置
	float totalLength = samplePoints.back();
	float currentLength = 0.0f;

	for (float length = 0.0f; length < totalLength; length += spacing) {
		float t = length / totalLength;
		Vector3 pos = CatmullRom(controlPoints, t);

		// オブジェクトの回転計算
		float deltaT = 0.001f;
		Vector3 posNext = CatmullRom(controlPoints, (std::min)(t + deltaT, 1.0f));
		Vector3 velocity = Normalize(Subtract(posNext, pos));

		float rotateY = std::atan2(velocity.x, velocity.z);
		float length2D = Length(Vector3(velocity.x, 0, velocity.z));
		float rotateX = std::atan2(velocity.y, length2D);

		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize();
		object3d->SetModel("rail.obj");
		object3d->SetCamera(camera.get());
		object3d->transform.translate = pos;
		object3d->transform.rotate = Vector3(rotateX, rotateY, 0);
		railObject2.push_back(std::move(object3d));

		currentLength += spacing;
	}
}

// カーブに沿ったオブジェクトの配置
void GamePlayScene::PlaceObjectsOnCurve(const std::vector<Vector3>& controlPoints, float spacing) {
	float totalLength = 0.0f;
	const int segments = 200;
	std::vector<Vector3> points;

	// カーブ全体の長さを計算し、サンプリング
	for (int i = 0; i < segments; ++i) {
		float t = static_cast<float>(i) / segments;
		points.push_back(CatmullRom2(controlPoints, t));
		if (i > 0) {
			totalLength += Length(Subtract(points[i], points[i - 1]));
		}
	}

	float currentLength = 0.0f;
	for (float length = 0.0f; length < totalLength; length += spacing) {
		// 現在のアーク長に対応するt値を探索
		float t = length / totalLength;
		Vector3 position = CatmullRom2(controlPoints, t);

		// 向きを計算
		Vector3 posNext = CatmullRom2(controlPoints, (std::min)(t + 0.01f, 1.0f));
		Vector3 forward = Normalize(Subtract(posNext, position));

		float rotateY = std::atan2(forward.x, forward.z);
		float length2D = Length({ forward.x, 0, forward.z });
		float rotateX = std::atan2(forward.y, length2D);

		// オブジェクトの配置
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize();
		object3d->SetModel("rail.obj");
		object3d->SetCamera(camera.get());
		object3d->transform.translate = position;
		object3d->transform.rotate = Vector3{ rotateX, rotateY, 0 };
		railObject2.push_back(std::move(object3d));
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

	// 天球
	objectSkydome_.Draw();


	// レール2
	for (int i = 0; i < railObject2.size(); ++i) {
		if (300 >= Distance(railObject2[i]->GetWorldPosition(), train.GetWorldPosition())) {
			railObject2[i]->Draw();
		}
	}

#ifdef _DEBUG
	// コントロールポイント位置描画
	for (int i = 0; i < static_cast<int>(controlPointObjects_.size()); ++i) {
		controlPointObjects_[i]->Draw();
	}
#endif

	// 敵
	for (int i = 0; i < enemys_.size(); i++) {
		if (enemys_[i]->GetAlive()) {
			enemys_[i]->Draw();
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


}
// 2D描画
void GamePlayScene::Draw2D()
{
	//////////////--------スプライト-----------///////////////////

	sprite2DReticle_->Draw();


	spriteEnergy_->Draw();


	int numDigits = (Score_ == 0) ? 1 : static_cast<int>(log10(Score_)) + 1;

	int divisor = 1;
	for (int j = 0; j < numDigits; j++) {
		int digit = (Score_ / divisor) % 10;
		divisor *= 10;

		for (int i = 0; i < 10; i++) {
			if (digit == i) {
				spriteScoreNum_[j][i]->SetPosition(Vector2{ 1000.0f - (j * 50), 650.0f });
				spriteScoreNum_[j][i]->Update();
				spriteScoreNum_[j][i]->Draw();
			}
		}
	}

	spriteEnter_->Update();
	spriteEnter_->Draw();

}

