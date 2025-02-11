#include "ParticleManager.h"
#include "DirectXGame/engine/base/TextureManager.h"
#include "imgui.h"


#include"DirectXGame/engine/MyGame/MyGame.h"

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ParticleManager;
	}
	return instance;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon)
{

	this->camera_ = Object3dCommon::GetInstance()->GetDefaltCamera();

	dxCommon_ = dxCommon;

	CreateGraphicsPipeline();
}

void ParticleManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void ParticleManager::DrawCommonSetting(RasterizerType rasteType, BlendType blendType)
{
	switch (blendType)
	{
	case ParticleManager::BlendType::MODE_ADD:
		if (rasteType == RasterizerType::MODE_SOLID_BACK) {
			dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState[0].Get());
		}
		else {
			dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState[1].Get());
		}
		break;
	case ParticleManager::BlendType::MODE_SUBTRACT:
		if (rasteType == RasterizerType::MODE_SOLID_BACK) {
			dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState[2].Get());
		}
		else {
			dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState[3].Get());
		}
		break;
	case ParticleManager::BlendType::MODE_MUlLIPLY:
		if (rasteType == RasterizerType::MODE_SOLID_BACK) {
			dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState[4].Get());
		}
		else {
			dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState[5].Get());
		}
		break;
	default:
		break;
	}




	//// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	
	//形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけば良い
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ParticleManager::Update()
{
	

	// カメラ設定
	Transform cameraTransform{
		{1.0f, 1.0f, 1.0f},
		{std::numbers::pi_v<float> / 3.0f, std::numbers::pi_v<float>, 0.0f},
		{0.0f, 23.0f, 10.0f}
	};

#ifdef _DEBUG
	ImGui::Begin("engine");


	if (ImGui::CollapsingHeader("Particle")) {



		// 全パーティクルグループに対する処理
		for (auto& pair : particleGroups) // 各パーティクルグループに対して
		{
			ParticleGroup& group = pair.second;
			group.instanceCount = 0; // 描画すべきインスタンスのカウント

			for (size_t i = 0; i < 24; i += 2) {
				group.line_[i]->Update();
			}

			if (ImGui::BeginTabBar("Particles"))
			{
				if (ImGui::BeginTabItem(group.name.c_str()))
				{
					ImGui::SeparatorText("Emitter");

					ImGui::Checkbox("usebillboard", &group.usebillboard);
					ImGui::Checkbox("isAlpha", &group.isAlpha);
					ImGui::Checkbox("isLine", &group.isLine);
					ImGui::Separator(); // 水平線を引く
					ImGui::DragFloat3("center", &group.emiter.worldtransform.translate_.x, 0.1f);
					ImGui::DragFloat3("renge.max", &group.emiter.renge.max.x, 0.1f);
					ImGui::DragFloat3("renge.min", &group.emiter.renge.min.x, 0.1f);
					ImGui::Separator(); // 水平線を引く
					ImGui::DragFloat3("rotate.max", &group.emiter.rotate.max.x, 0.1f);
					ImGui::DragFloat3("rotate.min", &group.emiter.rotate.min.x, 0.1f);
					ImGui::DragFloat3("size.max", &group.emiter.size.max.x, 0.1f);
					ImGui::DragFloat3("size.min", &group.emiter.size.min.x, 0.1f);
					ImGui::DragFloat3("velocity.max", &group.emiter.velocity.max.x, 0.1f);
					ImGui::DragFloat3("velocity.min", &group.emiter.velocity.min.x, 0.1f);
					ImGui::DragFloat("lifeTime.max", &group.emiter.lifeTime.max, 0.1f);
					ImGui::DragFloat("lifeTime.min", &group.emiter.lifeTime.min, 0.1f);

					ImGui::DragFloat("count", &group.emiter.count, 1.0f);

					ImGui::Separator(); // 水平線を引く
					ImGui::ColorEdit4("colorMax", &group.emiter.color.max.x);
					ImGui::ColorEdit4("colorMin", &group.emiter.color.min.x);




					LimitMaxMin();

					ImGui::EndTabItem();
				}

			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
#endif




	// 全パーティクルグループに対する処理
	for (auto& pair : particleGroups) // 各パーティクルグループに対して
	{
		ParticleGroup& group = pair.second;
		group.instanceCount = 0; // 描画すべきインスタンスのカウント

		Matrix4x4 projectionMatrix = camera_->GetProjectionMatrix();
		Matrix4x4 viewMatrix = camera_->GetViewMatrix();


		Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
		Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, camera_->GetWorldMatrix());
		//Matrix4x4 billboardMatrix = Multiply(Multiply(group.emiter.object.mat_,backToFrontMatrix), camera_->GetWorldMatrix());
		billboardMatrix.m[3][0] = 0.0f; // 平行移動成分は不要
		billboardMatrix.m[3][1] = 0.0f;
		billboardMatrix.m[3][2] = 0.0f;


		group.emiter.worldtransform.Update();

		//for (size_t i = 0; i < 24; i += 2) {
			//	group.line_[i]->Update();
		//}

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
					particleIterator->velocity = Add(particleIterator->velocity, Multiply(MyGame::GameTime(), acceleraionField.acceleration));
				}

				if (group.isGravity) { // 重力
					particleIterator->velocity.y -= kGravitationalAcceleration * MyGame::GameTime();
				};

				if (group.isLifeTimeScale_) { // スケール
					// アルファ値を計算
					float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);

					particleIterator->transform.scale = Lerp({}, particleIterator->strtTransform.scale, alpha);
				}

				if (group.isRotateVelocity) {// 回転
					particleIterator->transform.rotate += particleIterator->rotateVelocity;
				}
				// 移動処理 (速度を位置に加算)
				particleIterator->transform.translate += particleIterator->velocity * MyGame::GameTime();

				// 経過時間を加算
				particleIterator->currentTime += MyGame::GameTime();

				if (group.isBounce) {
					if (particleIterator->transform.translate.y < 0) {
						particleIterator->transform.translate.y = 0;
						particleIterator->velocity = Reflect(particleIterator->velocity, { 0,1,0 },0.85f);
					}
				}


				// ワールド行列を計算
				Matrix4x4 worldMatrix;
				if (group.usebillboard) {
					Matrix4x4 mat = group.emiter.worldtransform.worldMat_;

					worldMatrix = Multiply(Multiply(MakeScaleMatrix((*particleIterator).transform.scale), billboardMatrix), MakeTranslateMatrix((*particleIterator).transform.translate));

				}
				else {
					worldMatrix = MakeAffineMatrix(particleIterator->transform.scale, particleIterator->transform.rotate, particleIterator->transform.translate);

				}

				// ワールドビュー射影行列を合成
				Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

				// インスタンシング用データに情報を書き込み
				group.instanceData[group.instanceCount].World = worldMatrix;
				group.instanceData[group.instanceCount].WVP = worldViewProjectionMatrix;
				group.instanceData[group.instanceCount].color = particleIterator->color;

				if (group.isAlpha) {

					// アルファ値を計算
					float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);

					group.instanceData[group.instanceCount].color.w = alpha; //(std::max)((1.0f - alpha),0.0f);
					//group.instanceData[group.instanceCount].color.w = (std::max)((group.w),0.0f);
				}

				// インスタンス数をカウント
				++group.instanceCount;
			}

			++particleIterator;
		}
	}
	//}
}

