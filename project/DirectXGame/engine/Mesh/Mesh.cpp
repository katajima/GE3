#include "Mesh.h"

// 頂点を比較するためのオペレーター
bool operator==(const Mesh::VertexData& v1, const Mesh::VertexData& v2) {
	return v1.position == v2.position &&
		v1.normal == v2.normal &&
		v1.texcoord == v2.texcoord;
}

void Mesh::Initialize(DirectXCommon* dxcommon)
{
	dxCommon_ = dxcommon;

	vertexResource = dxCommon_->CreateBufferResource(sizeof(VertexData) * vertices.size());

	// リソースの先頭のアドレスを作成する
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, vertices.data(), sizeof(VertexData) * vertices.size());


	// インデクスリソース
	indexResource = dxCommon_->CreateBufferResource(sizeof(uint32_t) * indices.size());

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = UINT(sizeof(uint32_t) *indices.size());
	indexBufferView.Format = DXGI_FORMAT_R32_UINT; // インデックスフォーマット

	indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	std::memcpy(indexData, indices.data(), sizeof(uint32_t) * indices.size());

}

void Mesh::UpdateVertexBuffer()
{
	// 頂点データのサイズを計算
	size_t bufferSize = sizeof(VertexData) * vertices.size();

	// バッファを更新するためにマッピング
	void* pData;
	vertexResource->Map(0, nullptr, &pData);
	memcpy(pData, vertices.data(), bufferSize);
	vertexResource->Unmap(0, nullptr);
}

void Mesh::UpdateIndexBuffer()
{
	// インデックスデータのサイズを計算
	size_t bufferSize = sizeof(uint32_t) * indices.size();

	// バッファを更新するためにマッピング
	void* pData;
	indexResource->Map(0, nullptr, &pData);
	memcpy(pData, indices.data(), bufferSize);
	indexResource->Unmap(0, nullptr);
}

void Mesh::GetCommandList()
{
	// 頂点バッファの設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// インデックスバッファの設定
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView);
}

void Mesh::GetCommandList(const D3D12_VERTEX_BUFFER_VIEW& vbv)
{
	D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
		vertexBufferView,
		vbv
	};

	// 頂点バッファの設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 2, vbvs);
	// インデックスバッファの設定
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView);
}

void Mesh::GenerateIndices2()
{
	
	indices.clear();

	
	// ハッシュマップで頂点の重複を管理
	std::unordered_map<Mesh::VertexData, uint32_t, VertexHash> vertexMap;

	for (size_t i = 0; i < vertices.size(); ++i) {
		const Mesh::VertexData& vertex = vertices[i];

		// 既に同じ頂点が登録されているかチェック
		auto it = vertexMap.find(vertex);
		if (it != vertexMap.end()) {
			// 既存のインデックスを使用
			indices.push_back(it->second);
		}
		else {
			// 新しいインデックスを追加
			vertexMap[vertex] = static_cast<uint32_t>(i);
			indices.push_back(static_cast<uint32_t>(i));
		}
	}
}
