#include "Particle_System.h"


Particle::Particle()
{
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vAdd = D3DXVECTOR3(cosf(rand()), sinf(rand()), 0.0f); // rand()의 반환값은 int 랜덤이다.
	m_vColor.x = (float)(rand() % RAND_MAX) / RAND_MAX; // % 나머지는 무조건 나누는 수보다 작은 게 나온다.
	m_vColor.y = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.z = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.w = 1.0f;
	m_iApplyIndex = 0;

	m_fLifeTime = (float)(rand() % 5);
	m_iSpriteIndex = (rand() % SpriteManager->Sprite_Map.size()) + 1; // 1번부터 스프라이트가 저장되었음.	
}


Particle::~Particle()
{
	


}

	void Particle_System::SetRadius(float fRadius = 1.0f) { m_fRadius = fRadius; }
	void Particle_System::SetAddTime(float fTime_in) { m_fAddTime = fTime_in; };
	void Particle_System::SetAddRadius(float m_fAddRadius_in) { m_fAddRadius = m_fAddRadius_in; };
	void Particle_System::SetScale(float fX, float fY, float fZ) { m_vScale.x = fX; m_vScale.y = fY; m_vScale.z = fZ; }
	
	bool Particle_System::Init()
	{
		m_pd3dDevice = nullptr;
		david_pSprite = nullptr;
		david_particles = nullptr;
		m_fRadius = 1.0f;
		m_iStartCount = 0;
		m_iEndCount = 1;
		m_fTime = 0.0f;
		m_iMaxCount = MAX_PARTICLE;
		m_fAddTime = 0.1f;
		m_fAddRadius = 30.0f;
		m_vScale.x = 1.0f;
		m_vScale.y = 1.0f;
		m_vScale.z = 1.0f;
		return true;
	}

	//최대 파티클 개수 정하고, 생성한다.
	void Particle_System::SetMaxCounter(int iMaxNum)
	{
		delete[] david_particles; david_particles = nullptr;
		m_iMaxCount = iMaxNum;
		david_particles = new Particle[m_iMaxCount];
	}

	// 버텍스버퍼 하나를 사용하여 다수의 파티클을 그린다.
	bool Particle_System::Create(ID3D11Device* pDevice, INT iMaxCounter, float acc_Time, float fAddRadius, D3DXVECTOR3 vScale)
	{
		m_pd3dDevice = pDevice;
		assert(m_pd3dDevice);
		SetMaxCounter(iMaxCounter);
		SetAddTime(acc_Time / iMaxCounter);
		SetAddRadius(fAddRadius);
		SetScale(vScale.x, vScale.y, vScale.z);
		D3DXMatrixIdentity(&m_matWorld);	
		return true;
	}

	bool Particle_System::Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime)
	{
		D3DXVECTOR3 vReturnVaule = Circle(m_fRadius, fGlobalTime); //원상의 한 점의 좌표.

		vReturnVaule *= m_fAddRadius; // 곱한다?  각도라기보다, 원좌표계 중심을 원점이라 할때, 확장인데? 맞다.

		if ((fGlobalTime - m_fTime) > m_fAddTime)
		{
			if (m_iEndCount + 1 <= m_iMaxCount)
			{
				david_particles[m_iEndCount].m_vPos.x = vReturnVaule.x;
				david_particles[m_iEndCount].m_vPos.y = vReturnVaule.y;
				david_particles[m_iEndCount].m_vPos.z = sqrt(vReturnVaule.x*vReturnVaule.x + vReturnVaule.y*vReturnVaule.y);//루트
				
				m_iEndCount += 1;

			}
			else if (m_iEndCount + 1 > m_iMaxCount)
			{
				m_iEndCount = m_iMaxCount;
				m_iStartCount++;

				if (m_iStartCount >= m_iEndCount)
				{
					m_iStartCount = 0, m_iEndCount = 1;
				}
			}
		}
		return true;
	}

	bool Particle_System::Render(ID3D11DeviceContext* pContext)
	{
		D3DXMATRIX matTransposed, matTrans, matScale;
		D3DXMATRIX matY;//행렬들임.

		D3DXMatrixInverse(&matTransposed, NULL, &m_matView); // 왜 이렇게 했지? 음... 그건가? 빌보드?
		matTransposed._41 = 0.0f;	matTransposed._42 = 0.0f;	matTransposed._43 = 0.0f;		matTransposed._44 = 1.0f;

		for (int iParticle = m_iStartCount; iParticle < m_iEndCount; iParticle++)
		{
			D3DXMatrixTranslation(&matTrans, 
				david_particles[iParticle].m_vPos.x,
				david_particles[iParticle].m_vPos.y,
				david_particles[iParticle].m_vPos.z);

			D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
			D3DXMatrixMultiply(&m_matWorld, &matScale, &matTransposed); // 이 순서로 대면, 일단 모두 Scale 시키고 나서, 카메라 눈으로 다 모여.
			D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &matTrans); // 그곳에서, 원래 파티클 위치로 흩어지는 거야.

			david_pSprite = SpriteManager->GetPtr(david_particles[iParticle].m_iSpriteIndex);
			david_pSprite->m_cbData.vColor = david_particles[iParticle].m_vColor;
			david_pSprite->Updata(pContext,
				david_particles[iParticle].m_fTimer, //class Particle 객체의 멤버
				david_particles[iParticle].m_iApplyIndex,
				g_fGameTimer,
				g_fSecPerFrame);

			david_pSprite->Set_three_Matrixs(&m_matWorld, &m_matView, &m_matProj);
			david_pSprite->Render(pContext);
		}
		return true;
	}

	bool Particle_System::Release()
	{
		delete[] david_particles;
		david_particles = nullptr;

		return true;
	}



	int  Particle_System::GetMaxCounter() { return m_iMaxCount; }

	void Particle_System::ReSetCounter()
	{
		m_iStartCount = 0;
		m_iEndCount = 1;
		m_fTime = 0.0f;
	}

	void Particle_System::setMatrix(D3DXMATRIX* pmatWorld, D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj)
	{
		assert(pmatView && pmatProj); // pmatView 와 pmatProj가 0인지 체크해서, 0이면 프로그램중단.
		if (pmatWorld != nullptr) {m_matWorld = *pmatWorld;	}
		m_matView = *pmatView;
		m_matProj = *pmatProj;
	}

	D3DXVECTOR3 Particle_System::Circle(float fRadius, float fRadian)
	{
		D3DXVECTOR3 vFinally;
		vFinally.x = fRadius * cosf(fRadian);
		vFinally.y = fRadius * sinf(fRadian);
		vFinally.z = 0.0f;
		return vFinally;
	}





	Particle_System::Particle_System()
	{
		Init();
	}


Particle_System::~Particle_System()
{
}
