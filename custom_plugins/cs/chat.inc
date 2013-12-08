/*
* File: chat.inc
* Description: Chat functionalities: mainly cross-server talk
* Author(s): Stinkyfax
*/

#pragma semicolon 1



public Action:SayCommand(Handle:event, const String:name[], bool:dontBroadcast)  {
  new client = GetClientOfUserId(GetEventInt(event, "userid"));
  if(client == 0) return Plugin_Continue;
  decl String:args[255];
  GetEventString(event, "text", args, sizeof(args));
  ServerSay(client, args);

  return Plugin_Continue;
}

new String:g_chatKeywords[][] = { "wcmenu", "rpgmenu" , "wcsmenu", "top", "wowmenu", "rpgmenu" };
stock ServerSay(client, String:text[])   {
  if(StrContains(text, "!") == 0 || StrContains(text,"/") == 0 ) return; //Don't forward commands
  //Keywords
  for(new i=0; i< sizeof(g_chatKeywords ); i++) {
    if(StrContains(text, g_chatKeywords[i], false) == 0)  return;
  }
  decl String:buffer[300];
  Format(buffer, sizeof(buffer), "%s", text);
  ReplaceString(buffer, sizeof(buffer), "%", "");

  decl String:name[25];
  GetClientName(client, name, sizeof(name));

  ReplaceString(buffer,sizeof(buffer),"'","",false);
  sqlSendChat(name, buffer);
}
public Action:TimerChat(Handle:timer)  {
  sqlGetChats();

  return Plugin_Continue;
}

