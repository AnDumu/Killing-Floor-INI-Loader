#include "Utilities.h"
#include "M_Functions.h"

DWORD WINAPI ThreadFunc(VOID);
DWORD M_DetourGetPerObjectNames(DWORD Address);
DWORD M_DetourGetPerObjectNames_Sub1(DWORD Address);
DWORD M_DetourGetPerObjectNames_Sub2(DWORD Address);

DWORD GetPerObjectNames_EAX = NULL;
DWORD GetPerObjectNames_EBX = NULL;
DWORD GetPerObjectNames_ECX = NULL;
DWORD GetPerObjectNames_EDX = NULL;
DWORD GetPerObjectNames_ESI = NULL;
DWORD GetPerObjectNames_EDI = NULL;
DWORD GetPerObjectNames_EBP = NULL;
DWORD GetPerObjectNames_ESP = NULL;
MyGenericVoidNOPARAM pTrampolineGetPerObjectNames = NULL;
DWORD GetPerObjectNames_Ret = NULL;
__declspec(naked) void __stdcall DetourGetPerObjectNames()
{
	__asm
	{
		mov GetPerObjectNames_EAX, eax
		mov GetPerObjectNames_EBX, ebx
		mov GetPerObjectNames_ECX, ecx
		mov GetPerObjectNames_EDX, edx
		mov GetPerObjectNames_ESI, esi
		mov GetPerObjectNames_EDI, edi
		mov GetPerObjectNames_EBP, ebp
		mov GetPerObjectNames_ESP, esp
	}

	__asm pushad;
	__asm pushfd;

	GetPerObjectNames_Ret = MSKF.ProcessFunctions(SystemKF::eFunctions::fGetPerObjectNames);

	__asm popfd;
	__asm popad;

	__asm
	{
		jmp pTrampolineGetPerObjectNames
	}
}

MyGenericVoidNOPARAM pTrampolineGetPerObjectNames_Sub1 = NULL;
DWORD Core_GConfigOrWhatEver = NULL;
BYTE Core_GConfigOrWhatEver_Offset = NULL;
__declspec(naked) void __stdcall DetourGetPerObjectNames_Sub1()
{
	__asm
	{
		pushfd

		cmp [GetPerObjectNames_Ret], 0x0
		je _pass
		popfd
		ret 0x0008

_pass:
		popfd
		jmp pTrampolineGetPerObjectNames_Sub1
	}
}

MyGenericVoidNOPARAM pTrampolineGetPerObjectNames_Sub2 = NULL;
__declspec(naked) void __stdcall DetourGetPerObjectNames_Sub2()
{
	__asm
	{
		pushfd

		cmp [GetPerObjectNames_Ret], 0x0
		je _pass
		popfd
		ret 0x0008

		_pass:
		popfd
		jmp pTrampolineGetPerObjectNames_Sub2
	}
}

MyGenericVoidNOPARAM pTrampolineGetPerObjectNames_Sub3 = NULL;
__declspec(naked) void __stdcall DetourGetPerObjectNames_Sub3()
{
	__asm
	{
		mov [GetPerObjectNames_Ret], 0x00000000
		jmp pTrampolineGetPerObjectNames_Sub3
	}
}

DWORD M_DetourGetPerObjectNames(DWORD Address)
{
	pTrampolineGetPerObjectNames = (MyGenericVoidNOPARAM)DetourFunction((PBYTE)Address, (PBYTE)&DetourGetPerObjectNames);

	return (DWORD)pTrampolineGetPerObjectNames;
}

