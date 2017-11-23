#pragma once

#include "cGameNode.h"
class cCustomMap;
class cToolBar;
class cObjLoader;

class cMapTest : public cGameNode
{
private:
	cCustomMap* m_pMap;
	cToolBar* m_pTool;

	LPD3DXMESH m_me;
	cObjLoader* m_loader;

	bool m_isToolOpen;

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