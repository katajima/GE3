#pragma once
#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/input/Input.h"
#include"DirectXGame/engine/base/WinApp.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"DirectXGame/engine/base/D3DResourceLeakchecker.h"
#include"DirectXGame/engine/2d/Sprite.h"
#include"DirectXGame/engine/2d/SpriteCommon.h"
#include"DirectXGame/engine/base/TextureManager.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include"DirectXGame/engine/3d/Model.h"
#include"DirectXGame/engine/3d/ModelCommon.h"
#include"DirectXGame/engine/3d/ModelManager.h"
#include"DirectXGame/engine/base/Camera.h"
#include"DirectXGame/engine/base/SrvManager.h"
#include"DirectXGame/engine/base/ParticleManager.h"
#include"DirectXGame/engine/base/ParticleEmitter.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/audio/Audio.h"
#include"DirectXGame/engine/base/StringUtility.h"
#include"DirectXGame/engine/base/Logger.h"
#include"DirectXGame/engine/3d/LineCommon.h"
#include"DirectXGame/engine/3d/Line.h"
#include"DirectXGame/engine/scene/SceneManager.h"
#include"DirectXGame/engine/scene/AbstractSceneFactory.h"
#include"DirectXGame/application/GlobalVariables/GlobalVariables.h"
#include"memory"

// ゲーム全体
class Framework
{
public:
	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 毎フレーム更新
	virtual void Update();

	// 描画
	virtual void Draw() = 0;

	// 終了フラグのチェック
	virtual bool IsEndRequst() { return endRequst_; }
	
	virtual ~Framework() = default;

	// 実行
	void Run();

protected:
	// ゲーム終了フラグ
	bool endRequst_ = false;
	// ウィンアップ
	
	std::unique_ptr < WinApp> winApp;
	//
	Input* input = nullptr;

	// ダイレクトX
	std::unique_ptr < DirectXCommon> dxCommon = nullptr;
	// SRVマネージャー
	SrvManager* srvManager = nullptr;
	// ImGuiマネージャー
	ImGuiManager* imguiManager;
	// スプライトこもん
	SpriteCommon* spriteCommon = nullptr;
	// オブジェクトコモン
	Object3dCommon* object3dCommon = nullptr;
	// モデルコモン
	std::unique_ptr <ModelCommon> modelCommon;
	// ラインコモン
	//LineCommon* lineCommon = nullptr;

	ParticleManager* particleManager_ = nullptr;
	
	ParticleEmitter* emitter = nullptr;

	// シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;
};

