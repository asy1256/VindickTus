#pragma once

#include "cCamera.h"

class cGameNode
{
protected:
	cCamera* m_pCamera;

public:
	cGameNode();
	virtual ~cGameNode();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

