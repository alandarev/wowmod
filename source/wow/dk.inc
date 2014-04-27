/*
* File: dk.inc
* Description: Death Knight class
* Author(s): Stinkyfax
*/

#pragma semicolon 1

#define MAX_RUNES 3

new g_ff_runes[65];
new g_ff_runesTimer[65];


/*** DK Stances ***/
// 1: Blood Presence
// 2: Frost Presence
// 3: Unholy Presence

new g_ff_activeStance[65];
ActivateStance(client)  {
  if(GetClass(client)!=8) {
    _setStance(client, 0);
    return;
  }
  new blood = wcGetTreeTotal(client, getActiveSpec(client),1);
  new frost = wcGetTreeTotal(client, getActiveSpec(client),2);
  new unholy = wcGetTreeTotal(client, getActiveSpec(client),3);
  if(blood == 0 && frost == 0 && unholy == 0) {
    _setStance(client, 0);
    return;
  }
  new stance=0;
  if(blood>=frost)
    stance=1;
  else
    stance=2;
  if(unholy > blood && unholy > frost)
    stance=3;
  _setStance(client,stance);
  decl String:buffer[50];
  Format(buffer,50,"activated presence_%d",stance);
  WCMessage(client, buffer, client);

  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/presence_cast.wav",location);
}

_setStance(client, stance)  {
  g_ff_activeStance[client]=stance;
}

GetStance(client) {
  return g_ff_activeStance[client];
}


/*** DK Bones Shield ***/
new g_ff_bones[65];
GetBones(client)  {
  return g_ff_bones[client];
}
SetBones(client, amount) {
  g_ff_bones[client]=amount;
}

regenBoneShield(client) {
  new bones = GetBones(client);
  if(bones < 5)
    SetBones(client, bones + 1);
}

bool:SpendBone(client)  {
  if(GetClass(client)!=8 || wcGetTalentLevel(client,3,2,1)<=0)
    return false;
  if(GetBones(client)<=0)
    return false;
  SetBones(client, GetBones(client)-1);
  return true;
}

wcRunTalent_BoneShield(client, &damage) {

  if(SpendBone(client) && damage > 5) {
    decl String:line[255];
    wcGetTalentInfo(8,3,3,3,client,"block",line);
    new block = StringToInt(line);
    new check = damage - block;
    if(check < 0)
      damage = 0;
    else
      damage -=block;
    /*decl String:buffer[30];*/
    /*Format(buffer,sizeof(buffer),"%T", "bone shield", client);*/
    AddEMsgAff("bone shield", true);
  }
}


/*** DK Freeze ***/
new g_ff_freeze_dmg[65];
SetFreezeDmg(client, amount)  {
  g_ff_freeze_dmg[client]=amount;
}

GetFreezeDmg(client)  {
  return g_ff_freeze_dmg[client];
}

SpendFreeze(client,dmg) {
  new max = GetFreezeDmg(client);
  if(max<=0)
    return;
  new spend = GetMinInt(dmg,max);
  SetFreezeDmg(client, max - spend);
  if(max == spend)
    Unfreeze(client);
}

Unfreeze(client)  {
  if (!IsPlayerAlive(client))
  {
    return;
  }
  if (GetClientSpeed(client)>0.1)
  {
    return;
  }
  SetClientSpeed(client,GetNormalSpeed(client));
}

new bool:g_ff_iszombie[65];
new g_ff_zombieType[65];
bool:IsZombie(client) {
  return g_ff_iszombie[client];
}

new g_ff_internalZombie[65],g_ff_externalZombie[65];
__setZombie(client, caster, type=1) {
  g_ff_internalZombie[client]=caster;
  g_ff_zombieType[client]=type;
}
_getZombie(client)  {
  return g_ff_internalZombie[client];
}

stock _getZombieType(client) {
  return g_ff_zombieType[client];
}

new g_ff_externalZombieType[65];
_setZombie(client, caster, type)  {
  g_ff_externalZombie[client]=caster;
  g_ff_externalZombieType[client]=type;
  g_ff_iszombie[client] = (caster!=0);
  g_ff_ownsPet[caster]=client;
}

stock GetZombieType(client) {
  return g_ff_externalZombieType[client];
}

stock GetZombie(client) {
  return g_ff_externalZombie[client];
}
public GetOwnedPet(client) {
  return g_ff_ownsPet[client];
}
public Action:RunesTimer(Handle:timer, any:trash) {
  for(new client=1;client<=GetMaxClients(); client++)  {
    if(!IsClientInGame(client))
      continue;
    if(GetClass(client)!=8) {
      g_ff_runes[client]=0;
      g_ff_runesTimer[client]=0;
      continue;
    }
    _regenAntiMagic(client);
    if(GetRunes(client) == MAX_RUNES)  {
      g_ff_runesTimer[client]=0;
      continue;
    }
    g_ff_runesTimer[client]++;
    if(g_ff_runesTimer[client] % 5 == 0) // regen speed of bone shield counters
      regenBoneShield(client);
    if(g_ff_runesTimer[client]==8) {
      g_ff_runesTimer[client]=0;
      _addRunes(client, 1);
    }
  }

  return Plugin_Continue;
}

_setRunes(client, amount) {
  g_ff_runes[client]=amount;
}


_addRunes(client, amount)  {
  new runes = amount + GetRunes(client);
  runes = GetMinInt(runes, MAX_RUNES);
  if(GetRunes(client) < MAX_RUNES)  {

    PlaySound(client, "wc/spells/runes_regen.wav");
  }
  _setRunes(client, runes);
}

bool:SpendRune(client) {
  if(GetRunes(client) < 1)
    return false;
  _setRunes(client, GetRunes(client)-1);
  return true;
}

GetRunes(client)  {
  if(GetClass(client)!=8)
    return 0;
  return g_ff_runes[client];
}



new g_ff_antimagic[65];

getAntiMagic(client)  {
  return g_ff_antimagic[client];
}

setAntiMagic(client, amount)  {
  g_ff_antimagic[client] = amount;
}

_regenAntiMagic(client) {
  new amount = getAntiMagic(client) + ANTIMAGIC_REGEN;
  new max = MAX_ANTIMAGIC;
  if(CheckRequirements(client, 8, 3, 1, 3)) {
    decl String:line[255];
    wcGetTalentInfo(8,3,1,2,client,"absorb",line);
    max += StringToInt(line);
  }
  if(amount > max)
    amount = max;
  setAntiMagic(client, amount);
}

SpendAntiMagic(client, amount)  {
  new ret = GetMinInt(amount, getAntiMagic(client));
  setAntiMagic(client, getAntiMagic(client) - ret);
  if(ret>20)  {
    decl Float:location[3];
    GetClientAbsOrigin(client,location);
    EmitAmbientSound("wc/spells/antimagic.wav",location);
  }

  return ret;
}



