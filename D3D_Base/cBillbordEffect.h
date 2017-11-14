#pragma once

#include "cEffect.h"

struct ST_ANI_DATA
{

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