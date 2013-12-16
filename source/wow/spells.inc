/*
* File: spells.inc
* Description: This file contains most spell functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1

new ClientSpells[65][51];
new ClientOldSpells[65][51];
new ClientSpellBar[65][7];
new ClientOldSpellBar[65][7];
new Handle:SpellsDB1;
new Handle:SpellsDB2;
new Handle:SpellsDB3;
new Handle:SpellsDB4;
new Handle:SpellsDB5;
new Handle:SpellsDB6;
new Handle:SpellsDB7;
new Handle:SpellsDB8;
new Handle:SpellsDB9;
new Float:ClientCDSpells[65][20];
new String:ClientCDSpellsNames[65][20][30];
new g_iBT[65];
new g_iCombatTimer[65];
new g_iHolyPower[65];
new Float:prevDelta[65];
new maxFear[65];
new bool:ReopenSpellbar[65];
new g_iVanishBreakDmg[65];
new g_iMiss[65];
new g_iHarvest[65];
new g_iEcho[65];

stock bool:addToSpellbar(client, id)  {
  for(new i=0; i<7; i++)  {
    if(wcGetSpellBarSpell(client,i) <= 0) {
      wcSetSpellBarSpell(client,i,id);
      return;
    }
  }
}

public Float:GetCDSpell(client,type)
{
  if((type<20)&&(type>=0))
  return ClientCDSpells[client][type];
  else
  return float(0);
}


stock SetCdSpell(client,type,Float:cd, spellId=0, String:name[]="")
{
  if((type<20)&&(type>=0))
  {
    if(!((GetCDSpell(client,type)-GetTickedTime())>0.0))
    {
      //if(GetStance(client)==3)
      //{
      //  cd -= 1.0;
      //}
      if(GetPoisoned(client)>0)
      cd+=GetPoisoned(client);
      if(hasCurse(client))
      cd = cd * (1.0 + getCursePower(client) );
      cd = cd * (1.0 - GetHasteEffect(client));

      //Shocks?
      /*if(GetClass(client) == 6 && CheckRequirements(client,6,1,3,1))
      {
        if(spellId == 3 || spellId == 4 || spellId == 5)
        {
          decl String:buffer[255];
          wcGetTalentInfo(6,1,3,1,client,"multi",buffer);
          cd *= StringToFloat(buffer);
        }

      }
      */
    }
    ClientCDSpells[client][type]=GetTickedTime()+cd;
  }
  else
  {
    LogError("set CD spell error, out of array (%d)", type);
    return;
  }
  if(cd > 0.0)
  {
    if(spellId > 0)
    {
      decl String:spname[255];
      wcGetSpellInfo(GetClass(client), spellId, "name", spname, client);
      Format(ClientCDSpellsNames[client][type], 30, "%s", spname);
    }
    else
    {
      Format(ClientCDSpellsNames[client][type], 30, "%s", name);
    }
  }
}

GetCDSpellName(client, type, String:back[], size)
{
  Format(back, size, "%s", ClientCDSpellsNames[client][type]);
}

public wcGetSpellBarSpell(client,i)
{
  return ClientSpellBar[client][i];
}

public wcGetOldSpellBarSpell(client,i)
{
  return ClientOldSpellBar[client][i];
}

public wcSetSpellBarSpell(client,i,spell)
{
  ClientSpellBar[client][i]=spell;
}

public wcSetOldSpellBarSpell(client,i,spell)
{
  ClientOldSpellBar[client][i]=spell;
}

public wcSetSpell(client,spell)
{
  for(new x=1;x<=50;x++)
  {
    if (ClientSpells[client][x]==0)
    {
      ClientSpells[client][x]=spell;
      break;
    }
  }
}

public wcSearchSpell(client,spell)
{
  for(new x=1;x<=50;x++)
  {
    if(ClientSpells[client][x]==spell)
    return x;
  }
  return 0;
}

public wcSetStaticSpell(client,x,spell)
{
  if( (spell>0) && isAutoCast(GetClass(client), spell) )
  setAutoCast(client, spell, true);
  ClientSpells[client][x]=spell;
}

public wcSetOldStaticSpell(client,x,spell)
{
  ClientOldSpells[client][x]=spell;
}

public wcGetSpell(client,id)
{
  if(IsZombie(client))
  return 0;
  return ClientSpells[client][id];
}

public wcGetOldSpell(client,id)
{
  return ClientOldSpells[client][id];
}

stock wcGetSpellInfo(race,spellid,String:info[],String:line[255], client=0)
{
  decl String:trans[50];

  if(race<=0)
  {
    Format(line, sizeof(line), "");
    return;
  }
  new Handle:SpellsDB = GetSpellsHandle(race);
  KvRewind(SpellsDB);
  decl String:s_spellid[5];
  IntToString(spellid,s_spellid,sizeof(s_spellid));
  KvJumpToKey(SpellsDB,s_spellid);

  KvGetString(SpellsDB, "name", trans, sizeof(trans), "0");
  if(client == 0 || StrEqual(trans, "0"))
  KvGetString(SpellsDB, info, line, sizeof(line), "0");
  else
  {
    if(StrEqual(info, "name", false))
    {
      Format(trans, sizeof(trans), "%s name", trans);
    }
    else
    {
      LogError("[WC] Logics error 186 debug: %s", info);
    }
    Format(line, 255, "%T", trans, client);
  }
}

public wcGetSpellInfoInt(race,spellid,String:info[])
{
  if(race<=0)
  {
    return 0;
  }
  new Handle:SpellsDB = GetSpellsHandle(race);
  KvRewind(SpellsDB);
  decl String:s_spellid[5];
  IntToString(spellid,s_spellid,sizeof(s_spellid));
  KvJumpToKey(SpellsDB,s_spellid);
  //if(DEBUG)
  //  PrintToServer("returnig %s = %d for spell %d", info, KvGetNum(SpellsDB, info, 0), spellid);
  return KvGetNum(SpellsDB, info, 0);
}

public Float:wcGetSpellInfoFloat(race,spellid,String:info[])
{
  if(race<=0)
  {
    return 0.0;
  }
  new Handle:SpellsDB = GetSpellsHandle(race);
  KvRewind(SpellsDB);
  decl String:s_spellid[5];
  IntToString(spellid,s_spellid,sizeof(s_spellid));
  KvJumpToKey(SpellsDB,s_spellid);
  return KvGetFloat(SpellsDB, info, 0.0);
}

public wcShowHM()
{
  CreateTimer(0.5, ManaTimer, 0, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
  CreateTimer(1.0, ThreatTimer, 0, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
  CreateTimer(0.2, VisualTimer, 0, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}

public Action:VisualTimer(Handle:timer, any:temp)
{
  for(new x=1;x<=GetMaxClients();x++)
  if(IsClientConnected(x)) if(IsClientInGame(x)) if(IsPlayerAlive(x))  {
    SyncClientHpBar(x);
    /*VisualHP(x);*/
  }
  else {
    CheckClientHpBar(x);
  }

  return Plugin_Continue;
}

public Action:RemoveCDSpell(Handle:timer,Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new cdtype=ReadPackCell(datapack); // Get type
  if(IsClientInGame(client))
  SetCdSpell(client,cdtype,float(0), 0);
}

public wcRunSpell_1_3(spellid,race,client) //enrage rengeneration
{
  /*if(GetConVarBool(g_BossMod))
  {
    WCMessage(client,"boss spell blocked",client);
    return;
  }*/
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);


  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, spellid);
  //  SetEffectAttacker(client,2); // bloothirst effect
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  new Float:location2[3];
  for (new x=0;x<=2;x++)
  location2[x]=location[x];
  location2[2]+=100;
  new color[4]= {255,255,255,255};
  BeamEffect("@all",location,location2,g_ef_airlock_laser,5,5.0,20.0,20.0,color,0.0,5);
  BeamRingEffect("@all",location2,20.0,30.0,g_ef_airlock_laser,0,3,5.0,40.0,color,0.0,3);
  new amount = RoundToNearest(float(GetMaxHealth(client))*multi);
  new tick = RoundToNearest( float(amount) / 4.0);
  if(HasGlyph(client, GLYPH_WARRIOR_REGEN))
  {
    wcGetSpellInfo(race,spellid,"glyph",line);
    new Float:glyphmulti=StringToFloat(line);
    new glyph=RoundToNearest(float(GetMaxHealth)*glyphmulti);
    Renew(client, client, glyph, 1, 0, 0.0);
    WCMessage(client,"enrage regeneration cast with glyph",client, glyphmulti*100, glyph);
  }
  Renew(client, client, tick, 4, 0, 2.0);
  WCMessage(client,"enrage regeneration cast",client, amount);
}

public wcRunSpell_1_2(spellid,race,client)
{
  //  new target=GetAimTarget(client);
  //  if(target>0)
  //  {
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, spellid);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetSpellInfo(race,spellid,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"minimum",line);
  new minimum=StringToInt(line);
  wcGetTalentInfo(1,2,2,3,client,"increment",line);
  new Float:talentmulti = StringToFloat(line);
  multi *= 1.0 + talentmulti;
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
  new dmg=RoundToNearest(float(minimum)+float(GetDamage(client))*multi);
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

  if(talentmulti>0.0)
  WCMessage(client,"improved whirlwind",client,dmg,stargets);
  else
  WCMessage(client,"whirlwind",client,dmg,stargets);

  EmitAmbientSound("wc/spells/whirlwind.wav",location);
  new color[4] = { 255, 255, 255, 130 };
  new Float:width=20.0;
  BeamRingEffect("@all",location,20.0,80.0,g_ef_ocean,0,10,2.6,width,color,0.0,10);
  location[2]+=20;
  width+=20.0;
  BeamRingEffect("@all",location,20.0,80.0,g_ef_ocean,0,10,2.6,width,color,0.0,10);
  location[2]+=20;
  width+=20.0;
  BeamRingEffect("@all",location,20.0,80.0,g_ef_ocean,0,10,2.6,width,color,0.0,10);

  for (new x=0;x<=(amount-1);x++)
  {
    decl String:vicMsg[300];
    if(talentmulti>0.0)
    Format(vicMsg,sizeof(vicMsg),"%T","improved whirlwind target", targets[x]);
    else
    Format(vicMsg,sizeof(vicMsg),"%T","whirlwind target", targets[x]);

    wcPhysicalDamage(targets[x],client,dmg, "", vicMsg, true);
    if(HasGlyph(client, GLYPH_WARRIOR_WHIRLWIND))  {
      Effect40(targets[x],client);
    }
    BloodSpray(targets[x]);
  }
  //  }
}

public wcRunSpell_1_4(spellid,race,victim,attacker,&totaldmg)
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"mana",line);
  new mana = StringToInt(line);
  if(HasGlyph(attacker, GLYPH_WARRIOR_HEROIC))
  mana-=2;
  if(mana<=GetMana(attacker))
  {
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance=StringToInt(line);
    if(IsDoubleHand(attacker))
    chance = RoundToNearest(chance*1.5);
    if(chance>=random)
    {
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line);
      new dmg=RoundToNearest(float(GetDamage(attacker))*multi);
      totaldmg+=dmg;
      wcSetMana(attacker,GetMana(attacker)-mana);
      decl Float:location[3];
      GetClientAbsOrigin(attacker,location);
      EmitAmbientSound("wc/spells/warriorweaponswing.wav",location);
      AddEMsgAff("heroic strike", true);
    }
  }
  return totaldmg;
}

public wcRunSpell_1_5(spellid,race,victim,attacker,&totaldmg)
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"mana",line);
  new mana = StringToInt(line);
  if(mana<=GetMana(attacker))
  {
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance=StringToInt(line);
    if(!IsDoubleHand(attacker))
    chance/=2;
    if(chance>=random)
    {
      wcGetSpellInfo(race,spellid,"dmg",line);
      new dmg=StringToInt(line);
      wcGetSpellInfo(race,spellid,"speed",line);
      new Float:multi=StringToFloat(line);
      totaldmg+=dmg;
      decl Float:location[3];
      GetClientAbsOrigin(attacker,location);
      wcSetMana(attacker,GetMana(attacker)-mana);
      EmitAmbientSound("wc/spells/warriorweaponswing.wav",location);
      wcGetTalentInfo(1,3,1,2,attacker,"chance",line);
      chance = StringToInt(line);
      random = GetRandomInt(1,100);
      new Float:time;
      wcGetSpellInfo(race,spellid,"time",line);
      time=StringToFloat(line);
      if(chance >= random)
      {
        wcGetTalentInfo(1,3,1,2,attacker,"multi",line);
        multi = StringToFloat(line);
        wcGetTalentInfo(1,3,1,2,attacker,"duration",line);
        time = StringToFloat(line);
        AddEMsgAff("imp hamstring",true);
      }
      else
      AddEMsgAff("hamstring",true);
      new Float:oldspeed=GetClientSpeed(victim);
      new Float:newspeed=GetNormalSpeed(victim);
      newspeed*=multi;
      if(newspeed<oldspeed)
      {
        SetClientSpeed(victim,newspeed);
        new Handle:datapack=CreateDataPack();
        WritePackCell(datapack,victim); // Write the client index of owner
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
    }
  }
}



public wcRunSpell_6_1(spellid,race,victim,attacker,&totaldmg) //Shaman Shield Block
{
  new Float:chance = GetBlockChance(victim,true,attacker);
  decl Float:clienta[3],Float:targeta[3];
  GetClientEyeAngles(attacker, clienta);
  GetClientEyeAngles(victim, targeta);
  decl String:line[255];
  new Float:angle=clienta[1]-targeta[1];
  if(angle<0.0)
  angle*=-1.0;
  if(angle>180.0)
  angle = 360.0-angle;

  if( (chance>=GetRandomFloat(0.0001,1.0)) && (angle > 100.0 || !IsValidShooting(attacker, victim)) )
  {
    for(new x=1;x<=50;x++)
    {
      if((spellid=wcGetSpell(victim,x))!=0)
      {
        wcGetSpellInfo(race,spellid,"event",line);
        if(StrEqual(line,"player_block",false))
        {
          new tmp=0;
          wcRunSpell(spellid,race,victim,attacker,totaldmg,tmp,tmp,tmp,tmp,tmp,tmp,victim);
        }
      }
    }
    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    EmitAmbientSound("wc/spells/shieldwall.wav",location);
    totaldmg=0;
    AddEMsgAff("blocked damage", true);
  }
  return totaldmg;
}

public wcRunSpell_6_2(spellid,race,victim,attacker,&dmg)
{
  if(IsPlayerAlive(victim))
  {
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance = StringToInt(line);
    if(chance>=GetRandomInt(1,100))

    {
      new manareq = GetSpellCost(attacker, spellid);
      new newmana=GetMana(attacker)-manareq;
      if(newmana<0)
      {
        return false;
      }
      wcSetMana(attacker,newmana);
      wcGetTalentInfo(6,2,1,2,attacker,"increment",line);
      new Float:talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line)+talentbonus;
      dmg=RoundToNearest(float(dmg)*multi);
      decl Float:location[3];
      GetClientAbsOrigin(victim,location);
      if(HasGlyph(attacker, GLYPH_SHAMAN_WINDFURY))
      {
        new heal = RoundToNearest(float(dmg)*0.10);
        wcHeal(attacker,attacker,heal,0,false);
      }
      AddEMsgAff("windfury cast", true);
      if(!IsBoss())
      {
        EmitAmbientSound("wc/spells/rend.wav",location);
        new color[4] = { 150, 150, 100, 130 };
        new Float:width=20.0;
        BeamRingEffect("@all",location,20.0,80.0,g_ef_ocean,0,10,2.6,width,color,0.0,10);
        location[2]+=20;
        width+=20.0;
        BeamRingEffect("@all",location,20.0,80.0,g_ef_ocean,0,10,2.6,width,color,0.0,10);
        location[2]+=20;
        width+=20.0;
        BeamRingEffect("@all",location,20.0,80.0,g_ef_ocean,0,10,2.6,width,color,0.0,10);
      }
      return true;
    }
  }
  return false;
}

public wcRunSpell_6_3(spellid,race,victim,attacker) //Flame Shock
{
  if(IsPlayerAlive(victim))
  {
    new random=GetRandomInt(1,100);
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance=StringToInt(line)+GetThunder(attacker,1);

    if(IsDoubleHand(attacker))
    chance=RoundToNearest(float(chance)/2.0);
    if(chance>=random)
    {

      if(CheckRequirements(attacker,6,1,2,2))
      {
        wcGetTalentInfo(6,1,2,2,attacker,"increment",line);
        new thunderbonus = StringToInt(line);
        SetThunder(attacker, 1, GetThunder(attacker,1)+thunderbonus);
        wcGetTalentInfo(6,1,2,2,attacker,"duration",line);
        new Float:duration=StringToFloat(line);
        new Handle:datapack=CreateDataPack();
        WritePackCell(datapack,attacker);
        WritePackCell(datapack,1);
        WritePackCell(datapack,thunderbonus);
        WritePackCell(datapack,g_RoundCount);
        CreateTimer(duration,EndThunder,datapack,TIMER_HNDL_CLOSE);
      }

      wcGetTalentInfo(6,2,3,2,attacker,"chance",line);
      new echochance = StringToInt(line);
      new echorandom =GetRandomInt(1,100);
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line);
      if(echochance>=echorandom)
      {
        g_iEcho[attacker]=0;
        WCMessage(attacker,"Echo of the Elements activate",attacker);
      }
      else
        SetCdSpell(attacker,cdtype,cd, spellid);
      if(echochance>=echorandom)
        wcRunSpell_6_3(spellid,race,victim,attacker);

      new manareq = GetSpellCost(attacker, spellid);
      if(g_iEcho[attacker]>0)
      {
        manareq=0;
        g_iEcho[attacker]=0;
      }
      new newmana=GetMana(attacker)-manareq;
      if(newmana<0)
      {
        return false;
      }
      wcSetMana(attacker,newmana);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line);
      wcGetTalentInfo(6,1,1,4,attacker,"increment",line);
      multi+=StringToFloat(line);
      new dmg=GetSpellDmg(attacker,multi);
      wcGetSpellInfo(race,spellid,"base",line);
      dmg += StringToInt(line);
      wcGetSpellInfo(race,spellid,"multief",line);
      new Float:multief=StringToFloat(line);
      new dmgef=GetSpellDmg(attacker,multief);
      if(HasGlyph(attacker, GLYPH_SHAMAN_NPOWER))
        dmgef=RoundToNearest(dmgef*2.00);
      if(HasGlyph(attacker, GLYPH_SHAMAN_NPOWER))
        dmg=RoundToNearest(dmg*0.50);
      decl Float:location[3];
      GetClientAbsOrigin(victim,location);
      location[2]+=50;
      EmitAmbientSound("wc/spells/fireball.wav",location);
      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","flameshock cast vic", victim, dmgef);
      Format(attMsg,sizeof(attMsg),"%T","flameshock cast", attacker, dmgef);
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(victim,attacker,dmg, attMsg, vicMsg,line);
      GlowSpriteEffect("@all",location, g_redglow1, 1.0, 1.0, 255);
      wcGetSpellInfo(race,spellid,"tickdelay",line);
      new Float:tickdelay=StringToFloat(line);
      //wcGetTalentInfo(6,1,2,3,attacker,"duration",line);
      //new talentbonus = StringToInt(line);
      wcGetSpellInfo(race,spellid,"ticks",line);
      new ticks=StringToInt(line);
      ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(attacker)));
      tickdelay = tickdelay*(1.0-GetHasteEffect(attacker));
      new effid=CreateEffectData(victim,attacker,ticks,dmgef,0,0,0,1,g_RoundCount);
      g_iFlameShock[victim]+=1;
      new Handle:datapack=CreateDataPack();
      WritePackCell(datapack,victim);
      CreateTimer(tickdelay*ticks,RemoveFlameShockInt,datapack,TIMER_HNDL_CLOSE);
      if(effid!=0)
      {
        CreateTimer(tickdelay, EffectFlameShockAction, effid, TIMER_REPEAT);
      }
      //end
      return true;
    }
  }
  return false;
}
public Action:RemoveFlameShockInt(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  g_iFlameShock[client]-=1;
  if(0>g_iFlameShock[client])
    g_iFlameShock[client]=0;
}
public wcRunSpell_6_4(spellid,race,victim,attacker) //Frost Shock
{
  if(IsPlayerAlive(victim))
  {
    new random=GetRandomInt(1,100);
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance=StringToInt(line)+GetThunder(attacker,2);

    if(IsDoubleHand(attacker))
    chance=RoundToNearest(float(chance)/2.0);
    if(chance>=random)
    {
      if(CheckRequirements(attacker,6,1,2,2))
      {
        wcGetTalentInfo(6,1,2,2,attacker,"increment",line);
        new thunderbonus = StringToInt(line);
        SetThunder(attacker, 2, GetThunder(attacker,2)+thunderbonus);
        wcGetTalentInfo(6,1,2,2,attacker,"duration",line);
        new Float:duration=StringToFloat(line);
        new Handle:datapack=CreateDataPack();
        WritePackCell(datapack,attacker);
        WritePackCell(datapack,2);
        WritePackCell(datapack,thunderbonus);
        WritePackCell(datapack,g_RoundCount);
        CreateTimer(duration,EndThunder,datapack,TIMER_HNDL_CLOSE);
      }
      wcGetTalentInfo(6,2,3,2,attacker,"chance",line);
      new echochance = StringToInt(line);
      new echorandom =GetRandomInt(1,100);
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line);
      if(echochance>=echorandom)
        {
          WCMessage(attacker,"Echo of the Elements activate",attacker);
          g_iEcho[attacker]=1;
        }
      else
        SetCdSpell(attacker,cdtype,cd, spellid);
      if(echochance>=echorandom)
        wcRunSpell_6_4(spellid,race,victim,attacker);
      new manareq = GetSpellCost(attacker, spellid);
      new newmana=GetMana(attacker)-manareq;
      if(g_iEcho[attacker]>0)
      {
        manareq=0;
        g_iEcho[attacker]=0;
      }
      if(newmana<0)
      {
        return false;
      }
      wcSetMana(attacker,newmana);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line);
      wcGetTalentInfo(6,1,1,4,attacker,"increment",line);
      multi+=StringToFloat(line);
      //wcGetTalentInfo(6,1,2,3,attacker,"frostshock",line);
      //new Float:talentb=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"slowmulti",line);
      new Float:slowmulti=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"slowtime",line);
      new Float:slowtime=StringToFloat(line);
      new dmg=GetSpellDmg(attacker,multi);
      wcGetSpellInfo(race,spellid,"base",line);
      dmg += StringToInt(line);
      decl Float:location[3];
      GetClientAbsOrigin(victim,location);
      location[2]+=50;
      EmitAmbientSound("wc/spells/frostbolt.wav",location);
      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","frostshock cast vic", victim);
      Format(attMsg,sizeof(attMsg),"%T","frostshock cast", attacker);
      wcGetSpellInfo(race,spellid,"name",line);
      new bool:glyph = HasGlyph(attacker, GLYPH_SHAMAN_FROST);
      if(glyph)
      dmg = RoundToNearest(1.5 * dmg);
      wcSpellDamage(victim,attacker,dmg, attMsg, vicMsg,line);
      GlowSpriteEffect("@all",location, g_blueglow1, 1.0, 1.0, 255);
      if(!glyph)  {
        new Float:oldspeed=GetClientSpeed(victim);
        new Float:newspeed=GetNormalSpeed(victim);
        newspeed*=slowmulti;
        if(newspeed<oldspeed)
        {
          SetClientSpeed(victim,newspeed);
          new Handle:datapack2=CreateDataPack();
          WritePackCell(datapack2,victim); // Write the client index of owner
          WritePackFloat(datapack2,newspeed);
          WritePackFloat(datapack2,slowmulti);
          CreateTimer(slowtime,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
        }
      }
      //end
      return true;
    }
  }
  return false;
}

public wcRunSpell_6_5(spellid,race,victim,attacker) //Earth Shock
{
  if(IsPlayerAlive(victim))
  {
    new random=GetRandomInt(1,100);
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance=StringToInt(line)+GetThunder(attacker,3);

    if(IsDoubleHand(attacker))
    chance=RoundToNearest(float(chance)/2.0);
    if(chance>=random)
    {
      if(CheckRequirements(attacker,6,1,2,2))
      {
        wcGetTalentInfo(6,1,2,2,attacker,"increment",line);
        new thunderbonus = StringToInt(line);
        SetThunder(attacker, 3, GetThunder(attacker,3)+thunderbonus);
        wcGetTalentInfo(6,1,2,2,attacker,"duration",line);
        new Float:duration=StringToFloat(line);
        new Handle:datapack=CreateDataPack();
        WritePackCell(datapack,attacker);
        WritePackCell(datapack,1);
        WritePackCell(datapack,thunderbonus);
        WritePackCell(datapack,g_RoundCount);
        CreateTimer(duration,EndThunder,datapack,TIMER_HNDL_CLOSE);
      }

      wcGetTalentInfo(6,2,3,2,attacker,"chance",line);
      new echochance = StringToInt(line);
      new echorandom =GetRandomInt(1,100);
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line);
      if(echochance>=echorandom)
        {
          WCMessage(attacker,"Echo of the Elements activate",attacker);
          g_iEcho[attacker]=1;
        }
      else
        SetCdSpell(attacker,cdtype,cd, spellid);
      if(echochance>=echorandom)
        wcRunSpell_6_5(spellid,race,victim,attacker);

      new manareq = GetSpellCost(attacker, spellid);
      if(g_iEcho[attacker]>0)
      {
        manareq=0;
        g_iEcho[attacker]=0;
      }
      new newmana=GetMana(attacker)-manareq;
      if(newmana<0)
      {
        return false;
      }
      wcSetMana(attacker,newmana);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line);
      wcGetTalentInfo(6,1,1,4,attacker,"increment",line);
      multi+=StringToFloat(line);
      //wcGetTalentInfo(6,1,2,3,attacker,"increment",line);
      //new Float:talentb=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"silence",line);
      new Float:silence=StringToFloat(line);
      new dmg=GetSpellDmg(attacker,multi);
      wcGetSpellInfo(race,spellid,"base",line);
      dmg += StringToInt(line);
      decl Float:location[3];
      GetClientAbsOrigin(victim,location);
      location[2]+=50;
      EmitAmbientSound("wc/spells/fireball.wav",location);
      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","earthshock cast vic", victim, silence);
      Format(attMsg,sizeof(attMsg),"%T","earthshock cast", attacker, silence);
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(victim,attacker,dmg, attMsg, vicMsg,line);
      GlowSpriteEffect("@all",location, g_plasmaember, 1.0, 1.0, 255);
      SetSilence(victim,silence);
      return true;
    }
  }
  return false;
}

