#include "stdafx.h"
#include "cObjectManager.h"
#include "cObjLoader.h"


cObjectManager::cObjectManager()
	: m_pBox(NULL)
{
}


cObjectManager::~cObjectManager()
{
}

void cObjectManager::CreateObject(D3DXVECTOR3 pos, char* szFolder, char* szObj, char* szTex, int type)
{
	std::string fullpath(std::string(szFolder) + std::string("/") + std::string(szObj));

	cObjLoader loader;

	if (m_mapObject.find(fullpath) == m_mapObject.end())
	{
		loader.CreatMeshFromFile(m_mapObject[fullpath], m_mapMinMax[fullpath],fullpath);
	}
	ST_OBJ_DATA obj;
	obj.localPos = pos;
	obj.mesh = m_mapObject[fullpath];
	obj.minmax = &m_mapMinMax[fullpath];
	obj.localPos.y = pos.y + -obj.minmax->vMin.y;
	fullpath = std::string(szFolder) + std::string("/") + std::string(szTex);
	obj.texture = g_pTextureManager->GetTexture(fullpath);
	
	if (!m_pBox)
	{
		D3DXCreateBox(g_pD3DDevice, 2.0f, 2.0f, 2.0f, &m_pBox, 0);
	}
	obj.box = m_pBox;
	obj.bbScalSize.x = obj.minmax->vMax.x / 1.0f;
	obj.bbScalSize.y = obj.minmax->vMax.y / 1.0f;
	obj.bbScalSize.z = obj.minmax->vMax.z / 1.0f;

	m_vecObject.push_back(obj);
	m_vecObjType.push_back(type);
}

void cObjectManager::LoadObjectData(ST_OBJ_DATA& obj, int objNum)
{
	cObjLoader loader;
	std::string objName;
	std::string texName;

	switch (objNum)
	{
	case 0:
	{
		objName = std::string("obj/LageRock01.obj");
		texName = std::string("obj/T_Plains_Rock01_D.tga");
		break;
	}
	case 1:
	{
		objName = std::string("obj/LageRock02.obj");
		texName = std::string("obj/T_Plains_Rock01_D.tga");
		break;
	}
	case 2:
	{
		objName = std::string("obj/LageRock_Cliff01.obj");
		texName = std::string("obj/T_Plains_Cliff01.tga");
		break;
	}
	case 3:
	{
		objName = std::string("obj/LageRock_Cliffside01.obj");
		texName = std::string("obj/T_FlameDungeon_Cliffside01_D.tga");
		break;
	}
	case 4:
	{
		objName = std::string("obj/LageRock_FireRock01.obj");
		texName = std::string("obj/T_Env_Fire_Cliffs_TilingStone_02_D.tga");
		break;
	}
	case 5:
	{
		objName = std::string("obj/Env_Fire_MelancholicTroll_D.obj");
		texName = std::string("obj/Env_Fire_MelancholicTroll_D.tga");
		break;
	}
	case 6:
	{
		objName = std::string("obj/LageRock_FireHeed.obj");
		texName = std::string("obj/Env_Fire_NordHeed_D.tga");
		break;
	}
	case 7:
	{
		objName = std::string("obj/SM_Plains_Column_Small_01.obj");
		texName = std::string("obj/T_Plains_Ruins_Pillars_02_D.tga");
		break;
	}
	case 8:
	{
		objName = std::string("obj/SM_Plains_Column_Mid_01.obj");
		texName = std::string("obj/T_Plains_Ruins_Pillars_03_D.tga");
		break;
	}
	case 9:
	{
		objName = std::string("obj/SM_FlameDungeon_ArchWall01.obj");
		texName = std::string("obj/T_FlameDungeon_Wall01_D.tga");
		break;
	}
	default:
		break;
	}

	if (m_mapObject.find(objName) == m_mapObject.end())
	{
		loader.CreatMeshFromFile(m_mapObject[objName], m_mapMinMax[objName], objName);
	}

	obj.mesh = m_mapObject[objName];
	obj.minmax = &m_mapMinMax[objName];
	obj.texture = g_pTextureManager->GetTexture(texName);
	if (!m_pBox)
	{
		D3DXCreateBox(g_pD3DDevice, 2.0f, 2.0f, 2.0f, &m_pBox, 0);
	}
	obj.box = m_pBox;
}

