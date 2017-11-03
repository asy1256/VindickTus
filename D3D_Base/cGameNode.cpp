#include "stdafx.h"
#include "cGameNode.h"

cGameNode::cGameNode()
	: m_pCamera(NULL)
{
}

cGameNode::~cGameNode()
{

}

HRESULT cGameNode::Setup()
{
	return S_OK;
}

void cGameNode::Update()
{
}

void cGameNode::Render()
{
}

void cGameNode::Destroy()
{
	SAFE_DELETE(m_pCamera);
}

void cGameNode::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
