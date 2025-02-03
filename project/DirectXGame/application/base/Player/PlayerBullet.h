#pragma once
#include"DirectXGame/engine/Camera/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/2d/Sprite.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/input/Input.h"
#include <imgui.h>
#include <list>
#include "DirectXGame/engine/collider/Collider.h"

#include "DirectXGame/engine/collider/Collider.h"
#include"DirectXGame/engine/collider/ContactRecord.h"

#include "DirectXGame/engine/effect/Particle/ParticleManager.h"
#include "DirectXGame/engine/effect/Particle/ParticleEmitter.h"

#include "DirectXGame/engine/effect/Trail/TrailEffect.h"

class Enemy;

class Player;


class PlayerBullet : public Collider
{
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

	void SetTergetPos(const Vector3& pos) { terget = pos; }

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; };

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	virtual Vector3 GetCenterPosition() const;

	bool GetAlive() const { return isAlive_; };

	void SetPlayer(Player* player);

	void SetIndex(int index) { index_ = index; }

	Object3d& GetObject3D() { return object_; }

	void SetParent(WorldTransform parent) { object_.worldtransform_.parent_ = &parent; };

private:
	const Camera* camera_ = nullptr;
	Player* player_;
	
	//ParticleManager* particleManager_;
	ParticleEmitter* emitter_;
	ParticleManager::Constant bullet;
	ParticleManager::Constant exp;

	std::string strin;
	std::string strin2;

	ContactRecord contactRecord_;

	Object3d object_;

	Vector3 terget;

	// 速度
	Vector3 velocity_ = {};

	Enemy* enemy_ = nullptr;

	int phase_= 0;

	// 上に飛ぶ高さ
	float max_y = 20.0f;
	// 
	Vector3 randPosSky;
	Vector3 posGround;
	Vector3 str;
	float count = 0;
	float max_count = 0.5f;
	float t;

	Vector3 tragetPos;


	// デスフラグ
	bool isAlive_ = false;
	// ヒット
	bool Hit = false;

	int index_ = 0;


	std::unique_ptr<TrailEffect> trailEffect_;
	Object3d objectStr_;
	Object3d objectEnd_;

	int countTrail = 0;
};

