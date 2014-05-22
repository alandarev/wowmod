/*
* File: bots.inc
* Description: Bots enchancement for wowmod
* Author(s): Stinkyfax
*/

#pragma semicolon 1

new g_ff_bot_profiles[65];

SetBotProfile(client, profile)  {
  g_ff_bot_profiles[client]=profile;
}
GetBotProfile(client) {
  return g_ff_bot_profiles[client];
}

new bool:g_ff_bot_shield[65];

BotHasShield(client)  {
  return g_ff_bot_shield[client];
}

BotPreloaded(client)  {
  g_ff_bot_shield[client] = false;
  KvRewind(StatsDB);
  decl String:key[10];
  if(SERVER_LEVEL_POLICY == 1)
    KvJumpToKey(StatsDB, "bots_high");
  else
    KvJumpToKey(StatsDB, "bots");
  new profiles = KvGetNum(StatsDB, "profiles");
  new profileStart = KvGetNum(StatsDB, "start_id", 1);
  new profile = GetRandomInt(profileStart, profileStart+profiles);
  SetBotProfile(client, profile);
  KvRewind(StatsDB);
  Format(key, sizeof(key), "bot_%d", profile);
  KvJumpToKey(StatsDB, key);

  //choose race
  new race = KvGetNum(StatsDB, "class", 1);
  wcSetClass(client, race);

  if(SERVER_LEVEL_POLICY == 1)
    wcSetStaticLevel(client, GetRandomInt(81,85));
  else
    if(CSGO)
    {
      new avrglvl = 30 + GetAverageLevel();
      if(avrglvl > 80)
        avrglvl = 80;
      if(55 > avrglvl)
        avrglvl = 55;
      wcSetStaticLevel(client, GetRandomInt(avrglvl-5,avrglvl+5));
    }
    else
      wcSetStaticLevel(client, GetRandomInt(60,80));
  new level = GetLevel(client);
  wcSetInt(client, 1);
  wcSetSta(client, 1);
  wcSetAgi(client, 1);
  wcSetStr(client, 1);
  wcSetPoints(client, level*2);
  spreadPoints(client);

  LearnUnlearnedSpell(client);

}

BotCalculateStats(client) {
  new profile = GetBotProfile(client);
  KvRewind(StatsDB);
  decl String:key[10];
  Format(key, sizeof(key), "bot_%d", profile);
  KvJumpToKey(StatsDB, key);

  SetDoubleHand(client, bool:KvGetNum(StatsDB, "2hand"));
  g_ff_bot_shield[client] = bool:KvGetNum(StatsDB,"shield");
  new Float:bufferf;
  bufferf=float(KvGetNum(StatsDB, "damage"));
  wcClientsBonusStats[client][DAMAGE]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "armor"));
  wcClientsBonusStats[client][ARMOR]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "str"));
  wcClientsStats[client][STR]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "agi"));
  wcClientsStats[client][AGI]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "int"));
  wcClientsStats[client][INT]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "sta"));
  wcClientsStats[client][STA]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "ai"));
  wcClientsBonusStats[client][ARMORIGNORE]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "regen"));
  wcClientsBonusStats[client][MPREGEN]+=bufferf;
  bufferf=KvGetFloat(StatsDB, "as");
  wcClientsBonusStats[client][ATTACKSPEED]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "spelldmg"));
  wcClientsBonusStats[client][SPELLDAMAGE]+=bufferf;
  bufferf=KvGetFloat(StatsDB, "speed");
  wcClientsBonusStats[client][MOVESPEED]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "res"));
  wcClientsBonusStats[client][RESISTANCE]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "resilence"));
  wcClientsBonusStats[client][RESILIENCE]+=bufferf;  //resilence
  bufferf=KvGetFloat(StatsDB, "ws", 1.0);
  SetWeaponSpeed(client, bufferf);
  bufferf=KvGetFloat(StatsDB, "cs");
  wcClientsBonusStats[client][CRITCHANCE]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "haste"));
  wcClientsStats[client][HASTE]+=bufferf;
  bufferf=float(KvGetNum(StatsDB, "parry"));
  wcClientsStats[client][PARRY]+=bufferf;
}

