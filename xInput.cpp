#include "xInput.h"
TGameInput g_Input;
bool xInput::Frame()
{
	HRESULT hr;
	if (FAILED(hr = m_pKey->GetDeviceState(256, &m_KeyState)))
	{
		while (m_pKey->Acquire() == DIERR_INPUTLOST);
	}
	if (FAILED(hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
	{
		while (m_pMouse->Acquire() == DIERR_INPUTLOST);
	}


	g_Input.bFront = m_KeyState[DIK_W];
	g_Input.bBack = m_KeyState[DIK_S];
	g_Input.bLeft = m_KeyState[DIK_A];
	g_Input.bRight = m_KeyState[DIK_D];
	g_Input.bAttack = m_KeyState[DIK_RETURN];
	g_Input.bJump = m_KeyState[DIK_SPACE];


	g_Input.Yaw_clock = m_KeyState[DIK_Y];
	g_Input.Yaw_a_clock = m_KeyState[DIK_U];
	g_Input.Pitch_clock = m_KeyState[DIK_H];
	g_Input.Pitch_a_clock = m_KeyState[DIK_J];
	g_Input.Roll_clock = m_KeyState[DIK_N];
	g_Input.Roll_a_clock = m_KeyState[DIK_M];
	//
	g_Input.Left_Click = m_MouseState.rgbButtons[0];


	return true;
}
bool xInput::Release()
{
	m_pKey->Unacquire();
	m_pMouse->Unacquire();

	m_pKey->Release();
	m_pMouse->Release();
	m_pDI->Release();
	return true;
}
bool xInput::Init()
{
	memset(&m_KeyState, 0, sizeof(BYTE) * 256);
	memset(&m_MouseState, 0, sizeof(DIMOUSESTATE));

	HRESULT hr;
	if (FAILED(hr = DirectInput8Create(g_hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pDI, NULL)))
	{
		return false;
	}
	// keyboard
	if (FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKey, NULL)))
	{
		return false;
	}
	if (FAILED(hr = m_pKey->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}
	if (FAILED(hr = m_pKey->SetCooperativeLevel(
		g_hWnd, DISCL_NONEXCLUSIVE |DISCL_FOREGROUND | DISCL_NOWINKEY)))
	{
		return false;
	}
	while (m_pKey->Acquire() == DIERR_INPUTLOST);
	// mouse
	if (FAILED(hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
	{
		return false;
	}
	if (FAILED(hr = m_pMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}
	if (FAILED(hr = m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)))
	{
		return false;
	}
	while (m_pMouse->Acquire() == DIERR_INPUTLOST);
	return true;
}

xInput::xInput()
{
}


xInput::~xInput()
{
}
