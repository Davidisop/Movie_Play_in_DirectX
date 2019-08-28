#pragma once
#include "xStd.h"
#include "dinput.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
class xInput
{
	LPDIRECTINPUT8  m_pDI;
	LPDIRECTINPUTDEVICE8  m_pKey;
	LPDIRECTINPUTDEVICE8  m_pMouse;

	BYTE			m_KeyState[256];
	DIMOUSESTATE	m_MouseState;
public:
	bool	Init();
	bool	Frame();
	bool	Release();
public:
	xInput();
	virtual ~xInput();
};

