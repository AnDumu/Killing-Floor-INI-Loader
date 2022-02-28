// KFInjector2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <algorithm> //std::transform

// Inject is a tool which injects an ordered list of shared libraries into the
// address space of a binary executable. The created process is initially
// suspended, and resumes execution once the ordered list of shared libraries
// have been loaded into its address space, and their respective DllMain
// functions have finished executing.
//
// Usage
//
//    $ inject EXE [DLL...]
//
// Examples
//
//    $ inject a.exe b.dll c.dll
//
// Order of execution:
//
//    1. Creates a suspended process of "a.exe".
//    2. Loads "b.dll" into the address space of "a.exe".
//    3. Executes the "DllMain" function of "b.dll".
//    4. Loads "c.dll" into the address space of "a.exe".
//    5. Executes the "DllMain" function of "d.dll".
//    6. Resumes execution of "a.exe".

BOOL EnableDebugPrivileges()
{
	HANDLE tokenHandle;
	LUID luid;
	TOKEN_PRIVILEGES newPrivileges;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &tokenHandle))
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		CloseHandle(tokenHandle);
		return FALSE;
	}

	newPrivileges.PrivilegeCount = 1;
	newPrivileges.Privileges[0].Luid = luid;
	newPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(tokenHandle, FALSE, &newPrivileges, sizeof(newPrivileges), NULL, NULL))
	{
		CloseHandle(tokenHandle);
		return FALSE;
	}

	CloseHandle(tokenHandle);

	return TRUE;
}

std::string toLower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });

	return s;
}

HANDLE GetProcessByName(std::string name)
{
	DWORD pid = 0;

	// Create toolhelp snapshot.
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			// Compare process.szExeFile based on format of name, i.e., trim file path
			// trim .exe if necessary, etc.
			if (toLower(name) == toLower(std::string(process.szExeFile)))
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}

	CloseHandle(snapshot);

	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
}

/*
KFInjector.exe ucc.exe tuvieja.dll
Count: 4
0: KFInjector.exe
1: ucc.exe
2: tuvieja.dll
*/
int main(int argc, char** argv)
{
	int i, len;
	char* exe, * lib_path;
	void* page;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	HANDLE hThread;
	HANDLE hProc;

	EnableDebugPrivileges();

	std::cout << std::endl << std::endl;

	std::cout << "----------------------------------------" << std::endl;

	std::cout << "Injecting..." << std::endl;

	std::cout << "\t" << "Args Count: " << argc << std::endl;
	for (i = 0; i < argc; i++)
	{
		std::cout << "\t\t" << i << ": " << argv[i] << std::endl;
	}

	std::cout << std::endl;

	//return 0;

	// Print usage.
	if (argc < 2)
	{
		fprintf(stderr, "Usage: inject EXE [DLL...]\n");
		fprintf(stderr, "Inject an ordered list of shared libraries into the address space of a binary executable.\n");

		std::cout << "----------------------------------------" << std::endl;
		std::cout << std::endl << std::endl;

		return 1;
	}

	// Execute the process in suspended mode.
	exe = argv[1];
	si.cb = sizeof(STARTUPINFO);

	hProc = GetProcessByName(exe);

	if (!hProc)
	{
		std::cout << "Error: Could not find " << exe << std::endl << std::endl;

		std::cout << "----------------------------------------" << std::endl;
		std::cout << std::endl << std::endl;

		return 1;
	}

	std::cout << "\t" << "VirtualAllocEx" << std::endl;
	// Allocate a page in memory for the arguments of LoadLibrary.
	page = VirtualAllocEx(hProc, NULL, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE /*PAGE_EXECUTE_READWRITE*/);
	std::cout << "\t" << "VirtualAllocEx passed! 0x" << std::hex << (DWORD)page << std::dec << std::endl << std::endl;

	if (!page)
	{
		fprintf(stderr, "VirtualAllocEx failed; error code = 0x%08X\n\n", GetLastError());

		std::cout << "----------------------------------------" << std::endl;
		std::cout << std::endl << std::endl;

		CloseHandle(hProc);

		return 1;
	}

	// Inject the ordered list of shared libraries into the address space of the
	// process.
	for (i = 2; i < argc; i++)
	{
		// Verify path length.
		lib_path = argv[i];
		len = strlen(lib_path) + 1;

		if (len > MAX_PATH)
		{
			fprintf(stderr, "path length (%d) exceeds MAX_PATH (%d).\n\n", len, MAX_PATH);

			std::cout << "----------------------------------------" << std::endl;
			std::cout << std::endl << std::endl;

			VirtualFreeEx(hProc, page, 0, MEM_RELEASE);
			CloseHandle(hProc);

			return 1;
		}

		if (GetFileAttributes(lib_path) == INVALID_FILE_ATTRIBUTES)
		{
			fprintf(stderr, "unable to locate library (%s).\n\n", lib_path);

			std::cout << "----------------------------------------" << std::endl;
			std::cout << std::endl << std::endl;

			VirtualFreeEx(hProc, page, 0, MEM_RELEASE);
			CloseHandle(hProc);

			return 1;
		}

		std::cout << "\t" << "WriteProcessMemory" << std::endl;
		// Write library path to the page used for LoadLibrary arguments.
		if (WriteProcessMemory(hProc, page, lib_path, len, NULL) == 0)
		{
			fprintf(stderr, "WriteProcessMemory failed; error code = 0x%08X\n\n", GetLastError());

			std::cout << "----------------------------------------" << std::endl;
			std::cout << std::endl << std::endl;

			VirtualFreeEx(hProc, page, 0, MEM_RELEASE);
			CloseHandle(hProc);

			return 1;
		}
		std::cout << "\t" << "WriteProcessMemory passed!" << std::endl << std::endl;

		std::cout << "\t" << "CreateRemoteThread" << std::endl;
		// Inject the shared library into the address space of the process,
		// through a call to LoadLibrary.
		hThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, page, 0, NULL);

		if (hThread == NULL)
		{
			fprintf(stderr, "CreateRemoteThread failed; error code = 0x%08X\n\n", GetLastError());

			std::cout << "----------------------------------------" << std::endl;
			std::cout << std::endl << std::endl;

			VirtualFreeEx(hProc, page, 0, MEM_RELEASE);
			CloseHandle(hProc);

			return 1;
		}
		std::cout << "\t" << "CreateRemoteThread passed!" << std::endl << std::endl;

		std::cout << "\t" << "WaitForSingleObject" << std::endl;
		// Wait for DllMain to return.
		if (WaitForSingleObject(hThread, INFINITE) == WAIT_FAILED)
		{
			fprintf(stderr, "WaitForSingleObject failed; error code = 0x%08X\n\n", GetLastError());

			std::cout << "----------------------------------------" << std::endl;
			std::cout << std::endl << std::endl;

			VirtualFreeEx(hProc, page, 0, MEM_RELEASE);
			CloseHandle(hProc);

			return 1;
		}
		std::cout << "\t" << "WaitForSingleObject passed!" << std::endl << std::endl;

		// Cleanup.
		CloseHandle(hThread);
	}

	// Cleanup.
	VirtualFreeEx(hProc, page, 0, MEM_RELEASE);
	CloseHandle(hProc);

	std::cout << "Injected!!!" << std::endl << std::endl;

	std::cout << "----------------------------------------" << std::endl;
	std::cout << std::endl << std::endl;

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
