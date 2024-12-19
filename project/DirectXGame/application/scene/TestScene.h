#pragma once
#include"DirectXGame/engine/scene/BaseScene.h"
#include"DirectXGame/engine/scene/SceneManager.h"
#include "DirectXGame/engine/input/Input.h"
#include"DirectXGame/engine/base/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include "DirectXGame/engine/base/TextureManager.h"
#include "DirectXGame/engine/input/Input.h"
#include "DirectXGame/engine/audio/Audio.h"
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include"DirectXGame/engine/3d/LightCommon.h"
#include"DirectXGame/engine/2d/SpriteCommon.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include "DirectXGame/engine/effect/ParticleManager.h"
#include "DirectXGame/engine/effect/ParticleEmitter.h"
#include"DirectXGame/engine/Line/Line.h"

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

	void DrawP3D() override;

	void DrawLine3D() override;

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

	ParticleManager* particleManager_;
	ParticleManager* particleManager2_;


	ParticleEmitter* emitter_;

	ModelManager* modelm_;

	Object3d mm;
	Object3d mm2;
	Object3d tail;



	LineDraw lineDraw_;
	LineDraw lineDraw2_;

	Vector3 str;
	Vector3 end;
	//確認用
	Quaternion a = {2.0f,3.0f,4.0f,1.0f};
	Quaternion b = {1.0f,3.0f,5.0f,2.0f};
	
	Quaternion test{};

};



