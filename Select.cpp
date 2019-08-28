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
	monitor_pixel_vector = D3DXVECTOR3(ptCursor.x, ptCursor.y, 0);//  Near Plane ���� ���̶� ��������. �׷��� ��� ���̰� z= 0�̴�.
	//
	D3DXMATRIX ViewPort_Matrix; D3DXMATRIX ViewPort_Matrix_Inverse;//���� ����Ʈ ����� �������. ��, 0,0�ȼ����� ������.
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

	// Near Plane �������� ����ǥ���� ���ߴ�.
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

	// �������� �߿��� ���� ������.
	m_Ray.Direction_Normalized = Ray_End_Point - m_Ray.Eye_position;
	D3DXVec3Normalize(&m_Ray.Direction_Normalized, &m_Ray.Direction_Normalized);

	//���ñ����� ����߸��ſ���.
}

bool David_Select::Point_In_Polygon(
	D3DXVECTOR3 P,
	D3DXVECTOR3 A,
	D3DXVECTOR3 B,
	D3DXVECTOR3 C)
{
	//���� �Ǻ��� ��������. ������ 0���� ũ��, ���� ����. �ܺο� ����.
	// ������ 0���� ������, �ٸ� ����, ���� ���ɼ�.
	// �ܺθ� FALSE ��ȯ
	// ���θ� TRUE ��ȯ

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


	if (Decisive_Dot > 0) { return false; } //�ܺ�

	else
	{
		line_from_ancher__to_P = P - B;//BP
		line1_from_ancher_in_triangle = A - B;//BA
		line2_from_ancher_in_triangle = C - B;//BC
	
		D3DXVec3Cross(&Normal_compare_1, &line1_from_ancher_in_triangle, &line_from_ancher__to_P);
		D3DXVec3Cross(&Normal_compare_2, &line2_from_ancher_in_triangle, &line_from_ancher__to_P);

		Decisive_Dot = D3DXVec3Dot(&Normal_compare_1, &Normal_compare_2);

		if (Decisive_Dot > 0) { return false; }//�ܺ�

		else { return true; }//����
	}
}

