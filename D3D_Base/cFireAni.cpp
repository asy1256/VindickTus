#include "stdafx.h"
#include "cFireAni.h"

cFireAni::cFireAni()
{
}

cFireAni::~cFireAni()
{
}

void cFireAni::Setup(int maxparticle, float effectrange)
{
	m_nNumMakePtc = 0;
	m_fCurTime = 0.0f;
	m_fDelyTime = 0.3f;
	m_fEffectRange = effectrange;
	m_isMake = false;

	//�ؽ����� �ƽ�������
	m_nFrameX = 8;
	m_nFrameY = 4;
	//����� �ؽ��� �ҷ�����
	m_pTexture = g_pTextureManager->GetAniFxTexture(std::string("fx/flare2.png"));

	//�ƽ���ƼŬ��ŭ ���� ��������
	m_vecAniData.resize(maxparticle);

	ST_ANI_DATA temp; //���Ϳ� ���� �ִϸ��̼� ������ ����ü ����
	temp.fLifeTime = 2.0f; //��ƼŬ�� ����ִ� �ð�
						   //���� ��ġ
	temp.vertex[0].p = D3DXVECTOR3(-0.5f, 2.0f, 0.0f);
	temp.vertex[1].p = D3DXVECTOR3(+0.5f, 2.0f, 0.0f);
	temp.vertex[2].p = D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
	temp.vertex[3].p = D3DXVECTOR3(+0.5f, 0.0f, 0.0f);
	float rotY; //��ƼŬ ��ġ�� Y������ ������ ����� Y�� ������
	D3DXMATRIXA16 matR; //Y�� ȸ������� �̿��ؼ� �������� ������� ��Ʈ����

						//�ƽ���ƼŬ ������ŭ ���Ϳ� �־��ֱ�
	for (int i = 0; i < maxparticle; ++i)
	{
		temp.vPos.x = GETFLOAT(3.0f);
		rotY = GETFLOAT(D3DX_PI * 2);
		D3DXMatrixRotationY(&matR, rotY);
		D3DXVec3TransformNormal(&temp.vPos, &temp.vPos, &matR);

		temp.vertex[0].t = D3DXVECTOR2((float)temp.nCurFrameX / m_nFrameX, (float)temp.nCurFrameY / m_nFrameY);
		temp.vertex[1].t = D3DXVECTOR2((float)(temp.nCurFrameX + 1) / m_nFrameX, (float)temp.nCurFrameY / m_nFrameY);
		temp.vertex[2].t = D3DXVECTOR2((float)temp.nCurFrameX / m_nFrameX, (float)(temp.nCurFrameY + 1) / m_nFrameY);
		temp.vertex[3].t = D3DXVECTOR2((float)(temp.nCurFrameX + 1) / m_nFrameX, (float)(temp.nCurFrameY + 1) / m_nFrameY);

		m_vecAniData[i] = temp;
	}

	//���ؽ� ���� �����
	g_pD3DDevice->CreateVertexBuffer(4 * sizeof(ST_PT_VERTEX), D3DUSAGE_DYNAMIC,
		ST_PT_VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVb, 0);

	//�ε��� ���� �����
	g_pD3DDevice->CreateIndexBuffer(6 * sizeof(WORD), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIb, 0);
	//�ε��� ���� �ᱺ��
	WORD* ib;
	m_pIb->Lock(0, 0, (void**)&ib, 0);
	//�ε��� ���ۿ� �ε��� ä���
	ib[0] = 2;
	ib[1] = 0;
	ib[2] = 1;
	ib[3] = 2;
	ib[4] = 1;
	ib[5] = 3;
	//�ε��� ���� Ǯ���ش�
	m_pIb->Unlock();
}

