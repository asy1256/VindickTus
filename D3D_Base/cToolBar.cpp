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

	for (int i = 0; i < 3; ++i) { SAFE_RELEASE(m_stTagButton[i].texture); }

	for (int i = 0; i < 5; ++i)
	{
		SAFE_RELEASE(m_stEnvSample[i].texture);
		SAFE_RELEASE(m_stStcSample[i].texture);
	}

	for (int i = 0; i < 2; ++i) { SAFE_RELEASE(m_stSaLo[i].texture); }
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
	for (int i = 0; i < 3; ++i)
	{
		if (PtInRect(&m_stTagButton[i].rc, pt))
		{
			if (i == 0)
			{
				for (int i = 0; i < 5; ++i)
				{
					if (i < 3) { m_stSaLo[i].draw = false; }
					m_stEnvSample[i].draw = (m_stEnvSample[i].draw) ? false : true;
					m_stStcSample[i].draw = false;
				}
			}
			else if (i == 1)
			{
				for (int i = 0; i < 5; ++i)
				{
					if (i < 3) { m_stSaLo[i].draw = false; }
					m_stStcSample[i].draw = (m_stStcSample[i].draw) ? false : true;
					m_stEnvSample[i].draw = false;
				}
			}
			else
			{
				for (int i = 0; i < 5; ++i)
				{
					if (i < 3) { m_stSaLo[i].draw = (m_stSaLo[i].draw) ? false : true; }
					m_stStcSample[i].draw = false;
					m_stEnvSample[i].draw = false;
				}
			}
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
	//태그 버튼 그리기
	for (int i = 0; i < 3; ++i)
	{
		int yp;

		if (i == 0) { yp = 0; }
		else if (i == 1) { yp = 40; }
		else { yp = 80; }

		SetRect(&rc
			, 0
			, yp
			, m_stTagButton[i].imgInfo.Width
			, yp + (m_stTagButton[i].imgInfo.Height / 3));

		g_pSprite->Draw(m_stTagButton[i].texture,
			&rc,
			&D3DXVECTOR3(0, 0, 0),
			&(m_vecBasePos + m_stTagButton[i].localPos),
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//오브젝트 샘플 그리기
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
	//세이브 로드 그리기
	for (int i = 0; i < 2; ++i)
	{
		int yp;

		if (i == 0) { yp = 0; }
		else { yp = 80; }

		SetRect(&rc
			, 0
			, yp
			, m_stSaLo[i].imgInfo.Width
			, yp + (m_stSaLo[i].imgInfo.Height / 2));
		
		if (m_stSaLo[i].draw)
		{
			g_pSprite->Draw(m_stSaLo[i].texture,
				&rc,
				&D3DXVECTOR3(0, 0, 0),
				&(m_vecBasePos + m_stSaLo[i].localPos),
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

	//태그 버튼 m_stTagButton
	for (int i = 0, y = 10; i < 3; ++i, y += 50)
	{
		m_stTagButton[i].localPos = D3DXVECTOR3(-100, y, 0);
		m_stTagButton[i].texture = g_pTextureManager->GetTextureEx("tag", "ui/tagbutton.png", m_stTagButton[i].imgInfo);
		m_stTagButton[i].draw = true;
		SetRect(&m_stTagButton[i].rc
			, m_vecBasePos.x + m_stTagButton[i].localPos.x
			, m_vecBasePos.y + m_stTagButton[i].localPos.y
			, m_vecBasePos.x + m_stTagButton[i].localPos.x + m_stTagButton[i].imgInfo.Width
			, m_vecBasePos.y + m_stTagButton[i].localPos.y + m_stTagButton[i].imgInfo.Height / 3);
	}
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
	//세이브 로드 버튼
	for (int i = 0, y = 30; i < 2; ++i, y += 100)
	{
		m_stSaLo[i].localPos = D3DXVECTOR3(50, y, 0);
		m_stSaLo[i].texture = g_pTextureManager->GetTextureEx("saveload", "ui/salo.png", m_stSaLo[i].imgInfo);
		m_stSaLo[i].draw = false;
		SetRect(&m_stSaLo[i].rc
			, m_vecBasePos.x + m_stSaLo[i].localPos.x
			, m_vecBasePos.y + m_stSaLo[i].localPos.y
			, m_vecBasePos.x + m_stSaLo[i].localPos.x + m_stSaLo[i].imgInfo.Width
			, m_vecBasePos.y + m_stSaLo[i].localPos.y + m_stSaLo[i].imgInfo.Height / 2);
	}
}

void cToolBar::Save()
{
	g_pObjectManager->SaveObject();
}

void cToolBar::Load()
{
	g_pObjectManager->LoadObject();
}
