#include "stdafx.h"
#include "cMapTest.h"
#include "cCustomMap.h"

cMapTest::cMapTest()
{
}

cMapTest::~cMapTest()
{
	Destroy();
}

HRESULT cMapTest::Setup()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pMap = new cCustomMap;
	m_pMap->Setup(3, 3, 1.0f);

	return S_OK;
}

void cMapTest::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		Ray ray;
		ray.Origin = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 pos(0, 0, 0);
		GetRay(ray, g_ptMouse.x, g_ptMouse.y);
		m_pMap->Picking(ray, pos);
	}
}

void cMapTest::Render()
{
	if (m_pMap)
		m_pMap->Render();
}

void cMapTest::Destroy()
{
	SAFE_DELETE(m_pMap);
}

void cMapTest::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}

void cMapTest::GetRay(Ray& ray, int x, int y)
{
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 proj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);

	ray.Direction.x = (((2.0f * x) / vp.Width) - 1.0f) / proj(0, 0);
	ray.Direction.y = (((-2.0f * y) / vp.Height) + 1.0f) / proj(1, 1);
	ray.Direction.z = 1.0f;

	D3DXMATRIXA16 matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);
	D3DXVec3TransformCoord(&ray.Origin, &ray.Origin, &matInvView);
	D3DXVec3TransformNormal(&ray.Direction, &ray.Direction, &matInvView);
	D3DXVec3Normalize(&ray.Direction, &ray.Direction);
}

