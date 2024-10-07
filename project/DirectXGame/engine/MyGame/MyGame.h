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

class  MyGame
{
public:
	
	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 毎フレーム更新
	void Update();

	// 描画
	void Draw();

	// 終了フラグのチェック
	bool IsEndRequst() { return endRequst_; }

private:
	D3DResourceLeakchecker leakCheck;

	// ウィンアップ
	WinApp* winApp = nullptr;
	//インプット
	Input* input = nullptr;
	// 音
	Audio* audio = nullptr;
	// ダイレクトX
	DirectXCommon* dxCommon = nullptr;
	// SRVマネージャー
	SrvManager* srvManager = nullptr;
	// ImGuiマネージャー
	ImGuiManager* imguiManager;
	// スプライトこもん
	SpriteCommon* spriteCommon = nullptr;
	// カメラ
	Camera* camera;
	// オブジェクトコモン
	Object3dCommon* object3dCommon = nullptr;
	// モデルコモン
	ModelCommon* modelCommon = nullptr;
	// スプライト
	std::vector<Sprite*> sprites;
	// モデル
	std::vector<Object3d*> object3ds;
	// パーティクルマネジャー
	ParticleManager* particleManager;
	// パーティクルエミッター
	ParticleEmitter* emitter;
	
private:
	// ゲーム終了フラグ
	bool endRequst_ = false;



	const int MaxSprite = 1;
	const int MaxObject3d = 2;

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

