#pragma once
#include<cstdint>
#include "Structs.h"
#include "string"

//マテリアルデータ
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
	int32_t useLig;
	int32_t useHim;
	int32_t useNormalMap;
	int32_t useSpeculerMap;
};

//マテリアルデータ
struct MaterialData {
	std::string textuerFilePath;
	std::string textuerNormalFilePath;
	std::string textuerSpeculerFilePath;

	//テクスチャ番号
	uint32_t textureIndex = 0;
	uint32_t textureNormalIndex = 0;
	uint32_t textuerSpeculerIndex = 0;

	//
	float shininess;

};

struct CameraGPU {
	Vector3 worldPosition;
	float padding[1];
	Vector3 normal;
};

// 座標変換行列データ
struct TransfomationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 worldInverseTranspose;
};



