#pragma once

#include "cGameNode.h"
class cCustomMap;

class cMapTest : public cGameNode
{
private:
	cCustomMap* m_pMap;

public:
	cMapTest();
	virtual ~cMapTest();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void GetRay(Ray& ray, int x, int y);
};