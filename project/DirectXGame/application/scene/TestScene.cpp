#include"TestScene.h"
//#include"DirectXGame/engine/struct/Quaternion.h"

void TestScene::Initialize()
{
	//オーディオの初期化
	audio_ = Audio::GetInstance();
	// 入力初期化
	input_ = Input::GetInstance();

	// カメラ
	InitializeCamera();
	// リソース
	InitializeResources();

	for (int i = 0; i < 4; i++)
	{
		auto sprite = std::make_unique<Sprite>();

		sprite->Initialize("resources/Texture/uvChecker.png", false);
		sprite->SetTextureSize({ 64,64 });
		sprite->SetSize({ 128,128 });
		sprite->SetAnimeSize({ 64,64 });
		sprite->SetMaxAnimeNum({ 8,8 });
		sprite->SetPosition({ static_cast<float>(i) * (128 + 10) ,0 });

		sprite_.push_back(std::move(sprite));
	}


	ocean_ = std::make_unique<Ocean>();
	ocean_->Initialize({100,100});
	ocean_->SetCamera(camera.get());
	ocean_->transform.rotate.x = DegreesToRadians(90);
	ocean_->material->color.a = 0.99f;

	// 列車オブジェクトを unique_ptr で作成
	mm.Initialize();
	mm.SetModel("building.obj");
	mm.worldtransform_.translate_ = { 30,1,1 };
	//mm.worldtransform_.scale_ = { 10,10,10 };
	mm.SetCamera(camera.get());
	mm2.Initialize();
	mm2.SetModel("AnimatedCube.gltf");
	mm2.worldtransform_.translate_ = { -30,10,1 };
	mm2.worldtransform_.scale_ = { 10,10,10 };
	mm2.SetCamera(camera.get());

	
	tail.Initialize();
	tail.SetModel("renga.gltf");
	tail.SetCamera(camera.get());
	tail.model->modelData.material[0]->shininess_ = 1000.0f;

	multiy.Initialize();
	multiy.SetModel("multiMaterial.gltf");
	multiy.SetCamera(camera.get());
	multiy.worldtransform_.scale_ = { 10,10,10 };
	



	ParticleManager::GetInstance()->SetCamera(camera.get());


	trans_.Initialize();
	trans_.translate_ = { 0,10,0 };
	
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->Initialize("emitter","cc",ParticleManager::EmitType::kRandom);
	emitter_->GetFrequency() = 0.1f;
	emitter_->SetCount(1);
	emitter_->SetParent(tail.worldtransform_);
	emitter_->SetRotateMinMax(-Vector3{1.0f,1.0f,1.0f}, { 1.0f,1.0f,1.0f });
	emitter_->SetPos({ 0,10,0 });
	emitter_->SetVelocityMinMax({ -10,5,0 } ,{ 0, 10, 0 });
	emitter_->SetLifeTimeMinMax(1.0f, 2.0f);
	/*emitter_->SetUsebillboard(false);*/
	emitter_->SetIsGravity(true);
	emitter_->SetIsAlpha(true);

	emitterEnemy_ = std::make_unique<ParticleEmitter>();
	emitterEnemy_->Initialize("emitterPrimi","primi",ParticleManager::EmitType::kRandom);
	emitterEnemy_->GetFrequency() = 1.1f;
	emitterEnemy_->SetCount(1);
	emitterEnemy_->SetParent(mm.worldtransform_);
	emitterEnemy_->SetPos({ 0,50,0 });
	emitterEnemy_->SetVelocityMinMax({ -0,20,-5 } ,{ 5, 20, 5 });
	emitterEnemy_->SetRotateMinMax(-DegreesToRadians(Vector3{90,90,90}), DegreesToRadians(Vector3{ 90,90,90 }));
	emitterEnemy_->SetRotateVelocityMinMax(-Vector3{0.1f,0.1f,0.1f},{0.1f,0.1f,0.1f});
	emitterEnemy_->SetLifeTimeMinMax(5, 10);
	emitterEnemy_->SetIsGravity(true);
	emitterEnemy_->SetUsebillboard(false);
	emitterEnemy_->SetIsAlpha(true);
	emitterEnemy_->SetIsLifeTimeScale(true);
	emitterEnemy_->SetIsRotateVelocity(true);
	emitterEnemy_->SetIsBounce(true);
	emitterEnemy_->SetSizeMinMax(Vector3{0.1f,0.1f,0.1f},{ 0.2f,0.2f,0.2f });
}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	

	
	ocean_->Update();

	

	emitter_->Update();
	emitterEnemy_->Update();


	if (Input::GetInstance()->IsPushKey(DIK_A)) {
		camera->transform_.translate.x -= 0.3f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_D)) {
		camera->transform_.translate.x += 0.3f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_W)) {
		camera->transform_.translate.z += 0.3f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_S)) {
		camera->transform_.translate.z -= 0.3f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_UP)) {
		camera->transform_.translate.y += 0.3f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_DOWN)) {
		camera->transform_.translate.y -= 0.3f;
	}


	camera->UpdateMatrix();
	LightCommon::GetInstance()->SetLineCamera(camera.get());

