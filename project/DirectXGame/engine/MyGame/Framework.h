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
#include"DirectXGame/engine/Light/LightCommon.h"
#include"DirectXGame/engine/Skinning/Skinning.h"
#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/base/SrvManager.h"
#include"DirectXGame/engine/effect/Particle/ParticleManager.h"
#include"DirectXGame/engine/effect/Particle/ParticleEmitter.h"
#include"DirectXGame/engine/effect/Ocean/OceanManager.h"
#include"DirectXGame/engine/effect/Thunder/ThunderManager.h"
//#include"DirectXGame/engine/effect/Trail/TrailEffect.h"
#include"DirectXGame/engine/effect/Trail/TrailEffectManager.h"
#include"DirectXGame/engine/Primitive/PrimitiveCommon.h"


#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/Camera/CameraCommon.h"

#include"DirectXGame/engine/audio/Audio.h"
#include"DirectXGame/engine/base/StringUtility.h"
#include"DirectXGame/engine/base/Logger.h"
#include"DirectXGame/engine/Line/LineCommon.h"
#include"DirectXGame/engine/Line/Line.h"
#include"DirectXGame/engine/scene/SceneManager.h"
#include"DirectXGame/engine/scene/AbstractSceneFactory.h"
#include "DirectXGame/engine/base/RenderingCommon.h"
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
	// モデルコモン
	CameraCommon* cameraCommon = nullptr;
	// ラインコモン
	LineCommon* lineCommon = nullptr;
	// 
	OceanManager* oceanManager_ = nullptr;
	//
	ThunderManager* thunderManager = nullptr;

	TrailEffectManager* trailEffectManager_ = nullptr;

	ParticleManager* particleManager_ = nullptr;
	
	ParticleEmitter* emitter = nullptr;

	LightCommon* lightCommon = nullptr;

	RenderingCommon* renderingCommon = nullptr;

	SkinningConmmon* skinningCommon = nullptr;

	PrimitiveCommon* primitiveCommon = nullptr;
	// シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;
};

