#include"Model.h"
#include"ModelCommon.h"
#include"Object3d.h"
#include"DirectXGame/engine/base/TextureManager.h"

// 頂点を比較するためのオペレーター
bool operator==(const Model::VertexData& v1, const Model::VertexData& v2) {
	return v1.position == v2.position &&
		v1.normal == v2.normal &&
		v1.texcoord == v2.texcoord;
}

void Model::Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename)
{
	modelCommon_ = ModelCommon::GetInstance();;

	modelData = LoadOdjFile(directorypath, filename);


	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData.material.textuerFilePath);
	// 読み込んだテクスチャの番号を取得
	modelData.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textuerFilePath);

	vertexResource = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());

	// リソースの先頭のアドレスを作成する
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());


	// インデクスリソース
	indexResource = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * modelData.indices.size());

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = UINT(sizeof(uint32_t) * modelData.indices.size());
	indexBufferView.Format = DXGI_FORMAT_R32_UINT; // インデックスフォーマット

	uint32_t* indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	std::memcpy(indexData, modelData.indices.data(), sizeof(uint32_t) * modelData.indices.size());


	// マテリアル
	materialResource = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//今回は赤を書き込んで見る //白
	*materialData = Material({ 1.0f, 1.0f, 1.0f, 1.0f }, { false }); //RGBA
	materialData->uvTransform = MakeIdentity4x4();
	materialData->enableLighting = true;
	materialData->shininess = 40.0f; 
	materialData->useLig = false;

}

void Model::Draw()
{
	// マテリアルのバインド
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// テクスチャのバインド
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData.material.textuerFilePath));

	// 頂点バッファの設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// インデックスバッファの設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	modelCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(modelData.indices.size()), 1, 0, 0, 0);
	// 描画
	//modelCommon_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}

void Model::DrawInstance(size_t instanceCount) {
	// マテリアルのバインド
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// テクスチャのバインド
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData.material.textuerFilePath));

	// 頂点バッファの設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// インデックスバッファの設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// 描画コマンドの確認
	if (instanceCount > 0 && !modelData.indices.empty()) {
		// インデックス数とインスタンス数を正しく指定して描画
		modelCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(
			static_cast<UINT>(modelData.indices.size()), // インデックス数
			static_cast<UINT>(instanceCount),           // インスタンス数
			0,                                          // 最初のインデックス
			0,                                          // 最初の頂点
			0                                           // 最初の引数
		);
	}
}







MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{

	MaterialData materialData; // 構築するMaterialData
	std::string line; // ファイルから読んだ1行を格納するもの
	std::ifstream file(directoryPath + "/" + filename); //ファイルを開く
	assert(file.is_open()); //とりあえず開けなかったら

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textuerFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}

Model::ModelData Model::LoadOdjFile(const std::string& directoryPath, const std::string& filename)
{
	//必要な変数の宣言とファイルを開く
	ModelData modelData;//構築するModelData
	std::vector<Vector4> positions; //位置
	std::vector<Vector3> normals; //法線
	std::vector<Vector2> texcoords; //テクスチャ座標
	std::string line; //ファイルから読んだ1行を格納する

	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open()); //とりあえず開けなかったら止める

	//ファイル読み、ModelDataを構築
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; //先頭の識別子を読む

		//頂点情報を読み込む
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			position.x *= -1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			//texcoord.x = 1.0f - texcoord.x;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのindexは[位置/UV/法線]で格納されているので、分解してindexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// /区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				//要素へのindexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//VertexData vertex = { position,texcoord,normal };
				//modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}
			//頂点を逆順で登録することで、周り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			// MaterialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			//基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}

	}

	// インデックスを生成
	GenerateIndices2(modelData); // thisは省略可能

	return modelData;
};

void Model::GenerateIndices(ModelData& modelData) {
	modelData.indices.clear(); // インデックスをクリア

	// インデックス作成のために、すでに存在する頂点のリスト
	std::vector<uint32_t> uniqueIndices;

	// すべての頂点をチェック
	for (size_t i = 0; i < modelData.vertices.size(); ++i) {
		const VertexData& vertex = modelData.vertices[i];
		bool found = false; // 一致する頂点が見つかったかどうかのフラグ

		// すでに追加されている頂点と比較
		for (size_t j = 0; j < uniqueIndices.size(); ++j) {
			const VertexData& existingVertex = modelData.vertices[uniqueIndices[j]];

			// 頂点の位置、法線、テクスチャ座標が一致しているか確認
			if (vertex.position == existingVertex.position &&
				vertex.normal == existingVertex.normal &&
				vertex.texcoord == existingVertex.texcoord) {
				// 一致したら既存のインデックスを使用
				modelData.indices.push_back(uniqueIndices[j]);
				found = true;
				break;
			}
		}

		// 一致する頂点が見つからなければ、新しいインデックスを追加
		if (!found) {
			uniqueIndices.push_back(static_cast<uint32_t>(i));
			modelData.indices.push_back(static_cast<uint32_t>(i));
		}
	}
}

void Model::GenerateIndices2(ModelData& modelData) {
	modelData.indices.clear();

	// ハッシュマップで頂点の重複を管理
	std::unordered_map<VertexData, uint32_t, VertexHash> vertexMap;

	for (size_t i = 0; i < modelData.vertices.size(); ++i) {
		const VertexData& vertex = modelData.vertices[i];

		// 既に同じ頂点が登録されているかチェック
		auto it = vertexMap.find(vertex);
		if (it != vertexMap.end()) {
			// 既存のインデックスを使用
			modelData.indices.push_back(it->second);
		}
		else {
			// 新しいインデックスを追加
			vertexMap[vertex] = static_cast<uint32_t>(i);
			modelData.indices.push_back(static_cast<uint32_t>(i));
		}
	}
}

void Model::UpdateVertexBuffer() {
	// 頂点データのサイズを計算
	size_t bufferSize = sizeof(VertexData) * modelData.vertices.size();

	// バッファを更新するためにマッピング
	void* pData;
	vertexResource->Map(0, nullptr, &pData);
	memcpy(pData, modelData.vertices.data(), bufferSize);
	vertexResource->Unmap(0, nullptr);
}

void Model::UpdateIndexBuffer() {
	// インデックスデータのサイズを計算
	size_t bufferSize = sizeof(uint32_t) * modelData.indices.size();

	// バッファを更新するためにマッピング
	void* pData;
	indexResource->Map(0, nullptr, &pData);
	memcpy(pData, modelData.indices.data(), bufferSize);
	indexResource->Unmap(0, nullptr);
}

void Model::MoveVertices(const Vector3& offset) {
	for (const auto& index : modelData.indices) {
		modelData.vertices[index].position.x += offset.x;
		modelData.vertices[index].position.y += offset.y;
		modelData.vertices[index].position.z += offset.z;

	}
	UpdateVertexBuffer(); // バッファを更新
}