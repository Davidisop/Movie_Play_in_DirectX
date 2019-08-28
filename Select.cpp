#include "Select.h"



David_Select::David_Select()
{

	D3DXMatrixIdentity(&m_matWorld_in_Select);
	D3DXMatrixIdentity(&m_matView_in_Select);
	D3DXMatrixIdentity(&m_matProj_in_Select);

	m_Intersection_vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vDxR= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vCenter= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


David_Select::~David_Select()
{
}


void David_Select::SetMatrixs_and_Make_Ray(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld_in_Select = *pWorld;
		m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;
	}
	if (pView != NULL)
	{
		m_matView_in_Select = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj_in_Select = *pProj;
	}

	Update_Ray();
}

void David_Select::Update_Ray()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);
	//
	D3DXVECTOR3 monitor_pixel_vector;
	monitor_pixel_vector = D3DXVECTOR3(ptCursor.x, ptCursor.y, 0);//  Near Plane 위의 점이라 가정하자. 그래서 상대 깊이값 z= 0이다.
	//
	D3DXMATRIX ViewPort_Matrix; D3DXMATRIX ViewPort_Matrix_Inverse;//직접 뷰포트 행렬을 만들었다. 단, 0,0픽셀에서 시작인.
	D3DXMatrixIdentity(&ViewPort_Matrix);
	ViewPort_Matrix._11 = g_rtClient.right / 2;
	ViewPort_Matrix._41 = g_rtClient.right / 2;
	ViewPort_Matrix._22 = -g_rtClient.bottom / 2;
	ViewPort_Matrix._42 = g_rtClient.bottom / 2;
	ViewPort_Matrix._33 = 1;
	ViewPort_Matrix._44 = 1;
	D3DXMatrixInverse(&ViewPort_Matrix_Inverse, NULL, &ViewPort_Matrix);
	//
	D3DXVECTOR3 after_Projection_vector;
	D3DXVec3TransformCoord(&after_Projection_vector, &monitor_pixel_vector, &ViewPort_Matrix_Inverse);
	
	D3DXMATRIX Projection_Matrix_inverse; 
	D3DXMatrixInverse(&Projection_Matrix_inverse, NULL, &m_matProj_in_Select);
	
	D3DXVECTOR3 after_Viewing_vector;
	D3DXVec3TransformCoord(&after_Viewing_vector, &after_Projection_vector, &Projection_Matrix_inverse);



	D3DXVECTOR3 Ray_End_Point;

	// Near Plane 기준으로 뷰좌표까지 구했다.
	m_Ray.Eye_position.x = 0.0f;
	m_Ray.Eye_position.y = 0.0f;
	m_Ray.Eye_position.z = 0.0f;
	Ray_End_Point.x = after_Viewing_vector.x;
	Ray_End_Point.y = after_Viewing_vector.y;
	Ray_End_Point.z = after_Viewing_vector.z;

	D3DXVec3Normalize(&Ray_End_Point, &Ray_End_Point);

	D3DXMATRIX Inv_matrix_view_in_Select =  m_matView_in_Select;
	D3DXMatrixInverse(&Inv_matrix_view_in_Select, nullptr, &Inv_matrix_view_in_Select);
	//
	D3DXVec3TransformCoord(&m_Ray.Eye_position, &m_Ray.Eye_position, &Inv_matrix_view_in_Select);
	D3DXVec3TransformCoord(&Ray_End_Point, &Ray_End_Point, &Inv_matrix_view_in_Select);

/*	D3DXMATRIX Inv_matrix_world_in_Select = m_matWorld_in_Select;
	D3DXMatrixInverse(&Inv_matrix_world_in_Select, nullptr, &Inv_matrix_world_in_Select);
	D3DXVec3TransformCoord(&Ray_End_Point, &Ray_End_Point, &Inv_matrix_world_in_Select)*/;

	// ㅋㅋㅋㅋ 중요한 것은 빼야죠.
	m_Ray.Direction_Normalized = Ray_End_Point - m_Ray.Eye_position;
	D3DXVec3Normalize(&m_Ray.Direction_Normalized, &m_Ray.Direction_Normalized);

	//로컬까지로 떨어뜨린거에요.
}

