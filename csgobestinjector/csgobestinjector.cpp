#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <TlHelp32.h>

DWORD dwPID;
HANDLE hProcess;

bool AttachProcess(const char * name)
{
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(ProcessEntry);

	do
		if (!strcmp(ProcessEntry.szExeFile, name))
		{
			dwPID = ProcessEntry.th32ProcessID;
			CloseHandle(hPID);

			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
			return true;
		}
		else
			Sleep(1);
	while (Process32Next(hPID, &ProcessEntry));

	return false;
}


bool InjectDLL(const char * name)
{
	DWORD  retval = 0;
	TCHAR  buffer[4096] = TEXT("");
	TCHAR* *lppPart = { NULL };
	retval = GetFullPathName(name, 4096, buffer, lppPart);

	if (retval == 0)
	{
		printf("[DLL] GetFullPathName failed (%d)\n", GetLastError());
		return false;
	}
	else
	{
		_tprintf(TEXT("The full path name is:  %s\n"), buffer);
		if (lppPart != NULL && *lppPart != 0)
		{
			_tprintf(TEXT("The final component in the path name is:  %s\n"), *lppPart);
		}

		HANDLE Process;
		LPVOID Memory;
		LPVOID LoadLibrary;

		if (!dwPID)
			return false;

		Process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwPID);
		LoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

		Memory = (LPVOID)VirtualAllocEx(Process, NULL, strlen(buffer), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(Process, (LPVOID)Memory, buffer, strlen(buffer), NULL);
		CreateRemoteThread(Process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrary, (LPVOID)Memory, NULL, NULL);

		CloseHandle(Process);
		VirtualFree((LPVOID)Memory, strlen(buffer), NULL);

		return true;
	}
}

int main()
{
	bool debug = true;
	if (AttachProcess("csgo.exe"))
	{
		if (debug)
			InjectDLL("../Debug/csgobestgame.dll");
		else
			InjectDLL("csgobestgame.dll");
		CloseHandle(hProcess);
		return 0;
	}
	return 1;
}