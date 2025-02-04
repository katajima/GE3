#pragma once
#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/2d/Sprite.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/input/Input.h"
#include <imgui.h>
#include <list>


#include"BasePlayerState.h"

#include"playerWeapon.h"

#include "DirectXGame/engine/effect/Particle/ParticleManager.h"
#include "DirectXGame/engine/effect/Particle/ParticleEmitter.h"

#include "DirectXGame/engine/collider/Collider.h"
#include "PlayerBullet.h"

#include "DirectXGame/engine/effect/Trail/TrailEffect.h"

#include "DirectXGame/engine/struct/DeltaTime.h"

#include "DirectXGame/engine/MyGame/MyGame.h"

///< summary>
/// 自キャラ
///</summary>

class Enemy;

class Player : public Collider {
public:


	///< summary>
	/// 初期化
	///</summary>
	void Initialize(Vector3 position, Camera* camera);

	///< summary>
	/// 更新
	///</summary>
	void Update();

	///< summary>
	/// 描画
	///</summary>
	void Draw();

	void DrawP();
	
	void Draw2D();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	virtual Vector3 GetCenterPosition() const;

	
	// 振るまい
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
		kJump,   // ジャンプ中
		kDie,       // 死亡状態
	};
private: //Behavior

	//通常行動初期化
	void BehaviorRootInitialize();

	//通常行動更新
	void BehaviorRootUpdate();

	//攻撃行動初期化
	void BehaviorAttackInitialize();

	//攻撃行動更新
	void BehaviorAttackUpdate();

	//死亡行動初期化
	void BehaviorDieInitialize();

	//死亡行動更新
	//では必殺技でフェーズ1で選択、フェーズ2で発射(上空に)、フェーズ3で(的へ(上付近まで))、フェーズ4で(的に向かい着弾))
	void BehaviorDieUpdate();


	//振るまい
	Behavior behavior_ = Behavior::kRoot;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
public: // 攻撃関係
	struct Parameter
	{
		float t;
		float max_t;
	};
	struct AttackKeyFlag
	{
		bool IsAttack; // 攻撃するか

		bool IsNormalAttack; // B
		bool IsDashAttack;   // X
		bool IsJampAttack;   // Y
		bool IsSpecialAttack;// RT
	};
	enum class  AttackType
	{
		kNone = 0,
		kNormal, // 通常攻撃
		kDash,   // ダッシュ攻撃
		kJamp,   // ジャンプ攻撃
		kSpecial,// 必殺技攻撃
	};
private: // 攻撃関係
	struct StartEnd
	{
		Vector3 str;
		Vector3 end;
	};

	// 攻撃用ワーク
	struct WrokAttack {
		// 攻撃ギミックの媒介変数
		Parameter attackAll; // 攻撃
		AttackKeyFlag key; // 攻撃方法キー
		float parameter;
		// 
		//振るまい
		AttackType type = AttackType::kNone;
		// 次の振るまいリクエスト
		std::optional<AttackType> typeRequest_ = std::nullopt;
		//
		StartEnd pos; // 位置
		// 過去位置
		Vector3 oldPos;

		// ジャンプ攻撃時の移動ベクトル
		Vector3 velocity;


		int comboIndex = 0;
		int inComboPhase = 0;
		bool comboNext = false;
		int comboSwitchingNum = 0;
		Vector2 joyDirection;
		//
		// ヒットカウント
		int hitCount = 0;
		float hitTime = 0;
	};
	WrokAttack workAttack{};
	
	
	struct  SpecialAttack
	{
		int specialGauge = 0;
		bool isSpecial = false;

		// フェーズ
		int phese = 0;
		// 時間
		float time = 0;
		// 
		int max; 
		// クロック
		int clock = 1;

	};
	SpecialAttack specialAttack{};
	int index_b = 0;

	// コンボの数
	static const int ComboNum = 4;

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
	// 攻撃再発動時間
	float recastTime = 0;
	const float MaxRecastTime = 1.0f;

	// 攻撃キー入力
	void AttackKey();
	// 攻撃タイプ
	void AttackTypes();
	// 攻撃タイプ初期化
	void AttackTypeInit(int comboIndex);

	//攻撃
	void Attack();

	// 調整項目の適用
	void ApplyGlobalVariables();

	void SetAttackCombo(WrokAttack& work);



public:

	void LockOn(std::vector <std::unique_ptr< Enemy >>& enemys);

	const int MaxLockOn = 10;
	bool isLockOn = false;


private: // 移動
	 
	void Move();

	void Gravity();

	void AddMove();

	float graVelo;

	float groundY = 2;

	bool isJamp = false;

	float ty = 0;

	bool isMove = false;

	
public:
	std::string strin;

	Object3d& GetObject3D() { return objectBase_; }
	//Object3d& GetObjectWeapon3D() { return ; }

	

	// カメラのビュープロジェクション
	void SetCamera(Camera* camera) { camera_ = camera; };
	
	playerWeapon* GetWeapon() { return weapon_.get(); }

	//void SetPlayer(Player* play) { weapon_->SetPlayer(this); }

	Behavior GetBehavior() const { return behavior_; };
	AttackType GetAttackType() const { return workAttack.type; };

	uint32_t GetSerialNumber() const { return serialNumber; }

	bool GetAlive() const { return isAlive; };
	
	void AddDamege(float da) { hp -= int(da); };

	void AddSpecial(int d) { specialAttack.specialGauge += d; };

	bool GetInvincible() const { return isInvincible; }
	
	bool GetIsSpecial() const { return specialAttack.isSpecial; }

	int GetHitCount() const { return workAttack.hitCount; }

	void AddHit() { workAttack.hitCount++; };
	void AddSP() { specialAttack.specialGauge++; };

	void SetHitTime() { workAttack.hitTime = 1.5f; }

	// 弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return playerBullet_; }

private:

	//DeltaTime deltaTime_;

private:  // パラメータ
	
	uint32_t maxHp = 100;
	int hp = 100;
	bool isAlive = true;
	float damage_ = 0;
	bool isInvincible = false;
private:
	Camera* camera_ = nullptr;
	Object3d objectBase_;
	// 本体
	Object3d objectBody_;
	
	//　レティクル
	Object3d objectReticle_;
	
	// ミサイル発射位置
	Object3d injectionLeftObj_;
	Object3d injectionRightObj_;



	// 影
	Object3d objectSha_;

	std::unique_ptr<playerWeapon> weapon_;
	
	Object3d weaponStr;
	Object3d weaponEnd;


	std::list<std::unique_ptr<PlayerBullet>> playerBullet_;

	// シリアルナンバー
	uint32_t serialNumber = 0;

	std::unique_ptr<Sprite> HpBer_;
	std::unique_ptr<Sprite> SpecailBer_;
	std::unique_ptr<Sprite> textMax_;
	std::unique_ptr<Sprite> textRB_;
	bool isTextRB_ = false;

	// 速度
	Vector3 velocity_ = {};
	
	
	ParticleManager::Constant slash;
	
	// 浮遊ギミック媒介変数
	float floatingParameter_ = 0.0f;
	float attackParameter_;

	// 浮遊サイクル<frame>
	//int floatingPeriod = 120;

	// 浮遊の振幅<m>
	//float floatingAmplitude = 1.0f;
	
	
	float speed;

	std::vector<Enemy*> lockedOnEnemies;


	std::unique_ptr<TrailEffect> trailEffect_;
	bool flag33;
};