void ParticleManager::LimitMaxMin()
{
	for (auto& pair : particleGroups) // 各パーティクルグループに対して
	{
		ParticleGroup& group = pair.second;

		//	範囲 
		group.emiter.renge.min.x = (std::min)(group.emiter.renge.min.x, group.emiter.renge.max.x);
		group.emiter.renge.max.x = (std::max)(group.emiter.renge.min.x, group.emiter.renge.max.x);
		group.emiter.renge.min.y = (std::min)(group.emiter.renge.min.y, group.emiter.renge.max.y);
		group.emiter.renge.max.y = (std::max)(group.emiter.renge.min.y, group.emiter.renge.max.y);
		group.emiter.renge.min.z = (std::min)(group.emiter.renge.min.z, group.emiter.renge.max.z);
		group.emiter.renge.max.z = (std::max)(group.emiter.renge.min.z, group.emiter.renge.max.z);

		// 回転
		group.emiter.rotate.min.x = (std::min)(group.emiter.rotate.min.x, group.emiter.rotate.max.x);
		group.emiter.rotate.max.x = (std::max)(group.emiter.rotate.min.x, group.emiter.rotate.max.x);
		group.emiter.rotate.min.y = (std::min)(group.emiter.rotate.min.y, group.emiter.rotate.max.y);
		group.emiter.rotate.max.y = (std::max)(group.emiter.rotate.min.y, group.emiter.rotate.max.y);
		group.emiter.rotate.min.z = (std::min)(group.emiter.rotate.min.z, group.emiter.rotate.max.z);
		group.emiter.rotate.max.z = (std::max)(group.emiter.rotate.min.z, group.emiter.rotate.max.z);

		// 
		group.emiter.velocity.min.x = (std::min)(group.emiter.velocity.min.x, group.emiter.velocity.max.x);
		group.emiter.velocity.max.x = (std::max)(group.emiter.velocity.min.x, group.emiter.velocity.max.x);
		group.emiter.velocity.min.y = (std::min)(group.emiter.velocity.min.y, group.emiter.velocity.max.y);
		group.emiter.velocity.max.y = (std::max)(group.emiter.velocity.min.y, group.emiter.velocity.max.y);
		group.emiter.velocity.min.z = (std::min)(group.emiter.velocity.min.z, group.emiter.velocity.max.z);
		group.emiter.velocity.max.z = (std::max)(group.emiter.velocity.min.z, group.emiter.velocity.max.z);


		//	サイズ 
		group.emiter.size.min.x = (std::min)(group.emiter.size.min.x, group.emiter.size.max.x);
		group.emiter.size.max.x = (std::max)(group.emiter.size.min.x, group.emiter.size.max.x);
		group.emiter.size.min.y = (std::min)(group.emiter.size.min.y, group.emiter.size.max.y);
		group.emiter.size.max.y = (std::max)(group.emiter.size.min.y, group.emiter.size.max.y);
		group.emiter.size.min.z = (std::min)(group.emiter.size.min.z, group.emiter.size.max.z);
		group.emiter.size.max.z = (std::max)(group.emiter.size.min.z, group.emiter.size.max.z);

		if (group.emiter.size.min.x < 0) {
			group.emiter.size.min.x = 0;
			group.emiter.size.max.x = 0;
		}
		if (group.emiter.size.min.y < 0) {
			group.emiter.size.min.y = 0;
			group.emiter.size.max.y = 0;
		}
		if (group.emiter.size.min.z < 0) {
			group.emiter.size.min.z = 0;
			group.emiter.size.max.z = 0;
		}


		//　色
		group.emiter.color.min.x = (std::min)(group.emiter.color.min.x, group.emiter.color.max.x);
		group.emiter.color.max.x = (std::max)(group.emiter.color.min.x, group.emiter.color.max.x);
		group.emiter.color.min.y = (std::min)(group.emiter.color.min.y, group.emiter.color.max.y);
		group.emiter.color.max.y = (std::max)(group.emiter.color.min.y, group.emiter.color.max.y);
		group.emiter.color.min.z = (std::min)(group.emiter.color.min.z, group.emiter.color.max.z);
		group.emiter.color.max.z = (std::max)(group.emiter.color.min.z, group.emiter.color.max.z);
		group.emiter.color.min.w = (std::min)(group.emiter.color.min.w, group.emiter.color.max.w);
		group.emiter.color.max.w = (std::max)(group.emiter.color.min.w, group.emiter.color.max.w);


		group.emiter.lifeTime.min = (std::min)(group.emiter.lifeTime.min, group.emiter.lifeTime.max);
		group.emiter.lifeTime.max = (std::max)(group.emiter.lifeTime.min, group.emiter.lifeTime.max);


		if (group.emiter.lifeTime.min < 0) {
			group.emiter.lifeTime.min = 0;
		}
		if (group.emiter.lifeTime.max < 0) {
			group.emiter.lifeTime.max = 0;
		}
	}


}

