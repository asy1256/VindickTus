#pragma once
#include "cCharacter.h"

class cEnemyBasic : public cCharacter
{
protected:

public:
	cEnemyBasic();
	virtual~cEnemyBasic();

	virtual void Setup();
	virtual void Update();
	virtual void Render();
};

