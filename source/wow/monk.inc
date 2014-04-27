/*
* =====================================================================================
*
*       Filename:  monk.inc
*
*    Description:  Monk class for WoW mod.
*                  Shall contain everything related to the monk class.
*                  Ideally we will get as minimal as possible monk-code outside
*                  of this class.
*
*        Version:  5.0
*        Created:  27/10/2013 10:59:45
*       Compiler:  spcomp
*
*         Author:  Jevgenij Timosenko (jozh), stinkyfax@gmail.com
*        Company:  wowdev
*
* =====================================================================================
*/

#pragma semicolon 1

new g_ff_activeMonkStance[65];
new g_iBrew[65];
new g_iSSS[65];
new g_iJabDmg[65];
new g_iChiDmgBuff[65];
new Float:g_fAfterlifeDuration;
new Float:g_aAfterlifeLoc[3];
new g_iSereneMists[65];
new g_iSereneMistsTarget[65];
new g_iRevivalHeal[65];
new g_iChiSerenity[65];


ActivateMonkStance(client)  {
  if(GetClass(client)!=9) {
    _setMonkStance(client, 0);
    return;
  }
  new brewmaster = wcGetTreeTotal(client, getActiveSpec(client),1);
  new windwalker = wcGetTreeTotal(client, getActiveSpec(client),2);
  new mistweaver = wcGetTreeTotal(client, getActiveSpec(client),3);
  if(brewmaster == 0 && windwalker == 0 && mistweaver == 0) {
    _setMonkStance(client, 0);
    return;
  }
  new stance=0;
  if(brewmaster>=windwalker)
    stance=1;
  else
    stance=2;
  if(mistweaver > brewmaster && mistweaver > windwalker)
    stance=3;
  _setMonkStance(client,stance);
  decl String:buffer[50];
  Format(buffer,50,"activated stance_%d",stance);
  WCMessage(client, buffer, client);

  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  if(stance==1)
    EmitAmbientSound("wc/spells/brewmaster_stance.wav",location);
  if(stance==2)
    EmitAmbientSound("wc/spells/windwalker_stance.wav",location);
  if(stance==3)
    EmitAmbientSound("wc/spells/mistweaver_stance.wav",location);
}

_setMonkStance(client, stance)  {
  g_ff_activeMonkStance[client]=stance;
}

GetMonkStance(client) {
  return g_ff_activeMonkStance[client];
}

stock SetBrew(client, brew)  {
  g_iBrew[client]=brew;
}

stock GetBrew(client) {
  return g_iBrew[client];
}

// Chi control
#define MAX_CHI 5
new g_iChi[MAXPLAYERS];
//new g_iTouchOfKarma[65]; //Target
//new g_iTouchOfKarmac[65]; //Caster
//new Float:g_fTouchOfKarma[65]; //Duration
new g_iMist[65];

stock getChi(client)  {
  return g_iChi[client];
}

_setChi(client, amount) {
  g_iChi[client] = amount;
}

stock addChi(client, amount=1)  {
  // Do not give more than maximum
  new curChi = getChi(client);
  if(HasGlyph(client, GLYPH_MONK_CHI))
    if(10>=GetRandomInt(1,100))
      amount+=1;
  if (MAX_CHI < curChi + amount)  {
    amount = MAX_CHI - curChi;
  }
  _setChi(client, curChi+amount);

  return amount;
}

stock bool:spendChi(client, amount) {
  if (getChi(client) < amount) {
    return false;
  }
  else  {
    _setChi(client, getChi(client) - amount);
    if(CheckRequirements(client,9,2,3,1))
      wcRunTalent_9_2_3_1(client, amount);
    if(CheckRequirements(client,9,3,1,2))
      wcRunTalent_9_ManaThistleTea(client, amount);
    if(CheckRequirements(client,9,1,3,2))
    {
      wcRunTalent_9_TigereyeBrew(client, amount);
      if(HasGlyph(client, GLYPH_MONK_TBREW))
      {
        new heal = RoundToNearest(GetMaxHealth(client) * 0.01 * float(amount));
        wcHeal(client, client, heal,0,false);
      }
    }
    return true;
  }
}

stock spendAllChi(client, maximum=MAX_CHI)  {
  new chi = getChi(client);
  new spend = maximum;
  if (chi < spend)
    spend = chi;
  _setChi(client, chi - spend);
  if(CheckRequirements(client,9,2,3,1))
    wcRunTalent_9_2_3_1(client, chi);
  if(CheckRequirements(client,9,3,1,2))
    wcRunTalent_9_ManaThistleTea(client, chi);
  if(CheckRequirements(client,9,1,3,2))
      wcRunTalent_9_TigereyeBrew(client, chi);
  return spend;
}

bool:checkChi(client, req)
{
  if(req==0)
    return true;
  if(getChi(client)==req)
    return true;
  if(getChi(client)>=req)
    return true;
  if(getChi(client)<req) {
    WCMessage(client,"no chi",client);
    return false;
  }
  return true;
}


// Spells code
public wcRunSpell_9_1(spellid,race,victim,attacker,&totaldmg) //Jab
{
  decl String:line[255];
  new mana = wcGetSpellInfoInt(race,spellid,"mana");
  wcGetTalentInfo(9,2,1,2,attacker,"mana",line);
  mana-=StringToInt(line);
  if(mana>GetMana(attacker))
    return totaldmg;

  new random=GetRandomInt(1,100);
  new chance = wcGetSpellInfoInt(race,spellid,"chance");
  if(chance<random)
    return totaldmg;

  new Float:multi = wcGetSpellInfoFloat(race,spellid,"multi");
  new Float:agility=float(GetAgi(attacker)+GetBonusAgi(attacker));
  new dmg=RoundToNearest(agility*multi);
  if(CheckRequirements(attacker,9,2,2,4)) {
    new jabbonus = GetJabDmg(attacker)+1;
    SetJabDmg(attacker, jabbonus);
  }
  if(CheckRequirements(attacker,9,1,1,1))
    wcRunTalent_9_1_1_1(attacker);
  totaldmg+=dmg;
  wcSetMana(attacker,GetMana(attacker)-mana);
  addChi(attacker,1);
  decl Float:location[3];
  GetClientAbsOrigin(attacker,location);
  EmitAmbientSound("wc/spells/jab.wav",location);
  decl Float:location1[3];
  GetClientAbsOrigin(victim, location1);
  MonkJabEffect(location, location1);
  AddEMsgAff("jab", true);
  if(HasGlyph(attacker, GLYPH_MONK_JAB))
  {
    new Float:timeleft=GetCDSpell(attacker,4)-GetTickedTime();
    if(0.51>=timeleft)
      SetCdSpell(attacker, 4, 0.0, 4);
    else
      SetCdSpell(attacker, 4, timeleft-0.50, 4);
  }
  return totaldmg;
}

