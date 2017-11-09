#pragma once
#include "cCharacter.h"

class cBerserker : public cCharacter
{
private:

public:
	cBerserker();
	virtual ~cBerserker();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	void Control();
};

