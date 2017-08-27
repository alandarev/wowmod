/*
* File: tempSettings.inc
* Description: Temporary player/server settings
* Do not misunderstand with temporary files/code - this one isn't!
* Author(s): Stinkyfax
*/

#pragma semicolon 1

new bool:g_bNewbie[65];
new g_iNewbie[65];
new g_iNewbie2[65];
new g_iUsekeySpellbar[65];


ResetNewbie(client) {
  g_bNewbie[client]=true;
  g_iNewbie2[client]=1;
  g_iUsekeySpellbar[client]=1;
}

public Action:NewbieTimer(Handle:timer, any:id)
{
  new pass=0;
  for(new i=1; i<=GetMaxClients(); i++)
    if(playerids[i]==id)
      pass=i;
  if(pass<=0)
    return Plugin_Stop;
  if(!IsClientInGame(pass))
    return Plugin_Continue;

  NewbieMenu(pass);
  return Plugin_Stop;
}


new Slide:g_ff_prerunSlide[65];
public Action:SlideTimer(Handle:timer, any:id)
{
  new pass=0;
  for(new i=1; i<=GetMaxClients(); i++)
    if(playerids[i]==id)
      pass=i;
  if(pass<=0)
    return Plugin_Stop;
  if(!IsClientInGame(pass) || IsPlayerAlive(pass))
    return Plugin_Continue;

  PrerunSlide(pass, g_ff_prerunSlide[pass]);
  return Plugin_Stop;
}

PrerunSlide(client, Slide:type)  {
  g_ff_prerunSlide[client] = type;

  if(!IsClientInGame(client) || IsPlayerAlive(client))  {
    CreateTimer(2.0, SlideTimer, playerids[client], TIMER_REPEAT);
    return;
  }

  DisplaySlides(client, g_ff_prerunSlide[client]);

}

NewbieMenu(client) {
   if(!IsClientInGame(client))  {
      CreateTimer(10.0, NewbieTimer, playerids[client], TIMER_REPEAT);
      return;
    }
   if(!ShouldDisplayNewbie(client))
      return;
   g_bNewbie[client]=false;
   new Handle:panel = CreatePanel();
   decl String:temp[550];
   Format(temp, sizeof(temp), "%T", "panel newbie title", client);
   SetPanelTitle(panel, temp);
   Format(temp, sizeof(temp), "%T", "panel newbie text", client);
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   DrawPanelText(panel, temp);/*
   Format(temp,sizeof(temp),"  %s",name);
   DrawPanelItem(panel, name, ITEMDRAW_DISABLED);*/
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   Format(temp, sizeof(temp), "%T", "exit", client);
   SetPanelCurrentKey(panel, 2);
   DrawPanelItem(panel, temp, ITEMDRAW_CONTROL);
   SendPanelToClient(panel, client, AnnounceHandler, MENU_TIME_FOREVER);

   CloseHandle(panel);
}

bool:ShouldDisplayNewbie(client)  {
  if(playerloaded[client]!=1)
    return false;
  if(g_iNewbie[client]!=1)
    return false;
  return g_bNewbie[client];
}

GetNewbie(client) {
  return g_iNewbie[client];
}

SetNewbie(client, value)  {
  g_iNewbie[client]=value;
}

bool:TriggerNewbie(client)  {
  if(g_iNewbie[client]==1)  {
    g_iNewbie[client]=0;
    return false;
  }
  g_iNewbie[client]=1;
  return true;
}

GetUsekeySpellbar(client)
{
  return g_iUsekeySpellbar[client];
}

SetUsekeySpellbar(client, value)
{
  g_iUsekeySpellbar[client]=value;
}

bool:UsekeySpellbar2(client)
{
  if(g_iUsekeySpellbar[client]==1)
  {
    g_iUsekeySpellbar[client]=0;
    return false;
  }
  g_iUsekeySpellbar[client]=1;
  return true;
}

GetNewbie2(client) {
  return g_iNewbie2[client];
}

SetNewbie2(client, value)  {
  g_iNewbie2[client]=value;
}

bool:TriggerNewbie2(client)  {
  if(g_iNewbie2[client]==1)  {
    g_iNewbie2[client]=0;
    return false;
  }
  g_iNewbie2[client]=1;
  return true;
}

new Handle:g_tempXp, Handle:g_tempDrop, Handle:g_tempMsg;
public HookTempVars() {
  g_tempXp = CreateConVar("wc_temp_xp", "1.0", "XP Modifier",FCVAR_NOTIFY,true,0.0,true,10.0);
  SetTempVar(0, Temp_XPRate, 1.0);
  HookConVarChange(g_tempXp, TempVarChanged);
  g_tempDrop = CreateConVar("wc_temp_drop", "1.0", "Drop Modifier",FCVAR_NOTIFY,true,0.0,true,10.0);
  SetTempVar(0, Temp_DropRate, 1.0);
  HookConVarChange(g_tempDrop, TempVarChanged);

  g_tempMsg = CreateConVar("wc_event_message", "", "Event Message, if any");
  g_sEventMessage="";
  HookConVarChange(g_tempMsg, EventMsgChange);
}

public EventMsgChange(Handle:tempVar, const String:oldValue[], const String:newValue[])
{
  EventMessage(newValue);
}


public TempVarChanged(Handle:tempVar, const String:oldValue[], const String:newValue[])
{
  new Float:newv=StringToFloat(newValue);
  decl String:name[100];
  GetConVarName(tempVar, name, sizeof(name));
  new Temp:temp = GetConvar(name);
  SetTempVar(0, temp, newv);
}

SetTempVar(client, Temp:tempVar, Float:value) {
  UserTemps[client][tempVar]=value;
}

Float:GetTempVar(client, Temp:tempVar)  {
  new Float:value = UserTemps[client][tempVar];
  if(value>0.0)
    if(value<1.0)
      return value;
    else if(value > UserTemps[0][tempVar])
      return value;
  return UserTemps[0][tempVar];
}

Temp:GetConvar(String:name[])  {
  if(StrEqual(name, "wc_temp_xp", false))
    return Temp_XPRate;
  if(StrEqual(name, "wc_temp_drop", false))
    return Temp_DropRate;
  return Temp_None;
}

