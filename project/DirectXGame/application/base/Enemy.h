#pragma once
#include"DirectXGame/engine/base/Camera.h"
#include"DirectXGame/engine/3d/Object3d.h"
#include"DirectXGame/engine/base/ImGuiManager.h"
#include"DirectXGame/engine/math/MathFanctions.h"

class Enemy
{
public:
	// 初期化
	void Initialize(Vector3 position,float HP);
	
	// 毎フレーム更新
	void Update();

	// 描画
	void Draw();

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
	Object3d& GetObjectTrans() { return object_; }
	// 3Dオブジェクトの参照を返す（読み取り専用）
	//const Object3d& GetObjectTrans() const { return object_; }

private:
	// オブジェクト
	Object3d object_;

	// 生死
	bool isAlive_ = true;

	float HP_;

	// 移動
	Vector3 velocity_;
};


