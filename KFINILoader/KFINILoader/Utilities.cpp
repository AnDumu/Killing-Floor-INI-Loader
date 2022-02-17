#include "Utilities.h"

/*	MiseryCFG.ini
[INIT]
Total=1
Debug=1

[1]
INI=AreYouVipList.ini
Changed=0
*/

DWORD IDThread;
HANDLE hThread;
std::wstring DirPath;
BOOL Debug = FALSE;

std::wstring FullPath;
std::wstring MainINI = L"MiseryCFG.ini";

#pragma warning (disable:4996) //_SCL_SECURE_NO_WARNINGS

void LoadDebug()
{
	Debug = (GetPrivateProfileInt(L"INIT", L"Debug", 0, FullPath.c_str()));
}

int GetMainINICount()
{
	return (GetPrivateProfileInt(L"INIT", L"Total", 0, FullPath.c_str()));
}

int GetINICount(std::wstring File)
{
	return (GetPrivateProfileInt(L"INIT", L"Total", 0, File.c_str()));
}

std::wstring GetINIRetrieveFields(std::wstring File)
{
	//wchar_t Buf[MAX_PATH] = { 0 };

	//GetPrivateProfileString(L"INIT", L"RetrieveFields", L"", Buf, 255, File.c_str());;

	//return std::wstring(Buf);

	return GetINIInfo(L"INIT", L"RetrieveFields", L"", File.c_str());
}

std::wstring GetINIInfo(std::wstring lpAppName, std::wstring lpKeyName, std::wstring lpDefault, std::wstring File)
{
	wchar_t Buf[MAX_PATH] = { 0 };

	GetPrivateProfileString(lpAppName.c_str(), lpKeyName.c_str(), lpDefault.c_str(), Buf, 255, File.c_str());;

	return std::wstring(Buf);
}

std::wstring GetINIItem(std::wstring Name, std::vector<std::wstring> Fields, std::wstring File)
{
	std::wstring Data = L"";

	for (size_t i = 0; i < Fields.size(); i++)
	{
		Data += GetINIInfo(Name, Fields[i], L"", File.c_str()) + (i == Fields.size() - 1 ? L"" : L"|");
	}

	return Data;
}

BOOL IsInINIList(wchar_t * INIToFind)
{
	int count = GetMainINICount();

	//WriteLog("KF UCC.log", "Ini counts: ", IntToStr(count));

	if (count <= 0) return FALSE;

	std::wstring BufStr;

	for (size_t i = 1; i <= count; i++)
	{
		BufStr = GetINIInfo(std::to_wstring(i), L"INI", L"", FullPath.c_str());

		//GetPrivateProfileStringA(IntToStr(i).c_str(), "INI", "", Buf, 255, FullPath.c_str());

		if (wcsicmp(BufStr.c_str(), INIToFind) == NULL) return TRUE;
	}

	return FALSE;
}

