#include "MyGame.h"



void MyGame::Initialize()
{
	Framework::Initialize();

	scene_ = new GamePlayScene();
	scene_->Initialize();

}

void MyGame::Finalize()
{


	dxCommon->Finalize();
	//DirectXCommon::GetInstance()->Finalize();
	//
	Audio::GetInstance()->Finalize();
	//テクスチャマネージャーの終了
	TextureManager::GetInstance()->Finalize();
	// モデルマネージャーの終了
	ModelManager::GetInstance()->Finalize();
	//

	scene_->Finalize();
	delete scene_;
	// 基底クラスの終了処理
	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();
	scene_->Update();
}

void MyGame::Draw()
{
	scene_->Draw();	
}
