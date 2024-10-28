#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	//オーディオの初期化
	audio_ = Audio::GetInstance();
	// 入力初期化
	input_ = Input::GetInstance();
	
	// カメラ
	InitializeCamera();
	// リソース
	InitializeResources();
}

void GamePlayScene::InitializeResources()
{
	
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/train.png");
	TextureManager::GetInstance()->LoadTexture("resources/rail.png");

	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

	for (int i = 0; i < static_cast<int>(MaxSprite); ++i) {
		auto sprite = std::make_unique<Sprite>();
		if (i % 2 == 0) {

			sprite->Initialize("resources/uvChecker.png");
		}
		else {

			sprite->Initialize("resources/monsterBall.png");
		}
		sprites.push_back(std::move(sprite));
	}

	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("axis2.obj");
	ModelManager::GetInstance()->LoadModel("train.obj");
	ModelManager::GetInstance()->LoadModel("rail.obj");
	ModelManager::GetInstance()->LoadModel("building.obj");


	// スパイラルのパラメータ
	float radius = 10.0f;  // 半径
	float height = 50.0f;  // 全体の高さ
	int numPoints = MaxRailObject;   // 制御点の数
	float turns = 7.0f;    // 回転数 (スパイラルの巻き数)

	// スパイラル制御点を生成
	//std::vector<Vector3> controlPoints;
	controlPoints_ = GenerateSpiralControlPoints(radius, height, numPoints, turns);


	for (int i = 0; i < static_cast<int>(MaxRailObject); ++i) {
		// unique_ptr で Object3d を作成
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize();

		float index = float(i) / float(MaxRailObject);

		Vector3 pos = Catmullom(controlPoints_, index + 0.00001f);
		Vector3 pos2 = Catmullom(controlPoints_, index + 0.00002f);

		object3d->SetModel("rail.obj");
		object3d->transform.translate = pos;



		// 進行方向のベクトルを計算
		Vector3 velocity = Subtract(pos2, pos);

		// カメラの回転を更新
		float rotateY = std::atan2(velocity.x, velocity.z);
		float length = Length(Vector3(velocity.x, 0, velocity.z));
		float rotateX = std::atan2(velocity.y, -length);

		object3d->transform.rotate = Vector3(rotateX, rotateY, 0);

		// railObject に unique_ptr を追加
		railObject.push_back(std::move(object3d));
	}

	// 列車オブジェクトを unique_ptr で作成
	train = std::make_unique<Object3d>();
	train->Initialize();
	train->SetModel("train.obj");
	train->transform.translate = Catmullom(controlPoints_, 1);


	for (int i = 0; i < static_cast<int>(MaxBuildingObject3d); ++i) {
		// unique_ptr で Object3d を作成
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize();
		object3d->SetModel("building.obj");

		buildingObject.push_back(std::move(object3d));

	}


	buildingObject[0]->transform.translate = Vector3{ 30,0,30 };
	buildingObject[1]->transform.translate = Vector3{ -30,0,30 };
	buildingObject[2]->transform.translate = Vector3{ -30,0,-30 };
	buildingObject[3]->transform.translate = Vector3{ 30,0,-30 };
	buildingObject[4]->transform.translate = Vector3{ 30,0,-60 };

}

void GamePlayScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	camera->transform.rotate = { 0.36f,0,0 };
	camera->transform.translate = { 5,32.5f,-59.2f };

	cameraDebugT = camera->transform.translate;
	cameraDebugR = camera->transform.rotate;

	cameraT.y = 1.0f;
}


void GamePlayScene::Finalize()
{
	
}

