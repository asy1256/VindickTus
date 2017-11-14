#pragma once

#include "cBillbordEffect.h"

class cFireAni : public cBillbordEffect
{
private:
	int m_nFrameX;
	int m_nFrameY;
	int m_nCurFrameX;
	int m_nCurFrameY;

	int m_nTimeCount;

public:
	cFireAni();
	virtual ~cFireAni();

	void Setup();
	virtual void Update();
	virtual void Render();
};