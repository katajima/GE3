#include "Player.h"

void Player::Initialize(Vector3 position,Camera* camera)
{
	
	// プレイヤー
	object_.Initialize();
	object_.SetCamera(camera);
	object_.SetModel("rail.obj");
	object_.transform.translate = position;

	attackManeger_ = std::make_unique<AttackManeger>();
	
	ChangeState(std::make_unique<PlayerStateMove>(this));

}

void Player::Update()
{
	
	attackManeger_->DrawImGui();

	

	state_->Update();

	
	object_.Update();

}

void Player::Draw()
{
	object_.Draw();

	
}

void Player::Move()
{
	speed = 0.3f;
	move = { 0,0,0 };
	velocity_ = { 0,0,0 };
	if (Input::GetInstance()->IsPushKey(DIK_W)) {
		move.z += 0.3f;
	}
	if (Input::GetInstance()->IsPushKey(DIK_S)) {
		move.z -= 0.3f;

	}
	if (Input::GetInstance()->IsPushKey(DIK_A)) {
		move.x -= 0.3f;

	}
	if (Input::GetInstance()->IsPushKey(DIK_D)) {
		move.x += 0.3f;
	}


	if (move.x != 0.0f || move.z != 0.0f) {
		// 入力方向を正規化
		move = Normalize(move);

		// カメラのビュー行列の逆行列（カメラのワールド変換行列）を取得
		Matrix4x4 cameraWorldMatrix = Inverse(camera_->viewMatrix_);

		// カメラの向きに基づいて移動方向をワールド座標系に変換
		Vector3 worldDirection = {
			move.x * cameraWorldMatrix.m[0][0] + move.z * cameraWorldMatrix.m[2][0],
			0.0f,
			move.x * cameraWorldMatrix.m[0][2] + move.z * cameraWorldMatrix.m[2][2]
		};

		velocity_ = Multiply(Normalize(worldDirection),speed);

		
	}

	object_.transform.translate = Add(object_.transform.translate, velocity_);
}

void Player::Attack()
{
	workAttack_.isAttack = true;

	attackManeger_->Update(workAttack_);


	if (!workAttack_.isAttack) {
		ChangeState(std::make_unique<PlayerStateMove>(this));
	}

}


void Player::ApplyGlobalVariables()
{
}

void Player::ChangeState(std::unique_ptr<BasePlayerState> state)
{
	state_ = std::move(state);

	//name = 

}
