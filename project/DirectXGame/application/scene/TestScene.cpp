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
	mm.worldtransform_.translate_ = { 30,1,1 };
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
	walk.SetModel("walk.gltf");
	walk.SetCamera(camera.get());
	walk.worldtransform_.translate_ = { 0,10,0 };
	walk.worldtransform_.scale_ = { 20,20,20 };






	sphere1.Initialize();
	sphere1.SetModel("stair.obj");
	sphere1.SetCamera(camera.get());
	sphere1.worldtransform_.translate_ = { 2,0,-10 };
	sphere1.worldtransform_.scale_ = scale1;


	sphereStr.Initialize();
	sphereStr.worldtransform_.parent_ = &sphere1.worldtransform_;
	sphereStr.worldtransform_.translate_.y = sphere1.GetMesh(0)->GetMin().y;

	sphereEnd.Initialize();
	sphereEnd.worldtransform_.parent_ = &sphere1.worldtransform_;
	sphereEnd.worldtransform_.translate_.y = sphere1.GetMesh(0)->GetMax().y;






	ball1.mass = 2.0f;
	ball1.rad = scale1;
	ball1.velocity = 0;

	sphere2.Initialize();
	sphere2.SetModel("a.obj");
	sphere2.SetCamera(camera.get());
	sphere2.worldtransform_.translate_ = { 0,10,10 };
	sphere2.worldtransform_.scale_ = scale2;

	ball2.mass = 2.0f;
	ball2.rad = scale2;
	ball2.velocity = 0;


	setVelo.z = 0.6f;

	ocean_.Initialize(Vector2{ 30,30 });
	ocean_.SetCamera(camera.get());
	ocean_.transform.rotate.x = DegreesToRadians(-90);
	ocean_.material->color.w = 1.0f;


	sprite.Initialize("resources/Texture/uvChecker.png");
	sprite.SetPosition({ 0,0 });
	sprite.SetColor({ 1,1,1,0.1f });

	lineDraw_.Initialize();
	lineDraw_.SetCamera(camera.get());
	lineDraw2_.Initialize();
	lineDraw2_.SetCamera(camera.get());

	trailEffect_ = std::make_unique<TrailEffect>();
	trailEffect_->Initialize("resources/Texture/aaa.png", 4.0f);
	trailEffect_->SetCamera(camera.get());
	trailEffect_->SetObject(&sphere1);


	flag33 = false;

	cons.centar = { 0,0,0 };
	cons.rotate = { 0,0,0 };
	cons.size = { 0.3f,0.3f,0.3f };
	cons.count = 10;
	cons.lifeTime = 1.3f;
	cons.color = { 0.2f,0.2f,0.2f,1.0f };
	cons.velocity = { 1.0f,1.0f,10.0f };
	cons.renge = { -Vector3{1.5f,1.5f,1.5f},Vector3{1.5f,1.5f,1.5f} };
	cons.renge = { -Vector3{2.5f,2.5f,2.5f},Vector3{2.5f,2.5f,2.5f} };


	ParticleManager::GetInstance()->SetObject("cc", sphere1.worldtransform_);
	ParticleManager::GetInstance()->SetCamera(camera.get());


	rotate_ = { 0,0,0 };


	primitive = std::make_unique<Primitive>();
	primitive->Initialize(Primitive::ShapeType::Torus, "resources/Texture/uvChecker.png", { 1,1,1,1 });
	Primitive::AnimationPlane anime{};
	anime.height = 1;
	anime.width = 1;
	anime.interval = 1;
	anime.direction.x = 1;
	anime.count = 15;
	anime.num = 1;
	anime.isLoop = false;
	anime.isUV = false;
	//primitive->SetParametar(anime);
	primitive->SetCamera(camera.get());
	primitive->transform.translate.x = 20;
	primitive->transform.translate.y = 10;
	//primitive->SetName("cube1");
	lineDraw3_.Initialize();
	lineDraw3_.SetCamera(camera.get());

	for (int i = 0; i < 1; i++) {
		auto primi = std::make_unique<Primitive>();
		if (i == 0)
			primi->Initialize(Primitive::ShapeType::Cube, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		primi->SetName("cube2");
		if (i == 1)
			primi->Initialize(Primitive::ShapeType::Circle, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if (i == 2)
			primi->Initialize(Primitive::ShapeType::Crescent, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if (i == 3)
			primi->Initialize(Primitive::ShapeType::Cube, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if (i == 4)
			primi->Initialize(Primitive::ShapeType::Pyramid, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if (i == 5)
			primi->Initialize(Primitive::ShapeType::Ring, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if (i == 6)
			primi->Initialize(Primitive::ShapeType::Sphere, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if (i == 7)
			primi->Initialize(Primitive::ShapeType::Plane, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if (i == 8)
			primi->Initialize(Primitive::ShapeType::Tube, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if (i == 9)
			primi->Initialize(Primitive::ShapeType::Cylinder, "resources/Texture/uvChecker.png", { 1,1,1,1 });

		primi->SetCamera(camera.get());
		primi->transform.translate.x = float(i * 10);

		primitives.push_back(std::move(primi));
	}



	segment_.end = { 1,1,1 };
	segment_.origin = { 0,0,0 };

	testPos = { 0,0,0 };

	offset = 0.1f;

	cup1.radius = 0.1f;
	cup1.segment.origin = {testPos.x,offset,testPos.z};
	cup1.segment.end = { testPos.x,-offset,testPos.z };
	cup2.radius = 3.0f;
	cup2.segment.origin = { 10,1,1 };
	cup2.segment.end = { 15,1,1 };


	

	primitiveCup1[0] = std::make_unique<Primitive>();
	primitiveCup1[0]->Initialize(Primitive::ShapeType::Sphere, "resources/Texture/uvChecker.png", { 1,1,1,1 });
	primitiveCup1[0]->SetCamera(camera.get());
	primitiveCup1[0]->SetParametar(cup1.radius);
	primitiveCup1[0]->SetName("cup1_o");
	primitiveCup1[1] = std::make_unique<Primitive>();
	primitiveCup1[1]->Initialize(Primitive::ShapeType::Sphere, "resources/Texture/uvChecker.png", { 1,1,1,1 });
	primitiveCup1[1]->SetCamera(camera.get());
	primitiveCup1[1]->SetParametar(cup1.radius);
	primitiveCup1[1]->SetName("cup1_e");

	primitiveCup2[0] = std::make_unique<Primitive>();
	primitiveCup2[0]->Initialize(Primitive::ShapeType::Sphere, "resources/Texture/uvChecker.png", { 1,1,1,1 });
	primitiveCup2[0]->SetCamera(camera.get());
	primitiveCup2[0]->SetParametar(cup2.radius);
	primitiveCup2[0]->SetName("cup2_o");

	primitiveCup2[1] = std::make_unique<Primitive>();
	primitiveCup2[1]->Initialize(Primitive::ShapeType::Sphere, "resources/Texture/uvChecker.png", { 1,1,1,1 });
	primitiveCup2[1]->SetCamera(camera.get());
	primitiveCup2[1]->SetParametar(cup2.radius);
	primitiveCup2[1]->SetName("cup2_e");



}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	



	trailEffect_->Update(flag33, sphereStr, sphereEnd);

	

	

	if(Input::GetInstance()->IsTriggerKey(DIK_0)){
		walk.worldtransform_.rotate_.y += DegreesToRadians(1);
	}








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

	//#ifdef _DEBUG
	//	if (Input::GetInstance()->IsTriggerKey(DIK_P)) {
	//		// シーン切り替え
	//		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	//	}
	//
	//
	ImGui::Begin("engine");
	//
	//	
	//
	if (ImGui::CollapsingHeader("Gizmos")) {
		ImGuiManager::GetInstance()->RenderGizmo2(mm, *camera.get(), "buil");
		ImGuiManager::GetInstance()->RenderGizmo2(mm2, *camera.get(), "buil2");
		ImGuiManager::GetInstance()->RenderGizmo2(tail, *camera.get(), "tail");
		ImGuiManager::GetInstance()->RenderGizmo2(walk, *camera.get(), "walk");
		ImGuiManager::GetInstance()->RenderGizmo2(multiMesh, *camera.get(), "multiMesh");
		//ImGuiManager::GetInstance()->RenderGizmo2(sphere1, *camera.get(), "sphere1");
		ImGuiManager::GetInstance()->RenderGizmo2(sphere2, *camera.get(), "sphere1");

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

	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE)) {
		ball1.velocity = setVelo;
	}

	if (Input::GetInstance()->IsTriggerKey(DIK_R)) {
		ball1.velocity = 0.0f;
		ball2.velocity = 0.0f;
	}
	if (Input::GetInstance()->IsTriggerKey(DIK_T)) {
		ball1.velocity = 0.0f;
		ball2.velocity = 0.0f;

		sphere1.worldtransform_.translate_ = { 2, 0, -10 };
		sphere2.worldtransform_.translate_ = { 0, 0, 10 };
	}

	if (Mesh::IsCapsuleCollision(*sphere1.GetMesh(0),sphere1.GetWorldPosition(), cup1)) {
		sphere1.GetMaterial(0)->color = { 1,0,0,1 };
	}
	else {
		sphere1.GetMaterial(0)->color = { 1,1,1,1 };
	}




	primitiveCup1[0]->transform.translate = cup1.segment.origin;
	primitiveCup1[1]->transform.translate = cup1.segment.end;

	primitiveCup1[0]->SetParametar(cup1.radius);
	primitiveCup1[1]->SetParametar(cup1.radius);
	primitiveCup1[0]->transform.scale = cup1.radius * 0.33f;
	primitiveCup1[1]->transform.scale = cup1.radius * 0.33f;

	primitiveCup2[0]->transform.translate = cup2.segment.origin;
	primitiveCup2[1]->transform.translate = cup2.segment.end;
	
	//primitiveCup2[0]->SetParametar(cup2.radius);
	//primitiveCup2[1]->SetParametar(cup2.radius);

	cup1.segment.origin = { testPos.x,testPos.y + offset,testPos.z };
	cup1.segment.end = { testPos.x,testPos.y - offset,testPos.z };


	/*if (IsCollision(cup1,cup2)) {
		primitiveCup1[0]->SetColor(Vector4{1,0,0,1});
		primitiveCup1[1]->SetColor(Vector4{1,0,0,1});
	}
	else {
		primitiveCup1[0]->SetColor(Vector4{ 1,1,1,1 });
		primitiveCup1[1]->SetColor(Vector4{ 1,1,1,1 });
	}*/






	/*primitiveCup1[0]->Update();
	primitiveCup1[1]->Update();

	primitiveCup2[0]->Update();
	primitiveCup2[1]->Update();

	for (int i = 0; i < primitives.size(); i++) {
		primitives[i]->Update();
	}*/



	primitive->Update();

	walk.UpdateSkinning();
	mm.Update();
	mm2.UpdateAnimation();
	multiMesh.Update();
	tail.Update();


	ocean_.Update();

	sprite.Update();


	sphere1.Update();
	sphere2.Update();

	sphereEnd.Update();
	sphereStr.Update();

	lineDraw_.Update();
	lineDraw2_.Update();
	lineDraw3_.Update();



}

void TestScene::Draw3D()
{
	for (int i = 0; i < primitives.size(); i++) {
	//	primitives[i]->Draw();
	}

	primitive->Draw();


	//primitiveCup1[0]->Draw();
	//primitiveCup1[1]->Draw();

	//primitiveCup2[0]->Draw();
	//primitiveCup2[1]->Draw();

	//primitive2->Draw();

	walk.DrawSkinning();
	//walk.DrawLine();
	tail.Draw();
	multiMesh.Draw();
	mm.Draw();
	mm2.Draw();

	//ocean_.Draw();

	//sphere1.Draw();
	sphere2.Draw();

	trailEffect_->Draw();

	//ocean_.Draw();
	/*if (ball1.velocity.Length() == 0) {
		lineDraw_.Draw3D(sphere1.transform.translate, sphere1.transform.translate + (setVelo.Normalize() * 10), { 1,1,0,1 });

	}
	else {
		lineDraw_.Draw3D(sphere1.transform.translate, sphere1.transform.translate + (ball1.velocity.Normalize() * 10), { 1,0,0,1 });
	}
	lineDraw2_.Draw3D(sphere2.transform.translate, sphere2.transform.translate + (ball2.velocity.Normalize() * 10), { 1,0,0,1 });*/

//	lineDraw3_.DrawCapselLine(testPos,cup1);

	//lineDraw3_.Draw3D(segment_.origin, segment_.end, { 1,1,1,1 });
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