public wcRunSpell_9_2(spellid,race,victim,attacker,&totaldmg) //Tiger Palm
{
  decl String:line[255];
  new mana = wcGetSpellInfoInt(race,spellid,"mana");
  if(mana>GetMana(attacker))
    return totaldmg;

  new random=GetRandomInt(1,100);
  new chance = wcGetSpellInfoInt(race,spellid,"chance");
  if(chance<random)
    return totaldmg;

  new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:timeleft=GetCDSpell(attacker,cdtype)-GetTickedTime();
  if(timeleft > 0.0)
    return totaldmg;
  wcGetTalentInfo(9,2,1,1,attacker,"multi",line);
  new Float:talentbonus = 1+StringToFloat(line);
  new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
  SetCdSpell(attacker, cdtype, cd, spellid);
  new Float:dmgmulti = wcGetSpellInfoFloat(race,spellid,"dmgmulti");
  new Float:arpmulti = wcGetSpellInfoFloat(race,spellid,"multi");
  new base = wcGetSpellInfoInt(race,spellid,"base");
  new bonus = RoundToNearest((GetIgnoreArmor(attacker)*arpmulti)*talentbonus);
  new Float:agility=float(GetAgi(attacker)+GetBonusAgi(attacker));
  new dmgbonus = RoundToNearest(agility*dmgmulti);
  new dmg = base+dmgbonus;
  if(CheckRequirements(attacker,9,2,2,4))
  {
    new Float:talentbonusdmg = 1.0;
    wcGetTalentInfo(9,2,2,4,attacker,"multi",line);
    talentbonusdmg += RoundToNearest(float(GetJabDmg(attacker)) * StringToFloat(line));
    dmg = RoundToNearest(float(dmg)*talentbonusdmg);
    SetJabDmg(attacker, 0);
  }
  totaldmg+=dmg;
  if (bonus>0)
  {
    SetBonusIgnoreArmor(attacker,GetBonusIgnoreArmor(attacker)+float(bonus));
    wcRecalculateStats(attacker);
    new Handle:pack=CreateDataPack();
    WritePackCell(pack,g_RoundCount);
    WritePackCell(pack,attacker);
    WritePackCell(pack,bonus);
    new Float:duration=wcGetSpellInfoFloat(race,spellid,"duration");
    CreateTimer( duration, RemoveTigerPalmBonus, pack, TIMER_HNDL_CLOSE);
    WCMessage(attacker,"tiger palm bonus", attacker, bonus,GetIgnoreArmor(attacker), duration); //I added the translation this time, I promise! (I'll un-comment when my lazy ass wakes up again and I test)
  }
  wcSetMana(attacker,GetMana(attacker)-mana);
  addChi(attacker,1);
  decl Float:location[3];
  GetClientAbsOrigin(victim,location);
  MonkTigerPalmEffect(location);
  //EmitAmbientSound("wc/spells/tigerpalm.wav",location);
  AddEMsgAff("tiger palm", true);

  return totaldmg;
}
public Action:RemoveTigerPalmBonus(Handle:timer, any:pack)
{
  ResetPack(pack);
  if (g_RoundCount != ReadPackCell(pack))
    return Plugin_Stop;
  new client = ReadPackCell(pack);
  if (!IsClientInGame(client))
    return Plugin_Stop;
  if (!IsPlayerAlive(client))
    return Plugin_Stop;

  SetBonusIgnoreArmor(client,GetBonusIgnoreArmor(client)-float(ReadPackCell(pack)));
  wcRecalculateStats(client);
  return Plugin_Stop;
}
public wcRunSpell_9_3(spellid,race,client) // Expel Harm
{
  decl String:line[255];
  new target=GetAimTarget(client,45.0, true);
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target)) || (!IsPlayerAlive(target)))
    target=0;
  }
  if((target<=0) && SelfCast(client))
    target=client;
  if(SelfOnly(client))
    target=client;
  if(IsBoss())
  {
    if(CheckTargetOnly(client))
    target=g_iTargetOnly[client];
  }
  if(target<=0)
    return 0;
  if(wcGetHealth(target)>=GetMaxHealth(target))
    return 0;
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"distance",line);
  new Float:distance=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  if(GetChiSerenity(client)>0)
  SetChiSerenity(client, 0);
  else
  SetCdSpell(client, cdtype, cd, spellid);
  new Float:location[3],Float:locationatt[3],Float:locationatt2[3];
  GetClientAbsOrigin(target,location);
  GetClientAbsOrigin(client,locationatt);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  decl String:name[255],String:name2[255],String:name3[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  new heal=GetSpellDmg(client,multi);
  wcGetSpellInfo(race,spellid,"base",line);
  new base=StringToInt(line);
  heal+=base;
  new Float:smulti=0.50;
  wcGetTalentInfo(9,3,1,1,client,"multi",line);
  smulti*= 1.0+StringToFloat(line);
  new twoheal=RoundToNearest(float(heal)*smulti);
  new targett;
  new team=GetClientTeam(target);
  if(team==2)
    targett=GetNearestClient("@t",location,distance,target);
  if(team==3)
    targett=GetNearestClient("@ct",location,distance,target);
  GetClientName(targett,name3,sizeof(name3));
  EmitAmbientSound("wc/spells/heal.wav",location);
  heal=wcHeal(client,target,heal,spellid,true);
  if(targett>0)
    twoheal=wcHeal(client,targett,twoheal,spellid,true);
  if(target!=client)
  {
    WCMessage(target,"expel harm target",target,name,heal);
    WCMessage(client,"expel harm caster",client,name2,heal);
  }
  else
  {
    WCMessage(client,"expel harm caster",client,name,heal);
  }
  if(targett!=0)
  {
    WCMessage(targett,"expel harm target",targett,name,twoheal);
    WCMessage(client,"expel harm caster",client,name3,twoheal);
  }
  if(targett>0)
    GetClientAbsOrigin(targett,locationatt2);
  MonkExpelHarmEffect1(locationatt);
  MonkExpelHarmEffect(locationatt, location);
  if(targett>0)
    MonkExpelHarmEffect(location, locationatt2);
  if(CheckRequirements(client,9,3,2,2)) {
  new random=GetRandomInt(1,100);
  wcGetTalentInfo(9,3,2,2,client,"chance",line);
  new chance = StringToInt(line);
  if(chance>=random)
    addChi(client,1);
  }
  if(CheckRequirements(client,9,3,4,2)) {
    g_iSereneMists[client]=1;
    g_iSereneMistsTarget[client]=target;
    wcRunTalent_9_RenewingMist(client);
  }
  if(CheckRequirements(client,9,3,2,4))
    SetRevival(client, GetRevival(client)+1);
  return 1;
}
public wcRunSpell_9_4_BrewmasterPurifyingBrew(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  if(GetChiSerenity(client)>0)
  SetChiSerenity(client, 0);
  else
  SetCdSpell(client, cdtype, cd, spellid);
  wcGetSpellInfo(race,spellid,"brewmulti",line);
  new Float:multi=StringToFloat(line);
  new Float:oldspeed=GetClientSpeed(client);
  new Float:normalspeed=GetNormalSpeed(client);
  if(normalspeed>oldspeed) {
      SetClientSpeed(client,normalspeed);
      SetEntityMoveType(client, MOVETYPE_WALK);
    }
  new heal=RoundToNearest(float(getChi(client))*multi*GetMaxHealth(client));
  spendAllChi(client);
  heal=wcHeal(client,client,heal,0,false);
  WCMessage(client,"purifying brew",client,heal);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  if(getChi(client)>0)
  {
    Format(line,sizeof(line),"wc/spells/burb%d.wav",getChi(client));
    EmitAmbientSound(line,location);
    if(HasGlyph(client, GLYPH_MONK_PBREW))
    {
      new bhealth = RoundToNearest(0.03*float(GetMaxHealth(client)));
      new thealth=GetBonusMaxHealth(client)+bhealth;
      new oldhp=GetBonusMaxHealth(client);
      SetBonusMaxHealth(client, thealth);
      wcRecalculateStats(client);
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,client);
      WritePackFloat(datapack4,0.03);
      WritePackCell(datapack4,g_RoundCount);
      WritePackCell(datapack4, oldhp);
      CreateTimer(8.0,EndLastStand,datapack4,TIMER_HNDL_CLOSE);
    }
  }
}
public wcRunSpell_9_4_WindwalkerFistsofFury(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  if(GetChiSerenity(client)>0)
  SetChiSerenity(client, 0);
  else
  SetCdSpell(client, cdtype, cd, spellid);
  wcGetSpellInfo(race,spellid,"fistbase",line);
  new basedmg = StringToInt(line);
  wcGetSpellInfo(race,spellid,"ticks",line);
  new ticks = StringToInt(line);
  wcGetSpellInfo(race,spellid,"fistmulti",line);
  new Float:multi = StringToFloat(line);
  wcGetSpellInfo(race,spellid,"fistdistance",line);
  new range=StringToInt(line);
  wcGetSpellInfo(race,spellid,"fistchimulti",line);
  new Float:chimulti=StringToFloat(line);
  new dmg=RoundToNearest((float(GetAgi(client)+GetBonusAgi(client))*multi)+basedmg);
  dmg+=RoundToNearest(float(dmg)*float(getChi(client))*chimulti);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/fistsoffury.wav",location);
  new team=GetClientTeam(client);
  new maxplayers=GetMaxClients();
  new Float:loc2[3];
  new String:stargets[255];
  decl String:name2[255];
  new count=0;
  spendAllChi(client);
  new effid=CreateEffectData(client,dmg,ticks,range,0,0,0,0,g_RoundCount,0);
  CreateTimer(0.5, FistsOfFuryRepeat, effid, TIMER_REPEAT);
  for(new x=1;x<=maxplayers;x++)
  {
    if(!IsClientInGame(x))
      continue;
    if(team==GetClientTeam(x))
      continue;
    if(!IsPlayerAlive(x))
      continue;

    GetClientAbsOrigin(x,loc2);
    if(float(range)<GetDistanceBetween(location,loc2))
      continue;

    if(GetTargetFOV(client,x,90.0,true)==1)
    {
      GetClientName(x,name2,sizeof(name2));
      if (count==0)
        Format(stargets,sizeof(stargets),"%s%s",stargets,name2);
      else
        Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      decl String:vicMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","fists of fury target", x);
      wcPhysicalDamage(x,client,dmg, "", vicMsg, true,line);
      BloodSpray(x);
      count++;
      if(HasGlyph(client, GLYPH_MONK_FISTS))
      {
        new Float:oldspeed=GetClientSpeed(x);
        new Float:newspeed=GetNormalSpeed(x);
        newspeed*=0.9;
        if(newspeed<oldspeed)
        {
          SetClientSpeed(x,newspeed);
          new Handle:datapack2=CreateDataPack();
          WritePackCell(datapack2,x); // Write the client index of owner
          WritePackFloat(datapack2,newspeed);
          WritePackFloat(datapack2,0.9);
          CreateTimer(0.49,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
        }
      }
    }
  }
  if(count>0)
    WCMessage(client,"fists of fury client",client,stargets,dmg);
  else
    WCMessage(client,"fists of fury miss",client);

}
public wcRunSpell_9_4_MistweaverRevival(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"base",line);
  new baseheal = StringToInt(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi = StringToFloat(line);
  wcGetSpellInfo(race,spellid,"distance",line);
  new Float:range=StringToFloat(line);
  new heal=GetSpellDmg(client,multi)+baseheal;
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  if(GetChiSerenity(client)>0)
  SetChiSerenity(client, 0);
  else
  SetCdSpell(client, cdtype, cd, spellid);
  wcGetSpellInfo(race,spellid,"chimulti",line);
  heal+=RoundToNearest(float(heal)*(float(getChi(client))*StringToFloat(line)));
  if(CheckRequirements(client,9,3,2,4)) {
    wcGetTalentInfo(9,3,2,4,client,"multi",line);
    heal+=RoundToNearest(float(heal)*StringToFloat(line));
    SetRevival(client, 0);
  }
  spendAllChi(client);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new maxplayers=GetMaxClients();
  new String:stargets[255];
  new Float:loc2[3];
  new count=0;
  decl healed;
  for(new x=1;x<=maxplayers;x++)
  {
    if(!IsClientInGame(x))
      continue;
    if(team!=GetClientTeam(x))
      continue;
    if(!IsPlayerAlive(x))
      continue;

    GetClientAbsOrigin(x,loc2);
    if(range<GetDistanceBetween(location,loc2))
      continue;

    if(GetTargetFOV(client,x,110.0,true)==1 || client == x)
    {
      decl String:name[255],String:name2[255];
      GetClientName(client,name,sizeof(name));
      healed=wcHeal(client,x,heal,0,true);
      if(client != x)
        WCMessage(x,"revival heal",x,name,healed);
      count++;
      new Float:loc3[3];
      GetClientAbsOrigin(x,loc3);
      EmitAmbientSound("wc/spells/heal.wav",loc3);
      GetClientName(x,name2,sizeof(name2));
      if(x>1)
        Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      else
        Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
      if(HasGlyph(client, GLYPH_MONK_REVIVAL))
      {
        new effid=CreateEffectData(client,x,RoundToNearest(heal*0.05),3,0,0,0,0,g_RoundCount,0);
        CreateTimer(2.0, RevivalHoT, effid, TIMER_REPEAT);
      }
    }
  }
  WCMessage(client,"revival healed",client,stargets,healed);
  MonkRevivalEffect(location);
  EmitAmbientSound("wc/spells/revival.wav",location);
}
public Action:RevivalHoT(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new caster = effectsdata[effid][1];
  new target = effectsdata[effid][2];
  new ticks = effectsdata[effid][4];
  new heal = effectsdata[effid][3];
  if (!IsClientInGame(caster) || !IsClientInGame(target))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(target))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (ticks<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][4]--;
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }

  wcHeal(caster, target, heal,0,true);
  return Plugin_Continue;
}
public wcRunSpell_9_4(spellid,race,client) //Stance spells
{
  if(getChi(client)>=2)
  {
    switch (GetMonkStance(client))
    {
      case 1: wcRunSpell_9_4_BrewmasterPurifyingBrew(spellid,race,client);
      case 2: wcRunSpell_9_4_WindwalkerFistsofFury(spellid,race,client);
      case 3: wcRunSpell_9_4_MistweaverRevival(spellid,race,client);
    }
  }
  else
    WCMessage(client,"no chi",client);
}
public Action:FistsOfFuryRepeat(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  new client=effectsdata[effid][1];
  effectsdata[effid][3]-=1;
  new Float:range=float(effectsdata[effid][4]);
  new dmg=effectsdata[effid][2];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new maxplayers=GetMaxClients();
  new Float:loc2[3];
  new String:stargets[255];
  decl String:name2[255];
  new count=0;
  for(new x=1;x<=maxplayers;x++)
  {
    if(!IsClientInGame(x))
      continue;
    if(team==GetClientTeam(x))
      continue;
    if(!IsPlayerAlive(x))
      continue;
    GetClientAbsOrigin(x,loc2);
    if(range<GetDistanceBetween(location,loc2))
      continue;
    if(GetTargetFOV(client,x,90.0,true)==1)
    {
      GetClientName(x,name2,sizeof(name2));
      if (count==0)
        Format(stargets,sizeof(stargets),"%s%s",stargets,name2);
      else
        Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      decl String:vicMsg[300];
      MonkFistsOfFuryEffect(loc2);
      Format(vicMsg,sizeof(vicMsg),"%T","fists of fury target", x);
      wcPhysicalDamage(x,client,dmg, "", vicMsg, true);
      BloodSpray(x);
      count++;
      if(HasGlyph(client, GLYPH_MONK_FISTS))
      {
        new Float:oldspeed=GetClientSpeed(x);
        new Float:newspeed=GetNormalSpeed(x);
        newspeed*=0.9;
        if(newspeed<oldspeed)
        {
          SetClientSpeed(x,newspeed);
          new Handle:datapack2=CreateDataPack();
          WritePackCell(datapack2,x); // Write the client index of owner
          WritePackFloat(datapack2,newspeed);
          WritePackFloat(datapack2,0.9);
          CreateTimer(0.49,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
        }
      }
    }
  }
  if(count>0)
    WCMessage(client,"fists of fury client",client,stargets,dmg);
  else
    WCMessage(client,"fists of fury miss",client);
  return Plugin_Continue;
}
public wcRunSpell_9_5(spellid,race,client) //Spinning Crane Kick
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetTalentInfo(9,2,1,4,client,"multi",line);
  cd*=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  if(GetChiSerenity(client)>0)
  SetChiSerenity(client, 0);
  else
  SetCdSpell(client,cdtype,cd, spellid);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetSpellInfo(race,spellid,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"base",line);
  new base=StringToInt(line);
  wcGetSpellInfo(race,spellid,"delay",line);
  new Float:delay = StringToFloat(line);
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
  amount=GetNearClients("@ct",location,targets,distance);
  if(team==3)
  amount=GetNearClients("@t",location,targets,distance);
  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  new String:stargets[255];
  new Float:agility=float(GetAgi(client)+GetBonusAgi(client));
  new dmg=RoundToNearest(float(base)+agility*multi);
  for (new x=0;x<=(amount-1);x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
    Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
    Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
  }
  if(amount>=1)
    WCMessage(client,"spinning crane kick",client,dmg,stargets);
  if(amount==0)
    WCMessage(client,"spinning crane kick miss",client,dmg);

  new olddmg = dmg;
  new Float:talentmulti = 1.00;
  for (new x=0;x<=(amount-1);x++)
  {
    if(CheckRequirements(client,9,2,3,3))
    {
      wcGetTalentInfo(9,2,3,3,client,"multi",line);
      talentmulti=StringToFloat(line);
      decl Float:clienta[3],Float:targeta[3];
      GetClientEyeAngles(client, clienta);
      GetClientEyeAngles(targets[x], targeta);
      new Float:angle=clienta[1]-targeta[1];
      if(angle<0.0)
      angle*=-1.0;
      if(angle>180.0)
      angle = 360.0-angle;
      if(angle<=90.0)
        dmg=RoundToNearest(float(dmg)*talentmulti);
      else
      {
        new heal=RoundToNearest(float(dmg)*(talentmulti-1.0));
        wcHeal(client,client,heal,0,true);
      }
    }
    if(targets[x]>0 && GetMonkStance(client)==2)
      addChi(client,1);
    if(targets[x]>=3 && GetMonkStance(client)==2)
      addChi(client,1);
    decl String:vicMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","spinning crane kick target", targets[x]);
    wcPhysicalDamage(targets[x],client,dmg, "", vicMsg, true);
    dmg = olddmg; //So that the damage isn't increased and then sent to the repeat part where it'll be increased again, and to avoid it stacking multiplicatively from the amount of targets (e.g 3 targets would cause it to be 1.3^3 damage.)
    BloodSpray(targets[x]);
  }
  if(CheckRequirements(client,9,3,2,3))
  {
    if(team==3)
      amount=GetNearClients("@ct",location,targets,distance);
    if(team==2)
      amount=GetNearClients("@t",location,targets,distance);
    wcGetTalentInfo(9,3,2,3,client,"multi",line);
    new heal=RoundToNearest(float(dmg)*StringToFloat(line));
    for (new x=0;x<=(amount-1);x++)
    {
      GetClientName(targets[x],name2,sizeof(name2));
      if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
      new healed=wcHeal(client,targets[x],heal,0,true);
      if(targets[x]!=client)
      {
        WCMessage(targets[x],"combat healing target",targets[x],name,healed);
      }
    }
  }
  if(CheckRequirements(client,9,1,2,4))
  {
    wcGetTalentInfo(9,1,2,4,client,"parry",line);
    new Float:pamount=StringToFloat(line);
    wcGetTalentInfo(9,1,2,4,client,"duration",line);
    new Float:duration=StringToFloat(line);
    AddParry(client, pamount);
    wcRecalculateStats(client);
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,client); // Write the client index of owner
    WritePackFloat(datapack,pamount);
    //WritePackCell(datapack,g_RoundCount);
    CreateTimer(duration,RemoveCritParry,datapack,TIMER_HNDL_CLOSE);
  }
  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,client);
  WritePackCell(datapack3,dmg);
  WritePackFloat(datapack3,distance);
  CreateTimer(delay,SpinningCraneKickRepeat,datapack3,TIMER_HNDL_CLOSE);
  SpinningCraneKickEffect(client);
}
public Action:SpinningCraneKickRepeat(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);
  new dmg=ReadPackCell(datapack);
  new Float:distance=ReadPackFloat(datapack);
  new Float:location[3];
  decl String:line[255];
  if(!IsPlayerAlive(client))
    return;
  GetClientAbsOrigin(client,location);
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
  amount=GetNearClients("@ct",location,targets,distance);
  if(team==3)
  amount=GetNearClients("@t",location,targets,distance);
  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  new String:stargets[255];
  for (new x=0;x<=(amount-1);x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
    Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
    Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
  }
  if(amount>=1)
    WCMessage(client,"spinning crane kick",client,dmg,stargets);
  if(amount==0)
    WCMessage(client,"spinning crane kick miss",client,dmg);

  new olddmg = dmg;
  new Float:talentmulti = 1.00;
  for (new x=0;x<=(amount-1);x++)
  {
    if(CheckRequirements(client,9,2,3,3))
    {
      wcGetTalentInfo(9,2,3,3,client,"multi",line);
      talentmulti=StringToFloat(line);
      decl Float:clienta[3],Float:targeta[3];
      GetClientEyeAngles(client, clienta);
      GetClientEyeAngles(targets[x], targeta);
      new Float:angle=clienta[1]-targeta[1];
      if(angle<0.0)
      angle*=-1.0;
      if(angle>180.0)
      angle = 360.0-angle;
      if(angle<=90.0)
        dmg=RoundToNearest(float(dmg)*talentmulti);
      else
      {
        new heal=RoundToNearest(float(dmg)*(talentmulti-1.0));
        wcHeal(client,client,heal,0,true);
      }
    }
    if(targets[x]>0 && GetMonkStance(client)==2)
      addChi(client,1);
    if(targets[x]>=3 && GetMonkStance(client)==2)
      addChi(client,1);
    decl String:vicMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","spinning crane kick target", targets[x]);
    wcPhysicalDamage(targets[x],client,dmg, "", vicMsg, true);
    dmg = olddmg; //So that the damage isn't increased and then sent to the repeat part where it'll be increased again, and to avoid it stacking multiplicatively from the amount of targets (e.g 3 targets would cause it to be 1.3^3 damage.)
    BloodSpray(targets[x]);
  }
  if(CheckRequirements(client,9,3,2,3))
  {
    if(team==3)
      amount=GetNearClients("@ct",location,targets,distance);
    if(team==2)
      amount=GetNearClients("@t",location,targets,distance);
    wcGetTalentInfo(9,3,2,3,client,"multi",line);
    new heal=RoundToNearest(float(dmg)*StringToFloat(line));
    for (new x=0;x<=(amount-1);x++)
    {
      GetClientName(targets[x],name2,sizeof(name2));
      if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
      new healed=wcHeal(client,targets[x],heal,0,true);
      if(targets[x]!=client)
      {
        WCMessage(targets[x],"combat healing target",targets[x],name,healed);
      }
    }
  }
  SpinningCraneKickEffect(client);
  return;
}
public wcRunSpell_9_6(spellid,race,client) //Path of Blossoms
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  if(GetChiSerenity(client)>0)
  SetChiSerenity(client, 0);
  else
  SetCdSpell(client,cdtype,cd, spellid);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetSpellInfo(race,spellid,"distance",line);
  new Float:aoe=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"base",line);
  new base=StringToInt(line);
  //wcGetSpellInfo(race,spellid,"delay",line);
  //new Float:delay = StringToFloat(line);
  wcGetSpellInfo(race,spellid,"ticks",line);
  new ticks = StringToInt(line);
  ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
  wcGetSpellInfo(race,spellid,"duration",line);
  new duration = StringToInt(line);
  if(CheckRequirements(client,9,2,1,3))
  {
    wcGetTalentInfo(9,2,1,3,client,"speed",line);
    new Float:msmulti = StringToFloat(line);
    new Float:newspeed=GetNormalSpeed(client)*msmulti;
    SetClientSpeed(client,newspeed);
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,client); // Write the client index of owner
    WritePackFloat(datapack,newspeed);
    WritePackFloat(datapack,msmulti);
    CreateTimer(7.0,RemoveSpeedEffectMonk,datapack,TIMER_HNDL_CLOSE);
  }
  new team=GetClientTeam(client);
  team = (team-1) % 2 + 2;
  new Float:agility=float(GetAgi(client)+GetBonusAgi(client));
  new dmg=RoundToNearest((multi*agility)+float(base));
  new effid=CreateEffectData(client,ticks,dmg,RoundToNearest(aoe),team,duration,0,0,g_RoundCount,0);
  CreateTimer((1.0*(1.0-GetHasteEffect(client))),BlossomPath,effid,TIMER_REPEAT);
  WCMessage(client,"path of blossoms",client,dmg*2,duration);

  if (GetRandomInt(1,2) == 1)
    EmitAmbientSound("wc/spells/blossom1.wav",location);
  else
    EmitAmbientSound("wc/spells/blossom2.wav",location);
}
public Action:RemoveSpeedEffectMonk(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:oldspeed=ReadPackFloat(datapack); //current speed
  new Float:multi=ReadPackFloat(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (GetClientSpeed(client)!=oldspeed)
  {
    return Plugin_Stop;
  }
  new Float:speed=GetClientSpeed(client);
  speed*=(1/multi);
  if(speed<GetNormalSpeed(client))
    speed=GetNormalSpeed(client);
  SetEntityMoveType(client, MOVETYPE_WALK);
  SetClientSpeed(client,speed, true); // ignore anti-speed effects
  return Plugin_Stop;
}
public Action:BlossomPath(Handle:timer, any:effid) //Drops down the petals that deal damage
{
  if (!IsClientInGame(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][2]<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  new client=effectsdata[effid][1];
  new duration=effectsdata[effid][6];
  new Float:ddelay=0.5;
  new dticks = RoundToNearest(float(duration) / ddelay);
  decl Float:location[3];
  effectsdata[effid][2]-=1;
  GetClientAbsOrigin(client,location);
  new effid2=CreateEffectData(client,dticks,effectsdata[effid][3],effectsdata[effid][4],effectsdata[effid][5],_:location[0],_:location[1],_:location[2],g_RoundCount,0);
  CreateTimer(ddelay,BlossomPath2,effid2,TIMER_REPEAT);

  switch (GetRandomInt(1,3))
  {
    case 1: EmitAmbientSound("wc/spells/fireblossom1.wav",location);
    case 2: EmitAmbientSound("wc/spells/fireblossom2.wav",location);
    case 3: EmitAmbientSound("wc/spells/fireblossom3.wav",location);
  }

  location[2]+=2.0;
  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
    DispatchKeyValue(sprite, "basespread", "105");
    DispatchKeyValue(sprite, "spreadspeed", "50");
    DispatchKeyValue(sprite, "spreadspeed", "50");
    DispatchKeyValue(sprite, "speed", "3");
    DispatchKeyValue(sprite, "startsize", "11");
    DispatchKeyValue(sprite, "endsize", "11");
    DispatchKeyValue(sprite, "rate", "150");
    DispatchKeyValue(sprite, "jetlength", "20");

    switch (GetRandomInt(1,13))
    {
      case 13: DispatchKeyValue(sprite, "rendercolor", "255 10 0");
      case 12: DispatchKeyValue(sprite, "rendercolor", "255 91 0");
      case 11: DispatchKeyValue(sprite, "rendercolor", "255 10 0");
      case 10: DispatchKeyValue(sprite, "rendercolor", "255 91 91");
      case  9: DispatchKeyValue(sprite, "rendercolor", "101 245 217");
      case  8: DispatchKeyValue(sprite, "rendercolor", "247 251 96");
      case  7: DispatchKeyValue(sprite, "rendercolor", "78 181 72");
      case  6: DispatchKeyValue(sprite, "rendercolor", "234 128 186");
      case  5: DispatchKeyValue(sprite, "rendercolor", "210 90 0");
      case  4: DispatchKeyValue(sprite, "rendercolor", "3 207 197");
      case  3: DispatchKeyValue(sprite, "rendercolor", "147 231 255");
      case  2: DispatchKeyValue(sprite, "rendercolor", "165 250 155");
      case  1: DispatchKeyValue(sprite, "rendercolor", "192 235 167");
      default: PrintToChat(client,"Path of Blossoms effect is bugged, please report");
    }

    DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");
    DispatchKeyValueVector(sprite, "origin", location);
    DispatchSpawn(sprite);
    AcceptEntityInput(sprite, "TurnOn");
    KillEntityDelay(3.0, sprite);
  }
  return Plugin_Continue;
}
public Action:BlossomPath2(Handle:timer, any:effid2) //Makes the petals deal damage
{
  if (effectsdata[effid2][2]<=0)
  {
    DeleteEffectData(effid2);
    return Plugin_Stop;
  }
  if (effectsdata[effid2][9]!=g_RoundCount)
  {
    DeleteEffectData(effid2);
    return Plugin_Stop;
  }
  new client=effectsdata[effid2][1];
  decl String:line[255];
  effectsdata[effid2][2]-=1;
  new Float:location[3];
  location[0] = Float:effectsdata[effid2][6];
  location[1] = Float:effectsdata[effid2][7];
  location[2] = Float:effectsdata[effid2][8];
  wcSpellAoeDamageLocation(client, client, location, effectsdata[effid2][3], effectsdata[effid2][4], effectsdata[effid2][5], line);
  if(HasGlyph(client, GLYPH_MONK_BLOSSOMS))
  {
    new targets[MAXPLAYERS];
    new amount;
    if(effectsdata[effid2][5]==3)
      amount=GetNearClients("@ct",location,targets,float(effectsdata[effid2][4]));
    if(effectsdata[effid2][5]==2)
      amount=GetNearClients("@t",location,targets,float(effectsdata[effid2][4]));
    for (new x=0;x<=(amount-1);x++)
    {
      wcHeal(client,targets[x],RoundToNearest(float(effectsdata[effid2][3])*0.25),0,true);
    }
  }
  return Plugin_Continue;
}

wcRunTalent_9_Firebreath(client) //Breath of Fire
{
  decl String:line[255];
  wcGetTalentInfo(9,1,2,1,client,"cost",line);
  new cost = StringToInt(line);
  if(checkChi(client, cost))
  {
    wcGetTalentInfo(9,1,2,1,client,"base",line);
    new basedmg = StringToInt(line);
    wcGetTalentInfo(9,1,2,1,client,"multi",line);
    new Float:multi = StringToFloat(line);
    new Float:stamina=float(GetSta(client)+GetBonusSta(client));
    wcGetTalentInfo(9,1,2,1,client,"distance",line);
    new Float:range=StringToFloat(line);
    new dmg=RoundToNearest((stamina*multi)+float(basedmg));
    wcGetTalentInfo(9,1,2,1,client,"cooldown",line);
    new Float:cooldown=StringToFloat(line);
    wcGetTalentInfo(9,1,2,1,client,"cooldowntype",line);
    new cooldowntype=StringToInt(line);
    if(GetChiSerenity(client)>0)
    SetChiSerenity(client, 0);
    else
    SetCdSpell(client,cooldowntype,cooldown,0,"Breath of Fire");
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    new team=GetClientTeam(client);
    new team2;
    new maxplayers=GetMaxClients();
    new Float:loc2[3];
    new Float:distance2;
    new String:stargets[255];
    decl String:name2[255];
    new count=0;
    spendChi(client, cost);
    location[2]+=40;
    EmitAmbientSound("wc/spells/breathfire.wav",location);
    location[2]-=40;
    new Float:AngleLocation[3], Float:angles[3];
    new Float: vector[3];
    GetClientEyeAngles(client, AngleLocation);
    GetAngleVectors(AngleLocation, angles, NULL_VECTOR, NULL_VECTOR);
    ScaleVector(angles, 150.0);
    AddVectors(location, angles, vector);
    MonkBreathOfFireEffect(vector);

    for(new x=1;x<=maxplayers;x++)
    {
      if(!IsClientInGame(x))
        continue;
      team2=GetClientTeam(x);
      if(team2==team)
        continue;
      if(!IsPlayerAlive(x))
        continue;

      GetClientAbsOrigin(x,loc2);
      distance2=GetDistanceBetween(location,loc2);

      if(distance2>range)
        continue;

      if(GetTargetFOV(client,x,90.0,true)==1)
      {
        GetClientName(x,name2,sizeof(name2));
        if (count==0)
          Format(stargets,sizeof(stargets),"%s%s",stargets,name2);
        else
          Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
        decl String:vicMsg[300];
        dmg=RoundToNearest(dmg*(1.0+(float(GetBrew(x))*0.33))); //Checks whether there is a keg effect on the target
        Format(vicMsg,sizeof(vicMsg),"%T","firebreath target", x);
        wcSpellDamage(x,client,dmg, "", vicMsg,line);
        BloodSpray(x);
        count++;
      }
    }
    if(count>0)
      WCMessage(client,"firebreath caster",client,stargets,dmg);
    else
      WCMessage(client,"firebreath miss",client);
  }
  else
    WCMessage(client,"no chi",client);
}
wcRunTalent_9_AgileArmor(client)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,1,2,client,"multi",line);
  new Float:multi = StringToFloat(line);
  new bonus = RoundToNearest(float((GetAgi(client)+GetBonusAgi(client)))/multi);
  SetBonusArmor(client, float(GetBonusArmor(client)+bonus));
  wcRecalculateStats(client);
  WCMessage(client,"agile armor effect",client,bonus,GetArmor(client));

}
/*public wcRunTalent_9_TouchofKarma(client) // Touch of Karma
{
  decl String:line[255];
  wcGetTalentInfo(9,2,4,1,client,"cost",line);
  new cost = StringToInt(line);
  if(IsFakeClient(client))
    cost = 2;
  if(checkChi(client,cost))
  {
    new target=GetAimTarget(client,15.0);
    if(target>0)
    {
      if(GetClientTeam(client)!=GetClientTeam(target)&&IsPlayerAlive(target))
      {
        wcGetTalentInfo(9,2,4,1,client,"duration",line);
        new Float:duration=StringToFloat(line);
        if(IsFakeClient(client))
          duration=6.0;
        wcGetTalentInfo(9,2,4,1,client,"cooldowntype",line);
        new cdtype=StringToInt(line);
        if(IsFakeClient(client))
          cdtype=18;
        wcGetTalentInfo(9,2,4,1,client,"cooldown",line);
        new Float:cd=StringToFloat(line);
        if(IsFakeClient(client))
          cd=20.0;
        if(GetChiSerenity(client)>0)
        SetChiSerenity(client, 0);
        else
        SetCdSpell(client,cdtype,cd,0,"Touch of Karma");
        SetEffectVictim(client,55);
        SetEffectSpellVic(client,55);
        SetEffectVictim(target,55);
        SetEffectSpellVic(target,55);
        decl String:name[255],String:name2[255];
        GetClientName(client,name,sizeof(name));
        GetClientName(target,name2,sizeof(name2));
        WCMessage(client,"ToK cast",client,duration,name2);
        WCMessage(target,"ToK target",target,duration,name);
        g_iTouchOfKarma[client] = target;
        g_iTouchOfKarmac[client] = client;
        g_fTouchOfKarma[client] = GetEngineTime() + duration;
        g_iTouchOfKarma[target] = target;
        g_iTouchOfKarmac[target] = client;
        g_fTouchOfKarma[target] = GetEngineTime() + duration;
        new Float:location[3],Float:locationatt[3];
        GetClientAbsOrigin(target,location);
        GetClientAbsOrigin(client,locationatt);
        EmitAmbientSound("wc/spells/touchofkarma.wav",location);
        EmitAmbientSound("wc/spells/touchofkarma.wav",locationatt);
        spendChi(client, cost);
        new Handle:datapack4=CreateDataPack();
        WritePackCell(datapack4,KarmaEffect(client));
        WritePackCell(datapack4,KarmaEffect(target));
        CreateTimer(duration,EndKarmaEffect,datapack4,TIMER_HNDL_CLOSE);
      }
    }
    if(target<=0)
      if(IsChat(client,Chat:Chat_Target))
        WCMessage(client,"no target",client);
  }
}*/
/*KarmaEffect(target) {
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
      DispatchKeyValue(sprite, "rendercolor", "20 255 20");
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
}*/

