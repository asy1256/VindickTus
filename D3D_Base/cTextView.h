#pragma once
#include "cUIObject.h"

class cTextView : public cUIObject
{
protected:
	SYNTHESIZE(cFontManager::eFontType, m_eFontType, FontType);
	SYNTHESIZE(std::string, m_sText, Text);
	SYNTHESIZE(D3DCOLOR, m_dwTextColor, TextColor);
	SYNTHESIZE(DWORD, m_dwDrawTextFormat, DrawTextFormat);
	SYNTHESIZE(POINT, m_ptSize, Size);

public:
	cTextView();
	~cTextView();

	virtual void Render(char * szFile, int alpha);
};