public wcRunSpell_6_6(spellid,race,client) //Healing Wave
{
  new target=GetAimTarget(client,45.0,true);
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
  {
    WCMessage(client,"target full hp",client);
    return 0;
  }
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcGetTalentInfo(6,3,1,1,client,"multi",line);
  new Float:talentbonus=StringToFloat(line);

  manareq=RoundToNearest( float(manareq)*(1.0-talentbonus));
  if(g_iEcho[client]>0)
  {
    manareq=0;
    g_iEcho[client]=0;
  }
  wcSetMana(client,GetMana(client)-manareq);
  wcGetTalentInfo(6,2,3,2,client,"chance",line);
  new echochance = StringToInt(line);
  new echorandom =GetRandomInt(1,100);
  if(echochance>=echorandom) {
    g_iEcho[client]=1;
    wcRunSpell_6_6(spellid,race,client);
    WCMessage(client,"Echo of the Elements activate",client);
    }
  else
    SetCdSpell(client,cdtype,cd, spellid);

  new Float:location[3],Float:locationatt[3];
  GetClientAbsOrigin(target,location);
  GetClientAbsOrigin(client,locationatt);
  wcGetTalentInfo(6,3,2,3,client,"increment",line);
  talentbonus = StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  multi+=talentbonus;
  wcGetSpellInfo(race,spellid,"multiar",line);
  new Float:multiar=StringToFloat(line);
  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  new heal=GetSpellDmg(client,multi);
  new armor=GetSpellDmg(client,multiar);
  EmitAmbientSound("wc/spells/heal.wav",location);
  if(!HasGlyph(client, GLYPH_SHAMAN_WAVE))  {
    wcClientsBonusStats[target][ARMOR]+=armor;
    wcRecalculateStats(target);
    new Handle:pack = CreateDataPack();
    WritePackCell(pack, target);
    WritePackCell(pack, armor);
    WritePackCell(pack, g_respawns[client]);
    CreateTimer(30.0, RemoveArmorTimer, pack, TIMER_HNDL_CLOSE);
  }
  else  {
    heal = RoundToNearest(heal * 1.25);
    armor = 0;
  }
  heal=wcHeal(client,target,heal,spellid,true);
  if(target!=client)
  {
    WCMessage(target,"healing wave target",target,name,heal,armor);
  }
  WCMessage(client,"healing wave caster",client,name2,heal,armor);
  location[2]+=40;
  locationatt[2]+=40;

  wcRunTalent_6_NaturesBlessing(client,target);
  return 1;
}

public Action:RemoveArmorTimer(Handle:timer, Handle:pack) {
  ResetPack(pack);
  new client = ReadPackCell(pack);
  new armor = ReadPackCell(pack);
  new spawn = ReadPackCell(pack);
  if (spawn != g_respawns[client] || !IsClientInGame(client))
  return Plugin_Stop;
  wcClientsBonusStats[client][ARMOR]-=armor;
  wcRecalculateStats(client);

  return Plugin_Stop;

}

public wcRunSpell_6_7(spellid,race,client)
{

  //if(IsBoss())  
  //{ //Has mana cost now, it's k
  //  WCMessage(client, "boss spell blocked", client);
  //  return;
  //}

  if(!bMagicDamage[client])
  {
    new manareq = GetSpellCost(client, spellid);
    wcSetMana(client,GetMana(client)-manareq);
    new Float:pen = float(GetBonusAgi(client)+GetAgi(client)) / 11.0;
    WCMessage(client,"rockbiter cast",client,pen);
    bMagicDamage[client]=true;
  }
  else  {
    WCMessage(client, "rockbiter off", client);
    bMagicDamage[client] = false;
  }
}

public wcRunSpell_6_8(spellid,race,client)
{
  if(bArenaActive)
  return;
  decl clients[65];
  new team=GetClientTeam(client);
  new amount=0;
  if(team==2)
  amount=FindMatchingAlive("@t",clients);
  else if(team==3)
  amount = FindMatchingAlive("@ct",clients);
  new chance = wcGetSpellInfoInt(race, spellid, "chance");
  decl String:line[255];
  wcGetTalentInfo(6,3,2,2,client,"multi",line);
  chance+=StringToInt(line);

  if((amount>0) && (getOneRound(client)==0) && (!IsDmEnabled()) && (chance >= GetRandomInt(1,100)) )   {
    setOneRound(client,1);
    WCMessage(client,"reincarnation",client);
    CreateTimer(0.2,ReincEffect,client);
  }
}

public wcRunSpell_1_1(spellid,race,victim,attacker,&totaldmg) //Warrior Shield Block
{
  decl String:line[255];
  new Float:chance = GetBlockChance(victim,true,attacker);
  decl Float:clienta[3],Float:targeta[3];
  GetClientEyeAngles(attacker, clienta);
  GetClientEyeAngles(victim, targeta);
  new Float:angle=clienta[1]-targeta[1];
  if(angle<0.0)
  angle*=-1.0;
  if(angle>180.0)
  angle = 360.0-angle;

  if( (chance>=GetRandomFloat(0.0001,1.0)) && (angle > 100.0 || !IsValidShooting(attacker, victim)) )
  {
    for(new x=1;x<=50;x++)
    {
      if((spellid=wcGetSpell(victim,x))!=0)
      {
        wcGetSpellInfo(race,spellid,"event",line);
        if(StrEqual(line,"player_block",false))
        {
          new tmp=0;
          wcRunSpell(spellid,race,victim,attacker,totaldmg,tmp,tmp,tmp,tmp,tmp,tmp,victim);
        }
      }
    }
    wcRunOnBlockTalents(victim,attacker);
    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    EmitAmbientSound("wc/spells/shieldwall.wav",location);
    totaldmg=0;

    AddEMsgAff("blocked damage", true);
  }
  return totaldmg;
}

public wcRunSpell_1_7(spellid,race,&victim,&attacker,totaldmg,&any:stop) //Spell Reflection
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"chance",line);
  new chance=StringToInt(line);
  wcGetTalentInfo(1,1,2,3,attacker,"increment",line);
  new increment=StringToInt(line);
  if(chance+increment >= random)
  {
    if(HasShield(victim))
    {
      decl Float:location[3];
      GetClientAbsOrigin(victim,location);
      EmitAmbientSound("wc/spells/shieldwall.wav",location);

      if(increment>0) AddEMsgAff("improved spell reflection", true);
      else            AddEMsgAff("spell reflection", true);

      new tmp=victim;
      victim=attacker;
      attacker=tmp;
    }
  }
  stop=true;
}

public wcRunSpell_1_6(spellid,race,victim,attacker,totaldmg) //Revenge
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"base",line);
  new Float:base=StringToFloat(line);
  
  if (GetBoss(attacker)>0)
  {
    wcGetSpellInfo(race,spellid,"threat",line);
    AddThreat(victim,StringToInt(line));
  }
  
  decl String:vicMsg[300], String:attMsg[300];
  if (wcGetTalentLevel(victim,1,1,3)>0)
  {
    wcGetTalentInfo(1,1,1,3,attacker,"multi",line);
    multi *= (1.0+StringToFloat(line));
    wcGetTalentInfo(1,1,1,3,attacker,"base",line);
    base += StringToFloat(line);
    Format(vicMsg,sizeof(vicMsg),"%T","improved revenge got", attacker);
    Format(attMsg,sizeof(attMsg),"%T","improved revenge used", victim);
  }
  else
  {
    Format(vicMsg,sizeof(vicMsg),"%T","revenge got", attacker);
    Format(attMsg,sizeof(attMsg),"%T","revenge used", victim);
  }
  
  new dmg = RoundToNearest(
    base +
    ( float(GetDamage(victim)) * 0.12 +
      float(GetSta(victim)+GetBonusSta(victim)) * 0.13   ) * 
    multi);
  wcPhysicalDamage(attacker,victim,dmg,attMsg,vicMsg, true);
}

public wcRunSpell_1_8(spellid,race,victim,attacker,&totaldmg)
{
  if(!IsDoubleHand(attacker))
    return totaldmg;
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"mana",line);
  new mana = StringToInt(line);
  if(mana<=GetMana(attacker))
  {
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance=StringToInt(line);
    if(chance>=GetRandomInt(1,100))
    {
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line);
      totaldmg += RoundToNearest(float(GetDamage(attacker))*multi/GetWeaponSpeed(attacker, true));
      wcSetMana(attacker,GetMana(attacker)-mana);
      decl Float:location[3];
      GetClientAbsOrigin(attacker,location);
      EmitAmbientSound("wc/spells/warriorweaponswing.wav",location);
      wcGetSpellInfo(race,spellid,"multief",line);
      new Float:multief=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"duration",line);
      new Float:duration=StringToFloat(line);
      SetHM(victim,multief);
      SetHMT(victim,duration);

      AddEMsgAff("mortal strike ef", true);
      if(HasGlyph(attacker, GLYPH_WARRIOR_MORTAL))
      GiveDamageBuff(victim, 0.9, duration);
    }
  }
  return totaldmg;
}

public wcRunSpell_1_9(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, spellid);
  if(RemoveEffectVictim(client,10)) {
    RemoveEffectAttacker(client,15);
    RemoveEffectSpellVic(client,10);
    RemoveEffectSpellAtt(client,15);
    WCMessage(client,"def stance deac",client);
    return;
  }
  if(RemoveBerserk(client)) {
    WCMessage(client,"ber stance deac",client);
  }
  SetEffectAttacker(client,15);
  SetEffectVictim(client,10);
  SetEffectSpellVic(client,10);
  SetEffectSpellAtt(client,15);
  WCMessage(client,"def stance ac",client);
  return;
}

public bool:RemoveBerserk(client)  {
  if(RemoveEffectVictim(client,12)) {
    RemoveEffectSpellVic(client,12);
    decl String:line[255];
    wcGetSpellInfo(1,11,"crit",line);
    SetCriticalStrikeBonus(client, GetCriticalStrikeBonus(client)-StringToFloat(line));
    wcRecalculateStats(client);
    return true;
  }
  return false;
}

public wcRunSpell_1_11(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"crit",line);
  new Float:crit=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, spellid);
  if(RemoveBerserk(client)) {
    WCMessage(client,"ber stance deac",client);
    return;
  }
  if(RemoveEffectVictim(client,10)) {
    RemoveEffectAttacker(client,10);
    RemoveEffectSpellVic(client,10);
    RemoveEffectSpellAtt(client,10);
    WCMessage(client,"def stance deac",client);
  }
  SetEffectVictim(client,12);
  SetEffectSpellVic(client,12);

  SetCriticalStrikeBonus(client, GetCriticalStrikeBonus(client)+crit);
  wcRecalculateStats(client);
  WCMessage(client,"ber stance ac",client);
  return;
}

public wcRunSpell_1_12(spellid,race,client)
{
  if(!HasShield(client)) {
    WCMessage(client, "need shield", client);
    return;
  }
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"duration",line);
  new Float:dur=StringToFloat(line);
  if(HasGlyph(client, GLYPH_WARRIOR_WALL))
  dur+=1.0;
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);

  SetEffectVictim(client,13);
  SetEffectSpellVic(client,13);
  new effid=CreateEffectData(client,spellid,16,0,0,0,0,0,g_RoundCount);
  if(effid!=0)
  {
    effectsdata[effid][7]=WallEffect(client);
    CreateTimer(dur/16.0, EffectWall, effid, TIMER_REPEAT);
  }
  CreateTimer(dur,DestroyShieldWall,client);
  WCMessage(client,"shield wall ac",client, dur);
  return;
}

public wcRunSpell_1_13(spellid,race,client) //should be used as pattern for new spells when possible
{
  new target=GetAimTarget(client,20.0);
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

  if(!HasShield(client)) {
    WCMessage(client, "need shield", client);
    return;
  }

  //Spell Settings
  new cdtype = wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:cd = wcGetSpellInfoFloat(race,spellid,"cooldown");
  new Float:dur = wcGetSpellInfoFloat(race,spellid,"duration");
  new amount = wcGetSpellInfoInt(race, spellid, "amount");  //Amount of redirection hits
  new threatBonus = wcGetSpellInfoInt(race, spellid, "threat");

  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client, GetMana(client) - manareq);
  SetCdSpell(client, cdtype, cd, spellid);

  decl String:sCaster[50], String:sVictim[50];
  GetClientName(client, sCaster, 50);
  GetClientName(target, sVictim, 50);

  //Threat
  if(IsBoss() && (g_iAggro != client))  { //No effect if top threat is caster
    new threat = GetThreat(g_iAggro) + threatBonus;
    SetThreat(client, threat);
  }

  if(!IsBoss()) {

    SetRedirection(target, client, amount);
    CreateTimer(dur, RemoveRedirection, target);
  }

  WCMessage(client,"taunt ac",client, dur, sVictim);
  WCMessage(target,"taunt ac victim",target, dur, sCaster);
  new Float:location[3],Float:locationb[3];
  GetClientAbsOrigin(client,location);
  GetClientAbsOrigin(target,locationb);
  EmitAmbientSound("wc/spells/taunt.wav",location);
  EmitAmbientSound("wc/spells/taunt.wav",locationb);


}

public Action:RemoveRedirection(Handle:timer, any:client)
{
  WCMessage(client,"taunt deac",client);
  SetRedirection(client, 0, 0);
}


public Action:DestroyShieldWall(Handle:timer, any:client)
{
  WCMessage(client,"shield wall deac",client);
  RemoveEffectVictim(client,13);
  RemoveEffectSpellVic(client,13);
}

public Action:DestroyManaShield(Handle:timer, any:client)
{
  if(RemoveEffectVictim(client,11))
  {
    RemoveEffectSpellVic(client,11);
    WCMessage(client,"mana shield end",client);
  }
}

public wcRunSpell_1_10(spellid,race,client)
{
  {
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetSpellInfo(race,spellid,"cooldown",line);
    new Float:cd=StringToFloat(line);
    new manareq = GetSpellCost(client, spellid);
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client, cdtype, cd, spellid);
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    wcGetSpellInfo(race,spellid,"distance",line);
    new Float:distance=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"slowmulti",line);
    new Float:slowmulti=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"slowas",line);
    new Float:slowas=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"slowtime",line);
    new Float:slowtime=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"threat",line);
    new threat=StringToInt(line);
    new dmg=RoundToNearest(float(GetDamage(client)+GetSta(client)+GetBonusSta(client))*multi);
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
      if(GetBoss(targets[x])>0)
      AddThreat(client,threat);
    }
    if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);

    new color[4]={55,55,255,55};
    BeamRingEffect("@all",location,10.0,40.0,g_ef_lgtning,0,10,3.0,100.0,color,0.0,20);
    color[3]=255;
    BeamRingEffect("@all",location,50.0,350.0,g_ef_lgtning,0,10,3.0,50.0,color,0.0,10);
    WCMessage(client,"thunder clap caster",client,stargets,dmg);
    EmitAmbientSound("wc/spells/thunderclap.wav",location);

    for (new x=0;x<=(amount-1);x++)
    {
      new Float:oldspeed=GetClientSpeed(targets[x]);
      new Float:newspeed=GetNormalSpeed(targets[x]);
      newspeed*=slowmulti;
      new Float:newas=GetAttackSpeed(targets[x])*slowas;
      new Float:restoreAs=GetAttackSpeed(targets[x])-newas;
      wcClientsBonusStats[targets[x]][ATTACKSPEED]-=restoreAs;
      new Handle:datapack3=CreateDataPack();
      WritePackCell(datapack3,targets[x]);
      WritePackCell(datapack3,g_respawns[targets[x]]);
      WritePackFloat(datapack3,restoreAs);
      CreateTimer(slowtime,RemoveSlowAttack,datapack3,TIMER_HNDL_CLOSE);
      wcRecalculateStats(targets[x]);
      if(newspeed<oldspeed)
      {
        SetClientSpeed(targets[x],newspeed);
        new Handle:datapack2=CreateDataPack();
        WritePackCell(datapack2,targets[x]); // Write the client index of owner
        WritePackFloat(datapack2,newspeed);
        WritePackFloat(datapack2,slowmulti);
        CreateTimer(slowtime,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
      }
      decl String:vicMsg[300];
      if(CheckRequirements(client,1,1,2,4))
      {
        wcGetTalentInfo(1,1,2,4,client,"chance",line);
        new chance = StringToInt(line);
        new rand = GetRandomInt(1,100);
        if(rand <= chance)
        {
          wcGetTalentInfo(1,1,2,4,client,"duration",line);
          new Float:silence = StringToFloat(line);
          SetSilence(targets[x],silence);
          Format(vicMsg,sizeof(vicMsg),"%T","deafening thunder clap victim", targets[x], silence);
        }
        else
        {
          Format(vicMsg,sizeof(vicMsg),"%T","thunder clap victim", targets[x]);
        }

      }
      else
      {
        Format(vicMsg,sizeof(vicMsg),"%T","thunder clap victim", targets[x]);
      }
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(targets[x],client,dmg, "", vicMsg,line);
      BloodSpray(targets[x]);
    }
  }
}

public wcRunSpell_3_1(spellid,race,target, client)
{
  //new target=GetAimTarget(client,15.0);
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      decl String:line[255];
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(3,1,2,2,client,"cooldown",line);
      new Float:talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line)-talentbonus;
      new manareq=GetSpellCost(client, spellid);
      wcGetSpellInfo(race,spellid,"basedmg",line);
      new basedmg=StringToInt(line);

      wcSetMana(client,GetMana(client)-manareq);

      SetCdSpell(client, cdtype, cd, spellid);
      new Float:location[3],Float:locationatt[3];
      GetClientAbsOrigin(target,location);
      GetClientAbsOrigin(client,locationatt);
      wcGetTalentInfo(3,1,1,2,client,"duration",line);
      talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"slowtime",line);
      new Float:slowtime=StringToFloat(line)+talentbonus;
      wcGetTalentInfo(3,1,1,2,client,"increment",line);
      talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"slowmulti",line);
      new Float:slowmulti=StringToFloat(line)-talentbonus;
      wcGetTalentInfo(3,1,2,2,client,"increment",line);
      talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line)*(1+talentbonus);
      new dmg=GetSpellDmg(client,multi)+basedmg;
      EmitAmbientSound("wc/spells/frostbolt.wav",location);

      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","frostbolt target", target);
      Format(attMsg,sizeof(attMsg),"%T","frostbolt caster", client);
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(target,client,dmg, attMsg, vicMsg,line);

      new Float:oldspeed=GetClientSpeed(target);
      new Float:newspeed=GetNormalSpeed(target);
      newspeed*=slowmulti;
      if(newspeed<oldspeed)
      {
        SetClientSpeed(target,newspeed);
        new Handle:datapack2=CreateDataPack();
        WritePackCell(datapack2,target); // Write the client index of owner
        WritePackFloat(datapack2,newspeed);
        WritePackFloat(datapack2,slowmulti);
        CreateTimer(slowtime,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
      }
      location[2]+=20;
      locationatt[2]+=50;
      GlowSpriteEffect("@all",location, g_ef_glow1, 2.0, 2.0, 155);
      new color[4]={108,230,218,155};
      BeamEffect("@all",location,locationatt,g_ef_glow01,5,2.0,3.0,9.0,color,0.0,5);
      new color2[4]={175,115,255,55};
      BeamRingEffect("@all",location,60.0,59.0,g_ef_lgtning,0,1,2.0,50.0,color2,0.0,1);
    }
    else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
  WCMessage(client,"no target",client);
}

public wcRunSpell_3_2(spellid,race,client) //Frost Nova
{
  {
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(3,1,1,1,client,"cooldown",line);
    new Float:talentbonus = StringToFloat(line);
    wcGetSpellInfo(race,spellid,"cooldown",line);
    new Float:cd=StringToFloat(line)-talentbonus;
    new manareq=GetSpellCost(client, spellid);
    wcSetMana(client,GetMana(client)-manareq);

    SetCdSpell(client, cdtype, cd, spellid);
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    wcGetSpellInfo(race,spellid,"distance",line);
    new Float:distance=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"slowmulti",line);
    new Float:slowmulti=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"slowtime",line);
    new Float:slowtime=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"ticktime",line);
    new Float:ticktime=StringToFloat(line);

    wcGetSpellInfo(race,spellid,"breakdmg",line);
    new breakdmg=StringToInt(line);
    wcGetTalentInfo(3,1,1,1,client,"multi",line);
    new Float:talentmulti = StringToFloat(line);
    breakdmg += RoundToNearest(breakdmg*talentmulti);

    new targets[MAXPLAYERS];
    new team=GetClientTeam(client);
    new amount;
    if(team==2)
    amount=GetNearClients("@ct",location,targets,distance);
    if(team==3)
    amount=GetNearClients("@t",location,targets,distance);
    decl String:name2[255];
    new String:stargets[255];
    new dmg=GetSpellDmg(client,multi);
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
    WCMessage(client,"frostnova",client,stargets,dmg);
    EmitAmbientSound("wc/spells/frostbolt.wav",location);

    new Handle:dp=CreateDataPack();
    WritePackCell (dp,client); // Write the client index of victim
    WritePackCell (dp,10);
    WritePackFloat(dp,location[0]);
    WritePackFloat(dp,location[1]);
    WritePackFloat(dp,location[2]);

    CreateTimer(0.1, FrostNovaVisualEffect, dp, TIMER_REPEAT);
    for (new x=0;x<=(amount-1);x++)
    {
      decl String:vicMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","frostnova target", targets[x]);
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(targets[x],client,dmg, "", vicMsg,line);
      BloodSpray(targets[x]);

      new Float:oldspeed=GetClientSpeed(targets[x]);
      new Float:newspeed=GetNormalSpeed(targets[x]);
      newspeed*=slowmulti;
      if(newspeed<oldspeed)
      {
        new effid2=CreateEffectData(targets[x],RoundToNearest(slowtime/ticktime),0,0,0,0,0,0,RoundToNearest(oldspeed*100));
        SetClientSpeed(targets[x],newspeed);
        SetFrostNovaFreezeDmg(targets[x],breakdmg);
        SetFrostNovaFreeze(targets[x],effid2);
        CreateTimer(ticktime,FrostNovaFreezeEffect,effid2, TIMER_REPEAT);
      }
    }
  }
}

public Action:FrostNovaFreezeEffect(Handle:timer,any:effid)
{
  if (effectsdata[effid][0]==0) //effect data gone
  return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1])) //victim not in game
  {
    RemoveFrostNovaFreeze(effectsdata[effid][1]);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1])) //victim dead
  {
    RemoveFrostNovaFreeze(effectsdata[effid][1]);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(GetFrostNovaFreezeDmg(effectsdata[effid][1]) <= 0 || //no breakdmg remaining
      effectsdata[effid][2]                        <= 0 )  //no ticks left
  {
    new Handle:datapack2=CreateDataPack();
    WritePackCell (datapack2,effectsdata[effid][1]); // Write the client index of victim
    WritePackFloat(datapack2,0.0);
    WritePackFloat(datapack2,float(effectsdata[effid][9]/100));
    CreateTimer(0.0,RemoveFreezeEffect,datapack2,TIMER_HNDL_CLOSE);
    RemoveFrostNovaFreeze(effectsdata[effid][1]);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][2]--;
  return Plugin_Continue;
}

//zero or id of event
new FrostNovaFreeze[65];
//break damage remaining
new maxFrostNovaFreeze[65];

public SetFrostNovaFreeze(client,effid)
{
  FrostNovaFreeze[client]=effid;
}
public GetFrostNovaFreeze(client)
{
  return FrostNovaFreeze[client];
}
public RemoveFrostNovaFreeze(client)
{
  SetFrostNovaFreeze(client,0);
  SetFrostNovaFreezeDmg(client,0);
}
SetFrostNovaFreezeDmg(client,breakdmg)
{
  maxFrostNovaFreeze[client]=breakdmg;
}
GetFrostNovaFreezeDmg(client)
{
  return maxFrostNovaFreeze[client];
}

public Action:FrostNovaVisualEffect(Handle:timer, any:dp)
{
  ResetPack(dp);
  new caster = ReadPackCell(dp);
  if(caster <= 0 || caster > GetMaxClients()) {
    LogError("[wc] Error: FrostNovaVisualEffect called with caster = %d", caster);
    return Plugin_Stop;
  }
  if (!IsClientInGame(caster)) //caster not in game
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(caster)) //caster dead
  {
    return Plugin_Stop;
  }
  new ticks = ReadPackCell(dp);
  if (ticks<=0) //no ticks
  {
    return Plugin_Stop;
  }
  decl Float:location[3];
  for (new i=0;i<3;i++)
  {
    location[i]= ReadPackFloat(dp);
  }
  new color[4]={75,75,255,255};
  if(!BeamRingEffect("@all",location,10.0,500.0,g_ef_lgtning,0,10,2.0,100.0,color,0.0,10))
  LogError("didn't create effect #935");

  ResetPack(dp,true);
  WritePackCell (dp,caster);
  WritePackCell (dp,ticks-1);
  WritePackFloat(dp,location[0]);
  WritePackFloat(dp,location[1]);
  WritePackFloat(dp,location[2]);
  return Plugin_Continue;
}

public wcRunSpell_3_3(spellid,race, target,client)
{
  //new target=GetAimTarget(client,15.0);
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      decl String:line[255];
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(3,2,2,3,client,"cooldown",line);
      new Float:talentbonus = StringToFloat(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line)-talentbonus;
      new manareq=GetSpellCost(client, spellid);
      wcGetSpellInfo(race,spellid,"basedmg",line);
      new basedmg=StringToInt(line);

      wcSetMana(client,GetMana(client)-manareq);
      SetCdSpell(client, cdtype, cd, spellid);
      new Float:location[3],Float:locationatt[3];
      GetClientAbsOrigin(target,location);
      GetClientAbsOrigin(client,locationatt);
      wcGetTalentInfo(3,2,2,3,client,"increment",line);
      talentbonus = StringToFloat(line);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line)*(1+talentbonus);
      decl String:name[255],String:name2[255];
      GetClientName(client,name,sizeof(name));
      GetClientName(target,name2,sizeof(name2));
      new dmg=GetSpellDmg(client,multi)+basedmg;
      EmitAmbientSound("wc/spells/fireball.wav",location);

      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","fireball target", target);
      Format(attMsg,sizeof(attMsg),"%T","fireball caster", client);
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(target,client,dmg, attMsg, vicMsg,line);

      location[2]+=40;
      locationatt[2]+=40;
      GlowSpriteEffect("@all",location, g_ef_old_xfire, 3.0, 1.2, 255);
      new color[4]={255,101,67,255};
      BeamEffect("@all",locationatt,location,g_ef_tp_beam001,5,3.0,7.0,10.0,color,0.0,5);
    }
    else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
  WCMessage(client,"no target",client);
}