DWORD ProcessInput(wchar_t * INIFile, wchar_t * Seq, FString * OutPut)
{
	DWORD hRet = 0;
	std::wstring File;
	int len;
	int Total;
	std::wstring Data;

	WriteLog(L"KF UCC.log", 1, "", ">>>>>> ProcessInput");
	WriteLog(L"KF UCC.log", 2, "Sequence: ", Seq);

	if (!OutPut)
	{
		WriteLog(L"KF UCC.log", 2, "gettin out: ", "OutPut NULL");

		//dejo que llame a la función real y pise el valor (reset)
		return 0;
	}

	if (OutPut->Count == 0)
	{
		WriteLog(L"KF UCC.log", 2, "gettin out: ", "OutPut->Count == 0");

		//dejo que llame a la función real y pise el valor (reset)
		return 0;
	}

	File = DirPath + INIFile;

	Total = GetINICount(File);

	WriteLog(L"KF UCC.log", 2, "File: ", File);

	if (wcsicmp(Seq, L"TOTAL") == NULL)
	{
		if (OutPut->Count != 1)
		{
			WriteLog(L"KF UCC.log", 2, "gettin out: ", "OutPut->Count != 1 it must be 1");

			//dejo que llame a la función real y pise el valor (reset)
			return 0;
		}

		//GetPrivateProfileString(L"INIT", L"Total", L"", Buf, 255, File.c_str());;
		Data = GetINIInfo(L"INIT", L"Total", L"", File.c_str());

		WriteLog(L"KF UCC.log", 2, "BuffStr: ", Data);

		len = wcslen(OutPut->FStringPtr[0].Str);

		WriteLog(L"KF UCC.log", 2, "OutPut->FStringPtr[0].Str len: ", IntToStr(len));

		if (Data.size() > len)
		{
			WriteLog(L"KF UCC.log", 2, "gettin out: ", "BufStr.length() > len");

			//dejo que llame a la función real y pise el valor (reset)
			return 0;
		}

		//For example : old_str = "45"; n_zero = 4; new_str = "0045";
		//auto new_str = std::string(n_zero - std::min(n_zero, old_str.length()), '0') + old_str;
		std::wstring OutWStr = std::wstring(len - Data.size(), '0') + Data;

		WriteLog(L"KF UCC.log", 2, "OutWStr: ", OutWStr);

		wcscpy(OutPut->FStringPtr[0].Str, OutWStr.c_str());

		hRet = 1;
	}
	else
	{
		WriteLog(L"KF UCC.log", 2, "else", "");

		std::wstring Fields = GetINIRetrieveFields(File);

		if (Fields.size() == 0)
		{
			WriteLog(L"KF UCC.log", 2, "gettin out: ", "Retrieve Fields len 0");

			//dejo que llame a la función real y pise el valor (reset)
			return 0;
		}

		std::vector<std::wstring> splitFields = Split(Fields, L"|", false);

		if (wcsicmp(Seq, L"ALL") == NULL)
		{
			if (OutPut->Count != Total)
			{
				WriteLog(L"KF UCC.log", 2, "gettin out: OutPut->Count != ", std::to_string(Total));

				//dejo que llame a la función real y pise el valor (reset)
				return 0;
			}

			for (size_t i = 1; i <= Total; i++)
			{
				Data = GetINIItem(std::to_wstring(i), splitFields, File);
				len = wcslen(OutPut->FStringPtr[i - 1].Str);

				if (Data.size() > len)
				{
					WriteLog(L"KF UCC.log", 2, "gettin out: Data.size() > wcslen(OutPut->FStringPtr[i].Str) insufficient allocation size! at index: ", std::to_string(i));

					//dejo que llame a la función real y pise el valor (reset)
					return 0;
				}

				std::wstring OutWStr = Data + std::wstring(len - Data.size(), '|');

				WriteLog(L"KF UCC.log", 2, "OutWStr (in loop): ", OutWStr);

				wcscpy(OutPut->FStringPtr[i - 1].Str, OutWStr.c_str());
			}

			hRet = 1;
		}
		else
		{
			int num;

			if (OutPut->Count != 1)
			{
				WriteLog(L"KF UCC.log", 2, "gettin out: ", "OutPut->Count != 1 it must be 1");

				//dejo que llame a la función real y pise el valor (reset)
				return 0;
			}

			num = std::stoi(Seq);

			if (num > Total || num < 1)
			{
				WriteLog(L"KF UCC.log", 2, "gettin out: num > Total || num < 1: (num) ", Seq);

				//dejo que llame a la función real y pise el valor (reset)
				return 0;
			}

			//Data = GetINIInfo(L"INIT", L"Total", L"", File.c_str());
			Data = GetINIItem(std::to_wstring(num), splitFields, File);
			len = wcslen(OutPut->FStringPtr[0].Str);

			if (Data.size() > len)
			{
				WriteLog(L"KF UCC.log", 2, "gettin out: Data.size() > wcslen(OutPut->FStringPtr[0].Str) insufficient allocation size!", "");

				//dejo que llame a la función real y pise el valor (reset)
				return 0;
			}

			std::wstring OutWStr = Data + std::wstring(len - Data.size(), '|');

			WriteLog(L"KF UCC.log", 2, "OutWStr: ", OutWStr);

			wcscpy(OutPut->FStringPtr[0].Str, OutWStr.c_str());

			hRet = 1;
		}
	}

	WriteLog(L"KF UCC.log", 1, "", "<<<<<< ProcessInput out");
	WriteLog(L"KF UCC.log", 1, "", "----------------------------------");

	return hRet;
}

std::string GetExePath()
{
	using namespace std;

	char buffer[MAX_PATH] = { 0 };

	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	string::size_type pos = string(buffer).find_last_of("\\/");

	return string(buffer).substr(0, pos + 1);
}

std::wstring GetExePathW()
{
	using namespace std;

	wchar_t buffer[MAX_PATH] = { 0 };

	GetModuleFileName(NULL, buffer, MAX_PATH);

	string::size_type pos = wstring(buffer).find_last_of(L"\\/");

	return wstring(buffer).substr(0, pos + 1);
}

