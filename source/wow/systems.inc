/*
* File: systems.inc
* Description: contains xp system/item system/stats functions
* Author(s): Stinkyfax
*/
// 243-244 professions
// 245-246 profession skills
// 242 - boss ID
// 247 - character's ID in charactersdata list
// 248 - 1 if is killed 0 if is not or suicided. Is used for boss
// 249 - One round spells which work only once a round like Reincarnation
// 250 - arena points
// 251 - arenas played
// 252 - honor
// 253 - self cast
// 254 - 2hand  collapse with rank 254
// 258 - clip modifier

#pragma semicolon 1

bool:IsDoubleHand(client)  { //player wielding 2hand weapon //2-handed //two-handed //twohand
  if(wcUsers[client][254]==0)
    return false;
  return true;
}

SetDoubleHand(client, bool:b) {
  if(b)
    wcUsers[client][254]=1;
  else
    wcUsers[client][254]=0;
}

getOneRound(client)  {
   return wcUsers[client][249];
}

setOneRound(client,value)  {
   wcUsers[client][249]=value;
}

bool:IsKilled(client)   {
   if (wcUsers[client][248]==1)
      return true;
   return false;
}

Killed(client, bool:is) {
   decl val;
   if(is)
      val=1;
   else
      val=0;
   wcUsers[client][248]=val;
}

CurrentCharacterIndex(client)
{
  return wcUsers[client][247];
}

public Action:SaveWorld(Handle:timer, any:trash)
{
  if(G_RebootTotal==0)
  {
    for(new x=1;x<=GetMaxClients();x++)
      if(IsClientConnected(x))
        if(IsClientInGame(x)) if(playerloaded[x]==1)
        {
          wcSavePlayerData(x);
          WCMessage(x,"saving world",x);
        }
    LogMessage("Saving World");

  }
  return Plugin_Continue;
}

public Initialize(client)
{
  playerids[client]=0;
  playerloaded[client]=0;

  setLoadStage(client, 1);
  CreateTimer(0.5, InitLoadPlayer, client, TIMER_FLAG_NO_MAPCHANGE);
  CreateTimer(1.0,InitTimeOut,client,TIMER_FLAG_NO_MAPCHANGE|TIMER_REPEAT);
    //wcLoadCharacterData(client);
}

new g_ff_playerload_stage[65];
new Float:g_ff_playerload_time[65];
stock getLoadStage(client)  {
  return g_ff_playerload_stage[client];
}
stock setLoadStage(client, value) {
  g_ff_playerload_stage[client]=value;
  g_ff_playerload_time[client] = GetEngineTime();
  return value;
}
stock incLoadStage(client)  {
  setLoadStage(client, getLoadStage(client)+1 );
  g_ff_playerload_time[client] = GetEngineTime();
  return getLoadStage(client);
}
stock bool:canProceedLoading(client) {
  if(IsClientInGame(client) && IsFakeClient(client))
    return true;
  if(getLoadStage(client)==5) {
    if( (GetEngineTime() - g_ff_playerload_time[client]) < 30.0)
      return false;
  }
  return true;
}
stock waitLoadStage(client, String:func[], Float:time)  {
  new Handle:pack = CreateDataPack();
  ResetPack(pack);
  WritePackCell(pack, client);
  WritePackString(pack, func);
  CreateTimer(time, waitLoadTimer, pack, TIMER_HNDL_CLOSE);
}
public Action:waitLoadTimer(Handle:tmpr, Handle:pack) {
  ResetPack(pack);
  new client = ReadPackCell(pack);
  decl String:buffer[50];
  ReadPackString(pack, buffer, 50);
  new Function:func = GetFunctionByName(INVALID_HANDLE, buffer);
  Call_StartFunction(INVALID_HANDLE, func);
  Call_PushCell(client);
  if(Call_Finish() != SP_ERROR_NONE)  {
    decl String:err[100];
    Format(err,100,"Could not callback the function: %s",buffer);
    Logger_log(LogType:LT_file, LogLevel:LL_error, "PlayerLoader", err);
  }
}


/* load stages
0 - error
1 - get playerid
2 - get player info
3 - install new player
4 - load player data
5 - wait for previus serv to finish saving
6 - load characters data
10 - loaded

*/
translateLoadStage(stage, String:back[], max) {
  Format(back, max, "Unknown");
  switch(stage) {
    case 1:   Format(back, max, "In a queue");
    case 2:   Format(back, max, "Getting Information");
    case 3:   Format(back, max, "Installing");
    case 4:   Format(back, max, "Loading Data");
    case 5:   Format(back, max, "Waiting for other servers to finalize");
    case 6:   Format(back, max, "Loading Characters");
    case 10:   Format(back, max, "Complete!");
  }
}


public Action:InitLoadPlayer(Handle:timer, any:client)   {
  GetPlayerData(client);
  return Plugin_Stop;
}

public Action:InitTimeOut(Handle:timer, any:client)
{
  if(!IsClientConnected(client))
    return Plugin_Stop;
  new stage = getLoadStage(client);
  stageMessage(client);
  if(stage == 10) {
    WCMessage(client,"char data loaded",client);
    return Plugin_Stop;
  }

  return Plugin_Continue;


  /*
  if(!IsClientConnected(client))
    return Plugin_Stop;
  if(g_ServerPause)
   return Plugin_Continue;
  if(g_T2Queue>0)
   return Plugin_Continue;
  if(playerids[client]==0)
  {
    decl String:name[70];
    GetClientName(client,name,sizeof(name));
    KickClient(client,"Failed Authorization in WoW Mod.");
    return Plugin_Stop;
  }
  if(playerloaded[client]==0)
  {
    decl String:name[70];
    GetClientName(client,name,sizeof(name));
    KickClient(client,"Failed Loading Data in WoW Mod.");
    return Plugin_Stop;
  }
  WCMessage(client,"char data loaded",client);
  return Plugin_Stop;
  */
}

getServerId() {
  return COMM_ID;
}

public IsCharacterOnline(charId)
{
  for (new x=1;x<=GetMaxClients();x++)
  {
    if (characterids[x][0]==charId)
    {
      if(IsClientInGame(x))
        return x;
    }
  }
  return 0;
}

public IsPlayerOnline(charId)
{
  for (new x=1;x<=GetMaxClients();x++)
  {
    if (playerids[x]==charId)
    {
      if(IsClientInGame(x))
        return x;
    }
  }
  return 0;
}

public UniqueID2(client)
{
  new String:output[32];
  new bool:exists=GetClientAuthString(client,output,32);
  if(exists)
  {
    if(StrEqual(output,"BOT",true))
    {
      decl String:name[64];
      GetClientName(client,name,64);
      Format(output,64,"BOT_%s",name);
      return 1;
    }
    ReplaceString(output, 32, "STEAM_", "");
    ReplaceString(output, 32, ":", "");
    return StringToInt(output);
  }
  return -1;
}

new g_ff_32bitIds[65];

stock SteamToInt(String:output[]) {
  ReplaceString(output, 32, "STEAM_", "");
  ReplaceString(output, 32, ":", " ");
  new second,main;
  decl String:buffer[32];
  new StringID=BreakString(output, buffer, sizeof(buffer));
  /*first=StringToInt(buffer);*/
  new old=StringID;
  StringID=BreakString(output[StringID],buffer,sizeof(buffer));
  StringID+=old;
  second=StringToInt(buffer);
  BreakString(output[StringID],buffer,sizeof(buffer));
  main=StringToInt(buffer);
  return main + second *  2147483647;
}

public UniqueID(client)
{
  new ret=0;
  new String:output[32];
  new bool:exists=GetClientAuthString(client,output,32);
  if(exists)   if(!StrEqual("STEAM_ID_LAN",output))
  {
    if(IsFakeClient(client) || StrEqual(output,"BOT",true))
    {
      ret = 1;
    }
    else  {
      ret = SteamToInt(output);
    }
  }
  else
  {
      new intIp;
      intIp-= 256*256*256*128;
      new String:ip[50];
      if(!GetClientIP(client, ip, sizeof(ip), true))  {
         ret = 0;
      }
      else  {
      new ipPart[4];
      new ind=0;
      for(new i=0;i<3;i++) {
         decl String:temp[6];
         ind+=SplitString(ip[ind],".",temp,sizeof(temp));
         if(i==2)
            ipPart[3]=StringToInt(ip[ind]);
         ipPart[i]=StringToInt(temp);
      }
      intIp+=256*256*256*(ipPart[0]);
      intIp+=256*256*(ipPart[1]);
      intIp+=256*(ipPart[2]);
      intIp+=(ipPart[3]);
      ret = intIp;
      }
  }
  if(ret==0)
    LogError("Error in logics #310");
  g_ff_32bitIds[client]=ret;
  return ret;
}

