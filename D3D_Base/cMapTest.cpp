#include "stdafx.h"
#include "cMapTest.h"
#include "cCustomMap.h"
#include "cToolBar.h"

#include "cObjLoader.h"

cMapTest::cMapTest()
	: m_pMap(NULL)
	, m_pTool(NULL)
	, m_isToolOpen(false)
	, m_me(NULL)
	, m_loader(NULL)
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
	m_pMap->Setup(17, 17, 1.0f);

	m_pTool = new cToolBar;
	m_pTool->Setup();
	m_pTool->SetIsOpen(&m_isToolOpen);

	//m_loader = new cObjLoader;
	//m_loader->CreatMeshFromFile(m_me, "obj", "american_farm.obj");

	//g_pObjectManager->CreateObject(D3DXVECTOR3(1, 0, 1), "obj", "american_farm.obj");

	return S_OK;
}

void cMapTest::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	if (KEYMANAGER->isOnceKeyDown('M')) { m_isToolOpen = (m_isToolOpen) ? false : true; }

	if (m_isToolOpen)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			m_pTool->Click();
		}
	}
	else
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
		{
			Ray ray;
			ray.Origin = D3DXVECTOR3(0, 0, 0);
			D3DXVECTOR3 pos(0, 0, 0);
			POINT pt = g_ptMouse;
			pt.x += 128;
			pt.y += 50;
			GetRay(ray, pt.x, pt.y);
			m_pMap->Picking(ray, pos);

			g_pObjectManager->CreateObject(pos, "obj", "american_farm.obj", "american_farm.png");
		}
	}
}

void cMapTest::Render()
{
	if (m_pMap)
		m_pMap->Render();

	if (m_pTool)
		m_pTool->Render();


	//D3DXMATRIXA16 world;
	//D3DXMatrixIdentity(&world);
	//g_pD3DDevice->SetTexture(0, NULL);
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &world);
	//if (m_me)
	//	m_me->DrawSubset(0);

	g_pObjectManager->Render();
}

void cMapTest::Destroy()
{
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pTool);
	SAFE_RELEASE(m_me);
	SAFE_DELETE(m_loader);
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