/*** Spells Codes ***/
wcRunSpell_8_1(spellid,race,victim,client,&totaldmg) //Blood Strike
{
  new random=GetRandomInt(1,100);
  new chance=wcGetSpellInfoInt(race,spellid,"chance");
  if(chance>=random)
  {
    //DK specific
    new regen = wcGetSpellInfoInt(race,spellid,"regen");
    if(regen>0) AddMana(client, regen);

    new Float:multi=wcGetSpellInfoFloat(race,spellid,"multi");

    new dmg=RoundToNearest(float(totaldmg)*multi);
    totaldmg+=dmg;

    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    EmitAmbientSound("wc/spells/dk_strike.wav",location);

    /*decl String:buffer[30];*/
    /*Format(buffer,sizeof(buffer),"%T", "blood strike", client);*/
    AddEMsgAff("blood strike", true);
    new Float:pos[3];
    GetClientAbsOrigin(victim,pos);
    VS_BeamRingPoint(pos, 90.00, 60.00, g_ef_glow1, g_ef_lgtning, 10, 10, 0.50, 50.00, 0.00, {255, 102, 0, 255}, 60, 0);
    pos[2]+=45.0;
    VS_BeamRingPoint(pos, 50.00, 260.00, g_ef_particle_noisesphere, g_ef_lgtning, 10, 10, 0.50, 15.00, 0.00, {255, 12, 0, 255}, 6, 0);
    /*if (IsValidEntity(g_iEnvBlood))*/
      /*AcceptEntityInput(g_iEnvBlood, "EmitBlood", victim);*/
  }
  return totaldmg;
}

wcRunSpell_8_2(spellid,race,client) //Death Grip
{
  //Standard Targetting
  new target=GetAimTarget(client,15.0);
  if(target<=0) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"no target",client);
    return;
  }
  if((GetClientTeam(client)==GetClientTeam(target)) || (!IsPlayerAlive(target)) ) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
    return;
  }

  //DK specific
  new runes = wcGetSpellInfoInt(race,spellid,"runes");
  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }
  new regen = wcGetSpellInfoInt(race,spellid,"regen");
  if(regen>0) AddMana(client, regen);

  //Cooldown
  new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
  SetCdSpell(client, cdtype, cd, spellid);

  //Take mana off
  new manareq=GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);


  //Spell Specific variables
  new Float:zPower = wcGetSpellInfoFloat(race,spellid,"z");
  new Float:loc1[3];
  new Float:loc2[3];
  GetClientAbsOrigin(client,loc1);
  GetClientAbsOrigin(target,loc2);
  new Float:multi =wcGetSpellInfoFloat(race,spellid,"push");
  new Float:power = 175 + GetDistanceBetween(loc1,loc2) * multi;

  //Spell code
  //if not boss
  if(GetBoss(target)<=0 || (!IsBoss()) )
    PushPlayerTowardsPlayer(target, client, power, zPower);

  //Effect
  new Float:targpos[3];
  new Float:pos[3];
  GetClientAbsOrigin(client,pos);
  GetClientAbsOrigin(target,targpos);
  pos[2]+=45.0;
  targpos[2]+=45.0;
  //TE_SetupBeamRingPoint(targpos,45.0,44.0,g_ef_lgtning,HaloSprite,0,15,3.0,5.0,50.0,{255,0,255,255},0,0);
  //TE_SendToAll();
  //TE_SetupBeamPoints(pos, targpos, g_purplelaser1, g_purplelaser1, 0, 8, 0.5, 10.0, 10.0, 10, 10.0, {255,255,255,255}, 70);
  //TE_SendToAll();	
  VS_BeamRingPoint(pos, 50.00, 150.00, g_ef_lgtning, g_ef_lgtning, 10, 10, 1.00, 500.00, 0.00, {51, 51, 255, 255}, 10, 0);
  VS_BeamRingPoint(pos, 50.00, 150.00, g_ef_lgtning, g_ef_lgtning, 10, 10, 1.00, 500.00, 0.00, {102, 51, 255, 255}, 10, 0);
  pos[2] += 40.0;
  VS_BeamPoints(pos, targpos, g_ef_lgtning, g_ef_lgtning, 1, 5, 1.00, 15.00, 15.00, 1, 100.00, {51, 51, 255, 255}, 200);
  VS_BeamPoints(pos, targpos, g_ef_lgtning, g_ef_lgtning, 1, 5, 1.00, 15.00, 15.00, 1, 100.00, {102, 51, 255, 255}, 200);

  decl Float:location[3];
  GetClientAbsOrigin(target,location);
  EmitAmbientSound("wc/spells/deathgrip.wav", location);


  new threatBonus = wcGetSpellInfoInt(race, spellid, "threat");
  //Threat
  if(IsBoss() && (g_iAggro != client) && (GetBoss(client) <= 0))  { //No effect if top threat is caster
    new threat = GetThreat(g_iAggro) + threatBonus;
    SetThreat(client, threat);
  }

  //Message
  decl String:name[30],String:name2[30];
  GetClientName(client,name,30);
  GetClientName(target,name2,30);
  WCMessage(client,"death grip caster", client, name2);
  WCMessage(target,"death grip target", target, name);
}

wcRunSpell_8_3(spellid,race,victim,client,&totaldmg) //Plague Strike
{
  new random=GetRandomInt(1,100);
  new chance=wcGetSpellInfoInt(race,spellid,"chance");
  new bool:glyphed = false;
  if(GetClass(client)==8 && wcGetTalentLevel(client,3,3,1)>0)
  {
    //If you have the Scourge Strike talent, your Plague Strike will also reduce the damage the target deals by 10%.
    glyphed = HasGlyph(client, GLYPH_DK_SCOURGE); //Is in right place!
    decl String:line[255];
    wcGetTalentInfo(8,3,3,1,client,"multi",line);
    chance==RoundToNearest(float(chance)*StringToFloat(line));
  }
  if(chance>=random)
  {
    //DK specific
    new regen = wcGetSpellInfoInt(race,spellid,"regen");
    if(regen>0) AddMana(client, regen);

    new Float:multi=wcGetSpellInfoFloat(race,spellid,"multi");

    new dmg=RoundToNearest(float(totaldmg)*multi);
    totaldmg+=dmg;

    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    EmitAmbientSound("wc/spells/dk_strike.wav",location);


    /*decl String:buffer[30];*/
    /*Format(buffer,sizeof(buffer),"%T", "plague strike", client);*/
    AddEMsgAff("plague strike", true);
    new Float:pos[3];
    GetClientAbsOrigin(victim,pos);
    VS_BeamRingPoint(pos, 50.00, 49.00, g_ef_glow1, g_ef_lgtning, 10, 10, 0.50, 60.00, 0.00, {0, 204, 0, 255}, 10, 0);
    pos[2]+=45.0;
    VS_BeamRingPoint(pos, 50.00, 49.00, g_ef_glow1, g_ef_lgtning, 10, 10, 0.50, 60.00, 0.00, {0, 204, 0, 255}, 10, 0);
    new Float:tickdelay=wcGetSpellInfoFloat(race,spellid,"tickdelay");
    new ticks=wcGetSpellInfoInt(race,spellid,"ticks");
    if(CheckRequirements(client,8,3,1,2)) {
      decl String:line[255];
      wcGetTalentInfo(8,3,1,2,client,"bonus",line);
      new bonusTicks = StringToInt(line);
      wcGetTalentInfo(8,3,1,2,client,"chance",line);
      new chance2 = StringToInt(line);
      if(chance2 >= GetRandomInt(1, 100))
        ticks += bonusTicks;
    }
    ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
    tickdelay = tickdelay*(1.0-GetHasteEffect(client));
    if(CheckRequirements(client,8,3,2,4)) //Rapid Contagion
      wcRunTalent_8_RapidContagion(client,tickdelay);

    new aoe=wcGetSpellInfoInt(race,spellid,"aoe");

    new dmgef = wcGetSpellInfoInt(race,spellid,"baseef");
    dmgef += RoundToNearest(wcGetSpellInfoFloat(race,spellid,"multief")*GetDamage(client));

    new team=GetClientTeam(victim);
    new effid=CreateEffectData(victim,client,ticks,dmgef,aoe,team,0,803,g_RoundCount,GetClientDeaths(victim));
    if (tickdelay==0.0 || ticks == 0 || dmgef == 0)
    {
      LogError("ERROR: plague strike spell corrupted.");
      DeleteEffectData(effid);
    }
    else if(effid!=0)
    {
      CreateTimer(tickdelay, EffectAoeDamage, effid, TIMER_REPEAT);
    }
    if(CheckRequirements(client,8,3,2,1))
    {
      SetPlagued(victim,client);
      SetEffectSpellVic(victim,48);
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,victim);
      CreateTimer(tickdelay*ticks,EndPlaguebringer,datapack4,TIMER_HNDL_CLOSE);
    }
    if(glyphed)
      GiveDamageBuff(victim, 0.9, tickdelay*ticks);


  }
  return totaldmg;
}

