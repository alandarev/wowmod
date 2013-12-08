/*
* File: balancer.inc
* Description: mod balancer: teams, low kpd, high kpd, rating balancer...
* Author(s): Stinkyfax
*/

#pragma semicolon 1

#define COUNTER_TERRORIST_TEAM 3
#define TERRORIST_TEAM 2

static const String:ctModels[4][] = {"models/player/ct_urban.mdl", "models/player/ct_gsg9.mdl", "models/player/ct_sas.mdl", "models/player/ct_gign.mdl"};
static const String:tModels[4][] = {"models/player/t_phoenix.mdl", "models/player/t_leet.mdl", "models/player/t_arctic.mdl", "models/player/t_guerilla.mdl"};

public Float:getTempPlayerRating(client, teamWon)	{
//	new clientTeam = GetClientTeam(client);
//	new bool:winner=false;
//	if(clientTeam==teamWon)
//		winner=true;
	new Float:rating = GetPlayerRating(client);
	return rating;
	
}


public scrambleTeams()	{
  new max = GetMaxClients();
  new healers[max];
  new tanks[max];
  new dps[max];
  new ih,it,id; //iterators
  for(new i=1; i<=max; i++)	{
    if(IsClientConnected(i) && IsClientInGame(i) && (GetClientTeam(i)==2 || GetClientTeam(i)==3) )	{
      switch(getCharacterType(i))	{
        case Character_Healer:	healers[ih++]=i;
        case Character_Tank: tanks[it++]=i;
        default: dps[id++]=i;
      }
    }
  }
  SortCustom1D(healers, ih, SortRating);
  SortCustom1D(tanks, it, SortRating);
  SortCustom1D(dps, id, SortRating);

  PrintToServer("DEBUG: %d healers, %d tanks, %d dps", ih, it, id);

  //TODO finish scrambling algorithm.


}

//Sort by custom rating
public SortRating(elem1, elem2, const array[], Handle:hndl)	{
	if(getTempPlayerRating(elem1, -1) > getTempPlayerRating(elem2, -1))
	{
		return -1;
	} else if(getTempPlayerRating(elem1, -1) == getTempPlayerRating(elem2, -1)) {
		return 0;
	} else {
		return 1;
	}
}



// Switches the team of a client and associated a random player model
// Also sends a message to let the player know they have been switched
// Based on the team switch command from SM Super Commands by pRED*
public SwitchTeam(client, team)
{
	// If the player has the bomb we need to drop it
	new bomb;
	bomb = GetPlayerWeaponSlot(client, 4);
	if(bomb != -1 && bomb)
	{
		SDKCall(weaponDrop, client, bomb, false, false);
	}
	
	// Switch the players team
	SDKCall(switchTeam, client, team);
	
	// Set a random model
	new random = GetRandomInt(0, 3);
	if(team == TERRORIST_TEAM)
	{
		SDKCall(setModel, client, tModels[random]);
		PrintCenterText(client, "%t", "t switch");
	} else if(team == COUNTER_TERRORIST_TEAM) {
		SDKCall(setModel, client, ctModels[random]);
		PrintCenterText(client, "%t", "ct switch");
	}
	
	// Respawn the player so they end up back at their own spawn point
	SDKCall(roundRespawn, client);
}

//Balancing out low kpd new players
new bool:g_ff_bIsNewPlayer[65];

_setNewPlayer(client, bool:value) {
  g_ff_bIsNewPlayer[client] = value;
}

bool:defineIsPlayerNew(client) {
  new maxLvl=0;
  maxLvl = GetLevel(client);
  for (new x=1;x<=15;x++)
    if(characterids[client][x]!=0)
    {
      maxLvl = GetMaxInt(charactersdata[client][x][2], maxLvl);
    }
  if(maxLvl < NEWBIE_UNTIL )  {
    _setNewPlayer(client, true);
    return true;
  }
  _setNewPlayer(client, false);
  return false;
}

stock bool:isPlayerNew(client) {
  return g_ff_bIsNewPlayer[client];
}

