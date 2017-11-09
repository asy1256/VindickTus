#pragma once

class cEnemyBasic;
class cDragon;

enum Enemykey
{
	ENEMY_DRAGON = 0,
	ENEMY_END
};

class cEnemyManager
{
protected:
	SYNTHESIZE_REF(vector<cEnemyBasic*>, m_vEnemies, vEnemies);

public:
	cEnemyManager();
	~cEnemyManager();

	void SetUp();
	void Release();
	void Update();
	void Render();

	void AddEnemy(Enemykey E);
};

