#include "stdafx.h"
#include "cMapTest.h"
#include "cCustomMap.h"
#include "cToolBar.h"
#include "cGrid.h"
#include "cFreeCam.h"

cMapTest::cMapTest()
	: m_pMap(NULL)
	, m_pTool(NULL)
	, m_pGrid(NULL)
	, m_pCam(NULL)
	, m_isToolOpen(false)
	, m_nNowSelect(10)
	, m_nTargetObj(0)
{
}

cMapTest::~cMapTest()
{
	Destroy();
}

HRESULT cMapTest::Setup()
{
	//m_pCamera = new cCamera;
	//m_pCamera->Setup();

	m_pCam = new cFreeCam;
	m_pCam->Setup();

	m_pMap = new cCustomMap;
	m_pMap->Setup(129, 129, 1.5f);

	m_pGrid = new cGrid;
	m_pGrid->Setup(30, 1.0f);

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

	if (m_pCam)
		m_pCam->Update();

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
		if (KEYMANAGER->isStayKeyDown(VK_CONTROL))
		{
			if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
			{
				if (KEYMANAGER->isStayKeyDown('W')) { m_pCam->MoveLocalZ(0.4f); }
				if (KEYMANAGER->isStayKeyDown('A')) { m_pCam->MoveLocalX(-0.4f); }
				if (KEYMANAGER->isStayKeyDown('S')) { m_pCam->MoveLocalZ(-0.4f); }
				if (KEYMANAGER->isStayKeyDown('D')) { m_pCam->MoveLocalX(0.4f); }
			}
		}
		else
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				Ray ray;
				ray.Origin = D3DXVECTOR3(0, 0, 0);
				D3DXVECTOR3 pos(0, 0, 0);
				POINT pt = g_ptMouse;
				pt.x += 128;
				pt.y += 50;
				GetRay(ray, pt.x, pt.y);

				D3DXVECTOR3 vMax(0, 0, 0);
				D3DXVECTOR3 vMin(0, 0, 0);
				D3DXMATRIXA16 matR;

				for (int i = 0; i < g_pObjectManager->GetObjectData().size(); ++i)
				{
					D3DXMatrixRotationY(&matR, g_pObjectManager->GetObjectData()[i].rotAxis.y);
					vMax = g_pObjectManager->GetObjectData()[i].localPos + g_pObjectManager->GetObjectData()[i].minmax->vMax;
					vMin = g_pObjectManager->GetObjectData()[i].localPos + g_pObjectManager->GetObjectData()[i].minmax->vMin;
					if (IntersectBox(&ray, vMax, vMin))
					{
						m_nTargetObj = i;
						break;
					}
				}
			}
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

	if (!g_pObjectManager->GetObjectData().empty())
	{
		if (KEYMANAGER->isStayKeyDown('J')) //-x 방향으로 이동
		{
			g_pObjectManager->GetObjectData()[m_nTargetObj].localPos.x -= 0.1f;
		}
		if (KEYMANAGER->isStayKeyDown('L')) //+x 방향으로 이동
		{
			g_pObjectManager->GetObjectData()[m_nTargetObj].localPos.x += 0.1f;
		}
		if (KEYMANAGER->isStayKeyDown('I')) //+z 방향으로 이동
		{
			g_pObjectManager->GetObjectData()[m_nTargetObj].localPos.z += 0.1f;
		}
		if (KEYMANAGER->isStayKeyDown('K')) //-z 방향으로 이동
		{
			g_pObjectManager->GetObjectData()[m_nTargetObj].localPos.z -= 0.1f;
		}
		if (KEYMANAGER->isStayKeyDown('Y')) //+y 방향으로 이동
		{
			g_pObjectManager->GetObjectData()[m_nTargetObj].localPos.y += 0.1f;
		}
		if (KEYMANAGER->isStayKeyDown('H')) //-y 방향으로 이동
		{
			g_pObjectManager->GetObjectData()[m_nTargetObj].localPos.y -= 0.1f;
		}
		if (KEYMANAGER->isStayKeyDown('U')) //왼쪽으로 돌려
		{
			g_pObjectManager->GetObjectData()[m_nTargetObj].rotAxis.y -= 0.1f;
		}
		if (KEYMANAGER->isStayKeyDown('O')) //오른쪽으로 돌려
		{
			g_pObjectManager->GetObjectData()[m_nTargetObj].rotAxis.y += 0.1f;
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
	SAFE_DELETE(m_pCam);
}

bool cMapTest::IntersectBox(Ray* ray, D3DXVECTOR3 & max, D3DXVECTOR3 & min)
{
	D3DXVECTOR3 v0, v1, v2;

	v0 = D3DXVECTOR3(min.x, max.y, max.z);
	v1 = D3DXVECTOR3(max.x, max.y, max.z);
	v2 = D3DXVECTOR3(min.x, max.y, min.z);

	if (D3DXIntersectTri(&v0, &v1, &v2, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v0.x = max.x;
	v0.z = min.z;
	if (D3DXIntersectTri(&v0, &v2, &v1, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v2.x = max.x;
	v2.y = min.y;
	if (D3DXIntersectTri(&v0, &v1, &v2, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v0.y = min.y;
	v0.z = max.z;
	if (D3DXIntersectTri(&v0, &v2, &v1, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v2.x = min.x;
	v2.z = max.z;
	if (D3DXIntersectTri(&v0, &v1, &v2, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v0.x = min.x;
	v0.y = max.y;
	if (D3DXIntersectTri(&v0, &v2, &v1, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v1.x = min.x;
	v1.z = min.z;
	if (D3DXIntersectTri(&v0, &v1, &v2, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v0.y = min.y;
	v0.z = min.z;
	if (D3DXIntersectTri(&v0, &v2, &v1, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v2.x = max.x;
	v2.z = min.z;
	if (D3DXIntersectTri(&v0, &v1, &v2, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v0.x = max.x;
	v0.y = max.y;
	if (D3DXIntersectTri(&v0, &v2, &v1, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	// 6면체 (밑면)
	v0.y = min.y;
	v0.z = max.z;
	v1.z = max.z;
	v1.y = min.y;
	if (D3DXIntersectTri(&v0, &v2, &v1, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	v0.x = min.x;
	v0.z = min.z;
	if (D3DXIntersectTri(&v0, &v1, &v2, &ray->Origin, &ray->Direction, NULL, NULL, NULL)) return true;

	return false;
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

	if (m_pCam)
		m_pCam->WndProc(hWnd, message, wParam, lParam);
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

