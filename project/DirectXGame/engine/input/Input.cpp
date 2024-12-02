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


}

void Input::Update()
{
	//HRESULT result;

	memcpy(keyPre, key, sizeof(key));
	//キーボード情報の取得開始
	keyboard->Acquire();
	//　全キーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(key), key);


}

bool Input::PushKey(BYTE keyNumber)
{
	// 指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}

	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 指定されたキーが押された場合にtrueを返す
	if (key[keyNumber] && !keyPre[keyNumber]) {
		return true;
	}
	return false;
}
