#define RACES 10
#define SAVEID_OFFSET -48

new g_iClientTalents[65*2][10][10][10];
new g_iTalentUnique[RACES][10][10][10];  //Constant, is constructed on talents database load
new g_iTalentsMap[RACES][MAX_TALENTS+30]; //Constant, is constructed on talents database load
//new g_iElementalFury[65];
new g_iEarthShield[65][2];
//new g_iDeathWish[65];
new g_iEnrage[65];
new g_iWisdomClient[65];
new Float:g_fMightClient[65];
new g_iFortCast[65];
new Float:g_fDualWieldSpecc[65];
new g_iFlurry[65];
new g_iSweepingStrikes[65];
new bool:Adrenaline[65];
new bool:ColdBlood[65];
new g_iInspiration[65];
new g_iReflectiveShield[65];
new g_iReckoning[65];
new g_iRollingThunder[65][5];
new bool:g_bAutocastTalents[65][10];
new bool:g_bBlessedRes[65];
new g_iPoisoned[65];
new Float:g_fPoisoned[65];
new bool:g_bElusive[65];
new Float:g_iShadowGateLoc[65][3];
new Float:g_fArcanePotency[65];
new Float:g_fHealingBonus[65];
new bool:g_bFocusedWill[65];
new Float:g_fNaturesBlessing[65];
new g_iSpiritLink[65];
new g_iPlagued[65];
new g_iZoned[65];
new g_iBastion[65];

new g_ff_activeSpec[65]; //should be 0 or 1
setActiveSpec(client, value)  {
  g_ff_activeSpec[client]=value;
}

getActiveSpec(client) {
  if(client==0 || client > 64)
    return 0;
  return g_ff_activeSpec[client];
}

getInactiveSpec(client) {
  return (getActiveSpec(client)+1) % 2;
}

swapSpecs(client) {
  g_ff_activeSpec[client] = getInactiveSpec(client);
  if( (GetClass(client)==6 || GetClass(client)==8 ) && HasDual(client))
    PutIntoInventory(client,18);

}


//Database related
clearTalents(client)  {
  for(new spec=0; spec<2; spec++) for(new p1=0; p1<10; p1++) for(new p2=0; p2<10; p2++) for(new p3=0; p3<10; p3++)
    g_iClientTalents[client+(64*spec)][p1][p2][p3]=0;
}

//suppresses warnings if doesn't exist, and is optimized for unique id only
stock getTalentUniqueId(race, p1, p2, p3) {
  KvRewind(TalentsDB);
  decl String:temp[5];
  IntToString(race, temp, 5);
  if(!KvJumpToKey(TalentsDB, temp))
    return -1;
  IntToString(p1, temp, 5);
  if(!KvJumpToKey(TalentsDB, temp))
    return -1;
  IntToString(p2, temp, 5);
  if(!KvJumpToKey(TalentsDB, temp))
    return -1;
  IntToString(p3, temp, 5);
  if(!KvJumpToKey(TalentsDB, temp))
    return -1;
  //We are here
  //Woops not yet, jump to level 0!
  KvJumpToKey(TalentsDB, "0");
  KvGetString(TalentsDB, "saveid", temp, 5);
  return temp[0]+SAVEID_OFFSET;
}

talentsPreloadMaps() {
  //Preloads mapping between paths to talents and unique ids in both directions
  //Is needed for database to work
  new i=0;
  //our favo loopings
  for(new race=0; race < RACES; race++) {
    for(new tree=0; tree < 10; tree++)  {
      for(new tier=0; tier<10; tier++)  {
        for(new point=0; point<10; point++) {
          new index = getTalentUniqueId(race, tree, tier, point);
          if(index>=0)  { //it exists
            g_iTalentsMap[race][index]=tree*100+tier*10+point;
            g_iTalentUnique[race][tree][tier][point]=index;
            i++;
          }
        }
      }
    }
  }
  PrintToServer("[WC]: Preloaded %d talents", i);
}

setClientTalents_stirng(client, race, String:set[], spec)  {
  for(new i=0; i<strlen(set); i++)  {
    new c=set[i]+SAVEID_OFFSET;  //convert to start from 0 ASCII
    //Get mapping
    new talentMapped = g_iTalentsMap[race][c];
    new p3 = talentMapped % 10;
    talentMapped/=10;
    new p2 = talentMapped %10;
    talentMapped/=10;
    new p1 = talentMapped;
    if(checkTalent(p1,p2,p3,client, spec))
      wcAddStaticTalent(p1, p2, p3, client, spec);
  }
}

/**
 * Gets actual path to talent through it's saveid
 */
bool:getTalentPath(race, saveid, &tree, &tier, &talent)  {
  if(race <=0)
    return false;
  if(saveid < 0)
    return false;
  new map = g_iTalentsMap[race][saveid];
  if(map <= 0)
    return false;

  tree = map / 100;
  map -= tree*100;
  tier = map / 10;
  map -= tier*10;
  talent = map;
  if(talent <= 0)
    return false;
  return true;
}

checkTalent(tree,tier,talent,client, spec)
{
  decl String:line[255];
  new class = GetClass(client);
  wcGetTalentInfo(class,tree,tier,talent,66,"max",line);
  new max = StringToInt(line);
  new level = wcGetTalentLevel(client,tree,tier,talent, spec);
  if (level==max || wcGetTreeTotal(client, spec,tree)<5*(tier-1)) {
    setTPoints(client, spec,getTPoints(client, spec)+1);
    return false;
  }
  return true;
}

getClientTalents_string(client, spec, String:ret[], max) {
  new len=Format(ret,max,"");
  new race = GetClass(client);
  //loop
  for(new p1=0; p1<10; p1++) for(new p2=0; p2<10; p2++) for(new p3=0; p3<10; p3++)
  {
    for(new repeats=0;repeats < g_iClientTalents[client+(64*spec)][p1][p2][p3]; repeats++)  {
      len+=Format(ret[len],max-len,"%c",g_iTalentUnique[race][p1][p2][p3]- SAVEID_OFFSET);
    }
  }
}
//end of database related

stock wcGetTalentInfo(raceid,treeid,tierid,talentid,client,String:info[],String:line[255], transClient=0)
{
  new spec = getActiveSpec(client);
  KvRewind(TalentsDB);
  decl String:s_talentid[5];
  decl String:s_tierid[5];
  decl String:s_treeid[5];
  decl String:s_raceid[5];
  decl String:s_tlevel[5];
  IntToString(talentid,s_talentid,sizeof(s_talentid));
  IntToString(tierid,s_tierid,sizeof(s_tierid));
  IntToString(treeid,s_treeid,sizeof(s_treeid));
  IntToString(raceid,s_raceid,sizeof(s_raceid));
  if(!KvJumpToKey(TalentsDB,s_raceid))
    LogError("key race = %s doesn't exist", s_raceid);
  if(treeid != 0)
  {
    KvJumpToKey(TalentsDB,s_treeid);
    if(tierid != 0)
    {
      KvJumpToKey(TalentsDB,s_tierid);
      if(talentid != 0)
      {
        KvJumpToKey(TalentsDB,s_talentid);
        if(client!=66)
        {
        IntToString(g_iClientTalents[client+(64*spec)][treeid][tierid][talentid], s_tlevel, sizeof(s_tlevel));
        }
        else
        {
        IntToString(1, s_tlevel, sizeof(s_tlevel));
        }
        KvJumpToKey(TalentsDB,s_tlevel);
      }
    }


  }
  //Translate?
  decl String:trans[50];

  KvGetString(TalentsDB, "name", trans, sizeof(trans), "0");
  if(transClient == 0 || talentid == 0 || StrEqual(trans, "0") ) { //Don't support tree's translate yet {
    KvGetString(TalentsDB, info, line, sizeof(line), "0");
  }
  else  {
    if(StrEqual(info, "name", false))  {
      Format(trans, sizeof(trans), "%s name", trans);
    } else if(StrEqual(info, "description", false))  {
      Format(trans, sizeof(trans), "%s info", trans);
    }
    else {
      LogError("[WC] Logics error 227 debug: %s", info);
      return;
    }
    Format(line, 255, "%T", trans, transClient);
  }

}

/*
public wcGetTalentInfo2(raceid,saveid,String:info[],String:line[255])
{
  KvRewind(TalentsDB);
  decl String:s_talentid[5];
  decl String:s_tierid[5];
  decl String:s_treeid[5];
  decl String:s_raceid[5];
  decl String:s_tlevel[5];
  IntToString(talentid,s_talentid,sizeof(s_talentid));
  IntToString(tierid,s_tierid,sizeof(s_tierid));
  IntToString(treeid,s_treeid,sizeof(s_treeid));
  IntToString(raceid,s_raceid,sizeof(s_raceid));
  if(!KvJumpToKey(TalentsDB,s_raceid))
    LogError("key race = %s doesn't exist", s_raceid);
  if(treeid != 0)
  {
    KvJumpToKey(TalentsDB,s_treeid);
    if(tierid != 0)
    {
      KvJumpToKey(TalentsDB,s_tierid);
      if(talentid != 0)
      {
        KvJumpToKey(TalentsDB,s_talentid);
        if(client!=66)
        {
        IntToString(g_iClientTalents[client+(64*spec)][treeid][tierid][talentid], s_tlevel, sizeof(s_tlevel));
        }
        else
        {
        IntToString(1, s_tlevel, sizeof(s_tlevel));
        }
        KvJumpToKey(TalentsDB,s_tlevel);
      }
    }

  }
  KvGetString(TalentsDB, info, line, sizeof(line), "0");
}
*/

GetBastion(client)
{
  return g_iBastion[client];
}

SetBastion(target,caster)
{
  g_iBastion[target]=caster;
}

GetZoned(client)
{
  return g_iZoned[client];
}

SetZoned(client,effid)
{
  g_iZoned[client]=effid;
}

GetPlagued(client)
{
  return g_iPlagued[client];
}

SetPlagued(client,caster)
{
  g_iPlagued[client]=caster;
}

/*SetSpiritLink(client,target)
{
  g_iSpiritLink[client]=target;
}
*/

GetSpiritLink(client)
{
  return g_iSpiritLink[client];
}

Float:GetNBlessing(client)
{
  return g_fNaturesBlessing[client];
}

SetNBlessing(client,Float:amount)
{
  g_fNaturesBlessing[client]=amount;
}

GetFocusedWill(client)
{
  return g_bFocusedWill[client];
}

SetFocusedWill(client,bool:active)
{
  g_bFocusedWill[client]=active;
}

Float:GetHealingBonus(client)
{
  return g_fHealingBonus[client];
}

SetHealingBonus(client, Float:amount)
{
  g_fHealingBonus[client]=amount;
}

SetArcanePotency(client, Float:amount)
{
  g_fArcanePotency[client]=amount;
}

Float:GetArcanePotency(client)
{
  return g_fArcanePotency[client];
}

SetShadowGateLocation(client, x, Float:amount)
{
  g_iShadowGateLoc[client][x]=amount;
}

Float:GetShadowGateLocation(client, x)
{
  return g_iShadowGateLoc[client][x];
}

stock SetPoisoned2(client, Float:amount)
{
  g_fPoisoned[client]=amount;
}

Float:GetPoisoned2(client)
{
  return g_fPoisoned[client];
}

SetElusive(client, bool:active)
{
  g_bElusive[client]=active;
}

stock GetElusive(client)
{
  return g_bElusive[client];
}

GetPoisoned(client)
{
  return g_iPoisoned[client];
}

SetPoisoned(client, amount)
{
  g_iPoisoned[client]=amount;
}

SetBlessedResillience(client, bool:active)
{
  g_bBlessedRes[client]=active;
}

GetBlessedResillience(client)
{
  return g_bBlessedRes[client];
}

public SetHolyShock(client, bool:active)
{
  g_bAutocastTalents[client][0]=active;
}

public GetHolyShock(client)
{
  return g_bAutocastTalents[client][0];
}

public GetThunder(client,shock)
{
  return g_iRollingThunder[client][shock];
}

public SetThunder(client,shock,amount)
{
  g_iRollingThunder[client][shock]=amount;
}

public SetReckoning(client, attacks)
{
  g_iReckoning[client]=attacks;
}

public GetReckoning(client)
{
  return g_iReckoning[client];
}

public SetReflectiveShield(target, caster)
{
  g_iReflectiveShield[target]=caster;
}

public GetReflectiveShield(client)
{
  return g_iReflectiveShield[client];
}

public SetBoW(client, cast)
{
  g_iWisdomClient[client]=cast;
}

public SetBoM(client, Float:cast)
{
  g_fMightClient[client]=cast;
}

public SetPWF(client, cast)
{
  g_iFortCast[client]=cast;
}

public GetInspiration(client)
{
  return g_iInspiration[client];
}

public GetColdBlood(client)
{
  return ColdBlood[client];
}

public SetColdBlood(client, bool:blood)
{
  ColdBlood[client]=blood;
}

public GetAdrenaline(client)
{
  return Adrenaline[client];
}

public SetAdrenaline(client, bool:adrenaline)
{
  Adrenaline[client]=adrenaline;
  if(adrenaline)  {
    new Float:locationSound[3];
    GetClientAbsOrigin(client,locationSound);
    EmitAmbientSound("wc/spells/adrenalinerush.wav",locationSound);
  }
}

public GetSweepingStrikes(client)
{
  return g_iSweepingStrikes[client];
}

public SetSweepingStrikes(client, count)
{
  g_iSweepingStrikes[client]=count;
}

public GetEarthShield(client)
{
  return g_iEarthShield[client][0];
}

public GetEarthShieldCount(client)
{
  return g_iEarthShield[client][1];
}

SetEarthShieldCount(client,amount)
{
  g_iEarthShield[client][1]=amount;
}

//public GetDeathWish(client)
//{
//  return g_iDeathWish[client];
//}

/*public GetElementalFury(client)
{
  return g_iElementalFury[client];
}*/

public GetShadowform(client)
{
  return g_bShadowform[client];
}

SetShadowform(client, bool:value)
{
  g_bShadowform[client]=value;
}

public GetEnrage(client)
{
  return g_iEnrage[client];
}

public SetEnrage(client, stacks)
{
  g_iEnrage[client]=stacks;
}

public GetFlurry(client)  {
  return g_iFlurry[client];
}

public SetFlurry(client, amount) {
  g_iFlurry[client]=amount;
}

stock SetDualSpecClip(client, Float:val)  {
  g_fDualWieldSpecc[client]=val;
}

public Float:GetDualWieldSpecc(client)
{
  return g_fDualWieldSpecc[client];
}

public wcAddStaticTalent(treeid, tierid, talentid, client, spec)
{
  g_iClientTalents[client+(64*spec)][treeid][tierid][talentid]+=1;
}

public wcAddTalent(treeid, tierid, talentid, client)
{
  g_iClientTalents[client+(getActiveSpec(client)*64)][treeid][tierid][talentid]+=1;
}

new g_ff_tpoints[65][2];
getTPoints(client, spec)
{
  return g_ff_tpoints[client][spec];
}

checkTalentPoints(client) {
  new shouldHave = getMaxTalents(GetLevel(client));
  for(new spec=0;spec<2;spec++) {
    new ttpoints=getTPoints(client, spec);
    ttpoints+=wcGetTreeTotal(client, spec, 1)+wcGetTreeTotal(client, spec, 2)+wcGetTreeTotal(client, spec, 3);

    if(ttpoints > shouldHave) {
      new diff = ttpoints - shouldHave;
      if(getTPoints(client, spec) < diff) {
        wcResetTalents(client, spec, true); //true to prevent weapon loss since it's on loading
        LogMessage("fixed talent points for character %d spec %d. With reset of talents", characterids[client][0], spec);
      }

    }
    if(ttpoints < shouldHave)
    {
      shouldHave-= wcGetTreeTotal(client, spec, 1)+wcGetTreeTotal(client, spec, 2)+wcGetTreeTotal(client, spec, 3);
      setTPoints(client,spec,shouldHave);
      LogMessage("fixed talent points for character %d spec %d. Too few", characterids[client][0], spec);
    }
    setTPoints(client, spec, getTPoints(client, spec));
  }
}

setTPoints(client, spec, points)
{
  new shouldHave = getMaxTalents(GetLevel(client));
  new ttpoints=wcGetTreeTotal(client, spec, 1)+wcGetTreeTotal(client, spec, 2)+wcGetTreeTotal(client, spec, 3);

  new apply = shouldHave - ttpoints;
  if (apply < 0)  {
    apply = 0;
  }

  if (points+ttpoints != shouldHave && !IsFakeClient(client)) {
    LogError("[WC] Trying to set wrong amount of points. Client %d, characterId %d, spec = %d. Must %d, setting %d. Correcting to %d",
            client, characterids[client][0], spec, shouldHave, points + ttpoints, apply);
  }
  g_ff_tpoints[client][spec] = apply;
}



stock wcGetTreeTotal(client, spec, tree)
{
  decl String:buffer[255];

  getClientTalents_string(client, spec, buffer, 255);
  new treetotal = 0;
  for(new i=0; i<10; i++)
  {
    for(new p=0; p<10; p++)
    {
      treetotal+=g_iClientTalents[client+(64*spec)][tree][i][p];
    }
  }
  return treetotal;
}

stock wcGetTalentLevel(client, tree, tier, talent, spec=-1)
{
  if(spec==-1)
    spec = getActiveSpec(client);
  return g_iClientTalents[client+spec*64][tree][tier][talent];
}

wcResetTalents(client, spec, bool:careful=false)
{
  new ttpoints=getTPoints(client, spec);
  ttpoints+=wcGetTreeTotal(client, spec, 1)+wcGetTreeTotal(client, spec, 2)+wcGetTreeTotal(client, spec, 3);
  for(new j=0; j<10; j++)
  {
    for(new i=0; i<10; i++)
    {
      for(new p=0; p<10; p++)
      {
        g_iClientTalents[client+(64*spec)][j][i][p]=0;
      }
    }
  }
  setTPoints(client, spec,ttpoints);
  if(!careful)
    if( (GetClass(client)==6 || GetClass(client)==8 ) && HasDual(client))
      PutIntoInventory(client,18);

}

public CheckRequirements(client, race, tree, tier, talent)
{
  if(!IsClientInGame(client))
    return false;
  if(IsZombie(client))
    return false;
  if(GetClass(client) != race)
    return false; //Wrong class talent
  decl String:line[255];
  wcGetTalentInfo(race,tree,tier,talent,client,"active",line);
  new active = StringToInt(line);
  wcGetTalentInfo(race,tree,tier,talent,client,"nomsg",line);
  new nomsg = StringToInt(line);
  if (wcGetTalentLevel(client, tree, tier, talent)>=1)
  {

    if(!IsPlayerAlive(client) && active >= 1)
    {
      WCMessage(client,"you are dead",client);
      return false;
    }
    //Check if it is managable
    decl String:sManagable[255];
    wcGetTalentInfo(race,tree,tier,talent,client,"controlled",sManagable);
    if(StringToInt(sManagable)>=1)  {
      //Get talent cast id
      new talentId = getTalentUniqueId(race,tree,tier,talent);

      if(talentId <= 0) {
        LogError("[WC] Error in talentIsRecommended logics debug info: %d %d %d %d",
          race, tree, tier, talent);
        return false;
      }
      new bool:allow = castIsRecommended(client, talentId * -1);
      if(!allow)  {
        return false;
      }
    }
    if(GetSilence(client)<=GetTickedTime() && active >= 1)
    {
      new manareq = GetTalentCost(client, tree,tier,talent);
      new mana=GetMana(client);
      wcGetTalentInfo(race,tree,tier,talent,client,"cooldowntype",line);
      new type=StringToInt(line);
      new Float:timeleft=GetCDSpell(client,type)-GetTickedTime();
      if(timeleft<=0.0)
      {
        if(mana>=manareq)
        {
          SetCasting(client,talent,0.0);
          return true;
        }
        else if (active ==1)
        {
          WCMessage(client,"no mana",client);
          return false;
        }
        else
          return false;
      }
      else if( active ==1)
      {
        if(nomsg <= 0)
          WCMessage(client,"spell on cd",client,timeleft);
        return false;
      }
      else
        return false;
    }
    else
    {
      if(active == 0)
        return true;
      else
        WCMessage(client,"silenced for",client,(GetSilence(client)-GetTickedTime()));
      return false;
    }
  }
  else
  {
    if(active == 1)
      WCMessage(client,"no talent",client);
    return false;
  }
}

wcRunOnBlockTalents(victim, attacker)
{
  new class = GetClass(victim);
  switch(class)
  {
    case 1:
    {
      if(CheckRequirements(victim,1,1,2,2))
        wcRunTalent_1_FocusedRage(victim);
      if(CheckRequirements(victim,1,1,3,2))
        wcRunTalent_1_ManoAMano(victim,attacker);
    }
    case 4:
    {
      if((HasGlyph(victim, GLYPH_PALADIN_SHIELD)) && CheckRequirements(victim,4,2,2,4))
        wcRunTalent_4_HolyShieldGlyph(victim,attacker);
    }
  }
}

wcRunOnDeathTalents(victim,attacker)
{
  if(attacker!=victim)
  {
    new class = GetClass(victim);
    switch(class)
    {
      case 5:
      {
        if(CheckRequirements(victim,5,2,3,3))
          wcRunTalent_5_SoR(victim);
      }
      case 6:
      {
        if(CheckRequirements(victim,6,3,3,3))
          wcRunTalent_6_Earthgrip(victim,attacker);
      }
    }
    class = GetClass(attacker);
    switch(class)
    {
      case 1:
      {
        if(CheckRequirements(attacker,1,2,3,3)) //Battle Rush
          wcRunTalent_1_BattleRush(attacker); //Battle Rush
      }
      case 2:
      {
        //if(CheckRequirements(attacker,2,2,4,1))
          //wcRunTalent_2_KillingSpree(attacker);
        //if(CheckRequirements(attacker,2,3,3,2))
          //wcRunTalent_2_MurderousIntent(attacker);
        if(HasGlyph(attacker, GLYPH_ROGUE_SSTEP))
          SetCdSpell(attacker,11,0.0,0,"Shadowstep");
      }
      case 5:
      {
        if(CheckRequirements(attacker,5,3,1,1))
          wcRunTalent_5_12(attacker);
      }
      case 7:
      {
       if(CheckRequirements(attacker,7,1,2,1))
        wcRunTalent_7_4(attacker);
      }
      case 8:
      {
        if(CheckRequirements(attacker,8,1,3,2)) //Butchery
          wcRunTalent_Butchery(attacker);
      }
      case 9:
      {
        if(CheckRequirements(attacker,9,2,2,1)) // Afterlife
            wcRunTalent_9_Afterlife(attacker,victim);
      }
    }
  }
}

public wcRunPassiveTalents(client)
{
  if(IsZombie(client))
    return;
  new class = GetClass(client);
  new bool:noisy = IsChat(client,Chat:Chat_Spawn);

  switch(class)
  {
    case 1:
    {
      //rage
      if(CheckRequirements(client,1,3,3,3))
        wcRunTalent_1_ControlledRage(client, noisy); //Controlled Rage :P /wcmessage

      //str
      if(CheckRequirements(client,1,3,2,2))
        wcRunTalent_1_11(client, noisy); //Juggernaut /wcmessage

      //armor
      if(CheckRequirements(client,1,1,1,2))
        wcRunTalent_1_4(client,class,noisy); //Toughness /wcmessage
      if(CheckRequirements(client,1,1,4,2))
        wcRunTalent_1_Heavy(client,noisy); //Heavy Repercussion /wcmessage

      //DPS
      if(CheckRequirements(client,1,2,1,2))
        wcRunTalent_1_6(client,noisy); //Dual Wield Specc /wcmessage
      if(CheckRequirements(client,1,3,2,3))
        wcRunTalent_1_12(client,noisy); //Armored to the Teeth /wcmessage

      //crit
      if(CheckRequirements(client,1,3,1,1))
        wcRunTalent_1_10(client,noisy); //Two Handed Specc /wcmessage

    }
    case 2:
    {
      if(CheckRequirements(client,2,1,1,3))
        wcRunTalent_2_5(client); //Enveloping Shadows
      if(CheckRequirements(client,2,1,2,1))
        wcRunTalent_2_6(client); //Fleet Footed
      //if(CheckRequirements(client,2,2,1,2))
        //wcRunTalent_2_10(client); //Find Weakness
      if(CheckRequirements(client,2,2,2,1))
        wcRunTalent_2_9(client); //Aggression
      if(CheckRequirements(client,2,2,1,1))
        wcRunTalent_2_12(client); //Precision
      if(CheckRequirements(client,2,2,1,4))
        wcRunTalent_2_ReinforcedLeather(client);
      if(CheckRequirements(client,2,3,3,3))
        wcRunTalent_2_SwiftKiller(client);
    }
    case 3:
    {
      //Haste
      //if(CheckRequirements(client,3,3,2,3))
        //wcRunTalent_3_FocusedMind(client,noisy); //Focused Mind

      //Crit
      if(CheckRequirements(client,3,2,2,2))
        wcRunTalent_3_6(client); //Pyromanic

      //Armor
      //if(CheckRequirements(client,3,3,1,1))
        //wcRunTalent_3_7(client); //Arcane Fortitude

      //Effects
      if(CheckRequirements(client,3,3,2,4))
        wcRunTalent_3_MasterOfMagic(client);
      if(CheckRequirements(client,3,3,2,3))
        wcRunTalent_3_ArcanePower(client);
    }
    case 4:
    {
      if(CheckRequirements(client,4,1,1,3))
        wcRunTalent_4_4(client);  //Blessing of Wisdom
      if(CheckRequirements(client,4,1,2,3))
        wcRunTalent_4_5(client);  //Greater Blessing of Wisdom
      if(CheckRequirements(client,4,1,1,2))
        wcRunTalent_4_14(client); //Holy Guidance
      if(CheckRequirements(client,4,2,1,1))
        wcRunTalent_4_6(client);  //Divine Stamina
      if(CheckRequirements(client,4,2,1,3))
        wcRunTalent_4_7(client);  //Devotion Aura
      if(CheckRequirements(client,4,2,2,2))
        wcRunTalent_4_8(client);  //Divine Favor
      if(CheckRequirements(client,4,3,1,3))
        wcRunTalent_4_10(client); //Blessing of Might
      if(CheckRequirements(client,4,3,2,1))
        wcRunTalent_4_11(client); //Greater Blessing of Might
      if(CheckRequirements(client,4,3,2,2))
        wcRunTalent_4_12(client); //Sheath of Light
      if(CheckRequirements(client,4,3,1,2))
        wcRunTalent_4_13(client); //Eye for an eye
      if(CheckRequirements(client,4,1,4,1))
        wcRunTalent_4_BeaconOfLight(client);
      //if(CheckRequirements(client,4,2,4,1))
        //wcRunTalent_4_DivineGuardian(client);
      if(CheckRequirements(client,4,3,1,4))
        wcRunTalent_4_SpeedOfLight(client);
    }
    case 5:
    {
      if(CheckRequirements(client,5,1,1,1))
        wcRunTalent_5_4(client,noisy); //Inner Fire /WCMessage
      if(CheckRequirements(client,5,1,1,2))
        wcRunTalent_5_5(client,noisy); //Meditation /WCMessage
      if(CheckRequirements(client,5,1,3,2))
        wcRunTalent_5_6(client,noisy); //Mental Strength /WCMessage
      if(CheckRequirements(client,5,1,2,3))
        wcRunTalent_5_7(client); //Prayer of Fortitude /WCMessage
    }
    case 6:
    {
      if(CheckRequirements(client,6,2,1,1))
        wcRunTalent_6_7(client); //Thundering Strikes
      if(CheckRequirements(client,6,2,1,3))
        wcRunTalent_6_8(client); //Weapon Mastery
      if(CheckRequirements(client,6,3,1,2))
        wcRunTalent_6_11(client); //Restorative Casting
      if(CheckRequirements(client,6,3,1,3))
        wcRunTalent_6_12(client); //Natures Protection
      //if(CheckRequirements(client,6,2,2,3))
        //wcRunTalent_6_2(client); //Dual Wielding
      if(CheckRequirements(client,6,1,2,1))
        wcRunTalent_6_NaturesPower(client);
      if(CheckRequirements(client,6,2,1,4))
        wcRunTalent_6_FeralForm(client);
      if(CheckRequirements(client,6,2,4,2))
        wcRunTalent_6_AncestorsProtection(client); //Natures Protection
    }
    case 7:
    {
      if(CheckRequirements(client,7,2,4,2))
        wcRunTalent_7_7(client); //Demonic Knowledge
      if(CheckRequirements(client,7,2,2,3))
        wcRunTalent_7_DemonsBlood(client);  //Demon's Blood
      if(CheckRequirements(client,7,3,1,2))
        wcRunTalent_7_8(client); //Devestation
      if(CheckRequirements(client,7,3,2,3))
        wcRunTalent_7_9(client); //Molten Skin
      /*if(CheckRequirements(client,7,2,3,3))
        wcRunTalent_7_ReceptiveBlood(client);*/
      if(wcGetTalentLevel(client,2,2,1)>0) {
        decl String:line[255];
        wcGetTalentInfo(7,2,2,1,client,"stacks",line);
        new souls = StringToInt(line);
        SetHarvest(client, souls);
        WCMessage(client,"free soul",client,souls);
        }
    }
    case 8:
    {
      if(CheckRequirements(client,8,1,1,2))
        wcRunTalent_1_4(client,class,noisy); //Toughness /wcmessage
      if(CheckRequirements(client,8,2,3,3))
        wcRunTalent_8_CuttingChill(client);
      if(CheckRequirements(client,8,3,1,4))
        wcRunTalent_8_InateResistance(client);
    }
    case 9:
    {
      if(CheckRequirements(client,9,1,1,2))
        wcRunTalent_9_AgileArmor(client); //Agile Armor
      if(GetMonkStance(client)==3)
        SetHealingBonus(client, GetHealingBonus(client)+0.25);
      if(CheckRequirements(client,9,1,1,4))
        wcRunTalent_9_1_1_4(client); //Parry Control

    }
  }
}

public wcRunCritBonusChanceTalents(attacker,victim,&Float:critbonus, String:spellname[])
{
  new class=GetClass(attacker);
  decl String:line[255];
  switch(class)
  {
    case 3:
    {
      if(GetClientSpeed(victim)<=0.1)
      {
        wcGetTalentInfo(3,1,2,1,attacker,"increment",line); //Mage-Frost-T2-Shatter
        critbonus+=StringToFloat(line);
      }
//      if(CheckRequirements(attacker,3,2,3,3))
//        critbonus+=wcRunTalent_3_CriticalMass(attacker);
    }
    case 4:
    {
      if(6==wcGetSpellIDFromName(class,spellname))
      {
        wcGetTalentInfo(4,3,1,1,attacker,"increment",line);
        critbonus=StringToFloat(line);
        if(HasGlyph(attacker, GLYPH_PALADIN_CONVICTION))
        {
          new Float:currenthealth = float(wcGetHealth(victim)) / float(GetMaxHealth(victim));
          if(0.35 >= currenthealth)
            critbonus*=5.0;
        }
      }
    }
  }
  return true;
}

public wcGetTalentCritDamageBonus(attacker,String:spellname[],&Float:critbonusdamage,bool:spell)
{
  if(IsZombie(attacker))  return false;
  new class=GetClass(attacker);
  decl String:line[255];
  switch(class)
  {
    case 1:
    {
      if(CheckRequirements(attacker,1,3,2,1) && !spell)
        wcRunTalent_1_13(attacker,critbonusdamage);
    }
    case 2:
    {
      if(CheckRequirements(attacker,2,3,2,4))
        wcRunTalent_2_11(attacker,critbonusdamage);
    }
    case 3:
    {
      if (1 == wcGetSpellIDFromName(class,spellname))
      {
        wcGetTalentInfo(3,1,1,3,attacker,"multi",line);
        critbonusdamage+=StringToFloat(line);
      }
    }
    case 7:
    {
      if(spell)
      {
        if(CheckRequirements(attacker,7,3,2,2))
        {
          wcGetTalentInfo(7,3,2,2,attacker,"multi",line);
          critbonusdamage+=StringToFloat(line);
        }
      }
    }
    case 8:
    {
      if(CheckRequirements(attacker,8,2,2,3))
        wcRunTalent_8_Thassarian(attacker,critbonusdamage);
    }
  }
  return true;
}

public wcRunTalentSpellCrit(attacker,victim,String:spellname[],&damage,&Float:damagemulti,bool:aoe)
{
  if(IsZombie(attacker))  return;
  new class=GetClass(attacker);
  new spellid = wcGetSpellIDFromName(class, spellname);
  switch(class)
  {
    case 3:
    {
      wcRunTalent_3_10(attacker,spellid);
      if((spellid == 3 || spellid == 4) && wcGetTalentLevel(attacker, 2, 4, 2) > 0)
      {
        wcRunTalent_3_11(attacker,victim,damage,damagemulti);
      }
      if((StrEqual(spellname,"Living Bomb",false)) && HasGlyph(attacker, GLYPH_MAGE_IGNITE))
      {
        wcRunTalent_3_11(attacker,victim,damage,damagemulti);
      }
      
      if(spellid == 1 && CheckRequirements(attacker,3,1,1,4))
        wcRunTalent_3_PiercingChills(attacker,victim);
      if(spellid == 3 && CheckRequirements(attacker,3,2,2,4))
        wcRunTalent_3_SearingFlames(attacker,victim);
      if(CheckRequirements(attacker,3,3,3,3))
        wcRunTalent_3_ArcanePotency1(attacker, damage);
      if(StrEqual(spellname,"Arcane Missiles",false) && CheckRequirements(attacker,3,3,4,2))
        wcRunTalent_3_UnstableMagic(attacker);
    }
    case 7:
    {
      if(CheckRequirements(attacker,7,3,3,3) && !aoe)
        wcRunTalent_7_Flameburst(attacker,victim,damage);
    }
/*    case 8:
    {
      if(StrEqual(spellname,"Plague Strike",false)) {
        SetBones(attacker, 5);
      }
    } */
    case 9:
    {
      if(CheckRequirements(attacker,9,2,2,2))
        wcRunTalent_9_SSS(attacker);
    }
  }
}

