#pragma once

#include <string>
#include <time.h>
#include <windows.h>

// 获取时间
ULONGLONG TimeMilliSecond();
std::string Stamp2Time(ULONGLONG timestamp);

int CharToWchar_t(const char* from, UINT characterSet, std::wstring& out);

int Wchar_tToChar(const wchar_t* from, UINT characterSet, std::string& out);

int CharacterSetEncodeing(const char* from, UINT fromCharacterSet, UINT toCharacterSet, std::string& out);

int U2G(const char* from, std::string& out);

int G2U(const char* from, std::string& out);

void showError(DWORD error);
