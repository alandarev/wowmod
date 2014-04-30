  /*
* File: damages.inc
* Description: Damage functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1
new String:wcSuicideSource[25]="0";
new wcSuicideUserid=0;
new wcSuicideAttacker=0;
new bool:wcSuicideHeadshot=false;

//new way of handling kills
new bool:wcKillHandled=false;
new bool:wcKillHandled2=false;
new bool:wcKillHeadshot=false;
//new offsetscore;
//new entscore=0;

stock LowLvlDmg(victim,attacker,&dmg,bool:spell)
{
  if(bArenaActive)
    return; //no bonus if arena is active

  if(SERVER_LEVEL_POLICY==1)  {
    return; //No bonus for high lvls servers
  }
  if(GetClass(attacker)==0 || GetClass(victim)==0)
    return; //No bonus for no class
  //if(GetLevel(attacker) > GetLevel(victim))
  //  return; //No nerf's for high levels
  if(IsBoss() || IsHNS())  return; //No bonus during boss event
  new dif=(GetLevel(attacker)-GetLevel(victim));
  if((dif > 0 ) && IsZombie(victim))
    return;
  if((dif < 0) && IsZombie(attacker))
    return;
  //if(dif > 50) dif = 50;
  //if(dif < -50) dif = -50;

  //Low lvl server settings
  if(SERVER_LEVEL_POLICY==2) {
    if(GetLevel(attacker) < 70 && dif > 0)  dif /= 2; // Give feeling of becoming stronger for people leveling up on Low lvls server
    if(GetLevel(victim) < 70 && dif < 0) dif /= 2;

    //Punish high lvls
    if(GetLevel(attacker) > 80 && dif > 0)  dif +=15;
    if(GetLevel(victim) >80 && dif < 0) dif-=15;
  }

  new mod = dif;
  if (mod < 0)
    mod *= -1;
  new bool:extra=false;
  if(dif < 0)
    extra = isPlayerNew(attacker);
  else
    extra = isPlayerNew(victim);

  //Extra penality for being above average player levels
  if(GetLevel(attacker)>GetLevel(victim) && GetLevel(attacker)>(GetAverageLevel()+10) && GetLevel(victim)<GetAverageLevel())
  {
    dif+=(GetLevel(attacker)-GetAverageLevel()-15)/2;
  }
  else if(GetLevel(attacker)<GetLevel(victim) && GetLevel(attacker)<GetAverageLevel() && GetLevel(victim)>(GetAverageLevel()+10))
  {
    dif-=(GetLevel(victim)-GetAverageLevel()-15)/2;
  }

  if (mod>8)
  {
    new Float:dred=LOWLVL_ARG*float(dif);
    if(8 >= PlayersInTeams(true))
      dred*=1.25;
    if(spell)
      dred*=0.6;
    //if (dred>0.8)
      //dred=0.8;
    //if ((spell) && (dred > 0.5))
      //dred=0.5;
    //Normalize it so that reduction is % wise.
    if(dred > 0.0)
      dmg = RoundToNearest(float(dmg) / (1.0 + dred));
    else
      dmg=RoundToNearest(float(dmg)*(1.0-dred));
  }
  if(extra)
  {
    //Do we need to balance out damage?
    new Float:modifier = 1.0;
    if(dif < 0)
    {
      if(!spell)
      modifier = getNewPlayerModifier(attacker, true);
    }
    else
      modifier = getNewPlayerModifier(victim, false);

    dmg = RoundToNearest(float(dmg)*modifier);
  }
}

new bool:g_bLastHealCrit;
stock isHealCrit()  {
  return g_bLastHealCrit;
}

/**
 * Control healing.
 * Reduces healing done to target if it was healed too much.
 * Ofc, scheme is a little bit more complicated, but point is same
 */
stock HealControl(const client, const amount) {
  if(IsBoss() || IsHNS())  return amount;  //Disabled during boss
  new const Float:time = 35.0;
  new Float:coef = 0.6; // For 100% hp healed reduce healing by 100 * coef.  Ex: 0.5 = = 50%
  new healed = GetLastHeals(client, time);
  coef = float(healed) / 100.0 * coef;
  if(coef > 0.7)  coef = 0.7;
  coef-=0.2;
  if(coef <= 0.0) return amount;
  new heal = RoundToNearest(float(amount) * (1.0 - coef));
  return heal;

}

new g_ffHealedAmount[65][100];
new Float:g_ffHealedWhen[65][100];
new g_ffHealedIterator[65];

