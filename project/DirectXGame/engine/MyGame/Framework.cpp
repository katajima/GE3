#include"Framework.h"

void Framework::Initialize()
{
	// WindowsAPI解放
	winApp = new WinApp();
	winApp->Initialize();


	//dxCommon = 
	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Intialize();

	Input::GetInstance()->Intialize(winApp);
	
	// SRVマネージャの初期化
	srvManager = SrvManager::GetInstance();
	srvManager->Initialize(dxCommon.get());

	

	// ImGuiマネージャー
	imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(dxCommon.get());

	// スプライト共通部の初期化
	spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize(dxCommon.get());

	// 3Dオブジェクト共通部分の初期化
	object3dCommon = Object3dCommon::GetInstance();
	object3dCommon->Initialize(dxCommon.get());
	

	modelCommon = new ModelCommon;
	modelCommon->Initialize(dxCommon.get());


	TextureManager::GetInstance()->Initialize(dxCommon.get());
	ModelManager::GetInstance()->Initialize(dxCommon.get());

}

void Framework::Finalize()
{
	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPI解放
	delete winApp;
	
	dxCommon->Finalize();

	// ImGuiマネージャーの終了
	imguiManager->Finalize();
	
	delete modelCommon;	

	//delete sceneFactory_;
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