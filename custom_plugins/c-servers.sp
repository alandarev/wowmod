#include <sourcemod>
#include <steamtools>

#define COLOR_DEFAULT 0x01
#define COLOR_LIGHTGREEN 0x03
#define COLOR_GREEN 0x04

new g_cId=0;
new String:g_cKey[50]="KEY";
new String:g_cGroups[10][30];
new g_cGroupsAmount;
#include "cs/mysql"
#include "cs/chat"
#include "cs/Report"




new Handle:g_hId=INVALID_HANDLE;
new Handle:g_hKey=INVALID_HANDLE;

public Plugin:myinfo = {
  name = "c-servers",
  author = "stinkyfax",
  description = "c-servers plugin integrating c-servers.com to this gameserver",
  version = "1.1",
  url = "http://c-servers.com"
};


public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
  PrintToServer("[c-servers] Starting up");
  _sqlInit();

  return APLRes_Success;
}

public OnPluginStart()  {

  g_hId   = CreateConVar("cservers_id", "0", "Server ID in c-servers.com");
  g_hKey  = CreateConVar("cservers_key", "KEY", "Server Key in c-servers.com",FCVAR_PROTECTED);
  HookConVarChange(g_hId, cvarChanged);
  HookConVarChange(g_hKey, cvarChanged);

  RegAdminCmd("cservers_restart",AdminCmd_Reboot,ADMFLAG_CUSTOM5);
  AutoExecConfig(true, "c-servers");
  initTimers();

  /*RegConsoleCmd("say",SayCommand);*/
  HookEvent("player_say", SayCommand);
  
  //CreativeFrags
  LoadReport();
}

initTimers()  {
  CreateTimer(5.0, TimerChat, 0, TIMER_REPEAT);
}

public cvarChanged(Handle:cvar, String:old_val[], String:new_val[]) {
  if(cvar == g_hId) g_cId = StringToInt(new_val);
  if(cvar == g_hKey) Format(g_cKey, sizeof(g_cKey), "%s", new_val);
}


public Action:AdminCmd_Reboot(client,argc)
{
  RequireRestart(g_cId, g_cKey);
  PrintToChatAll("[c-servers] Server is about to go for restart");
  LogMessage("[c-servers] Admin requested restart");

  return Plugin_Handled;
}


public OnConfigsExecuted()  {
  g_cId = GetConVarInt(g_hId);
  GetConVarString(g_hKey, g_cKey, sizeof(g_cKey));
  if(g_cId <= 0 || StrEqual("KEY", g_cKey, false))  {
    SetFailState("[c-servers] Error: cservers_id or cservers_key are not set, try editing c-servers.cfg");
  }
  //Get sql configs
  sqlLoadConfig();
}



public Action:Steam_RestartRequested()
{
  LogMessage("[c-servers] Master Server requested restart");
  PrintToChatAll("[c-servers] Sever is about to restart for Steam Update!");
  PrintToChatAll("[c-servers] Sever is about to restart for Steam Update!");
  PrintToChatAll("[c-servers] Sever is about to restart for Steam Update!");
  RequireRestart(g_cId, g_cKey);

  return Plugin_Continue;
}



