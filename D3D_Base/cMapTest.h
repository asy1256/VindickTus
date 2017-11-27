#pragma once

#include "cGameNode.h"
class cCustomMap;
class cToolBar;
class cGrid;
class cFreeCam;

class cMapTest : public cGameNode
{
private:
	cCustomMap* m_pMap;
	cToolBar* m_pTool;
	cGrid* m_pGrid;
	cFreeCam* m_pCam;

	bool m_isToolOpen;
	int m_nNowSelect;

public:
	cMapTest();
	virtual ~cMapTest();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	void MakeObject(D3DXVECTOR3& pos);

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void GetRay(Ray& ray, int x, int y);
};