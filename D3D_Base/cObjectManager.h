#pragma once

#define g_pObjectManager	cObjectManager::GetInstance()

struct ST_BB_MINMAX
{
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	ST_BB_MINMAX()
		: vMin(0, 0, 0)
		, vMax(0, 0, 0)
	{}
};

struct ST_OBJ_DATA
{
	LPD3DXMESH mesh; //오브젝트 메쉬
	LPD3DXMESH box; //바운딩박스 메쉬
	ST_BB_MINMAX* minmax; //맥스, 미니먼 정점
	D3DXVECTOR3 localPos; //오브젝트 위치값
	D3DXVECTOR3 rotAxis; //오브젝트 돌린값
	D3DXVECTOR3 scalsize; //오브젝트 스케일 사이즈
	D3DXVECTOR3 bbScalSize; //바운딩박스 스케일 사이즈
	LPDIRECT3DTEXTURE9 texture; //텍스쳐
	D3DMATERIAL9 mtl; //메테리얼

	ST_OBJ_DATA()
		: mesh(NULL)
		, box(NULL)
		, minmax(NULL)
		, localPos(0, 0, 0)
		, rotAxis(0, 0, 0)
		, scalsize(1, 1, 1)
		, bbScalSize(1, 1, 1)
		, texture(NULL)
	{
		ZeroMemory(&mtl, sizeof(D3DMATERIAL9));
		mtl.Ambient = mtl.Diffuse = mtl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
};

class cObjectManager
{
private:
	SINGLETONE(cObjectManager);

	std::set<cObject*> m_setObject;
	std::map <std::string, LPD3DXMESH> m_mapObject;
	std::map <std::string, ST_BB_MINMAX> m_mapMinMax;
	std::vector<ST_OBJ_DATA> m_vecObject;
	std::vector<int> m_vecObjType;

	LPD3DXMESH m_pBox;

	int m_nNumObj;
	
public:
	void CreateObject(D3DXVECTOR3 pos, char* szFolder, char* szObj, char* szTex, int type);
	void LoadObjectData(ST_OBJ_DATA& obj, int objNum);
	void Render();

	void SaveObject();
	void LoadObject();

	std::vector<ST_OBJ_DATA>& GetObjectData() { return m_vecObject; }

public:
	void AddObject(cObject* pObject)
	{
		m_setObject.insert(pObject);
	}

	void RemoveObject(cObject* pObject)
	{
		m_setObject.erase(pObject);
	}

	void Destroy()
	{
		SAFE_RELEASE(m_pBox);

		for each (auto p in m_mapObject)
		{
			SAFE_RELEASE(p.second);
		}

		assert(m_setObject.empty());
	}
};

