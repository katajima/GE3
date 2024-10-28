#include"Framework.h"

void Framework::Initialize()
{
	// WindowsAPI解放
	winApp = new WinApp();
	winApp->Initialize();


	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Intialize();

	Input::GetInstance()->Intialize(winApp);
	
	// SRVマネージャの初期化
	srvManager = SrvManager::GetInstance();
	srvManager->Initialize(dxCommon);

	// ImGuiマネージャー
	imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(dxCommon);

	// スプライト共通部の初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	// 3Dオブジェクト共通部分の初期化
	object3dCommon = new Object3dCommon;
	object3dCommon->Initialize(dxCommon);
	

	modelCommon = new ModelCommon;
	modelCommon->Initialize(dxCommon);

}

void Framework::Finalize()
{
	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	delete winApp;
	
	
	// ImGuiマネージャーの終了
	imguiManager->Finalize();
	//delete imguiManager;

	

	delete srvManager;
	//
	delete spriteCommon;
	//
	delete object3dCommon;
	//
	delete modelCommon;	
}

void Framework::Update()
{

	Input::GetInstance()->Update();
	// Windowsのメッセージ処理
	if (winApp->ProcessMessage()) {
		// ゲームループを抜ける
		endRequst_ = true;
	};
}

void Framework::Run()
{
	// ゲームの初期化
	Initialize();
	while (true) {
		// 毎フレーム更新
		Update();
		// 終了リクエストが来たら抜ける
		if (IsEndRequst()) {
			break;
		}
		// 描画
		Draw();
	}
	// ゲームの終了
	Finalize();
}
