#pragma once
#include "cGameNode.h"

class cPlayerManager;
class cEnemyManager;
class cInventroy;

class cBattleScene : public cGameNode
{
private:
	cInventroy*			m_cInv;
	cPlayerManager*		m_cPMGR;
	cEnemyManager*		m_cEMGR;
public:
	cBattleScene();
	virtual ~cBattleScene();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();
};