GetClient32BitSteam(client) {
  return g_ff_32bitIds[client];
}

public wcGiveXp(client,amount,bool:display)
{
   if(g_ArenaRunning)
      return;
   if(GetClass(client)<=0)
    return;


   if(isClientPDModule(client, PDModule:PDM_xp)) {
    new pdbonus = RoundToNearest(float(amount)*0.35);
    if(pdbonus>10)
      WCMessage(client, "pdm xp increased", client, pdbonus);
    amount+=pdbonus;
   }


   new reqxp=GetReqXp(client);
   new level=GetLevel(client);
   new gained=amount;
   new oldmoney=GetMoney(client);
   amount+=GetXp(client);
   if  (
     (level >=85) ||
     (level >= 80
       && (SERVER_80 || !HasExpansion(client, Expansion:E_CATA))
       && (amount >=10000))
     )  {
      new convertMoney=RoundToNearest(float(amount)*EXP_MONEY_RATIO);
      WCMessage(client,"converted xp",client,convertMoney);
      wcSetMoney(client,GetMoney(client)+convertMoney);
      oldmoney=GetMoney(client);
      amount=1;
   }
   if(IsFakeClient(client))
   {
      if (level >= BOT_MAXLEVEL)
         amount =1;
   }
   while(amount>=reqxp)
   {
      amount-=reqxp;
      wcSetLevel(client,GetLevel(client)+1);
      reqxp=GetReqXp(client);
   }
   if(amount < 0)
    amount=0;
   wcSetStaticXp(client,amount);
   if(display&&EXP_NOTICE)
      WCMessage(client,"Give XP",client,gained,GetXp(client),GetReqXp(client));
   new newlevel=GetLevel(client);
   if(level!=newlevel)
   {
      if(ANNOUNCE_CHANGELVL)
      {
        decl String:name[64];
        GetClientName(client,name,64);
        new maxplayers=GetMaxClients();
        for(new x=1;x<=maxplayers;x++)
          if(IsClientInGame(x))
            WCMessage(x,"Announce levelup",x,name,newlevel);
      }
      else
        WCMessage(client,"Tell player levelup",client,newlevel);
      oldmoney=GetMoney(client)-oldmoney;
      WCMessage(client,"Notify levelup",client,oldmoney,GetMoney(client),GetPoints(client));
      if(getTPoints(client, getActiveSpec(client))>0)
      {
        WCMessage(client,"unused talent point",client,getTPoints(client, getActiveSpec(client)));
      }
      PlayLevelupSound(client);
      // Check if bot went over maxlevel
   }
}


public wcSetStaticXp(client,amount)
{
  if(amount<0) amount=0;
  wcUsers[GetPlayerID(client)][4]=amount;
}

/*    Format(uniqueid,sizeof(uniqueid),"%s_%i",uniqueid,id)
    decl String:buffer[8020],String:catname[255];String:itemname[255];
    Format(buffer,sizeof(buffer),"points = '%d', str = '%d', agi = '%d', int = '%d', money = '%d', xp = '%d', reqxp = '%d', level = '%d'",GetPoints(client),GetStr(client),GetAgi(client),GetInt(client),GetMoney(client),GetXp(client),GetReqXp(client),GetLevel(client));
    for (new x=1;x<=100;x++)
    {
      Format(buffer,sizeof(buffer),"%s, slot_%i_cat = '%s', slot_%i_item = '%s'",buffer,x,GetSlotCat(client,x),x,GetSlotItem(client,x));
    }
    Format(buffer,sizeof(buffer),"%s, backs_cat = '%s', backs_item = '%s'",buffer,GetCharCatItem(client,"backs"),GetCharItemId(client,"backs"));
    Format(buffer,sizeof(buffer),"%s, chests_cat = '%s', chests_item = '%s'",buffer,GetCharCatItem(client,"chests"),GetCharItemId(client,"chests"));
    Format(buffer,sizeof(buffer),"%s, feet_cat = '%s', feet_item = '%s'",buffer,GetCharCatItem(client,"feet"),GetCharItemId(client,"feet"));
    Format(buffer,sizeof(buffer),"%s, hands_cat = '%s', hands_item = '%s'",buffer,GetCharCatItem(client,"hands"),GetCharItemId(client,"hands"));
    Format(buffer,sizeof(buffer),"%s, helmets_cat = '%s', helmets_item = '%s'",buffer,GetCharCatItem(client,"helmets"),GetCharItemId(client,"helmets"));
    Format(buffer,sizeof(buffer),"%s, legs_cat = '%s', legs_item = '%s'",buffer,GetCharCatItem(client,"legs"),GetCharItemId(client,"legs"));
    Format(buffer,sizeof(buffer),"%s, necks_cat = '%s', necks_item = '%s'",buffer,GetCharCatItem(client,"necks"),GetCharItemId(client,"necks"));
    Format(buffer,sizeof(buffer),"%s, rings_cat = '%s', rings_item = '%s'",buffer,GetCharCatItem(client,"rings"),GetCharItemId(client,"rings"));
    Format(buffer,sizeof(buffer),"%s, shields_cat = '%s', shields_item = '%s'",buffer,GetCharCatItem(client,"shields"),GetCharItemId(client,"shields"));
    Format(buffer,sizeof(buffer),"%s, rightweapons_cat = '%s', rightweapons_item = '%s'",buffer,GetCharCatItem(client,"rightweapons"),GetCharItemId(client,"rightweapons"));
    Format(buffer,sizeof(buffer),"%s, leftweapons_cat = '%s', leftweapons_item = '%s'",buffer,GetCharCatItem(client,"leftweapons"),GetCharItemId(client,"leftweapons"));
    Format(buffer,sizeof(buffer),"%s, wrists_cat = '%s', wrists_item = '%s'",buffer,GetCharCatItem(client,"wrists"),GetCharItemId(client,"wrists"));
    decl String:query[8050];
    Format(query,sizeof(query),"UPDATE wcusers SET %s WHERE ID = '%s'",buffer,uniqueid);
    SQL_FastQuery(hSQL,query);
*/

public GetPoints(client)
{
   return wcUsers[GetPlayerID(client)][2];
}

public wcSetPoints(client,amount)
{
  if (amount<0) amount=0;
  wcUsers[GetPlayerID(client)][2]=amount;
}

public GetStr(client)
{
   return wcUsers[GetPlayerID(client)][7];
}

public GetAgi(client)
{
   return wcUsers[GetPlayerID(client)][8];
}

public GetSta(client)
{
   return wcUsers[GetPlayerID(client)][241];
}

public GetInt(client)
{
   return wcUsers[GetPlayerID(client)][9];
}

public GetMoney(client)
{
   return wcUsers[GetPlayerID(client)][6];
}

public GetXp(client)
{
  return wcUsers[GetPlayerID(client)][4];
}

public GetReqXp(client)
{
  return wcUsers[GetPlayerID(client)][5];
}

public GetClass(client)
{
  return wcUsers[GetPlayerID(client)][240];
}

public GetLevel(client)
{
  return wcUsers[GetPlayerID(client)][3];
}

public wcSetCharItem(client,category,item)
{

   if ((category>9)&&(category<=24))
   {
      wcUsers[GetPlayerID(client)][category]=item;
      return true;
   }
   return false;
}

public wcSetProfession(client,which,amount)
{
  if(which>2 || which<1)
    LogError("[WC] Wrong profession ID, %i",which);
  else
    wcUsers[GetPlayerID(client)][242+which]=amount;
}

public GetProfession(client,which)
{
  if(which>2 || which<1)
    LogError("[WC] Wrong profession ID, %i",which);
  else
    return wcUsers[GetPlayerID(client)][242+which];
  return 0;
}

public wcSetProfessionSk(client,which,amount)
{
  if(which>2 || which<1)
    LogError("[WC] Wrong profession ID, %i",which);
  else  {
    //V2 we want only skill being 'maximal' in the profesison levels range
    new amount2;
    if(GetProfession(client,which) > 0 && isCraftingProfession(GetProfession(client,which))) {
      amount2 = GetProfessionMaximalSkill(GetProfession(client,which), amount);
      if(amount2 > 0) amount = amount2;
    }
    wcUsers[GetPlayerID(client)][244+which]=amount;
  }
}