public Action:EndPlaguebringer(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  RemoveEffectVictim(client,48);
  RemoveEffectSpellVic(client,48);
}

wcRunSpell_8_4(spellid,race,client)  { //Icy Touch
  //Standard Targetting
  new target=GetAimTarget(client,15.0);
  if(target<=0) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"no target",client);
    return;
  }
  if((GetClientTeam(client)==GetClientTeam(target)) || (!IsPlayerAlive(target)) ) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
    return;
  }


  //DK specific
  new runes = wcGetSpellInfoInt(race,spellid,"runes");
  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }
  new regen = wcGetSpellInfoInt(race,spellid,"regen");
  if(GetStance(client)==2)
    regen+=RoundToNearest(float(regen)*0.50);
  if(regen>0) AddMana(client, regen);

  //Cooldown
  new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
  SetCdSpell(client, cdtype, cd, spellid);

  //Take mana off
  new manareq=GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);


  //Spell Specific variables

  //Slowdown
  new Float:oldspeed=GetClientSpeed(target);
  new Float:newspeed=GetNormalSpeed(target);
  new Float:time=wcGetSpellInfoFloat(race,spellid,"duration");
  new Float:multi=wcGetSpellInfoFloat(race,spellid,"multi");
  if(CheckRequirements(client, 8, 2, 1, 3)) {
    decl String:line[255];
    wcGetTalentInfo(8,2,1,3,client,"multi",line);
    new Float:talentMulti = StringToFloat(line);
    multi -= talentMulti;
  }

  newspeed*=multi;
  if(newspeed<oldspeed)
  {
    SetClientSpeed(target,newspeed);
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,target); // Write the client index of owner
    if(multi>0.1)
    {
      WritePackFloat(datapack,newspeed);
      WritePackFloat(datapack,multi);
      CreateTimer(time,RemoveSpeedEffect,datapack,TIMER_HNDL_CLOSE);
    }
    else
    {
      WritePackFloat(datapack,newspeed);
      WritePackFloat(datapack,oldspeed);
      CreateTimer(time,RemoveFreezeEffect,datapack,TIMER_HNDL_CLOSE);
    }
  }


  //Debuff
  new Float:tickdelay=wcGetSpellInfoFloat(race,spellid,"tickdelay");
  new ticks=wcGetSpellInfoInt(race,spellid,"ticks");
  new aoe=wcGetSpellInfoInt(race,spellid,"aoe");
  ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
  tickdelay = tickdelay*(1.0-GetHasteEffect(client));
  new dmgef = wcGetSpellInfoInt(race,spellid,"baseef");
  dmgef += RoundToNearest(wcGetSpellInfoFloat(race,spellid,"multief")*GetDamage(client));
  if(HasGlyph(client, GLYPH_DK_HOWLING))  {
    dmgef = RoundToNearest(1.2 * dmgef);
  }

  new team=GetClientTeam(target);
  new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,0,804,g_RoundCount,GetClientDeaths(target));
  if (tickdelay==0.0 || ticks == 0 || dmgef == 0)
  {
    LogError("ERROR: icy touch spell corrupted.");
    DeleteEffectData(effid);
  }
  else
    if(effid!=0)
    {
      CreateTimer(tickdelay, EffectAoeDamage, effid, TIMER_REPEAT);
    }

  decl Float:location[3];
  GetClientAbsOrigin(target,location);
  EmitAmbientSound("wc/spells/icytouch.wav", location);

  //Effect
  VS_BeamRingPoint(location, 20.00, 250.00, g_ef_lgtning, g_ef_lgtning, 1, 1, 2.00, 30.00, 0.00, {240, 255, 255, 255}, 10, 0);
  VS_BeamRingPoint(location, 19.00, 20.00, g_ef_lgtning, g_ef_lgtning, 100, 300, 3.00, 60.00, 1.00, {192, 240, 255, 255}, 30, 0);

  //Message
  decl String:name[30],String:name2[30];
  GetClientName(client,name,30);
  GetClientName(target,name2,30);
  WCMessage(client,"icy touch caster", client, name2, dmgef);
  WCMessage(target,"icy touch target", target, name);
}

wcRunSpell_8_5(spellid,race,victim,client,&totaldmg) //Death Strike
{
  new random=GetRandomInt(1,100);
  new chance=wcGetSpellInfoInt(race,spellid,"chance");
  if(chance>=random)
  {
    //CD check
    new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
    new Float:timeleft=GetCDSpell(client,cdtype)-GetTickedTime();
    if(timeleft > 0.0)
      return totaldmg;


    //DK specific
    new runes = wcGetSpellInfoInt(race,spellid,"runes");
    if(runes>0)
      if(!SpendRune(client))  {
        WCMessage(client, "no runes", client);
        return totaldmg;
    }
    new regen = wcGetSpellInfoInt(race,spellid,"regen");
    if(regen>0) AddMana(client, regen);

    //Cooldown

    new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
    SetCdSpell(client, cdtype, cd, spellid);


    new Float:multi=wcGetSpellInfoFloat(race,spellid,"multi");

    new Float:talentBonus;
    decl String:line[255];
    wcGetTalentInfo(8,1,3,3,client,"multi",line);
    talentBonus = StringToFloat(line);
    if(talentBonus > 1.0)
      multi *= talentBonus;

    new dmg=RoundToNearest(float(totaldmg)*multi);
    totaldmg+=dmg;

    new Float:multief=wcGetSpellInfoFloat(race,spellid,"multief");
    if(talentBonus > 1.0)
      multief *= talentBonus;
    RestoreHealthP(client, multief);

    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    EmitAmbientSound("wc/spells/dk_strike.wav",location);

    /*decl String:buffer[30];*/
    /*Format(buffer,sizeof(buffer),"%T", "death strike", client);*/
    AddEMsgAff("death strike", true);

  }
  return totaldmg;
}