public Action:ReincEffect(Handle:timer, any:client)
{
  //radius 500
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  decl clients[65];
  new team=GetClientTeam(client);
  new amount=0;
  if(team==2)
  amount=FindMatchingAlive("@t",clients);
  else if(team==3)
  amount = FindMatchingAlive("@ct",clients);
  if(amount>0) {
    CS_RespawnPlayer(client);
    CreateTimer(0.3,ReincEffect2,client);
  }
  return Plugin_Stop;
}

public Action:ReincEffect2(Handle:timer, any:client)
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
  GetClientAbsOrigin(client,location);
  new color[4]={255,153,0,255};
  new hp = GetMaxHealth(client);
  hp = RoundToNearest(float(hp) * 1.0);
  new mn = GetMaxMana(client);
  mn = RoundToNearest(float(mn) * (0.5));
  wcSetHealth(client,hp);
  wcSetMana(client,mn);
  SetEntProp(client, Prop_Send, "m_ArmorValue", 100, 1);
  SetEntProp(client, Prop_Send, "m_bHasHelmet", 1, 1);
  if(!BeamRingEffect("@all",location,10.0,500.0,g_ef_lgtning,0,10,2.0,100.0,color,0.0,10))
  LogError("didn't create effect #1002");
  return Plugin_Stop;
}

public Action:ResurrectionEffect(Handle:timer, any:client)
{
  //radius 500
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
  new color[4]={50,255,50,255};
  TeleportEntity(client,location,NULL_VECTOR,NULL_VECTOR);
  new hp = GetMaxHealth(client);
  hp = RoundToNearest(float(hp) * 0.5);
  new mn = GetMaxMana(client);
  mn = RoundToNearest(float(mn) * 0.5);
  wcSetHealth(client,hp);
  wcSetMana(client,mn);
  SetEntProp(client, Prop_Send, "m_ArmorValue", 100, 1);
  SetEntProp(client, Prop_Send, "m_bHasHelmet", 1, 1);
  if(!BeamRingEffect("@all",location,10.0,500.0,g_ef_lgtning,0,10,2.0,100.0,color,0.0,10))
  LogError("didn't create effect #1002");
  return Plugin_Stop;
}

public Action:ArcaneEffect(Handle:timer, any:client)
{
  //radius 500
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  decl Float:location[3];
  new color[4]={204,204,204,210};
  GetClientAbsOrigin(client,location);
  if(!BeamRingEffect("@all",location,199.0,199.0,g_ef_lgtning,0,20,0.5,199.0,color,0.5,15))
  LogError("didn't create effect #963");
  return Plugin_Stop;
}

public wcRunSpell_3_4(spellid,race,victim,attacker)
{
  if(IsPlayerAlive(victim))
  {
    new random=GetRandomInt(1,100);
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance=StringToInt(line);
    if(chance>=random)
    {
      new manareq=GetSpellCost(attacker, spellid);
      new newmana=GetMana(attacker)-manareq;
      if(newmana<0)
      {
        return false;
      }
      wcSetMana(attacker,newmana);
      wcGetTalentInfo(3,2,3,2,attacker,"increment",line);
      new Float:talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line);
      new dmg=GetSpellDmg(attacker,multi);
      dmg+=RoundToNearest(dmg*talentbonus);
      decl Float:location[3];
      GetClientAbsOrigin(victim,location);
      EmitAmbientSound("wc/spells/fireball.wav",location);
      decl String:attMsg[300],String:vicMsg[300];
      Format(attMsg,sizeof(attMsg),"%T","scorch cast att", attacker);
      Format(vicMsg,sizeof(vicMsg),"%T","scorch cast vic", attacker);
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(victim,attacker,dmg, attMsg, vicMsg,line);
      new color[4]={255,35,0,255};
      new color2[4]={215,35,0,255};
      BeamRingEffect("@all",location,60.0,120.0,g_ef_lgtning,0,10,3.0,35.0,color,5.0,10);
      location[2]+=40.0;
      BeamRingEffect("@all",location,2.0,77.0,g_ef_lgtning,0,10,3.0,30.0,color2,0.0,10);
      if(CheckRequirements(attacker,3,1,2,4))
      wcRunTalent_3_Frostburn(attacker,victim);
      if(CheckRequirements(attacker,3,2,1,1))
      wcRunTalent_3_SearedFlesh(attacker,victim);
      return true;
    }
  }
  return false;
}

public wcRunSpell_3_5(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(3,3,2,2,client,"blinkcd",line);
  new Float:talentbonus = StringToFloat(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line)-talentbonus;
  new manareq=GetSpellCost(client, spellid);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  location[2]+=32;//so that blink sprites shoot out their butt instead of feet.
  wcGetTalentInfo(3,2,3,3,client,"powermultiplier",line);
  new Float:powermultiplier=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"power",line);
  new Float:power=StringToFloat(line);
  new blinkw=0;
  if(powermultiplier>1.0)
  {
    power *=powermultiplier;
    blinkw=1;
  }
  wcGetSpellInfo(race,spellid,"silence",line);
  new Float:silence=StringToFloat(line);
  new color[4]={255,25,255,255};
  new Float:endpos[3];
  if(TeleportPlayerView(client,power,endpos))
  {
    SetSilence(client,silence);
    wcSetMana(client,GetMana(client)-manareq);

    SetCdSpell(client, cdtype, cd, spellid);
    if( (GetCDSpell(client, 4) - GetTickedTime() ) < 3.0)
    SetCdSpell(client, 4, 3.0, 2);  //Cd for frost nova
    WCMessage(client,"blink cast",client);

    if (blinkw==1)
    {
      wcGetTalentInfo(3,2,3,3,client,"BWradius",line);
      new Float:BWradius=StringToFloat(line);
      wcGetTalentInfo(3,2,3,3,client,"BWmulti",line);
      new Float:BWmulti=StringToFloat(line);
      wcGetTalentInfo(3,2,3,3,client,"BWbasedmg",line);
      new BWbasedmg=StringToInt(line);

      new targets[MAXPLAYERS];
      new team=GetClientTeam(client);
      new amount;
      if(team==2)
      amount=GetNearClients("@ct",endpos,targets,BWradius);
      if(team==3)
      amount=GetNearClients("@t",endpos,targets,BWradius);
      decl String:name2[255];
      new String:stargets[255];
      new dmg=GetSpellDmg(client,BWmulti)+BWbasedmg;
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
      WCMessage(client,"blinkwake",client,stargets,dmg);
      EmitAmbientSound("wc/spells/fireball.wav",location);
      EmitAmbientSound("wc/spells/fireball.wav",endpos);
      CreateTimer(0.0,BlinkwakeEffect,client);
      CreateTimer(0.1,BlinkwakeEffect,client);

      color={255,69,0,255};  //red orange
      BeamRingEffect("@all",location,10.0,BWradius,g_ef_lgtning,0,10,2.0,100.0,color,0.0,10);
      BeamEffect("@all",location,endpos,g_ef_lgtning,5, 1.0, 1.5, 0.5, color, 0.0, 5);
      for (new y=0;y<=(amount-1);y++)
      {
        decl String:vicMsg[300];
        wcGetSpellInfo(race,spellid,"name",line);
        Format(vicMsg,sizeof(vicMsg),"%T","blinkwake target", targets[y]);
        wcSpellDamage(targets[y],client,dmg, "", vicMsg,line);
        BloodSpray(targets[y]);
      }

    }
    else
    {
      EmitAmbientSound("wc/spells/blink.wav",location);
      EmitAmbientSound("wc/spells/blink.wav",endpos);
      BeamRingEffect("@all",location,12.0,17.0,g_ef_lgtning,0,10,1.0,35.0,color,0.3,10);
      BeamEffect("@all",location,endpos,g_ef_lgtning,5,1.0,0.5,0.5,color,0.0,5);

    }
    //derp, THATS why it was needed.... note "endpos" ...
    BeamRingEffect("@all",endpos,12.0,17.0,g_ef_lgtning,0,10,1.0,35.0,color,0.3,10);
  }
  else
  {
    WCMessage(client,"blink failed",client);
  }
}

public Action:BlinkwakeEffect(Handle:timer, any:client)
{
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  decl String:line[255];
  wcGetTalentInfo(3,2,3,3,client,"ERradius",line);
  new Float:ERradius=StringToFloat(line);

  decl Float:location[3];
  new color[4]={255,69,0,255};  //red orange
  GetClientAbsOrigin(client,location);
  if(!BeamRingEffect("@all",location,10.0,ERradius,g_ef_lgtning,0,10,2.0,100.0,color,0.0,10))
  LogError("didn't create effect #935 Blinkwake");
  return Plugin_Stop;
}

public wcRunSpell_3_6(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  new bonus=RoundToNearest(float(GetLevel(client))*multi);
  if(HasGlyph(client, GLYPH_MAGE_ARCANEINT))
  bonus=RoundToNearest(bonus*0.5);
  WCMessage(client,"intellect cast",client,bonus);
  SetBonusInt(client,GetBonusInt(client)+bonus);
  wcRecalculateStats(client);
  if(HasGlyph(client, GLYPH_MAGE_ARCANEINT))  {
    if(!IsClientInGame(client))
    return;
    WCMessage(client,"arcaneint cast all",client);
    new clients[MAXPLAYERS];
    new team=GetClientTeam(client);
    new amount;
    if(team==2)
    amount=FindMatchingPlayers("@t",clients);
    if(team==3)
    amount=FindMatchingPlayers("@ct",clients);
    for(new i=0;i<amount;i++)  {
      if(IsClientInGame(clients[i]))
      CastArcane(clients[i],multi,true);
    }
  }
}

stock bool:CastArcane(client, Float:multi, bool:cast=false, bool:custom=false)  {
  if((!custom) && ArcaneClient[client])
  return false;
  SetBonusInt(client,GetBonusInt(client)+RoundToNearest((float(GetBonusInt(client)+GetInt(client))*multi)*0.50));
  wcRecalculateStats(client);
  if(!custom)
  ArcaneClient[client]=true;
  if(cast) {
    WCMessage(client,"arcaneint aff",client);
  }
  return true;
}

public wcRunSpell_3_7(spellid,race,client)
{
  {
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(3,3,2,2,client,"aecd",line);
    new Float:talentbonus=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"cooldown",line);
    new Float:cd=StringToFloat(line)-talentbonus;
    new manareq=GetSpellCost(client, spellid);
    wcSetMana(client,GetMana(client)-manareq);

    SetCdSpell(client, cdtype, cd, spellid);
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    wcGetSpellInfo(race,spellid,"distance",line);
    new Float:distance=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"basedmg",line);
    new basedmg=StringToInt(line);
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
    new dmg=GetSpellDmg(client,multi)+basedmg;
    for (new x=0;x<=(amount-1);x++)
    {
      GetClientName(targets[x],name2,sizeof(name2));
      if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
      //WCMessage(targets[x],"arcexpl target",targets[x],name,dmg);
    }
    if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);
    WCMessage(client,"arcexpl",client,stargets,dmg);
    EmitAmbientSound("wc/spells/arcexpl.wav",location);
    CreateTimer(0.0,ArcaneEffect,client);
    CreateTimer(0.1,ArcaneEffect,client);
    wcGetSpellInfo(race,spellid,"name",line);
    for (new x=0;x<=(amount-1);x++)
    {
      decl String:vicMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","arcexpl target", targets[x]);
      wcSpellDamage(targets[x],client,dmg, "", vicMsg,line);
      BloodSpray(targets[x]);
    }
  }
}

public wcRunSpell_3_8(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq=GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);

  SetCdSpell(client, cdtype, cd, spellid);
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/seal.wav",location);
  bCombustion[client]=true;
  WCMessage(client,"combustion cast",client);
  //making inform when expired

}

new bool:g_ff_manashield[65];
public wcRunSpell_3_9(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcGetSpellInfo(race,spellid,"absorb",line);
  new absorb=StringToInt(line);
  wcSetMana(client,GetMana(client)-manareq);

  SetCdSpell(client, cdtype, cd, spellid);
  new bool:removed=false;
  while(RemoveEffectVictim(client,11)) {
    removed=true;
    RemoveEffectSpellVic(client,11);

  }
  if(wcManaShield[client] < absorb)
  removed=false;
  if(removed)  {
    g_ff_manashield[client]=false;
    WCMessage(client,"mana shield deac",client);
    g_iShieldStack[client]-=1;
    return;
  }
  g_ff_manashield[client]=true;
  g_iShieldStack[client]+=1;
  SetEffectVictim(client,11);
  SetEffectSpellVic(client,11);
  //CreateTimer(duration,DestroyManaShield,client);
  wcManaShield[client]=absorb;
  WCMessage(client,"mana shield ac",client,absorb);
  return;
}


stock bool:IsManaShieldOn(client) {
  return (GetClass(client) == 3) && g_ff_manashield[client] && (wcManaShield[client] > 0);
}

public wcRunSpell_3_10(spellid,race, client)
{
  new target=GetAimTarget(client,15.0);
  if(!( (IsBoss() || IsHNS()) && (GetBoss(target) > 0) && (UnableToAttack[target]) ) )
  {
    if(target>0)
    {
      if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
      {
        decl String:line[255];
        wcGetSpellInfo(race,spellid,"cooldowntype",line);
        new cdtype=StringToInt(line);
        wcGetSpellInfo(race,spellid,"cooldown",line);
        new Float:cd=StringToFloat(line);
        new manareq=GetSpellCost(client, spellid);
        wcGetSpellInfo(race,spellid,"duration",line);
        new Float:duration=StringToFloat(line);

        wcSetMana(client,GetMana(client)-manareq);

        SetCdSpell(client, cdtype, cd, spellid);
        new Float:location[3];
        GetClientAbsOrigin(target,location);
        decl String:name[255],String:name2[255];
        GetClientName(client,name,sizeof(name));
        GetClientName(target,name2,sizeof(name2));

        EmitAmbientSound("wc/spells/sheep.wav",location);

        WCMessage(target, "polymorph target", target, duration, name);
        WCMessage(client, "polymorph caster", client, duration, name2);

        setPolymorph(target, duration);

      }
      else  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
    }
    else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
  }
  else
  WCMessage(client,"boss spell blocked",client);
}



public wcRunSpell_2_1(spellid,race,client) //Evade
{
  //setting up cooldown and -mana
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  wcGetTalentInfo(2,2,1,3,client,"duration",line);
  new Float:talentbonus = StringToFloat(line);
  wcGetSpellInfo(race,spellid,"duration",line);
  new Float:duration=StringToFloat(line)+talentbonus;
  // setting up the evade effect
  SetEffectVictim(client,3); // evade effect
  new Handle:datapack2=CreateDataPack();
  WritePackCell(datapack2,client); // Write the client index of owner
  WritePackCell(datapack2,3);
  CreateTimer(duration,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
  WCMessage(client,"evade activated",client,duration);
}

public wcRunSpell_2_2(spellid,race,victim,attacker,&totaldmg)
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"mana",line);
  new mana = StringToInt(line);
  if(HasGlyph(attacker, GLYPH_ROGUE_SINISTER))
  mana-=2;
  if(mana<=GetMana(attacker))
  {
    wcGetSpellInfo(race,spellid,"chance",line);
    new chance=StringToInt(line);
    new weap = GetEntDataEnt2(attacker, ACTIVEWEAPON);
    if(weap>=0)
    {
      decl String:S_weapon[50];
      GetEdictClassname(weap, S_weapon, sizeof(S_weapon));
      if(StrEqual("weapon_knife",S_weapon))
      chance*=2;
    }
    chance = RoundToCeil(float(chance)/GetWeaponSpeed(attacker, false));
    wcGetTalentInfo(2,2,1,2,attacker,"chance",line);
    chance+=StringToInt(line);
    if(chance>=random)
    {
      wcGetSpellInfo(race,spellid,"agimulti",line);
      new Float:agimulti=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"apmulti",line);
      new Float:apmulti=StringToFloat(line);
      new Float:talentbonus = 1.00;
      wcGetTalentInfo(2,2,2,3,attacker,"increment",line);
      talentbonus+=StringToFloat(line);
      apmulti*=talentbonus;
      talentbonus = 1.00;
      wcGetTalentInfo(2,1,2,4,attacker,"increment",line);
      talentbonus+=StringToFloat(line);
      agimulti*=talentbonus;
      new Float:agility=float(GetAgi(attacker)+GetBonusAgi(attacker));
      new Float:attackpower=float(GetDamage(attacker)+GetBonusDamage(attacker));
      new dmg=RoundToNearest(agility*agimulti+attackpower*apmulti);
      //new dmg=RoundToNearest(float(totaldmg+GetIgnoreArmor(attacker)+GetBonusIgnoreArmor(attacker))*multi);
      totaldmg+=dmg;
      wcSetMana(attacker,GetMana(attacker)-mana);
      decl Float:location[3];
      GetClientAbsOrigin(attacker,location);
      EmitAmbientSound("wc/spells/strike.wav",location);

      AddEMsgAff("sinister strike", true);
    }
  }
}

new g_ffVanishId[65];

public wcRunSpell_2_3(spellid,race,client)
{
  //setting up cooldown and -mana
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  // setting up the visual effect
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  location[2]+=15;
  SmokeEffect("@all", location, g_ef_smoke, 100.0, 10);
  //es est_effect 7 #a 0 sprites/smoke.vmt server_var(wcs_x1) server_var(wcs_y1) server_var(wcs_z1) 100 10
  // invisibility
  wcGetSpellInfo(race,spellid,"alpha",line);
  new alpha=StringToInt(line);
  wcGetSpellInfo(race,spellid,"duration",line);
  new Float:duration=StringToFloat(line);
  decl color[4];
  for (new x=0;x<=2;x++)
  color[x]=255;
  color[3]=alpha;
  SetClientColor(client,color);

  //threat
  wcGetSpellInfo(race,spellid,"threat",line);
  AddThreat(client,StringToInt(line));
  //setting up the anti attack
  bVanish[client]=true;
  AntiDefuse(client);
  // removing anti attack
  new Handle:pack = CreateDataPack();
  WritePackCell(pack,client);
  WritePackCell(pack,++g_ffVanishId[client]);
  CreateTimer(duration,DestroyVanish,pack);
  // sound + message

  EmitAmbientSound("wc/spells/vanish.wav",location);

  WCMessage(client,"vanish cast",client,duration);

  wcGetSpellInfo(race,spellid,"breakdmg",line);
  new maxdmg = StringToInt(line);
  SetVanishBreakDmg(client,maxdmg);
}

public Action:DestroyShield(Handle:timer, any:client)
{
  WCMessage(client,"shield expired",client);
  UnableToAttack[client]=false;
  UnableToVictim[client]=false;
}

public Action:DestroyVanish(Handle:timer, Handle:pack)
{
  ResetPack(pack);
  new client = ReadPackCell(pack);
  new vanishId = ReadPackCell(pack);
  if(vanishId != g_ffVanishId[client])  return Plugin_Stop;
  if(bVanish[client]) {
    SetStealth(client, -30.0);
    WCMessage(client,"vanish expired",client);

  }
  bVanish[client]=false;
  return Plugin_Stop;
}

public wcRunSpell_2_4(spellid,race,victim,attacker,&totaldmg)
{
  decl Float:clienta[3],Float:targeta[3];
  GetClientEyeAngles(attacker, clienta);
  GetClientEyeAngles(victim, targeta);
  decl String:line[255];
  new Float:angle=clienta[1]-targeta[1];
  if(angle<0.0)
  angle*=-1.0;
  if(angle>180.0)
  angle = 360.0-angle;

  if(angle<=90.0 && IsValidShooting(attacker, victim))
  {
    wcGetSpellInfo(race,spellid,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetTalentInfo(2,3,1,3,attacker,"increment",line);
    new Float:talentbonus=StringToFloat(line);
    new Float:rem=0.0;
    if(CheckRequirements(attacker,2,3,2,3)) {
      wcGetTalentInfo(2,3,2,3,attacker,"multi",line);
      new armor=GetArmor(victim)-GetIgnoreArmor(attacker);
      rem=GetDmgRed(armor, victim);
      new Float:rem2 = GetDmgRed(armor -
      RoundToNearest(float(GetArmor(victim)) * StringToFloat(line)) - GetIgnoreArmor(attacker)
      , victim) ;
      rem = rem-rem2;
    }
    new dmg=RoundToNearest(float(totaldmg)*(1.0+multi+talentbonus + rem)* MaxFloat(GetWeaponSpeed(attacker, false), 1.0))-totaldmg;


    totaldmg+=dmg;

    AddEMsgAff("backstab used", true);
  }
}


public wcRunSpell_2_5(spellid,race,client)
{
  SetEffectAttacker(client,4); // deadly poison effect
  //WCMessage(client,"deadly poison activated",client);
}

public wcRunSpell_1_14(spellid,race,client)
{
  SetEffectAttacker(client,40); // Rend
}

public wcRunSpell_2_6(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:penetration = GetIgnoreArmor(client)*StringToFloat(line);
  SetBonusIgnoreArmor(client,GetBonusIgnoreArmor(client)+penetration);
  wcRecalculateStats(client);
}

public wcRunSpell_2_7(spellid,race,client)
{
  //setting up cooldown and -mana
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  // setting up the visual effect (it's in footsteps event)
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/sprint.wav",location);
  new Float:oldspeed=GetClientSpeed(client);
  new Float:newspeed=GetNormalSpeed(client);
  if(HasGlyph(client, GLYPH_ROGUE_SPRINT))
  {
    newspeed*=(multi*0.94);
    new Float:aspmulti=0.072;
    new Float:oldaspeed=(GetAttackSpeed(client));
    new Float:newaspeed=(oldaspeed*aspmulti);
    wcGetTalentInfo(2,2,1,3,client,"duration",line);
    new Float:talentbonus = StringToFloat(line);
    wcGetSpellInfo(race,spellid,"duration",line);
    new Float:time=StringToFloat(line)+talentbonus;
    new Handle:datapack4=CreateDataPack();
    WritePackCell(datapack4,client);
    WritePackCell(datapack4,g_respawns[client]);
    WritePackFloat(datapack4,newaspeed);
    CreateTimer(time,RemoveAttackSpeed,datapack4,TIMER_HNDL_CLOSE);
    wcClientsBonusStats[client][ATTACKSPEED]=newaspeed;
    wcRecalculateStats(client);
    WCMessage(client,"sprint activated",client,(newspeed-oldspeed)*100,newspeed*100);
    WCMessage(client,"sprint alacrity",client,(newaspeed-oldaspeed)*100,newspeed*100);
  }
  else
  {
    newspeed*=multi;
    WCMessage(client,"sprint activated",client,(newspeed-oldspeed)*100,newspeed*100);
  }
  if(newspeed>oldspeed)
  {
    wcClientSparks[client]=true;
    wcGetTalentInfo(2,2,1,3,client,"duration",line);
    new Float:talentbonus = StringToFloat(line);
    wcGetSpellInfo(race,spellid,"duration",line);
    new Float:time=StringToFloat(line)+talentbonus;
    SetClientSpeed(client,newspeed);
    new Handle:datapack3=CreateDataPack();
    WritePackCell(datapack3,client); // Write the client index of owner
    CreateTimer(time,RemoveSparksEffect,datapack3,TIMER_HNDL_CLOSE);
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,client); // Write the client index of owner
    WritePackFloat(datapack,newspeed);
    WritePackFloat(datapack,multi);
    CreateTimer(time,RemoveSpeedEffect2,datapack,TIMER_HNDL_CLOSE);

  }
}

RemoveSprint(client) {
  decl String:line[255];
  wcGetTalentInfo(2,2,1,3,client,"break",line);
  if(StringToInt(line)==1)
  {

  }
  else
  {
    if(wcClientSparks[client]==true)  {
      wcClientSparks[client]=false;
      new Float:newspeed=GetNormalSpeed(client);
      new Float:oldspeed=GetClientSpeed(client);
      if(newspeed < oldspeed) {
        SetClientSpeed(client,newspeed);
        WCMessage(client,"wc sprint is broken",client);
      }
    }
  }
}

wcRunSpell_2_8(spellid,race,victim,&totaldmg)
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"chance",line);
  new chance=StringToInt(line);
  if(chance>=random)
  {
    new hp=wcGetHealth(victim);
    if(hp<=totaldmg)
    {
      decl Float:location[3];
      GetClientAbsOrigin(victim,location);
      EmitAmbientSound("wc/spells/cheatdeath.wav",location);
      AddEMsgAff("cheated damage", true);
      totaldmg=0;
      new Float:percent = 0.00;
      wcGetTalentInfo(2,1,2,2,victim,"percent",line);
      percent = StringToFloat(line);
      new heal=RoundToNearest(GetMaxHealth(victim)*percent);
      wcHeal(victim,victim,heal,0,false);
    }
  }
}

wcRunSpell_2_9(spellid,race,client)
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"chance",line);
  new chance=StringToInt(line);
  if(chance>=random)
  {
    new id=GetPlayerID(client);
    wcGetSpellInfo(race,spellid,"multi",line);
    new Float:multi=StringToFloat(line);
    new Float:bonus=GetResistance(client)*multi;
    wcClientsBonusStats[id][RESISTANCE]+=bonus;
    wcRecalculateStats(client);
    WCMessage(client,"cloak of shadow activated",client,bonus,wcClientsStats[id][RESISTANCE]);
  }
}

wcRunSpell_2_10(spellid, race, client) {
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  new Float:location[3];
  GetClientAbsOrigin(client,location);

  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"duration",line);
  new Float:duration=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"threat",line);
  new threat=StringToInt(line);
  WCMessage(client, "slice and dice cast", client,RoundToNearest(multi*100.0), duration);
  if(threat>0)
  AddThreat(client,threat);
  new color[4]={255,255,255,150};
  BeamRingEffect("@all",location,10.0,20.0,g_ef_lgtning,0,10,3.0,150.0,color,0.0,20);
  EmitAmbientSound("wc/spells/sprint.wav",location);

  new Float:oldspeed=GetBonusAccuracy(client);
  SetBonusAccuracy(client,oldspeed+multi);

  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,client);
  WritePackCell(datapack3,g_RoundCount);
  WritePackFloat(datapack3,oldspeed);
  CreateTimer(duration,RemoveAccuracy,datapack3,TIMER_HNDL_CLOSE);
  wcRecalculateStats(client);
}

