/*
* File: admincmds.inc
* Description: Hooks admin commands
* Author(s): Stinkyfax
*/

#pragma semicolon 1

#define COLOR_DEFAULT 0x01
#define COLOR_LIGHTGREEN 0x03
#define COLOR_GREEN 0x04

public RegAdminCommands()
{
  // Hook all admin cmds

  RegAdminCmd("treasure_spawn_menu", Command_TreasureSpawnMenu, ADMFLAG_CUSTOM6, "Edits Treasure spawn points");
  
  RegAdminCmd("wc_test",wcTest,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_angletest",AngleTest,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_boss",AdminCmd_Boss,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_treasurehunt_enable",AdminCmd_TreasureHunt,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_ctf_enable",AdminCmd_Ctf,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_spawn_chest",AdminCmd_SpawnChest,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_hns",HNSModCmd,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_noradar", AdminCmd_NoRadar, ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_add_event",AdminCmd_Event,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_end_event",AdminCmd_EndEvent,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_addboss",AdminCmd_AddBoss,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_xp",AdminCmd_XP,ADMFLAG_CUSTOM4);
  RegAdminCmd("wc_armor",AdminCmd_Armor,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_reload",AdminCmd_Reload,ADMFLAG_CUSTOM5);
  RegAdminCmd("wc_reboot",AdminCmd_Reboot,ADMFLAG_CUSTOM5);
  RegAdminCmd("wc_level",AdminCmd_Level,ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_money",AdminCmd_Money,ADMFLAG_CUSTOM4);
  RegAdminCmd("wc_give",AdminCmd_Give,ADMFLAG_CUSTOM4);
  RegAdminCmd("wc_take",AdminCmd_Take,ADMFLAG_CUSTOM4);
  RegAdminCmd("wc_gold",AdminCmd_Gold,ADMFLAG_CUSTOM4);
  RegAdminCmd("wc_scramble", AdminCmd_Scramble, ADMFLAG_CUSTOM5);
  RegAdminCmd("wc_reloadpds", AdminCmd_reload_pds, ADMFLAG_CUSTOM5);
  //Pretty dangerous.
  //RegAdminCmd("wc_mailall",AdminCmd_MailAll,ADMFLAG_CUSTOM5);
  //RegConsoleCmd("testfade", FadeCmd);
  //RegAdminCmd("wc_remove",AdminCmd_Remove,ADMFLAG_CUSTOM6);
  // Quiz (fun.inc module) command
  RegAdminCmd("wc_quiz",RunQuiz,ADMFLAG_CUSTOM6);

  RegAdminCmd("wc_calcarena", CalculateArenaSql,ADMFLAG_BAN);
  RegAdminCmd("wc_calcarena_force", CalcArenaForce,ADMFLAG_BAN);

//  RegConsoleCmd("sm_s", ServerSay, "Is used to communicate players between few servers");
  RegConsoleCmd("sm_roll", Roll, "Gives a random value of person rolled 1-100");
  RegConsoleCmd("sm_target", CmdTargetOnly, "Makes your heals only heal the target specified.");
  RegConsoleCmd("wc_selfcast", CmdSelfcast, "Trigger self cast. Useful for healers");
  RegConsoleCmd("wc_selfonly", CmdSelfonly, "Trigger cast on yourself only. Useful when you're on low hp.");
  RegConsoleCmd("wc_drop", CmdDrop, "Trigger drop of your equipment. Except primary weapon and knife");
  RegConsoleCmd("wc_invprint", ManualInvPrint, "Print character inventory to console.");
  //RegConsoleCmd("wc_fovtest", FOVTEST,"Get all characters in FOV.");

  RegConsoleCmd("spell",SpellCmd,"select 1-7 spell to run from spellbar");
  RegConsoleCmd("wc_spellid",SpellIdCmd,"Run spell by it's ID. You can see ID in spellbook.");
  RegConsoleCmd("talent",TalentCmd,"Run talent by it's ID. Found in it's description");
  RegAdminCmd("sm_spawn_treasure_chest",TreasureCmd, ADMFLAG_CUSTOM6);

  RegAdminCmd("wc_honor",AdminCmd_Honor,ADMFLAG_CUSTOM4);

  RegConsoleCmd("sm_threat", ThreatCommand, "Triggers Threat Settings");
  RegAdminCmd("wc_setthreat",AdminCmd_Threat, ADMFLAG_CUSTOM6);
  RegAdminCmd("wc_facetarget",AdminCmd_FaceTarget, ADMFLAG_CUSTOM6);
  
  RegAdminCmd("sm_totem",AdminCmd_Totem, ADMFLAG_CUSTOM5);  //For testing
  RegAdminCmd("wc_generate_items", AdminCmd_GenerateItems, ADMFLAG_CUSTOM5, "Generates items to mysql");
  RegAdminCmd("sm_poly",AdminCmd_Poly, ADMFLAG_CUSTOM5);  //For testing
  RegAdminCmd("sm_randitem",AdminCmd_RandItem, ADMFLAG_CUSTOM5);  //For testing

  //Debug commnds
  RegAdminCmd("wc_debug_items",AdminCmd_DItems,ADMFLAG_CUSTOM4);
  RegAdminCmd("wc_debug_aim", AdminCmd_DAim, ADMFLAG_CUSTOM4);
  RegConsoleCmd("wc_debug_me", DebugMeCmd, "Debug your character, be careful!");
  RegConsoleCmd("sm_debug_me", DebugMeCmd, "Debug your character, be careful!");
  RegAdminCmd("wc_debug_give_talent",AdminCmd_debug_talents,ADMFLAG_ROOT);
  RegAdminCmd("wc_debug_fog", AdminCmd_debug_fog, ADMFLAG_ROOT);
  RegAdminCmd("wc_debug_premium", AdminCmd_debug_premium, ADMFLAG_ROOT);

  //chat commands
  RegConsoleCmd("sm_chat_attacker", ChatAttCommand, "Triggers Combat Messages for attacker");
  RegConsoleCmd("sm_chat_victim", ChatVicCommand, "Triggers Combat Messages for victim");
  RegConsoleCmd("sm_chat_target", ChatTarCommand, "Triggers Combat Messages for no-target");
  RegConsoleCmd("sm_chat_spawn", ChatSpaCommand, "Triggers Spawn Messages for player");

  //newbie helping
  RegConsoleCmd("sm_newbie", NewbieCommand, "Triggers Newbie Pop-up");
  RegConsoleCmd("sm_autotodo", NewbieCommand2, "Triggers Auto TO-DO Pop-up");
  RegConsoleCmd("sm_usekeyspellbar", UsekeySpellbar, "Toggles Spellbar from +use.");
  RegConsoleCmd("sm_todo", TodoCommand, "Display your TO-DO list");
  RegConsoleCmd("sm_rs", TodoCommand, "Display your TO-DO list");
  RegConsoleCmd("sm_referrer", ReferrerCommand, "Tell server who is your referrer person.");
  RegConsoleCmd("sm_reference", ReferrerCommand, "Tell server who is your referrer person.");
  RegConsoleCmd("sm_changes", ChangelogCommand, "Pops up window with changelog");
  RegConsoleCmd("sm_changelog", ChangelogCommand, "Pops up window with changelog");
  RegConsoleCmd("sm_auction", OpenAuctionMenu, "Opens up the auctions menu");

  //CSGO Specific commands
  if(CSGO)
  {
    RegAdminCmd("sm_cash", GiveCash, ADMFLAG_CUSTOM6);
    RegAdminCmd("sm_hp", SetHp, ADMFLAG_CUSTOM6);
  }
}
public Action:ChangelogCommand(client,argc)  {
  if(client<1 || client > GetMaxClients())  {
    ReplyToCommand(client, "Command can be used only in-game");
    return Plugin_Handled;
  }

  decl String:name[30];
  GetClientName(client, name, 30);

  for(new x=1;x<=GetMaxClients();x++)
    if(IsClientConnected(x))
      if(IsClientInGame(x))
        WCMessage(x,"used changelog cmd",x, name);

  decl String:address[350];
  Format(address, 350, "%sindex.php/Changelog/%s", WIKI_PAGE, WIKI_VERSION);
  ShowMOTDPanel(client, "WoW Help Wiki", address, MOTDPANEL_TYPE_URL);

  return Plugin_Handled;
}
public Action:ChatAttCommand(client,argc)  {
   decl String:temp[100];
   if(TriggerChat(client, Chat:Chat_Attacker))   {
      Format(temp,sizeof(temp),"%T","wc chat attacker is on", client);
   }
   else  {
      Format(temp,sizeof(temp),"%T","wc chat attacker is off", client);
   }
   ReplyToCommand(client, temp);
   return Plugin_Handled;
}
public Action:OpenAuctionMenu(client,argc)  {
   AuctionsMenu(client);
   return Plugin_Handled;
}
public Action:GiveCash(client,argc)
{
  PrintToChat(client,"Gave everyone cash");
  new maxplayers=GetMaxClients();
  for(new x=1;x<=maxplayers;x++)
  {
    if(IsClientInGame(x))
    {
      SetEntProp( x, Prop_Send, "m_iAccount", 64000);
    }
  }
}


public Action:SetHp(client, args)
{
  if (args < 2)
  {
    ReplyToCommand(client, "[WC] Usage: sm_hp <target name> <health percentage to set health>");
    return Plugin_Handled;
  }
  decl String:name[32], String:temp[20];
  new target = -1;
  GetCmdArg(1, name, 20);
  for (new i=1;i<=MaxClients;i++)
  {
    if (!IsClientConnected(i))
    {
      continue;
    }
    decl String:other[32];
    GetClientName(i, other, sizeof(other));
    if (StrEqual(name, other))
    {
      target = i;
    }
  }
  if (target == -1)
  {
    PrintToConsole(client, "Could not find any player with the name: \"%s\"", name);
  }
  else
  {
    new maxhp = GetMaxHealth(target);
    GetCmdArg(2, temp, 20);
    new health = StringToInt(temp);
    new sethp = RoundToNearest(float(health) * float(maxhp) * 0.01);
    wcSetHealth(target,sethp);
  }
  return Plugin_Handled;
}

public Action:ChatVicCommand(client,argc)  {
   decl String:temp[100];
   if(TriggerChat(client, Chat:Chat_Victim))   {
      Format(temp,sizeof(temp),"%T","wc chat victim is on", client);
   }
   else  {
      Format(temp,sizeof(temp),"%T","wc chat victim is off", client);
   }
   ReplyToCommand(client, temp);
   return Plugin_Handled;
}

public Action:ChatTarCommand(client,argc)  {
   decl String:temp[100];
   if(TriggerChat(client, Chat:Chat_Target))   {
      Format(temp,sizeof(temp),"%T","wc chat target is on", client);
   }
   else  {
      Format(temp,sizeof(temp),"%T","wc chat target is off", client);
   }
   ReplyToCommand(client, temp);
   return Plugin_Handled;
}

public Action:ChatSpaCommand(client,argc)  {
   decl String:temp[100];
   if(TriggerChat(client, Chat:Chat_Spawn))   {
      Format(temp,sizeof(temp),"%T","wc chat spawn is on", client);
   }
   else  {
      Format(temp,sizeof(temp),"%T","wc chat spawn is off", client);
   }
   ReplyToCommand(client, temp);
   return Plugin_Handled;
}

new dAimRunner;
public Action:AdminCmd_DAim(client, argc) {
   dAimRunner=client;
   new Float:value = 25.0;
   ReplyToCommand(client, "25 angle check");
   decl Float:loc[3],Float:view[3];
   GetClientEyePosition(client,loc);
   GetClientEyeAngles(client, view);
   ClientTracer=client;
   TR_TraceRayFilter(loc, view, MASK_SHOT, RayType_Infinite,AimTargetFilter2);
   new target=TR_GetEntityIndex(INVALID_HANDLE);
   if((target>0)&&(target<=GetMaxClients()))
      if(IsClientConnected(target))
         if(IsClientInGame(target)) {
            ReplyToCommand(client, "Direct target, no wall");
         }
   new Float:angelrec[2],Closest;
   angelrec[0]=181.0;
   angelrec[1]=181.0;
   for(new x=1;x<=GetMaxClients();x++)
   if(IsClientConnected(x)) if(IsClientInGame(x)) if(x!=client)
   {

    decl Float:loc2[3];
    decl Float:vector[3],Float:angles[3];
    GetClientEyePosition(x,loc2);
    MakeVectorFromPoints(loc, loc2, vector);
    GetVectorAngles(vector, angles);
    new Float:dif[2];
    dif[0]=angles[0]-view[0];
    dif[1]=angles[1]-view[1];
    if(dif[0]<0.0)
      dif[0]*=-1.0;
    if(dif[1]<0.0)
      dif[1]*=-1.0;
    if(dif[0]>180)
      dif[0]=(dif[0]-360.0)*-1.0;
    if(dif[1]>180)
      dif[1]=(dif[1]-360.0)*-1.0;
    decl String:name[50];
    GetClientName(x,name,sizeof(name));
    if((dif[0]<value) && (dif[1]<value) && (dif[0]<angelrec[0]) && (dif[1]<angelrec[1]))
    {
      ReplyToCommand(client,"client %s (%i) is in angle", name, x);
      new Handle:hTrace = TR_TraceRayFilterEx(loc, angles, MASK_SHOT, RayType_Infinite,AimTargetFilter2);
      new target2=TR_GetEntityIndex(hTrace);
      new Float:endvec[3];
      TR_GetEndPosition(endvec, hTrace);
      BeamEffect("@all",loc,endvec,g_ef_glow01,5,2.0,3.0,9.0,{200, 200, 200, 255 },0.0,5);
      CloseHandle(hTrace);
      if((target2>0)&&(target2<=GetMaxClients()))
      if(IsClientConnected(target2))
         if(IsClientInGame(target2))
            {
              angelrec[0]=dif[0];
              angelrec[1]=dif[1];
              Closest=target2;
            }
    }
    //ReplyToCommand(client,"client %s (%i) is NOT in angle", name, x);
   }
   decl String:sT[30];
   GetClientName(Closest,sT,sizeof(sT));
   ReplyToCommand(client,"Closest target is = %i (%s)", Closest, sT);

   return Plugin_Handled;
}

public Action:AdminCmd_DItems(client, argc)   {
   ReplyToCommand(client,"Debugging items database");
   KvRewind(items);
   if (!KvGotoFirstSubKey(items))
   {
      ReplyToCommand(client, "Could not open items.txt file");
   }
   new lastId=-1;
   new maxId=-1;
   decl String:info[25];
   do {
      KvGetSectionName(items, info, sizeof(info));
      lastId = StringToInt(info);
      if(maxId<lastId)
         maxId=lastId;
   }  while(KvGotoNextKey(items));
   ReplyToCommand(client," --- Debugging Items is finished --- \n-last item found: %i\n-max ID: %i", lastId, maxId);
   return Plugin_Handled;
}



public Action:AdminCmd_Scramble(client, argc)  {
  ReplyToCommand(client, "Scrambling teams");
  scrambleTeams();
  return Plugin_Handled;
}

public Action:AdminCmd_GenerateItems(client, argc)  {
  ReplyToCommand(client, "Generating items in 10 seconds");
  CreateTimer(10.0, GenerateItemsTimer);
  return Plugin_Handled;
}

public Action:GenerateItemsTimer(Handle:timer)  {
  _generateSqlItems();
  return Plugin_Handled;
}

public Action:ReferrerCommand(client, argc)   {
  if(playerloaded[client]!=1) {
    return Plugin_Handled;
  }
  if(argc!=1)  {
    WCMessage(client, "Say Refferrer ID", client);
    return Plugin_Handled;
  }
  decl String:temp[20];
  GetCmdArg(1, temp, 20);
  new referrer = StringToInt(temp);
  if(referrer <= 0) {
    WCMessage(client, "Say Refferrer ID Error", client);
    return Plugin_Handled;
  }
  if(referrer == playerids[client]) {
    WCMessage(client, "Say Refferrer ID Error2", client);
    return Plugin_Handled;
  }
  decl String:query[200];
  Format(query, sizeof(query), "SELECT '%d' AS `newref`, (SELECT `name` FROM wc_Players WHERE playerId = %d) AS `newname`, referrer, ROUND(HOUR(TIMEDIFF(NOW(), first_time))/24) AS days FROM wc_Players WHERE playerId = %d", referrer, referrer, playerids[client]);
  SQL_TQuery(TMSQL, TReferrerCommand, query, client);
  return Plugin_Handled;
}

public Action:AdminCmd_Poly(client, argc)   {
      ReplyToCommand(client,"Polymorphing you for 10 sec");
      setPolymorph(client, 10.0);
      return Plugin_Handled;
}
/**
 * @param tar      client to give item
 * @param announce -1 system decides, 0=self only, 1=self only, 2=all.
 * @param rarity   -1 system decides.
 */
public Action:AdminCmd_RandItem(client, argc)   {
  if (argc != 3)
  {
    ReplyToCommand(client,"Syntax: sm_randitem: <target> <announce> <rarity>");
    return Plugin_Handled;
  }

  decl String:temp[20];
  GetCmdArg(1, temp, 20);
  new tar = FindTarget(client, temp, false, false);
  GetCmdArg(2, temp, 20);
  new announce = StringToInt(temp);
  GetCmdArg(3, temp, 20);
  new rarity = StringToInt(temp);

  GivePlayerRandomItem(tar, "test", announce, rarity);
  return Plugin_Handled;
}

public Action:AdminCmd_Totem(client, argc)   {
      if(argc<1)  {
        return Plugin_Handled;
      }
      decl String:model[200];
      GetCmdArg(1, model, sizeof(model));
      ReplyToCommand(client,"effect");
      new Float:distance = 300.0;
      new Float:angle[3];
      GetClientEyeAngles(client,angle);
      new Float:endpos[3];
      new Float:startpos[3];
      GetClientEyePosition(client,startpos);
      new Float:dir[3];
      GetAngleVectors(angle, dir, NULL_VECTOR, NULL_VECTOR);
      ScaleVector(dir, distance);
      AddVectors(startpos, dir, endpos);
      ClientTracer=client;
      TR_TraceRayFilter(startpos,endpos,MASK_PLAYERSOLID,RayType_EndPoint,AimTargetFilter);
      TR_GetEndPosition(endpos);
      distance = (GetVectorDistance(startpos, endpos) - 55.0);
      endpos[1] = (startpos[1] + (distance * Sine(DegToRad(angle[1]))));
      endpos[0] = (startpos[0] + (distance * Cosine(DegToRad(angle[1]))));
      //endpos[2]-=40;

      ReplyToCommand(client,"Creating effect");

      GlowSpriteEffect("@all", endpos, -1, 4.0, 1.7, 255, model);

      return Plugin_Handled;
}

public Action:ThreatCommand(client,argc)  {
   decl String:temp[100];
   if(TriggerThreat(client))   {
      Format(temp,sizeof(temp),"%T","wc threat is on", client);
   }
   else  {
      Format(temp,sizeof(temp),"%T","wc threat is off", client);
   }
   ReplyToCommand(client, temp);
   return Plugin_Handled;
}

public Action:DebugMeCmd(client,argc)  {
   decl String:temp[100];
   if(client>0 && client <= GetMaxClients())  {
    Format(temp,sizeof(temp),"%T","wc debug me txt", client);
    ReplyToCommand(client, temp);
    DebugMenu(client);
   }
   return Plugin_Handled;
}

public Action:NewbieCommand(client,argc)  {
   decl String:temp[100];
   if(TriggerNewbie(client))   {
      Format(temp,sizeof(temp),"%T","wc newbie is on", client);
   }
   else  {
      Format(temp,sizeof(temp),"%T","wc newbie is off", client);
   }
   ReplyToCommand(client, temp);
   return Plugin_Handled;
}

public Action:NewbieCommand2(client,argc)  {
   decl String:temp[100];
   if(TriggerNewbie2(client))   {
      Format(temp,sizeof(temp),"%T","wc newbie2 is on", client);
   }
   else  {
      Format(temp,sizeof(temp),"%T","wc newbie2 is off", client);
   }
   ReplyToCommand(client, temp);
   return Plugin_Handled;
}

public Action:UsekeySpellbar(client,argc)  {
   decl String:temp[100];
   if(UsekeySpellbar2(client))   {
      Format(temp,sizeof(temp),"%T","usekeyspellbar on", client);
   }
   else  {
      Format(temp,sizeof(temp),"%T","usekeyspellbar off", client);
   }
   ReplyToCommand(client, temp);
   return Plugin_Handled;
}

public Action:TodoCommand(client,argc)  {
   displayTodoMenu(client, true);

   return Plugin_Handled;
}

public Action:SpellIdCmd(client,args)
{
   if(client<=0)  {
      ReplyToCommand(client, "[WC] wc_spellid only for players");
      return Plugin_Handled;
   }
   if (args < 1)
   {
		ReplyToCommand(client, "[WC] Usage: wc_spellid <spell ID from spellbook>");
		return Plugin_Handled;
   }
   decl String:Sspell[5];
   GetCmdArg(1, Sspell, sizeof(Sspell));
   new spell=StringToInt(Sspell);
   if(!IsPlayerAlive(client))
   {
      WCMessage(client,"you are dead",client);
      return Plugin_Handled;
   }
   TryCast(client, spell);
   return Plugin_Handled;
}

public Action:SpellCmd(client,args)
{
   if (args < 1)
   {
		ReplyToCommand(client, "[WC] Usage: spell <spell ID from spellbar>");
		return Plugin_Handled;
   }
   decl String:Sspell[5];
   GetCmdArg(1, Sspell, sizeof(Sspell));
   new spell=StringToInt(Sspell);
   if((spell>=8)||(spell<=0))
   {
		ReplyToCommand(client, "[WC] spell must be 1-7");
		return Plugin_Handled;
   }

   spell-=1;
   new spellid=wcGetSpellBarSpell(client,spell);

   TryCast(client, spellid);


   return Plugin_Handled;
}
public Action:TreasureCmd(client,args)
{
  PrintToChat(client,"command works");
  TreasureChestMap(1000);
  return Plugin_Handled;
}
public Action:TalentCmd(client,args)
{
  if(IsZombie(client)) return Plugin_Handled;
  decl String:Stalent[5];
  GetCmdArg(1, Stalent, sizeof(Stalent));
  new talent=StringToInt(Stalent);
  new race = GetClass(client);
  switch(race)
  {
  case 1:                                                                       //Warrior
    switch(talent)
    {
      case 1:
        {
          if(CheckRequirements(client, 1, 1, 3, 1))
            wcRunTalent_1_1(client); //Last Stand
        }
      case 2:
        {
          if(CheckRequirements(client, 1, 1, 2, 1))
            wcRunTalent_1_stun(client); //Stun
        }
      case 3:
        {
          if(CheckRequirements(client, 1, 3, 3, 1)) //Sweeping Strikes
            wcRunTalent_1_3(client); //Sweeping Strikes
        }
      case 4:
        {
          if(CheckRequirements(client,1,2,4,1)) //Bladestorm
            wcRunTalent_1_BladeStorm(client);
        }
      case 5:
        {
          if(CheckRequirements(client,1,1,4,1)) //Unbreakable Bastion
            wcRunTalent_1_Bastion(client);
        }
      default:
        {
          WCMessage(client,"no talent",client);
        }
    }
   case 2:                                                                      //Rogue
    switch(talent)
    {
      case 1:
        {
          //if(CheckRequirements(client,2, 1, 4, 1))
            //wcRunTalent_2_1(client); //Preparation
        }
      case 2:
        {
          if(CheckRequirements(client,2, 2, 3, 1))
            wcRunTalent_2_2(client); //Adrenaline Rush
        }
      case 3:
        {
          if(CheckRequirements(client,2, 3, 3, 1))
            wcRunTalent_2_3(client); //Coldblood
        }
      case 4:
        {
          if(CheckRequirements(client,2,1,3,1))
            wcRunTalent_2_ShadowStep(client); //shadowstep
        }
      case 5:
        {
          if(CheckRequirements(client,2,3,4,1))
            wcRunTalent_2_ShadowGate(client); //shadowgate
        }
      case 6:
      {
        if(CheckRequirements(client,2,1,4,1))
          wcRunTalent_2_Shadowdance(client);
      }

      default:
        {
          WCMessage(client,"no talent",client);
        }
    }
      case 3:                     //Mage
    switch(talent)
    {
      case 1:
        {
          if(CheckRequirements(client,3, 1, 3, 1))
            wcRunTalent_3_1(client); //Ice Barrier
        }
      case 2:
        {
          if(CheckRequirements(client, 3, 2, 3, 1))
            wcRunTalent_3_2(client); //Living Bomb
        }
      case 3:
        {
          if(CheckRequirements(client, 3, 3, 3, 1))
            wcRunTalent_3_3(client); //Invisibility
        }
      case 4:
        {
          if(CheckRequirements(client, 3, 1, 4, 1))
            wcRunTalent_3_Blizzard(client);
        }
      case 5:
        {
          if(CheckRequirements(client, 3, 2, 4, 1))
            wcRunSpell_3_8(8,3,client);
        }
      case 6:
        {
          if(CheckRequirements(client, 3, 3, 4, 1))
            wcRunTalent_3_RuneOfPower(client); //Rune of Power
        }
      default:
        {
          WCMessage(client,"no talent",client);
        }
    }
      case 4:                                                                   //Paladin
    switch(talent)
    {
      case 1:
        {
          if(CheckRequirements(client,4, 1, 3, 1))
            wcRunTalent_4_1_2(client); //Holy Shock
        }
      case 2:
        {
          if(CheckRequirements(client,4, 2, 3, 1))
            wcRunTalent_4_2(client); //Avengers Shield
        }
      case 3:
        {
          if(CheckRequirements(client,4, 3, 3, 1))
            wcRunTalent_4_3(client); //Divine Storm
        }
      case 4:
      {
        if(CheckRequirements(client,4,2,4,1))
          wcRunTalent_4_RighteousShield(client);
      }
      default:
        {
          WCMessage(client,"no talent",client);
        }
    }
      case 5:
    switch(talent)
    {                                                                           //Priest
      case 1:
        {
          if(CheckRequirements(client,5, 1, 3, 1))
            wcRunTalent_5_1(client); //Pain Suppression
        }
      case 2:
        {
          if(CheckRequirements(client,5, 2, 3, 1))
            wcRunTalent_5_2(client); //Circle of Healing
        }
      case 3:
        {
          if(CheckRequirements(client,5, 3, 3, 1))
            wcRunTalent_5_3(client); //Shadow Form
        }
      case 4:
        {
          if(CheckRequirements(client,5, 3, 2, 1))
            wcRunTalent_5_10(client); //Shadow Word:Death
        }
      case 5:
        {
          if(CheckRequirements(client,5,1,4,1))
            wcRunTalent_5_PWBarrier(client);
        }
      case 6:
        {
          if(CheckRequirements(client,5,2,4,1))
            wcRunTalent_5_LightWell(client);
        }
      case 7:
        {
          if(CheckRequirements(client,5,3,4,1))
            wcRunTalent_5_MindFlay(client);
        }
      default:
        {
          WCMessage(client,"no talent",client);
        }
    }
      case 6:
    switch(talent)                                                              //Shaman
    {
      case 1:
      {
        /*if(CheckRequirements(client,6, 1, 3, 1))*/
          /*wcRunTalent_6_1(client); //Elemental Fury*/
      }
      case 2:
      {
        if(CheckRequirements(client,6, 2, 3, 1))
          wcRunTalent_6_3(client); //Shamanistic Rage
      }
      case 3:
      {
        if(CheckRequirements(client,6, 3, 3, 1))
          wcRunTalent_6_5(client); //Earth Shield
      }
      case 4:
      {
        if(CheckRequirements(client,6,3,2,2))
          wcRunTalent_6_4(client); //Chainheal
      }
      case 5:
      {
        if(CheckRequirements(client,6,3,4,2))
          wcRunTalent_6_HealingTideTotem(client);
      }
      case 6:
      {
        if(CheckRequirements(client,6,1,4,1))
          wcRunTalent_6_LightningNovaTotem(client);
      }
      case 7:
      {
        if(CheckRequirements(client,6,1,2,3))
          wcRunTalent_6_ChainLightning(client);
      }
      default:
      {
        WCMessage(client,"no talent",client);
      }
    }
      case 7:                                                                   //Warlock
    switch(talent)
    {
      case 1:
        {
          if(CheckRequirements(client,7,1,4,1))
            wcRunTalent_7_CursedSoul(client);
        }
      case 2:
        {
          if(CheckRequirements(client,7, 2, 3, 1))
            wcRunTalent_7_Healthstone(client);
        }
      case 3:
        {
          if(CheckRequirements(client,7, 3, 4, 1))
            wcRunTalent_7_3(client); //Chaos Bolt
        }
      case 4:
        {
          if(CheckRequirements(client,7,3,3,1))
            wcRunTalent_7_Immolate(client);
        }
      case 5:
        {
          if(CheckRequirements(client,7,2,4,1))
            wcRunTalent_7_SacrificalPact(client);

        }
      default:
        {
          WCMessage(client,"no talent",client);
        }
    }
    case 8:
    switch(talent)
    {
      case 1: if(CheckRequirements(client,8, 1, 4, 1))
            wcRunTalent_RuneTap(client); //Rune Tap
      case 2: if(CheckRequirements(client,8, 1, 2, 2))
            wcRunTalent_VampiricBlood(client);
      case 3: if(CheckRequirements(client,8, 2, 3, 1))
            wcRunTalent_HungeringCold(client);
      case 4: if(CheckRequirements(client,8, 1, 3, 1))
            wcRunTalent_8_Heartfire(client);
      case 5: if(CheckRequirements(client,8, 2, 4, 1))
            wcRunTalent_8_Hypothermia(client);
      /*case 6: if(CheckRequirements(client,8, 3, 4, 1))*/
            /*wcRunTalent_8_AntiMagicZone(client);*/
    }
    case 9:
    switch(talent)
    {
      case 1: if(CheckRequirements(client,9,1,2,1))
        wcRunTalent_9_Firebreath(client);
      /*case 2: if(CheckRequirements(client,9,2,4,1))
        wcRunTalent_9_TouchofKarma(client);*/
      case 3: if(CheckRequirements(client,9,3,2,1))
        wcRunTalent_9_RenewingMist(client);
      case 4: if(CheckRequirements(client,9,2,3,2))
        wcRunTalent_9_SerpentKick(client);
      case 5: if(CheckRequirements(client,9,3,3,1))
        wcRunTalent_9_LifeCocoon(client);
      case 6: if(CheckRequirements(client,9,3,4,1))
        wcRunTalent_9_Resusciate(client);
      //case 7: if(CheckRequirements(client,9,1,4,1))
        //wcRunTalent_9_ChiSerenity(client);
      case 7: if(CheckRequirements(client,9,1,4,1))
        wcRunTalent_9_Guard(client);
      case 9: if(CheckRequirements(client,9,1,3,1))
        wcRunTalent_9_Keg(client);

    }
  }
  return Plugin_Handled;
}

 public Action:PrintInvPacket(Handle:timer, Handle:pack)
{
  ResetPack(pack);
  new client = ReadPackCell(pack);
  new String:line[1020];
  ReadPackString(pack, line, sizeof(line));
  PrintToConsole(client, line);
}

public Action:FOVTEST(client,argc)
{
  new targets[MAXPLAYERS];
  new Float:location1[3];
  new Float:location2[3];
  GetClientAbsOrigin(client,location1);
  new amount=GetNearClients("@all",location1,targets,5000.0);
  decl String:name[60];
  new Float:distance;
  decl String:line[510];
  for (new x=0;x<=(amount-1);x++)
  {
    GetClientAbsOrigin(targets[x],location2);
    distance = GetDistanceBetween(location1,location2);
    GetClientName(targets[x],name,sizeof(name));
    Format(line, sizeof(line),"[FOVTEST] You can see %s at %.0f distance, FOVfront(%d), FOVall(%d)",
      name,distance,GetTargetFOV(client,targets[x]),GetTargetFOV(client,targets[x],360.0));
    WCMessage(client,"testmessage",client,line,x);
  }
  return Plugin_Handled;
}

public Action:ManualInvPrint(client,argc)
{
  if((client>0) && client <=GetMaxClients())
  {
    if(argc<2)
    {
      //WCMessage(client,"invprint 1",client);
      PrintToConsole(client, "Syntax: wc_invprint <rarity> <category>\nValid Rarities: Any, Common, Uncommon, Rare, Epic, Legendary, PVP-R, PVP-E\nValid Categories: Any, Gear, Glyph, Other, Reagent, Recipe, Scroll, Back, Chest, Feet, Hands, Helm, Legs, Neck, Ring, Shield, Offhand, Shoulders, Trinket, Waist, Weapon, Wrist.");
      return Plugin_Handled;
    }

    decl String:reqrarity[255];
    new rarity;
    GetCmdArg(1,reqrarity,sizeof(reqrarity));
    if (!strncmp(reqrarity, "any",3,false) || !strncmp(reqrarity, "all",3,false) || !strncmp(reqrarity, "a",1,false))
      rarity=-1;
    else if (!strncmp(reqrarity, "common",6,false) || !strncmp(reqrarity, "c",1,false))
      rarity=0;
    else if (!strncmp(reqrarity, "uncommon",8,false) || !strncmp(reqrarity, "u",1,false))
      rarity=1;
    else if (!strncmp(reqrarity, "rare",4,false) || !strncmp(reqrarity, "r",1,false))
      rarity=2;
    else if (!strncmp(reqrarity, "epic",4,false) || !strncmp(reqrarity, "e",1,false))
      rarity=3;
    else if (!strncmp(reqrarity, "legendary",9,false) || !strncmp(reqrarity, "l",1,false))
      rarity=4;
    else if (!strncmp(reqrarity, "PVP-R",5,false) || !strncmp(reqrarity, "pvpr",4,false) || !strncmp(reqrarity, "pr",2,false))
      rarity=5;
    else if (!strncmp(reqrarity, "PVP-E",5,false) || !strncmp(reqrarity, "pvpe",4,false) || !strncmp(reqrarity, "pe",2,false))
      rarity=6;
    else
    {
      rarity=-1;
      WCMessage(client, "'%s' did not match any rarity options, lets go with (Any) then.",reqrarity);
    }

    new category;
    decl String:reqcategory[255];
    GetCmdArg(2,reqcategory,sizeof(reqcategory));
    if (!strncmp(reqcategory, "any",3,false) || !strncmp(reqcategory, "all",3,false) || !strncmp(reqcategory, "a",1,false))
      category=-1; //all
    else if (!strncmp(reqcategory, "gear",4,false))
      category=-2; //category 10/11/12/13/14/15/16/17/18/19/20/21/22/23
    else if (!strncmp(reqcategory, "other",5,false))
      category=-3; //category 27/24
    else if (!strncmp(reqcategory, "recipe",6,false))
      category=-4; //category 1/2/3/4/5/9
    else if (!strncmp(reqcategory, "back",4,false))
      category=10;
    else if (!strncmp(reqcategory, "chest",5,false))
      category=11;
    else if (!strncmp(reqcategory, "feet",4,false) || !strncmp(reqcategory, "foot",4,false))
      category=12;
    else if (!strncmp(reqcategory, "hand",4,false))
      category=13;
    else if (!strncmp(reqcategory, "helm",4,false))
       category=14;
    else if (!strncmp(reqcategory, "leg",3,false))
      category=15;
    else if (!strncmp(reqcategory, "neck",4,false))
      category=16;
    else if (!strncmp(reqcategory, "ring",4,false))
      category=17;
    else if (!strncmp(reqcategory, "shield",6,false) || !strncmp(reqcategory, "off",3,false))
      category=18;
    else if (!strncmp(reqcategory, "shoulder",8,false))
      category=19;
    else if (!strncmp(reqcategory, "trinket",7,false))
      category=20;
    else if (!strncmp(reqcategory, "waist",5,false))
      category=21;
    else if (!strncmp(reqcategory, "weapon",6,false))
      category=22;
    else if (!strncmp(reqcategory, "wrist",5,false))
      category=23;
    else if (!strncmp(reqcategory, "glyph",5,false))
      category=30;
    else if (!strncmp(reqcategory, "reagent",7,false))
      category=8;
    else if (!strncmp(reqcategory, "scroll",6,false))
      category=25;
    else
    {
      category=-1;
      WCMessage(client, "'%s' did not match any category options, lets go with (Any) then.",reqcategory);
    }

    InvPrint(client,rarity,category);
  }
  return Plugin_Handled;
}


/* Available paramaters for InvPrint are
 * rarity:
 * -1      : All Rarities
 *  0-6    : Individual Rarities
 *
 * category:
 * -4      : Recipe's for Crafting
 * -3      : Random Boxes and "General Usable"
 * -2      : Gear only
 * -1      : All Categories
 *  8      : Reagents
 * 30      : Glyphs
 * 10-23   : Individual Equipment Categories
 * 25      : Enchant Scrolls
 *
 * Entering numbers outside of these ranges will not error but will
 * probably not print items either unless it's category 24 or 27 as
 * those are the only other two categories that are available for
 * items that go IN a players inventory.
 */
public InvPrint(client,rarity,category)
{
  if((client>0) && client <=GetMaxClients())
  {
    decl String:line[1020],String:RL[80];
    Format(line, sizeof(line),"");
    new item[UniqueItem];
    new cate;
    new rare;
    new Float:duration=0.2;
    for(new x=1;x<=100;x++)
    {
      GetInvItem(item,client, x);
      if((item[Item_defId]==0) && (item[Item_id]==0))
        continue;
      rare=item[Item_quality];


      if( rarity >= 0 )
      {
        if(rare != rarity)
          continue;
      }

      cate=item[Item_category];
      switch (category)
      {
        case -4: //recipe 1/2/3/4/5/9
        {
          if((cate > 5 && cate < 1) && cate != 9)
            continue;
        }
        case -3: //other 24/27
        {
          if(cate != 24 && cate != 27)
            continue;
        }
        case -2: //gear 10/11/12/13/14/15/16/17/18/19/20/21/22/23
        {
          if (cate < 10 || cate > 23)
            continue;
        }
        case 8,10,11,12,13,14,15,16,17,18,19,20,21,22,23,25,30:
        {
          if(cate != category)
            continue;
        }
      }

      new amount = GetSlotItemAmount(client,x);

      if (strlen(line)>900)
      {
        new Handle:pack = CreateDataPack();
        WritePackCell(pack,client);
        WritePackString(pack,line);
        CreateTimer(duration,PrintInvPacket,pack,TIMER_HNDL_CLOSE);
        duration +=0.2;
        Format(line, sizeof(line),"");
      }
      if(cate>=10 && cate<=23) //equippable equipment!
      {

        switch(rare)
        {
          case 0: {Format(RL, sizeof(RL),"Level: %d (%d iLvl) Common, Rating: %.1f",    item[Item_level],item[Item_ilevel], GetItemRating(item));}
          case 1: {Format(RL, sizeof(RL),"Level: %d (%d iLvl) Uncommon, Rating: %.1f",  item[Item_level],item[Item_ilevel], GetItemRating(item));}
          case 2: {Format(RL, sizeof(RL),"Level: %d (%d iLvl) Rare, Rating: %.1f",      item[Item_level],item[Item_ilevel], GetItemRating(item));}
          case 3: {Format(RL, sizeof(RL),"Level: %d (%d iLvl) Epic, Rating: %.1f",      item[Item_level],item[Item_ilevel], GetItemRating(item));}
          case 4: {Format(RL, sizeof(RL),"Level: %d (%d iLvl) Legendary, Rating: %.1f", item[Item_level],item[Item_ilevel], GetItemRating(item));}
          case 5: {Format(RL, sizeof(RL),"Level: %d (%d iLvl) PVP-Rare, Rating: %.1f",  item[Item_level],item[Item_ilevel], GetItemRating(item));}
          case 6: {Format(RL, sizeof(RL),"Level: %d (%d iLvl) PVP-Epic, Rating: %.1f",  item[Item_level],item[Item_ilevel], GetItemRating(item));}
          default:{Format(RL, sizeof(RL),"");}
        }

        if(amount == -1)
          Format(line, sizeof(line), "%sSlot: %d, Name: %s (Soulbound)\n%s",line,x,item[Item_Name],RL);
        else if (amount == 0)
          Format(line, sizeof(line), "%sSlot: %d, Name: %s (Player Bound)\n%s",line,x,item[Item_Name],RL);
        else
          Format(line, sizeof(line), "%sSlot: %d, Name: %s\n%s",line,x,item[Item_Name],RL);

        for(new i=1;i<=20;i++)
        {
          switch(i)
          {
            case  1: //weapon speed
            {
              if(cate == 22)
              {
                if(item[Item_ws] >0)
                  Format(line, sizeof(line), "%s, Weapon Speed: %.0f\n",line, item[Item_ws]*100.0);
                else
                  Format(line, sizeof(line), "%s\n",line);
              }
              else
                Format(line, sizeof(line), "%s\n",line);

            }
            case  2:
            {
              if(cate == 22)
              {
                if(item[Item_2hand]==1)
                  Format(line, sizeof(line), "%s2-Handed Weapon\n",line);
                else
                  Format(line, sizeof(line), "%s1-Handed Weapon\n",line);
              }
            }
            case  3: {if((item[Item_weapon_ap] + item[Item_damage])  >0) {Format(line, sizeof(line),         "%s%d Damage\n",line, (item[Item_weapon_ap] + item[Item_damage]));}}                             //weapon_ap+damage
            case  4: {if((item[Item_weapon_sp] + item[Item_spelldmg])>0) {Format(line, sizeof(line),   "%s%d Spell Damage\n",line, (item[Item_weapon_sp] + item[Item_spelldmg]));}}                       //weapon_sp+spelldmg

            case  5: {if(item[Item_armor]    >0) {Format(line, sizeof(line),                     "%s%d Armor\n",line,item[Item_armor])       ;}}  //armor
            case  6: {if(item[Item_haste]    >0) {Format(line, sizeof(line),                     "%s%d Haste\n",line,item[Item_haste])       ;}}  //haste
            case  7: {if(item[Item_agi]      >0) {Format(line, sizeof(line),                   "%s%d Agility\n",line,item[Item_agi])         ;}}  //agi
            case  8: {if(item[Item_int]      >0) {Format(line, sizeof(line),                 "%s%d Intellect\n",line,item[Item_int])         ;}}  //int
            case  9: {if(item[Item_sta]      >0) {Format(line, sizeof(line),                   "%s%d Stamina\n",line,item[Item_sta])         ;}}  //sta
            case 10: {if(item[Item_str]      >0) {Format(line, sizeof(line),                  "%s%d Strength\n",line,item[Item_str])         ;}}  //str

            case 11: {if(item[Item_acc]      >0) {Format(line, sizeof(line),                "%s%.0f Accuracy\n",line,item[Item_acc]*100.0)   ;}}  //accuracy
            case 12: {if(item[Item_as]       >0) {Format(line, sizeof(line),      "%s%.0f Bonus Attack Speed\n",line,item[Item_as]*100.0)    ;}}  //bonus attack speed
            case 13: {if(item[Item_cs]       >0) {Format(line, sizeof(line),             "%s%.0f Crit Chance\n",line,item[Item_cs]*100.0)    ;}}  //crit
            case 14: {if(item[Item_ai]       >0) {Format(line, sizeof(line),              "%s%d Armor Pierce\n",line,item[Item_ai])          ;}}  //armor pierce
            case 15: {if(item[Item_regen]    >0) {Format(line, sizeof(line),   "%s%d/5 Mana Regen per second\n",line,item[Item_regen])       ;}}  //regen
            case 16: {if(item[Item_speed]    >0) {Format(line, sizeof(line),          "%s%.0f Movement Speed\n",line,item[Item_speed]*100.0) ;}}  //movement speed
            case 17: {if(item[Item_res]      >0) {Format(line, sizeof(line),                "%s%d Resistance\n",line,item[Item_res])         ;}}  //res
            case 18: {if(item[Item_resilence]>0) {Format(line, sizeof(line),                "%s%d Resilience\n",line,item[Item_resilence])   ;}}  //resil
            case 19: {if(item[Item_parry]    >0) {Format(line, sizeof(line),                   "%s%.0f Parry\n",line,item[Item_parry]*100.0) ;}}  //parry
            case 20: {Format(line, sizeof(line), "%s\n",line);} //add a new line after every equipment
          }
          if (strlen(line)>950)
          {
            new Handle:pack = CreateDataPack();
            WritePackCell(pack,client);
            WritePackString(pack,line);
            CreateTimer(duration,PrintInvPacket,pack,TIMER_HNDL_CLOSE);
            duration +=0.2;
            Format(line, sizeof(line),"");
          }
        }
      }
      else //non-equipment
      {
        if(amount == -1)
          Format(line,sizeof(line), "%sSlot: %d, Name: %s (Soulbound)\n",line,x,item[Item_Name]);
        else if(amount == 0)
          Format(line,sizeof(line), "%sSlot: %d, Name: %s (Player Bound)\n",line,x,item[Item_Name]);
        else
          Format(line,sizeof(line), "%sSlot: %d, Qty: %d, Name: %s\n",line,x,amount, item[Item_Name]);
      }
    }
    if (strlen(line)>0)
    {
      new Handle:pack = CreateDataPack();
      WritePackCell(pack,client);
      WritePackString(pack,line);
      CreateTimer(duration,PrintInvPacket,pack,TIMER_HNDL_CLOSE);
      duration +=0.2;
      Format(line, sizeof(line),"");
    }

  }
  return;
}

public Action:CmdDrop(client, argc)   {
  if( (client>0) && (client <= GetMaxClients()) ) {
  for(new s=0;s<48;s++)
  {
    new ent=0;
    ent=GetEntDataEnt2(client,g_MyWeaponsOffset+(s*4));
    if(ent>0)
    {
      new String:wepname[64];
      GetEdictClassname(ent,wepname,64);
      new bool:drop=false;
      if(StrContains(wepname,"grenade" ,false)>=0)
        drop=true;
      else if(StrContains(wepname,"bang" ,false)>=0)
        drop=true;
      else if(StrContains(wepname,"c4" ,false)>=0)
        drop=true;
      if(drop)  {
        DropWeapon(client, ent);
      }
    }
  }
  new ent = 0;
  ent=GetPlayerWeaponSlot(client, 1);
  if(ent>0) {
    DropWeapon(client, ent);
  }
  SetEntProp(client, Prop_Send, "m_bHasDefuser", 0, 1);
  }
  else  {
    ReplyToCommand(client, "Command is only for players");
  }
  return Plugin_Handled;
}

DropWeapon(client,weapon)
{
	new Float:angle[3];
	GetClientEyeAngles(client,angle);
	new Float:dir[3];
	GetAngleVectors(angle,dir,NULL_VECTOR,NULL_VECTOR);
	ScaleVector(dir,20.0);
	SDKCall(hWeaponDrop,client,weapon,NULL_VECTOR,dir);
}

public Action:CmdSelfonly(client, argc)   {
   if(TriggerSelfOnly(client))   {
      WCMessage(client,"selfonly enabled",client);
   }
   else
      WCMessage(client,"selfonly disabled",client);

   return Plugin_Handled;
}

public Action:CmdSelfcast(client, argc)   {
   if(TriggerSelfCast(client))   {
      WCMessage(client,"selfcast enabled",client);
   }
   else
      WCMessage(client,"selfcast disabled",client);

   return Plugin_Handled;
}

new Float: g_lastRoll[MAXPLAYERS];

public Action:Roll(client, argc)  {
   if (GetEngineTime() - g_lastRoll[client] >= 5.0)  {
       g_lastRoll[client] = GetEngineTime();
       decl String:name[25];
       GetClientName(client, name, sizeof(name));

       new rolled = GetRandomInt(1,100);

       WCMessage(client, "rolled number you", client, rolled);
       for (new i=1; i<=GetMaxClients(); i++)
         WCMessage(i, "rolled number", i, name, rolled);
   }
   else
      WCMessage(client, "roll cooldown", client, g_lastRoll[client] + 5.0 - GetEngineTime());
   return Plugin_Handled;		
}
public Action:AdminCmd_SpawnChest(client,argc)  {
  TreasureChestMap(1000);
  //PrintToChat(client,"Do not spawn more than 5 chests at once or else it'll bug out!");
  //PrintToChat(client,"If you see this message and you didn't see a chest spawning, it failed at spawning. IT IS NOT A BUG.");
  //PrintToChat(client,"It's intended during treasurehunting event in order to randomize their amount. It only applies to 1+ chests.");
  LogAction(client,
  -1,
  "\"%L\" [WC] Spawned a chest.",
  client);
  return Plugin_Handled;
}
/*
public Action:ServerSay(client, argc)   {
   if(!COMMUNICATION)   {
      ReplyToCommand(client,"%c[WC] %cCommunication is disabled on this server",COLOR_GREEN,COLOR_DEFAULT);
      return Plugin_Handled;
   }
   if(argc<1)  {
      ReplyToCommand(client,"%c[WC] %cSpecify text you want to send",COLOR_GREEN,COLOR_DEFAULT);
      return Plugin_Handled;
   }

   decl String:text[160];
   GetCmdArgString(text, sizeof(text));
   ReplaceString(text, 160, "%", "");

   decl String:name[25];
   GetClientName(client, name, sizeof(name));
	
   decl String:buffer[200];
   Format(buffer,sizeof(buffer),"C_L%s (C_G%sC_L): C_D%s",name,COMM_NICK,text);
   ReplyToCommand(client,"%c[WC] %cYour text is sent",COLOR_GREEN,COLOR_DEFAULT);
   ReplaceString(buffer,sizeof(buffer),"'","",false);
   CommSendMessage(buffer);
   LogAction(client, -1, "%L [WC] triggered sm_s (text %s)", client, text);
	
   return Plugin_Handled;		
}
*/

public Action:CalcArenaForce(client, argc)   {
   ReplyToCommand(client,"%c[WC] %cServer Has Started Arena Calculation",COLOR_GREEN,COLOR_DEFAULT);
   LogAction(client,
			  -1,
			  "\"%L\" [WC] Server has started arena Calculation",
			  client);
   CalcArenaForceSQL();
   ReplyToCommand(client,"%c[WC] %cServer Has Started Rank Calculation",COLOR_GREEN,COLOR_DEFAULT);
   LogAction(client,
			  -1,
			  "\"%L\" [WC] Server has started Rank Calculation",
			  client);
   CalcRankForceSQL();
   return Plugin_Handled;
}

public Action:CalculateArenaSql(client, argc)   {
   if(!MAIN_SERVER)  {
      ReplyToCommand(client,"%c[WC] %cServer is not main. Use wc_calcarena_force on your own risk.",COLOR_GREEN,COLOR_DEFAULT);
      return Plugin_Handled;
   }
   return CalcArenaForce(client, argc);
}

public Action:FadeCmd(client,argc)
{
  decl String:arg[64];
  GetCmdArg(1,arg,64);
  new type=StringToInt(arg);
  GetCmdArg(2,arg,64);
  new time=StringToInt(arg);
  GetCmdArg(3,arg,64);
  new duration=StringToInt(arg);
  new color[4]={255,255,255,230};
  FadeEffect(client,type,time,duration,color);
  return Plugin_Handled;
}
/*
public Action:AdminCmd_MailAll(client,argc)
{
  if(client>0)
  {
  if(GetAdminFlag(GetUserAdmin(client), Admin_Custom6))
  {
  if(argc==4)
  {
    decl String:topic[50];
    GetCmdArg(1,topic,50);
    decl String:Sgold[7];
    GetCmdArg(2,Sgold,sizeof(Sgold));
    new gold=StringToInt(Sgold);
    decl String:Sitem[7];
    GetCmdArg(3,Sitem,sizeof(Sitem));
    new item=StringToInt(Sitem);
    decl String:text[255];
    GetCmdArg(4,text,sizeof(text));
    CreateMail(30,0,"NULL",topic,0,gold,item,text,1);
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin Mail Sent");
    LogAction(client,
			  -1,
			  "\"%L\" [WC] sent mail to everyone with %i gold and %i item ID. Topic: \"%s\"",
			  client,
			  gold,
			  item,
			  topic);
  }
  else
  {
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin Mail Syntax flag6");
  }
  }
  else
  {
  if(argc==2)
  {
    decl String:topic[50];
    GetCmdArg(1,topic,50);
    decl String:text[255];
    GetCmdArg(2,text,sizeof(text));
    CreateMail(30,0,"NULL",topic,0,0,0,text,0);
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin Mail Sent");
    LogAction(client,
			  -1,
			  "\"%L\" [WC] sent mail to everyone with no gold and item. Topic: \"%s\"",
			  client,
			  topic);
  }
  else
  {
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin Mail Syntax flag1");
  }
  }
  }
  else
  {
  if(argc==4)
  {
    decl String:topic[50];
    GetCmdArg(1,topic,50);
    decl String:Sgold[7];
    GetCmdArg(2,Sgold,sizeof(Sgold));
    new gold=StringToInt(Sgold);
    decl String:Sitem[7];
    GetCmdArg(3,Sitem,sizeof(Sitem));
    new item=StringToInt(Sitem);
    decl String:text[255];
    GetCmdArg(4,text,sizeof(text));
    CreateMail(30,0,"NULL",topic,0,gold,item,text,1);
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin Mail Sent");
    LogAction(client,
			  -1,
			  "\"%L\" [WC] sent mail to everyone with %i gold and %i item ID. Topic: \"%s\"",
			  client,
			  gold,
			  item,
			  topic);
  }
  else
  {
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin Mail Syntax flag6");
  }
  }
  return Plugin_Handled;
}
*/

public Action:AdminCmd_Boss(client,argc)
{
  if(argc==1)
  {
    decl String:arg[64];
    GetCmdArg(1,arg,64);
    new con=StringToInt(arg);
    decl String:command[50];
    Format(command,sizeof(command),"wc_bossmod %i",con);
    ServerCommand(command);
    LogAction(client,
			  -1,
			  "\"%L\" [WC] Started boss event.",
			  client);
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin Boss Syntax");
  return Plugin_Handled;
}
public Action:AdminCmd_TreasureHunt(client,argc)
{
  if(argc==1)
  {
    decl String:arg[64];
    GetCmdArg(1,arg,64);
    new con=StringToInt(arg);
    decl String:command[50];
    Format(command,sizeof(command),"wc_treasurehunt %i",con);
    ServerCommand(command);
    LogAction(client,
			  -1,
			  "\"%L\" [WC] Started treasure hunt.",
			  client);
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Treasure Hunt Syntax");
  return Plugin_Handled;
}
public Action:AdminCmd_Ctf(client,argc)
{
  if(argc==1)
  {
    decl String:arg[64];
    GetCmdArg(1,arg,64);
    new con=StringToInt(arg);
    decl String:command[50];
    Format(command,sizeof(command),"wc_ctf %i",con);
    ServerCommand(command);
    LogAction(client,
			  -1,
			  "\"%L\" [WC] Started ctf event.",
			  client);
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Ctf Syntax");
  return Plugin_Handled;
}
public Action:AdminCmd_NoRadar(client, argc)  {
  if(argc==1) {
    decl String:arg[10];
    GetCmdArg(1, arg, 10);
    new con = StringToInt(arg);
    if(con != 0)  {
      LogAction(client, -1, "\"%L\" [WC] Enabled No Radar", client);
      g_noRadar=true;
      for(new i=1;i<65; i++)  {
        if(IsClientInGame(i))
          WCMessage(i, "no radar enabled", i);
      }
    }
    else  {
      LogAction(client, -1, "\"%L\" [WC] Disabled No Radar", client);
      g_noRadar=false;
      for(new i=1;i<65; i++)  {
        if(IsClientInGame(i))
          WCMessage(i, "no radar disabled", i);
      }
    }
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin NoRadar Syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_EndEvent(client,argc)
{
  EventMessage("");
  ReplyToCommand(client, "[WC] Current event message is removed");

  return Plugin_Handled;
}

public Action:AdminCmd_Event(client,argc)
{
  if(argc!=2) {
    ReplyToCommand(client, "[WC] wc_add_event <time> <message> time format is STRICT, Example: wc_add_event \"2010-10-23 17:00:00\" \"Our Event\"");
    return Plugin_Handled;
  }
  decl String:time[50];
  decl String:msg[255];
  GetCmdArg(1,time,50);
  GetCmdArg(2,msg,255);

  decl String:query[500];
  ReplaceString(msg, 255, "'", "");
  Format(query, 500, "INSERT INTO wc_events (`planned`, `msg`, `servId`) VALUES ('%s', '%s', '%d')", time, msg, COMM_ID);
  SQL_TQuery(SSQL,Tinserted,query,754);

  ReplyToCommand(client, "[WC] Adding event to database. No guarantee that there were no errors.");

  return Plugin_Handled;
}


public Action:AdminCmd_Reload(client,argc)
{
  if(argc==1)
  {
    decl String:arg[64];
    GetCmdArg(1,arg,64);
    if(StrEqual(arg,"all"))
    {
      LogAction(client,
			  -1,
			  "\"%L\" [WC] Reloaded wow mod.",
			  client);
      StartReboot(true);
    }
    else
      if(StrEqual(arg,"db"))
      {
        ReloadDBs();
        LogAction(client,
			  -1,
			  "\"%L\" [WC] Reloaded Databases.",
			  client);
      }
      else
      if(StrEqual(arg,"config"))
      {
        ReloadConfig();
        LogAction(client,
			  -1,
			  "\"%L\" [WC] Reloaded Config.",
			  client);
      }
      else
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin reload syntax");
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin reload syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_Reboot(client,argc)
{
  StartReboot();
  return Plugin_Handled;
}

StartReboot(bool:reload=false)
{
  G_RebootQueue=0;
  G_RebootTotal=0;
  for(new x=1;x<=GetMaxClients();x++)
  {
    if(IsClientConnected(x))  if(playerloaded[x]==1)
    {
      G_RebootQueue++;
      G_RebootTotal++;
      wcSavePlayerData(x, true);
    }
  }
  if(!reload)
    CreateTimer(0.4, RebootServer,0, TIMER_REPEAT);
  else
    CreateTimer(0.4, ReloadServer,0, TIMER_REPEAT);
}

public Action:ReloadServer(Handle:timer, any:trash)
{
  if(G_RebootTotal>0)
  {
    new progress=RoundToNearest(float(G_RebootTotal-G_RebootQueue)/float(G_RebootTotal)*100.0);
    for(new x=1;x<=GetMaxClients();x++)
      if(IsClientConnected(x))
        if(IsClientInGame(x))
          WCMessage(x,"reload progress",x,progress);

  }
  if(G_RebootQueue<=0)
  {
    ServerCommand("wcreset");
    return Plugin_Stop;
  }
  return Plugin_Continue;
}

public Action:RebootServer(Handle:timer, any:trash)
{
  if(G_RebootTotal>0)
  {
    new progress=RoundToNearest(float(G_RebootTotal-G_RebootQueue)/float(G_RebootTotal)*100.0);
    for(new x=1;x<=GetMaxClients();x++)
      if(IsClientConnected(x))
        if(IsClientInGame(x))
          WCMessage(x,"reboot progress",x,progress);

  }
  if(G_RebootQueue<=0)
  {
    ServerCommand("_restart");
    return Plugin_Stop;
  }
  return Plugin_Continue;
}

public Action:CmdTargetOnly(client,args)
{
  if(args==1)
  {
    decl String:target[64];
    GetCmdArg(1,target,64);
    new clients[MAXPLAYERS];
    decl String:targetname[64];
    new count=FindMatchingPlayers(target,clients);
    if(count<=0)
    {
      WCMessage(client,"targetonly notfound",client);
      return Plugin_Handled;
    }
    if(!IsBoss())
    {
      WCMessage(client,"targetonly notboss",client);
      return Plugin_Handled;
    }
    for(new i=0;i<count;i++)
    {
      if(IsClientInGame(clients[i]))
      {
        if(GetClientTeam(client)==GetClientTeam(clients[i]))
        {
          GetClientName(clients[i],targetname,64);
          if(g_iTargetOnly[client]==clients[i] && !TriggerTargetOnly(client))
          {
            WCMessage(client,"targetonly disabled",client);
            return Plugin_Handled;
          }
          g_iTargetOnly[client]=clients[i];
          if(CheckTargetOnly(client))
          {
            g_iTargetOnly[client]=clients[i];
            WCMessage(client,"targetonly enabled",client,targetname);
            return Plugin_Handled;
          }
          if(TriggerTargetOnly(client))
          {
            WCMessage(client,"targetonly enabled",client,targetname);
            return Plugin_Handled;
          }
          else
          {
            WCMessage(client,"targetonly disabled",client);
            return Plugin_Handled;
          }
        }
        else
        {
          WCMessage(client,"targetonly wrongteam",client);
          return Plugin_Handled;
        }
      }
    }
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"targetonly syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_AddBoss(client,argc)
{
  if(argc==2)
  {
    decl String:target[64];
    GetCmdArg(1,target,64);
    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(target,clients);
    for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        GetCmdArg(2,target,64);
        new turn=StringToInt(target);
        decl String:adminname[64],String:name[64];
        GetClientName(client,adminname,64);
        GetClientName(clients[x],name,64);
        SetBoss(clients[x],turn);
        PrintToChat(clients[x],"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin boss give",adminname,turn);
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin boss back",name,turn);
        LogAction(client,
			  clients[x],
			  "\"%L\" [WC] made %s a boss number %i.",client,name,turn);
      }
    }
    if(count==0)
      ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm inv player");
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd boss syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_Armor(client,argc)
{
  if(argc==2)
  {
    decl String:target[64];
    GetCmdArg(1,target,64);
    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(target,clients);
    for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        decl String:Samount[8],String:adminname[64],String:name[64];
        GetCmdArg(2,Samount,8);
        new Float:amount=StringToFloat(Samount);
        GetClientName(client,adminname,64);
        GetClientName(clients[x],name,64);
        wcClientsBonusStats[clients[x]][ARMOR]=amount;
        wcRecalculateStats(clients[x]);
        PrintToChat(clients[x],"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin armor give",adminname,amount);
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin armor back",name,amount);
        LogAction(client,
			  clients[x],
			  "\"%L\" [WC] set %s armor to %i value",client,name,amount);
      }
    }
    if(count==0)
      ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm inv player");
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd armor syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_XP(client,argc)
{
  if(argc>=2)
  {
    decl String:target[64];
    GetCmdArg(1,target,64);
    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(target,clients);
    for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        decl String:Samount[8],String:adminname[64],String:name[64];
        GetCmdArg(2,Samount,8);
        new amount=StringToInt(Samount);
        GetClientName(client,adminname,64);
        GetClientName(clients[x],name,64);
        wcGiveXp(clients[x],amount,true);
        PrintToChat(clients[x],"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin XP give",adminname,amount);
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin XP back",name,amount);
        LogAction(client,
			  clients[x],
			  "\"%L\" [WC] gave %s with characterID \"%i\" %i xp",client,name,characterids[clients[x]][0],amount);
      }
    }
    if(count==0)
      ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm inv player");
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd xp syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_Level(client,argc)
{
  if(argc>=2)
  {
    decl String:target[64];
    GetCmdArg(1,target,64);
    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(target,clients);
    for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        decl String:level[8],String:adminname[64],String:name[64];
        GetCmdArg(2,level,8);
        new newlvl=StringToInt(level);
        //if(newlvl<0)
        //  newlvl=0;
        GetClientName(client,adminname,64);
        GetClientName(clients[x],name,64);
        wcSetLevel(clients[x],GetLevel(clients[x])+newlvl);
        PrintToChat(clients[x],"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin lvl give",adminname,newlvl);
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin lvl back",name,newlvl);
        LogAction(client,
			  clients[x],
			  "\"%L\" [WC] gave %s with characterID \"%i\" %i LEVELS",client,name,characterids[clients[x]][0],newlvl);
      }
    }
    if(count==0)
      ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm inv player");
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd lvl syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_Threat(client,argc)   //Is used for experiments
{
  if(argc==2)
  {
    decl String:samount[8];
    GetCmdArg(2,samount,8);
    new amount = StringToInt(samount);
    new String:target[64];
    GetCmdArg(1,target,64);
    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(target,clients);
    for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        SetThreat(clients[x],amount);
      }
    }
    if(count==0)
      ReplyToCommand(client,"No targets match");
    else
      ReplyToCommand(client,"Threat is applied");
  }
  else
    ReplyToCommand(client,"wc_setthreat <target> <amount>");
  return Plugin_Handled;
}

public Action:AdminCmd_FaceTarget(client,argc)   //Testing
{
  if(argc==1)
  {
    new String:target[64];
    GetCmdArg(1,target,64);
    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(target,clients);
    for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        FaceTarget(client,clients[x]);
      }
    }
    if(count==0)
      ReplyToCommand(client,"No targets match");
  }
  else
    ReplyToCommand(client,"Syntax: wc_facetarget <target>");
  return Plugin_Handled;
}

public Action:AdminCmd_Honor(client,argc)
{
  if(argc>=3)
  {
    decl String:action[8],String:target[64];
    GetCmdArg(1,action,8);
    GetCmdArg(2,target,64);
    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(target,clients);
    for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        decl String:amount[8],String:adminname[64],String:name[64];
        GetCmdArg(3,amount,8);
        GetClientName(client,adminname,64);
        GetClientName(clients[x],name,64);
        new old=GetHonor(clients[x]);
        new newamt;
        if(StrEqual(action,"set")||StrEqual(action,"="))
          newamt=StringToInt(amount);
        else if(StrEqual(action,"add")||StrEqual(action,"+"))
          newamt=GetHonor(clients[x])+StringToInt(amount);
        else if(StrEqual(action,"take")||StrEqual(action,"-"))
          newamt=GetHonor(clients[x])-StringToInt(amount);
        else
        {
          ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd honor syntax");
          return Plugin_Handled;
        }
        SetHonor(clients[x],newamt);
        new changed=GetHonor(clients[x])-old;
        LogAction(client,
			  clients[x],
			  "\"%L\" [WC] changed %s with characterID \"%i\" honor by %i",client,name,characterids[clients[x]][0],changed);
        PrintToChat(clients[x],"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin honor changed",adminname,changed);
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin honor back",name,changed);
      }
    }
    if(count==0)
      ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm invalid player");
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd honor syntax");
  return Plugin_Handled;
}

public Action:AngleTest(client,argc)
{
  decl Float:UserA[3],Float:OtherA[3],String:name[50];
  GetClientEyeAngles(client, UserA);
  WCMessage(client,"testmessageF",client,"[AngleTest] User",UserA[1]);
  for(new other=1;other<=GetMaxClients();other++)
  {
    if(IsPlayerAlive(other) && client != other)
    {
      GetClientName(other,name,sizeof(name));
      GetClientEyeAngles(other, OtherA);
      WCMessage(client,"testmessageF",client,name,OtherA[1]);
      new Float:angle=UserA[1]-OtherA[1];
      if(angle<0.0)
        angle *= -1;
      if(angle>180.0)
        angle = 360.0-angle;
      WCMessage(client,"testmessageF",client,"[AngleTest] angle comp",angle);
    }
  }
  return Plugin_Handled;
}

public Action:wcTest(client, argc)
{
  /*decl String:name[40];*/
  /*GetCmdArg(1, name, 40);*/
  /*SQL_EscapeString(SSQL, name, name, 40);*/
  /*decl String:query[255];*/
  /*Format(query, sizeof(query), "UPDATE wc_Players set name = '%s' WHERE playerId = %d", name, playerids[client]);*/
  /*SQL_TQuery(SSQL,  Tinserted, query, 10);*/

  /*WCMessage(client, "testmessage", client, "THIS IS NOT A TEST", 0);*/

  //x         x coordinate, from 0 to 1.  -1.0 is the center.
  //y         y coordinate, from 0 to 1.  -1.0 is the center.
  //holdTime  Number of seconds to hold the text.
  //r         Red color value.
  //g         Green color value.
  //b         Blue color value.
  //a         Alpha transparency value.
  //effect    0/1 causes the text to fade in and fade out. 2 causes the text to flash[?].
  //fxTime    Duration of chosen effect (may not apply to all effects).
  //fadeIn    Number of seconds to spend fading in.
  //fadeOut   Number of seconds to spend fading out.
  //native SetHudTextParams(Float:x,          Float:y,          Float:holdTime, r,    g, b, a,    effect = 0,
  //                        Float:fxTime=6.0, Float:fadeIn=0.1, Float:fadeOut=0.2);
  SetHudTextParams(         -1.0,             -1.0,             10.0,           255,  0, 0, 255,  0,
                             6.0,              0.1,              0.2);

  //client  Client index to send the message to.
  //channel A channel number. If -1, then a channel will automatically be selected based on the least-recently-used channel.  If the channel is any other number, it will be modulo'd with the channel count to get a final channel number.
  //message Message text or formatting rules.
  //...     Message formatting parameters.
  //ShowHudText(client, channel, const String:message[], any:...);
  /*if(ShowHudText(client, -1, "Now THIS is a TEST!")== -1)*/
    /*WCMessage(client,"testmessage",client,"THIS TEST FAILED :(",0);*/


  return Plugin_Handled;
}

public Action:AdminCmd_Gold(client,argc)
{
  if(argc>=4)
  {
    decl String:action[8],String:target[64];
    GetCmdArg(1,action,8);
    GetCmdArg(2,target,64);

    new bool:force=false;
    new iTarget=0;
    if(argc>=5) {
      force=true;
      iTarget = StringToInt(target);
    }

    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(target,clients);
    decl String:reason[100];
    GetCmdArg(4,reason,90);
    Format(reason,sizeof(reason),"Admin Add: %s",reason);
    decl String:escapedReason[255];
    SQL_EscapeString(SSQL, reason, escapedReason, 255);
    if(!force) for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        decl String:amount[8],String:adminname[64],String:name[64];
        GetCmdArg(3,amount,8);
        GetClientName(client,adminname,64);
        GetClientName(clients[x],name,64);
        if(StrEqual(action,"add")||StrEqual(action,"+"))  {
          AddGold(clients[x], StringToInt(amount), "admin", escapedReason);
        }
        else if(StrEqual(action,"take")||StrEqual(action,"-"))
          SpendGold(clients[x], StringToInt(amount), "admin", escapedReason);
        else
        {
          ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd gold syntax");
          return Plugin_Handled;
        }
        new Float:changed=StringToFloat(amount);
        if(StrEqual(action,"take")||StrEqual(action,"-"))
          changed*=-1;
        LogAction(client,
			  clients[x],
			  "\"%L\" [WC] changed %s with characterID \"%i\" GOLD by %.2f",client,name,characterids[clients[x]][0],changed);
        changed = changed / 100.0;
        WCMessage(clients[x],"Admin gold changed",clients[x], adminname,changed);
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin gold back",name,changed);
      }
    }
    else  {
      if( !StrEqual(action,"add") && !StrEqual(action,"+"))  {
        ReplyToCommand(client,"%c[WC] %cOnly add/+ works with player_id",COLOR_GREEN,COLOR_DEFAULT);
        return Plugin_Handled;
      }
      decl String:amount[8];
      GetCmdArg(3,amount,8);
      new changed=StringToInt(amount);
      LogAction(client,
			  client,
			  "\"%L\" [WC] admin with playerId %d gave %d/100 gold to %d player",client,playerids[client],changed,iTarget);
      ReplyToCommand(client,"%c[WC] %cYou gave %d gold to %d playerId",COLOR_GREEN,COLOR_DEFAULT,changed,iTarget);
      addGoldToPlayer(iTarget, changed, "admin", escapedReason);
      return Plugin_Handled;
    }
    if((count==0) && (!force))  {
      new playerId = StringToInt(target);
      if(StrContains(target,"STEAM_",false)>=0)
        playerId = SteamToInt(target);
      if(playerId<=0)
        ReplyToCommand(client,"%c[WC] %cDid not find asked player",COLOR_GREEN,COLOR_DEFAULT);
      else  {
        ReplyToCommand(client,"%c[WC] %cSearching for Player/Character/Steam with id %d",COLOR_GREEN,COLOR_DEFAULT,playerId);
        decl String:query[450];
        Format(query, sizeof(query), "SELECT `last_time`, `name`, wc_Players.`playerId` FROM wc_Players, wc_Characters, wc_PlayerUniqueIds WHERE wc_Players.playerId = wc_Characters.playerId AND wc_PlayerUniqueIds.playerId = wc_Players.playerId AND (CharacterId = %d OR wc_Players.playerId = %d OR wc_PlayerUniqueIds.uniqueId = %d) GROUP BY playerId", playerId, playerId, playerId);
        SQL_TQuery(TMSQL, TGiveGold, query, client);
      }
    }
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd gold syntax");
  return Plugin_Handled;
}

public TGiveGold(Handle:db,Handle:query,String:error[],any:client)
{
  if(client>0 && (!IsClientInGame(client)))
    return;
  ReplyToCommand(client,"%c[WC] %cResults:",COLOR_GREEN,COLOR_DEFAULT);
  if(query)
  {
    SQL_Rewind(query);
    while(SQL_FetchRow(query))
    {
      PrintToServer("found guy");
      new String:date[50];
      GetSQLDataStr(query, "last_time", date, 50);
      new String:name[50];
      GetSQLDataStr(query, "name", name, 50);
      new playerId = GetSQLDataInt(query,"playerId");
      ReplyToCommand(client,"PlayerId: %d Nick: %s Last Login: %s",playerId, name, date);
    }
  }
  else
  {
    ReplyToCommand(client,"Failed loading wc_gold: %s",error);
    LogError ("WC FAILED a.1261 %s",error);
  }
  ReplyToCommand(client,"%c[WC] %End.",COLOR_GREEN,COLOR_DEFAULT);
}

public Action:AdminCmd_Money(client,argc)
{
  if(argc>=3)
  {
    decl String:action[8],String:target[64];
    GetCmdArg(1,action,8);
    GetCmdArg(2,target,64);
    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(target,clients);
    for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        decl String:amount[8],String:adminname[64],String:name[64];
        GetCmdArg(3,amount,8);
        GetClientName(client,adminname,64);
        GetClientName(clients[x],name,64);
        new old=GetMoney(clients[x]);
        new newamt;
        if(StrEqual(action,"set")||StrEqual(action,"="))
          newamt=StringToInt(amount);
        else if(StrEqual(action,"add")||StrEqual(action,"+"))
          newamt=GetMoney(clients[x])+StringToInt(amount);
        else if(StrEqual(action,"take")||StrEqual(action,"-"))
          newamt=GetMoney(clients[x])-StringToInt(amount);
        else
        {
          ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd money syntax");
          return Plugin_Handled;
        }
        wcSetMoney(clients[x],newamt);
        new changed=GetMoney(clients[x])-old;
        LogAction(client,
			  clients[x],
			  "\"%L\" [WC] changed %s with characterID \"%i\" money by %i",client,name,characterids[clients[x]][0],changed);
        PrintToChat(clients[x],"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin money changed",adminname,changed);
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin money back",name,changed);
      }
    }
    if(count==0)
      ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm invalid player");
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd money syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_Give(client,argc)
{
  if(argc>=3)
  {
    decl String:Sitem[20];
    GetCmdArg(2,Sitem,sizeof(Sitem));
    new item=StringToInt(Sitem);
    GetCmdArg(3,Sitem,sizeof(Sitem));
    new iamount=StringToInt(Sitem);
    if(iamount<=0)
      iamount=1;
    new uitem[UniqueItem];
    GetItemProperties(uitem, item, 0);
    if((item>0) && CheckIfItem(item))
    {
      decl String:target[64];
      GetCmdArg(1,target,64);
      new clients[MAXPLAYERS];
      new count=FindMatchingPlayers(target,clients);
      for(new x=0;x<count;x++)
      {
        if(IsClientInGame(clients[x]))
        {
        if(GiveItem(clients[x], uitem, iamount, "admin") >= 0)
        {
          decl String:adminname[64],String:name[64],String:itemname[255];
          GetItemName(item,itemname);
          GetClientName(client,adminname,64);
          GetClientName(clients[x],name,64);
          LogAction(client,
			     clients[x],
			     "\"%L\" [WC] gave %s with characterID \"%i\" item id %i name %s amount %i",client,name,characterids[clients[x]][0],item,itemname,iamount);
          PrintToChat(clients[x],"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin gave item",adminname,itemname,iamount);
          ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin gave back",name,itemname,iamount);
        }
        else
        {
          decl String:name[64];
          GetClientName(clients[x],name,64);
          ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin player no free slot",name);
        }
        }
      }
      if(count==0)
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm invalid player");
    }
    else
      ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"admin wrong item");
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd give syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_Take(client,argc)
{
  if(argc>=3)
  {
    decl String:Sitem[20];
    GetCmdArg(2,Sitem,sizeof(Sitem));
    new item=StringToInt(Sitem);
    GetCmdArg(3,Sitem,sizeof(Sitem));
    new iamount=StringToInt(Sitem);
    if(iamount<=0)
      iamount=1;
    if((item>0) && CheckIfItem(item))
    {
      decl String:target[64];
      GetCmdArg(1,target,64);
      new clients[MAXPLAYERS];
      new count=FindMatchingPlayers(target,clients);
      for(new x=0;x<count;x++)
      {
        if(IsClientInGame(clients[x]))
        {

          new remove = GetMinInt(iamount, GetAmountOfItems(clients[x],item));
          if(!RemoveItem(clients[x],item,remove))
            LogError("[WC] Error removing item!!");
          decl String:adminname[64],String:name[64],String:itemname[255];
          GetItemName(item,itemname);
          GetClientName(client,adminname,64);
          GetClientName(clients[x],name,64);
          LogAction(client,
			     clients[x],
			     "\"%L\" [WC] took %s with characterID \"%i\" item id %i name %s amount %i",client,name,characterids[clients[x]][0],item,itemname,remove);
          PrintToChat(clients[x],"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin took item",adminname,itemname,remove);
          ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin took back",name,itemname,remove);
        }
      }
      if(count==0)
        ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm invalid player");
    }
    else
      ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"admin wrong item");
  }
  else
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"adm cmd take syntax");
  return Plugin_Handled;
}

public Action:AdminCmd_reload_pds(client, argc)
{
  reloadPDs();
  ReplyToCommand(client,"[wow] Premium Data is reloading..");
  return Plugin_Handled;
}

public Action:AdminCmd_debug_premium(client, argc)
{
  debugOutdatedPDs(client);
  return Plugin_Handled;
}

public Action:AdminCmd_debug_fog(client, argc)
{
  Darken(client);
  return Plugin_Handled;
}

public Action:AdminCmd_debug_talents(client, argc)
{
  if(argc>=2)
  {
    decl String:argue[255];
    GetCmdArg(2,argue,sizeof(argue));
    new amount=StringToInt(argue);
    GetCmdArg(1,argue,sizeof(argue));
    new clients[MAXPLAYERS];
    new count=FindMatchingPlayers(argue,clients);
    for(new x=0;x<count;x++)
    {
      if(IsClientInGame(clients[x]))
      {
        setTPoints(clients[x], getActiveSpec(clients[x]) , getTPoints(clients[x], getActiveSpec(clients[x]))+amount);
        ReplyToCommand(client,"%c[WC] %cGave %i talents to %t",COLOR_GREEN,COLOR_DEFAULT,amount,argue);
      }
    }
  }
  else
    ReplyToCommand(client,"%c[WC] %c%s",COLOR_GREEN,COLOR_DEFAULT,"usage: wc_debug_give_talent 'target' 'amount'");
  return Plugin_Handled;
}

public Action:HNSModCmd(client, argc)
{
  if(argc != 5)  {
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"admin hns cmd");
    return Plugin_Handled;
  }
  decl String:arg[30];
  GetCmdArg(1, arg, sizeof(arg));
  new mode = StringToInt(arg);
  GetCmdArg(2, arg, sizeof(arg));
  new gold = StringToInt(arg);
  GetCmdArg(3, arg, sizeof(arg));
  new xp = StringToInt(arg);
  GetCmdArg(4, arg, sizeof(arg));
  new item = StringToInt(arg);
  GetCmdArg(5, arg, sizeof(arg));
  new amount = StringToInt(arg);

  if(mode != 1 && mode != 2)  {
    TriggerPlugins(true);
    SetHNS(false);
    for(new x=1;x<=GetMaxClients();x++)
      if(IsClientInGame(x))
        WCMessage(x,"Announce HNS0",x);
    return Plugin_Handled;
  }

  TriggerPlugins(false);
  SetHNS(true);
  if(mode == 1) {
    for(new x=1;x<=GetMaxClients();x++)
      if(IsClientInGame(x))
        WCMessage(x,"Announce HNS",x);
  }
  if(mode == 2) {
    for(new x=1;x<=GetMaxClients();x++)
      if(IsClientInGame(x))
        WCMessage(x,"Announce HNS1",x);
  }

  g_hns_mode= mode;
  if(gold > 100)  gold = 100;
  g_hns_gold= gold;
  g_hns_xp= xp;
  g_hns_itemId= item;
  g_hns_itemAmount= amount;

  return Plugin_Handled;
}
