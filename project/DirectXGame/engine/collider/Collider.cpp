#include "Collider.h"

void Collider::Initialize(Camera* camera) {

	object.Initialize();
	object.SetCamera(camera);
	object.SetModel("enemy.obj");
	count++;
}

void Collider::UpdateWorldTransform() {

	// ワールド座標をワールドトランスフォームに適応
	object.worldtransform_.translate_ = GetCenterPosition();

	object.Update();
}

void Collider::Draw() {

	object.Draw();
	
}

void Collider::SetTypeID(uint32_t typeID) { typeID_ = typeID; };
