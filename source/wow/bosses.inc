//Bosses.inc file, used for the thought process of bots during bossmod
//Starting code is in bots.inc where SetBoss is ran, outside of that all bot think should be handled here.
//Regular botthink is disabled while BossThink is ran


//Any and all global variables within this file should be set here for easy cleanings
new Float:GlobalDifficulty; //General difficulty
new g_iStartSize[65]; //The starting size for Defile's effect, necessary to know how large Defile should be.
new g_iEndSize[65]; //The ending size for Defile's effect, necessary to know how large Defile should be.

//Boss spell settings for easier modifying, no config file yet.
new Float:g_fDefileDelay = 1.25; //The time between each Defile tick
new g_iDefileTicks = 8; //How many times Defile ticks, duration of defile is delay * ticks

BossThink(client)
{
  GlobalDifficulty = float(PlayersInTeams(true)) * 0.05;
  if(0.0>=GlobalDifficulty)
    GlobalDifficulty = 0.1;
  if(GetClass(client)!=4)
    wcSetClass(client, 4); //Bot's class set to Paladin to avoid any passive spells from being cast
  if(GetLevel(client)!=85)
    wcSetStaticLevel(client, 85);
  new Handle:botstop=FindConVar("bot_freeze"); //
  SetConVarInt(botstop, 1); //Set to stop to avoid bots from turning around so people who need to stand behind can stand behind
  CloseHandle(botstop);
  switch(GetBoss(client)) //The ID of the boss as shown in wcbossdrop.txt. If the client is boss 70, boss 70's code is ran etc.
  {
    case 70: BotThink70(client); //Lich King
  }
  if(g_iTopThreat[0]>0) //Checks if there's a #1 on threat list
  {
    new target = GetAimTarget(g_iTopThreat[0],25.0, false, false, 0.0, true); //Checks if #1 threat list is looking at the boss
    PrintToChatAll("topthreat is %i",g_iTopThreat[0]);
    PrintToChatAll("target is %i",target);
    if(target==client)
    {
      decl Float:location[3],Float:angle[3],Float:anglec[3];
      GetClientEyeAngles(target, angle);
      GetClientEyeAngles(client, anglec);
      if(angle[1]>0.0)
        angle[1]-=180.0;
      else
        angle[1]+=180.0;
      anglec[1]=angle[1];
      GetClientAbsOrigin(client,location);
      TeleportEntity(client, location, anglec, NULL_VECTOR); //Sets the boss's eyeangles so that it's facing towards the #1 threat target
    }
  }
}

stock BotThink70(client) 
{
  
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,250.0);
  if(team==3)
    amount=GetNearClients("@t",location,targets,250.0);
  
  
  //3 Second global cooldown = spellid 1
  new Float:gcd=GetCDSpell(client,1)-GetTickedTime();
  
  new Float:timeleft=GetCDSpell(client,2)-GetTickedTime(); //Checks for the cooldown on CDSpell 2, which is in this case Defile
  if(0.0 >= timeleft && 0.0 >= gcd && amount >= 2) //Starts casting defile
    CastDefile(client,2);
  
  
}

CastDefile(client,spellid)
{
  SetCdSpell(client, spellid, (20.0/GlobalDifficulty), spellid);
  SetCdSpell(client, 1, 3.0, 1);
  DisplayCenterTextToAll(client, "Defile in 2 seconds");
  CreateTimer(2.0, BossDefile, client);
}

public Action:BossDefile(Handle:timer, any:client)
{
  new team=GetClientTeam(client);
  team = (team-1) % 2 + 2;

  
  new Float:location[3];
  GetClientAbsOrigin(client,location);

  new dmg = RoundToNearest(1.0 + GetSpellDmg(client,0.1) * GlobalDifficulty);
  new aoe = 250;
  
  new effid=CreateEffectData(client,aoe,dmg,g_iDefileTicks,team,_:location[0],_:location[1],_:location[2],g_RoundCount,0);
  CreateTimer(g_fDefileDelay, BossDefileAction, effid, TIMER_REPEAT);


  //Effect

  EmitAmbientSound("wc/spells/deathanddecay.wav", location);
  location[2]+=15.0;
  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
    DispatchKeyValue(sprite, "basespread", "105");
    DispatchKeyValue(sprite, "spreadspeed", "12");
    DispatchKeyValue(sprite, "speed", "1");
    DispatchKeyValue(sprite, "startsize", "15");
    DispatchKeyValue(sprite, "endsize", "10");
    DispatchKeyValue(sprite, "rate", "75");
    DispatchKeyValue(sprite, "jetlength", "20");
    DispatchKeyValue(sprite, "rendercolor", "255 40 6");
    DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");
    DispatchKeyValueVector(sprite, "origin", location);
    DispatchSpawn(sprite);
    AcceptEntityInput(sprite, "TurnOn");
    KillEntityDelay(g_fDefileDelay, sprite);
  }
  return Plugin_Stop;
}

public Action:BossDefileAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];
  new aoe = effectsdata[effid][2];
  new dmg = effectsdata[effid][3];
  new ticks = --effectsdata[effid][4];
  new team = effectsdata[effid][5];
  new Float:location[3];
  location[0] = Float:effectsdata[effid][6];
  location[1] = Float:effectsdata[effid][7];
  location[2] = Float:effectsdata[effid][8];
  new round = effectsdata[effid][9];
  if (!IsClientInGame(client))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (round!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(ticks <= 0)
  {
    g_iEndSize[client]=0;
    g_iStartSize[client]=0;
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  wcSpellAoeDamageLocation(client, client, location, dmg, aoe, team, "Defile");
  
  team=GetClientTeam(client);
  new targets[MAXPLAYERS];
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,float(aoe));
  if(team==3)
    amount=GetNearClients("@t",location,targets,float(aoe));
  
  new Float:hit = 1.0 + (float(amount) * 0.05);
  
  if(1 > g_iStartSize[client])
    g_iStartSize[client]=15;
  if(1 > g_iEndSize[client])
    g_iEndSize[client]=10;
    
  g_iEndSize[client]=RoundToNearest(float(g_iEndSize[client])*hit);
  g_iStartSize[client]=RoundToNearest(float(g_iStartSize[client])*hit);
  
  new String:startsize[255];
  IntToString(g_iStartSize[client],startsize,sizeof(startsize));
  
  new String:endsize[255];
  IntToString(g_iEndSize[client],endsize,sizeof(endsize));
  
  dmg = RoundToNearest(float(dmg)*hit);
  aoe = RoundToNearest(float(aoe)*hit);

  team = ReverseTeam(team);

  location[2]+=15.0;
  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
    DispatchKeyValue(sprite, "basespread", "105");
    DispatchKeyValue(sprite, "spreadspeed", "12");
    DispatchKeyValue(sprite, "speed", "1");
    DispatchKeyValue(sprite, "startsize", startsize);
    DispatchKeyValue(sprite, "endsize", endsize);
    DispatchKeyValue(sprite, "rate", "75");
    DispatchKeyValue(sprite, "jetlength", "20");
    DispatchKeyValue(sprite, "rendercolor", "255 40 6");
    DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");
    DispatchKeyValueVector(sprite, "origin", location);
    DispatchSpawn(sprite);
    AcceptEntityInput(sprite, "TurnOn");
    KillEntityDelay(g_fDefileDelay, sprite);
  }
  location[2]-=15.0;
  return Plugin_Continue;
}