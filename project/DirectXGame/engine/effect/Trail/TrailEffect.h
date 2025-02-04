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

#include"DirectXGame/engine/Mesh/Mesh.h"
#include"DirectXGame/engine/Material/Material.h"
#include "DirectXGame/engine/Transfomation/Transfomation.h"

#include"TrailEffectManager.h"

class Object3d;

class TrailEffect
{
public:
	

	void Initialize(const std::string& tex, float maxtime , const Color color = {1,1,1,1});

	
	void Update(bool& flag, const Object3d& str, const Object3d& end);
	//void Update(bool& flag, const WorldTransform& str, const WorldTransform& end);

	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; };

	void SetObject(Object3d* obj) { object_ = obj; };

	void SetMatrix(Matrix4x4& mat) { mat_ = mat; }

	Mesh* GetMesh() const { return mesh.get(); }

	std::unique_ptr<Mesh> mesh;

private:
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	// カメラ
	Camera* camera_ = nullptr;

	Object3d* object_;

	std::unique_ptr<Material> material;
	std::unique_ptr<Transfomation> transfomation = nullptr;

	Matrix4x4 mat_;
	Matrix4x4 parentTransform_;

	bool flag_ = false;
	Vector3 velocity_; // 速度
	//Object3d object_;

	int timer = 0;
};

