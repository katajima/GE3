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
	enum class EmitType // 出現方法
	{
		kRandom,   // ランダム
		kConstant, // 定数
	};

	enum class EmitSpawnShapeType // 出現形状
	{
		kAABB,		// AABB
		kOBB,		// OBB
		kSphere,	// Sphere
		kSegment,		// Line
	};

	enum class AAAAAAAAAAAAA;




public:
	// count: パーティクルの最大生成数を指定する値。frequency: パーティクルの発射間隔を秒単位で指定する値。frequencyTime: 現在の発射間隔の経過時間を追跡する値。
	//ParticleEmitter(std::string name,Transform transform, uint32_t count, float frequency, float frequencyTime);

	void Initialize(std::string emitName, std::string particleName);

	void Update();

	void Emit();
public: // ゲッター兼セッター

	float& GetFrequency() { return frequency_; };//発生数

	uint32_t& GetCount() { return count_; };// 発生頻度


	
	void SetParent(WorldTransform parent) { transform_.parent_ = &parent; }// 親子付け

private:
	EmitType emitType_ = EmitType::kRandom; // 出現方法
	EmitSpawnShapeType spawnShapeType_ = EmitSpawnShapeType::kAABB; // 出現形状

private:
	bool isLifeTimeScale_ = false;	// スケール
	bool isLifeTimeAlpha_ = false;	// 透明度
	bool isLifeTimeVelocity = false;// 速度

	bool isGravity = false; // 重力
	bool isWindow = false;  // 風


private:
	WorldTransform transform_;
	uint32_t count_;			// < 発生数
	float frequency_;		// < 発生頻度
	float frequencyTime_;	// < 頻度用時刻



	std::string emitName_; // エミッター名
	std::string particleName_; // パーティクル名



};



