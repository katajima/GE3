#pragma once
#include"DirectXGame/engine/struct/Structs.h"
#include"LineCommon.h"
#include"DirectXGame/engine/base/Camera.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>

class LineCommon;

class Line
{
public:
	void Initialize(LineCommon *lineCommon);

	void Update();

	void Draw();

private:
	Camera* camera = nullptr;

	LineCommon* lineCommon_ = nullptr;

	// 頂点データ
	struct VertexData {

		Vector4 position;
	};
	//マテリアルデータ
	struct Material {
		Vector4 color;
	};
	// 座標変換行列データ
	struct TransfomationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	// バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	Material* materialData;

	

	TransfomationMatrix* transfomationMatrixData;
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource;

	Transform cameraTransform;

public:
	Transform transform;
};


