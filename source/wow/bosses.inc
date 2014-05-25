//Bosses.inc file, used for the thought process of bots during bossmod
//Starting code is in bots.inc where SetBoss is ran, outside of that all bot think should be handled here.
//Regular botthink is disabled while BossThink is ran


//Any and all global variables within this file should be set here for easy cleanings
new g_iBaseSpread[65]; //For Defile AoE effect's size

//Boss spell settings for easier modifying, no config file yet.
new Float:g_fDefileDelay = 1.25; //The time between each Defile tick
new g_iDefileTicks = 8; //How many times Defile ticks, duration of defile is delay * ticks

BossThink(client)
{
  GlobalDifficulty = float(PlayersInTeams(true)) * 0.05;
  if(0.5 > GlobalDifficulty)
    GlobalDifficulty = 0.5;
  if(GetClass(client)!=4)
    wcSetClass(client, 4); //Bot's class set to Paladin to avoid any passive spells from being cast
  if(GetLevel(client)!=85)
    wcSetStaticLevel(client, 85);
  new Handle:botstop=FindConVar("bot_freeze");
  SetConVarInt(botstop, 1);
  CloseHandle(botstop);
  new Handle:botzombie=FindConVar("bot_zombie");
  SetConVarInt(botzombie, 1);
  CloseHandle(botzombie);
  switch(GetBoss(client)) //The ID of the boss as shown in wcbossdrop.txt. If the client is boss 70, boss 70's code is ran etc.
  {
    case 70: BotThink70(client); //Lich King
    case 71: BotThink71(client); //Alizabal
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

stock BotThink71(client) 
{
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,400.0);
  if(team==3)
    amount=GetNearClients("@t",location,targets,400.0);
  
  
  //Alizabal only has a global cooldown of 1 second due to the way the spells interact with each other
  new Float:gcd=GetCDSpell(client,1)-GetTickedTime();
  
  new Float:time1=GetCDSpell(client,2)-GetTickedTime();
  if(0.0 >= time1 && 0.0 >= gcd && amount >= 3) //Blade Dance, casts any time it is available and there are 3 or more targets nearby
    CastBladeDance(client,2);
    
  new Float:time2=GetCDSpell(client,3)-GetTickedTime();
  if(4.5 >= time1 && 0.0 >= gcd && time2 >= 0.0 && (GetThreat(g_iAggroTable[0])>=MIN_THREAT || ignoreLimit)) //Skewer, casts only if Blade Dance's cooldown is under 4.5 seconds to make sure the tank has to run away before the warning or they die
    CastSkewer(client,3);
  
  new Float:time3=GetCDSpell(client,4)-GetTickedTime();
  if(0.0 >= gcd && time3 >= 0.0 && time1 >= 3.0) //Seething Hate, casts only if Blade Dance has a cooldown of 3.0 seconds or higher, to make sure that it doesn't cast while Blade Dance is off-cooldown
    CastSeethingHate(client,4);
  
}
//Cast spells
//Alizabal
CastBladeDance(client,spellid)
{
  SetCdSpell(client, spellid, 20.0, spellid);
  SetCdSpell(client, 1, 1.0, 1);
  DisplayCenterTextToAll(client, "Blade Dance in 2 seconds");
  CreateTimer(2.0, BladeDance, client);
}
CastSkewer(client,spellid)
{
  SetCdSpell(client, spellid, 15.0, spellid);
  SetCdSpell(client, 1, 1.0, 1);
  DisplayCenterTextToAll(client, "Skewer in 2 seconds");
  CreateTimer(2.0, Skewer, client);
}
CastSeethingHate(client,spellid)
{
  SetCdSpell(client, spellid, 6.0, spellid);
  SetCdSpell(client, 1, 1.0, 1);
  CreateTimer(0.1, SeethingHate, client); //Displaycentertexttoall is within the SeethingHate code so it can tell the target's name
}


//Lich King
CastDefile(client,spellid)
{
  SetCdSpell(client, spellid, 20.0, spellid);
  SetCdSpell(client, 1, 3.0, 1);
  DisplayCenterTextToAll(client, "Defile in 2 seconds");
  CreateTimer(2.0, BossDefile, client);
}








//Actual spells
//Alizabal
public Action:SeethingHate(Handle:timer, any:client)
{
  new team=GetClientTeam(client);
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  new target;
  new players[GetMaxClients()];
  new amount = getActivePlayers(players);
  if(amount >= 2) {
    target = players[GetRandomInt(0,amount-1)];
    if(team==GetClientTeam(target)) {
      if(team==2)
        target=GetNearestClient("@ct",location,2000.0);
      if(team==3)
        target=GetNearestClient("@ct",location,2000.0);
    }
  }
  new effid=CreateEffectData(client,target,0,0,0,0,0,0,g_RoundCount,0);
  CreateTimer(4.0, SeethingHateAction, effid);
  WCMessage(target,"seething hate target",target);
  new color[4]={255,0,0,120};
  FadeEffect(target,2,300,50,color);
  color[4] = { 255, 0, 0, 255 };
  SetClientColor(target,color);
  decl String:name[255];
  GetClientName(target,name,sizeof(name));
  DisplayCenterTextToAll(client, "Seething Hate on %s",name);
}

public Action:SeethingHateAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];
  new target = effectsdata[effid][2];
  new color[4] = { 255, 255, 255, 255 };
  if(target>0)
    SetClientColor(target,color);
  if (!IsClientInGame(client) || !IsClientInGame(target))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (round!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  
  new Float:location[3];
  GetClientAbsOrigin(target,location);

  new dmg = RoundToNearest(30.0 + GetSpellDmg(client,0.20));
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,300.0);
  if(team==3)
    amount=GetNearClients("@t",location,targets,300.0);
  for (new x=0;x<=(amount-1);x++)
  {
    if(targets[x]!=target)
      wcSpellDamage(targets[x],client,dmg, "", "","Seething Hate");
  }
  location[2]+=40;
  VS_BeamRingPoint(location, 19.00, 20.00, g_ef_splodesprite, g_ef_splodesprite, 100, 300, 0.50, 60.00, 1.00, {255, 255, 255, 255}, 300, 0);
  VS_BeamRingPoint(location, 19.00, 50.00, g_ef_splodesprite, g_ef_splodesprite, 100, 300, 0.50, 60.00, 1.00, {255, 255, 255, 255}, 300, 0);
}