bool David_Select::Point_In_Polygon(
	D3DXVECTOR3 P,
	D3DXVECTOR3 A,
	D3DXVECTOR3 B,
	D3DXVECTOR3 C)
{
	//방향 판별은 내적으로. 내적이 0보다 크면, 같은 방향. 외부에 존재.
	// 내적이 0보다 작으면, 다른 방향, 내부 가능성.
	// 외부면 FALSE 반환
	// 내부면 TRUE 반환

	D3DXVECTOR3	line1_from_ancher_in_triangle;
	D3DXVECTOR3	line2_from_ancher_in_triangle;
	D3DXVECTOR3	line_from_ancher__to_P;

	line_from_ancher__to_P = P - A;//AP
	line1_from_ancher_in_triangle = B - A;//AB
	line2_from_ancher_in_triangle = C - A;//AC

	D3DXVECTOR3	Normal_compare_1;
	D3DXVECTOR3	Normal_compare_2;
	
	D3DXVec3Cross(&Normal_compare_1, &line1_from_ancher_in_triangle, &line_from_ancher__to_P);
	D3DXVec3Cross(&Normal_compare_2, &line2_from_ancher_in_triangle, &line_from_ancher__to_P);

	float Decisive_Dot;
	Decisive_Dot = D3DXVec3Dot(&Normal_compare_1, &Normal_compare_2);


	if (Decisive_Dot > 0) { return false; } //외부

	else
	{
		line_from_ancher__to_P = P - B;//BP
		line1_from_ancher_in_triangle = A - B;//BA
		line2_from_ancher_in_triangle = C - B;//BC
	
		D3DXVec3Cross(&Normal_compare_1, &line1_from_ancher_in_triangle, &line_from_ancher__to_P);
		D3DXVec3Cross(&Normal_compare_2, &line2_from_ancher_in_triangle, &line_from_ancher__to_P);

		Decisive_Dot = D3DXVec3Dot(&Normal_compare_1, &Normal_compare_2);

		if (Decisive_Dot > 0) { return false; }//외부

		else { return true; }//내부
	}
}

