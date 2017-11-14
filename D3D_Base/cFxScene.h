#pragma once

#include "cGameNode.h"

class cGrid;
class cStamping;
class cStonPtcSplash;
class cFireAni;

class cFxScene : public cGameNode
{
private:
	cGrid* m_pGrid;
	cStamping* m_pEffect;
	cStonPtcSplash* m_pSton;
	cFireAni* m_pFire;

public:
	cFxScene();
	virtual ~cFxScene();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};