public wcRunSpell_2_11(spellid,race,client)
{
  SetEffectVictim(client,14); // unleash stealth
  //WCMessage(client,"stealth activated",client);
}

// Priest spells 5_

public wcRunSpell_5_1(spellid,race,client) //Flash Heal
{
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
  new bool:glyph = HasGlyph(client, GLYPH_PRIEST_HEALS);
  decl String:line[255];
  new Float:talentbonus=0.0;
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  if(glyph)
  cd *= 0.7;
  new manareq = GetSpellCost(client, spellid);
  manareq = RoundToNearest(0.7 * manareq);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  new Float:location[3],Float:locationatt[3];
  GetClientAbsOrigin(target,location);
  GetClientAbsOrigin(client,locationatt);
  if(race==4)
  {
    wcGetTalentInfo(4,1,3,3,client,"fhincrement",line);
    talentbonus=StringToFloat(line);
  }
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line)+talentbonus;
  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  new heal=GetSpellDmg(client,multi);
  wcGetSpellInfo(race,spellid,"base",line);
  new base=StringToInt(line);
  heal+=base;
  if(glyph)
  heal = RoundToNearest(0.7 * heal);
  EmitAmbientSound("wc/spells/heal.wav",location);
  heal=wcHeal(client,target,heal,spellid,true);
  if(target!=client)
  {
    WCMessage(target,"flash heal target",target,name,heal);
  }
  WCMessage(client,"flash heal caster",client,name2,heal);
  location[2]+=40;
  locationatt[2]+=40;
  return 1;
}

public wcRunSpell_5_2(spellid,race,client) //Prayer of Healing
{
  {
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetSpellInfo(race,spellid,"cooldown",line);
    new Float:cd=StringToFloat(line);
    new manareq = GetSpellCost(client, spellid);
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client, cdtype, cd, spellid);
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    wcGetTalentInfo(5,2,2,2,client,"multi",line);
    new Float:talentbonus=1+StringToFloat(line);
    wcGetSpellInfo(race,spellid,"distance",line);
    new Float:distance=StringToFloat(line)*talentbonus;
    wcGetSpellInfo(race,spellid,"multi",line);
    new Float:multi=StringToFloat(line);
    new targets[MAXPLAYERS];
    new team=GetClientTeam(client);
    new amount;
    if(team==3)
    amount=GetNearClients("@ct",location,targets,distance);
    if(team==2)
    amount=GetNearClients("@t",location,targets,distance);
    decl String:name[255],String:name2[255];
    GetClientName(client,name,sizeof(name));
    new String:stargets[255];
    new heal=GetSpellDmg(client,multi);
    wcGetSpellInfo(race,spellid,"base",line);
    new base=StringToInt(line);
    heal+=base;
    for (new x=0;x<=(amount-1);x++)
    {
      GetClientName(targets[x],name2,sizeof(name2));
      if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
      new healed=wcHeal(client,targets[x],heal,spellid,true);
      if(targets[x]!=client)
      {
        WCMessage(targets[x],"prayer of healing target",targets[x],name,healed);
      }
    }
    if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);
    new color[4]={250,250,17,255};
    location[2]+=50;
    if(!BeamRingEffect("@all",location,150.0,90.0,g_purplelaser1,0,10,3.0,80.0,color,0.0,3))
    LogError("[WC ERROR]: didn't create effect #1341");
    WCMessage(client,"prayer of healing caster",client,stargets,heal);
    EmitAmbientSound("wc/spells/heal.wav",location);
  }
}

public wcRunSpell_5_3(spellid,race,client) //Greater Heal
{
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
  {
    WCMessage(client,"target full hp",client);
    return 0;
  }
  //
  new bool:glyph = HasGlyph(client, GLYPH_PRIEST_HEALS);
  decl String:line[255];
  new Float:talentbonus=0.0;
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  if(glyph) cd *= 0.7;
  new manareq = GetSpellCost(client, spellid);
  if(glyph)
  manareq = RoundToNearest(0.7*manareq);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  new Float:location[3],Float:locationatt[3];
  GetClientAbsOrigin(target,location);
  GetClientAbsOrigin(client,locationatt);
  if(race==4)
  {
    wcGetTalentInfo(4,1,3,3,client,"ghincrement",line);
    talentbonus=StringToFloat(line);
  }
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line)+talentbonus;
  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  new heal=GetSpellDmg(client,multi);
  wcGetSpellInfo(race,spellid,"base",line);
  new base=StringToInt(line);
  heal+=base;
  if(glyph)
  heal = RoundToNearest(0.7 * heal);
  new maxHeal = heal;
  EmitAmbientSound("wc/spells/heal.wav",location);
  heal=wcHeal(client,target,heal,spellid,true);
  if(race == 5 && wcSearchSpell(client,10)>0) {
    //Inner fire bonuses
    maxHeal = RoundToNearest(maxHeal * wcGetSpellInfoFloat(race, 10, "multi"));
    new tick = RoundToNearest( float(maxHeal) / 4.0);
    Renew(client, target, tick, 4, 0, 3.0);
  }
  if(target!=client)
  {
    WCMessage(target,"greater heal target",target,name,heal);
  }
  WCMessage(client,"greater heal caster",client,name2,heal);
  location[2]+=40;
  locationatt[2]+=40;
  return 0;
}


public wcRunSpell_5_4(spellid,race,client) //Power Word: Shield
{
  new target=GetAimTarget(client,50.0, true, true); //ignore if hp is full
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
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  wcGetSpellInfo(race,spellid,"absorb",line);
  new absorb=StringToInt(line);
  wcGetTalentInfo(5,1,1,3,client,"increment",line);
  new Float:talentbonus=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line)+talentbonus;
  wcGetSpellInfo(race,spellid,"duration",line);
  new Float:duration=StringToFloat(line);
  absorb+=GetSpellDmg(client,multi);
  if(CheckRequirements(client,5,1,4,2))
  {
    wcGetTalentInfo(5,1,4,2,client,"targets",line);
    new bonustargets = StringToInt(line);
    wcGetTalentInfo(5,1,4,2,client,"distance",line);
    new Float:distance = StringToFloat(line);
    new targett;
    decl Float:location[3];
    GetClientAbsOrigin(target,location);
    new team=GetClientTeam(client);
    if(team==2)
      targett=GetNearestClient("@t",location,distance,target);
    if(team==3)
      targett=GetNearestClient("@ct",location,distance,target);
    if(bonustargets>=2)
    {
      new targetb;
      if(team==2)
        targetb=GetNearestClient("@t",location,distance,target,targett);
      if(team==3)
        targetb=GetNearestClient("@ct",location,distance,target,targett);
      if(targetb>0)
      {
        wcClientAbsorb[targetb]=absorb;
        SetEffectVictim(targetb,5); // absorb shield
        SetEffectSpellVic(targetb,5); //Absorb magic dmg
        SetReflectiveShield(targetb,client);
        g_iShieldStack[targetb]+=1;

        new Handle:datapack2=CreateDataPack();
        WritePackCell(datapack2,targetb); // Write the client index of owner
        WritePackCell(datapack2,5);
        CreateTimer(duration,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
        new Handle:datapack3=CreateDataPack();
        WritePackCell(datapack3,targetb); // Write the client index of owner
        WritePackCell(datapack3,5);
        CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);
        if(HasGlyph(client, GLYPH_PRIEST_SHIELD)) {
          wcHeal(client, targetb, RoundToNearest(0.25 * absorb),spellid,false);
        }

        if(client!=targetb)
        {
          decl String:name[50],String:name2[50];
          GetClientName(client,name,sizeof(name));
          GetClientName(targetb,name2,sizeof(name2));
          if(talentbonus>0.0)
            WCMessage(targetb,"imp pw shield used victim",targetb,duration,absorb,name,"");
          else
            WCMessage(targetb,"pw shield used victim",targetb,duration,absorb,name,"");
          wcHealingDone[client]+=RoundToNearest(float(absorb)/2.0);
        }
        GenerateHealThreat(client, RoundToNearest(float(absorb)/2.0));
      }
    }
    if(targett>0)
      {
      wcClientAbsorb[targett]=absorb;
      SetEffectVictim(targett,5); // absorb shield
      SetEffectSpellVic(targett,5); //Absorb magic dmg
      SetReflectiveShield(targett,client);
      g_iShieldStack[targett]+=1;

      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,targett); // Write the client index of owner
      WritePackCell(datapack2,5);
      CreateTimer(duration,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
      new Handle:datapack3=CreateDataPack();
      WritePackCell(datapack3,targett); // Write the client index of owner
      WritePackCell(datapack3,5);
      CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);
      if(HasGlyph(client, GLYPH_PRIEST_SHIELD)) {
        wcHeal(client, targett, RoundToNearest(0.25 * absorb),spellid,false);
      }

      if(client!=targett)
      {
        decl String:name[50],String:name2[50];
        GetClientName(client,name,sizeof(name));
        GetClientName(targett,name2,sizeof(name2));
        if(talentbonus>0.0)
          WCMessage(targett,"imp pw shield used victim",targett,duration,absorb,name,"");
        else
          WCMessage(targett,"pw shield used victim",targett,duration,absorb,name,"");
        wcHealingDone[client]+=RoundToNearest(float(absorb)/2.0);
      }
      GenerateHealThreat(client, RoundToNearest(float(absorb)/2.0));
    }
  }
  wcClientAbsorb[target]=absorb;
  SetEffectVictim(target,5); // absorb shield
  SetEffectSpellVic(target,5); //Absorb magic dmg
  SetReflectiveShield(target,client);
  g_iShieldStack[target]+=1;

  new Handle:datapack2=CreateDataPack();
  WritePackCell(datapack2,target); // Write the client index of owner
  WritePackCell(datapack2,5);
  CreateTimer(duration,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,target); // Write the client index of owner
  WritePackCell(datapack3,5);
  CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);
  if(HasGlyph(client, GLYPH_PRIEST_SHIELD)) {
    wcHeal(client, target, RoundToNearest(0.25 * absorb),spellid,false);
  }
  if(HasGlyph(client, GLYPH_PRIEST_PWSHIELD)) {
    new Float:durations=8.0;
    AddHaste(client, 25.0);
    wcRecalculateStats(client);
    new Handle:datapack4=CreateDataPack();
    WritePackCell(datapack4,client); // Write the client index of owner
    CreateTimer(durations,PWSHaste,datapack4,TIMER_HNDL_CLOSE);
    WCMessage(client,"pwshaste glyph",client,"");
  }

  if(client!=target)
  {
    decl String:name[50],String:name2[50];
    GetClientName(client,name,sizeof(name));
    GetClientName(target,name2,sizeof(name2));
    if(talentbonus>0.0)
    {
      WCMessage(client,"imp pw shield used caster",client,duration,absorb,name2,"");
      WCMessage(target,"imp pw shield used victim",target,duration,absorb,name,"");
    }
    else
    {
      WCMessage(client,"pw shield used caster",client,duration,absorb,name2,"");
      WCMessage(target,"pw shield used victim",target,duration,absorb,name,"");
    }
    wcHealingDone[client]+=RoundToNearest(float(absorb)/2.0);
  }
  else
  {
    if(talentbonus>0.0)
    WCMessage(client,"imp pw shield used self",client,duration,absorb,"");
    else
    WCMessage(client,"pw shield used self",client,duration,absorb,"");
  }
  GenerateHealThreat(client, RoundToNearest(float(absorb)/2.0));
  return 1;
}

public wcRunSpell_5_5(spellid,race, target,client)
{
  //new target=GetAimTarget(client,15.0);
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      decl String:line[255];
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(5,3,1,2,client,"cooldown",line);
      new Float:talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line)-talentbonus;
      new manareq = GetSpellCost(client, spellid);
      wcGetTalentInfo(5,3,1,3,client,"multi",line);
      talentbonus=StringToFloat(line);
      manareq=RoundToNearest(manareq*(1-talentbonus));
      wcSetMana(client,GetMana(client)-manareq);
      SetCdSpell(client, cdtype, cd, spellid);
      new Float:location[3],Float:locationatt[3];
      GetClientAbsOrigin(target,location);
      GetClientAbsOrigin(client,locationatt);
      wcGetTalentInfo(5,3,1,2,client,"increment",line);
      talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line)+talentbonus;
      new dmg=GetSpellDmg(client,multi);

      EmitAmbientSound("wc/spells/mindblast.wav",location);

      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","mindblast target", target);
      Format(attMsg,sizeof(attMsg),"%T","mindblast caster", client);
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(target,client,dmg, attMsg, vicMsg,line);

      location[2]+=40;
      locationatt[2]+=40;
      GlowSpriteEffect("@all",location, g_ef_old_xfire, 3.0, 1.2, 255);
      new color[4]={30,30,30,255};
      BeamEffect("@all",locationatt,location,g_ef_tp_beam001,5,3.0,7.0,10.0,color,0.0,5);
    }
    else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
  WCMessage(client,"no target",client);
}

public wcRunSpell_5_6(spellid,race,client)
{
  new target=GetAimTarget(client,15.0);
  if(!IsBoss())
  {
    if(target>0)
    {
      if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
      {
        if(!IsCaster(target)) {
          WCMessage(client, "target no mana", client);
          return;
        }
        decl String:line[255];
        wcGetSpellInfo(race,spellid,"cooldowntype",line);
        new cdtype=StringToInt(line);
        wcGetSpellInfo(race,spellid,"cooldown",line);
        new Float:cd=StringToFloat(line);
        new manareq=GetSpellCost(client, spellid);
        wcGetTalentInfo(5,3,1,3,client,"multi",line);
        new Float:talentbonus=StringToFloat(line);
        manareq=RoundToNearest(manareq*(1-talentbonus));
        wcSetMana(client,GetMana(client)-manareq);
        SetCdSpell(client, cdtype, cd, spellid);
        new Float:location[3],Float:locationatt[3];
        GetClientAbsOrigin(target,location);
        GetClientAbsOrigin(client,locationatt);
        wcGetTalentInfo(5,3,2,2,client,"increment",line);
        talentbonus=StringToFloat(line);
        wcGetSpellInfo(race,spellid,"multi",line);
        new Float:multi=StringToFloat(line)+talentbonus;
        wcGetSpellInfo(race,spellid,"dmgmulti",line);
        new Float:dmgmulti=StringToFloat(line);
        decl String:name[255],String:name2[255];
        GetClientName(client,name,sizeof(name));
        GetClientName(target,name2,sizeof(name2));

        new vmana=wcGetSpellInfoInt(race,spellid,"base") + GetSpellDmg(client, multi);
        new vmana2= GetMaxMana(target)/4;
        if(vmana2 < vmana)
        vmana = vmana2;

        if(GetMana(target)<vmana)
        vmana=GetMana(target);
        new dmg=RoundToNearest(float(vmana)*dmgmulti);
        wcSetMana(target,GetMana(target)-vmana);
        if(CheckRequirements(client,5,3,2,4))
        wcRunTalent_5_ManaManipulation(client, vmana);
        EmitAmbientSound("wc/spells/manaburn.wav",location);

        decl String:vicMsg[300], String:attMsg[300];
        Format(vicMsg,sizeof(vicMsg),"%T","manaburn target", target, vmana);
        Format(attMsg,sizeof(attMsg),"%T","manaburn caster", client, vmana);
        wcGetSpellInfo(race,spellid,"name",line);
        wcSpellDamage(target,client,dmg, attMsg, vicMsg,line);

        TE_SetupBubbles(location, location,SWD_sprite,100.0,100,1000.0);
        TE_SendToAll();
        TE_SetupDynamicLight(location, 255,20,20,400,20.0,1.0,0.1);
        TE_SendToAll();
      }
      else  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
    }
    else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
  }
  else
  WCMessage(client,"boss spell blocked",client);
}

public wcRunSpell_5_7(spellid,race,client) //Power Word: Fortitude
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  new bonus=RoundToNearest(float(GetLevel(client))*multi);
  CastFort(client,bonus,true);
}

public wcRunSpell_5_8(spellid, race, client)  { //Resurrection
  if(!(GetCombat(client)>0) || IsBoss())  {
    ChooseTarget(client, 5,8, spellid, race, "Choose a player to Resurrect");
  }
  else
  {
    WCMessage(client,"in combat",client,float(GetCombat(client))/2.0);
  }
}

new g_ff_iTargetData[65][4];
ChooseTarget(client, first, second, spellid, race, String:title[])  {
  //  if (!IsClientInGame(client))
  //    return;
  if (!IsPlayerAlive(client))
  {
    WCMessage(client,"you are dead",client);
    return;
  }
  g_ff_iTargetData[client][0] = first;
  g_ff_iTargetData[client][1] = second;
  g_ff_iTargetData[client][2] = spellid;
  g_ff_iTargetData[client][3] = race;

  decl String:line[255];
  if(GetClass(client)==9)
    wcGetTalentInfo(9,3,4,1,client,"distance",line);
  else
    wcGetSpellInfo(race,spellid,"distance",line);
  new Float:distance=StringToFloat(line);

  new Handle:menu=CreateMenu(ChooseTargetHandle);
  SetMenuExitBackButton(menu,true);
  SetMenuTitle(menu,title);
  decl String:buffer[5];
  new maxplayers=GetMaxClients();
  for (new i=1;i<=maxplayers;i++)
  {
    if(IsClientInGame(i))
    if(IsClientAuthorized(i) && playerloaded[i]==1 && (!IsPlayerAlive(i)) && GetClientTeam(i) == GetClientTeam(client))
    {
      new Float:loc2[3];
      loc2 = vClientCorpse[i];
      new Float:loc[3];
      GetClientAbsOrigin(client, loc);
      if(GetDistanceBetween(loc, loc2) > distance)
      continue;

      decl String:Srace[255];
      race=GetClass(i);
      decl String:Sname[20];
      GetClientName(i,Sname,sizeof(Sname));
      wcClassToName(i,race,Srace,sizeof(Srace));
      Format(line,sizeof(line),"%s (%s  %i)",Sname,Srace,GetLevel(i));
      IntToString(i,buffer,sizeof(buffer));
      AddMenuItem(menu,buffer,line);
    }
  }
  DisplayMenu(menu,client,7);

}


public ChooseTargetHandle(Handle:menu,MenuAction:action,client,slot)
{

  if(action==MenuAction_Select)
  {
    if(!IsPlayerAlive(client))
    {
      WCMessage(client,"you are dead",client);
      return;
    }
    else
    {
      decl String:buffer[5];
      GetMenuItem(menu,slot,buffer,sizeof(buffer));
      new i=StringToInt(buffer);
      if(IsClientInGame(i) && IsClientAuthorized(i) && playerloaded[i]==1 && (!IsPlayerAlive(i)) && GetClientTeam(i) == GetClientTeam(client))
      {
        new first = g_ff_iTargetData[client][0];
        new second = g_ff_iTargetData[client][1];
        new spellid = g_ff_iTargetData[client][2];
        new race = g_ff_iTargetData[client][3];

        decl String:line[255];
        if(GetClass(client)==9)
          wcGetTalentInfo(9,3,4,1,client,"distance",line);
        else
          wcGetSpellInfo(race,spellid,"distance",line);
        new Float:distance=StringToFloat(line);

        new Float:loc2[3];
        loc2 = vClientCorpse[i];
        new Float:loc[3];
        GetClientAbsOrigin(client, loc);
        if(GetDistanceBetween(loc, loc2) > distance)
        WCMessage(client, "target out of range", client);
        else  {
          //Call function
          decl String:funcName[100];
          if(GetClass(client)==9)
            Format(funcName, sizeof(funcName), "wcRunTalent_9_Resusciate2", first, second);
          else
            Format(funcName, sizeof(funcName), "wcRunSpell_%d_%d_2", first, second);
          new Function:func = GetFunctionByName(INVALID_HANDLE, funcName);
          Call_StartFunction(INVALID_HANDLE, func);
          Call_PushCell(spellid);
          Call_PushCell(race);
          Call_PushCell(client);
          Call_PushCell(i); //Target
          if(GetReopenSpellbar(client))
          {
            SetReopenSpellbar(client,false);
            SpellBar(client);
          }
          if(Call_Finish() != SP_ERROR_NONE)  {
            decl String:err[100];
            Format(err,100,"Could not callback the function: %s",funcName);
            Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
          }

        }
      }
      else
      WCMessage(client, "invalid target", client);
    }

  }
  else if(action==MenuAction_Cancel)
  {
    if(slot==MenuCancel_ExitBack)
    SpellBar(client);
  }
  else if(action==MenuAction_End)
  CloseHandle(menu);
}

public wcRunSpell_5_8_2(spellid,race,client,target )
{
  if(!(GetCombat(client)>0) || IsBoss())
  {
    decl String:line[255];
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    wcGetSpellInfo(race,spellid,"distance",line);
    if(target > 0) {

      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(5,2,1,3,client,"cooldown",line); //Improved Resurrection
      new Float:talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line)-talentbonus;
      new manareq=GetSpellCost(client, spellid);
      wcSetMana(client,GetMana(client)-manareq);
      SetCdSpell(client, cdtype, cd, spellid);
      decl String:name[255],String:name2[255];
      GetClientName(client,name,sizeof(name));
      GetClientName(target,name2,sizeof(name2));
      if(talentbonus>0.0)
      {
        WCMessage(target,"improved resurrect target",target,name);
        WCMessage(client,"improved resurrect",client,name2);
      }
      else
      {
        WCMessage(target,"resurrect target",target,name);
        WCMessage(client,"resurrect",client,name2);
      }
      CS_RespawnPlayer(target);
      EmitAmbientSound("wc/spells/resurrect.wav",location);
      new hp = GetMaxHealth(target);
      hp = RoundToNearest(float(hp) * 0.5);
      wcHealingDone[client]+=hp;
      CreateTimer(0.1,ResurrectionEffect,target);
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

public wcRunSpell_5_9(spellid,race,client)
{
  //Hymn of Hope priest 80 lvl
  decl String:line[255];
  //CD and mana
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client, cdtype, cd, spellid);
  new manareq=GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  //Channeling
  wcGetSpellInfo(race,spellid,"cast",line);
  new Float:castTime=StringToFloat(line);
  SetCasting(client,spellid,castTime);

  //Getting values
  wcGetSpellInfo(race,spellid,"effect",line);
  new Float:perc=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"period",line);
  new Float:period=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"aoe",line);
  new aoe=StringToInt(line);

  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  location[2]+=50;
  EmitAmbientSound("wc/spells/hymn.wav",location); //modify
  WCMessage(client,"hymn cast",client,perc);

  //Setting up effect
  new team=GetClientTeam(client);
  new effid=CreateEffectData(client,spellid,RoundToNearest(perc*10.0),0,aoe,team,0,0,g_RoundCount);
  if(effid!=0)
  {
    effectsdata[effid][7]=SpawnHymn(client);
    effectsdata[effid][8]=SpawnHymn2(client);
    SpawnHymn3(client);
    CreateTimer((period-0.03), EffectHymnAction, effid, TIMER_REPEAT);
  }
  //end

}

public wcRunSpell_7_1(spellid,race, target,client,bool:nightfall)
{
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      decl String:line[255];
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line);
      new manareq = GetSpellCost(client, spellid);
      if(nightfall)
        manareq=0;
      wcGetTalentInfo(7,3,1,4,client,"multi",line);
      new Float:hred = StringToFloat(line);
      if(GetHM(target < hred))
      {
        SetHM(target,hred);
        wcGetTalentInfo(7,3,1,4,client,"duration",line);
        new Float:duration = StringToFloat(line);
        SetHMT(target,duration);
      }
      
      wcGetSpellInfo(race,spellid,"basedmg",line);
      new basedmg=StringToInt(line);
      wcSetMana(client,GetMana(client)-manareq);
      if(!nightfall)
      SetCdSpell(client, cdtype, cd, spellid);
      new Float:location[3],Float:locationatt[3];
      GetClientAbsOrigin(target,location);
      GetClientAbsOrigin(client,locationatt);
      wcGetTalentInfo(7,3,1,1,client,"increment",line);
      new Float:talentbonus=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line)+talentbonus;
      new dmg=GetSpellDmg(client,multi)+basedmg;
      if(nightfall)
      dmg=RoundToNearest(float(dmg)/3);
      EmitAmbientSound("wc/spells/shadow.wav",location);

      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","shadow bolt target", target);
      Format(attMsg,sizeof(attMsg),"%T","shadow bolt caster", client);
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(target,client,dmg, attMsg, vicMsg,line);
      if(HasGlyph(client, GLYPH_WARLOCK_SHADOWBOLT))
      {
        new breakdmg = RoundToNearest(2000.0);
        new Float:tickdelay=0.50;
        new ticks = RoundToNearest(6.0);
        new effid=CreateEffectData(target,client,ticks,0,breakdmg,0,0,race,g_RoundCount);
        if(effid!=0)
        {
          SetFearDmg(target,breakdmg);
          SetFear(target,effid);
          g_fearspeed[target][0]=GetClientSpeed(target);
          g_fearspeed[target][1]=GetClientSpeed(target);
          FadeEffect(target,2,300,50,{53,55,40,230});
          CreateTimer(tickdelay, EffectFearAction, effid, TIMER_REPEAT);
        }
      }
      location[2]+=40;
      locationatt[2]+=40;
      VS_BeamPoints(location, locationatt, g_ef_scanner,g_ef_scanner, 1, 5, 3.00, 15.00, 15.00, 1, 0.00, {153, 0, 153, 255}, 20);
      VS_BeamPoints(location, locationatt, g_ef_scanner, g_ef_scanner, 1, 5, 3.00, 15.00, 15.00, 1, 0.00, {153, 0, 153, 255}, 20);
      VS_BeamPoints(locationatt, location, g_ef_glow1, g_ef_glow1, 1, 5, 3.00, 20.00, 10.00, 10, 0.00, {153, 0, 153, 255}, 160);
      VS_BeamPoints(locationatt, location, g_ef_lgtning, g_ef_glow1, 1, 5, 3.00, 20.00, 10.00, 0, 0.00, {153, 0, 153, 255}, 160);
      VS_BeamRingPoint(location, 90.00, 60.00, g_ef_glow1, g_ef_lgtning, 10, 10, 3.00, 50.00, 0.00, {153, 0, 153, 255}, 60, 0);

    }
    else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
  WCMessage(client,"no target",client);
}
public Action:HaniGlyphFearThing(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0) //effect id no longer in use!
  return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2])) //Either of you arent ingame :P
  {
    RemoveFear(effectsdata[effid][1]); //remove fear from the poor sap
    DeleteEffectData(effid); //get rid of the effid so it can be used again later
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1])) //they died
  {
    RemoveFear(effectsdata[effid][1]);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0) //no ticks
  {
    RemoveFear(effectsdata[effid][1]);
    SetFear(effectsdata[effid][1],0);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount) //no longer the same round
  {
    DeleteEffectData(effid);
    RemoveFear(effectsdata[effid][1]);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--; //ticks--
  FadeEffect(effectsdata[effid][1],2,300,50,{53,55,40,230});

  return Plugin_Continue;
}

