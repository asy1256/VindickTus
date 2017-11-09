#include "stdafx.h"
#include "cEnemyManager.h"
#include "cDragon.h"


cEnemyManager::cEnemyManager()
{
}

cEnemyManager::~cEnemyManager()
{
}

void cEnemyManager::SetUp()
{
	AddEnemy(ENEMY_DRAGON);
}

void cEnemyManager::Release()
{
	if (m_vEnemies.empty()) return;

	for each (auto e in m_vEnemies)
		SAFE_DELETE(e);

	m_vEnemies.clear();
}

void cEnemyManager::Update()
{
	if (m_vEnemies.empty()) return;

	for each (auto e in m_vEnemies)
		e->Update();
}

void cEnemyManager::Render()
{
	if (m_vEnemies.empty()) return;

	for each (auto e in m_vEnemies)
		e->Render();
}

void cEnemyManager::AddEnemy(Enemykey E)
{
	switch (E)
	{
		case ENEMY_DRAGON:
		{
			cDragon* E1 = new cDragon;
			E1->Setup();

			m_vEnemies.push_back(E1);
		}break;

		case ENEMY_END:
		{

		}break;
	}
}
