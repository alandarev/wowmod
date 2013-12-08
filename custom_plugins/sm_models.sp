/*
Files required:
   configs/download_files.ini
   data/playermodels.ini
   data/models.txt
*/
#include <sourcemod>
#include <sdktools>
#include <wow_natives>

#pragma semicolon 1

#define TEAM_T 2
#define TEAM_CT 3

#define STR_VERSION "1.3"



new Handle:g_version=INVALID_HANDLE;
new Handle:kv;
new Handle:playermodelskv;
new Handle:hGameConf;
new Handle:hSetModel;

new String:authid[MAXPLAYERS + 1][35];
new downloadtype;




public Plugin:myinfo = 
{
	name = "Models: Menu",
	author = "Stinkyfax",
	description = "Part of Models family which allows people to choose custom models",
	version = STR_VERSION,
	url = "http://www.sourcemod.net"
}

public OnPluginStart()
{
	g_version = CreateConVar("sm_models_version",STR_VERSION,"SM MODELS:MENU VERSION",FCVAR_NOTIFY);
	SetConVarString(g_version,STR_VERSION);
	
		// Create the model menu command
	RegConsoleCmd("sm_models",Command_Model);
	
	
	// Load the game data file
	hGameConf = LoadGameConfigFile("setmodel.gamedata");
	
	HookEvent("player_spawn", Event_PlayerSpawn, EventHookMode_Post);
	//HookEvent("player_team", Event_PlayerTeam, EventHookMode_Post);
	
	// Model changing SDK call
	StartPrepSDKCall(SDKCall_Player);
	PrepSDKCall_SetFromConf(hGameConf, SDKConf_Virtual, "SetModel");
	PrepSDKCall_AddParameter(SDKType_String, SDKPass_Pointer);
	hSetModel = EndPrepSDKCall();
	
	// Load the player's model settings
	new String:file[256];
	BuildPath(Path_SM, file, 255, "data/playermodels.ini");
	playermodelskv = CreateKeyValues("Models");
	FileToKeyValues(playermodelskv, file);
	
	//Loading models file
	kv = CreateKeyValues("Models");
	BuildPath(Path_SM, file, 255, "data/models.txt");
	FileToKeyValues(kv, file);
	
	for(new i=1;i<=GetMaxClients();i++)   {
	  if(IsClientInGame(i))   if(IsClientAuthorized(i))
	      OnClientPostAdminCheck(i);
	}
}

public OnPluginEnd(){
	CloseHandle(g_version);
	// Write the the player's model settings
	new String:file[256];
	BuildPath(Path_SM, file, 255, "data/playermodels.ini");
	KvRewind(playermodelskv);
	KeyValuesToFile(playermodelskv, file);
	CloseHandle(playermodelskv);
	CloseHandle(kv);
}

public OnMapStart(){	
	ReadDownloads();
	new String:file[256];
	BuildPath(Path_SM, file, 255, "data/playermodels.ini");
	KvRewind(playermodelskv);
	KeyValuesToFile(playermodelskv, file);
	if(kv != INVALID_HANDLE)
	  CloseHandle(kv);
	kv = CreateKeyValues("Models");
	BuildPath(Path_SM, file, 255, "data/models.txt");
	FileToKeyValues(kv, file);
}



Handle:BuildMenu(client,team)
{
	/* Create the menu Handle */
	new Handle:menu = CreateMenu(Menu_Model);
	
	KvRewind(kv);
	
	// Make sure there is at least one model group
	if (!KvGotoFirstSubKey(kv))
	{
	  LogError("Loading Menu Failed");
	}
	// Load the model groups
	KvRewind(kv);
	if(team==2)
	  KvJumpToKey(kv,"Team1");
	else if (team==3)
	  KvJumpToKey(kv,"Team2");
	else
	  return INVALID_HANDLE;
	decl String:teamValue[5];
	IntToString(team-1,teamValue,sizeof(teamValue));
	AddMenuItem(menu,teamValue,"Default");
	KvGotoFirstSubKey(kv);
	do   {
	  decl String:keyName[255];
	  KvGetSectionName(kv,keyName,sizeof(keyName));
	  decl String:groups[500];
	  KvGetString(kv,"Groups",groups,sizeof(groups),"");
	  if(Verify(client,groups)) {
	     AddMenuItem(menu,keyName,keyName);
	  }
	} while(KvGotoNextKey(kv));
	SetMenuTitle(menu, "Choose a Model");
	return menu;
}

