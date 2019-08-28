#include "Sprite.h"






HRESULT Sprite::CreateInputLayout()
{
	HRESULT hr = S_OK;
	if (m_bInstancing == false)
	{
		David_Plane_Shape::CreateInputLayout();
	}
	else if(m_bInstancing == true)
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOLR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},

			{"TRANSFORM", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1}, // 4����Ʈ * 4���� = 16����Ʈ.
			{"TRANSFORM", 1 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"TRANSFORM", 2 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"TRANSFORM", 3 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},

			{"TRANSFORM", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"TRANSFORM", 1 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"TRANSFORM", 2 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"TRANSFORM", 3 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 112, D3D11_INPUT_PER_INSTANCE_DATA, 1},

			{"TRANSFORM", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 128, D3D11_INPUT_PER_INSTANCE_DATA, 1}	
		};

		UINT numElements = sizeof(layout) / sizeof(layout[0]);
		David_Obj_C_F.m_pInputLayout.Attach
		(David_DX::CreateInputLayout(g_pd3dDevice, David_Obj_C_F.m_pVSBlob->GetBufferSize(),
			David_Obj_C_F.m_pVSBlob->GetBufferPointer(),
			layout, numElements,
			David_Obj_C_F.m_pInputLayout.GetAddressOf()));
	}

	return hr;
}
HRESULT Sprite::CreateInstance(UINT iNumInstance_in)
{
	HRESULT hr = S_OK;
	m_pInstance.resize(iNumInstance_in);//vector <TInstatnce>   m_pInstance;


	for (int iST = 0; iST < m_pInstance.size(); iST++)
	{
		D3DXMatrixIdentity(&m_pInstance[iST].matWorld);
		m_pInstance[iST].uv[0] = D3DXVECTOR4(0, 0, 0, 0);
		m_pInstance[iST].uv[1] = D3DXVECTOR4(0.25f, 0, 0, 0);
		m_pInstance[iST].uv[2] = D3DXVECTOR4(0.25f, 0.25f, 0, 0);
		m_pInstance[iST].uv[3] = D3DXVECTOR4(0, 0.25f, 0, 0);

		m_pInstance[iST].color = D3DXVECTOR4((rand() & RAND_MAX) / (float)RAND_MAX,
			(rand()&RAND_MAX) / (float)RAND_MAX,
			(rand()&RAND_MAX) / (float)RAND_MAX,
			1);
		D3DXMatrixTranspose(&m_pInstance[iST].matWorld, &m_pInstance[iST].matWorld);
	}
	m_pVertexBufferInstance.Attach(David_DX::CreateVertexBuffer(g_pd3dDevice,  m_pInstance.size(), sizeof(TInstatnce), &m_pInstance[0]));// Q ����̽��� �� ���۽ᵵ �Ǵ���.

	return hr;
}

HRESULT Sprite::Load(ID3D11Device* pDevice, // Create ���.
	TCHAR* pLoadTextureString,
	TCHAR* pLoadShaderString,
	bool bInstancing,
	ID3D11BlendState* pBlendState)
{
	m_bInstancing = bInstancing;

	if (!Create(pDevice, pLoadTextureString, pLoadShaderString))
	{
		MessageBox(0, _T("Create"), _T("Fatal error"), MB_OK);
		return 0;
	}

	DaSp_BlendState = pBlendState;
	return S_OK;
}


void Sprite::SetUVAnimation(float fAnimTime, int iWidth, int iHeight)
{

	m_fAnimTime = fAnimTime;
	the_Number_of_Textures = iWidth * iHeight;
	// �״ϱ� ���� ���� �̹��� �� �� �ȿ�, ���η� ��� �迭, ���η� �� ���� �迭�Ǿ� �ִ� ���̴�.
	Sec_Per_another_Texture = m_fAnimTime / the_Number_of_Textures;


	float fStepW, fStepH;
	fStepW = 1.0f / iWidth; //UV ��ǥ��� ���ΰ� 1.0�̴�. �װ��� ���� �ؽ��� ������ ������ �ִ�.
	fStepH = 1.0f / iHeight; //UV ��ǥ��� ���ΰ� 1.0�̴�. �װ��� ���� �ؽ��� ������ ������ �ִ�.

	m_Rects_List.resize(iWidth*iHeight); // �״ϱ� ���� �ִ� Rect ������ '�� ���� �̹��� �ȿ��� UV��ǥ��'�� ��� �ִ�.
									     // vector<RectUV>  m_Rects_List;
										//  ���� ��� ���δ� 6��, ���δ� 4�� ��� �غ���. �׷� �� 24���� �� �� �̹��� �ȿ� �ִ�.
	for (int iRow = 0; iRow < iHeight; iRow++) //���� ��  = 6��
	{
		for (int iCollum = 0; iCollum < iWidth; iCollum++) //���� ��� = 4��
		{
			m_Rects_List[iRow*iWidth + iCollum].David_UV.x = iCollum * fStepW;//�������� X��ǥ
			m_Rects_List[iRow*iWidth + iCollum].David_UV.y = iRow * fStepH;//�������� Y��ǥ

			m_Rects_List[iRow*iWidth + iCollum].David_UV.z = iCollum * fStepW + fStepW;//�������� X��ǥ
			m_Rects_List[iRow*iWidth + iCollum].David_UV.w = iRow * fStepH + fStepH;//�������� Y��ǥ
		}
	}
}



