#include "AttackManeger.h"
#include <imgui.h>


void AttackManeger::Initialize()
{
	workAttack.attackParameter_ = 0;
	workAttack.comboNext = 0;
}



void AttackManeger::Update(WrokAttack& work)
{
	// コンボ上限に達していない
	if (work.comboIndex < ComboNum - 1) {
		if (Input::GetInstance()->IsTriggerKey(DIK_RETURN)) {
			// コンボ有効
			work.isAttack = true;

			work.comboNext = true;

		}
	}
	else {
		work.isAttack = false;
	}



	// コンボ段階によってモーションを分岐

	switch (work.comboIndex) {
		// 0:右から反時計回り
	case 0:
		SetAttackCombo(work);
		break;
	case 1:
		SetAttackCombo(work);
		break;
	case 2:
		SetAttackCombo(work);
		break;
	}

}


void AttackManeger::DrawImGui()
{
	ImGui::Begin("attack");
	ImGui::Checkbox("workAttack.comboNext", &workAttack.comboNext);
	ImGui::Checkbox("isCombo", &isCombo);
	ImGui::InputInt("comboIndex", &workAttack.comboIndex);
	ImGui::InputInt("ComboNum", &ComboNum);
	ImGui::InputInt("attackParameter_", &workAttack.attackParameter_);
	ImGui::End();
}

void AttackManeger::SetAttackCombo(WrokAttack& work)
{
	//  既定の時間経過で通常行動に戻る
	if (++work.attackParameter_ >= work.MaxAttackParameter_) {
		// コンボ継続なら次のコンボに進む
		if (work.comboNext) {
			

			// コンボフラグをリセット
			work.comboNext = false;
			work.attackParameter_ = 0;
			work.isAttack = true;
			// 各パーツの角度などを次のコンボ用に初期化
			
			work.comboIndex++;
		}
		else {
			work.comboIndex = 0;
		}
	}

}