/*
xxxx
EB 02 33 C0

Core.UObject::execGetPerObjectNames+EA - 74 11                 - je Core.UObject::execGetPerObjectNames+FD
Core.UObject::execGetPerObjectNames+EC - 39 7D C8              - cmp [ebp-38],edi
Core.UObject::execGetPerObjectNames+EF - 74 05                 - je Core.UObject::execGetPerObjectNames+F6
Core.UObject::execGetPerObjectNames+F1 - 8B 45 C4              - mov eax,[ebp-3C]
Core.UObject::execGetPerObjectNames+F4 - EB 09                 - jmp Core.UObject::execGetPerObjectNames+FF
Core.UObject::execGetPerObjectNames+F6 - B8 D8251710           - mov eax,Core.ULanguage::`vftable'+70 { (0) }
Core.UObject::execGetPerObjectNames+FB - EB 02                 - jmp Core.UObject::execGetPerObjectNames+FF
Core.UObject::execGetPerObjectNames+FD - 33 C0                 - xor eax,eax
Core.UObject::execGetPerObjectNames+FF - 8B 0D F4111B10        - mov ecx,[Core.GConfig] { (001D5328) }
Core.UObject::execGetPerObjectNames+105- 8B 11                 - mov edx,[ecx]
Core.UObject::execGetPerObjectNames+107- 50                    - push eax
Core.UObject::execGetPerObjectNames+108- 56                    - push esi { EBP+C						}
Core.UObject::execGetPerObjectNames+109- FF 52 24              - call dword ptr [edx+24] { >>> acá lo pisa o rompe						}

ret 0008
*/
DWORD M_DetourGetPerObjectNames_Sub1(DWORD Address)
{
	DWORD FoundAddress;

	FoundAddress = FindPattern_M((PBYTE)"\xEB\x02\x33\xC0", const_cast<char*>("xxxx"), 0x00, Address, 0x1000);

	if (FoundAddress)
	{
		Core_GConfigOrWhatEver = ReadInProcessDWORD(FoundAddress + 0x06);

		WriteLog(WriteLogFile, 0, ">>> M_DetourGetPerObjectNames_Sub1 -> Core_GConfigOrWhatEver ", IntToHexStr(Core_GConfigOrWhatEver));

		Core_GConfigOrWhatEver_Offset = ReadInProcessBYTE(FoundAddress + 0x10);

		WriteLog(WriteLogFile, 0, ">>> M_DetourGetPerObjectNames_Sub1 -> Core_GConfigOrWhatEver_Offset ", IntToHexStr(Core_GConfigOrWhatEver_Offset));

		//[[[101B11F4]]+24]

		__asm
		{
			mov eax, [Core_GConfigOrWhatEver]
			mov eax, [eax]							; ecx, [Core.GConfig]
			mov eax, [eax]							; mov edx, [ecx]
			xor ecx, ecx
			mov cl, [Core_GConfigOrWhatEver_Offset]
			add eax, ecx
			mov eax, [eax]
			mov [FoundAddress], eax
		}

		WriteLog(WriteLogFile, 0, ">>> M_DetourGetPerObjectNames_Sub1 -> FoundAddress ", IntToHexStr(FoundAddress));

		pTrampolineGetPerObjectNames_Sub1 = (MyGenericVoidNOPARAM)DetourFunction((PBYTE)FoundAddress, (PBYTE)&DetourGetPerObjectNames_Sub1);
	}

	return (DWORD)pTrampolineGetPerObjectNames_Sub1;
}

/*
xx???x
6A 01 ?? ?? ?? E8

Core.UObject::execGetPerObjectNames+335 - 6A 01                 - push 01 { 1 }
Core.UObject::execGetPerObjectNames+337 - 52                    - push edx
Core.UObject::execGetPerObjectNames+338 - 8B CE                 - mov ecx,esi
Core.UObject::execGetPerObjectNames+33A - E8 512CFEFF           - call Core.ULinker::operator=+520 { >>> acá se pisa el output					 }
*/
DWORD M_DetourGetPerObjectNames_Sub2(DWORD Address)
{
	DWORD FoundAddress;

	FoundAddress = FindPattern_M((PBYTE)"\x6A\x01\x00\x00\x00\xE8", const_cast<char*>("xx???x"), 0x05, Address, 0x1000);

	if (FoundAddress)
	{
		FoundAddress = GetRealCALL(FoundAddress);

		WriteLog(WriteLogFile, 0, ">>> M_DetourGetPerObjectNames_Sub2 -> FoundAddress ", IntToHexStr(FoundAddress));

		pTrampolineGetPerObjectNames_Sub2 = (MyGenericVoidNOPARAM)DetourFunction((PBYTE)FoundAddress, (PBYTE)&DetourGetPerObjectNames_Sub2);
	}

	return (DWORD)pTrampolineGetPerObjectNames_Sub2;
}