stock GetLastHeals(const client, const Float:time)  {
  new total;
  new iterator = g_ffHealedIterator[client];
  for(new i=iterator; i > iterator - 100; i--)  {
    new index = i;  /* i might be negative, so we don't want segmentation error */
    if(index < 0) index+=100;
    if(index >= 100) LogError("Logic error: index: %d with iterator %d", index, iterator);
    /**
     * Check if iterated healing is done long time ago
     */
    if(g_ffHealedWhen[client][index] < GetEngineTime() - time)
      break;
    //We are cool to add
    total += g_ffHealedAmount[client][index];
  }
  return total;
}

/**
 * amount should be percentage of health, not the health points.
 */
stock PushHealed(const client, const amount)  {
  //I know those [[[]]] Look a bit dodgy
  g_ffHealedIterator[client] = (g_ffHealedIterator[client] + 1) % 100;
  g_ffHealedWhen[client][g_ffHealedIterator[client]] = GetEngineTime();
  g_ffHealedAmount[client][g_ffHealedIterator[client]] = amount;
}

//Percentage heals should not crit as they heal a set percent.
wcHeal(client,target,heal, spellid=0, bool:cancrit=true) //Healing
{
  //heal = HealControl(target, heal);
  SetFocusedWill(client,false);
  new maxhp=GetMaxHealth(target);
  new hp=wcGetHealth(target);
  new oldhp=hp;
  decl String:line[255];
  //modify heal by kpd rule
  heal = KpdRule(target, heal, 4);


  if(GetClass(client)==5)
  {
    if(CheckRequirements(client,5,1,1,4) && spellid != 0)
      wcRunTalent_5_StrengthOfSoul(client);

    if(CheckRequirements(client,5,2,1,2) && heal>=15) //Inspiration
      wcRunTalent_5_8(client,target);
    wcGetTalentInfo(5,2,2,1,client,"health",line); //Test of Faith
    new Float:talentbonus=StringToFloat(line);
    if(hp<(maxhp*talentbonus))
    {
      wcGetTalentInfo(5,2,2,1,client,"multi",line); //Test of Faith
      talentbonus=StringToFloat(line);
      heal=RoundToNearest(heal*(talentbonus+1));
    }
    if(CheckRequirements(client,5,1,3,3))
      wcRunTalent_5_DivineAegis(client,target,heal);
  }
  if(GetClass(target)==4 && CheckRequirements(target,4,2,1,4) && target!=client)
    wcRunTalent_4_DivineBody(target,heal);
  if(GetClass(target)==7 && CheckRequirements(target,7,2,1,1))
    wcRunTalent_7_FelVitality(target,heal);
  if(GetCocoon(target)>0)
    CocoonBuff(target,heal);
  if(GetTigerBrew(target)>0)
    TigereyeBuff(target,heal);
  

  if(GetSealT(client)>GetTickedTime() && HasSeal(client, Seal:SEAL_Light))
  {
    wcGetSpellInfo(4,14,"base",line);
    new Float:multi=StringToFloat(line);
    heal = RoundToNearest(heal*(1+multi));
  }

  //crit?
  new bool:bCS=false;
  CalculateCrit(client,target,true,"heal",bCS,true);
  if(cancrit==false)
    bCS=false;
  if(bCS)
  {
    heal = RoundToNearest(float(heal)*1.5);
    if(GetClass(client)==4)
    {
      if(CheckRequirements(client,4,1,1,1))
      {
        wcGetTalentInfo(4,1,1,1,client,"chance",line);
        new chance = StringToInt(line);
        new rand = GetRandomInt(1,100);
        if (chance >= rand)
          SetHolyPower(client,GetHolyPower(client)+1);
      }
      if(CheckRequirements(client,4,1,3,2))
      {
        wcGetTalentInfo(4,1,3,2,client,"chance",line); //paladin talent, holy tree, Speed of Light
        new chance = StringToInt(line);
        new rand = GetRandomInt(1,100);
        if (chance >= rand)
        {
          wcGetSpellInfo(4,spellid,"cooldowntype",line);
          new type = StringToInt(line);
          wcGetTalentInfo(4,1,3,2,client,"multiplier",line); //paladin talent, holy tree, Speed of Light
          new Float:multiplier = StringToFloat(line);
          wcGetSpellInfo(4,spellid,"cooldown",line);
          new Float:cooldown = StringToFloat(line)*multiplier;
          SetCdSpell(client,type,cooldown,spellid);
          new Float:location[3];
          GetClientAbsOrigin(client,location);
          location[2]+=50;
          EmitAmbientSound("wc/spells/focusedmind.wav",location);
        }
      }
    }
  }
  if(GetSealT(client)>GetTickedTime() && HasSeal(client, Seal:SEAL_Insight) && bCS)
  {
    wcGetSpellInfo(4,21,"multi",line);
    new Float:multi=StringToFloat(line);
    heal = RoundToNearest(heal*multi);
  }

  g_bLastHealCrit=bCS;


  if(GetHMT(target)>GetTickedTime())
    heal=RoundToNearest(float(heal)*GetHM(target));
  if(GetHealingBonus(target)>0.0)
    heal=RoundToNearest(float(heal)*(1.0+GetHealingBonus(target)));
  if(GetClass(client) == 4 && GetHealingBonus(client) > 0.0)
    heal=RoundToNearest(float(heal)*(1.0+GetHealingBonus(client)));
  if(GetShadowform(target) && HasGlyph(target, GLYPH_PRIEST_SHADOWFORM))
    heal = RoundToNearest(float(heal)*1.15);
  if(g_iCarry[target]>0)
    heal = RoundToNearest(float(heal)*CTF_NERF);
  if(g_iNecrotic[client]>0)
  {
    if(heal>=g_iNecrotic[client])
    {
      heal-=g_iNecrotic[client];
      g_iNecrotic[client]=0;
    }
    else
    {
      g_iNecrotic[client]-=heal;
      heal=0;
    }
  }
  hp+=heal;
  if(maxhp<hp)
    hp=maxhp;
  heal=hp-oldhp;
  if(GetClass(client)==4 && client != target)
  {
    new random = GetRandomInt(1,50);
    if(random <= heal)
    {
      SetHolyPower(client,GetHolyPower(client)+1);
      if(heal-50>0)
      {
        random = GetRandomInt(1,50);
        if(heal-50>=random)
          SetHolyPower(client,GetHolyPower(client)+1);
      }
    }
  }
  if(GetClass(client)==5)
  {
    if(CheckRequirements(client,5,2,2,4) && client != target) //Body and Soul
      wcRunTalent_5_BodyAndSoul(client,heal);
    if(CheckRequirements(client,5,2,1,4) && client != target)
      wcRunTalent_5_SurgeOfLight(client,spellid);
  }
  if(GetClass(client)==9) //This should always be at the near end as it looks at the amount of healing done for its chance to happen!
  {
    if(CheckRequirements(client,9,3,3,2) || IsFakeClient(client))
    {
      wcGetTalentInfo(9,3,3,2,client,"maxchance",line);
      new maxchance = StringToInt(line);
      if(IsFakeClient(client))
        maxchance = 50;
      new random = GetRandomInt(1,maxchance);
      if(heal>=random)
        addChi(client,1);
    }
  }
  //PrintToServer("Healing done: %d target = %d, client = %d", heal, target, client);
  new item[UniqueItem];
  GetCharItem(item, client, 20);
  if(item[Item_defId]==25246 && 80>GetLevel(client))
    if(25 >= GetRandomInt(1,100))
    {
      decl Float:loc2[3];
      GetClientAbsOrigin(client,loc2);
      new damagetarget;
      if(GetClientTeam(client)==2)
        damagetarget = GetNearestClient("@ct",loc2,200.0);
      if(GetClientTeam(client)==3)
        damagetarget = GetNearestClient("@t",loc2,200.0);
      if(damagetarget!=0)
        wcSpellDamage(damagetarget,client,RoundToNearest(float(hp)*0.30), "", "",line);
    }
  //Get heal percentage and push it to stack
  
  
  new random=GetRandomInt(1,100);
  GetCharItem(item, client, 20);
  if(item[Item_defId]==25248 && 80>GetLevel(client))
    if(15>=random)
    {
      if(GetMana(client)+20>GetMaxMana(client))
        wcSetMana(client,GetMaxMana(client));
      else
        wcSetMana(client,GetMana(client)+20);
      
    }
  GetCharItem(item, client, 20);
  if(item[Item_defId]==25260 && 80>GetLevel(client))
    if(20 >= GetRandomInt(1,100))
      if(client==target)
      {
        new energyrestore = RoundToNearest(float(heal)*0.15)+2;
        if(GetMana(client)+energyrestore>GetMaxMana(client))
          wcSetMana(client,GetMaxMana(client));
        else
          wcSetMana(client,GetMana(client)+energyrestore);
      }
  
  new perc = RoundToNearest(float(heal) / float(GetMaxHealth(client)) * 100.0);
  PushHealed(client, perc);
  if(target!=client)
  {
      if(!IsFakeClient(target) || HEAL_BOT)
         wcHealingDone[client]+=heal;
  }
  wcSetHealth(target,hp);
  HealEffect(target);
  GenerateHealThreat(client,heal);
  return heal;
}

