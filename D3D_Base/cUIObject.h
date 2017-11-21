#pragma once
#include "cObject.h"

class cUIObject : public cObject
{
protected:
	std::vector<cUIObject*>		m_vecChild;

	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE_REF(D3DXMATRIXA16, m_matWorld, World);
	SYNTHESIZE(bool, m_bIsActivation, Activation);
	SYNTHESIZE(int, m_nTag, Tag);

public:
	cUIObject();
	virtual ~cUIObject();
	char *						m_cFile;

	void AddChild( cUIObject* pChild );
	virtual void Release();
	virtual void Update();
	virtual void Render(char * szFile, int alpha);
	virtual void SetPosition(float x, float y, float z = 0);

	cUIObject* FindChildByTag(int tag);
};

