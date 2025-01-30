#pragma once
#include<string>
#include <memory>


class Player;

class BasePlayerState {
public:
	BasePlayerState(const std::string& name, Player* player) :name_(name), player_(player){};

	virtual void Update() = 0;


protected:
	// 状態名
	std::string name_;

	// 操作状態
	Player* player_ = nullptr;

	
};


class PlayerStateMove : public BasePlayerState
{
public:
	// コンストラクタ
	PlayerStateMove(Player* player);

	void Update();
};


class PlayerStateAttack :public BasePlayerState
{
public:
	PlayerStateAttack(Player* player);

	void Update();
};





