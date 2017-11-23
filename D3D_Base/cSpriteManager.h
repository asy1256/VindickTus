#pragma once

#define g_pSpriteImageManager cSpriteManager::GetInstance()

struct TextureInfo
{
	LPDIRECT3DTEXTURE9	texture;
	D3DXIMAGE_INFO		imageInfo;
	RECT				rc;

	TextureInfo()
	: texture(NULL)
	{}
};

class cSpriteManager
{
private:
	SINGLETONE(cSpriteManager);

	std::map<std::string, TextureInfo> m_mapTexture;

public:
	
	void AddSpriteImage(char * szFile, char * szFullPath);

	void Render(char * szFile, D3DXVECTOR3 center, D3DXVECTOR3 position, D3DCOLOR color);
	void Render(char * szFile, D3DXVECTOR3 center, D3DXVECTOR3 position, D3DCOLOR color, int frameSizeX, int frameSizeY, int frameX, int frameY);

	RECT* getImageRC(char * szFile) { return &m_mapTexture[szFile].rc; }

	void Destroy();
};

