#include "windows.h"
#include "TlHelp32.h"
#include <iostream>;
using namespace std;

DWORD GetProcessId(PCWSTR procName) {
	DWORD procId = 0;
	// делаем снисок всех процесcов
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		//берем первый процесс , проверяем наш или нет, берем некст и тд 
		if (Process32First(hSnap, &procEntry)) {
			do {
				if (wstring(procEntry.szExeFile) == wstring(procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}


int main() {
	const char* dllPath = "MemoryDll.dll";
	PCWSTR procName = L"LittleProgram.exe";

	DWORD procId = 0;

	while (!procId) {
		procId = GetProcessId(procName);
		Sleep(30);
	}

	//получаем хэндл нашего процесса
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

	if (hProc && hProc != INVALID_HANDLE_VALUE) {
		//выделяем память под имя нашей длл
		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		//записываем в память процесса адрес нашей длл
		if (loc) 
		{
			WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
		}
		// вызываем нашу либу с помощью удаленного процесса
		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

		if (hThread) {
			CloseHandle(hThread);
		}
	}
	if (hProc) {
		CloseHandle(hProc);
	}
	return 0;
}
