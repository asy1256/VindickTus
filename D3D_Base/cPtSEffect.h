#pragma once

#include "cEffect.h"

class cPtSEffect : public cEffect
{
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pEffect;
	std::list<ST_SPPT_DATA> m_listParticle;
	std::vector<ST_EFX_WORK*> m_vecWork;
	std::vector<LPDIRECT3DTEXTURE9> m_vecTexture;
	LPDIRECT3DTEXTURE9 m_pTexture;

	int m_nMaxParticle;
	float m_fSize;

	DWORD m_vbSize;
	DWORD m_vbOffset;
	DWORD m_vbBatchSize;

public:
	cPtSEffect();
	virtual ~cPtSEffect();

	virtual void Update();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();

	virtual void ResetParticle(ST_SPPT_DATA& particle);
};