wcRunSpell_8_6(spellid,race,client)  {
  //Standard Targetting
  new target=GetAimTarget(client,15.0);
  if(target<=0) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"no target",client);
    return;
  }
  if((GetClientTeam(client)==GetClientTeam(target)) || (!IsPlayerAlive(target)) ) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
    return;
  }

  //DK specific
  new runes = wcGetSpellInfoInt(race,spellid,"runes");
  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }
  new regen = wcGetSpellInfoInt(race,spellid,"regen");
  if(regen>0) AddMana(client, regen);

  //Cooldown
  new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
  SetCdSpell(client, cdtype, cd, spellid);

  //Take mana off
  new manareq=GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);


  //Spell Specific variables
  new Float:multi = wcGetSpellInfoFloat(race,spellid,"multi");
  new damage = RoundToNearest(float(GetDamage(client)) * multi+wcGetSpellInfoInt(race,spellid,"basedmg"));

  if(CheckRequirements(client, 8, 2, 1, 2)) {
    decl String:line[255];
    wcGetTalentInfo(8,2,1,2,client,"multi",line);
    new Float:talentMulti = StringToFloat(line);
    damage = RoundToNearest(float(damage) * talentMulti);
  }

  //Spell code
  new Float:location[3],Float:locationatt[3];
  GetClientAbsOrigin(target,location);
  GetClientAbsOrigin(client,locationatt);
  EmitAmbientSound("wc/spells/deathcoil.wav",location);
  location[2]+=40;
  locationatt[2]+=40;
  
  VS_BeamPoints(location, locationatt, g_ef_scanner,g_ef_scanner, 1, 5, 3.00, 15.00, 15.00, 1, 0.00, {153, 0, 153, 255}, 20);
  VS_BeamPoints(location, locationatt, g_ef_scanner, g_ef_scanner, 1, 5, 3.00, 15.00, 15.00, 1, 0.00, {153, 0, 153, 255}, 20);
  VS_BeamPoints(locationatt, location, g_ef_glow1, g_ef_glow1, 1, 5, 3.00, 20.00, 10.00, 10, 0.00, {153, 0, 153, 255}, 160);
  VS_BeamPoints(locationatt, location, g_ef_lgtning, g_ef_glow1, 1, 5, 3.00, 20.00, 10.00, 0, 0.00, {153, 0, 153, 255}, 160);
  VS_BeamRingPoint(location, 90.00, 60.00, g_ef_glow1, g_ef_lgtning, 10, 10, 3.00, 50.00, 0.00, {153, 0, 153, 255}, 60, 0);


  //Message
  decl String:vicMsg[300], String:attMsg[300];
  Format(vicMsg,sizeof(vicMsg),"%T","death coil target", target);
  Format(attMsg,sizeof(attMsg),"%T","death coil caster", client);
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"name",line);
  wcSpellDamage(target,client,damage, attMsg, vicMsg,line);
}


wcRunSpell_8_7(victim,&totaldmg)
{
  new absorbed = SpendAntiMagic(victim, totaldmg);
  if(absorbed<=0)
    return totaldmg;
  totaldmg-=absorbed;
  AddEMsgAff("anti magic",true );

  new Float:pos[3];
  GetClientAbsOrigin(victim,pos);
  pos[2]+=35;
  TE_SetupGlowSprite(pos, AMSSprite, 0.2, 2.5, 10);
  TE_SendToAll();

  return totaldmg;
}


wcRunSpell_8_8(spellid,race,client)  {
  //Standard Targetting
  new target=GetAimTarget(client,15.0);
  if(target<=0) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"no target",client);
    return;
  }
  if((GetClientTeam(client)==GetClientTeam(target)) || (!IsPlayerAlive(target)) || (!CanSilence(target))) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
    return;
  }


  //DK specific
  new runes = wcGetSpellInfoInt(race,spellid,"runes");
  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }
  new regen = wcGetSpellInfoInt(race,spellid,"regen");
  if(regen>0) AddMana(client, regen);

  //Cooldown
  new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
  SetCdSpell(client, cdtype, cd, spellid);

  //Take mana off
  new manareq=GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);


  //Spell Specific variables
  new Float:duration = wcGetSpellInfoFloat(race,spellid,"duration");
  if(CheckRequirements(client, 8, 3, 1, 3)) {
    decl String:line[255];
    wcGetTalentInfo(8,3,1,3,client,"duration",line);
    duration += StringToFloat(line);
  }

  SetSilence(target,duration);

  decl Float:location[3];
  GetClientAbsOrigin(target,location);
  EmitAmbientSound("wc/spells/mindfreeze.wav", location);

  //Message
  decl String:name[30],String:name2[30];
  GetClientName(client,name,30);
  GetClientName(target,name2,30);
  WCMessage(client,"mind freeze caster", client, name2, duration);
  WCMessage(target,"mind freeze target", target, name, duration);
}

public wcRunSpell_8_9(spellid, race, client)  {
  if(GetOwnedPet(client) != -1)
  {
    WCMessage(client,"already have ghoul",client);
    return;
  }
  else
    ChooseTarget(client, 8,9, spellid, race, "Choose Player to cast Ghoul");
}

public wcRunSpell_8_9_2(spellid,race,client,target)  {
  decl String:line[255];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetSpellInfo(race,spellid,"distance",line);
  if(target <= 0) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"no target",client);
    return;
  }
  //Cooldown
  new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
  SetCdSpell(client, cdtype, cd, spellid);

  //Take mana off
  new manareq=GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);

  //DK specific
  new runes = wcGetSpellInfoInt(race,spellid,"runes");
  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }
  new regen = wcGetSpellInfoInt(race,spellid,"regen");
  if(regen>0) AddMana(client, regen);

  EmitAmbientSound("wc/spells/raiseally.wav", location);

  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  WCMessage(target,"raise ally target",target,name);
  WCMessage(client,"raise ally caster",client,name2);
  __setZombie(target, client);
  CS_RespawnPlayer(target);
  CreateTimer(0.1,GhoulEffect,target);

}

