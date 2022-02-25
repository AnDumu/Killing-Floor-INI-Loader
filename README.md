# Killing-Floor-INI-Loader
Load INI in Run-Time

Well, raw explanation, I don't have much time they are following me (???), this is for KF Server, when you run a dedicated server, you cannot load ini files in **run-time**, I patched a function named **GetPerObjectNames** to achieve this. (don't worry, you can use that function normally)

You have 2 projects, the injector and the DLL.
Both must be located in KF/System/ (**KFInjector2.exe** and **KFINILoader.dll**)
Also, you will need an **injector bat** file, that will inject the dll in UCC.exe **(also it works in a sandbox environment)**

https://github.com/AnDumu/Killing-Floor-INI-Loader/blob/main/System/KF_Server_Launcher.bat

```
start "Injector" cmd /c KFInjector.bat

ucc.exe server KF-westlondon.rom?game=KFmod.KFGameType?MaxPlayers=23?Mutator=MutLoader.MutLoader -log=server.log
```

https://github.com/AnDumu/Killing-Floor-INI-Loader/blob/main/System/KFInjector.bat
```
@echo off
timeout 3
KFInjector2 "ucc.exe" "KFINILoader.dll"
pause
```

All INIs must be located within **M_CFG.ini** or the name you give in the project.

https://github.com/AnDumu/Killing-Floor-INI-Loader/blob/main/System/M_CFG.ini

```
[INIT]
Total=1
Debug=1

[1]
INI=AreYouVipList.ini
Changed=0
```

**Debug=1** will result in an output file in **/System/**, like **KF UCC.log** or whatever you configured... (std::wstring WriteLogFile = L"KF UCC.log";)</br>

https://github.com/AnDumu/Killing-Floor-INI-Loader/blob/main/System/AreYouVipList.ini

```
[INIT]
Total=4
RetrieveFields=SteamID|Level|IsStillVIP

[1]
SteamID=76561198066780196
Level=1
IsStillVIP=0
TimesBought=1
Bought=2022/02/13
Expiraton=2022/03/14

[2]
SteamID=76561198066780195
Level=2
IsStillVIP=1
TimesBought=1
Bought=2022/02/13
Expiraton=2022/03/14

[3]
SteamID=76561198066780194
Level=3
IsStillVIP=1
TimesBought=1
Bought=2022/02/13
Expiraton=2022/03/14

[4]
SteamID=76561198066780193
Level=2
IsStillVIP=0
TimesBought=1
Bought=2022/02/13
Expiraton=2022/03/14
```

The **RetrieveFields** are the fiels that it's gonna return when processing.

And it can be called like:<br/>
**Total** to get the amount of items in the ini file.<br/>
By index using a number, **1**, **2**, **3**, etc.<br/>
**All** to get all items, the list cannot skip items (you need to program it if you want), so you can add a flag to do not processing when scripting in Unreal.

A working example in scripting:
https://github.com/AnDumu/Killing-Floor-INI-Loader/blob/main/UnrealScript/AreYouVIP.uc

**These functions are** 🔸mandatory🔸 (you can delete the MutLog function)<br/>
```
function AllocList(out array<string> List, string TmpField)
{
	local int i;
	
	MutLog("----->>> AllocList <<<-----");
	
	for (i = 0; i < List.Length; i++)
	{
		List[i] = TmpField;
	}
}

function GetINIInfo(out array<string> List, string INI, string KeyName)
{
	List = GetPerObjectNames(INI, KeyName, 9999999);
}

function int GetINITotal(out array<string> List, string INI)
{
	local string KeyName;
	
	KeyName = "Total";
	
	GetINIInfo(List, INI, Keyname);
	
	return int(List[0]);
}
```

**How you use it**:<br/>
```
function PrintList(out array<string> List)
{
	local int i;
	
	MutLog("----->>> PrintList <<<-----");
	
	for (i = 0; i < List.Length; i++)
	{
		MutLog("             List[" $ i $ "]: " $ List[i]);
	}
	
	MutLog("--------------------------");
}

function Testa()
{
	local array<string> List;
	local string INI;
	local string KeyName;
	local int Count;
	local string TmpField;
	
	//this one could be anything, just need 22 chars or more, the DLL will fill it with |
	TmpField = "AAAAAAAAAAAAAAAAAAAAAA"; //SteamID + 1 char (just in case for SteamID it could get bigger) + Level + IsStillVIP 
	
	MutLog("----->>> B4 List = GetPerObjectNames();  <<<-----");
	
	INI = "AreYouVipList.ini";
	
	//Get Total
	List.Length = 1;
	List[0] = "0000000000";
	Count = GetINITotal(List, INI);
	MutLog("----->>> GetINITotal(AreYouVipList.ini): " $ Count $ " <<<-----");
	List.Length = 0; //Reset (just in case)
	// -------------------------------------------------
	
	//Get only 1 by index (need to test 1 out of bounds)
	/*KeyName = "5";
	List.Length = 1;
	List[0] = "AAAAAAAAAAAAAAAAAAAAAA";
	GetINIInfo(List, INI, KeyName);
	
	if (List.Length > 0)
	{
		MutLog("----->>> GetINIIndex(AreYouVipList.ini, 1): " $ List[0] $ " <<<-----");
		
		List.Length = 0; //Reset
	}*/
	// -------------------------------------------------
	
	//Get All
	List.Length = Count;
	AllocList(List, TmpField);
	
	KeyName = "All";
	
	GetINIInfo(List, INI, KeyName);
	
	MutLog("----->>> GetINIInfo(AreYouVipList.ini, All): " $ List.Length $ " <<<-----");
	
	PrintList(List);
	List.Length = 0; //Reset (just in case)
	// -------------------------------------------------
}

simulated function MutLog(string s)
{
	log(s, 'AreYouVIP');
}
```

**Result**:<br/>
![Alt text](https://github.com/AnDumu/Killing-Floor-INI-Loader/blob/main/KF%20INI%20Loader.png "")

![Alt text](https://github.com/AnDumu/Killing-Floor-INI-Loader/blob/main/krusty-professional.gif "")

**Extra**:
You can add more tags like **AllWays** beyond **All** and **1,2,3,..** (numbers) or whatever you want to avoid **the Changed flag**, the code could be upgraded when processing data, but not everyone is gonna touch it or know anything about programming, so I did split it for a better understanding.

Logic, for this example, you add a new VIP one, you add it in **AreYouVipList.ini**, then you go to **M_CFG.ini** and you change **Changed flag** to **1**, and BOOM, it is gonna be reloaded, remmeber to use the above unrealscript that calls that function in a good way to not crash or to avoid unexpected situations.