DWORD GetFunctionByModule(std::string DLL, std::string Function)
{
	HMODULE hModule = LoadLibraryA(DLL.c_str());

	if (hModule)
	{
		FARPROC ProcAddress = GetProcAddress(hModule, Function.c_str());

		if (ProcAddress)
		{
			return (DWORD)ProcAddress;
		}
	}

	return NULL;
}

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

std::string GetModuleFileNameByAddress(DWORD Address)
{
	using namespace std;

	char buffer[MAX_PATH] = { 0 };

	GetModuleFileNameA((HMODULE)Address, buffer, MAX_PATH);

	return string(buffer);
}

std::string GetFileExtension(std::string FileName)
{
	using namespace std;

	string::size_type pos = string(FileName).find_last_of("\\/");

	return string(FileName).substr(pos + 1);
}

std::wstring GetFileExtension(std::wstring FileName)
{
	using namespace std;

	wstring::size_type pos = wstring(FileName).find_last_of(L"\\/");

	return wstring(FileName).substr(pos + 1);
}

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems)
{
	using namespace std;

	stringstream ss(s);
	string item;

	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}

	return elems;
}

std::vector<std::string> split(const std::string& s, char delim)
{
	using namespace std;

	vector<string> elems;

	split(s, delim, elems);

	return elems;
}

std::vector<std::string> Split(const std::string& s, const std::string& delim, const bool keep_empty)
{
	using namespace std;

	vector<string> result;

	if (delim.empty())
	{
		result.push_back(s);

		return result;
	}

	string::const_iterator substart = s.begin(), subend;

	while (true)
	{
		subend = search(substart, s.end(), delim.begin(), delim.end());
		string temp(substart, subend);

		if (keep_empty || !temp.empty())
		{
			result.push_back(temp);
		}

		if (subend == s.end())
		{
			break;
		}

		substart = subend + delim.size();
	}

	return result;
}

std::vector<std::wstring> Split(const std::wstring& s, const std::wstring& delim, const bool keep_empty)
{
	using namespace std;

	vector<wstring> result;

	if (delim.empty())
	{
		result.push_back(s);

		return result;
	}

	wstring::const_iterator substart = s.begin(), subend;

	while (true)
	{
		subend = search(substart, s.end(), delim.begin(), delim.end());
		wstring temp(substart, subend);

		if (keep_empty || !temp.empty())
		{
			result.push_back(temp);
		}

		if (subend == s.end())
		{
			break;
		}

		substart = subend + delim.size();
	}

	return result;
}

BOOL bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
	{
		if (*szMask == 'x' && *pData != *bMask) //?
			return 0;
	}
	return (*szMask) == NULL;
}

/*
ex.
FindPattern(0x00400000, 0x00300000, (PBYTE)"\x3A\xC1\x1B\xC9", "xxxx");
if( bCompare( (BYTE*)( dwAddress+i ), bMask, szMask) )
*/

DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE* bMask, char* szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
	{
		if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))
		{
			return (DWORD)(dwAddress + i);
		}
	}

	return 0;
}

bool Check(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask != '?' && *pData != *bMask)
			return false;

	return (*szMask) == NULL;
}

DWORD FindPattern_Misery(BYTE* bMask, char* szMask, DWORD dwOffset, DWORD dwAddress, DWORD dwLen)
{
	__try
	{
		for (DWORD i = 0; i < dwLen; i++)
			if (Check((BYTE*)(dwAddress + i), bMask, szMask))
				return (DWORD)(dwAddress + i + dwOffset);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//MessageBox(NULL, "Find Pattern Error", "Error", MB_OK);
	}

	return 0;
}

