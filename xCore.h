#pragma once
#include "xWindow.h"
#include "xDxWrite.h"
#include "xInput.h"
#include "Canon_Camera.h"
#include "Particle_System.h"
#include "David_Timer.h"
#include <wmsdk.h>
#include "Select.h"


using namespace David_DX;

class xCore : public xWindow
{
public:
	//xTimer		 m_Timer;
	David_Timer  Da_Timer;
	xDxWrite	 m_Font;
	xInput		 m_Input;
	//

	Canon_Camera  DefaultCamera;
	Canon_Camera* MainCamera;
	//
	xDirLineShape   m_dirAxis;
	ID3D11DepthStencilView*  m_pDSV;

	//
	Da_Sky_Box_Shape SkyBox;

public:	
	virtual bool	GamePreInit();
	bool	GameInit();
	bool	GameRun();
	bool	GameRelease();
	bool	GameFrame();
	bool	GameRender();
	bool	GamePreRender();
	bool	GamePostRender();
	HRESULT CreateDSV();
	void    DeleteDeviceResources();
	HRESULT CreateDeviceResources(UINT width, UINT height);
	virtual void    DeleteResources();
	virtual HRESULT CreateResources(UINT width, UINT height);
public:
	virtual bool	Init() { return true; }
	virtual bool	Frame() { return true; }
	virtual bool	Render() { return true; }
	virtual bool	PreRender() { return true; }
	virtual bool	PostRender() { return true; }
	virtual bool	Release() { return true; }
public:
	xCore();
	virtual ~xCore();
};

