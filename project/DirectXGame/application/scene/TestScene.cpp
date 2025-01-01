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


	particleManager_ = ParticleManager::GetInstance();

	ParticleManager::GetInstance()->CreateParticleGroup("aa", "resources/Texture/uvChecker.png", ModelManager::GetInstance()->FindModel("plane.obj"), camera.get());
	ParticleManager::GetInstance()->SetCamera(camera.get());

	particleManager2_ = ParticleManager::GetInstance();
	ParticleManager::GetInstance()->CreateParticleGroup("bb", "resources/Texture/aa.png", ModelManager::GetInstance()->FindModel("plane.obj"), camera.get());
	ParticleManager::GetInstance()->SetCamera(camera.get());

	ParticleManager::GetInstance()->SetPos("aa", { 0,0,0 });
	ParticleManager::GetInstance()->SetPos("bb", { 10,0,0 });


	//particleManager_->Emit("aa", Vector3(100.0f, 10.0f, 0.0f) , 100);
	emitter_ = new ParticleEmitter("aa", Transform{ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0,0,0) }, 100, 1.0f, 5.0f);

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
	
	walk.Initialize();
	walk.SetModel("walk.gltf");
	walk.SetCamera(camera.get());
	walk.transform.translate = { 0,10,0 };
	walk.transform.scale = { 20,20,20 };

	test = a.Conjugate();

	
	ocean_.Initialize(Vector2{30,30});
	ocean_.SetCamera(camera.get());
	ocean_.transform.rotate.x = DegreesToRadians(-90);
	



}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
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

	
	/*mm.model->modelData.materials[1].materialData->color;
	mm.model->modelData.materials[1].materialData->uvTransform;
	mm.model->modelData.materials[1].materialData->color;
*/

	camera->UpdateMatrix();
	LightCommon::GetInstance()->SetLineCamera(camera.get());