Float:getNewPlayerModifier(client, bool:attacker)	{
  if(client && attacker)  {}
  return 1.0; //disabled as 4.70 version
  /*if(SERVER_LEVEL_POLICY==1)  {*/
    /*return 1.0; //No modifier for high lvls servers*/
  /*}*/
  /*if(!isPlayerNew(client))*/
    /*return 1.0;*/
  /*new Float:kpd = getOptimizedKPD(client);*/

  /**
  * Thoughs of constructing formula
  * 0.2 kpd should give us +100% attack +50% defense
  * 0.5 kpd would be nice to give 60% attack and 30% def
  * 0.8 kpd something like 30% and 15%
  * 1.0 10% and 5%
  * more than 1.0 - 0% both
  */
  /*if(kpd >= 1.1)*/
    /*return 1.0;*/

  /*if(kpd > 1.0) kpd = 1.0;*/

  /*if(attacker)*/
    /*return 1.1 + 0.6 * (1.0 - kpd);*/
  /*return 0.95 - 0.15 * (1.0 - kpd);*/


}


stock Float:getOptimizedKPD(client)	{
  new deaths = GetClientDeaths(client);
  new kills = GetClientFrags(client);
  if(deaths<=0)
  {
    deaths = 1;
    if(kills<=0)
      kills = 1;
  }
  new Float:ratio = float(kills) / float(deaths);
  if( (ratio < 0.2) && (deaths < 8) )
    ratio = 0.2;
  if(GetClass(client)== 5)
    ratio += 0.25;
  return ratio;
}

spreadPoints(client)  {
  new race = GetClass(client);
  if(race<=0)
    return;
  new points=GetPoints(client);
  for(new i=0;i<points;i++) {
    new StatPoint:stat = getNextPointSpendTo(client);
    _addPoint(client, stat);

  }
  wcSetPoints(client, 0);
}

_addPoint(client, StatPoint:point)  {
  switch(point) {
    case SP_Strength: {
      wcSetStr(client, GetStr(client)+1);
      //WCMessage(client, "str increased",client);
    }
    case SP_Agility:  {
      wcSetAgi(client, GetAgi(client)+1);
      //WCMessage(client, "agi increased",client);
    }
    case SP_Intellect:  {
      wcSetInt(client, GetInt(client)+1);
      //WCMessage(client, "int increased",client);
    }
    case SP_Stamina:  {
      wcSetSta(client, GetSta(client)+1);
      //WCMessage(client, "sta increased",client);
    }
    default:  {
      //LogError("[WC]: Tried to spread invalid point");
    }
  }

}

stock StatPoint:getNextPointSpendTo(client) {
  new race = GetClass(client);
  if(race<=0)
    return StatPoint:SP_None;

  return _getNextStat(race, GetStr(client), GetAgi(client), GetInt(client), GetSta(client));

}

stock StatPoint:_getNextStat(race, str, agi, inte, sta)  {
  if(str == 0 || agi == 0 || inte == 0 || sta == 0) {
    str++;
    agi++;
    inte++;
    sta++;
  }

  KvRewind(StatsDB);
  decl String:key[10];
  Format(key, sizeof(key), "%d", race);
  KvJumpToKey(StatsDB, key);
  new Float:strRatio = float(KvGetNum(StatsDB, "str"));
  new Float:agiRatio = float(KvGetNum(StatsDB, "agi"));
  new Float:staRatio = float(KvGetNum(StatsDB, "sta"));
  new Float:intRatio = float(KvGetNum(StatsDB, "int"));
  strRatio /= str;
  agiRatio /= agi;
  staRatio /= sta;
  intRatio /= inte;
  new StatPoint:maxstat=StatPoint:SP_Strength;
  new Float:maxRatio = strRatio;
  if(agiRatio > maxRatio) {
    maxRatio = agiRatio;
    maxstat = StatPoint:SP_Agility;

  }
  if(staRatio > maxRatio) {
    maxRatio = staRatio;
    maxstat = StatPoint:SP_Stamina;

  }
  if(intRatio > maxRatio) {
    maxRatio = intRatio;
    maxstat = StatPoint:SP_Intellect;

  }

  return maxstat;
}

new g_ff_kills[65];
new g_ff_deaths[65];
resetKPDStats(client)  {
  g_ff_kills[client]=0;
  g_ff_deaths[client]=0;
}
addKill(client) {
  if(!IsBoss() && !IsHNS())
    g_ff_kills[client]++;
}
addRound(client)  {
  g_ff_deaths[client]++;
}
getKills(client)  {
  return g_ff_kills[client];
}
getRounds(client) {
  if(g_ff_deaths[client]<2)
    return 2;
  return g_ff_deaths[client];
}

