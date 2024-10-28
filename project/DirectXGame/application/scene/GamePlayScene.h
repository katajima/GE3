#pragma once
#include"DirectXGame/engine/2d/SpriteCommon.h"
#include "DirectXGame/engine/base/TextureManager.h"
#include "DirectXGame/engine/input/Input.h"
#include "DirectXGame/engine/audio/Audio.h"
#include "DirectXGame/engine/3d/ModelManager.h"
#include"DirectXGame/engine/base/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/2d/Sprite.h"
#include"DirectXGame/engine/3d/Line.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include"DirectXGame/engine/3d/ModelCommon.h"
#include"DirectXGame/engine/3d/LineCommon.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/scene/BaseScene.h"

// ゲームプレイシーン
class GamePlayScene : public BaseScene
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

	// 横回転
	std::vector<Vector3> GenerateSpiralControlPoints(float radius, float height, int numPoints, float turns);

	// 縦回転
	std::vector<Vector3> GenerateVerticalSpiralControlPoints(float radius, float height, int numPoints, float turns);
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

	// スプライト
	std::vector<std::unique_ptr<Sprite>> sprites;
	// モデル
	std::vector<std::unique_ptr<Object3d>> railObject;

	// 列車のオブジェクト
	std::unique_ptr<Object3d> train;

	// 建物オブジェクト
	std::vector<std::unique_ptr<Object3d>> buildingObject;

	//Line* line;

	// スプライン曲線制御点(通過点)
	std::vector<Vector3> controlPoints_;

	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing;
	// 線分の数
	const size_t segmentCount = 100;

	float  move_t = 0;

	float move_t2 = 0;

	float moveSpeed = 0.0001f;
private:
	const int MaxSprite = 1;

	// 建物
	const int MaxBuildingObject3d = 5;

	// レール
	const int MaxRailObject = 400;
};

