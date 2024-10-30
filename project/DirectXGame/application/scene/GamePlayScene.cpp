#include "GamePlayScene.h"
#include <iostream>//用いるヘッダファイルが変わります。

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


	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

	// スプライト
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


	// レールかんけい
	InitializeRail();


	// 列車オブジェクトを unique_ptr で作成
	train = std::make_unique<Object3d>();
	train->Initialize();
	train->SetModel("train.obj");
	train->transform.translate = CatmullRom(controlPoints_, 1);

	// 建物
	for (int i = 0; i < static_cast<int>(MaxBuildingObject3d); ++i) {
		// unique_ptr で Object3d を作成
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize();
		object3d->SetModel("building.obj");

		buildingObject.push_back(std::move(object3d));
	}


	buildingObject[0]->transform.translate = Vector3{ 50,0,50 };
	buildingObject[1]->transform.translate = Vector3{ -30,0,30 };
	buildingObject[2]->transform.translate = Vector3{ -30,0,-30 };
	buildingObject[3]->transform.translate = Vector3{ 30,0,-30 };
	buildingObject[4]->transform.translate = Vector3{ 30,0,-60 };

	
	SetParent(&ca);

	//train->parent_ = //(camera->GetWorldMatrix());

}

void GamePlayScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	camera->transform.rotate = { 0.36f,0,0 };
	camera->transform.translate = { 5,32.5f,-59.2f };

	cameraDebugT = camera->transform.translate;
	cameraDebugR = camera->transform.rotate;

	cameraT.y = 1.0f;

	//ca.transform = camera->transform;
}

