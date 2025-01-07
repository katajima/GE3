#include "Player.h"
#include "Enemy/Enemy.h"
void Player::Initialize(Vector3 position, Camera* camera)
{
	Collider::Initialize(camera);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));

	camera_ = camera;

	specialAttack.max = 40;

	// プレイヤー
	objectBase_.Initialize();
	objectBase_.SetCamera(camera_);
	objectBase_.transform.translate = position;
	objectBase_.Update();

	// レティクル
	objectReticle_.Initialize();
	objectReticle_.SetCamera(camera_);
	objectReticle_.SetModel("enemy.obj");
	objectReticle_.parent_ = &objectBase_;
	objectReticle_.transform.translate = { 0,0,30 };


	objectBody_.Initialize();
	objectBody_.SetCamera(camera_);
	objectBody_.SetModel("AnimatedCube.gltf");
	objectBody_.parent_ = &objectBase_;
	objectBody_.model->modelData.material[0]->shininess_ = 10000;

	objectSha_.Initialize();
	objectSha_.SetCamera(camera_);
	objectSha_.SetModel("plane.obj");
	objectSha_.model->modelData.material[0]->tex_.diffuseFilePath = "resources/Texture/aa.png";
	objectSha_.model->modelData.material[0]->color = { 0.9f,0.0f,0.0f,1 };
	objectSha_.transform.translate = position;
	objectSha_.transform.scale = { 4,4,4 };
	objectSha_.transform.rotate.x = DegreesToRadians(-90);


	weapon_ = std::make_unique<playerWeapon>();
	weapon_->Initialize(camera);
	weapon_->GetObject3D().parent_ = &objectBase_;
	weapon_->GetObject3D().transform.translate = { 0,0.5f,0.5f };
	weapon_->SetOffset({ 0,5.0f,0.5f });
	weapon_->SetPlayer(this);
	//weapon_->SetPlayer()

	//particleManager_ = ParticleManager::GetInstance();
	ParticleManager::GetInstance()->CreateParticleGroup("dust", "resources/Texture/uvChecker.png", ModelManager::GetInstance()->FindModel("plane.obj"), camera_);
	ParticleManager::GetInstance()->SetPos("dust", { 0,0,0 });
	ParticleManager::GetInstance()->SetObject("dust", objectBase_);

	
	emitter_ = new ParticleEmitter("cc", Transform{ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0,0,0) }, 100, 1.0f, 5.0f);


	

	
	HpBer_ = std::make_unique<Sprite>();
	HpBer_->Initialize("resources/Texture/Image.png");
	HpBer_->SetSize({ 50,-float(hp) });
	HpBer_->SetColor({ 0,1,0,1 });
	HpBer_->SetPosition({ 100,650 });

}

