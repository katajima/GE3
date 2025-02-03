#include "Enemy.h"
#include "imgui.h"
#include "Player/Player.h"

uint32_t Enemy::nextSerialNumber = 0;

Enemy::Enemy() {
	// シリアル番号を振る
	serialNumber = nextSerialNumber;
	// 次の番号を1加算
	++nextSerialNumber;
}

void Enemy::Initialize(Vector3 position, float HP,Camera*camera)
{
	Collider::Initialize(camera);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));


	object_.Initialize();
	object_.SetModel("enemy2.obj");
	object_.SetCamera(camera);
	object_.worldtransform_.translate_ = position;
	object_.worldtransform_.scale_ = { 2,2,2 };
	


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


	ParticleManager::GetInstance()->CreateParticleGroup("dame", "resources/Texture/aa.png", ModelManager::GetInstance()->FindModel("plane.obj"), camera);
	ParticleManager::GetInstance()->SetPos("dame", { 0,0,0 });
	ParticleManager::GetInstance()->SetObject("dame", object_.worldtransform_);


	icon_lockOn = std::make_unique<Sprite>();
	icon_lockOn->Initialize("resources/Texture/icon/LockOnW.png");
	icon_lockOn->SetSize(0.10f);
	icon_lockOn->SetColor({ 1,0,1,1 });
	icon_lockOn->SetPosition({ -100,650 });
	icon_lockOn->SetAnchorPoint({ 0.5f,0.5f });

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

	if (HP_ <= 0) {
		isAlive_ = false;
	}

	if (isAlive_) {
		if (!hit) {
			count = 0;
			Move();
		}
		else {

			HitMotion();
		}
		// 影
		objectSha_.worldtransform_.translate_ = object_.worldtransform_.translate_;
		objectSha_.worldtransform_.translate_.y = 0.1f;

		Vector3 scale{};
		scale = 7;

		objectSha_.worldtransform_.scale_ = scale;
	}

	//emitter_->Update();
	object_.Update();
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
}


void Enemy::Move() {

	// 回転と移動量の設定
	const float kMoveSpeed = 3.0f; // 移動速度
	// worldTransformBase_.rotation_.y += 0.00f; // 一定量のY軸回転

	// 向いている方向への移動ベクトルの計算
	Vector3 moveDirection = { 0.0f, 0.0f, kMoveSpeed };
	Matrix4x4 rotationMatrix = MakeRotateYMatrix(object_.worldtransform_.rotate_.y);
	moveDirection = TransformNormal(moveDirection, rotationMatrix);

	// ロックオン座標
	Vector3 lockOnPosition = player_->GetObject3D().GetWorldPosition();

	// 追跡対象からロックオン対象へのベクトル
	Vector3 sub = Subtract(lockOnPosition, object_.worldtransform_.translate_);

	// Y軸周り角度
	object_.worldtransform_.rotate_.y = std::atan2(sub.x, sub.z);

	if (Distance(player_->GetCenterPosition(), object_.GetWorldPosition()) >= 5) {

		// 移動
		object_.worldtransform_.translate_ = Add(object_.worldtransform_.translate_, moveDirection * MyGame::GameTime());
	}
}

void Enemy::HitMotion()
{
	count++;

	if (count >= 30) {
		hit = false;
	}
	

	// 回転と移動量の設定
	const float kMoveSpeed = -1.5f; // 移動速度
	// worldTransformBase_.rotation_.y += 0.00f; // 一定量のY軸回転

	// 向いている方向への移動ベクトルの計算
	Vector3 moveDirection = { 0.0f, 0.0f, kMoveSpeed };
	Matrix4x4 rotationMatrix = MakeRotateYMatrix(object_.worldtransform_.rotate_.y);
	moveDirection = TransformNormal(moveDirection, rotationMatrix);

	// ロックオン座標
	Vector3 lockOnPosition = player_->GetObject3D().GetWorldPosition();

	// 追跡対象からロックオン対象へのベクトル
	Vector3 sub = Subtract(lockOnPosition, object_.worldtransform_.translate_);

	// Y軸周り角度
	object_.worldtransform_.rotate_.y = std::atan2(sub.x, sub.z);

	//if (Distance(player_->GetCenterPosition(), object_.GetWorldPosition()) >= 10) {

		// 移動
		object_.worldtransform_.translate_ = Add(object_.worldtransform_.translate_, moveDirection);
	//}
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
