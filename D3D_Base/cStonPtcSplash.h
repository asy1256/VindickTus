#pragma once

#include "cPtSEffect.h"

class cStonPtcSplash : public cPtSEffect
{

public:
	cStonPtcSplash();
	virtual ~cStonPtcSplash();

	void Setup(int maxparticle);
	virtual void Update();
	virtual void Render();
};