void Sprite::SetRectAnimation(  // 	vector<RectUV>  m_Rects_List;�� David_UV�� ä��� �� ����
	float fAnimTime,
	int iWidth, //���� ĭ ����
	int iWidthSize,//���� �� ĭ��  ����
	int iHeight, // ���� ĭ ����
	int iHeightSize) // ���� �� ĭ�� ����
{
	m_fAnimTime = // 1Turn �� �ִϸ��̼� ��
		fAnimTime;

	//public: RECT  m_RectSet; ��������Ʈ Ŭ������ ��ü RECT

	m_RectSet.left = iWidth; //���� ĭ ����
	m_RectSet.right = iWidthSize; // �� ĭ�� �ȼ� ���� ����.
	m_RectSet.top = iHeight; // ���� ĭ ����
	m_RectSet.bottom = iHeightSize; // �� ĭ�� �ȼ� ���� ����.

	if (iWidth * iHeight <= 1) // ���� �⺻��
	{
		Sec_Per_another_Texture = m_fAnimTime / the_Number_of_Textures;
		return;	// �ڷ� �����ϰ� �Ϻη� �Է��� ���, ���� �͸� ����.
	}

	else if (iWidth * iHeight > 1) // ���� �ڷ���� ����� �Է��ϸ�
	{
		Sec_Per_another_Texture = m_fAnimTime / (iWidth * iHeight);
		the_Number_of_Textures = iWidth * iHeight;
		//��... �̰ɷ� �����ؼ�, m_RectSet�� �и��� �뵵�� �Ϲ� Rect�� �ٸ���.
	}

	//m_RectSet.left = iWidth; //���� ĭ ����
	for (int iHeight = 0; iHeight < m_RectSet.left; iHeight++)
	{
		//m_RectSet.top = iHeight; // ���� ĭ ����
		for (int iWidth = 0; iWidth < m_RectSet.top; iWidth++)
		{
			RectUV tRect;// ���⼭ �� �ҷ�������.

			tRect.Rect.left = iWidth * m_RectSet.right;
			tRect.Rect.right = (iWidth + 1) * m_RectSet.right;
			tRect.Rect.top = iHeight * m_RectSet.bottom;
			tRect.Rect.bottom = (iHeight + 1)*m_RectSet.bottom;

			tRect.David_UV = SetUV(tRect.Rect);
			m_Rects_List.push_back(tRect);// �̰� �����̴�.
		}
	}
}



D3DXVECTOR4 Sprite::SetUV(RECT& Rect) //�ȼ���ǥ��κ��� UV��ǥ�� ��.
{
	D3DXVECTOR4 vUV; // ���⿡ ����? �׷� ��� ���ڳ�. UV��ǥ��� �ǰ�???

#pragma region UV_LeftTop 
	
	float fOffsetX = 0.0f; //Left_X�ʱ�ȭ

	if (Rect.left > 0)
	{
		fOffsetX = (float)Rect.left / (float)(m_RectSet.left*m_RectSet.right);
	}

	float fOffsetY = 0.0f; //Left_Y�ʱ�ȭ

	if (Rect.top > 0)
	{
		fOffsetY = (float)Rect.top / (float)(m_RectSet.top*m_RectSet.bottom);
	}

		vUV.x = fOffsetX;
		vUV.y = fOffsetY;

#pragma endregion
#pragma region UV_RightBottom 


		fOffsetX = 1.0f; //Right_X�ʱ�ȭ

		if (Rect.right > 0)
		{
			fOffsetX = (float)Rect.right / (float)(m_RectSet.left*m_RectSet.right);
		}

		fOffsetY = 1.0f; //Right_Y�ʱ�ȭ

		if (Rect.bottom > 0)
		{
			fOffsetY = (float)Rect.bottom / (float)(m_RectSet.top*m_RectSet.bottom);// ���� ��������Ʈ RECT�� �ܺ� RECT�� ������ ���ϴ� ����?
		}

		vUV.z = fOffsetX;// �� Stamp�� U ���� ����
		vUV.w = fOffsetY;// �� Stamp�� V ���� ����
#pragma endregion
		return vUV;
}

