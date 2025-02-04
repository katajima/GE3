#include "Enemy.h"
#include "imgui.h"
#include "Player/Player.h"
#include "DirectXGame/application/base/FollowCamera/FollowCamera.h"

uint32_t Enemy::nextSerialNumber = 0;

Enemy::Enemy() {
	// シリアル番号を振る
	serialNumber = nextSerialNumber;
	// 次の番号を1加算
	++nextSerialNumber;
}

void Enemy::Initialize(Vector3 position, float HP, Camera* camera)
{
	Collider::Initialize(camera);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));


	transBase_.Initialize();
	transBase_.translate_ = position;


	object_.Initialize();
	object_.SetModel("enemy2.obj");
	object_.SetCamera(camera);
	object_.worldtransform_.parent_ = &transBase_;
	//object_.worldtransform_.translate_ = position;
	object_.worldtransform_.scale_ = { 1.7f,1.7f,1.7f };



	objectSha_.Initialize();
	objectSha_.SetCamera(camera);
	objectSha_.SetModel("plane.obj");
	objectSha_.model->modelData.material[0]->tex_.diffuseFilePath = "resources/Texture/aa.png";
	objectSha_.model->modelData.material[0]->color = { 0.0f,0.0f,0.0f,1 };
	objectSha_.worldtransform_.translate_ = position;
	objectSha_.worldtransform_.scale_ = { 4,4,4 };
	objectSha_.worldtransform_.rotate_.x = DegreesToRadians(-90);




	isAlive_ = true;
	HP_ = HP;
	MaxHP_ = HP;

	InitParticle();

	icon_lockOn = std::make_unique<Sprite>();
	icon_lockOn->Initialize("resources/Texture/icon/LockOnW.png");
	icon_lockOn->SetSize(0.10f);
	icon_lockOn->SetColor({ 1,0,1,1 });
	icon_lockOn->SetPosition({ -100,650 });
	icon_lockOn->SetAnchorPoint({ 0.5f,0.5f });

	hpBer_ = std::make_unique<Sprite>();
	hpBer_->Initialize("resources/Texture/Image.png");
	//hpBer_->SetSize(0.10f);
	hpBer_->SetColor({ 1,0,0,0.7f });
	hpBer_->SetPosition({ -100,650 });
	hpBer_->SetAnchorPoint({ 0.5f,0.0f });

	backHpBer_ = std::make_unique<Sprite>();
	backHpBer_->Initialize("resources/Texture/Image.png");
	//backHpBer_->SetSize(0.10f);
	backHpBer_->SetColor({ 0.1f,0.1f,0.1f,0.7f });
	backHpBer_->SetPosition({ -100,650 });
	backHpBer_->SetAnchorPoint({ 0.5f,0.0f });



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
		Vector2 pos2d = object_.GetScreenPosition();
		ImGui::InputFloat2("screen", &pos2d.x);
		/*ImGui::InputFloat("hp", &php);
		ImGui::Checkbox("isAlive", &pos3);*/


	}
	ImGui::End();
#endif
	
	HitStpoTime();
	if (HP_ <= 0) {
		isAlive_ = false;
	}

	if (isAlive_) {
		if (!hit) {
			count = 0.0f;
			Move();
			oldPos_ = {0,0,0};
		}
		else {
			oldPos_ = {0,0,0};
			HitMotion();
		}
		// 影
		objectSha_.worldtransform_.translate_ = transBase_.translate_;
		objectSha_.worldtransform_.translate_.y = 0.1f;

		Vector3 scale{};
		scale = 7;

		objectSha_.worldtransform_.scale_ = scale;

		// 煙
		dustEmit_->Update();
	}

	object_.Update();
	transBase_.Update();
	objectSha_.Update();

	
}

void Enemy::Draw()
{
	// 生きていたら
	if (isAlive_) {
		object_.Draw();
		objectSha_.Draw();
	}
}

void Enemy::DrawP()
{
	//ParticleManager::GetInstance()->GetInstance()->Draw();
	//ParticleManager::GetInstance()->GetInstance()->DrawAABB();
}