public Action:GhoulEffect(Handle:timer, any:client)
{
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  new Float:location[3];
  location[0] = vClientCorpse[client][0];
  location[1] = vClientCorpse[client][1];
  location[2] = vClientCorpse[client][2];
  location[2] -= 35.0;
  new color[4]={80,0,0,240};
  TeleportEntity(client,location,NULL_VECTOR,NULL_VECTOR);

  SetEntProp(client, Prop_Send, "m_ArmorValue", 100, 1);
  SetEntProp(client, Prop_Send, "m_bHasHelmet", 1, 1);
  if(!BeamRingEffect("@all",location,10.0,500.0,g_ef_lgtning,0,10,2.0,100.0,color,0.0,10))
    LogError("didn't create effect #763");

  //Model
  new team = GetClientTeam(client);
  if(GetZombieType(client) == 1)  switch(team)  {
    case 2:
      if(!CSGO)
        SetEntityModel(client,"models/player/slow/zombie_package/slow_5.mdl");
    case 3:
      if(!CSGO)
        SetEntityModel(client,"models/player/slow/zombie_package/slow_4.mdl");
  }
  else if(GetZombieType(client) == 2) switch(team)  {
    //TODO: infernal model
    case 2: {
      WCMessage(client, "boss spell blocked", client);
      if(!IsBoss())
        if(!CSGO)
          SetEntityModel(client,"models/player/drcube/firegolem/firegolem.mdl");
      new color2[4] = { 255, 200, 255, 255 };
      SetClientColor(client, color2);
    }
    case 3: {
      if(!IsBoss())
        if(!CSGO)
          SetEntityModel(client,"models/player/drcube/firegolem/firegolem.mdl");
      new color2[4] = { 200, 255, 255, 255 };
      SetClientColor(client, color2);
    }
  }

  //RestrictWeapons(client);
  return Plugin_Stop;
}


/*** Active Talents ***/
public wcRunTalent_RuneTap(client)
{
  decl String:line[255];

  /* wcGetTalentInfo(8,1,4,1,client,"runes",line); */
  /* new runes=StringToInt(line); */
  new spent=0;
  for(;(SpendRune(client));spent++){}
  if(spent<1)
  {
    WCMessage(client, "no runes", client);
    return;
  }

  wcGetTalentInfo(8,1,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(8,1,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  /* new manareq = GetTalentCost(client, 1,3,1); */

  /* wcSetMana(client,GetMana(client)-manareq); */
  SetCdSpell(client,cdtype,cd, 0, "Rune Tap");

  wcGetTalentInfo(8,1,4,1,client,"ticks",line);
  new ticks=StringToInt(line);
  wcGetTalentInfo(8,1,4,1,client,"multi",line);
  new Float:multi = float(spent)*StringToFloat(line);
  new bhealth = RoundToNearest((multi*0.5)*float(GetMaxHealth(client)));
  new hot     = RoundToNearest((multi*0.5)*float(GetMaxHealth(client)/ticks));

  new bool:glyph=HasGlyph(client, GLYPH_DK_RUNETAP);
  if(glyph)
  {
    wcGetTalentInfo(8,1,4,1,client,"glyph",line);
    hot += RoundToNearest(StringToFloat(line)*float(GetMaxHealth(client)/ticks));
    multi+=StringToFloat(line);
  }

  wcGetTalentInfo(8,1,4,1,client,"tickdelay",line);
  new Float:tickdelay=StringToFloat(line);

  Renew(client, client, hot, ticks, 0, tickdelay); //0=nocrit
  bhealth = wcHeal(client,client,bhealth,0,false);

  if(glyph)
    WCMessage(client,"Rune Tap glyph",client,RoundToNearest(multi*100),bhealth,ticks*hot,ticks*tickdelay);
  else
    WCMessage(client,"Rune Tap",client,RoundToNearest(multi*100),bhealth,ticks*hot,ticks*tickdelay);

  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/runetap.wav", location);
}

wcRunTalent_Vampirism(client, damage) {
  if(!CheckRequirements(client, 8,1,1,3))
    return;
  decl String:line[255];
  wcGetTalentInfo(8,1,1,3,client,"multi",line);
  new restore = RoundToNearest(  float(damage) * StringToFloat(line) );
  if(restore>0)
    wcHeal(client,client,restore,0,false); //No crit heal due to amount healed controlled by damage dealt

}

wcRunTalent_Rampage(client, damage) {
  if(!CheckRequirements(client, 1, 2, 4, 2))
    return;
  decl String:line[255];
  wcGetTalentInfo(1,2,4,2,client,"multi",line);
  new restore = RoundToNearest(  float(damage) * StringToFloat(line) );
  if(restore>0)
    wcHeal(client,client,restore,0,false); //No crit heal due to amount healed controlled by damage dealt

}


wcRunTalent_RogueVampirism(client, damage, victim) {
  if(!CheckRequirements(client,2,1,4,2))
    return;
  decl String:line[255];
  wcGetTalentInfo(2,1,4,2,client,"drain",line);
  new restore = RoundToNearest(  float(damage) * StringToFloat(line) );
  if(restore>0)
    wcHeal(client,client,restore,0,false);

  wcGetTalentInfo(2,1,4,2,client,"debuff",line);
  SetHM(victim,1.0-StringToFloat(line));
  wcGetTalentInfo(2,1,4,2,client,"duration",line);
  SetHMT(victim,StringToFloat(line));
}


wcRunTalent_VampiricBlood(client) { // 1 -> 2 -> 2
  decl String:line[255];

  wcGetTalentInfo(8,1,2,2,client,"runes",line);
  new runes=StringToInt(line);
  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }

  wcGetTalentInfo(8,1,2,2,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(8,1,2,2,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 1,2,2);

  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Vampiric");

  wcGetTalentInfo(8,1,2,2,client,"duration",line);
  new Float:duration=StringToFloat(line);
  wcGetTalentInfo(8,1,2,2,client,"multi",line);
  new Float:multief=StringToFloat(line);
  if(HasGlyph(client, GLYPH_DK_VAMBLOOD))
  {
    multief=multief*2;
    new Float:bhealth = 0.0;
    WCMessage(client,"Vampiric Blood",client,bhealth);
  }
  else
  {
    //+8% hp
    wcGetTalentInfo(8,1,2,2,client,"heal",line);
    new Float:multi = StringToFloat(line);

    new bhealth = RoundToNearest(multi*float(GetMaxHealth(client)));
    WCMessage(client,"Vampiric Blood",client,bhealth);
    bhealth = wcHeal(client,client,bhealth,0,false);
  }
  SetHM(client,1.0+multief);
  SetHMT(client,duration);

  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/vampiric.wav", location);
  new Float:locationvbfx[3];
  locationvbfx[2]+=100;
  GetClientAbsOrigin(client,locationvbfx);
  //VS_GlowSprite(locationvbfx, modelId(materials\effects/ar2_altfire1.vmt), 3.00, 0.30, 255); ar2_altfire1 is bugged in OB version
  VS_GlowSprite(locationvbfx, g_ef_yellowflare, 3.00, 0.30, 255);
  VS_GlowSprite(locationvbfx, g_redglow1, 3.00, 1.00, 255);
  VS_BeamRingPoint(locationvbfx, 5.00, 8.00, g_ef_lgtning, g_ef_lgtning, 100, 300, 3.00, 10.00, 1.00, {255, 24, 25, 255}, 30, 0);
}

public wcRunTalent_FrostStrike(client, victim) // Frost Strike
{
  decl String:line[255];
  wcGetTalentInfo(8,2,3,2,client,"mana",line);
  new mana = StringToInt(line);
  if(mana<=GetMana(client))
  {
    wcGetTalentInfo(8,2,3,2,client,"chance",line);
    new chance = StringToInt(line);
    if(HasGlyph(client, GLYPH_DK_FROST))
      chance +=5;
    new random = GetRandomInt(1,100);
    if(IsDoubleHand(client))
      chance+=RoundToNearest(float(chance)*0.50);
    if ( chance >= random )
    {
      wcSetMana(client,GetMana(client)-mana);
      wcGetTalentInfo(8,2,3,2,client,"multi",line);
      new Float:multi = StringToFloat(line);
      new damage = RoundToNearest(multi*GetDamage(client));

      wcGetTalentInfo(4,1,3,1,client,"name",line, false);
      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","froststrike target", victim);
      Format(attMsg,sizeof(attMsg),"%T","froststrike caster", client);
      wcSpellDamage(victim,client, damage, attMsg, vicMsg, line);
    }
  }
}


wcRunTalent_HungeringCold(client) { // 2 -> 3 -> 1
  decl String:line[255];

  wcGetTalentInfo(8,2,3,1,client,"runes",line);
  new runes=StringToInt(line);
  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }


  wcGetTalentInfo(8,2,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(8,2,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 2,3,1);

  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Hungering");

  wcGetTalentInfo(8,2,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  wcGetTalentInfo(8,2,3,1,client,"maxdmg",line);
  new maxdmg=StringToInt(line);

  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetTalentInfo(8,2,3,1,client,"distance",line);
  new Float:distance=StringToFloat(line);
  new Float:slowmulti=0.0;
  new Float:slowtime=duration;

  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,distance);
  if(team==3)
    amount=GetNearClients("@t",location,targets,distance);
  decl String:name[255];
  GetClientName(client,name,255);
  decl String:name2[255];
  new String:stargets[255];
  for (new x=0;x<=(amount-1);x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
    WCMessage(targets[x],"hungeringcold target",targets[x],name);
  }
  if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);
  WCMessage(client,"hungeringcold cast",client,stargets);
  EmitAmbientSound("wc/spells/hungeringcold.wav",location);
  //CreateTimer(0.0,FrostNovaEffect,client);


  for (new x=0;x<=(amount-1);x++)
  {
    new Float:oldspeed=GetClientSpeed(targets[x]);
    new Float:newspeed=GetNormalSpeed(targets[x]);
    newspeed*=slowmulti;
    if(newspeed<oldspeed)
    {
      SetClientSpeed(targets[x],newspeed);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,targets[x]); // Write the client index of owner
      WritePackFloat(datapack2,newspeed);
      WritePackFloat(datapack2,oldspeed);
      CreateTimer(slowtime,RemoveFreezeEffect,datapack2,TIMER_HNDL_CLOSE);
      SetFreezeDmg(targets[x],maxdmg);

      GetClientAbsOrigin(targets[x], location);
      VS_BeamRingPoint(location, 19.00, 20.00, g_ef_lgtning, g_ef_lgtning, 100, 300, 3.00, 800.00, 1.00, {64, 32, 127, 255}, 100, 0);
      location[2]+=50.0;
      VS_BeamRingPoint(location, 19.00, 20.00, g_ef_lgtning, g_ef_lgtning, 100, 300, 3.00, 800.00, 1.00, {64, 32, 127, 255}, 100, 0);
      location[2]+=100.0;
      VS_BeamRingPoint(location, 19.00, 20.00, g_ef_lgtning, g_ef_lgtning, 100, 300, 3.00, 800.00, 1.00, {64, 32, 127, 255}, 100, 0);

    }
  }

}


