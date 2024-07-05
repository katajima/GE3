#include"Sprite.h"
#include"SpriteCommon.h"
#include <iostream>

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	// 引数で受け取ってメンバ変数にする
	this->spriteCommon_ = spriteCommon;

	vertexResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);
	if (!vertexResource) {
		throw std::runtime_error("Failed to create vertex resource.");
	}
	indexResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);
	if (!indexResource) {
		throw std::runtime_error("Failed to create index resource.");
	}
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

	HRESULT hr;
	hr = vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to map vertex resource.");
	}
	hr = indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to map index resource.");
	}



	materialResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	if (!materialResource) {
		throw std::runtime_error("Failed to create material resource.");
	}
	//書き込むためのアドレスを取得
	hr = materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to map material resource.");
	}

	//今回は赤を書き込んで見る //白
	*materialData = Material({ 1.0f, 1.0f, 1.0f, 1.0f }, { false }); //RGBA
	materialData->uvTransform = MakeIdentity4x4();
	materialData->enableLighting = false;

	
	transformationMatrixResource = spriteCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransfomationMatrix));
	if (!transformationMatrixResource) {
		throw std::runtime_error("Failed to create transformation matrix resource.");
	}
	//書き込むためのアドレスを取得
	hr = transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transfomationMatrixData));
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to map transformation matrix resource.");
	}
	//単位行列を書き込んでおく
	transfomationMatrixData->WVP = MakeIdentity4x4();
	transfomationMatrixData->World = MakeIdentity4x4();


	
}

void Sprite::Update()
{

	// 1枚目の三角形
	vertexData[0].position = { 0.0f,1.0f,0.0f,1.0f };//左下
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };

	vertexData[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };

	vertexData[2].position = { 1.0f,1.0f,0.0f,1.0f };//右下
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };
	//2枚目の三角形
	vertexData[3].position = { 1.0f,0.0f,0.0f,1.0f };//右上
	vertexData[3].texcoord = { 1.0f,0.0f };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };

	indexData[0] = 0;		
	indexData[1] = 1;		
	indexData[2] = 2;
	indexData[3] = 1;		
	indexData[4] = 3;		
	indexData[5] = 2;

	transform.scale = { size.x,size.y,1.0f };
	transform.rotate = { 0.0f,0.0f,rotation };
	transform.translate = { position.x,position.y,0.0f };
	//transform変数を作る
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	////透視射影行列
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f);
	Matrix4x4 worldMatrix = MakeAffineMatrixMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transfomationMatrixData->World = worldViewProjectionMatrix;
	transfomationMatrixData->WVP = worldViewProjectionMatrix;


}

void Sprite::Draw()
{
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	//spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);

	//vertexBufferViewSprite
	spriteCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView); //VBVを設定

	spriteCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	//トランスフォームMatrixResource
	spriteCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	spriteCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	
}
