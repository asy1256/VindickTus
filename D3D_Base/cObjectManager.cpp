#include "stdafx.h"
#include "cObjectManager.h"
#include "cObjLoader.h"


cObjectManager::cObjectManager()
{
}


cObjectManager::~cObjectManager()
{
}

void cObjectManager::CreateObject(D3DXVECTOR3 pos, char* szFolder, char* szObj, char* szTex)
{
	std::string fullpath(std::string(szFolder) + std::string("/") + std::string(szObj));

	cObjLoader loader;

	if (m_mapObject.find(fullpath) == m_mapObject.end())
	{
		loader.CreatMeshFromFile(m_mapObject[fullpath], szFolder, szObj);
	}
	ST_OBJ_DATA obj;
	obj.localPos = pos;
	obj.mesh = m_mapObject[fullpath];
	fullpath = std::string(szFolder) + std::string("/") + std::string(szTex);
	obj.texture = g_pTextureManager->GetTexture(fullpath);

	m_vecObject.push_back(obj);
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

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &world);
		g_pD3DDevice->SetTexture(0, m_vecObject[i].texture);
		g_pD3DDevice->SetMaterial(&m_vecObject[i].mtl);
		
		m_vecObject[i].mesh->DrawSubset(0);
	}

}
