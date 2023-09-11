//
// Created by david on 2023-09-10.
//

#include "rs2client.h"
#include <Windows.h>
#include "../ws/config_ws.h"

void createProcess(RuneScapeInfo &rsInfo)
{
    CHAR RequestArgs[4096] = {0};
    CHAR Dir[MAX_PATH] = {0};

    strcat(RequestArgs, "\"");
    strcat(RequestArgs, rsInfo.exePath);
    strcat(RequestArgs, "\" ");

    STARTUPINFO info = {sizeof(info)};
    PROCESS_INFORMATION processInfo;

    int SessionResp = requestSession(RequestArgs);
    if (SessionResp == 0) {
        return;
    }

    size_t Len = strlen(rsInfo.exePath);
    int LastIndex = 0;
    for (int i = 0; i < Len; i++) {
        if (rsInfo.exePath[i] == '\\' || rsInfo.exePath[i] == '/') {
            LastIndex = i;
        }
    }

    memcpy(Dir, rsInfo.exePath, LastIndex + 1);
    Dir[LastIndex + 1] = 0;

    FILE *file;
    // Open the file in write mode
    file = fopen("output.txt", "w");

    // Check if the file was opened successfully
    if (file == nullptr) {
        printf("Unable to open the file.\n");
    }

    // Write data to the file
    fprintf(file, "%s", RequestArgs);

    // Close the file
    fclose(file);

    if (!CreateProcessA(rsInfo.exePath, RequestArgs, nullptr, nullptr, FALSE, 0x8000020, nullptr, Dir, &info, &processInfo)) {
        WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
        return;
    }
}
