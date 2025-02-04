#include "Player.h"
#include "Enemy/Enemy.h"

#include "assert.h"

void Player::Initialize(Vector3 position, Camera* camera)
{
	Collider::Initialize(camera);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));

	camera_ = camera;

	specialAttack.max = 40;

	// プレイヤー
	objectBase_.Initialize();
	objectBase_.SetCamera(camera_);
	objectBase_.worldtransform_.translate_ = position;
	objectBase_.Update();

	// レティクル
	objectReticle_.Initialize();
	objectReticle_.SetCamera(camera_);
	objectReticle_.SetModel("enemy.obj");
	objectReticle_.worldtransform_.parent_ = &objectBase_.worldtransform_;
	objectReticle_.worldtransform_.translate_ = { 0,0,30 };

	//assert(objectBody_ != nullptr);
	//assert(objectBase_ != nullptr);

	// 体
	objectBody_.Initialize();
	objectBody_.SetCamera(camera_);
	objectBody_.SetModel("AnimatedCube.gltf");
	objectBody_.worldtransform_.parent_ = &objectBase_.worldtransform_;
	objectBody_.model->modelData.material[0]->shininess_ = 10000;



	// 左ミサイル発射口
	injectionLeftObj_.Initialize();
	injectionLeftObj_.SetCamera(camera_);
	injectionLeftObj_.SetModel("AnimatedCube.gltf");
	injectionLeftObj_.worldtransform_.parent_ = &objectBase_.worldtransform_;
	injectionLeftObj_.worldtransform_.translate_ = { -2.5f,1.0f,-1.5f };

	injectionLeftObj_.worldtransform_.scale_= { 0.75f,1.25f,1.0f };

	// 右ミサイル発射口
	injectionRightObj_.Initialize();
	injectionRightObj_.SetCamera(camera_);
	injectionRightObj_.SetModel("AnimatedCube.gltf");
	injectionRightObj_.worldtransform_.parent_ = &objectBase_.worldtransform_;
	injectionRightObj_.worldtransform_.translate_ = { 2.5,1.0f,-1.5f };

	injectionRightObj_.worldtransform_.scale_ = { 0.75f,1.25f,1.0f };




	// 影
	objectSha_.Initialize();
	objectSha_.SetCamera(camera_);
	objectSha_.SetModel("plane.obj");
	objectSha_.model->modelData.material[0]->tex_.diffuseFilePath = "resources/Texture/aa.png";
	objectSha_.model->modelData.material[0]->color = { 0.9f,0.0f,0.0f,1 };
	objectSha_.worldtransform_.translate_ = position;
	objectSha_.worldtransform_.scale_ = { 4,4,4 };
	objectSha_.worldtransform_.rotate_.x = DegreesToRadians(-90);


	weapon_ = std::make_unique<playerWeapon>();
	weapon_->Initialize(camera);
	weapon_->GetObject3D().worldtransform_.parent_ = &objectBase_.worldtransform_;
	weapon_->GetObject3D().worldtransform_.translate_ = { 0,0.5f,0.5f };
	weapon_->SetOffset({ 0,5.0f,0.5f });
	weapon_->SetPlayer(this);
	

	weaponStr.Initialize();
	weaponStr.worldtransform_.parent_ = &weapon_->GetObject3D().worldtransform_;
	weaponStr.worldtransform_.translate_ = {0,weapon_->GetObject3D().GetMesh(0)->GetMax().y ,0};
		

	weaponEnd.Initialize();
	weaponEnd.worldtransform_.parent_ = &weapon_->GetObject3D().worldtransform_;
	weaponEnd.worldtransform_.translate_ = { 0,weapon_->GetObject3D().GetMesh(0)->GetMin().y ,0 };
	weaponEnd.worldtransform_.translate_ = { 0,2 ,0 };

	//particleManager_ = ParticleManager::GetInstance();
	ParticleManager::GetInstance()->CreateParticleGroup("dust", "resources/Texture/uvChecker.png", ModelManager::GetInstance()->FindModel("plane.obj"), camera_);
	ParticleManager::GetInstance()->SetPos("dust", { 0,0,0 });
	ParticleManager::GetInstance()->SetObject("dust", objectBase_.worldtransform_);

	
	

	

	
	HpBer_ = std::make_unique<Sprite>();
	HpBer_->Initialize("resources/Texture/Image.png");
	HpBer_->SetSize({ 50,-float(hp) });
	HpBer_->SetColor({ 0,1,0,1 });
	HpBer_->SetPosition({ 100,650 });

	SpecailBer_ = std::make_unique<Sprite>();
	SpecailBer_->Initialize("resources/Texture/Image.png");
	SpecailBer_->SetSize({ 50,-float(specialAttack.specialGauge) });
	SpecailBer_->SetColor({ 0,0,1,1 });
	SpecailBer_->SetPosition({ 40,650 });

	textMax_ = std::make_unique<Sprite>();
	textMax_->Initialize("resources/Texture/text/max.png");
	//textMax_->SetSize({ 50,-float(specialAttack.specialGauge) });
	textMax_->SetColor({ 1,0,0,1 });
	textMax_->SetPosition({ 45,350 });
	textMax_->SetRotation(DegreesToRadians(-30));
	textMax_->SetAnchorPoint({0.5f,0.5f});
	textMax_->SetSize(0.25f);

	textRB_ = std::make_unique<Sprite>();
	textRB_->Initialize("resources/Texture/icon/RB.png");
	textRB_->SetColor({ 1,1,1,1 });
	textRB_->SetPosition({ 1280 /2,550 });
	textRB_->SetAnchorPoint({0.5f,0.5f});
	textRB_->SetSize(0.02f);


	trailEffect_ = std::make_unique<TrailEffect>();
	"resources/Texture/uvChecker.png";
	trailEffect_->Initialize("resources/Texture/uvChecker.png",0.2f, Color{1,0,0,0.5f});
	trailEffect_->SetCamera(camera);
	trailEffect_->SetObject(&weapon_->GetObject3D());
	
	flag33 = false;
}

