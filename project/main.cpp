#include<Windows.h>
#include"winuser.h"
#include<cstdint>
#include<string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include<vector>

#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/input/Input.h"
#include"DirectXGame/engine/base/WinApp.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"DirectXGame/engine/base/D3DResourceLeakchecker.h"
#include"DirectXGame/engine/2d/Sprite.h"
#include"DirectXGame/engine/2d/SpriteCommon.h"
#include"DirectXGame/engine/base/TextureManager.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include"DirectXGame/engine/3d/Model.h"
#include"DirectXGame/engine/3d/ModelCommon.h"
#include"DirectXGame/engine/3d/ModelManager.h"
#include"DirectXGame/engine/base/Camera.h"
#include"DirectXGame/engine/base/SrvManager.h"
#include"DirectXGame/engine/base/ParticleManager.h"
#include"DirectXGame/engine/base/ParticleEmitter.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/audio/Audio.h"
#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include"DirectXGame/engine/base/StringUtility.h"
#include"DirectXGame/engine/base/Logger.h"


#include"externals/imgui/imgui.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakchecker leakCheck;

	//COMの初期化
	//CoInitializeEx(0, COINIT_MULTITHREADED);


	WinApp* winApp = nullptr;
	// WindowsAPI解放
	winApp = new WinApp();
	winApp->Initialize();

	//ポインタ
	Input* input = nullptr;
	// 入力初期化
	input = new Input();
	input->Intialize(winApp);

	Audio* audio = nullptr;
	//オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();
	

	uint32_t sound = audio->LoadWave("/Windows/Media/Alarm01.wav");


	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Intialize(winApp);

	SrvManager* srvManager = nullptr;
	// SRVマネージャの初期化
	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

	// ImGuiマネージャー
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(winApp, dxCommon);


	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize(dxCommon, srvManager);

	//モデルマネージャー
	ModelManager::GetInstance()->Initialize(dxCommon);

	SpriteCommon* spriteCommon = nullptr;
	// スプライト共通部の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Camera* camera = new Camera();
	camera->SetRotate({ 0,0,0 });
	camera->SetTranslate({ 0,0,-20 });


	Object3dCommon* object3dCommon = nullptr;
	// 3Dオブジェクト共通部分の初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);
	object3dCommon->SetDefaltCamera(camera);

	ModelCommon* modelCommon = nullptr;
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);

	std::vector<Sprite*> sprites;
	const int MaxSprite = 1;
	for (uint32_t i = 0; i < MaxSprite; ++i) {
		Sprite* sprite = new Sprite();
		if (i % 2 == 0) {

			sprite->Initialize(spriteCommon, "resources/uvChecker.png");
		}
		else {

			sprite->Initialize(spriteCommon, "resources/monsterBall.png");
		}
		sprites.push_back(sprite);
	}



	const int MaxObject3d = 2;

	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("axis2.obj");

	std::vector<Object3d*> object3ds;
	for (uint32_t i = 0; i < MaxObject3d; ++i) {
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

	ParticleManager* particleManager = ParticleManager::GetInstance();
	particleManager->Initialize(dxCommon, srvManager);
	particleManager->CreateParticleGroup("aa", "resources/monsterBall.png");

	ParticleEmitter* emitter = new ParticleEmitter("aa", { {1,1,1},{0,0,0},{0,0,0} }, 5, 0.5f, 0.0f);




#pragma region MyRegion 
#ifdef _DEBUG
	char buf[256];  // バッファサイズを固定
	buf[0] = '\0';  // 空の文字列で初期化

	float f = 0.0f;

	bool my_tool_active = true;
	float my_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Initial color

	// スプライト
	Vector2 aa = Vector2(100, 100);
	Vector3 cameraPos{0,10,0};
	Vector3 cameraRotate{0.84f,0,0};

	Vector3 axisPos{};
	Vector3 axisRotate{};
#endif // _DEBUG
#pragma endregion //ImGui試し用変数

	Vector3 offset{};

	audio->PlayWave(sound);

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// Windowsのメッセージ処理
		if (winApp->ProcessMessage()) {
			// ゲームループを抜ける
			break;
		};
		// ImGuiの受付開始
		imguiManager->Begin();

#ifdef _DEBUG

#pragma region MyRegion

		ImGui::Text("Hello, world %d", 123);
		if (ImGui::Button("Save"))
		{
			// Save ボタンの処理
		}
		ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);






		// Create a window called "My First Tool", with a menu bar.
		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Edit a color stored as 4 floats
		ImGui::ColorEdit4("Color", my_color);

		// Generate samples and plot them
		float samples[100];
		for (int n = 0; n < 100; n++)
			samples[n] = sinf(n * 0.2f + float(ImGui::GetTime()) * 1.5f);
		ImGui::PlotLines("Samples", samples, 100);

		// Display contents in a scrolling region
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
		ImGui::BeginChild("Scrolling");
		for (int n = 0; n < 50; n++)
			ImGui::Text("%04d: Some text", n);
		ImGui::EndChild();
		ImGui::End();

		// デモウィンドウの表示オン
		ImGui::ShowDemoWindow();


		ImGui::Begin("Sprite");
		ImGui::SetWindowSize(ImVec2(500, 100)); // Ensure you're using ImVec2 for the size

		ImGui::SliderFloat2("sprite", &aa.x, 0, 1200, "%5.1f");
		sprites[0]->SetPosition(aa);
		ImGui::End();

