#include "Ocean.h"
#include "DirectXGame/engine/base/TextureManager.h"
#include "OceanManager.h"
#include "imgui.h"

void Ocean::Initialize(Vector2 renge)
{
	renge_.renge = renge;

	mesh_ = std::make_unique<Mesh>();
	// 頂点データ
	mesh_->vertices.push_back({ .position = {renge_.renge.x, 0.0f, renge_.renge.y, 1.0f} ,.texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左上
	mesh_->vertices.push_back({ .position = {-renge_.renge.x, 0.0f, renge_.renge.y, 1.0f} ,.texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右上
	mesh_->vertices.push_back({ .position = {renge_.renge.x, 0.0f, -renge_.renge.y, 1.0f} ,.texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左下
	mesh_->vertices.push_back({ .position = {-renge_.renge.x, 0.0f, -renge_.renge.y, 1.0f} ,.texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右下
	
	mesh_->indices.push_back(0);
	mesh_->indices.push_back(1);
	mesh_->indices.push_back(2);

	mesh_->indices.push_back(1);
	mesh_->indices.push_back(3);
	mesh_->indices.push_back(2);

	mesh_->Initialize(OceanManager::GetInstance()->GetDxCommon());





	//vertexResource = OceanManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modeldata.vertices.size());

	//// リソースの先頭のアドレスを作成する
	//vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modeldata.vertices.size());
	//vertexBufferView.StrideInBytes = sizeof(VertexData);

	//vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//std::memcpy(vertexData, modeldata.vertices.data(), sizeof(VertexData) * modeldata.vertices.size());

	////index用のあれやこれを作る
	//indexResource = OceanManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	//
	//// リソースの先頭のアドレスから使う
	//indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();

	//// 使用するリソースのサイズはインデック6つ分のサイズ
	//indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;

	//// インデックはuint32_tとする
	//indexBufferView.Format = DXGI_FORMAT_R32_UINT;


	//// インデックリソースにデータを書き込む
	//indexData = nullptr;
	//indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	//indexData[0] = 0;		indexData[1] = 1;		indexData[2] = 2;
	//indexData[3] = 1;		indexData[4] = 3;		indexData[5] = 2;



	
	transfomation = std::make_unique<Transfomation>();
	transfomation->Initialize(OceanManager::GetInstance()->GetDxCommon());

	
	material = std::make_unique<Material>();
	material->Initialize(OceanManager::GetInstance()->GetDxCommon());
	material->tex_.diffuseFilePath = "resources/Texture/Image.png";
	material->LoadTex();
	material->color = { 0,0,1,1.0f };


	waveResource = OceanManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(WaveParameters));
	waveResource->Map(0, nullptr, reinterpret_cast<void**>(&waveData));

	waveData->amplitude = 1.0f;
	waveData->frequency = 2.0f;
	waveData->speed = 1.0f;
	waveData->time = 0;

	//transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,10.0f,0.0f} };
}

void Ocean::Update()
{
#ifdef _DEBUG
	ImGui::Begin("Ocean");
	ImGui::DragFloat("amplitude", &waveData->amplitude, 0.1f);
	ImGui::DragFloat("frequency", &waveData->frequency, 0.1f);
	ImGui::DragFloat("speed", &waveData->speed, 0.1f);
	ImGui::End();
#endif // _DEBUG



	waveData->time += 1.0f / 60.0f;

	// ワールド行列の計算
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	
	transfomation->Update(camera,mat_);

	material->GPUData();
}

void Ocean::Draw()
{
	OceanManager::GetInstance()->DrawCommonSetting();

	LightCommon::GetInstance()->DrawLight();

	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(7, waveResource->GetGPUVirtualAddress());

	transfomation->GetCommandList(1);
	transfomation->GetCommandList(8);

	camera->GetCommandList(4);

	material->GetCommandListMaterial(0);

	material->GetCommandListTexture(2);

	
	// 頂点バッファの設定
	//OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// インデックスバッファの設定
	//OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	mesh_->GetCommandList();

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	//OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh_->indices.size()), 1, 0, 0, 0);


	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(mesh_->vertices.size()), 1, 0, 0);


}
