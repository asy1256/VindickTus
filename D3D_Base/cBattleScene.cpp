#include "stdafx.h"
#include "cBattleScene.h"
#include "cPlayerManager.h"
#include "cEnemyManager.h"
#include "cInventroy.h"


cBattleScene::cBattleScene()
	: m_cInv(NULL)
	, m_cPMGR(NULL)
{
}


cBattleScene::~cBattleScene()
{
	SAFE_RELEASE(m_cPMGR);
	SAFE_RELEASE(m_cEMGR);
	SAFE_DELETE(m_cInv);
}

HRESULT cBattleScene::Setup()
{
	m_cInv = new cInventroy;
	m_cPMGR = new cPlayerManager;
	m_cEMGR = new cEnemyManager;
	
	m_cInv->SetUp();
	m_cPMGR->SetUp();
	m_cEMGR->SetUp();

	return S_OK;
}

void cBattleScene::Update()
{
	if (m_cInv)
		m_cInv->Update();

	if (m_cPMGR)
		m_cPMGR->Update();

	if (m_cEMGR)
		m_cEMGR->Update();
}

void cBattleScene::Render()
{
	if (m_cInv)
		m_cInv->Render();

	if (m_cPMGR)
		m_cPMGR->Render();
	if (m_cEMGR)
		m_cEMGR->Render();
}