void ParticleManager::Draw()
{
	

	auto commandList = dxCommon_->GetCommandList();




	for (auto& pair : particleGroups) {
		ParticleGroup& group = pair.second;
		if (group.instanceCount == 0) {
			continue;
		}


		ParticleManager::GetInstance()->DrawCommonSetting(group.rasteType, group.blendType);


		group.material->GetCommandListTexture(2);

		commandList->SetGraphicsRootConstantBufferView(0, group.resource->GetGPUVirtualAddress());
		
		// インスタンシングデータのSRVのDescriptorTableを設定
		commandList->SetGraphicsRootDescriptorTable(1, group.instancingSrvHandleGPU);


		group.mesh->GetCommandList();
		
		// インスタンシング描画
		uint32_t instanceCount = (std::min)(group.instanceCount, kNumMaxInstance);
		commandList->DrawIndexedInstanced(static_cast<UINT>(group.mesh->indices.size()), instanceCount, 0, 0, 0);
	}
}

void ParticleManager::Emit(const std::string name, const std::string emitName, EmitType type)
{
	// パーティクルグループが登録済みであることを確認
	assert(particleGroups.contains(name) && "Error: Particle group with this name is not registered.");


	if (type == EmitType::kRandom) {
		RandParticle(name);
	}
	if (type == EmitType::kConstant) {
		RandParticle(name);
	}

}

