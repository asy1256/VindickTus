#pragma once

#include "cPtSEffect.h"

class cStamping : public cPtSEffect
{
private:
	float m_fLength;

public:
	cStamping();
	virtual ~cStamping();

	void Setup(int maxparticle, float lange);
	virtual void Update();
	virtual void Render();
};