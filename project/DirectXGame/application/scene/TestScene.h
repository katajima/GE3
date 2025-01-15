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
#include"DirectXGame/engine/3d/LightCommon.h"
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

	// 列車のオブジェクト

	
	//ModelManager* modelm_;

	ParticleManager::Constant cons;

	Object3d mm;
	Object3d mm2;
	Object3d tail;
	Object3d walk;
	Object3d multiMesh;


	Object3d testObject;

	struct BallLLL {
		Vector3 velocity;
		float mass;
		float rad;
	}; 
	BallLLL ball1;
	BallLLL ball2;

	Vector3 setVelo;
	float scale1 = 5;
	float scale2 = 7;
	float refrect = 1.0f;
	Object3d sphere1;
	Object3d sphereStr;
	Object3d sphereEnd;
	Object3d sphere2;


	Vector3 rotate_;

	LineDraw lineDraw_;
	LineDraw lineDraw2_;

	Ocean ocean_;

	Vector3 str;
	Vector3 end;
	//確認用
	Quaternion a = {2.0f,3.0f,4.0f,1.0f};
	Quaternion b = {1.0f,3.0f,5.0f,2.0f};
	
	Quaternion test{};

	Sprite sprite;

	std::unique_ptr<TrailEffect> trailEffect_;

	std::unique_ptr<Primitive> primitive;

	bool flag33;
};



