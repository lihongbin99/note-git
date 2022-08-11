#include <iostream>
#include <windows.h>
#include "git.h"
#include "utils.h"

using namespace std;

BOOL hasUpdate = FALSE;
ULONGLONG lastUpdateTime = 0;

string commitS = "git commit -m \"commit: ";
void pushGit(string date) {
    int code = system("git add ./");
    if (code != 0) {
        cout << "git add error: " << code << endl;
        MessageBoxW(NULL, L"add error", L"错误", MB_OK);
        return;
    }

    code = system((commitS + date + "\"").c_str());
    if (code != 0) {
        cout << "git commit error: " << code << endl;
        return;
    }

    code = system("git pull");
    if (code != 0) {
        cout << "git pull error: " << code << endl;
        return;
    }

    code = system((commitS + date + "\"").c_str());

    code = system("git push -u origin master");
    if (code != 0) {
        cout << "git push error: " << code << endl;
        MessageBoxW(NULL, L"push error", L"错误", MB_OK);
        return;
    }

    cout << "git push success: " << date << endl;
}

DWORD WINAPI gitMain(LPVOID lpParameter) {
    ULONGLONG tempTime = lastUpdateTime;
    while(true) {
        if (tempTime == lastUpdateTime) {
            continue;
        }

        if (TimeMilliSecond() - lastUpdateTime > 30000) {
            tempTime = lastUpdateTime;

            // 更新
            string date = Stamp2Time(tempTime);
            cout << "update: " << date << endl;
            pushGit(date);
        }

        Sleep(1000);
    }
    return 0L;
}

void gitInit() {
    CreateThread(NULL, 0, gitMain, NULL, 0, NULL);
}

void fileUpdate() {
    hasUpdate = TRUE;
    lastUpdateTime = TimeMilliSecond();
}
