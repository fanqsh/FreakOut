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
	//是否退出
	bool bexit;
	float m_spacingTime;
	
};
//定义静态的回调函数以及应用程序句柄
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;


#endif