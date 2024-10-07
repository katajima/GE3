#include "MyGame.h"



void MyGame::Initialize()
{
	Framework::Initialize();
	
	// 入力初期化
	input = new Input();
	input->Intialize(winApp);

	//オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();



	



	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize(dxCommon, srvManager);
	//モデルマネージャー
	ModelManager::GetInstance()->Initialize(dxCommon);


	

	camera = new Camera();
	camera->SetRotate({ 0,0,0 });
	camera->SetTranslate({ 0,0,-20 });

	object3dCommon->SetDefaltCamera(camera);

	for (int i = 0;  i < static_cast<int>(MaxSprite); ++i) {
		Sprite* sprite = new Sprite();
		if (i % 2 == 0) {

			sprite->Initialize(spriteCommon, "resources/uvChecker.png");
		}
		else {

			sprite->Initialize(spriteCommon, "resources/monsterBall.png");
		}
		sprites.push_back(sprite);
	}

	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("axis2.obj");


	for (int i = 0;  i < static_cast<int>(MaxObject3d); ++i) {
		Object3d* object3d = new Object3d();
		object3d->Initialize(object3dCommon);
		if (i == 1) {

			object3d->SetModel("plane.obj");
			object3d->SetTranslate({ -0.136430234f,-0.876318157f,-0.0530188680f });
			object3d->SetTranslate({ 100,0,10 });
			object3d->SetRotate(Vector3(0, 3.14f, 0));
		}
		else {

			object3d->SetModel("axis2.obj");
			object3d->SetTranslate({ 0,0,10 });
			object3d->SetRotate(Vector3(-1.57f, 3.14f, 0));
		}

		object3ds.push_back(object3d);
	}

	particleManager = ParticleManager::GetInstance();
	particleManager->Initialize(dxCommon, srvManager);
	particleManager->CreateParticleGroup("aa", "resources/monsterBall.png");

	emitter = new ParticleEmitter("aa", { {1,1,1},{0,0,0},{0,0,0} }, 5, 0.5f, 0.0f);

}

void MyGame::Finalize()
{
	delete input;

	for (int i = 0; i < static_cast<int>(MaxSprite); ++i) {
		delete sprites[i];
	}
	for (int i = 0; i < static_cast<int>(MaxObject3d); ++i) {
		delete object3ds[i];
	}

	
	// 音
	audio->Finalize();
	// パーティクルマネージャーの終了
	particleManager->Finalize();
	//
	delete emitter;
	
	// ImGuiマネージャーの終了
	ImGuiManager::GetInstance()->Finalize();
	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();
	// モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();
	// 基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();
	// ImGuiの受付開始
	imguiManager->Begin();


	// Input
	input->Update();

	camera->Update();


	ImGui::Begin("Camera");
	ImGui::DragFloat3("Translate", &cameraPos.x, 0.1f);
	ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.01f);
	camera->SetTranslate(cameraPos);
	camera->SetRotate(cameraRotate);
	ImGui::End();

	//// 3Dモデル
	for (int i = 0;  i < static_cast<int>(MaxObject3d); ++i) {

		object3ds[i]->Update();
		object3ds[i]->SetCamera(object3dCommon->GetDefaltCamera());
		object3ds[i]->SetScale({ 1,1,1 });

		if (i == 0) {

			//object3ds[i]->SetRotate(Add(object3ds[i]->GetRotate(), Vector3{ 0.0f, 0.02f, 0.0f }));


			ImGui::Begin("Mash Axis");

			axisPos = object3ds[i]->GetTranslate();
			axisRotate = object3ds[i]->GetRotate();
			ImGui::DragFloat3("Translate", &axisPos.x, 0.1f);
			ImGui::DragFloat3("Rotate", &axisRotate.x, 0.01f);
			object3ds[i]->SetTranslate(axisPos);
			object3ds[i]->SetRotate(axisRotate);



			// モデルデータへのアクセス
			Model::ModelData& mode = object3ds[i]->GetModel()->GetModelData();

			// 頂点数とインデックス数をUIに表示
			int vertexSize = static_cast<int>(mode.vertices.size());
			ImGui::InputInt("vertices.size", &vertexSize);
			int indexSize = static_cast<int>(mode.indices.size());
			ImGui::InputInt("indices.size", &indexSize);

			// インデックスを使って頂点を操作
			for (size_t j = 0; j < mode.indices.size(); j++) {
				uint32_t index = mode.indices[j];  // インデックスで頂点を取得
				Model::VertexData& ver = mode.vertices[index];  // 頂点データを参照

				// ユニークなラベルを付与し、ImGuiで位置を操作可能に
				if (ImGui::DragFloat3(("pos " + std::to_string(j)).c_str(), &ver.position.x, 0.01f)) {
					// 値が変更された場合、モデルデータを更新
					object3ds[i]->GetModel()->SetModelData(mode);
				}
			}


			ImGui::End();
		}
		if (i == 1) {

			// モデルの回転を更新
			//object3ds[i]->SetRotate(Add(object3ds[i]->GetRotate(), Vector3{ 0.0f, 0.02f, 0.0f }));

			ImGui::Begin("Mash Plane");
			Model::ModelData& mode = object3ds[i]->GetModel()->GetModelData();

			// 頂点数とインデックス数を表示
			int vertexSize = static_cast<int>(mode.vertices.size());
			ImGui::InputInt("vertices.size", &vertexSize);
			int indexSize = static_cast<int>(mode.indices.size());
			ImGui::InputInt("indices.size", &indexSize);

			// インデックスを使って頂点を操作
			for (size_t j = 0; j < mode.indices.size(); j++) {
				uint32_t index = mode.indices[j];  // インデックスで頂点を取得
				Model::VertexData& ver = mode.vertices[index];  // 頂点データを参照

				// ユニークなラベルを付与して、ImGuiで位置を操作可能にする
				if (ImGui::DragFloat3(("pos " + std::to_string(j)).c_str(), &ver.position.x, 0.01f)) {
					// 値が変更された場合、モデルデータを更新
					object3ds[i]->GetModel()->SetModelData(mode);
				}
			}

			ImGui::End();
		}
	}

	// スプライト
	for (int i = 0;  i < static_cast<int>(MaxSprite); ++i) {
		sprites[i]->Update();
	}

	// パーティクルの更新
	//particleManager->Update();

	//emitter->Update();

	// ImGuiの受付終了
	imguiManager->End();
}

void MyGame::Draw()
{
	// 描画前処理
	srvManager->PreDraw();
	dxCommon->PreDraw();

	//////////////---------3Dモデル-------------///////////////

	//// 3Dオブジェクトの描画準備
	object3dCommon->DrawCommonSetting();

	////3Dオブジェクトの描画
	for (int i = 0;  i < static_cast<int>(MaxObject3d); ++i) {
		object3ds[i]->Draw();
	}

	//// パーティクルの描画
	//particleManager->Draw();

	//////////////--------スプライト-----------///////////////////


	// 2Dオブジェクトの描画準備
	spriteCommon->DrawCommonSetting();


	// 2Dオブジェクトの描画
	for (int i = 0;  i < static_cast<int>(MaxSprite); ++i) {
		//sprites[i]->Draw();
	}

	// ImGuiの描画
	imguiManager->Draw();

	//描画後処理
	dxCommon->PostDraw();
}
