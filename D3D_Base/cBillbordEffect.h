#pragma once

#include "cEffect.h"

struct ST_ANI_DATA
{
	int nCurFrameX;
	int nCurFrameY;
	int nFrameCount;
	float fLifeTime;
	float fAge;
	bool isAlive;
	ST_PT_VERTEX vertex[4];
	D3DXVECTOR3 vPos;
	ST_ANI_DATA()
		: nCurFrameX(0)
		, nCurFrameY(0)
		, nFrameCount(0)
		, fLifeTime(0.f)
		, fAge(0.f)
		, isAlive(false)
		, vPos(0, 0, 0)
	{}
};

class cBillbordEffect : public cEffect
{
protected:
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9	m_pVb;
	LPDIRECT3DINDEXBUFFER9 m_pIb;

public:
	cBillbordEffect();
	virtual ~cBillbordEffect();

	virtual void Update();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
};