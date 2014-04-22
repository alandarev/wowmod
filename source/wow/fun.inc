/* ========================================================================== */
/*                                                                            */
/*   Fun.inc                                                                  */
/*   (c) 2008 Stinkyfax                                                       */
/*                                                                            */
/*   Fun additions to the main mod                                            */
/*                                                                            */
/* ========================================================================== */


// The Quiz functions

new String:g_quizQuestion[100], g_quizItemId, g_quizXP, g_quizItemAmount, g_quizCorrectId;
new g_quizSilver;
new Float:g_fQuizTime;
new g_iQuizCAnswer[65];
new Handle:g_hQuizMenu=INVALID_HANDLE;


//Heroic Mod
new bool:g_heroic=false;

bool:IsHeroic() {
  return g_heroic;
}


SetHeroic(bool:value)  {
  g_heroic = value;
}
new bool:g_treasurehunt=false;
new bool:g_ctf=false;

bool:IsCtf() {
  return g_ctf;
}


SetCtf(bool:value)  {
  g_ctf = value;
}


bool:IsTreasureHunt() {
  return g_treasurehunt;
}


SetTreasureHunt(bool:value)  {
  g_treasurehunt = value;
}


new bool:g_boss=false;
bool:IsBoss() {
  return g_boss;
}

SetBossGlobal(bool:value) {
  g_boss = value;
}

bool:IsHNS() {
  return g_hns;
}

SetHNS(bool:value) {
  g_hns = value;
}

stock HNSWon(who) {
  //0 - nobody, 1 - alive CT's, 2 - all CT's
  new String:players[60];
  switch(who) {
    case 0:
      players = "Nobody";
    case 2: players = "Everyone";
    case 1: {
      for(new i=1; i<=GetMaxClients(); i++) {
        if(IsClientConnected(i) && IsClientInGame(i) && GetClientTeam(i) == 3 && playerloaded[i] == 1 && IsPlayerAlive(i)) {
          decl String:name[10];
          GetClientName(i, name, sizeof(name));
          Format(players, sizeof(players),"%s%s ", players, name);
        }
      }
    }
  }
  for(new i=1; i<=GetMaxClients(); i++) {
    if(IsClientConnected(i) && IsClientInGame(i)) {
      if (StrEqual(players, "Nobody"))
        Format(players,sizeof(players),"%T","Nobody",i);
      if (StrEqual(players, "Everyone"))
        Format(players,sizeof(players),"%T","All",i);
      WCMessage(i, "hns won", i, players);

      if(GetClientTeam(i) == 3 && playerloaded[i] == 1) {
        if(who == 2 || (who == 1 && IsPlayerAlive(i))) {

          //You won!
          decl String:wins[100];
          new bool:first=true;
          if(g_hns_gold > 0)  {
            decl String:gold[20];
            IntToStringfloat(g_hns_gold, gold, 20);
            Format(wins, sizeof(wins), "%s %T", gold, "gold", i);
            first=false;
            AddGold(i, g_hns_gold, "event", "Hide and Seek event");
          }
          if(g_hns_xp > 0)  {
            if(!first)  {
              Format(wins, sizeof(wins), "%s, ", wins);
            }
            Format(wins, sizeof(wins), "%s%d %T", wins, g_hns_xp, "xp", i);
            first = false;
            wcGiveXp(i,g_hns_xp,true);
          }
          if(g_hns_itemId > 0 && g_hns_itemAmount > 0) {
            if(!first)  {
              Format(wins, sizeof(wins), "%s, ", wins);
            }
            new uitem[UniqueItem];
            GetItemProperties(uitem, g_hns_itemId);
            Format(wins, sizeof(wins), "%s%s x%d", wins, uitem[Item_Name], g_hns_itemAmount);
            first = false;
            if(GiveItem(i, uitem, g_hns_itemAmount, "hns") < 0)
              WCMessage(i,"failed drop",i,uitem[Item_Name]);

          }

          WCMessage(i, "hns you won", i, wins);

        }
        else if(who == 1 && !IsPlayerAlive(i))  {
          wcGiveXp(i,g_hns_xp,true);
        }
      }
    }
  }


}




//threat
new g_iAggro;

public PreloadWeaponsMenu() {
	//not implemented yet.

}

stock Darken(client)  {
  new sprite = CreateEntityByName("env_fog_controller");
  if (IsValidEntity(sprite)) {
      DispatchKeyValue(sprite, "fogcolor", "0 0 0");
      DispatchKeyValue(sprite, "fogend", "4500");
      DispatchKeyValue(sprite, "fogenable", "1");
      DispatchKeyValue(sprite, "spawnflag", "1");

      DispatchSpawn(sprite);

      AcceptEntityInput(sprite, "TurnOn");

  }
  else
    LogError("error 51: could not create sprite");
}


public Action:RunQuiz(client, args)
{
   if (args < 8)
	{
		ReplyToCommand(client, "[WC] Usage: wc_quiz <question> <sec> <itemID> <amount> <XP> <Silver> [Answer1 - correct] [Answer2] ... [Answer8]");
		return Plugin_Handled;	
	}
	
   if (IsQuizInProgress())
	{
		ReplyToCommand(client, "[WC] Quiz in Progress");
		return Plugin_Handled;
	}
	
   decl String:text[500];
   GetCmdArgString(text, sizeof(text));

   new answerCount;	
   new len = BreakString(text, g_quizQuestion, sizeof(g_quizQuestion));

   new String:sItemId[20];
   new pos = BreakString(text[len], sItemId, sizeof(sItemId));
   if (pos == -1)
	   return Plugin_Handled;
   len += pos;
   new sec = StringToInt(sItemId);
	

   pos = BreakString(text[len], sItemId, sizeof(sItemId));
   if (pos == -1)
	   return Plugin_Handled;
   len += pos;
   g_quizItemId = StringToInt(sItemId);
	
   pos = BreakString(text[len], sItemId, sizeof(sItemId));
   if (pos == -1)
	   return Plugin_Handled;
   len += pos;
   g_quizItemAmount = StringToInt(sItemId);

   pos = BreakString(text[len], sItemId, sizeof(sItemId));
   if (pos == -1)
	   return Plugin_Handled;
   len += pos;
   g_quizXP = StringToInt(sItemId);

   pos = BreakString(text[len], sItemId, sizeof(sItemId));
   if (pos == -1)
	   return Plugin_Handled;
   len += pos;
   g_quizSilver = StringToInt(sItemId);
	
   if(!CheckIfItem(g_quizItemId)) {
    g_quizItemId=0;
	}

   if(((g_quizItemAmount <= 0) || (g_quizItemAmount > GetItemMaxAmount(g_quizItemId))) && (g_quizItemId>0)) {
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"admin wrong item amount",g_quizItemAmount,GetItemMaxAmount(g_quizItemId));
    return Plugin_Handled;
	}

   g_quizCorrectId = 0;

   new String:answers[10][255];
   while (pos != -1 && answerCount < 8)
	{	
      decl String:temp[255];
      pos = BreakString(text[len], temp, sizeof(temp));
      answers[answerCount]=temp;
      answerCount++;
		
      if (pos != -1)
		{
			len += pos;
		}	
	}

	// Shuffling array
   for (new x=1;x<answerCount;x++) {
    new y=GetRandomInt(x,answerCount-1);
    decl String:temp[255];
    temp=answers[x];
    answers[x]=answers[y];
    answers[y]=temp;
	}
   g_quizCorrectId = GetRandomInt(0, answerCount - 1);
   new String:temp[255];
   temp=answers[0];
   answers[0]=answers[g_quizCorrectId];
   answers[g_quizCorrectId]=temp;
   if(answerCount<2) {
    ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin quiz not enough answers");
    return Plugin_Handled;
   }
   if(g_hQuizMenu != INVALID_HANDLE)
    QuizMenuClose();
   ReplyToCommand(client,"%c[WC] %c%t",COLOR_GREEN,COLOR_DEFAULT,"Admin quiz started");
   decl String:item_name[255];
   if(g_quizItemId>0)
      GetItemName(g_quizItemId,item_name);
   else
      item_name = "No Item";
   LogAction(client,
			  -1,
			  "\"%L\" [WC] Created Quiz with Item %i (%s) x%i and %i XP %i Silver",
			  client,
			  g_quizItemId,
           item_name,
           g_quizItemAmount,
           g_quizXP,
           g_quizSilver);
   g_hQuizMenu = CreateMenu(Handler_QuizCallback, MenuAction:MENU_ACTIONS_ALL);
   SetMenuTitle(g_hQuizMenu, g_quizQuestion);
	
   for (new i = 0; i < answerCount; i++)
	{
	  AddMenuItem(g_hQuizMenu, answers[i], answers[i]);
	}
   SetMenuExitBackButton(g_hQuizMenu, false);
   SetMenuExitButton(g_hQuizMenu, false);
	
   for (new i=1;i<=GetMaxClients();i++) {
	 if(IsClientConnected(i))
	     DisplayMenu(g_hQuizMenu, i, sec);
	}
   CreateTimer(float(sec)+0.2, EndQuiz, 0);
   g_fQuizTime=GetEngineTime()+float(sec);
   return Plugin_Handled;	
}

