#pragma once
#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/struct/Material.h"

#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/Transfomation/Transfomation.h"
#include"DirectXGame/engine/Material/Material.h"

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
	enum class SpriteType {
		UvInterpolation_MODE_SOLID,
		NoUvInterpolation_MODE_SOLID,
		UvInterpolation_MODE_WIREFRAME,
		NoUvInterpolation_MODE_WIREFRAME,
	};


	// 初期化
	void Initialize(std::string textureFilePath, bool isTexLoad = true);
	// 更新
	void Update();
	// アニメーション
	void UpdateAmimetion(float time);

	// 描画
	void Draw(SpriteType type = {});

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
	void SetSize(const float& size) { this->size = this->size * size; }

	// 色
	const Color& GetColor() const { return material->color; }
	void SetColor(const Color& color) { material->color = color; }

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

	// アニメサイズ
	void SetAnimeSize(const Vector2& animeSize) { animeSize_ = animeSize; }
	// アニメ切り替え数
	void SetMaxAnimeNum(const Vector2& animeNum) { maxAnimeNum_ = animeNum; }
	// ピクセル補間
	void SetIsPixelInterpolation(bool is) { isPixelInterpolation_ = is; }


private:

	// テクスチャサイズをイメージに合わせる
	void AdjusttextureSize();

	void SpriteTypeDiscrimination(SpriteType type);

private:
	SpriteCommon* spriteCommon_ = nullptr;

	
	//頂点データ
	struct VertexData {

		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
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

	
	// トランスフォーム
	std::unique_ptr<Transfomation>transfomation = nullptr;
	// マテリアル
	std::unique_ptr<Material> material = nullptr;


	
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


	std::string textureFilePath_;

	//
	Vector2 maxAnimeNum_;
	Vector2 animeNum_;
	float animeTime_;
	Vector2 animeSize_;

	bool isPixelInterpolation_ = true;

};

