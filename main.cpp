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

#include"Structs.h"
#include"MathFanctions.h"
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"D3DResourceLeakchecker.h"
#include"Sprite.h"
#include"SpriteCommon.h"
#include"TextureManager.h"
#include"Object3d.h"
#include"Object3dCommon.h"
#include"Model.h"
#include"ModelCommon.h"


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

#include"StringUtility.h"
#include"Logger.h"

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

	TextureManager::GetInstance()->Initialize(dxCommon);

	SpriteCommon* spriteCommon = nullptr;
	// スプライト共通部の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Object3dCommon* object3dCommon = nullptr;
	// 3Dオブジェクト共通部分の初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

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

	Model* model = nullptr;
	model =	new Model();
	model->Initialize(modelCommon);


	std::vector<Object3d*> object3ds;
	const int MaxObject3d = 2;
	for (uint32_t i = 0; i < MaxObject3d; ++i) {
		Object3d* object3d = new Object3d();

		object3d->Initialize(object3dCommon);
		object3d->SetModel(model);

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

		// Input
		input->Update();


		// 3Dモデル
		for (uint32_t i = 0; i < MaxObject3d; ++i) {
			object3ds[i]->Update();
		}

		// スプライト
		for (uint32_t i = 0; i < MaxSprite; ++i) {
			sprites[i]->Update();
		}




		ImGui::Begin("Window");
		ImGui::Text("PushKey [DIK_SPACE] = Log [HIT 0]");
		ImGui::End();

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
			//sprites[i]->Draw();
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

	delete model;
	/*for (uint32_t i = 0; i < MaxModel; ++i) {
		delete models[i];
	}*/
	delete modelCommon;

	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();

	return 0;
}
