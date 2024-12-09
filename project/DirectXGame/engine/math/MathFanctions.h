#pragma once
#include"DirectXGame/engine/struct/Structs.h"
#include<vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include<assert.h>
#include<random>
#include<numbers>


#pragma region Math

Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector2 Add(const Vector2& v1, const Vector2& v2);
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);
float Length(const float& v);

float Clamp(float t);
float Clamp(float t, float min, float max);
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
Matrix4x4 MakeAffineMatrixMatrix(const  Vector3& scale, const  Vector3& rotate, const  Vector3& translate);
//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
//透視射影行列
Matrix4x4 MakePerspectiveFovMatrix(float forY, float aspectRatio, float nearClip, float farClip);
//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float leht, float top, float width, float height, float minDepth, float maxDepth);
//座標変換
Vector3 Transforms(const Vector3& vector, const Matrix4x4& matrix);

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
bool IsPointInsideAABB(const Vector3& point, const AABB& aabb);
//
Vector3 Perpendicular(const Vector3& vector);

Plane PlaneFromPoints(const Vector3& p1, const Vector3& p2, const Vector3& p3);
//反射ベクトル
Vector3 Reflect(const Vector3& input, const Vector3& normal);


// 2点間の距離を計算する関数
float Distance(const Vector3& a, const Vector3& b);

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
static Vector3 RandomMinMax(Vector3& min,Vector3& max, std::mt19937& randomEngine) {
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
