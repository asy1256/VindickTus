#pragma once

#include "cEffectWork.h"

class cEffect
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pEffect;
	std::list<ST_SPPT_DATA> m_listParticle;
	std::vector<ST_EFX_WORK*> m_vecWork;
	LPDIRECT3DTEXTURE9 m_pTexture;

	int m_nMaxParticle;
	float m_fSize;

	DWORD m_vbSize;
	DWORD m_vbOffset;
	DWORD m_vbBatchSize;

public:
	cEffect();
	virtual ~cEffect();

	virtual void Setup(int maxparticle);
	virtual void Update();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();

	virtual void ResetParticle(ST_SPPT_DATA& particle);

	virtual DWORD FtoDW(float f) { return *(DWORD*)&f; }
};