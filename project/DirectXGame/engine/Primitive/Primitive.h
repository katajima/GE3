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


class Primitive
{
public:
	enum class ShapeType
	{
		None,			// 形なし(デフォルト)
		Plane,			// 平面
		Triangle,		// 三角形
		Circle,			// 円
		Star,			// 星
		Crescent,		// 三日月

		Cube,			// 四角形
		Sphere,			// 球
		Cylinder,		// 円柱
		Tube,			// 円筒
		Pyramid,		// 角錐
		Torus,			// トーラス	
	};
	ShapeType type;


	// 形
	void Initialize(ShapeType type,const std::string& tex,const Vector4 color = { 1,1,1,1 });


	void Update();

	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; };

private:
	void CreatePlane();

	void CreateCube();

private:
	// カメラ
	Camera* camera_ = nullptr;

	std::unique_ptr<Mesh> mesh;
	std::unique_ptr<Material> material;
	std::unique_ptr<Transfomation> transfomation = nullptr;
public:
	Matrix4x4 mat_;
	Transform transform;
};

