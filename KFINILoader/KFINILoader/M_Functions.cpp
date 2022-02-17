#include "M_Functions.h"
#include "Utilities.h"

SystemKF MiserySKF;

/*
https://docs.unrealengine.com/udk/Three/StringsInUnrealScript.html

https://wiki.beyondunreal.com/String

https://wiki.beyondunreal.com/Legacy:Useful_String_Functions
*/

DWORD SystemKF::ProcessFunctions(SystemKF::eFunctions Function)
{
	DWORD hRet = 0;

	WriteLog(L"KF UCC.log", 0, "", ">>>>>> ProcessFunctions");

	switch (Function)
	{
		case SystemKF::_begin:
			break;
		case SystemKF::None:
			break;
		//native static final function array<string> GetPerObjectNames(string ININame, optional string ObjectClass, optional int MaxResults /*1024 if unspecified*/); //no extension
		//Core.UObject::execGetPerObjectNames
		case SystemKF::fGetPerObjectNames:
			tParam * Input;

			WriteLog(L"KF UCC.log", 1, "", "SystemKF::fGetPerObjectNames");

			__asm
			{
				mov eax, GetPerObjectNames_ESP
				mov eax, [eax + 0x04]
				mov eax, [eax + 0x10]

				mov [Input], eax
			}

			if (Input && Input->Param1.Str)
			{
				WriteLog(L"KF UCC.log", 1, "InputAddress n1: ", Input->Param1.Str);

				if (Input->Param2.Str)
				{
					WriteLog(L"KF UCC.log", 1, "InputAddress n2: ", Input->Param2.Str);

					WriteLog(L"KF UCC.log", 1, "Output Count: ", std::to_string(Input->OutPut.Count));

					if (Input->OutPut.Count > 0)
					{
						hRet = ProcessInput(Input->Param1.Str, Input->Param2.Str, &Input->OutPut);
					}
				}
			}

			break;
		case SystemKF::_end:
			break;
		default:
			break;
	}

	WriteLog(L"KF UCC.log", 0, "", "<<<<<< ProcessFunctions out");
	WriteLog(L"KF UCC.log", 0, "", "----------------------------------");

	return hRet;
}