public GetProfessionSk(client,which)
{
  if(which>2 || which<1)
    LogError("[WC] Wrong profession ID, %i",which);
  else
    return wcUsers[GetPlayerID(client)][244+which];
  return 0;
}

public SetBoss(client,amount)
{
  wcUsers[client][242]=amount;
}

public GetBoss(client)
{
  return wcUsers[client][242];
}

public PutIntoInventory(client,category)
{
  new item[UniqueItem];
  GetCharItem(item, client,category);
  new free=wcCheckFreeSlot(client);
  wcSetStaticSlot(client,free,item[Item_defId]);
  wcSetStaticSlotAmount(client,free,-1);
  if(category<30) {
    wcSetCharItem(client,category,0);
    SetCharUnique(client, category, 0);
  }
  else  {
    SetGlyphId(client, category-30, 0);
  }
  SetInvUnique(client, free, item[Item_id]);
}

public wcSetStaticSlotAmount(client,slot,amount)
{
  if(amount<1000)
   wcUsers[GetPlayerID(client)][139+slot]=amount;
  else
    LogError("[WC] Error: tried to bug items amount: slot = %d amount = %d charId = %d", slot, amount, characterids[client][0]);
}

public wcSetStaticSlot(client,slot,item)
{
   wcUsers[GetPlayerID(client)][38+slot]=item;
}

public GetCharItemId(client,category)
{
  if(category<30)
    return wcUsers[GetPlayerID(client)][category];
  else
    return GetGlyphId(client, category-30);
}

public MergeItem(client,slot)
{
  new item=GetSlotItem(client,slot);
  new amount=GetSlotItemAmount(client,slot);
  new max=GetItemMaxAmount(item);
  if(amount<=0 || max==1)
    WCMessage(client,"can't merge",client);
  else
  {
  for (new x=1;x<=100;x++)
  if(x!=slot)
  {
    amount=GetSlotItemAmount(client,slot);
    if(amount>=max)
      break;
    new item2=GetSlotItem(client,x);
    if(item2==item)
    {
      new amount2=GetSlotItemAmount(client,x);
      new delta=max-amount;
      new bool:remove=true;
      if(delta>=amount2)
        delta=amount2;
      else
        remove=false;
      wcSetStaticSlotAmount(client,slot,amount+delta);
      wcSetStaticSlotAmount(client,x,amount2-delta);
      if(remove)
      {
        wcSetStaticSlot(client,x,0);
        SetInvUnique(client, x, 0);
      }
    }
  }
  }
}

public ItemSold(client,slot,amount)
{
  wcSetMoney(client,(GetMoney(client)+amount));
  wcSetStaticSlot(client,slot,0);
  wcSetStaticSlotAmount(client,slot,0);
  new uniqueid = GetInvUnique(client, slot);
  SetInvUnique(client, slot, 0);
  if(uniqueid>0)  {
    UpdateUniqueState(uniqueid, 2);
  }
  WCMessage(client,"item sold",client,amount);
}

public AllItemsSold(client, itemId, amount) {
  wcSetMoney(client,(GetMoney(client)+amount));
  for(new slot=1; slot <= 100; slot++) {
    new id = GetSlotItem(client, slot);
    if (id == itemId) {
      wcSetStaticSlot(client,slot,0);
      wcSetStaticSlotAmount(client,slot,0);
      new uniqueid = GetInvUnique(client, slot);
      SetInvUnique(client, slot, 0);
      if(uniqueid>0)  {
        UpdateUniqueState(uniqueid, 2);
      }

    }
  }
  WCMessage(client,"item sold",client,amount);

}

public GetGlyphOn(client,slot)
{
  /*if(IsPlayerAlive(client))*/
  /*{*/
    /*ForcePlayerSuicide(client);*/
    /*WCMessage(client,"can not put items while alive",client);*/
  /*}*/
  if(!HasExpansion(client, Expansion:E_CATA)) {
    WCMessage(client, "cata expansion required", client);
    return;
  }
  new item[UniqueItem];
  GetInvItem(item, client, slot);
  if(wcCheckStats(client,item)==1)
  {
    ChooseGlyphSlot(client, slot, item);
  }
  else
  {
   if(wcCheckStats(client,item)==0)
    WCMessage(client,"nostats",client);
   else if(wcCheckStats(client,item)==2)
    WCMessage(client,"wrong class",client);
   else if(wcCheckStats(client,item)==3)
    WCMessage(client,"low lvl",client);
  }
}



public RefreshInventory(client)
{
  new slot1,slot2;
  while ((slot1 = wcCheckFreeSlot(client,true)) < (slot2 = wcCheckFilledSlot(client)))
    SwapSlots(client,slot1,slot2);
}

public SwapSlots(client,slot1,slot2)
{
  new item=GetSlotItem(client,slot1);
  new amount=GetSlotItemAmount(client,slot1);
  new uni=GetInvUnique(client, slot1);
  wcSetStaticSlot(client,slot1,GetSlotItem(client,slot2));
  wcSetStaticSlotAmount(client,slot1,GetSlotItemAmount(client,slot2));
  SetInvUnique(client, slot1, GetInvUnique(client, slot2));
  wcSetStaticSlot(client,slot2,item);
  wcSetStaticSlotAmount(client,slot2,amount);
  SetInvUnique(client, slot2, uni);
}

public wcCheckFilledSlot(client)
{
  for (new x=100;x>=1;x--)
  {
    if(GetSlotItem(client,x)!=0)
      return x;
  }
  return 0;
}

public GetItemCat(item, unique)
{
  new uitem[UniqueItem];
  GetItemProperties(uitem, item, unique);
  return uitem[Item_category];
}

stock GetItemQuality(item)
{
   decl String:key[10];
   IntToString(item,key,sizeof(key));
   KvRewind(items);
   if(item>0)
   {
   if(KvJumpToKey(items,key))
   {
      return KvGetNum(items,"noshop",0);
   }
   else
   {
      LogError("WC Error: could not find item %i",item);
   }
   }
   return 0;
}


public GetItemMaxAmount(item)
{
  new uitem[UniqueItem];
  GetItemProperties(uitem, item);
  return uitem[Item_amount];
}

stock bool:ItemUsable(item)
{
   decl String:key[10];
   IntToString(item,key,sizeof(key));
   KvRewind(items);
   if(item>0)
   {
   if(KvJumpToKey(items,key))
   {
      new usable=KvGetNum(items,"use");
      if(usable>0)
        return true;
   }
   else
   {
      LogError("WC Error: could not find item %i",item);
   }
   }
   return false;
}

public GetSlotCat(client,ID)
{
  new item[UniqueItem];
  GetInvItem(item, client, ID);
  return item[Item_category];
}

public GetSlotItemAmount(client,ID)
{
  new ret=wcUsers[GetPlayerID(client)][139+ID];
  return ret;
}

public GetSlotItem(client,ID)
{
   return wcUsers[GetPlayerID(client)][38+ID];
}

public wcSetReqXp(client,amount)
{
  if(amount<EXP_START) amount=EXP_START;
  wcUsers[GetPlayerID(client)][5]=amount;
}

