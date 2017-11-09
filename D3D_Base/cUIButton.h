#pragma once
#include "cUIObject.h"

class cUIButton;
class iButtonDelegate
{
public:
	virtual void OnClick(cUIButton* pSender) = 0;
};

class cUIButton : public cUIObject
{
public:
	enum eButtonState
	{
		BTN_NORMAL,
		BTN_MOUSESTAY,
		BTN_CLICKED,
		BTN_NUM_STATE
	};
protected:
	SYNTHESIZE(eButtonState, m_eButtonState, ButtonState);

	SYNTHESIZE(iButtonDelegate*, m_pDelegate, Delegate);
	
	RECT					m_rcButton;

public:
	cUIButton();
	virtual ~cUIButton();

	void SetTexture( char * szFile, char* szFullPath );
	virtual void Update();
	virtual void Render(char * szFile, int alpha) override;
};