/*public wcRunSpell_7_2(spellid,race,client)
{
  new target=GetAimTarget(client,15.0);
  decl String:line[255];
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line);
      SetCdSpell(client, cdtype, cd, spellid);
      new manareq = GetSpellCost(client, spellid);
      wcGetSpellInfo(race,spellid,"baseef",line);
      new baseef=StringToInt(line);
      wcGetTalentInfo(7,3,1,3,client,"multi",line);
      new Float:talentbonusbase=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"basedmg",line);
      new basedmg=RoundToNearest(StringToInt(line)*(1+talentbonusbase));
      wcSetMana(client,GetMana(client)-manareq);
      wcGetSpellInfo(race,spellid,"multi",line);
      new Float:multi=StringToFloat(line);
      new dmg=GetSpellDmg(client,multi)+basedmg;
      wcGetTalentInfo(7,1,2,4,client,"multi",line);
      new Float:talentbonusdamage=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"multief",line);
      new Float:multief=StringToFloat(line);
      new dmgef=RoundToNearest((GetSpellDmg(client,multief)+baseef)*(1+talentbonusdamage));
      decl Float:location[3];
      GetClientAbsOrigin(target,location);
      location[2]+=50;
      EmitAmbientSound("wc/spells/fireball.wav",location);
      wcGetSpellInfo(race,spellid,"ticks",line);
      new ticks = StringToInt(line);

      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","immolate cast vic", target, dmgef);
      Format(attMsg,sizeof(attMsg),"%T","immolate cast", client, dmgef);
      WCMessage(client,"immolate cast",client, dmgef);
      WCMessage(client,"immolate cast vic",target, dmgef);
      wcGetSpellInfo(race,spellid,"name",line);
      wcSpellDamage(target,client,dmg, attMsg, vicMsg,line);
      new aoe = 1;
      if(HasGlyph(client, GLYPH_WARLOCK_IMMOLATE))  {
      wcGetSpellInfo(race,spellid,"aoe",line);
      aoe=StringToInt(line)+talentbonusrange;
      wcGetTalentInfo(7,1,2,3,client,"increment",line);
      new talentbonusrange=StringToInt(line);
      }
      new team=GetClientTeam(target);
      wcSpellAoeDamage(target,client,damage,aoe,team,line);

      // immolate effect
      new Float:position_victim[3];
      GetClientAbsOrigin(target, position_victim);  //Target is victim here. we can see it
      //in the beginning of spell
      new Float:position_caster[3];
      GetClientAbsOrigin(client, position_caster);
      VS_GlowSprite(position_victim, g_ef_fire1, 3.00, 1.00, 255);
      VS_BeamRingPoint(position_victim, 40.00, 199.00, g_ef_fire1, g_ef_fire1, 100, 300, 2.00, 20.00, 0.80, {255, 255, 255, 255}, 20, 0);
      VS_BeamRingPoint(position_victim, 50.00, 10.00, g_ef_fire1, g_ef_fire1, 100, 300, 1.00, 199.00, 0.00, {255, 255, 255, 255}, 20, 0);
      VS_BeamRingPoint(position_victim, 40.00, 199.00, g_ef_fire1, g_ef_fire1, 100, 300, 1.00, 10.00, 0.80, {255, 255, 255, 255}, 20, 1);

      position_victim[2]+=40;
      position_caster[2]+=40;


      VS_BeamPoints(position_victim, position_caster, g_ef_fire2, g_ef_fire2, 10, 10, 1.00, 3.00, 4.00, 10, 1.00, {255, 255, 255, 255}, 200);
      //setting up flame ticks
      wcGetSpellInfo(race,spellid,"tickdelay",line);
      new Float:tickdelay=StringToFloat(line);
      wcGetTalentInfo(7,1,2,4,client,"ticks",line);
      new talentbonusticks=StringToInt(line);
      ticks+=talentbonusticks;
      wcGetTalentInfo(7,1,2,3,client,"increment",line);
      new talentbonusrange=StringToInt(line);
      wcGetSpellInfo(race,spellid,"aoe",line);
      aoe=StringToInt(line)+talentbonusrange;
      new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,IMMOLATE_UNIQUE,2,g_RoundCount);
      if(effid!=0)
        {
          CreateTimer(tickdelay, EffectAoeDamage, effid, TIMER_REPEAT);
        }
      //end
    }
    else  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
}*/


public wcRunSpell_7_3(spellid,race, target,client)
{
  //new target=GetAimTarget(client,15.0);
  decl String:line[255];
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line);
      SetCdSpell(client, cdtype, cd, spellid);
      new manareq = GetSpellCost(client, spellid);
      wcSetMana(client,GetMana(client)-manareq);
      wcGetSpellInfo(race,spellid,"multief",line);
      new Float:multief=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"baseef",line);
      new baseef=StringToInt(line);
      wcGetTalentInfo(7,1,2,4,client,"multi",line);
      new Float:talentbonusdamage=StringToFloat(line);
      wcGetTalentInfo(7,1,1,3,client,"multi",line);
      talentbonusdamage+=StringToFloat(line);
      wcGetTalentInfo(7,1,2,4,client,"ticks",line);
      new talentbonusticks=StringToInt(line);
      new dmgef=RoundToNearest((GetSpellDmg(client,multief)+baseef)*(1+talentbonusdamage));
      decl Float:location[3];
      GetClientAbsOrigin(target,location);
      location[2]+=50;
      EmitAmbientSound("wc/spells/curse.wav",location);
      new String:atname[50],String:vicname[50];
      GetClientName(client,atname,sizeof(atname));
      GetClientName(target,vicname,sizeof(vicname));
      WCMessage(client,"corruption cast",client,dmgef,vicname);
      WCMessage(target,"corruption cast vic",target,dmgef,atname);
      // corruption effect

      //setting up corruption
      wcGetSpellInfo(race,spellid,"tickdelay",line);
      new Float:tickdelay=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"ticks",line);
      new ticks=StringToInt(line)+talentbonusticks;
      ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
      tickdelay = tickdelay*(1.0-GetHasteEffect(client));
      wcGetTalentInfo(7,1,2,3,client,"increment",line);
      new talentbonusrange=StringToInt(line);
      wcGetSpellInfo(race,spellid,"aoe",line);
      new aoe=StringToInt(line)+talentbonusrange;
      new team=GetClientTeam(target);
      new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,0,3,g_RoundCount);
      if (tickdelay==0.0 || ticks == 0 || dmgef == 0)
      {
        LogError("ERROR: immolate spell corrupted.");
        DeleteEffectData(effid);
      }
      else
      if(effid!=0)
      {
        CreateTimer(tickdelay, EffectAoeDamage, effid, TIMER_REPEAT);
      }

      if(CheckRequirements(client,7,1,3,3))
      wcRunTalent_7_ShadowGrip(client,target);
      //end
    }
    else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
  WCMessage(client,"no target",client);
}

new bool:g_WarlockFeared[65];

bool:DidWarlockFear(client) {
  return g_WarlockFeared[client];
}

SetWarlockFeared(client, bool:value) {
  g_WarlockFeared[client] = value;
}

public Action:DestroyWarlockFeared(Handle:timer, any:client) {
  SetWarlockFeared(client, false);
}

public wcRunSpell_7_4(spellid,race,client) //Fear Spell
{
  if(IsBoss() && GetBoss(client)==0)  {
    WCMessage(client, "boss spell blocked", client);
    return;
  }
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetTalentInfo(7,2,2,4,client,"fcd",line);
  cd-=StringToFloat(line);
  SetCdSpell(client, cdtype, cd, spellid);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);

  //Range of fear
  wcGetSpellInfo(race,spellid,"range",line);
  new Float:range=StringToFloat(line);
  wcGetTalentInfo(7,2,2,1,client,"multi",line);
  range*=(1.0+StringToFloat(line));

  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
  amount=GetNearClients("@ct",location,targets,range);
  if(team==3)
  amount=GetNearClients("@t",location,targets,range);

  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  new String:stargets[255];

  //Time between ticks
  wcGetSpellInfo(race,spellid,"tickdelay",line);
  new Float:tickdelay=StringToFloat(line);

  //Number of ticks
  wcGetSpellInfo(race,spellid,"ticks",line);
  new ticks=StringToInt(line);
  wcGetTalentInfo(7,2,2,1,client,"duration",line);
  ticks += StringToInt(line);

  //duration
  new Float:dur=tickdelay*float(ticks);

  //breakdamage
  wcGetSpellInfo(race,spellid,"breakdmg",line);
  new breakdmg=StringToInt(line);

  //Glyph
  if(HasGlyph(client, GLYPH_WARLOCK_FEAR))
  {
    dur    *=0.6;
    breakdmg *=  2;
  }

  wcGetSpellInfo(race,spellid,"power",line);
  new power=StringToInt(line);
  new Float:location2[3];
  new Float:distance;
  new color2[4]={53,55,149,255};

  for (new x=0;x<=(amount-1);x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
    Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
    Format(stargets,sizeof(stargets),"%s %s",stargets,name2);

    WCMessage(targets[x],"fear target",targets[x],name);

    if(GetClass(targets[x])==8 && wcGetTalentLevel(targets[x],2,2,1)>0) //DK Lichborne Talent
    continue;
    if(UnableToAttack[targets[x]])
    continue;

    new effid=CreateEffectData(targets[x],client,ticks,power,breakdmg,0,RoundToNearest(ticks*0.5),race,g_RoundCount);
    if (tickdelay==0.0 || ticks == 0)
    {
      LogError("ERROR: fear spell corrupted.");
      DeleteEffectData(effid);
    }
    else if(effid!=0)
    {
      //anti defuse
      AntiDefuse(client);
      SetWarlockFeared(client, true);

      CreateTimer(dur, DestroyWarlockFeared, client);
      SetFearDmg(targets[x],breakdmg);
      SetFear(targets[x],effid);
      SetSilence(targets[x],dur * 0.5,true);
      GetClientAbsOrigin(targets[x],location2);
      g_fearspeed[targets[x]][0]=GetClientSpeed(targets[x]);
      g_fearspeed[targets[x]][1]=GetClientSpeed(targets[x]);
      distance=GetDistanceBetween(location,location2);
      if(distance<=range/2) //close enough to be scared out of their wits!
      {
        FadeEffect(targets[x],2,300,50,color2);
        effectsdata[effid][6] = 1;
      }
      CreateTimer(tickdelay, EffectFearAction, effid, TIMER_REPEAT);
    }
  }
  if(amount==0)
  Format(stargets,sizeof(stargets),"%T","nobody",client);
  WCMessage(client,"fear caster",client,stargets);
  EmitAmbientSound("wc/spells/fear.wav",location);

  VS_BeamRingPoint(location, 5.00, 300.00, g_ef_scanner, g_ef_bluelight, 10, 30, 1.00, 50.00, 0.20, {255, 255, 255, 255}, 3, 0);
}

public Action:EffectRemoveFearVictim(Handle:timer, any:client)
{
  RemoveFear(client);
  return Plugin_Handled;
}

public Action:EffectFearAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
  return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
  {
    RemoveFear(effectsdata[effid][1]);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    RemoveFear(effectsdata[effid][1]);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0)
  {
    RemoveFear(effectsdata[effid][1]);
    SetSilence(effectsdata[effid][1],0.0,true);
    SetFear(effectsdata[effid][1],0);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    RemoveFear(effectsdata[effid][1]);
    return Plugin_Stop;
  }

  new color2[4]={53,55,149,255};
  effectsdata[effid][3]--;
  //if true and more than X% and they are scared out of their wits
  if(( effectsdata[effid][6]             == 1                             //victim was close enough to be totally feared.
        && effectsdata[effid][7]              < effectsdata[effid][3]         //and ticks greater than total fear end point
        && GetFearDmg(effectsdata[effid][1])  > effectsdata[effid][5] * 0.60) //and breakdamage remaining higher than partial fear threshold
      || effectsdata[effid][8]             == 2)                            //or casters class is rogue
  {
    FadeEffect(effectsdata[effid][1],2,300,50,color2);
    new Float:power=float(effectsdata[effid][4]);
    decl Float:vec[3],Float:rightVec[3],Float:upVec[3];
    GetClientEyePosition(effectsdata[effid][1], vec);
    vec[0] -= 30;
    vec[1] -= 30;
    GetVectorVectors(vec, rightVec, upVec);
    static const color[] = {0,0,0,200};
    TE_SetupBloodSprite(vec, rightVec, color, 25, g_bloodsprayModel, g_bloodModel);
    TE_SendToAll();
    new Float:angel=GetRandomFloat(-1.0,1.0);
    new Float:velocity[3];
    velocity[0]=power*angel;
    new Float:angel2=Sine(ArcCosine(angel));
    new temp=GetRandomInt(1,2);
    if(temp==2)
    angel2*=-1;
    velocity[1]=power*angel2;
    velocity[2]=80.0;
    TeleportEntity(effectsdata[effid][1],NULL_VECTOR,NULL_VECTOR,velocity); // Push the
  }
  else
  { //not totally scared out of their wits, lets at least continue to let them know... they are feared.
    if(effectsdata[effid][6]==1)
    {
      effectsdata[effid][6] = 0;
      SetSilence(effectsdata[effid][1],0.0,true);
    }
    color2[3] = 40;
    FadeEffect(effectsdata[effid][1],2,300,50,color2);
  }
  return Plugin_Continue;
}

public SetFear(client,effid)
{
  Feared[client]=effid;
}

public GetFear(client)
{
  return Feared[client];
}

public RemoveFear(client)
{
  effectsdata[GetFear(client)][3]=0;
  SetFear(client,0);
  if(IsClientInGame(client) && IsPlayerAlive(client)) {
    SetClientSpeed(client,g_fearspeed[client][1],true);
  }
}

SetFearDmg(client,dmg)
{
  maxFear[client]=dmg;
}

GetFearDmg(client)
{
  return maxFear[client];
}

public wcRunSpell_7_5(spellid,race,client) //Lifetap / Life Tap
{
  if(GetMaxMana(client) == GetMana(client)) {
    WCMessage(client, "full mana", client);
    return;

  }
  //setting up cooldown and -mana
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcGetTalentInfo(7,1,1,2,client,"multi",line);
  new Float:talentbonus=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line)*(1+talentbonus);
  wcGetSpellInfo(race,spellid,"power",line);
  new Float:power=StringToFloat(line);
  new hp=RoundToNearest(float(GetMaxHealth(client))*power);
  new mp=RoundToNearest(float(hp)*multi);
  if(hp<wcGetHealth(client))
  {
    if(wcGetTalentLevel(client,2,3,2)>0)  {
      wcGetTalentInfo(7,2,3,2,client,"multi",line);
      new Float:bonusspd=float(mp)*StringToFloat(line);
      new Float:spd = bonusspd+GetBonusSpellDmg(client,1.0);
      SetBonusSpellDmg(client,spd);
      wcRecalculateStats(client);
      new Float:duration=8.0;
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,client); // Write the client index of owner
      WritePackFloat(datapack4,bonusspd);
      CreateTimer(duration,LTspd,datapack4,TIMER_HNDL_CLOSE);
    }
    wcSetHealth(client,wcGetHealth(client)-hp);
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client, cdtype, cd, spellid);
    decl Float:location[3];
    GetClientAbsOrigin(client,location);
    EmitAmbientSound("wc/spells/shadow.wav",location);
    new oldmp=GetMana(client);
    mp+=oldmp;
    if(mp>GetMaxMana(client))
    mp=GetMaxMana(client);
    wcSetMana(client,mp);
    mp-=oldmp;
    WCMessage(client,"life tap cast",client,hp,mp);
  }
  else
  WCMessage(client,"too low hp life tap",client);
}

public wcRunSpell_7_6(spellid,race,client)
{
  decl String:line[255];
  wcGetTalentInfo(7,2,1,3,client,"multi",line); //Fel Armor
  new Float:talentbonus=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line); //Demon Skin
  new Float:multi=StringToFloat(line);
  new bonus=RoundToNearest(float(GetLevel(client))*(multi+talentbonus));
  SetBonusArmor(client,float(GetBonusArmor(client)+bonus));
  wcRecalculateStats(client);
  if(talentbonus>1.0)
  WCMessage(client,"demon skin cast with talent",client,bonus,GetArmor(client));
  else
  WCMessage(client,"demon skin cast",client,bonus,GetArmor(client));
}

public wcRunSpell_7_7(spellid,race,client)
{
  new target=GetAimTarget(client,15.0);
  decl String:line[255];
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      wcGetSpellInfo(race,spellid,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetSpellInfo(race,spellid,"cooldown",line);
      new Float:cd=StringToFloat(line);
      SetCdSpell(client, cdtype, cd, spellid);
      new manareq = GetSpellCost(client, spellid);
      wcGetSpellInfo(race,spellid,"baseef",line);
      new baseef=StringToInt(line);
      wcSetMana(client,GetMana(client)-manareq);
      wcGetSpellInfo(race,spellid,"multief",line);
      new Float:multief=StringToFloat(line);
      wcGetTalentInfo(7,1,2,4,client,"multi",line);
      new Float:talentbonusdamage=StringToFloat(line);
      wcGetTalentInfo(7,1,2,4,client,"ticks",line);
      new talentbonusticks=StringToInt(line);
      new dmgef=RoundToNearest((GetSpellDmg(client,multief)+baseef)*(1+talentbonusdamage));
      if(CheckRequirements(client,7,1,4,2))
      {
        if(!IsBoss() && !IsHNS())
        {
          wcGetTalentInfo(7,1,4,2,client,"multi",line);
          new Float:artimulti=StringToFloat(line);
          wcGetTalentInfo(7,1,4,2,client,"dividedby",line);
          new highdmg=RoundToNearest((float(GetMaxHealth(target))/StringToFloat(line))*artimulti);
          dmgef+=highdmg;
        }
      }
      decl Float:location[3];
      GetClientAbsOrigin(target,location);
      location[2]+=50;
      EmitAmbientSound("wc/spells/curse.wav",location);
      new String:atname[50],String:vicname[50];
      GetClientName(client,atname,sizeof(atname));
      GetClientName(target,vicname,sizeof(vicname));
      WCMessage(client,"life drain cast",client,dmgef,vicname);
      WCMessage(target,"life drain cast vic",target,dmgef,atname);

      new Float:locationatt[3];
      GetClientAbsOrigin(target,location);
      GetClientAbsOrigin(client,locationatt);
      locationatt[2]+=40;
      VS_BeamPoints(locationatt, location, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 1, 5, 3.00, 5.00, 1.00, 1, 0.00, {25, 255, 25, 255}, 20);
      VS_BeamRingPoint(location, 19.00, 20.00, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 100, 300, 3.00, 30.00, 1.00, {25, 255, 25, 255}, 300, 0);
      wcGetSpellInfo(race,spellid,"tickdelay",line);
      new Float:tickdelay=StringToFloat(line);
      wcGetSpellInfo(race,spellid,"ticks",line);
      new ticks=StringToInt(line)+talentbonusticks;
      ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
      wcGetTalentInfo(7,1,2,3,client,"increment",line);
      new talentbonusrange=StringToInt(line);
      wcGetSpellInfo(race,spellid,"aoe",line);
      new aoe=StringToInt(line)+talentbonusrange;
      new team=GetClientTeam(target);
      new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,0,0,g_RoundCount);
      if (tickdelay==0.0 || ticks == 0 || dmgef == 0)
      {
        LogError("ERROR: immolate spell corrupted.");
        DeleteEffectData(effid);
      }
      else
      if(effid!=0)
      {
        CreateTimer((tickdelay*(1.0-GetHasteEffect(client))), EffectLifeDrainAction, effid, TIMER_REPEAT);
      }
      if(CheckRequirements(client,7,1,3,3))
      wcRunTalent_7_ShadowGrip(client,target);
      //end
    }
    else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
  WCMessage(client,"no target",client);
}

public wcRunSpell_7_8(spellid,race,client) //metamorphosis
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetTalentInfo(7,2,2,4,client,"mmcd",line);
  cd-=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"duration",line);
  new Float:duration=StringToFloat(line);
  new armor=GetArmor(client);
  wcGetSpellInfo(race,spellid,"base",line);
  new newarm = StringToInt(line) + RoundToNearest(float(armor) * multi);
  if(wcGetTalentLevel(client,2,2,2)>0) {
    wcGetTalentInfo(7,2,2,2,client,"armor",line);
    newarm+=RoundToNearest(float(newarm)*StringToFloat(line));
  }
  SetBonusArmor(client,float(GetBonusArmor(client)) + newarm);
  new res=0;
  if(HasGlyph(client, GLYPH_WARLOCK_META))  {
    res = GetSpellDmg(client, 0.3);
    AddResistance(client, float(res));
  }
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/metamorphosis.wav",location);
  new Handle:datapack=CreateDataPack();
  WritePackCell(datapack,client); // Write the client index of owner
  WritePackCell(datapack,g_RoundCount);
  WritePackCell(datapack,newarm);
  WritePackCell(datapack, res);
  CreateTimer(duration,DestroyDemonForm,datapack,TIMER_HNDL_CLOSE);
  new color[4] = { 10, 10, 50, 255 };
  SetClientColor(client,color);
  new Handle:datapack2=CreateDataPack();
  WritePackCell(datapack2,client); // Write the client index of owner
  WritePackCell(datapack2,255); // red
  WritePackCell(datapack2,255); // green
  WritePackCell(datapack2,255); // blue
  WritePackCell(datapack2,255); // alpha
  CreateTimer(duration,SetClientColorT,datapack2,TIMER_HNDL_CLOSE);
  wcRecalculateStats(client);
  WCMessage(client,"metamorph",client,newarm,duration);
  if(wcGetTalentLevel(client,2,3,3)>0) //Chaos Wave
  {
    wcGetTalentInfo(7,2,3,3,client,"distance",line);
    new Float:distance = StringToFloat(line);
    wcGetTalentInfo(7,2,3,3,client,"smulti",line);
    new Float:smulti=StringToFloat(line);
    decl Float:locationb[3];
    GetClientAbsOrigin(client,locationb);
    new dmg=GetSpellDmg(client,smulti);
    wcGetTalentInfo(7,2,3,3,client,"base",line);
    dmg+=StringToInt(line);
    new targets[MAXPLAYERS];
    new team=GetClientTeam(client);
    new amount;
    if(team==2)
      amount=GetNearClients("@ct",locationb,targets,distance);
    if(team==3)
      amount=GetNearClients("@t",locationb,targets,distance);
    for (new x=0;x<=(amount-1);x++)
    {
      decl Float:loc2[3];
      GetClientAbsOrigin(targets[x],loc2);
      dmg = RoundToNearest(float(dmg)* (400.0 / (250.0 + GetDistanceBetween(locationb,loc2))));
      wcSpellDamage(targets[x],client,dmg, "", "","Chaos Wave");
      new Float:zPower = 130.0;
      wcGetTalentInfo(7,2,3,3,client,"push",line);
      new Float:power = StringToFloat(line);
      power = power * (400.0 / (150.0 + GetDistanceBetween(locationb,loc2)));
      if(GetBoss(targets[x])<=0 || (!IsBoss()) )
        PushPlayerTowardsPlayer(targets[x], client, power, zPower);
    }
  }
  if(wcGetTalentLevel(client,2,2,2)>0) {
    SetEffectAttacker(client,30);
    new Handle:datapack4=CreateDataPack();
    WritePackCell(datapack4,client);
    CreateTimer(duration,EndDeathsEmbrace,datapack4,TIMER_HNDL_CLOSE);
  }
}

public Action:EndDeathsEmbrace(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  RemoveEffectAttacker(client,30);
}

public wcRunSpell_7_9(spellid,race,client)
{
  //Hellfire warlock
  decl String:line[255];
  //CD and mana
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client, cdtype, cd, spellid);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  //Channeling
  wcGetSpellInfo(race,spellid,"cast",line);
  new Float:castTime=StringToFloat(line);
  SetCasting(client,spellid,castTime);

  //Getting values
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"period",line);
  new Float:period=StringToFloat(line);
  period = period*(1.0-GetHasteEffect(client));
  wcGetSpellInfo(race,spellid,"multief",line);
  new Float:multief=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"aoe",line);
  new aoe=StringToInt(line);
  new dmg = GetSpellDmg(client, multi);
  wcGetTalentInfo(7,3,2,1,client,"multi",line);
  new Float:talentbonus=StringToFloat(line);
  new selfDmg = RoundToNearest(float(dmg)*(multief*(1-talentbonus)));

  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  location[2]+=50;
  EmitAmbientSound("wc/spells/hellfire.wav",location); //modify
  WCMessage(client,"hellfire cast",client,dmg,selfDmg);

  //Setting up effect
  new team=GetClientTeam(client);
  team = (team-1) % 2 + 2;
  new effid=CreateEffectData(client,spellid,dmg,selfDmg,aoe,team,0,0,g_RoundCount);
  if (multi==0.0 || period == 0.0 || castTime == 0.0) {
    LogError("ERROR: hellfire spell corrupted.");
    DeleteEffectData(effid);
  }
  else if(effid!=0)
  {
    new pet=GetOwnedPet(client);
    if(pet>0 && CheckRequirements(client,7,3,4,2))
    {
      effectsdata[effid][7]=SpawnHellfire(pet);
      effectsdata[effid][8]=pet;
    }
    else
    effectsdata[effid][7]=SpawnHellfire(client);
    CreateTimer((period-0.05), EffectHellfireAction, effid, TIMER_REPEAT);
  }

}

