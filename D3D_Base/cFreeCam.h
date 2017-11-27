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

	// ������ǥ���� *pv���� ��ġ�� �̵��Ѵ�.
	D3DXMATRIXA16*	MoveTo(D3DXVECTOR3* pv);

	// ī�޶� ��ǥ���� X��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalX(float dist);

	// ī�޶� ��ǥ���� Y��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalY(float dist);

	// ī�޶� ��ǥ���� Z��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalZ(float dist);

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	D3DXMATRIXA16* GetView() { return &m_matView; }
	D3DXMATRIXA16* GetProj() { return &m_matProj; }

	D3DXMATRIXA16 GetViewPorj() { return m_matView*m_matProj; }
};