void ParticleManager::Emit(const std::string name, const std::string emitName, const Constant& cons)
{
	// パーティクルグループが登録済みであることを確認
	assert(particleGroups.contains(name) && "Error: Particle group with this name is not registered.");

	if (emitName == "const") {
		ConstantParticle(name, cons);
	}
	if (emitName == "const2") {
		ConstantParticle2(name, cons);
	}

}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath, Model* model, bool flag, RasterizerType rasteType, BlendType blendType)
{
	// ランダムエンジンの初期化
	std::random_device seedGenerator;
	randomEngine_.seed(seedGenerator()); // randomEngine_ にシードを設定

	if (particleGroups.contains(name)) {
		return;
	}

	ParticleGroup& particleGroup = particleGroups[name];
	particleGroup.emiter.renge.max = Vector3{ 1.0f,1.0f,1.0f };
	particleGroup.emiter.renge.min = Vector3{ -1.0f,-1.0f,-1.0f };
	particleGroup.emiter.color.max = Vector4{ 1,1,1,1 };
	particleGroup.emiter.color.min = Vector4{ 0,0,0,0 };
	particleGroup.emiter.rotate.min = Vector3{ 0,0,0 };
	particleGroup.emiter.rotate.max = Vector3{ 0,0,0 };
	particleGroup.emiter.size.min = Vector3{ 1.0f,1.0f,1.0f };
	particleGroup.emiter.size.max = Vector3{ 1.0f,1.0f,1.0f };
	particleGroup.emiter.lifeTime.min = 1.0f;
	particleGroup.emiter.lifeTime.max = 3.0f;
	particleGroup.emiter.velocity.min = Vector3{ -1.0f,-1.0f,-1.0f };
	particleGroup.emiter.velocity.max = Vector3{ 1.0f,1.0f,1.0f };
	particleGroup.emiter.count = 10;
	particleGroup.emiter.rotateVelocity.min = Vector3{ 0,0,0 };
	particleGroup.emiter.rotateVelocity.max = Vector3{ 0,0,0 };

	particleGroup.emiter.worldtransform.Initialize();

	for (int i = 0; i < 24; i++) {
		auto line = std::make_unique<LineDraw>();
		line->Initialize();
		line->SetCamera(camera_);
		particleGroup.line_.push_back(std::move(line));
	}

	// 名前
	particleGroup.name = name;
	// モデル
	//particleGroup.model = model;
	particleGroup.mesh = model->modelData.mesh[0].get();
	particleGroup.mesh->UpdateVertexBuffer();
	particleGroup.mesh->UpdateIndexBuffer();

	// マテリアル
	particleGroup.material = std::make_unique<Material>();
	particleGroup.material->Initialize(dxCommon_);
	particleGroup.material->tex_.diffuseFilePath = textureFilePath;
	particleGroup.material->LoadTex();


	// GPUリソースの作成
	particleGroup.resource = dxCommon_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
	// マッピング
	particleGroup.resource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroup.instanceData));
	// 初期化
	for (uint32_t i = 0; i < kNumMaxInstance; ++i) {
		particleGroup.instanceData[i].World = MakeIdentity4x4();
		particleGroup.instanceData[i].WVP = MakeIdentity4x4();
		particleGroup.instanceData[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// SRVの設定
	// SRVインデックスの取得と設定
	particleGroup.srvIndex = SrvManager::GetInstance()->Allocate();
	particleGroup.instancingSrvHandleCPU = SrvManager::GetInstance()->GetCPUDescriptorHandle(particleGroup.srvIndex);
	particleGroup.instancingSrvHandleGPU = SrvManager::GetInstance()->GetGPUDescriptorHandle(particleGroup.srvIndex);
	SrvManager::GetInstance()->CreateSRVforStructuredBuffer(particleGroup.srvIndex, particleGroup.resource.Get(), kNumMaxInstance, sizeof(ParticleForGPU));

	// ブレンド
	particleGroup.blendType = blendType;

	// ラスタライザ
	particleGroup.rasteType = rasteType;

}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath, Primitive* primitive, bool flag, RasterizerType rasteType, BlendType blendType)
{
	// ランダムエンジンの初期化
	std::random_device seedGenerator;
	randomEngine_.seed(seedGenerator()); // randomEngine_ にシードを設定

	if (particleGroups.contains(name)) {
		return;
	}

	ParticleGroup& particleGroup = particleGroups[name];
	particleGroup.emiter.renge.max = Vector3{ 1.0f,1.0f,1.0f };
	particleGroup.emiter.renge.min = Vector3{ -1.0f,-1.0f,-1.0f };
	particleGroup.emiter.color.max = Vector4{ 1,1,1,1 };
	particleGroup.emiter.color.min = Vector4{ 0,0,0,0 };
	particleGroup.emiter.rotate.min = Vector3{ 0,0,0 };
	particleGroup.emiter.rotate.max = Vector3{ 0,0,0 };
	particleGroup.emiter.size.min = Vector3{ 1.0f,1.0f,1.0f };
	particleGroup.emiter.size.max = Vector3{ 1.0f,1.0f,1.0f };
	particleGroup.emiter.lifeTime.min = 1.0f;
	particleGroup.emiter.lifeTime.max = 3.0f;
	particleGroup.emiter.velocity.min = Vector3{ -1.0f,-1.0f,-1.0f };
	particleGroup.emiter.velocity.max = Vector3{ 1.0f,1.0f,1.0f };
	particleGroup.emiter.count = 10;
	particleGroup.emiter.rotateVelocity.min = Vector3{ 0,0,0 };
	particleGroup.emiter.rotateVelocity.max = Vector3{ 0,0,0 };

	particleGroup.emiter.worldtransform.Initialize();

	for (int i = 0; i < 24; i++) {
		auto line = std::make_unique<LineDraw>();
		line->Initialize();
		line->SetCamera(camera_);
		particleGroup.line_.push_back(std::move(line));
	}

	// 名前
	particleGroup.name = name;
	// モデル
	//particleGroup.model = model;
	particleGroup.mesh = primitive->GetMesh();
	particleGroup.mesh->UpdateVertexBuffer();
	particleGroup.mesh->UpdateIndexBuffer();

	// マテリアル
	particleGroup.material = std::make_unique<Material>();
	particleGroup.material->Initialize(dxCommon_);
	particleGroup.material->tex_.diffuseFilePath = textureFilePath;
	particleGroup.material->LoadTex();


	// GPUリソースの作成
	particleGroup.resource = dxCommon_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
	// マッピング
	particleGroup.resource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroup.instanceData));
	// 初期化
	for (uint32_t i = 0; i < kNumMaxInstance; ++i) {
		particleGroup.instanceData[i].World = MakeIdentity4x4();
		particleGroup.instanceData[i].WVP = MakeIdentity4x4();
		particleGroup.instanceData[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// SRVの設定
	// SRVインデックスの取得と設定
	particleGroup.srvIndex = SrvManager::GetInstance()->Allocate();
	particleGroup.instancingSrvHandleCPU = SrvManager::GetInstance()->GetCPUDescriptorHandle(particleGroup.srvIndex);
	particleGroup.instancingSrvHandleGPU = SrvManager::GetInstance()->GetGPUDescriptorHandle(particleGroup.srvIndex);
	SrvManager::GetInstance()->CreateSRVforStructuredBuffer(particleGroup.srvIndex, particleGroup.resource.Get(), kNumMaxInstance, sizeof(ParticleForGPU));



}














void ParticleManager::DrawAABB()
{

	for (auto& pair : particleGroups) {
		ParticleGroup& group = pair.second;

		if (group.isLine) {

			// 各頂点を計算
			Vector3 v0 = { group.emiter.renge.min.x, group.emiter.renge.min.y, group.emiter.renge.min.z }; // 左下前
			Vector3 v1 = { group.emiter.renge.max.x, group.emiter.renge.min.y, group.emiter.renge.min.z }; // 右下前
			Vector3 v2 = { group.emiter.renge.max.x, group.emiter.renge.max.y, group.emiter.renge.min.z }; // 右上前
			Vector3 v3 = { group.emiter.renge.min.x, group.emiter.renge.max.y, group.emiter.renge.min.z }; // 左上前

			Vector3 v4 = { group.emiter.renge.min.x, group.emiter.renge.min.y, group.emiter.renge.max.z }; // 左下後
			Vector3 v5 = { group.emiter.renge.max.x, group.emiter.renge.min.y, group.emiter.renge.max.z }; // 右下後
			Vector3 v6 = { group.emiter.renge.max.x, group.emiter.renge.max.y, group.emiter.renge.max.z }; // 右上後
			Vector3 v7 = { group.emiter.renge.min.x, group.emiter.renge.max.y, group.emiter.renge.max.z }; // 左上後

			// ライン描画用の配列
			Vector3 lines[24] = {
				// 前面
				v0, v1, v1, v2, v2, v3, v3, v0,
				// 背面
				v4, v5, v5, v6, v6, v7, v7, v4,
				// 左側面
				v0, v4, v3, v7,
				// 右側面
				v1, v5, v2, v6
			};
#ifdef _DEBUG
			// ライン描画
			Vector4 color = { 1, 1, 1, 1 }; // 白色
			for (size_t i = 0; i < 24; i += 2) {
				group.line_.emplace_back(std::make_unique<LineDraw>());
				group.line_[i]->Draw3D(Add(lines[i], group.emiter.worldtransform.worldMat_.GetWorldPosition()), Add(lines[i + 1], group.emiter.worldtransform.worldMat_.GetWorldPosition()), color);
			}
#endif // _DEBUG
		}
	}
}

void ParticleManager::SetPos(const std::string name, const Vector3& position)
{
	particleGroups[name].emiter.worldtransform.translate_ = position;
}


void ParticleManager::SetObject(const std::string name, WorldTransform& obj)
{
	particleGroups[name].emiter.worldtransform.parent_ = &obj;
}

void ParticleManager::CreateRootSignature()
{
	HRESULT hr;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 1; // 0から始まる
	descriptorRangeForInstancing[0].NumDescriptors = 1; // 数は1つ
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// Roosignature(ルートシグネチャ)作成
	//ShaderとResorceをどのように関連付けるかを示したオブジェクト

	//Roosignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionSignature{};

	descriptionSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。複数指定できるのではい
	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	//mat
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	// 
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	descriptionSignature.pParameters = rootParameters;
	descriptionSignature.NumParameters = _countof(rootParameters);


	///Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	descriptionSignature.pStaticSamplers = staticSamplers;
	descriptionSignature.NumStaticSamplers = _countof(staticSamplers);


	//シリアライズにしてバイナリする
	Microsoft::WRL::ComPtr < ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr < ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

		assert(false);
	}

	//バイナリを元に生成

	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	assert(SUCCEEDED(hr));
}

