#include "MyGame.h"



void MyGame::Initialize()
{
	Framework::Initialize();


	scene_ = new GamePlayScene();
	scene_->Initialize();
}

void MyGame::Finalize()
{
	//
	Object3dCommon::GetInstance()->Finalize();
	//
	SpriteCommon::GetInstance()->Finalize();
	// srv
	SrvManager::GetInstance()->Finalize();
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
	// ImGuiの受付開始
	imguiManager->Begin();

	Framework::Update();


	scene_->Update();


	// ImGuiの受付終了
	imguiManager->End();
}

void MyGame::Draw()
{
	// 描画前処理
	SrvManager::GetInstance()->PreDraw();
	dxCommon->PreDraw();

	//////////////---------3Dモデル-------------///////////////

	//// 3Dオブジェクトの描画準備
	Object3dCommon::GetInstance()->DrawCommonSetting();

	scene_->Draw3D();	

	// 2Dオブジェクトの描画準備
	SpriteCommon::GetInstance()->DrawCommonSetting();

	scene_->Draw2D();

	// ImGuiの描画
	imguiManager->Draw();

	//描画後処理
	dxCommon->PostDraw();
}
