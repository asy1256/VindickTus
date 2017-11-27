#pragma once

#define g_pObjectManager	cObjectManager::GetInstance()

struct ST_OBJ_DATA
{
	LPD3DXMESH mesh;
	LPD3DXMESH box;
	D3DXVECTOR3 localPos;
	D3DXVECTOR3 rotAxis;
	D3DXVECTOR3 scalsize;
	LPDIRECT3DTEXTURE9 texture;
	D3DMATERIAL9 mtl;

	ST_OBJ_DATA()
		: mesh(NULL)
		, box(NULL)
		, localPos(0, 0, 0)
		, rotAxis(0, 0, 0)
		, scalsize(1, 1, 1)
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
	std::vector<ST_OBJ_DATA> m_vecObject;
	std::vector<int> m_vecObjType;

	LPD3DXMESH m_pBox;
	LPD3DXMESH m_pSphere;

	int m_nNumObj;
	
public:
	void CreateObject(D3DXVECTOR3 pos, char* szFolder, char* szObj, char* szTex, int type);
	void LoadObjectData(ST_OBJ_DATA& obj, int objNum);
	void Render();

	void SaveObject();
	void LoadObject();

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

		for each (auto p in m_mapObject)
		{
			SAFE_RELEASE(p.second);
		}

		assert(m_setObject.empty());
	}
};

