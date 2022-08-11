#include <iostream>
#include "utils.h"

using namespace std;

ULONGLONG TimeMilliSecond() {
    FILETIME file_time;
	GetSystemTimeAsFileTime(&file_time);
    uint64_t time = ((uint64_t)file_time.dwLowDateTime) + ((uint64_t)file_time.dwHighDateTime << 32);
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);
    return (uint64_t)((time - EPOCH) / 10000LL);
}

string Stamp2Time(ULONGLONG timestamp) {
	int ms = timestamp % 1000;//取毫秒
	time_t tick = (time_t)(timestamp/1000);//转换时间
	struct tm tm;
	char s[40];
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	string str(s);
	str = str+ "." + to_string(ms);
	return str;
}

void showError(DWORD error) {
    LPWSTR buf;
    DWORD len = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), (LPWSTR)&buf, 0, NULL);
    MessageBoxW(NULL, buf, L"错误", MB_OK);
    LocalFree(buf);
}

void printArr(WCHAR *arr, int len) {
    for (int i = 0; i < len; i++) {
        cout << (int) arr[i] << " ";
    }
    cout << endl;
}

PCWSTR PCSTR2PCWSTR(PCSTR c) {
    const size_t len = strlen(c) + 1;
    PWSTR wc = new wchar_t[len];
    mbstowcs(wc, c, len);
    return wc;
}