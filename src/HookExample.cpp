#include "stdafx.h"
#include <Windows.h>
#include <winuser.h>

#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include <string>
#include <psapi.h>


//https://stackoverflow.com/questions/17747345/callwndproc-example
/* The handle to the hook is stored as a shared global variable and is the
* same for all hooked processes. We achieve that by placing it in the
* shared data segment of the DLL.
*
* Note that shared global variables must be explicitly initialized.
*
* And also note that this is really not the ideal way of doing this; it's just
* an easy way to get going. The better solution is to use a memory-mapped file.
* See Also: http://msdn.microsoft.com/en-us/library/h90dkhs0.aspx
*/
#pragma comment(linker, "/section:.SHARED,rws")
#pragma data_seg(".SHARED") /* begin the shared data segment */
HHOOK g_hhkCallWndProcRet = NULL;
#pragma data_seg()          /* end the shared data segment and default back to normal behavior */


LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	/* If nCode is greater than or equal to HC_ACTION,
	* we should process the message. */
	if (nCode >= HC_ACTION)
	{
		/* Retrieve a pointer to the structure that contains details about
		* the message, and see if it is one that we want to handle. */
		const LPCWPRETSTRUCT lpcwprs = (LPCWPRETSTRUCT)lParam;
		switch (lpcwprs->message)
		{
			/* ...SNIP: process the messages we're interested in ... */
		case WM_DROPFILES:
			OutputDebugString(L"WM_DROPFILES");
			break;
		}
	}

	/* At this point, we are either not processing the message
	* (because nCode is less than HC_ACTION),
	* or we've already finished processing it.
	* Either way, pass the message on. */
	return CallNextHookEx(g_hhkCallWndProcRet, nCode, wParam, lParam);
}

DWORD g_cPid = 0;

//int mainX(int argc, char **argv) {
HANDLE getConProc()
{
	int argc = 0; char **argv = NULL;
	DWORD pid = 0, i = 0, cPid = 0;
	PROCESSENTRY32 pe32;
	BOOL res = FALSE;
	HANDLE snap = INVALID_HANDLE_VALUE, proc = INVALID_HANDLE_VALUE;
	char c = 0;
	if (argc > 1) {
		pid = atoi(argv[1]);
	}
	else {
		pid = GetCurrentProcessId();
	}
	printf("PID: %d\n", pid);
	snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, pid);
	if (snap == INVALID_HANDLE_VALUE) {
		printf("CreateToolhelp32Snapshot failed: %d\n", GetLastError());
//		return -1;
		return NULL;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	res = Process32First(snap, &pe32);
	if (res == FALSE) {
		printf("Process32First failed: %d\n", GetLastError());
		CloseHandle(snap);
//		return -2;
		return NULL;
	}
	do {
		if (_tcscmp(pe32.szExeFile, TEXT("conhost.exe")) == 0) {
			_tprintf(TEXT("    Idx: %02d  PID: %5d  PPID: %5d  Name: %s\n"), i++, pe32.th32ProcessID, pe32.th32ParentProcessID, pe32.szExeFile);
			if (pe32.th32ParentProcessID == pid) {
				cPid = pe32.th32ProcessID;
			}
		}
	} while ((res = Process32Next(snap, &pe32)));
	CloseHandle(snap);

	if ((proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, cPid)) == INVALID_HANDLE_VALUE) {
		printf("OpenProcess failed: %d\n", GetLastError());
//		return -3;
		return NULL;
	}
	printf("Conhost handle: 0x%08X\n", proc);
	CloseHandle(proc);

	printf("Press a key to exit...\n");
//	c = getch();
	g_cPid = cPid;
	return proc;
}


extern HINSTANCE g__hInstance;

#ifndef MAKEULONGLONG
#define MAKEULONGLONG(ldw, hdw) ((ULONGLONG(hdw) << 32) | ((ldw) & 0xFFFFFFFF))
#endif