new Float:g_ff_bot_think[65];
new g_ff_bot_i=1;
BotTimer() {
  if(g_ff_bot_i > GetMaxClients())
    g_ff_bot_i = 1;
  new bot = g_ff_bot_i;
  if(g_ff_bot_think[bot] + 0.5 < GetEngineTime()) {
    g_ff_bot_think[bot] = GetEngineTime();
    BotThink(bot);
    g_ff_bot_i++;
  }
}

/**
 *  Main function which calls different AI actions
 *  BotThink is called each 0.5 seconds
 */
BotThink(client)  {
  if(!IsClientInGame(client) || !IsPlayerAlive(client) || !IsFakeClient(client))
    return;
  if(GetBoss(client)>=1)
  {
    BossThink(client);
  }
  else
    switch(GetClass(client))  {
      case 1: BotThinkWarrior(client);
      case 2: BotThinkRogue(client);
      case 3: BotThinkMage(client);
      case 4: BotThinkPaladin(client);
      case 5: BotThinkPriest(client);
      case 6: BotThinkShaman(client);
      case 7: BotThinkWarlock(client);
      case 8: BotThinkDK(client);
      case 9: BotThinkMonk(client);
    }
}

stock BotThinkWarrior(client) {

  //AOE damage spells
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,300.0);
  if(team==3)
    amount=GetNearClients("@t",location,targets,300.0);

  new aoetarget=GetAimTarget(client,110.0);
    
  if(amount > 1 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client))
    TryCast(client, 2); //Whirlwind
  if(amount > 2 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client))
    TryCast(client, 10);  //Thunder Clap

  new Float:hp = GetFloatHealth(client);
  if(hp < 0.5)
    TryCast(client, 3); //Regenerate
  if(hp < 0.7 && InCombat(client))
    TryCast(client, 12);  //Shield wall

}

stock BotThinkRogue(client) {

  //AOE damage spells
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,300.0);
  if(team==3)
    amount=GetNearClients("@t",location,targets,300.0);
    
  new aoetarget=GetAimTarget(client,110.0);
  
  if(amount >= 1 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client))
    TryCast(client, 13); //Fan of knives

  new Float:hp = GetFloatHealth(client);
  if(hp < 0.5 && InCombat(client))
    TryCast(client, 12); //Blind
  if(hp < 0.3 && InCombat(client))
    TryCast(client, 3);  //Vanish
    
  new vanishtarget=GetAimTarget(client,60.0);
  if(!InCombat(client) && vanishtarget!=0 && GetClientTeam(vanishtarget)!=GetClientTeam(client))
  {
    decl Float:clienta[3],Float:targeta[3];
    GetClientEyeAngles(client, clienta);
    GetClientEyeAngles(vanishtarget, targeta);
    new Float:angle=clienta[1]-targeta[1];
    if(angle<0.0)
    angle*=-1.0;
    if(angle>180.0)
    angle = 360.0-angle;
    if(angle<=90.0)
    {
      TryCast(client, 3);  //Vanish
      TryCast(client, 12); //Blind
    }
  }
  if(hp < 0.9 && !InCombat(client))
    TryCast(client, 7); //Sprint
  if(hp < 0.8 && InCombat(client))
    TryCast(client, 3); //Vanish
}

stock BotThinkMage(client) {

  //AOE damage spells
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,380.0);
  if(team==3)
    amount=GetNearClients("@t",location,targets,380.0);

  new aoetarget=GetAimTarget(client,110.0);
    
  if(amount >= 1 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client))
    TryCast(client, 2); //Frost nova
  if(amount >= 1 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client))
    TryCast(client, 7);  //Arcane Explosion
  if(amount >= 1 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client))
    TryCast(client, 12);  //Flame Orb

  new Float:hp = GetFloatHealth(client);
  if(hp < 0.5 && wcManaShield[client] < 10 && GetMana(client) > 300)
    TryCast(client, 9); //Mana Shield

  new target=GetAimTarget(client,25.0);
  new Float:targetHp = 0.0;
  if(target > 0)
    targetHp = GetFloatHealth(target);
  if(hp < 0.1 && targetHp > 0.8)
    TryCast(client, 10);  //Polymorph

}

