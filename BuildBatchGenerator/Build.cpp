#include <iostream>
#include <fstream>
#include <string>

#include <Windows.h>
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

VOID ExecuteCommand(LPSTR Cmd)
{
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(STARTUPINFOA));
    si.cb = sizeof(STARTUPINFOA);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    if (CreateProcessA(NULL, Cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == FALSE) {
        ErrorHandler("CreateProcessA");
        ExitProcess(EXIT_FAILURE);
    }

    // Wait the process done
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

BOOL WriteBuildBatchFile(LPCSTR lpFileName, LPCSTR lpVSPath)
{
    using namespace std::string_literals;
    std::ofstream output(lpFileName, std::ofstream::out);
    output << "@echo **********************************************************************" << std::endl;
    output << "@echo ** Build Batch for Microsoft detour library by Jim00000" << std::endl;
    output << "@echo **********************************************************************" << std::endl;
    output << R"(@call ")"  << lpVSPath << R"(")" << std::endl;
    output << R"(@cd "%cd%\Detours-4.0.1")" << std::endl;
    output << "nmake" << std::endl;
    output << "pause" << std::endl;
    output.flush();
    output.close();
    return TRUE;
}

INT main(INT argc, PCHAR argv[])
{
    std::cout << "Build Batch Generator under INF-N8 project by Jim00000" << std::endl;
    using namespace std::string_literals;
    const std::string cmd = R"("C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -property installationPath)";
    const std::string defaultBuildBatchName = "build.bat";

    CHAR buffer[BUFSIZ] = { 0 };
    DWORD dwReadSz = 0;

    HANDLE hReadPipe, hWritePipe;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    SECURITY_ATTRIBUTES secuAttr;
    secuAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    secuAttr.bInheritHandle = TRUE;
    secuAttr.lpSecurityDescriptor = NULL;

    if (CreatePipe(&hReadPipe, &hWritePipe, &secuAttr, NULL) == FALSE) {
        ErrorHandler("CreatePipe");
        return EXIT_FAILURE;
    }

    if (SetStdHandle(STD_OUTPUT_HANDLE, hWritePipe) == FALSE) {
        ErrorHandler("SetStdHandle");
        return EXIT_FAILURE;
    }

    ExecuteCommand((LPSTR)cmd.c_str());

    if (ReadFile(hReadPipe, buffer, BUFSIZ, &dwReadSz, NULL) == FALSE) {
        ErrorHandler("ReadFile");
        return EXIT_FAILURE;
    }

    if (SetStdHandle(STD_OUTPUT_HANDLE, hStdOut) == FALSE) {
        ErrorHandler("SetStdHandle");
        return EXIT_FAILURE;
    }

    CloseHandle(hReadPipe);
    CloseHandle(hWritePipe);

    const std::string vs_path = std::string(buffer).substr(0, dwReadSz - 2) + R"(\VC\Auxiliary\Build\vcvars64.bat)";
    std::cout << "Check " << vs_path << " ......... ";

    if (GetFileAttributesA(vs_path.c_str()) == INVALID_FILE_ATTRIBUTES) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            std::cerr << "File " << vs_path << " not found" << std::endl;
        }
        return EXIT_FAILURE;
    }

    std::cout << "exists" << std::endl;

    if (WriteBuildBatchFile(defaultBuildBatchName.c_str(), vs_path.c_str()) == FALSE) {
        std::cerr << "Cannot generate build batch " << defaultBuildBatchName << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << defaultBuildBatchName << " is generated" << std::endl;
    std::cout << "Done" << std::endl;

    system("pause");
    return EXIT_SUCCESS;
}