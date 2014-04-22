/*
* File: wow.sp
* Author: Stinkyfax
* Description: World of Warcraft mod for CSS servers (tested on Windows)
* Homepage: http://sugardas.lt/~jozh/
* Thanks to SumGuy14 (Aka SoccerDude) for creating RPGx plugin
* this one helped me starting my mod.
*/
//dbs
// max string legth -   new String:buffer[17980];

#pragma dynamic 16777216

new bool:DEBUG=false;

new Handle:effectsdb=INVALID_HANDLE;
new Handle:wcnews=INVALID_HANDLE;
new Handle:wchelp=INVALID_HANDLE;
new Handle:wcdrop=INVALID_HANDLE;
new Handle:wcsets=INVALID_HANDLE;
new Handle:wcbossdrop=INVALID_HANDLE;
new Handle:items=INVALID_HANDLE;
new Handle:wcitemnames=INVALID_HANDLE; //prefix names
new Handle:wcbaseitemnames=INVALID_HANDLE; //base names
new bool:g_mailclean=false;
new bool:g_mailattacher=false;
new bool:g_saveworld=false;
new bool:g_auctionclean=false;
new bool:g_playerpurge=false;
new bool:g_cronjob=false;
new offsSpeed;
#define MAX_TREASURE_SPAWNS      256
#define MAX_TREASURE               5  //max # of active treasure chests
new String:g_TreasureSpawnName[32];
new bool:g_occupiedTreasureSpawns[MAX_TREASURE_SPAWNS]=false;
new bool:g_AreWeSpawningTreasure = false;
new g_TreasureMenuOccupiedByClient=0;
new Handle:hTreasureSpawnMenu=INVALID_HANDLE;
#define DB_VERSION 30
#define VERSION 5.1
#define STR_VERSION "5.1"
#define WIKI_VERSION "5.1" //should not contain spaces or any special characters


#include <sourcemod>
#include <cstrike>
#include <sdkhooks>
#include <vs_natives>
#include <cssdm>
//#include <trophies>
#include "wow/wow"

#pragma semicolon 1



public Plugin:myinfo =
{
  name = "wow",
  author = "Stinkyfax",
  description = "Feel the spirit of World of Warcraft:Cataclysm in CS:S",
  version = STR_VERSION,
  url = "http://wowmod.eu/"
};

public Action:OnGetGameDescription(String:gameDesc[64]) {
  Format(gameDesc, 64, "WoW Mod %s", STR_VERSION);
  return Plugin_Changed;
}

public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
  PrintToServer("----------------|      WC guessing game   |---------------");
  new String:gameName[50];
  GetGameFolderName(gameName, sizeof(gameName));
  if(StrEqual(gameName, "csgo"))  g_csgo=true;

  PrintToServer("----------------|         WC preloading   |---------------");
  InitNatives();
  RegPluginLibrary("wow");
  PrintToServer("----------------|         WC preloaded    |---------------");
  return APLRes_Success;
}


