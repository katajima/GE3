#include"Input.h"

#include <cassert>


Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Intialize(WinApp* winApp)
{
	HRESULT result;

	// 借りてきたWinAppのインスタンスを記録
	this->winApp_ = winApp;

	//　DirectInputのインスタンス生成
	result = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//排他制御レベルセット
	result = keyboard->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));


	// マウス入力の初期化
	result = directInput->CreateDevice(GUID_SysMouse, &mouseDevice_, NULL);
	assert(SUCCEEDED(result));

	result = mouseDevice_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	result = mouseDevice_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

}

void Input::Update()
{
	HRESULT result;

	memcpy(keyPre, key, sizeof(key));
	//キーボード情報の取得開始
	keyboard->Acquire();
	//　全キーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(key), key);

	preMouse_ = mouse_;


	preXInputState_ = xInputState_;
	XInputGetState(0, &xInputState_);

	result = mouseDevice_->Acquire();
	result = mouseDevice_->GetDeviceState(sizeof(DIMOUSESTATE), &mouse_);

}

bool Input::IsPushKey(BYTE keyNumber) const
{
	// 指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}

	return false;
}

bool Input::IsTriggerKey(BYTE keyNumber) const
{
	// 指定されたキーが押された場合にtrueを返す
	if (key[keyNumber] && !keyPre[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::IsKeyReleased(uint8_t _key) const
{
	if (!key[_key] && keyPre[_key])
		return true;

	return false;
}

bool Input::IsMouseTriggered(uint8_t _buttonNum) const
{
	// マウスがトリガー
	if (mouse_.rgbButtons[_buttonNum] && !preMouse_.rgbButtons[_buttonNum]) {
		return true;
	}
	return false;
}

bool Input::IsMousePressed(uint8_t _buttonNum) const
{
	if (mouse_.rgbButtons[_buttonNum] && preMouse_.rgbButtons[_buttonNum]) {
		return true;
	}
	return false;
}

bool Input::IsMouseReleased(uint8_t _buttonNum) const
{
	if (!mouse_.rgbButtons[_buttonNum] && preMouse_.rgbButtons[_buttonNum])
		return true;
	return false;
}

/// <summary>
/// マウスの位置を取得
/// </summary>
/// <returns></returns>
Vector2 Input::GetMousePosition() const
{
	POINT mousePos;
	GetCursorPos(&mousePos);

	ScreenToClient(winApp_->GetHwnd(), &mousePos);

	Vector2 result;
	result.x = static_cast<float>(mousePos.x);
	result.y = static_cast<float>(mousePos.y);

	return result;
}
