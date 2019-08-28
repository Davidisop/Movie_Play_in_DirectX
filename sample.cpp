#include "Sample.h"


bool Sample::Init()
{
	// OBB를 화면에 보여주기 위한, 라인ptr 
	// 이걸로 12개 뿌리며 바꿀것임.
	m_pLine_ptr = new David_Line_Shape;
	m_pLine_ptr->Create(g_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/bitmap.bmp");

	//일단 감쌀 대상을 생성. 
	m_Geomtry_wraped.Create(g_pd3dDevice, L"../../data/shader/shape.hlsl", L"../../data/checker_with_numbers.bmp");

	//기초 AABB부터 시작.
	
	//AABB = x축길이 4, y축길이 2,  z축 길이 2.
	m_AABB_TO_OBB_Box.vMax = D3DXVECTOR3(4.0f, 2.0f, 2.0f);
	m_AABB_TO_OBB_Box.vMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AABB_TO_OBB_Box.vCenter = (m_AABB_TO_OBB_Box.vMax + m_AABB_TO_OBB_Box.vMin) / 2;
	
	//AABB 기저축
	m_AABB_TO_OBB_Box.vAxis_Normalized[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_AABB_TO_OBB_Box.vAxis_Normalized[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_AABB_TO_OBB_Box.vAxis_Normalized[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//절대값 : Extent_for_Axis[3] 각각을 구하기 위한, 그것이 mat_Scale_here행렬에 그대로 반영.
	m_AABB_TO_OBB_Box.Extent_for_Axis[0] = fabs(m_AABB_TO_OBB_Box.vCenter.x - m_AABB_TO_OBB_Box.vMax.x);
	m_AABB_TO_OBB_Box.Extent_for_Axis[1] = fabs(m_AABB_TO_OBB_Box.vCenter.y- m_AABB_TO_OBB_Box.vMax.y);
	m_AABB_TO_OBB_Box.Extent_for_Axis[2] = fabs(m_AABB_TO_OBB_Box.vCenter.z- m_AABB_TO_OBB_Box.vMax.z);

	//AABB 기저 축 Scale;// Build a matrix which scales by (sx, sy, sz)

	D3DXMATRIX mat_Scale_here;
	D3DXMatrixScaling(&mat_Scale_here, m_AABB_TO_OBB_Box.Extent_for_Axis[0], m_AABB_TO_OBB_Box.Extent_for_Axis[1], m_AABB_TO_OBB_Box.Extent_for_Axis[2]);



	// 기저축 회전 행렬만들기.

	D3DXMATRIX mat_Rotation;
	D3DXMatrixRotationY(&mat_Rotation, D3DX_PI / 6);



	////AABB 기저 축 회전하기 // Transform (x, y, z, 1) by matrix,
	//D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vAxis_Normalized[0], &m_AABB_TO_OBB_Box.vAxis_Normalized[0], &mat_Y);
	//D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vAxis_Normalized[1], &m_AABB_TO_OBB_Box.vAxis_Normalized[1], &mat_Y);
	//D3DXVec3TransformCoord(&m_AABB_TO_OBB_Box.vAxis_Normalized[2], &m_AABB_TO_OBB_Box.vAxis_Normalized[2], &mat_Y);




	// 이렇게 하면, 감쌀 대상인 게, S->R->T 된다.
	// 이 행렬만으로 기존의 David_Box가 충분한게, 이 AABB와 마찬가지로, 
	//David_Box도 0,0,0 기준으로 8개 점이 팔방에 찍혀서 1,1,1 이 Max이고, -1,-1,-1이 Min이다.
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
	// 근데 이렇게 하면, 박스와 OBB가 동일해지지 않나?
		return true;
	}


	bool Sample::Frame()
	{
		
			Select_Instance.SetMatrixs_and_Make_Ray(&m_Geomtry_wraped.m_matWorld, &(MainCamera->m_matView), &(MainCamera->m_matProj));
			// m_Geomtry_wraped의 월드행렬 : 즉 OBB Rotation & Scale& Trarns 을 한 것을 가지고,  m_Geomtry_wraped 박스를 바꿨고,
			// 그 월드 행렬은 RAY에 적용시켰다.
		

		return true;
	}

	bool Sample::Render()
	{

// OBB를 화면에 보여주기 위한, 라인ptr 
// 이걸로 12개 뿌리며 바꿀것임.

		m_pLine_ptr->Set_three_Matrixs(nullptr, &(MainCamera->m_matView), &(MainCamera->m_matProj));
		//여기서 월드는 nullptr이다. 왜냐면, 여기 저장된 좌표들이 이미, SRT적용된 좌표 이기 때문이다.


		// 4개
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[0], m_Geomtry_wraped.Collision_Box.vPos[1], D3DXVECTOR4(0.0f,0.0f,0.0f,1.0f));
		//Draw안에 버텍스벞 업뎃, 콘스탄트 업뎃, Pre, Post 다 들어가 있다.																													 
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[1], m_Geomtry_wraped.Collision_Box.vPos[2], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[2], m_Geomtry_wraped.Collision_Box.vPos[3], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[3], m_Geomtry_wraped.Collision_Box.vPos[0], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
																																																									   
		// 4개																																																					   
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[4], m_Geomtry_wraped.Collision_Box.vPos[5], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[5], m_Geomtry_wraped.Collision_Box.vPos[6], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[6], m_Geomtry_wraped.Collision_Box.vPos[7], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[7], m_Geomtry_wraped.Collision_Box.vPos[4], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
																																																									   
		// 4개																																																					   
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[0], m_Geomtry_wraped.Collision_Box.vPos[4], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[1], m_Geomtry_wraped.Collision_Box.vPos[5], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[2], m_Geomtry_wraped.Collision_Box.vPos[6], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pLine_ptr->Draw(m_pContext, m_Geomtry_wraped.Collision_Box.vPos[3], m_Geomtry_wraped.Collision_Box.vPos[7], D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));


		//Select_Instance.SetMatrixs_and_Make_Ray(&m_Geomtry_wraped.m_matWorld, &(MainCamera->m_matView), &(MainCamera->m_matProj));
	
		m_Geomtry_wraped.Set_three_Matrixs(&(m_Geomtry_wraped.m_matWorld), &(MainCamera->m_matView), &(MainCamera->m_matProj));
		// 왜 nullptr로 했냐면, 이미 월드행렬이 셋팅되어 있기 때문이다.
		m_Geomtry_wraped.Render(m_pContext);

		// 여기까지 다 출력.
		if (g_Input.Left_Click) // 이렇게 카메라에서도 썼다.
		{
			if (Select_Instance.Ray_OBB_Meeting_Check_in_Same_CoordSystem(&m_Geomtry_wraped.Collision_Box)==true)
			{
				TCHAR szBuffer[256];
				//버퍼에 내용 담아주기.
				//const TCHAR* david = L"Collision bewteen Ray and OBB";
				_stprintf_s(szBuffer, _T("Collision bewteen Ray and OBB") );
			
				MessageBox(0, szBuffer, _T("충돌"), MB_OK);
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