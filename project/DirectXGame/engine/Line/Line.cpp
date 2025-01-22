#include "Line.h"
#include "LineCommon.h"

void LineDraw::Initialize()
{

	lineCommon_ = LineCommon::GetInstance();

	// 引数で受け取ってメンバ変数に記録する
	//this->lineCommon_ = lineCommon;
	//this->camera = lineCommon_->GetDefaltCamera();


	mesh_ = std::make_unique<Mesh>();
	mesh_->verticesline.push_back({ 0,0,0,0 });
	mesh_->verticesline.push_back({ 0,0,0,0 });
	mesh_->indices.push_back({ 0 });
	mesh_->indices.push_back({ 1 });
	mesh_->InitializeLine(LineCommon::GetInstance()->GetDxCommon());


	// マテリアル
	materialResource = lineCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//今回は赤を書き込んで見る //白
	*materialData = Material({ 1.0f, 0.0f, 0.0f, 1.0f }); //RGBA

	//トランスフォーム
	transformationMatrixResource = lineCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransfomationMatrix));

	//書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transfomationMatrixData));

	//単位行列を書き込んでおく
	transfomationMatrixData->WVP = MakeIdentity4x4();
	transfomationMatrixData->World = MakeIdentity4x4();


	//transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	mat_.Identity();
}

void LineDraw::Update()
{


	// ワールド行列を作成（線の位置、スケール、回転を表す）
	mat_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	// カメラがある場合はビュー行列とプロジェクション行列を適用
	if (camera) {
		const Matrix4x4& viewMatrix = camera->GetViewMatrix();          // カメラのビュー行列
		const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix(); // カメラのプロジェクション行列

		// WVP行列を作成: World * View * Projection
		Matrix4x4 worldViewMatrix = Multiply(mat_, viewMatrix);  // World * View
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldViewMatrix, projectionMatrix); // World * View * Projection

		// WVP行列とワールド行列をシェーダに送る
		transfomationMatrixData->World = mat_;
		transfomationMatrixData->WVP = worldViewProjectionMatrix;
	}
	else {
		// カメラがない場合はワールド行列のみ適用
		transfomationMatrixData->World = mat_;
		transfomationMatrixData->WVP = mat_;  // WVP行列はワールド行列と同じ
	}


}

void LineDraw::Draw3D(const Vector3& p1, const Vector3& p2, const Vector4& color)
{
	LineCommon::GetInstance()->DrawCommonSetting();

	// 頂点データの設定
	mesh_->UpdateLineVertexBuffer();
	mesh_->UpdateIndexBuffer();

	mesh_->verticesline[0].position = { p1.x, p1.y, p1.z, 1.0f };
	mesh_->verticesline[1].position = { p2.x, p2.y, p2.z, 1.0f };

	mesh_->indices[0] = 0;
	mesh_->indices[1] = 1;



	// マテリアルの色を設定
	materialData->color = color;


	lineCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	// マテリアルのバインド
	lineCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());


	mesh_->GetCommandList();

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	lineCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh_->indices.size()), 1, 0, 0, 0);

}

void LineDraw::DrawMeshLine(Mesh* mesh)
{
	LineCommon::GetInstance()->DrawCommonSetting();
	
	// 頂点データの設定
	mesh_->UpdateLineVertexBuffer();
	mesh_->UpdateIndexBuffer();

	mesh_->verticesline.clear();
	mesh_->indices.clear();

	for (int i = 0; i < mesh->vertices.size(); i++) {
		mesh_->verticesline.push_back({ mesh->vertices[i].position });
	}
	for (int i = 0; i < mesh->indices.size(); i++) {
		mesh_->indices.push_back({ mesh->indices[i] });
	}

	// 頂点データの設定
	mesh_->UpdateLineVertexBuffer();
	mesh_->UpdateIndexBuffer();

	lineCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	// マテリアルのバインド
	lineCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());


	mesh_->GetCommandList();

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	lineCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh_->indices.size()), 1, 0, 0, 0);
}

void LineDraw::DrawCapselLine(const Vector3& torans ,const Capsule& capsure)
{
	LineCommon::GetInstance()->DrawCommonSetting();

	// 頂点データの設定
	mesh_->UpdateLineVertexBuffer();
	mesh_->UpdateIndexBuffer();

	mesh_->verticesline.clear();
	mesh_->indices.clear();

	// ラインカプセル生成
	int numSegments = 16;
	for (int i = 0; i < numSegments; ++i)
	{
		float theta = i * 2.0f * 3.141592653589793f / numSegments;
		float x = capsure.radius * cos(theta);
		float z = capsure.radius * sin(theta);
		Vector3 ori = capsure.segment.origin + torans;
		Vector3 end = capsure.segment.end + torans;


		mesh_->verticesline.push_back({ ori.x + x, ori.y, ori.z + z });
		mesh_->verticesline.push_back({ end.x + x, end.y, end.z + z });

		int nextIndex = (i + 1) % numSegments;
		mesh_->indices.push_back(i * 2);
		mesh_->indices.push_back(nextIndex * 2);
		mesh_->indices.push_back(i * 2 + 1);
		mesh_->indices.push_back(nextIndex * 2 + 1);
	}


	// 頂点データの設定
	mesh_->UpdateLineVertexBuffer();
	mesh_->UpdateIndexBuffer();

	lineCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	// マテリアルのバインド
	lineCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());


	mesh_->GetCommandList();

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	lineCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh_->indices.size()), 1, 0, 0, 0);
}
