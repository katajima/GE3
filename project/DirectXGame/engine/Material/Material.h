#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include "DirectXGame/engine/Color/Color.h"

#include "vector"
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
using namespace Microsoft::WRL;
#include<d3d12.h>
#include<dxgi1_6.h>

class Material
{
public:
	void Initialize(DirectXCommon* dxcommon);


	void GetCommandListMaterial(int index);

	void GetCommandListTexture(int indexDiffuse, int normalIndex = 0,int speculerIndex = 0);

	static void SetRootParameter(D3D12_ROOT_PARAMETER& parameter, int ShaderRegister);

	void GPUData();
	void LoadTex();
public:
	Transform transform;
	Color color;

	int32_t enableLighting_ = true;
	float shininess_;
	int32_t useLig_ = false;
	
	int32_t useNormalMap_;
	int32_t useSpeculerMap_;

	struct Tex {
		std::string diffuseFilePath;
		std::string normalFilePath;
		std::string speculerFilePath;

		//テクスチャ番号
		uint32_t diffuseIndex = 0;
		uint32_t normalIndex = 0;
		uint32_t speculerIndex = 0;
	};
	Tex tex_;

private:
	// GPUに送るデータ
	struct DataGPU
	{
		Color color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
		int32_t useLig;
		int32_t useHim;
		int32_t useNormalMap;
		int32_t useSpeculerMap;
		float padding2[3];
	};
	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr < ID3D12Resource> resource_;
	DataGPU* data_;


	// テクスチャ数
	uint32_t texDiffuseNum = 0;
};