bool:Verify(client,String:groups[]) {
   decl String:group[25][50];
   new counter=0;
   new index=0;
   new ind2=0;
   while(ind2>=0){
      decl String:part[25];
      ind2 = SplitString(groups[index],"|",part,sizeof(part));
      group[counter]=part;
      if(ind2<0)  {
         new String:newStr[25];
         strcopy(newStr, 25, groups[index]);
         /*
         for(new i = index;((i<size) && (i<25));i++)   {
            newStr[i-index]=groups[i];
         }
         */
         group[counter]=newStr;
      }
      else  {
         group[counter]=part;
      }
      index+=ind2;
      counter++;
   }
   for(new i=0;i<counter;i++) {
      if(StrEqual(group[i],"Any",false))
         return true;
      if(StrEqual(group[i],"Premium",false))  {
        if(wow_hasPremiumModule(client, PDModule:PDM_models))
          return true;
      }
   }
   new AdminId:AdmId = GetUserAdmin(client);
   new count = GetAdminGroupCount(AdmId);

   for (new i =0; i<count; i++) 
   {
      decl String:temp[50];
      new GroupId:AdmGroup = GetAdminGroup(AdmId, i, temp, sizeof(temp)); 
      for(new y=0;y<counter;y++) {
         if(FindAdmGroup(group[y])==AdmGroup)
            return true;
      }
   }
   return false;
}

public ReadFileFolder(String:path[]){
	new Handle:dirh = INVALID_HANDLE;
	new String:buffer[256];
	new String:tmp_path[256];
	new FileType:type = FileType_Unknown;
	new len;
	
	len = strlen(path);
	if (path[len-1] == '\n')
		path[--len] = '\0';

	TrimString(path);
	
	if(DirExists(path)){
		dirh = OpenDirectory(path);
		while(ReadDirEntry(dirh,buffer,sizeof(buffer),type)){
			len = strlen(buffer);
			if (buffer[len-1] == '\n')
				buffer[--len] = '\0';

			TrimString(buffer);

			if (!StrEqual(buffer,"",false) && !StrEqual(buffer,".",false) && !StrEqual(buffer,"..",false)){
				strcopy(tmp_path,255,path);
				StrCat(tmp_path,255,"/");
				StrCat(tmp_path,255,buffer);
				
				if(type == FileType_File){
					if(downloadtype == 1){
						ReadItem(tmp_path);
					}
				}
				else if(type == FileType_Directory)   {
				  if(downloadtype==1)
				     ReadFileFolder(tmp_path);
				}
			}
		}
	}
	else{
		if(downloadtype == 1){
			ReadItem(path);
		}
		
	}
	if(dirh != INVALID_HANDLE){
		CloseHandle(dirh);
	}
}

public ReadDownloads(){
	new String:file[256];
	BuildPath(Path_SM, file, 255, "configs/download_files.ini");
	new Handle:fileh = OpenFile(file, "r");
	new String:buffer[256];
	downloadtype = 1;
	new len;
	if(fileh == INVALID_HANDLE) return;
	while (ReadFileLine(fileh, buffer, sizeof(buffer)))
	{	
		len = strlen(buffer);
		if (buffer[len-1] == '\n')
			buffer[--len] = '\0';

		TrimString(buffer);

		if(!StrEqual(buffer,"",false)){
			ReadFileFolder(buffer);
		}
		
		if (IsEndOfFile(fileh))
			break;
	}
	if(fileh != INVALID_HANDLE){
		CloseHandle(fileh);
	}
}

public ReadItem(String:buffer[]){
	new len = strlen(buffer);
	if (buffer[len-1] == '\n')
		buffer[--len] = '\0';
	
	TrimString(buffer);

	if(len >= 2 && buffer[0] == '/' && buffer[1] == '/'){
		//Comment
		if(StrContains(buffer,"//") >= 0){
			ReplaceString(buffer,255,"//","");
		}
	}
	else if (!StrEqual(buffer,"",false) && FileExists(buffer))
	{
		AddFileToDownloadsTable(buffer);
	}
}

