#pragma once
#include<cstdint>
#include<string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include<vector>

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


#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include"DirectXGame/engine/base/StringUtility.h"
#include"DirectXGame/engine/base/Logger.h"


#include"externals/imgui/imgui.h"

#include"Framework.h"

class  MyGame : public Framework
{
public:
	
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;

	// 横回転
	std::vector<Vector3> GenerateSpiralControlPoints(float radius, float height, int numPoints, float turns);

	// 縦回転
	std::vector<Vector3> GenerateVerticalSpiralControlPoints(float radius, float height, int numPoints, float turns);

private:
	//D3DResourceLeakchecker leakCheck;

	
	//インプット
	Input* input = nullptr;
	// 音
	Audio* audio = nullptr;
	
	// カメラ
	Camera* camera;
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


	// パーティクルマネジャー
	ParticleManager* particleManager;
	// パーティクルエミッター
	ParticleEmitter* emitter;
	
	Line* line;

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
	// ゲーム終了フラグ
	bool endRequst_ = false;



	const int MaxSprite = 1;

	// 建物
	const int MaxBuildingObject3d = 5;

	// レール
	const int MaxRailObject = 100;

#pragma region MyRegion 
#ifdef _DEBUG
	char buf[256];  // バッファサイズを固定
	

	float f = 0.0f;

	bool my_tool_active = true;
	float my_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Initial color

	// スプライト
	Vector2 aa = Vector2(100, 100);
	Vector3 cameraPos{ 0,10,0 };
	Vector3 cameraRotate{ 0.84f,0,0 };

	Vector3 axisPos{};
	Vector3 axisRotate{};

	Vector3 offset{};
#endif // _DEBUG
#pragma endregion //ImGui試し用変数

};

