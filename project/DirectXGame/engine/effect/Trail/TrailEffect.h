#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<list>
#include<string>
#include<vector>
#include<format>
#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"DirectXGame/engine/Camera/Camera.h"

class Object3d;

class TrailEffect
{
public:
	static TrailEffect* instance;

	static TrailEffect* GetInstance();

	void Initialize(DirectXCommon* dxcommon);

	void Initialize();

	void Finalize();

	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; };

	void TargetObject();

	void Update();
private:


private:
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	// カメラ
	Camera* camera_ = nullptr;


	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices; // 追加：インデックスデータ


	Vector3 velocity_; // 速度
	//Object3d object_;

	int timer = 0;



};

