/*
* File: buy_controller.inc
* Description: Functions related to engine clock or timers.
* Author(s): Stinkyfax
*/

#pragma semicolon 1

new bool:g_ff_timer_gold;
//new bool:g_ff_timer_bot;

new bool:g_runesTimer;
new bool:g_weaponsTimer;
new bool:g_botcreateauction;
InitClock() {
  //setting up timer mail clean
  if(!g_mailclean && MAIN_SERVER)
  {
    CreateTimer(MAILCLEAN_DELAY, MailClean,0, TIMER_REPEAT);
    g_mailclean=true;
  }
  if(!g_playerpurge && MAIN_SERVER)
  {
    CreateTimer(PLAYER_REMOVE_DELAY, PlayerClean,0, TIMER_REPEAT);
    g_playerpurge=true;
  }
  if(!g_mailattacher && MAIN_SERVER)
  {
    CreateTimer(MAILATTACH_DELAY, MailAttacher,0, TIMER_REPEAT);
    g_mailattacher=true;
  }
  //setting up save world
  if(!g_saveworld)
  {
    CreateTimer(SAVEWORLD_DELAY, SaveWorld,0, TIMER_REPEAT);
    g_saveworld=true;
  }
  //auction timer
  if(!g_auctionclean && MAIN_SERVER)
  {
    CreateTimer(AUCTION_DELAY, AuctionClean,0, TIMER_REPEAT);
    g_auctionclean=true;
  }
  if(!g_cronjob)
  {
    CreateTimer(20.0, CronJob,0, TIMER_REPEAT);
    g_cronjob=true;
  }
  //DK runes timer
  if(!g_runesTimer)
  {
    CreateTimer(1.0, RunesTimer,0, TIMER_REPEAT);
    g_runesTimer=true;
  }
  //end

  //Weapon Restrict timer
  if(!g_weaponsTimer)
  {
    CreateTimer(0.1, DeciSecondTimer,_, TIMER_REPEAT);
    g_weaponsTimer=true;
  }

  if(!g_ff_timer_gold)
  {
    if(GOLD_CYCLE_PERIOD > 1.0 && GOLD_CYCLE_AMOUNT > 0)  {
      CreateTimer(GOLD_CYCLE_PERIOD, goldCycle, 0, TIMER_REPEAT);
      g_ff_timer_gold=true;
    }
  }

  //botauction timer
  if(!g_botcreateauction && MAIN_SERVER)
  {
    g_botcreateauction=true;
    CreateTimer(7200.0, BotCreateAuction,0, TIMER_REPEAT); //new item every 2:00 hours, which is 12 items per day and 45 items per full 90 hour auction cycle
  }

  /*if(!g_ff_timer_bot) {*/
    /*g_ff_timer_bot = true;*/
    /*CreateTimer(0.5, BotTimer, 0, TIMER_REPEAT);*/
  /*}*/
  //CreateTimer(0.1, blaTimer, 0, TIMER_REPEAT); //is used to test some random shit
}

public Action:blaTimer(Handle:timer, any:trash) {
  PrintToServer("client 2 loaded: %d",playerloaded[2]);
  return Plugin_Continue;
}

public Action:BotCreateAuction(Handle:timer, any:trash)
{
  new i=0;
  new String:set[500];
  new bool:drop=false;
  for (;(drop==false && i<30);i++) //keep looping until drop happens
  {
    KvRewind(wcdrop);
    if (!KvGotoFirstSubKey(wcdrop))
    {
      LogError("[BotCreateAuction:clock.inc] Error: failed to open drop database");
    }
    do
    {
      if(!KvGetNum(wcdrop,"autoauction",1))
        continue;
      new level=GetRandomInt(66,85);
      new minlevel=KvGetNum(wcdrop,"minlevel");
      if(KvGetNum(wcdrop,"profession") != 0)
        continue;
      if(minlevel<=level)
      {
        new Float:inc=KvGetFloat(wcdrop,"incriment",0.0);
        new Float:min=KvGetFloat(wcdrop,"minimum",0.01);
        new Float:max=KvGetFloat(wcdrop,"maximum",min);
        new Float:chance=min+inc*float(level-minlevel);
        if(chance>max)
          chance=max;
        if(chance+(i/29)>=GetRandomFloat(0.0,1.0)) //guarantee drop on last cycle if all else fails.
        {
          new id=KvGetNum(wcdrop,"id",0);
          if(id<=0)
          {
            KvGetString(wcdrop,"set",set,sizeof(set),"0");
            id = GetSetsRandomItem(set);
          }
          if(id>0)
          {
            new uitem[UniqueItem];
            GetItemProperties(uitem, id, 0);

            new rarity = uitem[Item_quality];
            if(rarity>=1)
            {
              drop=true;
              new iCost = uitem[Item_cost];
              new iMin  = RoundToNearest(GetRandomFloat(0.20,1.25)*iCost);
              new iBuy  = RoundToNearest(GetRandomFloat(0.40,1.25)*iCost*rarity);

              //comment out while testing, no need to spam auction house... again.
              if(iMin>(iBuy*0.75)) //if iMin/buyout too close make'm bid!
                CreateAuction(0,"Anonymous",iMin,0,uitem,1,false);
              else if(GetRandomInt(1,6) > rarity) //buyout more often the more common the item.
                CreateAuction(0,"Anonymous",iMin,iBuy,uitem,1,false);
              else //no buyout, it could be a good one
                CreateAuction(0,"Anonymous",iMin,0,uitem,1,false);

              for(new x=1;x<=GetMaxClients();x++)
                WCMessage(x,"item added auction",x,uitem[Item_Name], iMin);
            }
          }
        }
      }
    } while(KvGotoNextKey(wcdrop) && drop==false);
  }
  return Plugin_Continue;
}

public Action:goldCycle(Handle:timer, any:trash)
{
  //Get list of players
  new players[GetMaxClients()];
  new amount = getActivePlayers(players);
  if(amount >= 1) { //TODO replace with 4
    new client = players[GetRandomInt(0,amount-1)];
    if(playerloaded[client]==1) {
      if(!IsFakeClient(client)) {
        playerWonGoldCycle(client, GOLD_CYCLE_AMOUNT);
      }
    }
  }
  return Plugin_Continue;
}

playerWonGoldCycle(client, amount)  {
  decl String:format[255];
  decl String:MenuName[15];
  AddGold(client, amount, "event", "Gold Cycle event");
  IntToStringfloat(amount, format, sizeof(format));
  Format(format, sizeof(format), "%T", "You won gold", client, format);
  Format(MenuName, sizeof(MenuName), "%T", "gold", client);
  panelMessage(client, MenuName, format);
}

