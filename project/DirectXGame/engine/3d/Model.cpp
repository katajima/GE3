#include"Model.h"
#include"ModelCommon.h"
#include"Object3d.h"
#include"DirectXGame/engine/base/TextureManager.h"
#include <iostream>
#include"DirectXGame/engine/base/Logger.h"
#include"DirectXGame/engine/base/StringUtility.h"
#include"format"
#include<vector>


std::string getLastPartOfPath(const std::string& path) {
	size_t pos = path.find_last_of("/\\"); if (pos == std::string::npos) {
		return path;
	} return path.substr(pos + 1);
}



#pragma region Initialize

void Model::Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename, const std::string& file, const Vector2 texScale)
{
	modelCommon_ = ModelCommon::GetInstance();;

	std::string dire = directorypath;

	if (file != "") {
		dire = directorypath + "/" + file;
	}


	modelData = LoadOdjFileAssimp(dire, filename);

	for (auto& material : modelData.material) {
		material->LoadTex();
	};

}

void Model::InitializeAnime(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename, const std::string& file)
{
	modelCommon_ = ModelCommon::GetInstance();;

	std::string dire = directorypath;

	if (file != "") {
		dire = directorypath + "/" + file;
	}


	modelData = LoadOdjFileAssimpAmime(dire, filename);


	animation = LoadAnimationFile(dire, filename);

	skeleton = CreateSkeleton(modelData.rootNode);

	skinCluster = CreateSkinCluster(skeleton, modelData);

	ApplyAnimation(skeleton, animation, animationTime);

	InitializeDrawLineSkeleton(skeleton.joints, line_);

	for (auto& material : modelData.material) {
		material->LoadTex();
	};

	if (filename == "Kick.gltf") {
		modelData.mesh[0]->indices;
	}
}

#pragma endregion // 初期化

#pragma region Draw

void Model::Draw()
{
	for (auto& mesh : modelData.mesh)
	{

		//for (auto& material : modelData.material) {

		modelData.material[mesh->meshIndex]->GetCommandListMaterial(0);

		modelData.material[mesh->meshIndex]->GetCommandListTexture(2, 7, 8);
		//}

		// マテリアルのバインド
		//modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());



		mesh->GetCommandList();

		// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
		//ModelCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(mesh->vertices.size()),1,0,0);
		ModelCommon::GetInstance()->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh->indices.size()), 1, 0, 0, 0);
	}
}

void Model::DrawSkinning()
{
	for (auto& mesh : modelData.mesh)
	{

		modelData.material[mesh->meshIndex]->GetCommandListMaterial(0);

		modelData.material[mesh->meshIndex]->GetCommandListTexture(2, 7, 8);


		modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(10, SrvManager::GetInstance()->GetGPUDescriptorHandle(modelData.skinningSrvindex));

		mesh->GetCommandList(skinCluster.influenceBufferView);

		// 描画コマンドの修正：インスタンス数の代わりにインデックス数を使用
		modelCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh->indices.size()), 1, 0, 0, 0);

	}
}

#pragma endregion // 描画

#pragma region Load

Model::ModelData Model::LoadMesh(const aiScene* _scene)
{
	return {};
}

