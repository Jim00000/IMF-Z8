#include "pch.h"
#include "detours_wrapper.h"


namespace {
    DWORD (WINAPI * pGetTickCount)() = nullptr;
    ULONGLONG(WINAPI* pGetTickCount64)() = nullptr;
    BOOL(WINAPI* pQueryPerformanceCounter)(LARGE_INTEGER*) = nullptr;
    LARGE_INTEGER QPC_Base = LARGE_INTEGER();

    DWORD WINAPI __GetTickCount_Hook__() { return 0; }

    ULONGLONG WINAPI __GetTickCount64_Hook__() { return 0; }

    BOOL WINAPI __QueryPerformanceCounter_Hook__(LARGE_INTEGER* lpPerformanceCount) {
        LARGE_INTEGER qpc;
        pQueryPerformanceCounter(&qpc);
        lpPerformanceCount->QuadPart = QPC_Base.QuadPart + (qpc.QuadPart - QPC_Base.QuadPart) * 8.0;
        return TRUE;
    }
}

BOOL WINAPI attach()
{
    HMODULE hKernel32Module = GetModuleHandle(L"kernel32.dll");

    if (hKernel32Module == NULL) {
        return FALSE;
    }

    pQueryPerformanceCounter = (decltype(pQueryPerformanceCounter))GetProcAddress(hKernel32Module, "QueryPerformanceCounter");
    if (pQueryPerformanceCounter == nullptr)
        return FALSE;

    pQueryPerformanceCounter(&QPC_Base);

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)pQueryPerformanceCounter, __QueryPerformanceCounter_Hook__);
    DetourTransactionCommit();

    return TRUE;
}

BOOL WINAPI detach()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)pQueryPerformanceCounter, __QueryPerformanceCounter_Hook__);
    DetourTransactionCommit();
    return TRUE;
}

BOOL WINAPI freeThisLibrary()
{
    detach();
    HMODULE hMod = GetModuleHandle(L"SpeedHack.dll");
    FreeLibraryAndExitThread(hMod, 0);
    return TRUE;
}