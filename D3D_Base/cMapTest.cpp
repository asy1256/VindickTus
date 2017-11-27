#include "stdafx.h"
#include "cMapTest.h"
#include "cCustomMap.h"
#include "cToolBar.h"
#include "cGrid.h"

cMapTest::cMapTest()
	: m_pMap(NULL)
	, m_pTool(NULL)
	, m_pGrid(NULL)
	, m_isToolOpen(false)
	, m_nNowSelect(10)
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
	m_pMap->Setup(129, 129, 2.0f);

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pTool = new cToolBar;
	m_pTool->Setup();
	m_pTool->SetIsOpen(&m_isToolOpen);
	m_pTool->SetNowSelect(&m_nNowSelect);

	return S_OK;
}

void cMapTest::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	if (KEYMANAGER->isOnceKeyDown('M')) { m_isToolOpen = (m_isToolOpen) ? false : true; }
	
	if (KEYMANAGER->isOnceKeyDown('S')) { g_pObjectManager->SaveObject(); };
	if (KEYMANAGER->isOnceKeyDown('L')) { g_pObjectManager->LoadObject(); };

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

			MakeObject(pos);
		}
	}
}

void cMapTest::Render()
{
	if (m_pMap)
		m_pMap->Render();

	if (m_pTool)
		m_pTool->Render();

	if (m_pGrid)
		m_pGrid->Render();

	g_pObjectManager->Render();
}

void cMapTest::Destroy()
{
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pTool);
	SAFE_DELETE(m_pGrid);
}

void cMapTest::MakeObject(D3DXVECTOR3& pos)
{
	switch (m_nNowSelect)
	{
	case 0:
		g_pObjectManager->CreateObject(pos, "obj", "LageRock01.obj", "T_Plains_Rock01_D.tga", m_nNowSelect);
		break;
	case 1:
		g_pObjectManager->CreateObject(pos, "obj", "LageRock02.obj", "T_Plains_Rock01_D.tga", m_nNowSelect);
		break;
	case 2:
		g_pObjectManager->CreateObject(pos, "obj", "LageRock_Cliff01.obj", "T_Plains_Cliff01.tga", m_nNowSelect);
		break;
	case 3:
		g_pObjectManager->CreateObject(pos, "obj", "LageRock_Cliffside01.obj", "T_FlameDungeon_Cliffside01_D.tga", m_nNowSelect);
		break;
	case 4:
		g_pObjectManager->CreateObject(pos, "obj", "LageRock_FireRock01.obj", "T_Env_Fire_Cliffs_TilingStone_02_D.tga", m_nNowSelect);
		break;
	case 5:
		g_pObjectManager->CreateObject(pos, "obj", "Env_Fire_MelancholicTroll_D.obj", "Env_Fire_MelancholicTroll_D.tga", m_nNowSelect);
		break;
	case 6:
		g_pObjectManager->CreateObject(pos, "obj", "LageRock_FireHeed.obj", "Env_Fire_NordHeed_D.tga", m_nNowSelect);
		break;
	case 7:
		g_pObjectManager->CreateObject(pos, "obj", "SM_Plains_Column_Small_01.obj", "T_Plains_Ruins_Pillars_02_D.tga", m_nNowSelect);
		break;
	case 8:
		g_pObjectManager->CreateObject(pos, "obj", "SM_Plains_Column_Mid_01.obj", "T_Plains_Ruins_Pillars_03_D.tga", m_nNowSelect);
		break;
	case 9:
		g_pObjectManager->CreateObject(pos, "obj", "SM_FlameDungeon_ArchWall01.obj", "T_FlameDungeon_Wall01_D.tga", m_nNowSelect);
		break;
	default:
		break;
	}
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