public wcDealDamage(userid,attacker,totaldmg,String:source[],bool:headshot)
{
  if(IsPlayerAlive(userid))
  {
    new hp=wcGetHealth(userid);
    if (hp<=totaldmg) {
      wcFatalDamage(attacker, userid, source, totaldmg, headshot);
      //KillTarget(userid,attacker,source,headshot);
    }
    else
    {
      wcSetHealth(userid,hp-totaldmg);
    }
    GenerateDMGThreat(attacker,userid,totaldmg);
  }
}


public Action:TimerDmg(Handle:timer, Handle:pack)  {
  ResetPack(pack);
  new victim = ReadPackCell(pack);
  new attacker = ReadPackCell(pack);
  if(IsClientInGame(victim) && IsPlayerAlive(victim) && IsClientInGame(attacker)) {
    new damage = ReadPackCell(pack);
    SDKHooks_TakeDamage(victim, attacker, attacker, float(damage));
  }
  return Plugin_Stop;
}

//(War3_DealDamage(i,KnivesTFDamage,victim,DMG_BULLET,"knives",W3DMGORIGIN_SKILL,W3DMGTYPE_MAGIC))
stock wcFatalDamage(attacker, victim, String:weapon[], damage=500, bool:headshot=false)  {
  wcFatalDamageCS(attacker, victim, weapon, damage, headshot);
}

