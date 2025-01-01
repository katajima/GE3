#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/base/DirectXCommon.h"

#include "vector"
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
using namespace Microsoft::WRL;
#include<d3d12.h>
#include<dxgi1_6.h>


class Mesh
{
public:	
	struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
	};

	// カスタムハッシュ関数
	struct VertexHash {
		size_t operator()(const VertexData& vertex) const {
			// 簡単なハッシュ関数（例として位置のみを使用）
			return std::hash<float>()(vertex.position.x) ^
				std::hash<float>()(vertex.position.y) ^
				std::hash<float>()(vertex.position.z);
		}
	};

	size_t operator()(const VertexData& vertex) const {
		return std::hash<float>()(vertex.position.x) ^
			std::hash<float>()(vertex.position.y) ^
			std::hash<float>()(vertex.position.z) ^
			std::hash<float>()(vertex.normal.x) ^
			std::hash<float>()(vertex.texcoord.x);
	}

	void Initialize(DirectXCommon* dxcommon);
	

	void UpdateVertexBuffer();

	void UpdateIndexBuffer();


	// コマンドリスト
	void GetCommandList();
	void GetCommandList(const D3D12_VERTEX_BUFFER_VIEW& vbv);


	// インデクスデータ生成
	void GenerateIndices2();


public:
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices; // 追加：インデックスデータ

	uint32_t meshIndex = 0;
private:
	DirectXCommon* dxCommon_ = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	uint32_t* indexData;

};

