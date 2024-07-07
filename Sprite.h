#pragma once
#include"Structs.h"
#include"MathFanctions.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cstdint>
#include<wrl.h>
#include<string>
using namespace Microsoft::WRL;
class SpriteCommon;

class Sprite
{
public:// メンバ関数
	// 初期化
	void Initialize(SpriteCommon* spriteCommon,std::string textureFilePath);
	// 更新
	void Update();
	// 描画
	void Draw();

	///位置
	// getter
	const Vector2& GetPosition() const { return position; }
	// setter
	void SetPosition(const Vector2& position) { this->position = position; }

	// 回転
	float GetRotation() const { return rotation; }
	void SetRotation(float rotation) { this->rotation = rotation; }

	//サイズ
	const Vector2& GetSize() const { return size; }
	void SetSize(const Vector2& size) { this->size = size; }

	//色
	const Vector4& GetColor() const { return materialData->color; }
	void SetColor(const Vector4& color) { materialData->color = color; }

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
	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Vector2 size = { 640.0f,360.0f };
	Vector2 position = { 0.0f,0.0f };
	float rotation = 0.0f;

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

	//テクスチャ番号
	uint32_t textureIndex = 0;
};

