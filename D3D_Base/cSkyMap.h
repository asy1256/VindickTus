#pragma once

class cSkyMap
{
private:
	std::vector<ST_PNT_VERTEX>	m_vecVertex;

	D3DXVECTOR3			m_vPosition;
	D3DXMATRIXA16		m_matWorld;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DMATERIAL9		m_Material;



public:
	cSkyMap();
	~cSkyMap();

	void Setup();
	void Update(D3DXVECTOR3 pos);
	void Render();
};

