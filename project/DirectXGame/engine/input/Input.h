#pragma once
#include <windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <xinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"XInput.lib")

#include"DirectXGame/engine/base/WinApp.h"

#include "DirectXGame/engine/struct/Structs.h"


// ゲームパッド
enum class GamePadButton
{
	GAMEPAD_Up = XINPUT_GAMEPAD_DPAD_UP,			// 十字(上)
	GAMEPAD_Down = XINPUT_GAMEPAD_DPAD_DOWN,		// 十字(下)
	GAMEPAD_Left = XINPUT_GAMEPAD_DPAD_LEFT,		// 十字(左)
	GAMEPAD_Right = XINPUT_GAMEPAD_DPAD_RIGHT,		// 十字(右)
	GAMEPAD_A = XINPUT_GAMEPAD_A,					// A
	GAMEPAD_B = XINPUT_GAMEPAD_B,					// B
	GAMEPAD_X = XINPUT_GAMEPAD_X,					// X
	GAMEPAD_Y = XINPUT_GAMEPAD_Y,					// Y
	GAMEPAD_LB = XINPUT_GAMEPAD_LEFT_SHOULDER,		// LB
	GAMEPAD_RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,		// RB
	GAMEPAD_LT = XINPUT_GAMEPAD_LEFT_THUMB,			// LT
	GAMEPAD_RT = XINPUT_GAMEPAD_RIGHT_THUMB,		// RT
	GAMEPAD_Start = XINPUT_GAMEPAD_START,			// Start
	GAMEPAD_Back = XINPUT_GAMEPAD_BACK,			// Back
	GAMEPAD_Max       // 最大ボタン数
};
//XINPUT_GAMEPAD_LEFT_THUMB
//XINPUT_GAMEPAD_RIGHT_THUMB
//XINPUT_GAMEPAD_LEFT_SHOULDER
//XINPUT_GAMEPAD_RIGHT_SHOULDER

//入力
class Input
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct FlagXYZ
	{
		bool x;
		bool y;
		bool z;
	};

public: //メンバ関数
	static Input* GetInstance();

	//初期化
	void Intialize(WinApp* winApp);
	//更新
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	bool IsPushKey(BYTE keyNumber) const;

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	bool IsTriggerKey(BYTE keyNumber) const;

	/// <summary>
	/// プレキー
	/// </summary>
	bool IsKeyReleased(uint8_t _key) const;


	/// <summary>
	///  マウス
	/// </summary>
	/// <param name="_buttonNum"></param>
	/// <returns></returns>
	bool IsMouseTriggered(uint8_t _buttonNum) const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_buttonNum"></param>
	/// <returns></returns>
	bool IsMousePressed(uint8_t _buttonNum) const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_buttonNum"></param>
	/// <returns></returns>
	bool IsMouseReleased(uint8_t _buttonNum) const;


	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Vector2 GetMousePosition() const;


	// ゲームパッド
	bool IsGamePadTriggered(GamePadButton button) const;
	bool IsGamePadPressed(GamePadButton button) const;
	bool IsGamePadReleased(GamePadButton button) const;

	// パッドの左スティックの値を取得
	Vector2 GetGamePadLeftStick() const;
	// パッドの右スティックの値を取得
	Vector2 GetGamePadRightStick() const;

	// コントローラ操作
	bool IsControllerConnected() {
		XINPUT_STATE state; ZeroMemory(&state, sizeof(XINPUT_STATE));
		// コントローラの状態を取得
		DWORD result = XInputGetState(0, &state);
		// コントローラが接続されている場合は true を返す
		return (result == ERROR_SUCCESS);
	}




private: //メンバ変数
	//WindowsAPI
	WinApp* winApp_ = nullptr;
	//キーボードデバイス生成
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	Microsoft::WRL::ComPtr <IDirectInputDevice8> mouseDevice_ = nullptr;

	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	DIMOUSESTATE mouse_ = {};
	DIMOUSESTATE preMouse_ = {};

	_XINPUT_STATE xInputState_ = {};
	_XINPUT_STATE preXInputState_ = {};
	float currentVibrateTime_ = 0.0f;
	float vibrateTimeMax_ = 0.0f;
	float deadZone_ = 0.1f;
	bool enambleVibrate_ = false;


};
