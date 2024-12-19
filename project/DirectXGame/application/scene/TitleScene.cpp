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

}

void TitleScene::Draw3D()
{
	
}

void TitleScene::DrawP3D()
{
	ParticleManager::GetInstance()->GetInstance()->Draw();
}

void TitleScene::DrawLine3D()
{
	ParticleManager::GetInstance()->GetInstance()->DrawAABB();
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
	camera->transform_.translate = { 0,100,-60.0f };

	/*cameraDebugT = camera->transform_.translate;
	cameraDebugR = camera->transform_.rotate;*/

	cameraT.y = 1.0f;
}
