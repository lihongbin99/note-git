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
		printf("����Ŀ¼ʧ��...");
        showError(GetLastError());
        return EXIT_FAILURE;
    }
    printf("��ʼ�����ɹ�...\n");

    // ��ʼ��
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
            printf("��������ʧ��\n");
            showError(GetLastError());
            return EXIT_FAILURE;
        }
        if (len <= 6) {
            continue;
        }

        fileUpdate();

        // ����
        // parseChange(data);

        // ����
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
        actionType = "����ļ�";
    } else if (action == FILE_ACTION_REMOVED) {
        actionType = "ɾ���ļ�";
    } else if (action == FILE_ACTION_MODIFIED) {
        actionType = "�޸��ļ�";
    } else if (action == FILE_ACTION_RENAMED_OLD_NAME) {
        actionType = "���ļ���";
    } else if (action == FILE_ACTION_RENAMED_NEW_NAME) {
        actionType = "���ļ���";
    }

	string fileNameStr;
	Wchar_tToChar(data+6, CP_ACP, fileNameStr);
    printf("%s: %s\n", actionType, fileNameStr.c_str());

    if (nextEntryOffset != 0) {
        parseChange(data + nextEntryOffset);
    }
}
