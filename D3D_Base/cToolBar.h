#pragma once

struct ST_IMG_DATA
{
	D3DXVECTOR3 localPos;
	D3DXIMAGE_INFO imgInfo;
	LPDIRECT3DTEXTURE9 texture;
	RECT rc;
	bool draw;
};

enum TAG_OBJECT
{
	OBJECT_ROCK1,
	OBJECT_ROCK2,
	OBJECT_ROCK3,
	OBJECT_ROCK4,
	OBJECT_ROCK5,
	OBJECT_STATUE1,
	OBJECT_STATUE2,
	OBJECT_PIILER1,
	OBJECT_PIILER2,
	OBJECT_WALL,
	NUM_OBJECT
};

class cToolBar
{
private:
	D3DXVECTOR3 m_vecBasePos;
	std::list<ST_IMG_DATA*> m_listData;

	ST_IMG_DATA m_stPanel;
	ST_IMG_DATA m_stObjectBtn;
	ST_IMG_DATA m_stMenuBtn;
	ST_IMG_DATA m_stTagButton[3];
	ST_IMG_DATA m_stEnvSample[5];
	ST_IMG_DATA m_stStcSample[5];
	ST_IMG_DATA m_stSaLo[2];

	bool* m_pIsOpen;
	int* m_pNowselect;

public:
	cToolBar();
	~cToolBar();

	void Setup();
	void Click();
	void Render();

	void SetButton();
	void Save();
	void Load();

	void SetIsOpen(bool* open) { m_pIsOpen = open; }
	void SetNowSelect(int* now) { m_pNowselect = now; }
};