#ifdef _DEBUG
	Quaternion rotation0{};
	rotation0.MakeQuaternion(Vector3{ 0.71f,0.71f,0.0f }, 0.3f);
	Quaternion rotation1 = -rotation0;

	Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
	Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
	Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
	Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
	Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);


	ImGui::Begin("Quaternion");
	ImGui::InputFloat4("interpolate0,Slerp(q0, q1, 0.0f)", &interpolate0.x,"%.2f");
	ImGui::InputFloat4("interpolate0,Slerp(q0, q1, 0.3f)", &interpolate1.x,"%.2f");
	ImGui::InputFloat4("interpolate0,Slerp(q0, q1, 0.5f)", &interpolate2.x, "%.2f");
	ImGui::InputFloat4("interpolate0,Slerp(q0, q1, 0.7f)", &interpolate3.x, "%.2f");
	ImGui::InputFloat4("interpolate0,Slerp(q0, q1, 1.0f)", &interpolate4.x, "%.2f");
	ImGui::End();
	ImGui::Begin("Quaternion");
	test = a.IdentityQuaternion();
	ImGui::InputFloat4("Identity", &test.x);
	test = a.Conjugate();
	ImGui::InputFloat4("Conjugate", &test.x);
	float norm = a.Norm();
	ImGui::InputFloat("Norm", &norm,0,0,"%.2f");
	test = a.Normalize();
	ImGui::InputFloat4("Normalize", &test.x,"%.2f");
	test = Inverse(a);
	ImGui::InputFloat4("Inverse", &test.x,"%.2f");
	test.MakeQuaternion(Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	ImGui::InputFloat4("MakeQuaternion", &test.x,"%.2f");
	Matrix4x4 mat4x4 = test.MakeRotateMatrix();
	ImGui::InputFloat4("mat[0][~]", &mat4x4.m[0][0], "%.3f");
	ImGui::InputFloat4("mat[1][~]", &mat4x4.m[1][0], "%.3f");
	ImGui::InputFloat4("mat[2][~]", &mat4x4.m[2][0], "%.3f");
	ImGui::InputFloat4("mat[3][~]", &mat4x4.m[3][0], "%.3f");
	Vector3 vec = test.RotateVector(Vector3{2.1f,-0.9f,1.3f});
	ImGui::InputFloat3("rotateByQuaternion", &vec.x, "%.2f");
	vec = Transforms(Vector3{ 2.1f,-0.9f,1.3f }, mat4x4);
	ImGui::InputFloat3("rotateByMatrix", &vec.x, "%.2f");
	ImGui::End();

	/*ImGui::Begin("Matrix4x4");
	Matrix4x4 oo{};
	Matrix4x4 oo2{};
	Matrix4x4 oo3{};

	ImGui::InputFloat4("mat[0][~]", &oo3.m[0][0], "%.3f");
	ImGui::InputFloat4("mat[1][~]", &oo3.m[1][0], "%.3f");
	ImGui::InputFloat4("mat[2][~]", &oo3.m[2][0], "%.3f");
	ImGui::InputFloat4("mat[3][~]", &oo3.m[3][0], "%.3f");

	ImGui::End();*/
	ImGui::Begin("ObjectSize");
	int i = (int)mm.model->modelData.mesh[0]->indices.size();
	ImGui::InputInt("Index size",&i);
	i = (int)mm.model->modelData.mesh[0]->vertices.size();
	ImGui::InputInt("Vertex size",&i);
	i = (int)mm2.model->modelData.mesh[0]->indices.size();
	ImGui::InputInt("Index size",&i);
	i = (int)mm2.model->modelData.mesh[0]->vertices.size();
	ImGui::InputInt("Vertex size",&i);
	i = (int)tail.model->modelData.mesh[0]->indices.size();
	ImGui::InputInt("Index size",&i);
	i = (int)tail.model->modelData.mesh[0]->vertices.size();
	ImGui::InputInt("Vertex size",&i);
	i = (int)walk.model->modelData.mesh[0]->indices.size();
	ImGui::InputInt("Index size",&i);
	i = (int)walk.model->modelData.mesh[0]->vertices.size();
	ImGui::InputInt("Vertex size",&i);
	
	i = (int)multiMesh.model->modelData.mesh[0]->indices.size();
	ImGui::InputInt("multiMesh.Index size",&i);
	i = (int)multiMesh.model->modelData.mesh[0]->vertices.size();
	ImGui::InputInt("multiMesh.Vertex size",&i);
	i = (int)multiMesh.model->modelData.mesh[1]->indices.size();
	ImGui::InputInt("multiMesh.Index2 size",&i);
	i = (int)multiMesh.model->modelData.mesh[1]->vertices.size();
	ImGui::InputInt("multiMesh.Vertex2 size",&i);

	ImGui::End();

	ImGui::Begin("engine");

	if (ImGui::CollapsingHeader("Matelial")) {
		/*bool is = mm.model->materialData->enableLighting;
		ImGui::Checkbox("is", &is);
		mm.model->materialData->enableLighting = is;
		mm2.model->materialData->enableLighting = is;
		tail.model->materialData->enableLighting = is;
		walk.model->materialData->enableLighting = is;

		bool is2 = mm.model->materialData->useLig;
		ImGui::Checkbox("useLig", &is2);
		mm.model->materialData->useLig = is2;
		mm2.model->materialData->useLig = is2;
		tail.model->materialData->useLig = is2;

		bool is3 = mm.model->materialData->useHim;
		ImGui::Checkbox("useHim", &is3);
		mm.model->materialData->useHim = is3;
		mm2.model->materialData->useHim = is3;
		tail.model->materialData->useHim = is3;

		ImGui::SliderFloat("shininess", &tail.model->materialData->shininess, 0.1f, 100.0f);*/

	}

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

	if (ImGui::TreeNode("Test")) {
		ImGui::Text("Camera1");
		ImGui::SliderFloat("値", &camera->transform_.rotate.x, 0.0f, 1.0f);
		//ImGui::TreePop();
		ImGui::Text("Camera2");
		ImGui::SliderFloat("値", &camera->transform_.rotate.x, 0.0f, 1.0f);
		ImGui::TreePop();
	}


	ImGui::End();
#endif

	if (Input::GetInstance()->IsTriggerKey(DIK_P)) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}




	//walk.Update();
	walk.UpdateSkinning();
	mm.Update();
	mm2.UpdateAnimation();
	multiMesh.Update();
	tail.Update();

	emitter_->Update();

	ocean_.Update();
}

void TestScene::Draw3D()
{
	walk.DrawSkinning();
	walk.DrawLine();
	tail.Draw();
	multiMesh.Draw();
	mm.Draw();
	mm2.Draw();
}

void TestScene::DrawP3D()
{
	ParticleManager::GetInstance()->GetInstance()->Draw();

	//ocean_.Draw();

}

void TestScene::DrawLine3D()
{
	ParticleManager::GetInstance()->GetInstance()->DrawAABB();

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
	camera->transform_.rotate = { 1.0f,0,0 };
	camera->transform_.translate = { 0,100,-60.0f };

	/*cameraDebugT = camera->transform_.translate;
	cameraDebugR = camera->transform_.rotate;*/

	cameraT.y = 1.0f;
}