DWORD Universal_FindPattern(BYTE* bMask, char* szMask, DWORD dwOffset)
{
	DWORD MaxSize = (DWORD)LoadLibrary(L"KERNEL32.DLL");
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	LPVOID lpStartAddress = NULL;
	SYSTEM_INFO sysInfo = { 0 };

	GetSystemInfo(&sysInfo);

	lpStartAddress = sysInfo.lpMinimumApplicationAddress;

	SIZE_T hRet;

	hRet = VirtualQuery(lpStartAddress, &mbi, sizeof(mbi));

	if (hRet == 0) return 0;

	do
	{																																			//TYPE == MEM_IMAGE W10 Patch
		if ((mbi.State & MEM_COMMIT)) // && (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) && (mbi.Type & MEM_IMAGE))
		{
			/*if (mbi.RegionSize == RegionSize)
			{
				VirtualProtect((LPVOID)mbi.BaseAddress, (DWORD)mbi.RegionSize, PAGE_EXECUTE_READWRITE, &dwOldProtectionStr);

				return (DWORD)mbi.BaseAddress;
			}*/

			if ((mbi.Type & MEM_IMAGE))
			{
				if ((mbi.AllocationProtect & PAGE_EXECUTE_WRITECOPY))
				{
					//std::cout << std::hex << IntToHexStr((DWORD)mbi.BaseAddress) << " (" << IntToHexStr((DWORD)mbi.RegionSize) << ")" << std::endl;

					/*std::cout << GetSectionNameByAddress((DWORD)mbi.BaseAddress).c_str() << std::endl;
					std::cout << Detour.ModuleBaseName.c_str() << std::endl << std::endl;*/

					for (DWORD i = 0; i < (DWORD)mbi.RegionSize; i++)
					{
						if (Check((BYTE*)((DWORD)mbi.BaseAddress + i), bMask, szMask))
						{
							//std::cout << "FOUND! => " << IntToHexStr((DWORD)mbi.BaseAddress + i + dwOffset) << std::endl;

							return (DWORD)((DWORD)mbi.BaseAddress + i + dwOffset);
						}
					}
				}
			}
		}
	_pass:

		lpStartAddress = (LPVOID)((DWORD_PTR)(lpStartAddress)+mbi.RegionSize);

		//W10 Patch?
		if ((DWORD)lpStartAddress >= MaxSize) return 0;

		/*if (pTrampolineVirtualQuery)
		{
			hRet = pTrampolineVirtualQuery(lpStartAddress, &mbi, sizeof(mbi));
		}
		else
		{
			hRet = VirtualQuery(lpStartAddress, &mbi, sizeof(mbi));
		}*/

		//2020-08-16
		hRet = VirtualQuery(lpStartAddress, &mbi, sizeof(mbi));
	} while (hRet);

	return 0;
}

DWORD Universal_EpiSpecial(DWORD ActualAddress, DWORD Size)
{
	using namespace std;

	/*
	C3 ret
	CC int3
	CC int3
	*/
	//C3 CC CC
	//xxx

	return FindPattern_Misery((PBYTE)"\xC3\xCC\xCC", const_cast<char*>("xxx"), 0x00, ActualAddress, Size);
}

std::string M_Ucase(std::string Cadena)
{
	std::transform(Cadena.begin(), Cadena.end(), Cadena.begin(), ::toupper);

	return Cadena;
}

std::wstring M_Ucase(std::wstring Cadena)
{
	std::transform(Cadena.begin(), Cadena.end(), Cadena.begin(), ::toupper);

	return Cadena;
}

bool M_InStr(std::string ZaBody, std::string ToFind, bool UPPER)
{
	using namespace std;

	//wstring str1 = L"TEST";
	string str1 = ZaBody;
	//wstring str2 = L"TEST WORLD";
	string str2 = ToFind; //for

	if (UPPER == true)
	{
		str1 = M_Ucase(ZaBody);
		str2 = M_Ucase(ToFind);
	}

	long lPtr;
	lPtr = str1.find(str2.c_str(), 0);
	//lPtr = str2.find(str1.c_str() , 0);

	if (lPtr >= 0)
	{
		return true;
	}

	return false;
}

bool M_Replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);

	if (start_pos == std::string::npos)
		return false;

	str.replace(start_pos, from.length(), to);

	return true;
}

std::string M_Split2(const std::string& s, const std::string& c, int index)
{
	using namespace std;

	string::size_type i = 0;
	string::size_type j = s.find(c);

	if (j == 4294967295)
	{
		return "<ERROR>";
	}

	vector<string> v;

	while (j != string::npos)
	{
		v.push_back(s.substr(i, j - i));

		i = ++j + c.length() - 1; //Sumamos el length de lo que hay que buscar, y le restamos 1
		j = s.find(c, j);
		if (j == string::npos)
		{
			v.push_back(s.substr(i, s.length()));
		}
	}

	return v[index];
}

bool FileExist(const char* filename)
{
	return GetFileAttributesA(filename) != 0xFFFFFFFF;
}

bool FileExist(const wchar_t* filename)
{
	return GetFileAttributes(filename) != 0xFFFFFFFF;
}

std::random_device rd;     // only used once to initialise (seed) engine