#ifdef _DEBUG
	ImGui::Begin("sprite");
	ImGui::End();




	ImGui::Begin("trans");
	ImGui::DragFloat3("translate",&emitter_->transform_.translate_.x,0.1f);
	ImGui::DragFloat3("rotate",&emitter_->transform_.rotate_.x,0.1f);
	ImGui::End();
	ImGui::Begin("engine");
	
	
	if (ImGui::CollapsingHeader("Gizmos")) {
		ImGuiManager::GetInstance()->RenderGizmo2(mm, *camera.get(), "buil");
		ImGuiManager::GetInstance()->RenderGizmo2(mm2, *camera.get(), "buil2");
		ImGuiManager::GetInstance()->RenderGizmo2(tail, *camera.get(), "tail");
		ImGuiManager::GetInstance()->RenderGizmo2(multiy, *camera.get(), "multiy");
		
	}
	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::DragFloat3("Translate", &camera->transform_.translate.x, 0.1f);
		ImGui::DragFloat3("Rotate", &camera->transform_.rotate.x, 0.01f);
		ImGui::Checkbox("flag", &flag);
		if (ImGui::Button("cameraPos")) {
			camera->transform_.translate = { 0,20,-175 };
			camera->transform_.rotate = { 0,0,0 };
		}
		if (ImGui::Button("cameraPos2")) {
			camera->transform_.translate = { -30,10,-140 };
			camera->transform_.rotate = { 0,0,0 };
		}
		if (ImGui::Button("cameraPos3")) {
			camera->transform_.translate = { 0,500,0 };
			camera->transform_.rotate = { DegreesToRadians(90),0,0 };
		}
		if (ImGui::Button("cameraPos4")) {
			camera->transform_.translate = { 0,60,-220 };
			camera->transform_.rotate = { DegreesToRadians(10),0,0 };
		}
		if (ImGui::Button("cameraPos5")) {
			camera->transform_.translate = { 0,60,220 };
			camera->transform_.rotate = { DegreesToRadians(10),DegreesToRadians(180),0 };
		}


	}

	ImGui::End();
#endif // _DEBUG
	
	mm.Update();
	mm2.UpdateAnimation();
	tail.Update();
	multiy.Update();
}

void TestScene::Draw3D()
{
	tail.Draw(Object3d::ObjectType::NoUvInterpolation_MODE_SOLID_BACK);
	mm.Draw(Object3d::ObjectType::NoUvInterpolation_MODE_SOLID_BACK);
	mm2.Draw(Object3d::ObjectType::NoUvInterpolation_MODE_WIREFRAME_NONE);

	multiy.Draw();

	ocean_->Draw();
}

void TestScene::Draw2D()
{

	for (int i = 0; i < sprite_.size(); i++) {
		sprite_[i]->UpdateAmimetion(0.05f);
		
	}
	sprite_[0]->Draw();
	sprite_[1]->Draw(Sprite::SpriteType::NoUvInterpolation_MODE_SOLID);
	sprite_[2]->Draw(Sprite::SpriteType::UvInterpolation_MODE_WIREFRAME);
	sprite_[3]->Draw(Sprite::SpriteType::NoUvInterpolation_MODE_WIREFRAME);


	

	

}

void TestScene::InitializeResources()
{
	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());
}

void TestScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->transform_.rotate = { 1.0f,0,0 };
	camera->transform_.translate = { 0,100,-60.0f };


	cameraT.y = 1.0f;
}

