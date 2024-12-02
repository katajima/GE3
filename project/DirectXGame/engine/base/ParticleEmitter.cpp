#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(std::string name, Transform transform, uint32_t count, float frequency, float frequencyTime)
{
	name_ = name;
	count_ = count;
	frequency_ = frequency;
	frequencyTime_ = frequencyTime;
	transform_ = transform;
}

void ParticleEmitter::Update()
{
    // 発射間隔の更新
    frequencyTime_ += kDeltaTime;
    if (frequency_ <= frequencyTime_) {
        Emit();
        frequencyTime_ -= frequency_;
    }

    // 全パーティクルグループ内の全パーティクルを処理する
    for (auto& groupPair : ParticleManager::GetInstance()->GetParticleGroups()) {
        ParticleManager::ParticleGroup& particleGroup = groupPair.second;

        // 各パーティクルの更新
        for (auto& particle : particleGroup.particle) {
            // 位置の更新
            particle.transform.translate = Add(particle.transform.translate, Vector3{ 0.0f, 0.0f, 0.0f });
            // 寿命の増加
            particle.currentTime += kDeltaTime;
        }

        // 寿命が尽きたパーティクルを削除する処理
        particleGroup.particle.remove_if([](const ParticleManager::Particle& p)
            { return p.currentTime >= p.lifeTime; 
            });
    }
}

void ParticleEmitter::Emit()
{
    ParticleManager::GetInstance()->Emit(name_, transform_.translate, count_);
}