long RandomNumber(long LowerBound, long UpperBound)
{
	if (LowerBound < 0) LowerBound = 0;
	if (UpperBound < 0) UpperBound = 0;

	if (LowerBound > UpperBound) UpperBound = LowerBound;
	/*srand((unsigned)time(0));

	return (rand() % ((UpperBound - LowerBound) + 1) + LowerBound);*/

	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(LowerBound, UpperBound); // guaranteed unbiased

	return uni(rng);
}

std::string ByteToHexStr(int intnum)
{
	using namespace std;

	stringstream sstream;

	sstream << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << hex << intnum;

	//return toUpper(sstream.str());
	return sstream.str();
}

std::string BytesToHexStr(LPBYTE Add, int len)
{
	using namespace std;

	stringstream sstream;

	for (int i = 0; i < len; i++)
	{
		sstream << ByteToHexStr(*(Add + i)) << (i == (len - 1) ? "" : " ");
	}

	return sstream.str();
}

std::string IntToStr(int intnum)
{
	using namespace std;

	stringstream sstream;

	sstream << intnum;

	return sstream.str();
}

std::string IntToHexStr(int intnum)
{
	using namespace std;

	stringstream sstream;

	sstream << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << hex << intnum;

	//return toUpper(sstream.str());
	return sstream.str();
}

std::string WCharToStr(LPWSTR wChar)
{
	using namespace std;

	wstring ws(wChar);

	// your new String
	string str(ws.begin(), ws.end());

	return str;
}

std::string ConvertLPC_STRA(LPCSTR str, BOOL Quotes)
{
	using namespace std;

	if (str == NULL)
	{
		return "NULL";
	}

	if (!Quotes)
	{
		return std::string(str);
	}

	string tmp = "\"";
	tmp += str;
	tmp += "\"";

	return std::string(tmp);
}

std::string ConvertLPC_STRW(LPCWSTR str, BOOL Quotes)
{
	using namespace std;

	if (str == NULL)
	{
		return "NULL";
	}

	if (!Quotes)
	{
		return std::string(CW2A((LPCWSTR)str));
	}

	string tmp = "\"";
	tmp += CW2A((LPCWSTR)str);
	tmp += "\"";

	return std::string(tmp);
}

void MSG_Address(DWORD Address, std::string lpCaption)
{
	using namespace std;

	/*char szNumber2[16];
	itoa(Address, szNumber2, 10);
	MessageBox(NULL, szNumber2, "Address", MB_OK);*/

	stringstream ss;
	ss << uppercase << Address;
	MessageBoxA(0, ss.str().c_str(), lpCaption.c_str(), MB_OK);
}

void MSG_AddressHex(DWORD Address, std::string lpCaption)
{
	using namespace std;

	/*char szNumber2[16];
	itoa(Address, szNumber2, 10);
	MessageBox(NULL, szNumber2, "Address", MB_OK);*/

	stringstream ss;
	ss << uppercase << hex << Address;
	MessageBoxA(0, ss.str().c_str(), lpCaption.c_str(), MB_OK);
}

const wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];

	mbstowcs(wc, c, cSize);

	return wc;
}
DWORD dwOldProtectionByte;
void WriteInProcessBYTE(DWORD Address, BYTE Value)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	//2020-08-16
	if (VirtualQuery((LPVOID)Address, &mbi, sizeof(mbi)) == 0)
	{
		VirtualQueryEx(GetCurrentProcess(), (LPVOID)Address, &mbi, sizeof(mbi));
	}

	//*(BYTE*)Address = Value; //typecast variable to a pointer of a byte and dereference the pointer, letting you change what it holds
	//VirtualProtect((LPVOID)Address, 1, PAGE_EXECUTE_READWRITE, &dwOldProtectionByte);
	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &dwOldProtectionByte);

	*(BYTE*)Address = Value;

	//VirtualProtect((LPVOID)Address, 1, dwOldProtectionByte, &dwOldProtectionByte);
	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, dwOldProtectionByte, &dwOldProtectionByte);
}

BYTE ReadInProcessBYTE(DWORD Address)
{
	return *(BYTE*)Address;
}