/*public Action:EndKarmaEffect(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new effect = ReadPackCell(datapack);
  new effect2 = ReadPackCell(datapack);
  if(effect > 0)
    KillEntity(effect);
  if(effect2 > 0)
    KillEntity(effect2);
}*/
/*Effect55(client,attacker,&damage) //Touch of Karma
{
  if(g_fTouchOfKarma[client] > GetEngineTime() || g_fTouchOfKarma[attacker] > GetEngineTime())
  {
    new cliente=g_iTouchOfKarmac[client];
    new target=g_iTouchOfKarma[client];
    if(attacker==target || attacker==cliente)
    {
      if(client==target || client==cliente)
      {
        //decl String:line[255];
        //wcGetTalentInfo(9,2,4,1,client,"multi",line); //How much of the damage should be reflected
        new dmg = RoundToNearest(damage*1.0);
        wcPhysicalDamage(attacker,client,dmg);
        WCMessage(attacker,"ToK victim",attacker,dmg);
        //wcGetTalentInfo(9,2,4,1,client,"reduction",line);
        damage = RoundToNearest(damage * 0.0); //How much of the damage should be reduced
      }
    }
  }
}*/

stock MonkJabEffect(Float:loc1[3], Float:loc2[3]) {
  loc1[2] += 35;
  loc2[2] += 35;
  VS_BeamPoints(loc1, loc2, g_ef_playerredsmall, g_ef_playerredsmall, 0, 10, 2.0, 1.0, 1.0, 0, 0.0, {255, 25, 50, 255}, 0, 0.0);
  VS_BeamPoints(loc1, loc2, g_ef_tpbeam001, g_ef_tpbeam001, 0, 10, 2.0, 3.0, 3.0, 0, 0.0, {45, 45, 45, 255}, -1, 0.0);
  VS_GlowSprite(loc2, g_ef_playerredsmall, 1.0, 1.0, 255, 0.0);
  VS_BeamRingPoint(loc2, 35.0, 50.0, g_ef_tpbeam001, g_ef_tpbeam001, 0, 255, 2.0, 5.0, 0.0, {255, 25, 25, 255}, -1, 0, 0.0);
}

stock MonkTigerPalmEffect(Float:loc[3]) {
  new Float: loc1[3], Float:loc2[3], Float:loc3[3];
  for (new i=0; i<3; i++) {
    loc1[i] = loc[i];
    loc2[i] = loc[i];
    loc3[i] = loc[i];
  }
  loc1[2] += 35.0;
  loc2[2] += 35.0;
  loc3[2] += 35.0;

  VS_BeamRingPoint(loc1, 69.0, 70.0, g_ef_shellchrome, g_ef_shellchrome, 0, 1, 1.0, 10.0, 5.0, {75, 125, 255, 25}, -1, 0, 0.0);
  VS_BeamRingPoint(loc1, 69.0, 70.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 15.0, 5.0, {75, 125, 255, 25}, -1, 0, 0.0);
  VS_BeamRingPoint(loc1, 69.0, 70.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 15.0, 5.0, {75, 125, 255, 25}, 200, 0, 0.0);
  VS_BeamRingPoint(loc1, 69.0, 70.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 15.0, 5.0, {75, 125, 255, 25}, 200, 0, 0.0);
  VS_BeamRingPoint(loc1, 69.0, 70.0, g_ef_shellchrome, g_ef_shellchrome, 0, 1, 1.0, 10.0, 5.0, {255, 255, 255, 25}, 1, 0, 0.0);

  loc2[1] -= 33.0;
  loc3[1] += 33.0;
  VS_BeamPoints(loc2, loc3, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 4.0, 4.0, 0, 0.0, {75, 125, 200, 125}, -1, 0.0);
  loc2[1] += 33.0;
  loc3[1] -= 33.0;

  loc2[0] -= 33.0;
  loc3[0] += 33.0;
  VS_BeamPoints(loc2, loc3, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 4.0, 4.0, 0, 0.0, {75, 125, 200, 125}, -1, 0.0);
}

stock MonkRevivalEffect(Float: loc[3])  {
  decl Float:loc1[3];
  loc1[0]=loc[0];
  loc1[1]=loc[1];
  loc1[2]=loc[2];
  loc1[2] += 35;
  VS_BeamRingPoint(loc1, 254.0, 255.0, g_ef_overlay, g_ef_overlay, 0, 1, 1.0, 25.0, 1.0, {0, 0, 255, 175}, 1, 0, 0.0);
  VS_BeamRingPoint(loc1, 254.0, 255.0, g_ef_overlay, g_ef_overlay, 0, 1, 1.0, 25.0, 1.0, {0, 0, 255, 175}, 7, 0, 0.0);
  VS_BeamRingPoint(loc1, 0.0, 255.0, g_ef_overlay, g_ef_overlay, 0, 1, 1.0, 25.0, 1.0, {0, 0, 255, 175}, 24, 0, 0.0);
  VS_BeamRingPoint(loc1, 0.0, 255.0, g_ef_overlay, g_ef_overlay, 0, 1, 1.0, 25.0, 1.0, {0, 0, 255, 175}, 24, 0, 0.3);
  VS_BeamRingPoint(loc1, 0.0, 255.0, g_ef_overlay, g_ef_overlay, 0, 1, 1.0, 25.0, 1.0, {0, 0, 255, 175}, 24, 0, 0.6);
  VS_BeamRingPoint(loc1, 0.0, 255.0, g_ef_overlay, g_ef_overlay, 0, 1, 1.0, 25.0, 1.0, {0, 0, 255, 175}, 24, 0, 0.9);
}

stock MonkFistsOfFuryEffect(Float: loc[3])  {
  decl Float:loc1[3];
  loc1[0]=loc[0];
  loc1[1]=loc[1];
  loc1[2]=loc[2];
  loc1[2] += 5;
  VS_GlowSprite(loc1, g_ef_striderbulgedudvdx60, 1.0, 1.0, 255, 0.0);
}

stock MonkBreathOfFireEffect(Float: loc[3])  {
  new Float:loc1[3];
  loc1[0]=loc[0];
  loc1[1]=loc[1];
  loc1[2]=loc[2];
  loc1[2] += 25;
  VS_BeamRingPoint(loc1, 1.0, 150.0, g_ef_fire01, g_ef_fire01, 0, 1, 1.0, 18.0, 2.0, {255, 255, 255, 175}, -1, 0, 0.0);
  VS_BeamRingPoint(loc1, 1.0, 150.0, g_ef_fire01, g_ef_fire01, 0, 1, 1.0, 18.0, 2.0, {255, 255, 255, 175}, -1, 0, 0.0);
  VS_BeamRingPoint(loc1, 150.0, 1.0, g_ef_fire01, g_ef_fire01, 0, 1, 1.0, 18.0, 2.0, {255, 255, 255, 175}, -1, 0, 0.0);
  VS_BeamRingPoint(loc1, 150.0, 1.0, g_ef_fire01, g_ef_fire01, 0, 1, 1.0, 18.0, 2.0, {255, 255, 255, 175}, -1, 0, 0.0);
  loc1[2] += 50;
  VS_GlowSprite(loc1, g_ef_fire01, 1.0, 0.1, 255, 0.0);
}


