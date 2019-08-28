#pragma once
#include "xstd.h"
namespace David_DX
{
	ID3D11Buffer* CreateVertexBuffer(
		ID3D11Device* pd3dDevice,
		UINT iNumVertex,
		UINT iVertexSize,
		VOID* data,
		ID3D11Buffer** ppBuffer=nullptr);
	ID3D11Buffer* CreateIndexBuffer(
		ID3D11Device* pd3dDevice,
		UINT iNumCount,
		UINT iSize,
		VOID* data,
		ID3D11Buffer** ppBuffer);
	ID3D11Buffer* CreateConstantBuffer(
		ID3D11Device* pd3dDevice,
		UINT iNumCount,
		UINT iSize,
		ID3D11Buffer** ppBuffer,
		VOID* data = NULL,
		bool bDynamic = false);
	//
	ID3D11InputLayout* CreateInputLayout(
		ID3D11Device* pd3dDevice,
		DWORD dwSize,
		LPCVOID lpData,
		D3D11_INPUT_ELEMENT_DESC* layout,
		UINT numElements,
		ID3D11InputLayout** ppInputLayout);

	ID3D11VertexShader*  LoadVertexShaderFile(
		ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut = nullptr,
		const char*  pFuntionName = "VS");
	ID3D11PixelShader*  LoadPixelShaderFile(
		ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut = nullptr,
		const char*  pFuntionName = "PS");
	ID3D11GeometryShader*  LoadGeometryShaderFile(
		ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut=nullptr,
		const char*  pFuntionName="GS");

	HRESULT CompileShaderFromFile(const WCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut);

	ID3D11ShaderResourceView* CreateShaderResourceView(ID3D11Device* pd3dDevice, const TCHAR* szFileName);

	class David_Obj_Compo_Func
	{
	public:
		ComPtr<ID3D11Buffer> m_pVertexBuffer;
		ComPtr<ID3D11Buffer> m_pIndexBuffer;
		ComPtr<ID3D11Buffer> m_pConstantBuffer;
		ComPtr<ID3D11InputLayout>  m_pInputLayout;
		ComPtr<ID3D11VertexShader> m_pVertexShader;
		ComPtr<ID3D11PixelShader>  m_pPixelShader;
		ComPtr<ID3D11GeometryShader> m_pGeometryShader;
		ComPtr<ID3DBlob>           m_pVSBlob;
		ComPtr<ID3D11ShaderResourceView> m_pTextureRV;
	public:
		bool  PreRender(ID3D11DeviceContext* pContext,
			UINT iVertexSize);
		bool  Render(ID3D11DeviceContext* pContext,
			UINT iVertexSize, UINT iCount);
		bool  PostRender(ID3D11DeviceContext* pContext,
			UINT iVertexSize, UINT iCount);
		bool  Release();
	public:
		David_Obj_Compo_Func();
		virtual ~David_Obj_Compo_Func();
	};
}
using namespace David_DX;
