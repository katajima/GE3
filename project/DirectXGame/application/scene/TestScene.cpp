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
	mm.SetModel("building.obj");
	mm.transform.translate = { 30,1,1 };
	mm.SetCamera(camera.get());
	mm2.Initialize();
	mm2.SetModel("AnimatedCube.gltf");
	mm2.transform.translate = { -30,10,1 };
	mm2.transform.scale = { 10,10,10 };
	mm2.SetCamera(camera.get());
	
	multiMesh.Initialize();
	multiMesh.SetModel("multiMaterial.obj");
	multiMesh.transform.translate = { 0,10,20 };
	multiMesh.transform.rotate.y = DegreesToRadians(180);
	multiMesh.transform.scale = { 10,10,10 };
	multiMesh.SetCamera(camera.get());

	tail.Initialize();
	tail.SetModel("renga.gltf");
	tail.SetCamera(camera.get());
	tail.model->modelData.material[0]->shininess_ = 1000.0f;

	walk.Initialize();
	walk.SetModel("walk.gltf");
	walk.SetCamera(camera.get());
	walk.transform.translate = { 0,10,0 };
	walk.transform.scale = { 20,20,20 };

	test = a.Conjugate();

	
	ocean_.Initialize(Vector2{30,30});
	ocean_.SetCamera(camera.get());
	ocean_.transform.rotate.x = DegreesToRadians(-90);
	ocean_.material->color.w = 0.3f;


	sprite.Initialize("resources/Texture/uvChecker.png");
	sprite.SetPosition({ 0,0 });
	sprite.SetColor({ 1,1,1,0.1f });
	//sprite.SetSize({10,10});

}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	/*ParticleManager::Constant cons{};
	cons.lifeTime = 2;
	cons.count = 20;
	cons.size = 2;
	cons.renge = { -Vector3{2,2,2} ,Vector3{2,2,2} };
	cons.velocityRenge = {-Vector3{2,2,2},{2,2,2} };
	ParticleManager::GetInstance()->SetCamera(camera.get());
	ParticleManager::GetInstance()->Emit("cc","const", cons);
	ParticleManager::GetInstance()->SetObject("cc",walk);*/


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

	
	//LightCommon::GetInstance()->GetPointLight(0).intensity = 1.0f;

	camera->UpdateMatrix();
	LightCommon::GetInstance()->SetLineCamera(camera.get());

#ifdef _DEBUG
	if (Input::GetInstance()->IsTriggerKey(DIK_P)) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}


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
			camera->transform_.rotate = { DegreesToRadians(10),DegreesToRadians(180),0};
		}


	}

	ImGui::End();
#endif

	




	walk.UpdateSkinning();
	mm.Update();
	mm2.UpdateAnimation();
	multiMesh.Update();
	tail.Update();

	
	//ocean_.Update();

	sprite.Update();
}

void TestScene::Draw3D()
{
	
	
	walk.DrawSkinning();
	walk.DrawLine();
	tail.Draw();
	//multiMesh.Draw();
	//mm.Draw();
	//mm2.Draw();

	


	//ocean_.Draw();

}

void TestScene::Draw2D()
{
	//sprite.Draw();
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

