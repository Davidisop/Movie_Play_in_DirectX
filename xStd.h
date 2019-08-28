#pragma once
#define DIRECTINPUT_VERSION 0x800
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdlib.h>
#include <assert.h>
#include <tchar.h>
#include <iostream>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <tchar.h>
#include "d3dx11.h"
#include "dxgi.h" 
#include "d3dcompiler.h"
#include <d3dx10math.h>
#include <wrl.h>
#pragma comment(lib,"d3dx10d.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"TCore3D.lib")

using namespace std;
using namespace Microsoft::WRL;


typedef std::basic_string<char> C_STR;
typedef std::basic_string<wchar_t> W_STR;
typedef std::basic_string<TCHAR> T_STR;
typedef std::basic_string<char>::iterator C_ITOR;
typedef std::basic_string<wchar_t>::iterator W_ITOR;
typedef std::basic_string<TCHAR>::iterator T_ITOR;

#define GAME_START int WINAPI WinMain(HINSTANCE hInst,	HINSTANCE hPrevInst,LPSTR   lpCmdLine,int   nCmdShow){
#define GAME_END }
#define GAME_RUN(s,w,h) GAME_START{ Sample win;win.SetWindow(hInst, L#s,w,h);win.Run();}GAME_END

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern RECT  g_rtClient;
extern bool g_bActiveApp;
extern ID3D11Device* g_pd3dDevice;

extern float David_SecPerFrame;
extern float David_GameTimer;


struct TGameInput
{
	BOOL bFront;
	BOOL bBack;
	BOOL bLeft;
	BOOL bRight;
	BOOL bAttack;
	BOOL bJump;
	//
	BOOL Yaw_clock;
	BOOL Yaw_a_clock;
	BOOL Pitch_clock;
	BOOL Pitch_a_clock;
	BOOL Roll_clock;
	BOOL Roll_a_clock;
	//
	BOOL Left_Click;
};
extern TGameInput g_Input;
template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};

#ifndef V
#define V(x) { hr = (x);}
#endif

#ifndef V_RETURN
#define V_RETURN(x) { hr = (x); if( FAILED(hr)){return hr;}}
#endif
//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif