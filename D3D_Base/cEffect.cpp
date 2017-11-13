#include "stdafx.h"
#include "cEffect.h"

cEffect::cEffect()
{
	D3DXMatrixIdentity(&m_matWorld);
}

cEffect::~cEffect()
{
}