void cFireAni::Update()
{
	//���������� ��ƼŬ�� ���� �������� �ʾ�����
	if (!m_isMake)
	{
		//���� üũŸ�� ����
		m_fCurTime += g_pTimeManager->GetEllapsedTime();
		//����Ÿ���� ������Ÿ�Ӻ��� ũ��
		if (m_fCurTime > m_fDelyTime)
		{
			//����Ÿ�� 0.0f�� �ʱ�ȭ �� �ش� ���� ��ƼŬ�� �츰��
			m_fCurTime = 0.0f;
			m_vecAniData[m_nNumMakePtc].isAlive = true;
			//�ش��ϴ� ��ƼŬ �츰�� �ε����� ++
			++m_nNumMakePtc;
			//���� ���� ��ƼŬ�� �ε������� ��ƼŬ ���� ������� ������ ���λ��� true
			if (m_nNumMakePtc == m_vecAniData.size()) { m_isMake = true; }
		}
	}
	//��ƼŬ ���͸� ������ ��ƼŬ �ε��� ����ŭ ����.
	for (int i = 0; i < m_nNumMakePtc; ++i)
	{
		//���� �ش� ��ƼŬ�� �׾������� �츮�� ��Ƽ��
		if (!m_vecAniData[i].isAlive) { m_vecAniData[i].isAlive = true; continue; }
		//�ش���ƼŬ�� ���ӽð��� �ִ�����ð��� �Ѿ�� ����
		if (m_vecAniData[i].fAge > m_vecAniData[i].fLifeTime) { ResetParticle(m_vecAniData[i]); continue; }
		//�ش���ƼŬ ���ӽð� ����
		m_vecAniData[i].fAge += g_pTimeManager->GetEllapsedTime();
		//������ī��Ʈ ����
		++m_vecAniData[i].nFrameCount;
		//���� �ش���ƼŬ ������ī��Ʈ�� 3���� ũ�ų� ������
		if (m_vecAniData[i].nFrameCount >= 3)
		{
			m_vecAniData[i].nFrameCount = 0; //������ī��Ʈ�� 0���� �ʱ�ȭ
											 //���� ����Y�������� �ƽ�Y�����Ӱ� ������ ����Y������ 0���� �ʱ�ȭ
			if (m_vecAniData[i].nCurFrameY == m_nFrameY) { m_vecAniData[i].nCurFrameY = 0; }
			//�Ǵ� ����X�������� �ƽ�X�����Ӱ� ������ ����Y������ ����
			else if (m_vecAniData[i].nCurFrameX == m_nFrameX) { ++m_vecAniData[i].nCurFrameY; }
			//���� ����X�������� �ƽ�X�����Ӱ� ������ ����X������ 0���� �ʱ�ȭ
			if (m_vecAniData[i].nCurFrameX == m_nFrameX) { m_vecAniData[i].nCurFrameX = 0; }
			//�װ� �ƴ϶�� ����X������ ����
			else { ++m_vecAniData[i].nCurFrameX; }
			//���� ����X�������� �ƽ�X�������� �ƴҶ�
			if (m_vecAniData[i].nCurFrameX != m_nFrameX)
			{
				//���ؽ� ������ uv��ǥ�� ����X,Y������ �������� ����
				m_vecAniData[i].vertex[0].t = D3DXVECTOR2((float)m_vecAniData[i].nCurFrameX / m_nFrameX, (float)m_vecAniData[i].nCurFrameY / m_nFrameY);
				m_vecAniData[i].vertex[1].t = D3DXVECTOR2((float)(m_vecAniData[i].nCurFrameX + 1) / m_nFrameX, (float)m_vecAniData[i].nCurFrameY / m_nFrameY);
				m_vecAniData[i].vertex[2].t = D3DXVECTOR2((float)m_vecAniData[i].nCurFrameX / m_nFrameX, (float)(m_vecAniData[i].nCurFrameY + 1) / m_nFrameY);
				m_vecAniData[i].vertex[3].t = D3DXVECTOR2((float)(m_vecAniData[i].nCurFrameX + 1) / m_nFrameX, (float)(m_vecAniData[i].nCurFrameY + 1) / m_nFrameY);
			}
		}
	}
}

void cFireAni::Render()
{
	cBillbordEffect::PreRender();
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVb, 0, sizeof(ST_PT_VERTEX));
	g_pD3DDevice->SetIndices(m_pIb);

	for (int i = 0; i < m_nNumMakePtc; ++i)
	{
		if (m_vecAniData[i].isAlive)
		{
			m_matWorld._41 = m_vecAniData[i].vPos.x;
			m_matWorld._42 = m_vecAniData[i].vPos.y;
			m_matWorld._43 = m_vecAniData[i].vPos.z;
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

			ST_PT_VERTEX* vb;
			m_pVb->Lock(0, 0, (void**)&vb, 0);

			vb[0] = m_vecAniData[i].vertex[0];
			vb[1] = m_vecAniData[i].vertex[1];
			vb[2] = m_vecAniData[i].vertex[2];
			vb[3] = m_vecAniData[i].vertex[3];

			m_pVb->Unlock();

			g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
				4 * sizeof(ST_PT_VERTEX), 0, 2);
		}
	}

	cBillbordEffect::PostRender();
}

void cFireAni::ResetParticle(ST_ANI_DATA& data)
{
	data.isAlive = false;
	data.nCurFrameX = 0;
	data.nCurFrameY = 0;
	data.fAge = 0.0f;
	data.vPos = D3DXVECTOR3(GETFLOAT(m_fEffectRange), 0.0f, 0.0f);

	float rotY = GETFLOAT(D3DX_PI * 2);
	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, rotY);
	D3DXVec3TransformNormal(&data.vPos, &data.vPos, &matR);
}
