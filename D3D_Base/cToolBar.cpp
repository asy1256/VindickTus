#include "stdafx.h"
#include "cToolBar.h"

cToolBar::cToolBar()
	: m_pIsOpen(NULL)
	, m_pNowselect(NULL)
{
}

cToolBar::~cToolBar()
{
	std::list<ST_IMG_DATA*>::iterator iter;
	for (iter = m_listData.begin(); iter != m_listData.end(); ++iter)
	{
		SAFE_RELEASE((*iter)->texture);
	}

	for (int i = 0; i < 5; ++i)
	{
		SAFE_RELEASE(m_stEnvSample[i].texture);
		SAFE_RELEASE(m_stStcSample[i].texture);
	}
}

void cToolBar::Setup()
{
	m_vecBasePos.x = 1280 - 465;
	m_vecBasePos.y = 0;
	m_vecBasePos.z = 0;

	SetButton();
}

void cToolBar::Click()
{
	POINT pt = g_ptMouse;
	pt.x += 128;
	pt.y += 50;
	//메뉴 전환
	if (m_stMenuBtn.draw && PtInRect(&m_stMenuBtn.rc, pt))
	{
		for (int i = 0; i < 5; ++i)
		{
			m_stStcSample[i].draw = (m_stStcSample[i].draw) ? false : true;
			m_stEnvSample[i].draw = false;
		}
	}
	//설치할 오브젝트 고르기
	for (int i = 0; i < 5; ++i)
	{
		if (m_stEnvSample[i].draw && PtInRect(&m_stEnvSample[i].rc, pt))
		{
			*m_pNowselect = i;
		}
		if (m_stStcSample[i].draw && PtInRect(&m_stStcSample[i].rc, pt))
		{
			*m_pNowselect = i + 5;
		}
	}
	//세이브 OR 로드
	for (int i = 0; i < 2; ++i)
	{
		if (!m_stSaLo[i].draw) { break; }
		if (PtInRect(&m_stSaLo[i].rc, pt))
		{
			if (i == 0) { Save(); }
			else { Load(); }
		}
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

	for (int i = 0; i < 5; ++i)
	{
		int yp;

		switch (i)
		{
		case 0: case 1: yp = 0; break;
		case 2: case 3: yp = 148; break;
		case 4: yp = 296; break;
		}

		SetRect(&rc
			, (i % 2 == 0) ? 0 : m_stEnvSample[i].imgInfo.Width / 2
			, yp
			, (i % 2 == 0) ? (m_stEnvSample[i].imgInfo.Width) / 2 : m_stEnvSample[i].imgInfo.Width
			, yp + (m_stEnvSample[i].imgInfo.Height / 3));

		if (m_stEnvSample[i].draw)
		{
			g_pSprite->Draw(m_stEnvSample[i].texture,
				&rc,
				&D3DXVECTOR3(0, 0, 0),
				&(m_vecBasePos + m_stEnvSample[i].localPos),
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (m_stStcSample[i].draw)
		{
			g_pSprite->Draw(m_stStcSample[i].texture,
				&rc,
				&D3DXVECTOR3(0, 0, 0),
				&(m_vecBasePos + m_stStcSample[i].localPos),
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	g_pSprite->End();
}

void cToolBar::SetButton()
{
	//UI배경
	m_stPanel.localPos = D3DXVECTOR3(0, 0, 0);
	m_stPanel.texture = g_pTextureManager->GetTextureEx("panel", "ui/panel.png", m_stPanel.imgInfo);
	SetRect(&m_stPanel.rc, 0, 0, m_stPanel.imgInfo.Width, m_stPanel.imgInfo.Height);
	m_stPanel.draw = true;
	m_listData.push_back(&m_stPanel);

	//메뉴 버튼
	m_stMenuBtn.localPos = D3DXVECTOR3(-100, 10, 0);
	m_stMenuBtn.texture = g_pTextureManager->GetTextureEx("menu", "ui/test2.png", m_stMenuBtn.imgInfo);
	SetRect(&m_stMenuBtn.rc
		, m_vecBasePos.x + m_stMenuBtn.localPos.x
		, m_vecBasePos.y + m_stMenuBtn.localPos.y
		, m_vecBasePos.x + m_stMenuBtn.localPos.x + m_stMenuBtn.imgInfo.Width
		, m_vecBasePos.y + m_stMenuBtn.localPos.y + m_stMenuBtn.imgInfo.Height);
	m_stMenuBtn.draw = true;
	m_listData.push_back(&m_stMenuBtn);

	//오브젝트 샘플
	for (int i = 0, x = 50, y = 75; i < 5; ++i)
	{
		if (i % 2 == 0 && i != 0) { y += 200; }
		if (i % 2 == 0) { x = 50; }
		else { x += 200; }

		m_stEnvSample[i].localPos = D3DXVECTOR3(x, y, 0);
		m_stEnvSample[i].texture = g_pTextureManager->GetTextureEx("EnvSample", "ui/rock.png", m_stEnvSample[i].imgInfo);
		m_stEnvSample[i].draw = false;
		SetRect(&m_stEnvSample[i].rc
			, m_vecBasePos.x + m_stEnvSample[i].localPos.x
			, m_vecBasePos.y + m_stEnvSample[i].localPos.y
			, m_vecBasePos.x + m_stEnvSample[i].localPos.x + m_stEnvSample[i].imgInfo.Width / 2
			, m_vecBasePos.y + m_stEnvSample[i].localPos.y + m_stEnvSample[i].imgInfo.Height / 3);

		m_stStcSample[i].localPos = D3DXVECTOR3(x, y, 0);
		m_stStcSample[i].texture = g_pTextureManager->GetTextureEx("StcSample", "ui/structer.png", m_stStcSample[i].imgInfo);
		m_stStcSample[i].draw = false;
		SetRect(&m_stStcSample[i].rc
			, m_vecBasePos.x + m_stStcSample[i].localPos.x
			, m_vecBasePos.y + m_stStcSample[i].localPos.y
			, m_vecBasePos.x + m_stStcSample[i].localPos.x + m_stStcSample[i].imgInfo.Width / 2
			, m_vecBasePos.y + m_stStcSample[i].localPos.y + m_stStcSample[i].imgInfo.Height / 3);
	}

	
}

void cToolBar::Save()
{

}

void cToolBar::Load()
{
}
