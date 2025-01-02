#include "Thunder.h"
#include "DirectXGame/engine/base/TextureManager.h"
#include "ThunderManager.h"
#include "imgui.h"

void Thunder::Initialize(Vector2 renge)
{
	
	// 頂点データ
	//modeldata.vertices.push_back({ .position = {renge_.renge.x, 0.0f, renge_.renge.y, 1.0f} ,.texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左上
	//modeldata.vertices.push_back({ .position = {-renge_.renge.x, 0.0f, renge_.renge.y, 1.0f} ,.texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右上
	//modeldata.vertices.push_back({ .position = {renge_.renge.x, 0.0f, -renge_.renge.y, 1.0f} ,.texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左下
	//modeldata.vertices.push_back({ .position = {-renge_.renge.x, 0.0f, -renge_.renge.y, 1.0f} ,.texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右下

	// インデックスデータ（2つの三角形で矩形を形成）
	modeldata.indices = {
		0, 1, 2, // 左上、右上、左下
		1, 3, 2  // 右上、右下、左下
	};
	
	


	vertexResource = ThunderManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modeldata.vertices.size());

	// リソースの先頭のアドレスを作成する
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modeldata.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modeldata.vertices.data(), sizeof(VertexData) * modeldata.vertices.size());

	//index用のあれやこれを作る
	indexResource = ThunderManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	
	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデック6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;

	// インデックはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;


	// インデックリソースにデータを書き込む
	indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	indexData[0] = 0;		indexData[1] = 1;		indexData[2] = 2;
	indexData[3] = 1;		indexData[4] = 3;		indexData[5] = 2;



	

	//トランスフォーム
	transformationMatrixResource = ThunderManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(TransfomationMatrixThunder));

	//書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transfomationMatrixData));

	//単位行列を書き込んでおく
	transfomationMatrixData->WVP = MakeIdentity4x4();
	transfomationMatrixData->World = MakeIdentity4x4();
	transfomationMatrixData->time = 0;


	material = std::make_unique<Material>();
	material->Initialize(ThunderManager::GetInstance()->GetDxCommon());
	material->tex_.diffuseFilePath = "resources/Texture/Image.png";
	material->LoadTex();



	thunderResource = ThunderManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(ThunderParameters));
	//書き込むためのアドレスを取得
	thunderResource->Map(0, nullptr, reinterpret_cast<void**>(&thunderParameters));

	thunderParameters->alphaScale = 10.0f;
	thunderParameters->baseColorEnd = { 0.6f, 0.8f, 1.0f };
	thunderParameters->baseColorStart = { 0.0f, 0.2f, 0.8f };
	thunderParameters->falloffRate = 1.0f;

	

	//transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,10.0f,0.0f} };
}

void Thunder::Update()
{
	ImGui::Begin("Thunder");
	
	ImGui::End();

	transfomationMatrixData->time += 1.0f/60.0f;
	
	// ワールド行列の計算
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	
	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {

		const Matrix4x4& viewMatrix = camera->GetViewMatrix();
		const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix();


		// WVP計算
		worldViewProjectionMatrix = mat_; // ワールド変換
		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetViewMatrix()); // ビュー変換
		worldViewProjectionMatrix = Multiply(worldViewProjectionMatrix, camera->GetProjectionMatrix()); // 射影変換

		transfomationMatrixData->WVP = worldViewProjectionMatrix;
		transfomationMatrixData->World = mat_;

	}
	else {
		worldViewProjectionMatrix = mat_;
		transfomationMatrixData->WVP = worldViewProjectionMatrix;
		transfomationMatrixData->World = mat_;
	}

	transfomationMatrixData->worldInverseTranspose = Transpose(Inverse(mat_));
}

void Thunder::Draw()
{
	ThunderManager::GetInstance()->DrawCommonSetting();

	ThunderManager::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	// Cameraのバインド
	camera->GetCommandList(4);
	
	ThunderManager::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, thunderResource->GetGPUVirtualAddress());

	material->GetCommandListMaterial(0);
	material->GetCommandListTexture(2);

	
	// 頂点バッファの設定
	ThunderManager::GetInstance()->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// インデックスバッファの設定
	//OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	//OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(modeldata.indices.size()), 1, 0, 0, 0);


	ThunderManager::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modeldata.vertices.size()), 1, 0, 0);


}
