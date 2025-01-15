#include"Player.h"

// 攻撃更新
void Player::Attack()
{
	workAttack.parameter++;
	float t = static_cast<float>(workAttack.parameter) / workAttack.attackAll.max_t;
	
	float k = 0.5f;
	switch (workAttack.type)
	{
	case AttackType::kNormal:
		if (workAttack.parameter >= 5) {
			flag33 = true;
		}
		if (workAttack.comboIndex == 0) {
			if (workAttack.parameter >= 1) {
				k = 0.1f;
				weapon_->GetObject3D().transform.rotate.x += DegreesToRadians(8);
				
			}
		}if (workAttack.comboIndex == 1) {
			if (workAttack.parameter >= 1) {
				k = 0.3f;
				weapon_->GetObject3D().transform.rotate.x += DegreesToRadians(8);
			}
		}if (workAttack.comboIndex == 2) {
			if (workAttack.parameter >= 1) {
				k = 0.4f;
				weapon_->GetObject3D().transform.rotate.x += DegreesToRadians(16);
			}
		}if (workAttack.comboIndex == 3) {
			if (workAttack.parameter >= 1) {
				k = 1.5f;
				weapon_->GetObject3D().transform.rotate.x += DegreesToRadians(16);
			}
		}


		if (workAttack.parameter <= 5) {
			
			Vector3 move(0, 0, k);
			// 速度ベクトルを自機の向きに合わせて回転させる
			move = TransformNormal(move, objectBase_.mat_);

			objectBase_.transform.translate += move;
		}
		break;
	case AttackType::kJamp:
		if (objectBase_.transform.translate.y > groundY) {
			workAttack.attackAll.max_t++;
		}
		if (workAttack.parameter >= 5) {
			flag33 = true;
		}



		if (graVelo <= 0) {
			ty += 4;
			float r_t = ty / (float)workAttack.attackAll.max_t;
			weapon_->GetObject3D().transform.rotate.x = Easing::EaseOut(DegreesToRadians(30), DegreesToRadians(100), r_t);
		
		}
		else {
			objectBase_.transform.translate = workAttack.velocity + objectBase_.transform.translate;
		}

		
		

		break;
	case AttackType::kDash:
		if (workAttack.parameter >= 5) {
			flag33 = true;
		}


		
		objectBase_.transform.translate = Easing::EaseOut(workAttack.pos.str, workAttack.pos.end, t);

		break;
	case AttackType::kSpecial:

		break;
	}
}

// 攻撃キー入力
void Player::AttackKey()
{
	if (Input::GetInstance()->IsControllerConnected()) {
		workAttack.key.IsNormalAttack = Input::GetInstance()->IsGamePadTriggered(GamePadButton::GAMEPAD_B);
		workAttack.key.IsDashAttack = Input::GetInstance()->IsGamePadTriggered(GamePadButton::GAMEPAD_X);
		workAttack.key.IsJampAttack = Input::GetInstance()->IsGamePadTriggered(GamePadButton::GAMEPAD_Y);
		if (specialAttack.isSpecial) {
			workAttack.key.IsSpecialAttack = Input::GetInstance()->IsGamePadTriggered(GamePadButton::GAMEPAD_RB);
		}


		if (workAttack.key.IsAttack = workAttack.key.IsNormalAttack ||
			workAttack.key.IsDashAttack || workAttack.key.IsJampAttack) {
			workAttack.key.IsAttack = true;

		}

	}
	else {
		workAttack.key.IsNormalAttack = Input::GetInstance()->IsTriggerKey(DIK_SPACE);

		if (workAttack.key.IsAttack = workAttack.key.IsNormalAttack ||
			workAttack.key.IsDashAttack || workAttack.key.IsJampAttack) {
			workAttack.key.IsAttack = true;
		}
	}

}