void GamePlayScene::InitializeRail()
{
	// スパイラルのパラメータ
	float radius = 10.0f;  // 半径
	float height = 50.0f;  // 全体の高さ
	int numPoints = MaxRailObject;   // 制御点の数
	float turns = 7.0f;    // 回転数 (スパイラルの巻き数)

	// スパイラル制御点を生成
	//std::vector<Vector3> controlPoints;
	controlPoints_ = GenerateSpiralControlPoints(radius, height, numPoints, turns);

	// レール
	for (int i = 0; i < controlPoints_.size(); ++i) {
		// unique_ptr で Object3d を作成
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize();

		float index = float(i) / float(controlPoints_.size());

		Vector3 pos = CatmullRom(controlPoints_, index + 0.00001f);
		Vector3 pos2 = CatmullRom(controlPoints_, index + 0.00002f);

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


	controlPoints2_ = {
		{10,10,10},
		{50,20,20},
		{90,200,30},
		{130,40,10},
		{170,50,-10},
		{210,60,10},
		{250,30,-40},
		{290,10,10},
	};

	float spacing = 3.5f; // オブジェクト間の距離（密度調整）
	float totalDistanceTraveled = 0.0f; // 累積距離

	for (size_t i = 0; i < controlPoints2_.size() - 1; ++i) {
		Vector3 start = controlPoints2_[i];
		Vector3 end = controlPoints2_[i + 1];

		float segmentLength = Length(Subtract(end, start));
		float distanceTraveled = 0.0f;

		// セグメント内で等間隔にオブジェクトを配置
		while (distanceTraveled < segmentLength) {
			// 各セグメントの位置に基づくtの計算
			float t = (totalDistanceTraveled + distanceTraveled) / (segmentLength * (controlPoints2_.size() - 1));

			// CatmullRom関数で位置を補間
			Vector3 pos = CatmullRom(controlPoints2_, t);

			// 進行方向のベクトルを計算
			Vector3 posNext = CatmullRom(controlPoints2_, t + 0.01f);
			Vector3 velocity = Subtract(posNext, pos);


			//velocity = Normalize(velocity);

			// 回転計算
			float rotateY = std::atan2(velocity.x, velocity.z);
			float length = Length(Vector3(velocity.x, 0, velocity.z));
			float rotateX = std::atan2(velocity.y, -length);

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

		// セグメントの終了時に累積距離を更新
		totalDistanceTraveled += segmentLength;
	}







	AllrailObject = std::make_unique<Object3d>();
	AllrailObject->InitializeInstance(MaxRailObject);
	AllrailObject->SetModel("rail.obj");

	// レール インスタンス
	for (int i = 0; i < static_cast<int>(MaxRailObject); ++i) {
		float index = float(i) / float(MaxRailObject);

		Vector3 pos = CatmullRom(controlPoints_, index + 0.00001f);
		Vector3 pos2 = CatmullRom(controlPoints_, index + 0.00002f);

		AllrailObject->transforms[i].translate = pos;


		//AllrailObject->transforms[i].translate = Vector3{ float(i),float(i),float(i) };
		// 進行方向のベクトルを計算
		Vector3 velocity = Subtract(pos2, pos);

		// カメラの回転を更新
		float rotateY = std::atan2(velocity.x, velocity.z);
		float length = Length(Vector3(velocity.x, 0, velocity.z));
		float rotateX = std::atan2(velocity.y, -length);

		AllrailObject->transforms[i].rotate = Vector3(rotateX, rotateY, 0);
	}
}


void GamePlayScene::Finalize()
{

}

void GamePlayScene::UpdateImGui()
{
	ImGui::Begin("Camera");
	ImGui::DragFloat3("cameraDebugT", &cameraDebugT.x, 0.1f);
	ImGui::DragFloat3("cameraDebugR", &cameraDebugR.x, 0.01f);
	ImGui::InputFloat3("Translate", &camera->transform.translate.x);
	ImGui::DragFloat3("cameraT", &cameraT.x, 0.1f);
	ImGui::InputFloat3("Rotate", &camera->transform.rotate.x);
	ImGui::DragFloat3("CameraR", &cameraR.x, 0.01f);
	ImGui::InputFloat3("cameraCa", &ca.transform.translate.x);
	ImGui::Checkbox("flag", &flag);
	ImGui::End();

	ImGui::Begin("train");
	ImGui::SliderFloat("t", &move_t, 0.0f, 1.0f);
	ImGui::DragFloat("moveSpeed", &moveSpeed, 0.0001f, 0.0f, 0.0f, "%.6f");
	ImGui::InputFloat3("tanslate", &train->transform.translate.x);
	Vector3 pop = GetWorldPos();
	ImGui::InputFloat3("tanslate2", &pop.x);
	ImGui::End();

	ImGui::Begin("controlPoints");
	// 制御点の個々の編集
	for (int i = 0; i < controlPoints_.size(); ++i) {
		// ラベルをユニークにするために制御点のインデックスを使う
		std::string label = "Translate " + std::to_string(i);
		ImGui::DragFloat3(label.c_str(), &controlPoints_[i].x, 0.1f);
	}
	ImGui::End();
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


	ImGui::Begin("AllrailObject");
	for (int i = 0; i < controlPoints_.size(); ++i) {
		std::string label = "Translate " + std::to_string(i);
		ImGui::DragFloat3(label.c_str(), &AllrailObject->transforms[i].translate.x, 0.0001f, 0.0f, 0.0f);
	}
	ImGui::End();
}

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
}

void GamePlayScene::Update()
{
	// ImGui
	UpdateImGui();

	// パーティクルの更新
	//particleManager->Update();

	camera->Update();





	move_t += moveSpeed;
	move_t2 = move_t + 0.01f;
	if (move_t >= 1.0f) {
		move_t = 1.0f;
	}
	if (move_t <= 0.0f) {
		move_t = 0.0f;
	}
	// スプライン上の位置を取得
	Vector3 pos = CatmullRom(controlPoints2_, move_t);
	Vector3 pos2 = CatmullRom(controlPoints2_, move_t + 0.01f);

	// 進行方向のベクトルを計算
	Vector3 velocity = Subtract(pos2, pos);

	// カメラの回転を更新
	float rotateY = std::atan2(velocity.x, velocity.z);
	float length = Length(Vector3(velocity.x, 0, velocity.z));
	float rotateX = std::atan2(-velocity.y, length);

	// カメラの位置と回転を設定
	if (flag) {

		//Vector3 CameraPos = Vector3(pos.x + cameraT.x, pos.y + cameraT.y, pos.z + cameraT.z);
		//Vector3 CameraRot = Vector3(rotateX + cameraR.x, rotateY + cameraR.y, 0 + cameraR.z);
		//camera->worldMatrix = MakeAffineMatrixMatrix({1,1,1}, CameraRot,CameraPos)

		/*camera->worldMatrix.m[3][0] = pos.x;
		camera->worldMatrix.m[3][1] = pos.y;
		camera->worldMatrix.m[3][2] = pos.z;*/

		//camera->transform.translate.y = 1.0f;

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
	// 列車レール
	for (int i = 0; i < static_cast<int>(controlPoints_.size()); ++i) {
		float index = float(i) / float(controlPoints_.size());
		Vector3 pos = CatmullRom(controlPoints_, index + 0.00001f);
		Vector3 pos2 = CatmullRom(controlPoints_, index + 0.00002f);
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



	UpdateRail();




	// ビル
	for (int i = 0; i < static_cast<int>(MaxBuildingObject3d); ++i) {
		//buildingObject[i]->Update();
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
	for (int i = 0; i < static_cast<int>(controlPoints_.size()); ++i) {
		//railObject[i]->Draw();
	}

	// レール2
	for (int i = 0; i < railObject2.size(); ++i) {
		railObject2[i]->Draw();
	}

	// 建物
	for (int i = 0; i < static_cast<int>(MaxBuildingObject3d); ++i) {
		//buildingObject[i]->Draw();
	}


	//AllrailObject->DrawInstance();

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


