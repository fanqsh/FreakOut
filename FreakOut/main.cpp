#pragma once
#include "system.h"

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	SystemClass* System;
	bool result;

	//创建一个system对象
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	//初始化system对象
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = 0;
	return 0;
}
