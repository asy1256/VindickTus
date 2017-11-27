#pragma once
class cCamera
{
protected:
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3	m_vLookAt;
	D3DXVECTOR3	m_vUp;

	POINT		m_ptPrevMouse;
	bool		m_isLButtonDown;
	float		m_fCameraDistance;
	D3DXVECTOR3	m_vCamRotAngle;

public:
	cCamera();
	virtual ~cCamera();

	virtual void Setup();
	virtual void Update();
	virtual void Update(D3DXVECTOR3 look);

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

