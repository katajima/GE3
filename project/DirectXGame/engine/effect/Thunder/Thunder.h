#pragma once
#include <windows.h>
#include<cstdint>
#include<string>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
using namespace Microsoft::WRL;
#include<vector>
#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/struct/Material.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"DirectXGame/engine/base/SrvManager.h"
#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/Material/Material.h"

#include"DirectXGame/engine/Light/LightCommon.h"
#include<random>
#include<numbers>



class ThunderManager;

class Thunder
{
public:
	

	// 大きさ
	void Initialize(Vector2 renge);

	void Update();

	void Draw();

	void SetCamera(Camera* camera) { this->camera = camera; }


public:
	std::unique_ptr<Material> material = nullptr;

	Matrix4x4 mat_;

	Transform transform;


private:
	struct ThunderParameters
	{
		float falloffRate;        // 例: 10.0
		Vector3 baseColorStart;    // 例: {0.0f, 0.2f, 0.8f}
		Vector3 baseColorEnd;      // 例: {0.6f, 0.8f, 1.0f}
		float alphaScale;         // 例: 1.0
	};
	Microsoft::WRL::ComPtr < ID3D12Resource> thunderResource;
	ThunderParameters* thunderParameters;

	// 座標変換行列データ
	struct TransfomationMatrixThunder
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 worldInverseTranspose;
		float time; // 時間 (秒単位)
		float padding[3]; // アライメント調整用
	};

	// 頂点データ
	struct VertexData {

		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	//モデルデータ
	struct ModelData
	{
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices; // 追加：インデックスデータ
		std::vector<VertexData> indicesPos;
		MaterialData material;
	};
	ModelData modeldata; //LoadOdjFile("resources", "fence.obj");

	

	TransfomationMatrixThunder* transfomationMatrixData;
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource;

	// バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;


	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource;
	uint32_t* indexData = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;


	Camera* camera = nullptr;
};
