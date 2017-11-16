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

	//텍스쳐의 맥스프레임
	m_nFrameX = 8;
	m_nFrameY = 4;
	//사용할 텍스쳐 불러오기
	m_pTexture = g_pTextureManager->GetAniFxTexture(std::string("fx/flare2.png"));

	//맥스파티클만큼 벡터 리사이즈
	m_vecAniData.resize(maxparticle);

	ST_ANI_DATA temp; //벡터에 넣을 애니메이션 데이터 구조체 선언
	temp.fLifeTime = 2.0f; //파티클이 살아있는 시간
						   //정점 위치
	temp.vertex[0].p = D3DXVECTOR3(-0.5f, 2.0f, 0.0f);
	temp.vertex[1].p = D3DXVECTOR3(+0.5f, 2.0f, 0.0f);
	temp.vertex[2].p = D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
	temp.vertex[3].p = D3DXVECTOR3(+0.5f, 0.0f, 0.0f);
	float rotY; //파티클 위치를 Y축으로 돌려서 잡아줄 Y축 각도값
	D3DXMATRIXA16 matR; //Y축 회전행렬을 이용해서 포지션을 만들어줄 매트릭스

						//맥스파티클 갯수만큼 벡터에 넣어주기
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

	//버텍스 버퍼 만들기
	g_pD3DDevice->CreateVertexBuffer(4 * sizeof(ST_PT_VERTEX), D3DUSAGE_DYNAMIC,
		ST_PT_VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVb, 0);

	//인덱스 버퍼 만들고
	g_pD3DDevice->CreateIndexBuffer(6 * sizeof(WORD), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIb, 0);
	//인덱스 버퍼 잠군다
	WORD* ib;
	m_pIb->Lock(0, 0, (void**)&ib, 0);
	//인덱스 버퍼에 인덱스 채우고
	ib[0] = 2;
	ib[1] = 0;
	ib[2] = 1;
	ib[3] = 2;
	ib[4] = 1;
	ib[5] = 3;
	//인덱스 버퍼 풀어준다
	m_pIb->Unlock();
}

void cFireAni::Update()
{
	//순차적으로 파티클이 전부 생성되지 않았으면
	if (!m_isMake)
	{
		//생성 체크타임 증가
		m_fCurTime += g_pTimeManager->GetEllapsedTime();
		//생성타임이 딜레이타임보다 크면
		if (m_fCurTime > m_fDelyTime)
		{
			//생성타임 0.0f로 초기화 후 해당 순서 파티클을 살린다
			m_fCurTime = 0.0f;
			m_vecAniData[m_nNumMakePtc].isAlive = true;
			//해당하는 파티클 살린후 인덱스는 ++
			++m_nNumMakePtc;
			//만약 만들어낸 파티클의 인덱스수가 파티클 벡터 사이즈와 같으면 전부생성 true
			if (m_nNumMakePtc == m_vecAniData.size()) { m_isMake = true; }
		}
	}
	//파티클 벡터를 생성한 파티클 인덱스 수만큼 돈다.
	for (int i = 0; i < m_nNumMakePtc; ++i)
	{
		//만약 해당 파티클이 죽어있으면 살리고 컨티뉴
		if (!m_vecAniData[i].isAlive) { m_vecAniData[i].isAlive = true; continue; }
		//해당파티클의 지속시간이 최대생존시간을 넘어가면 리셋
		if (m_vecAniData[i].fAge > m_vecAniData[i].fLifeTime) { ResetParticle(m_vecAniData[i]); continue; }
		//해당파티클 지속시간 증가
		m_vecAniData[i].fAge += g_pTimeManager->GetEllapsedTime();
		//프레임카운트 증가
		++m_vecAniData[i].nFrameCount;
		//만약 해당파티클 프레임카운트가 3보다 크거나 같으면
		if (m_vecAniData[i].nFrameCount >= 3)
		{
			m_vecAniData[i].nFrameCount = 0; //프레임카운트는 0으로 초기화
											 //만약 현재Y프레임이 맥스Y프레임과 같으면 현재Y프레임 0으로 초기화
			if (m_vecAniData[i].nCurFrameY == m_nFrameY) { m_vecAniData[i].nCurFrameY = 0; }
			//또는 현재X프레임이 맥스X프레임과 같으면 현재Y프레임 증가
			else if (m_vecAniData[i].nCurFrameX == m_nFrameX) { ++m_vecAniData[i].nCurFrameY; }
			//만약 현재X프레임이 맥스X프레임과 같으면 현재X프레임 0으로 초기화
			if (m_vecAniData[i].nCurFrameX == m_nFrameX) { m_vecAniData[i].nCurFrameX = 0; }
			//그게 아니라면 현재X프레임 증가
			else { ++m_vecAniData[i].nCurFrameX; }
			//만약 현재X프레임이 맥스X프레임이 아닐때
			if (m_vecAniData[i].nCurFrameX != m_nFrameX)
			{
				//버텍스 정점의 uv좌표를 현재X,Y프레임 기준으로 갱신
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
