#include "PlayerBullet.h"
#include "Enemy/Enemy.h"
#include "Player.h"

void PlayerBullet::Initialize(Vector3 position, Camera* camera)
{
	Collider::Initialize(camera);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));
	Collider::SetRadius(3.0f);
	//モデル

	// プレイヤー
	object_.Initialize();
	object_.SetCamera(camera);
	object_.SetModel("player_bullet.obj");
	object_.transform.translate = position;
	object_.Update();

	
	

	// Y軸周り角度(θy)
	object_.transform.rotate.y = std::atan2(velocity_.x, velocity_.z);
	float length = Length(Vector3(velocity_.x, 0, velocity_.z));

	// X軸周り角度(θx)
	object_.transform.rotate.x = std::atan2(velocity_.y, -length);

	str = object_.transform.translate;

	phase_ = 0;

	isAlive_ = true;

	randPosSky = str + float(rand() % 11 - 5);
	randPosSky.y = max_y;

	
	
	

	emitter_ = new ParticleEmitter(strin, Transform{ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0,0,0) }, 100, 1.0f, 5.0f);


	bullet.centar = { 0,0.0,0 };
	bullet.rotate = { 0,0,0 };
	bullet.size = { 1.3f,1.3f,1.3f };
	bullet.count = 1;
	bullet.lifeTime = 2;
	bullet.velocity = Multiply(-velocity_, 1);
	bullet.color = { 0.3f, 0.3f, 0.3f,0.5f };
	bullet.renge = { -Vector3{0.2f,0.2f,0.2f},Vector3{0.2f,0.2f,0.2f} };

	exp.centar = { 0,0.0,0 };
	exp.rotate = { 0,0,0 };
	exp.size = { 3.3f,3.3f,3.3f };
	exp.count = 100;
	exp.lifeTime = 5;
	exp.velocity = Multiply(-velocity_, 1);
	exp.color = { 0.5f, 0.5f, 0.5f,1.0f };
	exp.renge = { -Vector3{1.2f,1.2f,1.2f},Vector3{1.2f,1.2f,1.2f} };



	strin = std::to_string(index_) + "bullet";
	ParticleManager::GetInstance()->SetObject(strin, object_);
	emitter_ = new ParticleEmitter(strin, Transform{ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0,0,0) }, 100, 1.0f, 5.0f);
	
	strin = std::to_string(index_) + "exp";
	ParticleManager::GetInstance()->SetObject(strin, object_);
	
	strin2 = std::to_string(index_) + "exp2";
	ParticleManager::GetInstance()->SetObject(strin2, object_);
	//emitter_ = new ParticleEmitter(strin, Transform{ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0,0,0) }, 100, 1.0f, 5.0f);

}

void PlayerBullet::Update()
{
	// 時間経過でデス
	if (Hit) {
		isAlive_ = false;
	}

	
	if (isAlive_) {
		switch (phase_)
		{
		case 0:
			count++;

			t = float(count)/ float(60);

			object_.transform.translate = Lerp(str, randPosSky, t);

			Vector3 norm =  randPosSky - str;
			
			velocity_ = norm.Normalize();

			if (t >= 1) {
				t = 0;
				count = 0;
				phase_++;
			}

			break;
		case 1:
			count++;
			if (count >= 30)
			{
				Vector3 pos = enemy_->GetObject3D().GetWorldPosition();

				pos =  enemy_->GetObject3D().GetWorldPosition() - object_.GetWorldPosition();
				Vector3 pos2 = pos;

				velocity_ = pos2;
				velocity_.y = 0;

				object_.transform.translate += velocity_.Normalize();

				if (5 >= DistanceXZ(object_.GetWorldPosition(), enemy_->GetObject3D().GetWorldPosition())) {
					phase_++;
					posGround = enemy_->GetObject3D().GetWorldPosition();
					count = 0;
				}

			}
			break;
		case 2:
			count++;
			if (count >= 30)
			{


				Vector3 pos =  posGround - object_.GetWorldPosition();
				pos.Normalize();

				velocity_ = pos;
				
				object_.transform.translate += velocity_.Normalize();;
			}

			if (2 >= object_.GetWorldPosition().y) {
				isAlive_ = false;
				count = 0;
				phase_ = 0;
			}

			break;
		}
	}

	bullet.velocity = Multiply(-velocity_, 0.5f);



	
	ParticleManager::GetInstance()->Emit(strin, "const", bullet);
	
	if (!isAlive_) {
		exp.velocity = { 0.0f,0.0f,0.0f };
		exp.velocityRenge = { {-6,0,-6},{6,6,6} };
		exp.count = 100;
		exp.lifeTime = 5;
		exp.color = { 0.5f, 0.5f, 0.5f,1.0f };
		
		ParticleManager::GetInstance()->Emit(strin, "const2", exp);
	
		exp.count = 30;
		exp.lifeTime = 4;
		exp.color = { 0.2f, 0.2f, 0.2f,1.0f };
		ParticleManager::GetInstance()->Emit(strin2, "const2", exp);
	}

	//座標を移動させる
	//object_.transform.translate += velocity_;

	// Y軸周り角度(θy)
	object_.transform.rotate.y = std::atan2(velocity_.x, velocity_.z);
	float length = Length(Vector3(velocity_.x, 0, velocity_.z));

	// X軸周り角度(θx)
	object_.transform.rotate.x = std::atan2(velocity_.y, -length);


	//
	//emitter_->Update();
	object_.Update();
}

void PlayerBullet::Draw()
{
	object_.Draw();
}

void PlayerBullet::DrawP()
{
	//ParticleManager::GetInstance()->GetInstance()->Draw();
}

void PlayerBullet::OnCollision(Collider* other)
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

		
		contactRecord_.AddHistory(serialNumber);

		enemy->AddDamege(10);

		enemy->SetHit();

		enemy->Emit();

		player_->AddHit();
		player_->SetHitTime();

		
	}
	
}

Vector3 PlayerBullet::GetCenterPosition() const
{
	return  object_.GetWorldPosition();
}

void PlayerBullet::SetPlayer(Player* player)
{
	player_ = player;
}
