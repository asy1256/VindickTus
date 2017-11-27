#include "stdafx.h"
#include "cQuadTree.h"

cQuadTree::cQuadTree(int nWidth, int nHeight)
{
	m_nConer[LT_TOP] = 0;
	m_nConer[RT_TOP] = nWidth - 1;
	m_nConer[LT_BOT] = nWidth * (nHeight - 1);
	m_nConer[RT_BOT] = nWidth * nHeight - 1;

	m_pChild[0] = m_pChild[1] = m_pChild[2] = m_pChild[3] = NULL;
	m_nCenter = (m_nConer[LT_TOP] + m_nConer[RT_TOP] + m_nConer[LT_BOT] + m_nConer[RT_BOT]) / 4;
	m_fRadius = 0.0f;
}

cQuadTree::cQuadTree(cQuadTree* pParant)
{
	m_pVecVertex = pParant->m_pVecVertex;
	m_pChild[0] = m_pChild[1] = m_pChild[2] = m_pChild[3] = NULL;
	m_nCenter = 0;
	m_fRadius = 0.0f;
}

cQuadTree::~cQuadTree()
{
	for (int i = 0; i < NUM_CONER; ++i) { SAFE_DELETE(m_pChild[i]); }
}

void cQuadTree::Setup(std::vector<D3DXVECTOR3>* vecvertex)
{
	m_pVecVertex = vecvertex;
	m_fRadius = D3DXVec3Length(&((*vecvertex)[m_nConer[LT_TOP]] - (*vecvertex)[m_nConer[RT_BOT]])) / 2;

	if (SubDivide())
	{
		m_pChild[LT_TOP]->Setup(vecvertex);
		m_pChild[RT_TOP]->Setup(vecvertex);
		m_pChild[LT_BOT]->Setup(vecvertex);
		m_pChild[RT_BOT]->Setup(vecvertex);
	}
}

bool cQuadTree::SubDivide()
{
	if ((m_nConer[RT_TOP] - m_nConer[LT_TOP]) <= 1) { return false; }

	int left, right, top, bot;

	left = (m_nConer[LT_TOP] + m_nConer[LT_BOT]) / 2;
	right = (m_nConer[RT_TOP] + m_nConer[RT_BOT]) / 2;
	top = (m_nConer[LT_TOP] + m_nConer[RT_TOP]) / 2;
	bot = (m_nConer[LT_BOT] + m_nConer[RT_BOT]) / 2;

	cQuadTree* tree;

	tree = new cQuadTree(this);
	tree->SetConer(m_nConer[LT_TOP], top, left, m_nCenter);
	m_pChild[LT_TOP] = tree;
	tree = new cQuadTree(this);
	tree->SetConer(top, m_nConer[RT_TOP], m_nCenter, right);
	m_pChild[RT_TOP] = tree;
	tree = new cQuadTree(this);
	tree->SetConer(left, m_nCenter, m_nConer[LT_BOT], bot);
	m_pChild[LT_BOT] = tree;
	tree = new cQuadTree(this);
	tree->SetConer(m_nCenter, right, bot, m_nConer[RT_BOT]);
	m_pChild[RT_BOT] = tree;

	return true;
}

void cQuadTree::SetConer(int lt, int rt, int lb, int rb)
{
	m_nConer[LT_TOP] = lt;
	m_nConer[RT_TOP] = rt;
	m_nConer[LT_BOT] = lb;
	m_nConer[RT_BOT] = rb;
	m_nCenter = (lt + rt + lb + rb) / 4;
	m_fRadius = D3DXVec3Length(&((*m_pVecVertex)[lt] - (*m_pVecVertex)[rb])) / 2;
}

void cQuadTree::Picking(Ray& ray, D3DXVECTOR3& pick)
{
	if (HitCheck(ray))
	{
		if (m_nConer[RT_TOP] - m_nConer[LT_TOP] > 1)
		{
			for (int i = 0; i < NUM_CONER; ++i)
			{
				m_pChild[i]->Picking(ray, pick);
			}
		}
		else
		{
			D3DXVECTOR3 lt = (*m_pVecVertex)[m_nConer[LT_TOP]];
			D3DXVECTOR3 rt = (*m_pVecVertex)[m_nConer[RT_TOP]];
			D3DXVECTOR3 lb = (*m_pVecVertex)[m_nConer[LT_BOT]];
			D3DXVECTOR3 rb = (*m_pVecVertex)[m_nConer[RT_BOT]];

			if (D3DXIntersectTri(&lt, &rb, &lb, &ray.Origin, &ray.Direction, NULL, NULL, &pick.y))
			{
				pick = ray.Origin + (ray.Direction * pick.y);
				return;
			}
			if (D3DXIntersectTri(&lt, &rt, &rb, &ray.Origin, &ray.Direction, NULL, NULL, &pick.y))
			{
				pick = ray.Origin + (ray.Direction * pick.y);
				return;
			}
		}
	}
}

bool cQuadTree::HitCheck(Ray& ray)
{
	D3DXVECTOR3 center = ((*m_pVecVertex)[m_nConer[LT_TOP]] + (*m_pVecVertex)[m_nConer[RT_TOP]] +
	(*m_pVecVertex)[m_nConer[LT_BOT]] + (*m_pVecVertex)[m_nConer[RT_BOT]]) / 4;
	D3DXVECTOR3 v = ray.Origin - center;

	float b = 2.0f * D3DXVec3Dot(&ray.Direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (m_fRadius * m_fRadius);

	float discimi = (b * b) - (4.0f * c);

	if (discimi < 0.0f) { return false; }

	discimi = sqrtf(discimi);

	float s0 = (-b + discimi) / 2.0f;
	float s1 = (-b - discimi) / 2.0f;

	if (s0 >= 0.0f || s1 >= 0.0f) { return true; }

	return false;
}
