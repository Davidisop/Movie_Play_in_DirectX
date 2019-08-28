#pragma once
#include "David_Obj_Compo_Func.h"

struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
};
struct PNCT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR3		vn,
		D3DXVECTOR4		vc,
		D3DXVECTOR2     vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};
struct PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
};

struct T_CB_DATA
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	float fTime;
	float fActiveVP;
	float z, w;
	D3DXVECTOR4  vColor;

	T_CB_DATA()
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matWorld);
		fTime=0;
		fActiveVP = 0;
		z = 0; w = 0;
		vColor = { 1.0f,1.0f,1.0f,1.0f };
	}
};


struct David_RAY
{
public:
	D3DXVECTOR3		Eye_position;
public:
	D3DXVECTOR3		Direction_Normalized;
	float							Extent_for_Direction;

	David_RAY()
	{
		//  fExtent > 0 : 세그먼트로 사용 
		Extent_for_Direction = -1;
	}
};


struct David_Collision_Sphere
{
	D3DXVECTOR3		vCenter;
	float							fRadius;
};


struct David_Collision_BOX
{
	// Common
	D3DXVECTOR3		vCenter;
	D3DXVECTOR3		vPos[8];
	// AABB
	D3DXVECTOR3		vMax;
	D3DXVECTOR3		vMin;
	// OBB
	D3DXVECTOR3		vAxis[3];
	float							fExtent[3];

	David_Collision_BOX()
	{
		vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	}
};


struct David_Collision_PLANE
{
	float	fA, fB, fC, fD;
	bool	CreatePlane(D3DXVECTOR3 Vertex0, D3DXVECTOR3 Vertex1, D3DXVECTOR3 Vertex2)
	{
		D3DXVECTOR3 Egde0;
		D3DXVECTOR3 Egde1;
		Egde0 = Vertex0 - Vertex1;
		Egde1 = Vertex0 - Vertex2;

		D3DXVECTOR3 Normal_Vector_Nomalized;
		D3DXVec3Cross(&Normal_Vector_Nomalized, &Egde0, &Egde1);
		D3DXVec3Normalize(&Normal_Vector_Nomalized, &Normal_Vector_Nomalized);

		fA = Normal_Vector_Nomalized.x;
		fB = Normal_Vector_Nomalized.y;
		fC = Normal_Vector_Nomalized.z;

		//평면 위의 점이면 =0 을 만족하므로,
		fD = -(fA*Vertex2.x + fB * Vertex2.y + fC * Vertex2.z);
		return true;
	}

	bool	CreatePlane(D3DXVECTOR3 Normal_Vector, D3DXVECTOR3 Vertex_any_on_Plane)
	{
		D3DXVECTOR3 Normal_Vector_Nomalized;
		D3DXVec3Normalize(&Normal_Vector_Nomalized, &Normal_Vector);


		fA = Normal_Vector_Nomalized.x;
		fB = Normal_Vector_Nomalized.y;
		fC = Normal_Vector_Nomalized.z;

		fD = -(fA*Vertex_any_on_Plane.x + fB * Vertex_any_on_Plane.y + fC * Vertex_any_on_Plane.z);

		return true;
	}

	void	Normalize_abc_to_get_distance_direction_from_any_vector()
	{
		float square_sigma = sqrt(fA*fA + fB * fB + fC * fC);
		fA = fA / square_sigma;
		fB = fB / square_sigma;
		fC = fC / square_sigma;

		fD = fD / square_sigma;
	}
};