public wcSetLevel(client,amount)
{
  //now works with negative
   new level=GetLevel(client);
   if(amount<1) amount=1;
   wcUsers[GetPlayerID(client)][3]=amount;
   new delta=amount-level;
   new money=0;
   new points = 0;
   new pLevel = level;
   new talentPoints=0;
   if(delta>0)  {
    if(amount >= 30)  {
      if(!CSGO && !IsFakeClient(client) && !SawSlides(client, Slide:SLIDE_30))
         PrerunSlide(client, Slide:SLIDE_30);

    }
    while (pLevel < amount) {
      pLevel++;
      LevelUpHook(client, pLevel);
      if ((pLevel % 4) == 0)
         points++;
      if(pLevel <= 30)
         points++;
      points++;
      if(getsTalentPoint(pLevel))
        talentPoints++;
    }
  }
   else  {
    while (amount < pLevel) {
        pLevel--;
        if ((pLevel % 4) == 3)
           points++;
        if (pLevel < 30)
          points++;
        points++;
    }
   }

   if(talentPoints>0) {
    setTPoints(client, 0, talentPoints + getTPoints(client, 0));
    setTPoints(client, 1, talentPoints + getTPoints(client, 1));
   }

   if (delta > 0)
   {
    wcSetPoints(client,GetPoints(client)+points);
    spreadPoints(client);

   }
   else {
    ResetPoints(client);
    wcSetPoints(client,GetPoints(client)-points);
    spreadPoints(client);
    //wcResetTalents(client);
    //setTPoints(client,tpoints); tpoints are not calculated if level is reducing. - TODO low priority
   }


   while (delta > 0)
   {
      level++;
      new d2 = RoundToFloor(float(level>80?80:level) / 4.0);
      money=RoundToNearest(float(money)+Pow(1.4,float(d2))*3.8);
      delta--;
   }
   while(delta < 0)  {
      new d2 = RoundToFloor(float(level>80?80:level) / 4.0);
      money=RoundToNearest(float(money)-Pow(1.4,float(d2))*3.8);
      level--;
      delta++;

   }
   if(IsFakeClient(client))
   {
    while(GetPoints(client)>0)
    {
      new x=GetRandomInt(2,4);
      switch(x)
      {
        case 2:
        {
          wcSetStr(client,GetStr(client)+2);
          wcSetPoints(client,GetPoints(client)-1);
        }
        case 3:
        {
          wcSetAgi(client,GetAgi(client)+2);
          wcSetPoints(client,GetPoints(client)-1);
        }
        case 4:
        {
          wcSetSta(client,GetSta(client)+2);
          wcSetPoints(client,GetPoints(client)-1);
        }
      }
    }
   }
   wcSetMoney(client,GetMoney(client)+money);
   wcSetReqXp(client,_getReqXp(amount)); //79 lvl - 261780    2689 gold

   RemoveLevelSpells(client);
   defineIsPlayerNew(client); //Check if he can still hope for being 'new'
   dingPanel(client); //Give info about what he achieved if anything.

   // 79 lvl ratio gold/exp 0.01
   //80 lvl - 413614       3765 gold
   // 80 lvl ratio gold/exp 0.009
}
// Determining how much XP per level
stock _getReqXp(amount)  {		// New Method that is SIMILIAR to WoW
  new d = RoundToFloor(float(amount)/10.0);
  new d2 = amount;
  new Float:d3 = 0.0;
  switch(d)   {
	case 0:
	d3 = 40 * Pow(float(d2),2.0) + 360 * d2;
    case 1:
    d3 = -0.4 * (Pow(float(d2),3.0))) + (40.4 * (Pow(float(d2),2.0))) + (396 * d2);
    case 2:
    d3 = -0.4 * (Pow(float(d2),3.0))) + (40.4 * (Pow(float(d2),2.0))) + (396 * d2);
    case 3:
    d3 = (65 * (Pow(float(d2),2.0)) - (165 * d2) - 6750) * 0.82;
    case 4:
    d3 = (65 * (Pow(float(d2),2.0)) - (165 * d2) - 6750) * 0.82;
    case 5:
    d3 = (65 * (Pow(float(d2),2.0)) - (165 * d2) - 6750) * 0.82;
    case 6:
    d3 = (155 + (45 + (5 * d2))) * (1344 - ((69-60) * (3 + (69-60) * 4)));
  }
  //amount = Roun
  //2nd version. 1st had 1.58 power, 220.0 coef
//  new Float:reqxp=Pow(1.49,float(d2));
//  if(amount >= 80) {
//    reqxp *= 1.25;
//  }
  new d4 = RoundToNearest(d3);
//  new Float:remove = d4 % 100.0; // remove becomes 70.4
//  d4 -= remove;
  return (d4 -= d4 % 100);
}

//stock _getReqXp(amount)  {
//  new d = amount % 4;
//  new d2 = RoundToFloor(float(amount)/4.0);
//  new Float:modifier = 0.2;
//  switch(d)   {
//    case 1:
//    modifier = 0.22;
//    case 2:
//    modifier = 0.28;
//    case 3:
//    modifier = 0.3;
//  }
  //amount = Roun
  //2nd version. 1st had 1.58 power, 220.0 coef
//  new Float:reqxp=Pow(1.49,float(d2));
//  if(amount >= 80) {
//    reqxp *= 1.25;
//  }
//  return RoundToCeil(500.0*reqxp*modifier);
//}

stock RemoveLevelSpells(client) {
  if(GetClass(client)<=0)
    return;
  RemoveSpells(client, GetSpellsHandle(GetClass(client)));
}

stock RemoveSpells(client,Handle:SpellsDB)
{
  decl String:buffer[255];

  new level=GetLevel(client);
  KvRewind(SpellsDB);
  if (!KvGotoFirstSubKey(SpellsDB))
  {
    LogError("[WC] Error: failed to open Spells database");
  }
  do
  {
    KvGetSectionName(SpellsDB, buffer, sizeof(buffer));
    new spellid=StringToInt(buffer);
    new ilevel=KvGetNum(SpellsDB, "level", 0);
    if(ilevel>level)  {
      RemoveSpellIfHas(client, spellid);
    }
   }  while (KvGotoNextKey(SpellsDB));
}

stock RemoveSpellIfHas(client, spellid) {
  new foundspell=wcSearchSpell(client,spellid);
  if(foundspell==0)
    return;
  wcSetStaticSpell(client,foundspell,0);
  for (new i=0;i<=6;i++)  {
    new id=wcGetSpellBarSpell(client,i);
    if(id==spellid)
      wcSetSpellBarSpell(client,i,0);
  }
}

wcCheckStats(client,item[UniqueItem])
{
   new bool:pass=false;
   new classes[10];
   new amount=ConvertBitToClasses(item[Item_class], classes);
   new race=GetClass(client);
   for(new x=0;x<amount;x++)
   {
    if(classes[x]==race)
      pass=true;
   }
   if(classes[0]==0)
    pass=true;
   if(pass==false)
    return 2;
   //end of checking for class
   new lev=item[Item_level];
   new category=item[Item_category];
   if((category>0) && (category<5))
    lev-=RECIPE_LVL_MINUS;
   if(category==8)
      lev=0;
   if(lev>GetLevel(client))
    return 3;
   /*
   if(item[Item_reqstr]>GetStr(client))
    return 0;
   if(item[Item_reqagi]>GetAgi(client))
    return 0;
   if(item[Item_reqint]>GetInt(client))
    return 0;
   if(item[Item_reqsta]>GetSta(client))
    return 0;
   */
   return 1;
}

public wcCheckLevel(client,item)
{
   KvRewind(items);
   decl String:key[10];
   IntToString(item,key,sizeof(key));
   KvJumpToKey(items,key);
   new Float:ver = KvGetFloat(items,"version",0.96);
   new lMulti = 1;
   if (ver <= 0.96)
      lMulti = 4;
   if((KvGetNum(items, "level", -1)*lMulti)>GetLevel(client))
    return false;
   return true;
}

stock wcCheckFreeSlot2(client,item2, bool:refresher=false)
{
  for (new x=1;x<=100;x++)
  {
    new item=GetSlotItem(client,x);
    if(item==0)
    {
      wcSetStaticSlotAmount(client,x,0);
      SetInvUnique(client, x, 0);
      return x;
    }
    if(item==item2)
    {
      new max=GetItemMaxAmount(item);
      new amount=GetSlotItemAmount(client,x);
      if((amount<max) && (amount>0))
        return x;
    }
  }
  if(refresher)
    return 101;
  return 0;
}

public wcCheckFreeSlot3(client,uitem[UniqueItem], &space) //A little smarter again :D
{

  if(uitem[Item_amount]<=1) {
    space=1;
    return wcCheckFreeSlot(client);
  }
  for (new x=1;x<=100;x++)
  {
    new item=GetSlotItem(client,x);
    if(item==uitem[Item_defId])
    {
      new amount=GetSlotItemAmount(client,x);
      if(amount<uitem[Item_amount]) {
        space=uitem[Item_amount]-amount;
        return x;
      }
    }
  }
  space=uitem[Item_amount];
  return wcCheckFreeSlot(client);
}

stock wcCheckFreeSlot(client, bool:refresher=false)
{
  for (new x=1;x<=100;x++)
  {
    if(GetSlotItem(client,x)==0)
    {
      wcSetStaticSlotAmount(client,x,0);
      SetInvUnique(client, x, 0);
      return x;
    }
  }
  if(refresher)
    return 101;
  return 0;
}