bool David_Select::Ray_Sphere_Meeting_Check_in_Same_CoordSystem(David_Collision_Sphere* pSphere, David_RAY* pRay)
{
	D3DXVECTOR3 A = pRay->Eye_position; 
	D3DXVECTOR3 B = pRay->Direction_Normalized;
	D3DXVECTOR3 C = pSphere->vCenter;

	//
	float Extent_for_Direction_of_Ray_here;
	// ��ǥ : D3DXVECTOR3 I = pRay->Direction_Normalized * Extent_for_Direction_of_Ray_here;
	
	D3DXVECTOR3	segment_in_Ray = B - A;
	// ��ǥ : D3DXVECTOR3 	Foot_of_Perpendicular = I - C;

	//float Decisive_Dot;
	// ���� ������ ���� : Decisive_Dot = D3DXVec3Dot(&segment_in_Ray, &Foot_of_Perpendicular) == 0;
	//AB*CI == 0;

	// ������ �� Ǯ�. OI���͸� �����ϰ� segment_in_Ray*E�� �� ���.
	//Extent_for_Direction_of_Ray_here =
	//	(segment_in_Ray.x * C.x + segment_in_Ray.y * C.y + segment_in_Ray.z * C.z) /
	//	(segment_in_Ray.x * B.x + segment_in_Ray.y * B.y + segment_in_Ray.z * B.z);

	//OI���͸� eye + segment_in_Ray*E�� �� ���.
	Extent_for_Direction_of_Ray_here =
		((segment_in_Ray.x * C.x + segment_in_Ray.y * C.y + segment_in_Ray.z * C.z) - (segment_in_Ray.x * A.x + segment_in_Ray.y * A.y + segment_in_Ray.z * A.z)) / (segment_in_Ray.x * segment_in_Ray.x + segment_in_Ray.y * segment_in_Ray.y + segment_in_Ray.z * segment_in_Ray.z)	;

	// Ray���� I���Ͱ� ���Դ�.
	pRay->Extent_for_Direction = Extent_for_Direction_of_Ray_here;
	D3DXVECTOR3 I = pRay->Eye_position +pRay->Direction_Normalized * Extent_for_Direction_of_Ray_here;

	// ���� ���̴� �Լ��� : FLOAT D3DXVec3Length	(CONST D3DXVECTOR3 *pV) 
	D3DXVECTOR3 Foot_of_Perpendicular = I - C;

	//�� �������� ��
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

	// ��ǥ : D3DXVECTOR3 	Foot_of_Perpendicular = I - C;
	//AB*CI == 0;

	//OI���͸� Oeye + ODirection_Normalized*E�� �� ���. 
	//Extent_for_Direction_of_Ray_here =  (������ ���� �ٽ� ����ؾ� �Ѵ�.)
	//	((segment_in_Ray.x * C.x + segment_in_Ray.y * C.y + segment_in_Ray.z * C.z) - (segment_in_Ray.x * A.x + segment_in_Ray.y * A.y + segment_in_Ray.z * A.z)) / (segment_in_Ray.x * segment_in_Ray.x + segment_in_Ray.y * segment_in_Ray.y + segment_in_Ray.z * segment_in_Ray.z);

	m_Ray.Extent_for_Direction = (D3DXVec3Dot(&C, &C) - D3DXVec3Dot(&A, &B)) / D3DXVec3Dot(&B, &B);


	// Ray���� I���Ͱ� ���Դ�.
	D3DXVECTOR3 I = m_Ray.Eye_position + m_Ray.Direction_Normalized * m_Ray.Extent_for_Direction;

	// ���� ���̴� �Լ��� : FLOAT D3DXVec3Length	(CONST D3DXVECTOR3 *pV) 
	D3DXVECTOR3 Foot_of_Perpendicular_Normailzed = I - C;
	D3DXVec3Normalize(&Foot_of_Perpendicular_Normailzed, &Foot_of_Perpendicular_Normailzed);
	
	// �ñ׸�( |OBB ���� ���� * Foot_of_Perpendicular| ) = d;

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


	//CI���� �� �ñ׸� ����� ���Ѵ�.
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

	//1 .������ ��� Ȥ�� ������ ��鿡 ���Ե� ��� ����
	if (D3DXVec3Dot(&pRay->Direction_Normalized, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC)) < 0.0001f) // Float ���� ����.
	{
		return false;
	}

	float Decisive_float_out;
	D3DXVECTOR3 B;

	// 2. (�� ��ǥ A�� ��鿡 ����) �ݴ� ���� �� B�� ����.

	// Ray�� �������� ���� : ���, ���� ��ȣ�� �ǵ�.
    // ����� �� �� ����, �� �� ���� �Ǵ�.
	// ���� �ٸ� �ʿ� �����ؾ� ��.

	Decisive_float_out = A.x*pPlane->fA + A.y*pPlane->fB + A.z*pPlane->fC + pPlane->fD;

	for (int integer = -1000; integer < 1001; integer =+100)
	{ // Ray ���� ���� ���� ����� �����Ŀ� �����Ѵ�.
		float Decisive_float_in =
		(A.x + A_Direction.x* integer) * pPlane->fA +
		(A.y + A_Direction.y* integer) * pPlane->fB +
		(A.z + A_Direction.z* integer) * pPlane->fC +
		pPlane->fD;
		
		if (Decisive_float_out*Decisive_float_in < 0) // ���� �������� ���� �ٸ� �ʿ� ������.	
		{
			B.x = A.x + A_Direction.x* integer;
			B.y = A.x + A_Direction.y* integer;
			B.z = A.x + A_Direction.z* integer;
			break;
		};
	}
	
	// 3.   ������� ������ �� E�� ���ؼ�, |AE * ����ֺ���|�� |AI| ���̸� ����. ( I ���ʹ� A���� ��鿡 ���� ������ ��Foot.)
	// 4.   |AB * ����ֺ���| �� |AI'| ���̸� ����.
	// 5.   �ﰢ�� ������� AC���� = (AI_Length / AL_Length) *AB����
	// 6.   ��� OC ���� = OA + AC;

	D3DXVECTOR3 Easy_Point_on_Plane;

	bool Decisive_bool1 = pPlane->fA;
	bool Decisive_bool2 = pPlane->fB;
	bool Decisive_bool3 = pPlane->fC;

	//a,b,c �� 0���� = 0 �� ��, ��� �� ���� �� Arbitary_on_Plane = (0,0,X);

	if (Decisive_bool1 == true && Decisive_bool2 == true && Decisive_bool3 == true)
	{
		Easy_Point_on_Plane.x = 0.0f;
		Easy_Point_on_Plane.y = 0.0f;
		Easy_Point_on_Plane.z = -pPlane->fD/ pPlane->fC;
	

		// |AI| ���� = |AE * ����ֺ���|
		float AI_Length;
		D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
		AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AI_Length < 0) { AI_Length *= -1; }

		// |AI'| ���� = |AB * ����ֺ���|
		float AL_Length;
		D3DXVECTOR3 AB = B - A;
		AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AL_Length < 0) { AL_Length *= -1; }

		// ������� AC���� = (AI_Length/AL_Length) *AB����
		D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

		//���� ��� OC = OA+AC

		D3DXVECTOR3 C = A + AC;
		m_Intersection_vector = C;
		return true;
	}

	//a,b,c �� 1���� = 0 �� ��,


	else if (Decisive_bool1 == false && Decisive_bool2 == true && Decisive_bool3 == true)
	{
		Easy_Point_on_Plane.x = 0.0f;
		Easy_Point_on_Plane.y = 0.0f;
		Easy_Point_on_Plane.z = -pPlane->fD / pPlane->fC;

		// |AI| ���� = |AE * ����ֺ���|
		float AI_Length;
		D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
		AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AI_Length < 0) { AI_Length *= -1; }

		// |AI'| ���� = |AB * ����ֺ���|
		float AL_Length;
		D3DXVECTOR3 AB = B - A;
		AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AL_Length < 0) { AL_Length *= -1; }

		// ������� AC���� = (AI_Length/AL_Length) *AB����
		D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

		//���� ��� OC = OA+AC

		D3DXVECTOR3 C = A + AC;
		m_Intersection_vector = C;
		return true;
	}

	else if (Decisive_bool1 == true && Decisive_bool2 == false && Decisive_bool3 == true)
	{
		Easy_Point_on_Plane.x = 0.0f;
		Easy_Point_on_Plane.y =  0.0f;
		Easy_Point_on_Plane.z = -pPlane->fD / pPlane->fC;

		// |AI| ���� = |AE * ����ֺ���|
		float AI_Length;
		D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
		AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AI_Length < 0) { AI_Length *= -1; }

		// |AI'| ���� = |AB * ����ֺ���|
		float AL_Length;
		D3DXVECTOR3 AB = B - A;
		AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AL_Length < 0) { AL_Length *= -1; }

		// ������� AC���� = (AI_Length/AL_Length) *AB����
		D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

		//���� ��� OC = OA+AC

		D3DXVECTOR3 C = A + AC;
		m_Intersection_vector = C;
		return true;
	}


	else if (Decisive_bool1 == true && Decisive_bool2 == true && Decisive_bool3 == false)
	{
		Easy_Point_on_Plane.x = 0.0f;
		Easy_Point_on_Plane.y = -pPlane->fD / pPlane->fB;
		Easy_Point_on_Plane.z = 0.0f;

		// |AI| ���� = |AE * ����ֺ���|
		float AI_Length;
		D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
		AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AI_Length < 0) { AI_Length *= -1; }

		// |AI'| ���� = |AB * ����ֺ���|
		float AL_Length;
		D3DXVECTOR3 AB = B - A;
		AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
		if (AL_Length < 0) { AL_Length *= -1; }

		// ������� AC���� = (AI_Length/AL_Length) *AB����
		D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

		//���� ��� OC = OA+AC

		D3DXVECTOR3 C = A + AC;
		m_Intersection_vector = C;
		return true;
	}

		//a,b,c �� 2���� = 0 �� ��,


	else if (Decisive_bool1 == true && Decisive_bool2 == false && Decisive_bool3 == false)
	{
	Easy_Point_on_Plane.x = -pPlane->fD / pPlane->fA;
	Easy_Point_on_Plane.y = 0.0f;
	Easy_Point_on_Plane.z = 0.0f;

	// |AI| ���� = |AE * ����ֺ���|
	float AI_Length;
	D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
	AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AI_Length < 0) { AI_Length *= -1; }

	// |AI'| ���� = |AB * ����ֺ���|
	float AL_Length;
	D3DXVECTOR3 AB = B - A;
	AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AL_Length < 0) { AL_Length *= -1; }

	// ������� AC���� = (AI_Length/AL_Length) *AB����
	D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

	//���� ��� OC = OA+AC

	D3DXVECTOR3 C = A + AC;
	m_Intersection_vector = C;
	return true;
	}


	//a,b,c �� 2���� = 0 �� ��,


	else if (Decisive_bool1 == false && Decisive_bool2 == true && Decisive_bool3 == false)
	{
	Easy_Point_on_Plane.x = 0.0f;
	Easy_Point_on_Plane.y = -pPlane->fD / pPlane->fB;
	Easy_Point_on_Plane.z = 0.0f;

	// |AI| ���� = |AE * ����ֺ���|
	float AI_Length;
	D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
	AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AI_Length < 0) { AI_Length *= -1; }

	// |AI'| ���� = |AB * ����ֺ���|
	float AL_Length;
	D3DXVECTOR3 AB = B - A;
	AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AL_Length < 0) { AL_Length *= -1; }

	// ������� AC���� = (AI_Length/AL_Length) *AB����
	D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

	//���� ��� OC = OA+AC

	D3DXVECTOR3 C = A + AC;
	m_Intersection_vector = C;
	return true;
	}


	else if (Decisive_bool1 == false && Decisive_bool2 == false && Decisive_bool3 == true)
	{
	Easy_Point_on_Plane.x = 0.0f;
	Easy_Point_on_Plane.y = 0.0f;
	Easy_Point_on_Plane.z = -pPlane->fD / pPlane->fC;

	// |AI| ���� = |AE * ����ֺ���|
	float AI_Length;
	D3DXVECTOR3 AE = Easy_Point_on_Plane - A;
	AI_Length = D3DXVec3Dot(&AE, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AI_Length < 0) { AI_Length *= -1; }

	// |AI'| ���� = |AB * ����ֺ���|
	float AL_Length;
	D3DXVECTOR3 AB = B - A;
	AL_Length = D3DXVec3Dot(&AB, &D3DXVECTOR3(pPlane->fA, pPlane->fB, pPlane->fC));
	if (AL_Length < 0) { AL_Length *= -1; }

	// ������� AC���� = (AI_Length/AL_Length) *AB����
	D3DXVECTOR3 AC = (AI_Length / AL_Length)*AB;

	//���� ��� OC = OA+AC

	D3DXVECTOR3 C = A + AC;
	m_Intersection_vector = C;
	return true;
	}

};