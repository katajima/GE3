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
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	camera->Update();


	ImGui::Begin("Camera");
	ImGui::DragFloat3("cameraDebugT", &cameraDebugT.x, 0.1f);
	ImGui::DragFloat3("cameraDebugR", &cameraDebugR.x, 0.01f);
	ImGui::InputFloat3("Translate", &camera->transform.translate.x);
	ImGui::DragFloat3("cameraT", &cameraT.x, 0.1f);
	ImGui::InputFloat3("Rotate", &camera->transform.rotate.x);
	ImGui::DragFloat3("CameraR", &cameraR.x, 0.01f);
	ImGui::Checkbox("flag", &flag);
	ImGui::End();

	camera->transform.rotate = cameraDebugR;
	camera->transform.translate = cameraDebugT;

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	// 列車
	train->Update();
}

void TitleScene::Draw3D()
{
	train->Draw();
}

void TitleScene::Draw2D()
{
}

void TitleScene::InitializeResources()
{
	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

	// 列車オブジェクトを unique_ptr で作成
	train = std::make_unique<Object3d>();
	train->Initialize();
	train->SetModel("train.obj");
}

void TitleScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	camera->transform.rotate = { 0.36f,0,0 };
	camera->transform.translate = { 5,32.5f,-59.2f };

	cameraDebugT = camera->transform.translate;
	cameraDebugR = camera->transform.rotate;

	cameraT.y = 1.0f;
}
