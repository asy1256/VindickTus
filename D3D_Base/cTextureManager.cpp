#include "stdafx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( std::string & sFullPath )
{
	wchar_t path[1024];

	mbstowcs_s(NULL, path, sFullPath.c_str(), sFullPath.length());

	if ( m_mapTexture.find( sFullPath ) == m_mapTexture.end() )
	{
		D3DXCreateTextureFromFile( g_pD3DDevice, path, &m_mapTexture[sFullPath] );
	}
	return m_mapTexture[sFullPath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetAniFxTexture(std::string & sFullPath)
{
	wchar_t path[1024];

	mbstowcs_s(NULL, path, sFullPath.c_str(), sFullPath.length());

	if (m_mapTexture.find(sFullPath) == m_mapTexture.end())
	{
		D3DXIMAGE_INFO temp;
		D3DXCreateTextureFromFileEx(
			g_pD3DDevice,
			path,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			(D3DCOLOR)D3DXCOLOR(0.f, 0.f, 0.f, 1.0f),
			&temp,
			NULL,
			&m_mapTexture[sFullPath]);
	}

	return m_mapTexture[sFullPath];
}

void cTextureManager::Destroy()
{
	for each( auto p in  m_mapTexture )
	{
		SAFE_RELEASE( p.second );
	}
	m_mapTexture.clear();
}