void Enemy::Draw2D()
{
	if (isLockOn) {
		icon_lockOn->SetPosition(object_.GetScreenPosition());

		icon_lockOn->Update();
		icon_lockOn->Draw();
	}

	if (isAlive_) {

		backHpBer_->SetSize({ MaxHP_ ,15.0f });
		backHpBer_->SetPosition(object_.GetScreenPosition() + Vector2{ 0,-30 });
		backHpBer_->Update();
		backHpBer_->Draw();

		hpBer_->SetPosition(object_.GetScreenPosition() + Vector2{ 0,-27.5f });
		hpBer_->SetSize({ (HP_ * 0.95f),10.0f });
		hpBer_->Update();
		hpBer_->Draw();
	}

}


void Enemy::Move() {

	// 回転と移動量の設定
	const float kMoveSpeed = 3.0f; // 移動速度
	// worldTransformBase_.rotation_.y += 0.00f; // 一定量のY軸回転

	// 向いている方向への移動ベクトルの計算
	Vector3 moveDirection = { 0.0f, 0.0f, kMoveSpeed };
	Matrix4x4 rotationMatrix = MakeRotateYMatrix(transBase_.rotate_.y);
	moveDirection = TransformNormal(moveDirection, rotationMatrix);

	// ロックオン座標
	Vector3 lockOnPosition = player_->GetObject3D().GetWorldPosition();

	// 追跡対象からロックオン対象へのベクトル
	Vector3 sub = Subtract(lockOnPosition, transBase_.translate_);

	// Y軸周り角度
	transBase_.rotate_.y = std::atan2(sub.x, sub.z);

	if (Distance(player_->GetCenterPosition(), object_.GetWorldPosition()) >= 5) {

		// 移動
		transBase_.translate_ = Add(transBase_.translate_, moveDirection * Timer());
	}
	groundRightEmit_->SetVelocityMinMax(-moveDirection * 2, -moveDirection * 2);
	groundRightEmit_->Update();
	groundLeftEmit_->SetVelocityMinMax(-moveDirection * 2, -moveDirection * 2);
	groundLeftEmit_->Update();

	
}

void Enemy::HitMotion()
{
	count += Timer();

	if (count >= 0.5f) {
		hit = false;
	}


	// 回転と移動量の設定
	const float kMoveSpeed = -45.0f; // 移動速度
	// worldTransformBase_.rotation_.y += 0.00f; // 一定量のY軸回転

	// 向いている方向への移動ベクトルの計算
	Vector3 moveDirection = { 0.0f, 0.0f, kMoveSpeed };
	Matrix4x4 rotationMatrix = MakeRotateYMatrix(transBase_.rotate_.y);
	moveDirection = TransformNormal(moveDirection, rotationMatrix);

	// ロックオン座標
	Vector3 lockOnPosition = player_->GetObject3D().GetWorldPosition();

	// 追跡対象からロックオン対象へのベクトル
	Vector3 sub = Subtract(lockOnPosition, transBase_.translate_);

	// Y軸周り角度
	//transBase_.rotate_.y = std::atan2(sub.x, sub.z);


	// 移動
	transBase_.translate_ = Add(transBase_.translate_, moveDirection * Timer());
	
}

void Enemy::Emit()
{
	dame.centar = { 0,0,0 };
	dame.rotate = { 0,0,0 };
	dame.size = { 0.5f,0.5f,0.5f };
	dame.count = 10;
	dame.lifeTime = 5;


	dame.velocity = velocity_;
	dame.renge = { -Vector3{0.1f,0.2f,0.1f},Vector3{0.1f,0.2f,0.1f} };
	dame.color = { 0,1,1,1 };
	//ParticleManager::GetInstance()->Emit("dame", "const", dame);

	starEmit_->Update();

	int dirac = rand() % 2;

	if (dirac == 0) {
		hitEmit_->SetVelocityMinMax(-Vector3{ 5,5,0 }, { 5, 5, 0 });
		traiEmit_->SetVelocityMinMax(-Vector3{ 5,5,0 }, { 5, 5, 0 });
	}
	else if (dirac == 1) {
		hitEmit_->SetVelocityMinMax(-Vector3{ 0,5,5 }, { 0, 5, 5 });
		traiEmit_->SetVelocityMinMax(-Vector3{ 0,5,5 }, { 0, 5, 5 });
	}
	else {
		hitEmit_->SetVelocityMinMax(-Vector3{ 5,0,5 }, { 5, 0, 5 });
		traiEmit_->SetVelocityMinMax(-Vector3{ 5,0,5 }, { 5, 0, 5 });
	}

	hitEmit_->Update();
	traiEmit_->Update();
}

