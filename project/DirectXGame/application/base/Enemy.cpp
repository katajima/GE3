#include "Enemy.h"

void Enemy::Initialize(Vector3 position, float HP)
{
	object_.Initialize();
	object_.SetModel("train.obj");
	object_.transform = { { 1, 1, 1}, { 0, 0, 0}, position };
	isAlive_ = true;
	HP_ = HP;
}

void Enemy::Update()
{

	if (HP_ <= 0) {
		isAlive_ = false;
	}


	object_.Update();
}

void Enemy::Draw()
{
	// 生きていたら
	if (isAlive_) {
		object_.Draw();
	}
}
