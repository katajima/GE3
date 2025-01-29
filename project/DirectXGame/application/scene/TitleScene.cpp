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
	if (Input::GetInstance()->IsTriggerKey(DIK_RETURN)) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	else if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->IsGamePadTriggered(GamePadButton::GAMEPAD_B)) {
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}

	tail.Update();
	camera->UpdateMatrix();
}

void TitleScene::Draw3D()
{
	
	tail.Draw();
}


void TitleScene::Draw2D()
{
	icon_B->Update();
	icon_B->Draw();
	title->Update();
	title->Draw();
}

void TitleScene::InitializeResources()
{
	// オブジェクト3D
	Object3dCommon::GetInstance()->SetDefaltCamera(camera.get());

	icon_B = std::make_unique<Sprite>();
	icon_B->Initialize("resources/Texture/icon/B.png");
	icon_B->SetPosition({ 640,500 });
	icon_B->SetAnchorPoint({ 0.5f,0.5f });
	icon_B->SetSize({200,200});

	title = std::make_unique<Sprite>();
	title->Initialize("resources/Texture/text/title.png");
	title->SetPosition({ 200,200 });
	//title->SetAnchorPoint({ 0.5f,0.5f });
	title->SetSize(2);


	tail.Initialize();
	tail.SetModel("renga.gltf");
	tail.SetCamera(camera.get());
	tail.worldtransform_.scale_ = { 10,10,10 };

}

void TitleScene::InitializeCamera()
{
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->transform_.rotate = { 1.0f,0,0 };
	camera->transform_.translate = { 0,100,-60.0f };

	/*cameraDebugT = camera->transform_.translate;
	cameraDebugR = camera->transform_.rotate;*/

	cameraT.y = 1.0f;
}
