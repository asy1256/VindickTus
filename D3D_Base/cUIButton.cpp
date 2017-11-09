#include "stdafx.h"
#include "cUIButton.h"


cUIButton::cUIButton()
	: m_eButtonState(BTN_NORMAL)
{
}


cUIButton::~cUIButton()
{
}

void cUIButton::SetTexture(char * szFile, char * szFullPath)
{
	m_cFile = szFile;
	g_pSpriteImageManager->AddSpriteImage(szFile, szFullPath);
}

void cUIButton::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT rc;
	SetRect(&rc,
		(int)m_matWorld._41,
		(int)m_matWorld._42,
		(int)m_matWorld._41 + (int)g_pSpriteImageManager->getImageRC(m_cFile)->right,
		(int)m_matWorld._42 + (int)g_pSpriteImageManager->getImageRC(m_cFile)->bottom);

	// over, clicked
	if (PtInRect(&rc, pt))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			m_eButtonState = BTN_CLICKED;
			m_pDelegate->OnClick(this);
		}
		else
		{
			m_eButtonState = BTN_MOUSESTAY;
		}
	}

	if (KEYMANAGER->isOnceKeyDown('I'))
	{
		m_eButtonState = BTN_CLICKED;
		m_pDelegate->OnClick(this);
	}

	cUIObject::Update();
}

void cUIButton::Render(char * szFile, int alpha)
{
	g_pDeviceManager->GetSprite()->SetTransform(&m_matWorld);

	switch (m_eButtonState)
	{
	case cUIButton::BTN_NORMAL:
	{
		g_pSpriteImageManager->Render(szFile, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(alpha, 255, 255, 255), 2, 1, 0, 0);
	}break;
	case cUIButton::BTN_MOUSESTAY:
	{
		g_pSpriteImageManager->Render(szFile, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(alpha, 255, 255, 255), 2, 1, 0, 0);
	}break;
	case cUIButton::BTN_CLICKED:
	{
		g_pSpriteImageManager->Render(szFile, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(alpha, 255, 255, 255), 2, 1, 1, 0);
	}break;
	}

	if (m_bIsActivation)
	{
		if (!m_vecChild.empty())
		{
			for each(auto p in m_vecChild)
				p->Render(p->m_cFile, 255);
		}
	}
}
