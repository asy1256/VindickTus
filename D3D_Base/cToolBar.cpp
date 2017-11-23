#include "stdafx.h"
#include "cToolBar.h"

cToolBar::cToolBar()
	: m_pIsOpen(NULL)
{
}

cToolBar::~cToolBar()
{
	std::list<ST_IMG_DATA*>::iterator iter;
	for (iter = m_listData.begin(); iter != m_listData.end(); ++iter)
	{
		SAFE_RELEASE((*iter)->texture);
	}
}

void cToolBar::Setup()
{
	m_vecBasePos.x = 1280 - 465;
	m_vecBasePos.y = 0;
	m_vecBasePos.z = 0;

	m_stPanel.localPos = D3DXVECTOR3(0, 0, 0);
	m_stPanel.texture = g_pTextureManager->GetTextureEx("panel", "ui/panel.png", m_stPanel.imgInfo);
	SetRect(&m_stPanel.rc, 0, 0, m_stPanel.imgInfo.Width, m_stPanel.imgInfo.Height);
	m_stPanel.draw = true;

	m_stMenu.localPos = D3DXVECTOR3(-100, 10, 0);
	m_stMenu.texture = g_pTextureManager->GetTextureEx("menu", "ui/test2.png", m_stMenu.imgInfo);
	SetRect(&m_stMenu.rc
		, m_vecBasePos.x + m_stMenu.localPos.x
		, m_vecBasePos.y + m_stMenu.localPos.y
		, m_vecBasePos.x + m_stMenu.localPos.x + m_stMenu.imgInfo.Width
		, m_vecBasePos.y + m_stMenu.localPos.y + m_stMenu.imgInfo.Height);
	m_stMenu.draw = true;

	m_listData.push_back(&m_stPanel);
	m_listData.push_back(&m_stMenu);
}

void cToolBar::Click()
{
	POINT pt = g_ptMouse;
	pt.x += 128;
	pt.y += 50;

	if (PtInRect(&m_stMenu.rc, pt))
	{

	}
}

void cToolBar::Render()
{
	if (!(*m_pIsOpen)) { return; }

	RECT rc;

	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	std::list<ST_IMG_DATA*>::iterator iter;
	for (iter = m_listData.begin(); iter != m_listData.end(); ++iter)
	{
		if (!(*iter)->draw) { continue; }
		
		SetRect(&rc, 0, 0
			, (*iter)->imgInfo.Width
			, (*iter)->imgInfo.Height);

		g_pSprite->Draw((*iter)->texture,
			&rc,
			&D3DXVECTOR3(0, 0, 0),
			&(m_vecBasePos + (*iter)->localPos),
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	g_pSprite->End();
}
