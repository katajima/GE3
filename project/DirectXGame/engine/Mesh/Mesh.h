#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"DirectXGame/engine/struct/Material.h"

#include "vector"
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
using namespace Microsoft::WRL;
#include<d3d12.h>
#include<dxgi1_6.h>

//#include "DirectXGame/engine/base/ImGuiManager.h"

class Mesh
{
public:	
	
	struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
	};
	struct LineVertexData {
	Vector4 position;
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

	// 通常用
	void Initialize(DirectXCommon* dxcommon);
	
	// ライン用
	void InitializeLine(DirectXCommon* dxcommon);

	void UpdateVertexBuffer();

	void UpdateLineVertexBuffer();

	void UpdateIndexBuffer();


	// コマンドリスト
	void GetCommandList();
	void GetCommandList(const D3D12_VERTEX_BUFFER_VIEW& vbv);


	// インデクスデータ生成
	void GenerateIndices2();

	Vector3 GetMin() const { return min_; }
	Vector3 GetMax() const { return max_; }

	Vector3 GetMiddle() const { return Vector3{ (max_.x + min_.x) / 2, (max_.y + min_.y) / 2, (max_.z + min_.z) / 2}; }

	void SetMin(const Vector3& min) { min_ = min; }
	void SetMax(const Vector3& max) { max_ = max; }

	static bool IsSegmentCollision(const Mesh& mesh, const Vector3& worldPos, const Segment& segment);



	static bool IsCapsuleCollision(const Mesh& mesh, const Vector3& worldPos, const Capsule& capsule);



	static void SetTriangleImGui(const Mesh& mesh,const std::string name, const Vector3& worldPos = {});

public:
	std::vector<VertexData> vertices;
	std::vector<LineVertexData> verticesline;
	std::vector<uint32_t> indices; // 追加：インデックスデータ
	std::vector<float> verticesTimer; // 追加：インデックスデータ
	std::vector<float> indicesTimer; // 追加：インデックスデータ

	std::vector<Triangle> triangle;



	float maxTime = 0.2f;


	uint32_t meshIndex = 0;


	MaterialData materialData_;
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

	Vector3 min_;
	Vector3 max_;

};

