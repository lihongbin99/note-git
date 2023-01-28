#include "utils.h"

ULONGLONG TimeMilliSecond() {
    FILETIME file_time;
	GetSystemTimeAsFileTime(&file_time);
    uint64_t time = ((uint64_t)file_time.dwLowDateTime) + ((uint64_t)file_time.dwHighDateTime << 32);
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);
    return (uint64_t)((time - EPOCH) / 10000LL);
}

std::string Stamp2Time(ULONGLONG timestamp) {
	int ms = timestamp % 1000;//取毫秒
	time_t tick = (time_t)(timestamp/1000);//转换时间
	struct tm tm;
	char s[40];
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	std::string str(s);
	str = str+ "." + std::to_string(ms);
	return str;
}

int CharToWchar_t(const char* from, UINT characterSet, std::wstring& out) {
	int WideCharLen = MultiByteToWideChar(
		characterSet, 0,
		from, -1,
		NULL, 0);
	wchar_t* WideChar = new wchar_t[WideCharLen] { 0 };
	WideCharLen = MultiByteToWideChar(
		characterSet, 0,
		from, -1,
		WideChar, WideCharLen);

	out = WideChar;

	if (WideChar) delete[] WideChar;

	return WideCharLen;
}

int Wchar_tToChar(const wchar_t* from, UINT characterSet, std::string& out) {
	int MultiByteLen = WideCharToMultiByte(
		characterSet, 0,
		from, -1,
		NULL, NULL,
		NULL, NULL);
	char* MultiByte = new char[MultiByteLen] { 0 };
	MultiByteLen = WideCharToMultiByte(
		characterSet, 0,
		from, -1,
		MultiByte, MultiByteLen,
		NULL, NULL);

	out = MultiByte;

	if (MultiByte) delete[] MultiByte;

	return MultiByteLen;
}

int CharacterSetEncodeing(const char* from, UINT fromCharacterSet, UINT toCharacterSet, std::string& out) {
	int len;
	std::wstring w;
	len = CharToWchar_t(from, fromCharacterSet, w);
	if (len > 0) {
		len = Wchar_tToChar(w.c_str(), toCharacterSet, out);
	}
	return len;
}

int U2G(const char* from, std::string& out) {
	return CharacterSetEncodeing(from, CP_UTF8, CP_ACP, out);
}

int G2U(const char* from, std::string& out) {
	return CharacterSetEncodeing(from, CP_ACP, CP_UTF8, out);
}

void showError(DWORD error) {
    WCHAR *buf;
    DWORD len = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), (LPWSTR) &buf, 0, NULL);
    MessageBoxW(NULL, buf, L"错误", MB_OK);
    LocalFree(buf);
}
