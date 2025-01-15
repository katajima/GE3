#include "Primitive.h"

#include"PrimitiveCommon.h"

void Primitive::Initialize(ShapeType type,const std::string& tex, const Vector4 color)
{
	mesh = std::make_unique<Mesh>();


	//CreatePlane();
	CreateCube();
	mesh->Initialize(PrimitiveCommon::GetInstance()->GetDxCommon());
	
	material = std::make_unique<Material>();
	material->Initialize(PrimitiveCommon::GetInstance()->GetDxCommon());
	material->tex_.diffuseFilePath = tex;

	material->color = color;
	transfomation = std::make_unique<Transfomation>();

	transfomation->Initialize(PrimitiveCommon::GetInstance()->GetDxCommon());

	
	
	//transform変数を作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	mat_.Identity();
}

void Primitive::Update()
{
	material->GPUData();

	mat_ = MakeAffineMatrix(transform.scale, Vector3(transform.rotate), transform.translate);

	transfomation->Update(camera_,mat_);
}

void Primitive::Draw()
{
	PrimitiveCommon::GetInstance()->DrawCommonSetting();

	transfomation->GetCommandList(1);

	material->GetCommandListMaterial(0);

	material->GetCommandListTexture(2, 7, 8);


	mesh->GetCommandList();

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	//PrimitiveCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(mesh->vertices.size()), 1, 0, 0);

	PrimitiveCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh->indices.size()), 1, 0, 0, 0);
}

void Primitive::CreatePlane()
{
	
	mesh->vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f} ,.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f } });	// 左上
	mesh->vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f} ,.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f } });	// 右上
	mesh->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f} ,.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 左下
	//mesh->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f} ,.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 左下
	//mesh->vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f} ,.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f } });	// 右上
	mesh->vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f} ,.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f } });	// 右下

	mesh->indices.push_back(0);
	mesh->indices.push_back(1);
	mesh->indices.push_back(2);
	mesh->indices.push_back(2);
	mesh->indices.push_back(1);
	mesh->indices.push_back(3);
}

void Primitive::CreateCube()
{
	// 各面の法線
	Vector3 normals[] = {
		{ 0.0f,  0.0f,  1.0f}, // 前面
		{ 0.0f,  0.0f, -1.0f}, // 背面
		{ 0.0f,  1.0f,  0.0f}, // 上面
		{ 0.0f, -1.0f,  0.0f}, // 底面
		{ 1.0f,  0.0f,  0.0f}, // 右側面
		{-1.0f,  0.0f,  0.0f}  // 左側面
	};

	// 各面の頂点座標 (1つの面に4頂点)
	Vector4 positions[][4] = {
		// 前面
		{{ 1.0f,  1.0f,  1.0f, 1.0f}, {-1.0f,  1.0f,  1.0f, 1.0f}, { 1.0f, -1.0f,  1.0f, 1.0f}, {-1.0f, -1.0f,  1.0f, 1.0f}},
		// 背面
		{{ 1.0f,  1.0f, -1.0f, 1.0f}, { 1.0f, -1.0f, -1.0f, 1.0f}, {-1.0f,  1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, -1.0f, 1.0f}},
		// 上面
		{{ 1.0f,  1.0f, -1.0f, 1.0f}, {-1.0f,  1.0f, -1.0f, 1.0f}, { 1.0f,  1.0f,  1.0f, 1.0f}, {-1.0f,  1.0f,  1.0f, 1.0f}},
		// 底面
		{{ 1.0f, -1.0f,  1.0f, 1.0f}, {-1.0f, -1.0f,  1.0f, 1.0f}, { 1.0f, -1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, -1.0f, 1.0f}},
		// 右側面
		{{ 1.0f,  1.0f, -1.0f, 1.0f}, { 1.0f,  1.0f,  1.0f, 1.0f}, { 1.0f, -1.0f, -1.0f, 1.0f}, { 1.0f, -1.0f,  1.0f, 1.0f}},
		// 左側面
		{{-1.0f,  1.0f,  1.0f, 1.0f}, {-1.0f,  1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f,  1.0f, 1.0f}, {-1.0f, -1.0f, -1.0f, 1.0f}}
	};

	// 各面のUV座標
	Vector2 texcoords[] = {
		{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
	};

	// 各面のインデックスオフセット
	int vertexOffset = 0;

	for (int i = 0; i < 6; ++i) { // 6面
		// 頂点データを追加
		for (int j = 0; j < 4; ++j) { // 各面の4頂点
			mesh->vertices.push_back({
				positions[i][j],      // 座標
				texcoords[j],         // UV
				normals[i]            // 法線
				});
		}

		// インデックスデータを追加 (2つの三角形)
		mesh->indices.push_back(vertexOffset + 0);
		mesh->indices.push_back(vertexOffset + 1);
		mesh->indices.push_back(vertexOffset + 2);

		mesh->indices.push_back(vertexOffset + 2);
		mesh->indices.push_back(vertexOffset + 1);
		mesh->indices.push_back(vertexOffset + 3);

		vertexOffset += 4; // 次の面に移動
	}


}
