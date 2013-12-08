#include <sourcemod>
#include <sdktools>

new Handle:hSetModel=INVALID_HANDLE;
new Handle:hGameConf=INVALID_HANDLE;

new Handle:g_method=INVALID_HANDLE;

public Plugin:myinfo = 
{
	name = "wcs_setmodel command",
	author = "Stinkyfax",
	description = "Precaches models on startup and adds wcs_setmodel",
	version = "0.9",
	url = "http://sugardas.lt/~jozh/"
}

public OnPluginStart()
{
	// Load the game data file
	hGameConf = LoadGameConfigFile("setmodel.gamedata");

   // Model changing SDK call
	StartPrepSDKCall(SDKCall_Player);
	PrepSDKCall_SetFromConf(hGameConf, SDKConf_Virtual, "SetModel");
	PrepSDKCall_AddParameter(SDKType_String, SDKPass_Pointer);
	hSetModel = EndPrepSDKCall();
	RegServerCmd("wcs_setmodel", SetModelCmd, "usage: <#userID> <model> <hide_knife 0/1>");
	g_method = CreateConVar("wcs_setmodel_method","1", "Methods of setting model: possible 1 or 2. 1 - uses SDKCall, 2 - SM Based");
}

public OnPluginEnd(){

}

public Action:SetModelCmd(args)  {
   if(args != 3)   {
      decl String:buffer[255];
      GetCmdArgString(buffer, 255);
      LogMessage("wcs_setmodel - Wrong Syntax Usage: %s. Correct: <#userID> <model> <hide_knife 0/1>",buffer);
      return Plugin_Handled;
   }
   
   decl String:s_userId[25];
   GetCmdArg(1, s_userId, sizeof(s_userId));
   decl String:s_filePath[255];
   GetCmdArg(2, s_filePath, sizeof(s_filePath));
   decl String:s_hideKnife[5];
   GetCmdArg(3, s_hideKnife,sizeof(s_hideKnife));
   
   new userId = StringToInt(s_userId);
   new iHide = StringToInt(s_hideKnife);
   new bool:hide=false;
   if(iHide==1)
      hide=true;
   
   new client = GetClientOfUserId(userId);
   if( (client==0) || (!IsClientInGame(client)))   {
      decl String:buffer[255];
      GetCmdArgString(buffer, 255);
      LogMessage("wcs_setmodel - Client is not in game: %s",buffer);
      return Plugin_Handled;
   }
   if(!IsPlayerAlive(client))
      return Plugin_Handled;
   if(!FileExists(s_filePath,true))   {
      decl String:buffer[255];
      GetCmdArgString(buffer, 255);
      LogMessage("wcs_setmodel - Model Does Not EXIST: %s",buffer);
      return Plugin_Handled;
   }
   if(!IsModelPrecached(s_filePath))   {
      decl String:buffer[255];
      GetCmdArgString(buffer, 255);
      LogMessage("wcs_setmodel - Model is not Precached: %s",buffer);
      return Plugin_Handled;
   }
   if(GetConVarInt(g_method) == 1)
      SetEntityModel(client, s_filePath);
   else
      SDKCall(hSetModel, client, s_filePath);
   if(hide) {
      new knife = GetPlayerWeaponSlot(client, 2);
      //if(knife>=0)
      //   SetEntityRenderColor(knife, 255, 255, 255, 0);
      CreateTimer(10.0, CheckKnife, client);
   }
   return Plugin_Handled;
}

public Action:CheckKnife(Handle:timer, any:client) {
   new bool:pass=true;
   if((!IsClientInGame(client)) || (!IsPlayerAlive(client)))
      pass=false;
   if(pass) {
      new knife = GetPlayerWeaponSlot(client, 2);
      if(knife>=0)   {
         SetEntityRenderColor(knife, 255, 255, 255, 0);
      }
   }
   return Plugin_Stop;
}


public ReadPrecache(){
	new String:file[256];
	BuildPath(Path_SM, file, 255, "configs/models2.ini");
	new Handle:fileh = OpenFile(file, "r");
	new String:buffer[256];
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


public OnMapStart(){	
   ReadPrecache();
}

public ReadFileFolder(String:path[]){
	new Handle:dirh = INVALID_HANDLE;
	new len;
	
	len = strlen(path);
	if (path[len-1] == '\n')
		path[--len] = '\0';

	TrimString(path);
	ReadItem(path);
	if(dirh != INVALID_HANDLE){
		CloseHandle(dirh);
	}
}

public ReadItem(String:buffer[]){
	new len = strlen(buffer);
	if (buffer[len-1] == '\n')
		buffer[--len] = '\0';
	
	TrimString(buffer);
	
	if (!StrEqual(buffer,"",false) && FileExists(buffer,true))
	{
	  if(PrecacheModel(buffer,true)<=0)
	     PrintToServer("Error precaching %s",buffer);
	}
}
