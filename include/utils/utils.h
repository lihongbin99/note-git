#pragma once

#include <string>
#include <windows.h>

// 获取时间
ULONGLONG TimeMilliSecond();
std::string Stamp2Time(ULONGLONG timestamp);

// 打印错误信息
void showError(DWORD error);

// 打印数组
void printArr(WCHAR *arr, int len);

// 窄字符转宽字符
PCWSTR PCSTR2PCWSTR(PCSTR c);