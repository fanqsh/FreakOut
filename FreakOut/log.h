#pragma once
#ifndef _LOG_H_   
#define _LOG_H_   
#include <fstream>
#include <string>
#include "TimerClass.h"

using namespace std;

#ifdef _DEBUG
	#define LOG(info) Logger::Log(strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__, __FUNCTION__, __LINE__,info)
	//#define LOG(info)
#else
	#define LOG(info)
#endif

class Logger 
{
public:
	Logger();
	static void Log(const char* fileName, const char* func, const int line, string info);
	
private:
	bool isOpen;
};

#endif
