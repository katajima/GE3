#pragma once
#include"DirectXGame/engine/2d/SpriteCommon.h"
#include "DirectXGame/engine/base/TextureManager.h"


// ゲームプレイシーン
class GamePlayScene
{
public:
	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 毎フレーム更新
	void Update();

	// 描画
	void Draw();
private:
	//DirectXCommon* dxCommon_ = nullptr;
	//Input* input_ = nullptr;
	//Audio* audio_ = nullptr;
};

