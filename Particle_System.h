#pragma once
#include "Sprite_Manager.h"
#define MAX_PARTICLE	100

class Particle
{
public:
	D3DXVECTOR3 m_vPos;           //��ƼŬ ��ġ
	D3DXVECTOR3 m_vAdd;           // ��ƼŬ ���� ��ġ�� �����Ӹ��� �������� ��
	D3DXVECTOR4 m_vColor;         // ��ƼŬ ����
	float	    m_fTimer;         // ����ú��� �����Ǵ� �ð�
	float       m_fLifeTime;      // �����ֱ�
	int			m_iApplyIndex;    // �ؽ��� �ε���
	int			m_iSpriteIndex;   // ��������Ʈ Ÿ�� �ε���

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


public://�ִ� ��ƼŬ ���� ���ϰ�, �Ҵ��Ѵ�.
	void setMatrix(D3DXMATRIX* pmatWorld, D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj);
	D3DXVECTOR3 Circle(float fRadius, float fRadian);
public:
	
	Particle_System();
	virtual ~Particle_System();
};