public GetFreeProfession(client,id)
{
  for (new x=1;x<=400;x++)
  {
    if(GetClientProfessionSlot(client,id,x)==0)
      return x;
  }
  return 0;
}

public GetFilledInventory(client)
{
  new amount=0;
  for (new x=1;x<=100;x++)
  {
    if(GetSlotItem(client,x)==0)
      amount++;
  }
  amount=100-amount;
  return amount;
}

PurchaseItemConfirmed(client,item, amount=1)
{
  new uitem[UniqueItem];
  GetItemProperties(uitem, item);
  new slot = GiveItem(client, uitem, amount, "shop");

  if((!FUN_SERVER) && (uitem[Item_gold]<=0))
    wcSetMoney(client,(GetMoney(client)-uitem[Item_cost]*amount));
  if( (uitem[Item_honor]>0) && (!FUN_SERVER))  {
   SetHonor(client,GetHonor(client)-uitem[Item_honor]*amount);
  }
  if( uitem[Item_valor] > 0)  {
    SpendValor(client,uitem[Item_valor]);
  }
  if( uitem[Item_justice] > 0)  {
    SpendJustice(client,uitem[Item_justice]);
  }

  decl String:msg[200];
  Format(msg, sizeof(msg), "Bought item id %d name %s from gold shop", item, uitem[Item_Name]);
  SpendGold(client, uitem[Item_gold], "shop", msg);
  decl String:category[255];
  IdToCategory(uitem[Item_category],category);
  Format(category,sizeof(category),"%T",category,client);
  WCMessage(client,"Item bought",client,uitem[Item_Name],category);
  if(getAutoEquip(client) && slot >= 0)  {
    UseItem(client, slot, true);

  }
}

public bool:CheckIfItem(item)
{
  KvRewind(items);
  new String:key[10];
  IntToString(item,key,sizeof(key));
  return KvJumpToKey(items,key);
}

stock GetItemName(item,String:back[255], unique=0)
{
  if(unique > 0)  {
    new uitem[UniqueItem];
    GetItemProperties(uitem, item, unique);
  }
  else  {
    KvRewind(items);
    new String:key[10];
    IntToString(item,key,sizeof(key));
    KvJumpToKey(items,key);
    KvGetString(items, "name", back,sizeof(back),"empty");
  }
}

public GetItemLevel(item)
{
  KvRewind(items);
  new String:key[10];
  IntToString(item,key,sizeof(key));
  KvJumpToKey(items,key);
  new Float:ver=KvGetFloat(items,"version",0.96);
  new multi = 1;
  if (ver<=0.96)
      multi = 4;
  return KvGetNum(items, "level",0)*multi;
}

public GetItemCost(item)
{
  KvRewind(items);
  new String:key[10];
  IntToString(item,key,sizeof(key));
  KvJumpToKey(items,key);
  return KvGetNum(items, "cost",0);
}

public GetCategoryName(item,String:back[255], client)
{
  new String:key[10];
  IntToString(item,key,sizeof(key));
  KvJumpToKey(items,key);
  IdToCategory(KvGetNum(items, "category",-1),back);
  Format(back,sizeof(back),"%T",back,client);
}

public TranslateCategory(id, String:back[255], client)  {
  IdToCategory(id,back);
  Format(back,sizeof(back),"%T",back,client);
}

public IdToCategory(id,String:back[255])
{
   back="no category";
   switch(id)
   {
      case 8:
        back="reagents";
      case 1:
        back="prof_1";
      case 2:
        back="prof_2";
      case 3:
        back="prof_3";
      case 4:
        back="prof_4";
      case 5:
        back="prof_8";
      case 6:
        back="prof_6";
      case 7:
        back="prof_7";
      case 9:
        back="prof_9";
      case 10:
         back="backs";
      case 11:
         back="chests";
      case 12:
         back="feet";
      case 13:
         back="hands";
      case 14:
         back="helmets";
      case 15:
         back="legs";
      case 16:
         back="necks";
      case 17:
         back="rings";
      case 18:
         back="shields";
      case 19:
         back="shoulders";
      case 20:
         back="trinkets";
      case 21:
         back="waists";
      case 22:
         back="weapons";
      case 23:
         back="wrists";
      case 25:
         back="scroll";
      case 26:
        back="Box";
      case 27:
        back="Special";
      case 30:
        back="Glyph";
      case 31:
        back="Glyph";
      case 32:
        back="Glyph";
      case 33:
        back="Glyph";
   }

}

public wcSetClass(client,amount)
{
  if (amount<0) amount=0;
  wcUsers[GetPlayerID(client)][240]=amount;
}

public wcSetStaticLevel(client,amount)
{
  if(amount<0) amount=0;
  wcUsers[GetPlayerID(client)][3]=amount;
}

public wcSetMoney(client,amount)
{
  if(amount<0) amount=0;
  wcUsers[GetPlayerID(client)][6]=amount;
}

public wcSetStr(client,amount)
{
  if (amount<0) amount=0;
  wcUsers[GetPlayerID(client)][7]=amount;
}

public wcSetAgi(client,amount)
{
  if (amount<0) amount=0;
  wcUsers[GetPlayerID(client)][8]=amount;
}

public wcSetInt(client,amount)
{
  if (amount<0) amount=0;
  wcUsers[GetPlayerID(client)][9]=amount;
}

public wcSetSta(client,amount)
{
  if (amount<0) amount=0;
  wcUsers[GetPlayerID(client)][241]=amount;
}

public FindMatchingHealer(team,clients[],ignore)
{
   new count=0;
   new maxplayers=GetMaxClients();
   for(new x=1;x<=maxplayers;x++)
   {
      if(IsClientInGame(x))   if(IsPlayerAlive(x))   if(  /*  IsHealer(x)  && */  (GetClientTeam(x)==team) && (x != ignore))
      {
        clients[count]=x;
        count++;
      }
   }
   return count;
}

stock bool:IsHealer(client)  {
   new race=GetClass(client);
   if((race==4) || (race==5) || (race==6))
      return true;
   return false;
}

public FindMatchingPlayers(const String:matchstr[],clients[])
{
  new count=0;
  new maxplayers=GetMaxClients();
  if(StrEqual(matchstr,"@all",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x))
      {
        clients[count]=x;
        count++;
      }
    }
  }
  else if(StrEqual(matchstr,"@alive",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x) && IsPlayerAlive(x))
      {
        clients[count]=x;
        count++;
      }
    }
  }
  else if(StrEqual(matchstr,"@t",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x)&&GetClientTeam(x)==2)
      {
        clients[count]=x;
        count++;
      }
    }
  }
  else if(StrEqual(matchstr,"@ct",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x)&&GetClientTeam(x)==3)
      {
        clients[count]=x;
        count++;
      }
    }
  }
  else if((matchstr[0]=='@') || (matchstr[0]=='#'))
  {
    new userid=StringToInt(matchstr[1]);
    if(userid)
    {
      new index=GetClientOfUserId(userid);
      if(index)
      {
        if(IsClientInGame(index))
        {
          clients[count]=index;
          count++;
        }
      }
    }
  }
  else
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x))
      {
        decl String:name[64];
        GetClientName(x,name,64);
        if(StrContains(name,matchstr,false)!=-1)
        {
          clients[count]=x;
          count++;
        }
      }
    }
  }
  return count;
}

public FindMatchingAlive(const String:matchstr[],clients[])
{
  new count=0;
  new maxplayers=GetMaxClients();
  if(StrEqual(matchstr,"@all",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x))   if(IsPlayerAlive(x))
      {
        clients[count]=x;
        count++;
      }
    }
  }
  else if(StrEqual(matchstr,"@t",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x)&&GetClientTeam(x)==2)   if(IsPlayerAlive(x))
      {
        clients[count]=x;
        count++;
      }
    }
  }
  else if(StrEqual(matchstr,"@ct",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x)&&GetClientTeam(x)==3)   if(IsPlayerAlive(x))
      {
        clients[count]=x;
        count++;
      }
    }
  }
  else if((matchstr[0]=='@') || (matchstr[0]=='#'))
  {
    new userid=StringToInt(matchstr[1]);
    if(userid)
    {
      new index=GetClientOfUserId(userid);
      if(index)
      {
        if(IsClientInGame(index))   if(IsPlayerAlive(index))
        {
          clients[count]=index;
          count++;
        }
      }
    }
  }
  else
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x))   if(IsPlayerAlive(x))
      {
        decl String:name[64];
        GetClientName(x,name,64);
        if(StrContains(name,matchstr,false)!=-1)
        {
          clients[count]=x;
          count++;
        }
      }
    }
  }
  return count;
}

