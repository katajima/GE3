#include "MyGame.h"

const float MyGame::kDeltaTime_ = 1.0f / 60.0f;
float MyGame::kTimeSpeed_ = 1.0f;
float MyGame::hitStopTimer = 0.0f;

void MyGame::Initialize()
{
	Framework::Initialize();


	// 最初のシーン
	sceneFactory_ = std::make_unique<SceneFactory>();
	// シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
	
	SceneManager::GetInstance()->ChangeScene("TEST");
	//SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	
	//Camera::GetInstance();
	// リソース初期化
	InitializeResource();

	// グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();
	//

}

void MyGame::Finalize()
{
	//
	Object3dCommon::GetInstance()->Finalize();
	//
	SkinningConmmon::GetInstance()->Finalize();
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

	//HitStpoTime(); // ストップ用

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
	ImGui::DragFloat("TimeScale", &kTimeSpeed_,0.01f);
	ImGui::End();

#endif // _DEBUG

	//kDeltaTime_.
	
	// グローバル変数の更新
	GlobalVariables::GetInstance()->Update();

	lightCommon->Update();


	SceneManager::GetInstance()->Update();
	
	ParticleManager::GetInstance()->Update();
	//particleManager_->Update();
	
	// ImGuiの受付終了
	imguiManager->End();
}

void MyGame::Draw()
{
	// 描画前処理
	SrvManager::GetInstance()->PreDraw();

	//dxCommon->PreDrawOffscreen();

	dxCommon->PreDrawSwap();

	//////////////---------3Dモデル-------------///////////////

	
	//// 3Dオブジェクトの描画準備
	
	SceneManager::GetInstance()->Draw3D();

	ParticleManager::GetInstance()->Draw();

	//particleManager_->Draw();

	//// 2Dオブジェクトの描画準備
	SceneManager::GetInstance()->Draw2D();
	
	///
	//dxCommon->CreateRenderTexture();
	//RenderingCommon::GetInstance()->DrawCommonSetting();


	//描画後処理
	//dxCommon->PostDrawOffscreen();

	// ImGuiの描画
	imguiManager->Draw();

	//描画後処理
	dxCommon->PostDrawSwap();
}

