#pragma once

struct ST_IMG_DATA
{
	D3DXVECTOR3 localPos;
	D3DXIMAGE_INFO imgInfo;
	LPDIRECT3DTEXTURE9 texture;
	RECT rc;
	bool draw;
};

class cToolBar
{
private:
	D3DXVECTOR3 m_vecBasePos;
	std::vector<ST_IMG_DATA> m_vecImgData;
	std::list<ST_IMG_DATA*> m_listData;

	ST_IMG_DATA m_stPanel;
	ST_IMG_DATA m_stMenu;

	bool* m_pIsOpen;

public:
	cToolBar();
	~cToolBar();

	void Setup();
	void Click();
	void Render();

	void SetIsOpen(bool* open) { m_pIsOpen = open; }
};
