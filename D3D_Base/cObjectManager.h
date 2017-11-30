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
	LPD3DXMESH mesh; //������Ʈ �޽�
	LPD3DXMESH box; //�ٿ���ڽ� �޽�
	ST_BB_MINMAX* minmax; //�ƽ�, �̴ϸ� ����
	D3DXVECTOR3 localPos; //������Ʈ ��ġ��
	D3DXVECTOR3 rotAxis; //������Ʈ ������
	D3DXVECTOR3 scalsize; //������Ʈ ������ ������
	D3DXVECTOR3 bbScalSize; //�ٿ���ڽ� ������ ������
	LPDIRECT3DTEXTURE9 texture; //�ؽ���
	D3DMATERIAL9 mtl; //���׸���

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

