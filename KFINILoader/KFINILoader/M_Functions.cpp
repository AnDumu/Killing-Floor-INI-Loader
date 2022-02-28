#include "M_Functions.h"
#include "Utilities.h"

SystemKF MSKF;

/*
https://docs.unrealengine.com/udk/Three/StringsInUnrealScript.html

https://wiki.beyondunreal.com/String

https://wiki.beyondunreal.com/Legacy:Useful_String_Functions
*/

DWORD SystemKF::ProcessFunctions(SystemKF::eFunctions Function)
{
	DWORD hRet = 0;

	WriteLog(WriteLogFile, 0, "", ">>>>>> ProcessFunctions");

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

			WriteLog(WriteLogFile, 1, "", "SystemKF::fGetPerObjectNames");

			__asm
			{
				mov eax, GetPerObjectNames_ESP
				mov eax, [eax + 0x04]
				mov eax, [eax + 0x10]

				mov [Input], eax
			}

			if (Input && Input->Param1.Str && Input->Param2.Str)
			{
				if (AvailableAddress(Input->Param1.Str) && AvailableAddress(Input->Param2.Str))
				{
					WriteLog(WriteLogFile, 1, "InputAddress n1: ", Input->Param1.Str);
					WriteLog(WriteLogFile, 1, "InputAddress n2: ", Input->Param2.Str);
					WriteLog(WriteLogFile, 1, "Output Count: ", std::to_string(Input->OutPut.Count));

					if (Input->OutPut.Count > 0)
					{
						hRet = ProcessInput(Input->Param1.Str, Input->Param2.Str, &Input->OutPut);
					}
					else
					{
						WriteLog(WriteLogFile, 2, "gettin out: ", "OutPut->Count == 0");
					}
				}
			}

			break;
		case SystemKF::_end:
			break;
		default:
			break;
	}

	WriteLog(WriteLogFile, 0, "", "<<<<<< ProcessFunctions out");
	WriteLog(WriteLogFile, 0, "", "----------------------------------");

	return hRet;
}
