#pragma once
#include "cUIButton.h"

class cUIImageView;
class cUIObject;

class cInventroy : public iButtonDelegate
{
private:
	cUIObject*					m_pRootInvBTN;
	cUIObject*					m_pRootInvIMGV;

	LPD3DXFONT					m_pFont;
public:
	cInventroy();
	~cInventroy();

	void SetUp();
	void Update();
	void Render();

	virtual void OnClick(cUIButton* pSender);
};