void GamePlayScene::Update()
{
	
	camera->Update();


	ImGui::Begin("Camera");
	ImGui::DragFloat3("cameraDebugT", &cameraDebugT.x, 0.1f);
	ImGui::DragFloat3("cameraDebugR", &cameraDebugR.x, 0.01f);
	ImGui::InputFloat3("Translate", &camera->transform.translate.x);
	ImGui::DragFloat3("cameraT", &cameraT.x, 0.1f);
	ImGui::InputFloat3("Rotate", &camera->transform.rotate.x);
	ImGui::DragFloat3("CameraR", &cameraR.x, 0.01f);
	ImGui::Checkbox("flag", &flag);
	ImGui::End();
	ImGui::Begin("t");
	ImGui::SliderFloat("t", &move_t, 0.0f, 1.0f);
	ImGui::DragFloat("moveSpeed", &moveSpeed, 0.0001f, 0.0f, 0.0f, "%.6f");
	ImGui::End();
	ImGui::Begin("controlPoints");
	// 制御点の個々の編集
	for (int i = 0; i < controlPoints_.size(); ++i) {
		// ラベルをユニークにするために制御点のインデックスを使う
		std::string label = "Translate " + std::to_string(i);
		ImGui::DragFloat3(label.c_str(), &controlPoints_[i].x, 0.1f);
	}
	ImGui::End();




	move_t += moveSpeed;
	move_t2 = move_t + 0.01f;
	if (move_t >= 1.0f) {
		move_t = 1.0f;
	}
	if (move_t <= 0.0f) {
		move_t = 0.0f;
	}
	// スプライン上の位置を取得
	Vector3 pos = Catmullom(controlPoints_, move_t);
	Vector3 pos2 = Catmullom(controlPoints_, move_t + 0.01f);

	// 進行方向のベクトルを計算
	Vector3 velocity = Subtract(pos2, pos);

	// カメラの回転を更新
	float rotateY = std::atan2(velocity.x, velocity.z);
	float length = Length(Vector3(velocity.x, 0, velocity.z));
	float rotateX = std::atan2(-velocity.y, length);

	// カメラの位置と回転を設定
	if (flag) {
		camera->transform.translate = Vector3(pos.x + cameraT.x, pos.y + cameraT.y, pos.z + cameraT.z);
		camera->transform.rotate = Vector3(rotateX + cameraR.x, rotateY + cameraR.y, 0 + cameraR.z);
	}
	else {
		camera->transform.rotate = cameraDebugR;
		camera->transform.translate = cameraDebugT;
	}


	train->transform.translate = pos;
	train->transform.rotate = Vector3(rotateX, rotateY, 0);



	//// 3Dモデル
	for (int i = 0; i < static_cast<int>(MaxRailObject); ++i) {
		float index = float(i) / float(MaxRailObject);
		Vector3 pos = Catmullom(controlPoints_, index + 0.00001f);
		Vector3 pos2 = Catmullom(controlPoints_, index + 0.00002f);
		railObject[i]->transform.translate = pos;

		// 進行方向のベクトルを計算
		Vector3 velocity = Subtract(pos2, pos);

		// カメラの回転を更新
		float rotateY = std::atan2(velocity.x, velocity.z);
		float length = Length(Vector3(velocity.x, 0, velocity.z));
		float rotateX = std::atan2(velocity.y, -length);

		// Z軸の回転を計算
		float rotateZ = 0.0f; // 初期値は0

		//// ここでZ軸の回転を計算する
		//// オブジェクトの前方ベクトルを計算
		//Vector3 forward;
		//forward.x = cos(rotateY) * cos(rotateX);
		//forward.y = sin(rotateX);
		//forward.z = sin(rotateY) * cos(rotateX);

		//// Z軸回転を計算（ベクトルから導出）
		//rotateZ = std::atan2(forward.y, forward.z); // Z軸のロール

		// Z軸を常に上方向に維持する
		railObject[i]->transform.rotate = Vector3(rotateX, rotateY, rotateZ);
		railObject[i]->Update();
	}


	for (int i = 0; i < static_cast<int>(MaxBuildingObject3d); ++i) {
		buildingObject[i]->Update();
	}


	// 列車
	train->Update();

	// スプライト
	for (int i = 0; i < static_cast<int>(MaxSprite); ++i) {
		//sprites[i]->Update();
	}
}

void GamePlayScene::Draw3D()
{	
	////3Dオブジェクトの描画

	// レール
	for (int i = 0; i < static_cast<int>(MaxRailObject); ++i) {
		railObject[i]->Draw();
	}

	// 建物
	for (int i = 0; i < static_cast<int>(MaxBuildingObject3d); ++i) {
		buildingObject[i]->Draw();
	}

	// 列車
	train->Draw();

	//// パーティクルの描画
	//particleManager->Draw();
}
void GamePlayScene::Draw2D() 
{
	//////////////--------スプライト-----------///////////////////





// 2Dオブジェクトの描画
	for (int i = 0; i < static_cast<int>(MaxSprite); ++i) {
		//sprites[i]->Draw();
	}
}


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


