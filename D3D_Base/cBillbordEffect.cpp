#include "stdafx.h"
#include "cBillbordEffect.h"

cBillbordEffect::cBillbordEffect()
	: m_pTexture(NULL)
	, m_pVb(NULL)
	, m_pIb(NULL)
{
}

cBillbordEffect::~cBillbordEffect()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVb);
	SAFE_RELEASE(m_pIb);
}

void cBillbordEffect::Update()
{
}

void cBillbordEffect::PreRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXMATRIXA16 view, bill;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixIdentity(&bill);
	bill._11 = view._11;
	bill._13 = view._13;
	bill._31 = view._31;
	bill._33 = view._33;
	D3DXMatrixInverse(&bill, 0, &bill);
	m_matWorld = bill;
}

void cBillbordEffect::Render()
{
}

void cBillbordEffect::PostRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