public wcRunTalentPhysicalCrit(attacker,victim,&Float:damagemulti, &damage)
{
  new class=GetClass(attacker);
  if(!IsZombie(attacker))  switch(class)
  {
    case 1:
    {
      if(CheckRequirements(attacker,1,2,2,2))
        wcRunTalent_1_9(attacker);
      if(CheckRequirements(attacker,1,3,3,2))
        wcRunTalent_1_DeepWounds(attacker,victim,damage);
    }
    case 6:
    {
      if(CheckRequirements(attacker,6,2,2,1))
        wcRunTalent_6_9(attacker);
      if(CheckRequirements(attacker,6,2,3,3))
        wcRunTalent_6_StormStrike(attacker,victim);
    }
    case 9:
    {
      if(CheckRequirements(attacker,9,2,2,2))
        wcRunTalent_9_SSS(attacker);
      if(CheckRequirements(attacker,9,1,2,2))
        wcRunTalent_9_CritParry(attacker);
    }
  }
  class=GetClass(victim);
  if(!IsZombie(victim))  switch(class)
  {
    case 5:
      if(CheckRequirements(victim,5,2,2,3))
        wcRunTalent_5_9(victim);
  }
}

public wcRunTalentHit(attacker,victim,String:source[],bool:spell,&damage,prio,olddmg, bool:crit, bool:pass) //onhit, onphysical, physical, ondamage
{
  new aclass = GetClass(attacker);
  new vclass = GetClass(victim);
  if(IsZombie(attacker))
  {
    new owner = GetZombie(attacker);
    if(GetClass(owner)==8 && wcGetTalentLevel(owner,3,4,2)>0 && prio==1) {
      wcRunTalent_GhoulGrip(attacker,victim);
      wcRunTalent_Cleave(attacker,victim);
    }
  }
  if(!IsZombie(attacker) && pass)  switch(aclass)
  {
    case 1:
    {
      if(CheckRequirements(attacker,1,2,1,1) && prio ==1 && !spell)
        wcRunTalent_1_5(attacker); //Unbridled Wrath
      if(CheckRequirements(attacker,1,2,1,3) && prio ==1 && !spell)
        wcRunTalent_1_7(attacker,victim);
      if(CheckRequirements(attacker,1,2,3,1) && prio ==1 && !spell)
        wcRunTalent_1_2(attacker,damage);
      if(CheckRequirements(attacker,1,3,4,1) && prio ==1 && !spell)
        wcRunTalent_1_Execute(attacker,victim,damage);
      if(CheckRequirements(attacker,1,3,4,2) && prio ==1 && !spell)
        wcRunTalent_1_ColossusSmash(attacker,victim,damage);
      if(CheckRequirements(attacker,1,3,3,2) && !spell && HasGlyph(attacker, GLYPH_WARRIOR_DEEPWOUNDS))
      {
        new Float:random=GetRandomFloat(0.0,1.0);
        if(GetCriticalStrike(attacker)>random)
          wcRunTalent_1_DeepWounds(attacker,victim,damage);
      }
    }

    case 2:
    {
      if(CheckRequirements(attacker,2,1,1,2) && prio ==1 && !spell)
        wcRunTalent_2_4(attacker);
      /*if(CheckRequirements(attacker,2,1,2,3) && prio ==2 && !spell)
        wcRunTalent_2_7(attacker,damage);*/
      //if(CheckRequirements(attacker,2,2,2,2) && prio ==1 && !spell)
        //wcRunTalent_2_8(attacker,victim,olddmg,source);
      /*if(CheckRequirements(attacker,2,2,3,2) && prio ==1 && !spell)
        wcRunTalent_2_BladeTwisting(attacker,victim);*/
      if(CheckRequirements(attacker,2,2,3,3) && prio ==1 && !spell)
        wcRunTalent_2_CombatReadiness(attacker);
      if(CheckRequirements(attacker,2,2,4,1) && prio == 1 && !spell)
        wcRunTalent_2_BasicMaster(attacker,victim,damage);
    }
    case 3:
    {
      if(CheckRequirements(attacker,3,1,2,3) && prio ==3)
        wcRunTalent_3_4(attacker,victim,source);
      if(CheckRequirements(attacker,3,2,2,1) && prio ==2 && spell)
        wcRunTalent_3_5(attacker,victim,damage);
      if(CheckRequirements(attacker,3,1,3,3) && prio ==1 && !spell)
        wcRunTalent_3_ColdSnap(attacker);
      if(CheckRequirements(attacker,3,3,3,3) && prio ==1 && spell && !crit)
        wcRunTalent_3_ArcanePotency2(attacker);
      if(CheckRequirements(attacker,3,3,3,2) && prio ==1 && !spell)
        wcRunTalent_3_ArcaneMissile(attacker,victim);
      if(CheckRequirements(attacker,3,1,4,2) && prio ==2 && spell)
        wcRunTalent_3_BrittleFlesh(attacker,damage);
      if(CheckRequirements(attacker,3,3,2,1) && prio ==1 && !spell)
        wcRunTalent_3_ArcaneBarrage(attacker, victim);
    }
    case 4:
    {
      if(CheckRequirements(attacker,4,1,3,1) && prio ==1 && !spell)
        wcRunTalent_4_1(attacker,victim);
      if(CheckRequirements(attacker,4,3,4,1) && prio == 2 && !spell)
        wcRunTalent_4_GrandCrussade(attacker,damage);
      if(g_iRighteousShield[attacker]>0)
        wcRunTalent_4_RighteousShieldActivate(attacker,victim,damage);
    }
    case 5:
    {
      if(CheckRequirements(attacker,5,3,2,3) && prio ==3 && spell)
        wcRunTalent_5_11(attacker,damage);
      if(CheckRequirements(attacker,5,3,1,4) && prio ==3 && spell && StrEqual(source,"Mind Blast",false))
        wcRunTalent_5_Paralyze(attacker,victim);
      if(CheckRequirements(attacker,5,3,3,2) && prio ==3 && spell)
        wcRunTalent_5_VampiricTouch(attacker,damage);
      if(CheckRequirements(attacker,5,3,3,3) && prio ==3)
        wcRunTalent_5_Silence(attacker,victim);
    }
    case 6:
    {
      if(CheckRequirements(attacker,6,2,2,2) && prio ==1 && !spell)
        wcRunTalent_6_10(attacker);
      if(CheckRequirements(attacker,6,1,4,2) && prio ==1 && !spell)
        wcRunTalent_6_AirShock(attacker,victim);
      if(CheckRequirements(attacker,6,2,4,1) && prio ==1 && !spell)
        wcRunTalent_6_LavaSurge(attacker,victim);
      if(CheckRequirements(attacker,6,3,4,1) && prio ==1 && !spell)
        wcRunTalent_6_NaturesAvatar(attacker);
      if(CheckRequirements(attacker,6,1,3,2) && prio ==1 && spell)
        wcRunTalent_6_MaelstromWeapon(attacker,victim,damage,source);
      if(CheckRequirements(attacker,6,1,3,1) && prio ==1 && !spell && g_iFlameShock[victim]>=1)
        wcRunTalent_6_LavaBurst(attacker,victim);
    }
    case 7:
    {
      if(CheckRequirements(attacker,7,1,2,2) && prio ==3 && spell && StrEqual(source,"Corruption",false))
      {
        wcRunTalent_7_10(attacker,damage);
        if(CheckRequirements(attacker,7,1,3,1))
          wcRunTalent_7_1(attacker,victim);
      }
      if(CheckRequirements(attacker,7,3,2,4) && prio ==3 && spell)
        wcRunTalent_7_Shock(attacker,victim,damage);
      if(CheckRequirements(attacker,7,3,3,2) && prio ==1 && !spell)
        wcRunTalent_7_ShadowTouch(attacker,victim);
    }
    case 8:
    {
      if(CheckRequirements(attacker,8,2,3,2) && prio ==1 && !spell)
        wcRunTalent_FrostStrike(attacker,victim);
      if(CheckRequirements(attacker,8,2,2,4) && prio ==2)
        wcRunTalent_8_FotG(attacker,damage);
      if(CheckRequirements(attacker,8,1,4,2) && prio == 1 && !spell)
        wcRunTalent_8_BloodParasites(attacker,victim);
      if(CheckRequirements(attacker,8,2,4,2) && prio == 1 && !spell)
        wcRunTalent_8_Obliterate(attacker,victim,damage);
    }
    case 9:
    {
      if(GetMonkStance(attacker)==3 && prio==1)
        wcRunTalent_9_Mistweaver(attacker,damage);
      if(CheckRequirements(attacker,9,2,4,2) && prio == 1 && !spell)
        wcRunTalent_9_ChiExplosion(attacker,victim);
      if(CheckRequirements(attacker,9,1,2,3) && prio == 1 && !spell)
        wcRunTalent_9_KegSmash(attacker, victim);
    }
  }
  if(!IsZombie(victim))  switch(vclass)
  {
    case 1:
    {
      if(CheckRequirements(victim,1,2,2,1) && prio == 1)
        wcRunTalent_1_8(victim);
      if(CheckRequirements(victim,1,2,3,2) && prio == 1)
        wcRunTalent_1_BloodCrazed(victim);
    }
    case 2:
    {
      //if(CheckRequirements(victim,2,2,2,4) && prio ==2 && !spell)
        //wcRunTalent_2_LightningReflexes(victim,damage);
    }
    case 3:
    {
      if(CheckRequirements(victim,3,2,1,4) && prio ==1)
        wcRunTalent_3_BurningSoul(victim);
      if(CheckRequirements(victim,3,2,1,3) && prio ==1)
        wcRunTalent_3_BlazingSpeed(victim);
    }
    case 4:
    {
      if(CheckRequirements(victim,4,1,1,4) && prio==1 && crit)
        wcRunTalent_4_BlessedLife(victim);
      if(CheckRequirements(victim,4,2,3,3) && prio==3)
        wcRunTalent_4_DivineInt(victim, damage);
      if(CheckRequirements(victim,4,3,3,2) && prio==1)
        wcRunTalent_4_VengefulWarrior(victim,attacker);
    }
    case 5:
    {
      if(CheckRequirements(victim,5,1,2,2) && prio==3)
        wcRunTalent_5_FocusedWill(victim,damage);
      if(CheckRequirements(victim,5,2,3,2) && prio==3)
        wcRunTalent_5_DesperatePrayer(victim,damage);
    }
    case 6:
    {
      if(CheckRequirements(victim,6,1,3,3) && prio==2 && spell)
        wcRunTalent_6_Guardian(victim,damage);
      if(CheckRequirements(victim,6,2,2,3) && prio==2 && !spell)
        wcRunTalent_6_CounterStrike(victim,attacker,damage);
      if(CheckRequirements(victim,6,3,2,4) && prio==1)
        wcRunTalent_6_Watershield(victim);
      if(CheckRequirements(attacker,6,3,1,4) && prio ==1 && !spell)
        wcRunTalent_6_NaturesGrace(victim);
    }
    case 7:
    {
      if(CheckRequirements(victim,7,2,2,4) && prio ==1)
        wcRunTalent_PainAbsorbtion(victim);
      /*if(CheckRequirements(victim,7,2,3,2) && prio ==3 && !spell)
        wcRunTalent_Fearsome(victim,attacker);*/
    }
    case 8:
    {
      if(CheckRequirements(victim,8,3,3,3) && prio ==3 && !spell)
        wcRunTalent_BoneShield(victim,damage);
      if(CheckRequirements(victim,8,1,1,1) && prio ==2 )
        wcRunTalent_BladeBarrier(victim,damage);
      if(CheckRequirements(victim,8,1,2,4) && prio ==1)
        wcRunTalent_8_Bloodtrail(victim);
      if(CheckRequirements(victim,8,1,2,1) && prio ==2 && !spell) //Rune Warding
        wcRunTalent_8_RuneWarding(victim,damage);
      if(CheckRequirements(victim,8,1,2,3) && prio ==3)
        wcRunTalent_8_PowerOfBlood(victim,damage);
      if(CheckRequirements(victim,8,2,2,2) && prio ==3 && crit)
        wcRunTalent_8_ArmorOfFrost(victim,attacker);
      if(CheckRequirements(victim,8,3,3,2) && prio ==1 && spell && crit)
        wcRunTalent_8_UnholySurge(victim,attacker);
    }
    case 9:
    {
      //if(GetMonkStance(victim)==1 && prio==2)
        //wcRunTalent_9_Brewmaster(victim);
      if(CheckRequirements(victim,9,1,4,2) && prio ==2)
        wcRunTalent_9_DampenedHarm(victim,damage);
      if(CheckRequirements(victim,9,1,3,3) && prio ==2)
        wcRunTalent_9_HealingBrew(victim);
    }
  }
  if(IsZombie(victim) && GetZombieType(victim)==2)
  {
    if(CheckRequirements(GetZombie(victim),7,2,4,1) && prio==3)
      wcRunTalent_7_SpiritLink(GetZombie(victim),damage);
  }
}

public wcRunTalent_1_stun(client) //Concussion Blow
{
  new target=GetAimTarget(client,25.0);
  if(target<=0) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"no target",client);
    return;
  }
  if((GetClientTeam(client)==GetClientTeam(target)) || !IsPlayerAlive(target)) {
    if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
    return;
  }


  decl String:line[255];
  wcGetTalentInfo(1,1,2,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(1,1,2,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 1,2,1);
  wcGetTalentInfo(1,1,2,1,client,"duration",line);
  new Float:duration=StringToFloat(line);

  duration *= float(GetSta(client)+GetBonusSta(client)) / float(GetDamage(client));
  if(duration > 1.5)
    duration = 1.5;

  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Concussion Blow");

  new Float:oldspeed=GetClientSpeed(target);
  new Float:newspeed=0.0;

  {
    SetClientSpeed(target,newspeed);
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,target); // Write the client index of owner
    WritePackFloat(datapack2,newspeed);
    WritePackFloat(datapack2,oldspeed);
    CreateTimer(duration,RemoveFreezeEffect,datapack2,TIMER_HNDL_CLOSE);

    decl String:name[50],String:name2[50];
    GetClientName(client,name,50);
    GetClientName(target,name2,50);
    WCMessage(target, "Concussion Blow target", target, name, duration);
    WCMessage(client, "Concussion Blow cast", client, name2, duration);
  }
}

