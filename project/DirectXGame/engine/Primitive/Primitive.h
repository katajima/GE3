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




namespace ShapeParameter {

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
			return flag == other.flag && num == other.num && count == other.count && interval == other.interval && width == other.width && height == other.height && direction == other.direction;
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


		// == オペレーター
		bool operator==(const Star& other) const {
			return innerRadius_ == other.innerRadius_ && outerRadius_ == other.outerRadius_ && segments_ == other.segments_;
		}

		// != オペレーター
		bool operator!=(const Star& other) const {
			return !(*this == other);
		}
	};

	struct Crescent
	{
		float innerRadius_;
		float outerRadius_;
		int segments_;
		float distance_;

		// == オペレーター
		bool operator==(const Crescent& other) const {
			return innerRadius_ == other.innerRadius_ && outerRadius_ == other.outerRadius_ && segments_ == other.segments_ && distance_ == other.distance_;
		}

		// != オペレーター
		bool operator!=(const Crescent& other) const {
			return !(*this == other);
		}
	};

	struct Ring {
		float innerRadius_;
		float outerRadius_;
		int segments_;

		// == オペレーター
		bool operator==(const Ring& other) const {
			return innerRadius_ == other.innerRadius_ && outerRadius_ == other.outerRadius_ && segments_ == other.segments_;
		}

		// != オペレーター
		bool operator!=(const Ring& other) const {
			return !(*this == other);
		}
	};

	struct SphereShep {
		float radius_;

		// == オペレーター
		bool operator==(const SphereShep& other) const {
			return radius_ == other.radius_;
		}

		// != オペレーター
		bool operator!=(const SphereShep& other) const {
			return !(*this == other);
		}
	};

	struct Cylinder {
		float radius_;
		float height_;
		int segments_;

		// == オペレーター
		bool operator==(const Cylinder& other) const {
			return radius_ == other.radius_ && height_ == other.height_ && segments_ == other.segments_;
		}

		// != オペレーター
		bool operator!=(const Cylinder& other) const {
			return !(*this == other);
		}
	};

	struct Tube {
		float radius_;
		float innerRadius_;
		float height_;
		int segments_;

		// == オペレーター
		bool operator==(const Tube& other) const {
			return innerRadius_ == other.innerRadius_ && radius_ == other.radius_ && segments_ == other.segments_ && height_ == other.height_;
		}

		// != オペレーター
		bool operator!=(const Tube& other) const {
			return !(*this == other);
		}
	};

	struct Pyramid {
		float radius_;
		float height_;
		int segments_;

		// == オペレーター
		bool operator==(const Pyramid& other) const {
			return radius_ == other.radius_ && segments_ == other.segments_ && height_ == other.height_;
		}

		// != オペレーター
		bool operator!=(const Pyramid& other) const {
			return !(*this == other);
		}
	};

	struct Torus {
		float innerRadius_;
		float outerRadius_;
		int segments_;
		int tubeSegments_;


		// == オペレーター
		bool operator==(const Torus& other) const {
			return innerRadius_ == other.innerRadius_ && segments_ == other.segments_ && outerRadius_ == other.outerRadius_ && tubeSegments_ == other.tubeSegments_;
		}

		// != オペレーター
		bool operator!=(const Torus& other) const {
			return !(*this == other);
		}
	};
}

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

	//

private:

	ShapeParameter::Cube cube;
	ShapeParameter::Cube oCube;

	ShapeParameter::Star star;
	ShapeParameter::Star oStar;

	ShapeParameter::Crescent crescent;
	ShapeParameter::Crescent oCrescent;

	ShapeParameter::Ring ring;
	ShapeParameter::Ring oRing;

	ShapeParameter::ShapeCross cross_;
	ShapeParameter::ShapeCross oCross_;

	ShapeParameter::Spring spring;
	ShapeParameter::Spring oSpring;

	ShapeParameter::AnimationPlane anime;
	ShapeParameter::AnimationPlane oAnime;

	ShapeParameter::SphereShep sphere;
	ShapeParameter::SphereShep oSphere;

	ShapeParameter::Cylinder cylinder;
	ShapeParameter::Cylinder oCylinder;

	ShapeParameter::Tube tube;
	ShapeParameter::Tube oTube;

	ShapeParameter::Pyramid pyramid;
	ShapeParameter::Pyramid oPyramid;

	ShapeParameter::Torus torus;
	ShapeParameter::Torus oTorus;


public: //セッター
	void SetParametar(const ShapeParameter::AnimationPlane& primi) { anime = primi; };
	//void SetParametar(const float& primi) { radius_ = primi; };

	void SetName(const std::string str) { name_ = str; };

	void SetCollider();

	void SetStar(ShapeParameter::Star& _star);

public: // ゲッター
	Vector3 GetCubeSize() const { return cube.size; };

	//float GetRad() const { return radius_; }
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
