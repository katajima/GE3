#pragma once
#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/math/MathFanctions.h"

#include "DirectXGame/engine/collider/Collider.h"
#include"DirectXGame/engine/collider/ContactRecord.h"

#include "DirectXGame/engine/effect/Particle/ParticleManager.h"
#include "DirectXGame/engine/effect/Particle/ParticleEmitter.h"

#include"DirectXGame/engine/2d/Sprite.h"

class Player;
class FollowCamera;

class Enemy : public Collider
{
public:
	// デフォルトコンストラクタ
	Enemy();

	// 初期化
	void Initialize(Vector3 position,float HP,Camera* camera);
	
	// 毎フレーム更新
	void Update();

	// 描画
	void Draw();
	//
	void DrawP();

	void Draw2D();

	// 移動
	void Move();
	// 
	void HitMotion();

	void Emit();

	// ダメージ
	void addDamage(float damage) {
		HP_ -= damage;
		if (HP_ <= 0) {
			HP_ = 0;
			isAlive_ = false; // 敵が死亡
		}
	}

	// 生存判定
	bool GetAlive() const { return isAlive_; }

	// 3Dオブジェクトの参照を返す（変更可能）
	Object3d& GetObject3D() { return object_; }
	// 3Dオブジェクトの参照を返す（読み取り専用）
	//const Object3d& GetObjectTrans() const { return object_; }

	Vector3& GetPostion() { return object_.worldtransform_.translate_; };
	float& GetHP() { return HP_; };
	void AddDamege(float da) { HP_ -= da; };


	void SetPostion(Vector3 pos) { object_.worldtransform_.translate_ = pos; }
	uint32_t GetSerialNumber() const { return serialNumber; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	virtual Vector3 GetCenterPosition() const;

	void SetPlayer(Player* player) { player_ = player; };

	void SetHit() { hit = true; };

	bool GetLockOn() { return isLockOn; }

	void SetLockOn(bool lock) { isLockOn = lock; }

	float Timer() const;
	void hitStop(float time) {
		hitStopTimer = time;
		kHitStopTimer = time;
	};

	void Shake();

	void SetFollowCamera(FollowCamera* followCamera) { followCamera_ = followCamera; }

private:

	void InitParticle();

	void HitStpoTime();

	// シリアルナンバー
	uint32_t serialNumber = 0;
	// 次のシリアルナンバー
	static uint32_t nextSerialNumber;
	ContactRecord contactRecord_;

	bool hit = false;
	float count = 0;

	bool isLockOn = false;

private:
	// オブジェクト
	Object3d object_;
	Object3d objectSha_;

	WorldTransform transBase_;
	
	Vector3 oldPos_;

	Player* player_;

	// 生死
	bool isAlive_ = true;

	float HP_;
	float MaxHP_;
	

	// 移動
	Vector3 velocity_;

	
	ParticleManager::Constant dame;


	std::unique_ptr<Sprite> icon_lockOn;
	std::unique_ptr<Sprite> hpBer_;
	std::unique_ptr<Sprite> backHpBer_;


	std::unique_ptr<ParticleEmitter> groundRightEmit_ = nullptr;
	std::unique_ptr<ParticleEmitter> groundLeftEmit_ = nullptr;

	std::unique_ptr<ParticleEmitter> starEmit_ = nullptr;
	std::unique_ptr<ParticleEmitter> traiEmit_ = nullptr;
	std::unique_ptr<ParticleEmitter> hitEmit_ = nullptr;
	std::unique_ptr<ParticleEmitter> dustEmit_ = nullptr;

	float hitStopTimer;
	float kHitStopTimer;
	float timeSpeed_ = 1.0f;

	FollowCamera* followCamera_;
};


