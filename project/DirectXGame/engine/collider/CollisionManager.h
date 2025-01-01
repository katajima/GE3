#pragma once
#include"DirectXGame/engine/math/MathFanctions.h"
#include"list"
#include"Collider.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {
public:
	// リセット
	void Reset();

	~CollisionManager();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	// 全ての当たり判定チェック
	void CheckAllCollisions();

	void AddCollider(Collider* collider);

	// 初期化
	void Initialize();

	// ワールドトランスフォームの更新
	void UpdateWorldTransform();

	//描画
	void Draw();

	// 調整項目の適用
	void ApplyGlobalVariables();

private:
	// コライダー
	std::list<Collider*> colliders_;

	// 衝突モデル
	//std::unique_ptr<Model> modelCollosion_;

	bool isCollider;

};

