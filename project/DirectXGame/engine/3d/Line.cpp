#include "Line.h"
#include "LineCommon.h"

void Line::Initialize(LineCommon *lineCommon)
{
	// 引数で受け取ってメンバ変数に記録する
	this->lineCommon_ = lineCommon;
	this->camera = lineCommon_->GetDefaltCamera();





	vertexResource = lineCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData)*2);

	// リソースの先頭のアドレスを作成する
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData)*2);
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0] = { 0,0,0,0 };
	vertexData[1] = { 1,0,0,0 };

	//std::memcpy(vertexData, vertexData,sizeof(VertexData)*2);


// インデクスリソース
	indexResource = lineCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * 2); // 2つのインデックス

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = UINT(sizeof(uint32_t) * 2);
	indexBufferView.Format = DXGI_FORMAT_R32_UINT; // インデックスフォーマット

	// インデックスデータを設定
	//uint32_t indices[] = { 0, 1 }; // 始点と終点を示す
	uint32_t* indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	indexData[0] = 0;
	indexData[1] = 1;
	//std::memcpy(indexData, indices, sizeof(uint32_t) * 2); // 正しいインデックスをコピー


	// マテリアル
	materialResource = lineCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//今回は赤を書き込んで見る //白
	*materialData = Material({ 1.0f, 0.0f, 0.0f, 1.0f }); //RGBA
	





	//トランスフォーム
	transformationMatrixResource = lineCommon->GetDxCommon()->CreateBufferResource(sizeof(TransfomationMatrix));

	//書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transfomationMatrixData));

	//単位行列を書き込んでおく
	transfomationMatrixData->WVP = MakeIdentity4x4();
	transfomationMatrixData->World = MakeIdentity4x4();


	//transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Line::Update()
{
	// ワールド行列を作成（線の位置、スケール、回転を表す）
	Matrix4x4 worldMatrix = MakeAffineMatrixMatrix(transform.scale, transform.rotate, transform.translate);

	// カメラがある場合はビュー行列とプロジェクション行列を適用
	if (camera) {
		const Matrix4x4& viewMatrix = camera->GetViewMatrix();          // カメラのビュー行列
		const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix(); // カメラのプロジェクション行列

		// WVP行列を作成: World * View * Projection
		Matrix4x4 worldViewMatrix = Multiply(worldMatrix, viewMatrix);  // World * View
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldViewMatrix, projectionMatrix); // World * View * Projection

		// WVP行列とワールド行列をシェーダに送る
		transfomationMatrixData->World = worldMatrix;
		transfomationMatrixData->WVP = worldViewProjectionMatrix;
	}
	else {
		// カメラがない場合はワールド行列のみ適用
		transfomationMatrixData->World = worldMatrix;
		transfomationMatrixData->WVP = worldMatrix;  // WVP行列はワールド行列と同じ
	}
}

void Line::Draw()
{
	lineCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	// マテリアルのバインド
	lineCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// 頂点バッファの設定
	lineCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// インデックスバッファの設定
	lineCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	lineCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(2, 1, 0, 0, 0);

}
