#pragma once

class cGroup;
class cMtlTex;
class cAseNode;

class cAseLoader
{
private:
	//�о���� ���
	std::map<std::string, cAseNode*>	m_mapNode;
	//������ ���׸���&�ؽ���
	std::vector<cMtlTex*>				m_vecMtlTex;
	//�о ���� ���ؽ� ����
	std::vector<D3DXVECTOR3>			m_vecVertex;
	//�о �ؽ��� uv ����
	std::vector<D3DXVECTOR2>			m_vecTver;
	int									m_nTverSize;
	//�о �븻 ����
	std::vector<D3DXVECTOR3>			m_vecNomal;
	//�ǳ��� ���ؽ����� ���빰
	std::vector<ST_PNT_VERTEX>			m_vecVerBuff;
	//�ﰢ���� �̷�� �ε����� ��� ����
	std::vector<ST_TNGLE_IDX>			m_vecIndex;
	//�̹��� �̸�
	std::string							m_strTex;
	//��� �̸�
	std::string							m_strNodeName;
	std::string							m_strParentsNodeName;
	//������
	DWORD								m_dwFirstFrame;
	DWORD								m_dwLastFrame;
	DWORD								m_dwFrameSpeed;
	DWORD								m_dwTicksPerFrame;
	//����
	FILE*								m_fp;
public:
	cAseLoader();
	~cAseLoader();

	cAseNode* LoadFile(char* szFolder, char* szFile);

private:
	void ReadSkip();
	void ReadScene();
	void ReadMaterialList();
	void ReadMaterial(int& index);
	void ReadMapDiffuse(int& index);
	void ReadGeometry();
	void ReadMesh();
	void ReadNodeTM();
	void ReadVertexList();
	void ReadFaceList();
	void ReadTverList();
	void ReadTfaceList();
	void ReadNomals();
	void ReadTmAnimation();
	void ReadCtrlPosTrack(std::vector<ST_ANI_POS>& vecpos);
	void ReadCtrlRotTrack(std::vector<ST_ANI_ROT>& vecrot);
	void ReadCtrlRotTcb();
};

