#pragma once
#ifndef _SYSTEM_H_ 
#define _SYSTEM_H_  
#include <windows.h>
#include "TimerClass.h"
#include "GameClass.h"

class SystemClass
{
public:
	HDC m_hdc;
	GameClass* m_game;

public:
	SystemClass(void);
	SystemClass(const SystemClass&);
	~SystemClass(void);

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	TimerClass* m_timer;
	//�Ƿ��˳�
	bool bexit;
	float m_spacingTime;
	
};
//���徲̬�Ļص������Լ�Ӧ�ó�����
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;


#endif