stock MonkExpelHarmEffect(Float: loc1[3], Float: loc2[3])  {
  loc1[2] -= 10;
  loc2[2] += 35;
  VS_BeamPoints(loc1, loc2, g_ef_yellowglow1, g_ef_yellowglow1, 0, 1, 1.0, 8.0, 8.0, 0, 0.0, {255, 255, 255, 175}, 1, 0.0);
  VS_BeamPoints(loc1, loc2, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 8.0, 8.0, 0, 0.0, {50, 255, 50, 175}, 1, 0.0);
  loc2[2] -= 5;
  VS_BeamRingPoint(loc2, 49.0, 50.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 5.0, 0.0, {50, 255, 0, 255}, -1, 0, 0.0);
  loc2[2] += 10;
  VS_BeamRingPoint(loc2, 49.0, 50.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 5.0, 0.0, {50, 255, 0, 255}, -1, 0, 0.0);
}

stock MonkExpelHarmEffect1(Float: loc1[3])  {
  loc1[2] += 30;
  VS_BeamRingPoint(loc1, 49.0, 50.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 5.0, 0.0, {50, 255, 0, 255}, -1, 0, 0.0);
  loc1[2] += 10;
  VS_BeamRingPoint(loc1, 49.0, 50.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 5.0, 0.0, {50, 255, 0, 255}, -1, 0, 0.0);
}

stock MonkSpinningCraneKickEffect(Float: loc[3])  {
  new Float:loc1[3];
  loc1[0]=loc[0];
  loc1[1]=loc[1];
  loc1[2]=loc[2];
  loc1[2] += 25;
  VS_BeamRingPoint(loc1, 150.0, 151.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 5.0, 0.0, {25, 75, 100, 25}, 0, 0, 0.0);
  VS_BeamRingPoint(loc1, 150.0, 151.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 0.9, 10.0, 0.0, {25, 75, 100, 50}, 5, 0, 0.1);
  VS_BeamRingPoint(loc1, 150.0, 151.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 0.7, 15.0, 0.0, {25, 75, 100, 75}, 10, 0, 0.3);
  VS_BeamRingPoint(loc1, 150.0, 151.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 0.5, 20.0, 0.0, {25, 75, 100, 100}, 15, 0, 0.5);
  VS_BeamRingPoint(loc1, 150.0, 151.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 0.4, 25.0, 0.0, {25, 75, 100, 100}, 20, 0, 0.6);
  VS_BeamRingPoint(loc1, 150.0, 151.0, g_ef_shellchrome, g_ef_shellchrome, 0, 1, 1.0, 5.0, 0.0, {255, 255, 255, 55}, 255, 0, 0.0);
  VS_BeamRingPoint(loc1, 150.0, 151.0, g_ef_shellchrome, g_ef_shellchrome, 0, 1, 0.5, 5.0, 0.0, {255, 255, 255, 55}, -1, 0, 0.5);
  VS_GlowSprite(loc1, g_ef_rollerglow, 0.5, 1.0, 255, 0.0);
}

stock MonkManaThistleEffect(Float: loc[3])  {
  new Float:loc1[3];
  loc1[0]=loc[0];
  loc1[1]=loc[1];
  loc1[2]=loc[2];
  loc1[2] += 25;
  VS_BeamRingPoint(loc1, 75.0, 0.0, g_ef_hydragutbeam, g_ef_hydragutbeam, 0, 1, 1.0, 7.0, 0.0, {25, 75, 100, 255}, 25, 0, 0.0);
  VS_BeamRingPoint(loc1, 0.0, 75.0, g_ef_hydragutbeam, g_ef_hydragutbeam, 0, 1, 1.0, 7.0, 0.0, {25, 75, 100, 255}, 25, 0, 0.0);
  VS_GlowSprite(loc1, g_ef_blueblackflash, 1.0, 1.0, 255, 0.0);
}

stock MonkRenewingMistEffect(Float: loc[3])  {
  new Float:loc1[3];
  loc1[0]=loc[0];
  loc1[1]=loc[1];
  loc1[2]=loc[2];
  loc1[2] += 25;
  VS_BeamRingPoint(loc1, 25.0, 75.0, g_ef_hydragutbeam, g_ef_hydragutbeam, 0, 1, 0.5, 7.0, 0.0, {25, 75, 100, 200}, 25, 0, 0.0);
  VS_BeamRingPoint(loc1, 75.0, 25.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 0.5, 7.0, 0.0, {25, 75, 100, 200}, 25, 0, 0.0);
  loc1[2] += 15;
  VS_BeamRingPoint(loc1, 75.0, 25.0, g_ef_hydragutbeam, g_ef_hydragutbeam, 0, 1, 0.5, 7.0, 0.0, {25, 75, 100, 200}, 25, 0, 0.0);
  VS_BeamRingPoint(loc1, 25.0, 75.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 0.5, 7.0, 0.0, {25, 75, 100, 200}, 25, 0, 0.0);
}

stock MonkUpliftEffect(Float: loc[3])  {
  new Float:loc1[3];
  loc1[0]=loc[0];
  loc1[1]=loc[1];
  loc1[2]=loc[2];
  loc1[2] += 25;
  for(new i=1;i<=15;i++)  {
    VS_BeamRingPoint({loc[0], loc[1], loc[2] + i * 5}, 75.0 - i * 5.0, 1.0, g_ef_hydragutbeam, g_ef_hydragutbeam, 0, 1, 0.5,3.0, 0.0, {25, 75, 100, 255}, 255, 0, 0.0);
  }
}

stock MonkDampenHarmEffect(Float: loc[3])  {
  new Float:loc1[3];
  loc1[0]=loc[0];
  loc1[1]=loc[1];
  loc1[2]=loc[2];
  loc1[2] += 45;
  //createRing(vectorStart = (x1,y1,z1 + 50), radiusStart = 50, radiusEnd = 51, model = "sun/overlay.vmt", width = 2, colorBlue = 0, colorRed = 255, colorGreen = 255)
  VS_BeamRingPoint(loc1, 25.0, 75.0, g_ef_overlay, g_ef_overlay, 0, 1, 1.0, 2.0, 0.0, {0, 255, 255, 255}, 25, 0, 0.0);
  loc1[2] += 5;
  VS_BeamRingPoint(loc1, 25.0, 75.0, g_ef_overlay, g_ef_overlay, 0, 1, 1.0, 2.0, 0.0, {0, 255, 255, 255}, 25, 0, 0.0);
  loc1[2] += 5;
  VS_BeamRingPoint(loc1, 25.0, 75.0, g_ef_overlay, g_ef_overlay, 0, 1, 1.0, 2.0, 0.0, {0, 255, 255, 255}, 25, 0, 0.0);
}

stock MonkKegEffect(Float: loc[3], Float:radius)  {
  new Float:loc1[3];
  loc1[0]=loc[0];
  loc1[1]=loc[1];
  loc1[2]=loc[2];
  loc1[2] -= 10;
  VS_BeamRingPoint(loc1, 30.0, radius + 10.0, g_ef_striderdarkflare, g_ef_striderdarkflare, 0, 1, 1.0, 20.0, 0.0, {255, 255, 255, 55}, -1, 0, 0.0);
  VS_BeamRingPoint(loc1, 30.0, radius + 10.0, g_ef_striderdarkflare, g_ef_striderdarkflare, 0, 1, 1.0, 20.0, 0.0, {255, 255, 255, 55}, -1, 0, 0.3);
  VS_BeamRingPoint(loc1, 30.0, radius + 10.0, g_ef_striderdarkflare, g_ef_striderdarkflare, 0, 1, 1.0, 20.0, 0.0, {255, 255, 255, 55}, -1, 0, 0.6);
  VS_BeamRingPoint(loc1, 30.0, radius + 10.0, g_ef_striderdarkflare, g_ef_striderdarkflare, 0, 1, 1.0, 20.0, 0.0, {255, 255, 255, 55}, -1, 0, 0.9);
  VS_BeamRingPoint(loc1, 30.0, radius + 10.0, g_ef_xbeam2, g_ef_xbeam2, 0, 1, 1.0, 35.0, 0.0, {200, 165, 51, 175}, -1, 0, 0.0);
  VS_BeamRingPoint(loc1, 30.0, radius + 10.0, g_ef_xbeam2, g_ef_xbeam2, 0, 1, 1.0, 35.0, 0.0, {200, 165, 51, 175}, -1, 0, 0.3);
  VS_BeamRingPoint(loc1, 30.0, radius + 10.0, g_ef_xbeam2, g_ef_xbeam2, 0, 1, 1.0, 35.0, 0.0, {200, 165, 51, 175}, -1, 0, 0.6);
  VS_BeamRingPoint(loc1, 30.0, radius + 10.0, g_ef_xbeam2, g_ef_xbeam2, 0, 1, 1.0, 35.0, 0.0, {200, 165, 51, 175}, -1, 0, 0.9);
}

public RunParry(victim,attacker,&totaldmg) //Monk Parry
{
  if(GetClass(victim) == 9 && GetLevel(victim) >= 52)
    runSpar(victim,attacker);
  new Float:chance = GetParryChance(victim,attacker);
  if (chance < GetRandomFloat(0.0001,1.0))
    return totaldmg;

  decl Float:loc1[3], Float:loc2[3];
  GetClientAbsOrigin(victim,loc1);
  GetClientAbsOrigin(attacker,loc2);
  if (GetDistanceBetween(loc1,loc2) > 750.0)
    return totaldmg;

  new reduceBy = RoundToNearest(float(totaldmg) * GetParryReduce(victim));

  wcRunOnParrySpells( victim,attacker,reduceBy,totaldmg);
  wcRunOnParryTalents(victim,attacker,reduceBy,totaldmg);

  if (reduceBy > totaldmg)
    reduceBy = totaldmg;

  totaldmg -= reduceBy;
  EmitAmbientSound("wc/spells/Parry.wav",loc1);
  AddEMsgAff("parried attack", true);
  return totaldmg;
}
public runSpar(victim, attacker)
{
  decl Float:clienta[3],Float:targeta[3];
  GetClientEyeAngles(attacker, clienta);
  GetClientEyeAngles(victim, targeta);
  new Float:anglee=clienta[1]-targeta[1];
  if(anglee<0.0)
    anglee*=-1.0;
  if(anglee>180.0)
    anglee = 360.0-anglee;
  if(anglee < 100.0)
  {
    addSpar(victim,attacker);
    g_bSpar[victim]=true;
  }
}
public addSpar(victim, attacker)
{
  g_iSpar[victim][attacker] += 0.01;
}
public resetSpar(client)
{
  if (!g_bSpar[client])
    return;
  for(new i=1;i<=GetMaxClients();i++)
    g_iSpar[client][i]=0.0;
  g_bSpar[client]=false;
}
public decaySpar(client)
{
  if(!g_bSpar[client])
    return;
  for(new i=1;i<=GetMaxClients();i++)
  {
    g_iSpar[client][i] -= (0.001 + g_iSpar[client][i]) * 0.85;
    if(g_iSpar[client][i] < 0.0)
      g_iSpar[client][i]=0.0;
  }
}
public Float:GetParryReduce(client)
{
  return 0.30 + g_fParryReduce[client];
}
Float:GetParryChance(client,attacker=0) {
  if(GetAdrenaline(attacker))
    return 0.0;
  new Float:spar=0.0;
  if (attacker>0) // no need for class check here, players only get g_iSpar if they are Monk.
    spar += g_iSpar[client][attacker];
  if(GetBoss(client)>0)
    return 0.08 + spar;
  new Float:agi = wcClientsBonusStats[client][AGI] + wcClientsStats[client][AGI];
  return wcClientsStats[client][PARRY] + spar + 0.1 *(agi / (50.0+agi));
}
public SetParry(client,Float:amount) {
  wcClientsStats[client][PARRY]=amount;
}
public Float:GetParryBonus(client) {
  return wcClientsBonusStats[client][PARRY];
}
public SetParryBonus(client,Float:amount) {
  wcClientsBonusStats[client][PARRY]=amount;
}
public AddParry(client, Float:amount) {
  wcClientsBonusStats[client][PARRY]+=amount;
}
public RemoveParry(client, Float:amount) {
  wcClientsBonusStats[client][PARRY]-=amount;
}
wcRunOnParrySpells(victim, attacker, &reduceBy, &totaldmg)
{
  decl String:line[255];
  new spellid;
  new race = GetClass(victim);
  new tmp=0;
  for(new x=1;x<=50;x++)
  {
    if((spellid=wcGetSpell(victim,x))!=0)
    {
      wcGetSpellInfo(race,spellid,"event",line);
      if(StrEqual(line,"player_parry",false))
      {
        wcRunSpell(spellid,race,victim,attacker,totaldmg,tmp,tmp,tmp,tmp,tmp,tmp,victim);
      }
    }
  }
}
wcRunOnParryTalents(victim, attacker, &reduceBy, &totaldmg)
{
  new aclass = GetClass(attacker), vclass = GetClass(victim);
  switch (vclass)
  {
    case 9:
    {
      if (CheckRequirements(victim,9,1,1,3))
        wcRunTalent_9_1_1_3(victim, attacker, reduceBy); //Reversal
      if(GetMonkStance(victim)==1)
        ParryChi(victim);
    }
  }
  switch (aclass)
  {
    case 9:
    {
      //none yet
    }
  }
  return totaldmg;
}
public ParryChi(client)
{
  new chance = 25;
  new random=GetRandomInt(1,100);
  if(chance>=random)
    addChi(client, 1);
}
public wcRunTalent_9_1_1_3(victim, attacker, &reduceBy)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,1,3,victim,"base",line);
  new base = StringToInt(line);
  wcGetTalentInfo(9,1,1,3,victim,"multi",line);
  new Float:multi = StringToFloat(line);

  new dmg = base + RoundToCeil(float(reduceBy) * multi);
  decl String:vicMsg[255], String:attMsg[255];
  Format(vicMsg,sizeof(vicMsg),"%T","reversal on", attacker);
  Format(attMsg,sizeof(attMsg),"%T","reversal user", victim);

  wcPhysicalDamage(attacker,victim,dmg,attMsg,vicMsg, true);
}
public wcRunTalent_9_1_1_4(client) //Parry Control
{
  decl String:line[255];
  wcGetTalentInfo(9,1,1,4,client,"multi",line);
  g_fParryReduce[client] *= 1.0 + StringToFloat(line);
}
public wcRunSpell_9_7(spellid,race,client)
{
  if(GetMonkStance(client)==1 || GetMonkStance(client)==2)
  {
    new Float:addChance = wcGetSpellInfoFloat(race, spellid, "chance");
    AddParry(client, addChance);
    new Float:addReduce = wcGetSpellInfoFloat(race, spellid, "reduce");
    g_fParryReduce[client] += addReduce;
    wcRecalculateStats(client);
    WCMessage(client,"enhanced parry spawn",client);
  }
}
public GetMist(client)
{
  return g_iMist[client];
}

public SetMist(client, count)
{
  g_iMist[client]=count;
}
new g_iMistDmg[65];

public GetMistDmg(client)
{
  return g_iMistDmg[client];
}

