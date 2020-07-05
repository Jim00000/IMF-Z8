#include "Helper.h"

#include <strsafe.h>

VOID ErrorHandler(LPCSTR lpszFunction)
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message.
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (strlen((LPCSTR)lpMsgBuf) + strlen((LPCSTR)lpszFunction) + 40) * sizeof(CHAR));
    StringCchPrintfA((LPSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(CHAR),
        "%s failed with error %d: %s",
        lpszFunction, dw, lpMsgBuf);
    MessageBoxA(NULL, (LPCSTR)lpDisplayBuf, "Error", MB_OK);

    // Free error-handling buffer allocations.
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}