// コンボするのか
void Player::SetAttackCombo(WrokAttack& work)
{
	//  既定の時間経過で通常行動に戻る
	if (++work.attackAll.t >= work.attackAll.max_t) {
		// コンボ継続なら次のコンボに進む
		if (work.comboNext) {

			work.attackAll.t = 0;

			workAttack.parameter = 0;
			// 各パーツの角度などを次のコンボ用に初期化

			work.comboIndex++;

			// 方向
			Move();
			// 攻撃タイプ
			AttackType();
			// 攻撃タイプによって初期化
			AttackTypeInit(work.comboIndex);
			

			// コンボフラグをリセット
			work.comboNext = false;
			work.key.IsAttack = false;
			flag33 = false;
		}
		else {
			flag33 = false;
			behaviorRequest_ = Behavior::kRoot;
		}
	}
	else {
		// コンボ上限に達していない
		if (workAttack.comboIndex < ComboNum - 1) {
			if (workAttack.key.IsAttack) {
				// 攻撃ボタンをトリガーしたら
					// コンボ有効
				workAttack.comboNext = true;


			}
		}

	}

}

// 攻撃位置初期化
void Player::AttackTypeInit(int comboIndex)
{
	objectBase_.Update();
	objectReticle_.Update();

	workAttack.pos.str = objectBase_.GetWorldPosition();
	
	weapon_->ContactRecordClear();
	// スピード
	const float k = 0.5f;

	if (workAttack.typeRequest_) {
		// ふるまいを変更する
		workAttack.type = workAttack.typeRequest_.value();
		// 各ふるまいごとの初期化を実行
		switch (workAttack.type)
		{
		case AttackType::kNormal:
			slash.color = { 1,0,0,1 };

			
			if (comboIndex == 0) {
				weapon_->SetRad(2.5f);
				workAttack.attackAll.max_t = 20;
				weapon_->GetObject3D().transform.rotate = DegreesToRadians({ 0,0,0 });
			}
			if (comboIndex == 1) {
				weapon_->SetRad(2.5f);
				workAttack.attackAll.max_t = 20;
				weapon_->GetObject3D().transform.rotate = DegreesToRadians({ 0,0,90 });
			}
			if (comboIndex == 2) {
				weapon_->SetRad(3.5f);
				workAttack.attackAll.max_t = 20;
				weapon_->GetObject3D().transform.rotate = DegreesToRadians({ 0,0,-90 });
			}
			if (comboIndex == 3) {
				weapon_->SetRad(4.5f);
				workAttack.attackAll.max_t = 20;
				weapon_->GetObject3D().transform.rotate = DegreesToRadians({ 0,0,90 });
			}
			break;
		case AttackType::kJamp:
			slash.color = { 0,1,0,1 };


			weapon_->SetRad(5.5f);

			//if (comboIndex == 0) {
				workAttack.attackAll.max_t = 20;
				ty = 0;
				graVelo = 1;
				weapon_->GetObject3D().transform.rotate = DegreesToRadians({ 20,0,0 });
				Vector3 move(0, 0, k);
				// 速度ベクトルを自機の向きに合わせて回転させる
				move = TransformNormal(move, objectBase_.mat_);
				workAttack.velocity = move;
			//}
			if (comboIndex == 1) {
				// 建築予定
				//ヒットした敵を上にあげ五回程度切り込み


			}
			if (comboIndex == 2) {
			//	behaviorRequest_ = Behavior::kRoot;
			}
			
			break;
		case AttackType::kDash:
			slash.color = { 0,0,1,1 };

			weapon_->SetRad(3.0f);

			workAttack.attackAll.max_t = 20;
			weapon_->GetObject3D().transform.rotate = DegreesToRadians({89,0,0 });

			workAttack.pos.end = objectReticle_.GetWorldPosition();

			break;
		case AttackType::kSpecial:

			break;
		}
		// ふるまいリクエストリセット
		workAttack.typeRequest_ = std::nullopt;
	}
}

// 攻撃タイプ判別
void Player::AttackTypes()
{
	if (workAttack.key.IsAttack) {
		if (workAttack.key.IsNormalAttack) {
			workAttack.typeRequest_ = AttackType::kNormal;
		}
		else if (workAttack.key.IsDashAttack) {
			workAttack.typeRequest_ = AttackType::kDash;
		}
		else if (workAttack.key.IsJampAttack) {
			workAttack.typeRequest_ = AttackType::kJamp;
		}
	}
}