DWORD dwOldProtectionWORD;
void WriteInProcessWORD(DWORD Address, WORD Value)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	//2020-08-16
	if (VirtualQuery((LPVOID)Address, &mbi, sizeof(mbi)) == 0)
	{
		VirtualQueryEx(GetCurrentProcess(), (LPVOID)Address, &mbi, sizeof(mbi));
	}

	//*(BYTE*)Address = Value; //typecast variable to a pointer of a byte and dereference the pointer, letting you change what it holds
	//VirtualProtect((LPVOID)Address, 2, PAGE_EXECUTE_READWRITE, &dwOldProtectionWORD);
	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &dwOldProtectionWORD);

	*(WORD*)Address = Value;

	//VirtualProtect((LPVOID)Address, 2, dwOldProtectionWORD, &dwOldProtectionWORD);
	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, dwOldProtectionWORD, &dwOldProtectionWORD);
}

WORD ReadInProcessWORD(DWORD Address)
{
	return *(WORD*)Address;
}

DWORD dwOldProtectionDWORD;
void WriteInProcessDWORD(DWORD Address, DWORD Value)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	//2020-08-16
	if (VirtualQuery((LPVOID)Address, &mbi, sizeof(mbi)) == 0)
	{
		VirtualQueryEx(GetCurrentProcess(), (LPVOID)Address, &mbi, sizeof(mbi));
	}

	//*(BYTE*)Address = Value; //typecast variable to a pointer of a byte and dereference the pointer, letting you change what it holds
	//VirtualProtect((LPVOID)Address, 4, PAGE_EXECUTE_READWRITE, &dwOldProtectionDWORD);
	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &dwOldProtectionDWORD);

	*(DWORD*)Address = Value;

	//VirtualProtect((LPVOID)Address, 4, dwOldProtectionDWORD, &dwOldProtectionDWORD);
	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, dwOldProtectionDWORD, &dwOldProtectionDWORD);
}

DWORD ReadInProcessDWORD(DWORD Address)
{
	return *(DWORD*)Address;
}

DWORD dwOldProtectionFLOAT;
void WriteInProcessFloat(DWORD Address, float Value)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	//2020-08-16
	if (VirtualQuery((LPVOID)Address, &mbi, sizeof(mbi)) == 0)
	{
		VirtualQueryEx(GetCurrentProcess(), (LPVOID)Address, &mbi, sizeof(mbi));
	}

	//VirtualProtect((LPVOID)Address, 4, PAGE_EXECUTE_READWRITE, &dwOldProtectionFLOAT);
	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &dwOldProtectionFLOAT);

	*(float*)Address = Value; //typecast variable to a pointer of a byte and dereference the pointer, letting you change what it holds

	//VirtualProtect((LPVOID)Address, 4, dwOldProtectionFLOAT, &dwOldProtectionFLOAT);
	VirtualProtect((LPVOID)mbi.BaseAddress, mbi.RegionSize, dwOldProtectionFLOAT, &dwOldProtectionFLOAT);
}

float ReadInProcessFloat(DWORD Address)
{
	return *(float*)Address;
}

/* 2021-03-18 11-02
Address + [Address + 1] + 5
*/
DWORD GetRealCALL(DWORD VarAddress)
{
	return VarAddress + ReadInProcessDWORD(VarAddress + 0x1) + 0x5;
}

std::string CharToStr(char* charstr)
{
	using namespace std;

	stringstream sstream;

	sstream << charstr;

	return sstream.str();
}

std::string CharToStr(char* charstr, int len)
{
	using namespace std;

	stringstream sstream;

	charstr[len] = 0;

	sstream << charstr;

	return sstream.str();
}

std::string StringToStr(std::string charstr)
{
	using namespace std;

	stringstream sstream;

	sstream << charstr;

	return sstream.str();
}

std::string WCharToStr(wchar_t* wcharstr, int len)
{
	using namespace std;

	stringstream sstream;

	wcharstr[len] = 0;

	sstream << wcharstr;

	return sstream.str();
}

std::string ConstCharToStr(const char* constchar)
{
	using namespace std;

	stringstream sstream;

	sstream << constchar;

	return sstream.str();
}

std::string toUpper(const std::string& s)
{
	using namespace std;

	std::string result;

	std::locale loc;
	for (unsigned int i = 0; i < s.length(); ++i)
	{
		result += std::toupper(s.at(i), loc);
	}

	return result;
}

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);

	std::wstring wstrTo(size_needed, 0);

	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);

	return wstrTo;
}

BYTE StrHex2BYTE(std::string Offset)
{
	BYTE hRet = 0;

	std::stringstream ss;

	ss << std::hex << Offset;

	ss >> hRet;

	return hRet;
}

