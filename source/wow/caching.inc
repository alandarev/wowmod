/*
* File: caching.inc
* Description: Caching algorithm to speed up few slow events like
*              beginning of new maps.
* Author(s): Stinkyfax
*/

#pragma semicolon 1

new Handle:g_cachedPlayers=INVALID_HANDLE;



Handle:CreateCachedPlayersArray()   {
   decl temp[CachedPlayer];
   temp[CP_id]=1;
   return CreateArray(sizeof(temp));
}


new g_lastCacheChecked=0;

public PreLoadCaching()  {
  g_cachedPlayers = CreateCachedPlayersArray();
  CreateTimer(1.0, CacheCleaner, 0, TIMER_REPEAT);
}

public Action:CacheCleaner(Handle:timer, any:trash)  {
  new size = GetArraySize(g_cachedPlayers);
  if(size <= g_lastCacheChecked)  {
    g_lastCacheChecked=0;
    return Plugin_Continue;
  }

  new Float:time;
  // is broken, make workaround, SM can't read array's cell.
  time = GetArrayCell(g_cachedPlayers, g_lastCacheChecked, 2) + 35.0;
  PrintToServer("Checking cached time %.2f against engine time %.2f",
    time, GetEngineTime());

  if( time < GetEngineTime() )  {
    RemoveFromArray(g_cachedPlayers, g_lastCacheChecked);
  }

  return Plugin_Continue;
}

stock GetCachedPlayerIndex(id) {
  for(new i=0; i < GetArraySize(g_cachedPlayers); i++) {
    if(GetArrayCell(g_cachedPlayers, i, 0) == id)
      return i;
  }
  return -1;
}

stock bool:IsCachedPlayer(id) {
  for(new i=0; i < GetArraySize(g_cachedPlayers); i++) {
    if(GetArrayCell(g_cachedPlayers, i, 0) == id)
      return true;
  }
  return false;
}

stock bool:GetCachedPlayer(item[CachedPlayer], client)  {
  item[CP_id] = 0;

  new id = GetClient32BitSteam(client);
  new index = GetCachedPlayerIndex(id);
  if(index<0)
    return false;
  GetArrayArray(g_cachedPlayers, index, item[0]);
  return true;
}

CachePlayerData(client) {
  new id = GetClient32BitSteam(client);
  new data[CachedPlayer];
  grabCache(client, data);
  data[CP_id] = id;
  data[CP_time] = GetEngineTime();
  PushArrayArray(g_cachedPlayers, data[0]);
  PrintToServer("Cached player");
}

grabCache(client, data[CachedPlayer]) {
  if(client)  {
  }
  if(data[0]) {}
}


























