#pragma once
#include"DirectXGame/engine/base/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/math/MathFanctions.h"
#include"DirectXGame/engine/input/Input.h"
#include <imgui.h>
#include <list>


#include"BasePlayerState.h"
#include"Attack/AttackManeger.h"

///< summary>
/// 自キャラ
///</summary>
class Player {
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

public:

	void Move();

	void Attack();

	// 調整項目の適用
	void ApplyGlobalVariables();

	void ChangeState(std::unique_ptr<BasePlayerState> state);


	Object3d& GetObject3D() { return object_; }

	// カメラのビュープロジェクション
	void SetCamera(const Camera* camera) { camera_ = camera; };

	AttackManeger::WrokAttack GetWorkAttak() const { return workAttack_; }
	
	void SetIsWorkAttak() {workAttack_.isAttack = true; }
	
	bool SetIsWWorkAttak() const { return workAttack_.isAttack; }
	


private:

	std::unique_ptr < AttackManeger> attackManeger_;

	
	AttackManeger::WrokAttack workAttack_;

	AttackManeger::ConstAttack combo_;

	// 状態
	std::unique_ptr<BasePlayerState> state_;



	// 行動フェーズ
	enum class Phase {
		Move, // 移動する
		Attack,    // 攻撃する
	};

	// フェーズ
	Phase phase_ = Phase::Move;


private: // 攻撃



private:
	const Camera* camera_ = nullptr;

	// 振るまい
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
		kJump,   // ジャンプ中
	};
	//振るまい
	Behavior behavior_ = Behavior::kRoot;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	Object3d object_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	int behaviorChange = 1;
	int behaviorTimer = 0;

	// テクスチャハンドル
	//uint32_t textureHandle_ = 0u;

	// 浮遊ギミック媒介変数
	float floatingParameter_ = 0.0f;
	float attackParameter_;

	// 浮遊サイクル<frame>
	int floatingPeriod = 120;

	// 浮遊の振幅<m>
	float floatingAmplitude = 1.0f;
	// 速度
	Vector3 velocity_ = {};

	int jampCount = 0;

	float speed;
};


