#include "MyGame.h"



void MyGame::Initialize()
{
	Framework::Initialize();

	// 最初のシーン
	sceneFactory_ = std::make_unique<SceneFactory>();
	// シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
	
	SceneManager::GetInstance()->ChangeScene("TITLE");
	//
	//Camera::GetInstance();
	// リソース初期化
	InitializeResource();

}

void MyGame::Finalize()
{
	//
	Object3dCommon::GetInstance()->Finalize();
	//
	SpriteCommon::GetInstance()->Finalize();
	// srv
	SrvManager::GetInstance()->Finalize();
	//
	Audio::GetInstance()->Finalize();
	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();
	// モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();
	//
	SceneManager::GetInstance()->Finalize();
	// 基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{
	// ImGuiの受付開始
	imguiManager->Begin();

	Framework::Update();

	// グローバル変数の更新
	GlobalVariables::GetInstance()->Update();

	SceneManager::GetInstance()->Update();
	
	
	particleManager_->Update();
	//ParticleManager::GetInstance()->Update();

	// ImGuiの受付終了
	imguiManager->End();
}

void MyGame::Draw()
{
	// 描画前処理
	SrvManager::GetInstance()->PreDraw();
	dxCommon->PreDraw();

	//////////////---------3Dモデル-------------///////////////

	
	ParticleManager::GetInstance()->DrawCommonSetting();

	SceneManager::GetInstance()->DrawP3D();

	//// 3Dオブジェクトの描画準備
	Object3dCommon::GetInstance()->DrawCommonSetting();

	SceneManager::GetInstance()->Draw3D();

	LineCommon::GetInstance()->DrawCommonSetting();

	SceneManager::GetInstance()->DrawLine3D();

	//SceneManager::GetInstance()

	// 2Dオブジェクトの描画準備
	SpriteCommon::GetInstance()->DrawCommonSetting();

	SceneManager::GetInstance()->Draw2D();

	// ImGuiの描画
	imguiManager->Draw();

	//描画後処理
	dxCommon->PostDraw();
}

void MyGame::InitializeResource()
{
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/train.png");
	TextureManager::GetInstance()->LoadTexture("resources/rail.png");
	TextureManager::GetInstance()->LoadTexture("resources/reticle.png");
	TextureManager::GetInstance()->LoadTexture("resources/white.png");
	TextureManager::GetInstance()->LoadTexture("resources/enemy.png");
	TextureManager::GetInstance()->LoadTexture("resources/sky.png");
	TextureManager::GetInstance()->LoadTexture("resources/enter.png");
	for (int i = 0; i < 10; i++) {
		std::string label = "resources/num/" + std::to_string(i) + ".png";
		TextureManager::GetInstance()->LoadTexture(label);
	}

	ModelManager::GetInstance()->LoadModel("rail.obj");
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("axis2.obj");
	ModelManager::GetInstance()->LoadModel("train.obj");
	
	ModelManager::GetInstance()->LoadModel("building.obj");
	ModelManager::GetInstance()->LoadModel("Sphere.obj");
	ModelManager::GetInstance()->LoadModel("long.obj");
	ModelManager::GetInstance()->LoadModel("skydome.obj");
	ModelManager::GetInstance()->LoadModel("enemy.obj");
	ModelManager::GetInstance()->LoadModel("title.obj");


	//ParticleManager::GetInstance()->CreateParticleGroup("aa", "resources/uvChecker.png");
	
}