bool Sprite::Frame(ID3D11DeviceContext* pContext, float fGlobalTime, float fElapsedTime_in_foreachFrame)
{
	Updata(pContext, DS_Timer, Applied_Texture_Index, fGlobalTime, fElapsedTime_in_foreachFrame);
	return true;
}


void Sprite::Updata(ID3D11DeviceContext* pContext,
	float& DS_Timer_ofSprite,
	int& Applied_Texture_Index_in,
	float fGloabalTime,
	float fElapsedTime_in_foreachFrame)
{
	//��������Ʈ �߻� ��� �ð� : ��� ���̱⸸ �Ѵ�.
	m_fElapseTime += fElapsedTime_in_foreachFrame;
	// �ִϸ��̼� ��ü �ֱ� ���� �ð� : �� ������ ���̸�, Ư�� '����'�Ѿ�� 0���� �ʱ�ȭ�ȴ�.
	DS_Timer_ofSprite += fElapsedTime_in_foreachFrame;

	if (DS_Timer_ofSprite >= Sec_Per_another_Texture)
	{
		if (++Applied_Texture_Index_in >= the_Number_of_Textures)
		{
			Applied_Texture_Index_in = 0;
		}
		DS_Timer_ofSprite = 0.0f; // Sprite ��ü ���� ���� DS_Timer //����, �ִϸ��̼� �ؽ��� �����ؾ� ��.
		// �̺�Ʈ Ÿ�̸Ӱ� 0 �̵�?
	}
	
	// �ؽ��� �ִϸ��̼�
	if (Texture_Index_List.size()>0)// ������ �ִϸ��̼Ǹ� ����.
	{
		m_pATextureRV.Attach(TextureManager->Getptr(Texture_Index_List[Applied_Texture_Index_in])->m_pTextureSRV);
	}

	else if(Texture_Index_List.size() ==0) // UV �ؽ�ó ���ϸ��̼� - �� ��ȿ��� Rect�� �޶���. Rect�� �����ϴ� ����.
	{
		if (m_Rects_List.size() == 0) return;	// �������� UV �ִϸ��̼��� �̹� init() �� ����ȴ�. m_Rects_List
		
		else if(m_Rects_List.size() > 0) 
		{
			if (m_bInstancing==true) //�ν�ź���̸�, 
			{
				for (int iSt = 0; iSt < m_pInstance.size(); iSt++) //vector <TInstatnce>   m_pInstance; 
				{
					D3DXVECTOR4 vUV = m_Rects_List[m_pInstance[iSt].uv[0].z].David_UV;  // �и��� Z������ UV���� �ƴ�, �ٸ� ������ ����.
					D3DXVECTOR4 vColor = m_pInstance[iSt].color;

					m_pInstance[iSt].uv[0] = D3DXVECTOR4(vUV.x, vUV.y, 0, 0); 
					m_pInstance[iSt].uv[1] = D3DXVECTOR4(vUV.z, vUV.y, 0, 0);
					m_pInstance[iSt].uv[2] = D3DXVECTOR4(vUV.z, vUV.w, 0, 0); 
					m_pInstance[iSt].uv[3] = D3DXVECTOR4(vUV.x, vUV.w, 0, 0);

					D3DXMatrixTranspose(&m_pInstance[iSt].matWorld, &m_pInstance[iSt].matWorld);
					// ��ġ�� ���״�. �ٷ� �ܽ�źƮ�� �ѱ�� �׷���? �׷���, ���߿� �̰� ������ ����. �̹� ���� ���� �� ���⵵...
				}
				//FOR�� ������.
				pContext->UpdateSubresource(m_pVertexBufferInstance.Get(), 0, NULL, &m_pInstance[0], 0, 0);
				// 	m_pContext->UpdateSubresource(m_dxobj.g_pIndexBuffer.Get(), 0, NULL, &m_IndexList.at(0), 0, 0);
				//	pContext->UpdateSubresource( m_pBoxShape->m_dxobj.g_pVertexBuffer.Get(), 0, NULL, &m_VertexList.at(0), 0, 0);
				// �׸���, �ν�ź�� ���۸� ������Ʈ �Ѵ�?
			}

			else if(m_bInstancing==false) 
			{
				D3DXVECTOR4 vUV = m_Rects_List[Applied_Texture_Index_in].David_UV; // 16�� ����Ǿ� ����.
				D3DXVECTOR4 vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

				if (David_Obj_C_F.m_pTextureRV != nullptr && m_Rects_List.size()) // ó��  ����¥���� SRV �����Ǿ�����.. AND ������. 16��.
				{
					PNCT_VERTEX vertices[] =
					{
						{D3DXVECTOR3(-1.0f,1.0f,0.0f),D3DXVECTOR3(-1.0f,1.0f,0.0f), vColor,D3DXVECTOR2(vUV.x,vUV.y)}, //T�� �ٽ���.
						{D3DXVECTOR3(1.0f,1.0f,0.0f),D3DXVECTOR3(-1.0f,1.0f,0.0f), vColor,D3DXVECTOR2(vUV.z,vUV.y)},
						{D3DXVECTOR3(1.0f,-1.0f,0.0f),D3DXVECTOR3(-1.0f,1.0f,0.0f), vColor,D3DXVECTOR2(vUV.z,vUV.w)},
						{D3DXVECTOR3(-1.0f,-1.0f,0.0f),D3DXVECTOR3(-1.0f,1.0f,0.0f), vColor,D3DXVECTOR2(vUV.x,vUV.w)},
					};
					pContext->UpdateSubresource(David_Obj_C_F.m_pVertexBuffer.Get(), 0, 0, &vertices, 0, 0);
				}
			}

		}
	}
}


