#include "M_Functions.h"
#include "Utilities.h"

SystemKF MiserySKF;

/*
https://docs.unrealengine.com/udk/Three/StringsInUnrealScript.html

https://wiki.beyondunreal.com/String

https://wiki.beyondunreal.com/Legacy:Useful_String_Functions

int("123")

UnrealScript
function FillStrByChar(out string str, int size)
{
	local int i;

	for (i = 0; i < size; i++)
	{
		str $= "|";
	}
}

function FillStrByStr(out string str, out string str2, int size)
{
	local int i;

	for (i = 0; i < size; i++)
	{
		str $= str2;
	}
}
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
		/*
		* //Get TOTAL
		* 
		* local array<string> List;
		* 
		* List.Length = 1;
		* 
		* List[0] = "4294967295"; //2147483647
		* 
		* List = GetPerObjectNames("AreYouVipList.ini", "Total");
		* 
		* -------------------------------------------------------
		* //Get CHANGED, tal vez este es mejor dejarlo a mano de la DLL
		* 
		* local array<string> List;
		* 
		* List.Length = 1;
		* 
		* List[0] = "0"
		* 
		* List = GetPerObjectNames("AreYouVipList.ini", "Changed");
		* 
		* -------------------------------------------------------
		* //Get ELEMENT
		* 
		* local array<string> List;
		* local string tmp;
		* 
		* //SteamID=76561198066780194 (17)
		* //Level=3 (1)
		* //25 chars por si las dudas
		* 
		* FillStrByChar(tmp, 25) //cantidad de chars por sección
		* 
		* List.Length = 1;
		* 
		* List[0] = tmp;
		* 
		* List = GetPerObjectNames("AreYouVipList.ini", "1") //el número index
		* 
		* -------------------------------------------------------
		* //Get ALL
		* 
		* local array<string> List;
		* local string tmp;
		* local int total; //ya obtenido
		* local int i;
		* 
		* List.Length = 1;
		* 
		* //Tal vez convenga completar como 0000000143
		* List[0] = "4294967295" //2147483647
		* 
		* List = GetPerObjectNames("AreYouVipList.ini", "Total");
		* 
		* total = intr(List[0]);
		* 
		* if (total > 0)
		* {
		*		//SteamID=76561198066780194 (17)
		*		//Level=3 (1)
		*		//25 chars por si las dudas
		* 
		*		FillStrByChar(tmp, 25) //cantidad de chars por sección
		* 
		*		List.Length = total;
		* 
		*		for (i = 0; i < total; i++)
		*		{
		*			List[i] = tmp;
		*		}
		* 
		*		List = GetPerObjectNames("AreYouVipList.ini", "All") //todos
		* }
		* 
		*/
		//native static final function array<string> GetPerObjectNames(string ININame, optional string ObjectClass, optional int MaxResults /*1024 if unspecified*/); //no extension
		//Core.UObject::execGetPerObjectNames
		case SystemKF::fGetPerObjectNames:
			//wchar_t * InPutAddress_1;
			//wchar_t * InPutAddress_2;
			//DWORD OutPutAddress;

			tParam * Input;

			WriteLog(WriteLogFile, 1, "", "SystemKF::fGetPerObjectNames");

			//InPutAddress_1 = NULL;
			//InPutAddress_2 = NULL;

			/*__asm
			{
				mov eax, GetPerObjectNames_ESP
				mov eax, [eax + 0x04]
				mov eax, [eax + 0x10]
				mov eax, [eax]
				cmp eax, 0x00000000
				je _out
				mov [InPutAddress_1], eax

				mov eax, GetPerObjectNames_ESP
				mov eax, [eax + 0x04]
				mov eax, [eax + 0x10]
				mov eax, [eax]
				mov[InPutAddress_2], eax

				mov eax, GetPerObjectNames_ESP
				mov eax, [eax + 0x08]
				mov [OutPutAddress], eax

			_out:
				nop
			}*/

			//if (InPutAddress_1)
			{
				/*WriteLog(WriteLogFile, 1, "InputAddress n1: ", InPutAddress_1);

				WriteLog(WriteLogFile, 1, "InputAddress n2: ", InPutAddress_2);

				if (IsInINIList(InPutAddress_1))
				{
					WriteLog(WriteLogFile, 0, "", "");
					WriteLog(WriteLogFile, 1, InPutAddress_1, " is in list!!!");
					WriteLog(WriteLogFile, 0, "", "");

					hRet = ProcessInput(InPutAddress_1, InPutAddress_2, (FString*)OutPutAddress);
				}*/

				/*FS = (FString*)OutPutAddress;

				for (size_t i = 0; i < FS->Count; i++)
				{
					if (FS->FStringPtr[i].Str)
					{
						WriteLog("KF UCC.log", 1, "OutPut[" + IntToStr(i) + "]: ", FS->FStringPtr[i].Str);
					}
				}*/
			}

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