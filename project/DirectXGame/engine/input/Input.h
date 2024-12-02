#pragma once
#include <windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION    0x0800 // DirectInputのバージョン設定
#include <dinput.h>

#include"DirectXGame/engine/base/WinApp.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//入力
class Input
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //メンバ関数
	static Input* GetInstance();

	//初期化
	void Intialize(WinApp* winApp);
	//更新
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	bool TriggerKey(BYTE keyNumber);


private: //メンバ変数
	//WindowsAPI
	WinApp* winApp_ = nullptr;
	//キーボードデバイス生成
	ComPtr<IDirectInputDevice8> keyboard;
	ComPtr<IDirectInput8> directInput = nullptr;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
};
