#pragma once

#include "cPtSEffect.h"

class cBreath : public cPtSEffect
{
private:


public:
	cBreath();
	virtual ~cBreath();

	void Setup(int maxparticle);
	virtual void Update();
	virtual void Render();
};