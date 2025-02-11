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

void ParticleEmitter::Initialize(std::string emitName, std::string particleName, ParticleManager::EmitType type)
{
	// name: パーティクルエミッターの名前を示す文字列。
	// transform: パーティクルエミッターの位置、回転、スケールなどを表す構造体。	
	// count: パーティクルの最大生成数を指定する値。
	// frequency: パーティクルの発射間隔を秒単位で指定する値。
	// frequencyTime: 現在の発射間隔の経過時間を追跡する値。

	emitType_ = type;

	emitName_ = emitName;
	particleName_ = particleName;
	transform_.Initialize();
	frequency_ = 1.0f;
	frequencyTime_ = 0.0f;

	emitter_.color = { {1,1,1,1} ,{1,1,1,1} };
	emitter_.renge.max = Vector3{ 1.0f,1.0f,1.0f };
	emitter_.renge.min = Vector3{ -1.0f,-1.0f,-1.0f };
	emitter_.color.max = Vector4{ 1,1,1,1 };
	emitter_.color.min = Vector4{ 0,0,0,0 };
	emitter_.rotate.min = Vector3{ 0,0,0 };
	emitter_.rotate.max = Vector3{ 0,0,0 };
	emitter_.size.min = Vector3{ 1.0f,1.0f,1.0f };
	emitter_.size.max = Vector3{ 1.0f,1.0f,1.0f };
	emitter_.lifeTime.min = 1.0f;
	emitter_.lifeTime.max = 3.0f;
	emitter_.velocity.min = Vector3{ -1.0f,-1.0f,-1.0f };
	emitter_.velocity.max = Vector3{ 1.0f,1.0f,1.0f };
	emitter_.count = 10;
	//emitter_.

	isLifeTimeScale_ = false;	// スケール
	isLifeTimeAlpha_ = false;	// 透明度
	isLifeTimeVelocity = false;// 速度

	isGravity = false; // 重力
	isWind = false;  // 風

	usebillboard = true;// ビルボード
	isAlpha = false;    // 透明度
}

void ParticleEmitter::Update()
{


	

	ParticleManager::ParticleGroup& particleGroup = ParticleManager::GetInstance()->GetParticleGroups(particleName_);
	
	transform_.Update();
	emitter_.worldtransform = transform_;
	particleGroup.emiter.worldtransform = emitter_.worldtransform;
	particleGroup.emiter.worldtransform.Update();



	frequencyTime_ += MyGame::GameTime();
		if (frequency_ <= frequencyTime_) {
			Emit();
			frequencyTime_ -= frequency_;
		}
	

	// 寿命が尽きたパーティクルを削除する処理
	particleGroup.particle.remove_if([](const ParticleManager::Particle& p)
		{
			return p.currentTime >= p.lifeTime;
		});
	

	
}

void ParticleEmitter::Emit()
{
	ParticleManager::GetInstance()->GetParticleGroups(particleName_).usebillboard = usebillboard; // ビルボード
	ParticleManager::GetInstance()->GetParticleGroups(particleName_).isAlpha = isAlpha; // 透明度
	ParticleManager::GetInstance()->GetParticleGroups(particleName_).isGravity = isGravity; // 重力
	ParticleManager::GetInstance()->GetParticleGroups(particleName_).isLifeTimeScale_ = isLifeTimeScale_; // 重力
	ParticleManager::GetInstance()->GetParticleGroups(particleName_).isRotateVelocity = isRotateVelocity; // 回転速度
	ParticleManager::GetInstance()->GetParticleGroups(particleName_).isBounce = isBounce; // 回転速度

	if (emitType_ == ParticleManager::EmitType::kRandom) {
		ParticleManager::GetInstance()->GetParticleGroups(particleName_).emiter = emitter_;
		ParticleManager::GetInstance()->Emit(particleName_, "rand", emitType_);
	}
	if (emitType_ == ParticleManager::EmitType::kConstant) {
		ParticleManager::GetInstance()->GetParticleGroups(particleName_).emiter = emitter_;
		ParticleManager::GetInstance()->Emit(particleName_, "rand", emitType_);
	}
}

void ParticleEmitter::SetParent(WorldTransform& parent)
{
	transform_.parent_ =  &parent;
	//emitter_.worldtransform.parent_ = &parent;
	//ParticleManager::GetInstance()->GetParticleGroups(particleName_).emiter.worldtransform.parent_ = &parent;
}

void ParticleEmitter::SetIsAll(bool billboard, bool alpha, bool gravity, bool isLifeTimeScale, bool rotateVelocity)
{
	usebillboard = billboard;
	isAlpha = alpha;
	isGravity = gravity;
	isLifeTimeScale_ = isLifeTimeScale;
	isRotateVelocity = rotateVelocity;
}
