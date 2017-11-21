#pragma once

enum TAG_CONER
{
	LT_TOP,
	RT_TOP,
	LT_BOT,
	RT_BOT,
	NUM_CONER
};

class cQuadTree
{
private:
	int m_nCenter;
	int m_nConer[NUM_CONER];
	bool m_isCual;
	float m_fRadius;
	std::vector<D3DXVECTOR3>* m_pVecVertex;

private:
	cQuadTree* m_pChild[4];

public:
	cQuadTree(int nWidth, int nHeight);
	cQuadTree(cQuadTree* pParant);
	~cQuadTree();

	void Setup(std::vector<D3DXVECTOR3>* vecvertex);
	bool SubDivide();
	void SetConer(int lt, int rt, int lb, int rb);
	void Picking(Ray& ray, D3DXVECTOR3& pick);
	bool HitCheck(Ray& ray);
};