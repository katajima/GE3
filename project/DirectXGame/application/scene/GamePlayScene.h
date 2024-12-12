#pragma once
#include"DirectXGame/engine/2d/SpriteCommon.h"
#include "DirectXGame/engine/base/TextureManager.h"
#include "DirectXGame/engine/input/Input.h"
#include "DirectXGame/engine/audio/Audio.h"
#include "DirectXGame/engine/3d/ModelManager.h"
#include"DirectXGame/engine/base/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/2d/Sprite.h"
#include"DirectXGame/engine/Line/Line.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/3d/Object3dCommon.h"
#include"DirectXGame/engine/3d/ModelCommon.h"
#include"DirectXGame/engine/Line/LineCommon.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/scene/BaseScene.h"
#include"DirectXGame/engine/scene/SceneManager.h"
#include"DirectXGame/engine/base/LevelData.h"


#include"DirectXGame/application/base/Enemy/Enemy.h"
#include"DirectXGame/application/base/Player/Player.h"
#include"DirectXGame/application/GlobalVariables/GlobalVariables.h"
#include"DirectXGame/application/base/FollowCamera/FollowCamera.h"

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

	void DrawP3D() override;

	void DrawLine3D() override;


	void Draw2D() override;

	void UpdateImGui();

	// レベルデータ
	void LoadLevelData();

	void ApplyGlobalVariables();

private:

	void InitializeResources();
	void InitializeCamera();

	/// <summary>
	//衝突判定と応答
	/// </summary>
	//void ChekAllCollisions();

	
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
	
	Vector3 cameraDebugT;
	Vector3 cameraDebugR;
	

	bool flag = true;
	Object3d cameraObj_;

	// 建物オブジェクト
	std::vector<std::unique_ptr<Object3d>> buildingObject;
	std::vector<Vector3> buildingPos;

	
	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;


	std::vector <Object3d*> objects;

	std::unique_ptr<Player> player_;

	Object3d tail;

private:

};

