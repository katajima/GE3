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


	setVelo.z = 0.2f;

	ocean_.Initialize(Vector2{ 30,30 });
	ocean_.SetCamera(camera.get());
	ocean_.transform.rotate.x = DegreesToRadians(-90);
	ocean_.material->color.w = 0.3f;


	sprite.Initialize("resources/Texture/uvChecker.png");
	sprite.SetPosition({ 0,0 });
	sprite.SetColor({ 1,1,1,0.1f });
	
	lineDraw_.Initialize();
	lineDraw_.SetCamera(camera.get());
	lineDraw2_.Initialize();
	lineDraw2_.SetCamera(camera.get());

	camera->transform_.translate = { 0,500,0 };
	camera->transform_.rotate = { DegreesToRadians(90),0,0 };
}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	sphere1.transform.scale = scale1;
	sphere2.transform.scale = scale2;
	ball1.rad = scale1;
	ball2.rad = scale2;

	


	

	ImGui::Begin("MT4");
	if (ImGui::TreeNode("MT4_01_01")) {
		ImGui::Text("01_01");
		Vector3 axis = Normalize({ 1.0f,1.0f,1.0f });
		float angle = 0.44f;
		Matrix4x4 rotate;
		Matrix4x4 aa = rotate.MakeRotateAxisAngle(axis, angle);
		ImGui::Text("rotateMatrix0");
		ImGui::InputFloat4("mat[0][~]", aa.m[0]);
		ImGui::InputFloat4("mat[1][~]", aa.m[1]);
		ImGui::InputFloat4("mat[2][~]", aa.m[2]);
		ImGui::InputFloat4("mat[3][~]", aa.m[3]);
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::TreeNode("MT4_01_02")) {
		ImGui::Text("01_02");
		Vector3 floam0 = (Vector3{ 1.0f,0.7f,0.5f });
		Vector3 to0 = -floam0;
		Vector3 floam1 = (Vector3{ -0.6f,0.9f,0.2f });
		Vector3 to1 = (Vector3{ 0.4f,0.7f,-0.5f });
		Matrix4x4 rotateMatrix0 = DirectionToDirection((Vector3{ 1.0f,0.0f,0.0f }), (Vector3{ -1.0f,0.0f,0.0f }));
		Matrix4x4 rotateMatrix1 = DirectionToDirection(floam0, to0);
		Matrix4x4 rotateMatrix2 = DirectionToDirection(floam1, to1);

		ImGui::Text("rotateMatrix0");
		ImGui::InputFloat4("mat[0][~]", rotateMatrix0.m[0]);
		ImGui::InputFloat4("mat[1][~]", rotateMatrix0.m[1]);
		ImGui::InputFloat4("mat[2][~]", rotateMatrix0.m[2]);
		ImGui::InputFloat4("mat[3][~]", rotateMatrix0.m[3]);
		ImGui::Text("rotateMatrix1");
		ImGui::InputFloat4("mat[0][~]", rotateMatrix1.m[0]);
		ImGui::InputFloat4("mat[1][~]", rotateMatrix1.m[1]);
		ImGui::InputFloat4("mat[2][~]", rotateMatrix1.m[2]);
		ImGui::InputFloat4("mat[3][~]", rotateMatrix1.m[3]);
		ImGui::Text("rotateMatrix2");
		ImGui::InputFloat4("mat[0][~]", rotateMatrix2.m[0]);
		ImGui::InputFloat4("mat[1][~]", rotateMatrix2.m[1]);
		ImGui::InputFloat4("mat[2][~]", rotateMatrix2.m[2]);
		ImGui::InputFloat4("mat[3][~]", rotateMatrix2.m[3]);
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::TreeNode("MT4_01_03")) {
		ImGui::Text("01_03");
		Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
		Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
		Quaternion identity = identity.IdentityQuaternion();
		Quaternion conj = q1.Conjugate();
		Quaternion inv = Inverse(q1);
		Quaternion normal = q1.Normalize();
		Quaternion mull1 = q1 * q2;
		Quaternion mull2 = q2 * q1;
		float norm = q1.Norm();
		ImGui::InputFloat4("Identity", &identity.x, "%.2f");
		ImGui::InputFloat4("Conjugate", &conj.x, "%.2f");
		ImGui::InputFloat4("Inverse", &inv.x, "%.2f");
		ImGui::InputFloat4("Normalize", &normal.x, "%.2f");
		ImGui::InputFloat4("Multiply(q1,q2)", &mull1.x, "%.2f");
		ImGui::InputFloat4("Multiply(q2,q1)", &mull2.x, "%.2f");
		ImGui::InputFloat("Norm", &norm);
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::TreeNode("MT4_01_04")) {
		ImGui::Text("01_04");
		Quaternion rotation = rotation.MakeQuaternion({ 1.0f,0.4f,-0.2f }, 0.45f);
		Vector3 point = { 2.1f,-0.9f,1.3f };
		Matrix4x4 rotateMatrix = rotation.MakeRotateMatrix();
		Vector3 rotateByQuaternion = rotation.RotateVector(point);
		Vector3 rotateByMatrix = Transforms(point, rotateMatrix);
		ImGui::InputFloat4("rotation", &rotation.x, "%.2f");
		ImGui::Text("rotateMatrix");
		ImGui::InputFloat4("mat[0][~]", rotateMatrix.m[0]);
		ImGui::InputFloat4("mat[1][~]", rotateMatrix.m[1]);
		ImGui::InputFloat4("mat[2][~]", rotateMatrix.m[2]);
		ImGui::InputFloat4("mat[3][~]", rotateMatrix.m[3]);
		ImGui::InputFloat3("rotateByQuaternion", &rotateByQuaternion.x, "%.2f");
		ImGui::InputFloat3("rotateByMatrix", &rotateByMatrix.x, "%.2f");
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::TreeNode("MT4_01_05")) {
		ImGui::Text("01_05");

		// rotation0 と rotation1 を初期化
		Quaternion rotation0 = rotation0.MakeQuaternion({ 0.71f, 0.71f, 0.0f }, 0.3f);
		Quaternion rotation1 = rotation1.MakeQuaternion({ 0.71f, 0.0f, 0.71f }, 0.3141592f);

		// Slerp 関数を使用して補間クォータニオンを計算
		Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
		Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
		Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
		Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
		Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);

		// ImGui でクォータニオンを表示
		ImGui::InputFloat4("interpolate0 Slerp(q0,q1,0.0f)", &interpolate0.x, "%.2f");
		ImGui::InputFloat4("interpolate1 Slerp(q0,q1,0.3f)", &interpolate1.x, "%.2f");
		ImGui::InputFloat4("interpolate2 Slerp(q0,q1,0.5f)", &interpolate2.x, "%.2f");
		ImGui::InputFloat4("interpolate3 Slerp(q0,q1,0.7f)", &interpolate3.x, "%.2f");
		ImGui::InputFloat4("interpolate4 Slerp(q0,q1,1.0f)", &interpolate4.x, "%.2f");

		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::TreeNode("MT4_01_05_EX")) {
		ImGui::Text("01_05_EX");
		Quaternion rotation0 = rotation0.MakeQuaternion({ 0.71f, 0.71f,0.0f }, 0.3f);
		Quaternion rotation1 = { -rotation0.x,-rotation0.y,-rotation0.z,-rotation0.w };
		Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
		Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
		Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
		Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
		Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);

		ImGui::InputFloat4("interpolate0 Sleap(q0,q1,0.0f)", &interpolate0.x, "%.2f");
		ImGui::InputFloat4("interpolate1 Sleap(q0,q1,0.3f)", &interpolate1.x, "%.2f");
		ImGui::InputFloat4("interpolate2 Sleap(q0,q1,0.5f)", &interpolate2.x, "%.2f");
		ImGui::InputFloat4("interpolate3 Sleap(q0,q1,0.7f)", &interpolate3.x, "%.2f");
		ImGui::InputFloat4("interpolate4 Sleap(q0,q1,1.0f)", &interpolate4.x, "%.2f");
		ImGui::TreePop();
	}
	ImGui::Separator();
	if (ImGui::TreeNode("MT4_02_01_EX")) {
		ImGui::Text("02_01_EX");
		ImGui::Text("reset velo R key");
		ImGui::Text("reset pos,velo T key");
		ImGui::Text("start SPACE key");
		
		ImGui::DragFloat("refrect", &refrect, 0.1f);
		ImGui::DragFloat3("setVelo", &setVelo.x, 0.01f);
		ImGui::Separator();
		ImGui::DragFloat("ball1.mass", &ball1.mass);
		ImGui::DragFloat("ball1.rad", &scale1,0.1f);
		ImGui::InputFloat3("ball1.velocity", &ball1.velocity.x, "%.2f");
		ImGui::DragFloat3("ball1.pos", &sphere1.transform.translate.x, 0.1f);
		ImGui::Separator();
		ImGui::DragFloat("ball2.mass", &ball2.mass);
		ImGui::DragFloat("ball2.rad", &scale2, 0.1f);
		ImGui::InputFloat3("ball2.velocity", &ball2.velocity.x, "%.2f");
		ImGui::DragFloat3("ball2.pos", &sphere2.transform.translate.x, 0.1f);

		ImGui::TreePop();
	}
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
	//	if (ImGui::CollapsingHeader("Gizmos")) {
	//		ImGuiManager::GetInstance()->RenderGizmo2(mm, *camera.get(), "buil");
	//		ImGuiManager::GetInstance()->RenderGizmo2(mm2, *camera.get(), "buil2");
	//		ImGuiManager::GetInstance()->RenderGizmo2(tail, *camera.get(), "tail");
	//		ImGuiManager::GetInstance()->RenderGizmo2(walk, *camera.get(), "walk");
	//		ImGuiManager::GetInstance()->RenderGizmo2(multiMesh, *camera.get(), "multiMesh");
	//
	//	}
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
	}





	



	//walk.UpdateSkinning();
	mm.Update();
	mm2.UpdateAnimation();
	multiMesh.Update();
	tail.Update();


	//ocean_.Update();

	sprite.Update();


	sphere1.Update();
	sphere2.Update();

	lineDraw_.Update();
	lineDraw2_.Update();
}

void TestScene::Draw3D()
{


	walk.DrawSkinning();
	walk.DrawLine();
	tail.Draw();
	//multiMesh.Draw();
	//mm.Draw();
	//mm2.Draw();



	sphere1.Draw();
	sphere2.Draw();


	//ocean_.Draw();
	if (ball1.velocity.Length() == 0) {
		lineDraw_.Draw3D(sphere1.transform.translate, sphere1.transform.translate + (setVelo.Normalize() * 10), { 1,1,0,1 });

	}
	else {
		lineDraw_.Draw3D(sphere1.transform.translate, sphere1.transform.translate + (ball1.velocity.Normalize() * 10), { 1,0,0,1 });
	}
	lineDraw2_.Draw3D(sphere2.transform.translate, sphere2.transform.translate + (ball2.velocity.Normalize() * 10), { 1,0,0,1 });
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