void ParticleManager::CreateGraphicsPipeline()
{
	CreateRootSignature();
	// RasterizerState(ラスタライザステート)の設定
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	BlendAdd();
	
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	GraphicsPipelineState(rootSignature, graphicsPipelineState[0], rasterizerDesc, blendDesc);
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	GraphicsPipelineState(rootSignature, graphicsPipelineState[1], rasterizerDesc, blendDesc);

	BlendSubtract();

	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	GraphicsPipelineState(rootSignature, graphicsPipelineState[2], rasterizerDesc, blendDesc);
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	GraphicsPipelineState(rootSignature, graphicsPipelineState[3], rasterizerDesc, blendDesc);


	BlendMuliply();

	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	GraphicsPipelineState(rootSignature, graphicsPipelineState[4], rasterizerDesc, blendDesc);
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	GraphicsPipelineState(rootSignature, graphicsPipelineState[5], rasterizerDesc, blendDesc);

	

}


void ParticleManager::GraphicsPipelineState(Microsoft::WRL::ComPtr<ID3D12RootSignature>& _rootSignature, Microsoft::WRL::ComPtr<ID3D12PipelineState>& _graphicsPipelineState, D3D12_RASTERIZER_DESC rasterizerDesc, D3D12_BLEND_DESC blendDesc)
{
	HRESULT hr;
	// InputLayout(インプットレイアウト)
	// VectorShaderへ渡す頂点データがどのようなものかを指定するオブジェクト


#pragma region InputLayout


	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


#pragma endregion //InputLayout(インプットレイアウト)

#pragma region CompileShader

	// Shaderをコンパイルする
	Microsoft::WRL::ComPtr < IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"resources/shaders/Particle/Particle.VS.hlsl",
		L"vs_6_0");

	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr < IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"resources/shaders/Particle/Particle.PS.hlsl",
		L"ps_6_0");

	assert(pixelShaderBlob != nullptr);

