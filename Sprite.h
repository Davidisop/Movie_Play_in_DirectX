#pragma once
#include "David_Shape.h"
#include "TextureManager.h"

typedef vector<T_STR>		T_STR_VECTOR;

struct RectUV
{
	D3DXVECTOR4 David_UV;
	RECT Rect;
};

struct TInstatnce
{
	D3DXMATRIX   matWorld;
	D3DXVECTOR4  uv[4]; // 왜 Vector4로 4개냐면, 평면이니까, 네 점이 필요하고, 사실 VECTOR2로도 되는데, x,y만써서, 평면 UV좌표들을 구성했다.
	D3DXVECTOR4  color;
};

class Sprite : public David_Plane_Shape
{
public:

	T_STR						m_szName;
	T_STR						m_szPath; // 경로
	UINT						m_iIndex; // 스프라이트 매니저에서 이 스프라이트 객체의 번호.
	UINT						m_BlendType; // 이것도 중요함. 블랜드 방법 혹은 여부.
	D3DXMATRIX					m_matBillboard; // 회전만 맞추면 되나?
	ID3D11BlendState*			DaSp_BlendState; // 중요.
	ComPtr<ID3D11ShaderResourceView> m_pATextureRV;

public:

	vector<int>		Texture_Index_List;   // 낱장 텍스쳐 애니메이션 인덱스
	int				Applied_Texture_Index;   // 현 프레임에서 적용할 텍스쳐의 인덱스
	int				the_Number_of_Textures;  // 전체 텍스쳐 개수
	float			Sec_Per_another_Texture; // 텍스쳐마다 교체 주기

	float					DS_Timer; // = m_fTime 교체될 시간의 누적 변수 (0으로 자주 초기화됨)
	float					m_fLifeTime;
	float					m_fAnimTime; // uv  반복 시간 = 1Turn 총 애니메이션 시간.
	float					m_fElapseTime; // 무조건 총 누적 시간.
public:

	RECT			m_RectSet;         // 텍스쳐 가로(x,y) 및 세로(z,w) 개수, 크기.
	vector<RectUV>  m_Rects_List;
	D3DXVECTOR4		m_ParticleColor;

	vector <TInstatnce>   m_pInstance;
	ComPtr <ID3D11Buffer> m_pVertexBufferInstance;
	bool				  m_bInstancing;

public:
	
	virtual HRESULT CreateInputLayout() override;
	virtual HRESULT CreateInstance(UINT iNumInstance);
	virtual HRESULT Load(ID3D11Device* pDevice, TCHAR* pLoadTextureString, TCHAR* pLoadShaderString, bool bInstancing = false, ID3D11BlendState* pBlendState = nullptr);

	virtual bool Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime_in_foreachFrame);
	virtual void Updata(ID3D11DeviceContext* pContext, float& pfCurrentTimer, int& iApplyIndex, float fGloabalTime, float fElapsedTime_in_foreachFrame);
	virtual bool PreRender(ID3D11DeviceContext* pContext) override;
	virtual bool Render(ID3D11DeviceContext* pContext) override;
	virtual bool PostRender(ID3D11DeviceContext* pContext) override;
	virtual bool RenderInstancing(ID3D11DeviceContext* pContext);



public:

	virtual void SetTextureArray(T_STR_VECTOR FileList);
	D3DXVECTOR4 SetUV(RECT& Rect);
	virtual void SetUVAnimation(float fAnimTime, int iWidth = 1, int iHeight = 1);
	virtual void SetRectAnimation(
		float fAnimTime = 1.0f,
		int iWidth = 1,
		int iWidthSize = 0,
		int iHeight = 1,
		int iHeightSize = 0);

public:
	Sprite();
	~Sprite();

};