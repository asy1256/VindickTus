#include "stdafx.h"
#include "cPtSEffect.h"

cPtSEffect::cPtSEffect()
	: m_pEffect(NULL)
	, m_pTexture(NULL)
{
}

cPtSEffect::~cPtSEffect()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pTexture);
	for (int i = 0; i < m_vecWork.size(); ++i)
	{
		SAFE_DELETE(m_vecWork[i]);
	}
}

void cPtSEffect::Update()
{
}

void cPtSEffect::PreRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_fSize));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDW(100.0f));

	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(1.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(1.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void cPtSEffect::Render()
{
	PreRender();

	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pEffect, 0, sizeof(ST_PC_VERTEX));
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	ST_PC_VERTEX* sprite;

	if (m_vbOffset >= m_vbSize)
		m_vbOffset = 0;

	m_pEffect->Lock(m_vbOffset * sizeof(ST_PC_VERTEX), m_vbBatchSize * sizeof(ST_PC_VERTEX)
		, (void**)&sprite, m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

	DWORD dwInsertParticle = 0;

	std::list<ST_SPPT_DATA>::iterator i;
	for (i = m_listParticle.begin(); i != m_listParticle.end(); ++i)
	{
		if (!i->isAlive) { continue; }

		sprite->p = i->pos;
		sprite->c = (D3DCOLOR)i->color;
		++sprite;
		++dwInsertParticle;

		if (dwInsertParticle >= m_vbBatchSize)
		{
			m_pEffect->Unlock();

			g_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);

			m_vbOffset += dwInsertParticle;

			if (m_vbOffset >= m_vbSize)
				m_vbOffset = 0;

			m_pEffect->Lock(m_vbOffset * sizeof(ST_PC_VERTEX), m_vbBatchSize * sizeof(ST_PC_VERTEX)
				, (void**)&sprite, m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

			dwInsertParticle = 0;
		}
	}

	m_pEffect->Unlock();

	if (dwInsertParticle)
	{
		g_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, dwInsertParticle);
	}

	m_vbOffset += m_vbBatchSize;

	PostRender();
}

void cPtSEffect::PostRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void cPtSEffect::ResetParticle(ST_SPPT_DATA & particle)
{
	particle.age = 0.0f;
	particle.isAlive = true;
	particle.pos = D3DXVECTOR3(0, 0, 0);
	particle.speed = (rand() % 100 + 1) / 100.0f;
}
