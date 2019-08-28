#include "Sample.h"


bool Sample::Init()
{
	// OBB�� ȭ�鿡 �����ֱ� ����, ����ptr 
	// �̰ɷ� 12�� �Ѹ��� �ٲܰ���.
	m_pLine_ptr = new David_Line_Shape;
	m_pLine_ptr->Create(g_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/bitmap.bmp");

	//�ϴ� ���� ����� ����. 
	m_Geomtry_wraped.Create(g_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/checker_with_numbers.bmp");

	//���� AABB���� ����.
	
	//AABB = x����� 4, y����� 2,  z�� ���� 2.
	m_AABB_TO_OBB_Box.vMax = D3DXVECTOR3(4.0f, 2.0f, 2.0f);
	m_AABB_TO_OBB_Box.vMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AABB_TO_OBB_Box.vCenter = (m_AABB_TO_OBB_Box.vMax + m_AABB_TO_OBB_Box.vMin) / 2;
	
	//AABB ������
	m_AABB_TO_OBB_Box.vAxis_Normalized[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_AABB_TO_OBB_Box.vAxis_Normalized[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_AABB_TO_OBB_Box.vAxis_Normalized[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//���밪 : Extent_for_Axis[3] ������ ���ϱ� ����, �װ��� mat_Scale_here��Ŀ� �״�� �ݿ�.
	m_AABB_TO_OBB_Box.Extent_for_Axis[0] = fabs(m_AABB_TO_OBB_Box.vCenter.x - m_AABB_TO_OBB_Box.vMax.x);
	m_AABB_TO_OBB_Box.Extent_for_Axis[1] = fabs(m_AABB_TO_OBB_Box.vCenter.y- m_AABB_TO_OBB_Box.vMax.y);
	m_AABB_TO_OBB_Box.Extent_for_Axis[2] = fabs(m_AABB_TO_OBB_Box.vCenter.z- m_AABB_TO_OBB_Box.vMax.z);

	//AABB ���� �� Scale;// Build a matrix which scales by (sx, sy, sz)

	D3DXMATRIX mat_Scale_here;
	D3DXMatrixScaling(&mat_Scale_here, m_AABB_TO_OBB_Box.Extent_for_Axis[0], m_AABB_TO_OBB_Box.Extent_for_Axis[1], m_AABB_TO_OBB_Box.Extent_for_Axis[2]);



	// ������ ȸ�� ��ĸ����.

	D3DXMATRIX mat_Rotation;
	D3DXMatrixRotationY(&mat_Rotation, D3DX_PI / 6);



	////AABB ���� �� ȸ���ϱ� // Transform (x, y, z, 1) by matrix,
	//D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vAxis_Normalized[0], &m_AABB_TO_OBB_Box.vAxis_Normalized[0], &mat_Y);
	//D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vAxis_Normalized[1], &m_AABB_TO_OBB_Box.vAxis_Normalized[1], &mat_Y);
	//D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vAxis_Normalized[2], &m_AABB_TO_OBB_Box.vAxis_Normalized[2], &mat_Y);




	// �̷��� �ϸ�, ���� ����� ��, S->R->T �ȴ�.
	// �� ��ĸ����� ������ David_Box�� ����Ѱ�, �� AABB�� ����������, 
	//David_Box�� 0,0,0 �������� 8�� ���� �ȹ濡 ������ 1,1,1 �� Max�̰�, -1,-1,-1�� Min�̴�.
	m_Geomtry_wraped.m_matWorld = mat_Scale_here * mat_Rotation;

	//
	D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vCenter, 
		&m_AABB_TO_OBB_Box.vCenter,  &mat_Rotation);
	//
	D3DXVECTOR3 david = D3DXVECTOR3(2, 0, 1);
	D3DXVec3TransformCoord(&david, &david, &mat_Rotation);

	D3DXVECTOR3 david2 = D3DXVECTOR3((4.4641+3.4641)/2-2.23205066, 0, (-2-0.26795)/2+0.13397);
	D3DXVec3Normalize(&david2, &david2);

	//
	D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vAxis_Normalized[0], &m_AABB_TO_OBB_Box.vAxis_Normalized[0], &mat_Rotation);
	D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vAxis_Normalized[1], &m_AABB_TO_OBB_Box.vAxis_Normalized[1], &mat_Rotation);
	D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vAxis_Normalized[2], &m_AABB_TO_OBB_Box.vAxis_Normalized[2], &mat_Rotation);


	//
	m_Geomtry_wraped.m_matWorld._41 = m_AABB_TO_OBB_Box.vCenter.x;
	m_Geomtry_wraped.m_matWorld._42 = m_AABB_TO_OBB_Box.vCenter.y;
	m_Geomtry_wraped.m_matWorld._43 = m_AABB_TO_OBB_Box.vCenter.z;



	m_Geomtry_wraped.Input_OBB_Data(
		m_AABB_TO_OBB_Box.Extent_for_Axis[0],
		m_AABB_TO_OBB_Box.Extent_for_Axis[1],
		m_AABB_TO_OBB_Box.Extent_for_Axis[2],
		m_AABB_TO_OBB_Box.vCenter,
		m_AABB_TO_OBB_Box.vAxis_Normalized[0],
		m_AABB_TO_OBB_Box.vAxis_Normalized[1],
		m_AABB_TO_OBB_Box.vAxis_Normalized[2]
	);
	// �ٵ� �̷��� �ϸ�, �ڽ��� OBB�� ���������� �ʳ�?
		return true;
	}


	bool Sample::Frame()
	{
		
			Select_Instance.SetMatrixs_and_Make_Ray(&m_Geomtry_wraped.m_matWorld, &(MainCamera->m_matView), &(MainCamera->m_matProj));
			// m_Geomtry_wraped�� ������� : �� OBB Rotation & Scale& Trarns �� �� ���� ������,  m_Geomtry_wraped �ڽ��� �ٲ��,
			// �� ���� ����� RAY�� ������״�.
		

		return true;
	}

	bool Sample::Render()
	{

// OBB�� ȭ�鿡 �����ֱ� ����, ����ptr 
// �̰ɷ� 12�� �Ѹ��� �ٲܰ���.

		m_pLine_ptr->Set_three_Matrixs(nullptr, &(MainCamera->m_matView), &(MainCamera->m_matProj));
		//���⼭ ����� nullptr�̴�. �ֳĸ�, ���� ����� ��ǥ���� �̹�, SRT����� ��ǥ �̱� �����̴�.


		// 4��
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[0], m_Geomtry_wraped.Collision_Box.vPos[1], D3DXVECTOR4(0.0f,0.0f,0.0f,1.0f));
		//Draw�ȿ� ���ؽ��� ����, �ܽ�źƮ ����, Pre, Post �� �� �ִ�.																													 
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[1], m_Geomtry_wraped.Collision_Box.vPos[2], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[2], m_Geomtry_wraped.Collision_Box.vPos[3], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[3], m_Geomtry_wraped.Collision_Box.vPos[0], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
																																																									   
		// 4��																																																					   
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[4], m_Geomtry_wraped.Collision_Box.vPos[5], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[5], m_Geomtry_wraped.Collision_Box.vPos[6], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[6], m_Geomtry_wraped.Collision_Box.vPos[7], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[7], m_Geomtry_wraped.Collision_Box.vPos[4], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
																																																									   
		// 4��																																																					   
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[0], m_Geomtry_wraped.Collision_Box.vPos[4], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[1], m_Geomtry_wraped.Collision_Box.vPos[5], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[2], m_Geomtry_wraped.Collision_Box.vPos[6], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[3], m_Geomtry_wraped.Collision_Box.vPos[7], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));


		//Select_Instance.SetMatrixs_and_Make_Ray(&m_Geomtry_wraped.m_matWorld, &(MainCamera->m_matView), &(MainCamera->m_matProj));
	
		m_Geomtry_wraped.Set_three_Matrixs(&(m_Geomtry_wraped.m_matWorld), &(MainCamera->m_matView), &(MainCamera->m_matProj));
		// �� nullptr�� �߳ĸ�, �̹� ��������� ���õǾ� �ֱ� �����̴�.
		m_Geomtry_wraped.Render(m_pContext);

		// ������� �� ���.
		if (g_Input.Left_Click) // �̷��� ī�޶󿡼��� ���.
		{
			if (Select_Instance.Ray_OBB_Meeting_Check_in_Same_CoordSystem(&m_Geomtry_wraped.Collision_Box)==true)
			{
				TCHAR szBuffer[256];
				//���ۿ� ���� ����ֱ�.
				//const TCHAR* david = L"Collision bewteen Ray and OBB";
				_stprintf_s(szBuffer, _T("Collision bewteen Ray and OBB") );
			
				MessageBox(0, szBuffer, _T("�浹"), MB_OK);
			/*	WINAPI
					MessageBoxW(
						_In_opt_ HWND hWnd,
						_In_opt_ LPCWSTR lpText,  // CONST WCHAR
						_In_opt_ LPCWSTR lpCaption,
						_In_ UINT uType);*/
			}
		}
		return true;
	}


	bool Sample::Release()
	{

		if (m_pLine_ptr != nullptr) 
		{	delete m_pLine_ptr;
			m_pLine_ptr = nullptr;}

		m_Geomtry_wraped.Release();
		return true;
	}
	Sample::Sample() 
	{
	
	}
	Sample::~Sample() {}


GAME_RUN("Sample", 800, 600)