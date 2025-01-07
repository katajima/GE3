#include "CollisionManager.h"
#include"DirectXGame/application/GlobalVariables/GlobalVariables.h"
void CollisionManager::Reset() {
	// リストを空っぽにする
	colliders_.clear();
}

CollisionManager::~CollisionManager() {


}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	// コライダーAの座標を取得
	Vector3 posA = colliderA->GetCenterPosition();
	// コライダーBの座標を取得
	Vector3 posB = colliderB->GetCenterPosition();
	// 座標の差分ベクトル
	Vector3 subtract = Subtract(posB, posA);
	// 座標AとBの距離を求める
	float distance = Length(subtract);
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// コライダーAの衝突判時コールバックを呼び出す
		colliderA->OnCollision(colliderB);
		// コライダーBの衝突判時コールバックを呼び出す
		colliderB->OnCollision(colliderA);
	}
	//  else {
	//	// コライダーAの衝突判時コールバックを呼び出す
	//	colliderA->NoCollision(colliderB);
	//	// コライダーBの衝突判時コールバックを呼び出す
	//	colliderB->NoCollision(colliderA);
	//}
	
}

void CollisionManager::CheckAllCollisions() {
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはi\イテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::AddCollider(Collider* collider) {
	// NULLポインタでないことを確認
	if (collider != nullptr) {
		colliders_.push_back(collider);
	}
}

void CollisionManager::Initialize() { 

	//modelCollosion_.reset(Model::CreateFromOBJ("collision", true)); 

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* gropName = "CollisionManager";
	// グループを追加する
	GlobalVariables::GetInstance()->CreateGroup(gropName);
	globalVariables->AddItem(gropName, "isCollider", isCollider);
}

void CollisionManager::UpdateWorldTransform() {

	ApplyGlobalVariables();

	// 非表示なら抜ける
	if (isCollider == false) {
		return;
	}

	// 全てのコライダーについて
	for (Collider* collider : colliders_) {
		//更新
		collider->UpdateWorldTransform();
	}

}

void CollisionManager::Draw() {

	// 非表示なら抜ける
	if (isCollider == false) {
		return;
	}

	// 全てのコライダーについて
	for (Collider* collider : colliders_) {
		// 更新
		collider->Draw();
	}

}

void CollisionManager::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* gropName = "CollisionManager";
	// グループを追加する
	GlobalVariables::GetInstance()->CreateGroup(gropName);
	isCollider = globalVariables->GetBoolValue(gropName, "isCollider");

}
