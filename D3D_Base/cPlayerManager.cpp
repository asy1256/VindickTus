#include "stdafx.h"
#include "cPlayerManager.h"
#include "cBerserker.h"

cPlayerManager::cPlayerManager()
{
}


cPlayerManager::~cPlayerManager()
{
}

void cPlayerManager::SetUp()
{
	AddPlay(PLAYER_BERSERKER);
}

void cPlayerManager::Release()
{
	if (m_vPlayer.empty()) return;

	for each (auto p in m_vPlayer)
		SAFE_DELETE(p);

	m_vPlayer.clear();
}

void cPlayerManager::Update()
{
	if (m_vPlayer.empty()) return;

	for each (auto p in m_vPlayer)
		p->Update();
		
}

void cPlayerManager::Render()
{
	if (m_vPlayer.empty()) return;

	for each (auto p in m_vPlayer)
		p->Render();
}

void cPlayerManager::AddPlay(Plyerkey p)
{
	switch (p)
	{
		case PLAYER_BERSERKER:
		{
			cBerserker* p1 = new cBerserker;
			p1->Setup();

			m_vPlayer.push_back(p1);
		}break;

		case PLAYER_END:
		{

		}break;
	}
}