public SetMistDmg(client, count)
{
  g_iMistDmg[client]=count;
}
public wcRunTalent_9_Mistweaver(client,&damage)
{
  decl String:line[255];
  new mist = GetMist(client);
  new mistdmg = GetMistDmg(client);
  wcGetTalentInfo(9,3,1,4,client,"hits",line);
  if(mist >= 5-StringToInt(line))
  {
    new targets[MAXPLAYERS];
    new amount=0;
    decl Float:location[3];
    new team=GetClientTeam(client);
    GetClientAbsOrigin(client,location);
    if(team==2)
      amount=GetNearClients("@t",location,targets,350.00);
    else if(team==3)
      amount=GetNearClients("@ct",location,targets,350.00);
    decl String:name[255];
    GetClientName(client,name,sizeof(name));
    for (new x=0;x<amount;x++)
    {
      wcHeal(client,targets[x],mistdmg,0,true);
      if(targets[x]!=client)
        WCMessage(targets[x],"mistweaver stance targets",targets[x],name,mistdmg);
      WCMessage(client,"mistweaver stance client",client,mistdmg);
      SetMist(client, 0);
      SetMistDmg(client, 0);
      location[2]+=30.0;
      new color[4] = { 20, 255, 17, 240 };
      if(!IsBoss())
        BeamRingEffect("@all",location,175.0,90.0,g_purplelaser1,0,10,3.0,80.0,color,0.0,3);
      EmitAmbientSound("wc/spells/stancehealmonk.wav",location);
    }
  }
  else
  {
    new Float:multi = (wcClientsStats[GetPlayerID(client)][SPELLDAMAGE])*0.0010;
    SetMist(client, (mist+1));
    new newmistdmg = 0;
    if(IsBoss())
      newmistdmg=RoundToNearest(float(mistdmg)+RoundToNearest(1+(multi*float(damage))*0.005));
    else
      newmistdmg=RoundToNearest(float(mistdmg)+(multi*float(damage)));
    SetMistDmg(client, newmistdmg);
  }
}
/*public wcRunTalent_9_Brewmaster(client) //Uses the same global int as mistweaver does; shouldn't matter because it's impossible to use two stances at once.
{
  new mist = GetMist(client);
  if(mist >= 5)
  {
    decl Float:location[3];
    GetClientAbsOrigin(client,location);
    new absorb=RoundToNearest(float(GetMaxHealth(client))*0.04);
    wcClientAbsorb[client]=absorb;
    g_iShieldStack[client]+=1;
    SetEffectVictim(client,5); // absorb shield
    SetEffectSpellVic(client,5); //Absorb magic dmg
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,client); // Write the client index of owner
    WritePackCell(datapack2,5);
    CreateTimer(5.0,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
    new Handle:datapack3=CreateDataPack();
    WritePackCell(datapack3,client); // Write the client index of owner
    WritePackCell(datapack3,5);
    CreateTimer(5.0,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);
    WCMessage(client,"brewmaster stance client",client,absorb);
    SetMist(client, 0);
    location[2]+=15.0;
    new color[4] = { 255, 255, 128, 240 };
    if(!IsBoss())
      BeamRingEffect("@all",location,100.0,90.0,g_purplelaser1,0,10,3.0,80.0,color,0.0,3);
  }
  else
  {
    SetMist(client, (mist+1));
  }
}*/
stock CheckAimTargetAlly(client, Float:range=45.0)
{
  new target=GetAimTarget(client,range);
  if(target>0)
  {
    if(GetClientTeam(client)==GetClientTeam(target)&&IsPlayerAlive(target))
    {
      return target;
    }
    else
    {
      if(IsChat(client,Chat:Chat_Target))
        WCMessage(client,"wrong target",client);
      return 0;
    }
  }
  if(target<=0) {
    if((target<=0) && SelfCast(client))
      return target;
    if(SelfOnly(client))
      return target;
    if(IsChat(client,Chat:Chat_Target)) {
      WCMessage(client,"no target",client);
      return 0;
      }
    }
  return target;
}
stock CheckAimTargetEnemy(client, Float:range=15.0)
{
  new target=GetAimTarget(client,range);
  if(target>0)
  {
    if(GetClientTeam(client)!=GetClientTeam(target)&&IsPlayerAlive(target))
    {
      return target;
    }
    else
    {
      if(IsChat(client,Chat:Chat_Target))
        WCMessage(client,"wrong target",client);
      return 0;
    }
  }
  if(target<=0) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"no target",client);
    return 0;
    }
  return target;
}
public wcRunTalent_9_RenewingMist(client) //Renewing Mist
{
  decl String:line[255];
  new target;
  wcGetTalentInfo(9,3,2,1,client,"cost",line);
  new cost = StringToInt(line);
  if(target<=0)
    target=GetAimTarget(client,45.0, true);
  if(g_iSereneMistsTarget[client]>0)
    target = g_iSereneMistsTarget[client];
  if(g_iSereneMists[client]>0)
    cost-=cost;
  if(getChi(client)>=cost)
  {
    if(target>0)
    {
      if((GetClientTeam(client)!=GetClientTeam(target)) || (!IsPlayerAlive(target)))
        target=0;
    }
    if((target<=0) && SelfCast(client))
      target=client;
    if(SelfOnly(client))
      target=client;
    if(IsBoss())
    {
      if(CheckTargetOnly(client))
      target=g_iTargetOnly[client];
    }
    if(target<=0)
      return 0;
    if(target>0)
    {
      wcGetTalentInfo(9,3,2,1,client,"base",line);
      new base = StringToInt(line);
      if(IsFakeClient(client))
        base = 3;
      wcGetTalentInfo(9,3,2,1,client,"multi",line);
      new Float:multi = StringToFloat(line);
      if(IsFakeClient(client))
        multi = 0.085;
      new heal = RoundToNearest(GetSpellDmg(client,multi)+float(base));
      wcGetTalentInfo(9,3,2,1,client,"jumps",line);
      new jumps = StringToInt(line);
      if(IsFakeClient(client))
        jumps = 5;
      if(g_iSereneMists[client]>0) {
        wcGetTalentInfo(9,3,4,2,client,"jumps",line);
        jumps = StringToInt(line);
      }
      wcGetTalentInfo(9,3,2,1,client,"ticks",line);
      new ticks = StringToInt(line);
      if(IsFakeClient(client))
        ticks = 4;
      wcGetTalentInfo(9,3,2,1,client,"ticklength",line);
      new Float:ticklength = StringToFloat(line);
      if(IsFakeClient(client))
        ticklength = 1.0;
      ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
      ticklength = ticklength*(1.0-GetHasteEffect(client));
      wcGetTalentInfo(9,3,2,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      if(IsFakeClient(client))
        cdtype = 19;
      wcGetTalentInfo(9,3,2,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      if(IsFakeClient(client))
        cd = 15.00;
      wcGetTalentInfo(9,3,2,1,client,"range",line);
      new range=StringToInt(line);
      if(IsFakeClient(client))
        range = 350;
      new team = GetClientTeam(client);
      new effid=CreateEffectData(client,target,heal,ticks,jumps,team,range,ticks,g_RoundCount,0);
      CreateTimer(ticklength, RenewingMistHeal, effid, TIMER_REPEAT);
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,RenewingEffect(target));
      CreateTimer((ticklength*ticks),EndRenewingEffect,datapack4,TIMER_HNDL_CLOSE);
      decl String:name[255],String:name2[255];
      GetClientName(client,name,sizeof(name));
      GetClientName(target,name2,sizeof(name2));
      if(g_iSereneMists[client]>0)
        WCMessage(target,"renewing mist target",target,heal,name);
      else
      {
        if(GetChiSerenity(client)>0)
        SetChiSerenity(client, 0);
        else
        SetCdSpell(client,cdtype,cd,0,"Renewing Mist");
        new Float:location[3];
        GetClientAbsOrigin(target,location);
        MonkRenewingMistEffect(location);
        WCMessage(client,"renewing mist client",client,heal,name2);
        WCMessage(target,"renewing mist target",target,heal,name);
        spendChi(client, cost);
      }
      g_iSereneMists[client]=0;
      g_iSereneMistsTarget[client]=0;
    }
  }
  else
    WCMessage(client,"no chi",client);
  return 1;
}

public Action:RenewingMistHeal(Handle:timer, any:effid)
{
  new client = effectsdata[effid][1];
  new target = effectsdata[effid][2];
  new heal = effectsdata[effid][3];
  new jumps = effectsdata[effid][5];
  new roundcount = effectsdata[effid][9];
  new origin = effectsdata[effid][8];
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(effectsdata[effid][4]<=0)
  {
    if(jumps>0)
    {
      new distance = effectsdata[effid][7];
      decl Float:location[3];
      GetClientAbsOrigin(effectsdata[effid][2],location);
      new team = effectsdata[effid][6];
      if(team==2)
        target=GetNearestClient("@t",location,float(distance),effectsdata[effid][2]);
      if(team==3)
        target=GetNearestClient("@ct",location,float(distance),effectsdata[effid][2]);
      if(target==0)
        return Plugin_Stop;
      jumps-=1;
      effectsdata[effid][4]+=origin;
      new effid2=CreateEffectData(client,target,heal,effectsdata[effid][4],jumps,team,distance,origin,roundcount,0);
      CreateTimer(1.0, RenewingMistHeal2, effid2, TIMER_REPEAT);
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,RenewingEffect(target));
      CreateTimer(4.0,EndRenewingEffect,datapack4,TIMER_HNDL_CLOSE);
      DeleteEffectData(effid);
      if(CheckRequirements(client,9,3,3,3) || IsFakeClient(client))
      {
        decl String:line[255],String:stargets[255];
        new healed;
        decl String:name[255],String:name2[255];
        GetClientName(client,name,sizeof(name));
        new targets[MAXPLAYERS];
        new amount;
        if(team==3)
        amount=GetNearClients("@ct",location,targets,float(distance));
        if(team==2)
        amount=GetNearClients("@t",location,targets,float(distance));
        if(!IsFakeClient(client))
        {
          wcGetTalentInfo(9,3,3,3,client,"heal",line);
          heal=RoundToNearest(float(heal)*StringToFloat(line));
        }
        else
          heal=RoundToNearest(float(heal)*1.20);
        for (new x=0;x<=(amount-1);x++)
        {
          GetClientName(targets[x],name2,sizeof(name2));
          if(x>0)
          Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
          else
          Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
          healed=wcHeal(client,targets[x],heal,0,true);
          
          
          if(targets[x]!=client)
          {
            WCMessage(targets[x],"uplift target",targets[x],name,healed);
          }
        }
        WCMessage(client,"uplift client",client,stargets,healed);
      }
      return Plugin_Stop;
    }
    else
    {
      DeleteEffectData(effid);
      return Plugin_Stop;
    }
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  wcHeal(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][3],0,true);
  if(HasGlyph(client, GLYPH_MONK_RENEWING)) 
  {
    g_iRenewingGlyph[target]+=1;
    CreateTimer(1.0,RemoveMist,target,TIMER_HNDL_CLOSE);
  }
  effectsdata[effid][4]-=1;
  return Plugin_Continue;
}
public Action:RemoveMist(Handle:timer, any:target) {
  g_iRenewingGlyph[target]-=1;
  if(0>g_iRenewingGlyph[target])
    g_iRenewingGlyph[target]=0;
}
public Action:RenewingMistHeal2(Handle:timer, any:effid)
{
  new client = effectsdata[effid][1];
  new target = effectsdata[effid][2];
  new heal = effectsdata[effid][3];
  new jumps = effectsdata[effid][5];
  new roundcount = effectsdata[effid][9];
  new origin = effectsdata[effid][8];
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(effectsdata[effid][10]<GetClientDeaths(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(effectsdata[effid][4]<=0)
  {
    if(jumps>0)
    {
      new distance = effectsdata[effid][7];
      decl Float:location[3];
      GetClientAbsOrigin(effectsdata[effid][2],location);
      new team = effectsdata[effid][6];
      if(team==2)
        target=GetNearestClient("@t",location,float(distance),effectsdata[effid][2]);
      if(team==3)
        target=GetNearestClient("@ct",location,float(distance),effectsdata[effid][2]);
      if(target==0)
        return Plugin_Stop;
      jumps-=1;
      effectsdata[effid][4]+=origin;
      new effid2=CreateEffectData(client,target,heal,effectsdata[effid][4],jumps,team,distance,origin,roundcount,0);
      CreateTimer(1.0, RenewingMistHeal2, effid2, TIMER_REPEAT);
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,RenewingEffect(target));
      CreateTimer(4.0,EndRenewingEffect,datapack4,TIMER_HNDL_CLOSE);
      DeleteEffectData(effid);
      if(CheckRequirements(client,9,3,3,3) || IsFakeClient(client))
      {
        decl String:line[255],String:stargets[255];
        new healed;
        decl String:name[255],String:name2[255];
        GetClientName(client,name,sizeof(name));
        new targets[MAXPLAYERS];
        new amount;
        if(team==3)
        amount=GetNearClients("@ct",location,targets,float(distance));
        if(team==2)
        amount=GetNearClients("@t",location,targets,float(distance));
        if(!IsFakeClient(client))
        {
          wcGetTalentInfo(9,3,3,3,client,"heal",line);
          heal=RoundToNearest(float(heal)*StringToFloat(line));
        }
        else
          heal=RoundToNearest(float(heal)*1.20);
        for (new x=0;x<=(amount-1);x++)
        {
          GetClientName(targets[x],name2,sizeof(name2));
          if(x>0)
          Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
          else
          Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
          healed=wcHeal(client,targets[x],heal,0,true);
          if(targets[x]!=client)
          {
            WCMessage(targets[x],"uplift target",targets[x],name,healed);
          }
        }
        WCMessage(client,"uplift client",client,stargets,healed);
      }
      return Plugin_Stop;
    }
    else
    {
      DeleteEffectData(effid);
      return Plugin_Stop;
    }
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  wcHeal(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][3],0,true);
  if(HasGlyph(client, GLYPH_MONK_RENEWING)) 
  {
    g_iRenewingGlyph[target]+=1;
    CreateTimer(1.0,RemoveMist,target,TIMER_HNDL_CLOSE);
  }
  effectsdata[effid][4]-=1;
  return Plugin_Continue;
}
public GetSSS(client)  {
  return g_iSSS[client];
}

public SetSSS(client, amount) {
  g_iSSS[client]=amount;
}
public GetJabDmg(client)  {
  return g_iJabDmg[client];
}

public SetJabDmg(client, amount) {
  g_iJabDmg[client]=amount;
}
public GetRevival(client)  {
  return g_iRevivalHeal[client];
}

public SetRevival(client, amount) {
  g_iRevivalHeal[client]=amount;
}
public wcRunTalent_9_SSS(client)  //Seven Sided Strikes / Seven-Sided-Strikes
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetTalentInfo(9,2,2,2,client,"chance",line);
  new chance=StringToInt(line);
  if(chance>=random)
  {
    wcGetTalentInfo(9,2,2,2,client,"stacks",line);
    new stacks = StringToInt(line);
    wcGetTalentInfo(9,2,2,2,client,"duration",line);
    new Float:duration = StringToFloat(line);
    if (g_iSSS[client]<stacks)
    {
      g_iSSS[client]++;
      new Handle:datapack=CreateDataPack();
      WritePackCell(datapack,client);
      CreateTimer(duration,EndMFlurry,datapack,TIMER_HNDL_CLOSE);
    }
    if(g_iSSS[client]==1)
    {
      SetEffectAttacker(client,33);
      SetEffectSpellAtt(client,33);
    }
  }
}
public Action:EndMFlurry(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  if(g_iSSS[client]==1)
  {
    RemoveEffectSpellAtt(client,33);
    RemoveEffectAttacker(client,33);
  }
  if(g_iSSS[client]>0)
    g_iSSS[client]--;
}
public wcRunTalent_9_ChiExplosion(client,victim)
{
  decl String:line[255];
  wcGetTalentInfo(9,2,4,2,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(9,2,4,2,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new Float:timeleft=GetCDSpell(client,cdtype)-GetTickedTime();
  if(timeleft > 0.0)
    return 0;
  wcGetTalentInfo(9,2,4,2,client,"chance",line);
  new chance = StringToInt(line);
  new random=GetRandomInt(1,100);
  if(chance>=random)
  {
    SetCdSpell(client,cdtype,cd, 0, "Chi Explosion");
    wcGetTalentInfo(9,2,4,2,client,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetTalentInfo(9,2,4,2,client,"base",line);
    new base = StringToInt(line);
    wcGetTalentInfo(9,2,4,2,client,"duration",line);
    new Float:delay = StringToFloat(line);
    delay = delay*(1.0-GetHasteEffect(client));
    new team=GetClientTeam(victim);
    wcGetTalentInfo(9,2,4,2,client,"aoe",line);
    new aoe=StringToInt(line);
    new Float:agility=float(GetAgi(client)+GetBonusAgi(client));
    new dmg=RoundToNearest((agility*multi)+float(base));
    wcGetTalentInfo(9,2,4,2,client,"chimulti",line);
    dmg = RoundToNearest(float(dmg)*(1.0+(float(getChi(client))*StringToFloat(line))));
    wcGetTalentInfo(9,2,2,3,client,"multi",line);
    dmg = RoundToNearest(float(dmg)*(1.0+StringToFloat(line)));
    new effid=CreateEffectData(victim,client,1,dmg,aoe,team,0,901,g_RoundCount,GetClientDeaths(victim));
    if(effid!=0)
      CreateTimer(delay, EffectAoeDamage, effid, TIMER_REPEAT);
    AddEMsgAff("Chi Explosion", true);
    ChiBombEffect(victim);
  }
  return 1;
}
public Effect56(client, &damage)
{
    decl String:line[255];
    new Float:multi=1.0;
    wcGetTalentInfo(9,2,3,1,client,"multi",line);
    multi = multi + StringToFloat(line)*float(GetChiDpsInc(client));
    damage = RoundToNearest(float(damage)*multi);
    return true;
}
public GetChiDpsInc(client)
{
    return g_iChiDmgBuff[client];
}

public SetChiDpsInc(client, amount)
{
    g_iChiDmgBuff[client]=amount;
}
public Action:EndChiDpsInc(Handle:timer, Handle:datapack)
{
    ResetPack(datapack);
    new client = ReadPackCell(datapack);
    new stacks = ReadPackCell(datapack);
    if(g_iChiDmgBuff[client]==1)
    {
      RemoveEffectSpellAtt(client,56);
      RemoveEffectAttacker(client,56);
    }
    if(g_iChiDmgBuff[client]>0)
      g_iChiDmgBuff[client]-=stacks;
}
public wcRunTalent_9_2_3_1(client, chi) //Monk DPS Increase from Chi Use
{
  decl String:line[255];
  wcGetTalentInfo(9,2,3,1,client,"duration",line);
  new Float:duration = StringToFloat(line);
  wcGetTalentInfo(9,2,3,1,client,"stackscap",line);
  new stackscap = StringToInt(line);
  new stacks = chi;
  new temp=g_iChiDmgBuff[client]+stacks;
  if(g_iChiDmgBuff[client]<=stackscap)
  {
    if(g_iChiDmgBuff[client]==0)
    {
      SetEffectAttacker(client,56);
      SetEffectSpellAtt(client,56);
    }
    if(temp>stackscap)
      g_iChiDmgBuff[client]=stackscap;
    else
    {
      g_iChiDmgBuff[client]+=stacks;
      new Handle:datapack = CreateDataPack();
      WritePackCell(datapack, client);
      WritePackCell(datapack, stacks);
      CreateTimer(duration,EndChiDpsInc,datapack,TIMER_HNDL_CLOSE);
    }
  }
}
stock SpinningCraneKickEffect(client)
{
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  location[2]+=10;
  VS_BeamRingPoint(location, 100.00, 250.00, g_ef_physring1, g_ef_physring1, 1, 15, 1.0, 15.00, 2.00, {108, 255, 0, 255}, 20, 0);
  location[2]+=10;
  VS_BeamRingPoint(location, 100.00, 275.00, g_ef_physring1, g_ef_physring1, 1, 15, 1.0, 15.00, 2.00, {108, 255, 0, 255}, 20, 0);
  location[2]+=10;
  VS_BeamRingPoint(location, 100.00, 300.00, g_ef_physring1, g_ef_physring1, 1, 15, 1.0, 15.00, 2.00, {108, 255, 0, 255}, 20, 0);
  EmitAmbientSound("wc/spells/whirlwind.wav",location);
  location[2]+=10;
  VS_BeamRingPoint(location, 100.00, 275.00, g_ef_physring1, g_ef_physring1, 1, 15, 1.0, 15.00, 2.00, {108, 255, 0, 255}, 20, 0);
  location[2]+=10;
  VS_BeamRingPoint(location, 100.00, 250.00, g_ef_physring1, g_ef_physring1, 1, 15, 1.0, 15.00, 2.00, {108, 255, 0, 255}, 20, 0);
  location[2]+=10;
  VS_BeamRingPoint(location, 100.00, 225.00, g_ef_physring1, g_ef_physring1, 1, 15, 1.0, 15.00, 2.00, {108, 255, 0, 255}, 20, 0);
}


public wcRunTalent_9_SerpentKick(client) //Flying Serpent Kick
{
  decl String:line[255];
  wcGetTalentInfo(9,2,3,2,client,"mana",line);
  new manareq = StringToInt(line);
  if(IsFakeClient(client))
    manareq = 40;
  if(GetMana(client)>=manareq)
  {
    wcGetTalentInfo(9,2,3,2,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    if(IsFakeClient(client))
      cdtype = 19;
    wcGetTalentInfo(9,2,3,2,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    if(IsFakeClient(client))
      cd = 10.00;
    wcSetMana(client,GetMana(client)-manareq);
    if(GetChiSerenity(client)>0)
    SetChiSerenity(client, 0);
    else
    SetCdSpell(client,cdtype,cd, 0, "Flying Serpent Kick");

    wcGetTalentInfo(9,2,3,2,client,"power",line);
    new Float:power = StringToFloat(line);
    if(IsFakeClient(client))
      power = 600.0;
    wcGetTalentInfo(9,2,3,2,client,"zpower",line);
    new Float:zPower = StringToFloat(line);
    if(IsFakeClient(client))
      zPower = 350.0;
    HookLand(client);
    PushPlayerView(client, power, zPower);
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    EmitAmbientSound("wc/spells/serpentjump.wav",location);
  }
  else
    WCMessage(client, "no energy", client);
}
stock KickLand(client)  {
  decl String:line[255];
  //Damage code
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetTalentInfo(9,2,3,2,client,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(9,2,3,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new Float:agility=float(GetAgi(client)+GetBonusAgi(client));
  new dmg=RoundToNearest(agility*multi);
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,distance);
  if(team==3)
    amount=GetNearClients("@t",location,targets,distance);
  decl String:name2[255];
  new String:stargets[255];
  for (new x=0;x<=(amount-1);x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
    Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
    Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
  }
  if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);
  if(HasGlyph(client, GLYPH_MONK_FLYING))
    if(amount!=0)
      addChi(client,1);
    
  new color[4]={55,255,55,55};
  BeamRingEffect("@all",location,10.0,40.0,g_ef_lgtning,0,10,3.0,100.0,color,0.0,20);
  color[3]=255;
  BeamRingEffect("@all",location,50.0,245.0,g_ef_lgtning,0,10,3.0,50.0,color,0.0,10);
  WCMessage(client,"flying serpent kick caster",client,stargets,dmg);
  EmitAmbientSound("wc/spells/serpentland.wav",location);

  for (new x=0;x<=(amount-1);x++)
  {
    decl String:vicMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","flying serpent kick victim", targets[x]);
    wcGetTalentInfo(9,2,3,2,client,"name",line);
    wcSpellDamage(targets[x],client,dmg, "", vicMsg,line);
    BloodSpray(targets[x]);
  }
}
public wcRunTalent_9_ManaThistleTea(client, chi)
{
  decl String:line[255];
  wcGetTalentInfo(9,3,1,2,client,"energy",line);
  new regen = StringToInt(line)*chi;
  new newmana = GetMana(client)+regen;
  if(newmana>100)
    newmana = 100;
  wcSetMana(client, newmana);
  WCMessage(client,"mana thistle tea restore",client,regen,newmana);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  MonkManaThistleEffect(location);
}
new g_iCocoon[65];

public GetCocoon(client)
{
    return g_iCocoon[client];
}

public SetCocoon(client, amount)
{
    g_iCocoon[client]=amount;
}
public wcRunTalent_9_LifeCocoon(client) //Life Cocoon
{
  decl String:line[255];
  new target=GetAimTarget(client,45.0, true);
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target)) || (!IsPlayerAlive(target)))
    target=0;
  }
  if((target<=0) && SelfCast(client))
    target=client;
  if(SelfOnly(client))
    target=client;
  if(IsBoss())
  {
    if(CheckTargetOnly(client))
    target=g_iTargetOnly[client];
  }
  if(target<=0)
    return 0;
  wcGetTalentInfo(9,3,3,1,client,"cost",line);
  new cost = StringToInt(line);
  if(getChi(client)<cost) {
    WCMessage(client,"no chi",client);
    return 0;
  }
  wcGetTalentInfo(9,3,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(9,3,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  if(GetChiSerenity(client)>0)
  SetChiSerenity(client, 0);
  else
  SetCdSpell(client, cdtype, cd, 0, "Life Cocoon");
  wcGetTalentInfo(9,3,3,1,client,"duration",line);
  new Float:duration = StringToFloat(line);
  SetCocoon(target, GetCocoon(target)+1);
  new Handle:datapack2=CreateDataPack();
  WritePackCell(datapack2,target);
  CreateTimer(duration,RemoveCocoonBuff,datapack2,TIMER_HNDL_CLOSE);
  wcGetTalentInfo(9,3,3,1,client,"base",line);
  new absorb = StringToInt(line);
  wcGetTalentInfo(9,3,3,1,client,"multi",line);
  new Float:multi = StringToFloat(line);
  absorb+=GetSpellDmg(client,multi);
  wcClientAbsorb[target]=absorb;
  SetEffectVictim(target,5); // absorb shield
  SetEffectSpellVic(target,5); //Absorb magic dmg
  wcGetTalentInfo(9,3,3,1,client,"buff",line);
  new buff = RoundToNearest(StringToFloat(line)*100);
  decl String:name[50],String:name2[50];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  if(target!=client)
    WCMessage(target,"lifecocoon target",target,name,absorb,buff);
  WCMessage(client,"lifecocoon caster",client,name2,absorb,buff);

  new Handle:datapack=CreateDataPack();
  WritePackCell(datapack,target); // Write the client index of owner
  WritePackCell(datapack,5);
  CreateTimer(duration,DestroyEffectVictim,datapack,TIMER_HNDL_CLOSE);
  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,target); // Write the client index of owner
  WritePackCell(datapack3,5);
  CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);
  spendChi(client, cost);
  return 1;
}
stock CocoonBuff(client, &heal) //Life Cocoon buff
{
  decl String:line[255];
  wcGetTalentInfo(9,3,3,1,client,"buff",line);
  heal+=RoundToNearest(heal*StringToFloat(line));
}
public Action:RemoveCocoonBuff(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  SetCocoon(client, GetCocoon(client)-1);
  if(GetCocoon(client)<0)
    SetCocoon(client, 0);
}
public wcRunTalent_9_Resusciate(client)  { //Revive
  decl String:line[255];
  wcGetTalentInfo(9,3,4,1,client,"mana",line);
  new mana=StringToInt(line);
  if(GetMana(client)>=mana)
  {
    if(!(GetCombat(client)>0) || IsBoss())  {
      ChooseTarget(client, 9,99, 99, 9, "Choose a player to Resurrect");
    }
    else
    {
      WCMessage(client,"in combat",client,float(GetCombat(client))/2.0);
    }
  }
  else
    WCMessage(client,"no energy",client);
}
public wcRunTalent_9_Resusciate2(number, numbs, client, target)
{
  if(!(GetCombat(client)>0) || IsBoss())
  {
    decl String:line[255];
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    if(target > 0) {

      wcGetTalentInfo(9,3,4,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      if(IsFakeClient(client))
        cdtype=15;
      wcGetTalentInfo(9,3,4,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      if(IsFakeClient(client))
        cd=40.00;
      if(GetChiSerenity(client)>0)
      SetChiSerenity(client, 0);
      else
      SetCdSpell(client, cdtype, cd, 0, "Resusciate");
      decl String:name[255],String:name2[255];
      GetClientName(client,name,sizeof(name));
      GetClientName(target,name2,sizeof(name2));
      WCMessage(target,"resusciate target",target,name);
      WCMessage(client,"resusciate",client,name2);
      CS_RespawnPlayer(target);
      EmitAmbientSound("wc/spells/resurrect.wav",location);
      new hp = GetMaxHealth(target);
      hp = RoundToNearest(float(hp) * 0.5);
      wcHealingDone[client]+=hp;
      CreateTimer(0.1,ResurrectionEffect,target);
      wcSetMana(client,0);
    }
    else {  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"no target",client);
    }
  }
  else
  {
    WCMessage(client,"in combat",client,float(GetCombat(client))/2.0);
  }
}

public wcRunTalent_9_1_1_1(client)
{
    decl String:line[255];
    wcGetTalentInfo(9,1,1,1,client,"multi",line);
    new Float:multi = StringToFloat(line);
    new health = RoundToNearest(multi*float(GetMaxHealth(client)));
    health = wcHeal(client,client,health,0,false);
    WCMessage(client, "jab heal cast", client, health);
}
public wcRunTalent_9_CritParry(client)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,2,2,client,"parry",line);
  new Float:amount = StringToFloat(line);
  wcGetTalentInfo(9,1,2,2,client,"duration",line);
  new Float:duration = StringToFloat(line);
  AddParry(client, amount);
  wcRecalculateStats(client);
  new Handle:datapack=CreateDataPack();
  WritePackCell(datapack,client); // Write the client index of owner
  WritePackFloat(datapack,amount);
  //WritePackCell(datapack,g_RoundCount);
  CreateTimer(duration,RemoveCritParry,datapack,TIMER_HNDL_CLOSE);
}
public Action:RemoveCritParry(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  new Float:amount=ReadPackFloat(datapack);
  //new global = ReadPackCell(datapack);
  //if(global==g_RoundCount)
  RemoveParry(client, amount);
  wcRecalculateStats(client);
}