public Action:EndQuiz(Handle:timer, any:trash)
{
   new total,totalC;
   new uitem[UniqueItem];
   if(g_quizItemId>0)
    GetItemProperties(uitem, g_quizItemId);
   for(new i=1;i<=GetMaxClients();i++) {
      if(IsClientConnected(i))   if(IsClientInGame(i)) {
         decl String:uAnswer[255],String:rAnswer[255];
         if (g_iQuizCAnswer[i] >= 0) {
            total++;
            GetMenuItem(g_hQuizMenu,g_iQuizCAnswer[i], uAnswer, sizeof(uAnswer) );
         }
         else
            Format(uAnswer,sizeof(uAnswer), "%T","Q no answer", i);
         GetMenuItem(g_hQuizMenu,g_quizCorrectId, rAnswer, sizeof(rAnswer) );
         WCMessage(i,"Q right answer",i, rAnswer, uAnswer);
         if (g_iQuizCAnswer[i] == g_quizCorrectId) {
            totalC++;
            decl String:itemName[255];
            if(g_quizItemId>0)
               GetItemName(g_quizItemId,itemName);
            else
               itemName="No Item";
            if(g_quizItemId>0)   {
               if(g_quizXP<=0)
                  WCMessage(i,"Q won item",i,itemName,g_quizItemAmount,g_quizSilver);
               else
                  WCMessage(i,"Q won item xp",i,itemName,g_quizItemAmount,g_quizXP, g_quizSilver);
            }
            else
               WCMessage(i,"Q won xp",i,g_quizXP, g_quizSilver);
            if(g_quizItemId>0)   {
               if(GiveItem(i, uitem, g_quizItemAmount, "quiz") < 0)
                  WCMessage(i,"failed drop",i,itemName);
            }
            if(g_quizXP > 0)  {
               wcGiveXp(i,g_quizXP,true);
            }
            if(g_quizSilver > 0)  {
              if(g_quizSilver > 10000)
                g_quizSilver = 10000;
              wcSetMoney(i,GetMoney(i)+g_quizSilver);
            }
         }
      }
   }
   for(new i=1;i<=GetMaxClients();i++) {
      if(IsClientConnected(i)) if(IsClientInGame(i))
         WCMessage(i,"Q total won",i,totalC,total);
   }
   QuizMenuClose();
   return Plugin_Stop;
}

public Handler_QuizCallback(Handle:menu,MenuAction:action,client,slot)
{
   if(action==MenuAction_Select && IsClientConnected(client))
   {
      decl String:buffer[255];
      GetMenuItem(g_hQuizMenu,slot,buffer,sizeof(buffer));
      WCMessage(client,"Q You answered",client,buffer);
      g_iQuizCAnswer[client] = slot;
   }
   return 0;
}

QuizMenuClose()
{
	CloseHandle(g_hQuizMenu);
	g_hQuizMenu = INVALID_HANDLE;
	for (new i=1;i<=64;i++) {
	   g_iQuizCAnswer[i] = -1;
	}
}

IsQuizInProgress() {
   if(g_hQuizMenu==INVALID_HANDLE)
      return false;
   return true;
}

GetBosses(indexes[]) {
   new total=0;
   for(new x=1;x<=GetMaxClients();x++)
      if(IsClientConnected(x) && IsClientInGame(x))
      {
         if(GetBoss(x)>0)
         {
            indexes[total]=x;
            total++;
         }
      }
   return total;
}

//Threat System, since 1.13beta
bool:IsThreatEnabled(client)  {
   return g_bThreat[client];
}

GetThreatEnabledInt(client)   {
   if(IsThreatEnabled(client))
      return 1;
   return 0;
}

bool:TriggerThreat(client) {
   g_bThreat[client] = !g_bThreat[client];
   return g_bThreat[client];
}
SetThreatSettings(client, bool:val)   {
   g_bThreat[client]=val;
}

SetThreatSettingsInt(client, val)   {
   if(val==0)
      g_bThreat[client]=false;
   else
      g_bThreat[client]=true;
}

GetThreat(client) {
   return g_iThreat[client];
}

stock SetThreat(client,amount)   {
   g_iThreat[client]=amount;
}

stock AddThreat(client,amount)   {
   if(-amount>GetThreat(client))
      SetThreat(client,0);
   else {
      if(GetSealT(client)>GetTickedTime() && HasSeal(client,Seal:SEAL_Righteousness) )
        amount = RoundToNearest(amount * 1.8);
      g_iThreat[client]+=amount;
   }
}

new g_iAggroTable[65];

ThreatTop(clients[],size)  {
   new amount=0;
   for(new i=0;i<size;i++)
      clients[i]=0;

   new bool:found = true;
   while(found)   {
      found=false;
      new max=-1;
      new maxC=0;
      for(new i=1;i<=GetMaxClients();i++)
         if(IsClientInGame(i) && IsPlayerAlive(i) && (playerloaded[i]==1) && (GetBoss(i)==0)) {
            new bool:already=false;
            for(new y=0;y<amount;y++)
               if(clients[y]==i) {
                  already=true;
                  break;
               }
            if(!already)   {
               if(max<GetThreat(i))  {
                  max = GetThreat(i);
                  maxC = i;
                  found=true;
               }
            }
         }
      if(found)
         clients[amount++]=maxC;
      if(amount==size)
         found=false;   //End coz of size is reached
   }
   g_iAggro=clients[0];
   for(new i=0; i<65; i++)  {
    g_iAggroTable[i]=0;
    if(i<amount)
      g_iAggroTable[i]=clients[i];
   }
   return amount;
}

new g_iTopThreat[5];

bool:TopThreat(client, max) {
  for(new i=0;i<max;i++)  {
    if(g_iTopThreat[i]==client)
      return true;
  }
  return false;
}

Handle:BuildThreatTable() {
   new Handle:panel = CreatePanel();
   new String:nameSpace[25];
   Format(nameSpace,25,"                         ");
   decl String:temp[255];
   Format(temp, sizeof(temp), "%T", "panel threat", LANG_SERVER);
   SetPanelTitle(panel, temp);
   Format(temp, sizeof(temp), "%T", "panel threat1", LANG_SERVER);
   DrawPanelText(panel, temp);

   new size = ThreatTop(g_iTopThreat, 5);
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   for(new i=0;i<size;i++) {
      decl String:name[20];
      GetClientName(g_iTopThreat[i],name,sizeof(name));
      decl String:name2[25];
      Format(name2,sizeof(name2), "%s%s", name, nameSpace);
      Format(temp,sizeof(temp), "%s%i", name2, GetThreat(g_iTopThreat[i]));
      DrawPanelItem(panel, temp, ITEMDRAW_DISABLED);
   }

   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);

   Format(temp,sizeof(temp),"%T", "threat disable",LANG_SERVER);
   DrawPanelItem(panel, temp);
   return panel;
}

public ThreatPanelHandler(Handle:menu, MenuAction:action, client, slot)
{
	if (action == MenuAction_Select)
	{
		WCMessage(client,"threat going off",client);
		SetThreatSettings(client, false);
	} else if (action == MenuAction_Cancel) {
	}
}

SendThreatPanel(duration) {
   new Handle:panel = BuildThreatTable();
   for(new i=1;i<=GetMaxClients();i++)  {
      if(IsClientInGame(i) && (playerloaded[i]==1))   {
         if(IsThreatEnabled(i))  {
            CancelClientMenu(i);
            SendPanelToClient(panel, i, ThreatPanelHandler, duration);
         }
      }
   }
   CloseHandle(panel);
}

public Action:ThreatTimer(Handle:timer, any:temp)
{
   if(IsBoss())  {
      SendThreatPanel(2);
      BossDamage();
   }
   return Plugin_Continue;
}

// Threat damage shall not stop if it has started
new g_iThreatOngoing = -1;

BossDamage()   {
  new online = PlayersOnline();
  new boss;
  for(new i=1;i<=GetMaxClients();i++)  {
    if(IsClientInGame(i) && IsPlayerAlive(i) && (playerloaded[i]==1))
    if((boss = GetBoss(i)) > 0)  {
      KvRewind(wcbossdrop);
      decl String:temp[9];
      IntToString(boss,temp,sizeof(temp));
      KvJumpToKey(wcbossdrop,temp);
      new bool:enraged=false;
      new Float:perc = float(wcGetHealth(i)) / GetMaxHealth(i);
      if(perc < KvGetFloat(wcbossdrop, "enrage_level", 0.0))
      enraged = true;

      CheckEnrageChange(i, enraged);

      new dmg = KvGetNum(wcbossdrop,"dmg", 0);
      new Float:multi = KvGetFloat(wcbossdrop, "multi", 0.0);
      new targets = KvGetNum(wcbossdrop,"targets",1);
      if(enraged)  {
        new tmp = KvGetNum(wcbossdrop,"enrage_dmg", 0);
        if(tmp>dmg)
        dmg=tmp;
        new Float:ftmp = KvGetFloat(wcbossdrop, "enrage_multi", 0.0);
        if(ftmp > multi)
        multi = ftmp;
        tmp = KvGetNum(wcbossdrop, "enrage_targets", 1);
        if(tmp > targets)
        targets = tmp;
      }
      dmg += RoundToNearest(multi * online);
      if(dmg<=0)
      continue;
      //BossTryHit(i,dmg, g_iAggroTable[0]);
      for(new target=0; target < targets; target ++) {
        BossTryHit(i, dmg, g_iAggroTable[target]);
      }
    }
  }
}

new bool:g_ff_checkEnrage[65];
CheckEnrageChange(client, bool:stage)  {
  if( (!g_ff_checkEnrage[client]) && (stage) )
    AnnounceEnrage(client);
  g_ff_checkEnrage[client]=stage;
}

