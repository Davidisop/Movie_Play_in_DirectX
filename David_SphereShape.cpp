#include "David_Shape.h"
HRESULT David_SphereShape::LoadVertexShader(const TCHAR*  szName) {

	HRESULT hr = S_OK;

	David_Obj_C_F.m_pVertexShader.Attach(
		David_DX::LoadVertexShaderFile(m_pd3dDevice,
			szName,
			David_Obj_C_F.m_pVSBlob.GetAddressOf(), "VS_NoMatrix"));

	return hr;
}
HRESULT David_SphereShape::LoadGeometryShader(const TCHAR*  szName)
{
	return David_Shape::LoadGeometryShader(szName);
}
David_SphereShape::David_SphereShape()
{

}

David_SphereShape::~David_SphereShape()
{

}