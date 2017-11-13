#pragma once

#include "cEffectWork.h"

class cEffect
{
protected:
	D3DXMATRIXA16 m_matWorld;

public:
	cEffect();
	virtual ~cEffect();

	DWORD FtoDW(float f) { return *(DWORD*)&f; }
};