public wcRunTalent_9_DampenedHarm(client, &damage)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,4,2,client,"multi",line);
  new Float:multi = StringToFloat(line);
  wcGetTalentInfo(9,1,4,2,client,"dmgreduction",line);
  new Float:dmgreduction = StringToFloat(line);
  new healththreshold = RoundToNearest(multi*float(GetMaxHealth(client)));
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  MonkDampenHarmEffect(location);
  if(damage>=healththreshold)
  {
    damage=RoundToNearest(damage*dmgreduction);
    AddEMsgAff("dampened harm msg", true);
  }
}
new g_iKegSmash[65];

public GetSmash(client)
{
  return g_iKegSmash[client];
}

public SetSmash(client, amount)
{
  g_iKegSmash[client]=amount;
}

public wcRunTalent_9_KegSmash(client, victim)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,2,3,client,"chance",line);
  new chance = StringToInt(line);
  new random = GetRandomInt(1,100);
  if(chance>=random)
  {
    wcGetTalentInfo(9,1,2,3,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(9,1,2,3,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    new Float:timeleft=GetCDSpell(client,cdtype)-GetTickedTime();
    if(timeleft < 0.0)
    {
      wcGetTalentInfo(9,1,2,3,client,"duration",line);
      new Float:duration=StringToFloat(line);
      SetCdSpell(client,cdtype,cd, 0, "Keg Smash");
      SetEffectAttacker(victim,57);
      SetEffectSpellAtt(victim,57);
      wcGetTalentInfo(9,1,2,3,client,"amount",line);
      new amount=StringToInt(line);
      SetSmash(victim, GetSmash(victim)+amount);
      new Handle:datapack=CreateDataPack();
      WritePackCell(datapack,victim);
      WritePackCell(datapack,amount);
      CreateTimer(duration,RemoveSmash,datapack,TIMER_HNDL_CLOSE);
      AddEMsgAff("keg smash", true);
      SetBrew(victim, GetBrew(victim)+1);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,victim);
      WritePackCell(datapack2,1);
      CreateTimer(duration,RemoveBrew,datapack2,TIMER_HNDL_CLOSE);
      decl Float:location[3];
      GetClientAbsOrigin(victim,location);
      EmitAmbientSound("wc/spells/kegsmash.wav",location);
    }
  }
}

public Action:RemoveSmash(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  new amount=ReadPackCell(datapack);
  SetSmash(client,GetSmash(client)-amount);
  if(GetSmash(client)<=0)
  {
    SetSmash(client, 0);
    RemoveEffectAttacker(client, 57);
    RemoveEffectSpellAtt(client, 57);
  }
}
public Action:RemoveBrew(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  new amount=ReadPackCell(datapack);
  SetBrew(client,GetBrew(client)-amount);
  if(GetBrew(client)<0)
    SetBrew(client, 0);
}

public Effect57(client,damage)
{
  new amount = GetSmash(client);
  if(amount>3)
    amount=3;
  if(amount<=0)
    return damage;
  new Float:multi = 0.07;
  if(IsBoss())
    multi = 0.02;
  damage-= RoundToNearest(float(damage)*(amount*multi));
  return damage;
}
RenewingEffect(target) {
   new sprite = CreateEntityByName("env_smokestack");
   if (IsValidEntity(sprite)) {
      //Give client a target name
      decl String:sTemp[30];
      Format(sTemp,sizeof(sTemp),"player_%i",target);
      DispatchKeyValue(target, "targetname", sTemp);


      DispatchKeyValue(sprite, "basespread", "-50");
      DispatchKeyValue(sprite, "spreadspeed", "-5");
      DispatchKeyValue(sprite, "speed", "50");
      DispatchKeyValue(sprite, "startsize", "15");
      DispatchKeyValue(sprite, "endsize", "5");
      DispatchKeyValue(sprite, "rate", "200");
      DispatchKeyValue(sprite, "jetlength", "20");
      DispatchKeyValue(sprite, "twist", "30");

      DispatchKeyValue(sprite, "angles", "0 0 180");
      DispatchKeyValue(sprite, "rendercolor", "20 255 20");
      DispatchKeyValue(sprite, "renderamt", "10");

      DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");

      DispatchKeyValue(sprite, "parentname", sTemp);

      new Float:Client_Origin[3];
      GetClientAbsOrigin(target,Client_Origin);
      Client_Origin[2]+=90;
      DispatchKeyValueVector(sprite, "origin", Client_Origin);


      DispatchSpawn(sprite);
      SetVariantString(sTemp);
      AcceptEntityInput(sprite, "SetParent", target, target, 0);
      AcceptEntityInput(sprite, "TurnOn");
      return sprite;
	}
   return 0;
}

public Action:EndRenewingEffect(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new effect = ReadPackCell(datapack);
  if(effect > 0)
    KillEntity(effect);
}

