#include <sourcemod>
#include <sdktools>
#pragma semicolon 1

public Plugin:myinfo = 
{
	name = "AutoMapCFG_loader",
	author = "CreativeFrags",
	description = "Automaticly Loads Mapspecific Config Files in your 'cstrike/cfg' folder",
	version = "0.1",
	url = "<- URL ->"
}

public OnConfigsExecuted()
{
	decl String:mapname[64];
	decl String:maptype[64];
	decl String:sFile[255];
	GetCurrentMap(mapname,sizeof(mapname));
	
	SplitString(mapname,"_",maptype,sizeof(mapname));
	
	Format(sFile, sizeof(sFile), "cfg/%s.cfg", maptype);

	if(FileExists(sFile)==true)
	{
		ServerCommand("exec %s",maptype);
	}
}