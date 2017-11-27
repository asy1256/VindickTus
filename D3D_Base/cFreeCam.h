#pragma once

#include "cCamera.h"

class cFreeCam : public cCamera
{
private:
	D3DXMATRIXA16 m_matProj;
	D3DXMATRIXA16 m_matView;

	D3DXVECTOR3 m_vCross;
	D3DXVECTOR3 m_vView;

public:
	cFreeCam();
	virtual ~cFreeCam();

	virtual void Setup();
	virtual void Update();
	virtual void Update(D3DXVECTOR3 look);
	float GetAngle(void) { return m_vCamRotAngle.y; }
	D3DXMATRIXA16* SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp);

	// 월드좌표계의 *pv값의 위치로 이동한다.
	D3DXMATRIXA16*	MoveTo(D3DXVECTOR3* pv);

	// 카메라 좌표계의 X축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
	D3DXMATRIXA16*	MoveLocalX(float dist);

	// 카메라 좌표계의 Y축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
	D3DXMATRIXA16*	MoveLocalY(float dist);

	// 카메라 좌표계의 Z축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
	D3DXMATRIXA16*	MoveLocalZ(float dist);

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	D3DXMATRIXA16* GetView() { return &m_matView; }
	D3DXMATRIXA16* GetProj() { return &m_matProj; }

	D3DXMATRIXA16 GetViewPorj() { return m_matView*m_matProj; }
};