#pragma once
#include"DirectXGame/engine/struct/Structs.h"
#include<vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include<assert.h>
#include<random>
#include<numbers>
#include"Easing.h"


#pragma region Math
static float LerpShortAngle(float a, float b, float t) {
	// 角度差分を求める
	float diff = b - a;

	// 角度を [-2*PI, +2*PI] に補正する
	diff = float(fmod(double(diff + 2.0f) * M_PI, double(4.0f) * M_PI)) - 2.0f * float(M_PI);

	// 角度を [-PI, +PI] に補正する
	if (diff > float(M_PI)) {
		diff -= 2 * float(M_PI);
	}
	if (diff < -float(M_PI)) {

		diff += 2 * float(M_PI);
	}

	return Lerp(a, a + diff, t);
}


Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector2 Add(const Vector2& v1, const Vector2& v2);
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);
float LengthSquared(const Vector3& v);
float Length(const float& v);

float Clamp(float t);
float Clamp(float t, float min, float max);

float Clamp3(float value, float min, float max);


//
float Distance(const Vector3& point1, const Vector3& point2);

//
Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
//
Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);

Vector3 Normalize(const Vector3& v);

//単位行列
Matrix4x4 MakeIdentity4x4();
//行列の積
Matrix4x4 Multiply(const Matrix4x4& v1, const Matrix4x4& v2);

Vector3 Multiply(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(const float& v2, const Vector3& v1);

Vector3 Multiply(const Vector3& v1, const float& v2);
//移動行列
Matrix4x4 MakeTranslateMatrix(const  Vector3& translate);
//拡大縮小行列
Matrix4x4 MakeScaleMatrix(const  Vector3& scale);
//回転行列X
Matrix4x4 MakeRotateXMatrix(float rotate);
//回転行列Y
Matrix4x4 MakeRotateYMatrix(float rotate);
//回転行列Z
Matrix4x4 MakeRotateZMatrix(float rotate);
//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
//転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
//アフィン変換;
Matrix4x4 MakeAffineMatrix(const  Vector3& scale, const  Vector3& rotate, const  Vector3& translate);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);
//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
//透視射影行列
Matrix4x4 MakePerspectiveFovMatrix(float forY, float aspectRatio, float nearClip, float farClip);
//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float leht, float top, float width, float height, float minDepth, float maxDepth);
//座標変換
Vector3 Transforms(const Vector3& vector, const Matrix4x4& matrix);
Vector4 Transforms(const Vector4& vec, const Matrix4x4& mat);
bool IsCollision(const AABB& aabb, const Vector3& point);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
// スプライン曲線
Vector3 CatmullRom(const Vector3& p0, const Vector3 p1, const Vector3 p2, const Vector3 p3, float t);
// スプライン曲線
Vector3 CatmullRom(std::vector<Vector3> controlPoints, float t);
// カーブ上の点を取得 (Catmull-Rom)
Vector3 CatmullRom2(const std::vector<Vector3>& controlPoints, float t);

Vector3 Cross(const Vector3& v1, const Vector3& v2);

//
Vector3 Project(const Vector3& v1, const Vector3& v2);
//最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
//球と四角形に対する最近接点
Vector3 ClosestPointAABBSphere(const Sphere& sphere, const AABB& aabb);
//衝突判定(球と球)
bool IsCollision(const Sphere& s1, const Sphere& s2);
//衝突判定(球と平面)
bool IsCollision(const Sphere& sphere, const Plane& plane);
//衝突判定(線と平面)
bool IsCollision(const Segment& segment, const Plane& plane);
//三角形と線の衝突判定
bool IsCollision(const Triangle& triangle, const Segment& segment);
//三角形とカプセルの衝突判定
bool IsCollision(const Triangle& triangle, const Capsule& capsule);
//四角形と四角形の衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);
//四角形と球の衝突判定
bool IsCollision(const AABB& aabb, const Sphere& sphere);
//四角形と線の衝突判定
bool IsCollision(const AABB& aabb, const Segment& segment);
//
// 線分と球の衝突判定 
bool IsCollision(const Sphere& sphere, const Segment& segment);
//
bool IsCollision(const OBB& obb, const Sphere& sphere);
//
bool IsCollision(const OBB& obb, const Segment& segment);
bool IsCollision(const OBB& obb, const Line& line); 
bool IsCollision(const OBB& obb, const Ray& ray);
//
bool IsCollision(const OBB& obb0, const OBB& obb1);
// 
bool IsCollision(const Capsule& cap0, const Capsule& cap1);

Vector3 ClosestPointSegmentTriangle(const Segment& segment, const Triangle& triangle);

