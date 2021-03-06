...
  
function AllocList(out array<string> List, string TmpField)
{
	local int i;
	
	MutLog("----->>> AllocList <<<-----");
	
	for (i = 0; i < List.Length; i++)
	{
		List[i] = TmpField;
	}
}

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
...