Model::ModelData Model::LoadOdjFileAssimp(const std::string& directoryPath, const std::string& filename, const Vector2 texScale) {
	////必要な変数の宣言とファイルを開く
	ModelData modelData;//構築するModelData

	Assimp::Importer importer;
	std::string filePach = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePach.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); //メッシュがないのは対応しない
	if (!scene) {
		std::cerr << "Error: " << importer.GetErrorString() << std::endl;
		return modelData;
	}

	

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0)); //TexcoordがないMeshは今回は非対応

		std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();
		pMesh->meshIndex = meshIndex;

		Vector3 min = { 100 };
		Vector3 max = { -100 };
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみサポート
			//Mesh::triangle
			Triangle triangle;
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				Mesh::VertexData vertex;

				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x * texScale.x,texcoord.y * texScale.y };

				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対応
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				pMesh->vertices.push_back(vertex);

				triangle.vertices[element] = vertex.position.xyz();
				
				min = Min(min, vertex.position.xyz());
				max = Max(max, vertex.position.xyz());
			}
			// 三角ポリゴン
			pMesh->triangle.push_back(std::move(triangle));
		}

		pMesh->SetMin(min);
		pMesh->SetMax(max);
		
		// インデックスを生成
		pMesh->GenerateIndices2(); // thisは省略可能

		pMesh->Initialize(ModelCommon::GetInstance()->GetDxCommon());



		modelData.mesh.push_back(std::move(pMesh));
	}

	modelData.rootNode = ReadNode(scene->mRootNode);

	
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		std::unique_ptr<Material> pMaterial = std::make_unique<Material>();
		pMaterial->Initialize(ModelCommon::GetInstance()->GetDxCommon());

		for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
			aiMaterial* material = scene->mMaterials[materialIndex];
			aiString textureFilePath;

			if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
				aiString textureFilePaths;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePaths);
				std::cout << "Diffuse Texture File Path: " << textureFilePaths.C_Str() << std::endl;
				//modelData.material.textuerFilePath = directoryPath + "/" + textureFilePaths.C_Str();
				pMaterial->tex_.diffuseFilePath = directoryPath + "/" + textureFilePaths.C_Str();

			}
			if (material->GetTextureCount(aiTextureType_SPECULAR) != 0) {
				aiString textureFilePath;
				material->GetTexture(aiTextureType_SPECULAR, 0, &textureFilePath);
				std::cout << "Specular Texture File Path: " << textureFilePath.C_Str() << std::endl;
				//modelData.material.textuerSpeculerFilePath = directoryPath + "/" + textureFilePath.C_Str();
				pMaterial->tex_.speculerFilePath = directoryPath + "/" + textureFilePath.C_Str();
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
				//modelData.material.textuerNormalFilePath = directoryPath + "/" + textureFilePath.C_Str();

				pMaterial->tex_.normalFilePath = directoryPath + "/" + textureFilePath.C_Str();
			}
		}
		modelData.material.push_back(std::move(pMaterial));

	}

	//
	return modelData;
}

Model::ModelData Model::LoadOdjFileAssimpAmime(const std::string& directoryPath, const std::string& filename) {
	//必要な変数の宣言とファイルを開く
	ModelData modelData;//構築するModelData


	Assimp::Importer importer;
	std::string filePach = directoryPath + "/" + filename;


	//modelData.mesh = std::make_unique<Mesh>();

	const aiScene* scene = importer.ReadFile(filePach.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); //メッシュがないのは対応しない



	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0)); //TexcoordがないMeshは今回は非対応
		std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();

		pMesh->meshIndex = meshIndex;

		Vector3 min = { 100 };
		Vector3 max = { -100 };
		pMesh->vertices.resize(mesh->mNumVertices);
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

			pMesh->vertices[vertexIndex].position = { -position.x,position.y,position.z,1.0f };
			pMesh->vertices[vertexIndex].normal = { -normal.x,normal.y,normal.z };
			pMesh->vertices[vertexIndex].texcoord = { texcoord.x,texcoord.y };


			min = Min(min, pMesh->vertices[vertexIndex].position.xyz());
			max = Max(max, pMesh->vertices[vertexIndex].position.xyz());
		}
		pMesh->SetMin(min);
		pMesh->SetMax(max);

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみサポート
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];

				pMesh->indices.push_back(vertexIndex);
			}
		}


		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData.skinClusterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
			Matrix4x4 bindPoseMatrix = MakeAffineMatrix(Vector3{ scale.x, scale.y, scale.z }, Quaternion{ rotate.x,-rotate.y,-rotate.z,rotate.w }, Vector3{ -translate.x,translate.y,translate.z });
			jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId });
			}

		}

		pMesh->Initialize(ModelCommon::GetInstance()->GetDxCommon());

		modelData.mesh.push_back(std::move(pMesh));
	}


	modelData.skinningSrvindex = SrvManager::GetInstance()->Allocate();

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		std::unique_ptr<Material> pMaterial = std::make_unique<Material>();

		//modelData.material[] = std::make_unique<Material>();

		pMaterial->Initialize(ModelCommon::GetInstance()->GetDxCommon());
		for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
			aiMaterial* material = scene->mMaterials[materialIndex];
			aiString textureFilePath;

			if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
				aiString textureFilePaths;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePaths);
				std::cout << "Diffuse Texture File Path: " << textureFilePaths.C_Str() << std::endl;
				//modelData.material.textuerFilePath = directoryPath + "/" + textureFilePaths.C_Str();
				pMaterial->tex_.diffuseFilePath = directoryPath + "/" + textureFilePaths.C_Str();
			}
			if (material->GetTextureCount(aiTextureType_SPECULAR) != 0) {
				aiString textureFilePath;
				material->GetTexture(aiTextureType_SPECULAR, 0, &textureFilePath);
				std::cout << "Specular Texture File Path: " << textureFilePath.C_Str() << std::endl;
				//modelData.material.textuerSpeculerFilePath = directoryPath + "/" + textureFilePath.C_Str();
				pMaterial->tex_.speculerFilePath = directoryPath + "/" + textureFilePath.C_Str();
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
				//modelData.material.textuerNormalFilePath = directoryPath + "/" + textureFilePath.C_Str();

				pMaterial->tex_.normalFilePath = directoryPath + "/" + textureFilePath.C_Str();
			}
		}
		modelData.material.push_back(std::move(pMaterial));

	}

	modelData.rootNode = ReadNode(scene->mRootNode);

	return modelData;
}