public CountHumans()
{
  new total=0;
  new maxplayers=GetMaxClients();
  for(new x=1;x<=maxplayers;x++)
  {
    if(IsClientInGame(x))
    {
      if(GetClientTeam(x)==2||GetClientTeam(x)==3)
      {
        if(!IsFakeClient(x))
          total++;
      }
    }
  }
  return total;
}

stock wcClassToName(client,id,String:back[],size, transClient=0)
{
  Format(back,size,"class_%i",id);
  if(transClient <= 0)  transClient = client;
  Format(back,size,"%T",back,transClient);
}

stock Float:GetDESkillUp(enchanting, item[UniqueItem]) {
  //v1 formula gives 3% for 80 lvl rare at skill 1, which is very wrong... let's construct soething more realistic
  //target: 80 lvl rare - 80% at level 1. -> 0% at iQua condition
  //I think best solution is bind chance to item rating, hence we also include the 'category'
  //abstract: chance*normalization = rating * iQua^(y) - c1 * enchanting
  //y = 1.7
  //c1 = 1.0
  //normalization = 200

  new Float:rating = GetItemRating(item);
  new Float:chance = rating * Pow(float(item[Item_quality]+1), 1.7) - 1.0 * float(enchanting);
  chance = chance/200.0;
  if(chance< 0.0) chance = 0.0;
  if(chance > 1.0) chance = 1.0;

  if(enchanting > (item[Item_quality] +2) * 75) chance = 0.0;
  if(enchanting >= 400) chance = 0.0;

  return chance;
}

stock TriggerDisEnch(client, item[UniqueItem], slot)
{
  new enchanting;
  new playerProfs[2][2];  // first index  - prof index, 2nd index:
                          // 0 - profession id
                          // 1 - profession skill
  for(new i=0;i<2;i++)  {
    playerProfs[i][0]=GetProfession(client,i+1);
    playerProfs[i][1]=GetProfessionSk(client,i+1);
    if(playerProfs[i][0]==8)
    {
      enchanting=playerProfs[i][1];
    }
  }
  if(!((playerProfs[0][0] == 8) || (playerProfs[1][0] == 8) ))
  {
    WCMessage(client,"need ench",client);
    return;
  }

  if ((100-GetFilledInventory(client))<8)
  {
    WCMessage(client, "no free slots", client);
    return;
  }

  RemoveInvItem(client, slot, 4);

  new level=item[Item_level];
  KvRewind(wcdrop);
  new iQua = item[Item_quality];
  if(iQua>4)
  {
    if(iQua==5)
      iQua=2; //make PVP-R equal to rare
    else
      iQua=3; //make PVP-E equal to epic
  }


  if (!KvGotoFirstSubKey(wcdrop))
  {
    LogError("[wow] Error: failed to open drop database");
  }
  new minlevel;
  new maxlevel;
  new prof;
  new tier;

  /*new Float:skillupChance = GetDESkillUp(enchanting, item);*/

  /*if(skillupChance > GetRandomFloat(0.0,1.0)) {*/
    /*InitProfBonuses(client);*/
    /*new String:profname[60];*/
    /*ProfToName(8,profname,sizeof(profname),client);*/
    /*WCMessage(client,"increased prof skill",client,profname);*/

    /*if(GetProfession(client,2)==8)  [> Intended to start from slot 2 to avoid ppl picking 2 enchanting professions in both slots <]*/
    /*{*/
      /*wcSetProfessionSk(client,2,GetProfessionSk(client,2)+1);*/
    /*}*/
    /*else*/
    /*{*/
      /*wcSetProfessionSk(client,1,GetProfessionSk(client,1)+1);*/
    /*}*/
  /*}*/


  do
  {
    prof=KvGetNum(wcdrop,"profession");
    if(prof==8 )
    {
      tier = KvGetNum(wcdrop,"tier");
      if(iQua==0)
      {
        minlevel=KvGetNum(wcdrop,"cMin");
        maxlevel=KvGetNum(wcdrop,"cMax");
      }//Common
      if(iQua==1)
      {
        minlevel=KvGetNum(wcdrop,"uMin");
        maxlevel=KvGetNum(wcdrop,"uMax");
      }//Uncommon
      if(iQua==2)
      {
        minlevel=KvGetNum(wcdrop,"rMin");
        maxlevel=KvGetNum(wcdrop,"rMax");
      }//Rare
      if(iQua==3)
      {
        minlevel=KvGetNum(wcdrop,"eMin");
        maxlevel=KvGetNum(wcdrop,"eMax");
      }//Epic
      if(iQua==4)
      {
        minlevel=KvGetNum(wcdrop,"lMin");
        maxlevel=KvGetNum(wcdrop,"lMax");
      }//Legendary
      new failId = KvGetNum(wcdrop,"failId");
      if ( (minlevel<=level) && (maxlevel>=level))
      {
        new Float:L2=level*1.0;
        new Float:IQ=iQua*1.0;
        new Float:TI=tier*1.0;
        new Float:EN1=enchanting*1.0;
        new Float:EN2=EN1/3000;
        new give=0;
        new failGive=0;
        new Float:primaryChance = (((L2/7)*(IQ+2))*((10-TI)/10)+(16-(TI*2)))/100;
        new Float:amount=IQ+((10-TI)+(10*IQ))/TI;
        new Float:secondaryChance=(0.20+EN2);
        for(new j=0;j<amount;j++)
        {
          if(primaryChance>=GetRandomFloat(0.0,1.0))
            give++;
          else
          {
            //You MUST have mistakes to learn something from disenchanting,
            //also helps reduce the chance as they get higher.

            if(failId > 0)
            {
              if(secondaryChance>=GetRandomFloat(0.0,1.0))
              {
                failGive++;//lucky
                if((secondaryChance/2)>=GetRandomFloat(0.0,1.0))
                {  failGive++;//doubly lucky
                }
              }
            }
          }
        }
        if(give>0)
        {
          new id=KvGetNum(wcdrop,"id",0);
          if(id<=0)
          {
            new String:set[500];
            KvGetString(wcdrop,"set",set,sizeof(set),"0");
            id = GetSetsRandomItem(set);
          }
          if(id>0)
          {
            new uitem[UniqueItem];
            GetItemProperties(uitem, id, 0);
            new islot = GiveItem(client, uitem, give, "disenchant");
            if(islot >= 0)
            {
              new an = KvGetNum(wcdrop,"announce",0);
              switch(an)
              {
                case 1: AnnounceDrop(client,uitem[Item_Name], islot);
                case 2: AnnounceDropAll(client, uitem[Item_Name], uitem[Item_quality]);
              }
              WCMessage(client,"item dropped2",client,uitem[Item_Name], give);
            }
          }
          else
            LogError("[WC] Could not find drop ID");
        }
        //Secondary DE Drops
        if(failGive>0)
        {
          new uitem[UniqueItem];
          GetItemProperties(uitem, failId, 0);
          new islot = GiveItem(client, uitem, failGive, "disenchant");
          if(islot >= 0)
          {
            new an = KvGetNum(wcdrop,"announce",0);
            switch(an)
            {
              case 1: AnnounceDrop(client,uitem[Item_Name], islot);
              case 2: AnnounceDropAll(client, uitem[Item_Name], uitem[Item_quality]);
            }
            WCMessage(client,"item dropped2",client,uitem[Item_Name], failGive);
          }
          else
          {
            LogError("[WC] Could not find failDrop ID");
          }
        }
      }
    }
  }
  while(KvGotoNextKey(wcdrop));
}

