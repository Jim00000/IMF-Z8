// dllmain.cpp : 定義 DLL 應用程式的進入點。
#include "pch.h"

#include <stdio.h>
#include <Psapi.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    CHAR buffer[BUFSIZ];
    GetModuleBaseNameA(GetCurrentProcess(), NULL, buffer, BUFSIZ);
    if (strcmp(buffer, "YuGiOh.exe") == 0) {
        switch (ul_reason_for_call)
        {
        case DLL_PROCESS_ATTACH:
            attach();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        default:
            break;
        }
    }
    return TRUE;
}

