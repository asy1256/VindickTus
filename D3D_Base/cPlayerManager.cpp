#include "stdafx.h"
#include "cPlayerManager.h"
#include "cCharacter.h"

cPlayerManager::cPlayerManager()
{
}


cPlayerManager::~cPlayerManager()
{
}

void cPlayerManager::SetUp()
{

}

void cPlayerManager::Release()
{
	if (m_vPlayer.empty()) return;

	for each (auto p in m_vPlayer)
		p->Release();
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
	case PLAYER_NONE:
		break;
	case PLAYER_END:
		break;
	default:
		break;
	}
}