public Action:Skewer(Handle:timer, any:client)
{
  new dmg = RoundToNearest(5.0 + float(GetDamage(client))*0.20);
  new target = g_iAggroTable[0];
  
  new effid=CreateEffectData(client,target,dmg,7,0,0,0,0,g_RoundCount,0);
  CreateTimer(1.0, SkewerAction, effid, TIMER_REPEAT);
  
  SetClientSpeed(target,0.0);
  g_iSkewered[target]=1;
  wcPhysicalDamage(target,client,dmg*2, "", "Skewer!",true);
}

public Action:SkewerAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];
  new target = effectsdata[effid][2];
  new dmg = effectsdata[effid][3];

  if (!IsClientInGame(client) || !IsClientInGame(target))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (round!=g_RoundCount)
  {
    g_iSkewered[target]=0;
    SetClientSpeed(target,GetNormalSpeed(target));
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(ticks <= 0)
  {
    g_iSkewered[target]=0;
    SetClientSpeed(target,GetNormalSpeed(target));
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  wcPhysicalDamage(target,client,dmg, "", "Skewer",true);
  return Plugin_Continue;
}

public Action:BladeDance(Handle:timer, any:client)
{
  new dmg = RoundToNearest(5.0 + float(GetDamage(client))*0.25);
  new aoe = 400;
  new effid=CreateEffectData(client,aoe,dmg,6,0,0,0,0,g_RoundCount,0);
  CreateTimer(1.0, BladeDanceAction, effid, TIMER_REPEAT);
  
  
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,float(aoe));
  if(team==3)
    amount=GetNearClients("@t",location,targets,float(aoe));

  EmitAmbientSound("wc/spells/whirlwind.wav",location);
  CreateTimer(0.0,FanOfKnivesEffect,client);
  CreateTimer(0.1,FanOfKnivesEffect,client);

  for (new x=0;x<=(amount-1);x++)
  {
    decl String:vicMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","blade dance victim", targets[x]);

    wcPhysicalDamage(targets[x],client,dmg, "", vicMsg, true);
    BloodSpray(targets[x]);
  }
}

public Action:BladeDanceAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];
  new aoe = effectsdata[effid][2];
  new dmg = effectsdata[effid][3];
  new ticks = --effectsdata[effid][4];
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
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,float(aoe));
  if(team==3)
    amount=GetNearClients("@t",location,targets,float(aoe));

  EmitAmbientSound("wc/spells/whirlwind.wav",location);
  CreateTimer(0.0,FanOfKnivesEffect,client);
  CreateTimer(0.1,FanOfKnivesEffect,client);

  for (new x=0;x<=(amount-1);x++)
  {
    decl String:vicMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","blade dance victim", targets[x]);

    wcPhysicalDamage(targets[x],client,dmg, "", vicMsg, true);
    BloodSpray(targets[x]);
  }
  return Plugin_Continue;
}




//Lich King
public Action:BossDefile(Handle:timer, any:client)
{
  new team=GetClientTeam(client);
  team = (team-1) % 2 + 2;

  
  new Float:location[3];
  GetClientAbsOrigin(client,location);

  new dmg = RoundToNearest(1.0 + GetSpellDmg(client,0.1));
  if(5>dmg)
    dmg = 5;
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
    //g_iEndSize[client]=0;
    //g_iStartSize[client]=0;
    g_iBaseSpread[client]=0;
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(ticks <= 0)
  {
    //g_iEndSize[client]=0;
    //g_iStartSize[client]=0;
    g_iBaseSpread[client]=0;
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
  
  new Float:hit = 1.1 + (float(amount) * 0.05);
  
  /*if(1 > g_iStartSize[client])
    g_iStartSize[client]=15;
  if(1 > g_iEndSize[client])
    g_iEndSize[client]=10;*/
  if(1 > g_iBaseSpread[client])
    g_iBaseSpread[client]=105;
    
  //g_iEndSize[client]=RoundToNearest(float(g_iEndSize[client])*hit);
  //g_iStartSize[client]=RoundToNearest(float(g_iStartSize[client])*hit);
  g_iBaseSpread[client]=RoundToNearest(float(g_iBaseSpread[client])*hit);
  
  /*new String:startsize[255];
  IntToString(g_iStartSize[client],startsize,sizeof(startsize));
  
  new String:endsize[255];
  IntToString(g_iEndSize[client],endsize,sizeof(endsize));*/
  
  new String:basespread[255];
  IntToString(g_iBaseSpread[client],basespread,sizeof(basespread));
  
  effectsdata[effid][3] = RoundToNearest(float(dmg)*hit);
  effectsdata[effid][2] = RoundToNearest(float(aoe)*hit);
  
  team = ReverseTeam(team);

  location[2]+=15.0;
  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
    DispatchKeyValue(sprite, "basespread", basespread);
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
  location[2]-=15.0;
  return Plugin_Continue;
}