#pragma once
#include "cUIObject.h"

class cUIImageView : public cUIObject
{
protected:
	//LPDIRECT3DTEXTURE9		m_pTexture;

	

public:
	cUIImageView();
	virtual ~cUIImageView();

	void SetTexture(char * szFile, char* szFullPath );
	virtual void Update();
	virtual void Render(char * szFile, int alpha) override;
};

