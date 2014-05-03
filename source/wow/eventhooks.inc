/*
* File: eventcalls.inc
* Description: Hooks events
* Author(s): Stinkyfax
*/

#pragma semicolon 1
new g_averagePlayerLevel;

public HookEvents()
{
  if(!HookEventEx("player_spawn",PlayerSpawnEvent))
    SetFailState("[WC] Error: Failed to hook event: player_spawn");
  if(!HookEventEx("weapon_fire",WeaponFireEvent, EventHookMode_Pre))
      SetFailState("[WC] Error: Failed to hook event: weapon_fire");
  if(!HookEventEx("weapon_fire_on_empty",WeaponFireEvent))
      SetFailState("[WC] Error: Failed to hook event: weapon_fire");
  if(!HookEventEx("player_hurt",PlayerHurtEvent,EventHookMode_Pre))
    SetFailState("[WC] Error: Failed to hook event: player_hurt");
  if(!HookEventEx("player_footstep",PlayerMoveEvent))
    SetFailState("[WC] Error: Failed to hook event: player_footstep");
  if(!HookEventEx("player_death",PlayerDeathEvent,EventHookMode_Pre))
    SetFailState("[WC] Error: Failed to hook event: player_death");
  if(!HookEventEx("round_end",RoundEndEvent))
    SetFailState("[WC] Error: Failed to hook event: round_end");
  if(!HookEventEx("round_start",RoundStartEvent))
    SetFailState("[WC] Error: Failed to hook event: round_start");
  if(!HookEventEx("game_newmap",GameStartEvent))
    SetFailState("[WC] Error: Failed to hook event: game_start");
  if(!HookEventEx("weapon_reload",PlayerReloadEvent))
    SetFailState("[WC] Error: Failed to hook event: weapon_reload");
  if(!HookEventEx("bomb_begindefuse",BeginDefuseEvent))
    SetFailState("[WC] Error: Failed to hook event: bomb_begindefuse");
//  if(!HookEventEx("player_death",PlayerDeathEvent))
//    SetFailState("[wc] Error: Failed to hook event: player_death");
  if(!HookEventEx("bomb_planted",BombPlantedEvent))
    SetFailState("[WC] Error: Failed to hook event: bomb_planted");
  if(!HookEventEx("bomb_defused",BombDefuseEvent))
    SetFailState("[WC] Error: Failed to hook event: bomb_defused");
//  if(!HookEventEx("bomb_exploded",BombExplodeEvent))
//    SetFailState("[WC] Error: Failed to hook event: bomb_exploded");
  if(!HookEventEx("hostage_rescued",HostageRescueEvent))
    SetFailState("[WC] Error: Failed to hook event: hostage_rescued");
  if(!HookEventEx("player_changename",PlayerChangeNameEvent))
    SetFailState("[WC] Error: Failed to hook event: player_changename");

  //for unlimited ammo
  if(!HookEventEx("weapon_reload", Event_CheckDepleted))
    SetFailState("[WC] Error: Failed to hook weapon_reload for unlimited ammo");
  HookEvent("weapon_fire_on_empty", Event_CheckDepleted);
  HookEvent("weapon_fire", Event_CheckDepleted);
}

public Event_CheckDepleted(Handle:event, const String:name[], bool:dontBroadcast)
{
  new client = GetClientOfUserId(GetEventInt(event, "userid"));
  if (!client || !IsClientInGame(client))
  {
    return;
  }

  new entity = GetEntDataEnt2(client, ACTIVEWEAPON);
  if (entity < 1)
  {
    return;
  }

  new ammoType = GetEntProp(entity, Prop_Send, "m_iPrimaryAmmoType");

  /*Give something reasonable -- the game will chop it off*/
  DM_GiveClientAmmo(client, ammoType, 50, false);
}


public BeginDefuseEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  new client=GetClientOfUserId(GetEventInt(event,"userid"));
  if(client)
  {
    Defuser=client;
    if(UnableToVictim[client] || IsMageInvis(client) || IsInvisible(client) || DidWarlockFear(client) )
    {
      new bombent = FindEntityByClassname(-1,"planted_c4");
      if (bombent>-1)
      {
        WCMessage(client, "cant defuse", client);
        CreateTimer(0.2,DefuseDelay,0);
      }
    }
  }
}

public Action:DefuseDelay(Handle:timer)
{
    new bombent = FindEntityByClassname(-1,"planted_c4");
    if (bombent>-1)
    {
				//SetEntPropFloat(bombent, Prop_Send, "m_flDefuseLength",20.0);
        SetEntPropFloat(bombent, Prop_Send, "m_flDefuseCountDown",GetEntPropFloat(bombent, Prop_Send, "m_flDefuseCountDown")+8.0);				
		}
}

public RoundStartEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  g_RoundCount++;
  //KPD rule code
  g_iTaken= 0;
  g_iTakenb= 0;
  g_iTakenc= 0;
  g_iTakend= 0;
  g_iTakene= 0;
  g_iTakenf= 0;
  g_iTakeng= 0;
  g_iTakenh= 0;
  g_iTakeni= 0;
  g_iTakenj= 0;
  g_iTakenk= 0;
  g_iTakenl= 0;
  g_iTakenm= 0;
  g_iTakenn= 0;
  g_iTakeno= 0;
  g_iRoundEnded = 0;
  g_iTreasureChestNumber= 0;
  for(new i=0;i<65;i++)
  {
    g_iCombatReady[i]=0;
    g_iHStone[i]=0;
  }
  /*for(new i=0;i<5;i++)
  {
    g_iTreasureChestRarity[i]=0;
  }*/
  SpawnChests(true);
  //Add getting flag positions here, this is only temporary
  new Float:startloc[3];
  new Float:startloc2[3];
  if(IsCtf())
  {
    startloc[0] = -1327.0;
    startloc[1] = -756.8;
    startloc[2] = 129.1;
    startloc2[0] = 282.7;
    startloc2[1] = 2366.4;
    startloc2[2] = -126.7;
    CTFStart(startloc,startloc2);
    PrintToChatAll("Got isctf");
  }
  for(new client=1; client<=GetMaxClients(); client++) {
    if(IsClientInGame(client))  {
      new team = GetClientTeam(client);
      if(team == 2 || team == 3)
      {
        if(bArenaActive) {
          for (new x=0;x<=19;x++)
            ClientCDSpells[client][x]=GetTickedTime();
        }
        addRound(client);
      }
    }
  }
  //end
  //Bloodeffect
  /*if (((g_iEnvBlood = CreateEntityByName("env_blood")) != -1) && DispatchSpawn(g_iEnvBlood))*/
	/*{*/
		/*DispatchKeyValue(g_iEnvBlood, "spawnflags", "13");*/
		/*DispatchKeyValue(g_iEnvBlood, "amount", "1000");*/
	/*}*/
  for(new x=1;x<=GetMaxClients();x++)  {
    setOneRound(x,0);
    ClearTotems(x);
  }
  if(!StrEqual(g_sEventMessage, "", false))
    TellEvent();
  if(FUN_SERVER)  {
   for(new x=1;x<=GetMaxClients();x++) {
    if(IsClientInGame(x))
      PlainMessage(x, FUN_MESSAGE);
   }
  }
  if(((g_RoundCount % 5) == 0) && (g_newVersion))
   for(new x=1;x<=GetMaxClients();x++) {
    if(IsClientInGame(x))
      WCMessage(x,"Announce server new ver",x);
  }


  if(PlayersInTeams(true) >= 1)
  {
    new Handle:bots=FindConVar("bot_quota");
    if(GetConVarInt(bots)>0)
      CloseHandle(bots);
    else
    {
      SetConVarInt(bots, 8);
      CloseHandle(bots);
    }
  }
  
  CalculateAverageLevel();
}

CalculateAverageLevel()
{
  new avg = 0;
  new apl = 0;
  for(new x=1;x<=GetMaxClients();x++)
  {
    if(IsClientInGame(x))
    {
      avg += GetLevel(x);
      apl++;
    }
  }
  if(apl>0)
    g_averagePlayerLevel = RoundToNearest(float(avg)/float(apl));
}

GetAverageLevel()
{
  return g_averagePlayerLevel;
}

public GameStartEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  g_RoundCount=-2;
}
new g_iRoundsBalance = 0;
new HaniBalance = 0;

public RoundEndEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  new winner=GetEventInt(event,"winner");
  TeamWinXP(winner);
  //AcceptEntityInput(g_TreasureChestEntity, "Kill");
  LowLevelRound(); // give exp to low lvls every round
  for (new i=0;i<=299;i++)
    effectsdata[i][0]=0;  // clearing the effects
  CreateTimer(0.3,RoundStats,0);
  CreateTimer(0.5,DamageDoneXP,0);
  CreateTimer(0.5,HealingDoneXP,0);
  if(HaniBalance>0)
  {
    if(!IsBoss() || !IsHNS())
    {
      if(g_iRoundsBalance>=1) {
        TeamBalance();
        g_iRoundsBalance = 0;
        PrintToChatAll("\x04Teams have been balanced");
      }
      else {
        if(g_iRoundsBalance==2)
          PrintToChatAll("\x04Teams will be balanced in %i round",3-g_iRoundsBalance);
        else
          PrintToChatAll("\x04Teams will be balanced in %i rounds",3-g_iRoundsBalance);
        g_iRoundsBalance+=1;
      }
    }
  }
  g_iRoundEnded =1;
  g_iTaken= 0;
  g_iTakenb= 0;
  g_iTakenc= 0;
  g_iTakend= 0;
  g_iTakene= 0;
  g_iTakenf= 0;
  g_iTakeng= 0;
  g_iTakenh= 0;
  g_iTakeni= 0;
  g_iTakenj= 0;
  g_iTakenk= 0;
  g_iTakenl= 0;
  g_iTakenm= 0;
  g_iTakenn= 0;
  g_iTakeno= 0;
  ArenaRoundEnd(winner);
  if(IsBoss() || IsHNS())
  {
    TriggerBossDrop();
    for(new x=1;x<=GetMaxClients();x++)
    {
      if(IsClientInGame(x))  {
        if((GetClientTeam(x)==3) && (GetBoss(x)>0))
        {
          CS_SwitchTeam(x, 2);
        }
        if((GetClientTeam(x)==2) && (GetBoss(x)==0))
        {
          CS_SwitchTeam(x, 3);
        }
      }
    }
  }
}

public PlayerMoveEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  new client=GetClientOfUserId(GetEventInt(event,"userid"));
  SetCasting(client,0,0.0);
  if(wcClientSparks[client] || FreedomClient[client])
  {
    decl Float:location[3];
    new Float:dir[3]={0.0,0.0,0.0};
    GetClientAbsOrigin(client,location);
    MetalSparkEffect("@all", location, dir);
    //HOLLIDAY: es es_xgetplayerlocation eps_x1 eps_y1 eps_z1 event_var(userid)
    //es est_GetViewCoord event_var(userid) esp_x2 esp_y2 esp_z2
    //es est_effect 8 #a 0 effects/bluespark.vmt server_var(eps_x1) server_var(eps_y1) server_var(eps_z1) server_var(eps_x2) server_var(eps_y2) server_var(eps_z2)
    // beam follow effect - reserved
  }
}

RegMove()   {
   CreateTimer(0.1, CheckMove,0,TIMER_REPEAT);
}


new Float:playerPoints[65][3];
public Action:CheckMove(Handle:timer, any:trash)   {
   for(new i=1;i<=GetMaxClients();i++)  {
      if(IsClientInGame(i) && IsPlayerAlive(i)) {
         decl Float:origin[3];
         GetClientAbsOrigin(i,origin);
         new bool:passed=true;
         for(new x=0;x<3;x++) {
            if(origin[x]!=playerPoints[i][x])
               passed=false;
         }
         playerPoints[i][0]=origin[0];
         playerPoints[i][1]=origin[1];
         playerPoints[i][2]=origin[2];
         if(!passed)
            PlayerMoved(i);
      }
   }

   return Plugin_Continue;
}

PlayerMoved(client)  {
   SetCasting(client,0,0.0);
   SetStealth(client, 0.0);
}

public GetFrags(client)
{
    new g_i_frags = FindDataMapOffs(client, "m_iFrags");
    if (g_i_frags != -1)
    {
        return GetEntData(client, g_i_frags);
    }

    return 0;
}

public SetFrags(client, frags)
{
    new g_i_frags = FindDataMapOffs(client, "m_iFrags");
    if (g_i_frags != -1)
    {
        SetEntData(client, g_i_frags, frags);
    }
}


referrerScheme(client, type, gold) {
  decl String:column[20];
  Format(column, sizeof(column), "referrer_%d", type);
  decl String:query[200];
  Format(query, sizeof(query),
    "SELECT `referrer`, '%d' AS `gold` FROM wc_Players WHERE playerId = %d",
    gold, playerids[client]);

  SQL_TQuery(T2SQL,TreferrerScheme,query,client);
}

public TreferrerScheme(Handle:db,Handle:query,String:error[],any:client)
{

  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      if(!SQL_FetchRow(query))  {
        LogError("WC FAILED ev.224, no rows");
      }
      new gold = GetSQLDataInt(query, "gold");
      new referrer = GetSQLDataInt(query, "referrer");
      if(referrer <= 0) {
        PrintToServer("No referrer for player %d", playerids[client]);
        return;
      }
      PrintToServer("referrer = %d", referrer);
      addGoldToPlayer(referrer, gold, "friends", "Gold for levelup of friend");
      PrintToServer("Line 232");
    }
    else
    {
      LogError ("WC FAILED ev.236 %s",error);
    }
  }
}

LevelUpHook(client, level) {
  new maxLvl = 0;
  for (new x=1;x<=15;x++)
    if(characterids[client][x]!=0)
    {
      maxLvl = GetMaxInt(charactersdata[client][x][2], maxLvl);
    }
  switch(level) {
    case 35:  if( level > maxLvl ) referrerScheme(client, 5, 30);
    case 50:  if( level > maxLvl ) referrerScheme(client, 1, 30);
    case 60:  if( level > maxLvl ) referrerScheme(client, 2, 40);
    case 70:  if( level > maxLvl ) referrerScheme(client, 3, 100);
    case 74:  if( level > maxLvl ) referrerScheme(client, 4, 200);
  }
}
