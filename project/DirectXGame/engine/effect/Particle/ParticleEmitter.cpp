#include "ParticleEmitter.h"

#include "DirectXGame/engine/MyGame/MyGame.h"

// count: パーティクルの最大生成数を指定する値。frequency: パーティクルの発射間隔を秒単位で指定する値。frequencyTime: 現在の発射間隔の経過時間を追跡する値。
//ParticleEmitter::ParticleEmitter(std::string name, Transform transform, uint32_t count, float frequency, float frequencyTime)
//{
//	// name: パーティクルエミッターの名前を示す文字列。
//	// transform: パーティクルエミッターの位置、回転、スケールなどを表す構造体。	
//	// count: パーティクルの最大生成数を指定する値。
//	// frequency: パーティクルの発射間隔を秒単位で指定する値。
//	// frequencyTime: 現在の発射間隔の経過時間を追跡する値。
//
//
//
//	//name_ = name;
//	//count_ = count;
//	//frequency_ = frequency;
//	//frequencyTime_ = frequencyTime;
//	//transform_ = transform;
//}

void ParticleEmitter::Initialize(std::string emitName, std::string particleName)
{
	// name: パーティクルエミッターの名前を示す文字列。
	// transform: パーティクルエミッターの位置、回転、スケールなどを表す構造体。	
	// count: パーティクルの最大生成数を指定する値。
	// frequency: パーティクルの発射間隔を秒単位で指定する値。
	// frequencyTime: 現在の発射間隔の経過時間を追跡する値。

	emitName_ = emitName;
	particleName_ = particleName;
	transform_.Initialize();
	count_ = 1;
	frequency_ = 1.0f;
	frequencyTime_ = 0.0f;

	isLifeTimeScale_ = false;	// スケール
	isLifeTimeAlpha_ = false;	// 透明度
	isLifeTimeVelocity = false;// 速度

	isGravity = false; // 重力
	isWindow = false;  // 風
}

void ParticleEmitter::Update()
{
	

	// 発射間隔の更新処理。
	// frequencyTime_に経過時間(kDeltaTime)を加算し、frequency_を超えた場合にEmit()で発射を行う。
	// 発射後、余剰時間はfrequencyTime_から差し引き、次の発射間隔に対応します。
	/*frequencyTime_ += kDeltaTime;
	if (frequency_ <= frequencyTime_) {
		Emit();
		frequencyTime_ -= frequency_;
	}*/

	// 全パーティクルグループ内の全パーティクルを処理する
	for (auto& groupPair : ParticleManager::GetInstance()->GetParticleGroups()) {
		ParticleManager::ParticleGroup& particleGroup = groupPair.second;
		particleGroup.emiter.worldtransform.Update();



		frequencyTime_ += MyGame::GameTime();
		if (particleGroup.emiter.isEmit) {
			if (frequency_ <= frequencyTime_) {
				Emit();
				frequencyTime_ -= frequency_;
			}
		}
		// 各パーティクルの更新
		for (auto& particle : particleGroup.particle) {
			// 位置の更新
			particle.transform.translate = Add(particle.transform.translate, Vector3{ 0.0f, 0.0f, 0.0f });
			
			
			// 寿命の増加
			particle.currentTime += MyGame::GameTime();
		}

		// 寿命が尽きたパーティクルを削除する処理
		particleGroup.particle.remove_if([](const ParticleManager::Particle& p)
			{
				return p.currentTime >= p.lifeTime;
			});
	}

	transform_.Update();
}

void ParticleEmitter::Emit()
{
	ParticleManager::GetInstance()->Emit(particleName_,"rand", transform_.worldMat_.GetWorldPosition(), count_);

	
	
	//	ParticleManager::ParticleGroup& particleGroup = particleGroups[name];
//	// 全パーティクルグループ内の全パーティクルを処理する
//	for (auto& groupPair : ParticleManager::GetInstance()->GetParticleGroups()) {
//		ParticleManager::ParticleGroup& particleGroup = groupPair.second;
//
	//	ParticleManager::GetInstance()->Emit(particleGroup.name, "rand", particleGroup.emiter.worldtransform.worldMat_.GetWorldPosition(), count_);
//	}
}