stock wcFatalDamageCS(attacker, victim, String:weapon[], damage=100, bool:headshot=false)
{
  //Checks are done fbefore, so we just go straight into damage
  //type: DMG_BULLET

  wcKillHeadshot=headshot;
  wcKillHandled=true;
  wcKillHandled2=true;

  //Set armor to 0, no need to restore because he is dying anyway.
  SetEntData(victim, offsArmor, 0);

  new dmg_bullet = (1 << 1);

  new String:dmg_str[16];
  IntToString(damage,dmg_str,16);
  new String:dmg_type_str[32];
  IntToString(dmg_bullet,dmg_type_str,32);
  new pointHurt=CreateEntityByName("point_hurt");
  if(pointHurt)
  {
    DispatchKeyValue(victim,"targetname","war3_hurtme"); //set victim as the target for damage
    DispatchKeyValue(pointHurt,"DamageTarget","war3_hurtme");
    DispatchKeyValue(pointHurt,"Damage",dmg_str);
    DispatchKeyValue(pointHurt,"DamageType",dmg_type_str);
    if(!StrEqual(weapon,""))
    {
      DispatchKeyValue(pointHurt,"classname",weapon);
    }
    DispatchSpawn(pointHurt);
    AcceptEntityInput(pointHurt,"Hurt",(attacker>0)?attacker:-1);
    DispatchKeyValue(pointHurt,"classname","point_hurt");
    DispatchKeyValue(victim,"targetname","no_hurt_me_t"); //unset the victim as target for damage
    RemoveEdict(pointHurt);
  }
  new String:vName[50];
  GetClientName(victim, vName, 50);
  return true;
}

stock wcFatalDamageGO(attacker, victim, String:weapon[], damage=500, bool:headshot=false)
{	
  //Checks are done fbefore, so we just go straight into damage
  //type: DMG_BULLET

  wcKillHeadshot=headshot;
  wcKillHandled=true;
  wcKillHandled2=true;

  //Set armor to 0, no need to restore because he is dying anyway.
  SetEntData(victim, offsArmor, 0);
			
  new dmg_bullet = (1 << 1);

  new String:dmg_str[16];
  IntToString(damage,dmg_str,16);
  new String:dmg_type_str[32];

  /*PrintToServer("Calling sdkhook take damage. victim %d attacker %d damage %d",victim, attacker, damage);*/
  new Handle:pack = CreateDataPack();
  WritePackCell(pack, victim);
  WritePackCell(pack, attacker);
  WritePackCell(pack, damage);
  PushArrayCell(g_dmgArray, _:pack);
  /*CreateTimer(0.0, TimerDmg, pack, TIMER_HNDL_CLOSE);*/

  /*SDKHooks_TakeDamage(victim, attacker, attacker, float(damage));*/
  IntToString(dmg_bullet,dmg_type_str,32);
  /*new pointHurt=CreateEntityByName("point_hurt");*/
  /*if(pointHurt)*/
  /*{*/
    /*DispatchKeyValue(victim,"targetname","war3_hurtme"); //set victim as the target for damage*/
    /*DispatchKeyValue(pointHurt,"DamageTarget","war3_hurtme");*/
    /*DispatchKeyValue(pointHurt,"Damage",dmg_str);*/
    /*DispatchKeyValue(pointHurt,"DamageType",dmg_type_str);*/
    /*if(!StrEqual(weapon,""))*/
    /*{*/
      /*DispatchKeyValue(pointHurt,"classname",weapon);*/
    /*}*/
    /*DispatchSpawn(pointHurt);*/
    /*AcceptEntityInput(pointHurt,"Hurt",(attacker>0)?attacker:-1);*/
    /*DispatchKeyValue(pointHurt,"classname","point_hurt");*/
    /*DispatchKeyValue(victim,"targetname","no_hurt_me_t"); //unset the victim as target for damage*/
    /*RemoveEdict(pointHurt);*/
  /*}*/
  new String:vName[50];
  GetClientName(victim, vName, 50);
  return true;
}

