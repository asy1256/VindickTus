#pragma once

class cQuadTree;

class cCustomMap
{
private:
	LPD3DXMESH					m_pMesh;
	std::vector<D3DXVECTOR3>	m_vecVertex;
	D3DMATERIAL9				m_mtl;
	LPDIRECT3DTEXTURE9			m_pTexture;
	int							m_nRow;
	D3DXMATRIXA16				m_matWorld;
	cQuadTree*					m_pQuadTree;

public:
	cCustomMap();
	~cCustomMap();

	void Setup(int nWidth, int nHeight, float fTileSize);
	void Update();
	void Render();
	void Destroy();
	void Picking(Ray& ray, D3DXVECTOR3& pick);
};