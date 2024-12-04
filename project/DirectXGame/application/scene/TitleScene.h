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
#include"DirectXGame/engine/2d/SpriteCommon.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include "DirectXGame/engine/base/ParticleManager.h"
#include "DirectXGame/engine/base/ParticleEmitter.h"
#include"DirectXGame/engine/3d/Line.h"

class TitleScene : public BaseScene
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

	

	LineDraw lineDraw_;
	LineDraw lineDraw2_;

	Vector3 str;
	Vector3 end;

	ImGuiManager* imgM_;

};