new Handle:g_kvKpd=INVALID_HANDLE;
resetKpdKv()  {
  if(g_kvKpd!=INVALID_HANDLE) {
    CloseHandle(g_kvKpd);
    g_kvKpd = INVALID_HANDLE;
  }
  g_kvKpd = CreateKeyValues("kpd");
}

saveKpd(client) {
  if(playerloaded[client]!=1)
    return;
  new uniqueId = GetClient32BitSteam(client);

  KvRewind(g_kvKpd);
  KvGotoFirstSubKey(g_kvKpd);
  decl String:key[20];
  Format(key, 20, "%d", uniqueId);
  KvJumpToKey(g_kvKpd, key, true);
  KvSetFloat(g_kvKpd, "time", GetEngineTime());
  KvSetNum(g_kvKpd, "kills", getKills(client));
  KvSetNum(g_kvKpd, "deaths", getRounds(client));
}

loadKpd(client) {
  new uniqueId = GetClient32BitSteam(client);
  KvRewind(g_kvKpd);
  //KvGotoFirstSubKey(g_kvKpd);
  decl String:key[20];
  Format(key, 20, "%d", uniqueId);
  if(KvJumpToKey(g_kvKpd, key, false)) {
    if(KvGetFloat(g_kvKpd,"time")+1200.0 < GetEngineTime() )
      return;
    g_ff_kills[client]=KvGetNum(g_kvKpd, "kills");
    g_ff_deaths[client]=KvGetNum(g_kvKpd, "deaths");
  }

}


KpdRule(client, damage, type=1, level=0) {
  if(bArenaActive)
    return damage;
  if(IsBoss() || IsHNS() || IsDmEnabled()) // not supposed to be active during DM right?
    return damage;

  if(playerloaded[client]!=1)
    return damage;
  //if(PlayersInTeams(true) >= 24)
  //  return damage;
  new rounds = getRounds(client);
  if(rounds<=3)
    rounds=3;
  new Float:limit=0.7;
  if(isClientPDModule(client, PDModule:PDM_kpd))
    limit=1.0;
  new Float:kpd = float(getKills(client)) / float(getRounds(client)) - limit;
  if(level == 1) //Checks if both the targets are level 85
    kpd*=0.33;
  if(kpd < 0.0)
    return damage;
  new Float:multi = 1.0;
  switch(type)  {
    case 1: //physical dmg
    {
      multi = 1.0 - kpd * 0.33; //reduce
      if(multi < 0.3)
        multi = 0.3;
    }
    case 2: //damage taken
    {
      multi = 1.0 + kpd * 0.33; //increase
    }
    case 3: //magic damage
    {
      multi = 1.0 - kpd * 0.33; //reduce
      if(multi < 0.3)
        multi = 0.3;
    }
    case 4: //healing taken
    {
      multi = 1.0 - kpd * 0.25; //reduce
      if(multi < 0.25)
        multi = 0.25;
    }
  }
  return RoundToNearest(float(damage)*multi);
}



/**
 * Give out rare-epic items to 5-84 levels functionality
 * v2: No longer a 85 limitation, as it is dynamic now.
 */
stock ItemDropBalancer(client)  {
  //Verify if person is eligable for drop
  new level = GetLevel(client);
  if(level < 5) return;
  if(GetClientTeam(client) != 2 && GetClientTeam(client) != 3)  return; //No specs

  //Chance
  //The higher level the lower chance
  new dice = level / 2;
  if(dice < 3)  dice = 3;
  if(GetRandomInt(1,dice) != 1)  return;
  /*new dropped = GetDroppedItems(client);*/   //No more needed to limit total amount
  /*if(RoundToCeil(1.0*level) < dropped)  return; //Got enough items by now*/

  /*new epicChance=(GetRandomInt(1,100)<=10)?4:3;*/


  GivePlayerRandomItem(client);

  //Item request from SQL
  /*decl String:query[300];*/
  /*if(level < 65)*/
    /*Format(query, sizeof(query),*/
    /*"SELECT itemid from wc_itemsCache where category='Wearable'  and level <= %d and level > %d and (`class` & 1<<%d) and quality > 0 and quality < %d order by rand() limit 1;",*/
    /*level, level-20, GetClass(client), epicChance);*/
  /*else*/
    /*Format(query, sizeof(query),*/
    /*"SELECT itemid from wc_itemsCache where category='Wearable'  and level <= %d and level > %d and (`class` & 1<<%d) and quality > 0 and quality < 3 order by rand() limit 1;",*/
    /*level, level-20, GetClass(client));*/

  /*SQL_TQuery(SSQL, TItemDrop, query, client);*/

}

