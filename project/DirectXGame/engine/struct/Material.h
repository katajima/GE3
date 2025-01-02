#pragma once
#include<cstdint>
#include "Structs.h"
#include "string"
#include "vector"
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
using namespace Microsoft::WRL;



//マテリアルデータ
struct MaterialData 
{
	// テクスチャ数
	uint32_t texDiffuseNum = 0;


	std::string textuerFilePath;
	std::string textuerNormalFilePath;
	std::string textuerSpeculerFilePath;

	//テクスチャ番号
	uint32_t textureIndex = 0;
	uint32_t textureNormalIndex = 0;
	uint32_t textuerSpeculerIndex = 0;

};





