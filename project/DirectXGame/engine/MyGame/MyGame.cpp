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

#ifdef _DEBUG
	auto currentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
	lastTime = currentTime;

	if (deltaTime > 0) {
		fps = 1.0f / deltaTime;
	}

	// FPS表示用ウィジェット
	ImGui::Begin("engine");
	ImGui::Text("FPS: %.2f", fps);
	ImGui::End();

#endif // _DEBUG

	// グローバル変数の更新
	GlobalVariables::GetInstance()->Update();

	lightCommon->Update();


	SceneManager::GetInstance()->Update();
	
	
	particleManager_->Update();
	
	// ImGuiの受付終了
	imguiManager->End();
}

void MyGame::Draw()
{
	// 描画前処理
	SrvManager::GetInstance()->PreDraw();
	dxCommon->PreDraw();

	//////////////---------3Dモデル-------------///////////////

	
	//ParticleManager::GetInstance()->DrawCommonSetting();

	SceneManager::GetInstance()->DrawP3D();

	//// 3Dオブジェクトの描画準備
	Object3dCommon::GetInstance()->DrawCommonSetting();
	
	LightCommon::GetInstance()->DrawLight();

	SceneManager::GetInstance()->Draw3D();

	//LineCommon::GetInstance()->DrawCommonSetting();

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
	//TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	//TextureManager::GetInstance()->LoadTexture("resources/train.png");
	//TextureManager::GetInstance()->LoadTexture("resources/rail.png");
	//TextureManager::GetInstance()->LoadTexture("resources/reticle.png");
	//TextureManager::GetInstance()->LoadTexture("resources/white.png");
	//TextureManager::GetInstance()->LoadTexture("resources/enemy.png");
	//TextureManager::GetInstance()->LoadTexture("resources/sky.png");
	//TextureManager::GetInstance()->LoadTexture("resources/enter.png");
	//TextureManager::GetInstance()->LoadTexture("resources/NormalMap.png");
	for (int i = 0; i < 10; i++) {
		std::string label = "resources/num/" + std::to_string(i) + ".png";
	//	TextureManager::GetInstance()->LoadTexture(label);
	}

	ModelManager::GetInstance()->LoadModel("plane.gltf");
	ModelManager::GetInstance()->LoadModel("rail.obj","rail");
	
	ModelManager::GetInstance()->LoadModel("grass.obj", "grass");
	ModelManager::GetInstance()->LoadModel("grass.gltf", "grass2");
	ModelManager::GetInstance()->LoadModel("walk.gltf", "walk");
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj","axis");
	ModelManager::GetInstance()->LoadModel("teapot.obj","teapot");
	
	ModelManager::GetInstance()->LoadModel("building.obj","building");
	ModelManager::GetInstance()->LoadModel("Sphere.obj","sphere");
	ModelManager::GetInstance()->LoadModel("skydome.obj", "skydome");
	ModelManager::GetInstance()->LoadModel("enemy.obj","enemy");
	ModelManager::GetInstance()->LoadModel("title.obj");


	//ParticleManager::GetInstance()->CreateParticleGroup("aa", "resources/uvChecker.png");
	
}
