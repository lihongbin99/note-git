#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "utils.h"
#include "git.h"

using namespace std;

void parseChange(WCHAR *data);

int main() {
    ios::sync_with_stdio(false);
    setlocale(LC_CTYPE, "");

    // 初始化
    gitInit();

    // ifstream ifs;
    // ifs.open("config", ios::in);
    // if (!ifs.is_open()) {
    //     cout << "获取配置文件失败" << endl;
    //     showError(GetLastError());
    //     return EXIT_FAILURE;
    // }
    // PSTR config = new CHAR[100];
    // memset(config, 0, 100);
    // while (ifs >> config) { }
    // PCWSTR baseDir = (PCWSTR) config + 1;
    // wcout << L"监听目录: " << baseDir << endl;

    HANDLE noteDir = CreateFileW(L".\\note",
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL, 
        OPEN_EXISTING, 
        FILE_FLAG_BACKUP_SEMANTICS  | FILE_FLAG_OVERLAPPED,
        NULL);
    if (noteDir == INVALID_HANDLE_VALUE) {
        cout << "监听目录失败" << endl;
        showError(GetLastError());
        return EXIT_FAILURE;
    }

    WCHAR data[1024];
    memset(data, 0, 1024);
    DWORD len = 0;

    cout << "开始监听成功..." << endl;
    BOOL readResult;
    while (true) {
        readResult = ReadDirectoryChangesW(noteDir,
            &data,
            sizeof(data),
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
            (LPDWORD)&len,
            NULL,
            NULL
        );

        if (!readResult) {
            cout << "监听过程失败" << endl;
            showError(GetLastError());
            return EXIT_FAILURE;
        }

        len /= 2;
        if (len < 6) {
            cout << "ReadDirectoryChangesW len error: " << len << endl;
            printArr(data, len);
            MessageBoxW(NULL, L"监听返回长度错误", L"错误", MB_OK);
            return EXIT_FAILURE;
        }

        fileUpdate();
        continue;

        // 解析
        parseChange(data);

        // 重置
        // memset(data, 0, len);
        // len = 0;
    }

    return EXIT_SUCCESS;
}

void parseChange(WCHAR *data) {
    DWORD nextEntryOffset = data[0] | data[1] << 16;
    nextEntryOffset /= 2;

    DWORD action = data[2] | data[3] << 16;

    DWORD fileNameLength = data[4] | data[5] << 16;
    fileNameLength /= 2;

    PCWSTR actionType;
    if (action == FILE_ACTION_ADDED) {
        actionType = L"添加文件";
    } else if (action == FILE_ACTION_REMOVED) {
        actionType = L"删除文件";
    } else if (action == FILE_ACTION_MODIFIED) {
        actionType = L"修改文件";
    } else if (action == FILE_ACTION_RENAMED_OLD_NAME) {
        actionType = L"旧文件名";
    } else if (action == FILE_ACTION_RENAMED_NEW_NAME) {
        actionType = L"新文件名";
    }

    wstring name = wstring(data + 6, data + 6 + fileNameLength);
    wcout << actionType << L": " << name << endl;

    if (nextEntryOffset != 0) {
        parseChange(data + nextEntryOffset);
    }
}


