//
// Created by david on 2023-09-10.
//
#pragma once

#include <Windows.h>

typedef struct
{
    char* exePath;
} RuneScapeInfo;

typedef struct {
    int current_pid;
} ProcessInfo;

PROCESS_INFORMATION createProcess(RuneScapeInfo& info);