public OnPluginStart()
{
  PrintToServer("----------------|         WC Loading        |---------------");
  PrintToServer("Creating abstract arrays");
  //Precache Some critical materials
  PrecacheHP();

  CreateAbstractArrays();
  DefineProfLevels();
  PrintToServer("Registering commands");
  RegAdminCommands();
  PrintToServer("Applying version number");
  SetCurrentVersion(STR_VERSION);
  PrintToServer("Finding offsets");
  InitOffsets(); // Finds offsets and defines functions to use them
  PrintToServer("Initializing game data");
  InitGameData(); // Finds virtual function offsets and signatures
  PrintToServer("Loading Config");
  LoadConfig();  // Load the config file
  PrintToServer("Loading Translations");
  LoadTranslationFile();  // Load the chat translations file
  PrintToServer("Pre-loading unique items database");
  PreLoadItems(); //Creates local database for unique items
  /*PreLoadCaching(); //Launch caching*/
  PrintToServer("Hooking events");
  HookEvents(); // Hooks events
  PrintToServer("Hooking commands");
  HookCmds(); // Hooks cmds
  PrintToServer("Refreshing static database");
  ReloadDBs();
  PrintToServer("Initializing MySQL database");
  InitMySQL(); // Initializes the SQL database
  PrintToServer("Initializing online clients");
  for (new x=1;x<=GetMaxClients();x++)
  {
    if (IsClientInGame(x))
      Initialize(x);
  }
  for (new x=1;x<=64;x++)
    for (new y=1;y<=49;y++)
      wcClientsStats[x][y]=float(0);
  PrintToServer("Some more offsets and commands…");
  // increase speed
  offsSpeed=FindSendPropInfo("CBasePlayer","m_flLaggedMovementValue");
  g_iOffs_ActiveWeapon = FindSendPropOffs("CBasePlayer","m_hActiveWeapon");
  g_vOffs_Punch = FindSendPropInfo("CCSPlayer",
                        "m_vecPunchAngle");
  //FindSendPropOffs("CCSPlayer","m_vecPunchAngle");
  g_iOffs_NextPrimaryAttack = FindSendPropOffs("CBaseCombatWeapon","m_flNextPrimaryAttack");
  // end of increase speed
  convar_friendlyfire=FindConVar("mp_friendlyfire");
  //boss setting
  RegConsoleCmd("jointeam", CommandJoinTeam);
  g_BossMod = CreateConVar("wc_bossmod", "0", "Activate/Deactivate Boss Mod",FCVAR_NOTIFY,true,0.0,true,1.0);
  HookConVarChange(g_BossMod, BossModChanged);
  g_Heroic = CreateConVar("wc_heroic", "0", "Activate/Deactivate Heroic Mod",any:0,true,0.0,true,1.0);
  HookConVarChange(g_Heroic, HeroicModChanged);
  g_TreasureHunt = CreateConVar("wc_treasurehunt", "0", "Activate/Deactivate Treasure Hunt",FCVAR_NOTIFY,true,0.0,true,1.0);
  HookConVarChange(g_TreasureHunt, TreasureHuntChanged);
  g_Ctf = CreateConVar("wc_ctf", "0", "Activate/Deactivate Capture the Flag",FCVAR_NOTIFY,true,0.0,true,1.0);
  HookConVarChange(g_Ctf, CtfChanged);
  if(g_dmgArray == INVALID_HANDLE)
    g_dmgArray = CreateArray(1, 64);

  PrintToServer("Hook temporary variables");
  HookTempVars();
  //end of boss setting
//  offsetscore = FindSendPropOffs("CPlayerResource", "m_iScore");
//  if((entscore=FindEntScore())==0)
//     SetFailState("[WC] Error: Could not find Score entity");
//  Call_StartForward(OnPluginLoaded);
//  Call_Finish();
  PrintToServer("Preload weapons menu? (I think there is none yet)");
  PreloadWeaponsMenu();
  //Check for hlstatsx Pre hook conflicts
  PrintToServer("Reload Conflicting plugins if exists to give it accurate event information.");
  CheckHLStatsX();
  PrintToServer("Caching premium accounts");
  getPDs();
  PrintToServer("----------------|   WC FIRST PART LOADED   |---------------");
}

public OnConfigsExecuted()
{
  PrintToServer("----------------|   Restarting WC Services  |---------------");
  PrintToServer("Load configs");
  LoadConfig();
  PrintToServer("Precache models");
  PrecacheModels();
  PrintToServer("Register custom events");
  RegMove(); //Registrates event move

  PrintToServer("Check SQL tables");
  //CheckSQLTables();
  //PrintToServer("Check for global information");
  //CheckGlobalSettings(); //Check for global information
  PrintToServer("Update string definitions");
  UpdateStringConfig();
  if(MAIN_SERVER)
   PrintToServer("WC Server is launched as MAIN SERVER due to config settings");
  else
   PrintToServer("WC Server is launched as SECONDARY SERVER due to config settings");
  PrintToServer("Load Arena functions?");
  CheckArenaMap();
  /*
  wcCreateSQLTableUniqueIds();
  wcCreateSQLTablePlayers();
  wcCreateSQLTableCharacters();
  wcCreateSQLTableCharacterSpells();
  wcCreateSQLTableCharacterInv();
  wcCreateSQLTableCharacterInv2();
  wcCreateSQLTableCharacterSb();
  wcCreateSQLTableMail();
  wcCreateSQLTableAuction();
  wcCreateSQLTableCharacterMail();
  wcCreateSQLTableProfessions2();
  wcCreateSQLTableProfessions1();
  */
  PrintToServer("loading skies");
  //ServerCommand("sv_skyname sky_day03_06");

  PrintToServer("----------------|  WC Services are started  |---------------");
  //TestSuite();
}

