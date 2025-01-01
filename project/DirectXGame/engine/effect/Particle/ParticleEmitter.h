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
	// count: パーティクルの最大生成数を指定する値。frequency: パーティクルの発射間隔を秒単位で指定する値。frequencyTime: 現在の発射間隔の経過時間を追跡する値。
	ParticleEmitter(std::string name,Transform transform, uint32_t count, float frequency, float frequencyTime);


	void Update();

	void Emit();

private:
	Transform transform_;	// < エミッタのTransform
	uint32_t count_;			// < 発生数
	float frequency_;		// < 発生頻度
	float frequencyTime_;	// < 頻度用時刻

	const float kDeltaTime = 1.0f / 60.0f;

	std::string name_;
};



