#pragma once
#include "xStd.h"


class David_Timer
{
public:
	LARGE_INTEGER m_Frequency; // 초당 주파수TICK (시스템 고정값)
	LARGE_INTEGER m_Current; // 현재까지 TICK
	LARGE_INTEGER m_dwBeforeTick;  // 이젠 프레임의 시간 체크
	float         m_fSecPerFrame;    // 1프레임의 경과 시간seconds
	
public:

	float m_fTimer;
	TCHAR m_csBuffer[256];
	int m_dwFrameCount;
	int m_dwFPS;

public:

	LARGE_INTEGER m_Start;    //이벤트 타이머 시작 틱
	LARGE_INTEGER m_Elapse;   // 이벤트 타이머 시작후부터 쌓인 틱 양

	bool          m_bStarted; // 이벤트 타이머 작동 여부
	float         m_fEventTime;

public:
	
	//엔진코어에서 사용하는 지원함수
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	// 이벤트 타이머에 대한 지원 함수들
	void  Reset();
	void  Start();
	float Stop();
	bool  IsStarted() const;
	float GetElapsedTime();


	// FPS,SPF 직접 구하기.
	int  GetFPS();
	int  GetSPF();

public:
	David_Timer();
	virtual ~David_Timer();
};

