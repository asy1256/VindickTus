#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	SINGLETONE(cTextureManager);

	std::map<std::string, LPDIRECT3DTEXTURE9>	m_mapTexture;
public:
	
	LPDIRECT3DTEXTURE9 GetTexture( std::string& sFullPath );
	LPDIRECT3DTEXTURE9 GetTexture(char * szFullPath);
	LPDIRECT3DTEXTURE9 GetTextureEx(char * szFile, char * szFullPath, D3DXIMAGE_INFO& ImageInfo);

	void Destroy();
};

