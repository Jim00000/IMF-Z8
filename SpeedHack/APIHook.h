#pragma once

#include <windows.h>

BOOL WINAPI attach();

BOOL WINAPI detach();

extern "C" {
    __declspec(dllexport) BOOL WINAPI freeThisLibrary();
}