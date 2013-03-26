#include "log.h"

void Logger::Log(const char* fileName, const char* func, const int line, string info)
{
	ofstream fout;
	char time[15];
	char date[15];
	
	fout.open("FreakOutLog.txt", ios_base::in | ios_base::app);
	
	if (fout.fail())
	{
		fout.open("FreakOutLog.txt", ios_base::in | ios_base::trunc);
	}

	TimerClass::GetSystemTime(time, date);

	fout << date << " " << time << " <Info>:" << fileName << "." << line << "." << func << "() " << info << endl;

	fout.close();
}