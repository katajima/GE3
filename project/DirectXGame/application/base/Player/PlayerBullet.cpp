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
	//object_.SetModel("AnimatedCube.gltf");
	object_.worldtransform_.translate_= position;
	object_.Update();

	
	

	// Y軸周り角度(θy)
	object_.worldtransform_.rotate_.y = std::atan2(velocity_.x, velocity_.z);
	float length = Length(Vector3(velocity_.x, 0, velocity_.z));

	// X軸周り角度(θx)
	object_.worldtransform_.rotate_.x = std::atan2(velocity_.y, -length);

	str = object_.worldtransform_.translate_;

	phase_ = 0;

	isAlive_ = true;

	randPosSky = str + float(rand() % 11 - 5);
	randPosSky.y = max_y;

	
	

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
	ParticleManager::GetInstance()->SetObject(strin, object_.worldtransform_);
	
	strin = std::to_string(index_) + "exp";
	ParticleManager::GetInstance()->SetObject(strin, object_.worldtransform_);
	
	strin2 = std::to_string(index_) + "exp2";
	ParticleManager::GetInstance()->SetObject(strin2, object_.worldtransform_);
	

	trailEffect_ = std::make_unique<TrailEffect>();
	trailEffect_->Initialize("resources/Texture/Image.png", 0.1f,{1.00f,1.00f,1.00f,0.9f});
	trailEffect_->SetCamera(camera);
	


	objectStr_.Initialize();
	objectStr_.worldtransform_.parent_ = &object_.worldtransform_;
	objectStr_.worldtransform_.translate_.y = object_.GetMesh(0)->GetMin().y * 1;

	objectEnd_.Initialize();
	objectEnd_.worldtransform_.parent_ = &object_.worldtransform_;
	objectEnd_.worldtransform_.translate_.y = object_.GetMesh(0)->GetMax().y * 1;

}

void PlayerBullet::Update()
{
	// 時間経過でデス
	if (Hit) {
		isAlive_ = false;
	}

	
	if (isAlive_) {
		countTrail++;
		Vector3 norm;

		switch (phase_)
		{
		case 0:
			count += MyGame::GameTime();

			t = count;

			object_.worldtransform_.translate_ = Lerp(str, randPosSky, t);

			norm =  randPosSky - str;
			
			velocity_ = norm * MyGame::GameTime();

			if (t >= 1) {
				t = 0;
				count = 0;
				phase_++;
			}

			break;
		case 1:
			count += MyGame::GameTime();
			if (count >= max_count)
			{
				Vector3 pos = enemy_->GetObject3D().GetWorldPosition() - object_.GetWorldPosition();

				//pos =  enemy_->GetObject3D().GetWorldPosition() - object_.GetWorldPosition();
				Vector3 pos2 = pos;

				velocity_ = pos2;
				velocity_.y = 0;

				object_.worldtransform_.translate_ += velocity_ * MyGame::GameTime();

				if (5 >= DistanceXZ(object_.GetWorldPosition(), enemy_->GetObject3D().GetWorldPosition())) {
					phase_++;
					posGround = enemy_->GetObject3D().GetWorldPosition();
					tragetPos = posGround - object_.GetWorldPosition();
					count = 0;
				}

			}
			break;
		case 2:
			count += MyGame::GameTime();
			if (count >= max_count)
			{
				velocity_ = tragetPos * 3;
				
				object_.worldtransform_.translate_ += velocity_ * MyGame::GameTime();
			}

			if (2.5f >= object_.GetWorldPosition().y) {
				isAlive_ = false;
				count = 0;
				phase_ = 0;
			}

			break;
		}
	}

	bullet.velocity = Multiply(-velocity_, 0.5f);

	if (countTrail >= 5) {
		bool flag_ =true ;
		trailEffect_->Update(flag_, objectStr_, objectEnd_);
	}


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

	
	// Y軸周り角度(θy)
	object_.worldtransform_.rotate_.y = std::atan2(velocity_.x, velocity_.z);
	float length = Length(Vector3(velocity_.x, 0, velocity_.z));

	// X軸周り角度(θx)
	object_.worldtransform_.rotate_.x = std::atan2(velocity_.y, -length);


	objectStr_.Update();
	objectEnd_.Update();
	object_.Update();
}

void PlayerBullet::Draw()
{
	object_.Draw();
}

void PlayerBullet::DrawP()
{
	trailEffect_->Draw();
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

		if (enemy->GetAlive()) {

			enemy->AddDamege(30);

			enemy->SetHit();

			enemy->Emit();

			player_->AddHit();
			player_->SetHitTime();
		}
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
