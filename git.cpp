#include "git.h"

using namespace std;

int pullIntervalTime = 10/*m*/;
int pushIntervalTime = 30/*s*/;

ULONGLONG lastUpdateTime = 0;
ULONGLONG lastPullTime = 0;
ULONGLONG lastPushTime = 0;

string commitStr = "git commit -m \"commit: ";

void gitPull() {
    int code = system("git pull");
    if (code != 0) {// 有文件冲突
        string date = Stamp2Time(TimeMilliSecond());
        code = system("git add ./");
        code = system((commitStr + date + "\"").c_str());
    }
}

void gitPush(string date) {
    int code = system("git add ./");
    if (code != 0) {
        MessageBoxW(NULL, L"add error", L"错误", MB_OK);
        return;
    }

    code = system((commitStr + date + "\"").c_str());
    if (code != 0) {
        return;
    }

    code = system("git pull");
    if (code != 0) {// 有文件冲突
        code = system("git add ./");
        code = system((commitStr + date + "\"").c_str());
    }

    code = system("git push -u origin master");
    if (code != 0) {
        MessageBoxW(NULL, L"push error", L"错误", MB_OK);
        return;
    }
}

DWORD WINAPI gitMain(LPVOID lpParameter) {
    ULONGLONG currentTime;
    while(true) {
        currentTime = TimeMilliSecond();

        if (lastUpdateTime != lastPushTime) {
            if (currentTime - lastUpdateTime > pushIntervalTime * 1000) {
                // 更新
                string date = Stamp2Time(lastUpdateTime);
                gitPush(date);

                lastPushTime = lastUpdateTime;
                lastPullTime = currentTime;
            }
        } else if (currentTime - lastPullTime > pullIntervalTime * 60000) {
            gitPull();
            lastPullTime = currentTime;
        }

        Sleep(1000);
    }
    return 0L;
}

void gitInit(int pushIntervalTime, int pullIntervalTime) {
    ::pushIntervalTime = pushIntervalTime;
    ::pullIntervalTime = pullIntervalTime;
    lastPullTime = TimeMilliSecond() - (pullIntervalTime * 60000 - 30000);
    CreateThread(NULL, 0, gitMain, NULL, 0, NULL);
}

void fileUpdate() {
    lastUpdateTime = TimeMilliSecond();
}