public TItemDrop(Handle:db, Handle:query, String:error[], any:client)  {
  if(!IsClientInGame(client) || playerloaded[client]!=1)  return;

  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query)) {
      new itemid = GetSQLDataInt(query, "itemid");
      new item[UniqueItem];
      GetItemProperties(item, itemid);
      if(GiveItem(client, item, 1, "Balancer", false, true) >= 0)
      setDroppedItems(client, GetDroppedItems(client)+1);
    }
    else  {
      LogError("WC Found 0 items, code 396");
    }

  }
  else  {
    LogError ("WC FAILED b.390 %s",error);
  }
}


new g_ffDroppedItems[65];
setDroppedItems(client, amount) {
  g_ffDroppedItems[client]=amount;

}
GetDroppedItems(client) {
  return g_ffDroppedItems[client];
}

new bool:g_ffServPol[65];
CleanServPol(client)  {
  g_ffServPol[client]=false;
}

CheckServerPolicy(client) {
  //Check server policy against client
  new level = GetLevel(client);
  if(playerloaded[client] != 1 || level == 0) return;
  switch ( SERVER_LEVEL_POLICY) {
    case 1: { //high lvl
      if(level < 70)  {
        WCMessage(client, "low lvl server ad", client, SERVER_LEVEL_POLICY_REDIRECT);
        if(!g_ffServPol[client])  {
          g_ffServPol[client]=true;

          panelMessage(client, "Please join right server for you", "Dear player,\nYou are currently on server for High Levels,\nPlease Click F3 to get to our main server for everyone\nIf you keep playing here, you might find very difficult\nto kill anybody.\nTo see all available servers write !servers\n\nBest Regards,\nAdministration", 30);
        }
        RedirectClient(client,SERVER_LEVEL_POLICY_REDIRECT,"15");
      }

    }
    case 2: { //low lvl
      if(level > 80)  {
        WCMessage(client, "high lvl server ad", client, SERVER_LEVEL_POLICY_REDIRECT);
        if(!g_ffServPol[client])  {
          g_ffServPol[client]=true;

          panelMessage(client, "Please join right server for you", "Dear player,\nYou are currently on server for Low Levels,\nPlease Click F3 to get to our High-levels only server for everyone\nIf you keep playing here, low levels \ndying hard and giving no rewards.\nTo see all available servers write !servers\n\nBest Regards,\nAdministration", 30);
        }
        RedirectClient(client,SERVER_LEVEL_POLICY_REDIRECT,"15");
      }

    }
  }
}

stock TeamBalance()
{
  new TCount = GetTeamClientCount(CS_TEAM_T);
  new CTCount = GetTeamClientCount(CS_TEAM_CT);
  new players[MAXPLAYERS];
  new amount[2];
  amount[0]=FindMatchingPlayers("@t",players);
  amount[1]=FindMatchingPlayers("@ct",players);
  new Float:Trating = 0.0;
  new Float:CTrating = 0.0;
  new PriestsT = 0;
  new PriestsCT = 0;
  for(new x=0;x<amount[0];x++)  {
    PrintToChatAll("rating is %f",GetPlayerRating(x));
    Trating+=GetPlayerRating(x);
    if(GetClass(x)==5)
      PriestsT+=1;
  }
  for(new x=0;x<amount[1];x++)  {
    PrintToChatAll("rating is %f",GetPlayerRating(x));
    CTrating+=GetPlayerRating(x);
    if(GetClass(x)==5)
      PriestsCT+=1;
  }
  new avgratingct = RoundToNearest(CTrating/float(CTCount));
  new avgratingt = RoundToNearest(Trating/float(TCount));
  PrintToChatAll("CTrating is %f",CTrating);
  PrintToChatAll("Trating is %f",Trating);
  PrintToChatAll("avgratingt is %i",avgratingt);
  PrintToChatAll("avgratingct is %i",avgratingct);
  PrintToChatAll("Terrorist Priests: %i",PriestsT);
  PrintToChatAll("Counter-Terrorist Priests: %i",PriestsCT);
}