public wcRunSpell_4_1(spellid,race,victim,attacker,&totaldmg) //Paladin Shield Block
{
  decl String:line[255];
  new Float:chance = GetBlockChance(victim,true,attacker);
  decl Float:clienta[3],Float:targeta[3];
  GetClientEyeAngles(attacker, clienta);
  GetClientEyeAngles(victim, targeta);
  new Float:angle=clienta[1]-targeta[1];
  if(angle<0.0)
  angle*=-1.0;
  if(angle>180.0)
  angle = 360.0-angle;

  if( (chance>=GetRandomFloat(0.0001,1.0)) && (angle > 100.0 || !IsValidShooting(attacker, victim)) )
  {
    for(new x=1;x<=50;x++)
    {
      if((spellid=wcGetSpell(victim,x))!=0)
      {
        wcGetSpellInfo(race,spellid,"event",line);
        if(StrEqual(line,"player_block",false))
        {
          new tmp=0;
          wcRunSpell(spellid,race,victim,attacker,totaldmg,tmp,tmp,tmp,tmp,tmp,tmp,victim);
        }
      }
    }
    wcGetTalentInfo(4,2,4,2,victim,"chance",line);
    new chancer = StringToInt(line);
    if (chancer >= GetRandomInt(1,100))
    wcRunTalent_4_15(victim);
    wcGetTalentInfo(4,2,2,1,victim,"chance",line);
    new chanced = StringToInt(line);
    if (chanced >= GetRandomInt(1,100))
      wcRunTalent_4_ArdentDefender(victim);
    wcRunTalent_4_HolyShield(victim);
    wcRunTalent_4_Sanctuary(victim);
    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    EmitAmbientSound("wc/spells/shieldwall.wav",location);
    totaldmg=0;

    AddEMsgAff("blocked damage", true);
  }
  return totaldmg;
}

public wcRunSpell_4_2(spellid,race,client)
{
  //setting up cooldown and -mana
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(4,2,1,2,client,"cooldown",line);
  new Float:talentbonus=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line)-talentbonus;
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  location[2]+=15;
  wcGetTalentInfo(4,2,1,2,client,"duration",line);
  talentbonus=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"duration",line);
  new Float:duration=StringToFloat(line)+talentbonus;

  UnableToAttack[client]=true;
  UnableToVictim[client]=true;

  new effid=CreateEffectData(client,spellid,16,0,0,0,0,0,g_RoundCount);
  if(effid!=0)
  {
    effectsdata[effid][7]=BubbleEffect(client);
    CreateTimer(duration/16.0, EffectBubble, effid, TIMER_REPEAT);
  }


  AntiDefuse(client);


  SetSilence(client, 0.0);
  RemoveFear(client);

  //end
  EmitAmbientSound("wc/spells/divine.wav",location);
  //threat
  wcGetSpellInfo(race,spellid,"threat",line);
  new threat = StringToInt(line);
  wcGetTalentInfo(4,2,1,2,client,"multi",line);
  threat = RoundToNearest(threat*(1-StringToFloat(line)));
  AddThreat(client,threat);
  WCMessage(client,"shield cast",client,duration);
}

AntiDefuse(client)  {
  if(Defuser==client)
  {
    new bombent = FindEntityByClassname(-1,"planted_c4");
    if (bombent>-1)
    {

      WCMessage(client, "cant defuse", client);
      CreateTimer(0.2,DefuseDelay,0);
    }
  }
}

public wcRunSpell_4_5(spellid,race,client)
{
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
  //setting up cooldown and -mana
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);
  // setting up the visual effect
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetSpellInfo(race,spellid,"duration",line);
  new Float:duration=StringToFloat(line);
  if(duration>0.0)
  {
    FreedomClient[target]=true;
    new Float:oldspeed=GetClientSpeed(target);
    new Float:newspeed=GetNormalSpeed(target);
    if(newspeed>oldspeed)
    {
      SetClientSpeed(target,newspeed);
    }
    CreateTimer(duration,RemoveFreedom,target);
  }
  // sound + message

  EmitAmbientSound("wc/spells/seal.wav",location);
  decl String:name[255],String:name2[255];
  GetClientName(target,name,sizeof(name));
  GetClientName(client,name2,sizeof(name2));
  if(target!=client)
  WCMessage(client,"freedom cast",client,duration,name);
  WCMessage(target,"freedom cast victim",target,duration,name2);
  return 1;
}

public Action:RemoveFreedom(Handle:timer, any:client)
{
  WCMessage(client,"freedom expired",client);
  FreedomClient[client]=false;
}

public wcRunSpell_4_6(spellid,race,client)
{
  //setting up cooldown and -mana
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, 0, "Seal");
  // setting up the visual effect
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetSpellInfo(race,spellid,"duration",line);
  new Float:duration=StringToFloat(line);



  //new
  new holy = wcGetTreeTotal(client, getActiveSpec(client),1);
  new protection = wcGetTreeTotal(client, getActiveSpec(client),2);
  new retribution = wcGetTreeTotal(client, getActiveSpec(client),3);
  if(IsFakeClient(client))  {
    holy = 6;
    protection = 6;
    retribution = 11;
  }
  new seals = SetSeals(holy, protection, retribution);
  SetSealT(client,duration);
  SetSeal(client,seals);
  // sound + message
  EmitAmbientSound("wc/spells/seal.wav",location);
  new String:sSeals[50];
  getSealsString(client, sSeals, 50);
  WCMessage(client,"seal cast",client, sSeals,duration);
  //making inform when expired
  new Handle:pack=CreateDataPack();
  WritePackCell(pack,client); // Write the client index of owner
  WritePackFloat(pack,GetSealT(client));
  CreateTimer(duration,InformSealExpired,pack,TIMER_HNDL_CLOSE);
}

public Action:InformSealExpired(Handle:timer,Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:time=ReadPackFloat(datapack); // Get type
  if(IsClientInGame(client))
  if(time==GetSealT(client))
  WCMessage(client,"seal expired",client);
}
public wcRunSpell_4_8(spellid,race,client) //Judgement(Judgment)
{
  new target=GetAimTarget(client,25.0);
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      if((GetSealT(client)>GetTickedTime()) && HasSeal(client,Seal:SEAL_Any))
      {
        decl String:line[255];
        wcGetSpellInfo(race,spellid,"cooldowntype",line);
        new cdtype=StringToInt(line);
        wcGetSpellInfo(race,spellid,"cooldown",line);
        new Float:cd=StringToFloat(line);
        new manareq = GetSpellCost(client, spellid);
        wcSetMana(client,GetMana(client)-manareq);
        SetCdSpell(client, cdtype, cd, spellid);

        new Float:location[3],Float:locationatt[3];
        GetClientAbsOrigin(target,location);
        GetClientAbsOrigin(client,locationatt);
        decl String:name[255],String:name2[255];
        GetClientName(client,name,sizeof(name));
        GetClientName(target,name2,sizeof(name2));
        wcGetTalentInfo(4,1,2,2,client,"multi",line);
        new Float:talentbonus=StringToFloat(line);

        wcGetSpellInfo(race,spellid,"judge",line);
        new Float:multi=StringToFloat(line);
        wcGetSpellInfo(race,spellid,"judgemulti",line);
        new power = GetHolyPower(client);
        //if(power>3)
        //power = 3;
        SetHolyPower(client,GetHolyPower(client)-power);
        multi+=StringToFloat(line)*(power*1.0);

        new dmg = GetSpellDmg(client, multi);
        wcGetTalentInfo(4,3,4,2,client,"multi",line);
        dmg+=RoundToNearest(GetDamage(client)*StringToFloat(line));
        dmg = RoundToNearest(dmg*(1+talentbonus));
        if(CheckRequirements(client,4,1,4,2))
        {
          wcGetTalentInfo(4,1,4,2,client,"multi",line);
          new heal = RoundToNearest(dmg*StringToFloat(line));
          heal=wcHeal(client,client,heal,0,true);
        }
          
        decl String:vicMsg[300], String:attMsg[300];
        Format(vicMsg,sizeof(vicMsg),"%T","judge command target", target);
        Format(attMsg,sizeof(attMsg),"%T","judge command caster", client);
        wcGetSpellInfo(4,6,"name",line);
        wcSpellDamage(target,client,dmg, attMsg, vicMsg,line);
        if(HasGlyph(client, GLYPH_PALADIN_JUDGEMENT)) {
          new Float:locationatt2[3], Float:location2[3];
          GetClientAbsOrigin(client,locationatt2);
          GetClientAbsOrigin(target,location2);
          new Float:locdis=GetDistanceBetween(locationatt2,location2);
          if(locdis >= 700)
          {
            new Float:multijud=1.20;
            new Float:normalspeed=GetNormalSpeed(client);
            new Float:newspeed=GetClientSpeed(client)*multijud;
            if(newspeed>normalspeed)
            {
              new Float:time=3.0;
              SetClientSpeed(client,newspeed);
              new Handle:datapack4=CreateDataPack();
              WritePackCell(datapack4,client); // Write the client index of owner
              WritePackFloat(datapack4,newspeed);
              WritePackFloat(datapack4,multijud);
              WritePackFloat(datapack4,normalspeed);
              CreateTimer(time,RemoveSpeedEffectJ,datapack4,TIMER_HNDL_CLOSE);
            }
          }
        }

        EmitAmbientSound("wc/spells/judge.wav",location);

        new bool:taunt = HasSeal(client,Seal:SEAL_Righteousness);

        SetSealT(client,0.0);
        SetSeal(client,0);

        new threatBonus = wcGetSpellInfoInt(race, spellid, "threat");
        //Threat
        //Check if it's protection paladin
        if(taunt && IsBoss() && (g_iAggro != client) && (GetBoss(client) <= 0))  {
          //No effect if top threat is caster
          new threat = GetThreat(g_iAggro) + threatBonus;
          SetThreat(client, threat);
        }
        //wcRunTalent_GiveItem(client, 4, 1, 4, 2);
        //wcRunTalent_GiveItem(client, 4, 2, 4, 2);

      }
      else
      WCMessage(client,"no seal for judge",client);
    }
    else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
  WCMessage(client,"no target",client);
}

public Action:DestroyMinResistance(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new round=ReadPackCell(datapack);
  new dmg=ReadPackCell(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (round!=g_RoundCount)
  {
    return Plugin_Stop;
  }
  AddResistance(client,float(dmg));
  return Plugin_Stop;
}

public Action:DestroyDemonForm(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new round=ReadPackCell(datapack);
  new armor=ReadPackCell(datapack);
  new res = ReadPackCell(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (round!=g_RoundCount)
  {
    return Plugin_Stop;
  }
  SetBonusArmor(client,float(GetBonusArmor(client))-float(armor));
  if(res>0)
  AddResistance(client, float(-res));
  wcRecalculateStats(client);
  WCMessage(client,"no longer demon",client);
  return Plugin_Stop;
}

public wcRunSpell_4_9(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  WCMessage(client,"kings cast",client);
  CastKings(client,multi);
}

stock bool:CastKings(client, Float:multi, bool:cast=false, bool:custom=false)  {
  if((!custom) && KingsClient[client])
  return false;
  SetBonusInt(client,GetBonusInt(client)+RoundToNearest(float(GetBonusInt(client)+GetInt(client))*multi));
  SetBonusSta(client,GetBonusSta(client)+RoundToNearest(float(GetBonusSta(client)+GetSta(client))*multi));
  SetBonusStr(client,GetBonusStr(client)+RoundToNearest(float(GetBonusStr(client)+GetStr(client))*multi));
  SetBonusAgi(client,GetBonusAgi(client)+RoundToNearest(float(GetBonusAgi(client)+GetAgi(client))*multi));
  wcRecalculateStats(client);
  RestoreHealthP(client,multi);
  RestoreManaP(client,multi);
  if(!custom)
  KingsClient[client]=true;
  if(cast) {
    WCMessage(client,"kings aff",client);
  }
  return true;
}

public wcRunSpell_4_10(spellid,race,client)
{
  decl String:line[255];
  wcGetTalentInfo(4,1,2,1,client,"multi",line);
  new Float:talentbonus=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  new bonusMana=GetBonusSta(client)+GetSta(client)+GetDamage(client);
  bonusMana = RoundToNearest(float(bonusMana)*(multi+talentbonus));
  WCMessage(client,"attunement cast",client,bonusMana);
  new Float:perc = float(GetMana(client)) / float(GetMaxMana(client));
  wcClientsBonusStats[client][MAXMP]+=bonusMana;
  wcRecalculateStats(client);
  new newMana=RoundToNearest(GetMaxMana(client) * perc);
  wcSetMana(client,newMana);
}

public Action:CastKingsAll(Handle:timer, any:client)  {
  if(!IsClientInGame(client))
  return;
  decl String:line[255];
  wcGetSpellInfo(4,9,"multi",line);
  new Float:multi=StringToFloat(line);
  WCMessage(client,"kings cast all",client);
  new clients[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
  amount=FindMatchingPlayers("@t",clients);
  if(team==3)
  amount=FindMatchingPlayers("@ct",clients);
  for(new i=0;i<amount;i++)  {
    if(IsClientInGame(clients[i]))
    CastKings(clients[i],multi,true);
  }
}


wcRunSpell_4_11(client)   {
  CreateTimer(0.5, CastKingsAll, client);
}


stock Float:GetBlockChance(client, bool:extra=true, attacker = 0) {
  if(!HasShield(client))
  return 0.0;
  new Float:chance = 0.10;
  if(!extra)
  return chance;
  new Float:talentbonus =0.0;
  decl String:line[255];
  if(GetClass(client)==1)
  {
    wcGetTalentInfo(1,1,1,1,client,"increment",line);
    talentbonus = StringToFloat(line);
  }
  else if(GetClass(client)==4)
  {
    wcGetTalentInfo(4,2,2,3,client,"increment",line);
    talentbonus = StringToFloat(line);
  }
  chance += talentbonus;
  if(GetAdrenaline(attacker))
  chance = 0.0;

  return chance;
}

KillEntityDelay(Float:time, entity) {
  CreateTimer(time, KillEntFunc, entity);
}

public Action:KillEntFunc(Handle:timer, any:entity) {
  KillEntity(entity);
  return Plugin_Stop;
}

bool:KillEntity(entity)   {
  if(entity!=0)  {
    if(IsValidEntity(entity)) {
      AcceptEntityInput(entity, "Kill");
      return true;
    }
  }
  return false;
}

WallEffect(target) {
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
    DispatchKeyValue(sprite, "rendercolor", "20 20 255");
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

BubbleEffect(target) {
  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
    //Give client a target name
    decl String:sTemp[30];
    Format(sTemp,sizeof(sTemp),"player_%i",target);
    DispatchKeyValue(target, "targetname", sTemp);


    DispatchKeyValue(sprite, "basespread", "50");
    DispatchKeyValue(sprite, "spreadspeed", "5");
    DispatchKeyValue(sprite, "speed", "150");
    DispatchKeyValue(sprite, "startsize", "30");
    DispatchKeyValue(sprite, "endsize", "31");
    DispatchKeyValue(sprite, "rate", "200");
    DispatchKeyValue(sprite, "jetlength", "20");
    DispatchKeyValue(sprite, "twist", "30");

    DispatchKeyValue(sprite, "angles", "90 120 90");
    DispatchKeyValue(sprite, "rendercolor", "235 255 0");
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

SpawnHymn(target)   {
  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
    DispatchKeyValue(sprite, "basespread", "35");
    DispatchKeyValue(sprite, "spreadspeed", "30");
    DispatchKeyValue(sprite, "speed", "60");
    DispatchKeyValue(sprite, "startsize", "5");
    DispatchKeyValue(sprite, "endsize", "1");
    DispatchKeyValue(sprite, "rate", "200");
    DispatchKeyValue(sprite, "jetlength", "175");
    DispatchKeyValue(sprite, "twist", "175");
    DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");
    DispatchKeyValue(sprite, "rendercolor", "100 100 220");
    DispatchSpawn(sprite);
    new Float:Client_Origin[3];
    GetClientAbsOrigin(target,Client_Origin);
    TeleportEntity(sprite, Client_Origin, NULL_VECTOR, NULL_VECTOR);
    AcceptEntityInput(sprite, "TurnOn");
    return sprite;
  }
  return 0;
}

SpawnHymn2(target)   {
  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
    DispatchKeyValue(sprite, "basespread", "135");
    DispatchKeyValue(sprite, "spreadspeed", "10");
    DispatchKeyValue(sprite, "speed", "10");
    DispatchKeyValue(sprite, "startsize", "5");
    DispatchKeyValue(sprite, "endsize", "11");
    DispatchKeyValue(sprite, "rate", "200");
    DispatchKeyValue(sprite, "jetlength", "100");
    DispatchKeyValue(sprite, "twist", "50");
    DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");
    DispatchKeyValue(sprite, "rendercolor", "100 100 220");
    DispatchSpawn(sprite);
    new Float:Client_Origin[3];
    GetClientAbsOrigin(target,Client_Origin);
    TeleportEntity(sprite, Client_Origin, NULL_VECTOR, NULL_VECTOR);
    AcceptEntityInput(sprite, "TurnOn");
    return sprite;
  }
  return 0;
}

SpawnHymn3(target)   {
  //TE effects
  decl Float:origin[3];
  GetClientAbsOrigin(target,origin);
  new Handle:datapack=CreateDataPack();
  WritePackFloat(datapack,origin[0]);
  WritePackFloat(datapack,origin[1]);
  WritePackFloat(datapack,origin[2]);
  CreateTimer(0.2,HymnTE,datapack);
  CreateTimer(0.4,HymnTE,datapack);
  CreateTimer(0.6,HymnTE,datapack);
  CreateTimer(0.8,HymnTE,datapack);
  CreateTimer(1.0,HymnTE,datapack);
  CreateTimer(1.2,HymnTE,datapack);
  CreateTimer(1.4,HymnTE,datapack);
  CreateTimer(1.6,HymnTE,datapack);
  CreateTimer(1.8,HymnTE,datapack);
  CreateTimer(2.0,HymnTE,datapack,TIMER_HNDL_CLOSE);
}

public Action:HymnTE(Handle:timer, Handle:pack)   {
  ResetPack(pack); // Set the reading position to the first cell
  decl Float:orig[3];
  orig[0] = ReadPackFloat(pack);
  orig[1] = ReadPackFloat(pack);
  orig[2] = ReadPackFloat(pack);
  new color[4] = { 245, 175, 255, 255 };
  orig[2]+=5;
  BeamRingEffect("@all",orig, 10.0, 500.0, g_ef_lgtning,0, 3, 2.0, 10.0, color,0.0,1);

}

SpawnHellfire(target)   {
  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
    DispatchKeyValue(sprite, "basespread", "100");
    DispatchKeyValue(sprite, "spreadspeed", "200");
    DispatchKeyValue(sprite, "speed", "22");
    DispatchKeyValue(sprite, "startsize", "40");
    DispatchKeyValue(sprite, "endsize", "40");
    DispatchKeyValue(sprite, "rate", "80");
    DispatchKeyValue(sprite, "jetlength", "100");
    DispatchKeyValue(sprite, "twist", "80");
    DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");
    DispatchKeyValue(sprite, "rendercolor", "255 128 100");
    DispatchKeyValue(sprite, "lifetime", "1.0");
    DispatchSpawn(sprite);
    new Float:Client_Origin[3];
    GetClientAbsOrigin(target,Client_Origin);
    Client_Origin[2] += 2.0;
    TeleportEntity(sprite, Client_Origin, NULL_VECTOR, NULL_VECTOR);
    AcceptEntityInput(sprite, "TurnOn");
    return sprite;
  }
  return 0;
}

SetCasting(client, spellId, Float:time)   {
  g_iCast[client]=spellId;
  g_fCast[client]=GetEngineTime()+time;
}

stock SetStealth(client, Float:time, bool:agressive=false) {
  g_fStealth[client]=GetEngineTime()+time;
  if(agressive) {
    ModifyStealth(client);
  }
}

bool:HasStealth(client) {
  if(GetClass(client)!=2)
  return false;
  return true;
}

GetStealthAlpha(client, Float:delta)  {
  new max = 0;
  new bool:knife=false;
  if(HasGlyph(client, GLYPH_ROGUE_STEALTH))
  delta*=1.25;
  decl String:line[255];
  SetElusive(client,false);
  if (prevDelta[client]>delta && CheckRequirements(client,2,1,3,3))
  {
    if(prevDelta[client]>9.0)
    prevDelta[client]=9.0;
    delta = (prevDelta[client]-delta)*1/2+delta;
  }
  if(CheckRequirements(client,2,1,1,4))
  {
    wcGetTalentInfo(2,1,1,4,client,"multi",line);
    delta*=(1+StringToFloat(line));
  }
  new alpha=255;
  if(delta<0.5)
  alpha=255;
  else if(delta<1.0)
  alpha=195;
  else if(delta<2.0)
  alpha=140;
  else if(delta<3.0)
  alpha=90;
  else if(delta<4.0)
  alpha=60;
  else if(delta<5.0)
  alpha=43;
  else if(delta<6.0)
  alpha=35;
  else if(delta<7.0)
  alpha=30;
  else if(delta<8.0)
  alpha=25;
  else
  {
    alpha=20;
  }
  if(alpha == 20 && delta > 9.0)
  {
    wcGetTalentInfo(2,1,1,1,client,"increment",line);
    new talentbonus=StringToInt(line);
    alpha = RoundToNearest(float(alpha)-(255/100*talentbonus));
  }
  if(CheckRequirements(client,2,1,3,3))
  prevDelta[client]=delta;
  if(!FindEffect(client, 14))
  alpha=255;
  new min=255;
  if(FindEffect(client, 3))
  min=122;


  new weap = GetEntDataEnt2(client, ACTIVEWEAPON);
  if(weap>=0)
  {
    decl String:S_weapon[50];
    GetEdictClassname(weap, S_weapon, sizeof(S_weapon));
    if(StrEqual("weapon_knife",S_weapon))
    {
      knife=true;
      if(CheckRequirements(client,2,1,2,3))
      {
        wcGetTalentInfo(2,1,2,3,client,"alpha",line);
        min = StringToInt(line);
        if(CheckRequirements(client,2,1,1,1))
        {
          wcGetTalentInfo(2,1,1,1,client,"alphai",line);
          min-= StringToInt(line);
        }
      }
    }
  }

  if(bVanish[client])
  min=0;
  max=CheckMaxStealth(client, knife);
  if(CheckRequirements(client,2,1,1,1))
  {
    wcGetTalentInfo(2,1,1,1,client,"alphai",line);
    max-=StringToInt(line);
  }

  //This is really misleading, at first I thought typo, then I though "Ohhhhh....." forgot opacity is backwards.
  if(min<alpha)
  alpha=min;
  if(max>alpha)
  alpha=max;

  if(alpha < 255 && alpha > 0 && !IsHNS())  SetElusive(client,true);
  return alpha;
}

CheckMaxStealth(client, bool:knife=false)
{
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  new team=GetClientTeam(client);
  new team2;

  new maxplayers=GetMaxClients();
  new Float:loc2[3];
  new Float:distance=255.0;
  new Float:distance2;

  for(new x=1;x<=maxplayers;x++)
  {
    if(!IsClientInGame(x))  continue;
    team2=GetClientTeam(x);
    if(IsClientInGame(x) && team2!=team)
    {
      if(IsPlayerAlive(x))
      {
        GetClientAbsOrigin(x,loc2);
        distance2=GetDistanceBetween(location,loc2);
        if(knife) distance2 += 50.0;

        if(distance2<=255.0)
        {
          //if(distance2<=distance && IsValidShooting(x,client) && !(GetFear(x)))
          if(distance2<=distance && GetTargetFOV(x,client,90.0)==1 && !(GetFear(x)))
          {
            distance=distance2;
          }
        }
      }
    }
  }
  return RoundToNearest(255.0-distance);
}


ModifyStealth(client) {
  new Float:delta=GetEngineTime()-g_fStealth[client];
  new color[4];
  for(new i=0;i<3;i++)
  color[i]=255;
  new alpha = GetStealthAlpha(client, delta);
  color[3]=alpha;
  SetClientColor(client, color);
  g_alpha[client]=alpha;
  return alpha;
}

stock GetStealth(client)  {
  return g_alpha[client];
}

Handle:GetSpellsHandle(race)  {
  switch(race)
  {
  case 1:
    return SpellsDB1;
  case 2:
    return SpellsDB2;
  case 3:
    return SpellsDB3;
  case 4:
    return SpellsDB4;
  case 5:
    return SpellsDB5;
  case 6:
    return SpellsDB6;
  case 7:
    return SpellsDB7;
  case 8:
    return SpellsDB8;
  case 9:
    return SpellsDB9;
  }
  LogError("Tried to get unavailable spells handle race = %d", race);
  return INVALID_HANDLE;
}

bool:SelfCast(client)   {
  if(wcUsers[client][253]==1)
  return false;
  return true;
}
bool:TriggerSelfCast(client)  {
  if(wcUsers[client][253]==1)   {
    wcUsers[client][253]=0;
    return true;
  }
  wcUsers[client][253]=1;
  return false;
}

bool:SelfOnly(client)   {
  if(wcUsers[client][257]==0)
  return false;
  return true;
}

bool:TriggerSelfOnly(client)  {
  if(wcUsers[client][257]==0)   {
    wcUsers[client][257]=1;
    return true;
  }
  wcUsers[client][257]=0;
  return false;
}

bool:CheckTargetOnly(client)
{
  if(wcUsers[client][258]==0)
  return false;
  return true;
}

bool:TriggerTargetOnly(client)
{
  if(wcUsers[client][258]==0)
  {
    wcUsers[client][258]=1;
    return true;
  }
  wcUsers[client][258]=0;
  return false;
}

ClearTotems(client)  {
  for(new i=0;i<10;i++)   {
    new ent = g_iTotems[client][i];
    g_iTotems[client][i]=-1;
    if(ent>0)   {
      KillTotem(ent);
    }
  }
}

stock bool:KillTotem(ent) {
  if(ent<=0)
  return false;
  if(IsValidEntity(ent)) {
    new String:clsname[60];
    if(GetEdictClassname(ent, clsname, sizeof(clsname)))   {
      if(StrEqual(clsname, "prop_physics", false))   {
        RemoveEdict(ent);
        return true;
      }
      //PrintToServer("probably totem: %s" , clsname);
    }
  }
  return false;
}

wcRunSpell_6_9(spellid,race,client) {
  if(CSGO)
  return;
  //Fire nova totem
  decl String:line[255];
  //CD and mana
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client, cdtype, cd, spellid);

  new manareq = GetSpellCost(client, spellid);

  wcGetTalentInfo(6,3,1,1,client,"multi",line);
  new Float:talentbonus=StringToFloat(line);
  manareq=RoundToNearest(manareq*(1-talentbonus));
  wcSetMana(client,GetMana(client)-manareq);

  //Getting values
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"dmg",line);
  new dmg=StringToInt(line);

  wcGetSpellInfo(race,spellid,"ticks",line);
  new ticks=StringToInt(line);
  if(HasGlyph(client, GLYPH_SHAMAN_NOVA)) {
    ticks = ticks / 2;
  }
  new hp=RoundToNearest(float(GetMaxHealth(client))*0.05);
  wcGetSpellInfo(race,spellid,"delay",line);
  new Float:delay=StringToFloat(line);
  ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
  delay = delay*(1.0-GetHasteEffect(client));

  wcGetSpellInfo(race,spellid,"aoe",line);
  new aoe=StringToInt(line);
  wcGetTalentInfo(6,1,1,3,client,"range",line);
  aoe+=StringToInt(line);

  dmg += GetSpellDmg(client, multi);

  decl Float:location[3];
  GetClientAbsOrigin(client,location);

  WCMessage(client,"totem nova cast",client,ticks*0.5);

  //Setting up effect
  new team=GetClientTeam(client);
  team = (team-1) % 2 + 2;
  new effid=CreateEffectData(client,spellid,dmg,0,aoe,team,ticks,0,g_RoundCount);
  if(effid!=0)
  {
    new iTotem = -1;
    if(GetClientTeam(client)==2)
    iTotem = SpawnTotem(location, true, hp);
    else
    iTotem = SpawnTotem(location, false, hp);
    if((effectsdata[effid][4]=iTotem) > 0) {
      if(TotemToPlayer(client,effectsdata[effid][4]))
      CreateTimer(delay, EffectNovaTotemAction0, effid);
      else  {
        KillTotem(effectsdata[effid][4]);
        DeleteEffectData(effid);
        LogError("ERROR: Could not attach totem to client");
      }
    }
    else  {
      LogError("ERROR: Failed spawning totem entity");
      DeleteEffectData(effid);
    }
  }
  //end

}

