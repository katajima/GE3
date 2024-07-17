#pragma once
#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/math/MathFanctions.h"
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

	// サイズ
	const Vector2& GetSize() const { return size; }
	void SetSize(const Vector2& size) { this->size = size; }

	// 色
	const Vector4& GetColor() const { return materialData->color; }
	void SetColor(const Vector4& color) { materialData->color = color; }

	// アンカーポイント
	const Vector2& GetAnchorPoint() const { return anchorPoint; };
	void SetAnchorPoint(const Vector2& anchorPoint) { this->anchorPoint = anchorPoint; }

	// フリップX
	const bool& GetIsFlipX() const { return isFlipX_; };
	void SetIsFlipX(const bool& isFlipX) { this->isFlipX_ = isFlipX; }
	// フリップY
	const bool& GetIsFlipY() const { return isFlipY_; };
	void SetIsFlipY(const bool& isFlipY) { this->isFlipY_ = isFlipY; }

	//テクスチャ左上座標
	const Vector2& GetextureLeftTop() const { return textureLeftTop; };
	void SetTextureLeftTop(const Vector2& textureLeftTop) { this->textureLeftTop = textureLeftTop; }
	// テクスチャ切り出し座標
	const Vector2& GetTextureSize() const { return textureSize; };
	void SetTextureSize(const Vector2& textureSize) { this->textureSize = textureSize; }
private:

	// テクスチャサイズをイメージに合わせる
	void AdjusttextureSize();



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

	// アンカーポイント
	Vector2 anchorPoint = { 0.0f,0.0f };

	// 左右フリップ
	bool isFlipX_ = false;
	// 上下フリップ
	bool isFlipY_ = false;

	// テクスチャ左上座標
	Vector2 textureLeftTop = { 0.0f,0.0f };
	// テクスチャ切り出し座標
	Vector2 textureSize = { 100.0f,100.0f };

};

