#include "stdafx.h"
#include "cEffect.h"
#include <time.h>

cEffect::cEffect()
	: m_pEffect(NULL)
	, m_pTexture(NULL)
{
}

cEffect::~cEffect()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pTexture);
	for (int i = 0; i < m_vecWork.size(); ++i)
	{
		SAFE_DELETE(m_vecWork[i]);
	}
}

void cEffect::Setup(int maxparticle)
{
	srand(time(NULL));

	m_vbOffset = 0;
	m_nMaxParticle = m_vbSize = maxparticle;
	m_vbBatchSize = maxparticle / 2;
	m_fSize = 1.0f;

	m_pTexture = g_pTextureManager->GetTexture(std::string("fx/yellowflare3.png"));

	g_pD3DDevice->CreateVertexBuffer(
		m_vbSize * sizeof(ST_PC_VERTEX),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		ST_PC_VERTEX::FVF,
		D3DPOOL_DEFAULT, // D3DPOOL_MANAGED can't be used with D3DUSAGE_DYNAMIC 
		&m_pEffect,
		0);

	ST_SPPT_DATA temp;
	temp.life = 60.0f;
	temp.roty = D3DX_PI;
	//D3DXVec3Normalize(&temp.dir, &D3DXVECTOR3(0, -1, 0));
	D3DXMATRIXA16 matr;
	D3DXMatrixRotationZ(&matr, temp.roty);
	D3DXVec3TransformNormal(&temp.dir, &temp.dir, &matr);
	temp.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	temp.colorfade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.1f);
	for (int i = 0; i < maxparticle; ++i)
	{
		temp.speed = (rand() % 100 + 1) / 100.0f;
		m_listParticle.push_back(temp);
	}

	ST_EFX_WORK* work = new ST_EFX_MOVE;
	m_vecWork.push_back(work);
	//work = new ST_EFX_MOVE;
	//m_vecWork.push_back(work);
}

void cEffect::Update()
{
	std::list<ST_SPPT_DATA>::iterator i;
	for (i = m_listParticle.begin(); i != m_listParticle.end(); ++i)
	{
		if (!i->isAlive) { ResetParticle(*i);  continue; }
		i->age += 1.0f;
		if (i->age > i->life)
		{
			i->isAlive = false;
			continue;
		}
		for (int j = 0; j < m_vecWork.size(); ++j)
		{
			m_vecWork[j]->Work(*i);
		}
	}
}

void cEffect::PreRender()
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

void cEffect::Render()
{
	PreRender();

	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pEffect, 0, sizeof(ST_PC_VERTEX));

	ST_PC_VERTEX* sprite;

	if (m_vbOffset >= m_vbSize)
		m_vbOffset = 0;

	m_pEffect->Lock(m_vbOffset * sizeof(ST_PC_VERTEX), m_vbBatchSize * sizeof(ST_PC_VERTEX)
		,(void**)&sprite ,m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

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

void cEffect::PostRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void cEffect::ResetParticle(ST_SPPT_DATA& particle)
{
	particle.age = 0.0f;
	particle.isAlive = true;
	particle.pos = D3DXVECTOR3(0, 0, 0);
	particle.speed = (rand() % 100 + 1) / 100.0f;
}
