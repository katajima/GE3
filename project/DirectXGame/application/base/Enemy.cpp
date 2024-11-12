#include "Enemy.h"
#include "imgui.h"

void Enemy::Initialize(Vector3 position, float HP,Camera*camera)
{

	object_.Initialize();
	object_.SetModel("enemy.obj");
	object_.SetCamera(camera);
	object_.transform = { { 1.5f, 1.5f, 1.5f}, { 0, 0, 0}, position };
	isAlive_ = true;
	HP_ = HP;
}

void Enemy::Update()
{
#ifdef _DEBUG
	ImGui::Begin("enemy");
	{
		
		Vector3 pos = object_.GetWorldPosition();
		float php = HP_;
		bool pos3 = isAlive_;
		ImGui::InputFloat3("tanslateW", &pos.x);
		ImGui::InputFloat("hp", &php);
		ImGui::Checkbox("isAlive", &pos3);
	}
	ImGui::End();
#endif

	if (HP_ <= 0) {
		isAlive_ = false;
	}


	object_.Update();
}

void Enemy::Draw()
{
	// 生きていたら
	//if (isAlive_) {
		object_.Draw();
	//}
}
