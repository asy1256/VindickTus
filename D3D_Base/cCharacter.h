#pragma once

class cAction;

struct UnitInfo
{
	int			nMaxHP;			// 최대 HP
	int			nCurrHP;		// HP

	int			nMaxSP;			// 최대 SP
	int			nCurrSP;		// SP

	int			nAttack;		// 공격력
	int			nDefense;		// 방어력

	int			nDetectionRange;// 상대방 탐지 거리
	int			nAttackRange;	// 공격 범위

	float		fSpeed;			// 속도

	bool		m_bDie;			// 죽었는지 살았는지

	UnitInfo() 
		: m_bDie(false)
	{

	}
	
};

class cCharacter : public cGameObject
{
protected:
	float			m_fRotY;
	D3DXMATRIXA16	m_matWorldTM;

	SYNTHESIZE(cAction*, m_pAction, Action);


public:
	cCharacter();
	virtual ~cCharacter();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnActionFinish(cAction* pSender) override;
};