bool David_Select::Ray_Sphere_Meeting_Check_in_Same_CoordSystem(David_Collision_Sphere* pSphere, David_RAY* pRay)
{
	D3DXVECTOR3 A = pRay->Eye_position; 
	D3DXVECTOR3 B = pRay->Direction_Normalized;
	D3DXVECTOR3 C = pSphere->vCenter;

	//
	float Extent_for_Direction_of_Ray_here;
	// 목표 : D3DXVECTOR3 I = pRay->Direction_Normalized * Extent_for_Direction_of_Ray_here;
	
	D3DXVECTOR3	segment_in_Ray = B - A;
	// 목표 : D3DXVECTOR3 	Foot_of_Perpendicular = I - C;

	//float Decisive_Dot;
	// 원래 쓰려던 공식 : Decisive_Dot = D3DXVec3Dot(&segment_in_Ray, &Foot_of_Perpendicular) == 0;
	//AB*CI == 0;

	// 손으로 다 풀어서. OI벡터를 순수하게 segment_in_Ray*E로 본 경우.
	//Extent_for_Direction_of_Ray_here =
	//	(segment_in_Ray.x * C.x + segment_in_Ray.y * C.y + segment_in_Ray.z * C.z) /
	//	(segment_in_Ray.x * B.x + segment_in_Ray.y * B.y + segment_in_Ray.z * B.z);

	//OI벡터를 eye + segment_in_Ray*E로 본 경우.
	Extent_for_Direction_of_Ray_here =
		((segment_in_Ray.x * C.x + segment_in_Ray.y * C.y + segment_in_Ray.z * C.z) - (segment_in_Ray.x * A.x + segment_in_Ray.y * A.y + segment_in_Ray.z * A.z)) / (segment_in_Ray.x * segment_in_Ray.x + segment_in_Ray.y * segment_in_Ray.y + segment_in_Ray.z * segment_in_Ray.z)	;

	// Ray위의 I벡터가 나왔다.
	pRay->Extent_for_Direction = Extent_for_Direction_of_Ray_here;
	D3DXVECTOR3 I = pRay->Eye_position +pRay->Direction_Normalized * Extent_for_Direction_of_Ray_here;

	// 벡터 길이는 함수로 : FLOAT D3DXVec3Length	(CONST D3DXVECTOR3 *pV) 
	D3DXVECTOR3 Foot_of_Perpendicular = I - C;

	//구 반지름과 비교
	if (D3DXVec3Length(&Foot_of_Perpendicular) > pSphere->fRadius)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool David_Select::Ray_OBB_Meeting_Check_in_Same_CoordSystem(David_Collision_BOX* pBOX)
{
	D3DXVECTOR3 A = m_Ray.Eye_position;
	D3DXVECTOR3 B = m_Ray.Direction_Normalized;
	D3DXVECTOR3 C = pBOX->vCenter;

	// 목표 : D3DXVECTOR3 	Foot_of_Perpendicular = I - C;
	//AB*CI == 0;

	//OI벡터를 Oeye + ODirection_Normalized*E로 본 경우. 
	//Extent_for_Direction_of_Ray_here =  (공식을 내가 다시 계산해야 한다.)
	//	((segment_in_Ray.x * C.x + segment_in_Ray.y * C.y + segment_in_Ray.z * C.z) - (segment_in_Ray.x * A.x + segment_in_Ray.y * A.y + segment_in_Ray.z * A.z)) / (segment_in_Ray.x * segment_in_Ray.x + segment_in_Ray.y * segment_in_Ray.y + segment_in_Ray.z * segment_in_Ray.z);

	m_Ray.Extent_for_Direction = (D3DXVec3Dot(&C, &C) - D3DXVec3Dot(&A, &B)) / D3DXVec3Dot(&B, &B);


	// Ray위의 I벡터가 나왔다.
	D3DXVECTOR3 I = m_Ray.Eye_position + m_Ray.Direction_Normalized * m_Ray.Extent_for_Direction;

	// 벡터 길이는 함수로 : FLOAT D3DXVec3Length	(CONST D3DXVECTOR3 *pV) 
	D3DXVECTOR3 Foot_of_Perpendicular_Normailzed = I - C;
	D3DXVec3Normalize(&Foot_of_Perpendicular_Normailzed, &Foot_of_Perpendicular_Normailzed);
	
	// 시그마( |OBB 각축 벡터 * Foot_of_Perpendicular| ) = d;

	float Decisive_Dot[3];

	Decisive_Dot[0] = D3DXVec3Dot(&pBOX->vAxis_Normalized[0], &Foot_of_Perpendicular_Normailzed);
	Decisive_Dot[0] *= pBOX->Extent_for_Axis[0];
	if (Decisive_Dot[0] < 0) { Decisive_Dot[0] *= -1; }
	//
	Decisive_Dot[1] = D3DXVec3Dot(&pBOX->vAxis_Normalized[1], &Foot_of_Perpendicular_Normailzed);
	Decisive_Dot[1] *= pBOX->Extent_for_Axis[1];
	if (Decisive_Dot[1] < 0) { Decisive_Dot[1] *= -1; }
	//
	Decisive_Dot[2] = D3DXVec3Dot(&pBOX->vAxis_Normalized[2], &Foot_of_Perpendicular_Normailzed);
	Decisive_Dot[2] *= pBOX->Extent_for_Axis[2];
	if (Decisive_Dot[2] < 0) { Decisive_Dot[2] *= -1; }


	//CI길이 와 시그마 결과를 비교한다.
	Foot_of_Perpendicular_Normailzed = I - C;

	float Length_CI = D3DXVec3Length(&Foot_of_Perpendicular_Normailzed);
	float Length_Minimum_From_Dots = Decisive_Dot[0] + Decisive_Dot[1] + Decisive_Dot[2];

	if (Length_CI < Length_Minimum_From_Dots)
	{
		return true; 
	}
	else
	{
		return false;
	};
}


bool David_Select::Ray_Plane_MeetCheck_Get_IntersectVector_in_Same_CoordSystem
(David_Collision_PLANE* pPlane, David_RAY* pRay)
{

	D3DXVECTOR3 A = pRay->Eye_position; 
	D3DXVECTOR3 A_Direction = pRay->Direction_Normalized;

	//1 .평행한 경우 혹은 직선이 평면에 포함된 경우 제외
	if (D3DXVec3Dot(&pRay->Direction_Normalized, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC)) < 0.0001f) // Float 오류 보정.
	{
		return false;
	}

	float Decisive_float_out;
	D3DXVECTOR3 B;

	// 2. (눈 좌표 A와 평면에 대해) 반대 편의 점 B를 구함.

	// Ray의 시작점을 대입 : 양수, 음수 부호를 판독.
    // 평면의 앞 점 인지, 뒷 점 인지 판단.
	// 서로 다른 쪽에 존재해야 함.

	Decisive_float_out = A.x*pPlane->fA + A.y*pPlane->fB + A.z*pPlane->fC + pPlane->fD;

	for (int integer = -1000; integer < 1001; integer =+100)
	{ // Ray 직선 안의 점을 평면의 방정식에 대입한다.
		float Decisive_float_in =
		(A.x + A_Direction.x* integer) * pPlane->fA +
		(A.y + A_Direction.y* integer) * pPlane->fB +
		(A.z + A_Direction.z* integer) * pPlane->fC +
		pPlane->fD;
		
		if (Decisive_float_out*Decisive_float_in < 0) // 면을 기준으로 서로 다른 쪽에 존재함.	
		{
			B.x = A.x + A_Direction.x* integer;
			B.y = A.x + A_Direction.y* integer;
			B.z = A.x + A_Direction.z* integer;
			break;
		};
	}
	
	// 3.   평면위의 임의의 점 E을 구해서, |AE * 평면노멀벡터|로 |AI| 길이를 구함. ( I 벡터는 A에서 평면에 내린 수선의 발Foot.)
	// 4.   |AB * 평면노멀벡터| 로 |AI'| 길이를 구함.
	// 5.   삼각형 닮음비로 AC벡터 = (AI_Length / AL_Length) *AB벡터
	// 6.   결과 OC 벡터 = OA + AC;

	D3DXVECTOR3 Easy_Point_on_Plane;

	bool Decisive_bool1 = pPlane->fA;
	bool Decisive_bool2 = pPlane->fB;
	bool Decisive_bool3 = pPlane->fC;

	//a,b,c 중 0개가 = 0 일 때, 평면 위 임의 점 Arbitary_on_Plane = (0,0,X);

	if (Decisive_bool1 == true && Decisive_bool2 == true && Decisive_bool3 == true)
	{
		Easy_Point_on_Plane.x = 0.0f;
		Easy_Point_on_Plane.y = 0.0f;
		Easy_Point_on_Plane.z = -pPlane->fD/ pPlane->fC;
	

		// |AI| 길이 = |AE * 평면노멀벡터|
		float AI_Length;
		D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
		AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AI_Length < 0) { AI_Length *= -1; }

		// |AI'| 길이 = |AB * 평면노멀벡터|
		float AL_Length;
		D3DXVECTOR3 AB = B - A;
		AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AL_Length < 0) { AL_Length *= -1; }

		// 닮음비로 AC벡터 = (AI_Length/AL_Length) *AB벡터
		D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

		//최종 결과 OC = OA+AC

		D3DXVECTOR3 C = A + AC;
		m_Intersection_vector = C;
		return true;
	}

	//a,b,c 중 1개가 = 0 일 때,


	else if (Decisive_bool1 == false && Decisive_bool2 == true && Decisive_bool3 == true)
	{
		Easy_Point_on_Plane.x = 0.0f;
		Easy_Point_on_Plane.y = 0.0f;
		Easy_Point_on_Plane.z = -pPlane->fD / pPlane->fC;

		// |AI| 길이 = |AE * 평면노멀벡터|
		float AI_Length;
		D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
		AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AI_Length < 0) { AI_Length *= -1; }

		// |AI'| 길이 = |AB * 평면노멀벡터|
		float AL_Length;
		D3DXVECTOR3 AB = B - A;
		AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AL_Length < 0) { AL_Length *= -1; }

		// 닮음비로 AC벡터 = (AI_Length/AL_Length) *AB벡터
		D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

		//최종 결과 OC = OA+AC

		D3DXVECTOR3 C = A + AC;
		m_Intersection_vector = C;
		return true;
	}

	else if (Decisive_bool1 == true && Decisive_bool2 == false && Decisive_bool3 == true)
	{
		Easy_Point_on_Plane.x = 0.0f;
		Easy_Point_on_Plane.y =  0.0f;
		Easy_Point_on_Plane.z = -pPlane->fD / pPlane->fC;

		// |AI| 길이 = |AE * 평면노멀벡터|
		float AI_Length;
		D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
		AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AI_Length < 0) { AI_Length *= -1; }

		// |AI'| 길이 = |AB * 평면노멀벡터|
		float AL_Length;
		D3DXVECTOR3 AB = B - A;
		AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AL_Length < 0) { AL_Length *= -1; }

		// 닮음비로 AC벡터 = (AI_Length/AL_Length) *AB벡터
		D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

		//최종 결과 OC = OA+AC

		D3DXVECTOR3 C = A + AC;
		m_Intersection_vector = C;
		return true;
	}


	else if (Decisive_bool1 == true && Decisive_bool2 == true && Decisive_bool3 == false)
	{
		Easy_Point_on_Plane.x = 0.0f;
		Easy_Point_on_Plane.y = -pPlane->fD / pPlane->fB;
		Easy_Point_on_Plane.z = 0.0f;

		// |AI| 길이 = |AE * 평면노멀벡터|
		float AI_Length;
		D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
		AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AI_Length < 0) { AI_Length *= -1; }

		// |AI'| 길이 = |AB * 평면노멀벡터|
		float AL_Length;
		D3DXVECTOR3 AB = B - A;
		AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AL_Length < 0) { AL_Length *= -1; }

		// 닮음비로 AC벡터 = (AI_Length/AL_Length) *AB벡터
		D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

		//최종 결과 OC = OA+AC

		D3DXVECTOR3 C = A + AC;
		m_Intersection_vector = C;
		return true;
	}

		//a,b,c 중 2개가 = 0 일 때,


	else if (Decisive_bool1 == true && Decisive_bool2 == false && Decisive_bool3 == false)
	{
	Easy_Point_on_Plane.x = -pPlane->fD / pPlane->fA;
	Easy_Point_on_Plane.y = 0.0f;
	Easy_Point_on_Plane.z = 0.0f;

	// |AI| 길이 = |AE * 평면노멀벡터|
	float AI_Length;
	D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
	AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AI_Length < 0) { AI_Length *= -1; }

	// |AI'| 길이 = |AB * 평면노멀벡터|
	float AL_Length;
	D3DXVECTOR3 AB = B - A;
	AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AL_Length < 0) { AL_Length *= -1; }

	// 닮음비로 AC벡터 = (AI_Length/AL_Length) *AB벡터
	D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

	//최종 결과 OC = OA+AC

	D3DXVECTOR3 C = A + AC;
	m_Intersection_vector = C;
	return true;
	}


	//a,b,c 중 2개가 = 0 일 때,


	else if (Decisive_bool1 == false && Decisive_bool2 == true && Decisive_bool3 == false)
	{
	Easy_Point_on_Plane.x = 0.0f;
	Easy_Point_on_Plane.y = -pPlane->fD / pPlane->fB;
	Easy_Point_on_Plane.z = 0.0f;

	// |AI| 길이 = |AE * 평면노멀벡터|
	float AI_Length;
	D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
	AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AI_Length < 0) { AI_Length *= -1; }

	// |AI'| 길이 = |AB * 평면노멀벡터|
	float AL_Length;
	D3DXVECTOR3 AB = B - A;
	AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AL_Length < 0) { AL_Length *= -1; }

	// 닮음비로 AC벡터 = (AI_Length/AL_Length) *AB벡터
	D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

	//최종 결과 OC = OA+AC

	D3DXVECTOR3 C = A + AC;
	m_Intersection_vector = C;
	return true;
	}


	else if (Decisive_bool1 == false && Decisive_bool2 == false && Decisive_bool3 == true)
	{
	Easy_Point_on_Plane.x = 0.0f;
	Easy_Point_on_Plane.y = 0.0f;
	Easy_Point_on_Plane.z = -pPlane->fD / pPlane->fC;

	// |AI| 길이 = |AE * 평면노멀벡터|
	float AI_Length;
	D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
	AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AI_Length < 0) { AI_Length *= -1; }

	// |AI'| 길이 = |AB * 평면노멀벡터|
	float AL_Length;
	D3DXVECTOR3 AB = B - A;
	AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AL_Length < 0) { AL_Length *= -1; }

	// 닮음비로 AC벡터 = (AI_Length/AL_Length) *AB벡터
	D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

	//최종 결과 OC = OA+AC

	D3DXVECTOR3 C = A + AC;
	m_Intersection_vector = C;
	return true;
	}

};