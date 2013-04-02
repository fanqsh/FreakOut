#include "TimerClass.h"

TimerClass::TimerClass()
{
}

TimerClass::TimerClass(const TimerClass&)
{
}

TimerClass::~TimerClass(void)
{
}


bool TimerClass::Initialize()
{
	//���ϵͳ�Ƿ�֧�ָ߾��ȼ�ʱ��
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (0 == m_frequency)
		return false;
	
	//�õ�ÿ������ٸ�������
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
	return true;
}

//ÿ����Ⱦ֡�����ã��Ӷ������ÿ֮֡���ʱ��
void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	timeDifference = (float)(currentTime - m_startTime);
	m_frameTime = timeDifference / m_ticksPerMs;
	m_startTime = currentTime;

	return;
}

float TimerClass::GetTime()
{
	return m_frameTime;
}

void TimerClass::GetSystemTime(char* time, char* date /* = nullptr*/)
{
	SYSTEMTIME sys;

	GetLocalTime(&sys);

	if (time != nullptr)
		sprintf(time, "%d:%d:%d", sys.wHour, sys.wMinute, sys.wSecond);
	if (date != nullptr)
		sprintf(date, "%d-%d-%d", sys.wYear, sys.wMonth, sys.wDay);

	return;
}