#include "stdafx.h"
#include "cFxScene.h"
#include "cGrid.h"
#include "cStamping.h"
#include "cStonPtcSplash.h"
#include "cFireAni.h"

cFxScene::cFxScene()
	: m_pGrid(NULL)
	, m_pEffect(NULL)
	, m_pSton(NULL)
	, m_pFire(NULL)
{
}

cFxScene::~cFxScene()
{
	Destroy();
}

HRESULT cFxScene::Setup()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	//m_pEffect = new cStamping;
	//m_pEffect->Setup(90, 5.0f);
	//
	//m_pSton = new cStonPtcSplash;
	//m_pSton->Setup(30);

	m_pFire = new cFireAni;
	m_pFire->Setup();

	return S_OK;
}

void cFxScene::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	if (m_pEffect)
		m_pEffect->Update();

	if (m_pSton)
		m_pSton->Update();

	if (m_pFire)
		m_pFire->Update();

}

void cFxScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pEffect)
		m_pEffect->Render();

	if (m_pSton)
		m_pSton->Render();

	if (m_pFire)
		m_pFire->Render();

}

void cFxScene::Destroy()
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pEffect);
	SAFE_DELETE(m_pSton);
	SAFE_DELETE(m_pFire);
}

void cFxScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}