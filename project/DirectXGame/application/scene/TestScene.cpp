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

	tail.Initialize();
	tail.SetModel("plane.gltf");
	tail.SetCamera(camera.get());
	tail.transform.scale = { 100,100,100 };

	test = a.Conjugate();

	


}

void TestScene::Finalize()
{
}

void TestScene::Update()
{

	camera->UpdateMatrix();
	LightCommon::GetInstance()->SetLineCamera(camera.get());

#ifdef _DEBUG

	ImGui::Begin("Qua");
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
	test = test.MakeQuaternion(Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	ImGui::InputFloat4("MakeQuaternion", &test.x,"%.2f");
	Matrix4x4 mat4x4 = test.MakeRotateMatrix();
	ImGui::InputFloat4("mat[0][~]", &mat4x4.m[0][0], "%.3f");
	ImGui::InputFloat4("mat[1][~]", &mat4x4.m[1][0], "%.3f");
	ImGui::InputFloat4("mat[2][~]", &mat4x4.m[2][0], "%.3f");
	ImGui::InputFloat4("mat[3][~]", &mat4x4.m[3][0], "%.3f");
	ImGui::End();

	ImGui::Begin("engine");

	if (ImGui::CollapsingHeader("Matelial")) {
		bool is = mm.model->materialData->enableLighting;
		ImGui::Checkbox("is", &is);
		mm.model->materialData->enableLighting = is;
		mm2.model->materialData->enableLighting = is;
		tail.model->materialData->enableLighting = is;

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

		ImGui::SliderFloat("shininess", &tail.model->materialData->shininess, 0.1f, 100.0f);

	}

	if (ImGui::CollapsingHeader("Gizmos")) {
		ImGuiManager::GetInstance()->RenderGizmo2(mm, *camera.get(), "buil");
		ImGuiManager::GetInstance()->RenderGizmo2(mm2, *camera.get(), "buil2");
		ImGuiManager::GetInstance()->RenderGizmo2(tail, *camera.get(), "tail");

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


	mm.Update();
	mm2.Update();

	tail.Update();

	emitter_->Update();
}

void TestScene::Draw3D()
{
	tail.Draw();

	mm.Draw();
	mm2.Draw();
}

void TestScene::DrawP3D()
{
	ParticleManager::GetInstance()->GetInstance()->Draw();
}

void TestScene::DrawLine3D()
{
	ParticleManager::GetInstance()->GetInstance()->DrawAABB();
	//particleManager2_->GetInstance()->DrawAABB();

	//LightCommon::GetInstance()->DrawLightLine();

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

