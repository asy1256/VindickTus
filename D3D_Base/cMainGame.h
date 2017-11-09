#pragma once

#include "cFxScene.h"

class cMainGame
{
private:

public:
	cMainGame();
	virtual ~cMainGame();

	void SetLight();

	void Setup();
	void Update();
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