#ifndef MAXULONGLONG
#define MAXULONGLONG ((ULONGLONG)~((ULONGLONG)0))
#endif
DWORD GetThreadId(DWORD dwProcID)
{
	DWORD dwMainThreadID = 0;
	ULONGLONG ullMinCreateTime = MAXULONGLONG;

	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 th32;
		th32.dwSize = sizeof(THREADENTRY32);
		BOOL bOK = TRUE;
		for (bOK = Thread32First(hThreadSnap, &th32); bOK;
			bOK = Thread32Next(hThreadSnap, &th32)) {
			if (th32.th32OwnerProcessID == dwProcID) {
				HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION,
					TRUE, th32.th32ThreadID);
				if (hThread) {
					FILETIME afTimes[4] = { 0 };
					if (GetThreadTimes(hThread,
						&afTimes[0], &afTimes[1], &afTimes[2], &afTimes[3])) {
						ULONGLONG ullTest = MAKEULONGLONG(afTimes[0].dwLowDateTime,
							afTimes[0].dwHighDateTime);
						if (ullTest && ullTest < ullMinCreateTime) {
							ullMinCreateTime = ullTest;
							dwMainThreadID = th32.th32ThreadID; // let it be main... :)
						}
					}
					CloseHandle(hThread);
				}
			}
		}
#ifndef UNDER_CE
		CloseHandle(hThreadSnap);
#else
		CloseToolhelp32Snapshot(hThreadSnap);
#endif
	}
	return dwMainThreadID;
}

HMODULE GetModule()
{
	HMODULE hMods[1024];
	HANDLE pHandle = getConProc();//GetHandle();
	DWORD cbNeeded;
	unsigned int i;

	if (EnumProcessModules(pHandle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(pHandle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				std::wstring wstrModName = szModName;
				//you will need to change this to the name of the exe of the foreign process
				std::wstring wstrModContain = L"conhost.exe";
				if (wstrModName.find(wstrModContain) != std::string::npos)
				{
					CloseHandle(pHandle);
					return hMods[i];
				}
			}
		}
	}
	DWORD er = ::GetLastError();
	return nullptr;
}


BOOL __stdcall InstallHook(void)
{
//	HINSTANCE g_hinstDLL = g__hInstance;
	HINSTANCE g_hinstDLL = GetModuleHandle(NULL);

    //HWND hwnd = GetConsoleWindow();
	//HINSTANCE g_hinstDLL = (HINSTANCE)GetWindowLong(hwndC, GWL_HINSTANCE);

	///HINSTANCE g_hinstDLL = (HINSTANCE)GetWindowLongPtr(hwndC, GWLP_HINSTANCE);


	/* Try to install the WH_CALLWNDPROCRET hook,
	* if it is not already installed. */
	if (!g_hhkCallWndProcRet)
	{
		//g_hhkCallWndProcRet = SetWindowsHookEx(WH_CALLWNDPROCRET,
		//	CallWndRetProc,
		//	g_hinstDLL,
		//	0);

		//g_hhkCallWndProcRet = SetWindowsHookEx(WH_CALLWNDPROCRET,
		//	CallWndRetProc,
		//	g_hinstDLL,
		//	::GetWindowThreadProcessId(GetConsoleWindow(), NULL));

		g_hhkCallWndProcRet = SetWindowsHookEx(WH_CALLWNDPROCRET,
			CallWndRetProc,
			NULL,
			GetCurrentThreadId());

		HANDLE handle = getConProc();
		DWORD threadId = GetThreadId(g_cPid);
		HMODULE amodule = GetModule();

		//g_hhkCallWndProcRet = SetWindowsHookEx(WH_CALLWNDPROCRET,
		//	CallWndRetProc,
		//	amodule,
		//	threadId);

		DWORD er = ::GetLastError();
		if (!g_hhkCallWndProcRet)
		{
			/* ...SNIP: handle failure condition ... */
			return FALSE;
		}
	}

	return TRUE;  /* return success */
}

BOOL __stdcall RemoveHook(void)
{
	/* Try to remove the WH_CALLWNDPROCRET hook, if it is installed. */
	if (g_hhkCallWndProcRet)
	{
		if (!UnhookWindowsHookEx(g_hhkCallWndProcRet))
		{
			/* ...SNIP: handle failure condition ... */
			return FALSE;
		}
		g_hhkCallWndProcRet = NULL;
	}
}