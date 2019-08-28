#pragma once
#include "Sprite_Manager.h"
#define MAX_PARTICLE	100

class Particle
{
public:
	D3DXVECTOR3 m_vPos;           //파티클 위치
	D3DXVECTOR3 m_vAdd;           // 파티클 기존 위치에 프레임마다 더해지는 값
	D3DXVECTOR4 m_vColor;         // 파티클 색상
	float	    m_fTimer;         // 실행시부터 누적되는 시간
	float       m_fLifeTime;      // 생명주기
	int			m_iApplyIndex;    // 텍스쳐 인덱스
	int			m_iSpriteIndex;   // 스프라이트 타입 인덱스

public:
	Particle();
	~Particle();

};

class Particle_System
{
public:
	ID3D11Device*				m_pd3dDevice;
	Sprite*   david_pSprite;
	Particle* david_particles;
	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_matView;
	D3DXMATRIX				m_matProj;
public:
	int		m_iStartCount;
	int		m_iEndCount;
	float   m_fTime;
	float   m_fRadius;
	int     m_iMaxCount;
	float   m_fAddTime;
	float   m_fAddRadius;
	D3DXVECTOR3 m_vScale;

public:
	void SetRadius(float fRadius = 1.0f);
	void SetAddTime(float fTime_in);
	void SetAddRadius(float m_fAddRadius_in);
	void SetScale(float fX, float fY, float fZ);

	void SetMaxCounter(int iMaxNum);
	int  GetMaxCounter();
	void ReSetCounter();

public:
	bool Init();
	bool Create(ID3D11Device* pDevice, INT iMaxCounter = 100, float acc_Time = 10.0f, float fAddRadius = 30.0f, D3DXVECTOR3 vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	bool Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime);
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();


public://최대 파티클 개수 정하고, 할당한다.
	void setMatrix(D3DXMATRIX* pmatWorld, D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj);
	D3DXVECTOR3 Circle(float fRadius, float fRadian);
public:
	
	Particle_System();
	virtual ~Particle_System();
};

