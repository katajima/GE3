#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include "DirectXGame/engine/input/Input.h"


class AttackManeger
{
public:
	// 攻撃用ワーク
	struct WrokAttack {
		// 攻撃ギミックの媒介変数
		int attackParameter_ = 0;
		int MaxAttackParameter_ = 0;
		int comboIndex = 0;
		int inComboPhase = 0;
		bool comboNext = false;
		int comboSwitchingNum = 0;
		Vector2 joyDirection;

		bool isAttack = false;
	};
	WrokAttack workAttack{};

	// 攻撃用定数
	struct ConstAttack {
		// 振りかぶり時間
		uint32_t anticipationTime;
		// ための時間
		uint32_t chargeTime;
		// 攻撃振り時間
		uint32_t swingTime;
		// 硬直時間
		uint32_t recoveryTime;
		// 振りかぶり移動速さ
		float anticipationSpeed;
		// ための移動速さ
		float chargeSpeed;
		// 攻撃振りの移動速さ
		float swingSpeed;
	};
public:

	///< summary>
	/// 初期化
	///</summary>
	//void Initialize(int comboNum);

	void Initialize();

	void Initialize(WrokAttack& work);

	///< summary>
	/// 更新
	///</summary>
	void Update(WrokAttack& work);

	///< summary>
	/// 描画
	///</summary>
	void DrawImGui();

	//bool GetIsAttackCombo() const { return isAttack; };

	
private:

	void SetAttackCombo(WrokAttack& work);

	
private:
	// コンボの数
	int ComboNum = 3;

	bool isCombo = false;

	
	// コンボ定数表
	//static std::array<ConstAttack, ComboNum> kConstAttacks_;

};