#pragma endregion//Shaderをコンパイルする


	// PSOを作成する

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = _rootSignature.Get();// RootSignature

	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;// InputLayout

	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() }; // VertexShader

	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() }; // PixelShader

	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState

	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;// RasterizerState

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトロポジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;



	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 透明オブジェクトの場合はデプス書き込みを無効化
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	////DepthStencilStateの設定を行う
	//D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//// Depthの機能を有効化する
	//depthStencilDesc.DepthEnable = true;
	//// 書き込みします
	//depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//// 比較関数はLessEqual。つまり、近ければ描画される
	//depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&_graphicsPipelineState));

	assert(SUCCEEDED(hr));
}

void ParticleManager::BlendAdd()
{
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
}

void ParticleManager::BlendSubtract()
{
	// 減算ブレンドの設定
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// RGB の減算ブレンド
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;      // ソースの影響度
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE; // デスティネーションの影響度
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_SUBTRACT;     // 減算

	// アルファブレンドの設定（通常の設定）
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
}

void ParticleManager::BlendMuliply()
{
	// 乗算ブレンドの設定
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	// 通常のカラー（RGB）乗算
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;      // 影響なし
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR; // 乗算（DestColor * SrcColor）
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	// アルファブレンドの設定（乗算に影響しない場合はそのまま）
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

}

