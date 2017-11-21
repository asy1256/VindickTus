#include "stdafx.h"
#include "cTextView.h"


cTextView::cTextView()
	: m_dwDrawTextFormat(DT_LEFT | DT_TOP)
	, m_dwTextColor(D3DCOLOR_XRGB(0, 0, 0))
	, m_eFontType(cFontManager::FT_DEFAULT)
	, m_ptSize({50, 30})
{
}


cTextView::~cTextView()
{
}

void cTextView::Render(char * szFile, int alpha)
{
	LPD3DXFONT pFont = g_pFontManager->GetFont(m_eFontType);

	RECT rc;
	SetRect(&rc, (int)m_matWorld._41, (int)m_matWorld._42,
		(int)m_matWorld._41 + g_pSpriteImageManager->getImageRC(szFile)->right,
		(int)m_matWorld._42 + g_pSpriteImageManager->getImageRC(szFile)->bottom);
	
	pFont->DrawTextA(NULL,
		m_sText.c_str(),
		m_sText.length(),
		&rc,
		m_dwDrawTextFormat,
		m_dwTextColor);
}