void Player::Update()
{
	trailEffect_->Update(flag33, weaponStr, weaponEnd);

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
		specialAttack.specialGauge = specialAttack.max;
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


	if (objectBase_.worldtransform_.translate_.x > 200) {
		objectBase_.worldtransform_.translate_.x = 200;
	}
	if (objectBase_.worldtransform_.translate_.x < -200) {
		objectBase_.worldtransform_.translate_.x = -200;
	}
	if (objectBase_.worldtransform_.translate_.z > 200) {
		objectBase_.worldtransform_.translate_.z = 200;
	}
	if (objectBase_.worldtransform_.translate_.z < -200) {
		objectBase_.worldtransform_.translate_.z = -200;
	}

	if (hp <= 0) {
		isAlive = false;
	}
	workAttack.hitTime  -= MyGame::GameTime();
	if (workAttack.hitTime <= 0) {
		workAttack.hitCount = 0;
	}


	// 影
	objectSha_.worldtransform_.translate_ = objectBase_.worldtransform_.translate_;
	objectSha_.worldtransform_.translate_.y = 0.1f;

	Vector3 scale{};
	scale = std::abs((std::min)((objectSha_.worldtransform_.translate_.y + 9.0f / objectBase_.worldtransform_.translate_.y), 6.0f));

	objectSha_.worldtransform_.scale_ = scale;

#ifdef _DEBUG
	ImGui::Begin("trail");
	Vector3 min = weapon_->GetObject3D().GetMesh(0)->GetMin();
	ImGui::InputFloat3("min", &min.x);
	Vector3 max = weapon_->GetObject3D().GetMesh(0)->GetMax();
	ImGui::InputFloat3("max", &max.x);
	
	Vector3 str =weaponStr.GetWorldPosition();
	ImGui::InputFloat3("str", &str.x);
	str =weaponStr.GetPreWorldPosition();
	ImGui::InputFloat3("strpre", &str.x);


	Vector3 end = weaponEnd.GetWorldPosition();
	ImGui::InputFloat3("end", &end.x);
	end = weaponEnd.GetPreWorldPosition();
	ImGui::InputFloat3("endpre", &end.x);

	ImGui::Checkbox("frag", &flag33);
	int ii = (int)trailEffect_->mesh->vertices.size();
	ImGui::InputInt("vertice", &ii);
	//for (int i = 0; i < trailEffect_->mesh->vertices.size(); i++) {
	//	//char* str =  std::to_string(i);

	//	ImGui::InputFloat3("pos", &trailEffect_->mesh->vertices[i].position.x);
	//	ImGui::InputFloat2("tex", &trailEffect_->mesh->vertices[i].texcoord.x);
	//}
	
	ImGui::End();
#endif // _DEBUG


	Gravity();
	
	objectBase_.Update();
	objectBody_.Update();
	weapon_->Update();

	injectionLeftObj_.Update();
	injectionRightObj_.Update();

	weaponStr.Update();
	weaponEnd.Update();

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
			injectionLeftObj_.Draw();
			injectionRightObj_.Draw();
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

	trailEffect_->Draw();
	for (const auto& bullet : playerBullet_) {
		bullet->DrawP();
	}
}

void Player::Draw2D()
{
	HpBer_->SetSize({ 50,-float(hp) * 2 });
	HpBer_->Update();
	HpBer_->Draw();

	SpecailBer_->SetSize({ 50,-float(specialAttack.specialGauge) * 2 * 2.5f });
	SpecailBer_->Update();
	SpecailBer_->Draw();

	if (specialAttack.isSpecial) {
		
		
		textMax_->Update();
		textMax_->Draw();
	};

	if (isTextRB_) {
		textRB_->Update();
		textRB_->Draw();
	}

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
	speed = 20.0f;
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
				objectBase_.worldtransform_.rotate_.y = std::atan2(velocity_.x, velocity_.z);
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
	if (behavior_ == Behavior::kRoot || behavior_ == Behavior::kDie) {
		
	}
	if (isMove) {
		ParticleManager::Constant cons{};
		cons.centar = { 0,-0.5f,0 };
		cons.rotate = { 0,0,0 };
		cons.size = { 0.3f,0.3f,0.3f };
		cons.count = 2;
		cons.lifeTime = 0.3f;
		cons.velocity = -velocity_;// Multiply(-velocity_, 10);
		cons.color = { 0.824f, 0.706f, 0.549f,0.5f };
		cons.renge = { -Vector3{1.4f,0.1f,1.4f},Vector3{1.4f,0.1f,1.4f} };
		ParticleManager::GetInstance()->Emit("cc", "const", cons);
	}
	
}

void Player::Gravity() {
	// 移動
	//velocity_.y = graVelo;

	// 重力加速度
	const float kGravityAcceleration = 4.4f;

	// 加速度ベクトル
	float accelerationVector = -kGravityAcceleration; // 毎フレームのデルタ時間で重力を適用

	// 加速する
	velocity_.y += accelerationVector;

	AddMove();
	// 着地
	if (objectBase_.worldtransform_.translate_.y <= groundY) {
		objectBase_.worldtransform_.translate_.y = groundY;
		graVelo = 0;
		isJamp = false;
	}
}

void Player::AddMove()
{
	if(isAlive)
	objectBase_.worldtransform_.translate_ += velocity_ * MyGame::GameTime();
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






