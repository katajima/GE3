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

#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"

#include<vector>

//ImGui
#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include"DirectXGame/engine/base/StringUtility.h"
#include"DirectXGame/engine/base/Logger.h"

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

	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Intialize(winApp);

	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize(dxCommon);

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
	
	std::vector<Object3d*> object3ds;
	for (uint32_t i = 0; i < MaxObject3d; ++i) {
		Object3d* object3d = new Object3d();
		object3d->Initialize(object3dCommon);
		//object3d->SetCamera(object3dCommon->GetDefaltCamera());
		if (i == 1) {
			ModelManager::GetInstance()->LoadModel("plane.obj");
			object3d->SetModel("plane.obj");
			object3d->SetTranslate({ 4,0,10 });
		}
		else {
			ModelManager::GetInstance()->LoadModel("axis.obj");
			object3d->SetModel("axis.obj");
			object3d->SetTranslate({ -2,0,10 });
		}
		

		object3ds.push_back(object3d);
	}

	
	
	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// Windowsのメッセージ処理
		if (winApp->ProcessMessage()) {
			// ゲームループを抜ける
			break;
		};

		//ゲームの処理
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Vector3 cameraR = camera->GetRotate();
		Vector3 cameraT = camera->GetTranslate();
		ImGui::Begin("Window");
		ImGui::DragFloat3("camera Rotate", &cameraR.x,0.01f);
		camera->SetRotate(cameraR);
		ImGui::DragFloat3("camera translate", &cameraT.x, 0.1f);
		camera->SetTranslate(cameraT);
		Vector3 translateObj1 = object3ds[0]->GetTranslate();
		ImGui::DragFloat3("object1 translate", &translateObj1.x, 0.1f);
		object3ds[0]->SetTranslate(translateObj1);
		Vector3 translateObj2 = object3ds[1]->GetTranslate();
		ImGui::DragFloat3("object2 translate", &translateObj2.x, 0.1f);
		object3ds[1]->SetTranslate(translateObj2);
		ImGui::Text("PushKey [DIK_SPACE] = Log [HIT 0]");
		ImGui::End();

		// Input
		input->Update();

		camera->Update();

		// 3Dモデル
		for (uint32_t i = 0; i < MaxObject3d; ++i) {
			
			object3ds[i]->Update();
			object3ds[i]->SetCamera(object3dCommon->GetDefaltCamera());
			object3ds[i]->SetScale({ 1,1,1 });
			if (i == 0) {
				object3ds[i]->SetRotate(Add(object3ds[i]->GetRotate(), Vector3{ 0.01f,0.02f,0.01f }));
			}
			if (i == 1) {
				
				object3ds[i]->SetRotate(Add(object3ds[i]->GetRotate(), Vector3{ 0.0f,0.02f,0.0f }));
			}
		}

		// スプライト
		for (uint32_t i = 0; i < MaxSprite; ++i) {
			sprites[i]->Update();
		}



		

		//開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
		ImGui::ShowDemoWindow();

		// 描画前処理
		dxCommon->PreDraw();

		//////////////---------3Dモデル-------------///////////////

		// 3Dオブジェクトの描画準備
		object3dCommon->DrawCommonSetting();

		//3Dオブジェクトの描画
		for (uint32_t i = 0; i < MaxObject3d; ++i) {
			object3ds[i]->Draw();
		}


		//////////////--------スプライト-----------///////////////////


		// 2Dオブジェクトの描画準備
		spriteCommon->DrawCommonSetting();


		// 2Dオブジェクトの描画
		for (uint32_t i = 0; i < MaxSprite; ++i) {
			sprites[i]->Draw();
		}

		//描画後処理
		dxCommon->PostDraw();

	}



	delete input;

	dxCommon->Finalize();
	delete dxCommon;

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

	for (uint32_t i = 0; i < MaxObject3d; ++i) {
		//delete models[i];
	}
	/*for (uint32_t i = 0; i < MaxModel; ++i) {
		delete models[i];
	}*/
	delete modelCommon;

	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();

	// モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();

	return 0;
}