void Player::Update()
{

	Gravity();

	if (isAlive) {
		if (behaviorRequest_) {
			// ふるまいを変更する
			behavior_ = behaviorRequest_.value();
			// 各ふるまいごとの初期化を実行
			switch (behavior_) {
			case Behavior::kRoot:
			default:
				BehaviorRootInitialize();
				isInvincible = false;
				break;
			case Behavior::kAttack:
				BehaviorAttackInitialize();
				isInvincible = true;
				break;
			case Behavior::kJump:
				break;
			case Behavior::kDie:
				BehaviorDieInitialize();
				break;
			}
			// ふるまいリクエストリセット
			behaviorRequest_ = std::nullopt;
		}
		switch (behavior_) {
		case Behavior::kRoot: // 通常行動更新
		default:
			BehaviorRootUpdate();
			break;
		case Behavior::kAttack: // 攻撃行動更新
			BehaviorAttackUpdate();
			break;
		case Behavior::kJump:
			break;
		case Behavior::kDie:
			BehaviorDieUpdate();
			break;
		}
	}
	if (specialAttack.specialGauge >= specialAttack.max) {
		specialAttack.isSpecial = true;
	}
	else {
		specialAttack.isSpecial = false;
	}


#ifdef _DEBUG
	if (isInvincible) {
		objectBody_.model->modelData.material[0]->color = { 0,0,1,1 };
	}
	else {
		objectBody_.model->modelData.material[0]->color = { 1,1,1,1 };
	}

#endif // _DEBUG


	if (objectBase_.transform.translate.x > 200) {
		objectBase_.transform.translate.x = 200;
	}
	if (objectBase_.transform.translate.x < -200) {
		objectBase_.transform.translate.x = -200;
	}
	if (objectBase_.transform.translate.z > 200) {
		objectBase_.transform.translate.z = 200;
	}
	if (objectBase_.transform.translate.z < -200) {
		objectBase_.transform.translate.z = -200;
	}

	if (hp <= 0) {
		isAlive = false;
	}
	workAttack.hitTime--;
	if (workAttack.hitTime <= 0) {
		workAttack.hitCount = 0;
	}


	// 影
	objectSha_.transform.translate = objectBase_.transform.translate;
	objectSha_.transform.translate.y = 0.1f;

	Vector3 scale{};
	scale = std::abs((std::min)((objectSha_.transform.translate.y + 9.0f / objectBase_.transform.translate.y), 6.0f));

	objectSha_.transform.scale = scale;


	//emitter_->Update();

	objectBase_.Update();
	objectBody_.Update();
	weapon_->Update();
	objectReticle_.Update();
	objectSha_.Update();

	for (const auto& bullet : playerBullet_) {
		bullet->Update();
	}

	playerBullet_.remove_if([](const std::unique_ptr<PlayerBullet>& bullet) { return !bullet->GetAlive(); });
}

void Player::Draw()
{
	if (isAlive) {
		switch (behavior_) {
		case Behavior::kRoot: // 通常行動更新
		default:
			break;
		case Behavior::kAttack: // 攻撃行動更新
			weapon_->Draw();
			break;
		case Behavior::kJump:
			break;
		case Behavior::kDie:
			break;
		}


		objectBody_.Draw();
		objectSha_.Draw();
	}

	for (const auto& bullet : playerBullet_) {
		bullet->Draw();
	}
}

void Player::DrawP()
{

	for (const auto& bullet : playerBullet_) {
	//	bullet->DrawP();
	}
	//ParticleManager::GetInstance()->GetInstance()->Draw();
	
}

void Player::Draw2D()
{
	HpBer_->SetSize({ 50,-float(hp) * 2 });
	HpBer_->Update();
	HpBer_->Draw();
}

void Player::OnCollision(Collider* other)
{
	// 衝突判定の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
	}
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyWeapon)) {
	}
}

Vector3 Player::GetCenterPosition() const
{
	return objectBase_.GetWorldPosition();
}


