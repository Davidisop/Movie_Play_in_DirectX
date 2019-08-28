#include "xCore.h"

class Sample : public xCore
{
public:

	David_Select Select_Instance;
	//
	David_RAY						m_Ray;
	David_Collision_BOX		m_AABB_TO_OBB_Box;
	David_Box_Shape			m_Geomtry_wraped;
	David_Line_Shape*			m_pLine_ptr;
	//
	D3DXVECTOR3 m_Intersection_Sample;
	D3DXVECTOR3 m_vDxR;

public:

	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	Sample();
	~Sample();

};

