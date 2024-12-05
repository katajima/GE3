#pragma once
#include<cstdint>
#include "Structs.h"

struct DirectionalLight {
	Vector4 color; //!< ライトの色
	Vector3 direction; //!< ライトの向き
	float intensity; //!< 輝度
};

struct PointLight {
	Vector4 color; //ライト色
	Vector3 position;// ライト位置
	float intensity; // 輝度
	float radius;    // ライトの届く距離
	float decay;     // 減衰率
	float padding[2];
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
	float padding[2];
};