void Player::Move()
{
	speed = 0.3f;
	velocity_ = { 0,0,0 };
	isMove = false;



	if (Input::GetInstance()->IsControllerConnected()) {


		velocity_.x = Input::GetInstance()->GetGamePadLeftStick().x;
		velocity_.z = Input::GetInstance()->GetGamePadLeftStick().y;


		if (velocity_.x != 0.0f || velocity_.z != 0.0f) {
			isMove = true;
			// 入力方向を正規化
			velocity_ = Normalize(velocity_);
			velocity_ = Multiply(velocity_, speed);


			// カメラのビュー行列の逆行列（カメラのワールド変換行列）を取得
			Matrix4x4 cameraWorldMatrix = Inverse(camera_->viewMatrix_);

			// カメラの向きに基づいて移動方向をワールド座標系に変換
			Vector3 worldDirection = {
				velocity_.x * cameraWorldMatrix.m[0][0] + velocity_.z * cameraWorldMatrix.m[2][0],
				0.0f,
				velocity_.x * cameraWorldMatrix.m[0][2] + velocity_.z * cameraWorldMatrix.m[2][2]
			};

			velocity_ = Multiply(Normalize(worldDirection), speed);

			//// 移動ベクトルをカメラの角度だけ回転する
			//Matrix4x4 rotateMatrixY = MakeRotateYMatrix(camera_->transform_.rotate.y);
			//velocity_ = TransformNormal(velocity_, rotateMatrixY);
			//
			if (velocity_.Length() != 0) {
				objectBase_.transform.rotate.y = std::atan2(velocity_.x, velocity_.z);
			}


		}
	}
	else {


		if (Input::GetInstance()->IsPushKey(DIK_W)) {
			velocity_.z += 0.3f;
		}
		if (Input::GetInstance()->IsPushKey(DIK_S)) {
			velocity_.z -= 0.3f;

		}
		if (Input::GetInstance()->IsPushKey(DIK_A)) {
			velocity_.x -= 0.3f;

		}
		if (Input::GetInstance()->IsPushKey(DIK_D)) {
			velocity_.x += 0.3f;
		}


		if (velocity_.x != 0.0f || velocity_.z != 0.0f) {
			// 入力方向を正規化
			velocity_ = Normalize(velocity_);
			isMove = true;

			// カメラのビュー行列の逆行列（カメラのワールド変換行列）を取得
			Matrix4x4 cameraWorldMatrix = Inverse(camera_->viewMatrix_);

			// カメラの向きに基づいて移動方向をワールド座標系に変換
			Vector3 worldDirection = {
				velocity_.x * cameraWorldMatrix.m[0][0] + velocity_.z * cameraWorldMatrix.m[2][0],
				0.0f,
				velocity_.x * cameraWorldMatrix.m[0][2] + velocity_.z * cameraWorldMatrix.m[2][2]
			};

			velocity_ = Multiply(Normalize(worldDirection), speed);


		}
	}

	if (behavior_ == Behavior::kRoot || behavior_ == Behavior::kDie)
		objectBase_.transform.translate = Add(objectBase_.transform.translate, velocity_);

	if (isMove) {
		ParticleManager::Constant cons{};
		cons.centar = { 0,-0.5f,0 };
		cons.rotate = { 0,0,0 };
		cons.size = { 0.3f,0.3f,0.3f };
		cons.count = 2;
		cons.lifeTime = 1.0f;
		cons.velocity = Multiply(-velocity_, 10);
		cons.color = { 0.824f, 0.706f, 0.549f,0.5f };
		cons.renge = { -Vector3{1.4f,0.1f,1.4f},Vector3{1.4f,0.1f,1.4f} };
		ParticleManager::GetInstance()->Emit("cc", "const", cons);
	}
	
}

void Player::Gravity()
{
	// 移動
	objectBase_.transform.translate.y += graVelo;
	// 重力加速度
	const float kGravityAcceleration = 0.05f;
	// 加速度ベクトル
	float accelerationVector = -kGravityAcceleration;
	// 加速する
	graVelo += accelerationVector;
	// 着地
	if (objectBase_.transform.translate.y <= groundY) {
		objectBase_.transform.translate.y = groundY;
		graVelo = 0;
		isJamp = false;
	}
}

void Player::LockOn(std::vector<std::unique_ptr<Enemy>>& enemys)
{

	if (behavior_ == Behavior::kDie) {
		if (specialAttack.phese == 0)
		{
			// ロックオン処理のリセットと更新
			lockedOnEnemies.clear();
			int i = 0;
			bool is = true;

			/*while (true)
			{

			}*/
			for (int j = 0; j < enemys.size(); j++) {	
				if (i >= MaxLockOn) {
					is = false;
					//break; // 最大ロックオン数を超えた場合
				}
				
				Vector2 posEne = enemys[j]->GetObject3D().GetScreenPosition();
				Vector2 diff = Vector2{ 640,360 } - posEne;
				float length = diff.Length();

				if (length <= 300.0f && enemys[j]->GetAlive() && is) {
					enemys[j]->SetLockOn(true);
					lockedOnEnemies.push_back(enemys[j].get());
					i++;
				}
				else {
					enemys[j]->SetLockOn(false);
				}
			}
		}

	}
	else {
		for (int j = 0; j < enemys.size(); j++) {
			enemys[j]->SetLockOn(false);
		}

	}
}



void Player::ApplyGlobalVariables()
{

}






