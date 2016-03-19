#include <Windows.h>

#include "Game.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	Game* game = new Game();

	game->run();

	delete game;

	return 0;
}