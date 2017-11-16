#pragma once

#include "cBillbordEffect.h"

class cFireAni : public cBillbordEffect
{
private:
	std::vector<ST_ANI_DATA> m_vecAniData;
	int m_nFrameX;
	int m_nFrameY;
	int m_nNumMakePtc;
	float m_fDelyTime;
	float m_fCurTime;
	float m_fEffectRange;
	bool m_isMake;

public:
	cFireAni();
	virtual ~cFireAni();

	void Setup(int maxparticle, float effectrange);
	virtual void Update();
	virtual void Render();

	void ResetParticle(ST_ANI_DATA& data);
};