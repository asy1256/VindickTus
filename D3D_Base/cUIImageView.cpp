#include "stdafx.h"
#include "cUIImageView.h"


cUIImageView::cUIImageView()
{
}


cUIImageView::~cUIImageView()
{
}

void cUIImageView::SetTexture(char * szFile, char * szFullPath)
{
	m_cFile = szFile;
	g_pSpriteImageManager->AddSpriteImage(szFile, szFullPath);
}

void cUIImageView::Update()
{
	if (m_pParent)
	{
		if (m_pParent->GetActivation())
			m_bIsActivation = true;
	}

	if (!m_bIsActivation) return;

	cUIObject::Update();
}

void cUIImageView::Render(char * szFile, int alpha)
{
	if (!m_bIsActivation) return;

	g_pDeviceManager->GetSprite()->SetTransform(&m_matWorld);

	g_pSpriteImageManager->Render(szFile, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(alpha, 255, 255, 255));

	if (!m_vecChild.empty())
	{
		for each(auto p in m_vecChild)
			p->Render(p->m_cFile, 255);
	}
}
