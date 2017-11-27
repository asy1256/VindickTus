#include "stdafx.h"
#include "cFreeCam.h"

cFreeCam::cFreeCam()
{
}

cFreeCam::~cFreeCam()
{
}

void cFreeCam::Setup()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);


	D3DXMatrixPerspectiveFovLH(&m_matProj,
		D3DX_PI / 4.0f,
		rc.right / (float)rc.bottom,
		1.0f,
		300000);

	POINT  ptCurrMouse;

	SetCursor(NULL); // 마우스를 나타나지 않게 않다.
	GetClientRect(g_hWnd, &rc);
	ptCurrMouse.x = (rc.right - rc.left) / 2;
	ptCurrMouse.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(g_hWnd, &ptCurrMouse);
	SetCursorPos(ptCurrMouse.x, ptCurrMouse.y);
	m_ptPrevMouse.x = ptCurrMouse.x;
	m_ptPrevMouse.y = ptCurrMouse.y;

	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void cFreeCam::Update()
{
	D3DXMATRIXA16	matR, matRX, matRY;
	D3DXMatrixRotationX(&matRX, m_vCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, m_vCamRotAngle.y);

	matR = matRX * matRY;


	m_vEye = D3DXVECTOR3(0, 0, m_fCameraDistance);

	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);

	m_vEye = m_vEye + m_vLookAt;

	SetView(&m_vEye, &m_vLookAt, &m_vUp);

	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void cFreeCam::Update(D3DXVECTOR3 look)
{
}

D3DXMATRIXA16 * cFreeCam::SetView(D3DXVECTOR3 * pvEye, D3DXVECTOR3 * pvLookat, D3DXVECTOR3 * pvUp)
{
	m_vEye = *pvEye;
	m_vLookAt = *pvLookat;
	m_vUp = *pvUp;

	D3DXMATRIXA16 matBill;
	D3DXVec3Normalize(&m_vView, &(m_vLookAt - m_vEye));
	D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookAt, &m_vUp);
	D3DXMatrixInverse(&matBill, NULL, &m_matView);
	matBill._41 = 0.0f;
	matBill._42 = 0.0f;
	matBill._43 = 0.0f;

	return &m_matView;
}

D3DXMATRIXA16 * cFreeCam::MoveTo(D3DXVECTOR3 * pv)
{
	D3DXVECTOR3	dv = *pv - m_vEye;
	m_vEye = *pv;
	m_vLookAt += dv;
	return SetView(&m_vEye, &m_vLookAt, &m_vUp);
}

D3DXMATRIXA16 * cFreeCam::MoveLocalX(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookAt;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vCross);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16 * cFreeCam::MoveLocalY(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookAt;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vUp);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16 * cFreeCam::MoveLocalZ(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookAt;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vView);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

void cFreeCam::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		m_isLButtonDown = true;
		break;
	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		if (m_isLButtonDown)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);
			float fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
			float fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;
			m_vCamRotAngle.x -= (fDeltaY / 100.0f);
			m_vCamRotAngle.y += (fDeltaX / 100.0f);

			m_ptPrevMouse = ptCurrMouse;
		}
		break;
	case WM_MOUSEWHEEL:
		m_fCameraDistance -= (GET_WHEEL_DELTA_WPARAM(wParam) / 30.0f);
		if (m_fCameraDistance < 0.001f)
			m_fCameraDistance = 0.001f;
		break;
	}
}