stock BotThinkPaladin(client) {

  //AOE healing spells
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@t",location,targets,800.0);
  if(team==3)
    amount=GetNearClients("@ct",location,targets,800.0);

  new lowHps=0;
  for(new i=0; i < amount; i++)
    if(GetFloatHealth(targets[i]) < 0.8)
      lowHps++;
  if(lowHps > 2)
    TryCast(client, 18);  //aoe heal

  //Keep seal up
  if(GetSealT(client) < GetTickedTime() && GetMana(client) > 100)
    TryCast(client, 16);



  new Float:hp = GetFloatHealth(client);

  if(hp < 0.60)
    TryCast(client, 4);
  if(hp < 0.8)
    TryCast(client, 3);

  if(hp < 0.15 && InCombat(client) && GetRandomInt(1,4) == 1)
    TryCast(client, 2);


  new target=GetAimTarget(client,25.0);
  new Float:targetHp = 0.0;
  if(target > 0)
    targetHp = GetFloatHealth(target);
  if(target == 0)
    return;
  new bool:enemy = GetClientTeam(client) != GetClientTeam(target);
  if(!enemy && targetHp < 0.5)
    TryCast(client, 4);
  if(!enemy && targetHp < 0.8)
    TryCast(client, 3);
  if(enemy && GetHolyPower(client) > 2)
    TryCast(client, 8);
  if(GetClientSpeed(client) < 0.8 && GetMana(client) > 200)
    TryCast(client, 5);

}

stock BotThinkPriest(client) {

  //AOE healing spells
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@t",location,targets,800.0);
  if(team==3)
    amount=GetNearClients("@ct",location,targets,800.0);

  new lowHps=0;
  for(new i=0; i < amount; i++)
    if(GetFloatHealth(targets[i]) < 0.8)
      lowHps++;
  if(lowHps > 2)
    TryCast(client, 2);  //aoe heal


  new Float:hp = GetFloatHealth(client);

  if(hp < 0.60)
    TryCast(client, 3);
  if(hp < 0.8)
    TryCast(client, 1);

  if(hp < 0.9 && InCombat(client))
    TryCast(client, 4);


  new target=GetAimTarget(client,25.0);
  new Float:targetHp = 0.0;
  if(target > 0)
    targetHp = GetFloatHealth(target);
  TryCast(client, 8);
  if(hp < 0.3 && InCombat(client))
    TryCast(client, 11);
  if(target == 0)
    return;
  new bool:enemy = GetClientTeam(client) != GetClientTeam(target);
  if(!enemy && targetHp < 0.5)
    TryCast(client, 3);
  if(!enemy && targetHp < 0.8)
    TryCast(client, 1);
  if(!enemy && targetHp < 0.9 && InCombat(target))
    TryCast(client, 4);
  if(enemy && GetMana(target) > 300)
    TryCast(client, 6);

}

stock BotThinkShaman(client) {

  //AOE healing spells
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@t",location,targets,800.0);
  if(team==3)
    amount=GetNearClients("@ct",location,targets,800.0);

  new lowHps=0;
  for(new i=0; i < amount; i++)
    if(GetFloatHealth(targets[i]) < 0.8)
      lowHps++;
  if(lowHps > 2)
    TryCast(client, 11);  //aoe heal

  //Totems
  if(team==2)
    amount=GetNearClients("@ct",location,targets,380.0);
  if(team==3)
    amount=GetNearClients("@t",location,targets,380.0);
  new aoetarget=GetAimTarget(client,110.0);
  if(amount >= 1 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client))
    TryCast(client, 9); //nova totem
  if(amount >= 1 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client))
    TryCast(client, 10);  //earthbind totem

  new Float:hp = GetFloatHealth(client);

  if(hp < 0.60)
    TryCast(client, 6);

}

stock BotThinkWarlock(client) {

  //AOE damage spells
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,500.0);
  if(team==3)
    amount=GetNearClients("@t",location,targets,500.0);
  new aoetarget=GetAimTarget(client,110.0);
  if(amount >= 2 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client))
    TryCast(client, 4); //Fear


  new target=GetAimTarget(client,25.0);
  new Float:targetHp = 0.0;
  if(target > 0)
    targetHp = GetFloatHealth(target);
  if(targetHp > 0.5)
    TryCast(client, 7); //LifeDrain
  new Float:health = float(wcGetHealth(client)) / float(GetMaxHealth(client));
  if(0.5 > health && !InCombat(client))
    TryCast(client, 2);  //Soul Shatter

  new Float:hp = GetFloatHealth(client);
  if(hp < 0.8 && InCombat(client))
    TryCast(client,8);  //Metamorphos

  TryCast(client, 11);  //Infernal
  if(targetHp > 0.6)
    TryCast(client, 10);  //Curse

  if(hp > 0.5 && GetMana(client) < 200)
    TryCast(client, 5);
  if(hp > 0.9 && GetMana(client) < 1000)
    TryCast(client, 5);

}