new bool:g_mageInv[65];
bool:IsMageInvis(client)  {
  return g_mageInv[client];
}

stock SetMageInvis(client, bool:value, bool:announce=false, bool:overlay=true)  {
  if(announce && g_mageInv[client] && (!value))  {
    WCMessage(client, "invis expired", client);
  }
  g_mageInv[client] = value;
  if(value) {
    //anti defuse
    AntiDefuse(client);

    if(overlay) {
      ClientCommand(client, "r_screenoverlay effects/tp_eyefx/tp_eyefx.vmt");
      sendshakemsg(client, all:SHAKE_START, 0.2, 1.0, 10.0);
    }
    new Float:position[3];
    GetClientAbsOrigin(client, position);
    position[2]+=40;
    VS_GlowSprite(position, g_ef_muzzleflashX, 1.00, 0.30, 255);
    VS_GlowSprite(position, g_ef_animglow, 1.00, 0.50, 255);
  }
  else  {
    ClientCommand(client, "r_screenoverlay 0");
    sendshakemsg(client, all:SHAKE_STOP, 0.2, 1.0, 10.0);
  }
}

stock bool:SpellExists(race, id)  {
  if(race <= 0)
  return false;
  new Handle:spells = GetSpellsHandle(race);
  KvRewind(spells);
  decl String:s_id[5];
  IntToString(id, s_id, sizeof(s_id));
  return (KvJumpToKey(spells, s_id));
}

//Auto cast spells library
stock bool:isAutoCast(race,spellid)  {
  //check if exists
  if(!SpellExists(race, spellid))
  return false;

  new auto = wcGetSpellInfoInt(race, spellid, "autocast");
  new active = wcGetSpellInfoInt(race, spellid, "active");
  if(auto<=0 || active<=0)
  return false;
  return true;
}

#define MAX_SPELLS 30
new bool:g_ff_autocasts[65][MAX_SPELLS+1];
new g_ff_iAutocasts[65][MAX_SPELLS];
stock bool:isAutoCastEnabled(client, id) {
  return g_ff_autocasts[client][id];
}
stock bool:setAutoCast(client, id, bool:val)  {
  g_ff_autocasts[client][id]=val;
  if(val) {
    g_ff_iAutocasts[client][id] = wcGetSpellInfoInt(GetClass(client), id, "chance");
  }
}
stock resetAutoCasts(client)  {
  for(new i=1; i <= MAX_SPELLS; i++) {
    setAutoCast(client, i, false);
    if(isAutoCast(GetClass(client), i)) {
      setAutoCast(client, i, true);
    }
  }
}
stock getAutoCastChance(client, id) {
  return g_ff_iAutocasts[client][id];
}

stock bool:triggerAutoCast(client, id, bool:msg=true) {
  if(isAutoCastEnabled(client, id)) {
    setAutoCast(client, id, false);
    if(msg) {
      decl String:spellname[255];
      wcGetSpellInfo(GetClass(client), id, "name", spellname, client);
      WCMessage(client, "auto cast off", client, spellname);
    }
    _addAutocast(client, id, true);
    return false;
  }
  else  {
    if(!isAutoCast(GetClass(client), id)) {
      WCMessage(client, "auto cast only off", client);
      return false;
    }
    setAutoCast(client, id, true);
    if(msg) {
      decl String:spellname[255];
      wcGetSpellInfo(GetClass(client), id, "name", spellname, client);
      WCMessage(client, "auto cast on", client, spellname);
    }
    _addAutocast(client, id, false);
    return true;
  }
}

shootAutoCasts(client, damage, victim)  {

  for(new i=1; i <= MAX_SPELLS; i++)  {
    if(isAutoCast(GetClass(client), i) && isAutoCastEnabled(client, i)) {

      new Float:chance=0.0;
      if(wcGetSpellInfoInt(GetClass(client), i, "version2")!=0)
      chance = 0.01 * float(damage) * 0.01 * float(getAutoCastChance(client, i));  //Normalize
      else
      chance = 1.0; //It will be checked later inside of the spell code

      if(chance >= GetRandomFloat(0.0, 1.0))  {
        //Make sure owner has spell (double check)
        TryCast(client, i, true, victim);
      }
    }
  }
}

bool:TryCast(client, spellid, bool:auto=false, victim=0)  {
  if(!IsPlayerAlive(client))  {
    WCMessage(client,"you are dead",client);
    return false;
  }

  if(wcSearchSpell(client,spellid)<=0) {
    if(!auto)
    WCMessage(client,"no spell",client);
    return false;
  }

  new active = wcGetSpellInfoInt(GetClass(client), spellid, "active");
  new iauto = wcGetSpellInfoInt(GetClass(client), spellid, "autocast");
  new bool:contr = (wcGetSpellInfoInt(GetClass(client), spellid, "controlled")!=0);

  if((!auto) && contr) {
    triggerControlledSpell(client, spellid);
    return true;
  }


  if(!active) {
    WCMessage(client,"spell inactive", client);
    return false;
  }

  if( iauto && (!auto) )  {
    triggerAutoCast(client, spellid);
    return true;
  }

  if( (GetClass(client) != 4) || ( spellid != 2 )  )  if(GetSilence(client) > GetTickedTime())  {
    WCMessage(client,"silenced for",client,(GetSilence(client)-GetTickedTime()));
    return false;
  }

  //making sure everything works as intended
  if(auto && (!iauto))  {
    LogError("[WC] Error sp.3928: id %d class %d", spellid, GetClass(client));
    return false;
  }

  new race=GetClass(client);
  new type = wcGetSpellInfoInt(race, spellid, "cooldowntype");
  new Float:timeleft=GetCDSpell(client,type)-GetTickedTime();
  if(timeleft > 0.0)  {
    if(!auto)
    WCMessage(client,"spell on cd",client,timeleft);
    return false;
  }

  new mana=GetMana(client),tmp=0;
  new manareq = GetSpellCost(client, spellid);
  if(mana < manareq)  {
    if(!auto)
    WCMessage(client,"no mana",client);
    return false;
  }

  SetCasting(client,spellid,0.0);

  if(auto)
  wcRunSpell(spellid,race,client,victim,tmp,tmp,tmp,tmp,tmp,tmp,tmp,client);
  else
  wcRunSpell(spellid,race,client,tmp,tmp,tmp,tmp,tmp,tmp,tmp,tmp,client);

  return true;
}

new g_ff_redirection_to[65];
new g_ff_redirection_amount[65];
GetRedirection(client, &redirectTo) {
  redirectTo = g_ff_redirection_to[client];
  return g_ff_redirection_amount[client];
}

SetRedirection(client, redirectTo, value) {
  g_ff_redirection_to[client] = redirectTo;
  g_ff_redirection_amount[client] = value;
}

_findLearnableSpell(client, String:name[60]) {
  new race = GetClass(client);
  if(race<= 0 )
  return 0;
  new Handle:SpellsDB = GetSpellsHandle(race);
  KvRewind(SpellsDB);
  if (!KvGotoFirstSubKey(SpellsDB))
  {
    LogError("[WC] Error: failed to open Spells database");
  }
  do
  {
    decl String:buffer[50];
    KvGetSectionName(SpellsDB, buffer, sizeof(buffer));
    new spellid=StringToInt(buffer);
    KvGetString(SpellsDB, "name", name, sizeof(name), "0");
    new ilevel=KvGetNum(SpellsDB, "level", 0);
    if(ilevel>GetLevel(client))
    continue;
    new foundspell=wcSearchSpell(client,spellid);
    if(foundspell>0)
    continue;
    if(KvGetNum(SpellsDB,"buyable",0))
    continue;
    //can lernz it!
    return spellid;
  }  while (KvGotoNextKey(SpellsDB));
  return 0;
}

learnSpell(client, String:spellName[60])
{
  new found = _findLearnableSpell(client, spellName);
  if(found<=0)
  return 0;

  wcSetSpell(client,found);
  return found;
}

LearnUnlearnedSpell(client)
{
  new race = GetClass(client);
  if(race<= 0 )
  return 0;
  new Handle:SpellsDB = GetSpellsHandle(race);
  KvRewind(SpellsDB);
  if (!KvGotoFirstSubKey(SpellsDB))
  {
    LogError("[WC] Error: failed to open Spells database");
  }
  do
  {
    decl String:buffer[50];
    decl String:name[60];
    KvGetSectionName(SpellsDB, buffer, sizeof(buffer));
    new spellid=StringToInt(buffer);
    KvGetString(SpellsDB, "name", name, sizeof(name), "0");
    new ilevel=KvGetNum(SpellsDB, "level", 0);
    if(ilevel>GetLevel(client))
    continue;
    new foundspell=wcSearchSpell(client,spellid);
    if(foundspell>0)
    continue;
    if(KvGetNum(SpellsDB,"buyable",0))
    continue;
    //can lernz it!
    wcSetSpell(client,spellid);
  }  while (KvGotoNextKey(SpellsDB));
  return 0;
}

public wcGetSpellIDFromName(class,String:spellname[])
{
  decl String:line[255];
  for(new spell=1;spell<20;spell++)
  {
    wcGetSpellInfo(class,spell,"name",line);
    if(!strcmp(spellname,line))
    return spell;
    wcGetSpellInfo(class,spell,"name_ru",line);
    if(!strcmp(spellname,line))
    return spell;
  }
  return 0;
}

stock GenerateRage(client,damage, bool:victim=false)
{
  new mana = GetMana(client);
  new maxmana = GetMaxMana(client);
  new regen = RoundToNearest(float(damage)/4.0);
  if(victim)
  regen = RoundToNearest(float(regen)*0.6);
  else if(HasShield(client))
  regen = RoundToNearest(float(regen)*0.8);
  if(regen<0)
  regen=0;
  if(CheckRequirements(client,1,3,1,4))
  {
    decl String:line[255];
    wcGetTalentInfo(1,3,1,4,client,"limit",line);
    if(mana<=StringToInt(line))
    {
      wcGetTalentInfo(1,3,1,3,client,"multi",line);
      regen = RoundToNearest(regen*(StringToFloat(line)+1));
    }
  }
  if(CheckRequirements(client,1,3,3,3)) {
    decl String:line[255];
    wcGetTalentInfo(1,3,3,3,client,"increment", line);
    new increment = StringToInt(line);
    if(increment > 0) {
      regen = RoundToNearest(regen * (1.0 + float(increment)/100.0));
    }
  }
  mana+=regen;
  if(mana>maxmana)
  mana=maxmana;
  wcSetMana(client,mana);
}

SetCombat(client,amount)
{
  g_iCombatTimer[client]=amount;
}

GetCombat(client)
{
  return g_iCombatTimer[client];
}

stock bool:InCombat(client)  {
  return (GetCombat(client)>0);
}

GetHolyPower(client)
{
  return g_iHolyPower[client];
}

SetHolyPower(client, amount)
{
  if(!IsClientInGame(client) || !IsPlayerAlive(client))
    return;
  if(GetClass(client)==4) {
    if(amount !=0) {
      //Try cast word of glory (healing)
      if(wcSearchSpell(client,17)>0) {
        new tmp = 0;
        wcRunSpell(17,4,client,tmp,tmp,tmp,tmp,tmp,tmp,tmp,tmp,client);
      }
      if(amount < g_iHolyPower[client] && CheckRequirements(client,4,3,2,3))
        wcRunTalent_4_RighteusWarrior(client, (g_iHolyPower[client]-amount));
    }
  }

  if(amount > 5)
    amount = 5;
  if(amount>=0)
    g_iHolyPower[client]=amount;
}

FindRandomSeal(client)
{
  new holy = GetRandomInt(1,20)+wcGetTreeTotal(client, getActiveSpec(client),1);
  new protection = GetRandomInt(1,20)+wcGetTreeTotal(client, getActiveSpec(client),2);
  new retribution = GetRandomInt(1,20)+wcGetTreeTotal(client, getActiveSpec(client),3);
  new random = GetRandomInt(1,2);
  new seal =0;
  if(holy > protection && holy > retribution)
  {
    if(GetLevel(client)>=50 && random==1)
    {
      seal=15;
    }
    else
    {
      seal=14;
    }
  }
  else if(protection > holy && protection > retribution)
  {
    if(GetLevel(client)>=50 && random==1)
    {
      seal=13;
    }
    else
    {
      seal=12;
    }
  }
  else if(retribution > holy && retribution > protection)
  {
    if(GetLevel(client)>=50 && random==1)
    {
      seal=6;
    }
    else
    {
      seal=7;
    }
  }
  else
  {
    seal = FindRandomSeal(client);
  }
  return seal;
}

RunSealsHit(attacker,victim,&damage,prio,Float:weaponmodifier,bool:crit, bool:spell, bool:bonus=false)
{
  decl String:line[255];
  if(GetSealT(attacker)>GetTickedTime() && HasSeal(attacker,Seal:SEAL_Command) && IsDoubleHand(attacker) && prio == 1 && (!bonus))
  {
    wcGetSpellInfo(4,6,"chance",line);
    new chance=StringToInt(line);
    if(GetRandomInt(1,100)<=chance)
    {
      wcGetSpellInfo(4,6,"base",line);
      new Float:multi=StringToFloat(line);

      new dmg2=RoundToNearest(float(GetDamage(attacker))*multi*weaponmodifier);

      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","seal of command activated", victim);
      Format(attMsg,sizeof(attMsg),"%T","seal of command activated", attacker);
      wcGetTalentInfo(4,3,3,3,attacker,"chance",line);
      chance = StringToInt(line);
      if(chance >= GetRandomInt(1,100))
      {
        wcGetTalentInfo(4,3,3,3,attacker,"distance",line);
        new Float:distance=StringToFloat(line);
        new targets[MAXPLAYERS];
        new team=GetClientTeam(attacker);
        new amount;
        new Float:location[3];
        GetClientAbsOrigin(victim,location);
        if(team==2)
        amount=GetNearClients("@ct",location,targets,distance);
        if(team==3)
        amount=GetNearClients("@t",location,targets,distance);

        decl String:name[255],String:name2[255];
        GetClientName(attacker,name,sizeof(name));
        new String:stargets[255];
        for (new x=0;x<=(amount-1);x++)
        {
          GetClientName(targets[x],name2,sizeof(name2));
          if(x>0)
          Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
          else
          Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
        }
        WCMessage(attacker,"seal of command aoe",attacker,stargets,dmg2);
        wcGetSpellInfo(4,6,"name",line);
        for (new x=0;x<=(amount-1);x++)
        {
          if(HasGlyph(attacker, GLYPH_PALADIN_SOC))
          {
            Format(vicMsg,sizeof(vicMsg),"%T","seal of command activated", targets[x]);
            wcSpellDamage(targets[x],attacker,dmg2, "", vicMsg,line);
          }
          else
          {
            Format(vicMsg,sizeof(vicMsg),"%T","seal of command activated", targets[x]);
            wcPhysicalDamage(targets[x],attacker,dmg2, "", vicMsg,true,line);
          }
          BloodSpray(targets[x]);
        }
      }
      else
      {
        wcGetSpellInfo(4,6,"name",line);
        if(HasGlyph(attacker, GLYPH_PALADIN_SOC))
        {
          wcSpellDamage(victim,attacker,dmg2, attMsg, vicMsg, line);
        }
        else
        {
          wcPhysicalDamage(victim,attacker,dmg2, attMsg, vicMsg, true,line);
        }
      }
    }
  }
  if(GetSealT(victim)>GetTickedTime() && HasSeal(victim,Seal:SEAL_Justice) && prio == 2)
  {
    wcGetSpellInfo(4,12,"base",line);
    new Float:multi=StringToFloat(line);
    damage=RoundToNearest(damage*(1.0-multi));
  }
  if(GetSealT(victim)>GetTickedTime() && HasSeal(victim,Seal:SEAL_Righteousness) && prio == 1)
  {
    wcGetSpellInfo(4,13,"base",line);
    new Float:multi=StringToFloat(line);
    new heal = RoundToNearest(GetMaxHealth(victim)*multi*weaponmodifier);
    wcHeal(victim,victim,heal,13,false); //spellid 13
  }
  if(GetSealT(attacker)>GetTickedTime() && HasSeal(attacker,Seal:SEAL_Wisdom) && prio == 1 && (!bonus))
  {
    wcGetSpellInfo(4,15,"base",line);
    new Float:multi=StringToFloat(line);
    new mana = RoundToNearest(GetMaxMana(attacker)*multi*weaponmodifier) +GetMana(attacker);
    if(mana>GetMaxMana(attacker))
    mana=GetMaxMana(attacker);
    wcSetMana(attacker,mana);
  }
  if(GetSealT(attacker)>GetTickedTime() && HasSeal(attacker,Seal:SEAL_Crusader) && prio == 1 && (!bonus))
  {
    new chance = 30;//+10*GetSealP(attacker);
    new random = GetRandomInt(1,100);
    if (chance >= random)
    {
      new weapon = GetEntDataEnt2(attacker, ACTIVEWEAPON);
      if(IsValidEntity(weapon)) {
        new ammo = GetEntData(weapon, CLIPOFFSET, 4);
        if(ammo>3)
        {
          decl String:Sweapon[50];
          GetEdictClassname(weapon, Sweapon, sizeof(Sweapon));

          if((!StrEqual("weapon_xm1014",Sweapon))&&(!StrEqual("weapon_m3",Sweapon))&&(!StrEqual("weapon_c4",Sweapon)))
          {
            ammo += 1;
            SetEntData(weapon, CLIPOFFSET, ammo, 4, true);
          }
        }
      }
    }
  }
  if(GetSealT(victim)>GetTickedTime() && HasSeal(victim,Seal:SEAL_Truth) && prio == 2 && spell)
  {
    wcGetSpellInfo(4,19,"chance",line);
    new chance=StringToInt(line);
    new random = GetRandomInt(1,100);
    if(chance>=random)
    {
      new mana = damage+GetMana(attacker);
      if(mana>GetMaxMana(victim))
      mana=GetMaxMana(victim);
      wcSetMana(attacker,mana);
      damage=0;
      AddEMsgAff("seal of truth", true);
    }
  }
  if(GetSealT(attacker)>GetTickedTime() && HasSeal(attacker,Seal:SEAL_Blood) && prio == 2 && crit && (!bonus))
  {
    wcGetSpellInfo(4,20,"multi",line);
    new Float:multi=StringToFloat(line);
    damage=RoundToNearest(damage*multi);
  }
}


stock Renew(caster, target, heal, ticks, cancrit, Float:delay) {
  new effid = CreateEffectData(caster, target, ticks, heal, cancrit, 0, 0, 0, g_RoundCount);
  if(effid!=0)
  {
    CreateTimer(delay, EffectRenewAction, effid, TIMER_REPEAT);
  }
}

//Polymorph control
new Float:g_ffPoly[65];
bool:IsPolymorphed(client)  {
  return (GetEngineTime() < g_ffPoly[client]);
}

setPolymorph(client, Float:duration)  {
  g_ffPoly[client]=GetEngineTime() + duration;
  if(duration > 0.0)  {
    SetSilence(client, duration, true);
    InitPolymorph(client);
  }
}

new String:g_ffOldModel[65][255];

InitPolymorph(client) {
  //TODO set model or color
  decl String:model[255];
  GetClientModel(client, model, 255);
  if(StrContains(model, "chicken")==-1)  {
    strcopy(g_ffOldModel[client],255,model);
    CreateTimer(0.2, PolyTick, client, TIMER_REPEAT);
    if(CSGO)
    SetEntityModel(client,"models/chicken/chicken.mdl");
    else  {
      SetEntityModel(client,"models/lduke/chicken/chicken3.mdl");
    }
  }
}

public Action:PolyTick(Handle:timer, any:client) {
  if(!IsClientInGame(client) || !IsPlayerAlive(client)) {
    TurnPolyOff(client);
    return Plugin_Stop;
  }

  new Float:left = g_ffPoly[client] - GetEngineTime();
  if(left < 0.15) {
    TurnPolyOff(client);
    return Plugin_Stop;
  }


  //Poly regen tick
  new heal = RoundToNearest(GetMaxHealth(client) * 0.04);
  wcHeal(client, client, heal,0,false);

  //Poly random move
  new Float:angel=GetRandomFloat(-1.0,1.0);
  new Float:velocity[3];
  velocity[0]=100.0*angel;
  new Float:angel2=Sine(ArcCosine(angel));
  new temp=GetRandomInt(1,2);
  if(temp==2)
  angel2*=-1;
  velocity[1]=100.0*angel2;
  velocity[2]=100.0;
  TeleportEntity(client,NULL_VECTOR,NULL_VECTOR,velocity);


  return Plugin_Continue;
}

stock SetPolymorphOff(client) {

  if(IsClientConnected(client) && IsPlayerAlive(client) && IsPolymorphed(client)) {
    TurnPolyOff(client);
    SetSilence(client, 0.0);
  }
}

TurnPolyOff(client) {
  setPolymorph(client, 0.0);

  if(IsClientConnected(client) && IsPlayerAlive(client)) {
    decl String:model[255];
    GetClientModel(client, model, 255);
    if(StrContains(model, "chicken")>=0)  {
      SetEntityModel(client,g_ffOldModel[client]);
    }
  }
}

new g_iceblocks[65];

public Action:DestroyIceBlock(Handle:timer, any:client)
{
  if(UnableToVictim[client])
  {//if not true, the message was already sent
    WCMessage(client,"iceblock expired",client);
    UnableToVictim[client]=false;
  }
}

GetIceBlocks(client)
{
  return g_iceblocks[client];
}

SetIceBlocks(client, amount)
{
  g_iceblocks[client]=amount;
}

bool:SpendIceBlocks(client)
{
  if(GetClass(client)!=3)
    return false;
  if(GetIceBlocks(client)<=0)
  return false;
  SetIceBlocks(client, GetIceBlocks(client)-1);
  return true;
}


public wcRunSpell_3_11(spellid,race,client,attacker, &totaldmg)
{
  if(IsBoss() && (GetBoss(client) > 0))
  return totaldmg;
  if(totaldmg < wcGetHealth(client))
  return totaldmg;

  //CD check
  new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:timeleft=GetCDSpell(client,cdtype)-GetTickedTime();
  if(timeleft > 0.0)
  return totaldmg;

  //Cooldown
  new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");

  SetCdSpell(client, cdtype, cd, spellid);
  totaldmg = 0; //absorb dmg
  //Set 'divine shield' for 1 second
  new Float:duration = wcGetSpellInfoFloat(race,spellid,"duration");

  //setting up the anti attack
  UnableToVictim[client]=true;
  SetIceBlocks(client, 3);

  // removing anti victim
  new effid=CreateEffectData(client,2,2,0,0,0,0,0,g_RoundCount);
  if(effid!=0)
  {
    CreateTimer(duration/3.0, EffectIceBlock, effid, TIMER_REPEAT);
  }

  //remove negative effects
  SetSilence(client, 0.0);
  RemoveFear(client);

  WCMessage(client,"iceblock cast",client,duration);

  //Put hp to 10%
  wcSetHealth(client, RoundToNearest(GetMaxHealth(client)*0.1));


  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/iceblock.wav",location);


  AddEMsgAff("ice block", true);

  //TODO: visual effect

  //Ice Block Glyph
  if(HasGlyph(client, GLYPH_MAGE_NOVA))  {
    SetCdSpell(client, 4, 0.0, 0);
  }

  return totaldmg;
}

public wcRunSpell_3_12(spellid,race,client)
{
  if(!IsClientInGame(client))
  return;


  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq=GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);

  SetCdSpell(client, cdtype, cd, spellid);


  new Float:multi = wcGetSpellInfoFloat(race, spellid, "multi");
  new Float:range = wcGetSpellInfoFloat(race, spellid, "range");
  new Float:delay = wcGetSpellInfoFloat(race, spellid, "delay");
  new dmg = wcGetSpellInfoInt(race, spellid, "basedmg");
  dmg += GetSpellDmg(client, multi);


  new Float:distance = wcGetSpellInfoFloat(race, spellid, "distance");
  new steps = wcGetSpellInfoInt(race, spellid, "steps");

  new iSteps;
  new Float:points[3];
  for(iSteps=0;iSteps<steps;iSteps++) {
    if(!GetViewPoint(client, distance / 12 * (iSteps+1), points))
    break;
    CreateOrb(delay*(iSteps+1), client, dmg, range, points);

  }

  new Float:location[3];
  GetClientEyePosition(client, location);
  CreateOrb(0.01, client, dmg, range, location);
  WCMessage(client, "flame orb", client, dmg);
  EmitAmbientSound("wc/spells/flameorb.wav",location);
}