void cObjectManager::Render()
{
	D3DXMATRIXA16 world, matS, matR, matT;

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	for (int i = 0; i < m_vecObject.size(); ++i)
	{
		D3DXMatrixScaling(&matS, m_vecObject[i].scalsize.x, m_vecObject[i].scalsize.y, m_vecObject[i].scalsize.z);
		D3DXMatrixRotationYawPitchRoll(&matR, m_vecObject[i].rotAxis.y, m_vecObject[i].rotAxis.x, m_vecObject[i].rotAxis.z);
		D3DXMatrixTranslation(&matT, m_vecObject[i].localPos.x, m_vecObject[i].localPos.y, m_vecObject[i].localPos.z);
		world = matS * matR * matT;

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &world);
		g_pD3DDevice->SetTexture(0, m_vecObject[i].texture);
		g_pD3DDevice->SetMaterial(&m_vecObject[i].mtl);
		m_vecObject[i].mesh->DrawSubset(0);
		

		D3DXMatrixScaling(&matS, m_vecObject[i].bbScalSize.x, m_vecObject[i].bbScalSize.y, m_vecObject[i].bbScalSize.z);
		world = matS * matR * matT;

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &world);
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetMaterial(&m_vecObject[i].mtl);
		m_vecObject[i].box->DrawSubset(0);
	}

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void cObjectManager::SaveObject()
{
	HANDLE file, file2, file3;
	DWORD write;

	m_nNumObj = m_vecObject.size();

	ST_OBJ_DATA* objdata = new ST_OBJ_DATA[m_nNumObj];
	int* objtype = new int[m_nNumObj];

	for (int i = 0; i < m_nNumObj; ++i)
	{
		objdata[i] = m_vecObject[i];
		objtype[i] = m_vecObjType[i];
	}

	file = CreateFile(L"objNum.txt", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, &m_nNumObj, sizeof(int), &write, NULL);

	file2 = CreateFile(L"objData.txt", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file2, objdata, sizeof(ST_OBJ_DATA) * m_nNumObj, &write, NULL);

	file3 = CreateFile(L"objType.txt", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file3, objtype, sizeof(int) * m_nNumObj, &write, NULL);

	CloseHandle(file);
	CloseHandle(file2);
	CloseHandle(file3);

	delete[] objdata;
	delete[] objtype;
}

void cObjectManager::LoadObject()
{
	HANDLE file, file2, file3;
	DWORD read;

	file = CreateFile(L"objNum.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, &m_nNumObj, sizeof(int), &read, NULL);

	ST_OBJ_DATA* objdata = new ST_OBJ_DATA[m_nNumObj];
	int* objtype = new int[m_nNumObj];
	m_vecObject.resize(m_nNumObj);
	m_vecObjType.resize(m_nNumObj);

	file2 = CreateFile(L"objData.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file2, objdata, sizeof(ST_OBJ_DATA) * m_nNumObj, &read, NULL);

	file3 = CreateFile(L"objType.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file3, objtype, sizeof(int) * m_nNumObj, &read, NULL);

	CloseHandle(file);
	CloseHandle(file2);
	CloseHandle(file3);

	for (int i = 0; i < m_vecObject.size(); ++i)
	{
		m_vecObject[i] = objdata[i];
		m_vecObjType[i] = objtype[i];
		LoadObjectData(m_vecObject[i], m_vecObjType[i]);
	}

	delete[] objdata;
	delete[] objtype;
}
