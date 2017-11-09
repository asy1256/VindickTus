#pragma once

#include "cGameNode.h"

class cGrid;
class cEffect;

class cFxScene : public cGameNode
{
private:
	cGrid* m_pGrid;
	cEffect* m_pEffect;

public:
	cFxScene();
	virtual ~cFxScene();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};