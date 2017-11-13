#include "stdafx.h"
#include "cStonPtcSplash.h"
#include <time.h>

cStonPtcSplash::cStonPtcSplash()
{
}

cStonPtcSplash::~cStonPtcSplash()
{
}

void cStonPtcSplash::Setup(int maxparticle)
{
	srand(time(NULL));

	m_vbOffset = 0;
	m_nMaxParticle = m_vbSize = maxparticle;
	m_vbBatchSize = maxparticle;
	m_fSize = 1.0f;

	m_pTexture = g_pTextureManager->GetTexture(std::string("fx/stone.png"));

	g_pD3DDevice->CreateVertexBuffer(
		m_vbSize * sizeof(ST_PC_VERTEX),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		ST_PC_VERTEX::FVF,
		D3DPOOL_DEFAULT, // D3DPOOL_MANAGED can't be used with D3DUSAGE_DYNAMIC
		&m_pEffect,
		0);

	ST_SPPT_DATA temp;
	temp.isTriger = false;
	temp.pos = D3DXVECTOR3(0, 0, 0);
	temp.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	temp.colorfade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.1f);
	temp.power = 0.f;
	D3DXMATRIX matR;
	for (int i = 0; i < maxparticle; ++i)
	{
		temp.roty = D3DXToRadian(rand() % 360 + 1);
		D3DXMatrixRotationY(&matR, temp.roty);
		temp.dir = D3DXVECTOR3(0, 0, 1);
		D3DXVec3TransformNormal(&temp.dir, &temp.dir, &matR);
		temp.powerlimit = (rand() % 30 + 1) / 10.f;
		temp.speed = (rand() % 10 + 1) / 100.f;
		m_listParticle.push_back(temp);
	}

	ST_EFX_WORK* work = new ST_EFX_DIRMOVE;
	m_vecWork.push_back(work);
	work = new ST_EFX_FALL;
	m_vecWork.push_back(work);
}

void cStonPtcSplash::Update()
{
	std::list<ST_SPPT_DATA>::iterator i;
	for (i = m_listParticle.begin(); i != m_listParticle.end(); ++i)
	{
		if (i->pos.y < 0) { continue; }
		for (int j = 0; j < m_vecWork.size(); ++j)
		{
			m_vecWork[j]->Work(*i);
		}
	}
}

void cStonPtcSplash::Render()
{
	cPtSEffect::Render();
}