Vector3 ClosestPointSegmentSegment(const Segment& seg1, const Segment& seg2, Vector3 currentClosest = {});

Vector3 ClosestPointSegment(const Segment& segment, const Vector3& point);

Vector3 ClosestPointOnPlane(const Plane& plane, const Vector3& point);

float PointLineDistanceSquared(const Vector3& point, const Vector3& a, const Vector3& b);

float SegmentSegmentDistanceSquared(const Segment& seg1, const Segment& seg2);

float SegmentClosestDistanceSq(const Segment& seg0, const Segment& seg1);
//
bool IsPointInsideAABB(const Vector3& point, const AABB& aabb);
//
Vector3 Perpendicular(const Vector3& vector);

Plane PlaneFromPoints(const Vector3& p1, const Vector3& p2, const Vector3& p3);
//反射ベクトル
Vector3 Reflect(const Vector3& input, const Vector3& normal);
Vector3 Reflect(const Vector3& input, const Vector3& normal, float restitution);

// 2点間の距離を計算する関数
float DistanceXZ(const Vector3& a, const Vector3& b);

// アーク長を計算する関数
float CalculateArcLength(const std::vector<Vector3>& controlPoints, int numSamples);

// アーク長に基づく位置を取得する関数
float FindTByArcLength(const std::vector<Vector3>& controlPoints, float targetLength, int numSamples);


// 曲線を細かくサンプリングし、累積アーク長を計算
std::vector<std::pair<float, float>> CalculateArcLengths(const std::vector<Vector3>& controlPoints, int numSamples);

// アーク長からtを逆算する関数
float GetTFromArcLength(const std::vector<std::pair<float, float>>& arcLengths, float targetLength);

// 曲率を計算する関数
float Curvature(const Vector3& p0, const Vector3& p1, const Vector3& p2);

// 曲率に基づくアダプティブサンプリング
std::vector<float> AdaptiveSampling(const std::vector<Vector3>& controlPoints, int baseSamples);


// 行列のポインタを取得する関数
const float* GetMatrixPointer(const Matrix4x4& mat);

float* GetMatrix(Matrix4x4 mat);

#pragma endregion //数学関数

float DegreesToRadians(float degrees);

Vector3 DegreesToRadians(Vector3 degrees);

float RadiansToDegrees(float radians);

Vector3 RadiansToDegrees(Vector3 degrees);

// ランダム
//template <typename T>
static Vector3 RandomMinMax(Vector3& min, Vector3& max, std::mt19937& randomEngine) {
	// 範囲の確認と修正
	if (min.x > max.x) std::swap(min.x, max.x);
	if (min.y > max.y) std::swap(min.y, max.y);
	if (min.z > max.z) std::swap(min.z, max.z);

	// 各軸の乱数生成
	std::uniform_real_distribution<float> distX(min.x, max.x);
	std::uniform_real_distribution<float> distY(min.y, max.y);
	std::uniform_real_distribution<float> distZ(min.z, max.z);

	return Vector3{
		distX(randomEngine),
		distY(randomEngine),
		distZ(randomEngine)
	};
}

// ランダムな位置を生成する関数
Vector3 GenerateRandomPosition(const Vector3& min, const Vector3& max, std::mt19937& randomEngine);


static std::pair<Vector3, Vector3> ComputeCollisionVelocities(float mass1, const Vector3& velocity1, float mass2, const Vector3& velocity2,
	float coefficientOfRestitution, const Vector3& normal) {
	// 衝突面法線方向の速度成分を射影
	Vector3 project1 = Project(velocity1, normal); // 質点1の法線方向成分
	Vector3 project2 = Project(velocity2, normal); // 質点2の法線方向成分
	Vector3 sub1 = velocity1 - project1;           // 質点1の接線方向成分
	Vector3 sub2 = velocity2 - project2;           // 質点2の接線方向成分

	// 衝突後の法線方向速度を計算 (反発係数と運動量保存則に基づく)
	Vector3 relativeVelocity = project1 - project2; // 衝突前の相対速度（法線方向）
	float impulse = (-(1 + coefficientOfRestitution) * Dot(relativeVelocity, normal)) /
		(1.0f / mass1 + 1.0f / mass2); // 衝撃量の計算
	Vector3 impulseVector = normal * impulse;       // 衝撃量ベクトル

	// 衝突後の法線方向速度を更新
	Vector3 velocityAfter1 = project1 + impulseVector / mass1;
	Vector3 velocityAfter2 = project2 - impulseVector / mass2;

	// 接線成分を加算して最終的な速度を計算
	return std::make_pair(velocityAfter1 + sub1, velocityAfter2 + sub2);
}


