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

#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/base/SrvManager.h"
#include"DirectXGame/engine/effect/Particle/ParticleManager.h"
#include"DirectXGame/engine/effect/Particle/ParticleEmitter.h"
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

#include "DirectXGame/application/scene/SceneFactory.h"
#include "DirectXGame/engine/struct/DeltaTime.h"


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

	void InitializeResource();

public:
	static float const kDeltaTime_;
	static float kTimeSpeed_;
	static float GameTime() { return kDeltaTime_ * kTimeSpeed_; };
	static void hitStop(float time) { hitStopTimer = time; };
	static float hitStopTimer;
private:
	void HitStpoTime();

private:
	std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();

	float fps = 0.0f;
	
	//static const DeltaTime kDeltaTime_;
	
	std::unique_ptr<Primitive> primi = nullptr;
	std::unique_ptr<Primitive> primiPlane = nullptr;
	std::unique_ptr<Primitive> primiStar = nullptr;
	std::unique_ptr<Primitive> primiTrai = nullptr;

	

private:
	// ゲーム終了フラグ
	bool endRequst_ = false;
};

