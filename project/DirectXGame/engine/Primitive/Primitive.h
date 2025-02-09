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
#include"DirectXGame/engine/Line/Line.h"

#include "DirectXGame/engine/collider/Collider.h"

class Primitive : public Collider
{
public:
	enum class ShapeType
	{
		None,			// 形なし(デフォルト)
		Plane,			// 平面
		AnimationPlane, // アニメーション平面
		Triangle,		// 三角形
		Circle,			// 円
		Star,			// 星
		Crescent,		// 三日月
		Ring,			// リング
		Arrow,			// 矢印
		Cross,          // 十字

		Cube,			// 四角形
		Sphere,			// 球
		Cylinder,		// 円柱
		Tube,			// 円筒
		Pyramid,		// 角錐
		Torus,			// トーラス	
		Spring          // ばね
	};
	

	// 形
	void Initialize(ShapeType type,const std::string& tex,const Color color = { 1,1,1,1 },bool isLine = false);


	void Update();

	

	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; };

	Mesh* GetMesh() { return mesh.get(); }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	virtual Vector3 GetCenterPosition() const;

private:
	void MeshInitialize();

	void MeshUpdate();

	void MeshUpdateImGui();
private:

	// 2D
	// 平面
	void CreatePlane();

	// アニメーションを始めるか(flag) 一回で増える個数(num) 何回増やすか(count)  増えるまでの時間(interval)  横幅(width) 縦幅(height)　増える方向XY(direction)
	// directionが(1,0,0)の場合はwidth方向に(direction.x　*　width * num)分ずらして増やしてほしい
	void CreateAnimationPlane(bool flag, bool isLoop,bool isUV,int num,int count ,float interval,float width,float height ,const Vector2& direction);



	// 三角面
	void CreateTriangle();
	// 円
	void CreateCircle(float radius, int segments);
	// リング
	void CreateRing(float innerRadius, float outerRadius, int segments);
	// 星
	void CreateStar(float innerRadius, float outerRadius, int segments);
	// 三日月
	void CreateCrescent(float innerRadius, float outerRadius, float distance, int segments);
	// 矢印
	void CreateArrow(float shaftLength, float shaftRadius, float headLength, float headRadius, int segments);
	// 十字
	void CreateCross(float armLength, float armWidth);

	// 3D

	// 立方体
	void CreateCube(Vector3 size);
	// 球
	void CreateSphere(float radius, int latitudeSegments, int longitudeSegments, bool isTopBased);
	// 円柱
	void CreateCylinder(float radius, float height, int segments);
	// 円筒
	void CreateTube(float radius, float innerRadius, float height, int segments);
	// 角錐
	void CreatePyramid(float height, float radius, int segments);
	// トーラス
	void CreateTorus(float innerRadius, float outerRadius, int tubeSegments, int radialSegments);
	// ばね
	void CreateSpring(float length, float width, float height, int segments, float spacing);
	//
	void CreateSpring(float length, float width, float height, int turns, int segments, float thickness);

public:
	void SetColor(const Color& color) { material->color = color; }

	template<typename T>
	void SetParameter(const T& parame);

public:
	template<typename T>
	struct Parameter
	{
		T parameter;
	};

	Parameter<Spring> spring_;


	struct ShapeCross {
		float armLength;
		float armWidth;
	};

	struct Spring
	{
		float length = 1;
		float width = 1;
		float height = 1;
		int segments = 3;
		float spacing = 1;
		int turns = 3;
		float thickness = 2;
	};

	struct AnimationPlane {
		bool flag;
		bool isLoop;
		bool isUV;
		int num;
		int count;
		float interval;
		float width;
		float height;
		Vector2 direction;

		// == オペレーター
		bool operator==(const AnimationPlane& other) const {
			return flag == other.flag  && num == other.num && count == other.count && interval == other.interval && width == other.width && height == other.height && direction == other.direction;
		}

		// != オペレーター
		bool operator!=(const AnimationPlane& other) const {
			return !(*this == other);
		}
	};

	struct Cube 
	{
		Vector3 size = { 1,1,1 };
	};
	struct  Sphere
	{
		float radius;
	};

	struct Star 
	{
		float innerRadius_ = 2.0f;
		float outerRadius_ = 5.0f;
		int segments_ = 5;
	};


private:

	Cube cube;
	Cube oCube;

	Star star;
	Star oStar;

public: //セッター
	void SetParametar(const AnimationPlane& primi) { anime = primi; };
	void SetParametar(const float& primi) { radius_ = primi; };

	void SetName(const std::string str) { name_ = str; };

	void SetCollider();

	void SetStar(Star& _star);

public: // ゲッター
	Vector3 GetCubeSize() const { return cube.size; };

	float GetRad() const { return radius_; }
private:
	// カメラ
	Camera* camera_ = nullptr;

	std::unique_ptr <LineDraw> line_;
	bool isLine_ = false;

	std::unique_ptr<Mesh> mesh;
	std::unique_ptr<Material> material;
	std::unique_ptr<Transfomation> transfomation = nullptr;

	// 名前
	std::string name_ = "primitive";

	// 図形タイプ
	ShapeType type_;

	
	ShapeCross cross_;
	ShapeCross oCross_;
	

	Spring spring;
	Spring oSpring;

	AnimationPlane anime;
	AnimationPlane oAnime;

	

	float innerRadius_ = 2.0f;
	float outerRadius_ = 5.0f;
	int tubeSegments_ = 5;
	int segments_ = 5;
	float height_ = 2.0f;
	float radius_ = 3.0f;
	float distance_  = 8.0f;


	float oInnerRadius_ = 2.0f;
	float oOuterRadius_ = 5.0f;
	int oTubeSegments_ = 5;
	int oSegments_ = 5;
	float oHeight_ = 2.0f;
	float oRadius_ = 3.0f;
	float oDistance_ = 2.0f;


	float timer_ = 0.0f;
	int count_ = 0;

public:
	Matrix4x4 mat_;
	Transform transform;
};

template<typename T>
inline void Primitive::SetParameter(const T& parame)
{
	//std::is_same<T, Spring>;
}
