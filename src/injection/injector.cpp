//
// Created by david on 2023-09-15.
//

#include "injector.h"
#include <iostream>

bool inject(const char *dll_path, const PROCESS_INFORMATION& Info)
{
    // Open the target process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Info.dwProcessId);
    if (hProcess == NULL) {
        printf("Failed to open the target process.\n");
        return FALSE;
    }

    // Allocate memory in the target process for the DLL path
    LPVOID remotePath = VirtualAllocEx(hProcess, NULL, strlen(dll_path) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (remotePath == NULL) {
        printf("Failed to allocate memory in the target process.\n");
        CloseHandle(hProcess);
        return FALSE;
    }

    // Write the DLL path to the allocated memory
    if (!WriteProcessMemory(hProcess, remotePath, dll_path, strlen(dll_path) + 1, NULL)) {
        printf("Failed to write DLL path to the target process.\n");
        VirtualFreeEx(hProcess, remotePath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    // Get the address of the LoadLibraryA function
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    if (kernel32 == NULL) {
        printf("Failed to get the address of kernel32.dll.\n");
        VirtualFreeEx(hProcess, remotePath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }
    LPVOID loadLibraryAddr = (LPVOID) GetProcAddress(kernel32, "LoadLibraryA");
    if (loadLibraryAddr == NULL) {
        printf("Failed to get the address of LoadLibraryA.\n");
        VirtualFreeEx(hProcess, remotePath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    // Create a remote thread in the target process to execute LoadLibraryA
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE) loadLibraryAddr, remotePath, 0,
                                        NULL);
    if (hThread == NULL) {
        printf("Failed to create a remote thread in the target process.\n");
        VirtualFreeEx(hProcess, remotePath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    // Wait for the remote thread to finish
    WaitForSingleObject(hThread, INFINITE);

    // Clean up resources
    VirtualFreeEx(hProcess, remotePath, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return TRUE;
}