public wcRunTalent_9_Afterlife(client,victim)
{
    decl String:line[255];
    wcGetTalentInfo(9,2,2,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetTalentInfo(9,2,2,1,client,"duration",line);
    new Float:duration=StringToFloat(line);
    g_fAfterlifeDuration = duration;
    wcGetTalentInfo(9,2,2,1,client,"selfduration",line);
    new Float:selfduration=StringToFloat(line);
    wcGetTalentInfo(9,2,2,1,client,"distance",line);
    new Float:distance=StringToFloat(line);
    new Float:location[3];
    GetClientAbsOrigin(victim,location);
    for(new i=0;i<3;i++)
        g_aAfterlifeLoc[i]=location[i];
    new Handle:datapack = CreateDataPack();
    WritePackCell(datapack, client);
    WritePackFloat(datapack, multi);
    WritePackFloat(datapack, distance);
    WritePackFloat(datapack, selfduration);
    CreateTimer(0.5, EndAfterlife, datapack, TIMER_HNDL_CLOSE);
}

public Action:EffectAfterlife(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client = ReadPackCell(datapack);
  if(!IsClientConnected(client)){ 
    g_fAfterlifeDuration=0.0;
    return Plugin_Stop; 
    }
  new Float:multi = ReadPackFloat(datapack);
  new Float:distance = ReadPackFloat(datapack);
  new Float:selfduration = ReadPackFloat(datapack);
  new team=GetClientTeam(client);
  new target;
  new targets[MAXPLAYERS];
  new amount;
  new health;

  if(team==3)
  {
      amount=GetNearClients("@ct",g_aAfterlifeLoc,targets,distance);
      VS_GlowSprite(g_aAfterlifeLoc, g_ef_greenglow1, 0.55, 2.00, 255);
  }
  if(team==2)
  {
      amount=GetNearClients("@t",g_aAfterlifeLoc,targets,distance);
      VS_GlowSprite(g_aAfterlifeLoc, g_ef_greenglow1, 0.55, 2.00, 255);
  }
  if(g_fAfterlifeDuration<=0)
      return Plugin_Stop;
  if(amount<=0){
      g_fAfterlifeDuration-=0.5;
      return Plugin_Continue;
      }
  if(g_fAfterlifeDuration>=(10.0-selfduration))
  {
      for(new j=0;j<amount;j++)
      {
          if(client==targets[j])
          {
              if(wcGetHealth(client)>=GetMaxHealth(client))
                return Plugin_Continue;
              health = RoundToNearest(multi*float(GetMaxHealth(client)));
              wcHeal(client,client,health,0,false);
              if(HasGlyph(client, GLYPH_MONK_AFTERLIFE))
                if(GetClass(client)==9)
                  addChi(client,1);
              WCMessage(client,"afterlife msg",client, health);
              g_fAfterlifeDuration=0.0;
              return Plugin_Stop;
          }
      }
      g_fAfterlifeDuration-=0.5;
      return Plugin_Continue;
  }
  else{
      if(team==3)
          target=GetNearestClient("@ct",g_aAfterlifeLoc,distance);
      if(team==2)
          target=GetNearestClient("@t",g_aAfterlifeLoc,distance);
      if(wcGetHealth(target)>=GetMaxHealth(target))
        return Plugin_Continue;
      health = RoundToNearest(multi*float(GetMaxHealth(target)));
      wcHeal(client,target,health,0,false);
      if(HasGlyph(client, GLYPH_MONK_AFTERLIFE))
        if(GetClass(target)==9)
          addChi(target,1);
      decl String:name[255],String:name2[255];
      GetClientName(client,name,sizeof(name));
      GetClientName(target,name2,sizeof(name2));
      WCMessage(client,"afterlife caster msg",client,name2,health);
      WCMessage(target,"afterlife target msg",target,name,health);
      g_fAfterlifeDuration=0.0;
      return Plugin_Stop;
  }
}

public Action:EndAfterlife(Handle:timer, Handle:datapack)
{
    ResetPack(datapack);
    new client = ReadPackCell(datapack);
    new Float:multi = ReadPackFloat(datapack);
    new Float:distance = ReadPackFloat(datapack);
    //new Float:duration = ReadPackFloat(datapack);
    new Float:selfduration = ReadPackFloat(datapack);

    new Handle:datapack2 = CreateDataPack();
    WritePackCell(datapack2, client);
    WritePackFloat(datapack2, multi);
    WritePackFloat(datapack2, distance);
    //WritePackFloat(datapack2, duration);
    WritePackFloat(datapack2, selfduration);
    CreateTimer(0.5, EffectAfterlife, datapack2, TIMER_REPEAT);
    return Plugin_Stop;
}
stock ChiBombEffect(client)
{
  new effid=CreateEffectData(client,60,0,0,0,0,0,0,0,0);
  CreateTimer(0.10, ChiBombEffectRepeat, effid, TIMER_REPEAT);
}
public Action:ChiBombEffectRepeat(Handle:timer, any:effid)
{
  new client = effectsdata[effid][1];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  if(effectsdata[effid][2]<=0) {
    VS_BeamRingPoint(location, 5.00, 450.00, g_ef_yellowflare1, g_ef_yellowflare1, 10, 30, 1.00, 50.00, 0.20, {255, 255, 128, 255}, 50, 0);
    return Plugin_Stop;
  }
  location[2]+=40;
  VS_GlowSprite(location, g_ef_yellowflare1, 0.10, 1.25, 255);
  effectsdata[effid][2]-=1;
  return Plugin_Continue;
}
stock UpliftEffect(client)
{
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  VS_BeamRingPoint(location, 5.00, 525.00, g_ef_yellowflare1, g_ef_yellowflare1, 10, 30, 2.50, 50.00, 0.20, {64, 255, 10, 255}, 35, 0);
}
public wcRunSpell_9_9(client)
{
  CreateTimer(0.5, CastLegacyAll, client);
}

public Action:CastLegacyAll(Handle:timer, any:client)  {
  if(!IsClientInGame(client))
  return;
  decl String:line[255];
  wcGetSpellInfo(9,9,"crit",line);
  new Float:multi=StringToFloat(line);
  //WCMessage(client,"legacy cast all",client);
  new clients[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
  amount=FindMatchingPlayers("@t",clients);
  if(team==3)
  amount=FindMatchingPlayers("@ct",clients);
  for(new i=0;i<amount;i++)  {
    if(IsClientInGame(clients[i]))
    CastLegacy(clients[i],multi,true);
  }
}

stock bool:CastLegacy(client, Float:multi, bool:cast=false, bool:custom=false)  {
  if((!custom) && LegacyClient[client])
    return false;
  SetCriticalStrikeBonus(client, GetCriticalStrikeBonus(client)+multi);
  wcRecalculateStats(client);
  if(!custom)
  LegacyClient[client]=true;
  if(cast) {
    WCMessage(client,"legacy aff",client);
  }
  return true;
}

public wcRunSpell_9_10(spellid,race,client) //Chi Burst
{
  decl String:line[255];
  new chi = wcGetSpellInfoInt(race, spellid, "chi");
  if (!spendChi(client, chi))  {
    // Not enough Chi
    WCMessage(client,"no chi",client);
    return;
  }

  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);

  SetCdSpell(client,cdtype,cd, spellid);

  // Launch Chi Burst
  new ent = CreateChiBurst(client);
  if (ent < 0)  {
    LogError("[WC] Chi burst could not be created. Invalid entity received");
    return;
  }
  // Create a timer which will do the spell code
  decl Float:location[3];
  GetClientAbsOrigin(client, location);
  new Handle:datapack = CreateDataPack();
  WritePackCell(datapack, client);
  WritePackCell(datapack, ent);
  WritePackCell(datapack, GetClientTeam(client));
  WritePackFloat(datapack, wcGetSpellInfoFloat(race, spellid, "distance"));
  WritePackFloat(datapack, location[0]);
  WritePackFloat(datapack, location[1]);
  WritePackFloat(datapack, location[2]);
  WritePackCell(datapack, g_RoundCount);
  CreateTimer((0.10*(1.0-GetHasteEffect(client))), ChiBurstThink, datapack, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
  location[2]+=40;
  EmitAmbientSound("wc/spells/chiburstcast.wav",location);
  WCMessage(client, "chi burst cast", client);
}

public Action:ChiBurstThink(Handle:timer, Handle:datapack)  {
  decl Float:loc[3];
  ResetPack(datapack);
  new client = ReadPackCell(datapack);
  new ent = ReadPackCell(datapack);
  new team = ReadPackCell(datapack);
  new Float:distance = ReadPackFloat(datapack);
  loc[0] = ReadPackFloat(datapack);
  loc[1] = ReadPackFloat(datapack);
  loc[2] = ReadPackFloat(datapack);
  new round = ReadPackCell(datapack);
  decl String:line[255];

  // Now get the targets it has already heal/damage
  new old_targets[65];
  new old_amount=0;
  while(IsPackReadable(datapack, 4))  {
    old_targets[old_amount++] = ReadPackCell(datapack);
  }

  // Check if client is in game and if entity still exists
  if (!IsValidEntity(ent))  {
    return Plugin_Stop;
  }
  if (ent <= 0) {
    return Plugin_Stop;
  }
  if (!IsClientInGame(client) || (team != GetClientTeam(client)))  {
    KillBurst(ent);
    return Plugin_Stop;
  }

  if (g_RoundCount != round)  {
    KillBurst(ent);
    return Plugin_Stop;
  }

  // See if there is anybody around
  decl Float:entpos[3];
  GetEntPropVector(ent, Prop_Send, "m_vecOrigin", entpos);



  new friends[65], enemies[65];
  new friends_amount, enemies_amount;

  if (team == 2)  {
    friends_amount = GetNearClients("@t",entpos,friends,distance);
    enemies_amount = GetNearClients("@ct",entpos,enemies,distance);
  }
  if (team == 3)  {
    friends_amount = GetNearClients("@ct",entpos,friends,distance);
    enemies_amount = GetNearClients("@t",entpos,enemies,distance);
  }

  wcGetSpellInfo(9,10,"base",line);
  new base = StringToInt(line);
  wcGetSpellInfo(9,10,"agimulti",line);
  new dmg = RoundToNearest((StringToFloat(line)*(float(GetAgi(client)+GetBonusAgi(client))))+float(base));
  wcGetSpellInfo(9,10,"multi",line);
  base+=GetSpellDmg(client,StringToFloat(line));
  decl String:name[255];
  decl Float:location[3];
  GetClientName(client,name,sizeof(name));
  // Heal
  for(new i=0; i<friends_amount; i++) {
    new target = friends[i];
    // Check if it is old
    new bool:found = false;
    for (new y=0; y<old_amount; y++)  {
      if (old_targets[y] == target) {
        found = true;
      }
    }
    if (found)  {
      continue; // Don't heal/damage twice
    }
    // Add it to old targets for future ticks
    WritePackCell(datapack, target);

    wcGetTalentInfo(9,3,1,3,client,"multi",line);
    base = RoundToNearest(float(base)*(1.0+StringToFloat(line)));
    new heal=wcHeal(client,target,base,0,true);
    if(target!=client)
      WCMessage(target,"chi burst ally",target,name,heal);
    // Healing code goes here :)  + Efect + sound.
    // Desired effect - some line which will go from entpos to target position.
    // Ent pos is a current location of our flying chicken
    GetClientAbsOrigin(target, location);
    location[2]+=40;
    if(target!=client)
      VS_BeamPoints(entpos, location, g_ef_orangelight1, g_ef_orangelight1, 0, 0, 0.50, 6.00, 6.00, 0, 0.00, {255, 155, 15, 255}, 160);
    EmitAmbientSound("wc/spells/chiburstheal.wav",location);
    if(HasGlyph(client, GLYPH_MONK_CHIBURST))
    {
      new targets[MAXPLAYERS];
      new amount;
      if(team==3)
        amount=GetNearClients("@ct",location,targets,200.0);
      if(team==2)
        amount=GetNearClients("@t",location,targets,200.0);
      for (new x=0;x<=(amount-1);x++)
      {
        wcHeal(client,targets[x],RoundToNearest(float(base)*0.15),0,true);
      }
      decl color[4];
      color[0]=10;
      color[1]=220;
      color[2]=10;
      color[3]=255;
      TE_SetupBeamRingPoint(location, 20.0, 200.0, g_ef_laserbeam, g_HaloSprite, 0, 15, 1.0, 5.0, 0.0, color, 10, 0);
      TE_SendToAll();
    }
    //PrintToChatAll("Hitting target %d", target);
  }

  // Damage
  for(new i=0; i<enemies_amount; i++) {
    new target = enemies[i];
    // Check if it is old
    new bool:found = false;
    for (new y=0; y<old_amount; y++)  {
      if (old_targets[y] == target) {
        found = true;
      }
    }
    if (found)  {
      continue; // Don't heal/damage twice
    }
    // Add it to old targets for future ticks
    WritePackCell(datapack, target);
    
    wcGetTalentInfo(9,2,2,3,client,"multi",line);
    dmg = RoundToNearest(float(dmg)*(1.0+StringToFloat(line)));
    if(CheckRequirements(client,9,2,4,1))
    {
      wcGetTalentInfo(9,2,4,1,client,"range",line);
      new Float:range=StringToFloat(line);
      new Float:dist=GetDistanceBetween(loc,entpos);
      if(dist>range)
      {
        wcGetTalentInfo(9,2,4,1,client,"bonus",line);
        dmg = RoundToNearest(float(dmg)*(1.0+StringToFloat(line)));
        wcGetTalentInfo(9,2,4,1,client,"duration",line);
        new Float:duration = StringToFloat(line);
        wcGetTalentInfo(9,2,4,1,client,"slowmulti",line);
        new Float:slowmulti = StringToFloat(line);
        new Float:oldspeed=GetClientSpeed(target);
        new Float:newspeed=GetNormalSpeed(target);
        new Float:newnewspeed = newspeed*slowmulti;
        if(newnewspeed<oldspeed)
        {
          SetClientSpeed(target,newnewspeed);
          new Handle:datapack2=CreateDataPack();
          WritePackCell(datapack2,target); // Write the client index of owner
          WritePackFloat(datapack2,newnewspeed);
          WritePackFloat(datapack2,newspeed);
          CreateTimer(duration,RemoveSpeedBurst,datapack2,TIMER_HNDL_CLOSE);
        }
      }
    }
    WCMessage(target,"chi burst enemy",target,name,dmg);
    wcSpellDamage(target,client,dmg, "", "",line);
    // Damaging code goes here :)  + Efect + sound.
    // Desired effect - some line which will go from entpos to target position.
    GetClientAbsOrigin(target, location);
    location[2]+=40;
    VS_BeamPoints(entpos, location, g_ef_orangelight1, g_ef_orangelight1, 0, 0, 0.50, 6.00, 6.00, 0, 0.00, {255, 255, 255, 255}, 160);
    EmitAmbientSound("wc/spells/chiburstdmg.wav",location);
    if(HasGlyph(client, GLYPH_MONK_CHIBURST))
    {
      new tteam = (team-1) % 2 + 2;
      wcSpellAoeDamage(target,client,RoundToNearest(float(dmg)*0.15),150,tteam,line);
      decl color[4];
      color[0]=220;
      color[1]=10;
      color[2]=10;
      color[3]=255;
      TE_SetupBeamRingPoint(location, 20.0, 150.0, g_ef_laserbeam, g_HaloSprite, 0, 15, 1.0, 5.0, 0.0, color, 10, 0);
      TE_SendToAll();
    }
    // Ent pos is a current location of our flying chicken

    //PrintToChatAll("Hitting target %d", target);
  }

  // I think thats it? :) We could see how much the object has travelled and kill it. But I would personally like to see it fly infinitely (there are walls anyway).

  return Plugin_Continue;
}
public Action:RemoveSpeedBurst(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:oldspeed=ReadPackFloat(datapack); //current speed
  new Float:speed=ReadPackFloat(datapack); //current speed
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (GetClientSpeed(client)!=oldspeed)
  {
    return Plugin_Stop;
  }
  SetClientSpeed(client,speed);
  SetEntityMoveType(client, MOVETYPE_WALK);
  return Plugin_Stop;
}


stock CreateChiBurst(client)  {

  decl Float:clienteyeangle[3], Float:anglevector[3], Float:clienteyeposition[3], Float:resultposition[3], entity;
  GetClientEyeAngles(client, clienteyeangle);
  GetClientEyePosition(client, clienteyeposition);
  GetAngleVectors(clienteyeangle, anglevector, NULL_VECTOR, NULL_VECTOR);
  NormalizeVector(anglevector, anglevector);
  ScaleVector(anglevector, 30.0);
  AddVectors(clienteyeposition, anglevector, resultposition);
  NormalizeVector(anglevector, anglevector);
  ScaleVector(anglevector, 450.0); // speed

  entity = CreateEntityByName("hegrenade_projectile");
  if (entity <= 0)  {
    return -1;
  }

  SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", client);
  setTakeDamage(entity, DAMAGE_NO);
  DispatchSpawn(entity);
  new Float:vecmax[3] = {4.0, 4.0, 4.0};
  new Float:vecmin[3] = {-4.0, -4.0, -4.0};
  SetEntPropVector(entity, Prop_Send, "m_vecMins", vecmin);
  SetEntPropVector(entity, Prop_Send, "m_vecMaxs", vecmax);
  SetEntProp(entity, Prop_Send, "m_nSolidType", 2 );
  SetEntProp(entity, Prop_Data, "m_CollisionGroup", 2, 4);
  SetEntProp(entity, Prop_Send, "m_CollisionGroup", 2, 4);

  SetEntityMoveType(entity, MOVETYPE_FLY);
  SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", client);
  SetEntityModel(entity, "models/effects/combineball.mdl");  // flying chicken? :D
  TeleportEntity(entity, resultposition, clienteyeangle, anglevector);

  new gascloud = CreateEntityByName("env_rockettrail");
  DispatchKeyValueVector(gascloud,"Origin", resultposition);
  DispatchKeyValueVector(gascloud,"Angles", clienteyeangle);

  new team = GetClientTeam(client);
  new Float:smokecolor[3] = {0.2, 0.0, 0.4};
  if (team == 2)  {
    smokecolor[0] = 0.82;
    smokecolor[2] = 0.22;
  }
  SetEntPropVector(gascloud, Prop_Send, "m_StartColor", smokecolor);
  SetEntPropFloat(gascloud, Prop_Send, "m_Opacity", 0.8);
  SetEntPropFloat(gascloud, Prop_Send, "m_SpawnRate", 150.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_ParticleLifetime", 1.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_StartSize", 5.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_EndSize", 30.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_SpawnRadius", 0.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_MinSpeed", 0.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_MaxSpeed", 20.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_flFlareScale", 1.0);
  DispatchSpawn(gascloud);
  decl String:target[64];
  Format(target, 64, "target%d_%f", client, GetGameTime());
  DispatchKeyValue(entity, "targetname", target);
  SetVariantString(target);
  AcceptEntityInput(gascloud, "SetParent");
  SetEntPropEnt(entity, Prop_Send, "m_hEffectEntity", gascloud);

  /*playsound(client, SOUNDTYPEROCKETFIRE1, clienteyeposition);*/
  /*playingrocketsound[entity] = true;*/
  /*playentitysound(entity, SOUNDROCKET1, resultposition);*/
  SDKHook(entity, SDKHook_StartTouch, BurstTouchHook);
  SDKHook(entity, SDKHook_OnTakeDamage, BurstDamageHook);

  /*new Handle:datapack = CreateDataPack();*/
  /*WritePackCell(datapack, EntIndexToEntRef(entity));*/
  /*CreateTimer(0.1, RocketSeekThink, datapack, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);*/
  setTakeDamage(entity, DAMAGE_YES);

  return entity;
}


public Action:BurstTouchHook(entity, other){
  if(other != 0){
    if(other == GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity")){
      return Plugin_Continue;
    }else{
      // Dont include player into a check
      if(!IsEntityCollidable(other, false, false, true)){
        return Plugin_Continue;
      }
      if (other <= GetMaxClients() && other >= 1)  {
        return Plugin_Continue;
      }
    }
  }

  KillBurst(entity);

  return Plugin_Continue;

}

public Action:BurstDamageHook(entity, &attacker, &inflictor, &Float:damage, &damagetype){
  if(GetEntProp(entity, Prop_Data, "m_takedamage") == DAMAGE_YES){
    KillBurst(entity);
  }

  return Plugin_Continue;
}

stock KillBurst(entity){

  SDKUnhook(entity, SDKHook_StartTouch, BurstTouchHook);
  SDKUnhook(entity, SDKHook_OnTakeDamage, BurstDamageHook);

  if(GetEntProp(entity, Prop_Data, "m_takedamage") == DAMAGE_YES){
    setTakeDamage(entity, DAMAGE_NO);
  }
  new gasentity = GetEntPropEnt(entity, Prop_Send, "m_hEffectEntity");
  AcceptEntityInput(gasentity, "Kill");
  AcceptEntityInput(entity, "Kill");


}





new g_iTigereyeBrew[65];
public GetTigerBrew(client)
{
  return g_iTigereyeBrew[client];
}

public SetTigerBrew(client, count)
{
  g_iTigereyeBrew[client]=count;
}
public wcRunTalent_9_TigereyeBrew(client, chi)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,3,2,client,"duration",line);
  new Float:duration = StringToFloat(line);
  SetTigerBrew(client, GetTigerBrew(client)+chi);
  new Handle:datapack2=CreateDataPack();
  WritePackCell(datapack2,client);
  WritePackCell(datapack2,chi);
  CreateTimer(duration,RemoveTigereyeBuff,datapack2,TIMER_HNDL_CLOSE);
}
public Action:RemoveTigereyeBuff(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new chi=ReadPackCell(datapack);
  SetTigerBrew(client, GetTigerBrew(client)-chi);
  if(GetTigerBrew(client)<0)
    SetTigerBrew(client, 0);
}
stock TigereyeBuff(client, &heal) //Tigereye Brew buff
{
  decl String:line[255];
  wcGetTalentInfo(9,1,3,2,client,"multi",line);
  new Float:multi=StringToFloat(line)*float(GetTigerBrew(client));
  heal+=RoundToNearest(heal*multi);
}
public wcRunTalent_9_HealingBrew(client)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,3,3,client,"limit",line);
  new Float:limit=StringToFloat(line);
  if(HasGlyph(client, GLYPH_MONK_HBREW))
    limit+=0.10;
  new Float:hp = float(wcGetHealth(client)) / float(GetMaxHealth(client));
  if(hp > limit)
    return 0;
  wcGetTalentInfo(9,1,3,3,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  new Float:timeleft=GetCDSpell(client,cdtype)-GetTickedTime();
  if(timeleft > 0.0)
    return 0;
  wcGetTalentInfo(9,1,3,3,client,"heal",line);
  new Float:heal=StringToFloat(line);
  wcGetTalentInfo(9,1,3,3,client,"cooldown",line);
  new Float:cooldown=StringToFloat(line);
  SetCdSpell(client,cdtype,cooldown,0,"Healing Brew");
  new bhealth = RoundToNearest(float(GetMaxHealth(client))*heal);
  bhealth = wcHeal(client,client,bhealth,0,true);
  WCMessage(client,"healing brew",client,bhealth);
  return 1;
}
public wcRunTalent_9_ChiSerenity(client)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(9,1,4,1,client,"cooldown",line);
  new Float:cooldown=StringToFloat(line);
  if(GetChiSerenity(client)>0)
  SetChiSerenity(client, 0);
  else
  SetCdSpell(client,cdtype,cooldown,0,"Chi Serenity");
  SetChiSerenity(client, 1);
  WCMessage(client,"chi serenity",client);
}
public GetChiSerenity(client)
{
  return g_iChiSerenity[client];
}

