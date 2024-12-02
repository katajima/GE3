#include<Windows.h>
#include"winuser.h"

#include"DirectXGame/engine/MyGame/MyGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakchecker leakCheck;
	

	Framework* game = new MyGame();

	game->Run();

	delete game;
	
	return 0;
}
