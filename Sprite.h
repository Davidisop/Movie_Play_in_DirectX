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
	D3DXVECTOR4  uv[4]; // �� Vector4�� 4���ĸ�, ����̴ϱ�, �� ���� �ʿ��ϰ�, ��� VECTOR2�ε� �Ǵµ�, x,y���Ἥ, ��� UV��ǥ���� �����ߴ�.
	D3DXVECTOR4  color;
};

class Sprite : public David_Plane_Shape
{
public:

	T_STR						m_szName;
	T_STR						m_szPath; // ���
	UINT						m_iIndex; // ��������Ʈ �Ŵ������� �� ��������Ʈ ��ü�� ��ȣ.
	UINT						m_BlendType; // �̰͵� �߿���. ���� ��� Ȥ�� ����.
	D3DXMATRIX					m_matBillboard; // ȸ���� ���߸� �ǳ�?
	ID3D11BlendState*			DaSp_BlendState; // �߿�.
	ComPtr<ID3D11ShaderResourceView> m_pATextureRV;

public:

	vector<int>		Texture_Index_List;   // ���� �ؽ��� �ִϸ��̼� �ε���
	int				Applied_Texture_Index;   // �� �����ӿ��� ������ �ؽ����� �ε���
	int				the_Number_of_Textures;  // ��ü �ؽ��� ����
	float			Sec_Per_another_Texture; // �ؽ��ĸ��� ��ü �ֱ�

	float					DS_Timer; // = m_fTime ��ü�� �ð��� ���� ���� (0���� ���� �ʱ�ȭ��)
	float					m_fLifeTime;
	float					m_fAnimTime; // uv  �ݺ� �ð� = 1Turn �� �ִϸ��̼� �ð�.
	float					m_fElapseTime; // ������ �� ���� �ð�.
public:

	RECT			m_RectSet;         // �ؽ��� ����(x,y) �� ����(z,w) ����, ũ��.
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