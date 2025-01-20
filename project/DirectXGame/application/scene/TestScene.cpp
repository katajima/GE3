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



	sphere1.Initialize();
	sphere1.SetModel("Sphere.obj");
	sphere1.SetCamera(camera.get());
	sphere1.transform.translate = { 2,0,-10 };
	sphere1.transform.scale = scale1;


	sphereStr.Initialize();
	sphereStr.parent_ = &sphere1;
	sphereStr.transform.translate.y = sphere1.GetMesh(0)->GetMin().y;

	sphereEnd.Initialize();
	sphereEnd.parent_ = &sphere1;
	sphereEnd.transform.translate.y = sphere1.GetMesh(0)->GetMax().y;






	ball1.mass = 2.0f;
	ball1.rad = scale1;
	ball1.velocity = 0;

	sphere2.Initialize();
	sphere2.SetModel("Sphere.obj");
	sphere2.SetCamera(camera.get());
	sphere2.transform.translate = { 0,0,10 };
	sphere2.transform.scale = scale2;

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


	ParticleManager::GetInstance()->SetObject("cc", sphere1);
	ParticleManager::GetInstance()->SetCamera(camera.get());


	rotate_ = { 0,0,0 };


	primitive = std::make_unique<Primitive>();
	primitive->Initialize(Primitive::ShapeType::Cube, "resources/Texture/uvChecker.png",{1,1,1,1});
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
	primitive->transform.translate.x = -10;
	primitive->SetName("cube1");
	lineDraw3_.Initialize();
	lineDraw3_.SetCamera(camera.get());
	
	for (int i = 0; i < 1; i++) {
		auto primi = std::make_unique<Primitive>();
		if(i == 0)
			primi->Initialize(Primitive::ShapeType::Cube, "resources/Texture/uvChecker.png", { 1,1,1,1 });
			primi->SetName("cube2");
		if(i == 1)
			primi->Initialize(Primitive::ShapeType::Circle, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if(i == 2)
			primi->Initialize(Primitive::ShapeType::Crescent, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if(i == 3)
			primi->Initialize(Primitive::ShapeType::Cube, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if(i == 4)
			primi->Initialize(Primitive::ShapeType::Pyramid, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if(i == 5)
			primi->Initialize(Primitive::ShapeType::Ring, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if(i == 6)
			primi->Initialize(Primitive::ShapeType::Sphere, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if(i == 7)
			primi->Initialize(Primitive::ShapeType::Plane, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if(i == 8)
			primi->Initialize(Primitive::ShapeType::Tube, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		if(i == 9)
			primi->Initialize(Primitive::ShapeType::Cylinder, "resources/Texture/uvChecker.png", { 1,1,1,1 });
		
		primi->SetCamera(camera.get());
		primi->transform.translate.x = float(i * 10);

		primitives.push_back(std::move(primi));
	}

	

	segment_.end   = { 1,1,1 };
	segment_.origin = { 0,0,0 };

}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	cons.velocity = -ball1.velocity.Normalize() * 10;
	cons.color = { 0.2f,0.2f,0.2f,1.0f };
	cons.size = { 0.5f,0.5f,0.5f };
	cons.renge = { -Vector3{3.0f,3.0f,3.0f},Vector3{3.0f,3.0f,3.0f} };
	ParticleManager::GetInstance()->Emit("cc","const", cons);
	cons.color = { 0.0f,0.0f,0.0f,1.0f };
	cons.size = { 0.7f,0.7f,0.7f };
	cons.renge = { -Vector3{2.0f,2.0f,2.0f},Vector3{2.0f,2.0f,2.0f} };
	ParticleManager::GetInstance()->Emit("cc","const", cons);
	cons.velocity = -ball1.velocity.Normalize() * 15;
	cons.color = { 1.0f,0.5f,0.0f,1.0f };
	cons.size = { 0.3f,0.3f,0.3f };
	cons.renge = { -Vector3{2.0f,2.0f,2.0f},Vector3{2.0f,2.0f,2.0f} };
	ParticleManager::GetInstance()->Emit("cc", "const", cons);
	cons.color = { 1.0f,0.0f,0.0f,1.0f };
	cons.renge = { -Vector3{1.5f,1.5f,1.5f},Vector3{1.5f,1.5f,1.5f} };
	ParticleManager::GetInstance()->Emit("cc", "const", cons);


	sphere1.transform.scale = scale1;
	sphere2.transform.scale = scale2;
	ball1.rad = scale1;
	ball2.rad = scale2;
	
	if (ball1.velocity.Length() != 0) {
		flag33 = true;
	}
	else {
		flag33 = false;
	}
	flag33 = true;


	

	trailEffect_->Update(flag33,sphereStr,sphereEnd);
	
	ImGui::Begin("segment");
	ImGui::DragFloat3("origin", &segment_.origin.x);
	//if()
	ImGui::DragFloat3("end", &segment_.end.x);
	ImGui::End();

	ImGui::Begin("trail");
	ImGui::Checkbox("frag", &flag33);
	ImGui::DragFloat3("rotate", &sphere1.transform.rotate.x,0.01f);
	ImGui::DragFloat3("rotateSpeed", &rotate_.x,0.01f);
	ImGui::DragFloat("maxTime", &trailEffect_->GetMesh()->maxTime, 0.01f);
	int ii = (int)trailEffect_->mesh->vertices.size();
	ImGui::InputInt("vertice", &ii);
	ImGui::End();
	
	
	

	


	


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
			ImGuiManager::GetInstance()->RenderGizmo2(sphere1, *camera.get(), "sphere1");
	
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

		sphere1.transform.translate = { 2, 0, -10 };
		sphere2.transform.translate = { 0, 0, 10 };
	}


	 // 球の速度を更新
	sphere1.transform.translate += ball1.velocity;
	sphere2.transform.translate += ball2.velocity;

	sphere1.transform.rotate += rotate_;

	// 球の位置を取得
	Vector3 position1 = sphere1.transform.translate;
	Vector3 position2 = sphere2.transform.translate;

	// 球の半径（仮に1.0fとします）
	//float radius = 1.0f;
	// 衝突の検出
	Vector3 distance = position2 - position1;
	if (distance.Length() <= ball1.rad + ball2.rad) {
		// 衝突検出時の処理
		Vector3 normal = sphere1.transform.translate - sphere2.transform.translate;
		normal.Normalize();
		auto result = ComputeCollisionVelocities(ball1.mass, ball1.velocity, ball2.mass, ball2.velocity, refrect, normal.Normalize());
		ball1.velocity = result.first;
		ball2.velocity = result.second;

		rotate_.x = ball1.velocity.Normalize().Length() /10;
	}

	if (sphere1.transform.translate.x > 30 || -30 > sphere1.transform.translate.x) {
		ball1.velocity.x *= -1.0f;
	}
	if (sphere1.transform.translate.z > 30 || -30 > sphere1.transform.translate.z) {
		ball1.velocity.z *= -1.0f;
	}
	if (sphere2.transform.translate.x > 30 || -30 > sphere2.transform.translate.x) {
		ball2.velocity.x *= -1.0f;
	}
	if (sphere2.transform.translate.z > 30 || -30 > sphere2.transform.translate.z) {
		ball2.velocity.z *= -1.0f;
	}




	

	OBB obb{};
	obb.center = primitives[0]->transform.translate;
	obb.size = primitives[0]->GetCubeSize();
	Vector3 rotate = primitives[0]->transform.rotate;
	
	Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	obb.orientations[0].x = rotateMatrix.m[0][0];
	obb.orientations[0].y = rotateMatrix.m[0][1];
	obb.orientations[0].z = rotateMatrix.m[0][2];

	obb.orientations[1].x = rotateMatrix.m[1][0];
	obb.orientations[1].y = rotateMatrix.m[1][1];
	obb.orientations[1].z = rotateMatrix.m[1][2];

	obb.orientations[2].x = rotateMatrix.m[2][0];
	obb.orientations[2].y = rotateMatrix.m[2][1];
	obb.orientations[2].z = rotateMatrix.m[2][2];

	OBB obb1{};
	obb1.center = primitive->transform.translate;
	obb1.size = primitive->GetCubeSize();
	
	rotate = primitive->transform.rotate;
	rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
	
	obb1.orientations[0].x = rotateMatrix.m[0][0];
	obb1.orientations[0].y = rotateMatrix.m[0][1];
	obb1.orientations[0].z = rotateMatrix.m[0][2];

	obb1.orientations[1].x = rotateMatrix.m[1][0];
	obb1.orientations[1].y = rotateMatrix.m[1][1];
	obb1.orientations[1].z = rotateMatrix.m[1][2];

	obb1.orientations[2].x = rotateMatrix.m[2][0];
	obb1.orientations[2].y = rotateMatrix.m[2][1];
	obb1.orientations[2].z = rotateMatrix.m[2][2];


	if (IsCollision(obb, obb1)) {
		primitive->SetColor(Vector4{ 1,0,0,1 });
	}
	else {
		primitive->SetColor(Vector4{ 1,1,1,1 });
	}
	







	for (int i = 0; i < primitives.size(); i++) {
		primitives[i]->Update();
	}

	

	primitive->Update();
	//primitive2->Update();

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
	//lineDraw3_.SetTransform(primitive->transform);
	lineDraw3_.Update();



}

void TestScene::Draw3D()
{
	for (int i = 0; i < primitives.size(); i++) {
		primitives[i]->Draw();
	}

	primitive->Draw();
	//primitive2->Draw();

	//walk.DrawSkinning();
	//walk.DrawLine();
	//tail.Draw();
	//multiMesh.Draw();
	//mm.Draw();
	//mm2.Draw();

	//ocean_.Draw();

	sphere1.Draw();
	sphere2.Draw();

	trailEffect_->Draw();

	//ocean_.Draw();
	if (ball1.velocity.Length() == 0) {
		lineDraw_.Draw3D(sphere1.transform.translate, sphere1.transform.translate + (setVelo.Normalize() * 10), { 1,1,0,1 });

	}
	else {
		lineDraw_.Draw3D(sphere1.transform.translate, sphere1.transform.translate + (ball1.velocity.Normalize() * 10), { 1,0,0,1 });
	}
	lineDraw2_.Draw3D(sphere2.transform.translate, sphere2.transform.translate + (ball2.velocity.Normalize() * 10), { 1,0,0,1 });


	lineDraw3_.Draw3D(segment_.origin,segment_.end,{1,1,1,1});
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

