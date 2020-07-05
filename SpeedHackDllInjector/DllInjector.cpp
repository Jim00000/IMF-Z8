#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

DWORD getProcessIdByName(LPCWSTR procName)
{
	DWORD processId = 0;
	SIZE_T workingSetSz = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;

	PROCESSENTRY32W procEntry = { 0 };
	procEntry.dwSize = sizeof(PROCESSENTRY32W);
	
	if (Process32FirstW(hSnapshot, &procEntry)) {
		while (Process32Next(hSnapshot, &procEntry)) {
			if (wcscmp(procName, procEntry.szExeFile) == 0) {
				const HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry.th32ProcessID);
				PROCESS_MEMORY_COUNTERS memCounters;
				GetProcessMemoryInfo(hProcess, &memCounters, sizeof(memCounters));
				if (workingSetSz < memCounters.WorkingSetSize) {
					workingSetSz = memCounters.WorkingSetSize;
					processId = procEntry.th32ProcessID;
				}
				CloseHandle(hProcess);
			}
		}
	}

	CloseHandle(hSnapshot);
	return processId;
}

int main()
{	
	CHAR dllPath[BUFSIZ] = { 0 };
	GetFullPathNameA("SpeedHack.dll", BUFSIZ, dllPath, NULL);

	DWORD dwProcId = getProcessIdByName(L"YuGiOh.exe");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcId);
	LPVOID LoadLibraryAddr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"),"LoadLibraryA");
	LPVOID lpAddr = (LPVOID)VirtualAllocEx(hProcess, NULL, strlen((LPCSTR)dllPath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, lpAddr, dllPath, strlen((LPCSTR)dllPath), NULL);
	CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryAddr, lpAddr, NULL, NULL);

	system("pause");

	HMODULE hMod = LoadLibraryA((LPCSTR)dllPath);
	LPVOID lpFunc = (LPVOID)GetProcAddress(hMod, "freeThisLibrary");
	CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpFunc, NULL, NULL, NULL);
	FreeLibrary(hMod);
	VirtualFreeEx(hProcess, lpAddr, 0, MEM_RELEASE);
	CloseHandle(hProcess);
    return 0;
}