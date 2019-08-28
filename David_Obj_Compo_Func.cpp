#include "David_Obj_Compo_Func.h"
namespace David_DX
{
	ID3D11Buffer* CreateVertexBuffer(
		ID3D11Device* pd3dDevice,
		UINT iNumCount,
		UINT iSize,
		VOID* data,
		ID3D11Buffer** ppBuffer)
	{
		HRESULT hr;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&bd, sizeof(bd));
		ZeroMemory(&sd, sizeof(sd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.ByteWidth = iNumCount * iSize;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		sd.pSysMem = data;
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &sd, &pBuffer)))
		{
			return nullptr;
		}
		*ppBuffer = pBuffer;
		return pBuffer;
	}
	ID3D11Buffer* CreateIndexBuffer(
		ID3D11Device* pd3dDevice,
		UINT iNumCount,
		UINT iSize,
		VOID* data,
		ID3D11Buffer** ppBuffer)
	{
		HRESULT hr;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&bd, sizeof(bd));
		ZeroMemory(&sd, sizeof(sd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.ByteWidth = iNumCount * iSize;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		sd.pSysMem = data;
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &sd, &pBuffer)))
		{
			return nullptr;
		}
		*ppBuffer = pBuffer;
		return pBuffer;
	}
	ID3D11Buffer* CreateConstantBuffer(
		ID3D11Device* pd3dDevice,
		UINT iNumCount,
		UINT iSize,
		ID3D11Buffer** ppBuffer,
		VOID* data,
		bool bDynamic)
	{
		HRESULT hr;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&bd, sizeof(bd));
		ZeroMemory(&sd, sizeof(sd));

		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		bd.ByteWidth = iNumCount * iSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (data != NULL)
		{
			sd.pSysMem = data;
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &sd, &pBuffer)))
			{
				return nullptr;
			}
		}
		else
		{
			sd.pSysMem = data;
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
			{
				return nullptr;
			}
		}
		*ppBuffer = pBuffer;
		return pBuffer;
	}

	ID3D11InputLayout* CreateInputLayout(
		ID3D11Device* pd3dDevice,
		DWORD dwSize,
		LPCVOID lpData,
		D3D11_INPUT_ELEMENT_DESC* layout,
		UINT numElements,
		ID3D11InputLayout** ppInputLayout)
	{
		HRESULT hr;
		ID3D11InputLayout* pInputLayout;
		if (FAILED(hr = pd3dDevice->CreateInputLayout(layout,
			numElements,
			lpData,
			dwSize,
			&pInputLayout)))
		{
			return nullptr;
		}
		*ppInputLayout = pInputLayout;
		return pInputLayout;
	}

	HRESULT CompileShaderFromFile(const WCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )	
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			if (pErrorBlob != NULL)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			if (pErrorBlob) pErrorBlob->Release();
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}

	ID3D11VertexShader*  LoadVertexShaderFile(
		ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut,
		const char*  pFuntionName)
	{
		HRESULT hr;
		ID3D11VertexShader* pVertexShader;
		ID3DBlob* pBlob;
		if (FAILED(hr = CompileShaderFromFile(
			(TCHAR*)pShaderFile,
			pFuntionName,
			"vs_5_0",
			&pBlob)))
		{
			return nullptr;
		}
		if (FAILED(hr = pd3dDevice->CreateVertexShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			NULL,
			&pVertexShader)))
		{
			if (pBlob)pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
		{
			if (pBlob)pBlob->Release();
		}
		else
		{
			*ppBlobOut = pBlob;
		}
		return pVertexShader;
	}
	ID3D11PixelShader*  LoadPixelShaderFile(
		ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut,
		const char*  pFuntionName)
	{
		HRESULT hr;
		ID3D11PixelShader* pPixelShader;
		ID3DBlob* pBlob;
		if (FAILED(hr = CompileShaderFromFile(
			(TCHAR*)pShaderFile,
			pFuntionName,
			"ps_5_0",
			&pBlob)))
		{
			return nullptr;
		}
		if (FAILED(hr = pd3dDevice->CreatePixelShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			NULL,
			&pPixelShader)))
		{
			if (pBlob)pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
		{
			if (pBlob)pBlob->Release();
		}
		else
		{
			*ppBlobOut = pBlob;
		}
		return pPixelShader;
	}
	ID3D11GeometryShader*  LoadGeometryShaderFile(
		ID3D11Device* pd3dDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut,
		const char*  pFuntionName)
	{
		HRESULT hr;
		ID3D11GeometryShader* pGShader;
		ID3DBlob* pBlob;
		if (FAILED(hr = CompileShaderFromFile(
			(TCHAR*)pShaderFile,
			pFuntionName,
			"gs_5_0",
			&pBlob)))
		{
			return nullptr;
		}
		if (FAILED(hr = pd3dDevice->CreateGeometryShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			NULL,
			&pGShader)))
		{
			if (pBlob)pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
		{
			if (pBlob)pBlob->Release();
		}
		else
		{
			*ppBlobOut = pBlob;
		}
		return pGShader;
	}





	ID3D11ShaderResourceView* CreateShaderResourceView(ID3D11Device* pd3dDevice, const TCHAR* szFileName)
	{
		HRESULT hr = S_OK;
		ID3D11ShaderResourceView* pSRV = nullptr;
		D3DX11_IMAGE_LOAD_INFO loadinfo;
		ZeroMemory(&loadinfo, sizeof(loadinfo));
		loadinfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		loadinfo.Format = DXGI_FORMAT_FROM_FILE;

		ID3D11ShaderResourceView* pTex = NULL;

		hr = D3DX11CreateShaderResourceViewFromFile(
			pd3dDevice,
			szFileName,
			&loadinfo, NULL,
			&pSRV, NULL);
		if (FAILED(hr))
		{
			return nullptr;
		}
		return pSRV;
	}




	bool  David_Obj_Compo_Func::PreRender(
		ID3D11DeviceContext* pContext,
		UINT iVertexSize)
	{
		pContext->IASetInputLayout(
			m_pInputLayout.Get());
		UINT stride = iVertexSize;
		UINT offset = 0;
		pContext->IASetVertexBuffers(0, 1,
			m_pVertexBuffer.GetAddressOf(), &stride, &offset);
		pContext->IASetIndexBuffer(
			m_pIndexBuffer.Get(),
			DXGI_FORMAT_R32_UINT, 0);

		pContext->VSSetConstantBuffers(0,1, 
			m_pConstantBuffer.GetAddressOf());
		pContext->PSSetConstantBuffers(0, 1,
			m_pConstantBuffer.GetAddressOf());
		pContext->GSSetConstantBuffers(0, 1,
			m_pConstantBuffer.GetAddressOf());
		pContext->VSSetShader(
			m_pVertexShader.Get(), NULL, 0);
		pContext->PSSetShader(
			m_pPixelShader.Get(), NULL, 0);
		pContext->GSSetShader(
			m_pGeometryShader.Get(), NULL, 0);

		pContext->PSSetShaderResources(
			0,1,
			m_pTextureRV.GetAddressOf());
		return true;
	}
	bool  David_Obj_Compo_Func::Render(
		ID3D11DeviceContext* pContext,
		UINT iVertexSize, UINT iCount)
	{
		PreRender(pContext, iVertexSize);
		PostRender(pContext, iVertexSize, iCount);
		return true;
	}
	bool  David_Obj_Compo_Func::PostRender(
		ID3D11DeviceContext* pContext,
		UINT iVertexSize, UINT iCount)
	{
		if (m_pIndexBuffer != nullptr)
			pContext->DrawIndexed(iCount, 0, 0);
		else
			pContext->Draw(iCount,0);
		return true;
	}
	bool  David_Obj_Compo_Func::Release()
	{
		return true;
	}
	David_Obj_Compo_Func::David_Obj_Compo_Func()
	{		
	}


	David_Obj_Compo_Func::~David_Obj_Compo_Func()
	{
		Release();
	}

}