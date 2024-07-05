#pragma once
#include"Structs.h"
#include"MathFanctions.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
using namespace Microsoft::WRL;
class SpriteCommon;

class Sprite
{
public:// メンバ関数
	// 初期化
	void Initialize(SpriteCommon* spriteCommon);
	// 更新
	void Update();
	//
	void Draw();
private:
	SpriteCommon* spriteCommon_ = nullptr;

	//頂点データ
	struct VertexData {

		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	//マテリアルデータ
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
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
	uint32_t* indexData = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	Material* materialData;

	TransfomationMatrix* transfomationMatrixData;
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResource;
};