wcRunTalent_BladeBarrier(client, &damage) { // 1 -> 1 -> 1
  decl String:line[255];

  wcGetTalentInfo(8,1,1,1,client,"multi",line);
  new Float:multi = StringToFloat(line);
  wcGetTalentInfo(8,1,1,1,client,"multi1",line);
  new Float:multi1 = StringToFloat(line);
  wcGetTalentInfo(8,1,1,1,client,"multi2",line);
  new Float:multi2 = StringToFloat(line);
  if(GetRunes(client)>=3)
    return;
  if(GetRunes(client)==2)
    damage = RoundToNearest(float(damage) * multi1);
  if(GetRunes(client)==1)
    damage = RoundToNearest(float(damage) * multi2);
  if(GetRunes(client)==0)
    damage = RoundToNearest(float(damage) * multi);
  AddEMsgAff("blade barrier", true);
}

wcRunTalent_Butchery(client)  {
  decl String:line[255];
  wcGetTalentInfo(8,1,3,2,client,"multi",line);
  new Float:multi = StringToFloat(line);
  new bhealth = RoundToNearest(multi*float(GetMaxHealth(client)));
  bhealth = wcHeal(client,client,bhealth,0,false);
  WCMessage(client, "butchery cast", client, bhealth);

  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/butchery.wav", location);
}

/*wcRunTalentDKDual(client)
{
  new leftitem[UniqueItem],rightitem[UniqueItem];
  GetCharItem(leftitem, client, 18);
  GetCharItem(rightitem, client, 22);
  if(leftitem[Item_category] != 22 || rightitem[Item_category] != 22)
    return;
  new Float:multi = 0.12;
  if(multi <= 0.0)
    return;
  new  leftbonus = RoundToCeil(multi*( leftitem[Item_damage]+ leftitem[Item_weapon_ap]));
  new rightbonus = RoundToCeil(multi*(rightitem[Item_damage]+rightitem[Item_weapon_ap]));
  if((leftbonus + rightbonus) <= 0)
    return;
  SetBonusDamage(client, float(GetBonusDamage(client)+leftbonus+rightbonus));
  wcRecalculateStats(client);
  WCMessage(client, "dual wield spec used", client, leftbonus+rightbonus, GetDamage(client));
}*/