public FindEntScore()
{
    new maxplayers = GetMaxClients();
    new maxents = GetMaxEntities();

    decl String:classname[64];

    for(new i = maxplayers; i <= maxents; i++)
    {
        if(IsValidEntity(i))
        {
            GetEntityNetClass(i, classname, sizeof(classname));
            if(StrEqual(classname, "CPlayerResource"))
            {
                return i;
            }
        }
    }
    return 0;
}

public KillTarget(userid,attacker,String:source[],bool:headshot)
{
  strcopy(wcSuicideSource, sizeof(wcSuicideSource), source);
  wcSuicideUserid=userid;
  wcSuicideAttacker=attacker;
  wcSuicideHeadshot=headshot;
  ForcePlayerSuicide(userid);
}

stock wcSpellDamage(userid,attacker,amount, String:amsg[]="", String:vmsg[]="", String:source[]="", bool:aoe=false)
{
  new Handle:event=CreateEvent("player_hurt");
  SetEventInt(event,"userid",GetClientUserId(userid));
  SetEventInt(event,"attacker",GetClientUserId(attacker));
  SetEventInt(event,"dmg_health",amount);
  SetEventString(event,"weapon","Spell");
  SetEventInt(event,"hitgroup",2);
  SetEventString(event,"amsg",amsg);
  SetEventString(event,"vmsg",vmsg);
  SetEventString(event,"name",source);
  if(aoe)
    SetEventBool(event,"aoe",aoe);
  FireEvent(event);
}

stock wcSpellAoeDamage(userid,attacker,dmg,iDistance,team, String:source[]="",Float:location[3]={0.0,0.0,0.0})
{
  //new Float:location[3];
  if(location[0] == 0.0 || location[1] == 0.0 || location[2] == 0.0)
    GetClientAbsOrigin(userid,location);
  wcSpellAoeDamageLocation(userid, attacker, location, dmg, iDistance, team, source);
}


stock wcSpellAoeDamageLocation(userid,attacker, Float:location[3],dmg,iDistance,team, String:source[]="")
{
  new Float:distance=float(iDistance);
  new targets[MAXPLAYERS];
  new amount;
  if(team==2)
    amount=GetNearClients("@t",location,targets,distance);
  else if(team==3)
    amount=GetNearClients("@ct",location,targets,distance);
  else
  return;
  decl String:name[40];
  GetClientName(userid,name,sizeof(name));
  for (new x=0;x<=(amount-1);x++)
  {
    if(targets[x]!=userid)
    WCMessage(targets[x],"aoe damaged",targets[x],name,dmg);
    wcSpellDamage(targets[x],attacker,dmg, "", "",source, true);
  }
}


stock wcPhysicalDamage(userid,attacker,amount, String:amsg[]="", String:vmsg[]="", bool:reduction=false, String:source[]="")
{
  /*return;*/
  new Handle:event=CreateEvent("player_hurt");
  SetEventInt(event,"userid",GetClientUserId(userid));
  SetEventInt(event,"attacker",GetClientUserId(attacker));
  SetEventInt(event,"dmg_health",amount);
  SetEventString(event,"weapon","Physical");
  SetEventInt(event,"hitgroup",2);
  SetEventString(event,"amsg",amsg);
  SetEventString(event,"vmsg",vmsg);
  SetEventBool(event, "reduction", reduction);
  SetEventString(event,"source",source);
  FireEvent(event);
}

stock Float:GetDmgRed(armor, userid)   {
   new Float:dred;
   if (armor>0)
      dred=float(1) - (float(armor) / (float(armor) + (10.0*float(GetLevel(userid))+20.0)));
   else
      dred=1.0;
   if (dred < 0.1)
      dred=0.1;
   return dred;
}
