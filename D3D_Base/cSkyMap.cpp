#include "stdafx.h"
#include "cSkyMap.h"


cSkyMap::cSkyMap()
	:m_pTexture(NULL)
	, m_vPosition(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cSkyMap::~cSkyMap()
{
	SAFE_RELEASE(m_pTexture);
}

void cSkyMap::Setup()
{
	//if (FAILED(D3DXCreateCubeTextureFromFile(g_pD3DDevice, L"Texture1.dds", (LPDIRECT3DCUBETEXTURE9*)&m_pTexture))) {}
	if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice, L"skybox.jpg", &m_pTexture)))
	{
		assert(false && "ÅØ½ºÃ³ ·Îµå ½ÇÆÐ!");
	}

	m_Material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	std::vector<ST_PNT_VERTEX> vecVertex;
	vecVertex.resize(8);

	vecVertex[0].p.x = m_vPosition.x - 0.5f; vecVertex[0].p.y = m_vPosition.y - 0.5f; vecVertex[0].p.z = m_vPosition.z - 0.5f;
	vecVertex[1].p.x = m_vPosition.x - 0.5f; vecVertex[1].p.y = m_vPosition.y + 0.5f; vecVertex[1].p.z = m_vPosition.z - 0.5f;
	vecVertex[2].p.x = m_vPosition.x + 0.5f; vecVertex[2].p.y = m_vPosition.y + 0.5f; vecVertex[2].p.z = m_vPosition.z - 0.5f;
	vecVertex[3].p.x = m_vPosition.x + 0.5f; vecVertex[3].p.y = m_vPosition.y - 0.5f; vecVertex[3].p.z = m_vPosition.z - 0.5f;

	vecVertex[4].p.x = m_vPosition.x - 0.5f; vecVertex[4].p.y = m_vPosition.y - 0.5f; vecVertex[4].p.z = m_vPosition.z + 0.5f;
	vecVertex[5].p.x = m_vPosition.x - 0.5f; vecVertex[5].p.y = m_vPosition.y + 0.5f; vecVertex[5].p.z = m_vPosition.z + 0.5f;
	vecVertex[6].p.x = m_vPosition.x + 0.5f; vecVertex[6].p.y = m_vPosition.y + 0.5f; vecVertex[6].p.z = m_vPosition.z + 0.5f;
	vecVertex[7].p.x = m_vPosition.x + 0.5f; vecVertex[7].p.y = m_vPosition.y - 0.5f; vecVertex[7].p.z = m_vPosition.z + 0.5f;


	//Å©±â°¡ (1,1,1) »óÀÚ ¼¼ÆÃ
	std::vector<DWORD> vecIndex;
	{
		vecIndex.push_back(2);	//¾Õ¸é À­»ï°¢
		vecIndex.push_back(1);
		vecIndex.push_back(0);

		vecIndex.push_back(2);	//¾Õ¸é ¾Æ·§»ï°¢
		vecIndex.push_back(0);
		vecIndex.push_back(3);

		vecIndex.push_back(5);	//µÞ¸é À­»ï°¢
		vecIndex.push_back(6);
		vecIndex.push_back(4);

		vecIndex.push_back(6);	//µÞ¸é ¾Æ·§»ï°¢
		vecIndex.push_back(7);
		vecIndex.push_back(4);

		vecIndex.push_back(1);	//¿Þ¸é À­»ï°¢
		vecIndex.push_back(5);
		vecIndex.push_back(4);

		vecIndex.push_back(1);  //¿Þ¸é ¾Æ·§»ï°¢
		vecIndex.push_back(4);
		vecIndex.push_back(0);

		vecIndex.push_back(6);	//¿À¸¥¸é À­»ï°¢
		vecIndex.push_back(2);
		vecIndex.push_back(3);

		vecIndex.push_back(6);  //¿À¸¥¸é ¾Æ·§»ï°¢
		vecIndex.push_back(3);
		vecIndex.push_back(7);

		vecIndex.push_back(6);	//À­¸é À­»ï°¢
		vecIndex.push_back(5);
		vecIndex.push_back(1);

		vecIndex.push_back(6);	//À­¸é ¾Æ·§»ï°¢
		vecIndex.push_back(1);
		vecIndex.push_back(2);

		vecIndex.push_back(3);	//¾Æ·§¸é À­»ï°¢
		vecIndex.push_back(0);
		vecIndex.push_back(4);

		vecIndex.push_back(3);  //¾Æ·§¸é ¾Æ·§»ï°¢
		vecIndex.push_back(4);
		vecIndex.push_back(7);
	}

	m_vecVertex.resize(36);

	for (size_t i = 0; i < vecIndex.size(); i += 3)
	{
		m_vecVertex[i] = vecVertex[vecIndex[i]];
		m_vecVertex[i + 1] = vecVertex[vecIndex[i + 1]];
		m_vecVertex[i + 2] = vecVertex[vecIndex[i + 2]];
	}

	D3DXVECTOR3 u, v, n;

	for (int i = 0; i < 36; i += 3)
	{
		u = m_vecVertex[i + 1].p - m_vecVertex[i].p;
		v = m_vecVertex[i + 2].p - m_vecVertex[i].p;

		D3DXVec3Cross(&n, &u, &v);
		D3DXVec3Normalize(&n, &n);

		m_vecVertex[i].n = n;
		m_vecVertex[i + 1].n = n;
		m_vecVertex[i + 2].n = n;
	}

	// ¾Õ¸é
	m_vecVertex[0].t.x = (float)0.750f;		m_vecVertex[0].t.y = (float)0.500f;
	m_vecVertex[1].t.x = (float)1.000f;		m_vecVertex[1].t.y = (float)0.500f;
	m_vecVertex[2].t.x = (float)1.000f;		m_vecVertex[2].t.y = (float)0.750f;

	m_vecVertex[3].t.x = (float)0.750f;		m_vecVertex[3].t.y = (float)0.500f;
	m_vecVertex[4].t.x = (float)1.000f;		m_vecVertex[4].t.y = (float)0.750f;
	m_vecVertex[5].t.x = (float)0.750f;		m_vecVertex[5].t.y = (float)0.750f;

	// µÞ¸é
	m_vecVertex[6].t.x = (float)0.250f;		m_vecVertex[6].t.y = (float)0.500f;
	m_vecVertex[7].t.x = (float)0.500f;		m_vecVertex[7].t.y = (float)0.500f;
	m_vecVertex[8].t.x = (float)0.250f;		m_vecVertex[8].t.y = (float)0.750f;

	m_vecVertex[9].t.x = (float)0.500f;		m_vecVertex[9].t.y = (float)0.500f;
	m_vecVertex[10].t.x = (float)0.500f;	m_vecVertex[10].t.y = (float)0.750f;
	m_vecVertex[11].t.x = (float)0.250f;	m_vecVertex[11].t.y = (float)0.750f;

	// ¿Þ¸é
	m_vecVertex[12].t.x = (float)0.000f;	m_vecVertex[12].t.y = (float)0.500f;
	m_vecVertex[13].t.x = (float)0.250f;	m_vecVertex[13].t.y = (float)0.500f;
	m_vecVertex[14].t.x = (float)0.250f;	m_vecVertex[14].t.y = (float)0.750f;

	m_vecVertex[15].t.x = (float)0.000f;	m_vecVertex[15].t.y = (float)0.500f;
	m_vecVertex[16].t.x = (float)0.250f;	m_vecVertex[16].t.y = (float)0.750f;
	m_vecVertex[17].t.x = (float)0.000f;	m_vecVertex[17].t.y = (float)0.750f;

	// ¿À¸¥¸é
	m_vecVertex[18].t.x = (float)0.500f;	m_vecVertex[18].t.y = (float)0.500f;
	m_vecVertex[19].t.x = (float)0.750f;	m_vecVertex[19].t.y = (float)0.500f;
	m_vecVertex[20].t.x = (float)0.750f;	m_vecVertex[20].t.y = (float)0.750f;

	m_vecVertex[21].t.x = (float)0.500f;	m_vecVertex[21].t.y = (float)0.500f;
	m_vecVertex[22].t.x = (float)0.750f;	m_vecVertex[22].t.y = (float)0.750f;
	m_vecVertex[23].t.x = (float)0.500f;	m_vecVertex[23].t.y = (float)0.750f;

	// À­¸é
	m_vecVertex[24].t.x = (float)0.250f;	m_vecVertex[24].t.y = (float)0.250f;
	m_vecVertex[25].t.x = (float)0.500f;	m_vecVertex[25].t.y = (float)0.250f;
	m_vecVertex[26].t.x = (float)0.500f;	m_vecVertex[26].t.y = (float)0.500f;

	m_vecVertex[27].t.x = (float)0.250f;	m_vecVertex[27].t.y = (float)0.250f;
	m_vecVertex[28].t.x = (float)0.500f;	m_vecVertex[28].t.y = (float)0.500f;
	m_vecVertex[29].t.x = (float)0.250f;	m_vecVertex[29].t.y = (float)0.500f;

	// ¾Æ·§¸é
	m_vecVertex[30].t.x = (float)0.250f;	m_vecVertex[30].t.y = (float)0.750f;
	m_vecVertex[31].t.x = (float)0.500f;	m_vecVertex[31].t.y = (float)0.750f;
	m_vecVertex[32].t.x = (float)0.500f;	m_vecVertex[32].t.y = (float)1.000f;

	m_vecVertex[33].t.x = (float)0.250f;	m_vecVertex[33].t.y = (float)0.750f;
	m_vecVertex[34].t.x = (float)0.500f;	m_vecVertex[34].t.y = (float)1.000f;
	m_vecVertex[35].t.x = (float)0.250f;	m_vecVertex[35].t.y = (float)1.000f;
}

void cSkyMap::Update(D3DXVECTOR3 pos)
{
	D3DXMATRIXA16 matS, matT, matView;

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matS);
	D3DXMatrixIdentity(&matT);
	D3DXMatrixScaling(&matS, 300, 300, 300);
	D3DXMatrixTranslation(&matT, pos.x, 0.0f, pos.z);

	m_matWorld = matS * matT;
}

void cSkyMap::Render()
{
	g_pD3DDevice->LightEnable(0, true);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetMaterial(&m_Material);
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PNT_VERTEX));
}
