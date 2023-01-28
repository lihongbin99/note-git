#include <iostream>

#include <windows.h>

#include "git.h"

using namespace std;

void parseChange(WCHAR *data);

int main(int argc, char** argv) {
	system("chdir");
	HANDLE noteDir = CreateFileW(L".\\note",
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL, 
        OPEN_EXISTING, 
        FILE_FLAG_BACKUP_SEMANTICS  | FILE_FLAG_OVERLAPPED,
        NULL);
    if (noteDir == INVALID_HANDLE_VALUE) {
		printf("监听目录失败...");
        showError(GetLastError());
        return EXIT_FAILURE;
    }
    printf("开始监听成功...\n");

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
            printf("监听过程失败\n");
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

    char* actionType;
    if (action == FILE_ACTION_ADDED) {
        actionType = "添加文件";
    } else if (action == FILE_ACTION_REMOVED) {
        actionType = "删除文件";
    } else if (action == FILE_ACTION_MODIFIED) {
        actionType = "修改文件";
    } else if (action == FILE_ACTION_RENAMED_OLD_NAME) {
        actionType = "旧文件名";
    } else if (action == FILE_ACTION_RENAMED_NEW_NAME) {
        actionType = "新文件名";
    }

	string fileNameStr;
	Wchar_tToChar(data+6, CP_ACP, fileNameStr);
    printf("%s: %s\n", actionType, fileNameStr.c_str());

    if (nextEntryOffset != 0) {
        parseChange(data + nextEntryOffset);
    }
}
