#include "stdafx.h"
#include "cSpriteManager.h"


cSpriteManager::cSpriteManager()
{
}


cSpriteManager::~cSpriteManager()
{
}

void cSpriteManager::AddSpriteImage(char * szFile, char * szFullPath)
{
	m_mapTexture[szFile].texture = g_pTextureManager->GetTextureEx(szFile, szFullPath, m_mapTexture[szFile].imageInfo);
}

void cSpriteManager::Render(char * szFile, D3DXVECTOR3 center, D3DXVECTOR3 position, D3DCOLOR color)
{
	g_pDeviceManager->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	m_mapTexture[szFile].rc;
	SetRect(&m_mapTexture[szFile].rc, 0, 0, m_mapTexture[szFile].imageInfo.Width, m_mapTexture[szFile].imageInfo.Height);

	g_pDeviceManager->GetSprite()->Draw(m_mapTexture[szFile].texture,
		&m_mapTexture[szFile].rc,
		&center,
		&position,
		color);

	g_pDeviceManager->GetSprite()->End();
}

void cSpriteManager::Render(char * szFile, D3DXVECTOR3 center, D3DXVECTOR3 position, D3DCOLOR color, int frameSizeX, int frameSizeY, int frameX, int frameY)
{
	g_pDeviceManager->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	if (frameX >= frameSizeX) frameX = frameSizeX - 1;
	if (frameY >= frameSizeY) frameY = frameSizeY - 1;

	m_mapTexture[szFile].rc;
	SetRect(&m_mapTexture[szFile].rc, 
		(m_mapTexture[szFile].imageInfo.Width / frameSizeX) * frameX,
		(m_mapTexture[szFile].imageInfo.Height / frameSizeY) * frameY,
		(m_mapTexture[szFile].imageInfo.Width / frameSizeX) + ((m_mapTexture[szFile].imageInfo.Width / frameSizeX) * frameX),
		(m_mapTexture[szFile].imageInfo.Height / frameSizeY) + ((m_mapTexture[szFile].imageInfo.Height / frameSizeY) * frameY));

	g_pDeviceManager->GetSprite()->Draw(m_mapTexture[szFile].texture,
		&m_mapTexture[szFile].rc,
		&center,
		&position,
		color);

	g_pDeviceManager->GetSprite()->End();
}

void cSpriteManager::Destroy()
{
	m_mapTexture.clear();
}
