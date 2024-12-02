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
	particleManager_->CreateParticleGroup("aa", "resources/uvChecker.png", ModelManager::GetInstance()->FindModel("axis.obj"));
	particleManager_->SetCamera(camera.get());
	
	emitter_ = new ParticleEmitter("aa", Transform{ (1.0f, 1.0f, 1.0f),(0.0f, 0.0f, 0.0f),(0.0f, 0.0f, 0.0f) }, 10, 1, 1);
	emitter_->Emit();

	// 列車オブジェクトを unique_ptr で作成
	mm.Initialize();
	mm.SetModel("building.obj");
	mm.transform.translate = {30,1,1};
	mm.SetCamera(camera.get());

}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	camera->UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("cameraDebugT", &cameraDebugT.x, 0.1f);
	ImGui::DragFloat3("cameraDebugR", &cameraDebugR.x, 0.01f);
	ImGui::InputFloat3("Translate", &camera->transform_.translate.x);
	ImGui::DragFloat3("cameraT", &cameraT.x, 0.1f);
	ImGui::InputFloat3("Rotate", &camera->transform_.rotate.x);
	ImGui::DragFloat3("CameraR", &cameraR.x, 0.01f);
	ImGui::Checkbox("flag", &flag);
	ImGui::End();
#endif
	camera->transform_.rotate = cameraDebugR;
	camera->transform_.translate = cameraDebugT;

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	
	// 列車
	title->Update();

	mm.Update();

	emitter_->Update();
}

void TitleScene::Draw3D()
{
	mm.Draw();
	title->Draw();
}

void TitleScene::DrawP3D()
{
	particleManager_->GetInstance()->Draw();
}

void TitleScene::Draw2D()
{
}

void TitleScene::InitializeResources()
{
	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

	// 列車オブジェクトを unique_ptr で作成
	title = std::make_unique<Object3d>();
	title->Initialize();
	//title->SetModel("title.obj");
	title->SetCamera(camera.get());
	title->transform.translate = { 0,0,20 };
}

void TitleScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	camera->transform_.rotate = { 0.0f,0,0 };
	camera->transform_.translate = { 0,0,-30.0f };

	cameraDebugT = camera->transform_.translate;
	cameraDebugR = camera->transform_.rotate;

	cameraT.y = 1.0f;
}
