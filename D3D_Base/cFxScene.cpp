#include "stdafx.h"
#include "cFxScene.h"
#include "cGrid.h"
#include "cEffect.h"

cFxScene::cFxScene()
	: m_pGrid(NULL)
	, m_pEffect(NULL)
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

	m_pEffect = new cEffect;
	m_pEffect->Setup(100);

	return S_OK;
}

void cFxScene::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	if (m_pEffect)
		m_pEffect->Update();

}

void cFxScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pEffect)
		m_pEffect->Render();

}

void cFxScene::Destroy()
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pEffect);
}

void cFxScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
}
