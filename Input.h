#pragma once
#include <windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION    0x0800 // DirectInputのバージョン設定
#include <dinput.h>
//入力
class Input 
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //メンバ関数
	//初期化
	void Intialize(HINSTANCE hInstance, HWND hwnd);
	//更新
	void Update();


private: //メンバ変数
	//キーボードデバイス生成
	ComPtr<IDirectInputDevice8> keyboard;

};
