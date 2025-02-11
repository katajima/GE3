#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/struct/Structs.h"
#include"DirectXGame/engine/base/DirectXCommon.h"
#include"DirectXGame/engine/base/SrvManager.h"
#include<random>
#include<numbers>
#include"ParticleManager.h"

class ParticleEmitter
{
public:
	// 

	enum class EmitSpawnShapeType // 出現形状
	{
		kAABB,		// AABB
		kOBB,		// OBB
		kSphere,	// Sphere
		kSegment,		// Line
	};

public:
	// count: パーティクルの最大生成数を指定する値。frequency: パーティクルの発射間隔を秒単位で指定する値。frequencyTime: 現在の発射間隔の経過時間を追跡する値。
	//ParticleEmitter(std::string name,Transform transform, uint32_t count, float frequency, float frequencyTime);

	void Initialize(std::string emitName, std::string particleName, ParticleManager::EmitType type);

	void Update();

	void Emit();
public: // ゲッター兼セッター

	float& GetFrequency() { return frequency_; };//発生数

	uint32_t& GetCount() { return count_; };// 発生頻度


	
	void SetParent(WorldTransform& parent);// 親子付け

	void SetCount(uint32_t count) { count_ = count; }; // 出現数

	void SetFrequency(float frequency) { frequency_ = frequency_; } // 頻度

	void SetPos(Vector3 pos) { transform_.translate_ = pos; } // 位置

	void SetIsGravity(bool is) { isGravity = is; } // 重力

	void SetUsebillboard(bool is) { usebillboard = is; } // ビルボード

	void SetIsAlpha(bool is) { isAlpha = is; } // 透明度
	
	void SetIsLifeTimeScale(bool is) { isLifeTimeScale_ = is; } // スケール変更
	
	void SetIsRotateVelocity(bool is) { isRotateVelocity = is; } // 回転速度

	void SetIsBounce(bool is) { isBounce = is; } // 跳ねるか

	void SetIsAll(bool billboard, bool alpha, bool gravity, bool isLifeTimeScale, bool rotateVelocity);

	// ランダム用
	void SetRengeMinMax(Vector3 min, Vector3 max) { emitter_.renge.min = min; emitter_.renge.max; }; // 範囲

	void SetColorMinMax(Vector4 min, Vector4 max) { emitter_.color.min = min;emitter_.color.max = max; } // カラー

	void SetSizeMinMax(Vector3 min, Vector3 max) { emitter_.size.min = min; emitter_.size.max = max; } // サイズ

	void SetRotateMinMax(Vector3 min, Vector3 max) { emitter_.rotate.min = min; emitter_.rotate.max = max; } // 回転

	void SetLifeTimeMinMax(float min, float max) { emitter_.lifeTime.min = min;emitter_.lifeTime.max = max; } // 生存時間

	void SetVelocityMinMax(Vector3 min, Vector3 max) { emitter_.velocity.min = min;emitter_.velocity.max = max; } // 速度

	void SetRotateVelocityMinMax(Vector3 min, Vector3 max) { emitter_.rotateVelocity.min = min;emitter_.rotateVelocity.max = max; } // 速度

	
	
private:
	ParticleManager::EmitType emitType_ = ParticleManager::EmitType::kRandom; // 出現方法
	EmitSpawnShapeType spawnShapeType_ = EmitSpawnShapeType::kAABB; // 出現形状

private:
	bool isLifeTimeScale_ = false;	// スケール
	bool isLifeTimeAlpha_ = false;	// 透明度
	bool isLifeTimeVelocity = false;// 速度
	bool isRotateVelocity = false;  // 回転

	bool isGravity = false; // 重力
	bool isWind = false;  // 風
	bool usebillboard = true; // ビルボード
	bool isAlpha = false; // 透明度
	bool isBounce = false; // 透明度


public:
	WorldTransform transform_;
private:
	uint32_t count_;			// < 発生数
	float frequency_;		// < 発生頻度
	float frequencyTime_;	// < 頻度用時刻



	std::string emitName_; // エミッター名
	std::string particleName_; // パーティクル名


	ParticleManager::Emiter emitter_{};
};



