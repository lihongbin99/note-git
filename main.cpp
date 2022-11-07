#include <windows.h>
#include "utils.h"
#include "git.h"

#include <iostream>

void parseChange(WCHAR *data);
int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "chs");

    HANDLE noteDir = CreateFileW(L".\\note",
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL, 
        OPEN_EXISTING, 
        FILE_FLAG_BACKUP_SEMANTICS  | FILE_FLAG_OVERLAPPED,
        NULL);
    if (noteDir == INVALID_HANDLE_VALUE) {
        wprintf(L"监听目录失败...\n");
        showError(GetLastError());
        return EXIT_FAILURE;
    }
    wprintf(L"开始监听成功...\n");

    // 初始化
    if (argc >= 3) {
        gitInit(atoi(argv[1]), atoi(argv[2]));
    } if (argc >= 2) {
        gitInit(atoi(argv[1]), 10);
    } else {
        gitInit(30, 10);
    }

    WCHAR data[1024];
    memset(data, 0, sizeof(data));
    DWORD len = 0;

    while (true) {
        BOOL readResult = ReadDirectoryChangesW(noteDir,
            &data,
            sizeof(data) / 2,
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
            (LPDWORD)&len,
            NULL,
            NULL
        );

        if (!readResult) {
            wprintf(L"监听过程失败\n");
            showError(GetLastError());
            return EXIT_FAILURE;
        }
        if (len <= 6) {
            continue;
        }

        fileUpdate();

        // 解析
        // parseChange(data);

        // 重置
        memset(data, 0, len);
        len = 0;
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

    wprintf(L"%ls: %ls\n", actionType, data+6);

    if (nextEntryOffset != 0) {
        parseChange(data + nextEntryOffset);
    }
}