void Enemy::OnCollision(Collider* other)
{
	// 衝突判定の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)) {
		Player* player = static_cast<Player*>(other);
		uint32_t serialNumber = player->GetSerialNumber();

		if (isAlive_) {
			if (!player->GetInvincible()) {

				// 接触履歴があれば何もせず抜ける
				if (contactRecord_.CheckHistory(serialNumber)) {
					return;
				}

				contactRecord_.AddHistory(serialNumber);

				followCamera_->GetViewProjection().SetShake(0.1f,{1.5f,1.5f,1.5f });

				player->AddDamege(10);
			}
		}
	}
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon)) {

	}
}

Vector3 Enemy::GetCenterPosition() const
{
	return object_.GetWorldPosition();
}

float Enemy::Timer() const
{
	return MyGame::GameTime() * timeSpeed_;
}

void Enemy::Shake()
{
	

	float x = oldPos_.x + float(rand() % 20 - 10);
	float xShake = float(x) / static_cast<float>(10);
	float z = oldPos_.z + float(rand() % 20 - 10);
	float zShake = float(z) / static_cast<float>(10);


	object_.worldtransform_.translate_.x = xShake;
	object_.worldtransform_.translate_.z = zShake;

}

void Enemy::InitParticle()
{


	groundRightEmit_ = std::make_unique<ParticleEmitter>();
	groundRightEmit_->Initialize("groundRight", "enemyGround", ParticleManager::EmitType::kRandom);
	groundRightEmit_->GetFrequency() = 0.15f;
	groundRightEmit_->SetCount(1);
	groundRightEmit_->SetParent(object_.worldtransform_);
	groundRightEmit_->SetPos({ -1,-0.5f,0 });
	groundRightEmit_->SetVelocityMinMax({ 0,0,0 }, { 0, 0, 0 });
	groundRightEmit_->SetLifeTimeMinMax(0.4f, 0.7f);
	groundRightEmit_->SetIsAlpha(true);
	groundRightEmit_->SetSizeMinMax(Vector3{ 0.2f,0.2f,0.2f }, { 0.4f,0.4f,0.4f });
	groundRightEmit_->SetColorMinMax({ 0.604f, 0.384f, 0.161f }, { 0.604f, 0.384f, 0.161f });

	groundLeftEmit_ = std::make_unique<ParticleEmitter>();
	groundLeftEmit_->Initialize("groundLeft", "enemyGround", ParticleManager::EmitType::kRandom);
	groundLeftEmit_->GetFrequency() = 0.15f;
	groundLeftEmit_->SetCount(1);
	groundLeftEmit_->SetParent(object_.worldtransform_);
	groundLeftEmit_->SetPos({ 1,-0.5f,0 });
	groundLeftEmit_->SetVelocityMinMax({ 0,0,0 }, { 0, 0, 0 });
	groundLeftEmit_->SetLifeTimeMinMax(0.4f, 0.7f);
	groundLeftEmit_->SetIsAlpha(true);
	groundLeftEmit_->SetSizeMinMax(Vector3{ 0.2f,0.2f,0.2f }, { 0.4f,0.4f,0.4f });
	groundLeftEmit_->SetColorMinMax({ 0.604f, 0.384f, 0.161f }, { 0.604f, 0.384f, 0.161f });

	dustEmit_ = std::make_unique<ParticleEmitter>();
	dustEmit_->Initialize("dust", "enemyGround", ParticleManager::EmitType::kRandom);
	dustEmit_->GetFrequency() = 0.25f;
	dustEmit_->SetCount(3);
	dustEmit_->SetParent(object_.worldtransform_);
	dustEmit_->SetPos({ 0,1.1f,-0.45f });
	dustEmit_->SetRengeMinMax({0,0,0},{0,0,0});
	dustEmit_->SetVelocityMinMax({ 0,2,0 }, { 0, 5, 0 });
	dustEmit_->SetLifeTimeMinMax(0.5f, 0.7f);
	dustEmit_->SetIsAlpha(true);
	dustEmit_->SetSizeMinMax(Vector3{ 0.1f,0.1f,0.1f }, { 0.2f,0.2f,0.2f });
	dustEmit_->SetColorMinMax({ 0.1f, 0.1f, 0.1f }, { 0.12f, 0.12f, 0.12f });

	starEmit_ = std::make_unique<ParticleEmitter>();
	starEmit_->Initialize("dust", "hitStar", ParticleManager::EmitType::kRandom);
	starEmit_->GetFrequency() = 0.0f;
	starEmit_->SetCount(1);
	starEmit_->SetParent(object_.worldtransform_);
	starEmit_->SetPos({ 0,0.0f,0.0f });
	starEmit_->SetRotateMinMax(-DegreesToRadians({ 180,180,180 }), DegreesToRadians({ 180,180,180 }));
	starEmit_->SetVelocityMinMax({ 0,0,0 }, { 0, 0, 0 });
	starEmit_->SetLifeTimeMinMax(0.2f, 0.2f);
	starEmit_->SetIsAlpha(true);
	starEmit_->SetUsebillboard(false);
	starEmit_->SetSizeMinMax(Vector3{ 1.6f,1.6f,1.6f }, { 1.8f,1.8f,1.8f });
	starEmit_->SetColorMinMax({ 0.424f, 0.404f, 0.431f }, { 0.424f, 0.404f, 0.431f });

	traiEmit_ = std::make_unique<ParticleEmitter>();
	traiEmit_->Initialize("dust", "hitTrai", ParticleManager::EmitType::kRandom);
	traiEmit_->GetFrequency() = 0.0f;
	traiEmit_->SetCount(5);
	traiEmit_->SetParent(object_.worldtransform_);
	traiEmit_->SetPos({ 0,0.0f,0.0f });
	traiEmit_->SetRotateMinMax(-DegreesToRadians({ 180,180,180 }), DegreesToRadians({ 180,180,180 }));
	traiEmit_->SetVelocityMinMax({ 0,0,0 }, { 0, 0, 0 });
	traiEmit_->SetLifeTimeMinMax(0.2f, 0.2f);
	traiEmit_->SetIsAlpha(true);
	traiEmit_->SetUsebillboard(false);
	traiEmit_->SetSizeMinMax(Vector3{ 1.6f,1.6f,1.6f }, { 1.8f,1.8f,1.8f });
	traiEmit_->SetColorMinMax({ 1, 0, 0 }, { 1, 1, 0 });
	traiEmit_->SetRengeMinMax(Vector3{-5,-5,-5},Vector3{5,5,5});

	hitEmit_ = std::make_unique<ParticleEmitter>();
	hitEmit_->Initialize("dust", "hit", ParticleManager::EmitType::kRandom);
	hitEmit_->GetFrequency() = 0.0f;
	hitEmit_->SetCount(10);
	hitEmit_->SetParent(object_.worldtransform_);
	hitEmit_->SetPos({ 0,0.0f,0.0f });
	hitEmit_->SetRotateMinMax(-DegreesToRadians({ 90,90,90 }), DegreesToRadians({ 90,90,90 }));
	hitEmit_->SetLifeTimeMinMax(0.5f, 0.6f);
	hitEmit_->SetIsAlpha(true);
	hitEmit_->SetUsebillboard(false);
	hitEmit_->SetSizeMinMax(Vector3{ 3.6f,3.6f,3.6f }, { 3.8f,3.8f,3.8f });
	hitEmit_->SetColorMinMax({ 1, 0, 0 }, { 1, 1, 0 });


}

void Enemy::HitStpoTime()
{
	bool is = false;
	hitStopTimer -= MyGame::GameTime();
	if (hitStopTimer <= 0.0f) {
		hitStopTimer = 0.0f;
	}
	if (hitStopTimer > 0) {
		is = true;
	}

	if (is) {

		timeSpeed_ = 0.0f;
		Shake();
	}
	else {
		object_.worldtransform_.translate_ = {0,0,0};
		timeSpeed_ = 1.0f;
	}
}
