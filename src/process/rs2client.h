//
// Created by david on 2023-09-10.
//
#pragma once

typedef struct
{
    char* exePath;
} RuneScapeInfo;

void createProcess(RuneScapeInfo& info);