stock TriggerDrop(client,victim, bool:self=false, Float:custom_rate=1.0)
{
  new team1=GetClientTeam(client);
  new team2=GetClientTeam(victim);
  if (((team1!=team2) && ( (IsFakeClient(victim) && BOT_DROP) || (!IsFakeClient(victim)) ) ) || (self) )
  {
    new level=GetLevel(victim);
    KvRewind(wcdrop);
    if (!KvGotoFirstSubKey(wcdrop))
      {
        LogError("[WC] Error: failed to open drop database");
      }
    do
    {
      new minlevel=KvGetNum(wcdrop,"minlevel");

      if(KvGetNum(wcdrop,"autoauctiononly", 0)==1) //drop marked for auto-auction only in wcdrop.txt
        continue;
      new prof=KvGetNum(wcdrop,"profession");
      if(prof==8) //Enchanting drop
        continue;
      new prof2=GetProfession(client,1);
      new skill=GetProfessionSk(client,1);
      new which=1;
      if (prof2!=prof)
      {
        which=2;
        prof2=GetProfession(client,2);
        skill=GetProfessionSk(client,2);
      }
      new minskill=KvGetNum(wcdrop,"minskill",0);
      new skillup=KvGetNum(wcdrop,"skillup",0);
      if((minlevel<=level) && (((prof==prof2) && (minskill<=skill))|| (prof==0)))
      {
        new Float:inc=KvGetFloat(wcdrop,"incriment",0.0);
        new Float:min=KvGetFloat(wcdrop,"minimum",0.01);
        new Float:max=KvGetFloat(wcdrop,"maximum",min);
        new Float:chance=min+inc*float(level-minlevel);
        if(IsFakeClient(victim))
        {
          switch(SERVER_LEVEL_POLICY)
          {
            case 0: chance *= BOT_NORMAL_RATE;
            case 1: chance *= BOT_HIGH_RATE;
            case 2: chance *= BOT_NORMAL_RATE;
          }
        }
        chance *= custom_rate*GLOBAL_DROP*GetTempVar(client, Temp_DropRate)*(PlayerHasBonusFast(client, Profession:Prof_ExtraDrop)?1.1:1.0);
        if(isClientPDModule(client, PDModule:PDM_drop))
          chance *= 1.25;
        if(!self)
           chance*= 1.0 + EXP_SPREE*float(g_Deaths[client]);
        if(chance>max)
          chance=max;
        if(chance>=GetRandomFloat(0.0,1.0))
        {
          if(prof!=0)
          {
            if(skill<=skillup)
            {
              wcSetProfessionSk(client,which,GetProfessionSk(client,which)+1);

              InitProfBonuses(client);
              new String:profname[60];
              ProfToName(prof,profname,sizeof(profname),client);
              WCMessage(client,"increased prof skill",client,profname);
            }
          }
          new id=KvGetNum(wcdrop,"id",0);
          if(id<=0)
          {
            new String:set[500];
            KvGetString(wcdrop,"set",set,sizeof(set),"0");
            id = GetSetsRandomItem(set);
          }
          if(id>0)
          {
            new uitem[UniqueItem];
            GetItemProperties(uitem, id, 0);

            new slot = GiveItem(client, uitem, 1, "drop");
            if(slot < 0)
              WCMessage(client,"failed drop",client,uitem[Item_Name]);
            else
            {
              WCMessage(client,"item dropped",client,uitem[Item_Name]);
              new an = KvGetNum(wcdrop,"announce",0);
              switch(an)
              {
                case 1: AnnounceDrop(client,uitem[Item_Name], slot);
                case 2:
                {
                  AnnounceDropAll(client, uitem[Item_Name], uitem[Item_quality]);
                }
              }
            }
          }
          else
            LogError("[WC] Could not find drop ID");
        }
      }
    } while(KvGotoNextKey(wcdrop));
  }
}

TriggerBossDrop() {
  if(!IsBoss() &&  !IsHNS())
  return;

  //HNS Integration
  if(IsHNS()) {
    new bool:hns_won=true;
    for(new i=1; i<=GetMaxClients(); i++) {
      if(IsClientConnected(i) && IsClientInGame(i) && GetBoss(i) > 0 && !IsKilled(i)) {
        hns_won=false;
      }
    }
    if(g_hns_mode == 1) {
      if(hns_won) {
        //All CTs won
        HNSWon(2);
      }
      else  {
        //Surviving CT's won
        HNSWon(1);
      }
    }
    if(g_hns_mode == 2) {
      if(hns_won) {
        //All CTs won
        HNSWon(2);
      }
      else  {
        //No-one won
        HNSWon(0);
      }
    }
  }


  if(IsBoss() ) for(new i=1;i<=GetMaxClients();i++)  {
    if(IsClientConnected(i))   if(IsClientInGame(i))   if(playerloaded[i]>=1)
    if(IsKilled(i))  {
      new boss=GetBoss(i);
      if(boss>0)
      {
        KvRewind(wcbossdrop);
        decl String:temp[9];
        IntToString(boss,temp,sizeof(temp));
        KvJumpToKey(wcbossdrop,temp);
        new xp=KvGetNum(wcbossdrop,"xp");
        if(xp>0)
          ServerCommand("wc_xp @all %i",xp);
        new Float:setchance=KvGetFloat(wcbossdrop,"chance");
        new Camount=KvGetNum(wcbossdrop,"amount");

        //Tokens
        new tokenId = KvGetNum(wcbossdrop,"dropall");
        new tokenAmount = KvGetNum(wcbossdrop,"dropall_amount");

        //giving sets drops
        decl String:Ssets[255];
        KvGetString(wcbossdrop,"sets",Ssets,sizeof(Ssets),"0");
        if((!StrEqual(Ssets,"0",false)) && (setchance>0.0)&&(Camount>0))
        {
          new sets[51];
          decl String:Stemp[255];
          new StringID=BreakString(Ssets, Stemp, sizeof(Stemp));
          new amount=1;
          sets[1]=StringToInt(Stemp);
          while(StringID>=0)
          {
            amount++;
            new StringID2=BreakString(Ssets[StringID], Stemp, sizeof(Stemp));
            StringID+=StringID2;
            if(StringID2==-1)
            StringID=-1;
            sets[amount]=StringToInt(Stemp);
          }
          new setitems[1501];
          new Iamount=0;
          for(new x=1;x<=amount;x++)
          {
            KvRewind(wcsets);
            decl String:Sset[800];
            Format(Stemp,sizeof(Stemp),"set%i",sets[x]);
            KvGetString(wcsets,Stemp,Sset,sizeof(Sset),"0");
            StringID=BreakString(Sset, Stemp, sizeof(Stemp));
            Iamount++;
            setitems[Iamount]=StringToInt(Stemp);
            while(StringID>=0)
            {
              Iamount++;
              new StringID2=BreakString(Sset[StringID], Stemp, sizeof(Stemp));
              StringID+=StringID2;
              if(StringID2==-1)
              StringID=-1;
              setitems[Iamount]=StringToInt(Stemp);
            }
          }
          for(new y=1;y<=Camount;y++) if(setchance>=GetRandomFloat(0.0,1.0))
          {
            new itemId=GetRandomInt(1,Iamount);
            new targets[65];
            new pamount=FindMatchingPlayers("@ct",targets);
            new winner=RollBetween(targets,pamount);
            new bool:foundwinner=false;
            while(!foundwinner)
            {
              winner=RollBetween(targets,pamount);
              if(wcDamageDone[winner]<g_iBossDropDmgMin && wcHealingDone[winner]<g_iBossDropHealMin)
              {
                PrintToChat(winner,"Ineligible for boss drops.");
                foundwinner=false;
              }
              else
                foundwinner=true;
            }
            if(winner==0)
            LogError("[WC] Could not roll for item");
            new String:winnername[50];
            GetClientName(winner,winnername,sizeof(winnername));
            new uitem[UniqueItem];
            GetItemProperties(uitem, setitems[itemId], 0);

            if(GiveItem(winner, uitem, 1, "boss") < 0)
            WCMessage(winner,"failed drop",winner,uitem[Item_Name]);
            else
            {
              WCMessage(winner,"item dropped",winner,uitem[Item_Name]);
              for(new x=1;x<=GetMaxClients();x++)
              if(IsClientInGame(x) && (x!=winner))
              WCMessage(x,"item dropped back",x,winnername,uitem[Item_Name]);
            }
          }

        }
        //end of sets drops
        if(KvGotoFirstSubKey(wcbossdrop))
        {
          do
          {
            new chance=KvGetNum(wcbossdrop,"chance");
            if(chance>=GetRandomInt(1,100))
            {
              new id=KvGetNum(wcbossdrop,"id");
              new targets[65];
              new amount=FindMatchingPlayers("@ct",targets);
              new winner=RollBetween(targets,amount);
              if(winner==0)
              LogError("[WC] Could not roll for item");
              new String:winnername[50];
              GetClientName(winner,winnername,sizeof(winnername));

              new uitem[UniqueItem];
              GetItemProperties(uitem, id, 0);

              if(GiveItem(winner, uitem, 1, "boss") < 0)
              WCMessage(winner,"failed drop",winner,uitem[Item_Name]);
              else
              {
                WCMessage(winner,"item dropped",winner,uitem[Item_Name]);
                for(new x=1;x<=GetMaxClients();x++)
                if(IsClientInGame(x) && (x!=winner))
                WCMessage(x,"item dropped back",x,winnername,uitem[Item_Name]);
              }
            }
          } while(KvGotoNextKey(wcbossdrop));
        }
        if(tokenId > 0 && tokenAmount > 0) {
          new uitem[UniqueItem];
          GetItemProperties(uitem, tokenId, 0);
          new targets[65];
          new pamount=FindMatchingPlayers("@all",targets);

          for(new y=0; y<pamount; y++) {
            if(GetClientTeam(targets[y]) == 2 || GetClientTeam(targets[y] == 3))
            GiveItem(targets[y], uitem, tokenAmount, "boss");

          }
        }

      }
    }
  }
}

