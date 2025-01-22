#pragma once
#include "DirectXGame/engine/struct/Vector4.h"
#include "DirectXGame/engine/struct/Vector3.h"
#include "DirectXGame/engine/struct/Vector2.h"
#include "DirectXGame/engine/struct/Quaternion.h"
#include "DirectXGame/engine/struct/Matrix4x4.h"


//行列
struct Matrix3x3
{
	float m[3][3];
};








//Transform
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

//AABB
struct AABB {
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};


// 円
struct Sphere {
	Vector3 center; //!<中心点
	float radius;   //!<半径 
};

//平面
struct Plane {
	Vector3 normal;  //!< 法線
	float distance; //!< 距離 
};



//直線
struct Line
{
	Vector3 origin; //!<始点
	Vector3 diff;   //!<終点への差分ベクトル
};
//半直線
struct Ray
{
	Vector3 origin; //!<始点
	Vector3 diff;   //!<終点への差分ベクトル
};
//線分
struct Segment
{
	
	Vector3 origin; //!<始点
	Vector3 end;
	Vector3 diff() const {
		return end - origin;
	}
};
//三角形
struct Triangle
{
	Vector3 vertices[3]; // !頂点

	// +=オペレーターのオーバーロード 
	Triangle& operator+=(const Vector3& offset) { 
		for (auto& vertex : vertices) { 
			vertex += offset; 
		} 
		return *this; 
	}

	Triangle OffsetVector3(const Vector3& offset) const {
		Triangle result = *this;  // コピーを作成
		for (auto& vertex : result.vertices) {
			vertex += offset;
		}
		return result;
	}

};

//ばね
struct Spring
{
	Vector3 anchor;      // アンカーの固定された端の位置
	float naturalLength; // 自然長
	float stiffness;     // 剛性。バネ定数k
	float dampingCoefficient; // 減衰係数
};
struct Ball {
	Vector3 position;		//位置
	Vector3 veloctiy;		//速度
	Vector3 acceleration;	//加速度
	float mass;				//質量
	float radius;			//半径
	unsigned int color;		//色
};
struct Pendulum {
	Vector3 anchor;				// アンカーポイント
	float length;				// 紐の長さ
	float angle;				// 現在の角度
	float angularVelocity;		// 角度ω
	float angularAcceleration;	// 角加速度
};
struct ConicalPendulum {
	Vector3 anchor;				// アンカーポイント
	float length;				// 紐の長さ
	float halfApexAngle;		// 円錐の頂角の半分
	float angle;				// 現在の角度
	float angularVelocity;		// 角度ω

};

// カプセル
struct Capsule
{
	Segment segment;
	float radius;
};


struct OBB {
	Vector3 center;
	Vector3 orientations[3];
	Vector3 size;
};
