#pragma once
#include<cstdint>
#include "Structs.h"

struct DirectionalLight {
	Vector4 color; //!< ライトの色
	Vector3 direction; //!< ライトの向き
	float intensity; //!< 輝度
	float lig; // リグ
	int32_t isLight;
	float padding[2];
	Vector3 groundColor; // 地面色
	float padding2;
	Vector3 skyColor; // 天球色
	float padding3;
	Vector3 groundNormal; // 地面法線方向
};

struct PointLight {
	Vector4 color; //ライト色
	Vector3 position;// ライト位置
	float intensity; // 輝度
	float radius;    // ライトの届く距離
	float decay;     // 減衰率
	float lig; // リグ
	uint32_t isLight;
	//float padding[1]; // パディング
};

struct SpotLight
{
	Vector4 color; //ライト色
	Vector3 position; // ライト位置
	float intensity; //輝度
	Vector3 direction; //!< ライトの向き
	float distance; //!< ライト届く距離
	float decay; //!< 減衰率 
	float cosAngle; //!< スポットライトの余弦
	float cosFalloffStart;
	int32_t isLight;
	//float padding[2];
};
