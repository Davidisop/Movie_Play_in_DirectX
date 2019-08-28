#pragma once
#include "David_shape.h"



class David_Select
{
public:
	David_RAY		    m_Ray;
	D3DXVECTOR3 m_Intersection_vector; //±³Á¡

public:
	D3DXMATRIX     m_matWorld_in_Select;
	D3DXMATRIX     m_matView_in_Select;
	D3DXMATRIX     m_matProj_in_Select;

public:
	D3DXVECTOR3 m_vDxR;
	D3DXVECTOR3 m_vCenter;

public:
	D3DXVECTOR3		m_vSrcVex[3];

public:
	float			m_fPickDistance;

public:
	D3DXMATRIX	  m_matWorldPick;

public:

	D3DXVECTOR3 qvec;
	D3DXVECTOR3 tvec;
	D3DXVECTOR3 pvec;

public:

	bool Point_In_Polygon(D3DXVECTOR3 vert, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	void SetMatrixs_and_Make_Ray(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	void Update_Ray();

public:

	bool Ray_Sphere_Meeting_Check_in_Same_CoordSystem(David_Collision_Sphere* p_Collision_Sphere, David_RAY* pRay );
	bool Ray_OBB_Meeting_Check_in_Same_CoordSystem(David_Collision_BOX* p_Collision_Box);

public:

	bool Ray_Plane_MeetCheck_Get_IntersectVector_in_Same_CoordSystem(David_Collision_PLANE* pPlane, David_RAY* pRay);

	David_Select();
	virtual ~David_Select();
};

