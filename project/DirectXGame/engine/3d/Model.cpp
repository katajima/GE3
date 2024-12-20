#include"Model.h"
#include"ModelCommon.h"
#include"Object3d.h"
#include"DirectXGame/engine/base/TextureManager.h"
#include <iostream>

std::string getLastPartOfPath(const std::string& path) { 
	size_t pos = path.find_last_of("/\\"); if (pos == std::string::npos) { 
		return path; 
	} return path.substr(pos + 1); 
}
// 頂点を比較するためのオペレーター
bool operator==(const Model::VertexData& v1, const Model::VertexData& v2) {
	return v1.position == v2.position &&
		v1.normal == v2.normal &&
		v1.texcoord == v2.texcoord;
}

void Model::Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename, const std::string& file)
{
	modelCommon_ = ModelCommon::GetInstance();;

	std::string dire = directorypath;

	if (file != "") {
		dire = directorypath + "/" + file;
	}


	modelData = LoadOdjFileAssimp(dire, filename);


	animation = LoadAnimationFile(dire, filename);

	skeleton = CreateSkeleton(modelData.rootNode);

	ApplyAnimation(skeleton, animation, animationTime);




	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData.material.textuerFilePath);
	// 読み込んだテクスチャの番号を取得
	modelData.material.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textuerFilePath);

	if (modelData.material.textuerNormalFilePath == "") {
		useNormalMap = false;
	}
	else {
		useNormalMap = true;

	}

	if (useNormalMap) {
		TextureManager::GetInstance()->LoadTexture(modelData.material.textuerNormalFilePath);

		modelData.material.textureNormalIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textuerNormalFilePath);
	}

	
	if (modelData.material.textuerSpeculerFilePath == "") {
		useSpecularMap = false;
	}
	else {
		useSpecularMap = true;
	}
	if (useSpecularMap) {
		
		TextureManager::GetInstance()->LoadTexture(modelData.material.textuerSpeculerFilePath);

		modelData.material.textuerSpeculerIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textuerSpeculerFilePath);
	}


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
	materialData->shininess = 20.0f;
	materialData->useLig = false;

	if (useNormalMap) {
		materialData->useNormalMap = true;
	}
	if (useSpecularMap) {
		materialData->useSpeculerMap = true;
	}
}


void Model::Draw()
{
	// マテリアルのバインド
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// テクスチャのバインド
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData.material.textuerFilePath));
	if (useNormalMap) {
		modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(7, TextureManager::GetInstance()->GetSrvHandleGPU(modelData.material.textuerNormalFilePath));
		modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(9, TextureManager::GetInstance()->GetSrvHandleGPU(modelData.material.textuerNormalFilePath));

	}
	if (useSpecularMap) {
		modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(8, TextureManager::GetInstance()->GetSrvHandleGPU(modelData.material.textuerSpeculerFilePath));
	}


	// 頂点バッファの設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// インデックスバッファの設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
	modelCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(modelData.indices.size()), 1, 0, 0, 0);
	// 描画
	//modelCommon_->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}


Model::ModelData Model::LoadOdjFileAssimp(const std::string& directoryPath, const std::string& filename) {
	//必要な変数の宣言とファイルを開く
	ModelData modelData;//構築するModelData


	Assimp::Importer importer;
	std::string filePach = directoryPath + "/" + filename;

	

	const aiScene* scene = importer.ReadFile(filePach.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); //メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0)); //TexcoordがないMeshは今回は非対応
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみサポート
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };

				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対応
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}
		}

	}

//	modelData.isAssimp = true;


	// インデックスを生成
	GenerateIndices2(modelData); // thisは省略可能

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		aiString textureFilePath;

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePaths;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePaths);
			std::cout << "Diffuse Texture File Path: " << textureFilePaths.C_Str() << std::endl;
			modelData.material.textuerFilePath = directoryPath + "/" + textureFilePaths.C_Str();
		}
		if (material->GetTextureCount(aiTextureType_SPECULAR) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_SPECULAR, 0, &textureFilePath);
			std::cout << "Specular Texture File Path: " << textureFilePath.C_Str() << std::endl;
			modelData.material.textuerSpeculerFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
		if (material->GetTextureCount(aiTextureType_HEIGHT) != 0 || material->GetTextureCount(aiTextureType_NORMALS) != 0) {
			aiString textureFilePath;
			if (material->GetTextureCount(aiTextureType_HEIGHT) != 0) {
				material->GetTexture(aiTextureType_HEIGHT, 0, &textureFilePath);
			}
			else {
				material->GetTexture(aiTextureType_NORMALS, 0, &textureFilePath);
			}
			std::cout << "Normal/Height Texture File Path: " << textureFilePath.C_Str() << std::endl;
			modelData.material.textuerNormalFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}
//	modelData.isAssimp = true;

	modelData.rootNode = ReadNode(scene->mRootNode);

	return modelData;
}

Node Model::ReadNode(aiNode* node) {
	Node result;
	
	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate); // assimpの行列からSRTを抽出する関数を利用
	result.transform.scale = { scale.x,scale.y,scale.z }; // Scaleはそのまま
	result.transform.rotate = { rotate.x,-rotate.y,-rotate.z,rotate.w }; // x軸を反転、さらに回転方向が逆なので軸を反転させる
	result.transform.tarnslate = { -translate.x,translate.y,translate.z }; // x軸を反転
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.tarnslate);


	result.name = node->mName.C_Str(); // Node名を格納
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}

Animation Model::LoadAnimationFile(const std::string& directoryPath, const std::string& filename)
{
	Animation animation; // 今回作るアニメーション
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);

	if (scene->mAnimations == 0) {
		animation.flag = false;
		return animation;
	}
	else {
		animation.flag = true;
	}

	assert(scene->mAnimations != 0); // アニメーションがない
	aiAnimation* animationAssimp = scene->mAnimations[0]; // 最初のアニメーションだけ採用。もちろん作数対応するに越したことはない
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);// 時間の単位を秒に変換

	// assimpでは個々のNodeのAnimationをchannelと呼んでいるのでchannelを回してNodeAnimationの情報をとってくる
	// assimpでは個々のNodeのAnimationをchannelと呼んでいるのでchannelを回してNodeAnimationの情報をとってくる
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

		// Position Keysの処理
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
			keyframe.value = { -keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z }; // 右手->左手
			nodeAnimation.translate.keyframes.push_back(keyframe);
		}

		// Rotate Keysの処理
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuaternion keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
			keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w }; // 右手->左手
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}

		// Scale Keysの処理
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
			keyframe.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z }; // Scaleはそのまま
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}
	}

	// 解析完了
	return animation;
}

#pragma region MyRegion

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


#pragma endregion