void ParticleManager::RandParticle(const std::string name)
{


	ParticleGroup& particleGroup = particleGroups[name];

	//particleGroup.emiter.count;
	//particleGroup.emiter.center = position;

	// 出る位置
	std::uniform_real_distribution<float> distributionX(particleGroup.emiter.renge.min.x, particleGroup.emiter.renge.max.x);
	std::uniform_real_distribution<float> distributionY(particleGroup.emiter.renge.min.y, particleGroup.emiter.renge.max.y);
	std::uniform_real_distribution<float> distributionZ(particleGroup.emiter.renge.min.z, particleGroup.emiter.renge.max.z);

	// 方向
	std::uniform_real_distribution<float> distributionVeloX(particleGroup.emiter.velocity.min.x, particleGroup.emiter.velocity.max.x);
	std::uniform_real_distribution<float> distributionVeloY(particleGroup.emiter.velocity.min.y, particleGroup.emiter.velocity.max.y);
	std::uniform_real_distribution<float> distributionVeloZ(particleGroup.emiter.velocity.min.z, particleGroup.emiter.velocity.max.z);

	// 色RGBA
	std::uniform_real_distribution<float> distColorR(particleGroup.emiter.color.min.x, particleGroup.emiter.color.max.x);
	std::uniform_real_distribution<float> distColorG(particleGroup.emiter.color.min.y, particleGroup.emiter.color.max.y);
	std::uniform_real_distribution<float> distColorB(particleGroup.emiter.color.min.z, particleGroup.emiter.color.max.z);
	std::uniform_real_distribution<float> distColorA(particleGroup.emiter.color.min.w, particleGroup.emiter.color.max.w);

	// 時間
	std::uniform_real_distribution<float> distTime(particleGroup.emiter.lifeTime.min, particleGroup.emiter.lifeTime.max);

	// 回転
	std::uniform_real_distribution<float> distributionRotateX(particleGroup.emiter.rotate.min.x, particleGroup.emiter.rotate.max.x);
	std::uniform_real_distribution<float> distributionRotateY(particleGroup.emiter.rotate.min.y, particleGroup.emiter.rotate.max.y);
	std::uniform_real_distribution<float> distributionRotateZ(particleGroup.emiter.rotate.min.z, particleGroup.emiter.rotate.max.z);

	// スケール
	std::uniform_real_distribution<float> distributionSizeX(particleGroup.emiter.size.min.x, particleGroup.emiter.size.max.x);
	std::uniform_real_distribution<float> distributionSizeY(particleGroup.emiter.size.min.y, particleGroup.emiter.size.max.y);
	std::uniform_real_distribution<float> distributionSizeZ(particleGroup.emiter.size.min.z, particleGroup.emiter.size.max.z);

	// 回転速度
	std::uniform_real_distribution<float> distributionRotateVelocityX(particleGroup.emiter.rotateVelocity.min.x, particleGroup.emiter.rotateVelocity.max.x);
	std::uniform_real_distribution<float> distributionRotateVelocityY(particleGroup.emiter.rotateVelocity.min.y, particleGroup.emiter.rotateVelocity.max.y);
	std::uniform_real_distribution<float> distributionRotateVelocityZ(particleGroup.emiter.rotateVelocity.min.z, particleGroup.emiter.rotateVelocity.max.z);


	// パーティクル
	for (uint32_t t = 0; t < particleGroup.emiter.count; ++t) {
		Particle newParticle;
		// パーティクルの初期化 (必要に応じて詳細を設定)
		newParticle.transform.scale =
		{
			distributionSizeX(randomEngine_),
			distributionSizeY(randomEngine_),
			distributionSizeZ(randomEngine_)
		};
		newParticle.transform.rotate =
		{
			distributionRotateX(randomEngine_),
			distributionRotateY(randomEngine_),
			distributionRotateZ(randomEngine_)
		};
		newParticle.transform.translate =
		{
			particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition().x + distributionX(randomEngine_),
			particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition().y + distributionY(randomEngine_),
			particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition().z + distributionZ(randomEngine_)
		};
		newParticle.color =
		{
			distColorR(randomEngine_),
			distColorG(randomEngine_),
			distColorB(randomEngine_),
			1.0f
		};
		newParticle.rotateVelocity = {
			distributionRotateVelocityX(randomEngine_),
			distributionRotateVelocityY(randomEngine_),
			distributionRotateVelocityZ(randomEngine_)
		};

		newParticle.lifeTime = distTime(randomEngine_);
		newParticle.currentTime = 0;

		// 初期値
		newParticle.strtTransform = newParticle.transform;

		//速度
		newParticle.velocity =
		{
			distributionVeloX(randomEngine_),
			distributionVeloY(randomEngine_),
			distributionVeloZ(randomEngine_)
		};
		// パーティクルをグループに追加
		particleGroup.particle.push_back(newParticle);
	}
}