DWORD StrHex2Int(std::string Offset)
{
	DWORD hRet = 0;

	std::stringstream ss;

	ss << std::hex << Offset;

	ss >> hRet;

	return hRet;
}

DWORD Str2DWORD(std::string Offset)
{
	DWORD hRet = 0;

	std::stringstream ss;

	ss << std::dec << Offset;

	ss >> hRet;

	return hRet;
}

LONG Str2LONG(std::string Offset)
{
	LONG hRet = 0;

	std::stringstream ss;

	ss << std::dec << Offset;

	ss >> hRet;

	return hRet;
}

int Str2Int(std::string Offset)
{
	int hRet = 0;

	std::stringstream ss;

	ss << std::dec << Offset;

	ss >> hRet;

	return hRet;
}

float Str2Float(std::string Offset)
{
	float hRet = 0;

	std::stringstream ss;

	ss << std::dec << Offset;

	ss >> hRet;

	return hRet;
}

BOOL Str2BOOL(std::string Offset)
{
	BOOL hRet;

	std::stringstream ss;

	ss << std::dec << Offset;

	ss >> hRet;

	return hRet;
}

template <typename T>
std::string to_hex(T data)
{
	std::ostringstream result;
	result << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << static_cast<int>(data);
	return result.str();
}

template <typename T>
std::string to_str(T data)
{
	std::ostringstream result;
	result << static_cast<int>(data);
	return result.str();
}

// trim from start (in place)
void ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
void rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string& s)
{
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
std::string ltrimmed(std::string s)
{
	ltrim(s);
	return s;
}

// trim from end (copying)
std::string rtrimmed(std::string s)
{
	rtrim(s);
	return s;
}

// trim from both ends (copying)
std::string trimmed(std::string s)
{
	trim(s);
	return s;
}

std::string return_current_time_and_date()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S: ");

	return ss.str();
}

LARGE_INTEGER Frequency;
BOOL CheckTime(LARGE_INTEGER& Tick, double Interval, BOOL SaveTick)
{
	if (Frequency.QuadPart == 0)
	{
		QueryPerformanceFrequency(&Frequency);
	}

	LARGE_INTEGER StartTime;

	QueryPerformanceCounter(&StartTime);

	LONGLONG llTimeDiff = (StartTime.QuadPart - Tick.QuadPart) / (double(Frequency.QuadPart) / 1000.0);

	//cout << "CheckTime " << dec << llTimeDiff << endl;

	if (llTimeDiff >= Interval)
	{
		if (SaveTick) QueryPerformanceCounter(&Tick);

		return true;
	}

	return false;
}

BOOL CheckTimeSaveInterval(LARGE_INTEGER& Tick, DWORD& Interval, BOOL SaveTick)
{
	if (Frequency.QuadPart == 0)
	{
		QueryPerformanceFrequency(&Frequency);
	}

	LARGE_INTEGER StartTime;

	QueryPerformanceCounter(&StartTime);

	LONGLONG llTimeDiff = (StartTime.QuadPart - Tick.QuadPart) / (double(Frequency.QuadPart) / 1000.0);

	//cout << "CheckTime " << dec << llTimeDiff << endl;

	if (llTimeDiff < (LONGLONG)Interval)
	{
		Interval = (DWORD)llTimeDiff;

		if (SaveTick) QueryPerformanceCounter(&Tick);

		return true;
	}

	return false;
}

BOOL CheckTimeManualInterval(LARGE_INTEGER& Tick, DWORD& Interval)
{
	BOOL hRet = false;

	if (Frequency.QuadPart == 0)
	{
		QueryPerformanceFrequency(&Frequency);
	}

	LARGE_INTEGER StartTime;

	QueryPerformanceCounter(&StartTime);

	LONGLONG llTimeDiff = (StartTime.QuadPart - Tick.QuadPart) / (double(Frequency.QuadPart) / 1000.0);

	//cout << "CheckTimeManualInterval " << dec << llTimeDiff << endl;

	if (llTimeDiff < (LONGLONG)Interval)
	{
		Interval = (DWORD)llTimeDiff;

		hRet = true;
	}

	QueryPerformanceCounter(&Tick);

	return hRet;
}

void UpdateTime(LARGE_INTEGER& Tick)
{
	if (Frequency.QuadPart == 0)
	{
		QueryPerformanceFrequency(&Frequency);
	}

	QueryPerformanceCounter(&Tick);
}

