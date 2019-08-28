#include "xDxWrite.h"
bool	xDxWrite::Set( HWND hWnd, 
					int iWidth, 
					int iHeight,
					IDXGISurface* pSurface)
{
	CreateDeviceIndependentResources();
	CreateDeviceResources(pSurface);
	return true;
}
HRESULT xDxWrite::CreateDeviceIndependentResources()
{
	HRESULT hr;
	if (FAILED(hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pd2dFactory)))
	{
		return hr;
	}
	if (FAILED(hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown **)&m_pWriteFactory)))
	{
		return hr;
	}

	if (FAILED(hr = m_pWriteFactory->CreateTextFormat(
		L"¸íÁ¶", NULL, 
		DWRITE_FONT_WEIGHT_BOLD, // WEIGHT
		DWRITE_FONT_STYLE_OBLIQUE, // STYLE
		DWRITE_FONT_STRETCH_EXTRA_EXPANDED, //STRETCH
		20,
		L"ko-kr",//L"un-us",//L"ko-kr",
		&m_pTextFormat)))
	{
		return hr;
	}
	return hr;
}
HRESULT xDxWrite::CreateDeviceResources(IDXGISurface* pSurface)
{
	HRESULT hr;	
	D2D1_PIXEL_FORMAT pf;
	pf.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	pf.format = DXGI_FORMAT_UNKNOWN;

	m_pd2dFactory->GetDesktopDpi(&m_fdpiX, &m_fdpiY);
	// 800 *  ppi * 96.0f;
	// 600 *  ppi * 96.0f;

	D2D1_RENDER_TARGET_PROPERTIES rtp;
	rtp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;	
	rtp.pixelFormat = pf;
	rtp.dpiX = m_fdpiX;
	rtp.dpiY = m_fdpiY;
	rtp.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	rtp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	if (FAILED(hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(pSurface,
		&rtp,
		&m_pRT)))
	{
		return hr;
	}
	D2D1::ColorF color(D2D1::ColorF::Yellow);
	if (FAILED(hr = m_pRT->CreateSolidColorBrush(
		color,&m_pColorBrush)))
	{
		return hr;
	}
	return hr;
}
HRESULT xDxWrite::David_DrawText(RECT rt, const TCHAR* pText, D2D1::ColorF color)
{
	m_pRT->BeginDraw();
	//D2D1::Matrix3x2F matWorld;
	//D2D1_POINT_2F center;
	//center.x = 400;
	//center.y = 300;
	//float fScale = (cosf(g_fGameTimer)*0.5f + 0.5f) * 10;
	//D2D1::Matrix3x2F scale = matWorld.Scale(fScale, fScale, center);
	//D2D1::Matrix3x2F rot = matWorld.Rotation(g_fGameTimer*100,center);
	//rot = scale * rot;
	m_pRT->SetTransform(m_matWorld);// D2D1::IdentityMatrix() );
		D2D1_RECT_F rf = D2D1::RectF(rt.left, rt.top, 
			rt.right, rt.bottom);
		m_pColorBrush->SetColor(color);
		m_pRT->DrawText(pText, wcslen(pText),
						m_pTextFormat, rf, m_pColorBrush);
	m_pRT->EndDraw();
	return S_OK;
}
bool xDxWrite::Render()
{
	m_pRT->BeginDraw();	
	for (int iText = 0; iText < m_TextList.size(); iText++)
	{
		m_pRT->SetTransform(m_TextList[iText].matWorld);// D2D1::IdentityMatrix());
		D2D1_RECT_F rf = 
			m_TextList[iText].rf;
		rf.top += iText * 30.0f;
		m_pColorBrush->SetColor(m_TextList[iText].color);

		m_pRT->DrawText(m_TextList[iText].pText.c_str(),
			m_TextList[iText].pText.size(),
			m_pTextFormat, rf, m_pColorBrush);
	}
	m_pRT->EndDraw();
	return true;
}
void    xDxWrite::OnResize(int iWidth, int iHeight, IDXGISurface* pSurface)
{
	DiscardDeviceResources();
	CreateDeviceResources(pSurface);
}
void xDxWrite::DiscardDeviceResources()
{
	if (m_pColorBrush)m_pColorBrush->Release();
	if (m_pRT)m_pRT->Release();
	m_pRT = nullptr;	
}
bool	xDxWrite::Init()
{
	return true;
}
bool	xDxWrite::Frame() {
	return true;
}

bool	xDxWrite::Release() 
{
	DiscardDeviceResources();	
	if(m_pTextFormat)m_pTextFormat->Release();
	if(m_pWriteFactory)m_pWriteFactory->Release();
	if(m_pd2dFactory)m_pd2dFactory->Release();
	m_pWriteFactory=nullptr;
	m_pd2dFactory = nullptr;
	return true;
}


xDxWrite::xDxWrite()
{
	m_matWorld = m_matWorld.Identity();
}


xDxWrite::~xDxWrite()
{
}