stock BotThinkDK(client) {
  new Float:hp = GetFloatHealth(client);

  new target=GetAimTarget(client,25.0);
  new Float:targetHp = 0.0;
  if(target > 0)
    targetHp = GetFloatHealth(target);
  if(hp > 0.5)  {
    TryCast(client, 2);  //Death Grip
  }
  if(targetHp > 0.2)
    TryCast(client, 4); //Icy Touch

  TryCast(client, 6); //Death Coil
  TryCast(client, 9); //Raise Ally

}
new Float:g_fSerpentKickCD[65] = 0.0;
new Float:g_fRenewingMistCD[65] = 0.0;
new Float:g_fCocoonCD[65] = 0.0;
new g_iStanceStaved[65];
stock BotThinkMonk(client) {
  if(g_iStanceStaved[client]==0 && IsFakeClient(client))
  {
    new random = GetRandomInt(1,100);
    if(random>=66)
      _setMonkStance(client, 1);
    if(random>=33 && 66 > random)
      _setMonkStance(client, 2);
    if(random>0 && 33 > random)
      _setMonkStance(client, 3);
    g_iStanceStaved[client]=1;
    decl String:name[50];
    GetClientName(client,name,50);
  }
  new aoetarget=GetAimTarget(client,110.0);
  //AOE damage spells
  new targets[MAXPLAYERS];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,250.0);
  if(team==3)
    amount=GetNearClients("@t",location,targets,250.0);

  if(amount >= 1 && aoetarget!=0 && GetClientTeam(aoetarget)!=GetClientTeam(client)) {
    TryCast(client, 6); //Path of Blossoms
    TryCast(client, 5); //Spinning Crane Kick
  }
  new Float:hp = GetFloatHealth(client);
  if(GetMonkStance(client)==3 && 0.35 > hp)
    TryCast(client, 4);
  if(GetMonkStance(client)==1 && 0.50 > hp)
    TryCast(client, 4);
  new target=GetAimTarget(client,25.0);
  decl Float:locationt[3];
  if(target > 0)
  {
    GetClientAbsOrigin(target, locationt);
    new Float:dif = location[0]-locationt[0];
    if(dif<300.0)
      dif = location[1]-locationt[1];
    if(target > 0 && GetClientTeam(target) != team && dif > 300.0 && (GetEngineTime()>=g_fSerpentKickCD[client])) {
      wcRunTalent_9_SerpentKick(client);
      g_fSerpentKickCD[client] = GetEngineTime() + 10.0;
    }
    new Float:targethp = 0.00;
    if(target > 0)
      targethp = GetFloatHealth(target);
    if(target > 0 && team == GetClientTeam(target) && targethp < 0.30)
      TryCast(client, 10);
    if(target > 0 && team != GetClientTeam(target))
      TryCast(client, 10);
    if(hp < 0.9 || (targethp < 0.7 && target > 0))
      TryCast(client, 3);
    //if(target > 0 && 0.75 > hp)
      //wcRunTalent_9_TouchofKarma(client);
    if(GetMonkStance(client)==3 && team == GetClientTeam(target) && 0.40 > targethp && 300.0 > dif)
      TryCast(client, 4);
    if(GetMonkStance(client)==3 && team == GetClientTeam(target) && 0.50 > targethp && 650.0 > dif && (GetEngineTime()>=g_fRenewingMistCD[client]))
    {
      wcRunTalent_9_RenewingMist(client);
      g_fRenewingMistCD[client] = GetEngineTime() + 15.0;
    }
    if(GetMonkStance(client)==3 && team == GetClientTeam(target) && 0.80 > targethp && GetCombat(target) > 0 && (GetEngineTime()>=g_fCocoonCD[client]))
    {
      wcRunTalent_9_LifeCocoon(client);
      g_fCocoonCD[client] = GetEngineTime() + 25.0;
    }
    if(GetMonkStance(client)==3 && 0.35 > hp)
      TryCast(client, 4);
    if(GetMonkStance(client)==2 && team != GetClientTeam(target) && 225.0 > dif)
      TryCast(client, 4);
    if(GetMonkStance(client)==1 && 0.50 > hp)
      TryCast(client, 4);
  }
}