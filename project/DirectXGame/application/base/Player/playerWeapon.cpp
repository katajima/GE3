#include "playerWeapon.h"
#include "Enemy/Enemy.h"
#include "Player.h"

void playerWeapon::Initialize(Camera* camera)
{
	Collider::Initialize(camera);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));

	

	objectWeapon_.Initialize();
	objectWeapon_.SetCamera(camera);
	objectWeapon_.SetModel("Sword.obj");
	
}

void playerWeapon::Update()
{
	objectWeapon_.Update();
}

void playerWeapon::Draw()
{
	objectWeapon_.Draw();
}

void playerWeapon::OnCollision(Collider* other)
{
	// 衝突判定の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// 衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		Enemy* enemy = static_cast<Enemy*>(other);
		uint32_t serialNumber = enemy->GetSerialNumber();
		


		// 接触履歴があれば何もせず抜ける
		if (contactRecord_.CheckHistory(serialNumber)) {
			return;
		}
		
		if (player_->GetAttackType() == Player::AttackType::kJamp && player_->GetObject3D().GetWorldPosition().y <= 2) {

			contactRecord_.AddHistory(serialNumber);
			if (enemy->GetAlive()) {
				enemy->AddDamege(10);

				enemy->SetHit();


				enemy->Emit();

				enemy->hitStop(0.2f);


				player_->AddHit();
				player_->AddSP();
				player_->SetHitTime();
			}
		}
		else if (player_->GetAttackType() != Player::AttackType::kJamp) {
			contactRecord_.AddHistory(serialNumber);

			if (enemy->GetAlive()) {
				enemy->AddDamege(10);

				enemy->SetHit();
				enemy->Emit();

				enemy->hitStop(0.1f);

				player_->AddHit();
				player_->AddSP();
				player_->SetHitTime();
			}
		}
	}
}

Vector3 playerWeapon::GetCenterPosition() const
{
	const Vector3 ofset = offset;
	// ワールド座標に変換
	Vector3 worldPos = Transforms(ofset, objectWeapon_.worldtransform_.worldMat_);

	return worldPos;
}

void playerWeapon::ContactRecordClear()
{
	contactRecord_.Clear();
}

void playerWeapon::SetPlayer(Player* player)
{
	{ player_ = player; }
}
