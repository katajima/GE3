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







	// 列車オブジェクトを unique_ptr で作成
	mm.Initialize();
	mm.SetModel("plane.obj");
	mm.worldtransform_.translate_ = { 30,1,1 };
	//mm.worldtransform_.scale_ = { 10,10,10 };
	mm.SetCamera(camera.get());
	mm2.Initialize();
	mm2.SetModel("AnimatedCube.gltf");
	mm2.worldtransform_.translate_ = { -30,10,1 };
	mm2.worldtransform_.scale_ = { 10,10,10 };
	mm2.SetCamera(camera.get());

	multiMesh.Initialize();
	multiMesh.SetModel("plane.obj");
	multiMesh.worldtransform_.translate_ = { 0,10,20 };
	multiMesh.worldtransform_.rotate_.y = DegreesToRadians(180);
	multiMesh.worldtransform_.scale_ = { 10,10,10 };
	multiMesh.SetCamera(camera.get());

	tail.Initialize();
	tail.SetModel("renga.gltf");
	tail.SetCamera(camera.get());
	tail.model->modelData.material[0]->shininess_ = 1000.0f;

	walk.Initialize();
	//walk.SetModel("Kick.gltf");
	//walk.SetModel("Hand.gltf");
	//walk.SetModel("iku.gltf");
	//walk.SetModel("Man.gltf");
	//walk.SetModel("Women2.gltf");
	//walk.SetModel("Women.gltf");
	//walk.SetModel("walk.gltf");
	walk.SetCamera(camera.get());
	walk.worldtransform_.translate_ = { 0,10,0 };
	walk.worldtransform_.scale_ = { 20,20,20 };




	cons.centar = { 0,0,0 };
	cons.rotate = { 0,0,0 };
	cons.size = { 0.3f,0.3f,0.3f };
	cons.count = 10;
	cons.lifeTime = 1.3f;
	cons.color = { 0.2f,0.2f,0.2f,1.0f };
	cons.velocity = { 1.0f,1.0f,10.0f };
	cons.renge = { -Vector3{1.5f,1.5f,1.5f},Vector3{1.5f,1.5f,1.5f} };
	cons.renge = { -Vector3{2.5f,2.5f,2.5f},Vector3{2.5f,2.5f,2.5f} };


	//ParticleManager::GetInstance()->SetObject("test", tail.worldtransform_);
	ParticleManager::GetInstance()->SetCamera(camera.get());


	trans_.Initialize();
	trans_.translate_ = { 0,10,0 };
	//trans_.rotate_ = { 0,DegreesToRadians(180),0};

	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->Initialize("emitter","primiCylinder",ParticleManager::EmitType::kRandom);
	emitter_->GetFrequency() = 0.1f;
	emitter_->SetCount(1);
	emitter_->SetParent(tail.worldtransform_);
	emitter_->SetRotateMinMax(-Vector3{1.0f,1.0f,1.0f}, { 1.0f,1.0f,1.0f });
	emitter_->SetPos({ 0,10,0 });
	emitter_->SetVelocityMinMax({ -10,5,0 } ,{ 0, 10, 0 });
	emitter_->SetLifeTimeMinMax(0.1f, 0.1f);
	emitter_->SetUsebillboard(false);
	emitter_->SetIsGravity(true);
	emitter_->SetIsAlpha(true);

	emitterEnemy_ = std::make_unique<ParticleEmitter>();
	emitterEnemy_->Initialize("emitterPrimi","primi",ParticleManager::EmitType::kRandom);
	emitterEnemy_->GetFrequency() = 0.1f;
	emitterEnemy_->SetCount(1);
	emitterEnemy_->SetParent(mm.worldtransform_);
	emitterEnemy_->SetPos({ 0,0,0 });
	emitterEnemy_->SetVelocityMinMax({ -10,20,-10 } ,{ 10, 40, 10 });
	emitterEnemy_->SetRotateMinMax(-DegreesToRadians(Vector3{90,90,90}), DegreesToRadians(Vector3{ 90,90,90 }));
	emitterEnemy_->SetRotateVelocityMinMax(-Vector3{0.1f,0.1f,0.1f},{0.1f,0.1f,0.1f});
	emitterEnemy_->SetLifeTimeMinMax(1, 5);
	emitterEnemy_->SetIsGravity(true);
	emitterEnemy_->SetUsebillboard(false);
	emitterEnemy_->SetIsAlpha(true);
	emitterEnemy_->SetIsLifeTimeScale(true);
	emitterEnemy_->SetIsRotateVelocity(true);
	emitterEnemy_->SetSizeMinMax(Vector3{0.1f,0.1f,0.1f},{ 0.2f,0.2f,0.2f });
}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	//ParticleManager::GetInstance()->SetCamera(camera.get());
	//ParticleManager::GetInstance()->Emit("cc", "const", cons);


	if(Input::GetInstance()->IsTriggerKey(DIK_0)){
		walk.worldtransform_.rotate_.y += DegreesToRadians(1);
	}


	

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
	ImGui::Begin("trans");
	ImGui::DragFloat3("translate",&emitter_->transform_.translate_.x,0.1f);
	ImGui::DragFloat3("rotate",&emitter_->transform_.rotate_.x,0.1f);
	ImGui::End();
	ImGui::Begin("engine");
	
	
	if (ImGui::CollapsingHeader("Gizmos")) {
		ImGuiManager::GetInstance()->RenderGizmo2(mm, *camera.get(), "buil");
		ImGuiManager::GetInstance()->RenderGizmo2(mm2, *camera.get(), "buil2");
		ImGuiManager::GetInstance()->RenderGizmo2(tail, *camera.get(), "tail");
		ImGuiManager::GetInstance()->RenderGizmo2(walk, *camera.get(), "walk");
		ImGuiManager::GetInstance()->RenderGizmo2(multiMesh, *camera.get(), "multiMesh");
		
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
	//#endif

	//tail.GetMaterial(0)->color = GetColorSet(ColorName::BLUE);


	/*ImGui::Begin("asj");
	int i = (int)walk.model->modelData.rootNode.children.size();
	ImGui::InputInt("child", &i);
	i = (int)walk.model->skeleton.joints.size();
	ImGui::InputInt("joint", &i);
	ImGui::End();*/
	
	//walk.Update();
	//walk.UpdateSkinning();
	mm.Update();
	mm2.UpdateAnimation();
	multiMesh.Update();
	tail.Update();

}

void TestScene::Draw3D()
{

	//walk.GetMesh(0)->indices;

	//walk.Draw();
	//walk.DrawLine();
	//walk.DrawSkinning();
	
	tail.Draw();
	mm.Draw();
	//mm2.Draw();

	
	
}

void TestScene::Draw2D()
{
	
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

