#pragma once
#include <windows.h>
#include<cstdint>



// WindowsAPI
class WinApp 
{
public: // 静的メンバ関数
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public: // メンバ関数
	// 初期化
	void Initialize();
	// 終了
	void Finalize();

	// メッセージの処理
	bool ProcessMessage();

	/// ウィンドウハンドルの取得
	HWND GetHwnd() const { return hwnd; }

	HINSTANCE GetHInstance() { return wc.hInstance; }

	const int32_t GetClientWidth() { return kClientWidth; }
	const int32_t GetClientHeight() { return kClientHeight; }
	
	//クライアント領域のサイズ
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;
	//const FLOAT GetFLOATClientWidth() { return kClientWidth; }
	//const FLOAT GetFLOATClientHeight() { return kClientHeight; }
private:
	HWND hwnd = nullptr; // ウィンドウハンドル
	WNDCLASS wc{};       // ウィンドウクラス
	
};