Node Model::ReadNode(aiNode* node) {
	Node result;

	aiVector3D scale{}, translate{};
	aiQuaternion rotate{};
	node->mTransformation.Decompose(scale, rotate, translate); // assimpの行列からSRTを抽出する関数を利用

	result.transform.scale = { scale.x,scale.y,scale.z }; // Scaleはそのまま
	result.transform.rotate = { rotate.x,-rotate.y,-rotate.z,rotate.w }; // x軸を反転、さらに回転方向が逆なので軸を反転させる	
	result.transform.translate = { -translate.x,translate.y,translate.z }; // x軸を反転

	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);


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

#pragma endregion // 読み込み系


SkinCluster Model::CreateSkinCluster(const Skeleton& skeleton, const ModelData& modelData)
{
	SkinCluster skinCluster;

	// palette用のResourceを確保
	skinCluster.paletteResource = ModelCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	std::memset(mappedPalette, 0, sizeof(WellForGPU) * skeleton.joints.size());
	skinCluster.mappedPalette = { mappedPalette, skeleton.joints.size() }; // spanを使ってアクセスするようにする
	//skinCluster.paletteResource->Unmap(0, nullptr);

	skinCluster.paletteSrvHandle.first = SrvManager::GetInstance()->GetCPUDescriptorHandle(modelData.skinningSrvindex);
	skinCluster.paletteSrvHandle.second = SrvManager::GetInstance()->GetGPUDescriptorHandle(modelData.skinningSrvindex);

	// palette用のSrvを作成。StructuredBufferでアクセスできるようにする。
	D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
	paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSrvDesc.Buffer.FirstElement = 0;
	paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSrvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
	paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	ModelCommon::GetInstance()->GetDxCommon()->GetDevice()->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);



	// influence用のResourceを確保。頂点ごとにinfluence情報を追加できるようにする
	skinCluster.influenceResource = ModelCommon::GetInstance()->GetDxCommon()->CreateBufferResource(sizeof(VertexInfluence) * modelData.mesh[0]->vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.mesh[0]->vertices.size()); // 仮埋め。weightを0にしておく。
	skinCluster.mappedInfluence = { mappedInfluence, modelData.mesh[0]->vertices.size() };
	//skinCluster.influenceResource->Unmap(0, nullptr);

	// Influence用のVB作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.mesh[0]->vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	// InverseBindPoseMatrixを格納する場所を作成して、単位行列で埋める
	skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), MakeIdentity4x4);


	for (const auto& jointWeight : modelData.skinClusterData) {
		auto it = skeleton.jointMap.find(jointWeight.first);
		if (it == skeleton.jointMap.end()) {
			continue;
		}
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
			bool weightSet = false;
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				if (currentInfluence.weights[index] == 0.0f) {
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					weightSet = true;
					break;
				}
			}
			if (!weightSet) {
				// デバッグ用出力
				Logger::Log(StringUtility::ConvertString(std::format(L"Warning: Vertex %d has more influences than supported.\n", vertexWeight.vertexIndex)));
				//printf("Warning: Vertex %d has more influences than supported.\n", vertexWeight.vertexIndex);
			}
		}
	}



	return skinCluster;
}