/*
Core.UObject::execGetPerObjectNames+3F0 - 8B E5                 - mov esp,ebp
Core.UObject::execGetPerObjectNames+3F2 - 5D                    - pop ebp
Core.UObject::execGetPerObjectNames+3F3 - C2 0800               - ret 0008 { 00000008 }
*/
DWORD M_DetourGetPerObjectNames_Sub3(DWORD Address)
{
	DWORD FoundAddress;

	FoundAddress = FindPattern_M((PBYTE)"\x8B\xE5\x5D\xC2\x08\x00", const_cast<char*>("xxxxxx"), 0x00, Address, 0x1000);

	if (FoundAddress)
	{
		WriteLog(WriteLogFile, 0, ">>> M_DetourGetPerObjectNames_Sub3 -> FoundAddress ", IntToHexStr(FoundAddress));

		pTrampolineGetPerObjectNames_Sub3 = (MyGenericVoidNOPARAM)DetourFunction((PBYTE)FoundAddress, (PBYTE)&DetourGetPerObjectNames_Sub3);
	}

	return (DWORD)pTrampolineGetPerObjectNames_Sub3;
}

DWORD WINAPI ThreadFunc(VOID)
{
	std::string DLL = "Core.dll";
	std::string Function = "?execGetPerObjectNames@UObject@@QAEXAAUFFrame@@QAX@Z";
	DWORD dFunction = NULL;
	DWORD dFunction_bk = NULL;

	Sleep(1000);

	EnableDebugPrivileges();

	DirPath = GetExePathW();
	FullPath = DirPath + MainINI;
	LoadedINIs.clear();
	LoadDebug();

	WriteLog(WriteLogFile, 0, "DirPath: ", DirPath.c_str());

	while (1)
	{
		LoadDebug();

		if (!pTrampolineGetPerObjectNames)
		{
			dFunction = GetFunctionByModule(DLL, Function);
			dFunction_bk = dFunction;

			if (dFunction)
			{
				if (M_DetourGetPerObjectNames(dFunction))
				{
					WriteLog(WriteLogFile, 0, "", "M_DetourGetPerObjectNames Detoured!!!");

					if (M_DetourGetPerObjectNames_Sub1(dFunction))
					{
						WriteLog(WriteLogFile, 0, "", "M_DetourGetPerObjectNames_Sub1 Detoured!!!");

						if (M_DetourGetPerObjectNames_Sub2(dFunction))
						{
							WriteLog(WriteLogFile, 0, "", "M_DetourGetPerObjectNames_Sub2 Detoured!!!");

							if (M_DetourGetPerObjectNames_Sub3(dFunction_bk))
							{
								WriteLog(WriteLogFile, 0, "", "M_DetourGetPerObjectNames_Sub3 Detoured!!!");
								WriteLog(WriteLogFile, 0, "", "");
							}
							else
							{
								WriteLog(WriteLogFile, 0, "", "M_DetourGetPerObjectNames_Sub3 COULD NOT Detoured :(");
								ExitProcess(0);
							}
						}
						else
						{
							WriteLog(WriteLogFile, 0, "", "M_DetourGetPerObjectNames_Sub2 COULD NOT Detoured :(");
							ExitProcess(0);
						}
					}
					else
					{
						WriteLog(WriteLogFile, 0, "", "M_DetourGetPerObjectNames_Sub1 COULD NOT Detoured :(");
						ExitProcess(0);
					}
				}
				else
				{
					WriteLog(WriteLogFile, 0, "", "M_DetourGetPerObjectNames COULD NOT Detoured :(");
					ExitProcess(0);
				}
			}
		}

		Sleep(666);
	}

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);

			if ((hThread = ::CreateThread(NULL, 0, (::LPTHREAD_START_ROUTINE)&ThreadFunc, (::HMODULE)hModule, 0, NULL)) == NULL)
			{
				return FALSE;
			}

			if (::CloseHandle(hThread) == FALSE)
			{
				//do nothing
			}

            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
