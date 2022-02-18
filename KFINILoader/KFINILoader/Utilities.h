#pragma once

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include <iostream>
#include <windows.h>
#include <vector>
#include <sstream> //stringstream

#include <algorithm> //std::transform, search

#include <random> //randomnumber

#include <iomanip> //std::setfill, std::setw, put_time y más

#include <atlstr.h> //CW2A

#include <locale> //std::toupper, std::locale

#include <functional> //std::not1, std::ptr_fun
#include <cctype> //std::ptr_fun, std::isspace

#include <chrono>  // chrono::system_clock

#include <psapi.h>  //GetModuleBaseName, EnumProcessModulesEx
#pragma comment(lib, "psapi.lib") //GetModuleBaseName, EnumProcessModulesEx

#include <fstream>

//Detour 1.5
#include "detours.h"
#pragma comment ( lib, "detours.lib" )

struct FStringSub
{
	wchar_t* Str;
	DWORD Arg0; //Len + 1?
	DWORD Arg1; //Len + 1?
};

struct FString
{
	FStringSub * FStringPtr;
	DWORD Count;
	DWORD Unknown1;
};

struct tParam
{
	FString OutPut;
	FStringSub Param1;
	FStringSub Param2;
};

extern DWORD IDThread;
extern HANDLE hThread;
extern std::wstring DirPath;
extern BOOL Debug;

extern std::wstring FullPath;
extern std::wstring MainINI;
extern std::wstring WriteLogFile;

extern std::vector<std::wstring> LoadedINIs;

BOOL EnableDebugPrivileges();
DWORD GetFunctionByModule(std::string DLL, std::string Function);

//Detours
typedef void(__stdcall* MyGenericVoidNOPARAM) ();

extern DWORD GetPerObjectNames_EAX;
extern DWORD GetPerObjectNames_EBX;
extern DWORD GetPerObjectNames_ECX;
extern DWORD GetPerObjectNames_EDX;
extern DWORD GetPerObjectNames_ESI;
extern DWORD GetPerObjectNames_EDI;
extern DWORD GetPerObjectNames_EBP;
extern DWORD GetPerObjectNames_ESP;
extern MyGenericVoidNOPARAM pTrampolineGetPerObjectNames;
extern void __stdcall DetourGetPerObjectNames();

extern MyGenericVoidNOPARAM pTrampolineGetPerObjectNames_Sub1;
extern void __stdcall DetourGetPerObjectNames_Sub1();
extern DWORD Core_GConfigOrWhatEver;
extern BYTE Core_GConfigOrWhatEver_Offset;

extern MyGenericVoidNOPARAM pTrampolineGetPerObjectNames_Sub2;
extern void __stdcall DetourGetPerObjectNames_Sub2();

extern MyGenericVoidNOPARAM pTrampolineGetPerObjectNames_Sub3;
extern void __stdcall DetourGetPerObjectNames_Sub3();
//

