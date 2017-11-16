#include "stdafx.h"
#include "cStamping.h"

cStamping::cStamping()
{
}

cStamping::~cStamping()
{
}

void cStamping::Setup(int maxparticle, float lange)
{
	m_vbOffset = 0;
	m_nMaxParticle = m_vbSize = maxparticle;
	m_vbBatchSize = maxparticle;
	m_fSize = 2.0f;
	m_fLength = lange;

	m_pTexture = g_pTextureManager->GetTexture(std::string("fx/cold_dust.png"));

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
	temp.speed = 0.05f;
	temp.color = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);
	temp.colorfade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.1f);
	D3DXMATRIX matR;
	for (int i = 0; i < maxparticle; ++i)
	{
		temp.roty = D3DXToRadian(i) * 4; 
		D3DXMatrixRotationY(&matR, temp.roty);
		temp.dir = D3DXVECTOR3(0, 0, 1);
		D3DXVec3TransformNormal(&temp.dir, &temp.dir, &matR);
		m_listParticle.push_back(temp);
	}

	ST_EFX_WORK* work = new ST_EFX_DIRMOVE;
	m_vecWork.push_back(work);
	work = new ST_EFX_UP_DOWN;
	m_vecWork.push_back(work);
}

void cStamping::Update()
{
	std::list<ST_SPPT_DATA>::iterator i;
	for (i = m_listParticle.begin(); i != m_listParticle.end(); ++i)
	{
		if (D3DXVec3Length(&i->pos) > m_fLength)
		{
			continue;
		}
		for (int j = 0; j < m_vecWork.size(); ++j)
		{
			m_vecWork[j]->Work(*i);
		}
	}
}

void cStamping::Render()
{
	cPtSEffect::Render();
}