void MyGame::InitializeResource()
{
	ParticleManager::GetInstance()->SetCamera(&Camera::GetInstance());

	TextureManager::GetInstance()->LoadTexture("resources/Texture/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/Image.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/aa2.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/aaa.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/aaaa.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/aaaaa.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/aaaaaa.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/aa3.png");
	for (int i = 0; i < 10; i++) {
		std::string label = "resources/Texture/num/" + std::to_string(i) + ".png";
		TextureManager::GetInstance()->LoadTexture(label);
	}
	TextureManager::GetInstance()->LoadTexture("resources/Texture/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/Image.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/Image.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/dust.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/hit.png");
	TextureManager::GetInstance()->LoadTexture("resources/Texture/text/max.png");


	ModelManager::GetInstance()->LoadModel("multiMaterial.gltf", "multiMaterial");

	ModelManager::GetInstance()->LoadModel("multiMaterial.obj", "multiMaterial");
	ModelManager::GetInstance()->LoadModel("a.obj");
	

	//ModelManager::GetInstance()->LoadModelAmime("walk.gltf", "human");
	//ModelManager::GetInstance()->LoadModelAmime("Women.gltf", "Women");
	//ModelManager::GetInstance()->LoadModelAmime("Man.gltf", "Man");
	//ModelManager::GetInstance()->LoadModelAmime("Man.gltf", "Man");
	//ModelManager::GetInstance()->LoadModelAmime("Hand.gltf", "Hand");
	//ModelManager::GetInstance()->LoadModelAmime("iku.gltf", "iku");
	//ModelManager::GetInstance()->LoadModelAmime("Kick.gltf", "Kick");



	ModelManager::GetInstance()->LoadModel("player_bullet.obj", "player_bullet");
	ModelManager::GetInstance()->LoadModelAmime("Sword.obj", "Sword");
	ModelManager::GetInstance()->LoadModelAmime("plane.obj", "plane");
	//ModelManager::GetInstance()->LoadModelAmime("sneakWalk.gltf", "human");
	ModelManager::GetInstance()->LoadModelAmime("AnimatedCube.gltf", "AnimatedCube");

	ModelManager::GetInstance()->LoadModel("grass.obj", "grass");
	ModelManager::GetInstance()->LoadModel("grass.gltf", "grass2");
	ModelManager::GetInstance()->LoadModel("renga.gltf", "renga",{10,10});
	ModelManager::GetInstance()->LoadModel("Ground.obj", "Ground");
	ModelManager::GetInstance()->LoadModel("stair.obj");



	ModelManager::GetInstance()->LoadModel("plane.obj", "plane");
	ModelManager::GetInstance()->LoadModel("axis.obj", "axis");
	ModelManager::GetInstance()->LoadModel("multiMesh.obj", "multiMesh");


	ModelManager::GetInstance()->LoadModel("multiMaterial.obj", "multiMaterial");
	ModelManager::GetInstance()->LoadModel("teapot.obj", "teapot");



	ModelManager::GetInstance()->LoadModel("rail.obj","rail");
	ModelManager::GetInstance()->LoadModel("building.obj","building");
	ModelManager::GetInstance()->LoadModel("Sphere.obj","sphere");
	ModelManager::GetInstance()->LoadModel("Sphere2.obj","sphere");
	ModelManager::GetInstance()->LoadModel("Sphere3.obj","sphere");
	ModelManager::GetInstance()->LoadModel("skydome.obj", "skydome");
	ModelManager::GetInstance()->LoadModel("enemy.obj","enemy");
	ModelManager::GetInstance()->LoadModel("enemy2.obj","enemy2");
	

	primi = std::make_unique<Primitive>();
	primi->Initialize(Primitive::ShapeType::Torus, "resources/Texture/uvChecker.png");
	
	primiTrai = std::make_unique<Primitive>();
	primiTrai->Initialize(Primitive::ShapeType::Triangle, "resources/Texture/Image.png");
	
	primiPlane = std::make_unique<Primitive>();
	primiPlane->Initialize(Primitive::ShapeType::Cylinder, "resources/Texture/uvChecker.png");

	primiStar = std::make_unique<Primitive>();
	primiStar->Initialize(Primitive::ShapeType::Star, "resources/Texture/Image.png");
	ShapeParameter::Star star;
	star.innerRadius_ = 1.0f;
	star.outerRadius_ = 7.0f;
	star.segments_ = 4;
	primiStar->SetStar(star);
	primiStar->SetName("star");
	primiStar->Update();
	ParticleManager::GetInstance()->CreateParticleGroup("test", "resources/Texture/uvChecker.png", ModelManager::GetInstance()->FindModel("plane.obj"), true);
	
	ParticleManager::GetInstance()->CreateParticleGroup("cc", "resources/Texture/Image.png", ModelManager::GetInstance()->FindModel("plane.obj"), true,{}, ParticleManager::BlendType::MODE_ADD);
	ParticleManager::GetInstance()->CreateParticleGroup("hitStar", "resources/Texture/Image.png", primiStar.get(), true);
	ParticleManager::GetInstance()->CreateParticleGroup("hitTrai", "resources/Texture/Image.png", primiTrai.get(), true);
	ParticleManager::GetInstance()->CreateParticleGroup("hit", "resources/Texture/hit.png", ModelManager::GetInstance()->FindModel("plane.obj"), true);
	ParticleManager::GetInstance()->CreateParticleGroup("enemyGround", "resources/Texture/Image.png", ModelManager::GetInstance()->FindModel("plane.obj"), true);
	ParticleManager::GetInstance()->CreateParticleGroup("enemyDust", "resources/Texture/dust.png", ModelManager::GetInstance()->FindModel("plane.obj"), true);
	
	ParticleManager::GetInstance()->CreateParticleGroup("dustt", "resources/Texture/Image.png", ModelManager::GetInstance()->FindModel("plane.obj"), true);
	
	ParticleManager::GetInstance()->CreateParticleGroup("enemyaaaa", "resources/Texture/uvChecker.png", ModelManager::GetInstance()->FindModel("teapot.obj"), true);

	ParticleManager::GetInstance()->CreateParticleGroup("primi", "resources/Texture/uvChecker.png", primi.get(), true);

	ParticleManager::GetInstance()->CreateParticleGroup("primiCylinder", "resources/Texture/uvChecker.png", primiPlane.get(), true);

}



void MyGame::HitStpoTime()
{
	bool is = false;
	hitStopTimer -= kDeltaTime_;
	if (hitStopTimer <= 0.0f) {
		hitStopTimer = 0.0f;
	}
	if (hitStopTimer > 0) {
		is = true;
	}

	if (is) {
		
		kTimeSpeed_ = 0.4f;
	}
	else {
		kTimeSpeed_ = 1.0f;
	}
}
