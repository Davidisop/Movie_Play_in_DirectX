#include "David_Timer.h"


	float David_SecPerFrame = 0.0f;
	float David_GameTimer = 0.0f;

	//엔진코어에서 사용하는 지원함수

	bool David_Timer::Init() 
	{
		QueryPerformanceCounter(&m_dwBeforeTick); //Init() 호출될 때까지의 Tick양.
 		Start();
		return true;
	}


	bool David_Timer::Frame() 
	{
		// 1프레임의 시간 체크 
		// 현재 시간을 찾는다. 
		QueryPerformanceCounter(&m_Current); //현재까지 쌓인 Tick양.
		m_fSecPerFrame = static_cast<float> (m_Current.QuadPart - m_dwBeforeTick.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
		// 1프레임 사이의 Tick양을 주파수로 나눴으니 SPF가 나온다.
		David_SecPerFrame = m_fSecPerFrame;
		David_GameTimer += m_fSecPerFrame; //매 Frame 마다 second가 쌓인다.

		m_fTimer += m_fSecPerFrame;

		if (m_fTimer >= 1.0f) //1초가 쌓이면, 즉 1초마다 업데이트 됨.
		{
			m_dwFPS = m_dwFrameCount;
			m_dwFrameCount = 0; 
			m_fTimer= m_fTimer-1.0f;

			_stprintf_s(m_csBuffer, L" FPS:%d,  Time : %10.4f, SPF : %10.4f", m_dwFPS, David_GameTimer, m_fSecPerFrame);
		}
		m_dwFrameCount++;
		m_dwBeforeTick = m_Current;

		return true;
	}


	bool David_Timer::Render() { return true; }
	bool David_Timer::Release() { return true; }


	// FPS,SPF 직접 구하기.
	int  David_Timer::GetFPS() { return m_dwFPS; }
	int  David_Timer::GetSPF() { return m_fSecPerFrame; }


	David_Timer::David_Timer()
	{
		m_fSecPerFrame = 0.0f;

		m_bStarted = false;
		m_fEventTime = 0.0f;
		m_fTimer = 0.0f;

		m_dwFrameCount = 0;
		m_dwFPS = 0;
		memset(m_csBuffer, 0, sizeof(m_csBuffer));

		// 시스템의 주파수 변동폭을 얻어 온다. // 초당 주파수TICK (시스템 고정값)
		QueryPerformanceFrequency((LARGE_INTEGER *)&m_Frequency);
	}



	David_Timer::~David_Timer()
	{
	}





	// 이벤트 타이머에 대한 지원 함수들

	void  David_Timer::Start()
	{
		m_fEventTime = 0.0f;
		m_bStarted = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_Start); // 이벤트 타이머 시작 쌓인 Tick.
	}


	void  David_Timer::Reset() 
	{
		m_bStarted = false;
		m_fEventTime = 0.0f;
		memset(&m_Start, 0, sizeof(LARGE_INTEGER));
		memset(&m_Elapse, 0, sizeof(LARGE_INTEGER));
	}

	float  David_Timer::Stop() 
	{
		float result = GetElapsedTime();
		m_bStarted = false;
		return result;
	}

	bool  David_Timer::IsStarted() const { return m_bStarted; }

	float David_Timer::GetElapsedTime() 
	{
		if (m_bStarted == true)
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&m_Elapse);
			m_fEventTime = static_cast<float>(m_Elapse.QuadPart - m_Start.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
			//타이머 INIT()되고 나서 지금까지 흐른 시간.
		}
		return m_fEventTime;
	}



