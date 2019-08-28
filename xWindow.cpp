#include "xWindow.h"
xWindow*	g_pWindow = 0;
HWND		g_hWnd = NULL;
HINSTANCE	g_hInstance = NULL;
RECT		g_rtClient;
bool		g_bActiveApp = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	assert(g_pWindow != NULL);
	LRESULT ret;
	if (ret = g_pWindow->MsgProc(hWnd, msg, wParam, lParam))
	{
		return ret;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT xWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATEAPP:
			g_bActiveApp = (BOOL)wParam; //wParam에 TRUE가 들어오면 활성화.
			break;

	case WM_CREATE:
		{
			//MessageBox(NULL, L"윈도우가 떴다.", L"윈도우생성", MB_OK);
		}break;

	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		ResizeDevice(width, height);		
	}break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			case '0':
			{
				BOOL isFullMode = FALSE;
				m_pSwapChain->GetFullscreenState(&isFullMode, NULL);
				m_pSwapChain->SetFullscreenState(!isFullMode, NULL);
				if (isFullMode)
				{
					ShowWindow(m_hWnd, SW_SHOW);
				}
			}break;
			case '1':
			{
				if (m_pSwapChain)
				{
					DXGI_MODE_DESC desc;
					ZeroMemory(&desc, sizeof(desc));
					desc.Width = 1024;
					desc.Height = 768;
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.RefreshRate.Denominator = 0;
					desc.RefreshRate.Numerator = 0;
					desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
					desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
					m_pSwapChain->ResizeTarget(&desc);
				}
			}break;
			case '2':
			{
				if (m_pSwapChain)
				{
					DXGI_MODE_DESC desc;
					ZeroMemory(&desc, sizeof(desc));
					desc.Width = 800;
					desc.Height = 600;
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.RefreshRate.Denominator = 0;
					desc.RefreshRate.Numerator = 0;
					desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
					desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
					m_pSwapChain->ResizeTarget(&desc);
				}
			}break;
		}
	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);// WM_QUIT--> PUSH			
	}return 0;
	}
	return 0;
}

void xWindow::MsgEvent(MSG msg)
{
}
bool xWindow::SetWindow(HINSTANCE	hInst,
	const TCHAR* pTitleName,
	UINT iWidth, UINT iHeight)
{
	m_hInstance = hInst;
	g_hInstance = hInst;
	// 윈도우 클래스 등록
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = L"MyWindow";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	if (RegisterClassEx(&wc) == FALSE)
	{
		return false;
	}
	// 클라이언트 영역 수정
	RECT rt = { 0,0,iWidth, iHeight };
	AdjustWindowRect(&rt, m_dwStyle, FALSE);
	//// 윈도우 생성( 등록된 클래스를 사용해서 )
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		L"MyWindow", pTitleName,
		m_dwStyle,
		0, 0,
		rt.right - rt.left,
		rt.bottom - rt.top,
		NULL, NULL, m_hInstance, NULL);
	if (m_hWnd == NULL)
	{
		return false;
	}
	g_hWnd = m_hWnd;

	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;

	CenterWindow();

	ShowWindow(m_hWnd, SW_SHOW);
	return true;
}
void xWindow::Set(DWORD dwStyle)
{
	m_dwStyle = dwStyle;
}
void xWindow::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHegiht = GetSystemMetrics(SM_CYFULLSCREEN);
	int x = (iScreenWidth - (m_rtWindow.right - m_rtWindow.left)) / 2;
	int y = (iScreenHegiht - (m_rtWindow.bottom - m_rtWindow.top)) / 2;
	MoveWindow(m_hWnd, x, y, m_rtWindow.right, m_rtWindow.bottom, true);
}
bool xWindow::Run()
{
	if (!GameInit()) return false;
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
			MsgEvent(msg);
		}
		else
		{
			GameRun();
		}
	}
	return GameRelease();
}
bool xWindow::GameInit()
{
	return true;
}
bool xWindow::GameRun() {
	return true;
}
bool xWindow::GameRelease() {
	return true;
}
xWindow::xWindow()
{
	m_dwStyle = WS_OVERLAPPEDWINDOW;
	g_pWindow = this;
	m_hWnd = nullptr;
	m_hInstance = nullptr;
}


xWindow::~xWindow()
{
}
