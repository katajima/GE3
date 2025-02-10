#include"Sprite.h"
#include"SpriteCommon.h"
#include <iostream>
#include"DirectXGame/engine/base/TextureManager.h"
#include"DirectXGame/engine/MyGame/MyGame.h"

void Sprite::Initialize(std::string textureFilePath, bool isTexLoad)
{

	textureFilePath_ = textureFilePath;
	// 引数で受け取ってメンバ変数にする
	this->spriteCommon_ = SpriteCommon::GetInstance();

	vertexResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);

	indexResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	//リソースの先頭のアドレスを作成する
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つの分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデック6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;


	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 1;
	indexData[4] = 3;
	indexData[5] = 2;

	// マテリアル
	material = std::make_unique<Material>();
	material->Initialize(SpriteCommon::GetInstance()->GetDxCommon());
	material->tex_.diffuseFilePath = textureFilePath;
	if (isTexLoad) {
		material->LoadTex();
	}
	//

	// トランスフォーム
	transfomation = std::make_unique<Transfomation>();
	transfomation->Initialize(SpriteCommon::GetInstance()->GetDxCommon());


	transform.scale = { size.x,size.y,1.0f };
	transform.rotate = { 0.0f,0.0f,rotation };
	transform.translate = { position.x,position.y,0.0f };

	float left = 0.0f - anchorPoint.x;
	float right = 1.0f - anchorPoint.x;
	float top = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;

	// 左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	// 上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	/// テクスチャ範囲指定-反映処理-
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMataData(textureFilePath_);
	float tex_left = textureLeftTop.x / metadata.width;
	float tex_right = (textureLeftTop.x + textureSize.x) / metadata.width;
	float tex_top = textureLeftTop.y / metadata.height;
	float tex_bottom = (textureLeftTop.y + textureSize.y) / metadata.height;

	AdjusttextureSize();
}

void Sprite::Update()
{
	transform.scale = { size.x,size.y,1.0f };
	transform.rotate = { 0.0f,0.0f,rotation };
	transform.translate = { position.x,position.y,0.0f };

	float left = 0.0f - anchorPoint.x;
	float right = 1.0f - anchorPoint.x;
	float top = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;

	// 左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	// 上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	/// テクスチャ範囲指定-反映処理-
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMataData(textureFilePath_);
	float tex_left = textureLeftTop.x / metadata.width;
	float tex_right = (textureLeftTop.x + textureSize.x) / metadata.width;
	float tex_top = textureLeftTop.y / metadata.height;
	float tex_bottom = (textureLeftTop.y + textureSize.y) / metadata.height;

	// 1枚目の三角形
	vertexData[0].position = { left,bottom,0.0f,1.0f };//左下
	vertexData[0].texcoord = { tex_left,tex_bottom };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };

	vertexData[1].position = { left,top,0.0f,1.0f };//左上
	vertexData[1].texcoord = { tex_left,tex_top };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };

	vertexData[2].position = { right,bottom,0.0f,1.0f };//右下
	vertexData[2].texcoord = { tex_right,tex_bottom };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };
	//2枚目の三角形
	vertexData[3].position = { right,top,0.0f,1.0f };//右上
	vertexData[3].texcoord = { tex_right,tex_top };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };


	//transform変数を作る
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	////透視射影行列
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::GetClientWidth()), float(WinApp::GetClientHeight()), 0.0f, 100.0f);
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	// トランスフォーム
	transfomation->UpdateSprite(worldViewProjectionMatrix);

	// マテリアル
	material->GPUData();
}

void Sprite::UpdateAmimetion(float time)
{
	animeTime_ += MyGame::GameTime();

	if (animeTime_ >= time) {
		textureLeftTop.x += animeSize_.x;
		animeNum_.x++;

		if (animeNum_.x >= maxAnimeNum_.x) {
			textureLeftTop.x = 0;
			animeNum_.x = 0;

			textureLeftTop.y += animeSize_.y;
			animeNum_.y++;

			if (animeNum_.y >= maxAnimeNum_.y) {
				textureLeftTop.y = 0;
				animeNum_.y = 0;
			}
		}
		animeTime_ = 0;
	}
	Update();
}

void Sprite::Draw(SpriteType type)
{
	SpriteTypeDiscrimination(type);
	
	material->GetCommandListMaterial(0);

	material->GetCommandListTexture(2, 2, 2);


	//vertexBufferViewSprite
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView); //VBVを設定

	spriteCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	//トランスフォームMatrixResource
	transfomation->GetCommandList(1);


	spriteCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);

}

void Sprite::AdjusttextureSize()
{
	// テクスチャメタデータを取得
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMataData(textureFilePath_);

	textureSize.x = static_cast<float>(metadata.width);
	textureSize.y = static_cast<float>(metadata.height);
	// 画像サイズをテクスチャサイズに合わせる
	size = textureSize;
}

void Sprite::SpriteTypeDiscrimination(SpriteType type)
{
	switch (type)
	{
	case Sprite::SpriteType::UvInterpolation_MODE_SOLID:
		SpriteCommon::GetInstance()->DrawCommonSetting(SpriteCommon::PSOType::UvInterpolation_MODE_SOLID);
		break;
	case Sprite::SpriteType::NoUvInterpolation_MODE_SOLID:
		SpriteCommon::GetInstance()->DrawCommonSetting(SpriteCommon::PSOType::NoUvInterpolation_MODE_SOLID);
		break;
	case Sprite::SpriteType::UvInterpolation_MODE_WIREFRAME:
		SpriteCommon::GetInstance()->DrawCommonSetting(SpriteCommon::PSOType::UvInterpolation_MODE_WIREFRAME);
		break;
	case Sprite::SpriteType::NoUvInterpolation_MODE_WIREFRAME:
		SpriteCommon::GetInstance()->DrawCommonSetting(SpriteCommon::PSOType::UvInterpolation_MODE_WIREFRAME);
		break;
	default:
		break;
	}

	
}