AnnounceEnrage(client)  {
  decl String:name[40];
  GetClientName(client, name, 40);
  for(new i=1;i<=GetMaxClients(); i++)  {
    if(IsClientInGame(i))
      WCMessage(i, "announce enrage", i, name);
  }
  decl Float:location[3];
  GetClientAbsOrigin(client,location);
  EmitAmbientSound("wc/spells/boss_enrage.wav", location);
}


BossTryHit(boss, dmg, target)   {
  new bool:ignoreLimit = (g_iThreatOngoing == g_RoundCount);
  if((target > 0) && IsClientInGame(target) && IsPlayerAlive(target) && (GetThreat(target)>=MIN_THREAT || ignoreLimit))   {
    g_iThreatOngoing = g_RoundCount;
    BossShoot(boss,target,dmg);
    decl Float:loc1[3],Float:loc2[3];
    GetClientAbsOrigin(boss,loc1);
    GetClientAbsOrigin(target,loc2);
    loc1[2]+=25.0;
    loc2[2]+=25.0;
    BeamEffect("@all",loc1,loc2,g_ef_tp_beam001,5,1.0,7.0,10.0,{100, 100, 100, 255},0.0,5);
  }
}

BossShoot(boss, target, dmg)  {
   if(UnableToAttack[boss])  {
      wc_iPlayerHurtEvent=0;
      wc_aPlayerHurtEvent=0;
      return;
   }
   if(UnableToVictim[target])
   {
      return;
   }
   //Show effect
   decl Float:position[3];
   GetClientAbsOrigin(target, position);
   position[2]+=170;
   VS_GlowSprite(position, g_ef_train, 1.00, 1.00, 255, 0.0);

   wc_iPlayerHurtEvent=0;
   wc_aPlayerHurtEvent=0;
   new Handle:event=CreateEvent("player_hurt");
   SetEventInt(event,"userid",GetClientUserId(target));
   SetEventInt(event,"attacker",GetClientUserId(boss));
   SetEventInt(event,"dmg_health",dmg);
   SetEventString(event,"weapon","knife");
   SetEventInt(event,"hitgroup",2);
   FireEvent(event);
}

stock PlayersOnline()   {
   new amount=0;
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         amount++;
   return amount;
}

GenerateDMGThreat(attacker, boss, dmg) {
   if(GetBoss(boss)==0)
      return;
   new cl = GetClass(attacker);
   if(cl == 1)   {
      //Def Stance
      if(FindEffect(attacker, 10))
         dmg = RoundToNearest(dmg * 2.0);
   }
   //rogues -10% threat
   else if(cl == 2)
      dmg = RoundToNearest(dmg*0.9);
   if(GetMonkStance(attacker)==1)
      dmg = RoundToNearest(dmg * 3.0);
   dmg = RoundToNearest(dmg * DMG_THREAT);
   AddThreat(attacker, dmg);
}

GenerateHealThreat(healer, hp) {
   if(!IsBoss())
      return;
   new cl = GetClass(healer);
   if(cl == 5)   {
      //priests -20% from healing
      hp = RoundToNearest(hp*0.8);
   }
   hp = RoundToNearest(hp * HEAL_THREAT);
   AddThreat(healer, hp);
}


EventMessage(const String:msg[])  {
  if(strlen(msg)>4) {
    strcopy(g_sEventMessage, sizeof(g_sEventMessage), msg);
    TellEvent();
  }
  else  {
    EventMessageClean();
    for(new x=1;x<=GetMaxClients();x++)
      if(IsClientInGame(x))
        /*PlainMessage(x, "@lightgreenEvent @defaultis Over");*/
        WCMessage(x, "event is over", x);
  }
}

EventMessageClean()  {
  g_sEventMessage="";
}

TellEvent() {
   for(new x=1;x<=GetMaxClients();x++) {
    if(IsClientInGame(x))
      PlainMessage(x, g_sEventMessage);
   }
}


stock SetClientModel(client)  {
  if(GetClass(client) <= 0)
    return;
  if(!IsPlayerAlive(client))
    return;

  new team = GetClientTeam(client);
  new race = GetClass(client);

  if((team != 2) && (team != 3))
    return;

  new String:path[255];
  GetModelPath(team,race,path);
  if(strlen(path) < 3)
    return;

  /*SetEntityModel(client, path);*/
}

//warrior
//rogue
//mage
//paladin
//priest
//shaman
//warlock
//DK
stock GetModelPath(team, race, String:path[255])  {
  switch(team)  {
   case 2:
      switch(race)  {
        case 1: Format(path, sizeof(path), "models/player/wowmodels/teacher/warrior_t.mdl");
        case 2: Format(path, sizeof(path), "models/player/wowmodels/teacher/rogue_t.mdl");
        case 3: Format(path, sizeof(path), "models/player/wowmodels/teacher/mage_t.mdl");
        case 4: Format(path, sizeof(path), "models/player/wowmodels/teacher/paladin_t.mdl");
        case 5: Format(path, sizeof(path), "models/player/wowmodels/teacher/priest_t.mdl");
        case 6: Format(path, sizeof(path), "models/player/wowmodels/teacher/shaman_t.mdl");
        case 7: Format(path, sizeof(path), "models/player/wowmodels/teacher/warlock_t.mdl");
        case 8: Format(path, sizeof(path), "models/player/wowmodels/teacher/deathknight_t.mdl");

      }
   case 3:
      switch(race)  {
        case 1: Format(path, sizeof(path), "models/player/wowmodels/teacher/warrior_ct.mdl");
        case 2: Format(path, sizeof(path), "models/player/wowmodels/teacher/rogue_ct.mdl");
        case 3: Format(path, sizeof(path), "models/player/wowmodels/teacher/mage_ct.mdl");
        case 4: Format(path, sizeof(path), "models/player/wowmodels/teacher/paladin_ct.mdl");
        case 5: Format(path, sizeof(path), "models/player/wowmodels/teacher/priest_ct.mdl");
        case 6: Format(path, sizeof(path), "models/player/wowmodels/teacher/shaman_ct.mdl");
        case 7: Format(path, sizeof(path), "models/player/wowmodels/teacher/warlock_ct.mdl");
        case 8: Format(path, sizeof(path), "models/player/wowmodels/teacher/deathknight_ct.mdl");
      }
  }
}


stock TriggerPlugins(bool:enable) {
  new Handle:tb=FindConVar(TEAMBALANCE_COMMAND);
  if(tb!=INVALID_HANDLE)  {
    SetConVarBool(tb, enable);
    CloseHandle(tb);
  }
  new Handle:immunity=FindConVar("sm_immunity_mode");
  if(immunity!=INVALID_HANDLE)  {
    if(enable)  {
      SetConVarInt(immunity, g_oldImmunity);
    }
    else  {
      g_oldImmunity=GetConVarInt(immunity);
      SetConVarInt(immunity, 0);
    }
    CloseHandle(immunity);
  }
  new Handle:scramble=FindConVar("ats");
  if(scramble != INVALID_HANDLE)  {
    SetConVarBool(scramble, enable);
    CloseHandle(scramble);
  }
  if(!enable) {
    ServerCommand("log off");
  }
  else  {
    ServerCommand("log on");
  }

}

//new g_iTreasureChestName = 0; //Changed into non-global
//new g_iTreasureChestRarity[65]; // replaced with checkrarity
new g_iTreasureChestRound = 0;
new g_iTaken= 0;
new g_iTakenb= 0;
new g_iTakenc= 0;
new g_iTakend= 0;
new g_iTakene= 0;
new g_iTakenf= 0;
new g_iTakeng= 0;
new g_iTakenh= 0;
new g_iTakeni= 0;
new g_iTakenj= 0;
new g_iTakenk= 0;
new g_iTakenl= 0;
new g_iTakenm= 0;
new g_iTakenn= 0;
new g_iTakeno= 0;
new g_iTreasureChestNumber=-1;

