#include"Framework.h"

void Framework::Initialize()
{
	// WindowsAPI解放
	winApp = new WinApp();
	winApp->Initialize();


	dxCommon = new DirectXCommon();
	dxCommon->Intialize(winApp);


	// SRVマネージャの初期化
	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

	// ImGuiマネージャー
	imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(winApp, dxCommon);

	// スプライト共通部の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	// 3Dオブジェクト共通部分の初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);
	

	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);

	// ラインコモン
	lineCommon = new LineCommon();
	lineCommon->Initialize(dxCommon);

}

void Framework::Finalize()
{
	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	delete winApp;
	dxCommon->Finalize();
	delete dxCommon;

	delete srvManager;
	
	delete spriteCommon;

	delete object3dCommon;

	delete modelCommon;

	delete lineCommon;
}

void Framework::Update()
{
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