public wcRunTalent_1_1(client) //Last Stand
{
  new manareq = GetTalentCost(client, 1,3,1);
  if(GetMana(client) >= manareq)
  {
    decl String:line[255];
    wcGetTalentInfo(1,1,3,1,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(1,1,3,1,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    wcGetTalentInfo(1,1,3,1,client,"duration",line);
    new Float:duration=StringToFloat(line);
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client,cdtype,cd, 0, "Last Stand");
    wcGetTalentInfo(1,1,3,1,client,"multi",line);
    new Float:multi = StringToFloat(line);
    new bhealth = RoundToNearest(multi*float(GetMaxHealth(client)));
    new thealth=GetBonusMaxHealth(client)+bhealth;
    new oldhp=GetBonusMaxHealth(client);
    SetBonusMaxHealth(client, thealth);
    wcRecalculateStats(client);
    wcHeal(client,client,bhealth,0,false);
    WCMessage(client,"Activate Last Stand",client,bhealth,duration);
    new Handle:datapack4=CreateDataPack();
    WritePackCell(datapack4,client);
    WritePackFloat(datapack4,multi);
    WritePackCell(datapack4,g_RoundCount);
    WritePackCell(datapack4, oldhp);
    CreateTimer(duration,EndLastStand,datapack4,TIMER_HNDL_CLOSE);
  }
}

public Action:EndLastStand(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  new Float:multi=ReadPackFloat(datapack);
  new round = ReadPackCell(datapack);
  new oldhp = ReadPackCell(datapack);
  if(!IsZombie(client))
  {
    if(round==g_RoundCount)
    {
      SetBonusMaxHealth(client, oldhp);
      wcRecalculateStats(client);
      if(GetClass(client)==1)
        WCMessage(client,"Deactivate Last Stand",client);
      multi*=-1;
      RestoreHealthP(client,multi);
    }
  }
}

public wcRunTalent_1_2(client, &damage) //Raging Blow
{
  decl String:line[255];
  wcGetTalentInfo(1,2,3,1,client,"chance",line);
  new chance = StringToInt(line);
  wcGetTalentInfo(1,2,3,1,client,"mana",line);
  new mana = StringToInt(line);
  new random = GetRandomInt(1,100);
  if ( (chance >= random) && (GetEnrage(client)>0) && HasDual(client) && (mana <= GetMana(client)) )
  {
    wcGetTalentInfo(1,2,3,1,client,"multi",line);
    new Float:multi = StringToFloat(line);
    if(HasGlyph(client, GLYPH_WARRIOR_RAGING))
    {
      multi*1.3;
      g_iEnrage[client]-=1;
    }
    damage += RoundToNearest(multi*GetDamage(client));
    /*Format(line,sizeof(line),"%T", "raging blow", client);*/
    AddEMsgAff("raging blow", true);
    wcSetMana(client,GetMana(client)-mana);
  }
}

public wcRunTalent_1_3(client) //Sweeping Strikes
{
  decl String:line[255];
  wcGetTalentInfo(1,3,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(1,3,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 3,3,1);
  wcGetTalentInfo(1,3,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);

  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Sweeping Strikes");
  wcGetTalentInfo(1,3,3,1,client,"multi",line);
  new Float:multi = StringToFloat(line);
  wcGetTalentInfo(1,3,3,1,client,"count",line);
  new count = StringToInt(line);
  WCMessage(client,"Sweeping Strikes",client,(multi*100),count);

  g_iSweepingStrikes[client]=count;
  SetEffectAttacker(client,35);

  new Handle:datapack4=CreateDataPack();
  WritePackCell(datapack4,client);
  CreateTimer(duration,EndSweepingStrikes,datapack4,TIMER_HNDL_CLOSE);
}

public Action:EndSweepingStrikes(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  RemoveEffectAttacker(client,35);
}

stock wcRunTalent_1_4(client, race=1, bool:noisy) //Toughness
{
  decl String:line[255];
  wcGetTalentInfo(race,1,1,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(race,1,1,2,client,"base",line);
  new base=StringToInt(line);
  new Float:bonus = float(GetArmor(client))*multi+base;
  SetBonusArmor(client, float(GetBonusArmor(client)) + bonus);
  wcRecalculateStats(client);
  if(noisy)
    WCMessage(client,"toughness used", client, RoundToNearest(bonus), GetArmor(client));
}

stock wcRunTalent_1_Heavy(client,bool:noisy) //Heavy Repercussion
{
  decl String:line[255];
  wcGetTalentInfo(1,1,4,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  if(HasGlyph(client, GLYPH_WARRIOR_HEAVYREP))
  {
    new Float:bonus = float(GetResistance(client))*multi;
    wcClientsBonusStats[client][RESISTANCE]+=bonus;
    wcClientsStats[client][RESISTANCE]=wcClientsBonusStats[client][RESISTANCE];
    wcRecalculateStats(client);
    if(noisy)
      WCMessage(client,"heavy repercussions used glyph",client,RoundToNearest(bonus),GetResistance(client));
  }
  else
  {
    new Float:bonus = float(GetArmor(client))*multi;
    SetBonusArmor(client, float(GetBonusArmor(client)) + bonus);
    wcRecalculateStats(client);
    if(noisy)
      WCMessage(client,"heavy repercussions used",client,RoundToNearest(bonus),GetArmor(client));
  }
}


public wcRunTalent_1_5(client) //Unbridled Wrath
{
  decl String:line[255];
  new random=GetRandomInt(1,100);
  wcGetTalentInfo(1,2,1,1,client,"chance",line);
  new chance = StringToInt(line);
  if(chance>=random)
  {
    AddEMsgAffAttOnly("Unbridled Wrath name",true);
    wcGetTalentInfo(1,2,1,1,client,"mana",line);
    new restore=StringToInt(line);
    if(GetMana(client)+restore >= GetMaxMana(client))
      wcSetMana(client, GetMaxMana(client));
    else
      wcSetMana(client, GetMana(client)+restore);
  }
}


wcRunTalent_1_6(client,bool:noisy) { //Dual Wield Specc
  if(HasDual(client))  //Make sure offhand is weapon
  {
    decl String:line[255];
    wcGetTalentInfo(1,2,1,2,client,"dpsmulti",line);
    new Float:bonus = float(GetDamage(client))*StringToFloat(line);
    SetBonusDamage(client, float(GetBonusDamage(client)) + bonus );
    wcGetTalentInfo(1,2,1,2,client,"clipmulti",line);
    g_fDualWieldSpecc[client]=StringToFloat(line);
    wcRecalculateStats(client);
    if(noisy)
      WCMessage(client,"dual wield spec used",client, RoundToNearest(bonus), GetDamage(client));
  }
}

public wcRunTalent_1_7(client, &victim) //Furious Attacks
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetTalentInfo(1,2,1,3,client,"chance",line);
  new chance=StringToInt(line);
  if(chance>=random)
  if(HasDual(client))
  {
    wcGetTalentInfo(1,2,1,3,client,"multi",line);
    new Float:multi=1.0-StringToFloat(line);
    wcGetTalentInfo(1,2,1,3,client,"duration",line);
    new Float:duration=StringToFloat(line);
    SetHM(victim,multi);
    SetHMT(victim,duration);
    Format(line,sizeof(line),"%T", "furious attacks", client);
    AddEMsgAff("furious attacks" , true);
  }
}

public wcRunTalent_1_8(client) //Enrage
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetTalentInfo(1,2,2,1,client,"chance",line);
  new chance=StringToInt(line);
  if(chance>=random)
  {
    wcGetTalentInfo(1,2,2,1,client,"stacks",line);
    new stacks = StringToInt(line);
    wcGetTalentInfo(1,2,2,1,client,"duration",line);
    new Float:duration = StringToFloat(line);
    if (g_iEnrage[client]<stacks)
      {
        g_iEnrage[client]++;
        new Handle:datapack=CreateDataPack();
        WritePackCell(datapack,client);
        CreateTimer(duration,EndEnrage,datapack,TIMER_HNDL_CLOSE);
      }
    if(g_iEnrage[client]==1)
      {
        SetEffectAttacker(client,33);
        SetEffectSpellAtt(client,33);
        WCMessage(client,"enrage procc",client);
      }
  }
}

public Action:EndEnrage(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  if(g_iEnrage[client]==1)
  {
    RemoveEffectSpellAtt(client,33);
    RemoveEffectAttacker(client,33);
    WCMessage(client,"enrage ends",client);
  }
  g_iEnrage[client]--;
}

public wcRunTalent_1_9(client)                                                  //Flurry
{
  decl String:line[255];
  wcGetTalentInfo(1,2,2,2,client,"stacks",line);
  new max = StringToInt(line);
  if (max > g_iFlurry[client])
  {
    g_iFlurry[client]++;
    wcGetTalentInfo(1,2,2,2,client,"increment",line);
    new Float:talentbonus=StringToFloat(line);
    SetAttackSpeed(client, GetAttackSpeed(client)+talentbonus);
    wcRecalculateStats(client);
    wcGetTalentInfo(1,2,2,2,client,"duration",line);
    new Float:duration=StringToFloat(line);
    WCMessage(client,"warrior flurry procc",client,duration);
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,client);
    WritePackFloat(datapack,talentbonus);
    CreateTimer(duration,EndFlurry,datapack,TIMER_HNDL_CLOSE);
  }
}

public Action:EndFlurry(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  new Float:talentbonus=ReadPackFloat(datapack);
  g_iFlurry[client]--;
  SetAttackSpeed(client,GetAttackSpeed(client)-talentbonus);
  wcRecalculateStats(client);
}

wcRunTalent_1_10(client,bool:noisy) { //Two-Handed weapon spec.
  decl String:line[255];
  if(IsDoubleHand(client))
  {
    wcGetTalentInfo(1,3,1,1,client,"increment",line);
    new Float:bonus=StringToFloat(line);
    SetCriticalStrikeBonus(client, GetCriticalStrikeBonus(client)+bonus);
    if(HasGlyph(client, GLYPH_WARRIOR_TWOHWEP))
    {
      SetBonusAccuracy(client,GetBonusAccuracy(client)+bonus);
      wcRecalculateStats(client);
      if(noisy)
      {
        WCMessage(client,"2h weap spec used",client,bonus*100,GetCriticalStrike(client)*100);
        WCMessage(client,"2h weap spec glyph used",client,bonus*100,GetAccuracy(client)*100);
      }
    }
    else
    {
      wcRecalculateStats(client);
      if(noisy)
        WCMessage(client,"2h weap spec used",client,bonus*100,GetCriticalStrike(client)*100);
    }
  }
}

wcRunTalent_1_11(client,bool:noisy) { //Juggernaut
  decl String:line[255];
  wcGetTalentInfo(1,3,2,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new bonus=RoundToNearest(float(GetStr(client)+GetBonusStr(client))*multi);
  SetBonusStr(client,(GetBonusStr(client)+bonus));
  wcRecalculateStats(client);
  if(noisy)
    WCMessage(client,"juggernaut used",client,bonus,GetStr(client)+GetBonusStr(client));
}

wcRunTalent_1_12(client,bool:noisy) { //Armored To The Teeth
  decl String:line[255];
  wcGetTalentInfo(1,3,2,3,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new Float:bonus = GetArmor(client)*multi;
  SetBonusDamage(client,GetBonusDamage(client)+bonus);
  wcRecalculateStats(client);
  if(noisy)
    WCMessage(client,"armored to the teeth used",client, bonus, GetDamage(client));
}

public wcRunTalent_1_13(client,&Float:damagemulti) //Impale
{
  decl String:line[255];
  wcGetTalentInfo(1,3,2,1,client,"multi",line);
  damagemulti+=StringToFloat(line);
  return true;
}

wcRunTalent_1_FocusedRage(client)
{
  decl String:line[255];
  new mana = GetMana(client);
  new maxmana = GetMaxMana(client);
  wcGetTalentInfo(1,1,2,2,client,"rage",line);
  mana+=StringToInt(line);
  if(mana>maxmana)
    mana=maxmana;
  wcSetMana(client,mana);
}

wcRunTalent_1_ManoAMano(client,victim)
{
  decl String:line[255];
  wcGetTalentInfo(1,1,3,2,client,"push",line);
  new Float:power = StringToFloat(line);
  wcGetTalentInfo(1,1,3,2,client,"z",line);
  new Float:zmodi = StringToFloat(line);
  new Float:location[3];
  new Float:locationvic[3];
  GetClientAbsOrigin(client,location);
  GetClientAbsOrigin(victim,locationvic);
  if((GetBoss(victim)<=0 || (! IsBoss())) && GetDistanceBetween(location,locationvic) > 200)
    PushPlayerTowardsPlayer(victim, client, power, zmodi);
}

wcRunTalent_1_RapidHealing(client)
{
  decl String:line[255];
  wcGetTalentInfo(1,1,3,3,client,"heal",line);
  new heal = StringToInt(line);
  new hp=wcGetHealth(client)+heal;
  new maxhp=GetMaxHealth(client);
  if(hp>maxhp)
    hp=maxhp;
  wcSetHealth(client, hp);
}

wcRunTalent_1_Bastion(client)
{
  decl String:line[255];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetTalentInfo(1,1,4,1,client,"cooldown",line);
  new Float:cooldown=StringToFloat(line);
  wcGetTalentInfo(1,1,4,1,client,"cooldowntype",line);
  new cooldowntype=StringToInt(line);
  SetCdSpell(client,cooldowntype,cooldown,0,"Unbreakable Bastion");
  wcGetTalentInfo(1,1,4,1,client,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(1,1,4,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
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

  for (new x=0;x<=(amount-1);x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
       Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
       Format(stargets,sizeof(stargets),"%s %s",stargets,name2);

    new Handle:datapack4=CreateDataPack();
    WritePackCell(datapack4,targets[x]);
    WritePackCell(datapack4,g_RoundCount);
    WritePackCell(datapack4,client);
    WritePackString(datapack4,name);
    CreateTimer(duration,EndBastion,datapack4,TIMER_HNDL_CLOSE);
    if(targets[x]!=client)
    {
      WCMessage(targets[x],"unbreakable bastion target",targets[x],name);
      SetEffectVictim(targets[x],50);
      SetEffectSpellVic(targets[x],50);
      SetBastion(targets[x],client);
    }
  }
}

public Action:EndBastion(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client= ReadPackCell(datapack);
  new round = ReadPackCell(datapack);
  new bastionuser= ReadPackCell(datapack);
  if(round==g_RoundCount && client != bastionuser)
  {
    decl String:name[255];
    ReadPackString(datapack,name, sizeof(name));
    WCMessage(client,"unbreakable bastion target end",client, name);
    RemoveEffectVictim(client,50);
    RemoveEffectSpellVic(client,50);
  }
  else
  {
    WCMessage(client,"unbreakable bastion user end",client);
  }
}

wcRunTalent_1_PreciseBlows(client,victim)
{
  decl String:line[255];
  wcGetTalentInfo(1,2,1,4,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new armormodifier = RoundToNearest(GetArmor(victim)*multi);
  return armormodifier;
}

wcRunTalent_1_BloodCrazed(client)
{
  //1h+1h should be allowed only
  if(!HasDual(client))
    return;
  decl String:line[255];
  wcGetTalentInfo(1,2,3,2,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if(chance >= rand)
  {
    wcGetTalentInfo(1,2,3,2,client,"multi",line);
    new Float:percentage = StringToFloat(line);
    new hp = wcGetHealth(client);
    new maxhp = GetMaxHealth(client);
    hp+=RoundToNearest(maxhp*percentage);
    if(hp>maxhp)
      hp = maxhp;
    WCMessage(client,"Blood Crazed",client,percentage*100,percentage*maxhp);
    wcSetHealth(client,hp);
  }
}

wcRunTalent_1_BattleRush(client)
{
  decl String:line[255];
  wcGetTalentInfo(1,2,3,3,client,"speed",line); //Battle Rush
  new Float:bonus=StringToFloat(line);
  wcGetTalentInfo(1,2,3,3,client,"duration",line); //Battle Rush
  new Float:duration=StringToFloat(line);

  new Float:oldspeed=GetNormalSpeed(client);
  new Float:newspeed=oldspeed * bonus;
  if(newspeed>oldspeed)
  {
    WCMessage(client,"Battle Rush Used",client);
    new Float:time=duration;
    SetClientSpeed(client,newspeed);
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,client); // Write the client index of owner
    WritePackFloat(datapack,newspeed);
    WritePackFloat(datapack,bonus);
    CreateTimer(time,RemoveSpeedEffect2,datapack,TIMER_HNDL_CLOSE);
  }
}

public Action:EndBattleRush(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  new Float:bonus=float(ReadPackCell(datapack)/100);
  new round = ReadPackCell(datapack);
  if(round==g_RoundCount)
  {
    SetClientSpeed(client, GetClientSpeed(client)-bonus, true);
  }
}

wcRunTalent_1_BladeStorm(client) { //Bladestorm
  decl String:line[255];
  wcGetTalentInfo(1,2,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(1,2,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);

  wcGetTalentInfo(1,2,4,1,client,"duration",line);
  new Float:duration=StringToFloat(line);

  SetCdSpell(client,cdtype,cd, 0, "Blade Storm");

  WCMessage(client,"blade storm",client,duration);

  wcGetTalentInfo(1,2,4,1,client,"hits",line);
  SetSweepingStrikes(client,StringToInt(line));
  SetEffectAttacker(client,41);

  new Float:locationSound[3];
  GetClientAbsOrigin(client,locationSound);
  EmitAmbientSound("wc/spells/bladestorm.wav",locationSound);

  new Handle:datapack4=CreateDataPack();
  WritePackCell(datapack4,client);
  CreateTimer(duration,EndBladeStorm,datapack4,TIMER_HNDL_CLOSE);
}

public Action:EndBladeStorm(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  RemoveEffectAttacker(client,41);
  if(GetSweepingStrikes(client)>0)
  {
    SetSweepingStrikes(client,0);
    WCMessage(client,"Bladestorm Ends",client);
  }
}

wcRunTalent_1_DeepWounds(attacker,victim,damage)
{
  if(IsPlayerAlive(attacker))
  {
    decl String:line[255];
    wcGetTalentInfo(1,3,3,2,attacker,"ticks",line);
    new ticks = StringToInt(line);
    wcGetTalentInfo(1,3,3,2,attacker,"multi",line);
    new dmgef = RoundToNearest(damage*StringToFloat(line));
    wcGetTalentInfo(1,3,3,2,attacker,"tickdelay",line);
    new Float:tickdelay=StringToFloat(line);
    if (dmgef==0)
      dmgef = 1;
    new effid=CreateEffectData(victim,attacker,ticks,dmgef,0,0,0,2,g_respawns[victim],0);
    if (tickdelay==0.0 || ticks == 0 || dmgef == 0)
    {
      LogError("ERROR: deepwounds talent corrupted.");
      DeleteEffectData(effid);
    }
    else  if(effid!=0)
    {
      CreateTimer(tickdelay, Effect4Action, effid, TIMER_REPEAT);
      WCMessage(attacker,"deep wounds",attacker,dmgef*2,tickdelay*ticks);
    }
  }
}

wcRunTalent_1_ControlledRage(client,bool:noisy) //Controlled Rage
{
  decl String:line[255];
  wcGetTalentInfo(1,3,3,3,client,"increment",line);
  new bonus=StringToInt(line);
  SetBonusMaxMana(client,GetBonusMaxMana(client)+bonus);
  wcRecalculateStats(client);
  if(noisy)
    WCMessage(client,"controlled rage used",client,bonus,GetMaxMana(client));
}

wcRunTalent_1_Execute(client,victim,&damage)
{
  if (IsDoubleHand(client) != true)
    return damage;

  decl String:line[255];
  wcGetTalentInfo(1,3,4,1,client,"limit",line);
  new Float:healthhp = StringToFloat(line);

  new bool:pass = false;

  if((wcGetHealth(victim)<= GetMaxHealth(victim) * healthhp) || pass)
  {
    wcGetTalentInfo(1,3,4,1,client,"chance",line);
    new chance = StringToInt(line);
    new rand = GetRandomInt(1,100);
    if(chance >= rand || pass)
    {
      wcGetTalentInfo(1,3,4,1,client,"multi",line);
      new Float:multi = StringToFloat(line);
      wcGetTalentInfo(1,3,4,1,client,"base",line);
      new base = StringToInt(line);
      damage+= base + RoundToNearest(multi*GetMana(client));
      wcSetMana(client,0);
      wcGetTalentInfo(1,3,4,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(1,3,4,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      SetCdSpell(client,cdtype,cd, 0, "Execution");
      AddEMsgAff("execute", true);
      new Float:locationSound[3];
      GetClientAbsOrigin(victim,locationSound);
      EmitAmbientSound("wc/spells/execute.wav",locationSound);

    }
  }
  return damage;
}
public wcRunTalent_1_ColossusSmash(client, victim, &damage) //Colossus Smash
{
  if (IsDoubleHand(client) != true)
    return damage;

  decl String:line[255];
  wcGetTalentInfo(1,3,4,2,client,"chance",line);
  new Float:chance = StringToFloat(line);
  new random = GetRandomInt(1,100);
  if (  (chance >= random)  && !IsBoss())
  {
    wcGetTalentInfo(1,3,4,2,client,"damage",line);
    new Float:multi = StringToFloat(line);
    wcGetTalentInfo(1,3,4,2,client,"reduction",line);
    new Float:reduction = StringToFloat(line);
    wcGetTalentInfo(1,3,4,2,client,"reductiond",line);
    new Float:duration=StringToFloat(line);
    damage+=RoundToNearest(multi*GetDamage(client));
    //new armor=GetBonusArmor(victim);
    //new Float:newarm = float(armor) * reduction;
    wcClientsBonusStats[GetPlayerID(victim)][ARMOR]*=reduction;
    wcRecalculateStats(victim);
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,victim); // Write the client index of owner
    WritePackCell(datapack2,g_RoundCount);
    //WritePackCell(datapack2,armor);
    WritePackFloat(datapack2,reduction);
    CreateTimer(duration,ReturnArmor,datapack2,TIMER_HNDL_CLOSE);
    wcGetTalentInfo(1,3,4,2,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(1,3,4,2,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    SetCdSpell(client,cdtype,cd, 0, "Colossus Smash");
    AddEMsgAff("colossus smash", true);
    decl Float:location[3];
    GetClientAbsOrigin(client,location);
    EmitAmbientSound("wc/spells/colossussmash.wav",location);
  }
  return 1;
}
public Action:ReturnArmor(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new round=ReadPackCell(datapack);
  //new armor=ReadPackCell(datapack);
  new Float:reduction=ReadPackFloat(datapack);
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
  wcClientsBonusStats[GetPlayerID(client)][ARMOR]/=reduction;
  wcRecalculateStats(client);
  return Plugin_Stop;
}

/*public wcRunTalent_2_1(client)                                                  //Preparation
{
  decl String:line[255];
  wcGetTalentInfo(2,1,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(2,1,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 1,4,1);
  wcSetMana(client,GetMana(client)-manareq);
  WCMessage(client,"Preperation",client);

  for(new i=1; i<20; i++)
  {
    SetCdSpell(client,i,0.0);
  }
  SetCdSpell(client,cdtype,cd, 0, "Preperation");
  wcGetTalentInfo(2,1,4,1,client,"restore",line);
  new mana = GetMana(client)+StringToInt(line);
  if(mana>100)
    mana = 100;
  wcSetMana(client,mana);
}*/

public wcRunTalent_2_2(client)                                                  //Adrenaline Rush
{
  decl String:line[255];
  wcGetTalentInfo(2,2,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(2,2,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetTalentInfo(2,2,3,1,client,"restore",line);
  new mana = StringToInt(line);
  wcGetTalentInfo(2,2,3,1,client,"ticks",line);
  new ticks = StringToInt(line);
  wcGetTalentInfo(2,2,3,1,client,"delay",line);
  new Float:delay = StringToFloat(line);
  wcSetMana(client,GetMana(client)+mana);
  SetAdrenaline(client, true);
  wcGetTalentInfo(2,2,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  WCMessage(client,"Adrenaline",client,duration);
  new Handle:datapack4=CreateDataPack();
  WritePackCell(datapack4,client);
  CreateTimer(duration,EndAdrenalineRush,datapack4,TIMER_HNDL_CLOSE);
  new effid=CreateEffectData(client,ticks,mana,0,0,0,0,0,g_RoundCount,0);
  CreateTimer(delay, AdrenalineMana, effid, TIMER_REPEAT);
  SetCdSpell(client,cdtype,cd, 0, "Adrenaline Rush");
  if(HasGlyph(client, GLYPH_ROGUE_RUSH))  {
    SetEffectAttacker(client,51);
    new Handle:datapack3=CreateDataPack();
    WritePackCell(datapack3,client);
    WritePackCell(datapack3,51);
    CreateTimer(duration,DestroyEffectAttacker,datapack3,TIMER_HNDL_CLOSE);
  }
}
public Action:AdrenalineMana(Handle:timer, any:effid)
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
  if(effectsdata[effid][2]<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  wcSetMana(effectsdata[effid][1],GetMana(effectsdata[effid][1])+effectsdata[effid][3]);
  effectsdata[effid][2]-=1;
  return Plugin_Continue;
}

public Action:EndAdrenalineRush(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  SetAdrenaline(client, false);
}

public wcRunTalent_2_3(client)                                                  //Cold Blood
{
  decl String:line[255];
  wcGetTalentInfo(2,3,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(2,3,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new mana = GetTalentCost(client, 3,3,1);
  wcSetMana(client,GetMana(client)-mana);
  WCMessage(client,"ColdBlood",client);
  SetColdBlood(client, true);
  SetCdSpell(client,cdtype,cd, 0, "Cold Blood");
}

public Action:EndColdBlood(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  SetAutoCrit(client,false);
}

public wcRunTalent_2_4(client)                                                  //Filthy Tricks
{
  decl String:line[255];
  new random=GetRandomInt(1,100);
  wcGetTalentInfo(2,1,1,2,client,"chance",line);
  new chance = StringToInt(line);
  new weap = GetEntDataEnt2(client, ACTIVEWEAPON);
  if(weap>=0)
	{
    decl String:S_weapon[50];
    GetEdictClassname(weap, S_weapon, sizeof(S_weapon));
    if(StrEqual("weapon_knife",S_weapon))
      chance = chance * 3 +10;
  }
  if(chance>=random)
  {
    wcGetTalentInfo(2,1,1,2,client,"mana",line);
    new restore=StringToInt(line);
    if(GetMana(client)+restore >= GetMaxMana(client))
      wcSetMana(client, GetMaxMana(client));
    else
      wcSetMana(client, GetMana(client)+restore);
  }
}

public wcRunTalent_2_5(client) //Enveloping Shadows
{
  SetEffectSpellVic(client,32);
}

public wcRunTalent_2_6(client) //Fleet Footed
{
  decl String:line[255];
  wcGetTalentInfo(2,1,2,1,client,"increment",line);
  SetNormalBonusSpeed(client,GetNormalBonusSpeed(client)+StringToFloat(line));


  wcGetTalentInfo(2,1,2,1,client,"gravity",line);
  SetEntityGravity(client,StringToFloat(line));
  wcRecalculateStats(client);
}
public wcRunTalent_2_LightFooted(client) //used to keep lowgrav on rogue for talent_2_6   (above)
{
  decl String:line[255];
  wcGetTalentInfo(2,1,2,1,client,"gravity",line);
  SetEntityGravity(client,StringToFloat(line));
}

/*wcRunTalent_2_7(client, &damage)                                 //Master of Subtlety
{
  new weap = GetEntDataEnt2(client, ACTIVEWEAPON);
  if(weap>=0)
	{
    decl String:S_weapon[50];
    GetEdictClassname(weap, S_weapon, sizeof(S_weapon));
    if(StrEqual("weapon_knife",S_weapon))
    {
      decl String:line[255];
      wcGetTalentInfo(2,1,2,3,client,"multi",line);
      damage = RoundToNearest(damage*(1+StringToFloat(line)));
    }
  }
}*/
/*public wcRunTalent_2_10(client)                                                  //Find Weakness
{
  decl String:line[255];
  wcGetTalentInfo(2,2,1,2,client,"multi",line);
  new Float:penetration = GetIgnoreArmor(client)*StringToFloat(line);
  SetBonusIgnoreArmor(client,GetBonusIgnoreArmor(client)+penetration);
  wcRecalculateStats(client);
}*/

public wcRunTalent_2_9(client)                                                  //Aggression
{
  decl String:line[255];
  wcGetTalentInfo(2,2,2,1,client,"increment",line);
  SetCriticalStrikeBonus(client, GetCriticalStrikeBonus(client)+StringToFloat(line));
  wcRecalculateStats(client);
}

/*public wcRunTalent_2_8(client,victim, damage, String:weaponname[])             //Hack and Slash.
{
  decl String:line[255];
  wcGetTalentInfo(2,2,2,2,client,"chance",line);
  new chance=StringToInt(line);
  new random=GetRandomInt(1,100);
  if(chance>=random && GetWeaponSpeed(client)<=1.15 )
  {
    new seconddamage = RoundToNearest(float(damage)*0.50);
    dealExtraHit(client,victim,seconddamage,weaponname,"Hack and Slash");
    new Float:locationSound[3];
    GetClientAbsOrigin(victim,locationSound);
    EmitAmbientSound("wc/spells/hackandslash.wav",locationSound);
  }

  return damage;
}*/

public wcRunTalent_2_11(client,&Float:damagemulti)                              //Lethality
{
  decl String:line[255];
  wcGetTalentInfo(2,3,2,4,client,"multi",line);
  damagemulti+=StringToFloat(line);
  return true;
}

wcRunTalent_2_12(client)                                                        //Precision
{
  decl String:line[255];
  if(GetWeaponSpeed(client)<=1.15)
  {
    wcGetTalentInfo(2,2,1,1,client,"increment",line);
    SetBonusAccuracy(client,GetBonusAccuracy(client)+StringToFloat(line));
    wcRecalculateStats(client);
  }
}


wcRunTalent_2_ReinforcedLeather(client)
{
  decl String:line[255];
  wcGetTalentInfo(2,2,1,4,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(2,2,1,4,client,"base",line);
  new Float:bonus = float(GetArmor(client))*multi+float(StringToInt(line));
  SetBonusArmor(client, float(GetBonusArmor(client)) + bonus);
  wcRecalculateStats(client);
}

/*wcRunTalent_2_LightningReflexes(client, &damagemulti)
{
  decl String:line[255];
  wcGetTalentInfo(2,2,2,4,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if (chance >= rand)
  {
    damagemulti = 0;
    AddEMsgAff("dodge", true);
  }
}*/

/*wcRunTalent_2_BladeTwisting(attacker,target)
{
  decl String:line[255];
  wcGetTalentInfo(2,2,3,2,attacker,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if (chance >= rand && GetWeaponSpeed(attacker)<=1.15)
  {
    wcGetTalentInfo(2,2,3,2,attacker,"multi",line);
    new Float:slowmulti=StringToFloat(line);
    wcGetTalentInfo(2,2,3,2,attacker,"duration",line);
    new Float:slowtime=StringToFloat(line);

    new Float:oldspeed=GetClientSpeed(target); //Current
    new Float:newspeed=GetNormalSpeed(target); //Base (right after spawn/etc)
    newspeed*=slowmulti;
    if(newspeed<oldspeed)
    {
      SetClientSpeed(target,newspeed);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,target); // Write the client index of owner
      WritePackFloat(datapack2,newspeed);
      WritePackFloat(datapack2,slowmulti);
      CreateTimer(slowtime,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
      AddEMsgAff("blade twisting", true);
      new Float:location[3];
      GetClientAbsOrigin(attacker,location);
      location[2]+=50;
      EmitAmbientSound("wc/spells/bladetwisting.wav",location);
    }
  }
}*/

/*wcRunTalent_2_KillingSpree(client)
{
  new rweapon = GetEntDataEnt2(client, ACTIVEWEAPON);
  if(!IsValidEntity(rweapon))
    return;
  new ammo = GetEntData(rweapon, CLIPOFFSET, 4);
  decl String:weapon[50];
  GetEdictClassname(rweapon, weapon, sizeof(weapon));
  new clip;
  if(StrEqual("weapon_galil",weapon))
    clip = 35;
  else if(StrEqual("weapon_ak47",weapon))
    clip = 30;
  else if(StrEqual("weapon_scout",weapon))
    clip = 10;
  else if(StrEqual("weapon_sg552",weapon))
    clip = 30;
  else if(StrEqual("weapon_awp",weapon))
    clip = 10;
  else if(StrEqual("weapon_g3sg1",weapon))
    clip = 30;
  else if(StrEqual("weapon_famas",weapon))
    clip = 25;
  else if(StrEqual("weapon_m4a1",weapon))
    clip = 30;
  else if(StrEqual("weapon_aug",weapon))
    clip = 30;
  else if(StrEqual("weapon_sg550",weapon))
    clip = 30;
  else if(StrEqual("weapon_glock",weapon))
    clip = 20;
  else if(StrEqual("weapon_usp",weapon))
    clip = 12;
  else if(StrEqual("weapon_p228",weapon))
    clip = 13;
  else if(StrEqual("weapon_deagle",weapon))
    clip = 7;
  else if(StrEqual("weapon_elite",weapon))
    clip = 30;
  else if(StrEqual("weapon_fiveseven",weapon))
    clip = 20;
  else if(StrEqual("weapon_mac10",weapon))
    clip = 30;
  else if(StrEqual("weapon_tmp",weapon))
    clip = 30;
  else if(StrEqual("weapon_mp5navy",weapon))
    clip = 30;
  else if(StrEqual("weapon_ump45",weapon))
    clip = 40;
  else if(StrEqual("weapon_p90",weapon))
    clip = 50;
  else if(StrEqual("weapon_m249",weapon))
    clip = 100;
  else
    clip = 0;

  if(ammo < RoundToNearest(GetAttackSpeed(client) * clip))
  {
    decl String:line[255];
    wcGetTalentInfo(2,2,4,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    clip=RoundToNearest(multi*float(clip));
    ammo+=clip;
    SetEntData(rweapon, CLIPOFFSET, ammo, 4, true);
  }
}*/

/*wcRunTalent_2_MurderousIntent(client)
{
  decl String:line[255];
  wcGetTalentInfo(2,3,3,2,client,"restore",line);
  if(HasGlyph(client, GLYPH_ROGUE_MINTENT))
  {
    new Float:healamount = (StringToFloat(line)*0.005); //Turns 20 -> 0.10 and 40 -> 0.20
    new bhealth = RoundToNearest(healamount*float(GetMaxHealth(client)));
    bhealth = wcHeal(client,client,bhealth,0,false);
  }
  else
  {
  new mana = GetMana(client)+StringToInt(line);
  if(mana > GetMaxMana(client))
    mana = GetMaxMana(client);
  wcSetMana(client,mana);
  }
}*/

wcRunTalent_2_SwiftKiller(client)
{
  decl String:line[255];
  wcGetTalentInfo(2,3,3,3,client,"multi",line);
  new Float:bonus=StringToFloat(line);
  SetBonusAttackSpeed(client,GetBonusAttackSpeed(client)+bonus);
  wcRecalculateStats(client);
}

wcRunTalent_2_ShadowStep(client)
{
  decl String:line[255];

  wcGetTalentInfo(2,1,3,1,client,"distance",line);
  new Float:distance = StringToFloat(line);

  new Float:speed = GetClientSpeed(client);
  if (speed>1.0)
    distance *= ((speed - 1.0) * 3.0) + 1.0;

  // setting up for the visual effect
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  location[2]+=15;

  new Float:endpos[3];

  new mana = GetMana(client)-40;
  if(mana > 0)
  {
    if(TeleportPlayerView(client,distance,endpos))
    {
      wcSetMana(client,mana);
      wcGetTalentInfo(2,1,3,1,client,"duration",line);
      new Float:duration = StringToFloat(line);
      wcGetTalentInfo(2,1,3,1,client,"vanishid",line);
      new vanishid = StringToInt(line);
      new race = GetClass(client);
      wcGetTalentInfo(2,1,3,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      wcGetTalentInfo(2,1,3,1,client,"cooldowntype",line);
      new cdt = StringToInt(line);
      SetCdSpell(client,cdt,cd,0, "Shadowstep");


      // invisibility
      wcGetSpellInfo(race, vanishid, "alpha", line);
      new alpha=StringToInt(line);
      decl color[4];
      for (new x=0;x<=2;x++)
      color[x]=255;
      color[3]=alpha;
      SetClientColor(client,color);

      //visual effect & sound
      SmokeEffect("@all", location, g_ef_smoke, 100.0, 10);
      EmitAmbientSound("wc/spells/shadowstep.wav", location);

      bVanish[client]=true;
      AntiDefuse(client);

      new Handle:pack = CreateDataPack();
      WritePackCell(pack,client);
      WritePackCell(pack,++g_ffVanishId[client]);
      CreateTimer(duration,DestroyVanish,pack);

      wcGetSpellInfo(race, vanishid, "breakdmg", line);
      SetVanishBreakDmg(client,StringToInt(line));

      WCMessage(client,"shadowstep",client);
    }
    else
    {
      WCMessage(client,"shadowstep failed",client);
    }
  }
  else
  {
    WCMessage(client,"no energy",client);
  }
}

wcRunTalent_2_ShadowGate(client)
{
  decl Float:location[3];
  decl String:line[255];
  if(GetShadowGateLocation(client,0)==0.0 && GetShadowGateLocation(client,1)==0.0 && GetShadowGateLocation(client,2)==0.0 && !IsHNS() && !IsCtf())
  {
    if(GetMana(client) >= 40)
    {
      wcSetMana(client,GetMana(client)-40);
      GetClientAbsOrigin(client,location);
      SetShadowGateLocation(client,0,location[0]);
      SetShadowGateLocation(client,1,location[1]);
      SetShadowGateLocation(client,2,location[2]);
      WCMessage(client,"shadowgate loc",client);
      wcGetTalentInfo(2,3,4,1,client,"cooldowntype",line);
      SetCdSpell(client, StringToInt(line), 2.0, 0,"Shadow Gate");
    }
    else
    {
      WCMessage(client,"no energy",client);
    }
  }
  else
  if(!IsHNS() && !IsCtf())
  {
    {
      GetClientAbsOrigin(client,location);
      location[2]+=15;
      SmokeEffect("@all", location, g_ef_smoke, 100.0, 10);
      EmitAmbientSound("wc/spells/shadowgate.wav",location);
      location[0]=GetShadowGateLocation(client,0);
      location[1]=GetShadowGateLocation(client,1);
      location[2]=GetShadowGateLocation(client,2);
      TeleportEntity(client, location, NULL_VECTOR, NULL_VECTOR);
      EmitAmbientSound("wc/spells/shadowgate2.wav",location);
      wcGetTalentInfo(2,3,4,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      wcGetTalentInfo(2,3,4,1,client,"cooldowntype",line);
      SetCdSpell(client, StringToInt(line), cd, 0,"Shadow Gate");
      if(HasGlyph(client, GLYPH_ROGUE_SHADOWGATE))
      {
        SetShadowGateLocation(client,0,0.0);
        SetShadowGateLocation(client,1,0.0);
        SetShadowGateLocation(client,2,0.0);
        WCMessage(client,"shadowgate reset",client);
      }
    }
  }
}

public wcRunTalent_3_1(client)                                                  //Ice Barrier
{
  decl String:line[255];
  wcGetTalentInfo(3,1,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(3,1,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 1,3,1);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Ice Barrier");
//  wcGetTalentInfo(3,3,2,3,client,"chance",line); //Focused Mind
//  new chance = StringToInt(line);
//  new random=GetRandomInt(1,100);
//  if (chance >= random)
//  {
//    new Float:locationSound[3];
//    GetClientAbsOrigin(client,locationSound);
//    EmitAmbientSound("wc/spells/focusedmind.wav",locationSound);
//
//    wcGetTalentInfo(3,3,2,3,client,"multi",line); //Focused Mind
//    new Float:multi = StringToFloat(line);
//    cd = (cd * multi);
//  }
  wcGetTalentInfo(3,1,3,1,client,"absorb",line);
  new absorb=StringToInt(line);
  wcGetTalentInfo(3,1,3,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(3,1,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  absorb+=GetSpellDmg(client,multi);
  wcClientAbsorb[client]=absorb;
  SetEffectVictim(client,25); // absorb shield
  SetEffectSpellVic(client,25); //Absorb magic dmg
  new Handle:datapack2=CreateDataPack();
  WritePackCell(datapack2,client); // Write the client index of owner
  WritePackCell(datapack2,25);
  CreateTimer(duration,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,client); // Write the client index of owner
  WritePackCell(datapack3,25);
  CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);
  WCMessage(client,"ice barrier",client,duration,absorb);
}

public wcRunTalent_3_2(client)                                                  //Living Bomb
{
  new target=GetAimTarget(client,25.0);
  decl String:line[255];
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      wcGetTalentInfo(3,2,3,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(3,2,3,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);

//      wcGetTalentInfo(3,3,2,3,client,"chance",line); //Focused Mind
//      new chance = StringToInt(line);
//      new random=GetRandomInt(1,100);
//      if (chance >= random)
//      {
//        new Float:locationSound[3];
//        GetClientAbsOrigin(client,locationSound);
//        EmitAmbientSound("wc/spells/focusedmind.wav",locationSound);
//
//        wcGetTalentInfo(3,3,2,3,client,"multi",line); //Focused Mind
//        new Float:multi = StringToFloat(line);
//        cd = (cd * multi);
//      }

      SetCdSpell(client,cdtype,cd, 0, "Living Bomb");
      new manareq = GetTalentCost(client, 2,3,1);

      wcSetMana(client,GetMana(client)-manareq);
      wcGetTalentInfo(3,2,3,1,client,"tickmulti",line);
      new Float:tickmulti=StringToFloat(line);
      new dmgef=GetSpellDmg(client,tickmulti);
      decl Float:location[3];
      GetClientAbsOrigin(target,location);
      location[2]+=50;

      new String:atname[50],String:vicname[50];
      GetClientName(client,atname,sizeof(atname));
      GetClientName(target,vicname,sizeof(vicname));
      WCMessage(client,"living bomb cast",client,dmgef,vicname);
      WCMessage(target,"living bomb cast vic",client,dmgef,atname);

      wcGetTalentInfo(3,2,3,1,client,"tickdelay",line);
      new Float:tickdelay=StringToFloat(line);
      wcGetTalentInfo(3,2,3,1,client,"ticks",line);
      new ticks=StringToInt(line);
      new team=GetClientTeam(target);
      ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
      tickdelay = tickdelay*(1.0-GetHasteEffect(client));

      wcGetTalentInfo(3,2,3,1,client,"range",line);
      new aoe=StringToInt(line);

      new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,0,1,g_RoundCount,GetClientDeaths(target));
      if(effid!=0)
      {
        CreateTimer(tickdelay, EffectAoeDamage, effid, TIMER_REPEAT);
      }

      //Damage is now included into ticks damage, this one does not seem to work
      new Float:locationSound[3];
      GetClientAbsOrigin(target,locationSound);
      EmitAmbientSound("wc/spells/livingbomb2.wav",locationSound);
    }
    else  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
}

public wcRunTalent_3_3(client)    //Invisibility
{
  //setting up cooldown and -mana
  decl String:line[255];
  wcGetTalentInfo(3,3,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(3,3,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 3, 3, 1);
  wcGetTalentInfo(3,3,1,2,client,"multi",line);
  new Float:manabonus=StringToFloat(line);
  manareq=RoundToNearest(float(manareq)*(1.0-manabonus));
  wcSetMana(client,GetMana(client)-manareq);

  SetCdSpell(client,cdtype,cd, 0, "Invisibility");
  wcGetTalentInfo(3,3,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);

  CreateTimer(duration,DestroyMageInvis,client);

  wcGetTalentInfo(3,3,3,1,client,"duration",line);
  duration=StringToFloat(line);

  if(Defuser==client)
  {
    new bombent = FindEntityByClassname(-1,"planted_c4");
    if (bombent>-1)
    {
      WCMessage(client, "cant defuse", client);
      CreateTimer(0.2,DefuseDelay,0);
    }
  }
  SetMageInvis(client,true);
  wcGetTalentInfo(3,3,3,1,client,"threat",line);
  AddThreat(client,StringToInt(line));
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/invisibility.wav",location);
  WCMessage(client,"invisibility",client,duration);

  //Invis Glyph
  if(!HasGlyph(client, GLYPH_MAGE_INVIS))
    return;
  new Float:oldspeed=GetClientSpeed(client);
  new Float:newspeed=GetNormalSpeed(client);
  newspeed*=1.25;
  if(newspeed>oldspeed)
  {
    SetClientSpeed(client,newspeed);
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,client); // Write the client index of owner
    WritePackFloat(datapack,newspeed);
    WritePackFloat(datapack,1.3);
    CreateTimer(duration,RemoveSpeedEffect2,datapack,TIMER_HNDL_CLOSE);
  }

}

public Action:DestroyMageInvis(Handle:timer, any:client)
{
  if(IsMageInvis(client)) {
    SetMageInvis(client, false);
    WCMessage(client, "invis expired", client);
  }
}

public wcRunTalent_3_4(client,victim, String:source[])                          //Frost Bite.
{
  decl String:line[255];
  if(wcGetSpellIDFromName(3,source)==1)
    wcGetTalentInfo(3,1,2,3,client,"boltchance",line);
  else
    wcGetTalentInfo(3,1,2,3,client,"chance",line);
  new chance = StringToInt(line);
  if(HasGlyph(client, GLYPH_MAGE_FROSTBITE))
  {
    new Float:currentspeed=GetClientSpeed(client);
    new Float:normalspeed=GetNormalSpeed(client);
    if(normalspeed>currentspeed)
      chance+=5;
  }
  new random=GetRandomInt(1,100);
  new item[UniqueItem];
  GetCharItem(item, client, 16);
  if(item[Item_defId]==25243 && 80>GetLevel(client))
    chance+=5;
  if(chance>=random && GetClientSpeed(victim)>0.1)
  {
    wcGetTalentInfo(3,1,2,3,client,"slowmulti",line);
    new Float:slowmulti=StringToFloat(line);
    wcGetTalentInfo(3,1,2,3,client,"duration",line);
    new Float:duration=StringToFloat(line);
    /*decl String:buffer[30];*/
    /*Format(buffer,sizeof(buffer),"%T", "frost bite ef", client);*/
    AddEMsgAff("frost bite ef", true);

    new Float:oldspeed=GetClientSpeed(victim);
    new Float:newspeed=GetNormalSpeed(victim);
    newspeed*=slowmulti;
    if(newspeed<oldspeed)
    {
      SetClientSpeed(victim,newspeed);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,victim); // Write the client index of owner
      WritePackFloat(datapack2,newspeed);
      WritePackFloat(datapack2,oldspeed);
      CreateTimer(duration,RemoveFreezeEffect,datapack2,TIMER_HNDL_CLOSE);
    }
    new Float:locationSound[3];
    GetClientAbsOrigin(victim,locationSound);
    EmitAmbientSound("wc/spells/frostbite.wav",locationSound);
  }
}

public wcRunTalent_3_5(client, victim, &damage)                                 //Molten Fury
{
  decl String:line[255];
  new max = GetMaxHealth(victim)+GetBonusMaxHealth(victim);
  wcGetTalentInfo(3,2,2,1,client,"health",line);
  new Float:multi = StringToFloat(line);
  new hp = wcGetHealth(victim);
  if(max*multi > hp)
  {
    wcGetTalentInfo(3,2,2,1,client,"multi",line);
    multi=StringToFloat(line);
    /*decl String:buffer[30];*/
    damage=RoundToNearest(damage*(1+multi));
    /*Format(buffer,sizeof(buffer),"%T", "molten fury", client);*/
    AddEMsgAff("molten fury", true);
  }
  return damage;
}

public wcRunTalent_3_6(client)                                                  //Pyromanic.
{
  decl String:line[255];
  wcGetTalentInfo(3,2,2,2,client,"increment",line);
  new Float:crit = StringToFloat(line);
  SetCriticalStrikeBonus(client, GetCriticalStrikeBonus(client)+crit);
  wcRecalculateStats(client);
}

/*stock wcRunTalent_3_7(client)                                                  //Arcane Fortitude
{
    decl String:line[255];
    wcGetTalentInfo(3,3,1,1,client,"multi",line);
    new Float:bonus = float(GetInt(client)+GetBonusInt(client)) *StringToFloat(line);
    SetBonusArmor(client, float(GetBonusArmor(client)) + bonus);
    wcRecalculateStats(client);
}*/


public wcRunTalent_3_9(client, mana)                                            //Arcane Student
{
  decl String:line[255];
  wcGetTalentInfo(3,3,1,2,client,"multi",line);
  mana -= RoundToNearest(mana*StringToFloat(line));
  return mana;
}

public wcRunTalent_3_10(client,spellid)                                         //Master of Elements
{
  decl String:line[255];
  new mana = GetSpellCost(client, spellid);
  wcGetTalentInfo(3,2,1,2,client,"multi",line);
  wcSetMana(client,GetMana(client)+RoundToNearest(mana*StringToFloat(line)));
}

public wcRunTalent_3_11(attacker,victim,damage,Float:damagemulti)
{
  decl String:line[255];
  wcGetTalentInfo(3,2,4,2,attacker,"ticks",line);
  new ticks = StringToInt(line);
  wcGetTalentInfo(3,2,4,2,attacker,"multi",line);
  new dmgef = RoundToNearest(damage*StringToFloat(line)*damagemulti);
  wcGetTalentInfo(3,2,4,2,attacker,"tickdelay",line);
  new Float:tickdelay=StringToFloat(line);
  ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(attacker)));
  tickdelay = tickdelay*(1.0-GetHasteEffect(attacker));
  new effid=CreateEffectData(victim,attacker,ticks,dmgef,0,0,0,2,g_RoundCount,0);
  if (dmgef==0)
    dmgef = 1;
  if (tickdelay==0.0 || ticks == 0 || dmgef == 0)
  {
    LogError("ERROR: ignite talent corrupted.");
    DeleteEffectData(effid);
  }
  else  if(effid!=0)
    {
      CreateTimer(tickdelay, EffectFlameShockAction, effid, TIMER_REPEAT);
      WCMessage(attacker,"ignite",attacker,dmgef);
    }
}

wcRunTalent_3_PiercingChills(client,victim)
{
  decl String:line[255];
  wcGetTalentInfo(3,1,1,2,client,"duration",line);
  new Float:talentbonus=StringToFloat(line);
  wcGetSpellInfo(3,1,"slowtime",line);
  new Float:slowtime=StringToFloat(line)+talentbonus;
  wcGetTalentInfo(3,1,1,2,client,"increment",line);
  talentbonus=StringToFloat(line);
  wcGetSpellInfo(3,1,"slowmulti",line);
  new Float:slowmulti=StringToFloat(line)-talentbonus;
  wcGetTalentInfo(3,1,1,4,client,"distance",line);
  new Float:distance=StringToFloat(line);

  new Float:location[3];
  GetClientAbsOrigin(victim,location);
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,distance);
  if(team==3)
    amount=GetNearClients("@t",location,targets,distance);

  location[2]+=20.0;
  VS_BeamRingPoint(location, 50.00, 250.00, g_bluelight1, g_bluelight1, 0, 0, 2.00, 10.00, 0.00, {255, 255, 255, 255}, 0, 0);

  for (new x=0;x<=(amount-1);x++)
  {
    new Float:oldspeed=GetClientSpeed(targets[x]);
    new Float:newspeed=GetNormalSpeed(targets[x]);
    newspeed*=slowmulti;
    if(newspeed<oldspeed)
    {
      SetClientSpeed(targets[x],newspeed);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,targets[x]);
      WritePackFloat(datapack2,newspeed);
      WritePackFloat(datapack2,slowmulti);
      CreateTimer(slowtime,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
    }
  }
}

wcRunTalent_3_Frostburn(attacker,victim)
{
  decl String:line[255];
  wcGetTalentInfo(3,1,2,4,attacker,"multi",line);
  new Float:slowas=StringToFloat(line);
  wcGetTalentInfo(3,1,2,4,attacker,"slowtime",line);
  new Float:slowtime=StringToFloat(line);
  new Float:newas=GetAttackSpeed(victim)*(1-slowas);
  new Float:restoreAs=GetAttackSpeed(victim)-newas;
  wcClientsBonusStats[victim][ATTACKSPEED]-=restoreAs;
  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,victim);
  WritePackCell(datapack3,g_respawns[victim]);
  WritePackFloat(datapack3,restoreAs);
  CreateTimer(slowtime,RemoveSlowAttack,datapack3,TIMER_HNDL_CLOSE);
}

wcRunTalent_3_ShatteredBarrier(client)
{
  decl String:line[255];
  wcGetTalentInfo(3,1,3,2,client,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(3,1,3,2,client,"slowmulti",line);
  new Float:slowmulti=StringToFloat(line);
  wcGetTalentInfo(3,1,3,2,client,"duration",line);
  new Float:slowtime=StringToFloat(line);
  wcGetTalentInfo(3,1,3,2,client,"ticktime",line);
  new Float:ticktime=StringToFloat(line);
  wcGetTalentInfo(3,1,3,2,client,"breakdmg",line);
  new breakdmg=StringToInt(line);

  new Float:location[3];
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,distance);
  if(team==3)
    amount=GetNearClients("@t",location,targets,distance);

  EmitAmbientSound("wc/spells/frostbolt.wav",location);

  //THE FUCK. FrostNovaVisualEffect requires DataPack not EffectData
  //Indeed, I rewrote that twice but only updated shattered barrier once >.>
  new Handle:dp=CreateDataPack();
  WritePackCell (dp,client); // Write the client index of victim
  WritePackCell (dp,10);
  WritePackFloat(dp,location[0]);
  WritePackFloat(dp,location[1]);
  WritePackFloat(dp,location[2]);

  CreateTimer(0.1, FrostNovaVisualEffect, dp, TIMER_REPEAT);
  for (new x=0;x<=(amount-1);x++)
  {
    new Float:oldspeed=GetClientSpeed(targets[x]);
    new Float:newspeed=GetNormalSpeed(targets[x]);
    newspeed*=slowmulti;
    if(newspeed<oldspeed)
    {
      new effid2=CreateEffectData(targets[x],RoundToNearest(slowtime/ticktime),0,0,0,0,0,0,RoundToNearest(oldspeed*100),0);
      SetClientSpeed(targets[x],newspeed);
      SetFrostNovaFreezeDmg(targets[x],breakdmg);
      SetFrostNovaFreeze(targets[x],effid2);
      CreateTimer(ticktime,FrostNovaFreezeEffect,effid2, TIMER_REPEAT);
    }
  }
}

wcRunTalent_3_ColdSnap(client)
{
  decl String:line[255];
  wcGetSpellInfo(3,1,"cooldowntype",line);
  new type = StringToInt(line);
  wcGetTalentInfo(3,1,3,3,client,"duration",line);
  new Float:talent=StringToFloat(line);
  new Float:cd = GetCDSpell(client,type)-GetTickedTime();
  if (cd<talent)
    cd=0.0;
  else
    cd-=talent;
  SetCdSpell(client,type,cd,1);
}


public Action:EffectBlizzardAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];
  new Float:slow = Float:effectsdata[effid][2];
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
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  new aoe = 300;
  decl String:line[255];
  wcGetSpellInfo(8,11,"name",line);

  wcSpellAoeDamageLocation(client, client, location, dmg, aoe, team, line);
  location[2] += 20;
  VS_BeamRingPoint(location, 20.00, 200.00, g_ef_lgtning, g_ef_lgtning, 0, 0, 0.5, 300.00, 0.00, {55, 55, 157, 255}, 0, 0);
  VS_BeamRingPoint(location, 200.00, 20.00, g_ef_lgtning, g_ef_lgtning, 0, 0, 0.5, 300.00, 0.00, {55, 55, 157, 255}, 0, 0);

  new targets[MAXPLAYERS];
  new amount;

  if(team==2)
   amount=GetNearClients("@t",location,targets,float(aoe));
  if(team==3)
   amount=GetNearClients("@ct",location,targets,float(aoe));

  for (new x=0;x<amount;x++)
  {
    //Slodown effect
    new Float:oldspeed=GetClientSpeed(targets[x]);
    new Float:newspeed=GetNormalSpeed(targets[x]);
    newspeed*=slow;
    if(newspeed<oldspeed)
    {
      SetClientSpeed(targets[x],newspeed);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,targets[x]); // Write the client index of owner
      WritePackFloat(datapack2,newspeed);
      WritePackFloat(datapack2,slow);
      CreateTimer(0.9,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
    }

  }


  return Plugin_Continue;
}



wcRunTalent_3_Blizzard(client)  {

  //Cooldown
  decl String:line[255];
  wcGetTalentInfo(3,1,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(3,1,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client, cdtype, cd, 0, "Blizzard");

  //Take mana off
  new manareq = GetTalentCost(client, 1,4,1);
  wcSetMana(client,GetMana(client)-manareq);


  //Spell Specific variables
  wcGetTalentInfo(3,1,4,1,client,"distance",line);
  new Float:distance = StringToFloat(line);
  wcGetTalentInfo(3,1,4,1,client,"duration",line);
  new Float:duration = StringToFloat(line);
  wcGetTalentInfo(3,1,4,1,client,"multi",line);
  new Float:multi = StringToFloat(line);
  wcGetTalentInfo(3,1,4,1,client,"base",line);
  new base = StringToInt(line);
  wcGetTalentInfo(3,1,4,1,client,"period",line);
  new Float:tick = StringToFloat(line);
  wcGetTalentInfo(3,1,4,1,client,"slowmulti",line);
  new Float:slow=StringToFloat(line);



  new team=GetClientTeam(client);
  team = (team-1) % 2 + 2;
  //Spell code

  //Get location of where we center
  new Float:location[3];
  GetViewPoint(client, distance, location);

  new dmg = base + GetSpellDmg(client, multi);

  //Create the hellfire like (different) effect
  new effid=CreateEffectData(client,_:slow,dmg,RoundToNearest(duration / tick),team,_:location[0],_:location[1],_:location[2],g_RoundCount,0);
  if (multi==0.0 || tick == 0.0 || duration == 0.0) {
    LogError("ERROR: blizzard talent corrupted.");
    DeleteEffectData(effid);
  }
  else
  if(effid!=0)
  {
    CreateTimer(tick, EffectBlizzardAction, effid, TIMER_REPEAT);

    EmitAmbientSound("wc/spells/blizzard.wav",location);
    //Effect

    location[2]+=15.0;
    new sprite = CreateEntityByName("env_smokestack");
    if (IsValidEntity(sprite)) {
      DispatchKeyValue(sprite, "basespread", "105");
      DispatchKeyValue(sprite, "spreadspeed", "18");
      DispatchKeyValue(sprite, "speed", "2");
      DispatchKeyValue(sprite, "startsize", "22");
      DispatchKeyValue(sprite, "endsize", "17");
      DispatchKeyValue(sprite, "rate", "150");
      DispatchKeyValue(sprite, "jetlength", "20");
      DispatchKeyValue(sprite, "rendercolor", "20 20 255");
      DispatchKeyValue(sprite, "SmokeMaterial", "particle/fire.vmt");
      DispatchKeyValueVector(sprite, "origin", location);
      DispatchSpawn(sprite);
      AcceptEntityInput(sprite, "TurnOn");
      KillEntityDelay(3.0, sprite);
    }


  }

  WCMessage(client,"blizzard cast",client,dmg);
}




wcRunTalent_3_BurningSoul(client)
{
  decl String:line[255];
  wcGetSpellInfo(3,3,"cooldowntype",line);
  new type = StringToInt(line);
  wcGetTalentInfo(3,2,1,4,client,"cooldown",line);
  new Float:talent=StringToFloat(line);
  new Float:cd = GetCDSpell(client,type)-GetTickedTime();
  if (cd<talent)
    cd=0.0;
  else
    cd-=talent;
  SetCdSpell(client,type,cd,3);
}

wcRunTalent_3_SearingFlames(client,victim)
{
  if(!IsBoss() && !IsHNS())
  {
    decl String:line[255];
    wcGetTalentInfo(3,2,2,4,client,"multi",line);
    new Float:multi = StringToFloat(line);
    new delta = RoundToNearest(float(GetMaxHealth(victim))*(multi));
    SetBonusMaxHealth(victim, GetBonusMaxHealth(victim) - delta);
    wcRecalculateStats(victim);
    new hp = wcGetHealth(victim);
    wcSetHealth(victim, RoundToNearest(hp * (1.0 - multi)));
    if(GetMaxHealth(victim) < wcGetHealth(victim))
      wcSetHealth(victim, GetMaxHealth(victim));
    if(HasGlyph(client, GLYPH_MAGE_SEARINGF))
    {
      wcGetTalentInfo(3,2,2,4,client,"glyphaoe",line);
      new Float:aoe = StringToFloat(line);
      new Float:location[3];
      GetClientAbsOrigin(victim,location);
      new targets[MAXPLAYERS];
      new team=GetClientTeam(client);
      new amount;
      if(team==2)
        amount=GetNearClients("@ct",location,targets,aoe);
      if(team==3)
        amount=GetNearClients("@t",location,targets,aoe);
      for (new x=0;x<=(amount-1);x++)
      {
        new delta2 = RoundToNearest(float(GetMaxHealth(targets[x]))*(multi));
        SetBonusMaxHealth(targets[x], GetBonusMaxHealth(targets[x]) - delta2);
        wcRecalculateStats(targets[x]);
        new hpa = wcGetHealth(targets[x]);
        wcSetHealth(targets[x], RoundToNearest(hpa * (1.0 - multi)));
        if(GetMaxHealth(targets[x]) < wcGetHealth(targets[x]))
          wcSetHealth(targets[x], GetMaxHealth(targets[x]));
        decl String:name[255];
        GetClientName(client,name,sizeof(name));
        WCMessage(targets[x],"searingaoe",targets[x],name,RoundToNearest(multi*100));
      }
    }
  }
}

wcRunTalent_3_BlazingSpeed(client)
{
  decl String:line[255];
  wcGetTalentInfo(3,2,1,3,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if(chance >= rand)
  {
    wcGetTalentInfo(3,2,1,3,client,"speed",line);
    new Float:multi=StringToFloat(line);
    new Float:oldspeed=GetClientSpeed(client);
    new Float:newspeed=GetNormalSpeed(client);
    newspeed*=multi;
    if(newspeed>oldspeed)
    {
      wcClientSparks[client]=true;
      wcGetTalentInfo(3,2,1,3,client,"duration",line);
      new Float:time=StringToFloat(line);
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
}

//Float:wcRunTalent_3_CriticalMass(client)
//{
//  decl String:line[255];
//  wcGetTalentInfo(3,2,3,3,client,"multi",line);
//  new Float:multi=StringToFloat(line);
//  multi = (1-wcGetHealth(client)/GetMaxHealth(client))*multi;
//  return multi;
//}

stock wcRunTalent_3_FocusedMind(client,bool:noisy)
{
  decl String:line[255];
  new Float:haste;
  wcGetTalentInfo(3,3,2,3,client,"base",line);
  haste += StringToFloat(line);
  wcGetTalentInfo(3,3,2,3,client,"multi",line);
  haste += (StringToFloat(line) * float(GetInt(client)+GetBonusInt(client)));
  AddHaste(client, haste);
  wcRecalculateStats(client);
  if(noisy)
    WCMessage(client, "mage focused mind used", client, RoundToNearest(haste), GetHaste(client));
}

wcRunTalent_3_MasterOfMagic(client)
{
  SetEffectSpellVic(client,32);
}

wcRunTalent_3_ArcanePower(client)
{
  SetEffectSpellAtt(client,43);
}

wcRunTalent_3_ArcanePower2(client,mana)
{
  decl String:line[255];
  wcGetTalentInfo(3,3,2,3,client,"mana",line);
  return RoundToNearest(mana*(1+StringToFloat(line)));
}

wcRunTalent_3_ArcanePotency1(client, &damage)
{
  damage += RoundToNearest(damage*GetArcanePotency(client));
  SetArcanePotency(client,0.0);
}

wcRunTalent_3_ArcanePotency2(client)
{
  decl String:line[255];
  wcGetTalentInfo(3,3,3,3,client,"increment",line);
  SetArcanePotency(client,GetArcanePotency(client)+StringToFloat(line));
  if(GetArcanePotency(client)>0.5)
    SetArcanePotency(client,0.5);
}

wcRunTalent_3_ArcaneMissile(attacker,victim)
{
  if(IsPlayerAlive(victim))
  {
    new random=GetRandomInt(1,100);
    decl String:line[255];
    wcGetTalentInfo(3,3,3,2,attacker,"cooldowntype",line);
    new cdtype = StringToInt(line);
    wcGetTalentInfo(3,3,3,2,attacker,"chance",line);
    new chance=StringToInt(line);
    new Float:cool=GetCDSpell(attacker,cdtype)-GetTickedTime();
    if(chance>=random && !(cool>0.0))
    {
      new bool:glyph = HasGlyph(attacker, GLYPH_MAGE_MISSLES);
      new manareq=GetTalentCost(attacker, 3,4,1);
      if(glyph)
        manareq-=5;
      new newmana=GetMana(attacker)-manareq;
      if(newmana<0)
      {
        return false;
      }
      wcSetMana(attacker,newmana);
      wcGetTalentInfo(3,3,3,2,attacker,"cooldown",line);
      new Float:cd=StringToFloat(line);
      SetCdSpell(attacker,cdtype,cd,0);
      wcGetTalentInfo(3,3,3,2,attacker,"multi",line);
      new Float:multi=StringToFloat(line);
      new dmg=GetSpellDmg(attacker,multi);
      dmg = RoundToNearest(1.05 * float(dmg));
      wcGetTalentInfo(3,3,3,2,attacker,"base",line);
      dmg+=StringToInt(line);
      decl String:attMsg[300],String:vicMsg[300];
      Format(attMsg,sizeof(attMsg),"%T","arcane missile att", attacker);
      Format(vicMsg,sizeof(vicMsg),"%T","arcane missile vic", victim);
      wcGetTalentInfo(3,3,3,2,attacker,"name",line);
      wcSpellDamage(victim,attacker,dmg, attMsg, vicMsg,line);
      GiveArcaneCharge(attacker, 1);
      decl Float:start[3],Float:end[3];
      GetClientAbsOrigin(attacker, start);
      start[2]+=50;
      GetClientAbsOrigin(victim,end);
      end[2]+=50;
      VS_BeamPoints(start, end, g_bluelight1, g_bluelight1, 0, 0, 1.00, 9.00, 9.00, 5, 100.00, {255, 255, 255, 255}, 100);

      new Float:locationSound[3];
      GetClientAbsOrigin(attacker,locationSound);
      EmitAmbientSound("wc/spells/arcanemissile.wav",locationSound);
      if(CheckRequirements(attacker,4,3,4,2))
      {
        wcGetTalentInfo(4,3,4,2,attacker,"increase",line);
        SetBonusSpellDmg(attacker,GetBonusSpellDmg(attacker,1.0) + StringToFloat(line));
        wcRecalculateStats(attacker);
      }
    }
  }
  return false;
}

wcRunTalent_3_UnstableMagic(client)
{
  decl String:line[255];
  wcGetTalentInfo(3,3,4,2,client,"decrease",line);
  SetBonusSpellDmg(client,GetBonusSpellDmg(client,1.0) - StringToFloat(line));
  wcRecalculateStats(client);
}

wcRunTalent_3_BrittleFlesh(attacker,damage)
{
  if(!IsPlayerAlive(attacker))
    return 0;
  decl String:line[255];
  wcGetTalentInfo(3,1,4,2,attacker,"multi",line);
  new Float:multi=StringToFloat(line);
  damage=RoundToNearest(damage*(1+multi));
  AddEMsgAff("Brittle Flesh", true);
  return damage;
}

wcRunTalent_3_SearedFlesh(attacker,victim)
{
//  decl String:line[255];
//  wcGetTalentInfo(3,2,4,2,attacker,"multi",line);
//  new Float:multi=StringToFloat(line);
//  wcGetTalentInfo(3,2,4,2,attacker,"duration",line);
//  new Float:duration=StringToFloat(line);
//  SetHM(victim,multi);
//  SetHMT(victim,duration);

  decl String:line[255]; //good lord, it said "Sting" i've been reading that incorrectly off and on for days...
  wcGetTalentInfo(3,2,1,1,attacker,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(3,2,1,1,attacker,"duration",line);
  new Float:duration=StringToFloat(line);
  SetHM(victim,multi);
  SetHMT(victim,duration);

}

public wcRunTalent_4_1(client,victim)                                                  //Holy Shock
{
  decl String:line[255];
  wcGetTalentInfo(4,1,3,1,client,"chance",line);
  new chance = StringToInt(line);
  new random = GetRandomInt(1,100);
  if(chance >= random)
  {
    wcGetTalentInfo(4,1,3,1,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(4,1,3,1,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    new manareq = GetTalentCost(client, 1, 3, 1);
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client,cdtype,cd, 0, "Holy Shock");
    new Float:location[3],Float:locationatt[3];
    GetClientAbsOrigin(victim,location);
    GetClientAbsOrigin(client,locationatt);
    wcGetTalentInfo(4,1,3,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    decl String:name[255],String:name2[255];
    GetClientName(client,name,sizeof(name));
    GetClientName(victim,name2,sizeof(name2));
    new dmg=GetSpellDmg(client,multi);

    GlowSpriteEffect("@all",location, g_redglow1, 1.0, 0.5, 255);
    decl String:vicMsg[300], String:attMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","holyshock target", victim);
    Format(attMsg,sizeof(attMsg),"%T","holyshock caster", client);
    wcGetTalentInfo(4,1,3,1,client,"name",line);
    wcSpellDamage(victim,client,dmg, attMsg, vicMsg, line);
    wcHeal(client,client,dmg,0,false);

    new Float:locationSound[3];
    GetClientAbsOrigin(victim, locationSound);
    EmitAmbientSound("wc/spells/holyshock.wav",locationSound);
    if(HasGlyph(client, GLYPH_PALADIN_SHOCK))
    {
      GetClientAbsOrigin(client,location);
      new targets[MAXPLAYERS];
      new team=GetClientTeam(client);
      new amount;
      if(team==3)
      amount=GetNearClients("@ct",location,targets,500.0);
      if(team==2)
      amount=GetNearClients("@t",location,targets,500.0);
      for (new x=0;x<=(amount-1);x++)
      {
        if(targets[x]!=client)
        {
          wcHeal(client,targets[x],dmg,0,true);
        }
      }
    }
  }
}

public wcRunTalent_4_1_2(client)
{
  if(GetHolyShock(client))
  {
    SetHolyShock(client,false);
    WCMessage(client,"holy shock disable",client);
  }
  else
  {
    SetHolyShock(client,true);
    WCMessage(client,"holy shock enable",client);
  }
}

public wcRunTalent_4_2(client)                                                  //Avengers Shield
{
  new target=GetAimTarget(client,25.0);
  if(target>0)
  {
  if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
  {
    decl String:line[255];
    wcGetTalentInfo(4,2,3,1,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(4,2,3,1,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    new manareq = GetTalentCost(client, 2, 3, 1);
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client,cdtype,cd, 0, "Avenger's Shield");
    new Float:location[3],Float:locationatt[3];
    GetClientAbsOrigin(target,location);
    GetClientAbsOrigin(client,locationatt);
    wcGetTalentInfo(4,2,3,1,client,"slowtime",line);
    new Float:slowtime=StringToFloat(line);
    wcGetTalentInfo(4,2,3,1,client,"slowmulti",line);
    new Float:slowmulti=StringToFloat(line);
    wcGetTalentInfo(4,2,3,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetTalentInfo(4,2,3,1,client,"amulti",line);
    new Float:amulti=StringToFloat(line);
    wcGetTalentInfo(4,2,3,1,client,"range",line);
    new Float:range=StringToFloat(line);
    new dmg=RoundToNearest(GetSpellDmg(client,multi)+GetDamage(client)*amulti);
    GetClientAbsOrigin(target,location);
    new targets[3];
    new targetsAmount = GetNearestPlayers(GetClientTeam(target),location,range, targets, 3);

    wcGetTalentInfo(4,2,3,1,client,"name",line);
    decl String:vicMsg[300], String:attMsg[300], String:name2[300];
    for(new p=0;p<targetsAmount;p++){
      GetClientName(targets[p],name2,sizeof(name2));
      Format(vicMsg,sizeof(vicMsg),"%T","avengers shield target", targets[p]);
      wcSpellDamage(targets[p],client,dmg, "", vicMsg,line);
      Format(attMsg,sizeof(attMsg),"%s %s",attMsg,name2);
      if(HasGlyph(client, GLYPH_PALADIN_AVENGER)) {
        SetSilence(targets[p], 1.0);
      }
    }

    new Float:oldspeed=GetClientSpeed(target);
    new Float:newspeed=GetNormalSpeed(target);

    for(new i=0;i<targetsAmount;i++)  {
      oldspeed=GetClientSpeed(targets[i]);
      newspeed=GetNormalSpeed(targets[i]);
      newspeed*=slowmulti;
      if(newspeed<oldspeed)
      {
        SetClientSpeed(targets[i],newspeed);
        new Handle:datapack6=CreateDataPack();
        WritePackCell(datapack6,targets[i]);
        WritePackFloat(datapack6,newspeed);
        WritePackFloat(datapack6,slowmulti);
        CreateTimer(slowtime,RemoveSpeedEffect,datapack6,TIMER_HNDL_CLOSE);
      }
    }
    WCMessage(client,"avengers shield caster",client,attMsg,dmg);

    location[2]+=20;
    locationatt[2]+=50;
    new color[4];
    new color2[4];
    color2={240,115,35,155};
    BeamRingEffect("@all",location,60.0,59.0,g_ef_lgtning,0,1,2.0,50.0,color2,0.0,1);
    location[2]+=30;
    for(new o=1;o<targetsAmount;o++) {
      GetClientAbsOrigin(targets[o],locationatt);
      locationatt[2]+=20;
      color={240,115,35,155};
      BeamEffect("@all",locationatt,location,g_ef_glow01,5,2.0,3.0,9.0,color,0.0,5);
      BeamRingEffect("@all",locationatt,60.0,59.0,g_ef_lgtning,0,1,2.0,50.0,color2,0.0,1);
    }
    EmitAmbientSound("wc/spells/avengersshield.wav",location);
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
}

//Recoded
stock GetNearestPlayers(team, Float:location[3],Float:distance, clients[], max)
{
  new Float:possDistance[65];
  new amount=0;

  //Get array of possible clients
  for(new i=1;i<=GetMaxClients();i++)  {
    if(IsClientInGame(i))
      if(IsPlayerAlive(i) && (GetClientTeam(i)==team))  {
        new Float:loc2[3];
        GetClientAbsOrigin(i, loc2);
        new Float:dist = GetDistanceBetween(location, loc2);

        //make sure distance fits
        if(dist<=distance)  {
          //Push into array if needed
          amount = PushSortLowest(i, dist, clients, possDistance, amount, max);
        }
      }

  }
  return amount;
}

//variable now is not index but amount!
stock PushSortLowest(value, Float:weight, array[], Float:weights[], now, max)  {
  //Consider array is sorted!

  //Find index for new member
  new index=0;
  for(index=0; index < now; index++) {
    if(weight < weights[index])
      break;
  }
  //Found index where we insert

  //Check if it's valid
  if(index>=max)  {
    //index is higher than maximum amount
    return now; //return without insertion
  }

  //Safe to insert new
  //Push all the 'next' elements down
  for(new i=now-1;i>=index;i--)  {
    if(i>=max-1)  {
      continue;
    }
    weights[i+1]=weights[i];
    array[i+1]=array[i];
  }

  //Now we can safely insert our value into arrays
  array[index] = value;
  weights[index] = weight;

  //Find out new size
  new size=now;
  if(size<max)
    size++;

  return size;

}

public wcRunTalent_4_3(client)                                                  //Divine Storm
{
  decl String:line[255];
  wcGetTalentInfo(4,3,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(4,3,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 3, 3, 1);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Divine Storm");
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetTalentInfo(4,3,3,1,client,"range",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(4,3,3,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(4,3,3,1,client,"base",line);
  new base=StringToInt(line);
  new friendlies[MAXPLAYERS];
  new victims[MAXPLAYERS];
  new team=GetClientTeam(client);
  new healed;
  new damaged;
  if(team==3)
  {
    healed=GetNearClients("@ct",location,friendlies,distance);
    damaged=GetNearClients("@t",location,victims,distance);
  }
  if(team==2)
  {
    healed=GetNearClients("@t",location,friendlies,distance);
    damaged=GetNearClients("@ct",location,victims,distance);
  }
  wcGetTalentInfo(4,3,3,1,client,"healbonus",line);
  new Float:bonusheal=StringToFloat(line)*damaged;
  decl String:name[255],String:name2[255],String:vicMsg[300];
  GetClientName(client,name,sizeof(name));
  new String:stargets[255];
  new effect=RoundToNearest(multi*GetDamage(client))+base;
  for (new x=0;x<=(healed-1);x++)
  {
    GetClientName(friendlies[x],name2,sizeof(name2));
    if(x>0)
       Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
       Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
    new healing=wcHeal(client,friendlies[x],RoundToNearest(effect*(1+bonusheal)),0,true);
    if(friendlies[x]!=client)
    {
      WCMessage(friendlies[x],"divine storm healed target",friendlies[x],name,healing);
    }
  }
  wcGetTalentInfo(4,3,3,1,client,"name",line);

  for (new x=0;x<=(damaged-1);x++)
  {
    GetClientName(victims[x],name2,sizeof(name2));
    Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);

    Format(vicMsg,sizeof(vicMsg),"%T","divine storm damaged target", victims[x],name,effect);
    wcSpellDamage(victims[x],client,effect, "", vicMsg,line);
    BloodSpray(victims[x]);
  }
  if(damaged >= 3)  {
    if(HasGlyph(client, GLYPH_PALADIN_STORM))
      SetHolyPower(client, GetHolyPower(client)+3);
  }
  if(damaged == 2)  {
    if(HasGlyph(client, GLYPH_PALADIN_STORM))
      SetHolyPower(client, GetHolyPower(client)+2);
  }
  if(damaged == 1)  {
    if(HasGlyph(client, GLYPH_PALADIN_STORM))
      SetHolyPower(client, GetHolyPower(client)+1);
  }

  //location[2]+=50;
  decl Float:location2[3];
  GetClientAbsOrigin(client, location2);
  EmitAmbientSound("wc/spells/divinestorm.wav",location);
  location[2]+=40;
  location2[2]+=300;
  VS_BeamPoints(location, location2, g_plasmabeam, g_plasmabeam, 1, 5, 2.00, 15.00, 15.00, 1, 0.00, {255, 255, 255, 255}, 120);
  VS_BeamRingPoint(location, 20.00, 40.00, g_plasmabeam, g_plasmabeam, 20, 50, 2.00, 20.00, 1.00, {255, 255, 255, 255}, 30, 0);
  VS_BeamRingPoint(location2, 20.00, 500.00, g_plasmabeam, g_plasmabeam, 10, 30, 2.00, 80.00, 4.00, {255, 255, 255, 255}, 30, 0);


  WCMessage(client,"divine storm caster",client,stargets,effect);
}

public wcRunTalent_4_4(client)                                                  //Blessing of Wisdom
{
  decl String:line[255];
  wcGetTalentInfo(4,1,1,3,client,"manaregen",line);
  new mana=StringToInt(line);
  CastWisdom(client,mana);
}

stock bool:CastWisdom(client, mana, bool:cast=false,bool:custom=false)
{
   if((!custom) && g_iWisdomClient[client]>=mana)
      return false;
   SetBonusManaReg(client, GetBonusManaReg(client)+mana-g_iWisdomClient[client]);
   wcRecalculateStats(client);
   if(!custom)
    g_iWisdomClient[client]=mana;
   if(cast) {
      WCMessage(client,"wisdom aff",client);
   }
   return true;
}

public Action:CastWisdomAll(Handle:timer, any:client)
{
   if(!IsClientInGame(client))
    return;
   decl String:line[255];
   wcGetTalentInfo(4,1,1,3,client,"manaregen",line);
   new mana=StringToInt(line);
   WCMessage(client,"wisdom cast all",client);
   new clients[MAXPLAYERS];
   new team=GetClientTeam(client);
   new amount;
   if(team==2)
      amount=FindMatchingPlayers("@t",clients);
   if(team==3)
      amount=FindMatchingPlayers("@ct",clients);
   for(new i=0;i<amount;i++)
      CastWisdom(clients[i],mana+2,true);
}

public wcRunTalent_4_5(client)                                                  //Greater Blessing of Wisdom
{
   CreateTimer(0.5, CastWisdomAll, client);
}

public wcRunTalent_4_6(client)                                                  //Divine Stamina
{
  decl String:line[255];
  wcGetTalentInfo(4,2,1,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  SetBonusSta(client,GetBonusSta(client)+RoundToNearest(float(GetSta(client)+GetBonusSta(client))*multi));
  wcRecalculateStats(client);
}

public wcRunTalent_4_7(client)                                                  //Devotion Aura
{
  decl String:line[255];
  if(!HasShield(client)) {
    WCMessage(client, "need shield", client);
    return;
  }
  wcGetTalentInfo(4,2,1,3,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new Float:bonus = float(GetArmor(client)) * multi;
  SetBonusArmor(client, float(GetBonusArmor(client)) + bonus);
  wcRecalculateStats(client);
  return;
}

public wcRunTalent_4_8(client)                                                  //Divine Favor
{
  SetEffectSpellVic(client,32);
}

public wcRunTalent_4_10(client)                                                 //Blessing of Might
{
  decl String:line[255];
  wcGetTalentInfo(4,3,1,3,client,"bonus",line);
  new Float:dps=StringToFloat(line);
  CastMight(client,dps);
}

stock bool:CastMight(client, Float:dps, bool:cast=false, bool:custom=false)
{
   if((!custom) && g_fMightClient[client]>=dps)
      return false;
   SetBonusDamage(client, GetBonusDamage(client)+dps-g_fMightClient[client]);
   wcRecalculateStats(client);
   if(!custom)
    g_fMightClient[client]=dps;
   if(cast) {
      WCMessage(client,"might aff",client);
   }
   return true;
}

public Action:CastMightAll(Handle:timer, any:client)
{
   decl String:line[255];
   wcGetTalentInfo(4,3,1,3,client,"bonus",line);
   new Float:dps=StringToFloat(line);
   WCMessage(client,"might cast all",client);
   new clients[MAXPLAYERS];
   new team=GetClientTeam(client);
   new amount;
   if(team==2)
      amount=FindMatchingPlayers("@t",clients);
   if(team==3)
      amount=FindMatchingPlayers("@ct",clients);
   for(new i=0;i<amount;i++)
      CastMight(clients[i],dps+3,true);
}

public wcRunTalent_4_11(client)                                                 //Greater Blessing of Might
{
  CreateTimer(0.5, CastMightAll, client);
}

public wcRunTalent_4_12(client)                                                 //Sheath of Light
{
  if(IsDoubleHand(client))
  {
    decl String:line[255];
    wcGetTalentInfo(4,3,2,2,client,"multi",line);
    new Float:bonus = StringToFloat(line);
    bonus *= float(GetSpellDmg(client,1.0));
    bonus += GetBonusDamage(client);
    SetBonusDamage(client, bonus);
    wcRecalculateStats(client);
  }
}

public wcRunTalent_4_13(client)                                                 //Eye for an Eye
{
  SetEffectVictim(client,34);
  SetEffectSpellVic(client,34);
}

public wcRunTalent_4_14(client)                                                 //Holy Guidance
{
  decl String:line[255];
  wcGetTalentInfo(4,1,1,2,client,"multi",line);
  new Float:bonus=StringToFloat(line);
  bonus *= (GetInt(client)+GetBonusInt(client))/5.0;
  bonus += GetBonusSpellDmg(client, 1.0);
  SetBonusSpellDmg(client, bonus);
  wcRecalculateStats(client);
}

public wcRunTalent_4_15(client)                                                 //Reckoning
{
  decl String:line[255];
  wcGetTalentInfo(4,2,4,2,client,"attacks",line);
  SetReckoning(client, GetReckoning(client)+StringToInt(line));
  WCMessage(client,"reckoning procc",client);
}

wcRunTalent_4_BlessedLife(client)
{
  decl String:line[255];
  wcGetSpellInfo(4,3,"cooldowntype",line);
  new type = StringToInt(line);
  wcGetTalentInfo(4,1,1,4,client,"cooldown",line);
  new Float:talent=StringToFloat(line);
  new Float:cd = GetCDSpell(client,type)-GetTickedTime();
  if (cd<talent)
    cd=0.0;
  else
    cd-=talent;
  SetCdSpell(client,type,cd,3);
}

wcRunTalent_4_BeaconOfLight(client)
{
  decl String:line[255];
  wcGetTalentInfo(4,1,4,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  WCMessage(client,"beacon of light",client);
  SetHealingBonus(client, GetHealingBonus(client)+multi);
}

wcRunTalent_4_DivineBody(client,&heal)
{
  decl String:line[255];
  wcGetTalentInfo(4,2,1,4,client,"multi",line);
  heal+=RoundToNearest(heal*StringToFloat(line));
}

wcRunTalent_4_HolyShield(client)
{
  decl String:line[255];
  wcGetTalentInfo(4,2,2,4,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if(chance >= rand)
  {
    SetHolyPower(client,GetHolyPower(client)+1);
  }
}

wcRunTalent_4_Sanctuary(client)
{
  decl String:line[255];
  wcGetTalentInfo(4,2,3,2,client,"mana",line);
  RestoreManaP(client,StringToFloat(line));
}

wcRunTalent_4_DivineInt(client, &damage)
{
  decl String:line[255];
  new hp = wcGetHealth(client);
  if(damage >= hp)
  {
    wcGetTalentInfo(4,2,3,3,client,"chance",line);
    new chance = StringToInt(line);
    new rand = GetRandomInt(1,100);
    if(chance>=rand)
    {
      if(HasGlyph(client, GLYPH_PALADIN_DIVINT))
      {
        new Float:duration = 30.0;
        SetIceBlocks(client, 1);
        new effid=CreateEffectData(client,2,2,0,0,0,0,0,g_RoundCount,0);
        if(effid!=0)
        {
          CreateTimer(duration/1.0, EffectIceBlock, effid, TIMER_REPEAT);
          WCMessage(client,"divine intervention glyph",client);
        }
      }
      wcHeal(client,client,damage,0,false);
      damage=0;
      decl String:buffer[30];
      Format(buffer,sizeof(buffer),"%T", "divine intervention", client);
      AddEMsgAff("divine intervention", true);
    }
  }
}

/*wcRunTalent_4_DivineGuardian(client)
{

  WCMessage(client,"divine guardian",client);
  new clients[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=FindMatchingPlayers("@t",clients);
  if(team==3)
    amount=FindMatchingPlayers("@ct",clients);
  for(new i=0;i<amount;i++)
  {
    if(IsClientInGame(clients[i]))
    {
      SetEffectVictim(clients[i],44);
      SetEffectSpellVic(clients[i],44);
    }
  }
}*/

wcRunTalent_4_SpeedOfLight(client)
{
  decl String:line[255];
  wcGetTalentInfo(4,3,1,4,client,"increment",line);
  new Float:bonus=StringToFloat(line);
  SetBonusAttackSpeed(client,GetBonusAttackSpeed(client)+bonus);
  SetNormalBonusSpeed(client,GetNormalBonusSpeed(client)+bonus);
  wcRecalculateStats(client);
}

wcRunTalent_4_DivineBlood(client)
{
  decl String:line[255];
  wcGetTalentInfo(4,3,2,4,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if (chance >= rand)
  {
    SetHolyPower(client,GetHolyPower(client)+1);
  }
}

wcRunTalent_4_VengefulWarrior(client,victim)
{
  decl String:line[255];
  wcGetTalentInfo(4,3,3,2,client,"chance",line);
  new chance = StringToInt(line);
  decl Float:clienta[3],Float:targeta[3];
  GetClientEyeAngles(victim, clienta);
  GetClientEyeAngles(client, targeta);
  new Float:angle=clienta[1]-targeta[1];
  if(angle<0.0)
    angle*=-1.0;
  if(angle>180.0)
    angle = 360.0-angle;

  if( (chance>=GetRandomInt(1,100)) && ((angle > 100.0) || IsValidShooting(client, victim) ))
  {
    dealExtraHit(client,victim,RoundToNearest(float(GetDamage(client))/2.0),"Vengeful Warrior","Vengeful Warrior");
  }
}

wcRunTalent_4_GrandCrussade(client,&damage) //Templar's Verdict
{
  decl String:line[255];
  wcGetTalentInfo(4,3,4,1,client,"chance",line);
  new chance = StringToInt(line);
  new random = GetRandomInt(1,100);
  new power = GetHolyPower(client);
  if(chance >= random && power >= 1)
  {
    wcGetTalentInfo(4,3,4,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    multi*=GetDamage(client);


    damage+= RoundToNearest(damage+multi);
    if(!HasGlyph(client, GLYPH_PALADIN_VERDICT) || GetRandomInt(1,100) > 25)
      SetHolyPower(client,GetHolyPower(client)-1);
    /*decl String:buffer[30];*/
    /*Format(buffer,sizeof(buffer),"%T", "grand crussade",client);*/
    AddEMsgAff("grand crussade", true);
  }
}

wcRunTalent_4_RighteusWarrior(client, power)
{
  decl String:line[255];
  wcGetTalentInfo(4,3,2,3,client,"multi",line);
  new Float:multi=StringToFloat(line);
  multi*=power;

  RestoreManaP(client,multi);
  new restore = RoundToNearest(float(GetMaxHealth(client))*multi);
  wcHeal(client,client,restore,0,false);
}

public wcRunTalent_5_1(client)                                                  //Pain Suppresion
{
  decl String:line[255];
  wcGetTalentInfo(5,1,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(5,1,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 1, 3, 1);
  wcGetTalentInfo(5,1,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  wcGetTalentInfo(5,1,3,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Pain Suppresion");
  WCMessage(client,"pain suppression",client,RoundToNearest(multi*100),duration);

  SetEffectVictim(client,26);
  SetEffectSpellVic(client,26);
  SetEffectAttacker(client,53);



  new effid=CreateEffectData(client,26,10,0,0,0,0,0,g_RoundCount,0);
  if(effid!=0)
  {
      effectsdata[effid][7]=PainEffect(client);
      effectsdata[effid][6]=PainEffect2(client);
      CreateTimer(duration/10.0, EffectPain, effid, TIMER_REPEAT);
  }
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/painsuppression.wav",location);
}

stock PainEffect(target)  {
  new Float:pos[3];
  GetClientAbsOrigin(target,pos);

  VS_BeamRingPoint(pos, 10.00, 101.00, g_ef_lgtning, g_ef_rollermine, 100, 300, 0.30, 40.00, 0.30, {255, 255, 125, 255}, 30, 0);


  new sprite = CreateEntityByName("env_smokestack");
  if (IsValidEntity(sprite)) {
      decl String:sTemp[30];
      Format(sTemp,sizeof(sTemp),"player_%i",target);
      DispatchKeyValue(target, "targetname", sTemp);
      DispatchKeyValue(sprite, "basespread", "50");
      DispatchKeyValue(sprite, "spreadspeed", "10");
      DispatchKeyValue(sprite, "speed", "50");
      DispatchKeyValue(sprite, "startsize", "5");
      DispatchKeyValue(sprite, "endsize", "1");
      DispatchKeyValue(sprite, "rate", "200");
      DispatchKeyValue(sprite, "jetlength", "5");
      DispatchKeyValue(sprite, "twist", "10");
      DispatchKeyValue(sprite, "angles", "0 90 90");
      DispatchKeyValue(sprite, "rendercolor", "20 20 255");
      DispatchKeyValue(sprite, "SmokeMaterial", "models/Roller/rollermine_glow.vmt");
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

stock PainEffect2(target) {
  new sprite = CreateEntityByName("env_lightglow");
  if (IsValidEntity(sprite)) {
      decl String:sTemp[30];
      Format(sTemp,sizeof(sTemp),"player_%i",target);
      DispatchKeyValue(target, "targetname", sTemp);
      DispatchKeyValue(sprite, "rendercolor", "16 75 125");
      DispatchKeyValue(sprite, "VerticalGlowSize", "100");
      DispatchKeyValue(sprite, "HorizontalGlowSize", "100");
      DispatchKeyValue(sprite, "OuterMaxDist", "900");
      DispatchKeyValue(sprite, "MinDist", "20");
      DispatchKeyValue(sprite, "MaxDist", "200");
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

public Action:EffectPain(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new eff = effectsdata[effid][7];
  new eff2 = effectsdata[effid][6];
  if (!IsClientInGame(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    KillEntity(eff2);
    return Plugin_Stop;
  }
  new client = effectsdata[effid][1];
  if (!IsPlayerAlive(client))
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    KillEntity(eff2);
    DestroyPain(INVALID_HANDLE, client);
    return Plugin_Stop;
  }
  if(effectsdata[effid][2]!=26)   {
    DeleteEffectData(effid);
    KillEntity(eff);
    KillEntity(eff2);
    DestroyPain(INVALID_HANDLE, client);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    KillEntity(eff2);
    DestroyPain(INVALID_HANDLE, client);
    return Plugin_Stop;
  }
  new ticks = effectsdata[effid][3]--;
  if (ticks>0)
  {
    return Plugin_Continue;
  }
  DeleteEffectData(effid);
  KillEntity(eff);
  KillEntity(eff2);
  DestroyPain(INVALID_HANDLE, client);
  return Plugin_Stop;
}

public Action:DestroyPain(Handle:timer, any:client)
{
  WCMessage(client,"pain supp deac",client);
  RemoveEffectVictim(client,26);
  RemoveEffectSpellVic(client,26);
  RemoveEffectAttacker(client,262);
}

public wcRunTalent_5_2(client) //Circle of Healing
{
  decl String:line[255];
  wcGetTalentInfo(5,2,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(5,2,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 2, 3, 1);

  wcGetTalentInfo(5,2,4,2,client,"mana",line);
  new mana2 = StringToInt(line);
  if(mana2>0) {
    manareq+= mana2;
  }
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Circle of Healing");
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetTalentInfo(5,2,2,2,client,"multi",line);
  new Float:talentbonus=1+StringToFloat(line);
  wcGetTalentInfo(5,2,3,1,client,"range",line);
  new Float:distance=StringToFloat(line)*talentbonus;
  wcGetTalentInfo(5,2,3,1,client,"multi",line);
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
  wcGetTalentInfo(5,2,3,1,client,"base",line);
  heal+=StringToInt(line);
  wcGetTalentInfo(5,2,4,2,client, "multi", line);
  new Float:multi2 = StringToFloat(line);
  if(multi2 > 1.0)  {
    heal = RoundToNearest(multi2 * heal);
  }
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
      WCMessage(targets[x],"circle of healing target",targets[x],name,healed);
    }
  }
  if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);
  new color[4]={250,250,17,255};
  location[2]+=50;
  BeamRingEffect("@all",location,150.0,90.0,g_purplelaser1,0,10,3.0,80.0,color,0.0,3);
  WCMessage(client,"circle of healing caster",client,stargets,heal);
  EmitAmbientSound("wc/spells/heal.wav",location);
  VS_BeamRingPoint(location, 50.00, 100.00, g_ef_lgtning, g_ef_lgtning, 20, 50, 3.00, 50.00, 1.00, {255, 255, 20, 255}, 10, 0);
  VS_BeamRingPoint(location, 99.00, 100.00, g_ef_lgtning, g_ef_lgtning, 20, 50, 3.00, 10.00, 5.00, {255, 255, 20, 255}, 10, 0);

}

public wcRunTalent_5_3(client)                                                  //Shadow Form.
{
   decl String:line[255];
   wcGetTalentInfo(5,3,3,1,client,"cooldowntype",line);
   new cdtype=StringToInt(line);
   wcGetTalentInfo(5,3,3,1,client,"cooldown",line);
   new Float:cd=StringToFloat(line);

   wcGetTalentInfo(5,3,4,2,client, "cd", line);
   new Float:minusCd = StringToFloat(line);
   if(minusCd > 0.0)
    cd -= minusCd;

   if(g_bShadowform[client] && !HasGlyph(client, GLYPH_PRIEST_SHADOWF))
   {
      RemoveEffectVictim(client,28);
      RemoveEffectSpellVic(client,28);
      RemoveEffectSpellAtt(client,27);
      WCMessage(client,"shadow form deactivated",client);
      g_bShadowform[client]=false;
      SetCdSpell(client,cdtype,cd, 0, "Shadow Form");
   }
   else if(!g_bShadowform[client])
   {
     SetEffectVictim(client,28);
     SetEffectSpellVic(client,28);
     SetEffectSpellAtt(client,27);
     WCMessage(client,"shadow form activated",client);
     g_bShadowform[client]=true;
     new manareq = GetTalentCost(client, 3, 3, 1);
     wcSetMana(client,GetMana(client)-manareq);
     SetCdSpell(client,cdtype,cd, 0, "Shadow Form");
   }
}

wcRunTalent_5_4(client,bool:noisy) { //Inner Fire
  decl String:line[255];
  wcGetTalentInfo(5,1,1,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new Float:bonus=multi*GetLevel(client);
  SetBonusArmor(client, float(GetBonusArmor(client)) + bonus);
  if(HasGlyph(client, GLYPH_PRIEST_INNER))
  {
    new lvl = wcGetTalentLevel(client,1,1,1);
    new sp = GetSpellDmg(client, 1.0);
    new Float:multi2 = 0.034*lvl;
    if(multi2 > 0.10)
      multi2 = 0.10;
    new Float:bonus2 = sp * multi2;
    SetBonusSpellDmg(client, float(GetBonusSpellDmg(client,1.0)) + bonus2);

    wcRecalculateStats(client);
    if(noisy)
    {
      WCMessage(client, "inner fire used", client,        RoundToNearest(bonus),  GetArmor(client));
      WCMessage(client, "inner fire glyph used", client,  RoundToNearest(bonus2), GetSpellDmg(client, 1.0));
    }
  }
  else
  {
    wcRecalculateStats(client);
    if(noisy)
      WCMessage(client, "inner fire used", client, RoundToNearest(bonus), GetArmor(client));
  }
}

wcRunTalent_5_5(client,bool:noisy) { //Meditation
  decl String:line[255];
  wcGetTalentInfo(5,1,1,2,client,"manaregen",line);
  new bonus=StringToInt(line);
  SetBonusManaReg(client, GetBonusManaReg(client)+bonus);
  wcRecalculateStats(client);
  if(noisy)
    WCMessage(client,"meditation used", client, bonus, GetManaReg(client));
}

wcRunTalent_5_6(client,bool:noisy) { //Mental Strength
  decl String:line[255];
  wcGetTalentInfo(5,1,3,2,client,"multi",line);
  new base =  GetInt(client)+GetBonusInt(client);
  new bonus = RoundToNearest(StringToFloat(line)*base);
  SetBonusInt(client, bonus+GetBonusInt(client));
  if(HasGlyph(client, GLYPH_PRIEST_MENTAL))
  {
    wcGetTalentInfo(5,1,3,2,client,"multi",line);
    new mana = RoundToNearest(GetMaxMana(client)*(StringToFloat(line)));
    wcClientsBonusStats[client][MAXMP]+=mana;
    wcRecalculateStats(client);
    wcGetTalentInfo(5,1,3,2,client,"multi",line);
    new newMana=RoundToNearest(GetMaxMana(client) * StringToFloat(line));
    wcSetMana(client,newMana);
  }
  else
    wcRecalculateStats(client);
  if(noisy)
    WCMessage(client,"mental strength used", client, bonus, GetInt(client)+GetBonusInt(client));
}

public wcRunTalent_5_7(client) //Prayer of Fortitude
{
  CreateTimer(0.5, CastFortAll, client);
}

stock bool:CastFort(client, bonus, bool:cast=false, bool:custom=false,caster=0)
{
  if((!custom) && g_iFortCast[client]>=bonus)
  return false;
  SetBonusSta(client, GetBonusSta(client)+bonus-g_iFortCast[client]);
  wcRecalculateStats(client);
  wcSetHealth(client,wcGetHealth(client)+RoundToNearest(float(bonus)*0.78));
  if(!custom)
  g_iFortCast[client]=bonus;

  if(caster > 0)
  {
    decl String:name[50];
    GetClientName(caster, name, sizeof(name));
    if(!(IsChat(client,Chat:Chat_Spawn)))
      WCMessage(client,"stamina casttoyou",client,bonus,name);
  }
  return true;
}

public Action:CastFortAll(Handle:timer, any:client)
{
  if(!IsClientInGame(client) || !IsPlayerAlive(client))  return;
  decl String:line[255];
  wcGetTalentInfo(5,1,2,3,client,"multi",line);
  new bonus=RoundToNearest(float(GetLevel(client))*StringToFloat(line));
  if(!(IsChat(client,Chat:Chat_Spawn)))
    WCMessage(client,"stamina cast all",client);
  new clients[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=FindMatchingPlayers("@t",clients);
  if(team==3)
    amount=FindMatchingPlayers("@ct",clients);
  for(new i=0;i<amount;i++)
  {
    if(IsClientInGame(clients[i]))
      CastFort(clients[i],bonus,true,false,client);
  }
}
public wcRunTalent_5_8(client, target) //Inspiration
{
  decl String:line[255];
  wcGetTalentInfo(5,2,1,2,client,"chance",line);
  new chance = StringToInt(line);
  new random = GetRandomInt(1,100);
  if (chance >= random)
  {
    SetEffectVictim(target,36);
    wcGetTalentInfo(5,2,1,2,client,"duration",line); //Inspiration
    new Float:duration = StringToFloat(line);
    wcGetTalentInfo(5,2,1,2,client,"increment",line);
    WCMessage(target,"inspiration",target,StringToFloat(line)*100.0,duration);
    g_iInspiration[target]=client;
    new Handle:datapack3=CreateDataPack();
    WritePackCell(datapack3,target);
    WritePackCell(datapack3,36);
    CreateTimer(duration,DestroyEffectVictim,datapack3,TIMER_HNDL_CLOSE);
  }
}

public wcRunTalent_5_9(client)                                                  //Blessed Ressilience
{
  if(!GetBlessedResillience(client))
  {
    decl String:line[255];
    SetBlessedResillience(client,true);
    wcGetTalentInfo(5,2,2,3,client,"duration",line);
    new Float:duration=StringToFloat(line);
    WCMessage(client,"blessed ress",client,duration);
    new Handle:datapack3=CreateDataPack();
    WritePackCell(datapack3,client);
    CreateTimer(duration,EndBlessedRessilience,datapack3,TIMER_HNDL_CLOSE);
  }
}

public Action:EndBlessedRessilience(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  SetBlessedResillience(client,false);
}

public wcRunTalent_5_10(client) //Shadow Word:Death
{
  new target=GetAimTarget(client,25.0);
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      decl String:line[255];
      wcGetTalentInfo(5,3,1,3,client,"multi",line);
      new Float:talentbonus=StringToFloat(line);
      wcGetTalentInfo(5,3,2,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(5,3,2,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      new manareq = GetTalentCost(client, 3, 2, 1);
      manareq=RoundToNearest(float(manareq)*(1.0-talentbonus));
      wcSetMana(client,GetMana(client)-manareq);
      SetCdSpell(client,cdtype,cd, 0, "Shadow Word:Death");
      wcGetTalentInfo(5,3,2,1,client,"multi",line);
      new Float:multi=StringToFloat(line);
      new dmg=GetSpellDmg(client,multi);
      wcGetTalentInfo(5,3,2,1,client,"base",line);
      dmg+=StringToInt(line);
      if(HasGlyph(client, GLYPH_PRIEST_DEATH))  {
        new Float:hpp = float(wcGetHealth(target)) / GetMaxHealth(target);
        if(hpp <= 0.35)
          dmg = RoundToNearest(1.5 * dmg);
      }
      decl String:vicMsg[300], String:attMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","shadow death target", target);
      Format(attMsg,sizeof(attMsg),"%T","shadow death caster", client);
      wcGetTalentInfo(5,3,2,1,client,"name",line);
      wcSpellDamage(target,client,dmg, attMsg, vicMsg,line);
      //effect
      new Float:targpos[3];
      GetClientAbsOrigin(target,targpos);
      TE_SetupBeamRingPoint(targpos, 20.0, 80.0,ChaosSprite,ChaosSprite, 0, 5, 2.6, 20.0, 0.0, {100,100,100,100}, 10,FBEAM_HALOBEAM);
      TE_SendToAll();
      targpos[2]+=20.0;
      TE_SetupBeamRingPoint(targpos, 40.0, 100.0,ChaosSprite,ChaosSprite, 0, 5, 2.4, 20.0, 0.0, {100,100,100,100}, 10,FBEAM_HALOBEAM);
      TE_SendToAll();
      targpos[2]+=20.0;
      TE_SetupBeamRingPoint(targpos, 60.0, 120.0,ChaosSprite,ChaosSprite, 0, 5, 2.2, 20.0, 0.0, {100,100,100,100}, 10, FBEAM_HALOBEAM);
      TE_SendToAll();
      targpos[2]+=20.0;
      TE_SetupBeamRingPoint(targpos, 80.0, 140.0,ChaosSprite,ChaosSprite, 0, 5, 2.0, 20.0, 0.0, {100,100,100,100}, 10, FBEAM_HALOBEAM);
      TE_SendToAll();	
      targpos[2]+=20.0;
      TE_SetupBeamRingPoint(targpos, 100.0, 160.0,ChaosSprite,ChaosSprite, 0, 5, 1.8, 20.0, 0.0, {100,100,100,100}, 10, FBEAM_HALOBEAM);
      TE_SendToAll();	
      targpos[2]+=20.0;
      TE_SetupBeamRingPoint(targpos, 120.0, 180.0,ChaosSprite,ChaosSprite, 0, 5, 1.6, 20.0, 0.0, {100,100,100,100}, 10, FBEAM_HALOBEAM);
      TE_SendToAll();	
      targpos[2]+=20.0;
      TE_SetupBeamRingPoint(targpos, 140.0, 200.0,ChaosSprite,ChaosSprite, 0, 5, 1.4, 20.0, 0.0, {100,100,100,100}, 10, FBEAM_HALOBEAM);
      TE_SendToAll();	
      targpos[2]+=20.0;
      TE_SetupBeamRingPoint(targpos, 160.0, 220.0,ChaosSprite,ChaosSprite, 0, 5, 1.2, 20.0, 0.0, {100,100,100,100}, 10, FBEAM_HALOBEAM);
      TE_SendToAll();	
      targpos[2]+=20.0;
      TE_SetupBeamRingPoint(targpos, 180.0, 240.0,ChaosSprite,ChaosSprite, 0, 5, 1.0, 20.0, 0.0, {100,100,100,100}, 10, FBEAM_HALOBEAM);
      TE_SendToAll();
      EmitAmbientSound("ambient/levels/prison/inside_battle_zombie1.wav",targpos);

      if(!IsPlayerAlive(target))
      {
        //Now if target is dead, give hp :) v3 ? lol
        //
        wcHeal(client,client,dmg,0,false);

        //dmg=RoundToNearest(dmg*float(wcGetHealth(target))/GetMaxHealth(target));
        //Format(attMsg,sizeof(vicMsg),"%T","shadow death self target", target);
        //Format(vicMsg,sizeof(attMsg),"%T","shadow death self caster", client);
        //wcSpellDamage(client,target,dmg, attMsg, vicMsg,line);
      }
    }
    else  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
}

public wcRunTalent_5_11(client, damage) //Vampiric Embrace
{
  decl String:line[255];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetTalentInfo(5,3,2,3,client,"range",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(5,3,2,3,client,"multi",line);
  new Float:multi=StringToFloat(line);
  if(g_bShadowform[client])
  {
    wcGetTalentInfo(5,3,4,2,client, "increase", line);
    new Float:shadowbonus = StringToFloat(line);
    if(shadowbonus > 1.00)
    {
      distance*=shadowbonus;
      multi*=shadowbonus;
    }
  }
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
  new heal=RoundToNearest(damage*multi);
  for (new x=0;x<=(amount-1);x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
       Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
       Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
    new healed=wcHeal(client,targets[x],heal,0,false);
    if(targets[x]!=client)
    {
      WCMessage(targets[x],"vampiric embrace target",targets[x],name,healed);
    }
  }
  WCMessage(client,"vampiric embrace caster",client,stargets,heal);
}

public wcRunTalent_5_12(client)                                                 //Spirit Tap
{
  new mana = GetMana(client);
  decl String:line[255];
  wcGetTalentInfo(5,3,1,1,client,"multi",line);
  new Float:multi = StringToFloat(line);
  if(g_bShadowform[client])
  {
    wcGetTalentInfo(5,3,4,2,client, "increase", line);
    new Float:shadowbonus = StringToFloat(line);
    if(shadowbonus > 1.00)
    {
      multi*=shadowbonus;
    }
  }
  new add = RoundToNearest(float(GetManaReg(client))*multi);
  wcSetMana(client, mana+add);
  WCMessage(client,"spirit tab",client,add);
}

public wcRunTalent_5_13(client, mana)                                           //Improved Healing
{
  decl String:line[255];
  wcGetTalentInfo(5,2,1,1,client,"multi",line);
  mana -= RoundToNearest(mana*StringToFloat(line));
  return mana;
}

public wcRunTalent_5_14(client, mana)                                           //Focused Mind
{
  decl String:line[255];
  wcGetTalentInfo(5,3,1,3,client,"multi",line);
  new Float:multi = StringToFloat(line);
  if(g_bShadowform[client])
  {
    wcGetTalentInfo(5,3,4,2,client, "increase", line);
    new Float:shadowbonus = StringToFloat(line);
    if(shadowbonus > 1.00)
    {
      multi*=shadowbonus;
    }
  }
  mana -= RoundToNearest(float(mana)*multi);
  return mana;
}

public wcRunTalent_5_15(client, damage, attacker)                               //Reflective Shield
{
  decl String:line[255];
  if(!IsPlayerAlive(attacker))
    return;
  new caster = GetReflectiveShield(client);
  wcGetTalentInfo(5,1,2,1,caster,"multi",line);
  new dmg = RoundToNearest(damage*StringToFloat(line));
  decl String:vicMsg[100],String:attMsg[100];
  Format(vicMsg,sizeof(vicMsg),"%T","reflective shield attacker", attacker);
  Format(attMsg,sizeof(attMsg),"%T","reflective shield victim", caster);
  wcGetTalentInfo(5,1,2,1,caster,"name",line);
  if(dmg > 0)
    wcSpellDamage(attacker,caster,dmg, attMsg, vicMsg,line);
  return;
}

wcRunTalent_5_StrengthOfSoul(client) //Strength of Soul
{
  decl String:line[255];
  wcGetSpellInfo(5,4,"cooldowntype",line);
  new type = StringToInt(line);
  wcGetTalentInfo(5,1,1,4,client,"cooldown",line); //Strength of Soul
  new Float:talent=StringToFloat(line);
  new Float:cd = GetCDSpell(client,type)-GetTickedTime();
  if (cd<talent)
    cd=0.0;
  else
    cd-=talent;
  SetCdSpell(client,type,cd,4);

  if(CheckRequirements(client,5,1,4,1)) //PW:Barrier
  {
    wcGetTalentInfo(5,1,4,1,client,"cooldowntype",line);
    type = StringToInt(line);
    cd = GetCDSpell(client,type)-GetTickedTime();
    if(cd<talent)
      cd=0.0;
    else
      cd-=talent;
    SetCdSpell(client, type, cd, 0, "PW: Barrier");
  }
}

wcRunTalent_5_SoulWarding(client, &damage)
{
  decl String:line[255];
  new caster = GetReflectiveShield(client);
  wcGetTalentInfo(5,1,2,4,caster,"multi",line);
  damage = RoundToNearest(damage*(1-StringToFloat(line)));
}

wcRunTalent_5_FocusedWill(client,damage) //Focused Will
{
  decl String:line[255];
  new maxhp = GetMaxHealth(client);
  wcGetTalentInfo(5,1,2,2,client,"healthper",line);
  new Float:hp = StringToFloat(line);
  if (damage >= RoundToNearest(maxhp*hp))
  {
    wcGetTalentInfo(5,1,2,2,client,"chance",line);
    new chance = StringToInt(line);
    new rand = GetRandomInt(1,100);
    if (chance >= rand)
    {
      SetFocusedWill(client,true);
      decl Float:location[3];
      GetClientAbsOrigin(client,location);
      EmitAmbientSound("wc/spells/clearcast.wav",location);
    }
  }
}

wcRunTalent_5_FocusedWillEffect(client,&mana)
{
  if(GetFocusedWill(client))
    mana=0;
}

wcRunTalent_5_PWBarrier(client) //Power Word: Barrier
{
  decl String:line[255];
  wcGetTalentInfo(5,1,4,1,client,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(5,1,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(5,1,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 1,4,1);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, 0, "PW: Barrier");

  new Float:location[3];
  GetClientAbsOrigin(client,location);
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
  wcGetTalentInfo(5,1,4,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new absorb=GetSpellDmg(client,multi);
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
  WCMessage(client,"pw barrier",client,stargets,absorb);
  wcGetTalentInfo(5,1,4,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  for (new x=0;x<=(amount-1);x++)
  {
    /*new absorb2=0;
    if(targets[x] == client)  {
      wcGetTalentInfo(5,1,4,2,client,"multi",line);
      new Float:multi2 = StringToFloat(line);
      if(multi2 > 0.0)  {
        absorb2 += RoundToNearest(multi2 * amount * absorb);
      }
    }*/
    wcClientAbsorb[targets[x]]=absorb;
    new item[UniqueItem];
    GetCharItem(item, client, 20);
    if(item[Item_defId]==25264)
      wcHeal(client,targets[x],absorb/2,0,true);
    //wcClientAbsorb[targets[x]]=absorb+absorb2;
    g_iShieldStack[targets[x]]+=1;
    SetEffectVictim(targets[x],5);
    SetEffectSpellVic(targets[x],5);
    SetReflectiveShield(targets[x],client);

    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,targets[x]);
    WritePackCell(datapack2,5);
    CreateTimer(duration,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
    new Handle:datapack3=CreateDataPack();
    WritePackCell(datapack3,targets[x]);
    WritePackCell(datapack3,5);
    CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);
    //WCMessage(targets[x],"pw barrier vic",targets[x],duration,absorb+absorb2,name);
    WCMessage(targets[x],"pw barrier vic",targets[x],duration,absorb,name);
    if(client!=targets[x])
    {
      wcHealingDone[client]+=RoundToNearest(float(absorb)/2.0);
    }
    GenerateHealThreat(client, RoundToNearest(float(absorb)/2.0));


  }
  location[2]+=50.0;
  VS_BeamRingPoint(location, 1.00, 2000.00, g_bluelight1, g_bluelight1, 100, 300, 0.50, 200.00, 1.00, {255, 255, 255, 255}, 300, 0);
  decl Float:soundlocation[3];
  GetClientAbsOrigin(client,soundlocation);
}

wcRunTalent_5_DivineAegis(client,target,heal)
{
  decl String:line[255];
  wcGetTalentInfo(5,1,3,3,client,"multi",line);
  new Float:multi = StringToFloat(line);
  wcGetTalentInfo(5,1,1,3,client,"increment",line); //Improved power word shield
  new Float:talentbonus = StringToFloat(line);
  multi*=(1.0+talentbonus);
  new absorb = RoundToNearest(heal*multi);
  if(absorb > wcClientAbsorb[target])
  {
    wcClientAbsorb[target]=absorb;
    g_iShieldStack[target]+=1;
    SetEffectVictim(target,5);
    SetEffectSpellVic(target,5);
    wcGetTalentInfo(5,1,3,3,client,"duration",line);
    new Float:duration=StringToFloat(line);
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,target);
    WritePackCell(datapack2,5);
    CreateTimer(duration,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
    new Handle:datapack3=CreateDataPack();
    WritePackCell(datapack3,target);
    WritePackCell(datapack3,5);
    CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);

    if(client==target)
    {
      if(talentbonus>0.0)
        WCMessage(client,"imp pw shield used self",client,duration,absorb,"Divine Aegis: ");
      else
        WCMessage(client,"pw shield used self",client,duration,absorb,"Divine Aegis: ");
    }
    else
    {
      decl String:name[50],String:name2[50];
      GetClientName(client,name,sizeof(name));
      GetClientName(target,name2,sizeof(name2));
      if(talentbonus>0.0)
      {
        WCMessage(client,"imp pw shield used caster",client,duration,absorb,name2,"Divine Aegis: ");
        WCMessage(target,"imp pw shield used victim",target,duration,absorb,name,"Divine Aegis: ");
      }
      else
      {
        WCMessage(client,"pw shield used caster",client,duration,absorb,name2,"Divine Aegis: ");
        WCMessage(target,"pw shield used victim",target,duration,absorb,name,"Divine Aegis: ");
      }
    }
  }
}

wcRunTalent_5_BodyAndSoul(client,heal) //Body and Soul
{
  if(!IsBoss())
  {
    decl String:line[255];
    wcGetTalentInfo(5,2,2,4,client,"multi",line);
    new Float:multi=StringToFloat(line);
    WCMessage(client,"body and soul used",client,RoundToNearest(heal*multi));
    wcHeal(client,client,RoundToNearest(heal*multi),0,false);
  }
}

wcRunTalent_5_SurgeOfLight(client,spellid)
{
  decl String:line[255];
  wcGetTalentInfo(5,2,1,4,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if(chance >= rand)
  {
    new mana = RoundToNearest(float(GetSpellCost(client,spellid))/2);
    if(mana > 0)
    {
      SetBonusManaReg(client,GetBonusManaReg(client)+mana);
      wcGetTalentInfo(5,2,1,4,client,"duration",line);
      new Float:duration=StringToFloat(line);
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,client);
      WritePackCell(datapack4,mana);
      WritePackCell(datapack4,g_RoundCount);
      CreateTimer(duration,EndSoL,datapack4,TIMER_HNDL_CLOSE);
    }
  }
}

public Action:EndSoL(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  new mana=ReadPackCell(datapack);
  new round = ReadPackCell(datapack);
  if(round==g_RoundCount)
  {
    SetBonusManaReg(client,GetBonusManaReg(client)-mana);
  }
}

wcRunTalent_5_DesperatePrayer(client,damage)
{
  if(damage>=wcGetHealth(client))
  {
    decl String:line[255];
    wcGetTalentInfo(5,2,3,2,client,"cooldown",line);
    new Float:cd = StringToFloat(line);
    wcGetTalentInfo(5,2,3,2,client,"cooldowntype",line);
    new cdtype = StringToInt(line);
    new manareq = GetTalentCost(client,2,3,2);
    wcGetTalentInfo(5,2,3,2,client,"name",line, client);
    SetCdSpell(client,cdtype,cd,0,line);
    wcSetMana(client,GetMana(client)-manareq);
    wcGetTalentInfo(5,2,3,2,client,"multi",line);
    new heal = GetSpellDmg(client,StringToFloat(line));
    wcHeal(client,client,heal,0,true);
    decl Float:location[3];
    GetClientAbsOrigin(client,location);
    EmitAmbientSound("wc/spells/heal.wav",location);
    WCMessage(client,"desperate prayer",client,heal);
  }
}

wcRunTalent_5_SoR(client)
{
  decl String:line[255];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetTalentInfo(5,2,3,3,client,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(5,2,3,3,client,"multi",line);
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
      WCMessage(targets[x],"sor target",targets[x],name,healed);
    }
  }
  if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",client);
  WCMessage(client,"sor caster",client,stargets,heal);
  EmitAmbientSound("wc/spells/heal.wav",location);
}

wcRunTalent_5_LightWell(client)
{
  decl String:line[255];

  //CD and mana
  wcGetTalentInfo(5,2,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(5,2,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client, cdtype, cd, 0, "Light Well");
  new manareq = GetTalentCost(client, 2,4,1);
  wcSetMana(client,GetMana(client)-manareq);
  //Channeling
  wcGetTalentInfo(5,2,4,1,client,"duration",line);
  new Float:duration=StringToFloat(line);

  //Getting values
  wcGetTalentInfo(5,2,4,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(5,2,4,1,client,"period",line);
  new Float:period=StringToFloat(line);
  period = period*(1.0-GetHasteEffect(client));
  wcGetTalentInfo(5,2,4,1,client,"distance",line);
  new aoe=StringToInt(line);
  new heal = GetSpellDmg(client, multi);

  WCMessage(client,"light well cast",client,heal);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  //Setting up effect
  new team=GetClientTeam(client);
  new effid=CreateEffectData(client,cdtype,heal,RoundToNearest(location[0]),aoe,team,RoundToNearest(location[1]),RoundToNearest(location[2]),g_RoundCount,0);
  new Float:center[3];
  GetClientAbsOrigin(client, center);
  center[2]+=25.0;
  VS_BeamRingPoint(center, 20.00, 22.00, g_bloodModel, g_bloodModel, 0, 3, duration, 100.00, 0.00, {235, 255, 20, 255}, 5, 0);
  if (multi==0.0 || period == 0.0 || duration == 0.0) {
    LogError("ERROR: light well spell corrupted.");
    DeleteEffectData(effid);
  }
  else if(effid!=0)
  {
    CreateTimer((period-0.05), EffectLightWellAction, effid, TIMER_REPEAT);
    CreateTimer(duration,EndLightWellAction,effid);
  }
}

public Action:EndLightWellAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][9]==g_RoundCount)
  {
    DeleteEffectData(effid);
  }
}

wcRunTalent_5_Paralyze(client,victim)
{
  decl String:line[255];
  wcGetTalentInfo(5,3,1,4,client,"chance",line);
  new chance = StringToInt(line);
  if(g_bShadowform[client])
  {
    wcGetTalentInfo(5,3,4,2,client, "increase", line);
    new Float:shadowbonus = StringToFloat(line);
    if(shadowbonus > 1.00)
    {
      chance = RoundToNearest(float(chance)*shadowbonus);
    }
  }
  new rand = GetRandomInt(1,100);
  if(chance>=rand)
  {
    wcGetTalentInfo(5,3,1,4,client,"duration",line);
    new Float:slowtime=StringToFloat(line);
    wcGetTalentInfo(5,3,1,4,client,"slowmulti",line);
    new Float:slowmulti=StringToFloat(line);
    if(g_bShadowform[client])
    {
      wcGetTalentInfo(5,3,4,2,client, "increase", line);
      new Float:shadowbonus = StringToFloat(line);
      if(shadowbonus > 1.00)
      {
        slowmulti*=shadowbonus;
        slowtime*=shadowbonus;
      }
    }
    new Float:oldspeed=GetClientSpeed(victim);
    new Float:newspeed=GetNormalSpeed(victim);
    newspeed*=slowmulti;
    if(newspeed<oldspeed)
    {
      SetClientSpeed(victim,newspeed);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,victim); // Write the client index of owner
      WritePackFloat(datapack2,newspeed);
      WritePackFloat(datapack2,oldspeed);
      CreateTimer(slowtime,RemoveFreezeEffect,datapack2,TIMER_HNDL_CLOSE);
    }
  }
}

wcRunTalent_5_ManaManipulation(client, mana)
{
  decl String:line[255];
  wcGetTalentInfo(5,3,2,4,client,"multi",line);
  new Float:multi=StringToFloat(line);
  if(g_bShadowform[client])
  {
    wcGetTalentInfo(5,3,4,2,client, "increase", line);
    new Float:shadowbonus = StringToFloat(line);
    if(shadowbonus > 1.00)
    {
      multi*=shadowbonus;
    }
  }
  mana = RoundToNearest(mana*multi)+GetMana(client);
  if(mana>GetMaxMana(client))
    mana = GetMaxMana(client);
  wcSetMana(client,mana);
}

wcRunTalent_5_VampiricTouch(client,damage)
{
  decl String:line[255];
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcGetTalentInfo(5,3,3,2,client,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(5,3,3,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  if(g_bShadowform[client])
  {
    wcGetTalentInfo(5,3,4,2,client, "increase", line);
    new Float:shadowbonus = StringToFloat(line);
    if(shadowbonus > 1.00)
    {
      multi*=shadowbonus;
      distance*=shadowbonus;
    }
  }
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==3)
    amount=GetNearClients("@ct",location,targets,distance);
  if(team==2)
    amount=GetNearClients("@t",location,targets,distance);
  new restore=RoundToNearest(damage*multi);
  for (new x=0;x<=(amount-1);x++)
  {
    if(restore+GetMana(targets[x])>GetMaxMana(targets[x]))
      wcSetMana(targets[x],GetMaxMana(targets[x]));
    else
      wcSetMana(targets[x],GetMana(targets[x])+restore);
  }
}

wcRunTalent_5_Silence(attacker,victim)
{
  decl String:line[255];
  wcGetTalentInfo(5,3,3,3,attacker,"chance",line);
  new chance = StringToInt(line);
  if(g_bShadowform[attacker])
  {
    wcGetTalentInfo(5,3,4,2,attacker, "increase", line);
    new Float:shadowbonus = StringToFloat(line);
    if(shadowbonus > 1.00)
    {
      chance = RoundToNearest(float(chance)*shadowbonus);
    }
  }
  new rand = GetRandomInt(1,100);
  if(chance>=rand)
  {
    wcGetTalentInfo(5,3,3,3,attacker,"duration",line);
    new Float:silence=StringToFloat(line);
    SetSilence(victim,silence);
  }
}

wcRunTalent_5_MindFlay(client)
{
  new target=GetAimTarget(client,25.0);
  decl String:line[255];
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      wcGetTalentInfo(5,3,4,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(5,3,4,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);

      SetCdSpell(client,cdtype,cd, 0, "Mind Flay");
      new manareq = GetTalentCost(client, 3,4,1);
      wcSetMana(client,GetMana(client)-manareq);
      wcGetTalentInfo(5,3,4,1,client,"multi",line);
      new Float:tickmulti=StringToFloat(line);
      new dmgef=GetSpellDmg(client,tickmulti);


      new String:atname[50],String:vicname[50];
      GetClientName(client,atname,sizeof(atname));
      GetClientName(target,vicname,sizeof(vicname));
      WCMessage(client,"mind flay cast",client,dmgef,vicname);
      WCMessage(target,"mind flay cast vic",client,dmgef,atname);

      wcGetTalentInfo(5,3,4,1,client,"tickdelay",line);
      new Float:tickdelay=StringToFloat(line);
      wcGetTalentInfo(5,3,4,1,client,"ticks",line);
      new ticks=StringToInt(line);
      ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
      tickdelay = tickdelay*(1.0-GetHasteEffect(client));

      new effid=CreateEffectData(target,client,ticks,dmgef,0,0,0,0,g_RoundCount,0);
      if(effid!=0)
      {
        CreateTimer(tickdelay, EffectMindFlayAction, effid, TIMER_REPEAT);
      }
      wcGetTalentInfo(5,3,4,1,client,"slowmulti",line);
      new Float:slowmulti=StringToFloat(line);
      wcGetTalentInfo(5,3,4,1,client,"duration",line);
      new Float:slowtime=StringToFloat(line);
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

      decl Float:soundlocation[3];
      GetClientAbsOrigin(target,soundlocation);
      EmitAmbientSound("wc/spells/mindflay.wav",soundlocation);

      new Float:location[3], Float:locationatt[3];
      GetClientAbsOrigin(client, locationatt);
      GetClientAbsOrigin(target, location);
      location[2]+=40;
      locationatt[2]+=40;
      VS_BeamPoints(location, locationatt, g_ef_scanner,g_ef_scanner, 1, 5, 1.00, 15.00, 15.00, 1, 1.00, {80, 0, 80, 255}, 9);
      VS_BeamPoints(location, locationatt, g_ef_scanner, g_ef_scanner, 1, 5, 1.00, 15.00, 15.00, 1, 0.00, {80, 0, 80, 255}, 9);
      VS_BeamPoints(locationatt, location, g_ef_glow1, g_ef_glow1, 1, 5, 1.00, 20.00, 10.00, 10, 0.00, {80, 0, 80, 255}, 8);
      VS_BeamPoints(locationatt, location, g_ef_lgtning, g_ef_glow1, 1, 5, 1.00, 20.00, 10.00, 0, 0.00, {80, 0, 80, 255}, 8);

    }
    else  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
}

/*public wcRunTalent_6_1(client)                                                  //Elemental Fury*/
/*{*/
  /*decl String:line[255];*/
  /*wcGetTalentInfo(6,1,3,1,client,"cooldowntype",line);*/
  /*new cdtype=StringToInt(line);*/
  /*wcGetTalentInfo(6,1,3,1,client,"cooldown",line);*/
  /*new Float:cd=StringToFloat(line);*/
  /*wcGetTalentInfo(6,1,3,1,client,"duration",line);*/
  /*new Float:duration=StringToFloat(line);*/
  /*SetCdSpell(client,cdtype,cd, 0, "Elemental Fury");*/
  /*WCMessage(client,"Elemental Fury",client,duration);*/
  /*g_iElementalFury[client]=1;*/
  /*new Handle:datapack=CreateDataPack();*/
  /*WritePackCell(datapack,client);*/
  /*CreateTimer(duration,EndElementalFury,datapack,TIMER_HNDL_CLOSE);*/
/*}*/

/*public wcRunTalent_6_1_2(client,mana)                                           //Elemental Fury mana reduction part
{
  decl String:line[255];
  wcGetTalentInfo(6,1,3,1,client,"multi",line);
  mana -= RoundToNearest(mana*StringToFloat(line));
  return mana;
}

public Action:EndElementalFury(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  g_iElementalFury[client]=0;
  return;
}*/

/*public wcRunTalent_6_2(client)                                                  //Dual Wielding
{
  decl String:line[255];
  wcGetTalentInfo(6,2,2,3,client,"multi",line);
  new Float:penetration = GetIgnoreArmor(client)*StringToFloat(line);
  SetBonusIgnoreArmor(client,GetBonusIgnoreArmor(client)+penetration);
  wcRecalculateStats(client);
}
*/

new Float:g_fShamanisticRage[65];
new g_iShamanisticRageProcs[65];
stock SetShamanisticRage(client, Float:duration, procs)  {
  g_fShamanisticRage[client] = GetTickedTime() + duration;
  g_iShamanisticRageProcs[client] = procs;
}
stock bool:SpendShamanisticProc(client) {
  return (g_iShamanisticRageProcs[client]-- > 0);
}

stock bool:HasShamanisticRage(client)  {
  return (GetTickedTime() < g_fShamanisticRage[client]);
}

public wcRunTalent_6_3(client)                                                  //Shamanistic Rage
{
  decl String:line[255];
  wcGetTalentInfo(6,2,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(6,2,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetTalentInfo(6,2,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  wcGetTalentInfo(6,2,3,1,client,"procs",line);
  new procs=StringToInt(line);

  new manareq = GetTalentCost(client, 2, 3, 1);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Shamanistic Rage");
  WCMessage(client,"Shamanistic Rage",client,duration);

  SetEffectVictim(client,37);
  SetEffectSpellVic(client,37);
  SetEffectAttacker(client,38);
  new Handle:datapack=CreateDataPack();
  WritePackCell(datapack,client);
  WritePackCell(datapack,37);
  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,client);
  WritePackCell(datapack3,37);
  new Handle:datapack2=CreateDataPack();
  WritePackCell(datapack2,client);
  WritePackCell(datapack2,38);
  CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE); //fixed
  CreateTimer(duration,DestroyEffectVictim,datapack,TIMER_HNDL_CLOSE);
  CreateTimer(duration,DestroyEffectAttacker,datapack2,TIMER_HNDL_CLOSE);
  new Float:location[3];
  GetClientAbsOrigin(client, location);
  EmitAmbientSound("wc/spells/shamanisticrage.wav",location);
  SetShamanisticRage(client, duration, procs);
  if(HasGlyph(client, GLYPH_SHAMAN_RAGE) && HasShamanisticRage(client))
  {
    new Float:oldspeed=GetClientSpeed(client);
    new Float:newspeed=GetNormalSpeed(client);
    if(newspeed>oldspeed)
    {
      SetClientSpeed(client,newspeed);
    }
  }
  /*if(HasGlyph(client, GLYPH_SHAMAN_RAGE))
  {
    new Float:oldspeed=GetClientSpeed(client);
    new Float:newspeed=GetNormalSpeed(client);
    new Float:multi=1.20;
    newspeed*=multi;
    if(newspeed>oldspeed)
    {
      duration*=0.5;
      wcClientSparks[client]=true;
      SetClientSpeed(client,newspeed);
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,client); // Write the client index of owner
      CreateTimer(duration,RemoveSparksEffect,datapack4,TIMER_HNDL_CLOSE);
      new Handle:datapack5=CreateDataPack();
      WritePackCell(datapack5,client); // Write the client index of owner
      WritePackFloat(datapack5,newspeed);
      WritePackFloat(datapack5,multi);
      CreateTimer(duration,RemoveSpeedEffect2,datapack5,TIMER_HNDL_CLOSE);
    }
  }*/
}

public wcRunTalent_6_4(client) //Chain Heal
{
  new target=GetAimTarget(client,50.0, true);
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
  wcGetTalentInfo(6,3,2,2,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(6,3,2,2,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 3, 2, 2);
  wcGetTalentInfo(6,3,1,1,client,"multi",line);
  new Float:talentbonus=StringToFloat(line);
  manareq=RoundToNearest(manareq*(1-talentbonus));
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
    wcRunTalent_6_4(client);
    WCMessage(client,"Echo of the Elements activate",client);
    }
  else
    SetCdSpell(client,cdtype,cd, 0, "Chain Heal");
  new Float:location[3],Float:locationatt[3];
  GetClientAbsOrigin(target,location);
  GetClientAbsOrigin(client,locationatt);
  wcGetTalentInfo(6,3,2,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(6,3,2,2,client,"base",line);
  new base=StringToInt(line);
  wcGetTalentInfo(6,3,2,2,client,"range",line);
  new Float:range=StringToFloat(line);
  wcGetTalentInfo(6,3,2,2,client,"jump",line);
  new Float:jump=StringToFloat(line);
  decl String:name[255],String:name2[20];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  new heal=GetSpellDmg(client,multi)+base;
  EmitAmbientSound("wc/spells/heal.wav",location);
  heal=wcHeal(client,target,heal,0,true);


  new targets[3];
  new targetsHealed[3];
  new targetsAmount = GetNearestPlayers(GetClientTeam(target),location,range, targets, 3);

  if(target!=client)
  {
    WCMessage(target,"chain heal target",target,name,heal);
  }

  new String:names[255];
  decl String:temp[20];
  Format(names, sizeof(names), "%s (%i)", name2,heal);

  for(new i=1;i<targetsAmount;i++)
  {
    //chain heal effect
    //Add bounce effect between targets
    BounceChainHeal(targets[i-1], targets[i]);

    new previousHeal=heal;
    if(i>1)
      previousHeal = targetsHealed[i];

    targetsHealed[i] = RoundToNearest(previousHeal*jump);
    wcHeal(client,targets[i], targetsHealed[i],0,true);
    WCMessage(targets[i],"chain heal target",targets[i],name,heal);

    GetClientName(targets[i], temp, sizeof(temp));
    Format(names, sizeof(names), "%s, %s (%i)", names, temp,targetsHealed[i]);
  }

  WCMessage(client,"chain heal caster",client, names);

  return 0;
}

BounceChainHeal(startClient, endClient) {
  decl Float:start[3], Float:end[3];
  GetClientAbsOrigin(startClient, start);
  GetClientAbsOrigin(endClient, end);
  start[2]+=40;
  end[2]+=40;
  VS_BeamPoints(start, end, g_plasmabeam, g_plasmabeam, 1, 5, 3.00, 4.00, 4.00, 1, 0.00, {255, 255, 255, 255}, 10);
  VS_BeamPoints(start, end, g_bluelight1, g_ef_muzzleflashX, 1, 5, 3.00, 2.00, 1.00, 1, 1.00, {10, 255, 0, 255}, 30);

}

public wcRunTalent_6_5(client)                                                  //Earth Shield
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
  wcGetTalentInfo(6,3,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(6,3,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 3, 3, 1);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Earth Shield");

  wcGetTalentInfo(6,3,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);

  SetEffectVictim(target,29);
  SetEffectSpellVic(target,29);
  g_iEarthShield[target][0]=client;

  wcGetTalentInfo(6,3,3,1,client,"count",line);
  new count = StringToInt(line);
  if(HasGlyph(client, GLYPH_SHAMAN_EARTHS))
    count-=2;
  g_iEarthShield[target][1]=count;
  new Handle:datapack2=CreateDataPack();
  WritePackCell(datapack2,target); // Write the client index of owner
  WritePackCell(datapack2,29);
  CreateTimer(duration,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,target); // Write the client index of owner
  WritePackCell(datapack3,29);
  CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);
  decl String:name[50],String:name2[50];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  WCMessage(client,"earth shield activated caster",client,duration,count,name2);
  if(client!=target)
  {
    WCMessage(target,"earth shield activated vic",target,duration,count,name);
  }
  return 1;
}

/*public wcRunTalent_6_6(client)                                                  //Unrelenting Storm
{
  decl String:line[255];
  wcGetTalentInfo(6,1,1,3,client,"mana",line);
  new talentbonus=StringToInt(line);
  SetBonusManaReg(client, GetBonusManaReg(client)+talentbonus);
  wcRecalculateStats(client);
}*/

public wcRunTalent_6_7(client)                                                  //Thundering Strikes
{
  decl String:line[255];
  wcGetTalentInfo(6,2,1,1,client,"increment",line);
  new Float:crit = StringToFloat(line);
  SetCriticalStrikeBonus(client, GetCriticalStrikeBonus(client)+crit);
  wcRecalculateStats(client);
}

public wcRunTalent_6_8(client) //Weapon Mastery
{
  decl String:line[255];
  wcGetTalentInfo(6,2,1,3,client,"base",line);
  new base = StringToInt(line);
  wcGetTalentInfo(6,2,1,3,client,"multi",line);
  SetBonusDamage(client, GetDamage(client)*StringToFloat(line)+GetBonusDamage(client)+float(base));
  wcRecalculateStats(client);
}

public wcRunTalent_6_9(client)                                                  //Shaman Flurry
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetTalentInfo(6,2,2,1,client,"chance",line);
  new chance=StringToInt(line);
  if(chance>=random)
  {
    wcGetTalentInfo(6,2,2,1,client,"stacks",line);
    new stacks = StringToInt(line);
    wcGetTalentInfo(6,2,2,1,client,"duration",line);
    new Float:duration = StringToFloat(line);
    if (g_iFlurry[client]<stacks)
    {
      g_iFlurry[client]++;
      new Handle:datapack=CreateDataPack();
      WritePackCell(datapack,client);
      CreateTimer(duration,EndSFlurry,datapack,TIMER_HNDL_CLOSE);
    }
    if(g_iFlurry[client]==1)
    {
      SetEffectAttacker(client,33);
      SetEffectSpellAtt(client,33);
    }
  }
}

public Action:EndSFlurry(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  if(g_iFlurry[client]==1)
  {
    RemoveEffectSpellAtt(client,33);
    RemoveEffectAttacker(client,33);
  }
  g_iFlurry[client]--;
}

public wcRunTalent_6_10(client)                                                 //Natures Combatant
{
  decl String:line[255];
  wcGetTalentInfo(6,2,2,2,client,"chance",line);
  new chance = StringToInt(line);
  new random = GetRandomInt(1,100);
  if (chance >= random)
  {
    new weapon = GetEntDataEnt2(client, ACTIVEWEAPON);
    new ammo = GetEntData(weapon, CLIPOFFSET, 4);
    if(ammo>6)
    {
      decl String:Sweapon[50];
      GetEdictClassname(weapon, Sweapon, sizeof(Sweapon));
      //weapon = weapon_xm1014
      //weapon = weapon_m3
      if((!StrEqual("weapon_xm1014",Sweapon))&&(!StrEqual("weapon_m3",Sweapon))&&(!StrEqual("weapon_c4",Sweapon))&&(!StrEqual("weapon_sawedoff",Sweapon))&&(!StrEqual("weapon_nova",Sweapon))&&(!StrEqual("weapon_mag7",Sweapon))&&(!StrEqual("weapon_hegrenade",Sweapon)))
      {
        wcGetTalentInfo(6,2,2,2,client,"restore",line);
        ammo += StringToInt(line);
        SetEntData(weapon, CLIPOFFSET, ammo, 4, true);
      }
    }
  }
}


public wcRunTalent_6_11(client)                                                 //Restorative Casting
{
  decl String:line[255];
  wcGetTalentInfo(6,3,1,2,client,"base",line);
  new base = StringToInt(line);
  wcGetTalentInfo(6,3,1,2,client,"mana",line);
  new bonus = RoundToNearest(float(GetManaReg(client))*StringToFloat(line))+base;
  wcClientsBonusStats[client][MPREGEN]+=bonus;
  wcRecalculateStats(client);
}


public wcRunTalent_6_12(client)                                                 //Natures Protection
{
    decl String:line[255];
    wcGetTalentInfo(6,3,1,3,client,"multi",line);
    new Float:bonus = float(GetArmor(client))*StringToFloat(line);
    SetBonusArmor(client, float(GetBonusArmor(client)) + bonus);
    wcRecalculateStats(client);
}

public wcRunTalent_6_13(client, mana)                                           //Convection
{
  decl String:line[255];
  wcGetTalentInfo(6,1,1,1,client,"multi",line);
  mana -= RoundToNearest(mana*StringToFloat(line));
  return mana;
}

public wcRunTalent_6_14(client, mana) //Shamanistic Focus
{
  decl String:line[255];
  wcGetTalentInfo(6,3,1,1,client,"multi",line);
  mana -= RoundToNearest(mana*StringToFloat(line));
  return mana;
}

wcRunTalent_6_NaturesPower(client)
{
  decl String:line[255];
  wcGetTalentInfo(6,1,2,1,client,"multi",line);
  new Float:multi = StringToFloat(line);
  new total = RoundToNearest(GetBonusSpellDmg(client,1.0)+float(GetDamage(client))*multi);
  SetBonusSpellDmg(client,float(total));
  wcRecalculateStats(client);
}

wcRunTalent_6_MaelstromWeapon(client,victim,damage, String:source[])
{
  decl String:line[255];
  new spellid = wcGetSpellIDFromName(6, source);
  if(spellid == 3 || spellid == 4 || spellid == 5)
  {
    wcGetTalentInfo(6,1,3,2,client,"chance",line);
    new chance = StringToInt(line);
    new rand = GetRandomInt(1,100);
    if(chance >= rand)
    {
      new Float:location[3];
      GetClientAbsOrigin(victim,location);
      wcGetTalentInfo(6,1,3,2,client,"distance",line);
      new Float:distance=StringToFloat(line);
      wcGetTalentInfo(6,1,3,2,client,"multi",line);
      new Float:multi=StringToFloat(line);
      new dmg = RoundToNearest(damage*multi);

      new targets[MAXPLAYERS];
      new team=GetClientTeam(client);
      new amount;
      if(team==2)
        amount=GetNearClients("@ct",location,targets,distance);
      if(team==3)
        amount=GetNearClients("@t",location,targets,distance);
      wcGetSpellInfo(6,spellid,"name",line);
      wcGetTalentInfo(6,1,3,2,client,"name",line);
      for (new x=0;x<=(amount-1);x++)
      {
        if(targets[x]!=victim)
          wcSpellDamage(targets[x],client,dmg, "", "",line);
      }
    }
  }
}
/*
public wcRunTalent_6_15(client) //Riptide
{
  new target=GetAimTarget(client,50.0, true);
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
  wcGetTalentInfo(6,3,4,2,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(6,3,4,2,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 3, 4, 2);
  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client, cdtype, cd, 0, "Riptide");

  wcGetTalentInfo(6,3,4,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(6,3,4,2,client,"base",line);
  new heal=StringToInt(line)+GetSpellDmg(client,multi);

  wcGetTalentInfo(6,3,4,2,client,"split",line);
  new Float:split=StringToFloat(line);
  wcGetTalentInfo(6,3,4,2,client,"ticks",line);
  new ticks = StringToInt(line);
  new hot = RoundToNearest(float(heal)*split) % ticks;
  heal = heal-(ticks*hot);

  wcGetTalentInfo(6,3,4,2,client,"tickdur",line);
  new Float:tickdur=StringToFloat(line);
  Renew(client,target,hot,ticks,1,tickdur);
  heal=wcHeal(client,target,heal,0,true);

  if(target!=client)
  {
    decl String:name[255],String:name2[255];
    GetClientName(client,name,sizeof(name));
    GetClientName(target,name2,sizeof(name2));
    WCMessage(target,"riptide target",target,name,heal,hot,float(ticks)*tickdur);
    WCMessage(client,"riptide caster",client,name2,heal,hot,float(ticks)*tickdur);
  }
  else
    WCMessage(client,"riptide self",client,heal,hot,float(ticks)*tickdur);
  return 0;
}*/

wcRunTalent_6_Guardian(client,&damage)
{
  decl String:line[255];
  wcGetTalentInfo(6,1,3,3,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if(chance >= rand)
  {
    damage=0;
    /*decl String:buffer[30];*/
    /*Format(buffer,sizeof(buffer),"%T", "natures guardian", client);*/
    AddEMsgAff("natures guardian", true);
  }
}

wcRunTalent_6_AirShock(attacker,victim)
{
  new random=GetRandomInt(1,100);
  decl String:line[255];
  wcGetTalentInfo(6,1,4,2,attacker,"chance",line);
  new chance=StringToInt(line)+GetThunder(attacker,4);

  if(IsDoubleHand(attacker))
      chance=RoundToNearest(float(chance)/2.5);
  if(chance>=random)
  {
    if(CheckRequirements(attacker,6,1,2,2))
    {
      wcGetTalentInfo(6,1,2,2,attacker,"increment",line);
      new thunderbonus = StringToInt(line);
      SetThunder(attacker, 1, GetThunder(attacker,4)+thunderbonus);
      wcGetTalentInfo(6,1,2,2,attacker,"duration",line);
      new Float:duration=StringToFloat(line);
      new Handle:datapack=CreateDataPack();
      WritePackCell(datapack,attacker);
      WritePackCell(datapack,4);
      WritePackCell(datapack,thunderbonus);
      WritePackCell(datapack,g_RoundCount);
      CreateTimer(duration,EndThunder,datapack,TIMER_HNDL_CLOSE);
    }

    wcGetTalentInfo(6,1,4,2,attacker,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(6,1,4,2,attacker,"cooldown",line);
    new Float:cd=StringToFloat(line);
    if(IsDoubleHand(attacker))
      cd=cd/2.0;
    SetCdSpell(attacker,cdtype,cd, 0, "Air Shock");

    new manareq = GetTalentCost(attacker, 1,4,2);
    new newmana=GetMana(attacker)-manareq;
    if(newmana<0)
      return 0;
    wcSetMana(attacker,newmana);
    wcGetTalentInfo(6,1,4,2,attacker,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetTalentInfo(6,1,1,4,attacker,"increment",line);
    multi+=StringToFloat(line);
    new dmg=GetSpellDmg(attacker,multi);
    decl String:vicMsg[300], String:attMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","airshock cast vic", victim);
    Format(attMsg,sizeof(attMsg),"%T","airshock cast", attacker);
    wcGetTalentInfo(6,1,4,2,attacker,"name",line);
    wcSpellDamage(victim,attacker,dmg, attMsg, vicMsg,line);


    /*if(GetBoss(victim)<=0 || (! IsBoss()) )
    {
      new weapon = GetEntDataEnt2(victim, ACTIVEWEAPON);
      new ammo = GetEntData(weapon, CLIPOFFSET, 4);
      if(ammo>5)
      {
        decl String:Sweapon[50];
        GetEdictClassname(weapon, Sweapon, sizeof(Sweapon));
        //weapon = weapon_xm1014
        //weapon = weapon_m3
        if((!StrEqual("weapon_xm1014",Sweapon))&&(!StrEqual("weapon_m3",Sweapon))&&(!StrEqual("weapon_c4",Sweapon))&&(!StrEqual("weapon_sawedoff",Sweapon))&&(!StrEqual("weapon_nova",Sweapon))&&(!StrEqual("weapon_mag7",Sweapon))&&(!StrEqual("weapon_hegrenade",Sweapon)))
        {
          wcGetTalentInfo(6,1,4,1,attacker,"ammo",line);
          ammo -= StringToInt(line);
          SetEntData(weapon, CLIPOFFSET, ammo, 4, true);
        }
      }
    }*/

    decl Float:soundlocation[3];
    GetClientAbsOrigin(victim,soundlocation);
    EmitAmbientSound("wc/spells/airshock.wav",soundlocation);


  }
  return 1;
}
wcRunTalent_6_LightningNovaTotem(client)  {
  //if(CSGO)
    //return;
  decl String:line[255];
  wcGetTalentInfo(6,3,1,1,client,"multi",line);
  new Float:talentbonus=StringToFloat(line);
  wcGetTalentInfo(6,1,4,1,client,"manareq",line);
  new manareq = RoundToNearest(float(StringToInt(line))*(1.0-talentbonus));
  if(0>GetMana(client)-manareq)
  {
    WCMessage(client,"no mana",client);
    return;
  }
  wcSetMana(client,GetMana(client)-manareq);
  wcGetTalentInfo(6,1,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(6,1,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client,cdtype,cd, 0, "Lightning Nova Totem");

  //Getting values
  wcGetTalentInfo(6,1,4,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(6,1,4,1,client,"dmg",line);
  new dmg=StringToInt(line);
  wcGetTalentInfo(6,1,4,1,client,"ticks",line);
  new ticks=StringToInt(line);
  new hp=RoundToNearest(float(GetMaxHealth(client))*0.05);
  wcGetTalentInfo(6,1,4,1,client,"delay",line);
  new Float:delay=StringToFloat(line);
  wcGetTalentInfo(6,1,4,1,client,"aoe",line);
  new aoe=StringToInt(line);
  wcGetTalentInfo(6,1,1,3,client,"range",line);
  aoe+=StringToInt(line);
  dmg += GetSpellDmg(client, multi);
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  WCMessage(client,"lightning nova totem cast",client,delay*ticks);

  new team=GetClientTeam(client);
  team = (team-1) % 2 + 2;
  new effid=CreateEffectData(client,0,dmg,0,aoe,team,ticks,0,g_RoundCount,0);
  if(effid!=0)
  {
    new iTotem = -1;
    if(GetClientTeam(client)==2)
      iTotem = SpawnTotem(location, true, hp, 3);
    else
      iTotem = SpawnTotem(location, false, hp, 3);
    if((effectsdata[effid][4]=iTotem) > 0)
    {
      if(TotemToPlayer(client,effectsdata[effid][4]))
        CreateTimer(delay, EffectLightningNovaTotemAction0, effid);
    }
  }
}

wcRunTalent_6_FeralForm(client)
{
  decl String:line[255];
  wcGetTalentInfo(6,2,1,4,client,"increment",line);
  SetNormalBonusSpeed(client,GetNormalBonusSpeed(client)+StringToFloat(line));
  wcRecalculateStats(client);
}

Float:wcRunTalent_6_ImpRock(client)
{
  if(CheckRequirements(client,6,2,2,4))
  {
    decl String:line[255];
    wcGetTalentInfo(6,2,2,4,client,"increment",line);
    return StringToFloat(line)*2;
  }
  else
    return 0.0;
}

wcRunTalent_6_CounterStrike(client,attacker,&damage)
{
  decl String:line[255];
  wcGetTalentInfo(6,2,2,3,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if (chance >= rand)
  {
    new Float:loc1[3];
    new Float:loc2[3];
    GetClientAbsOrigin(client,loc1);
    GetClientAbsOrigin(attacker,loc2);
    wcGetTalentInfo(6,2,2,3,client,"distance",line);
    new Float:distance=StringToFloat(line);
    if(GetDistanceBetween(loc1,loc2)<=distance)
    {
      wcGetTalentInfo(6,2,2,3,client,"multi",line);
      new Float:multi = StringToFloat(line);
      dealExtraHit(client,attacker,RoundToNearest(damage*multi),"Counter Strike","Counter Strike");
    }
  }
}

wcRunTalent_6_StormStrike(client,victim)
{
  if(GetResistance(victim)>0 || !IsBoss())
  {
    decl String:line[255];
    wcGetTalentInfo(6,2,3,3,client,"decrease",line);
    new decrease = StringToInt(line);
    AddResistance(victim,float(-decrease));
    wcGetTalentInfo(6,2,3,3,client,"duration",line);
    new Float:duration = StringToFloat(line);
    new Handle:datapack4=CreateDataPack();
    WritePackCell(datapack4,victim);
    WritePackCell(datapack4,decrease);
    CreateTimer(duration,EndStormStrike,datapack4,TIMER_HNDL_CLOSE);
  }
}

public Action:EndStormStrike(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new victim=ReadPackCell(datapack);
  new effect=ReadPackCell(datapack);
  AddResistance(victim,float(effect));
}
wcRunTalent_6_LavaSurge(client,victim) //Lava Lash
{
  decl String:line[255];
  wcGetTalentInfo(6,2,4,1,client,"chance",line);
  new chance=StringToInt(line);
  new random = GetRandomInt(1,100);
  if(chance>=random)
  {
    wcGetTalentInfo(6,2,4,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetTalentInfo(6,2,4,1,client,"base",line);
    new dmg=RoundToNearest(float(GetAgi(client)+GetBonusAgi(client))*multi)+StringToInt(line);
    wcSpellDamage(victim,client,dmg, "", "",line);
    AddEMsgAff("lava surge", true);
    wcGetTalentInfo(6,2,3,2,client,"chance",line);
    new echochance = StringToInt(line);
    new echorandom =GetRandomInt(1,100);
    if(echochance>=echorandom) {
      g_iEcho[client]=1;
      wcRunTalent_6_LavaSurge(client,victim);
      WCMessage(client,"Echo of the Elements activate",client);
    }
    if(HasGlyph(client, GLYPH_SHAMAN_LAVALASH))
    {
      new shockrandom=GetRandomInt(1,100);
      if(20>=shockrandom) {
        wcRunSpell_6_3(3,6,victim,client);
      }
      shockrandom=GetRandomInt(1,100);
      if(20>=shockrandom) {
        wcRunSpell_6_4(4,6,victim,client);
      }
      shockrandom=GetRandomInt(1,100);
      if(20>=shockrandom) {
        wcRunSpell_6_5(5,6,victim,client);
      }
    }
    decl Float:soundlocation[3];
    GetClientAbsOrigin(client,soundlocation);
    EmitAmbientSound("wc/spells/lavasurge.wav",soundlocation);

  }
}

wcRunTalent_6_NaturesGrace(client) //Nature's Grace
{
  decl String:line[255];
  wcGetSpellInfo(6,6,"cooldowntype",line);
  new type = StringToInt(line);
  wcGetTalentInfo(6,3,1,4,client,"cooldown",line);
  new Float:talent=StringToFloat(line);
  new Float:cd = GetCDSpell(client,type)-GetTickedTime();
  if (cd<talent)
    cd=0.0;
  else
    cd-=talent;
  SetCdSpell(client,type,cd,6);
}

wcRunTalent_6_Watershield(client)
{
  decl String:line[255];
  wcGetTalentInfo(6,3,2,4,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetTalentInfo(6,3,2,4,client,"cooldowntype",line);
  new cdtype = StringToInt(line);
  SetCdSpell(client,cdtype,cd,0,"Watershield");
  wcGetTalentInfo(6,3,2,4,client,"multi",line);
  new mana = RoundToNearest(GetMaxMana(client)*StringToFloat(line))+GetMana(client);
  if(mana> GetMaxMana(client))
    mana = GetMaxMana(client);
  wcSetMana(client,mana);

}

wcRunTalent_6_NaturesBlessing(client,target)
{
  decl String:line[255];
  wcGetTalentInfo(6,3,2,4,client,"increment",line);
  new Float:bonus = StringToFloat(line) + GetNBlessing(target);
  wcGetTalentInfo(6,3,2,4,client,"max",line);
  new Float:max = StringToFloat(line);
  if (bonus > max)
    bonus = max;
  SetNBlessing(target,bonus);
  SetEffectVictim(target,45);
  SetEffectSpellVic(target,45);
}

wcRunTalent_6_Earthgrip(client,attacker)
{
  decl String:line[255];
  wcGetTalentInfo(6,3,3,3,client,"slowmulti",line);
  new Float:slowmulti=StringToFloat(line);
  wcGetTalentInfo(6,3,3,3,client,"duration",line);
  new Float:duration=StringToFloat(line);
  new Float:oldspeed=GetClientSpeed(attacker);
  new Float:newspeed=GetNormalSpeed(attacker);
  newspeed*=slowmulti;
  if(newspeed<oldspeed)
  {
    SetClientSpeed(attacker,newspeed);
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,attacker);
    WritePackFloat(datapack2,newspeed);
    WritePackFloat(datapack2,slowmulti);
    CreateTimer(duration,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
  }
  SetSilence(attacker,duration);
  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(attacker,name2,sizeof(name2));
  WCMessage(client,"earth grip client",client,name2,RoundToNearest(duration));
  WCMessage(attacker,"earth grip attacer",attacker,name,RoundToNearest(duration));
}

wcRunTalent_6_NaturesAvatar(client)
{
  decl String:line[255];
  wcGetTalentInfo(6,3,4,1,client,"chance",line);
  new chance = StringToInt(line);
  new rand = GetRandomInt(1,100);
  if(chance >= rand)
  {
    wcGetTalentInfo(6,3,4,1,client,"mana",line);
    new manareq = StringToInt(line);
    if(GetMana(client)-manareq >= 0)
    {
      wcSetMana(client,GetMana(client)-manareq);
      wcGetTalentInfo(6,3,4,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      wcGetTalentInfo(6,3,4,1,client,"cooldowntype",line);
      new cdtype = StringToInt(line);
      SetCdSpell(client,cdtype,cd,0,"Natures Avatar");
      new Float:location[3];
      GetClientAbsOrigin(client,location);
      wcGetTalentInfo(6,3,4,1,client,"distance",line);
      new Float:distance=StringToFloat(line);
      wcGetTalentInfo(6,3,4,1,client,"multi",line);
      new Float:multi=StringToFloat(line);
      new targets[MAXPLAYERS];
      new team=GetClientTeam(client);
      new amount;
      if(team==3)
        amount=GetNearClients("@ct",location,targets,distance);
      if(team==2)
        amount=GetNearClients("@t",location,targets,distance);
      EmitAmbientSound("wc/spells/heal.wav",location);
      new heal=GetSpellDmg(client,multi);
      wcGetTalentInfo(6,3,4,1,client,"base",line);
      new base=StringToInt(line);
      heal+=base;
      for (new x=0;x<=(amount-1);x++)
      {
        wcHeal(client,targets[x],heal,0,true);
      }
      location[2]+=30.0;
      new color[4] = { 20, 255, 17, 240 };
      BeamRingEffect("@all",location,150.0,90.0,g_purplelaser1,0,10,3.0,80.0,color,0.0,3);
      WCMessage(client,"natures avatar client",client,heal);
    }
  }
}

public Action:EndThunder(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  new shock=ReadPackCell(datapack);
  new bonus=ReadPackCell(datapack);
  new round=ReadPackCell(datapack);
  if(round==g_RoundCount && GetThunder(client,shock) > 0)
    SetThunder(client,shock,GetThunder(client,shock)-bonus);
}

public wcRunTalent_7_1(client,victim)                                           //Nightfall
{
  decl String:line[255];
  wcGetTalentInfo(7,1,3,1,client,"chance",line);
  new chance = StringToInt(line);
  new random = GetRandomInt(1,100);
  if(chance>=random)
  {
    wcRunSpell_7_1(1,7,victim,client,true);
  }
}

/*public wcRunTalent_7_2(client)                                                  //Deaths Embrace
{
  decl String:line[255];
  wcGetTalentInfo(7,2,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(7,2,3,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  new manareq = GetTalentCost(client, 2, 3, 1);
  wcGetTalentInfo(7,2,3,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  wcGetTalentInfo(7,2,3,1,client,"multi",line);
  new Float:multi=StringToFloat(line);

  wcSetMana(client,GetMana(client)-manareq);
  SetCdSpell(client,cdtype,cd, 0, "Deaths Embrace");
  WCMessage(client,"deaths embrace",client,multi,duration);

  SetEffectAttacker(client,30);

  new Handle:datapack4=CreateDataPack();
  WritePackCell(datapack4,client);
  CreateTimer(duration,EndDeathsEmbrace,datapack4,TIMER_HNDL_CLOSE);
}*/

/*public Action:EndDeathsEmbrace(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  RemoveEffectAttacker(client,30);
}*/

stock TE_SetupDynamicLight(const Float:vecOrigin[3], r,g,b,iExponent,Float:fRadius,Float:fTime,Float:fDecay)
{
    TE_Start("Dynamic Light");
    TE_WriteVector("m_vecOrigin",vecOrigin);
    TE_WriteNum("r",r);
    TE_WriteNum("g",g);
    TE_WriteNum("b",b);
    TE_WriteNum("exponent",iExponent);
    TE_WriteFloat("m_fRadius",fRadius);
    TE_WriteFloat("m_fTime",fTime);
    TE_WriteFloat("m_fDecay",fDecay);
}
stock TE_SetupBubbles(const Float:vecOrigin[3], const Float:vecFinish[3],modelIndex,const Float:heightF,count,const Float:speedF)
{
	TE_Start("Bubbles");
	TE_WriteVector("m_vecMins", vecOrigin);
	TE_WriteVector("m_vecMaxs", vecFinish);
	TE_WriteFloat("m_fHeight", heightF);
	TE_WriteNum("m_nModelIndex", modelIndex);
	TE_WriteNum("m_nCount", count);
	TE_WriteFloat("m_fSpeed", speedF);
}

public wcRunTalent_7_3(client)                                                  //Chaos Bolt
{
  new target=GetAimTarget(client,25.0);
  if(target>0)
  {
  if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
  {
    decl String:line[255];
    new Float:reduction = GetSpellReduction(target,0.0);
    if(HasGlyph(client, GLYPH_WARLOCK_CHAOS))
    {

      new resred = RoundToNearest(reduction*0.35); //20% Reduction
      if(10 > resred)
        resred = 10;
      AddResistance(target,float(-resred)); //Add negative reduction
      wcRecalculateStats(target);
      new Float:duration = 8.0; //Set duration after which to return back to normal resistance
      new Handle:datapack4=CreateDataPack(); //Creates a datapack to send over to effects.inc
      WritePackCell(datapack4,target); //Sends the target so resistance can be changed on them later
      WritePackCell(datapack4,resred); //Sends the decreased amount so it can be returned after 8 seconds
      CreateTimer(duration,EndChaosBolt,datapack4,TIMER_HNDL_CLOSE);
    }
    wcGetTalentInfo(7,3,4,1,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(7,3,4,1,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    new manareq = GetTalentCost(client, 3, 4, 1);
    wcGetTalentInfo(7,3,4,1,client,"base",line);
    new basedmg=StringToInt(line);

    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client,cdtype,cd, 0, "Chaos Bolt");
    new Float:location[3],Float:locationatt[3];
    GetClientAbsOrigin(target,location);
    GetClientAbsOrigin(client,locationatt);
    wcGetTalentInfo(7,3,4,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    decl String:name[255],String:name2[255];
    GetClientName(client,name,sizeof(name));
    GetClientName(target,name2,sizeof(name2));

    new dmg=(GetSpellDmg(client,multi)+basedmg);
    if(reduction < 1.0)
      dmg = RoundToNearest(float(dmg) / reduction);

    EmitAmbientSound("wc/spells/chaosbolt.wav",location);

    decl String:vicMsg[300], String:attMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","chaosbolt target", target);
    Format(attMsg,sizeof(attMsg),"%T","chaosbolt caster", client);
    wcGetTalentInfo(7,3,4,1,client,"name",line);
    wcSpellDamage(target,client,dmg, attMsg, vicMsg,line);
    //Effect
    locationatt[2]+=45;
    TE_SetupBeamPoints(locationatt, location, ChaosSprite, ChaosSprite, 0, 8, 0.5, 10.0, 10.0, 10, 10.0, {255,255,255,255}, 50);
    TE_SendToAll();
    TE_SetupDynamicLight(location,255,2,2,500,80.0,0.5,0.5);
    TE_SendToAll();
    TE_SetupBubbles(location, location,ChaosSprite,900.0, 25, 900.0);
    TE_SendToAll();
    //location[2]+=45;
    //TE_SetupGlowSprite(location,ChaosSprite,1.0,0.9,255);
    //TE_SendToAll();
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
}

public wcRunTalent_7_4(client)                                                  //Drain soul
{
  decl String:line[255];
  wcGetTalentInfo(7,1,2,1,client,"multi",line);
  new Float:multi = StringToFloat(line);
  new mana = RoundToNearest(GetMaxMana(client)*multi);
  wcSetMana(client, mana + GetMana(client));
  WCMessage(client,"spirit tab",client, mana);
}

/*public wcRunTalent_7_5(client)                                                  //Old Blood Pact
{
  decl String:line[255];
  wcGetTalentInfo(7,2,1,1,client,"multi",line);
  new Float:multi=StringToFloat(line);
  SetBonusMaxHealth(client, RoundToNearest(GetMaxHealth(client)*multi+GetBonusMaxHealth(client)));
  RestoreHealthP(client,multi);
  wcRecalculateStats(client);
}*/
wcRunTalent_7_FelVitality(client,&heal)
{
  decl String:line[255];
  wcGetTalentInfo(7,2,1,1,client,"multi",line);
  heal+=RoundToNearest(heal*StringToFloat(line));
}

public wcRunTalent_7_6(client)                                                  //Demonic Resilience
{
  decl String:line[255];
  wcGetTalentInfo(7,2,1,3,client,"increment",line);
  new Float:bonus = StringToFloat(line);
  SetMinusCritChanceBonus(client, MinusCritChanceBonus(client)+bonus);
  wcRecalculateStats(client);
}

public wcRunTalent_7_7(client)                                                  //Blood Pact
{
  decl String:line[255];
  wcGetTalentInfo(7,2,2,2,client,"multi",line);
  new Float:bonus=StringToFloat(line);
  bonus *= float(GetSta(client)+GetBonusSta(client));
  bonus += GetBonusSpellDmg(client,1.0);
  SetBonusSpellDmg(client,bonus);
  wcRecalculateStats(client);
}

public wcRunTalent_7_8(client)                                                  //Devestation
{
  decl String:line[255];
  wcGetTalentInfo(7,3,1,2,client,"increment",line);
  new Float:bonus = StringToFloat(line);
  bonus += GetCriticalStrikeBonus(client);
  SetCriticalStrikeBonus(client, bonus);
  wcRecalculateStats(client);
}

public wcRunTalent_7_9(client)                                                  //Molten Skin
{
  SetEffectVictim(client,32);
  SetEffectSpellVic(client,32);
}

public wcRunTalent_7_10(attacker, totaldmg)                                       //Siphon Life
{
  decl String:line[255];
  wcGetTalentInfo(7,1,2,2,attacker,"multi",line);
  new heal = RoundToNearest(totaldmg*StringToFloat(line));
  heal=wcHeal(attacker,attacker,heal,0,false);
}

wcRunTalent_7_LurkingShadows(client,mana)
{
  decl String:line[255];
  wcGetTalentInfo(7,1,1,1,client,"multi",line);
  return RoundToNearest(mana*(1-StringToFloat(line)));
}

wcRunTalent_7_ShadowGrip(client,target)
{
  decl String:line[255];
  wcGetTalentInfo(7,1,3,3,client,"slowmulti",line);
  new Float:slowmulti=StringToFloat(line);
  wcGetTalentInfo(7,1,3,3,client,"duration",line);
  new Float:slowtime=StringToFloat(line);
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
}

wcRunTalent_7_CursedSoul(client) //Cursed Soul
{
  new target=GetAimTarget(client,25.0);
  decl String:line[255];
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      wcGetTalentInfo(7,1,4,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(7,1,4,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      SetCdSpell(client, cdtype, cd, 0, "Cursed Soul");
      new manareq = GetTalentCost(client, 1,4,1);
      wcSetMana(client,GetMana(client)-manareq);

      new String:atname[50],String:vicname[50];
      GetClientName(client,atname,sizeof(atname));
      GetClientName(target,vicname,sizeof(vicname));
      WCMessage(client,"cursed soul cast",client,vicname);
      WCMessage(target,"cursed soul cast vic",target,atname);

      wcGetTalentInfo(7,1,4,1,client,"tickdelay",line);
      new Float:tickdelay=StringToFloat(line);
      wcGetTalentInfo(7,1,4,1,client,"ticks",line);
      new ticks=StringToInt(line);
      ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
      tickdelay = tickdelay*(1.0-GetHasteEffect(client));
      wcGetTalentInfo(7,1,4,1,client,"distance",line);
      new aoe=StringToInt(line);
      wcGetTalentInfo(7,1,4,1,client,"base",line);
      new base=StringToInt(line);
      new team=GetClientTeam(target);
      new effid=CreateEffectData(target,client,ticks,base,aoe,team,0,0,g_RoundCount,0);
      
      new Float:loc[3];
      new Float:locatt[3];
      GetClientAbsOrigin(target,loc);
      GetClientAbsOrigin(client,locatt);
      loc[2]+=50;
      locatt[2]+=40;
      VS_BeamPoints(locatt, loc, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 1, 5, 3.00, 5.00, 1.00, 1, 0.00, {31, 31, 61, 255}, 20);
      VS_BeamRingPoint(loc, 19.00, 20.00, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 100, 300, 3.00, 30.00, 1.00, {31, 31, 61, 255}, 300, 0);
      
      
      
      if (tickdelay==0.0 || ticks == 0)
      {
        LogError("ERROR: cursed soul spell corrupted.");
        DeleteEffectData(effid);
      }
      else
        if(effid!=0)
        {
          CreateTimer(tickdelay, EffectCursedSoulDamage, effid, TIMER_REPEAT);
          new Float:location[3];
          GetClientAbsOrigin(client,location);
          EmitAmbientSound("wc/spells/cursedsoul.wav",location);
        }

      if(CheckRequirements(client,7,1,3,3))
        wcRunTalent_7_ShadowGrip(client,target);
    }
    else  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);
}

wcRunTalent_PainAbsorbtion(client)
{
  decl String:line[255];
  wcGetTalentInfo(7,2,2,4,client,"chance",line);
  new chance = StringToInt(line);
  new random = GetRandomInt(1,100);
  if (chance >= random) {
    wcGetTalentInfo(7,2,2,4,client,"mana",line);
    new mana = RoundToNearest(GetMaxMana(client)*StringToFloat(line)) + GetMana(client);
    if(mana > GetMaxMana(client))
      mana = GetMaxMana(client);
    wcSetMana(client,mana);
  }
}

/*wcRunTalent_Fearsome(client,victim)
{
  if(client!=victim)
  {
    decl String:line[255];
    wcGetTalentInfo(7,2,3,2,client,"chance",line);
    new chance = StringToInt(line);
    new random = GetRandomInt(1,100);
    if (chance >= random)
    {
      new race = GetClass(client);
      if(GetClass(victim)==8 && wcGetTalentLevel(victim,2,2,1)>0)
        return 0;
      if(UnableToAttack[victim])
        return 0;
      wcGetTalentInfo(7,2,3,2,client,"ticks",line);
      new ticks=StringToInt(line);

      new color2[4]={53,55,149,255};
      FadeEffect(victim,2,300,50,color2);

      wcGetSpellInfo(7,4,"power",line);
      new power=StringToInt(line);
      wcGetSpellInfo(7,4,"tickdelay",line);
      new Float:tickdelay=StringToFloat(line);

      //breakdamage
      wcGetSpellInfo(race,4,"breakdmg",line); //Keep 4 same as fear spellid.
      new breakdmg=StringToInt(line);

      new effid=CreateEffectData(victim,client,ticks,power,breakdmg,0,ticks,race,g_RoundCount);
      new Float:dur=tickdelay*float(ticks);
      if (tickdelay==0.0 || ticks == 0)
      {
        LogError("ERROR: fearsome presence spell corrupted.");
        DeleteEffectData(effid);
      }
      else if(effid!=0)
      {
        SetFear(victim,effid);
        SetSilence(victim,dur,true);
        CreateTimer(tickdelay, EffectFearAction, effid, TIMER_REPEAT);
      }
    }
  }
  return 1;
}*/

/*wcRunTalent_7_ReceptiveBlood(client)
{
  decl String:line[255];
  wcGetTalentInfo(7,2,3,3,client,"multi",line);
  SetHealingBonus(client,GetHealingBonus(client)+StringToFloat(line));
}*/

wcRunTalent_7_SpiritLink(client,damage)
{
  decl String:line[255];
  wcGetTalentInfo(7,2,4,1,client,"multi",line);
  new heal = RoundToCeil(damage*StringToFloat(line));
  wcHeal(client,client,heal,0,false);
}

wcRunTalent_7_Shock(client,target,damage)
{
  decl String:line[255];
  wcGetTalentInfo(7,3,2,4,client,"chancemulti",line);
  new chance = RoundToNearest(damage*StringToFloat(line));
  new rand = GetRandomInt(1,100);
  if (chance >= rand)
  {
    wcGetTalentInfo(7,3,2,4,client,"slowmulti",line);
    new Float:slowmulti=StringToFloat(line);
    wcGetTalentInfo(7,3,2,4,client,"duration",line);
    new Float:slowtime=StringToFloat(line);
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
  }
}

wcRunTalent_7_ShadowTouch(client,victim)
{
  new Float:random=GetRandomFloat(0.0,1.0);
  decl String:line[255];
  wcGetTalentInfo(7,3,3,2,client,"chance",line);
  new Float:chance=StringToFloat(line);
  if(chance>=random)
  {
    wcGetTalentInfo(7,3,3,2,client,"multi",line);
    new Float:multi=StringToFloat(line);
    new dmg=GetSpellDmg(client,multi);

    decl String:attMsg[300];
    Format(attMsg,sizeof(attMsg),"%T","shadow touch", client);
    wcGetTalentInfo(7,3,3,2,client,"name",line);
    wcSpellDamage(victim,client,dmg, attMsg, "",line);
    new Float:location[3];
    GetClientAbsOrigin(client,location);
    EmitAmbientSound("wc/spells/shadowtouch.wav",location);
  }
}

wcRunTalent_7_Flameburst(client,victim,&damage)
{
  decl String:line[255];
  wcGetTalentInfo(7,3,3,3,client,"distance",line);
  new aoe = StringToInt(line);
  wcGetTalentInfo(7,3,3,3,client,"multi",line);
  new dmg = RoundToNearest(damage*StringToFloat(line));
  if (dmg > 0 && aoe > 0)
  {
    new team = GetClientTeam(victim);
    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    new targets[MAXPLAYERS];
    new amount;
    if(team==2)
      amount=GetNearClients("@t",location,targets,float(aoe));
    else if(team==3)
      amount=GetNearClients("@ct",location,targets,float(aoe));
    else
      return;
    decl String:name[40];
    GetClientName(victim,name,sizeof(name));
    for (new x=0;x<=(amount-1);x++)
    {
      if(targets[x]!=victim)
      {
        WCMessage(targets[x],"aoe damaged",targets[x],name,dmg);
        wcSpellDamage(targets[x],client,dmg, "", "");
      }
    }
  }
}

/*wcRunTalent_7_ShadowWell(client)
{
  new target=GetAimTarget(client,25.0);
  decl String:line[255];
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {

      //CD and mana
      wcGetTalentInfo(7,3,3,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(7,3,3,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      SetCdSpell(client, cdtype, cd, 0, "Shadow Well");
      new manareq = GetTalentCost(client, 3,4,1);
      wcSetMana(client,GetMana(client)-manareq);

      wcGetTalentInfo(7,3,3,1,client,"duration",line);
      new Float:duration=StringToFloat(line);


      wcGetTalentInfo(7,3,3,1,client,"tickdelay",line);
      new Float:period=StringToFloat(line);

      wcGetTalentInfo(7,3,3,1,client,"distance",line);
      new aoe=StringToInt(line);

      WCMessage(client,"shadow well cast",client);
      new Float:location[3];
      GetClientAbsOrigin(target,location);

      //Setting up effect
      new team=GetClientTeam(client);
      new effid=CreateEffectData(client,team,aoe,RoundToNearest(location[0]),RoundToNearest(location[1]),RoundToNearest(location[2]),0,0,g_RoundCount);
      new Float:center[3];
      center[2]+=20.0;
      VS_BeamRingPoint(center, 20.00, 22.00, g_bloodsprayModel, g_bloodsprayModel, 0, 3, 5.00, 100.00, 0.00, {0, 0, 40, 255}, 5, 0);
      if (period == 0.0 || duration == 0.0) {
        LogError("ERROR: shadow well spell corrupted.");
        DeleteEffectData(effid);
      }
      else if(effid!=0)
      {
        CreateTimer(period, EffectShadowWellAction, effid, TIMER_REPEAT);
        CreateTimer(duration,EndShadowWellAction,effid);
        EmitAmbientSound("wc/spells/shadowzone.wav",location);
      }
      else  if(IsChat(client,Chat:Chat_Target))
      WCMessage(client,"wrong target",client);
    }
  }
  else  if(IsChat(client,Chat:Chat_Target))
    WCMessage(client,"no target",client);

}

public Action:EndShadowWellAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][9]==g_RoundCount)
  {
    DeleteEffectData(effid);
  }
}*/

public wcRunTalent_7_Immolate(client)
{
  new target=GetAimTarget(client,25.0);
  decl String:line[255];
  if(target>0)
  {
    if((GetClientTeam(client)!=GetClientTeam(target))&&IsPlayerAlive(target))
    {
      wcGetTalentInfo(7,3,3,1,client,"cooldowntype",line);
      new cdtype=StringToInt(line);
      wcGetTalentInfo(7,3,3,1,client,"cooldown",line);
      new Float:cd=StringToFloat(line);
      SetCdSpell(client, cdtype, cd, 0, "Immolate");
      wcGetTalentInfo(7,3,3,1,client,"mana",line);
      new manareq = StringToInt(line);
      wcGetTalentInfo(7,3,3,1,client,"baseef",line);
      new baseef=StringToInt(line);
      wcGetTalentInfo(7,3,1,3,client,"multi",line);
      new Float:talentbonusbase=StringToFloat(line);
      wcGetTalentInfo(7,3,3,1,client,"basedmg",line);
      new basedmg=RoundToNearest(StringToInt(line)*(1+talentbonusbase));
      wcSetMana(client,GetMana(client)-manareq);
      wcGetTalentInfo(7,3,3,1,client,"multi",line);
      new Float:multi=StringToFloat(line);
      new dmg=GetSpellDmg(client,multi)+basedmg;
      wcGetTalentInfo(7,1,2,4,client,"multi",line);
      new Float:talentbonusdamage=StringToFloat(line);
      wcGetTalentInfo(7,3,3,1,client,"multief",line);
      new Float:multief=StringToFloat(line);
      new dmgef=RoundToNearest((GetSpellDmg(client,multief)+baseef)*(1+talentbonusdamage));
      decl Float:location[3];
      GetClientAbsOrigin(target,location);
      location[2]+=50;
      EmitAmbientSound("wc/spells/fireball.wav",location);
      wcGetTalentInfo(7,3,3,1,client,"ticks",line);
      new ticks = StringToInt(line);

      decl String:name[255],String:name2[255];
      GetClientName(target,name2,sizeof(name2));
      GetClientName(client,name,sizeof(name));
      WCMessage(client,"immolate cast",client, dmg, dmgef, name2);
      WCMessage(target,"immolate cast vic",target, name, dmg, dmgef);
      new aoe = 1;
      new talentbonusrange = 1;
      if(HasGlyph(client, GLYPH_WARLOCK_IMMOLATE))  {
      wcGetTalentInfo(7,1,2,3,client,"increment",line);
      talentbonusrange=StringToInt(line);
      wcGetTalentInfo(7,3,3,1,client,"aoe",line);
      aoe=StringToInt(line)+talentbonusrange;
      }
      new team=GetClientTeam(target);
      wcSpellAoeDamage(target,client,dmg,aoe,team,line);

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
      wcGetTalentInfo(7,3,3,1,client,"tickdelay",line);
      new Float:tickdelay=StringToFloat(line);
      wcGetTalentInfo(7,1,2,4,client,"ticks",line);
      new talentbonusticks=StringToInt(line);
      ticks+=talentbonusticks;
      ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
      tickdelay = tickdelay*(1.0-GetHasteEffect(client));
      wcGetTalentInfo(7,1,2,3,client,"increment",line);
      talentbonusrange=StringToInt(line);
      wcGetTalentInfo(7,3,3,1,client,"aoe",line);
      aoe=StringToInt(line)+talentbonusrange;
      new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,IMMOLATE_UNIQUE,2,g_RoundCount,GetClientDeaths(target));
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
}

stock wcRunTalent_GiveItem(client, race, tree, tier, talent)  {

  if(!CheckRequirements(client,race,tree,tier, talent))  return;
  decl String:line[255];
  wcGetTalentInfo(race,tree,tier,talent,client,"itemid",line);
  new itemid = StringToInt(line);
  wcGetTalentInfo(race,tree,tier,talent,client,"duration",line);
  new Float:duration = StringToFloat(line);
  new item[UniqueItem];
  GetItemProperties(item, itemid);

  AddTemporaryItem(client, item, duration, true);

}

//heal totem
wcRunTalent_6_HealingTideTotem(client)  {
  //if(CSGO)
    //return;
  decl String:line[255];
  wcGetTalentInfo(6,3,1,1,client,"multi",line);
  new Float:talentbonus=StringToFloat(line);
  wcGetTalentInfo(6,3,4,2,client,"manareq",line);
  new manareq = RoundToNearest(float(StringToInt(line))*(1.0-talentbonus));
  if(0>GetMana(client)-manareq)
  {
    WCMessage(client,"no mana",client);
    return;
  }
  wcSetMana(client,GetMana(client)-manareq);

  wcGetTalentInfo(6,3,4,2,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(6,3,4,2,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client,cdtype,cd, 0, "Healing Tide Totem");

  //Getting values
  wcGetTalentInfo(6,3,4,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(6,3,4,2,client,"heal",line);
  new heal=StringToInt(line);
  wcGetTalentInfo(6,3,4,2,client,"ticks",line);
  new ticks=StringToInt(line);
  ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
  //wcGetTalentInfo(6,3,4,2,client,"tickdur",line);
  //new Float:tickdur=StringToFloat(line);
  wcGetTalentInfo(6,3,4,2,client,"radius",line);
  new radius=StringToInt(line);
  wcGetTalentInfo(6,1,1,3,client,"range",line);
  radius+=StringToInt(line);
  heal += GetSpellDmg(client, multi);
  wcGetTalentInfo(6,3,4,2,client,"add",line);
  new add=StringToInt(line);
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  new Float:tickdur = 0.9090909090909090909091;
  tickdur = tickdur*(1.0-GetHasteEffect(client));
  //WCMessage(client,"heal tide totem cast",client,10.0);
  WCMessage(client,"heal tide totem cast",client,RoundToNearest(tickdur*float(ticks)));

  new team=GetClientTeam(client);
  new effid=CreateEffectData(client,add,heal,0,radius,team,ticks,0,g_RoundCount,0);
  if(effid!=0)
  {
    new hp=RoundToNearest(float(GetMaxHealth(client))*0.05);
    new iTotem = -1;
    if(team==2)
      iTotem = SpawnTotem(location, true, hp, 4);
    else
      iTotem = SpawnTotem(location, false, hp, 4);
    if((effectsdata[effid][4]=iTotem) > 0)
    {
      if(TotemToPlayer(client,effectsdata[effid][4]))
        CreateTimer(0.1, EffectCreateHealingTideTotem, effid);
    }
  }
  return;
}

public Action:EffectCreateHealingTideTotem(Handle:timer, any:effid)  {
  CreateTimer(0.9090909090909090909091, EffectHealingTideTotem, effid, TIMER_REPEAT);
}

public Action:EffectHealingTideTotem(Handle:timer, any:effid)
{
  new client = effectsdata[effid][1];
  new totem = effectsdata[effid][4];
  new Float:radius = float(effectsdata[effid][5]);
  new heal = effectsdata[effid][3];
  new team = effectsdata[effid][6];
  //new round = effectsdata[effid][9];
  if (!IsClientInGame(client))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if (effectsdata[effid][7]<=0)//7=ticks
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  effectsdata[effid][7]--; //7=ticks
  if (effectsdata[effid][9]!=g_RoundCount) //9=round totem created in
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if(!IsValidEntity(totem))   {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    return Plugin_Stop;
  }

  new Float:location[3];
  GetEntPropVector(totem, Prop_Send, "m_vecOrigin", location);

  new targets[MAXPLAYERS];
  new amount=0;
  if(team==2)
    amount=GetNearClients("@t",location,targets,radius);
  else if(team==3)
    amount=GetNearClients("@ct",location,targets,radius);
  else
    return Plugin_Continue;

  if(amount==0)
    return Plugin_Continue;
  else
    heal = effectsdata[effid][2] + RoundToCeil(float(heal / amount));


  decl String:name2[255];
  new String:stargets[255];
  for (new x=0;x<amount;x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
    new healed=wcHeal(client,targets[x],heal,0,true);
    if(targets[x]!=client)
      WCMessage(targets[x],"heal tide totem vict",targets[x],healed);
  }
  if(HasGlyph(client, GLYPH_SHAMAN_HTD))
  {
    new Float:distance = 400.0;
    decl Float:locationb[3];
    GetClientAbsOrigin(client,locationb);
    new targeto[MAXPLAYERS];
    new zad;
    if(team==2)
      zad=GetNearClients("@ct",locationb,targeto,distance);
    if(team==3)
      zad=GetNearClients("@t",locationb,targeto,distance);
    for (new x=0;x<=(zad-1);x++)
    {
      decl Float:loc2[3];
      GetClientAbsOrigin(targeto[x],loc2);
      new Float:zPower = 130.0;
      new Float:power = 90.0;
      power = power * (400.0 / (150.0 + GetDistanceBetween(locationb,loc2)));
      if(GetBoss(targeto[x])<=0 || (!IsBoss()) )
        PushPlayerAwayPlayer(targeto[x], client, power, zPower);
    }
  }
  EmitAmbientSound("wc/spells/tidetotem.wav",location);
  return Plugin_Continue;


}
wcRunTalent_8_BloodParasites(client,victim)
{
  decl String:line[255];
  wcGetTalentInfo(8,1,4,2,client,"chance",line); //Proc chance
  new chance=StringToInt(line);
  new random=GetRandomInt(1,100);
  if(chance>=random)
  {
    wcGetTalentInfo(8,1,4,2,client,"multi",line); //AP * multi = Bonus damage the parasites do
    new Float:multi=StringToFloat(line);
    wcGetTalentInfo(8,1,4,2,client,"base",line); //Base damage the parasites do
    new base = StringToInt(line);
    wcGetTalentInfo(8,1,4,2,client,"ticks",line); //How many times the parasites attack
    new ticks=StringToInt(line);
    new Float:delay=0.0;
    wcGetTalentInfo(8,1,4,2,client,"delay",line); //How often the parasites attack
    delay+=StringToFloat(line);
    ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(client)));
    delay = delay*(1.0-GetHasteEffect(client));
    new heal = 0;
    new dmg = RoundToNearest(float(base) + RoundToNearest(float(GetDamage(client))*multi));
    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    new effid=CreateEffectData(victim,client,ticks,dmg,_:location[0],heal,_:location[1],_:location[2],g_RoundCount,0);
    if(effid!=0)
    {
      CreateTimer(delay, EffectParasites, effid, TIMER_REPEAT);
    }
  }
}
public Action:EffectParasites(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0) //empty effectdata
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2])) //victim/attacker
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1])) //victim
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0) //ticks
  {
    new Float:locationa[3];
    GetClientAbsOrigin(effectsdata[effid][2],locationa);
    new targeta[MAXPLAYERS];
    new ateam=GetClientTeam(effectsdata[effid][2]);
    new amounta;
    new distance = RoundToNearest(float(effectsdata[effid][5])*1.50);
    if(ateam==3)
      amounta=GetNearClients("@ct",locationa,targeta,float(distance));
    if(ateam==2)
      amounta=GetNearClients("@t",locationa,targeta,float(distance));
    decl String:name[255],String:name2[255];
    GetClientName(effectsdata[effid][2],name,sizeof(name));
    new String:stargets[255];
    for (new x=0;x<=(amounta-1);x++)
    {
      GetClientName(targeta[x],name2,sizeof(name2));
      if(x>0)
         Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
      else
         Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
      new healed=wcHeal(effectsdata[effid][2],targeta[x],effectsdata[effid][6],0,true);
      if(targeta[x]!=effectsdata[effid][2])
      {
        WCMessage(targeta[x],"parasite heal",targeta[x],name,healed);
        if(HasGlyph(effectsdata[effid][2], GLYPH_DK_PARASITES))
          wcHeal(effectsdata[effid][2],effectsdata[effid][2],effectsdata[effid][6],0,true);
      }
    }
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount) //dot from another round
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--; //expend one tick and continue
  decl Float:vec[3],Float:rightVec[3],Float:upVec[3];
  GetClientAbsOrigin(effectsdata[effid][1], vec); //victim
  vec[0] -= 30;
  vec[1] -= 30;
  GetVectorVectors(vec, rightVec, upVec);
  static const color[] = {255,40,40,200};
  if(!CSGO) {
    TE_SetupBloodSprite(vec, rightVec, color, 25, g_bloodsprayModel, g_bloodModel);
    TE_SendToAll();
  }
  decl String:line[255];
  new damage = effectsdata[effid][4]; //damage
  wcGetTalentInfo(8,1,4,2,effectsdata[effid][2],"distance",line); //Distance of the blood parasites' damage
  new aoe=StringToInt(line);
  new team=GetClientTeam(effectsdata[effid][1]);
  new Float:location[3];
  location[0] = Float:effectsdata[effid][5];
  location[1] = Float:effectsdata[effid][7];
  location[2] = Float:effectsdata[effid][8];
  wcSpellAoeDamageLocation(effectsdata[effid][2], effectsdata[effid][2], location, damage, aoe, team, line);
  VS_BeamRingPoint(location,50.00,150.00,g_ef_glow01,g_ef_glow01, 10, 10, 0.50, float(aoe), 0.00, {196, 3, 35, 255}, 10, 0);
  wcGetTalentInfo(8,1,4,2,effectsdata[effid][2],"heal",line); //Base heal the parasites do
  new baseheal = StringToInt(line);
  new targets[MAXPLAYERS];
  new steam=GetClientTeam(effectsdata[effid][2]);
  new amount;
  if(steam==2)
    amount=GetNearClients("@ct",location,targets,float(aoe));
  if(steam==3)
    amount=GetNearClients("@t",location,targets,float(aoe));
  effectsdata[effid][6]+= RoundToNearest(float(baseheal) + RoundToNearest(float(amount) * float(damage)));
  return Plugin_Continue;
}
wcRunTalent_8_Obliterate(client, victim, &totaldmg)
{
  decl String:line[255];
  new random=GetRandomInt(1,100);
  wcGetTalentInfo(8,2,4,2,client,"chance",line);
  new chance = StringToInt(line);
  if(IsDoubleHand(client))
    chance+=RoundToNearest(float(chance)*0.50);
  if(chance>=random)
  {
      new Float:oldspeed=GetClientSpeed(victim);
      new Float:normalspeed=GetNormalSpeed(victim);
      if(normalspeed>oldspeed)
      {
        wcGetTalentInfo(8,2,4,2,client,"mana",line);
        wcSetMana(client,GetMana(client)+StringToInt(line));
        if(GetMana(client)>130)
          wcSetMana(client,130);
        wcGetTalentInfo(8,2,4,2,client,"multi",line);
        new Float:multi=StringToFloat(line);
        if(HasGlyph(client, GLYPH_DK_OBLITERATE))
        {
          new Float:percent = oldspeed/normalspeed;
          new Float:stra = 0.6+(0.4 / percent);
          if(1.0>stra)
            stra=1.0;
          if(stra>1.5)
            stra=1.5;
          multi*=stra;
        }
        new dmg=RoundToNearest(float(totaldmg)*multi);
        totaldmg+=dmg;
        decl Float:location[3];
        GetClientAbsOrigin(victim,location);
        EmitAmbientSound("wc/spells/obliterate.wav",location);
        AddEMsgAff("obliterate", true);
        new Float:pos[3];
        GetClientAbsOrigin(victim,pos);
        VS_BeamRingPoint(pos, 90.00, 60.00, g_ef_glow1, g_ef_lgtning, 10, 10, 0.50, 50.00, 0.00, {0, 102, 255, 255}, 60, 0);
        pos[2]+=45.0;
        VS_BeamRingPoint(pos, 50.00, 260.00, g_ef_particle_noisesphere, g_ef_lgtning, 10, 10, 0.50, 15.00, 0.00, {0, 12, 255, 255}, 6, 0);
      }
  }
  return totaldmg;
}
public wcRunTalent_8_Thassarian(client,&Float:damagemulti) //Threat of Thassarian
{
  decl String:line[255];
  wcGetTalentInfo(8,2,2,3,client,"increment",line);
  damagemulti+=StringToFloat(line);
  return true;
}
wcRunTalent_Cleave(client,victim) //Cleave for Dark Transformation
{
  new Float:distance = 125.0;
  new Float:multi=0.30;
  new Float:location[3];
  GetClientAbsOrigin(victim,location);
  new dmg = RoundToNearest(GetDamage(client)*multi);
  new targets[MAXPLAYERS];
  new team=GetClientTeam(client);
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,distance);
  else if(team==3)
    amount=GetNearClients("@t",location,targets,distance);
  for (new x=0;x<=(amount-1);x++)
  {
    if(victim==targets[x])
      continue;
    wcPhysicalDamage(targets[x],client,dmg, "", "", true);
  }
}
wcRunTalent_GhoulGrip(client,victim) //Death Grip for Dark Transformation
{
  new cdtype=1;
  new Float:cooldown=1.50;
  new Float:timeleft=GetCDSpell(client,cdtype)-GetTickedTime();
  if(timeleft > 0.0)
    return;
  //Spell Specific variables
  new Float:zPower = 130.0;
  new Float:loc1[3];
  new Float:loc2[3];
  GetClientAbsOrigin(client,loc1);
  GetClientAbsOrigin(victim,loc2);
  new Float:power = 325.0;
  SetCdSpell(client, cdtype, cooldown, 0, "Ghoul Grip");
  //Spell code
  //if not boss
  if(GetBoss(victim)<=0 || (!IsBoss()) )
    PushPlayerTowardsPlayer(victim, client, power, zPower);

  //Effect

  decl Float:location[3];
  GetClientAbsOrigin(victim,location);
  EmitAmbientSound("wc/spells/deathgrip.wav", location);

  //Threat
  if(IsBoss() && (g_iAggro != client) && (GetBoss(client) <= 0))  { //No effect if top threat is caster
    new threat = GetThreat(g_iAggro) + 500;
    SetThreat(client, threat);
  }
  return;
}
public wcRunTalent_7_DemonsBlood(client)                                                  //Demon's Blood
{
  decl String:line[255];
  wcGetTalentInfo(7,2,2,3,client,"multi",line);
  new Float:multi=StringToFloat(line);
  SetBonusSta(client,GetBonusSta(client)+RoundToNearest(float(GetSta(client)+GetBonusSta(client))*multi));
  wcRecalculateStats(client);
}
public GetHealthstone(client)
{
  return g_iHStone[client];
}

public SetHealthstone(client, count)
{
  g_iHStone[client]=count;
}
public wcRunTalent_7_Healthstone(client)
{
  decl String:line[255];
  if(GetHealthstone(client)>0)
  {
    new target=GetAimTarget(client,50.0, true);
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
    wcGetTalentInfo(7,2,3,1,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(7,2,3,1,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    SetCdSpell(client, cdtype, cd, 0, "Healthstone");
    new Float:location[3],Float:locationatt[3];
    GetClientAbsOrigin(target,location);
    GetClientAbsOrigin(client,locationatt);
    decl String:name[255],String:name2[255];
    GetClientName(client,name,sizeof(name));
    GetClientName(target,name2,sizeof(name2));
    wcGetTalentInfo(7,2,3,1,client,"heal",line);
    new Float:percent=StringToFloat(line);
    new heal=RoundToNearest(float(GetMaxHealth(client))*percent);
    EmitAmbientSound("wc/spells/heal.wav",location);
    heal=wcHeal(client,target,heal,0,false);
    if(target!=client)
    {
      WCMessage(target,"healthstone target",target,name,heal);
    }
    WCMessage(client,"healthstone caster",client,name2,heal);
    location[2]+=40;
    locationatt[2]+=40;
    SetHealthstone(client, 0);
  }
  else
  {
    wcGetTalentInfo(7,2,3,1,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    new Float:cooldown=1.50;
    wcGetTalentInfo(7,2,3,1,client,"multi",line);
    new newhp = RoundToNearest(float(wcGetHealth(client))*StringToFloat(line));
    if(newhp < 1)
      newhp = 1;
    if(1 >= wcGetHealth(client)) {
      WCMessage(client,"too low hp",client);
      return 0;
      }
    wcSetHealth(client,newhp);
    SetCdSpell(client, cdtype, cooldown, 0, "Healthstone");
    SetHealthstone(client, 1);
    WCMessage(client,"healthstone created",client);
  }
  return 1;
}
public wcRunTalent_7_SacrificalPact(client) //Sacrifical Pact
{
  decl String:line[255];
  wcGetTalentInfo(7,2,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(7,2,4,1,client,"cooldown",line);
  new Float:cooldown=StringToFloat(line);
  new Float:timeleft=GetCDSpell(client,cdtype)-GetTickedTime();
  if(timeleft > 0.0)
    return 0;
  SetCdSpell(client, cdtype, cooldown, 0, "Sacrifical Pact");

  wcGetTalentInfo(7,2,4,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  wcGetTalentInfo(7,2,4,1,client,"multi",line);
  new absorb=RoundToNearest(float(GetMaxHealth(client))*StringToFloat(line));
  wcGetTalentInfo(7,2,4,1,client,"cost",line);
  new newhp = RoundToNearest(float(wcGetHealth(client))*StringToFloat(line));
  new hplost = wcGetHealth(client)-newhp;
  wcSetHealth(client,newhp);
  WCMessage(client,"pact cast",client,hplost,absorb);
  new Float:location[3];
  GetClientAbsOrigin(client,location);
  wcClientAbsorb[client]=absorb;
  SetEffectVictim(client,5); // absorb shield
  SetEffectSpellVic(client,5); //Absorb magic dmg
  g_iShieldStack[client]+=1;

  new Handle:datapack2=CreateDataPack();
  WritePackCell(datapack2,client); // Write the client index of owner
  WritePackCell(datapack2,5);
  CreateTimer(duration,DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
  new Handle:datapack3=CreateDataPack();
  WritePackCell(datapack3,client); // Write the client index of owner
  WritePackCell(datapack3,5);
  CreateTimer(duration,DestroyEffectSpellVic,datapack3,TIMER_HNDL_CLOSE);
  return 1;
}

public wcRunTalent_6_ChainLightning(client)
{
  new target=GetAimTarget(client,25.0);
  if(target>0)
  {
    if((GetClientTeam(client)==GetClientTeam(target)) || (!IsPlayerAlive(target)))
    target=0;
  }
  if(target<=0)
    return 0;
  decl String:line[255];
  wcGetTalentInfo(6,1,2,3,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(6,1,2,3,client,"base",line);
  new base=StringToInt(line);
  wcGetTalentInfo(6,1,2,3,client,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(6,1,2,3,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(6,1,2,3,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  wcGetTalentInfo(6,1,2,3,client,"mana",line);
  new manareq=StringToInt(line);
  wcGetTalentInfo(6,1,2,3,client,"manainc",line);
  manareq+=RoundToNearest(float(GetLevel(client))*StringToFloat(line));
  if(g_iEcho[client]>0)
  {
    manareq=0;
    g_iEcho[client]=0;
  }
  if(GetMana(client)-manareq<0) {
    WCMessage(client,"no mana",client);
    return 0;
  }
  wcSetMana(client,GetMana(client)-manareq);
  wcGetTalentInfo(6,2,3,2,client,"chance",line);
  new echochance = StringToInt(line);
  new echorandom =GetRandomInt(1,100);
  if(echochance>=echorandom) {
    g_iEcho[client]=1;
    wcRunTalent_6_ChainLightning(client);
    WCMessage(client,"Echo of the Elements activate",client);
    }
  else
    SetCdSpell(client,cdtype,cd, 0, "Chain Lightning");
  new dmg = RoundToNearest(GetSpellDmg(client,multi)+float(base));
  decl Float:location[3],Float:locationb[3];
  new targett;
  new targetc;
  GetClientAbsOrigin(target,location);
  new team=GetClientTeam(client);
  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(target,name2,sizeof(name2));
  wcSpellDamage(target,client,dmg, "", "",line);
  WCMessage(client,"chain lightning caster",client,name2,dmg);
  WCMessage(target,"chain lightning victim",target,name,dmg);
  if(team==3) {
    targett=GetNearestClient("@t",location,distance,target);
    if(targett>0) {
      GetClientAbsOrigin(targett,locationb);
      targetc=GetNearestClient("@t",locationb,distance,target,targett);
    }
  }
  if(team==2) {
    targett=GetNearestClient("@ct",location,distance,target);
    if(targett>0) {
      GetClientAbsOrigin(targett,locationb);
      targetc=GetNearestClient("@ct",locationb,distance,target,targett);
    }
  }
  if(targett>0) {
    dmg=RoundToNearest(float(dmg)*0.66);
    wcSpellDamage(targett,client,dmg, "", "",line);
    WCMessage(targett,"chain lightning victim",targett,name,dmg);
    dmg=RoundToNearest(float(dmg)/0.66);
  }
  if(targetc>0) {
    dmg=RoundToNearest(float(dmg)*0.33);
    wcSpellDamage(targetc,client,dmg, "", "",line);
    WCMessage(targetc,"chain lightning victim",targetc,name,dmg);
    dmg=RoundToNearest(float(dmg)/0.33);
  }
  ChainLightningEffect(client, target, targett, targetc);
  return 1;
}
ChainLightningEffect(caster, target, target1, target2) {
  decl Float:start[3], Float:cont[3],Float:end[3],Float:origin[3];
  GetClientAbsOrigin(caster, origin);
  GetClientAbsOrigin(target, start);
  origin[2]+=40;
  start[2]+=40;
  VS_BeamPoints(origin, start, g_plasmabeam, g_plasmabeam, 1, 5, 1.50, 4.00, 4.00, 1, 0.00, {255, 255, 255, 255}, 10);
  VS_BeamPoints(origin, start, g_bluelight1, g_ef_muzzleflashX, 1, 5, 1.50, 2.00, 1.00, 1, 1.00, {10, 0, 255, 255}, 30);
  EmitAmbientSound("wc/spells/lightningbolt.wav",start);
  if(target1>0) {
    GetClientAbsOrigin(target1, cont);
    cont[2]+=40;
    VS_BeamPoints(start, cont, g_plasmabeam, g_plasmabeam, 1, 5, 1.50, 4.00, 4.00, 1, 0.00, {255, 255, 255, 255}, 10);
    VS_BeamPoints(start, cont, g_bluelight1, g_ef_muzzleflashX, 1, 5, 1.50, 2.00, 1.00, 1, 1.00, {10, 0, 255, 255}, 30);
    EmitAmbientSound("wc/spells/lightningbolt.wav",cont);
  }
  if(target2>0) {
    GetClientAbsOrigin(target2, end);
    end[2]+=40;
    VS_BeamPoints(cont, end, g_plasmabeam, g_plasmabeam, 1, 5, 1.50, 4.00, 4.00, 1, 0.00, {255, 255, 255, 255}, 10);
    VS_BeamPoints(cont, end, g_bluelight1, g_ef_muzzleflashX, 1, 5, 1.50, 2.00, 1.00, 1, 1.00, {10, 0, 255, 255}, 30);
    EmitAmbientSound("wc/spells/lightningbolt.wav",end);
  }
}
/*
wcRunTalent_6_ChainLightning(client,bool:echo=false)
{
  new targets[3];
  targets[0]=GetAimTarget(client,25.0);
  new team=GetClientTeam(client);
  if(targets[0]<=0)
    return 0;
  else if ((team==GetClientTeam(targets[0])) || (!IsPlayerAlive(targets[0])))
    return 0;

  decl String:line[255];
  if(!echo)
  {
    wcGetTalentInfo(6,1,2,3,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(6,1,2,3,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    wcGetTalentInfo(6,1,2,3,client,"mana",line);
    new manareq=StringToInt(line);
    if(GetMana(client)-manareq<0) {
      WCMessage(client,"no mana",client);
      return 0;
    }
    wcSetMana(client,GetMana(client)-manareq);
    SetCdSpell(client,cdtype,cd, 0, "Chain Lightning");
  }
  wcGetTalentInfo(6,2,3,2,client,"chance",line);
  new echochance = StringToInt(line);
  if(echochance>=GetRandomInt(1,100))
  {
    wcRunTalent_6_ChainLightning(client,true);
    WCMessage(client,"Echo of the Elements activate",client);
  }

  wcGetTalentInfo(6,1,2,3,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(6,1,2,3,client,"base",line);
  new base=StringToInt(line);
  new dmg = RoundToNearest(GetSpellDmg(client,multi)+float(base));


  decl String:name[255],String:name2[255];
  GetClientName(client,name,sizeof(name));
  GetClientName(targets[0],name2,sizeof(name2));
  WCMessage(client,"chain lightning caster",client,name2,dmg);

  new Float:loc[3];
  GetClientAbsOrigin(client,loc);
  EmitAmbientSound("wc/spells/lightningbolt.wav",loc);

  wcGetTalentInfo(6,1,2,3,client,"distance",line);
  new Float:distance=StringToFloat(line);
  wcGetTalentInfo(6,2,3,2,client,"name",line); //line must contain talentname after this line

  new amount;
  new people[3];
  new Float:coords[3][3];

  switch(team)
  {
    case 2: amount = GetPlayerChain(client,3 , loc, distance, 3, people, coords, 1);
    case 3: amount = GetPlayerChain(client,2 , loc, distance, 3, people, coords, 1);
  }
  new Float:m[3]={1.0,0.66,0.33};
  new tmp;
  if(amount >0)
  {
    WCMessage(client,"testmessage",client,"amount",amount);
    ChainLightningEffect(loc,coords[0]);
    tmp=RoundToNearest(float(dmg)*m[0]);
    wcSpellDamage(people[0],client,tmp, "", "",line);
    WCMessage(people[0],"chain lightning victim",people[0],name,tmp);
    WCMessage(client,"testmessageF",client,"coord A-0",coords[0][0]);
    WCMessage(client,"testmessageF",client,"coord A-1",coords[0][1]);
    WCMessage(client,"testmessageF",client,"coord A-2",coords[0][0]);

    for (new i=1;i<amount;i++)
    {
      WCMessage(client,"testmessageF",client,"coord A-0",coords[i][0]);
      WCMessage(client,"testmessageF",client,"coord A-1",coords[i][1]);
      WCMessage(client,"testmessageF",client,"coord A-2",coords[i][2]);
      tmp=RoundToNearest(float(dmg)*m[i]);
      wcSpellDamage(people[i],client,tmp, "", "",line);
      WCMessage(people[i],"chain lightning victim",people[i],name,tmp);
      ChainLightningEffect(coords[i],coords[i+1]);
    }
  }
  else
    return 0;

  return 1;
}

ChainLightningEffect(Float:start[3],Float:end[3])
{
  start[2]+=40;
  end[2]+=40;
  VS_BeamPoints(start, end, g_plasmabeam, g_plasmabeam, 1, 5, 1.50, 4.00, 4.00, 1, 0.00, {255, 255, 255, 255}, 10);
  VS_BeamPoints(start, end, g_bluelight1, g_ef_muzzleflashX, 1, 5, 1.50, 2.00, 1.00, 1, 1.00, {10, 0, 255, 255}, 30);
}

//teams 0=noteam, 1=spectator, 2=terrorist, 3=counter-terrorist
//type: 1=closest in distance, 2=farthest in distance, 3=random in distance
stock GetPlayerChain(client, team, Float:loc[3], Float:distance, length, people[], Float:coords[][3], type=1)
{
  new amount=0, index;
  new chain[length];
  new bool:found=false, bool:skip=false;
  new Float:loc2[3], Float:loc3[3];
  for(new i=0;i<3;i++)
    loc2[i]=loc[i];
  for(new i=0;i<=(length-1);i++)
  {
    new Float:est;
    found=false;
    switch(type)
    {
      case 1: est=1000.0;
      case 2: est=   0.0;
      case 3: est=   0.0;
    }
    WCMessage(client,"testmessage",client,"length",i);
    for(new j=1;j<=GetMaxClients();j++)
    {
      skip=false;
      if (IsClientInGame(j))
      {
        if(IsPlayerAlive(j) && (GetClientTeam(j)==team))
        {
          for(new k=0;k<amount;k++)
          {
            if(j==chain[k])
              skip=true;
          }
          if(skip)
            continue;
          GetClientAbsOrigin(j,loc3);
          new Float:dist=GetDistanceBetween(loc2,loc3);
          if(dist<distance)
            continue;
          switch(type)
          {
            case 1: //nearest within distance
            {
              if(est>dist)
              {
                est=dist;
                index=j;
                found=true;
              }
            }
            case 2: //farthest within distance
            {
              if(est<dist)
              {
                est=dist;
                index=j;
                found=true;
              }
            }
            case 3: //random within distance
            {
              index=j;
              found=true;
              if(GetRandomInt(1,3)==1)
                break;
            }
          }
        }
      }
    }

    if(found)
    {
      chain[i]=index;
      loc2=loc3;
      coords[i]=loc3;
      amount++;
    }
  }
  for(new i=0;i<amount;i++)
    people[i]=chain[i];
  WCMessage(client,"testmessage",client,"outer return amount",amount);
  return amount;
}
*/
public wcRunTalent_4_ArdentDefender(client)   //Ardent Defender
{
  decl String:line[255];
  wcGetTalentInfo(4,2,2,1,client,"chance",line);
  new chance = StringToInt(line);
  new random = GetRandomInt(1,100);
  if(chance>=random)
  {
    wcGetTalentInfo(4,2,2,1,client,"increase",line);
    new Float:increase=StringToFloat(line);
    wcGetTalentInfo(4,2,2,1,client,"duration",line);
    new Float:duration=StringToFloat(line);
    AddParry(client, increase);
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,client); // Write the client index of owner
    WritePackFloat(datapack2,increase);
    CreateTimer(duration,EndArdent,datapack2,TIMER_HNDL_CLOSE);
    AddEMsgAff("ardent defender", true);
  }
}
public Action:EndArdent(Handle:timer,Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  new Float:increase=ReadPackFloat(datapack);
  RemoveParry(client, increase);
}
public wcRunTalent_6_LavaBurst(client,victim) //Lava Burst
{
  decl String:line[255];
  wcGetTalentInfo(6,1,3,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  new Float:timeleft=GetCDSpell(client,cdtype)-GetTickedTime();
  wcGetTalentInfo(6,1,3,1,client,"mana",line);
  new mana = StringToInt(line);
  wcGetTalentInfo(6,1,3,1,client,"manainc",line);
  mana+=RoundToNearest(float(GetLevel(client))*StringToFloat(line));
  wcGetTalentInfo(6,1,3,1,client,"chance",line);
  if((StringToInt(line)>=GetRandomInt(1,100)) && (0.0 >= timeleft) && (GetMana(client)-mana>0))
  {
    wcGetTalentInfo(6,1,3,1,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    wcGetTalentInfo(6,1,3,1,client,"basedmg",line);
    new basedmg=StringToInt(line);
    wcSetMana(client,GetMana(client)-mana);
    SetCdSpell(client, cdtype, cd, 0, "Lava Burst");
    new Float:location[3];
    GetClientAbsOrigin(victim,location);
    new Float:locationatt[3];
    GetClientAbsOrigin(client,locationatt);
    wcGetTalentInfo(6,1,3,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    new dmg=GetSpellDmg(client,multi)+basedmg;
    dmg=RoundToNearest(float(dmg)*float(g_iFlameShock[victim]));
    if(HasGlyph(client, GLYPH_SHAMAN_LAVABURST))
      if(g_iFlameShock[victim]==0)
        wcRunSpell_6_3(3,6,victim,client);
    decl String:name[255],String:name2[255];
    GetClientName(client,name,sizeof(name));
    GetClientName(victim,name2,sizeof(name2));
    WCMessage(client,"lava burst caster",client,name2,dmg);
    WCMessage(victim,"lava burst vic",victim,name,dmg);
    wcSpellDamage(victim,client,dmg, "", "",line);
    location[2]+=40;
    EmitAmbientSound("wc/spells/fireball.wav",location);
    //VS_Explosion(location, g_ef_fire1, 2.0, 1, 1, 75, 0, {0.0, 0.0, 1.0}, 0, 0.0);
    locationatt[2]+=40;
    VS_BeamPoints(location, locationatt, g_ef_scanner,g_ef_scanner, 1, 5, 3.00, 15.00, 15.00, 1, 0.00, {222, 30, 30, 255}, 20);
    VS_BeamPoints(location, locationatt, g_ef_scanner, g_ef_scanner, 1, 5, 3.00, 15.00, 15.00, 1, 0.00, {222, 30, 30, 255}, 20);
    VS_BeamPoints(locationatt, location, g_ef_glow1, g_ef_glow1, 1, 5, 3.00, 20.00, 10.00, 10, 0.00, {222, 30, 30, 255}, 160);
    VS_BeamPoints(locationatt, location, g_ef_lgtning, g_ef_glow1, 1, 5, 3.00, 20.00, 10.00, 0, 0.00, {222, 30, 30, 255}, 160);
    VS_BeamRingPoint(location, 90.00, 60.00, g_ef_glow1, g_ef_lgtning, 10, 10, 3.00, 50.00, 0.00, {222, 30, 30, 255}, 60, 0);
  }
}

public wcRunTalent_6_AncestorsProtection(client)  {
  decl String:line[255];
  wcGetTalentInfo(6,2,4,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  wcGetTalentInfo(6,2,4,2,client,"multi_armor",line);
  new Float:multi_armor=StringToFloat(line);

  new agility = GetAgi(client)+GetBonusAgi(client);

  new resbonus=RoundToNearest(float(agility)*multi);
  wcClientsBonusStats[client][RESISTANCE]+=resbonus;

  new bonus = RoundToNearest(float(agility)*multi_armor);
  SetBonusArmor(client, float(GetBonusArmor(client)+bonus));

  wcRecalculateStats(client);
}

public wcRunTalent_2_GarroteDamage(client,victim,&damage)
{
  decl String:line[255];
  wcGetTalentInfo(2,3,4,2,client,"bleed",line);
  new bleed = RoundToNearest(float(damage)*StringToFloat(line));
  wcGetTalentInfo(2,3,4,2,client,"tickdelay",line);
  new Float:tickdelay = StringToFloat(line);
  wcGetTalentInfo(2,3,4,2,client,"ticks",line);
  new ticks = StringToInt(line);
  bleed = RoundToNearest(float(bleed)/float(ticks));
  new effid=CreateEffectData(victim,client,ticks,bleed,0,0,0,0,g_respawns[victim],0);
  CreateTimer(tickdelay, Garrote, effid, TIMER_REPEAT);
}
public Action:Garrote(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
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
  if (effectsdata[effid][9]!=g_respawns[effectsdata[effid][1]])
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--;
  wcPhysicalDamage(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][4],"","",true);
  return Plugin_Continue;
}
new g_iCombatReadyB[65];
wcRunTalent_2_CombatReadiness(client) //Combat Readiness
{
  decl String:line[255];
  wcGetTalentInfo(2,2,3,3,client,"multi",line);
  new Float:increase = StringToFloat(line);
  g_iCombatReadyB[client]+=1;
  AddParry(client, increase);
  wcRecalculateStats(client);
  g_iCombatReady[client]=1;
}
RemoveReadiness(client)
{
  decl String:line[255];
  wcGetTalentInfo(2,2,3,3,client,"multi",line);
  new Float:increase = StringToFloat(line)*float(g_iCombatReadyB[client]);
  if(g_iCombatReadyB[client]>0)
    AddParry(client, -increase);
  wcRecalculateStats(client);
  if(g_iCombatReadyB[client]>0)
    WCMessage(client,"combat readiness end",client);
  g_iCombatReady[client]=0;
  g_iCombatReadyB[client]=0;
}
wcRunTalent_2_BasicMaster(client,victim,&totaldmg) //Master of Basics
{
  decl String:line[255];
  new random=GetRandomInt(1,100);
  wcGetTalentInfo(2,2,4,1,client,"chance",line);
  new chance = StringToInt(line);
  if(chance>=random)
  {
    wcGetTalentInfo(2,2,4,1,client,"mana",line);
    wcSetMana(client,GetMana(client)+StringToInt(line));
    wcGetTalentInfo(2,2,4,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    new dmg=RoundToNearest(float(totaldmg)*multi);
    totaldmg+=dmg;
    decl Float:location[3];
    GetClientAbsOrigin(victim,location);
    EmitAmbientSound("wc/spells/obliterate.wav",location);
    AddEMsgAff("master of basics msg", true);
    new Float:pos[3];
    GetClientAbsOrigin(victim,pos);
    pos[2]+=45.0;
    VS_BeamRingPoint(pos, 25.00, 130.00, g_ef_particle_noisesphere, g_ef_lgtning, 10, 10, 0.50, 15.00, 0.00, {0, 0, 25, 255}, 6, 0);
  }
  return totaldmg;
}
public wcRunTalent_4_HolyShieldGlyph(victim,attacker) //Holy Shield Revenge glyph
{
  decl String:line[255];

  new dmg = RoundToNearest(
    2 +
    ( float(GetDamage(victim)) * 0.03 +
      float(GetSta(victim)+GetBonusSta(victim)) * 0.04   ) *
    0.4);
  wcSpellDamage(attacker,victim,dmg, "", "",line);
}

public wcRunTalent_4_RighteousShield(client)
{
  decl String:line[255];
  if(HasShield(client)) 
  {
    wcGetTalentInfo(4,2,4,1,client,"holy",line);
    new holyreq=StringToInt(line);
    new power=GetHolyPower(client);
    if(power>=holyreq)
    {
      if(g_iRighteousShield[client]==0)
      {
        WCMessage(client,"righteous shield on",client);
        SetHolyPower(client,GetHolyPower(client)-holyreq);
        g_iRighteousShield[client]=1;
      }
      else
        WCMessage(client,"already active",client);
    }
    else
      WCMessage(client,"no holy",client);
  }
  else
    WCMessage(client, "need shield", client);
}

public wcRunTalent_4_RighteousShieldActivate(client,victim,&damage) //Shield of the Righteous
{
  decl String:line[255];
  if(1.0>g_fGreaterHeal[client])
    g_fGreaterHeal[client]=1.0;
  wcGetTalentInfo(4,2,4,1,client,"heal",line);
  g_fGreaterHeal[client]+=StringToFloat(line);
  AddEMsgAff("Shield of the Righteous name", true);
  wcGetTalentInfo(4,2,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(4,2,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client,cdtype,cd, 0, "Shield of the Righteous");
  wcGetTalentInfo(4,2,4,1,client,"base",line);
  new dmg = StringToInt(line);
  wcGetTalentInfo(4,2,4,1,client,"multi",line);
  dmg+=RoundToNearest(float(GetSta(client)+GetBonusSta(client))*StringToFloat(line));
  damage+=dmg;
  g_iRighteousShield[client]=0;
  new Float:location[3];
  GetClientAbsOrigin(victim,location);
  EmitAmbientSound("wc/spells/Shield_of_the_Righteous.wav",location);
  
  wcGetTalentInfo(4,2,4,1,client,"duration",line);
  new Float:duration=StringToFloat(line);
  SetEffectVictim(client,13);
  CreateTimer(duration,RemoveRighteous,client);

}
public Action:RemoveRighteous(Handle:timer, any:client)
{
  RemoveEffectVictim(client,13);
}


public wcRunTalent_2_Shadowdance(client) //Shadow Dance
{
  decl String:line[255];
  wcGetTalentInfo(2,1,4,1,client,"cost",line);
  if(GetMana(client)>=StringToInt(line))
  {
    wcSetMana(client,GetMana(client)-StringToInt(line));
    g_iShadowDance[client]=1;
    wcGetTalentInfo(2,1,4,1,client,"cooldowntype",line);
    new cdtype=StringToInt(line);
    wcGetTalentInfo(2,1,4,1,client,"cooldown",line);
    new Float:cd=StringToFloat(line);
    SetCdSpell(client,cdtype,cd, 0, "Shadow Dance");
    wcGetTalentInfo(2,1,4,1,client,"duration",line);
    new Float:duration=StringToFloat(line);
    CreateTimer(duration,RemoveShadowDance,client);
    WCMessage(client,"shadowdance cast",client,duration);
  }
  else
    WCMessage(client,"no energy",client);
}


public Action:RemoveShadowDance(Handle:timer, any:client)
{
  g_iShadowDance[client]=0;
  WCMessage(client,"shadowdance end",client,g_iShadowDamageDealt[client]);
  g_iShadowDamageDealt[client]=0;
}

public wcRunTalent_3_ArcaneBarrage(client, victim)
{
  decl String:line[255];
  wcGetTalentInfo(3,3,2,1,client,"cost",line);
  new cost = StringToInt(line);
  if(GetArcaneCharge(client)>=cost)
  {
    SpendArcaneCharge(client, cost);
    wcGetTalentInfo(3,3,2,1,client,"multi",line);
    new Float:multi=StringToFloat(line);
    new dmg=GetSpellDmg(client,multi);
    wcGetTalentInfo(3,3,2,1,client,"base",line);
    dmg+=StringToInt(line);
    decl String:vicMsg[300], String:attMsg[300];
    Format(vicMsg,sizeof(vicMsg),"%T","arcaneb target", victim);
    Format(attMsg,sizeof(attMsg),"%T","arcaneb caster", client);
    wcSpellDamage(victim,client,dmg, attMsg, vicMsg,line);

    decl Float:location[3],Float:locationatt[3];
    GetClientAbsOrigin(victim,location);
    GetClientAbsOrigin(client,locationatt);
    new color[4]={255,255,255,255};
    location[2]+=35;
    locationatt[2]+=30;
    EmitAmbientSound("wc/spells/arcanebarrage.wav",location);
    BeamEffect("@all",locationatt,location,g_ef_tp_beam001,5,1.5,3.0,4.0,color,0.0,25);
    locationatt[2]+=10;
    VS_GlowSprite(locationatt, g_ef_WXplo1, 1.00, 1.00, 255);
    locationatt[0]-=8;
    BeamEffect("@all",locationatt,location,g_ef_tp_beam001,5,1.5,3.0,4.0,color,0.0,25);
    locationatt[0]+=16;
    BeamEffect("@all",locationatt,location,g_ef_tp_beam001,5,1.5,3.0,4.0,color,0.0,25);
  }
}

wcRunTalent_3_RuneOfPower(client)
{
  decl String:line[255];
  
  wcGetTalentInfo(3,3,4,1,client,"cooldowntype",line);
  new cdtype=StringToInt(line);
  wcGetTalentInfo(3,3,4,1,client,"cooldown",line);
  new Float:cd=StringToFloat(line);
  SetCdSpell(client,cdtype,cd, 0, "Rune of Power");
  new manareq = GetTalentCost(client, 3,4,1);

  wcSetMana(client,GetMana(client)-manareq);
  wcGetTalentInfo(3,3,4,1,client,"duration",line);
  new ticks=RoundToNearest(StringToFloat(line)/0.5);
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  new effid=CreateEffectData(0,client,ticks,0,_:location[0],0,_:location[1],_:location[2],g_RoundCount,0);
  if(effid!=0)
  {
    CreateTimer(0.5, RuneofPowerEffect, effid, TIMER_REPEAT);
  }
  VS_BeamRingPoint(location, 350.00, 365.00, g_ef_lgtning, g_ef_lgtning, 1, 15, 20.0, 12.00, 2.00, {0, 32, 255, 255}, 1, 0);
  location[2]+=6;
  VS_BeamRingPoint(location, 350.00, 365.00, g_ef_lgtning, g_ef_lgtning, 1, 15, 20.0, 12.00, 2.00, {0, 32, 255, 255}, 1, 0);
}
public Action:RuneofPowerEffect(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0) //empty effectdata
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][2])) 
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0) //ticks
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  decl String:line[255];
  wcGetTalentInfo(3,3,4,1,effectsdata[effid][2],"distance",line);
  new Float:aoe=StringToFloat(line);
  new Float:location[3];
  location[0] = Float:effectsdata[effid][5];
  location[1] = Float:effectsdata[effid][7];
  location[2] = Float:effectsdata[effid][8];
  
  new targets[MAXPLAYERS];
  new team=GetClientTeam(effectsdata[effid][2]);
  new amount;
  if(team==3)
    amount=GetNearClients("@ct",location,targets,aoe);
  if(team==2)
    amount=GetNearClients("@t",location,targets,aoe);
  
  for (new x=0;x<=(amount-1);x++)
  {
    if(targets[x]==effectsdata[effid][2])
    {
      g_iRuneBuff[effectsdata[effid][2]]+=1;
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,effectsdata[effid][2]);
      CreateTimer(0.5,EndRuneofPower,datapack4,TIMER_HNDL_CLOSE);
      wcRecalculateStats(effectsdata[effid][2]);
    }
  }
  return Plugin_Continue;
}

public Action:EndRuneofPower(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  if(g_iRuneBuff[client]!=0)
    g_iRuneBuff[client]-=1;
  wcRecalculateStats(client);
}