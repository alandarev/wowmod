/*
* File: configuration.inc
* Description: Configuration functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1

new Handle:config;

public bool:LoadConfig()
{
  ServerCommand("alias wcreset \"sm plugins unload wow;changelevel de_dust2\"");
  if(config!=INVALID_HANDLE)
    CloseHandle(config);
  config=CreateKeyValues("WCConfig");
  decl String:path[PLATFORM_MAX_PATH];
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,"configs/wow.ini");
  if(!FileToKeyValues(config,path))
    SetFailState("[WC] Error: Failed to load WC configuration, aborting");
}

public ReloadConfig()
{
  if(config!=INVALID_HANDLE)
    CloseHandle(config);
  config=CreateKeyValues("WCConfig");
  decl String:path[PLATFORM_MAX_PATH];
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,"configs/wow.ini");
  if(!FileToKeyValues(config,path))
    SetFailState("[WC] Error: Failed to load WC configuration, aborting");
  UpdateStringConfig();
}

UpdateStringConfig() {
   FindSettingString("general","TEAMBALANCE_COMMAND",TEAMBALANCE_COMMAND,sizeof(TEAMBALANCE_COMMAND));
   FindSettingString("general","FUN_MESSAGE",FUN_MESSAGE,sizeof(FUN_MESSAGE));
   FindSettingString("general","RANKING_PAGE",RANKING_PAGE,sizeof(RANKING_PAGE));
   FindSettingString("general","ACCOUNT_PAGE",ACCOUNT_PAGE,sizeof(ACCOUNT_PAGE));
   FindSettingString("general","WIKI_PAGE",WIKI_PAGE,sizeof(WIKI_PAGE));
   FindSettingString("pvp","MAIN_SERVER_ADRESS",MAIN_SERVER_ADRESS,sizeof(MAIN_SERVER_ADRESS));
   FindSettingString("pvp","ARENA_SERVER",ARENA_SERVER,sizeof(ARENA_SERVER));
   FindSettingString("pvp","ARENA_FORCE_MAP",ARENA_FORCE_MAP,sizeof(ARENA_FORCE_MAP));
   FindSettingString("general","COMM_NICK",COMM_NICK,sizeof(COMM_NICK));
   FindSettingString("general","SERVER_LEVEL_POLICY_REDIRECT", SERVER_LEVEL_POLICY_REDIRECT, sizeof(SERVER_LEVEL_POLICY_REDIRECT));

}

public bool:FindSettingBool(const String:category[],const String:name[])
{
  KvRewind(config);
  KvJumpToKey(config,category);
  return bool:KvGetNum(config,name);
}

public FindSettingInt(const String:category[],const String:name[])
{
  KvRewind(config);
  KvJumpToKey(config,category);
  return KvGetNum(config,name);
}

public Float:FindSettingFloat(const String:category[],const String:name[])
{
  KvRewind(config);
  KvJumpToKey(config,category);
  return KvGetFloat(config,name);
}

public FindSettingString(const String:category[],const String:name[],String:output[],maxlen)
{
  KvRewind(config);
  KvJumpToKey(config,category);
  KvGetString(config,name,output,maxlen);
}

public FindMySQLSetting(const String:name[],String:output[],maxlen)
{
  KvRewind(config);
  KvJumpToKey(config,"mysql");
  KvGetString(config,name,output,maxlen);
}

CheckArenaMap()   {
  //Arena check
  if(ARENA_ENABLED)  {
      decl String:sMap[20];
      GetCurrentMap(sMap,sizeof(sMap));
      new count=StrContains(sMap,"ba_",false);
      if (count>=0)  {
         bArenaActive=true;
         LogMessage("Arena functions are activated");
      }
      else  {
         if(!bArenaActive) {
            if(StrContains(ARENA_FORCE_MAP,"ba_",false) >= 0)   {
               decl String:format[255];
               Format(format,sizeof(format), "Map is changed to %s because of ARENA_FORCE_MAP settings", ARENA_FORCE_MAP);
               LogMessage("[WC]: CHANGING MAP to arena map because of ARENA_FORCE_MAP");
               ForceChangeLevel(ARENA_FORCE_MAP, format);
            }
         }
         if(bArenaActive)  LogMessage("Arena functions are deactivated");
         bArenaActive=false;
      }
  }
}
