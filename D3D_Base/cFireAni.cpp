#include "stdafx.h"
#include "cFireAni.h"

cFireAni::cFireAni()
{
}

cFireAni::~cFireAni()
{
}

void cFireAni::Setup()
{
	m_nFrameX = 8;
	m_nFrameY = 4;
	m_nCurFrameX = 0;
	m_nCurFrameY = 0;

	m_pTexture = g_pTextureManager->GetAniFxTexture(std::string("fx/flare2.png"));

	g_pD3DDevice->CreateVertexBuffer(4 * sizeof(ST_PT_VERTEX), 0,
		ST_PT_VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVb, 0);

	ST_PT_VERTEX temp;
	temp.p = D3DXVECTOR3(-0.5, 2, 0);
	temp.t = D3DXVECTOR2((float)m_nCurFrameX / m_nFrameX, m_nCurFrameY);
	ST_PT_VERTEX* vb;
	m_pVb->Lock(0, 0, (void**)&vb, 0);

	vb[0] = temp;

	temp.p = D3DXVECTOR3(0.5, 2, 0);
	temp.t = D3DXVECTOR2((float)(m_nCurFrameX + 1) / m_nFrameX, 0);
	vb[1] = temp;

	temp.p = D3DXVECTOR3(-0.5, 0, 0);
	temp.t = D3DXVECTOR2((float)m_nCurFrameX / m_nFrameX, (float)(m_nCurFrameY + 1) / m_nFrameY);
	vb[2] = temp;

	temp.p = D3DXVECTOR3(0.5, 0, 0);
	temp.t = D3DXVECTOR2((float)(m_nCurFrameX + 1) / m_nFrameX, (float)(m_nCurFrameY + 1) / m_nFrameY);
	vb[3] = temp;

	m_pVb->Unlock();

	g_pD3DDevice->CreateIndexBuffer(6 * sizeof(WORD), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIb, 0);

	WORD* ib;
	m_pIb->Lock(0, 0, (void**)&ib, 0);

	ib[0] = 2;
	ib[1] = 0;
	ib[2] = 1;
	ib[3] = 2;
	ib[4] = 1;
	ib[5] = 3;

	m_pIb->Unlock();
}

void cFireAni::Update()
{
	++m_nTimeCount;
	if (m_nTimeCount == 3)
	{
		m_nTimeCount = 0;
		if (m_nCurFrameY == m_nFrameY) { m_nCurFrameY = 0; }
		else if (m_nCurFrameX == m_nFrameX) { ++m_nCurFrameY; }
		if (m_nCurFrameX == m_nFrameX) { m_nCurFrameX = 0; }
		else { ++m_nCurFrameX; }
		if (m_nCurFrameX != m_nFrameX)
		{
			ST_PT_VERTEX* v;
			m_pVb->Lock(0, 0, (void**)&v, 0);

			v[0].t = D3DXVECTOR2((float)m_nCurFrameX / m_nFrameX, (float)m_nCurFrameY / m_nFrameY);
			v[1].t = D3DXVECTOR2((float)(m_nCurFrameX + 1) / m_nFrameX, (float)m_nCurFrameY / m_nFrameY);
			v[2].t = D3DXVECTOR2((float)m_nCurFrameX / m_nFrameX, (float)(m_nCurFrameY + 1) / m_nFrameY);
			v[3].t = D3DXVECTOR2((float)(m_nCurFrameX + 1) / m_nFrameX, (float)(m_nCurFrameY + 1) / m_nFrameY);

			m_pVb->Unlock();
		}
	}
}

void cFireAni::Render()
{
	cBillbordEffect::PreRender();

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVb, 0, sizeof(ST_PT_VERTEX));
	g_pD3DDevice->SetIndices(m_pIb);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		4 * sizeof(ST_PT_VERTEX), 0, 2);

	cBillbordEffect::PostRender();
}