RollBetween(targets[65],amount)
{
  new total,targetfrom[65],targetto[65];
  targetfrom[0]=0;
  targetto[0]=RoundToNearest(Pow(float(GetLevel(targets[0])),1.4));
  total=0;
  total+=RoundToNearest(Pow(float(GetLevel(targets[0])),1.4));
  for(new x=1;x<amount;x++)
  {
    targetfrom[x]=targetto[x-1]+1;
    new level=RoundToNearest(Pow(float(GetLevel(targets[x])),1.4));
    targetto[x]=targetfrom[x]+level;
    total+=level;
  }
  new random=GetRandomInt(0,total);
  for(new x=0;x<amount;x++)
  {
    if((random<=targetto[x])&&(random>=targetfrom[x]))
      return targets[x];
  }
  return 0;
}

SetClientProfessionSlot(client,id,id2,value)
{
  if(id==1)
    ClientProfessions[client][id2]=value;
    else  if(id==2)
      ClientProfessions[client][id2+200]=value;
      else
      LogError("wrong id, error #938, value %i",id);
}

GetClientProfessionSlot(client,id,id2)
{
  if(id==1)
    return ClientProfessions[client][id2];
    else  if(id==2)
      return ClientProfessions[client][id2+200];
      else
      LogError("wrong id, error #948, value %i",id);
  return -1;
}

FindProfessionR(client,profid,item)
{
  for (new x=1;x<=400;x++)
    if(item==GetClientProfessionSlot(client,profid,x))
      return 1;
  return 0;
}

ProfToName(id,String:back[],size,client)
{
  Format(back,size,"prof_%i",id);
  Format(back,size,"%T",back,client);
}

GetAmountOfItems(client,item2)
{
  new total;
  for (new x=1;x<=100;x++)
  {
    new item=GetSlotItem(client,x);
    if(item==item2)
    {
      new amount=GetSlotItemAmount(client,x);
      if(amount<=-1)
        amount=1;
      total+=amount;
    }
  }
  return total;
}

RemoveItem(client,item,amount)
{
  new x=1;
  while ((x<=100)&&(amount>0))
  {
    new item2=GetSlotItem(client,x);
    if(item2==item)
    {
      new amount2=GetSlotItemAmount(client,x);
      if(amount2==-1)
        amount2=1;
      if(amount2>amount)
      {
        wcSetStaticSlotAmount(client,x,amount2-amount);
        amount=0;
      }
      else
      {
        wcSetStaticSlot(client,x,0);
        wcSetStaticSlotAmount(client,x,0);
        new unique = GetInvUnique(client, x);
        SetInvUnique(client, x, 0);
        if(unique >0)  {
            UpdateUniqueState(x, 6);
        }
        amount-=amount2;
      }
    }
    x++;
  }
  if (amount==0) return 1;
  return 0;
}

GetRequiredLevel(item[UniqueItem])
{
   if( (item[Item_defId]>0) || (item[Item_id]>0) )
   {
      new cat=item[Item_category];
      new delta=0;
      if((cat>0)&&(cat<5))
        delta=RECIPE_LVL_MINUS;
      return item[Item_level]-delta;
   }
   return 0;
}

GetRequiredSkill(item)
{
   decl String:key[10];
   IntToString(item,key,sizeof(key));
   KvRewind(items);
   if(item>0)
   {
   if(KvJumpToKey(items,key))
   {
      return KvGetNum(items,"reqskill",0);
   }
   else
   {
      LogError("WC Error: could not find item %i",item);
   }
   }
   return 0;
}

GetClasses(item,classes[10])
{
  KvRewind(items);
  new String:info[50];
  IntToString(item,info,sizeof(info));
  KvJumpToKey(items,info);
  decl String:Sclass[50];
  KvGetString(items,"class",Sclass,sizeof(Sclass),"0");
  decl String:Stemp[50];
  new StringID=BreakString(Sclass, Stemp, sizeof(Stemp));
  new amount=1;
  classes[1]=StringToInt(Stemp);
  while(StringID>=0)
  {
    amount++;
    new StringID2=BreakString(Sclass[StringID], Stemp, sizeof(Stemp));
    StringID+=StringID2;
    if(StringID2==-1)
      StringID=-1;
    classes[amount]=StringToInt(Stemp);
  }
  return amount;
}

GetSetsRandomItem(String:Ssets[])  {
   if(StrEqual(Ssets,"0",false))
      return 0;
   new sets[51];
   decl String:Stemp[255];
   new StringID=BreakString(Ssets, Stemp, sizeof(Stemp));
   new amount=1;
   sets[1]=StringToInt(Stemp);

   while(StringID>=0)   {
      amount++;
      new StringID2=BreakString(Ssets[StringID], Stemp, sizeof(Stemp));
      StringID+=StringID2;
      if(StringID2==-1)
         StringID=-1;
      sets[amount]=StringToInt(Stemp);
   }

   new setitems[501];
   new Iamount=0;
   for(new x=1;x<=amount;x++) {
      KvRewind(wcsets);
      decl String:Sset[800];
      Format(Stemp,sizeof(Stemp),"set%i",sets[x]);
      KvGetString(wcsets,Stemp,Sset,sizeof(Sset),"0");
      StringID=BreakString(Sset, Stemp, sizeof(Stemp));
      Iamount++;
      setitems[Iamount]=StringToInt(Stemp);
      while(StringID>=0)   {
         Iamount++;
         new StringID2=BreakString(Sset[StringID], Stemp, sizeof(Stemp));
         StringID+=StringID2;
         if(StringID2==-1)
            StringID=-1;
         setitems[Iamount]=StringToInt(Stemp);
      }
   }
   new itemId=GetRandomInt(1,Iamount);
   return setitems[itemId];
}

stock GetMaxInt(int1,int2) {
   if(int1>int2)
      return int1;
   return int2;
}

stock GetMinInt(int1,int2) {
   if(int1<int2)
      return int1;
   return int2;
}

stock bool:HasCharacter(client, charId) {
  if(charId<=0)
    return false;
  for(new i=0;i<20;i++)
    if(characterids[client][i]==charId)
      return true;
  return false;
}

new Handle:g_cvarDm=INVALID_HANDLE;

stock bool:IsDmEnabled()  {
  if(g_cvarDm == INVALID_HANDLE)  {
    g_cvarDm = FindConVar("cssdm_enabled");
    return false;
  }
  new bool:ret = GetConVarBool(g_cvarDm);
  return ret;
}

stock bool:getsTalentPoint(level)  {
  switch(level) {
    case 85:  return true; //21
    case 84:  return true; //20
    case 83:  return true; //19
    case 82:  return true; //18
    case 81:  return true; //17
    case 80:  return true; //16
    case 78:  return true; //15
    case 76:  return true; //14
    case 74:  return true; //13
    case 72:  return true; //12
    case 70:  return true; //11
    case 67:  return true; //10
    case 64:  return true; // 9
    case 61:  return true; // 8
    case 58:  return true; // 7
    case 54:  return true; // 6
    case 50:  return true; // 5
    case 46:  return true; // 4
    case 41:  return true; // 3
    case 36:  return true; // 2
    case 30:  return true; // 1

  }

  return false;
}

stock getMaxTalents(level)  {
  new amount=0;
  for(new i=0; i <= level; i++) {
    if(getsTalentPoint(i))
      amount++;
  }
  return amount;
}