#pragma endregion // ImGui

#endif // _DEBUG

		// Input
		input->Update();

		

		camera->Update();


		ImGui::Begin("Camera");
		ImGui::DragFloat3("Translate",&cameraPos.x,0.1f);
		ImGui::DragFloat3("Rotate",&cameraRotate.x,0.01f);
		camera->SetTranslate(cameraPos);
		camera->SetRotate(cameraRotate);
		ImGui::End();

		//// 3Dモデル
		for (uint32_t i = 0; i < MaxObject3d; ++i) {

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
		for (uint32_t i = 0; i < MaxSprite; ++i) {
			sprites[i]->Update();
		}

		// パーティクルの更新
		//particleManager->Update();

		//emitter->Update();

		// ImGuiの受付終了
		imguiManager->End();


		// 描画前処理
		srvManager->PreDraw();

		dxCommon->PreDraw();



		//////////////---------3Dモデル-------------///////////////

		//// 3Dオブジェクトの描画準備
		object3dCommon->DrawCommonSetting();

		////3Dオブジェクトの描画
		for (uint32_t i = 0; i < MaxObject3d; ++i) {
			object3ds[i]->Draw();
		}

		//// パーティクルの描画
		//particleManager->Draw();

		//////////////--------スプライト-----------///////////////////


		// 2Dオブジェクトの描画準備
		spriteCommon->DrawCommonSetting();


		// 2Dオブジェクトの描画
		for (uint32_t i = 0; i < MaxSprite; ++i) {
			//sprites[i]->Draw();
		}

		// ImGuiの描画
		imguiManager->Draw();

		//描画後処理
		dxCommon->PostDraw();

	}



	delete input;

	dxCommon->Finalize();
	delete dxCommon;

	delete srvManager;

	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	delete winApp;
	for (uint32_t i = 0; i < MaxSprite; ++i) {
		delete sprites[i];
	}
	delete spriteCommon;

	//delete object3d;


	for (uint32_t i = 0; i < MaxObject3d; ++i) {
		delete object3ds[i];
	}
	delete object3dCommon;


	delete modelCommon;

	// ImGuiマネージャーの終了
	ImGuiManager::GetInstance()->Finalize();

	// 音
	audio->Finalize();
	// パーティクルマネージャーの終了
	particleManager->Finalize();

	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();

	// モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();

	return 0;
}
