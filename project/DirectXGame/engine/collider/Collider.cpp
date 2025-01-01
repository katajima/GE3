#include "Collider.h"

void Collider::Initialize() {

object.Initialize();
	count++;
}

void Collider::UpdateWorldTransform() {

	// ワールド座標をワールドトランスフォームに適応
	object.transform.translate = GetCenterPosition();

	object.Update();
}

void Collider::Draw() {

	
}

void Collider::SetTypeID(uint32_t typeID) { typeID_ = typeID; };