wcRunTalent_8_Bloodtrail(client)
{
  decl String:line[255];
  wcGetTalentInfo(8,1,2,4,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if (chance >= rand)
    _addRunes(client, 1);
}

wcRunTalent_8_RuneWarding(client, &damage) //Rune Warding
{
  decl String:line[255];
  wcGetTalentInfo(8,1,2,1,client,"health",line);
  new Float:hp=StringToFloat(line);
  new Float:health= float(wcGetHealth(client))/float(GetMaxHealth(client));
  if(hp >= health)
  {
    wcGetTalentInfo(8,1,2,1,client,"chance",line);
    new chance = StringToInt(line);
    new rand = GetRandomInt(1,100);
    if(chance >= rand)
      if(SpendRune(client))
      {
        damage=0;
        AddEMsgAff("Rune Warding name", true);
        new Float:location[3];
        GetClientAbsOrigin(client, location);
        location[2]+=15.0;
        new Float:dir[3];
        dir[2]=-1.0;
        VS_EnergySplash(location, dir, true);
        EmitAmbientSound("wc/spells/absorb.wav",location);
      }
  }
}

wcRunTalent_8_PowerOfBlood(client,damage)
{
  decl String:line[255];
  wcGetTalentInfo(8,1,2,3,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new mana = RoundToNearest(damage*multi)+GetMana(client);
  if(mana > GetMaxMana(client))
    mana = GetMaxMana(client);
  wcSetMana(client,mana);
}

wcRunTalent_8_Heartfire(client)
{
  decl String:line[255];
  wcGetTalentInfo(8,1,3,1,client,"runes",line);
  new runes = StringToInt(line);

  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }
  WCMessage(client,"heartfire cast",client);
  wcGetTalentInfo(8,1,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(8,1,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);

  SetCdSpell(client,cdtype,cd,0,"Heartfire");
  SetEffectVictim(client,47);
  SetEffectSpellVic(client,47);
  wcGetTalentInfo(8,1,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  new Handle:datapack4=CreateDataPack();
  WritePackCell(datapack4,client);
  WritePackCell(datapack4,HeatfireEffect(client));
  CreateTimer(duration,EndHeartfire,datapack4,TIMER_HNDL_CLOSE);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/heartfire.wav",location);
}

HeatfireEffect(target) {
   new sprite = CreateEntityByName("env_smokestack");
   if (IsValidEntity(sprite)) {
      //Give client a target name
      decl String:sTemp[30];
      Format(sTemp,sizeof(sTemp),"player_%i",target);
      DispatchKeyValue(target, "targetname", sTemp);


      DispatchKeyValue(sprite, "basespread", "50");
      DispatchKeyValue(sprite, "spreadspeed", "5");
      DispatchKeyValue(sprite, "speed", "50");
      DispatchKeyValue(sprite, "startsize", "30");
      DispatchKeyValue(sprite, "endsize", "31");
      DispatchKeyValue(sprite, "rate", "200");
      DispatchKeyValue(sprite, "jetlength", "20");
      DispatchKeyValue(sprite, "twist", "30");

      DispatchKeyValue(sprite, "angles", "0 0 180");
      DispatchKeyValue(sprite, "rendercolor", "255 20 20");
      DispatchKeyValue(sprite, "renderamt", "10");

      DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");

      DispatchKeyValue(sprite, "parentname", sTemp);

      new Float:Client_Origin[3];
      GetClientAbsOrigin(target,Client_Origin);
      Client_Origin[2]+=40;
      DispatchKeyValueVector(sprite, "origin", Client_Origin);


      DispatchSpawn(sprite);
      SetVariantString(sTemp);
      AcceptEntityInput(sprite, "SetParent", target, target, 0);
      AcceptEntityInput(sprite, "TurnOn");
      return sprite;
	}
   return 0;
}

public Action:EndHeartfire(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  RemoveEffectVictim(client,47);
  RemoveEffectSpellVic(client,47);
  new effect = ReadPackCell(datapack);
  if(effect > 0)
    KillEntity(effect);
}

public wcRunSpell_8_10(spellid,race,victim,attacker,&totaldmg) //Necrotic Strike
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  {
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance=StringToInt(line);
    if(chance>=random)
    {
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multief=StringToFloat(line);
      decl Float:location[3];
      GetClientAbsOrigin(attacker,location);
      EmitAmbientSound("wc/spells/necroticstrike.wav",location);
      wcGetSpellInfo(race,spellid,"duration",line);
      new Float:duration=StringToFloat(line);
      if(HasGlyph(attacker, GLYPH_DK_NECROTIC)) {
        duration = 6.0;
        totaldmg = RoundToNearest(1.1 * totaldmg);
      }
      if(HasGlyph(attacker, GLYPH_DK_NECROTICSTRIKE))
        g_iNecrotic[victim]+=RoundToNearest(totaldmg*0.20);
      SetHM(victim,multief);
      SetHMT(victim,duration);
      /*decl String:buffer[30];*/
      /*Format(buffer,sizeof(buffer),"%T", "necrotic strike ef", attacker);*/
      AddEMsgAff("necrotic strike ef", true);
      decl Float:position[3];
      GetClientAbsOrigin(victim, position);
      VS_GlowSprite(position, g_ef_greenglow1, 1.00, 3.00, 255);
      VS_Smoke(position, g_ef_greenglow1, Float:1092616192, 10);
    }
  }
  return totaldmg;
}

wcRunSpell_8_11(spellid,race,client) //Death and decay
{

  //DK specific
  new runes = wcGetSpellInfoInt(race,spellid,"runes");
  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }
  new regen = wcGetSpellInfoInt(race,spellid,"regen");
  if(regen>0) AddMana(client, regen);

  //Cooldown
  new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
  if(wcGetTalentLevel(client,3,4,1)>0)  {
    decl String:buff[255];
    wcGetTalentInfo(8,3,4,1,client,"multi",buff);
    new Float:cdMulti = StringToFloat(buff);
    cd *= (1.0 - cdMulti);
  }
  SetCdSpell(client, cdtype, cd, spellid);

  //Take mana off
  new manareq=GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);


  //Spell Specific variables
  new Float:distance = wcGetSpellInfoFloat(race,spellid,"distance");
  //We run out of effect data space, so we assume AOE is 350
  //new Float:aoe = wcGetSpellInfoFloat(race,spellid,"aoe");
  new Float:duration = wcGetSpellInfoFloat(race,spellid,"duration");
  duration = duration * (1.0 + GetHasteEffect(client));
  new Float:multi = wcGetSpellInfoFloat(race,spellid,"multi");
  new Float:tick = wcGetSpellInfoFloat(race,spellid,"tick");
  tick = tick*(1.0-GetHasteEffect(client));
  new team=GetClientTeam(client);
  team = (team-1) % 2 + 2;
  //Spell code

  //Get location of where we center
  new Float:location[3];
  GetViewPoint(client, distance, location);

  new dmg = wcGetSpellInfoInt(race,spellid,"base");
  dmg += RoundToNearest(GetDamage(client) * multi);

  //Create the hellfire like (different) effect
  new effid=CreateEffectData(client,spellid,dmg,RoundToNearest(duration / tick),team,_:location[0],_:location[1],_:location[2],g_RoundCount,0);
  if (multi==0.0 || tick == 0.0 || duration == 0.0) {
    LogError("ERROR: death and decay spell corrupted.");
    DeleteEffectData(effid);
  }
  else
  if(effid!=0)
  {
    CreateTimer(tick, EffectDecayAction, effid, TIMER_REPEAT);
  }


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
    KillEntityDelay(8.0, sprite);
  }


  //Message
  WCMessage(client,"death and decay caster", client, dmg);
}



public Action:EffectDecayAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];
  new spellid = effectsdata[effid][2];
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
  if (spellid!=11)
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
  new aoe = 250;
  decl String:line[255];
  wcGetSpellInfo(8,11,"name",line);

  wcSpellAoeDamageLocation(client, client, location, dmg, aoe, team, line);

  //TODO: add death and decay effect

  return Plugin_Continue;
}

wcRunTalent_8_CuttingChill(client)
{
  decl String:line[255];
  wcGetTalentInfo(8,2,3,3,client,"multi",line);
  new Float:penetration = GetIgnoreArmor(client)*StringToFloat(line);
  SetBonusIgnoreArmor(client,GetBonusIgnoreArmor(client)+penetration);
  wcRecalculateStats(client);
}