public SetChiSerenity(client, amount)
{
  g_iChiSerenity[client]=amount;
}
public wcRunTalent_9_Guard(client)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,4,1,client,"cost",line);
  new req=StringToInt(line);
  if(checkChi(client, req))
  {
    wcGetTalentInfo(9,1,4,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetTalentInfo(9,1,4,1,client,"duration",line);
    new Float:duration=StringToFloat(line);
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,client);
    WritePackFloat(datapack2,multi);
    CreateTimer(duration,RemoveGuard,datapack2,TIMER_HNDL_CLOSE);
    g_fParryReduce[client] += multi;
    WCMessage(client,"guard increase",client);
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    EmitAmbientSound("wc/spells/guard.wav",location);
    new effid=CreateEffectData(client,0,16,0,0,0,0,0,g_RoundCount,0);
    if(effid!=0)
    {
      effectsdata[effid][7]=GuardEffect(client);
      CreateTimer(duration/16.0, EffectGuard, effid, TIMER_REPEAT);
    }
    spendChi(client, req);
  }
}
public Action:RemoveGuard(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:multi=ReadPackFloat(datapack);
  g_fParryReduce[client] -= multi;
  if(g_fParryReduce[client]<0.2000)
    g_fParryReduce[client]=0.20;
  WCMessage(client,"guard end",client);
}
public Action:EffectGuard(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new eff = effectsdata[effid][7];
  if (!IsClientInGame(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  new ticks = effectsdata[effid][3]--;
  if (ticks>0)
  {
    if(IsValidEntity(eff))  SyncEntityLocation(eff);
    return Plugin_Continue;
  }
  DeleteEffectData(effid);
  KillEntity(eff);
  return Plugin_Stop;
}
GuardEffect(target) {
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
    DispatchKeyValue(sprite, "rendercolor", "255 255 10");
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
stock RevivalEffect(target) {
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
      DispatchKeyValue(sprite, "rendercolor", "20 255 20");
      DispatchKeyValue(sprite, "renderamt", "10");

      DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");

      DispatchKeyValue(sprite, "parentname", sTemp);

      new Float:Client_Origin[3];
      GetClientAbsOrigin(target,Client_Origin);
      GetClientEyePosition(client, location);
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


public wcRunTalent_9_Keg(client)
{
  decl String:line[255];
  wcGetTalentInfo(9,1,3,1,client,"chi",line);
  new req=StringToInt(line);
  if(spendChi(client, req))
  {
    // Spawn Keg
    // Pretty much it, wait for the keg to touch ground
    if (createKeg(client))  {
      wcGetTalentInfo(9,1,3,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      wcGetTalentInfo(9,1,3,1,client,"cooldowntype",line);
      new cdtype = StringToInt(line);
      SetCdSpell(client,cdtype,cd,0,"Dizzying Haze");
    }
    else  {
      LogError("[WC] Failed creating keg. Entity invalid");
    }
  }
}


stock createKeg(client) {
  decl Float:clienteyeangle[3], Float:anglevector[3], Float:clienteyeposition[3], Float:resultposition[3], entity;
  GetClientEyeAngles(client, clienteyeangle);
  GetClientEyePosition(client, clienteyeposition);
  GetAngleVectors(clienteyeangle, anglevector, NULL_VECTOR, NULL_VECTOR);
  NormalizeVector(anglevector, anglevector);
  ScaleVector(anglevector, 30.0);
  AddVectors(clienteyeposition, anglevector, resultposition);
  NormalizeVector(anglevector, anglevector);
  ScaleVector(anglevector, 350.0); // Speed

  decl Float:playerspeed[3];
  GetEntPropVector(client, Prop_Data, "m_vecVelocity", playerspeed);
  AddVectors(anglevector, playerspeed, anglevector);

  entity = CreateEntityByName("hegrenade_projectile");
  if (entity <= 0)  {
    return -1;
  }
  SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", client);
  setTakeDamage(entity, DAMAGE_NO);
  DispatchSpawn(entity);
  SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", client);
  SetEntPropFloat(entity, Prop_Data, "m_flGravity", 0.15);
  /*SetEntityModel(entity, "models/Items/ar2_grenade.mdl");*/
  /*SetEntityModel(entity, "models/lduke/chicken/chicken3.mdl");  // flying chicken? :D*/
  SetEntityModel(entity, "models/cf_models_src/beerfest_keg_pony01.mdl");
  /*SetEntityModel(entity, "models/cf_models_src/treasurechest01.mdl");*/
  // Play sound

  SDKHook(entity, SDKHook_StartTouch, KegTouchHook);
  SDKHook(entity, SDKHook_OnTakeDamage, KegDamageHook);

  setTakeDamage(entity, DAMAGE_YES);
  TeleportEntity(entity, resultposition, clienteyeangle, anglevector);

  new gascloud = CreateEntityByName("env_smoketrail");
  DispatchKeyValueVector(gascloud,"Origin", resultposition);
  DispatchKeyValueVector(gascloud,"Angles", clienteyeangle);
  new Float:smokecolor[3] = {0.2, 0.0, 0.4};
  new team = GetClientTeam(client);
  if (team == 2)  {
    smokecolor[0] = 0.82;
    smokecolor[2] = 0.22;
  }

  new Float:endcolor[3] = {0.0, 0.0, 0.0};
  SetEntPropVector(gascloud, Prop_Send, "m_StartColor", smokecolor);
  SetEntPropVector(gascloud, Prop_Send, "m_EndColor", endcolor);
  SetEntPropFloat(gascloud, Prop_Send, "m_Opacity", 0.2);
  SetEntPropFloat(gascloud, Prop_Send, "m_SpawnRate", 48.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_ParticleLifetime", 1.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_StartSize", 5.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_EndSize", 30.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_SpawnRadius", 0.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_MinSpeed", 0.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_MaxSpeed", 10.0);
  DispatchSpawn(gascloud);
  SetVariantString("!activator");
  AcceptEntityInput(gascloud, "SetParent", entity);
  SetEntPropEnt(entity, Prop_Send, "m_hEffectEntity", gascloud);


  return entity;
}

public Action:KegTouchHook(entity, other){
  if(other != 0){
    if(other == GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity")){
      return Plugin_Continue;
    }else{
      if(!IsEntityCollidable(other, true, true, true)){
        return Plugin_Continue;
      }
    }
  }

  KegActivate(entity);

  return Plugin_Continue;

}

public Action:KegDamageHook(entity, &attacker, &inflictor, &Float:damage, &damagetype){
  if(GetEntProp(entity, Prop_Data, "m_takedamage") == DAMAGE_YES){
    KegActivate(entity);
  }

  return Plugin_Continue;
}

stock KegActivate(entity) {

  SDKUnhook(entity, SDKHook_StartTouch, KegTouchHook);
  SDKUnhook(entity, SDKHook_OnTakeDamage, KegDamageHook);

  if(GetEntProp(entity, Prop_Data, "m_takedamage") == DAMAGE_YES){

    setTakeDamage(entity, DAMAGE_NO);
    decl Float:entityposition[3];
    GetEntPropVector(entity, Prop_Send, "m_vecOrigin", entityposition);
    new client = GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity");
    new gasentity = GetEntPropEnt(entity, Prop_Send, "m_hEffectEntity");
    AcceptEntityInput(gasentity, "Kill");
    entityposition[2] = entityposition[2] + 10.0;
    AcceptEntityInput(entity, "Kill");

    // Check if player is still active
    if (IsClientInGame(client)) {
      DoKegSpell(client, entityposition);
    }


  }

}


stock DoKegSpell(client, Float:location[3])  {
  decl String:line[255];
  EmitAmbientSound("wc/spells/dizzyinghaze.wav",location);

  new team = GetClientTeam(client);
  new targets[MAXPLAYERS];
  new amount=0;
  wcGetTalentInfo(9,1,3,1,client,"range",line);
  new Float:radius = StringToFloat(line);
  wcGetTalentInfo(9,1,3,1,client,"duration",line);
  new duration = StringToInt(line);
  duration=RoundToNearest(float(duration)*100);
  wcGetTalentInfo(9,1,3,1,client,"base",line);
  new base = StringToInt(line);
  wcGetTalentInfo(9,1,3,1,client,"multi",line);
  new Float:multi = StringToFloat(line);
  wcGetTalentInfo(9,1,3,1,client,"kegs",line);
  new kegamount = StringToInt(line);
  //wcGetTalentInfo(9,1,3,1,client,"alpha",line);
  //new alpha = StringToInt(line);
  wcGetTalentInfo(9,1,3,1,client,"amount",line);
  new miss = StringToInt(line);
  new dmg =RoundToNearest(float(GetAgi(client)+GetBonusAgi(client))*multi)+base;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,radius);
  else if(team==3)
    amount=GetNearClients("@t",location,targets,radius);
  decl String:name[255],String:name2[255],String:stargets[255];
  GetClientName(client,name,sizeof(name));
  for (new x=0;x<=(amount-1);x++)
  {
    wcSpellDamage(targets[x],client,dmg, "", "",line);
    //FadeEffect(targets[x],2,alpha,duration,{253,219,196,211});
    WCMessage(targets[x],"dizzying haze target",targets[x],name,dmg,RoundToNearest(float(duration)/100));
    SetBrew(targets[x], GetBrew(targets[x])+kegamount);
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,targets[x]);
    WritePackCell(datapack2,kegamount);
    CreateTimer(float(duration)/100,RemoveBrew,datapack2,TIMER_HNDL_CLOSE);
    SetMiss(targets[x],miss);
    SetEffectAttacker(targets[x],54); // Miss effect
  }
  WCMessage(client,"dizzying haze caster",client,stargets,dmg,RoundToNearest(float(duration)/100));
  MonkKegEffect(location, radius);
}
