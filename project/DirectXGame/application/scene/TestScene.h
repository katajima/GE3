#pragma once
#include"DirectXGame/engine/scene/BaseScene.h"
#include"DirectXGame/engine/scene/SceneManager.h"
#include "DirectXGame/engine/input/Input.h"
#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include "DirectXGame/engine/base/TextureManager.h"
#include "DirectXGame/engine/input/Input.h"
#include "DirectXGame/engine/audio/Audio.h"
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include"DirectXGame/engine/Light/LightCommon.h"
#include"DirectXGame/engine/2d/SpriteCommon.h"
#include"DirectXGame/engine/2d/Sprite.h"

#include"DirectXGame/engine/base/ImGuiManager.h"
#include "DirectXGame/engine/effect/Particle/ParticleManager.h"
#include "DirectXGame/engine/effect/Particle/ParticleEmitter.h"
#include"DirectXGame/engine/Line/Line.h"
#include"DirectXGame/engine/effect/Ocean/Ocean.h"
#include"DirectXGame/engine/effect/Trail/TrailEffect.h"

#include"DirectXGame/engine/Primitive/Primitive.h"

class TestScene : public BaseScene
{
public:

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw3D() override;

	void Draw2D() override;

private:
private:

	void InitializeResources();
	void InitializeCamera();
private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;



	// カメラ
	std::unique_ptr < Camera> camera;
	Vector3 cameraR;
	Vector3 cameraT;


	Vector3 cameraDebugT;
	Vector3 cameraDebugR;
	bool flag = false;

	ParticleManager::Constant cons;

	Object3d mm;
	Object3d mm2;
	Object3d tail;

	Object3d multiy;

	
	WorldTransform trans_;

	std::unique_ptr<ParticleEmitter> emitter_  =nullptr;

	std::unique_ptr<ParticleEmitter> emitterEnemy_  =nullptr;

	std::vector<std::unique_ptr<Sprite>> sprite_;
	
	std::unique_ptr < Ocean> ocean_ = nullptr;
	
	//std::unique_ptr<Sprite> sprite2_ = nullptr;

};



