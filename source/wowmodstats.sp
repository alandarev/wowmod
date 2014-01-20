#include <sourcemod>
#include <wow_natives>

public Plugin:myinfo =
{
  name = "WoWMod Stat Tracker",
  author = "Brook Bowers",
  description = "WoWMod Stat Tracker",
  version = "0.5.1",
  url = "http://www.sourcemod.net/"
};
//TABLE SCHEMA
//| log_id | row_id | timestamp | victim_class | victim_talents | victim_rating | attacker_class | attacker_talents | attacker_rating |

#define TABLE_NAME "wc_kills_stats"

static Handle:g_hDatabase = INVALID_HANDLE;
static Handle:g_hEnabled = INVALID_HANDLE;

public OnPluginStart() {
  g_hEnabled = CreateConVar("wow_stats_enabled", "1", "Sets whether WoWMod Stat Tracker is enabled.");
  g_hEnabled = FindConVar("wow_stats_enabled");
  HookConVarChange(g_hEnabled, Stats_OnCvarSwitch);
  Stats_HookHandler(GetConVarBool(g_hEnabled));
  
  RegConsoleCmd("wow_stats_last_5", Stats_Retrieve5Entries, "Retrieves last 5 WoWMod Stat Tracker entries.");
  
  PrintToServer("Started Stat Tracker!");
  Stats_StartSQL();
}

public Action:Stats_Retrieve5Entries(client, args) {
  new String:query[100];
  Format(query, sizeof(query), "SELECT * FROM %s ORDER BY timestamp DESC LIMIT 5;", TABLE_NAME);
  SQL_TQuery(g_hDatabase, Stats_QueryHandler, query, client);
}

public OnPluginEnd() {
  CloseHandle(g_hDatabase);
}

public Stats_OnCvarSwitch(Handle:cvar, const String:oldVal[], const String:newVal[]) {
  Stats_HookHandler(GetConVarBool(cvar));
}

public Stats_HookHandler(bool:on) {
  if(on) {
    HookEvent("player_death", Stats_LogDeath);
  } else {
    UnhookEvent("player_death", Stats_LogDeath);
  }
}

Stats_StartSQL() {
  SQL_TConnect(Stats_GotDatabase);
}

public Stats_GotDatabase(Handle:owner, Handle:hndl, const String:error[], any:data) {
  if (hndl == INVALID_HANDLE) {
    PrintToServer("Failed to connect to database!");
    LogError("Database failure: %s", error);
  } else {
    g_hDatabase = hndl;
    PrintToServer("Successfully connected to database!");
  }
}

public Stats_LogDeath(Handle:event, const String:name[], bool:dontBroadcast) {
  PrintToServer("line 43");
  new attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
  new victim = GetClientOfUserId(GetEventInt(event, "userid"));
  //Attacker was world/c4/barrels, suicide, or bot don't record
  if(IsWorld(attacker) || IsTeammate(attacker, victim) || IsBot(attacker)) { return;} 
  
  Stats_LookUpCharacterInfo(attacker, victim);
}

public bool:IsWorld(client) {
  return client == 0;
}

public bool:IsSelf(client1, client2) {
  return client1 == client2;
}

public bool:IsBot(client) {
  return IsClientInGame(client) && IsFakeClient(client);
}

public bool:IsTeammate(client1, client2) {
  return GetClientTeam(client1) == GetClientTeam(client2);
}

public Stats_LookUpCharacterInfo(attacker, victim) {
  new v_class, v_talents, Float:v_rating, a_class, a_talents, Float:a_rating;
  //Gather data about victim
  v_class = Stats_GetPlayerClass(victim);
  v_talents = Stats_GetPlayerTalents(victim);
  v_rating = Stats_GetPlayerRating(victim);
  //Gather data about attacker
  a_class = Stats_GetPlayerClass(attacker);
  a_talents = Stats_GetPlayerTalents(attacker);
  a_rating = Stats_GetPlayerRating(attacker);

  Stats_CreateEntry(v_class, v_talents, v_rating, a_class, a_talents, a_rating);
}

public Stats_CreateEntry(v_class, v_talents, Float:v_rating, a_class, a_talents, Float:a_rating) {
  new String:insertQuery[500];
  Format(insertQuery, sizeof(insertQuery),
         "REPLACE DELAYED INTO %s SET row_id = (SELECT COALESCE(MAX(log_id), 0) %% 10000000 + 1 FROM wc_kills_stats AS t), victim_class = '%d', victim_talents = '%d', victim_rating = '%d', attacker_class = '%d', attacker_talents = '%d', attacker_rating = '%d';",
         TABLE_NAME, v_class, v_talents, RoundFloat(v_rating), a_class, a_talents, RoundFloat(a_rating));
  PrintToServer("Inserting query: %s", insertQuery);
  SQL_TQuery(g_hDatabase, Stats_ErrorHandler, insertQuery, 1);
}

// wowmodstats SQL Error: You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '10000000 + 1 FROM circular_log_table AS t), victim_class = '2', victim_talents ='

//"INSERT INTO %s (victim_class, victim_talents, victim_rating, attacker_class, attacker_talents, attacker_rating) VALUES (%d, %d, %d, %d, %d, %d);"

public Float:Stats_GetPlayerRating(clientid) {
  new Float:rating = wow_getTempPlayerRating(clientid);
  PrintToServer("attacker rating: = %.2f", rating);
  return rating;
}

public Stats_GetPlayerClass(clientid) {
  new class = wow_getClass(clientid);
  return class;
}

public Stats_GetPlayerTalents(clientid) {
  new talents = wow_getTalentTrees(clientid);
  return talents;
}

public Stats_ErrorHandler(Handle:owner, Handle:query, const String:error[], any:data) {
  if(query == INVALID_HANDLE) {
    if(data == 1) {//Error_Handler called by INSERT query, no problem
      LogError("wowmodstats SQL Error: %s", error);
    }
    else {
      PrintToServer("WoWModStats Tracker encountered an error: (%s).", error);
    }
  }
}

public Stats_QueryHandler(Handle:owner, Handle:query, const String:error[], any:data) {
  if(query == INVALID_HANDLE) { LogError("Failed to retrieve logs: %s", error); }
  
  while (SQL_FetchRow(query)) {
    new String:timestamp[50];
    SQL_FetchString(query, 2, timestamp, sizeof(timestamp));
    PrintToConsole(data, "Log #%d (%s).", SQL_FetchInt(query, 0), timestamp);
    }
  return 0;
}