stock CreateOrb(Float:delay, client, dmg, Float:range, Float:location[3])  {
  new Handle:pack = CreateDataPack();
  WritePackCell(pack, client);
  WritePackCell(pack, g_respawns[client]);
  WritePackCell(pack, dmg);
  WritePackFloat(pack, range);
  WritePackFloat(pack, location[0]);
  WritePackFloat(pack, location[1]);
  WritePackFloat(pack, location[2]);

  CreateTimer(delay,TimedOrb,pack,TIMER_HNDL_CLOSE);
}


public Action:TimedOrb(Handle:timer,Handle:pack) {
  ResetPack(pack);
  new client = ReadPackCell(pack);
  new spawn = ReadPackCell(pack);
  new dmg = ReadPackCell(pack);
  new Float:range = ReadPackFloat(pack);
  new Float:location[3];
  location[0] = ReadPackFloat(pack);
  location[1] = ReadPackFloat(pack);
  location[2] = ReadPackFloat(pack);

  VS_GlowSprite(location, g_ef_combineball, 0.50, 2.00, 255);
  VS_GlowSprite(location, g_ef_orangeflare1, 0.50, 2.00, 255);

  if(!IsClientInGame(client))
  return Plugin_Stop;
  if(spawn != g_respawns[client])
  return Plugin_Stop;

  //AOE dmg
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  ClientTracer = client;
  if(team==2)
  amount=GetNearClients("@ct",location,targets,range,false);
  if(team==3)
  amount=GetNearClients("@t",location,targets,range,false);

  EmitAmbientSound("wc/spells/flameorb2.wav",location);
  for (new x=0;x<=(amount-1);x++)
  {
    decl String:vicMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","flameorb target", targets[x]);
    wcSpellDamage(targets[x],client,dmg, "", vicMsg,"Flame Orb");
    decl Float:loc[3];
    GetClientAbsOrigin(targets[x], loc);
    loc[2]+=40;

    VS_BeamPoints(location, loc, g_ef_orangelight1, g_ef_orangelight1, 0, 0, 0.50, 6.00, 6.00, 0, 0.00, {255, 255, 255, 255}, 160);

    BloodSpray(targets[x]);

    if(!HasGlyph(client, GLYPH_MAGE_ORB))
    continue;
    //Slodown effect
    new Float:oldspeed=GetClientSpeed(targets[x]);
    new Float:newspeed=GetNormalSpeed(targets[x]);
    newspeed*=0.8;
    if(newspeed<oldspeed)
    {
      SetClientSpeed(targets[x],newspeed);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,targets[x]); // Write the client index of owner
      WritePackFloat(datapack2,newspeed);
      WritePackFloat(datapack2,0.8);
      CreateTimer(0.48,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
    }

  }
  return Plugin_Stop;

}


public wcRunSpell_2_12(spellid,race,client) //Blind
{
  new target=GetAimTarget(client,15.0);
  if(target<=0) {
    if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
    return;
  }
  if(((IsBoss() || IsHNS()) && (GetBoss(target) > 0)) )  {
    //disable for boss
    return;
  }
  if((GetClientTeam(client)==GetClientTeam(target)) || (!IsPlayerAlive(target)) ) {
    if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
    return;
  }


  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetTalentInfo(2,1,3,2,client,"cdreduction",line);
  cd-=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcGetTalentInfo(2,1,3,2,client,"redmana",line);
  manareq-=StringToInt(line);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);


  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(target, name2, sizeof(name2));
  wcGetSpellInfo(race,spellid,"tickdelay",line);
  new Float:tickdelay=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"ticks",line);
  new ticks=StringToInt(line);
  wcGetSpellInfo(race,spellid,"power",line);
  new power=StringToInt(line);
  WCMessage(target,"blind target",target,name);

  if(!UnableToAttack[target]) {


    new effid=CreateEffectData(target,client,ticks,power,0,0,0,race,g_RoundCount);
    new color2[4]={203,55,149,255};
    FadeEffect(target,2,300,50,color2);
    new Float:dur=tickdelay*float(ticks);
    if(effid!=0)
    {
      SetFear(target,effid);
      SetSilence(target,dur,true);
      CreateTimer(tickdelay, EffectFearAction, effid, TIMER_REPEAT);
    }
  }

  WCMessage(client,"blind caster",client,name2);

  new Float:location[3];
  GetClientAbsOrigin(target, location);

  EmitAmbientSound("wc/spells/blind.wav",location);

  VS_BeamRingPoint(location, 5.00, 300.00, g_ef_scanner, g_ef_bluelight, 10, 30, 1.00, 50.00, 0.20, {255, 255, 255, 255}, 3, 0);
}

public wcRunSpell_2_13(spellid,race,client)
{
  {
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetSpellInfo(race,spellid,"cooldown",line);
    new Float:cd=StringToFloat(line);
    new manareq=GetSpellCost(client, spellid);
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client, cdtype, cd, spellid);


    new Float:location[3];
    GetClientAbsOrigin(client,location);
    wcGetSpellInfo(race,spellid,"distance",line);
    new Float:distance=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"basedmg",line);
    new basedmg=StringToInt(line);
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
    new dmg=RoundToNearest(GetDamage(client)*multi)+basedmg;

    wcGetTalentInfo(2,2,4,2,client,"multi",line);
    new Float:multi2 = StringToFloat(line);
    if(multi2 > 1.0)
    dmg = RoundToNearest(multi2 * dmg);
    dmg=RoundToNearest(float(dmg)/GetWeaponSpeed(client, false));

    for (new x=0;x<=(amount-1);x++)
    {
      GetClientName(targets[x],name2,sizeof(name2));
      if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
      //WCMessage(targets[x],"arcexpl target",targets[x],name,dmg);
    }
    if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);
    WCMessage(client,"fanofknives",client,stargets,dmg);
    EmitAmbientSound("wc/spells/fanofknives.wav",location);
    CreateTimer(0.0,FanOfKnivesEffect,client);
    CreateTimer(0.1,FanOfKnivesEffect,client);
    for (new x=0;x<=(amount-1);x++)
    {
      decl String:vicMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","fanofknives target", targets[x]);
      wcPhysicalDamage(targets[x],client,dmg, "", vicMsg, true);
      BloodSpray(targets[x]);
      bVanish[client]=false;
      if(HasGlyph(client, GLYPH_ROGUE_FAN))  {
        Effect4(targets[x],client,true);
      }
    }
  }
}

public Action:FanOfKnivesEffect(Handle:timer, any:client)
{
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  decl Float:location[3];
  //new color[4]={104,104,104,170};
  GetClientAbsOrigin(client,location);
  location[2]+=40;
  VS_Bubbles(g_mRanks[5], location, location, 75.00, 25, 350.00);
  VS_BeamRingPoint(location, 20.00, 300.00, g_ef_trail_blue, g_ef_trail_blue, 0, 0, 0.50, 3.00, 0.00, {205, 205, 255, 255}, 5, 0);
  VS_BeamRingPoint(location, 20.00, 250.00, g_ef_trail_blue, g_ef_trail_blue, 0, 0, 0.50, 3.00, 0.00, {205, 205, 255, 255}, 5, 0);
  VS_BeamRingPoint(location, 20.00, 300.00, g_ef_trail_blue, g_ef_trail_blue, 0, 0, 0.50, 3.00, 0.00, {255, 25, 25, 255}, 5, 0);

  return Plugin_Stop;
}

public wcRunSpell_1_15(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, spellid);

  new Float:power = wcGetSpellInfoFloat(race, spellid, "power");
  new Float:zPower = wcGetSpellInfoFloat(race, spellid, "zpower");

  HookLand(client);

  PushPlayerView(client, power, zPower);
  if(HasGlyph(client, GLYPH_WARRIOR_LEAP))  {

    wcGetSpellInfo(race,1,"cooldown",line);
    cd = StringToFloat(line);
    wcGetSpellInfo(race,spellid,"glyph",line);
    cd *= (1.0-StringToFloat(line));
    SetCdSpell(client, 1, GetCDSpell(client, 1) - GetTickedTime() -cd, 2); //Whirlwind cd reduced
  }
}

new bool:g_ff_hookland[65];
stock HookLand(client)  {
  g_ff_hookland[client]=true;
}

stock UnhookLand(client)  {
  g_ff_hookland[client]=false;
}


public OnThinkPostClient(client)
{

}


stock LeapLand(client)  {
  new race = 1;
  new spellid = 15;
  decl String:line[255];
  //Damage code
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetSpellInfo(race,spellid,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  new dmg=RoundToNearest(float(GetDamage(client))*multi);
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

  new color[4]={55,55,255,55};
  BeamRingEffect("@all",location,10.0,40.0,g_ef_lgtning,0,10,3.0,100.0,color,0.0,20);
  color[3]=255;
  BeamRingEffect("@all",location,50.0,350.0,g_ef_lgtning,0,10,3.0,50.0,color,0.0,10);
  WCMessage(client,"heroic leap caster",client,stargets,dmg);
  EmitAmbientSound("wc/spells/thunderclap.wav",location);

  for (new x=0;x<=(amount-1);x++)
  {
    decl String:vicMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","heroic leap victim", targets[x]);
    wcGetSpellInfo(race,spellid,"name",line);
    wcSpellDamage(targets[x],client,dmg, "", vicMsg,line);
    BloodSpray(targets[x]);
  }
}

public wcRunSpell_4_17(spellid,race,client) //Word of Glory
{
  new target=client;
  if(wcGetHealth(target)>=GetMaxHealth(target))
  return 0;
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  new heal=GetSpellDmg(client,multi);
  wcGetSpellInfo(race,spellid,"base",line);
  new base=StringToInt(line);
  heal+=base;
  heal=wcHeal(client,target,heal,spellid,true);
  WCMessage(client, "Word of Glory Cast", client, heal);
  return 1;
}

public wcRunSpell_4_18(spellid,race,client) //Holy Radiance
{
  {
    decl String:line[255];
    wcGetSpellInfo(race,spellid,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetSpellInfo(race,spellid,"cooldown",line);
    new Float:cd=StringToFloat(line);
    new manareq = GetSpellCost(client, spellid);
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client, cdtype, cd, spellid);
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    wcGetSpellInfo(race,spellid,"distance",line);
    new Float:distance=StringToFloat(line);
    wcGetSpellInfo(race,spellid,"multi",line);
    new Float:multi=StringToFloat(line);
    new targets[MAXPLAYERS];
    new team=GetClientTeam(client);
    new amount;
    if(team==3)
    amount=GetNearClients("@ct",location,targets,distance);
    if(team==2)
    amount=GetNearClients("@t",location,targets,distance);
    decl String:name[255],String:name2[255];
    GetClientName(client,name,sizeof(name));
    new String:stargets[255];
    new heal=GetSpellDmg(client,multi);
    wcGetSpellInfo(race,spellid,"base",line);
    new base=StringToInt(line);
    heal+=base;
    for (new x=0;x<=(amount-1);x++)
    {
      GetClientName(targets[x],name2,sizeof(name2));
      if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
      new healed=wcHeal(client,targets[x],heal,spellid,true);
      if(targets[x]!=client)
      {
        WCMessage(targets[x],"holy radiance target",targets[x],name,healed);
      }
      if(HasGlyph(client, GLYPH_PALADIN_RADIANCE))  {
        GiveDamageBuff(client, 1.1, 3.0);
      }
    }
    if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);
    location[2]+=50;

    VS_GlowSprite(location, g_ef_yellowglow1, 1.00, 2.00, 25);
    VS_GlowSprite(location, g_ef_WXplo1, 1.00, 1.00, 255);

    WCMessage(client,"holy radiance caster",client,stargets,heal);
    EmitAmbientSound("wc/spells/heal.wav",location);
  }
}

public wcRunSpell_5_10(spellid,race,client)
{
  decl String:line[255];
  wcGetSpellInfo(race,spellid,"ms",line);
  new Float:multi=StringToFloat(line);
  new Float:oldspeed=GetClientSpeed(client);
  new Float:newspeed=GetNormalSpeed(client);
  newspeed*=1.0+multi;

  SetNormalBonusSpeed(client, GetNormalBonusSpeed(client)+multi);
  WCMessage(client, "inner will cast1", client, (newspeed-oldspeed)*100,newspeed*100);
  WCMessage(client, "inner will cast2", client);
}

public wcRunSpell_5_11(spellid, race, client)    //Fade - mage invis copy
{
  //setting up cooldown and -mana
  new cdtype=wcGetSpellInfoInt(race, spellid, "cooldowntype");
  new Float:cd=wcGetSpellInfoFloat(race, spellid, "cooldown");
  new manareq = GetSpellCost(client, spellid);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 11);
  new Float:duration=wcGetSpellInfoFloat(race, spellid, "duration");

  CreateTimer(duration,DestroyMageInvis,client);


  if(Defuser==client)
  {
    new bombent = FindEntityByClassname(-1,"planted_c4");
    if (bombent>-1)
    {

      WCMessage(client, "cant defuse", client);
      CreateTimer(0.2,DefuseDelay,0);
    }
  }
  SetMageInvis(client,true, false, false);  //No overlay effects
  AddThreat(client,wcGetSpellInfoInt(race, spellid, "threat"));
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/invisibility.wav",location);
  WCMessage(client,"fade",client,duration);
}


wcRunSpell_6_10(spellid,race,client) {
  //Earthbind totem
  if(CSGO)
  return;
  decl String:line[255];
  //CD and mana
  wcGetSpellInfo(race,spellid,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetSpellInfo(race,spellid,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client, cdtype, cd, spellid);

  new manareq = GetSpellCost(client, spellid);

  wcGetTalentInfo(6,3,1,1,client,"multi",line);
  new Float:talentbonus=StringToFloat(line);
  manareq=RoundToNearest(manareq*(1-talentbonus));
  wcSetMana(client,GetMana(client)-manareq);

  //Getting values
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  if(HasGlyph(client, GLYPH_SHAMAN_TOTEM))
  multi-=0.1;

  wcGetSpellInfo(race,spellid,"ticks",line);
  new ticks=StringToInt(line);
  new hp=RoundToNearest(float(GetMaxHealth(client))*0.10);
  wcGetSpellInfo(race,spellid,"delay",line);
  new Float:delay=StringToFloat(line);
  ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
  delay = delay*(1.0-GetHasteEffect(client));
  wcGetSpellInfo(race,spellid,"aoe",line);
  new aoe=StringToInt(line);
  wcGetTalentInfo(6,1,1,3,client,"range",line);
  aoe+=StringToInt(line);

  decl Float:location[3];
  GetClientAbsOrigin(client,location);

  WCMessage(client,"totem earthbind cast",client,ticks*0.5);

  //Setting up effect
  new team=GetClientTeam(client);
  team = (team-1) % 2 + 2;
  new effid=CreateEffectData(client,spellid,any:multi,0,aoe,team,ticks,0,g_RoundCount);
  if(effid!=0)
  {
    new iTotem = -1;
    if(GetClientTeam(client)==2)
    iTotem = SpawnTotem(location, true, hp, 2); //Totem type 2 (greeny)
    else
    iTotem = SpawnTotem(location, false, hp, 2);
    if((effectsdata[effid][4]=iTotem) > 0) {
      if(TotemToPlayer(client,effectsdata[effid][4]))
      CreateTimer(delay, EffectEarthbindTotemAction0, effid);
      else  {
        KillTotem(effectsdata[effid][4]);
        DeleteEffectData(effid);
        LogError("ERROR: Could not attach totem to client");
      }
    }
    else  {
      LogError("ERROR: Failed spawning totem entity");
      DeleteEffectData(effid);
    }
  }
  //end

}

wcRunSpell_6_11(spellid,race,client,bool:echo=false) //Healing Rain
{
  decl String:line[255];
  if (!echo)
  {
    wcGetSpellInfo(race,spellid,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetSpellInfo(race,spellid,"cooldown",line);
    new Float:cd=StringToFloat(line);
    new manareq = GetSpellCost(client, spellid);
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client, cdtype, cd, spellid);
    
  }
  wcGetTalentInfo(6,2,3,2,client,"chance",line);
  new echochance = StringToInt(line);
  if(echochance>=GetRandomInt(1,100)) 
  {
    wcRunSpell_6_11(spellid,race,client,true);
    WCMessage(client,"Echo of the Elements activate",client);
  }
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetSpellInfo(race,spellid,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetSpellInfo(race,spellid,"multi",line);
  new Float:multi=StringToFloat(line);
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==3)
    amount=GetNearClients("@ct",location,targets,distance);
  if(team==2)
    amount=GetNearClients("@t",location,targets,distance);
  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  new String:stargets[255];
  new heal=GetSpellDmg(client,multi);
  wcGetSpellInfo(race,spellid,"base",line);
  new base=StringToInt(line);
  heal+=base;
  for (new x=0;x<=(amount-1);x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
    new healed=wcHeal(client,targets[x],heal,spellid,false);
    if(targets[x]!=client)
    {
      WCMessage(targets[x],"healing rain target",targets[x],name,healed);
    }
  }
  if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);

  location[2]+=20;

  VS_BeamRingPoint(location, 199.00, 200.00, g_ef_trail_blue, g_ef_trail_blue, 0, 0, 4.50, 90.00, 0.00, {75, 75, 255, 255}, 0, 0);
  VS_BeamRingPoint(location, 20.00, 200.00, g_ef_trail_blue, g_ef_trail_blue, 0, 0, 4.50, 10.00, 0.00, {75, 75, 255, 255}, 0, 0);
  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
    decl String:sTemp[30];
    Format(sTemp,sizeof(sTemp),"player_%i",client);
    DispatchKeyValue(client, "targetname", sTemp);
    DispatchKeyValue(sprite, "basespread", "100");
    DispatchKeyValue(sprite, "spreadspeed", "10");
    DispatchKeyValue(sprite, "speed", "105");
    DispatchKeyValue(sprite, "startsize", "5");
    DispatchKeyValue(sprite, "endsize", "5");
    DispatchKeyValue(sprite, "rate", "200");
    DispatchKeyValue(sprite, "jetlength", "400");
    DispatchKeyValue(sprite, "WindAngle", "10");
    DispatchKeyValue(sprite, "WindSpeed", "5");
    DispatchKeyValue(sprite, "angles", "0 0 900");
    DispatchKeyValue(sprite, "rendercolor", "175 175 255");
    DispatchKeyValue(sprite, "SmokeMaterial", "effects/bluespark.vmt");
    DispatchKeyValue(sprite, "parentname", sTemp);
    new Float:Client_Origin[3];
    GetClientAbsOrigin(client,Client_Origin);
    Client_Origin[2]+=300;
    DispatchKeyValueVector(sprite, "origin", Client_Origin);
    DispatchSpawn(sprite);
    SetVariantString(sTemp);
    //AcceptEntityInput(sprite, "SetParent", client, client, 0);
    AcceptEntityInput(sprite, "TurnOn");

    LiveEntity(sprite, 4.5);
  }

  WCMessage(client,"healing rain caster",client,stargets,heal);
  EmitAmbientSound("wc/spells/heal.wav",location);
}
public GetMiss(client)
{
  return g_iMiss[client];
}

public SetMiss(client, count)
{
  g_iMiss[client]=count;
}
//copied from 8_5
wcRunSpell_7_10(spellid,race,victim,client,&totaldmg)
{

  if((IsBoss() && (GetBoss(victim) > 0)) )  {
    //boss immunity
    return totaldmg;
  }
  new random=GetRandomInt(1,100);
  new chance=wcGetSpellInfoInt(race,spellid,"chance");
  if(chance>=random)
  {
    //CD check
    new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
    new Float:timeleft=GetCDSpell(client,cdtype)-GetTickedTime();
    if(timeleft > 0.0)
    return totaldmg;


    //Cooldown
    new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
    SetCdSpell(client, cdtype, cd, spellid);


    new Float:multi=wcGetSpellInfoFloat(race,spellid,"multi");

    new dmg=RoundToNearest(float(totaldmg)*multi);
    totaldmg+=dmg;

    GiveCurse(victim, multi, cd);

    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    EmitAmbientSound("wc/spells/curse.wav",location);
    if(CheckRequirements(client,7,2,1,4))
    {
      decl String:line[255];
      wcGetTalentInfo(7,2,1,4,client,"miss",line);
      SetMiss(victim,StringToInt(line));
      SetEffectAttacker(victim,54); // Miss effect
    }
    AddEMsgAff("curse", true);


  }
  return totaldmg;
}

new Float:g_fDamageBuff[65][2];
stock bool:hasDamageBuff(client)  {
  if(g_fDamageBuff[client][0] >= GetEngineTime())
  return true;
  return false;
}

stock GiveDamageBuff(client, Float:multiplier=0.0, Float:duration=0.0)  {
  g_fDamageBuff[client][0] = GetEngineTime() + duration;
  g_fDamageBuff[client][1] = multiplier;
}

stock removeDamageBuff(client)  {
  g_fDamageBuff[client][0]=0.0;
}

stock Float:getDamageBuff(client) {
  if(!hasDamageBuff(client))
  return 1.0;
  else
  return g_fDamageBuff[client][1];
}

new Float:g_fCurse[65][2];

stock bool:hasCurse(client) {
  if(g_fCurse[client][0] >= GetEngineTime())
  return true;
  return false;
}

stock GiveCurse(client, Float:multiplier=0.0, Float:duration=0.0) {
  g_fCurse[client][0] = GetEngineTime() + duration;
  g_fCurse[client][1] = multiplier;
}

stock removeCurse(client) {
  g_fCurse[client][0]=0.0;
}

stock Float:getCursePower(client) {
  if(!hasCurse(client))
  return 0.0;
  return g_fCurse[client][1];
}

public wcRunSpell_7_11(spellid, race, client)  {
  if(GetOwnedPet(client) != -1)
  {
    WCMessage(client,"already have infernal",client);
    return;
  }
  else
  ChooseTarget(client, 7,11, spellid, race, "Choose Player to cast Infernal");
}

public wcRunSpell_7_11_2(spellid,race,client, target)  {
  new Float:location[3];
  GetClientAbsOrigin(client,location);
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

  EmitAmbientSound("wc/spells/infernal.wav", location);

  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  WCMessage(target,"infernal target",target,name);
  WCMessage(client,"infernal caster",client,name2);
  __setZombie(target, client, 2); //Infernal = 2
  CS_RespawnPlayer(target);
  CreateTimer(0.1,GhoulEffect,target);

}
public GetHarvest(client)
{
  return g_iHarvest[client];
}

public SetHarvest(client, count)
{
  g_iHarvest[client]=count;
}

public wcRunSpell_7_2(spellid,race,client) //Soul Harvest
{
  if(GetHarvest(client)<=0) {
    WCMessage(client,"no souls harvested",client);
    return;
  }

  if(InCombat(client))
  {
    WCMessage(client,"in combat",client,float(GetCombat(client))/2.0);
    return;
  }
  if(GetMaxHealth(client)==wcGetHealth(client))
  {
    WCMessage(client,"full health",client);
    return;
  }
  new mana = wcGetSpellInfoInt(race,spellid,"mana");
  if (mana > GetMana(client))
    return;
  else
    wcSetMana(client,GetMana(client)-mana);
  
  //Cooldown
  new cdtype=wcGetSpellInfoInt(race,spellid,"cooldowntype");
  new Float:cd=wcGetSpellInfoFloat(race,spellid,"cooldown");
  decl String:line[255];
  wcGetTalentInfo(7,2,2,4,client,"shcd",line);
  cd-=StringToFloat(line);
  SetCdSpell(client, cdtype, cd, spellid);

  new Float:multi = wcGetSpellInfoFloat(race,spellid,"multi");
  multi = multi * GetHarvest(client);
  RestoreHealthP(client,multi);
  new pet=GetOwnedPet(client);
  if(pet>0)
  {
    WCMessage(pet,"soul harvest pet",pet,multi*100.0);
    RestoreHealthP(pet,multi);
  }
  else 
  {
    WCMessage(client,"soul harvest caster",client,multi*100.0);
  }
  SetHarvest(client, 0);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/soulharvest.wav",location);
  SoulHarvestEffect(client);
}

SetReopenSpellbar(client,bool:reopen)
{
  ReopenSpellbar[client]=reopen;
}

GetReopenSpellbar(client)
{
  return ReopenSpellbar[client];
}

SetVanishBreakDmg(client,damage)
{
  g_iVanishBreakDmg[client]=damage;
}

GetVanishBreakDmg(client)
{
  return g_iVanishBreakDmg[client];
}

wcRunSpell_6_12(client)
{
  new bonus = GetAgi(client)+GetBonusAgi(client);
  SetBonusArmor(client, float(GetBonusArmor(client)+bonus));
  wcRecalculateStats(client);
  WCMessage(client,"agile defence effect",client,bonus,GetArmor(client));
  if(CheckRequirements(client,6,2,4,2))
  {
    decl String:line[255];
    wcGetTalentInfo(6,2,4,2,client,"multi",line);
    new Float:multi=StringToFloat(line);
    new resbonus=RoundToNearest(float(bonus)*multi);
    wcClientsBonusStats[client][RESISTANCE]+=resbonus;
    wcRecalculateStats(client);
  }
}

stock SoulHarvestEffect(client)
{
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  location[2]+=10;
  VS_BeamRingPoint(location, 50.00, 80.00, g_ef_physring1, g_ef_physring1, 1, 10, 1.0, 15.00, 2.00, {153, 217, 234, 255}, 20, 0);
  location[2]+=10;
  VS_BeamRingPoint(location, 50.00, 85.00, g_ef_physring1, g_ef_physring1, 1, 10, 1.0, 15.00, 2.00, {153, 217, 234, 255}, 20, 0);
  location[2]+=10;
  VS_BeamRingPoint(location, 50.00, 85.00, g_ef_physring1, g_ef_physring1, 1, 10, 1.0, 15.00, 2.00, {153, 217, 234, 255}, 20, 0);
  location[2]+=10;
  EmitAmbientSound("wc/spells/soulharvest.wav",location);
  VS_BeamRingPoint(location, 50.00, 85.00, g_ef_physring1, g_ef_physring1, 1, 10, 1.0, 15.00, 2.00, {153, 217, 234, 255}, 20, 0);
  location[2]+=10;
  VS_BeamRingPoint(location, 50.00, 80.00, g_ef_physring1, g_ef_physring1, 1, 10, 1.0, 15.00, 2.00, {153, 217, 234, 255}, 20, 0);
  location[2]+=10;
  VS_BeamRingPoint(location, 50.00, 75.00, g_ef_physring1, g_ef_physring1, 1, 10, 1.0, 15.00, 2.00, {153, 217, 234, 255}, 20, 0);


}
