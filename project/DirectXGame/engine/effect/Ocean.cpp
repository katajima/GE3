#include "Ocean.h"
#include "DirectXGame/engine/base/TextureManager.h"
#include "OceanManager.h"
#include "imgui.h"

void Ocean::Initialize(Vector2 renge)
{
	renge_.renge = renge;

	// 頂点データ
	modeldata.vertices.push_back({ .position = {renge_.renge.x, 0.0f, renge_.renge.y, 1.0f} ,.texcoord = {0.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左上
	modeldata.vertices.push_back({ .position = {-renge_.renge.x, 0.0f, renge_.renge.y, 1.0f} ,.texcoord = {1.0f, 0.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右上
	modeldata.vertices.push_back({ .position = {renge_.renge.x, 0.0f, -renge_.renge.y, 1.0f} ,.texcoord = {0.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 左下
	modeldata.vertices.push_back({ .position = {-renge_.renge.x, 0.0f, -renge_.renge.y, 1.0f} ,.texcoord = {1.0f, 1.0f}, .normal = {0.0f, 0.0f, 1.0f} }); // 右下

	// インデックスデータ（2つの三角形で矩形を形成）
	modeldata.indices = {
		0, 1, 2, // 左上、右上、左下
		1, 3, 2  // 右上、右下、左下
	};
	
	

	modeldata.material.textuerFilePath = "resources/Texture/Image.png";



	vertexResource = OceanManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modeldata.vertices.size());

	// リソースの先頭のアドレスを作成する
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modeldata.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modeldata.vertices.data(), sizeof(VertexData) * modeldata.vertices.size());

	//index用のあれやこれを作る
	indexResource = OceanManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	
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
	transformationMatrixResource = OceanManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(TransfomationMatrix));

	//書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transfomationMatrixData));

	//単位行列を書き込んでおく
	transfomationMatrixData->WVP = MakeIdentity4x4();
	transfomationMatrixData->World = MakeIdentity4x4();



	// マテリアル
	materialResource = OceanManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//今回は赤を書き込んで見る //白
	*materialData = Material({ 0.0f, 0.0f, 1.0f, 1.0f }, { false }); //RGBA
	materialData->uvTransform = MakeIdentity4x4();
	materialData->enableLighting = true;
	materialData->shininess = 20.0f;
	materialData->useLig = false;


	cameraResource = OceanManager::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(CameraGPU));
	//書き込むためのアドレスを取得
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));



	cameraData->worldPosition = Vector3{ 1.0f,1.0f,1.0f };
	cameraData->normal = { 0,0,0 };


	

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
	ImGui::Begin("Ocean");
	ImGui::DragFloat("amplitude", &waveData->amplitude, 0.1f);
	ImGui::DragFloat("frequency", &waveData->frequency, 0.1f);
	ImGui::DragFloat("speed", &waveData->speed, 0.1f);

	ImGui::End();


	waveData->time += 1.0f / 60.0f;

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


		// カメラデータの更新
		Vector3 cameraFront(viewMatrix.m[0][2], viewMatrix.m[1][2], viewMatrix.m[2][2]);
		cameraData->normal = Normalize(cameraFront); // 必要なら正規化
		cameraData->worldPosition = camera->transform_.translate;

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

void Ocean::Draw()
{
	OceanManager::GetInstance()->DrawCommonSetting();

	LightCommon::GetInstance()->DrawLight();

	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(7, waveResource->GetGPUVirtualAddress());
	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(8, transformationMatrixResource->GetGPUVirtualAddress());


	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	// Cameraのバインド
	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());

	// マテリアルのバインド
	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// テクスチャのバインド
	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modeldata.material.textuerFilePath));

	// 頂点バッファの設定
	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// インデックスバッファの設定
	//OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	//OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(modeldata.indices.size()), 1, 0, 0, 0);


	OceanManager::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modeldata.vertices.size()), 1, 0, 0);


}