public OnClientPostAdminCheck(client)
{
	// Save the client auth string (steam)
	GetClientAuthString(client, authid[client], sizeof(authid[]));
}

public Action:Command_Model(client,args)
{
	if(client<=0 || client > GetMaxClients())  {
      ReplyToCommand(client,"sm_models Is only for players");
      return Plugin_Handled;
   }
	if((!IsClientInGame(client)) || ( (GetClientTeam(client) != 2) && (GetClientTeam(client) != 3)))
      return Plugin_Handled;
	new Handle:menu;
	if(args==0)
    menu=BuildMenu(client,GetClientTeam(client));
	else
    menu=BuildMenu(client,args);
	if (menu == INVALID_HANDLE)
	{
		// We don't, send an error message and return
		if(args==0)
		 PrintToChat(client,"\x03[Models]\x01: Model Menu is available only when you are in team.");
		return Plugin_Handled;
	}
	// We have a valid menu, display it and return
	DisplayMenu(menu, client, MENU_TIME_FOREVER);	
	return Plugin_Handled;
}

public Menu_Model(Handle:menu, MenuAction:action, client, slot)
{
	if (action == MenuAction_Select)
	{
      new String:temp[50];
      GetMenuItem(menu, 0, temp,sizeof(temp));
      new iTeam = StringToInt(temp);
      GetMenuItem(menu,slot,temp,sizeof(temp));
      new String:modelName[50];
      strcopy(modelName,sizeof(modelName),temp);
      if(slot==0)
         modelName="";
      KvRewind(playermodelskv);
      KvJumpToKey(playermodelskv,authid[client], true);		
      Format(temp,sizeof(temp),"Team%i",iTeam);
      KvSetString(playermodelskv, temp, modelName);
      PrintToChat(client,"\x03[Models]\x01: Setting \x04%s \x01model for next Spawn.",modelName);
	}
	if (action == MenuAction_End)
	{
		CloseHandle(menu);
	}
}

public Event_PlayerTeam(Handle:event, const String:name[], bool:dontBroadcast)
{
 
 new client = GetClientOfUserId(GetEventInt(event, "userid"));
 new team = GetEventInt(event, "team");
 if((team == TEAM_T || team == TEAM_CT) && IsClientInGame(client))
 {
  Command_Model(client, team);
 }
 return;
}

public Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)  {
	new clientId = GetEventInt(event, "userid");
	new client = GetClientOfUserId(clientId);
	//CreateTimer(1.5, PlayerSpawnTimer, client, TIMER_FLAG_NO_MAPCHANGE);
	PlayerSpawnTimer(INVALID_HANDLE, client);
}

public Action:PlayerSpawnTimer(Handle:timer, any:client)
{
	if(!IsClientInGame(client))
      return;
	if(!IsPlayerAlive(client))
      return;
	// Get the user's authid
	KvRewind(playermodelskv);
	KvJumpToKey(playermodelskv,authid[client],true);

	new String:model[256];
	// Get the user's model pref
	decl String:sTeam[20];
	Format(sTeam,sizeof(sTeam),"Team%i",GetClientTeam(client)-1);
	KvGetString(playermodelskv, sTeam, model, sizeof(model),"");
	new String:path[300];
	if(!StrEqual(model,"",false))   {
	  KvRewind(kv);
	  KvJumpToKey(kv,sTeam);
	  if(!KvJumpToKey(kv,model))  {
	     PrintToChat(client,"\x03[Models]\x01: Model you selected does not exist, try another one");
	     return;
	  }
	  decl String:groups[500];
	  KvGetString(kv,"groups",groups,sizeof(groups));
	  if(!Verify(client,groups)) {
	     PrintToChat(client,"\x03[Models]\x01: You do not have access to model you selected");
	     return;
	  }
	  KvGetString(kv,"path",path,sizeof(path));
	}
	// check for precached
	if ((!StrEqual(model,"", false)) && (!StrEqual(path,"",false)))
	{
	  if (IsModelPrecached(path))   {
	     PrintToChat(client,"\x03[Models]\x01: Setting \x04%s \x01model.",model);
	     SDKCall(hSetModel, client, path);
	  }
	  else
	     LogError("Model is not precached: %s",path);
	}
	else
	  PrintToChat(client,"\x03[Models]\x01: Setting default model.");
}