wcRunTalent_8_FotG(client,&damage)
{
  if(GetNormalSpeed(client)>GetClientSpeed(client))
  {
    decl String:line[255];
    wcGetTalentInfo(8,2,2,4,client,"multi",line);
    new Float:multi=StringToFloat(line);
    damage = RoundToNearest(damage*(1+multi));
  }
}

wcRunTalent_8_ArmorOfFrost(client,attacker)
{
  decl String:line[255];
  wcGetTalentInfo(8,2,2,2,client,"slow",line);
  new Float:slowas=StringToFloat(line);
  wcGetTalentInfo(8,2,2,2,client,"duration",line);
  new Float:slowtime=StringToFloat(line);
  new Float:newas=GetAttackSpeed(attacker)+(GetBonusAttackSpeed(attacker))*(1-slowas);
  new Float:restoreAs=(GetAttackSpeed(attacker)+(GetBonusAttackSpeed(attacker))-newas);
  if(HasGlyph(client, GLYPH_DK_AOF))
    {
    new Float:oldspeed=(GetClientSpeed(attacker));
    new Float:newspeed=(GetClientSpeed(attacker))*(0.5-(slowas*0.5));
    if(newspeed<oldspeed)
      {
      SetClientSpeed(attacker,newspeed);
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,attacker);
      WritePackCell(datapack4,g_respawns[attacker]);
      WritePackFloat(datapack4,oldspeed);
      CreateTimer(slowtime,RemoveAFSpeed,datapack4,TIMER_HNDL_CLOSE);
      }
    }
  wcClientsBonusStats[attacker][ATTACKSPEED]-=restoreAs;
  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,attacker);
  WritePackCell(datapack3,g_respawns[attacker]);
  WritePackFloat(datapack3,restoreAs);
  CreateTimer(slowtime,RemoveSlowAttack,datapack3,TIMER_HNDL_CLOSE);

}

wcRunTalent_8_PowerOfFrost(client,&mana)
{
  if(mana>GetMana(client))
  {
    decl String:line[255];
    wcGetTalentInfo(8,2,1,4,client,"multi",line);
    mana = RoundToNearest((mana-GetMana(client))*(1+StringToFloat(line))+GetMana(client));
    if(mana>GetMaxMana(client))
      mana=GetMaxMana(client);
  }
}

wcRunTalent_8_Hypothermia(client)
{
  new target=GetAimTarget(client,15.0);

  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      decl String:line[255];
      wcGetTalentInfo(8,2,4,1,client,"runes",line);
      new runes = StringToInt(line);
      if(runes>0)
        if(!SpendRune(client))  {
          WCMessage(client, "no runes", client);
          return;
      }
      wcGetTalentInfo(8,2,4,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(8,2,4,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      SetCdSpell(client, cdtype, cd, 0, "Hypothermia");

      wcGetTalentInfo(8,2,4,1,client,"multi",line);
      new Float:multi=StringToFloat(line);
      wcGetTalentInfo(8,2,4,1,client,"scaling",line);
      new Float:scale=StringToFloat(line);
      scale*=GetDamage(client);
      if(scale < 1)
        scale = 1.0;
      new damage = RoundToNearest((GetMana(client)*multi)*scale);
      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","hypothermia target", target);
      Format(attMsg,sizeof(attMsg),"%T","hypothermia caster", client);
      wcGetTalentInfo(8,2,4,1,client,"name",line);
      wcSpellDamage(target,client,damage, attMsg, vicMsg,line);
      new Float:vicLoc[3], Float:attLoc[3];
      GetClientAbsOrigin(target, vicLoc);
      GetClientAbsOrigin(client, attLoc);
      vicLoc[2]+=30.0;
      attLoc[2]+=30.0;
      VS_BeamPoints(vicLoc, attLoc, g_ef_bluecore, ChaosSprite, 0, 5, 3.0, 15.0, 15.0, 2, 1.0, {150,150,255,250}, 10);
      wcSetMana(client,0);

      EmitAmbientSound("wc/spells/hypothermia.wav",attLoc);
    }
    else  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
}

wcRunTalent_8_InateResistance(client)
{
  decl String:line[255];
  wcGetTalentInfo(8,3,1,4,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new bonus=RoundToNearest(GetMaxHealth(client)*multi);
  wcClientsBonusStats[client][RESISTANCE]+=bonus;
  wcRecalculateStats(client);
  WCMessage(client,"innateresistanceact",client,bonus,RoundToNearest(wcClientsStats[client][RESISTANCE]));
}

wcRunTalent_8_RapidContagion(client,&Float:tickdelay)
{
  decl String:line[255];
  wcGetTalentInfo(8,3,2,4,client,"time",line);
  tickdelay-=StringToFloat(line);
}

wcRunTalent_8_UnholySurge(client,attacker)
{
  decl String:line[255];
  wcGetTalentInfo(8,3,3,2,client,"silence",line);
  SetSilence(attacker,StringToFloat(line));
}

stock wcRunTalent_8_AntiMagicZone(client)
{
  decl String:line[255];

  //CD and mana
  wcGetTalentInfo(8,3,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(8,3,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client, cdtype, cd, 0, "Anti-Magic Zone");

  wcGetTalentInfo(8,3,4,1,client,"runes",line);
  new runes = StringToInt(line);
  if(runes>0)
    if(!SpendRune(client))  {
      WCMessage(client, "no runes", client);
      return;
  }
  wcGetTalentInfo(8,3,4,1,client,"duration",line);
  new Float:duration=StringToFloat(line);


  wcGetTalentInfo(8,3,4,1,client,"tickdelay",line);
  new Float:period=StringToFloat(line);

  wcGetTalentInfo(8,3,4,1,client,"distance",line);
  new aoe=StringToInt(line);

  WCMessage(client,"anti magic zone cast",client);
  new Float:location[3];
  GetClientAbsOrigin(client,location);

  //Setting up effect
  new team=GetClientTeam(client);
  new effid=CreateEffectData(client,team,aoe,RoundToNearest(location[0]),RoundToNearest(location[1]),RoundToNearest(location[2]),0,0,g_RoundCount);
  new Float:loc[3];
  GetClientAbsOrigin(client,loc);
  loc[2]+=25.0;
  VS_BeamRingPoint(loc, 340.00, 350.00, g_ef_ocean, g_ef_ocean, 0, 3, 10.00, 30.00, 0.00, {20, 20, 150, 255}, 5, 0);
  if (period == 0.0 || duration == 0.0) {
    LogError("ERROR: anti magic zone spell corrupted.");
    DeleteEffectData(effid);
  }
  else if(effid!=0)
  {
    CreateTimer(period, EffectAMZoneAction, effid, TIMER_REPEAT);
    CreateTimer(duration,EndAMZoneAction,effid);
    EmitAmbientSound("wc/spells/antimagiczone.wav",location);
  }
}

public Action:EndAMZoneAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][9]==g_RoundCount)
  {
    DeleteEffectData(effid);
  }
}