bool Sprite::Render(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(David_Obj_C_F.m_pConstantBuffer.Get(),	0, NULL, &m_cbData, 0, 0);
	pContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)m_Primitive);

	PreRender(pContext); // �̰� �� ����? ���ϱ� PreRender()�� �³�. �׷���, ���� ������Ʈ �ٲٰ�, �ؽ��ĸ��ҽ��� �ٲ��. �ؽ��� �ٲ峪 ����.
	PostRender(pContext);
	return true;
}


bool Sprite::PreRender(ID3D11DeviceContext* pContext)
{
	if (DaSp_BlendState != nullptr)
	{
		David_DX::Apply_Blend_State(pContext, DaSp_BlendState);
	}

	 David_Shape::PreRender(pContext); 


	if (m_pATextureRV != nullptr)
	{
		pContext->PSSetShaderResources(0, 1, m_pATextureRV.GetAddressOf()); // �� �ؽ��ķ� �ٲ۴�.
	}


	return true;
}


bool Sprite::PostRender(ID3D11DeviceContext* pContext)
{
	David_Plane_Shape::PostRender(pContext); //David_Shape::PostRender ���ذ� �Ȱ��°�.. �׷� Draw Ȥ�� DrawIndexed��...
	return true;
}

bool Sprite::RenderInstancing(ID3D11DeviceContext* pContext)
{
	Sprite::PreRender(pContext);

#pragma region Instanc Preparation 

	ID3D11Buffer* David_vb[2] = { David_Obj_C_F.m_pVertexBuffer.Get(), m_pVertexBufferInstance.Get() };
	UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(TInstatnce) };
	UINT offset[2] = { 0,0 };
	pContext->IASetVertexBuffers(0, 2, David_vb, stride, offset);
	pContext->UpdateSubresource(David_Obj_C_F.m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);

#pragma endregion

	pContext->DrawIndexedInstanced(m_iNumIndex, m_pInstance.size(), 0, 0, 0);

	return true;
}

void Sprite::SetTextureArray(T_STR_VECTOR FileList)
{    

	HRESULT hr = S_OK;

	for (int iList = 0; iList < FileList.size(); iList++)
	{
		int iIndex = TextureManager->ADD(m_pd3dDevice, FileList[iList].c_str());
		// ���⼭ �ؽ��ĵ� �Ŵ����� �ε��ϰ� ����.
		// SRV�� ���.

		if (iIndex <= 0)//��Ͼȵ� ����.
		{
			MessageBox(0, _T("m_Texture�� �ؽ��� �Ŵ����� ��� ����"), _T("Fatal Error"), MB_OK);
			return;
		}

		// �Ŵ����� ��ϵ� �ε����� ��ȯ�Ǿ���.
		Texture_Index_List.push_back(iIndex);
	}
	the_Number_of_Textures = Texture_Index_List.size();
	//�� ��������Ʈ�� ���Ǵ� �ؽ��ĵ� �� ����.
}


Sprite::Sprite()
{
	m_iIndex = 1004;
	m_fAnimTime = 0.0f;
	m_fLifeTime = 0.0f;
	m_fElapseTime = 0.0f;
	m_bInstancing = false;
	m_BlendType = 0;
	DS_Timer = 0.0f;
	Applied_Texture_Index = 0;
	Sec_Per_another_Texture = 0.2f;
	the_Number_of_Textures = 0;
	m_RectSet.left = 4;
	m_RectSet.right = 64; // �ؽ��� ���� �� ���� �� ũ��	
	m_RectSet.top = 4;
	m_RectSet.bottom = 64; // �ؽ��� ���� �� ���� �� ũ��
	m_pATextureRV = nullptr;
	DaSp_BlendState = nullptr;
	m_ParticleColor = D3DXVECTOR4(1, 1, 1, 1);
}


Sprite::~Sprite()
{
}

