#pragma once
#include <iostream>
#include <fstream>

static void QuickLog(const char* log)
{
	std::fstream logStream;
	logStream.open("SSA_Log.txt",std::ios::in | std::ios::out | std::ios::app);
	if (!logStream.is_open()) {
		return;
	}
	logStream << log;
	logStream << '\n';
	logStream.close();
}

static void QuickLogW(const wchar_t* log)
{
	std::wfstream logStream;
	logStream.open("SSA_Log.txt", std::ios::in | std::ios::out | std::ios::app);
	if (!logStream.is_open()) {
		return;
	}
	logStream << log;
	logStream << '\n';
	logStream.close();
}