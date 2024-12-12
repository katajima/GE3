#include "BasePlayerState.h"
#include"Player.h"

#include"DirectXGame/engine/input/Input.h"


void PlayerStateMove::Update()
{
	player_->Move();

	if (Input::GetInstance()->IsTriggerKey(DIK_RETURN)) {

		player_->SetIsWorkAttak();
		player_->ChangeState(std::make_unique<PlayerStateAttack>(player_));

	}
}

void PlayerStateAttack::Update()
{
	player_->Attack();
}

PlayerStateMove::PlayerStateMove(Player* player) : BasePlayerState("State Move", player) {}

PlayerStateAttack::PlayerStateAttack(Player* player) : BasePlayerState("State Attack", player) {}


