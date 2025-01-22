#include"Framework.h"

void Framework::Initialize()
{
	// WindowsAPI解放
	winApp = std::make_unique<WinApp>();
	winApp->Initialize();


	//dxCommon = 
	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Intialize();

	renderingCommon = RenderingCommon::GetInstance();
	renderingCommon->Initialize(dxCommon.get());

	Input::GetInstance()->Intialize(winApp.get());
	
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
	
	// モデルコモン
	modelCommon = std::make_unique<ModelCommon>();
	modelCommon->Initialize(dxCommon.get());
	
	// カメラコモン
	cameraCommon = CameraCommon::GetInstance();
	cameraCommon->Initialize(dxCommon.get());

	skinningCommon = SkinningConmmon::GetInstance();
	skinningCommon->Initialize(dxCommon.get());


	lineCommon = LineCommon::GetInstance();
	lineCommon->Initialize(dxCommon.get());

	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Initialize(dxCommon.get());
	//
	oceanManager_ = OceanManager::GetInstance();
	oceanManager_->Initialize(dxCommon.get());
	//
	//thunderManager = ThunderManager::GetInstance();
	//thunderManager->Initialize(dxCommon.get());

	trailEffectManager_ = TrailEffectManager::GetInstance();
	trailEffectManager_->Initialize(dxCommon.get());

	primitiveCommon = PrimitiveCommon::GetInstance();
	primitiveCommon->Initialize(dxCommon.get());

	//ライト
	lightCommon = LightCommon::GetInstance();
	lightCommon->Initialize();

	// テクスチャマネージャ
	TextureManager::GetInstance()->Initialize(dxCommon.get());
	
	// モデルマネージャ
	ModelManager::GetInstance()->Initialize(dxCommon.get());

	
}

void Framework::Finalize()
{
	// WindowsAPIの終了処理
	winApp->Finalize();
	
	dxCommon->Finalize();
	// パーティクルマネージャーの終了
	//ParticleManager::GetInstance()->Finalize();
	// ImGuiマネージャーの終了
	imguiManager->Finalize();

	particleManager_->Finalize();

	oceanManager_->Finalize();

	trailEffectManager_->Finalize();

	primitiveCommon->Finalize();

	cameraCommon->Finalize();
	
	lineCommon->Finalize();

	lightCommon->Finalize();

	renderingCommon->Finalize();
}

void Framework::Update()
{

	Input::GetInstance()->Update();

	
	// Windowsのメッセージ処理
	if(winApp->ProcessMessage()) {
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