void ParticleManager::ConstantParticle(const std::string name, const Constant& cons)
{
	ParticleGroup& particleGroup = particleGroups[name];
	particleGroup.emiter.count = (float)cons.count;

	// 出る位置
	std::uniform_real_distribution<float> distributionX(cons.renge.min.x, cons.renge.max.x);
	std::uniform_real_distribution<float> distributionY(cons.renge.min.y, cons.renge.max.y);
	std::uniform_real_distribution<float> distributionZ(cons.renge.min.z, cons.renge.max.z);


	// 時間
	//std::uniform_real_distribution<float> distTime(particleGroup.emiter.lifeTime.min, particleGroup.emiter.lifeTime.max);


	particleGroup.emiter.worldtransform.translate_ = cons.centar;
	particleGroup.emiter.worldtransform.Update();
	// パーティクル
	for (uint32_t t = 0; t < particleGroup.emiter.count; ++t) {
		Particle newParticle;
		// パーティクルの初期化 (必要に応じて詳細を設定)
		newParticle.transform.scale = cons.size;
		newParticle.transform.rotate = cons.rotate;
		newParticle.transform.translate =
		{
			particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition().x + distributionX(randomEngine_),
			particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition().y + distributionY(randomEngine_),
			particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition().z + distributionZ(randomEngine_)
		};
		newParticle.color = cons.color;

		newParticle.lifeTime = cons.lifeTime;
		newParticle.currentTime = 0;


		//速度
		newParticle.velocity = cons.velocity;

		// パーティクルをグループに追加
		particleGroup.particle.push_back(newParticle);
	}
}

void ParticleManager::ConstantParticle2(const std::string name, const Constant& cons)
{
	ParticleGroup& particleGroup = particleGroups[name];
	particleGroup.emiter.count = (float)cons.count;

	// 出る位置
	std::uniform_real_distribution<float> distributionX(cons.renge.min.x, cons.renge.max.x);
	std::uniform_real_distribution<float> distributionY(cons.renge.min.y, cons.renge.max.y);
	std::uniform_real_distribution<float> distributionZ(cons.renge.min.z, cons.renge.max.z);

	// 方向
	std::uniform_real_distribution<float> distributionVeloX(particleGroup.emiter.velocity.min.x, particleGroup.emiter.velocity.max.x);
	std::uniform_real_distribution<float> distributionVeloY(particleGroup.emiter.velocity.min.y, particleGroup.emiter.velocity.max.y);
	std::uniform_real_distribution<float> distributionVeloZ(particleGroup.emiter.velocity.min.z, particleGroup.emiter.velocity.max.z);

	// 時間
	//std::uniform_real_distribution<float> distTime(particleGroup.emiter.lifeTime.min, particleGroup.emiter.lifeTime.max);


	particleGroup.emiter.worldtransform.translate_ = cons.centar;
	particleGroup.emiter.worldtransform.Update();
	// パーティクル
	for (uint32_t t = 0; t < particleGroup.emiter.count; ++t) {
		Particle newParticle;
		// パーティクルの初期化 (必要に応じて詳細を設定)
		newParticle.transform.scale = cons.size;
		newParticle.transform.rotate = cons.rotate;
		newParticle.transform.translate =
		{
			particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition().x + distributionX(randomEngine_),
			particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition().y + distributionY(randomEngine_),
			particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition().z + distributionZ(randomEngine_)
		};
		newParticle.color = cons.color;

		newParticle.lifeTime = cons.lifeTime;
		newParticle.currentTime = 0;


		//速度
		newParticle.velocity =
		{
			cons.velocity.x + distributionVeloX(randomEngine_),
			cons.velocity.y + distributionVeloY(randomEngine_),
			cons.velocity.z + distributionVeloZ(randomEngine_)
		};
		// パーティクルをグループに追加
		particleGroup.particle.push_back(newParticle);
	}
}


