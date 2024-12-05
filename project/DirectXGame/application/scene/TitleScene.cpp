#include"TitleScene.h"



void TitleScene::Initialize()
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
	
	particleManager_->CreateParticleGroup("aa", "resources/uvChecker.png", ModelManager::GetInstance()->FindModel("plane.obj"),camera.get());
	particleManager_->SetCamera(camera.get());
	
	particleManager2_ = ParticleManager::GetInstance();
	particleManager2_->CreateParticleGroup("bb", "resources/aa.png", ModelManager::GetInstance()->FindModel("plane.obj"),camera.get());
	particleManager2_->SetCamera(camera.get());

	//particleManager_->Emit("aa", Vector3(100.0f, 10.0f, 0.0f) , 100);
	emitter_ =   new ParticleEmitter("aa", Transform{ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0,0,0)}, 100, 1.0f, 5.0f);
	
	// 列車オブジェクトを unique_ptr で作成
	mm.Initialize();
	mm.SetModel("building.obj");
	mm.transform.translate = {30,1,1};
	mm.SetCamera(camera.get());
	
	mm2.Initialize();
	mm2.SetModel("building.obj");
	mm2.transform.translate = {-30,1,1};
	mm2.SetCamera(camera.get());

	
	
	LineCommon::GetInstance()->SetDefaltCamera(camera.get());
	

	
	str = { 0,0,0 };
	end = { 10,0,10 };

	imgM_ = ImGuiManager::GetInstance();
	
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	
	camera->UpdateMatrix();
	LightCommon::GetInstance()->SetLineCamera(camera.get());

#ifdef _DEBUG
	ImGui::Begin("sfaf");
	ImGui::StyleColorsClassic();
	ImGui::End();

	ImGui::Begin("engine");
	
	if (ImGui::CollapsingHeader("Gizmos")) {
		ImGuiManager::GetInstance()->RenderGizmo2(mm, *camera.get(), "buil");
		ImGuiManager::GetInstance()->RenderGizmo2(mm2, *camera.get(), "buil2");

	}
	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::DragFloat3("Translate", &camera->transform_.translate.x, 0.1f);
		ImGui::DragFloat3("Rotate", &camera->transform_.rotate.x, 0.01f);
		ImGui::Checkbox("flag", &flag);
		
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
	
	if (Input::GetInstance()->IsTriggerKey(DIK_RETURN)) {
		// シーン切り替え
		//SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	
	
	mm.Update();
	mm2.Update();

	emitter_->Update();
}

void TitleScene::Draw3D()
{
	mm.Draw();
	mm2.Draw();
}

void TitleScene::DrawP3D()
{
	particleManager_->GetInstance()->Draw();
}

void TitleScene::DrawLine3D()
{
	particleManager_->GetInstance()->DrawAABB();
	particleManager2_->GetInstance()->DrawAABB();
	
	LightCommon::GetInstance()->DrawLightLine();

}

void TitleScene::Draw2D()
{
}

void TitleScene::InitializeResources()
{
	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

}

void TitleScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	camera->transform_.rotate = { 1.0f,0,0 };
	camera->transform_.translate = { 0,150,-90.0f };

	/*cameraDebugT = camera->transform_.translate;
	cameraDebugR = camera->transform_.rotate;*/

	cameraT.y = 1.0f;
}
