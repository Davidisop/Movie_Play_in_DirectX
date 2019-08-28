#pragma once
#include "xStd.h"



class xDevice
{
public:
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*  m_pRenderTargetView;
	IDXGIFactory*			m_pDXGIFactory;
	DXGI_SWAP_CHAIN_DESC    m_sd;
public:
	HRESULT CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain();
	HRESULT SetRenderTargetView();
	void	SetViewPort();
	bool	ResizeDevice(UINT width, UINT height);
	virtual void    DeleteDeviceResources();
	virtual HRESULT CreateDeviceResources(UINT width, UINT height);
public:
	xDevice();
	virtual ~xDevice();
};

