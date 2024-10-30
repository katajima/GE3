#include "ParticleManager.h"
#include "TextureManager.h"


ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ParticleManager;
	}
	return instance;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon/*, SrvManager* srvManager*/)
{
	dxCommon_ = dxCommon;
	srvManager_ = SrvManager::GetInstance();

	// ランダムエンジンの初期化
	std::random_device seedGenerator;
	randomEngine_.seed(seedGenerator()); // randomEngine_ にシードを設定
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	//CreateGraphicsPipeline();

	// モデルデータの設定
	ModelData modeldata;
	modeldata.vertices.push_back({ {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }); // 左上
	modeldata.vertices.push_back({ {-1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }); // 右上
	modeldata.vertices.push_back({ {1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }); // 左下
	modeldata.vertices.push_back({ {1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }); // 左下
	modeldata.vertices.push_back({ {-1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }); // 右上
	modeldata.vertices.push_back({ {-1.0f, -1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }); // 右下

	// 頂点リソースを作成
	//vertexResource = dxCommon_->CreateBufferResource(sizeof(VertexData) * modeldata.vertices.size());


	//// 頂点バッファビューを設定
	//vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//vertexBufferView.SizeInBytes = sizeof(VertexData) * static_cast<UINT>(modeldata.vertices.size());
	//vertexBufferView.StrideInBytes = sizeof(VertexData);

	//// 頂点データを書き込む
	//void* vertexData = nullptr;
	//vertexResource->Map(0, nullptr, &vertexData);
	//std::memcpy(vertexData, modeldata.vertices.data(), sizeof(VertexData) * modeldata.vertices.size());
	//vertexResource->Unmap(0, nullptr); // マッピングを解除


	//materialResource = dxCommon_->CreateBufferResource(sizeof(Material));

	////書き込むためのアドレスを取得
	//materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	////スプライトはLightingしないのでfalseにする
	//materialData->enableLighting = false;

	//materialData->uvTransform = MakeIdentity4x4();

	////今回は赤を書き込んで見る //白
	//*materialData = Material({ 1.0f, 1.0f, 1.0f, 1.0f }, { false }); //RGBA


	////平行光源用のリソースを作る
	//directionalLightResource = dxCommon_->CreateBufferResource(sizeof(DirectionalLight));
	//directionalLightData = nullptr;
	//directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	////今回は赤を書き込んで見る //白
	//*directionalLightData = DirectionalLight({ 1.0f,1.0f,1.0f,1.0f }, { 0.0f,-1.0f,0.0f }, 1.0f);



	//// 加速度場の設定
	//acceleraionField.acceleration = { 15.0f, 0.0f, 0.0f };
	//acceleraionField.area.min = { -1.0f, -1.0f, -1.0f };
	//acceleraionField.area.max = { 1.0f, 1.0f, 1.0f };
}

void ParticleManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void ParticleManager::Update()
{
	// カメラ設定
	Transform cameraTransform{
		{1.0f, 1.0f, 1.0f},
		{std::numbers::pi_v<float> / 3.0f, std::numbers::pi_v<float>, 0.0f},
		{0.0f, 23.0f, 10.0f}
	};

	// 透視射影行列
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);

	Matrix4x4 cameraMatrix = MakeAffineMatrixMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);

	// ビルボード用行列
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
	billboardMatrix.m[3][0] = 0.0f; // 平行移動成分は不要
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	
	// 全パーティクルグループに対する処理
	for (auto& pair : particleGroups) // 各パーティクルグループに対して
	{
		ParticleGroup& group = pair.second;
		group.instanceCount = 0; // 描画すべきインスタンスのカウント
		
		for (auto particleIterator = group.particle.begin(); particleIterator != group.particle.end(); )
		{
			// パーティクルの寿命をチェック
			if (particleIterator->lifeTime <= particleIterator->currentTime) {
				particleIterator = group.particle.erase(particleIterator);
				continue;
			}

			if (group.instanceCount < kNumMaxInstance) {
				// 場の影響を計算 (加速)
				if (upDataWind && IsCollision(acceleraionField.area, particleIterator->transform.translate)) {
					particleIterator->velocity = Add(particleIterator->velocity, Multiply(kDeltaTime, acceleraionField.acceleration));
				}

				// 移動処理 (速度を位置に加算)
				particleIterator->transform.translate = Add(particleIterator->transform.translate, Multiply(kDeltaTime, particleIterator->velocity));
				
				if (group.instanceCount == 0) {
					poa = particleIterator->transform.translate;
				}
				// 経過時間を加算
				particleIterator->currentTime += kDeltaTime;

				// アルファ値を計算
				float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);

				// ワールド行列を計算
				Matrix4x4 worldMatrix;
				if (usebillboard) {
					worldMatrix = Multiply(MakeScaleMatrix(particleIterator->transform.scale), billboardMatrix);
				}
				else {
					worldMatrix = MakeAffineMatrixMatrix(particleIterator->transform.scale, particleIterator->transform.rotate, particleIterator->transform.translate);
				}

				// ワールドビュー射影行列を合成
				Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

				// インスタンシング用データに情報を書き込み
				group.instanceData[group.instanceCount].World = worldMatrix;
				group.instanceData[group.instanceCount].WVP = worldViewProjectionMatrix;
				group.instanceData[group.instanceCount].color = particleIterator->color;
				group.instanceData[group.instanceCount].color.w = alpha;



				// インスタンス数をカウント
				++group.instanceCount;
			}

			++particleIterator;
		}
	}
}

void ParticleManager::Draw()
{
	//auto commandList = dxCommon_->GetCommandList();

	//commandList->SetGraphicsRootSignature(rootSignature.Get());

	//commandList->SetPipelineState(graphicsPipelineState.Get()); // PSOを設定

	//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	//commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	////commandList->
	//for (auto& pair : particleGroups) {
	//	ParticleGroup& group = pair.second;

	//	//commandList->SetGraphicsRootConstantBufferView(0, group.resource->GetGPUVirtualAddress());


	//	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	//	// テクスチャのSRVのDescriptorTableを設定
	//	auto textureHandle = TextureManager::GetInstance()->GetSrvHandleGPU(group.materialData.textuerFilePath);
	//	commandList->SetGraphicsRootDescriptorTable(2, textureHandle);

	//	// インスタンシングデータのSRVのDescriptorTableを設定
	//	auto srvHandle = srvManager_->GetGPUDescriptorHandle(group.srvIndex);
	//	commandList->SetGraphicsRootDescriptorTable(1, srvHandle);

	//	// インスタンシングの描画コール
	//	commandList->DrawInstanced(6 , kNumMaxInstance, 0, 0); // ここで6は1インスタンスあたりの頂点数
	//}
}

void ParticleManager::Emit(const std::string name, const Vector3& position, uint32_t count)
{
	// パーティクルグループが登録済みであることを確認
	assert(particleGroups.contains(name) && "Error: Particle group with this name is not registered.");

	ParticleGroup& particleGroup = particleGroups[name];

	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	for (uint32_t t = 0; t < count; ++t) {
		Particle newParticle;
		// パーティクルの初期化 (必要に応じて詳細を設定)
		newParticle.transform.scale = { 1.0f,1.0f,1.0f };
		newParticle.transform.rotate = { 0.0f,0.0f,0.0f };
		newParticle.transform.translate = { distribution(randomEngine_),distribution(randomEngine_),distribution(randomEngine_) };
		newParticle.color = { distColor(randomEngine_),distColor(randomEngine_),distColor(randomEngine_),1.0f };
		newParticle.lifeTime = distTime(randomEngine_);
		newParticle.currentTime = 0;


		Vector3 randomTranslate{ distribution(randomEngine_),distribution(randomEngine_),distribution(randomEngine_) };
		newParticle.transform.translate = Add(position, randomTranslate);

		//速度
		newParticle.velocity = { distribution(randomEngine_),distribution(randomEngine_),distribution(randomEngine_) };

		// パーティクルをグループに追加
		particleGroup.particle.push_back(newParticle);
	}
}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath)
{
	if (particleGroups.contains(name)) {
		return;
	}

	ParticleGroup& particleGroup = particleGroups[name];
	particleGroup.materialData.textuerFilePath = textureFilePath;

	TextureManager::GetInstance()->LoadTexture(textureFilePath);

	particleGroup.srvIndex = srvManager_->Allocate();
	particleGroup.materialData.textureIndex = particleGroup.srvIndex;

	particleGroup.resource = dxCommon_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);

	particleGroup.instanceData = nullptr;
	HRESULT hr = particleGroup.resource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroup.instanceData));
	for (uint32_t i = 0; i < kNumMaxInstance; ++i) {
		//単位行列を書き込んでおく
		particleGroup.instanceData[i].World = MakeIdentity4x4();
		particleGroup.instanceData[i].WVP = MakeIdentity4x4();
		particleGroup.instanceData[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	if (FAILED(hr)) {
		return;
	}

	//if (particleGroup.instanceData) {
	//	std::memset(particleGroup.instanceData, 0, sizeof(ParticleForGPU) * kNumMaxInstance);
	//}

	//D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	//instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	//instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	//instancingSrvDesc.Buffer.FirstElement = 0;
	//instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	//instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
	//instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	//D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU = srvManager_->GetCPUDescriptorHandle(particleGroup.srvIndex);
	//D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU = srvManager_->GetGPUDescriptorHandle(particleGroup.srvIndex);
	//dxCommon_->GetDevice()->CreateShaderResourceView(particleGroup.resource.Get(), &instancingSrvDesc, instancingSrvHandleCPU);
}

//ParticleManager::Particle ParticleManager::MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate)
//{
//	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
//	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
//	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
//	Particle particle;
//	particle.transform.scale = { 1.0f,1.0f,1.0f };
//	particle.transform.rotate = { 0.0f,0.0f,0.0f };
//	particle.transform.translate = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
//	particle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
//	particle.lifeTime = distTime(randomEngine);
//	particle.currentTime = 0;
//
//
//	Vector3 randomTranslate{ distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
//	particle.transform.translate = Add(translate, randomTranslate);
//
//	//速度
//	particle.velocity = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
//
//	return particle;
//}

//void ParticleManager::CreateRootSignature()
//{
//	HRESULT hr;
//
//	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
//	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
//	descriptorRange[0].NumDescriptors = 1; // 数は1つ
//	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
//	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
//	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
//	descriptorRangeForInstancing[0].BaseShaderRegister = 0; // 0から始まる
//	descriptorRangeForInstancing[0].NumDescriptors = 1; // 数は1つ
//	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
//	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
//
//	// Roosignature(ルートシグネチャ)作成
//	//ShaderとResorceをどのように関連付けるかを示したオブジェクト
//
//	//Roosignature作成
//	D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};
//
//	descriptionSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//
//	// RootParameter作成。複数指定できるのではい
//	D3D12_ROOT_PARAMETER rootParameters[4] = {};
//
//	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
//	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//	rootParameters[0].Descriptor.ShaderRegister = 0;
//
//	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
//	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
//	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
//	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);
//
//	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
//	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
//	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
//
//	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
//	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//	rootParameters[3].Descriptor.ShaderRegister = 1;
//
//	descriptionSignature.pParameters = rootParameters;
//	descriptionSignature.NumParameters = _countof(rootParameters);
//
//
//	///Samplerの設定
//	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
//	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
//	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲外をリピート
//	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
//	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
//	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
//	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
//	descriptionSignature.pStaticSamplers = staticSamplers;
//	descriptionSignature.NumStaticSamplers = _countof(staticSamplers);
//
//
//	//シリアライズにしてバイナリする
//	Microsoft::WRL::ComPtr < ID3DBlob> signatureBlob = nullptr;
//	Microsoft::WRL::ComPtr < ID3DBlob> errorBlob = nullptr;
//
//	hr = D3D12SerializeRootSignature(&descriptionSignature,
//		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
//
//	if (FAILED(hr)) {
//		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
//
//		assert(false);
//	}
//
//	//バイナリを元に生成
//
//	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
//		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
//
//	assert(SUCCEEDED(hr));
//}
//
//void ParticleManager::CreateGraphicsPipeline()
//{
//	HRESULT hr;
//	CreateRootSignature();
//
//	// InputLayout(インプットレイアウト)
//// VectorShaderへ渡す頂点データがどのようなものかを指定するオブジェクト
//
//
//#pragma region InputLayout
//
//
//	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
//	inputElementDescs[0].SemanticName = "POSITION";
//	inputElementDescs[0].SemanticIndex = 0;
//	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputElementDescs[1].SemanticName = "TEXCOORD";
//	inputElementDescs[1].SemanticIndex = 0;
//	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
//	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputElementDescs[2].SemanticName = "NORMAL";
//	inputElementDescs[2].SemanticIndex = 0;
//	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
//	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//
//	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
//	inputLayoutDesc.pInputElementDescs = inputElementDescs;
//	inputLayoutDesc.NumElements = _countof(inputElementDescs);
//
//
//#pragma endregion //InputLayout(インプットレイアウト)
//
//
//#pragma region BlendState
//
//
//	D3D12_BLEND_DESC blendDesc{};
//	//すべての色要素を書き込む
//	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
//	blendDesc.RenderTarget[0].BlendEnable = TRUE;
//	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
//	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
//	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
//	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
//	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
//	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
//
//#pragma endregion //BlendState(ブレンドステート)
//
//
//#pragma region RasterizerState
//
//	// RasterizerState(ラスタライザステート)の設定
//	// 三角形の内部をピクセルに分解して、PixelShaderを起動することで、この処理の設定
//
//	D3D12_RASTERIZER_DESC rasterizerDesc{};
//
//	//裏面(時計回り)を表示しない
//	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
//
//	//三角形の中を塗りつぶす
//	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
//
//#pragma endregion //(ラスタライザステート)
//
//
//#pragma region CompileShader
//
//	// Shaderをコンパイルする
//	Microsoft::WRL::ComPtr < IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"resources/shaders/Particle.VS.hlsl",
//		L"vs_6_0");
//
//	assert(vertexShaderBlob != nullptr);
//
//	Microsoft::WRL::ComPtr < IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"resources/shaders/Particle.PS.hlsl",
//		L"ps_6_0");
//
//	assert(pixelShaderBlob != nullptr);
//
//#pragma endregion//Shaderをコンパイルする
//
//
//	// PSOを作成する
//
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
//
//	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();// RootSignature
//
//	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;// InputLayout
//
//	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
//	vertexShaderBlob->GetBufferSize() }; // VertexShader
//
//	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
//	pixelShaderBlob->GetBufferSize() }; // PixelShader
//
//	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState
//
//	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;// RasterizerState
//
//	//書き込むRTVの情報
//	graphicsPipelineStateDesc.NumRenderTargets = 1;
//	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//
//	//利用するトロポジ(形状)のタイプ。三角形
//	graphicsPipelineStateDesc.PrimitiveTopologyType =
//		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//
//	//どのように画面に色を打ち込むかの設定(気にしなくて良い)
//	graphicsPipelineStateDesc.SampleDesc.Count = 1;
//	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
//
//
//
//	//DepthStencilStateの設定を行う
//	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
//	// Depthの機能を有効化する
//	depthStencilDesc.DepthEnable = true;
//	// 書き込みします
//	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
//	// 比較関数はLessEqual。つまり、近ければ描画される
//	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
//
//
//	// DepthStencilの設定
//	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
//	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
//
//
//	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
//		IID_PPV_ARGS(&graphicsPipelineState));
//
//	assert(SUCCEEDED(hr));
//
//
//}
