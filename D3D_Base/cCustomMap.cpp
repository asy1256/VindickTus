#include "stdafx.h"
#include "cCustomMap.h"
#include "cQuadTree.h"

cCustomMap::cCustomMap()
	: m_pMesh(NULL)
	, m_pTexture(NULL)
	, m_pQuadTree(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCustomMap::~cCustomMap()
{
	Destroy();
}

void cCustomMap::Setup(int nWidth, int nHeight, float fTileSize)
{
	g_fTileSize = fTileSize;			//�� �������� �Ÿ�
	m_nRow = nWidth;					//�ϳ��� ���̳� �࿡ �����ϴ� ���� ����
										//m_nRow - 1 == �ϳ��� ���̳� �࿡ �ִ� �簢���� ����
	int nNumVertex = nWidth * nHeight;	//���� �����ϴ� ��� ������ ����

	std::vector<ST_PNT_VERTEX> vecVertex;
	vecVertex.resize(nNumVertex);
	m_vecVertex.resize(nNumVertex);

	std::vector<DWORD> vecIndex;
	vecIndex.resize((m_nRow - 1) * (m_nRow - 1) * 2 * 3);

	//���� ����ֱ�
	for (int i = 0; i < nNumVertex; i++)
	{
		ST_PNT_VERTEX v;
		v.p = D3DXVECTOR3((i % m_nRow) * g_fTileSize, 0.0f, (i / m_nRow) * g_fTileSize);
		v.n = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2((i % m_nRow) / (float)m_nRow,
			(i / m_nRow) / (float)m_nRow);
		vecVertex[i] = v;
		m_vecVertex[i] = v.p;
	}
	//�ε��� ����ֱ�
	for (int x = 0; x < m_nRow - 1; x++)
	{
		for (int z = 0; z < m_nRow - 1; z++)
		{
			int _0 = (z + 0) * m_nRow + (x + 0);
			int _1 = (z + 1) * m_nRow + (x + 0);
			int _2 = (z + 0) * m_nRow + (x + 1);
			int _3 = (z + 1) * m_nRow + (x + 1);

			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);

			vecIndex.push_back(_3);
			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
		}
	}
	//�븻 ����ֱ�
	for (int x = 1; x < m_nRow - 1; x++)
	{
		for (int z = 1; z < m_nRow - 1; z++)
		{
			int nLeft = (z + 0) * m_nRow + (x - 1);
			int nRight = (z + 0) * m_nRow + (x + 1);
			int nUp = (z + 1) * m_nRow + (x + 0);
			int nDown = (z - 1) * m_nRow + (x + 0);

			D3DXVECTOR3	vLeftToRight = vecVertex[nRight].p - vecVertex[nLeft].p;
			D3DXVECTOR3	vDownToUp = vecVertex[nUp].p - vecVertex[nDown].p;
			D3DXVECTOR3	vNormal;
			D3DXVec3Cross(&vNormal, &vDownToUp, &vLeftToRight);
			D3DXVec3Normalize(&vNormal, &vNormal);

			int nIndex = z * m_nRow + x;
			vecVertex[nIndex].n = vNormal;
		}
	}
	//�޽� �����
	{
		D3DXCreateMeshFVF(vecIndex.size() / 3,
			vecVertex.size(),
			D3DXMESH_MANAGED | D3DXMESH_32BIT,
			ST_PNT_VERTEX::FVF,
			g_pD3DDevice,
			&m_pMesh);
	}
	//�������ۿ� �����ϱ�
	{
		ST_PNT_VERTEX* pV = NULL;
		m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
		memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
		m_pMesh->UnlockVertexBuffer();
	}
	//�ε������ۿ� �����ϱ�
	{
		DWORD* pDw = NULL;
		m_pMesh->LockIndexBuffer(0, (LPVOID*)&pDw);
		memcpy(pDw, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
		m_pMesh->UnlockIndexBuffer();
	}
	//�Ӽ� �ʱ�ȭ���ֱ�
	{
		DWORD* pDw = NULL;
		m_pMesh->LockAttributeBuffer(0, &pDw);
		ZeroMemory(pDw, (vecIndex.size() / 3) * sizeof(DWORD));
		m_pMesh->UnlockAttributeBuffer();
	}
	//�޽� ����ȭ�ϱ�
	{
		std::vector<DWORD> vecAdj(vecIndex.size());
		m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

		m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT |
			D3DXMESHOPT_COMPACT |
			D3DXMESHOPT_VERTEXCACHE,
			&vecAdj[0],
			0, 0, 0);
	}
	//���׸��� & �ؽ��� ����
	{
		ZeroMemory(&m_mtl, sizeof(D3DMATERIAL9));
		m_mtl.Ambient = m_mtl.Diffuse = m_mtl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		m_pTexture = g_pTextureManager->GetTexture(std::string("terrain/K_BlueCaustic002_emis2.jpg"));
	}
	//�� ����Ʈ�� ����
	m_pQuadTree = new cQuadTree(nWidth, nHeight);
	m_pQuadTree->Setup(&m_vecVertex);
}

void cCustomMap::Update()
{
}

void cCustomMap::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetMaterial(&m_mtl);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	m_pMesh->DrawSubset(0);
}

void cCustomMap::Destroy()
{
	SAFE_DELETE(m_pQuadTree);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pMesh);
}

void cCustomMap::Picking(Ray & ray, D3DXVECTOR3 & pick)
{
	m_pQuadTree->Picking(ray, pick);
}