LONGLONG GetElapsedTime(LARGE_INTEGER Tick)
{
	if (Frequency.QuadPart == 0)
	{
		QueryPerformanceFrequency(&Frequency);
	}

	LARGE_INTEGER StartTime;

	QueryPerformanceCounter(&StartTime);

	return (StartTime.QuadPart - Tick.QuadPart) / (double(Frequency.QuadPart) / 1000.0);
}

BOOL CheckTimeInternal(LARGE_INTEGER& Tick, DWORD& Interval, DWORD MinInt, DWORD MaxInt, BOOL SaveTick)
{
	if (Frequency.QuadPart == 0)
	{
		QueryPerformanceFrequency(&Frequency);
	}

	LARGE_INTEGER StartTime;

	QueryPerformanceCounter(&StartTime);

	LONGLONG llTimeDiff = (StartTime.QuadPart - Tick.QuadPart) / (double(Frequency.QuadPart) / 1000.0);

	if (llTimeDiff >= Interval)
	{
		if (SaveTick) QueryPerformanceCounter(&Tick);
		Interval = RandomNumber(MinInt, MaxInt);

		return true;
	}

	return false;
}

void AddTime_QueryPerf(LARGE_INTEGER& Tick, double Interval)
{
	Tick.QuadPart += (Interval * (double(Frequency.QuadPart) / 1000.0));
}

DWORD GetBaseAddress()
{
	TCHAR szProcessName[MAX_PATH] = { 0 }; // TEXT("<unknown>");
	HMODULE hMod;
	DWORD cbNeeded;

	if (EnumProcessModulesEx(GetCurrentProcess(), &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_32BIT | LIST_MODULES_64BIT))
	{
		GetModuleBaseName(GetCurrentProcess(), hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));

		if (hMod) return (DWORD)hMod;
	}

	return 0;
}

std::string ConvertBSTRToMBS(BSTR bstr)
{
	int wslen = SysStringLen(bstr);

	return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}

std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
	int len = WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

	std::string dblstr(len, '\0');

	len = WideCharToMultiByte(CP_ACP, 0 /* no flags */,
		pstr, wslen /* not necessary NULL-terminated */,
		&dblstr[0], len,
		NULL, NULL /* no default char */);

	return dblstr;
}

BSTR ConvertMBSToBSTR(const std::string& str)
{
	int wslen = MultiByteToWideChar(CP_ACP, 0 /* no flags */,
		str.data(), str.length(),
		NULL, 0);

	BSTR wsdata = SysAllocStringLen(NULL, wslen);

	MultiByteToWideChar(CP_ACP, 0 /* no flags */,
		str.data(), str.length(),
		wsdata, wslen);

	return wsdata;
}

int GetPercent(int From, int Max)
{
	if (Max == 0) return 0;

	if (From > Max) From = Max;

	return From * 100 / Max;
}

void WriteLog(std::wstring File, int tabs, std::string body, std::string Text)
{
	if (!Debug) return;

	std::wstring Path = DirPath + File;
	std::string Tab;

	for (size_t i = 0; i < tabs; i++)
	{
		Tab += "\t";
	}

	std::ofstream ofs(Path.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << return_current_time_and_date() << Tab << body << Text << '\n';
	ofs.close();
}

void WriteLog(std::wstring File, int tabs, std::wstring body, std::string Text)
{
	if (!Debug) return;

	std::wstring Path = DirPath + File;
	std::string Tab;
	std::string Body2(body.begin(), body.end());

	for (size_t i = 0; i < tabs; i++)
	{
		Tab += "\t";
	}

	std::ofstream ofs(Path.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << return_current_time_and_date() << Tab << Body2 << Text << '\n';
	ofs.close();
}

void WriteLog(std::wstring File, int tabs, std::string body, std::wstring Text)
{
	if (!Debug) return;

	std::wstring Path = DirPath + File;
	std::string Tab;
	std::string Text2(Text.begin(), Text.end());

	for (size_t i = 0; i < tabs; i++)
	{
		Tab += "\t";
	}

	std::ofstream ofs(Path.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << return_current_time_and_date() << Tab << body << Text2 << '\n';
	ofs.close();
}

void WriteLog(std::wstring File, int tabs, std::string body, wchar_t* Text)
{
	if (!Debug) return;

	std::wstring Text2(Text);

	WriteLog(File, tabs, body, Text2);
}

void WriteLog(std::wstring File, int tabs, std::string body, char* Text)
{
	if (!Debug) return;

	std::string Text2(Text);

	WriteLog(File, tabs, body, Text2);
}