stock TreasureChestMap(chance, times=1)
{
  chance = RoundToCeil(float(chance) * float(PlayersInTeams(true)) * 0.20);
  new spawnedChests = 0;
  for(new iTimes=0; iTimes < times; iTimes++) {
    new randomchance = GetRandomInt(1,100);
    if(chance>=randomchance)
    {
      new TreasureChestName = 0;
      decl String:sMap[20];
      decl Float:location[3];
      spawnedChests++;
      GetCurrentMap(sMap,sizeof(sMap));
      if(StrEqual(sMap,"de_dust2",false))  {
        new random=GetRandomInt(1,11);
        if(random==1 && g_iTaken==0) {
          location[0]=-2072.9;
          location[1]=1301.2;
          location[2]=159.2;
          TreasureChestName = 2; //999
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=-1195.5;
          location[1]=1263.9;
          location[2]=48.9;
          TreasureChestName = 2; //Tunnel
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=-272.0;
          location[1]=1429.3;
          location[2]=-31.9;
          TreasureChestName = 3; //Double Doors
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=-860.1;
          location[1]=2572.4;
          location[2]=-46.8;
          TreasureChestName = 4; //Bombsite B
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=-2063.5;
          location[1]=3119.9;
          location[2]=160.0;
          TreasureChestName = 4; //Bombsite B
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=495.9;
          location[1]=1368.0;
          location[2]=96.0;
          TreasureChestName = 6; //Middle
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=1438.5;
          location[1]=188.8;
          location[2]=-190.5;
          TreasureChestName = 7; //Side
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=733.5;
          location[1]=680.0;
          location[2]=96.0;
          TreasureChestName = 8; //Side Door
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=-572.7;
          location[1]=649.6;
          location[2]=160.0;
          TreasureChestName = 6;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=-828.4;
          location[1]=1362.2;
          location[2]=16.0;
          TreasureChestName = 2;
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=690.9;
          location[1]=2763.8;
          location[2]=224.0;
          TreasureChestName = 9; //Bombsite A
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"cs_compound",false))  {
        new random=GetRandomInt(1,15);
        if(random==1 && g_iTaken==0) {
          location[0]=452.2;
          location[1]=1103.3;
          location[2]=0.0;
          TreasureChestName = 10; //Far side
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=2378.6;
          location[1]=-1820.6;
          location[2]=56.6;
          TreasureChestName = 11; //Back
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=1681.1;
          location[1]=-1520.4;
          location[2]=16.0;
          TreasureChestName = 12; //Storage Room
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=3504.7;
          location[1]=-479.4;
          location[2]=16.1;
          TreasureChestName = 7; //Side
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=2568.0;
          location[1]=632.9;
          location[2]=4.0;
          TreasureChestName = 14; //Gate house
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=1648.5;
          location[1]=671.8;
          location[2]=408.3;
          TreasureChestName = 13; //Inside
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=1771.9;
          location[1]=865.5;
          location[2]=128.8;
          TreasureChestName = 15; //Front
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=1726.9;
          location[1]=-1482.2;
          location[2]=168.9;
          TreasureChestName = 12;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=336.1;
          location[1]=41.8;
          location[2]=0.0;
          TreasureChestName = 10;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=-3247.8;
          location[1]=109.3;
          location[2]=1.9;
          TreasureChestName = 16; //Gate
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=3599.8;
          location[1]=1507.6;
          location[2]=1.8;
          TreasureChestName = 17; //Outside
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==12 && g_iTakenl==0) {
          location[0]=1130.5;
          location[1]=1024.4;
          location[2]=49.9;
          TreasureChestName = 15;
          g_iTakenl=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==13 && g_iTakenm==0) {
          location[0]=2161.3;
          location[1]=-952.0;
          location[2]=0.0;
          TreasureChestName = 11;
          g_iTakenm=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==14 && g_iTakenn==0) {
          location[0]=2487.5;
          location[1]=708.5;
          location[2]=2.6;
          TreasureChestName = 15;
          g_iTakenn=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==15 && g_iTakeno==0) {
          location[0]=2536.6;
          location[1]=-548.4;
          location[2]=-0.3;
          TreasureChestName = 7;
          g_iTakeno=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"de_nuke",false))  {
        new random=GetRandomInt(1,12);
        if(random==1 && g_iTaken==0) {
          location[0]=1507.7;
          location[1]=-1481.1;
          location[2]=-103.9;
          TreasureChestName = 18; //Roof
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=320.5;
          location[1]=-1484.9;
          location[2]=-127.8;
          TreasureChestName = 19; //Loft
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=905.0;
          location[1]=-1420.4;
          location[2]=-415.9;
          TreasureChestName = 9;
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=42.1;
          location[1]=-947.2;
          location[2]=-767.9;
          TreasureChestName = 20; //Back Way
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=1324.6;
          location[1]=-377.6;
          location[2]=-639.9;
          TreasureChestName = 21; //Underground
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=566.7;
          location[1]=854.0;
          location[2]=-479.9;
          TreasureChestName = 22; //Ramp
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=975.9;
          location[1]=-203.0;
          location[2]=-415.9;
          TreasureChestName = 23; //Side Hall
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=208.7;
          location[1]=-1118.8;
          location[2]=-383.9;
          TreasureChestName = 24; //Inside
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=91.0;
          location[1]=-1669.7;
          location[2]=-46.4;
          TreasureChestName = 20;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=1551.4;
          location[1]=-2256.0;
          location[2]=-415.9;
          TreasureChestName = 25; //Garage
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=2082.5;
          location[1]=-1930.6;
          location[2]=-415.9;
          TreasureChestName = 25; //Garage
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==12 && g_iTakenl==0) {
          location[0]=2421.8;
          location[1]=-1793.9;
          location[2]=-415.9;
          TreasureChestName = 26; //CT Spawn
          g_iTakenl=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"de_inferno",false))  {
        new random=GetRandomInt(1,13);
        if(random==1 && g_iTaken==0) {
          location[0]=690.9;
          location[1]=2763.8;
          location[2]=224.0;
          TreasureChestName = 27; //Window
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=-178.7;
          location[1]=-696.6;
          location[2]=64.0;
          TreasureChestName = 27;
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=-214.3;
          location[1]=96.5;
          location[2]=182.0;
          TreasureChestName = 28; //Upstairs
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=75.7;
          location[1]=181.0;
          location[2]=182.0;
          TreasureChestName = 29; //Deck
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=-480.0;
          location[1]=-564.4;
          location[2]=192.0;

          TreasureChestName = 28; //Upstairs
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=1059.8;
          location[1]=-517.5;
          location[2]=256.0;
          TreasureChestName = 30; //Bedroom
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=1131.8;
          location[1]=-245.4;
          location[2]=256.0;
          TreasureChestName = 31; //Balcony
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=2465.7;
          location[1]=-39.4;
          location[2]=80.0;
          TreasureChestName = 4;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=-76.4;
          location[1]=1412.2;
          location[2]=106.0;

          TreasureChestName = 32; //Alley
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=863.9;
          location[1]=1895.3;
          location[2]=134.5;
          TreasureChestName = 32; //Garage
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=292.3;
          location[1]=521.3;
          location[2]=21.9;
          TreasureChestName = 33; //Crawlspace
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==12 && g_iTakenl==0) {
          location[0]=653.9;
          location[1]=3077.6;
          location[2]=160.0;

          TreasureChestName = 9;
          g_iTakenl=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==13 && g_iTakenm==0) {
          location[0]=943.8;
          location[1]=136.3;
          location[2]=256.0;

          TreasureChestName = 34; //Back Alley
          g_iTakenm=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"de_train",false))  {
        new random=GetRandomInt(1,12);
        if(random==1 && g_iTaken==0) {
          location[0]=-887.5;
          location[1]=-87.9;
          location[2]=16.0;
          TreasureChestName = 35; //Bathroom
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=-1047.9;
          location[1]=-504.0;
          location[2]=-95.9;
          TreasureChestName = 36; //Stairs
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=-966.1;
          location[1]=-452.3;
          location[2]=16.0;
          TreasureChestName = 37; //Long Hall
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=-336.0;
          location[1]=-334.3;
          location[2]=-217.9;
          TreasureChestName = 38; //Ladder
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=257.8;
          location[1]=751.9;
          location[2]=40.0;

          TreasureChestName = 9;
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=-48.0;
          location[1]=776.9;
          location[2]=-217.4;
          TreasureChestName = 34; //Back Alley
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=1174.1;
          location[1]=221.9;
          location[2]=-66.7;
          TreasureChestName = 9;
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=991.9;
          location[1]=-156.4;
          location[2]=-150.9;
          TreasureChestName = 9;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=1437.9;
          location[1]=735.9;
          location[2]=-215.9;

          TreasureChestName = 9;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=506.2;
          location[1]=-1533.2;
          location[2]=-359.9;
          TreasureChestName = 4;
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=270.6;
          location[1]=642.0;
          location[2]=40.0;
          TreasureChestName = 4;
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==12 && g_iTakenl==0) {
          location[0]=-369.0;
          location[1]=1232.0;
          location[2]=-140.9;

          TreasureChestName = 34;
          g_iTakenl=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if((StrEqual(sMap,"de_dust",false)) && ((strlen(sMap)==7)))
      {
        new random=GetRandomInt(1,12);
        if(random==1 && g_iTaken==0) {
          location[0]=-304.6;
          location[1]=3041.6;
          location[2]=17.0;
          TreasureChestName = 1;
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=-1730.8;
          location[1]=2077.3;
          location[2]=128.0;
          TreasureChestName = 39; //Back
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=-1975.6;
          location[1]=2881.2;
          location[2]=32.0;
          TreasureChestName = 39;
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=-1232.7;
          location[1]=1936.0;
          location[2]=-127.9;
          TreasureChestName = 40; //Bridge
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=462.8;
          location[1]=852.4;
          location[2]=128.0;

          TreasureChestName = 15;
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=717.7;
          location[1]=437.0;
          location[2]=128.0;
          TreasureChestName = 3;
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=1110.1;
          location[1]=879.9;
          location[2]=120.0;
          TreasureChestName = 9;
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=-224.6;
          location[1]=1184.4;
          location[2]=128.0;
          TreasureChestName = 6;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=216.3;
          location[1]=1539.2;
          location[2]=128.0;

          TreasureChestName = 6;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=26.1;
          location[1]=1335.9;
          location[2]=32.0;
          TreasureChestName = 6;
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=-1068.8;
          location[1]=1284.7;
          location[2]=32.0;
          TreasureChestName = 7;
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==12 && g_iTakenl==0) {
          location[0]=-1215.9;
          location[1]=768.7;
          location[2]=-63.9;
          TreasureChestName = 40;
          g_iTakenl=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"aim_ag_texture_mix",false))  {
        new random=GetRandomInt(1,15);
        if(random==1 && g_iTaken==0) {
          location[0]=-445.6;
          location[1]=283.4;
          location[2]=128.0;
          TreasureChestName = 5; //Far side
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=515.8;
          location[1]=281.5;
          location[2]=128.0;
          TreasureChestName = 5; //Back
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=6.1;
          location[1]=-815.9;
          location[2]=128.0;
          TreasureChestName = 5; //Storage Room
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=-661.5;
          location[1]=-430.9;
          location[2]=384.0;
          TreasureChestName = 5; //Side
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=1775.9;
          location[1]=-313.3;
          location[2]=640.0;
          TreasureChestName = 5; //Gate house
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=61.6;
          location[1]=-726.2;
          location[2]=512.0;
          TreasureChestName = 5; //Inside
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=-648.1;
          location[1]=282.6;
          location[2]=640.0;
          TreasureChestName = 5; //Front
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=103.3;
          location[1]=244.9;
          location[2]=640.0;
          TreasureChestName = 5;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=-667.6;
          location[1]=559.9;
          location[2]=448.0;
          TreasureChestName = 5;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=1553.0;
          location[1]=166.8;
          location[2]=768.0;
          TreasureChestName = 5; //Gate
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=1906.3;
          location[1]=-317.2;
          location[2]=960.0;
          TreasureChestName = 5;
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==12 && g_iTakenl==0) {
          location[0]=845.3;
          location[1]=-199.2;
          location[2]=512.0;
          TreasureChestName = 5;
          g_iTakenl=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==13 && g_iTakenm==0) {
          location[0]=463.0;
          location[1]=-168.6;
          location[2]=128.0;
          TreasureChestName = 5;
          g_iTakenm=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==14 && g_iTakenn==0) {
          location[0]=1314.1;
          location[1]=-164.7;
          location[2]=128.0;
          TreasureChestName = 5;
          g_iTakenn=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==15 && g_iTakeno==0) {
          location[0]=791.8;
          location[1]=-879.9;
          location[2]=128.0;
          TreasureChestName = 5;
          g_iTakeno=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"de_cpl_mill",false))  {
        new random=GetRandomInt(1,10);
        if(random==1 && g_iTaken==0) {
          location[0]=2031.9;
          location[1]=2082.4;
          location[2]=208.0;
          TreasureChestName = 4;
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=1375.6;
          location[1]=2127.8;
          location[2]=184.0;
          TreasureChestName = 41; //Courtyard
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=420.1;
          location[1]=2008.8;
          location[2]=68.0;
          TreasureChestName = 16;
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=574.6;
          location[1]=1163.0;
          location[2]=-43.9;
          TreasureChestName = 42; //Side
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=-9.2;
          location[1]=1607.1;
          location[2]=-111.9;
          TreasureChestName = 6; //Gate house
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=-1519.9;
          location[1]=1551.1;
          location[2]=-111.9;
          TreasureChestName = 9; //Inside
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=-671.0;
          location[1]=2271.9;
          location[2]=207.1;
          TreasureChestName = 9; //Front
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=-1092.0;
          location[1]=2218.7;
          location[2]=-287.9;
          TreasureChestName = 43;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=-72.3;
          location[1]=2371.8;
          location[2]=-159.9;
          TreasureChestName = 43;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=-375.4;
          location[1]=1647.9;
          location[2]=-111.9;
          TreasureChestName = 6; //Gate
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"de_cpl_strike",false))  {
        new random=GetRandomInt(1,13);
        if(random==1 && g_iTaken==0) {
          location[0]=338.1;
          location[1]=-691.8;
          location[2]=-103.9;
          TreasureChestName = 6; //Far side
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=173.7;
          location[1]=98.0;
          location[2]=-159.9;
          TreasureChestName = 11; //Back
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=-774.2;
          location[1]=-484.4;
          location[2]=-167.9;
          TreasureChestName = 6; //Storage Room
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=-1148.4;
          location[1]=-1493.9;
          location[2]=-81.5;
          TreasureChestName = 44; //Front Yard
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=-23.9;
          location[1]=-1920.0;
          location[2]=-39.9;
          TreasureChestName = 9; //Gate house
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=-446.6;
          location[1]=-2248.3;
          location[2]=8.0;
          TreasureChestName = 9; //Inside
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=-448.8;
          location[1]=-1591.8;
          location[2]=-39.9;
          TreasureChestName = 9; //Front
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=-446.6;
          location[1]=-2248.3;
          location[2]=8.0;
          TreasureChestName = 9;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=-756.9;
          location[1]=-991.2;
          location[2]=-167.9;
          TreasureChestName = 6;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=472.0;
          location[1]=-1453.9;
          location[2]=-167.9;
          TreasureChestName = 20; //Gate
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=363.3;
          location[1]=-2086.0;
          location[2]=-39.9;
          TreasureChestName = 45;
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==12 && g_iTakenl==0) {
          location[0]=-1047.9;
          location[1]=81.3;
          location[2]=-367.9;
          TreasureChestName = 21;
          g_iTakenl=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==13 && g_iTakenm==0) {
          location[0]=-1105.3;
          location[1]=352.0;
          location[2]=-79.9;
          TreasureChestName = 41;
          g_iTakenm=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"de_season",false))  {
        new random=GetRandomInt(1,11);
        if(random==1 && g_iTaken==0) {
          location[0]=3439.9;
          location[1]=290.6;
          location[2]=-79.9;
          TreasureChestName = 46; //Far side
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=3507.1;
          location[1]=704.4;
          location[2]=48.1;
          TreasureChestName = 34; //Back
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=3507.4;
          location[1]=1450.5;
          location[2]=-53.1;
          TreasureChestName = 34; //Storage Room
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=1868.9;
          location[1]=159.9;
          location[2]=68.0;
          TreasureChestName = 47; //Front Yard
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=2106.5;
          location[1]=728.0;
          location[2]=0.0;
          TreasureChestName = 48; //Gate house
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=969.0;
          location[1]=249.7;
          location[2]=-42.2;
          TreasureChestName = 6; //Inside
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=179.8;
          location[1]=220.5;
          location[2]=-43.5;
          TreasureChestName = 9; //Bombsite A
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=-765.1;
          location[1]=-1085.8;
          location[2]=-15.9;
          TreasureChestName = 37;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=146.2;
          location[1]=-828.6;
          location[2]=48.0;
          TreasureChestName = 2;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=-445.0;
          location[1]=215.8;
          location[2]=128.0;
          TreasureChestName = 9; //Gate
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=-397.5;
          location[1]=-1276.2;
          location[2]=-167.9;
          TreasureChestName = 37;
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"cs_italy_tactik2",false))  {
        new random=GetRandomInt(1,13);
        if(random==1 && g_iTaken==0) {
          location[0]=-37.4;
          location[1]=-152.0;
          location[2]=-151.9;
          TreasureChestName = 5; //Far side
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=-364.5;
          location[1]=2191.9;
          location[2]=-82.0;
          TreasureChestName = 5; //Back
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=-1073.8;
          location[1]=2124.3;
          location[2]=-151.9;
          TreasureChestName = 5; //Storage Room
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=-988.1;
          location[1]=1247.7;
          location[2]=-7.9;
          TreasureChestName = 5; //Front Yard
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=-1417.5;
          location[1]=56.4;
          location[2]=8.0;
          TreasureChestName = 5; //Gate house
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=-1327.9;
          location[1]=1300.8;
          location[2]=-143.6;
          TreasureChestName = 5; //Inside
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=-777.7;
          location[1]=189.0;
          location[2]=8.0;
          TreasureChestName = 5; //Front
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=-822.6;
          location[1]=805.9;
          location[2]=160.0;
          TreasureChestName = 5;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=34.9;
          location[1]=425.9;
          location[2]=152.0;
          TreasureChestName = 5;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=-410.7;
          location[1]=506.9;
          location[2]=32.0;
          TreasureChestName = 5; //Gate
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=-172.4;
          location[1]=501.3;
          location[2]=-31.9;
          TreasureChestName = 5;
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==12 && g_iTakenl==0) {
          location[0]=1181.9;
          location[1]=768.6;
          location[2]=-194.9;
          TreasureChestName = 5;
          g_iTakenl=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==13 && g_iTakenm==0) {
          location[0]=556.0;
          location[1]=-391.6;
          location[2]=-41.9;
          TreasureChestName = 5;
          g_iTakenm=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"knas_sandland_css",false))  {
        new random=GetRandomInt(1,11);
        if(random==1 && g_iTaken==0) {
          location[0]=384.0;
          location[1]=-1832.0;
          location[2]=-263.9;
          TreasureChestName = 5; //Far side
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=-78.1;
          location[1]=-343.0;
          location[2]=-191.9;
          TreasureChestName = 5; //Back
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=608.0;
          location[1]=-547.9;
          location[2]=32.0;
          TreasureChestName = 5; //Storage Room
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=336.0;
          location[1]=231.9;
          location[2]=128.0;
          TreasureChestName = 5; //Front Yard
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=-332.6;
          location[1]=199.4;
          location[2]=-175.9;
          TreasureChestName = 5; //Gate house
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=-785.1;
          location[1]=78.0;
          location[2]=-415.9;
          TreasureChestName = 5; //Inside
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=-1295.9;
          location[1]=537.4;
          location[2]=-399.9;
          TreasureChestName = 5; //Front
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=-815.7;
          location[1]=368.0;
          location[2]=-191.9;
          TreasureChestName = 5;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=-911.9;
          location[1]=519.2;
          location[2]=-127.9;
          TreasureChestName = 5;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=48.0;
          location[1]=-512.0;
          location[2]=32.0;
          TreasureChestName = 5; //Gate
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=920.0;
          location[1]=-79.9;
          location[2]=32.0;
          TreasureChestName = 5;
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
      if(StrEqual(sMap,"de_losttemple_pro",false))  {
        new random=GetRandomInt(1,12);
        if(random==1 && g_iTaken==0) {
          location[0]=1890.0;
          location[1]=420.0;
          location[2]=200.0;
          TreasureChestName = 37; //Far side
          g_iTaken=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==2 && g_iTakenb==0) {
          location[0]=1875.1;
          location[1]=-465.6;
          location[2]=232.0;
          TreasureChestName = 37; //Back
          g_iTakenb=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==3 && g_iTakenc==0) {
          location[0]=1048.0;
          location[1]=-434.2;
          location[2]=184.0;
          TreasureChestName = 50; //Water
          g_iTakenc=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==4 && g_iTakend==0) {
          location[0]=392.1;
          location[1]=-478.2;
          location[2]=238.4;
          TreasureChestName = 54; //Main Hall
          g_iTakend=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==5 && g_iTakene==0) {
          location[0]=1328.6;
          location[1]=-350.9;
          location[2]=102.0;
          TreasureChestName = 50; //Water
          g_iTakene=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==6 && g_iTakenf==0) {
          location[0]=-590.2;
          location[1]=-510.2;
          location[2]=195.8;
          TreasureChestName = 51; //Overpass
          g_iTakenf=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==7 && g_iTakeng==0) {
          location[0]=-711.3;
          location[1]=-646.2;
          location[2]=64.0;
          TreasureChestName = 52; //Underpass
          g_iTakeng=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==8 && g_iTakenh==0) {
          location[0]=383.9;
          location[1]=-1003.6;
          location[2]=184.0;
          TreasureChestName = 6;
          g_iTakenh=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==9 && g_iTakeni==0) {
          location[0]=75.5;
          location[1]=-1580.5;
          location[2]=240.0;
          TreasureChestName = 53;
          g_iTakeni=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==10 && g_iTakenj==0) {
          location[0]=-99.9;
          location[1]=587.4;
          location[2]=2.3;
          TreasureChestName = 36; //Gate
          g_iTakenj=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==11 && g_iTakenk==0) {
          location[0]=788.9;
          location[1]=-424.9;
          location[2]=6.5;
          TreasureChestName = 50;
          g_iTakenk=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
        if(random==12 && g_iTakenl==0) {
          location[0]=-246.8;
          location[1]=-751.9;
          location[2]=0.0;
          TreasureChestName = 52;
          g_iTakenl=1;
          TreasureChest(location,TreasureChestName);
          g_iTreasureChestNumber++;
        }
      }
    }
  }
  return spawnedChests;
}

stock TreasureChest(Float:location[3],TreasureChestName)
{
  new Float:eventmulti = 1.0;
  if(IsTreasureHunt())
    eventmulti+= float(PlayersInTeams(true))* 0.005;
  new rarity=GetRandomInt(1,3750);
  rarity = RoundToCeil(eventmulti * float(rarity));
  if(3750==RoundToNearest(eventmulti * float(rarity)) && 3750!=RoundToCeil(eventmulti * float(rarity)))
    PrintToChatAll("\x03That was close!");
  new chestrarity=0;
  if(rarity==3750)
    chestrarity=4; // Extremely rare
  else if(rarity>=3725 && rarity!=3750)
    chestrarity=3; // Epic
  else if(rarity>=3500 && rarity<3725)
    chestrarity=2; // Rare
  else if(rarity<3500)
    chestrarity=1; // Uncommon
  switch(chestrarity)
  {
    case 1: PrintToChatAll("\x03A treasure chest has spawned!");
    case 2: PrintToChatAll("\x03A rare treasure chest has spawned!");
    case 3: PrintToChatAll("\x03An epic treasure chest has spawned!");
    case 4: PrintToChatAll("\x03An extremely rare treasure chest has spawned!");
  }
  new entity = SpawnChestModel(location,NULL_VECTOR,chestrarity);
  /*SpawnTotem(location, true, 200 );*/
  new roundcount=g_RoundCount;
  new Handle:datapack=CreateDataPack();
  WritePackCell(datapack,roundcount);
  WritePackCell(datapack,TreasureChestName);
  WritePackCell(datapack,entity);
  WritePackCell(datapack,chestrarity);
  WritePackFloat(datapack,location[0]);
  WritePackFloat(datapack,location[1]);
  WritePackFloat(datapack,location[2]);
  CreateTimer(0.10,TreasureChestCheck,datapack,TIMER_REPEAT);
  g_iTreasureChestRound=g_RoundCount;
}
new ChestEntity[5];
stock SpawnChestModel(Float:location[3],Float:angles[3],chestrarity) {
  ChestEntity[g_iTreasureChestNumber] = CreateEntityByName( "prop_physics_override" );
  if( !IsValidEntity( ChestEntity[g_iTreasureChestNumber] ) )
  {
    return -1;
  }

  switch(chestrarity) {
    case 1:
    {
      SetEntityModel(ChestEntity[g_iTreasureChestNumber], "models/cf_models_src/treasurechest02.mdl");
    }
    case 2:
    {
      SetEntityModel(ChestEntity[g_iTreasureChestNumber], "models/cf_models_src/treasurechest05.mdl");
    }
    case 3:
    {
      SetEntityModel(ChestEntity[g_iTreasureChestNumber], "models/cf_models_src/treasurechest03.mdl");
    }
    case 4:
    {
      SetEntityModel(ChestEntity[g_iTreasureChestNumber], "models/cf_models_src/treasurechest04.mdl");
    }
  }


  SetEntityMoveType( ChestEntity[g_iTreasureChestNumber], MOVETYPE_NONE );

  DispatchSpawn( ChestEntity[g_iTreasureChestNumber] );
  SetEntProp( ChestEntity[g_iTreasureChestNumber], Prop_Send, "m_nSolidType", 2 );
  SetEntProp( ChestEntity[g_iTreasureChestNumber], Prop_Data, "m_iHealth", 1000 );
  AcceptEntityInput(ChestEntity[g_iTreasureChestNumber], "DisableMotion");

  TeleportEntity( ChestEntity[g_iTreasureChestNumber], location, angles, NULL_VECTOR );
  //SetEntPropFloat( ChestEntity[g_iTreasureChestNumber], Prop_Data, "m_flNextThink", GetGameTime() + 0.1 );

  return ChestEntity[g_iTreasureChestNumber];
}
stock bool:KillChest(ent) {
  if(ent<=0) {
    return false;
  }
  if(IsValidEntity(ent)) {
    new String:clsname[60];
    if(GetEdictClassname(ent, clsname, sizeof(clsname)))   {
      if(StrEqual(clsname, "prop_physics", false))   {
        RemoveEdict(ent);
        return true;
      }
    }
  }
  return false;
}
public Action:TreasureChestCheck(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  decl Float:location[3];
  new roundcount=ReadPackCell(datapack);
  new TreasureChestName=ReadPackCell(datapack);
  new entity=ReadPackCell(datapack);
  new chestrarity=ReadPackCell(datapack);
  location[0]=ReadPackFloat(datapack);
  location[1]=ReadPackFloat(datapack);
  location[2]=ReadPackFloat(datapack);
  new claimer;
  claimer = GetNearestClient("@all",location,70.0);
  if(claimer>0) {
    if(!(GetCombat(claimer)>0)) {
      TreasureChestClaimed(claimer,location,TreasureChestName,entity,chestrarity);
      return Plugin_Stop;
    }
  }
  if(g_RoundCount!=roundcount) {
    return Plugin_Stop;
  }
  //VS_BeamRingPoint(location, 69.0, 70.0, g_ef_shellchrome, g_ef_shellchrome, 0, 1, 1.0, 10.0, 5.0, {255, 255, 255, 25}, -1, 0, 0.0);
  //VS_BeamRingPoint(location, 69.0, 70.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 15.0, 5.0, {255, 255, 255, 25}, -1, 0, 0.0);
  //VS_BeamRingPoint(location, 69.0, 70.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 15.0, 5.0, {255, 255, 255, 25}, 200, 0, 0.0);
  //VS_BeamRingPoint(location, 69.0, 70.0, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 0, 1, 1.0, 15.0, 5.0, {255, 255, 255, 25}, 200, 0, 0.0);
  //VS_BeamRingPoint(location, 69.0, 70.0, g_ef_shellchrome, g_ef_shellchrome, 0, 1, 1.0, 10.0, 5.0, {255, 255, 255, 25}, 1, 0, 0.0);
  return Plugin_Continue;
}
new g_iTreasureChestTicks[65];
stock TreasureChestClaimed(claimer,Float:location[3],TreasureChestName,entity,chestrarity)
{
  decl String:name[20];
  GetClientName(claimer,name,sizeof(name));
  decl String:raritystr[50];
  switch(chestrarity)
  {
    case 1: raritystr="a";
    case 2: raritystr="a rare";
    case 3: raritystr="an epic";
    case 4: raritystr="an extremely rare";
  }
  switch(TreasureChestName)
  {
    case 1: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at T Spawn!",name,raritystr);
    case 2: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Tunnel!",name,raritystr);
    case 3: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Double Doors!",name,raritystr);
    case 4: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Bombsite B",name,raritystr);
    case 5: PrintToChatAll("\x04%s \x03has claimed %s treasure chest!",name,raritystr);
    case 6: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Middle",name,raritystr);
    case 7: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Side",name,raritystr);
    case 8: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Side Door",name,raritystr);
    case 9: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Bombsite A",name,raritystr);
    case 10: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Far Side",name,raritystr);
    case 11: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Back",name,raritystr);
    case 12: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Storage Room",name,raritystr);
    case 13: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Inside",name,raritystr);
    case 14: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Gatehouse",name,raritystr);
    case 15: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Front",name,raritystr);
    case 16: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Gate",name,raritystr);
    case 17: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Outside",name,raritystr);
    case 18: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Roof",name,raritystr);
    case 19: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Loft",name,raritystr);
    case 20: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Back Way",name,raritystr);
    case 21: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Underground",name,raritystr);
    case 22: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Ramp",name,raritystr);
    case 23: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Side Hall",name,raritystr);
    case 24: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Inside",name,raritystr);
    case 25: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Garage",name,raritystr);
    case 26: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at CT Spawn",name,raritystr);
    case 27: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Window",name,raritystr);
    case 28: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Upstairs",name,raritystr);
    case 29: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Desk",name,raritystr);
    case 30: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Bedroom",name,raritystr);
    case 31: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Balcony",name,raritystr);
    case 32: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Alley",name,raritystr);
    case 33: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Crawlspace",name,raritystr);
    case 34: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Back Alley",name,raritystr);
    case 35: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Bathroom",name,raritystr);
    case 36: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Stairs",name,raritystr);
    case 37: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Long Hall",name,raritystr);
    case 38: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Ladder",name,raritystr);
    case 39: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Back",name,raritystr);
    case 40: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Bridge",name,raritystr);
    case 41: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Courtyard",name,raritystr);
    case 42: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Basement",name,raritystr);
    case 43: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Sewers",name,raritystr);
    case 44: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Front Yard",name,raritystr);
    case 45: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Apartment",name,raritystr);
    case 46: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Front Hall",name,raritystr);
    case 47: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Ducts",name,raritystr);
    case 48: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Lobby",name,raritystr);
    case 49: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Side Alley",name,raritystr);
    case 50: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Water",name,raritystr);
    case 51: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Overpass",name,raritystr);
    case 52: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Underpass",name,raritystr);
    case 53: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Ramp",name,raritystr);
    case 54: PrintToChatAll("\x04%s \x03has claimed %s treasure chest at Main Hall",name,raritystr);
  }
  SetChestTick(claimer,chestrarity);
  new Handle:datapack=CreateDataPack();
  WritePackCell(datapack,claimer);
  WritePackCell(datapack,TreasureChestName);
  WritePackCell(datapack,entity);
  WritePackCell(datapack,chestrarity);
  WritePackFloat(datapack,location[0]);
  WritePackFloat(datapack,location[1]);
  WritePackFloat(datapack,location[2]);
  CreateTimer(0.50,TreasureChestTick,datapack,TIMER_REPEAT);
}

public SetChestTick(claimer,chestrarity)
{
  switch(chestrarity)
  {
    case 1:
    {
      g_iTreasureChestTicks[claimer] = 17;
      if(IsDmEnabled())
        g_iTreasureChestTicks[claimer]+=4;
      if(IsTreasureHunt())
        g_iTreasureChestTicks[claimer]+=8;
    }
    case 2:
    {
      g_iTreasureChestTicks[claimer] = 22;
      if(IsDmEnabled())
        g_iTreasureChestTicks[claimer]+=5;
      if(IsTreasureHunt())
        g_iTreasureChestTicks[claimer]+=11;
    }
    case 3:
    {
      g_iTreasureChestTicks[claimer] = 27;
      if(IsDmEnabled())
        g_iTreasureChestTicks[claimer]+=6;
      if(IsTreasureHunt())
        g_iTreasureChestTicks[claimer]+=13;
    }
    case 4:
    {
      g_iTreasureChestTicks[claimer] = 32;
      if(IsDmEnabled())
        g_iTreasureChestTicks[claimer]+=8;
      if(IsTreasureHunt())
        g_iTreasureChestTicks[claimer]+=28;
    }
  }
}
new g_iRoundEnded;
public Action:TreasureChestTick(Handle: timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  decl Float:location[3];
  new claimer=ReadPackCell(datapack);
  new TreasureChestName=ReadPackCell(datapack);
  new entity=ReadPackCell(datapack);
  new chestrarity=ReadPackCell(datapack);
  location[0]=ReadPackFloat(datapack);
  location[1]=ReadPackFloat(datapack);
  location[2]=ReadPackFloat(datapack);
  decl String:name[20];
  GetClientName(claimer,name,sizeof(name));
  if(GetCombat(claimer)>0 || IsPolymorphed(claimer)) {
    PrintToChatAll("\x04%s \x03has been interrupted while claiming a treasure chest!",name);
    SetChestTick(claimer,chestrarity);
    new roundcount=g_RoundCount;
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,roundcount);
    WritePackCell(datapack2,TreasureChestName);
    WritePackCell(datapack2,entity);
    WritePackCell(datapack2,chestrarity);
    WritePackFloat(datapack2,location[0]);
    WritePackFloat(datapack2,location[1]);
    WritePackFloat(datapack2,location[2]);
    CreateTimer(0.10,TreasureChestCheck,datapack2,TIMER_REPEAT);
    return Plugin_Stop;
  }
  if(UnableToVictim[claimer]==true) {
    PrintToChatAll("\x04%s \x03used Divine Shield and stopped claiming the chest!",name);
    SetChestTick(claimer,chestrarity);
    new roundcount=g_RoundCount;
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,roundcount);
    WritePackCell(datapack2,TreasureChestName);
    WritePackCell(datapack2,entity);
    WritePackCell(datapack2,chestrarity);
    WritePackFloat(datapack2,location[0]);
    WritePackFloat(datapack2,location[1]);
    WritePackFloat(datapack2,location[2]);
    CreateTimer(0.10,TreasureChestCheck,datapack2,TIMER_REPEAT);
    return Plugin_Stop;
  }
  if(bVanish[claimer] || IsMageInvis(claimer)) {
    PrintToChatAll("\x04%s \x03has turned invisible and stopped claiming the chest!",name);
    SetChestTick(claimer,chestrarity);
    new roundcount=g_RoundCount;
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,roundcount);
    WritePackCell(datapack2,TreasureChestName);
    WritePackCell(datapack2,entity);
    WritePackCell(datapack2,chestrarity);
    WritePackFloat(datapack2,location[0]);
    WritePackFloat(datapack2,location[1]);
    WritePackFloat(datapack2,location[2]);
    CreateTimer(0.10,TreasureChestCheck,datapack2,TIMER_REPEAT);
    return Plugin_Stop;
  }

  new targets[MAXPLAYERS];
  new amount;
  amount=GetNearClients("@all",location,targets,225.0);
  new check = 0;
  for (new x=0;x<=(amount-1);x++)
  {
    if(claimer==targets[x])
      check = 1;
  }
  if(g_iRoundEnded==1 && check == 1)
  {
    TreasureChestOpen(claimer,chestrarity);
    PrintToChatAll("\x04%s \x03has opened a treasure chest!",name);
    SetChestTick(claimer,chestrarity);
    KillChest(entity);
    return Plugin_Stop;
  }
  if(check==0) {
    if(g_RoundCount!=g_iTreasureChestRound)
    {
      TreasureChestOpen(claimer,chestrarity);
      PrintToChatAll("\x04%s \x03has opened a treasure chest!",name);
      SetChestTick(claimer,chestrarity);
      KillChest(entity);
      return Plugin_Stop;
    }
    PrintToChatAll("\x04%s \x03has left the area while claiming a treasure chest!",name);
    SetChestTick(claimer,chestrarity);
    new roundcount=g_RoundCount;
    new Handle:datapack3=CreateDataPack();
    WritePackCell(datapack3,roundcount);
    WritePackCell(datapack3,TreasureChestName);
    WritePackCell(datapack3,entity);
    WritePackCell(datapack3,chestrarity);
    WritePackFloat(datapack3,location[0]);
    WritePackFloat(datapack3,location[1]);
    WritePackFloat(datapack3,location[2]);
    CreateTimer(0.10,TreasureChestCheck,datapack3,TIMER_REPEAT);
    return Plugin_Stop;
  }
  if(g_iTreasureChestTicks[claimer]<=0)
  {
    TreasureChestOpen(claimer,chestrarity);
    PrintToChatAll("\x04%s \x03has opened a treasure chest!",name);
    SetChestTick(claimer,chestrarity);
    KillChest(entity);
    //TreasureRank(claimer,chestrarity);
    return Plugin_Stop;
  }
  PrintToChat(claimer,"\x03%f seconds left till the chest opens!",float(g_iTreasureChestTicks[claimer])/2);
  new team = GetClientTeam(claimer);
  new Float:vec[3];
  GetClientAbsOrigin(claimer, vec);
  vec[2] += 10;
  decl color[4];
  color[0]=10;
  color[1]=10;
  color[2]=0;
  color[3]=255;
  TE_SetupBeamRingPoint(vec, 10.0, 400.0, g_ef_laserbeam, g_HaloSprite, 0, 15, 1.0, 5.0, 0.0, color, 10, 0);
  TE_SendToAll();
	
  if (team == 2)
  {
    color[0]=225;
    color[1]=10;
    color[2]=0;
    color[3]=175;
    TE_SetupBeamRingPoint(vec, 10.0, 400.0, g_ef_laserbeam, g_HaloSprite, 0, 10, 1.1, 10.0, 0.5, color, 10, 0);
  }
  if (team == 3)
  {
    color[0]=10;
    color[1]=0;
    color[2]=225;
    color[3]=175;
    TE_SetupBeamRingPoint(vec, 10.0, 400.0, g_ef_laserbeam, g_HaloSprite, 0, 10, 1.1, 10.0, 0.5, color, 10, 0);
  }
  TE_SendToAll();
  GetClientEyePosition(claimer, vec);
  EmitAmbientSound("buttons/blip1.wav", vec, claimer, 130);	
  g_iTreasureChestTicks[claimer]-=1;
  return Plugin_Continue;
}

stock TreasureChestOpen(client,chestrarity)
{
  new uncommon = 750;
  new rare = 100;
  new epic = 20;
  new exrare = 1;
  new item=0;
  new itemb=0;
  new itemc=0;
  new itemd=0;
  new iamount = 1;
  new bamount = 1;
  new camount = 1;
  new damount = 1;
  new silver = 0;
  new experience = 0;
  if(chestrarity>=1)
  {
    switch(chestrarity)
    {
      case 1: uncommon = 750;
      case 2: rare = 750;
      case 3: epic = 750;
      case 4: exrare = 7500;
    }
  }
  if(uncommon>=GetRandomInt(1,750)) {
    new drop = GetRandomInt(1,3);
    switch(drop)
    {
      case 1:
      {
        new drop2 = GetRandomInt(1,10);
        switch(drop2)
        {
          case 1: item=GetSetsRandomItem("1");
          case 2: item=GetSetsRandomItem("2");
          case 3: item=GetSetsRandomItem("3");
          case 4: item=GetSetsRandomItem("4");
          case 5: item=GetSetsRandomItem("5");
          case 6: item=GetSetsRandomItem("6");
          case 7: item=GetSetsRandomItem("7");
          case 8: item=GetSetsRandomItem("8");
          case 9: item=GetSetsRandomItem("111");
          case 10: item=GetSetsRandomItem("108");
        }
      }
      case 2: GivePlayerRandomItem(client, "treasure chest", 1, 1);
      case 3: silver+=GetRandomInt(1000,5000);
    }
    experience+=GetRandomInt(500,2500);
  }
  if(rare>=GetRandomInt(1,750)) {
    new drop = GetRandomInt(1,4);
    switch(drop)
    {
      case 1:
      {
        new drop2 = GetRandomInt(1,16);
        switch(drop2)
        {
          case 1: itemb=GetSetsRandomItem("114");
          case 2: itemb=GetSetsRandomItem("107");
          case 3: itemb=GetSetsRandomItem("26");
          case 4: itemb=GetSetsRandomItem("27");
          case 5: itemb=GetSetsRandomItem("60");
          case 6: itemb=GetSetsRandomItem("11");
          case 7: itemb=GetSetsRandomItem("12");
          case 8: itemb=GetSetsRandomItem("13");
          case 9: itemb=GetSetsRandomItem("14");
          case 10: itemb=GetSetsRandomItem("15");
          case 11: itemb=GetSetsRandomItem("16");
          case 12: itemb=GetSetsRandomItem("17");
          case 13: itemb=GetSetsRandomItem("110");
          case 14: itemb=GetSetsRandomItem("112");
          case 15: itemb=GetSetsRandomItem("40");
          case 16: itemb=GetSetsRandomItem("137");
        }
      }
      case 2: GivePlayerRandomItem(client, "treasure chest", 1, 2);
      case 3:
      {
        new gold = GetRandomInt(10,30);
        AddGold(client, gold, "mod", "treasure chest");
        WCMessage(client,"gold chest",gold);
      }
      case 4: silver+=GetRandomInt(7000,12000);
    }
    experience+=GetRandomInt(3500,7500);
  }
  if(epic>=GetRandomInt(1,750)) {
    new drop = GetRandomInt(1,5);
    switch(drop)
    {
      case 1:
      {
        new drop2 = GetRandomInt(1,31);
        switch(drop2)
        {
          case 1: itemc=GetSetsRandomItem("139");
          case 2: itemc=GetSetsRandomItem("141");
          case 3: itemc=GetSetsRandomItem("142");
          case 4: itemc=GetSetsRandomItem("143");
          case 5: itemc=GetSetsRandomItem("144");
          case 6: itemc=GetSetsRandomItem("145");
          case 7: itemc=GetSetsRandomItem("146");
          case 8: itemc=GetSetsRandomItem("147");
          case 9: itemc=GetSetsRandomItem("135");
          case 10: itemc=GetSetsRandomItem("136");
          case 11: itemc=GetSetsRandomItem("115");
          case 12: itemc=GetSetsRandomItem("105");
          case 13: itemc=GetSetsRandomItem("18");
          case 14: itemc=GetSetsRandomItem("19");
          case 15: itemc=GetSetsRandomItem("20");
          case 16: itemc=GetSetsRandomItem("21");
          case 17: itemc=GetSetsRandomItem("22");
          case 18: itemc=GetSetsRandomItem("23");
          case 19: itemc=GetSetsRandomItem("24");
          case 20: itemc=GetSetsRandomItem("25");
          case 21: itemc=GetSetsRandomItem("20");
          case 22: itemc=GetSetsRandomItem("28");
          case 23: itemc=GetSetsRandomItem("29");
          case 24: itemc=GetSetsRandomItem("30");
          case 25: itemc=GetSetsRandomItem("31");
          case 26: itemc=GetSetsRandomItem("32");
          case 27: itemc=GetSetsRandomItem("33");
          case 28: itemc=GetSetsRandomItem("34");
          case 29: itemc=GetSetsRandomItem("35");
          case 30: itemc=GetSetsRandomItem("36");
          case 31: itemd=GetSetsRandomItem("376");
        }
      }
      case 2:
      {
        new gold = GetRandomInt(25,90);
        AddGold(client, gold, "mod", "treasure chest");
        WCMessage(client,"gold chest",gold);
      }
      case 3: silver+=GetRandomInt(9000,22000);
      case 4: itemd=14999;
      case 5: GivePlayerRandomItem(client, "treasure chest", 1, 3);
    }
    experience+=GetRandomInt(10000,22500);
  }
  if(exrare>=GetRandomInt(1,7500)) {
    new drop = GetRandomInt(1,4);
    switch(drop)
    {
      case 1:
      {
        new drop2 = GetRandomInt(1,22);
        switch(drop2)
        {
          case 1: itemd=GetSetsRandomItem("362");
          case 2: itemd=GetSetsRandomItem("363");
          case 3: itemd=GetSetsRandomItem("364");
          case 4: itemd=GetSetsRandomItem("365");
          case 5: itemd=GetSetsRandomItem("366");
          case 6: itemd=GetSetsRandomItem("367");
          case 7: itemd=GetSetsRandomItem("368");
          case 8: itemd=GetSetsRandomItem("306");
          case 9: itemd=GetSetsRandomItem("307");
          case 10: itemd=GetSetsRandomItem("308");
          case 11: itemd=GetSetsRandomItem("379");
          case 12: itemd=GetSetsRandomItem("309");
          case 13: itemd=GetSetsRandomItem("310");
          case 14: itemd=GetSetsRandomItem("311");
          case 15: itemd=GetSetsRandomItem("313");
          case 16: itemd=GetSetsRandomItem("368");
          case 17: itemd=GetSetsRandomItem("369");
          case 18: itemd=GetSetsRandomItem("370");
          case 19: itemd=GetSetsRandomItem("371");
          case 20: itemd=GetSetsRandomItem("372");
          case 21: itemd=GetSetsRandomItem("373");
          case 22: itemd=GetSetsRandomItem("312");
        }
      }
      case 2:
      {
        new gold = GetRandomInt(225,500);
        AddGold(client, gold, "mod", "treasure chest");
        WCMessage(client,"gold chest",gold);
      }
      case 3: GivePlayerRandomItem(client, "treasure chest", 2, 4);
      case 4:
      {
        itemd=14999;
        damount=3;
      }
    }
    experience+=GetRandomInt(50000,10000);
  }
  if(silver>0) {
    wcSetMoney(client,(GetMoney(client)+silver));
    WCMessage(client,"silver chest",client,silver);
  }
  if(experience>0) {
    wcGiveXp(client,experience,true);
    WCMessage(client,"experience chest",client,experience);
  }
  new uitem[UniqueItem];
  decl String:itemname[255];
  if(item>0) {
    GetItemProperties(uitem, item, 0);
    GiveItem(client, uitem, iamount, "treasure chest", false, true);
    GetItemName(item,itemname);
    WCMessage(client, "chest item", client,itemname,iamount);
  }
  if(itemb>0) {
    GetItemProperties(uitem, itemb, 0);
    GiveItem(client, uitem, bamount, "treasure chest", false, true);
    GetItemName(itemb,itemname);
    WCMessage(client, "chest item", client,itemname,bamount);
  }
  if(itemc>0) {
    GetItemProperties(uitem, itemc, 0);
    GiveItem(client, uitem, camount, "treasure chest", false, true);
    GetItemName(itemc,itemname);
    WCMessage(client, "chest item", client,itemname,camount);
  }
  if(itemd>0) {
    GetItemProperties(uitem, itemd, 0);
    GiveItem(client, uitem, damount, "treasure chest", false, true);
    GetItemName(itemd,itemname);
    WCMessage(client, "chest item", client,itemname,damount);
  }
}

stock TreasureRank(client,chestrarity)
{







}









