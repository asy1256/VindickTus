#include "stdafx.h"
#include "cUIObject.h"


cUIObject::cUIObject()
	:m_bIsActivation(false)
	, m_vPosition(0, 0, 0)
	, m_pParent(NULL)
{
}


cUIObject::~cUIObject()
{
}

void cUIObject::AddChild(cUIObject * pChild)
{
	pChild->SetParent(this);
	m_vecChild.push_back(pChild);
}

void cUIObject::Release()
{
	if (!m_vecChild.empty())
	{
		for each (auto p in m_vecChild)
		{
			SAFE_DELETE(p);
		}
	}
}

void cUIObject::Update()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._41 = m_vPosition.x;
	m_matWorld._42 = m_vPosition.y;
	m_matWorld._43 = m_vPosition.z;

	if (m_pParent)
	{
		m_matWorld._41 += m_pParent->m_matWorld._41;
		m_matWorld._42 += m_pParent->m_matWorld._42;
		m_matWorld._43 += m_pParent->m_matWorld._43;
	}

	if (!m_vecChild.empty())
	{
		for each (auto child in m_vecChild)
		{
			child->Update();
		}
	}
}

void cUIObject::Render(char * szFile, int alpha)
{
	if (!m_vecChild.empty())
	{
		for each (auto child in m_vecChild)
		{
			child->Render(child->m_cFile, alpha);
		}
	}
}

void cUIObject::SetPosition(float x, float y, float z)
{
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;
}

cUIObject * cUIObject::FindChildByTag(int tag)
{
	if (m_nTag == tag)
		return this;

	else
	{
		for each(auto child in m_vecChild)
		{
			cUIObject* p = child->FindChildByTag(tag);

			if (p)
			{
				return p;
			}
		}
	}

	return nullptr;
}