public TestSuite()  {
  new String:query[]="CALL bid(1,16);";
  SQL_TQuery(TSQL, TTestSuite, query, 1, DBPrio_High);
}

public TTestSuite(Handle:db, Handle:hndl, String:error[], any:id)  {
  if(hndl==INVALID_HANDLE)
    PrintToServer("Error: %s", error);
  SQL_Rewind(hndl);
  SQL_FetchRow(hndl);
  new status=SQL_FetchInt(hndl, 0);
  PrintToServer("got: %d", status);
}

public OnMapStart()
{
  PrecacheHP();
  //restart kpd
  resetKpdKv();
  g_AreWeSpawningTreasure = LoadTreasureMapConfig();
  //Bind entity manager
  new PMIndex = FindEntityByClassname(0, "cs_player_manager");
  SDKHook(PMIndex, SDKHook_ThinkPost, OnThinkPost);


  g_RoundCount=-1;

  InitClock();

  CheckBgStyle();
  CheckArenaMap();
  //end

  wcUpdateNews();
  wcShowHM();
  wcCheckMail();
  LaunchComm();
  InstallDownloads();
  if(g_T2Queue > 20)
      BlockServer(); // Blocks playing on the server for the period of loading data
}

public OnClientPutInServer(client)
{
  SDKHook(client,SDKHook_OnTakeDamage,TraceAttack); //hooks.inc
  SDKHook(client, SDKHook_SetTransmit, SetTransmitHook);
  //PrintToServer("hooking weapon can use for client %d", client);
  SDKHook(client, SDKHook_WeaponCanUse, OnWeaponCanUse); //weapon touch and equip only
  //Darken(client);
  SDKHook(client, SDKHook_PreThink, PreThinkHook);
  SDKHook(client, SDKHook_ThinkPost, OnThinkPostClient);
}

public OnClientAuthorized(client, const String:auth[])
{
  Initialize(client);
  RefreshAccData(client);


//  Call_StartForward(OnPlayerAuthed);
//  Call_PushCell(index);
//  Call_Finish();
}

public bool:OnClientConnect(client, String:rejectmsg[], maxlen) {
  ClearPlayerData(client);
  resetCachedPD(client);
  return true;
}

public OnClientDisconnect(index)
{
  SDKUnhook(index,SDKHook_OnTakeDamage,TraceAttack);
  //SDKUnhook(index,SDKHook_WeaponCanUse,OnWeaponCanUse);
  SDKUnhook(index, SDKHook_ThinkPost, OnThinkPostClient);
  SDKUnhook(index, SDKHook_PreThink, PreThinkHook);
  if(bArenaActive)
   CheckArenaChange(index); //Before save so that disc. loose honor
  if(playerloaded[index]==1)
  {
    saveKpd(index); //save kpd
    wcSavePlayerName(index);
    wcSavePlayerData(index,true);
    /*CachePlayerData(index);*/
  }

  playerloaded[index]=0;
}

public OnPluginEnd()
{
  if(G_RebootTotal==0)
  for (new x=1;x<=GetMaxClients();x++)
  {
    if (IsClientInGame(x))
    {
      //SDKUnhook(x,SDKHook_OnTakeDamage,TraceAttack);
      //if(playerloaded[x]==1)
      //  wcSavePlayerData(x);  - too late to save now
    }
  }
}


