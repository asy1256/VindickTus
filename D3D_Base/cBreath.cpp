#include "stdafx.h"
#include "cBreath.h"

cBreath::cBreath()
{
}

cBreath::~cBreath()
{
}

void cBreath::Setup(int maxparticle)
{
	m_vbOffset = 0;
	m_nMaxParticle = m_vbSize = maxparticle;
	m_vbBatchSize = maxparticle;
	m_fSize = 2.0f;

	m_pTexture = g_pTextureManager->GetAniFxTexture(std::string("fx/breath1.png"));

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
	temp.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	temp.colorfade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.01f);

}

void cBreath::Update()
{

}

void cBreath::Render()
{
	cPtSEffect::Render();
}