//Utilities Functions
std::string GetExePath();
std::wstring GetExePathW();
DWORD GetFunctionByModule(std::string DLL, std::string Function);
BOOL EnableDebugPrivileges();
std::string GetModuleFileNameByAddress(DWORD Address);
std::string GetFileExtension(std::string FileName);
std::wstring GetFileExtension(std::wstring FileName);
std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);
std::vector<std::string> split(const std::string& s, char delim);
std::vector<std::string> Split(const std::string& s, const std::string& delim, const bool keep_empty);
std::vector<std::wstring> Split(const std::wstring& s, const std::wstring& delim, const bool keep_empty);
BOOL bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);
DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE* bMask, char* szMask);
bool Check(const BYTE* pData, const BYTE* bMask, const char* szMask);
DWORD FindPattern_Misery(BYTE* bMask, char* szMask, DWORD dwOffset, DWORD dwAddress, DWORD dwLen);
DWORD Universal_FindPattern(BYTE* bMask, char* szMask, DWORD dwOffset);
DWORD Universal_EpiSpecial(DWORD ActualAddress, DWORD Size);
std::string M_Ucase(std::string Cadena);
std::wstring M_Ucase(std::wstring Cadena);
bool M_InStr(std::string ZaBody, std::string ToFind, bool UPPER);
bool M_Replace(std::string& str, const std::string& from, const std::string& to);
std::string M_Split2(const std::string& s, const std::string& c, int index);
bool FileExist(const char* filename);
bool FileExist(const wchar_t* filename);
long RandomNumber(long LowerBound, long UpperBound);
std::string ByteToHexStr(int intnum);
std::string BytesToHexStr(LPBYTE Add, int len);
std::string IntToStr(int intnum);
std::string IntToHexStr(int intnum);
std::string WCharToStr(LPWSTR wChar);
std::string ConvertLPC_STRA(LPCSTR str, BOOL Quotes);
std::string ConvertLPC_STRW(LPCWSTR str, BOOL Quotes);
void MSG_Address(DWORD Address, std::string lpCaption);
void MSG_AddressHex(DWORD Address, std::string lpCaption);
const wchar_t* GetWC(const char* c);
void WriteInProcessBYTE(DWORD Address, BYTE Value);
BYTE ReadInProcessBYTE(DWORD Address);
void WriteInProcessWORD(DWORD Address, WORD Value);
WORD ReadInProcessWORD(DWORD Address);
void WriteInProcessDWORD(DWORD Address, DWORD Value);
DWORD ReadInProcessDWORD(DWORD Address);
void WriteInProcessFloat(DWORD Address, float Value);
float ReadInProcessFloat(DWORD Address);
DWORD GetRealCALL(DWORD VarAddress);
std::string CharToStr(char* charstr);
std::string CharToStr(char* charstr, int len);
std::string StringToStr(std::string charstr);
std::string WCharToStr(wchar_t* wcharstr, int len);
std::string ConstCharToStr(const char* constchar);
std::string toUpper(const std::string& s);
std::wstring s2ws(const std::string& str);
BYTE StrHex2BYTE(std::string Offset);
DWORD StrHex2Int(std::string Offset);
DWORD Str2DWORD(std::string Offset);
LONG Str2LONG(std::string Offset);
int Str2Int(std::string Offset);
float Str2Float(std::string Offset);
BOOL Str2BOOL(std::string Offset);
void ltrim(std::string& s);
void rtrim(std::string& s);
void trim(std::string& s);
std::string ltrimmed(std::string s);
std::string rtrimmed(std::string s);
std::string trimmed(std::string s);
std::string return_current_time_and_date();
extern LARGE_INTEGER Frequency;
BOOL CheckTime(LARGE_INTEGER& Tick, double Interval, BOOL SaveTick);
BOOL CheckTimeSaveInterval(LARGE_INTEGER& Tick, DWORD& Interval, BOOL SaveTick);
BOOL CheckTimeManualInterval(LARGE_INTEGER& Tick, DWORD& Interval);
void UpdateTime(LARGE_INTEGER& Tick);
LONGLONG GetElapsedTime(LARGE_INTEGER Tick);
BOOL CheckTimeInternal(LARGE_INTEGER& Tick, DWORD& Interval, DWORD MinInt, DWORD MaxInt, BOOL SaveTick);
void AddTime_QueryPerf(LARGE_INTEGER& Tick, double Interval);
DWORD GetBaseAddress();
std::string ConvertBSTRToMBS(BSTR bstr);
std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);
BSTR ConvertMBSToBSTR(const std::string& str);
int GetPercent(int From, int Max);
void WriteLog(std::wstring File, size_t tabs, std::string body, std::string Text);
void WriteLog(std::wstring File, size_t tabs, std::wstring body, std::string Text);
void WriteLog(std::wstring File, size_t tabs, std::string body, std::wstring Text);
void WriteLog(std::wstring File, size_t tabs, std::string body, wchar_t* Text);
void WriteLog(std::wstring File, size_t tabs, std::string body, char* Text);
bool AvailableAddress(LPVOID lpAddress);

//Functions
void LoadDebug();
int GetMainINICount();
int GetMainIndexChanged(int Index);
int SetMainIndexChanged(int Index, std::wstring Value);
size_t GetINICount(std::wstring File);
std::wstring GetINIRetrieveFields(std::wstring File);
std::wstring GetINIInfo(std::wstring lpAppName, std::wstring lpKeyName, std::wstring lpDefault, std::wstring File);
BOOL HasItChanged(wchar_t* INIToFind, int index);
BOOL IsInINIList(wchar_t * INIToFind, wchar_t* Seq);
DWORD ProcessInput(wchar_t * INIFile, wchar_t * Seq, FString* OutPut);