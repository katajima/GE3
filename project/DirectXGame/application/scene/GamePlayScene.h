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
#include"DirectXGame/engine/scene/SceneManager.h"

#include"DirectXGame/application/base/Enemy.h"
#include"DirectXGame/application/GlobalVariables/GlobalVariables.h"

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

	void UpdateImGui();


	void ApplyGlobalVariables();

private: //レール関係
	void InitializeRail();
	// レール更新
	void UpdateRail();
	// トロッコ更新
	void UpdateTrain();


	// 横回転
	std::vector<Vector3> GenerateSpiralControlPoints(float radius, float height, int numPoints, float turns);

	// 縦回転
	std::vector<Vector3> GenerateVerticalSpiralControlPoints(float radius, float height, int numPoints, float turns);


	void CalculationWorldCameraPosition();

	void CalculationWorld3DReticlePosition();

	void CalculationWorld2DReticlePosition();

	// レティクル
	void UpdateReticle();

	// レーザー更新
	void UpdateLaser();

private:

	void InitializeResources();
	void InitializeCamera();

	/// <summary>
	//衝突判定と応答
	/// </summary>
	void ChekAllCollisions();

	void PlaceObjectsAlongSpline(const std::vector<Vector3>& controlPoints, float spacing);
	
	void PlaceObjectsOnCurve(const std::vector<Vector3>& controlPoints, float spacing);

private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 敵
	std::vector<std::unique_ptr <Enemy>> enemys_;



	// カメラ
	std::unique_ptr < Camera> camera;
	Vector3 cameraR;
	Vector3 cameraT;
	Object3d ca;

	Vector3 cameraDebugT;
	Vector3 cameraDebugR;
	

	bool flag = false;
	Object3d cameraObj_;

	// スプライト
	std::vector<std::unique_ptr<Sprite>> sprites;
	// モデル
	std::vector<std::unique_ptr<Object3d>> railObject;
	std::vector<std::unique_ptr<Object3d>> railObject2;

	// 列車のオブジェクト
	Object3d train;
	Object3d trainTemp;
	// レティクル
	Object3d object3DReticle_;

	// レティクル位置
	Vector2 reticlePos_;


	// レール
	//std::unique_ptr<Object3d> AllrailObject;

	// 建物オブジェクト
	std::vector<std::unique_ptr<Object3d>> buildingObject;
	std::vector<Vector3> buildingPos;

	
	// スプライン曲線
	std::vector<Vector3> controlPoints2_;
	std::vector<std::unique_ptr<Object3d>> controlPointObjects_;


	// レチクル画像
	std::unique_ptr <Sprite> sprite2DReticle_;

	// レーザー
	Object3d laser;



	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing;
	// 線分の数
	const size_t segmentCount = 100;

	float  move_t = 0;

	float move_t2 = 0;

	float moveSpeed = 0.0001f;

	// スコア
	int Score_ = 0;
	std::unique_ptr<Sprite> spriteScoreNum_[4][10];


	// レーザーエネルギー
	float energy_ = 100;
	std::unique_ptr <Sprite> spriteEnergy_;

	float damage_ = 100;

	// 天球
	Object3d objectSkydome_;

	int t = 180;

	//
	std::unique_ptr <Sprite> spriteEnter_;

private:
	const int MaxSprite = 1;

	// 建物
	const int MaxBuildingObject3d = 10;

	// レール
	const int MaxRailObject = 100;
};

