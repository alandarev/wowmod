/*
* File: gold.inc
* Description: Mainly the menus to support gold functionality and it's functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1

GoldMenu(client) {

  new Handle:menu=CreateMenu(GoldMenuHandler);
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc gold menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[100];

  //balance
  new String:sgold[20];
  IntToStringfloat(GetGold(client), sgold, 20);
  Format(buffer, sizeof(buffer), "%T", "gold balance string", client, sgold);
  AddMenuItem(menu,"AccountMenu", buffer);

  //Spending gold on
  Format(buffer, sizeof(buffer), "%T", "wc spend gold menu", client);
  AddMenuItem(menu,"SpendGoldMenu", buffer); //disabled for now

  //Donate for gold
  Format(buffer, sizeof(buffer), "%T", "wc buy gold menu", client);
  if(DONATIONS>0)
    AddMenuItem(menu,"BuyGoldMenu", buffer);
  else
    AddMenuItem(menu,"BuyGoldMenu", buffer, ITEMDRAW_DISABLED);

  //Earning gold
  Format(buffer, sizeof(buffer), "%T", "wc earn gold menu", client);
  AddMenuItem(menu,"EarnGoldMenu", buffer); //disabled for now

  Format(buffer, sizeof(buffer), "%T", "wc premium shop", client);
  AddMenuItem(menu,"GoldShopPremium0",buffer);

  Format(buffer, sizeof(buffer), "%T", "wc expansion title", client);
  AddMenuItem(menu,"ExpansionsMenu",buffer);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public GoldMenuHandler(Handle:menu, //rename
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {

    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));

    //Call function
    new Function:func = GetFunctionByName(INVALID_HANDLE, buffer);
    Call_StartFunction(INVALID_HANDLE, func);
    Call_PushCell(client);
    if(Call_Finish() != SP_ERROR_NONE)  {
      decl String:err[100];
      Format(err,100,"Could not callback the function: %s",buffer);
      Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
    }
  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      WCMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

public EarnGoldFriend2(client) {

  new Handle:menu=CreateMenu(EarnGoldFriendHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc earn gold invite friend",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[400];

  Format(buffer, sizeof(buffer), "%T", "earn gold invite friend info2", client);
  AddMenuItem(menu,"meta",buffer,ITEMDRAW_DISABLED);
  Format(buffer, sizeof(buffer), "%T", "earn gold invite friend page1", client);
  AddMenuItem(menu,"EarnGoldFriend",buffer);




  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}


public EarnGoldFriend(client) {

  new Handle:menu=CreateMenu(EarnGoldFriendHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc earn gold invite friend",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[400];
  Format(buffer, sizeof(buffer), "%T", "earn gold invite friend info", client, playerids[client]);
  AddMenuItem(menu,"meta",buffer,ITEMDRAW_DISABLED);
  Format(buffer, sizeof(buffer), "%T", "earn gold invite friend page2", client);
  AddMenuItem(menu,"EarnGoldFriend2",buffer);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public EarnGoldFriendHandler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {

    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));

    //Call function
    new Function:func = GetFunctionByName(INVALID_HANDLE, buffer);
    Call_StartFunction(INVALID_HANDLE, func);
    Call_PushCell(client);
    if(Call_Finish() != SP_ERROR_NONE)  {
      decl String:err[100];
      Format(err,100,"Could not callback the function: %s",buffer);
      Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
    }

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      EarnGoldMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

public EarnGoldDrops(client) {

  new Handle:menu=CreateMenu(EarnGoldFriendHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","earn gold from drops",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[400];
  Format(buffer, sizeof(buffer), "%T", "earn gold from drops info", client);
  AddMenuItem(menu,"meta",buffer,ITEMDRAW_DISABLED);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public EarnGoldBinding(client) {

  new Handle:menu=CreateMenu(EarnGoldFriendHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","earn gold by binding",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[400];
  Format(buffer, sizeof(buffer), "%T", "earn gold binding info", client);
  AddMenuItem(menu,"meta",buffer,ITEMDRAW_DISABLED);

  AddMenuItem(menu,"AccountMenu","Open Account Menu");


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public EarnGoldTournaments(client) {

  new Handle:menu=CreateMenu(EarnGoldFriendHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","earn gold by tournaments",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[400];
  Format(buffer, sizeof(buffer), "%T", "earn gold tournaments info", client);
  AddMenuItem(menu,"meta",buffer,ITEMDRAW_DISABLED);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public EarnGoldEvents(client) {

  new Handle:menu=CreateMenu(EarnGoldFriendHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","earn gold by events",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[400];
  Format(buffer, sizeof(buffer), "%T", "earn gold events info", client);
  AddMenuItem(menu,"meta",buffer,ITEMDRAW_DISABLED);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public BuyGoldMenu(client) {

  new Handle:menu=CreateMenu(EarnGoldMenuHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc earn gold menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[300];
  Format(buffer, sizeof(buffer), "%T", "donation info", client,playerids[client]);
  AddMenuItem(menu,"",buffer,ITEMDRAW_DISABLED);
  Format(buffer, sizeof(buffer), "%T", "donation info2", client, "http://wowmod.eu/index.php?topic=251.0");
  AddMenuItem(menu,"printDonationLink",buffer);

  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public printDonationLink(client)  {
  /*PlainMessage(client, "Link to donation: http://wowmod.eu/index.php?topic=251.0");*/
  WCMessage(client, "link to donation", client);
  BuyGoldMenu(client);
}


public EarnGoldMenu(client) {

  new Handle:menu=CreateMenu(EarnGoldMenuHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc earn gold menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[300];
  Format(buffer, sizeof(buffer), "%T", "earn gold invite friend", client);
  AddMenuItem(menu,"EarnGoldFriend",buffer);
  Format(buffer, sizeof(buffer), "%T", "earn gold by tournaments", client);
  AddMenuItem(menu,"EarnGoldTournaments",buffer);
  Format(buffer, sizeof(buffer), "%T", "earn gold from drops", client);
  AddMenuItem(menu,"EarnGoldDrops",buffer);
  Format(buffer, sizeof(buffer), "%T", "earn gold by events", client);
  AddMenuItem(menu,"EarnGoldEvents",buffer);
  Format(buffer, sizeof(buffer), "%T", "earn gold by binding", client);
  AddMenuItem(menu,"EarnGoldBinding",buffer, ITEMDRAW_DISABLED);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public EarnGoldMenuHandler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {

    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));

    //Call function
    new Function:func = GetFunctionByName(INVALID_HANDLE, buffer);
    Call_StartFunction(INVALID_HANDLE, func);
    Call_PushCell(client);
    if(Call_Finish() != SP_ERROR_NONE)  {
      decl String:err[100];
      Format(err,100,"Could not callback the function: %s",buffer);
      Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
    }

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      GoldMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}


public TReferrerCommand(Handle:db,Handle:query,String:error[],any:client)
{
  if(!IsClientConnected(client))
    return;
  if(!query)  {
    LogError("SQL Error 5777: %s", error);
    return;
  }
  SQL_Rewind(query);
  if(!SQL_FetchRow(query))  {
    LogError("SQL Error 5781 for playerid = %d",playerids[client]);
    return;
  }
  new referrer = GetSQLDataInt(query, "referrer");
  new newRef = GetSQLDataInt(query, "newref");
  new days = GetSQLDataInt(query, "days");
  decl String:newName[30];
  GetSQLDataStr(query, "newname", newName, 30);
  if(referrer>0)  {
    /*decl String:text[255];*/
    /*Format(text, sizeof(text), "You already have referrer with id %d. Referrers can't be changed", referrer);*/
    /*PlainMessage(client, text);*/
    WCMessage(client, "already have referrer", client, referrer);
    return;
  }
  if(days > 5)  {
    /*decl String:text[255];*/
    /*Format(text, sizeof(text), "You have been playing on this server for %d days, while registering referrer is allowed in first 5 days", days);*/
    /*PlainMessage(client, text);*/
    WCMessage(client, "played days limit", client, days);
    return;
  }


  new Handle:menu=CreateMenu(ReferrerMenuHandle); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc referrer menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[200];
  Format(buffer, sizeof(buffer), "%T", "referrer text", client, newRef, newName);
  decl String:temp[15];
  IntToString(newRef, temp, sizeof(temp));
  AddMenuItem(menu,temp,buffer,ITEMDRAW_DISABLED);

  AddMenuItem(menu,"0","Leave");
  AddMenuItem(menu,temp,"Confirm!");


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);
}

public ReferrerMenuHandle(Handle:menu, //rename
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    //insert here
    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));
    new id = StringToInt(buffer);
    if(id<=0) {
      return;
    }

    WCMessage(client, "is now your referrer", client, id);
    wcUpdatePlayerInt(playerids[client],"referrer",id,"playerId","wc_Players");
    /*addGoldToPlayer(id, 30, "friends", "Gold for accepting reference");*/

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      //Go previous menu?
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

public SpendGoldMenu(client) { //remove stock modifier and rename

  new Handle:menu=CreateMenu(SpendGoldMenuHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc spend gold menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[200];
  Format(buffer, sizeof(buffer), "%T", "wc gold shop", client);
  AddMenuItem(menu,"GoldShopItems",buffer);
  Format(buffer, sizeof(buffer), "%T", "wc silver shop", client);
  AddMenuItem(menu,"GoldShopSilver",buffer);
  Format(buffer, sizeof(buffer), "%T", "wc charid shop", client);
  AddMenuItem(menu,"GoldCharIdShop",buffer);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public SpendGoldMenuHandler(Handle:menu, //rename
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));

    //Call function
    new Function:func = GetFunctionByName(INVALID_HANDLE, buffer);
    Call_StartFunction(INVALID_HANDLE, func);
    Call_PushCell(client);
    if(Call_Finish() != SP_ERROR_NONE)  {
      decl String:err[100];
      Format(err,100,"Could not callback the function: %s",buffer);
      Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
    }

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      GoldMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

public GoldShopSilver(client)  {
  new Handle:menu_category=CreateMenu(GoldShopSilverHandle);
  SetMenuExitBackButton(menu_category,true);
  decl String:buffer[200], String:sgold[20];
  Format(buffer,sizeof(buffer), "%T", "wc buying silver", client);
  SetMenuTitle(menu_category,buffer);
  IntToStringfloat(GetGold(client), sgold, 20);
  Format(buffer, sizeof(buffer), "%T", "you have gold", client, sgold);
  AddMenuItem(menu_category,"",buffer, ITEMDRAW_DISABLED);

  Format(buffer, sizeof(buffer), "%T", "buy silver for gold", client, 1000, "0.25");
  AddMenuItem(menu_category, "25", buffer);
  Format(buffer, sizeof(buffer), "%T", "buy silver for gold", client, 4000, "1.00");
  AddMenuItem(menu_category, "100", buffer);
  Format(buffer, sizeof(buffer), "%T", "buy silver for gold", client, 20000, "5.00");
  AddMenuItem(menu_category, "500", buffer);

  DisplayMenu(menu_category,client,MENU_TIME_FOREVER);
}

stock goldToSilver(client, amount)  {
  if(GetGold(client) < amount)  {
    decl String:arg1[20],String:arg2[20];
    IntToStringfloat(amount, arg2, 20);
    IntToStringfloat(GetGold(client),arg1,20);
    WCMessage(client,"no gold", client, arg1, arg2);
    return;
  }

  new Float:multi=1.0;

  new Float:ratio = 40.0 * multi;

  new give = RoundToNearest(amount * ratio);

  SpendGold(client, amount, "gold shop", "Bought silver");
  wcSetMoney(client, GetMoney(client) + give);
  decl String:spent[20];
  IntToStringfloat(amount, spent, 20);
  WCMessage(client, "bought silver", client, spent, give);
}

public GoldShopSilverHandle(Handle:menu_category,MenuAction:action,client,slot)
{
   if(action==MenuAction_Select)
   {
    decl String:category[255];
    GetMenuItem(menu_category,slot,category,sizeof(category));
    new amount=StringToInt(category);
    goldToSilver(client, amount);
    GoldShopSilver(client);
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack)
         GoldMenu(client);
   if(action==MenuAction_End)
      CloseHandle(menu_category);
}

public GoldShopItems(client)  {
  new Handle:menu_category=CreateMenu(GoldShopItemssHandle);
  SetMenuExitBackButton(menu_category,true);
  decl String:buffer[80];
  Format(buffer, sizeof(buffer), "%T", "select category", client);
  SetMenuTitle(menu_category,buffer);
  Format(buffer, sizeof(buffer), "%T", "special items", client);
  AddMenuItem(menu_category,"27",buffer);
  for(new i=1;i<=9;i++)   {
    decl String:sClass[50];
    Format(sClass,sizeof(sClass),"class_%i",i);
    Format(sClass,sizeof(sClass),"%T",sClass,client);
    decl String:sCat[10];
    Format(sCat,sizeof(sCat),"%i",i+100);
    AddMenuItem(menu_category,sCat,sClass);
  }
  Format(buffer, sizeof(buffer), "%T", "patterns for blacksmith", client);
  AddMenuItem(menu_category,"1",buffer);
  Format(buffer, sizeof(buffer), "%T", "schemas for jewelcrafting", client);
  AddMenuItem(menu_category,"2",buffer);
  Format(buffer, sizeof(buffer), "%T", "recipes for tailoring", client);
  AddMenuItem(menu_category,"3",buffer);
  Format(buffer, sizeof(buffer), "%T", "patterns for leatherworking", client);
  AddMenuItem(menu_category,"4",buffer);
  Format(buffer, sizeof(buffer), "%T", "formulas for enchanting", client);
  AddMenuItem(menu_category,"5",buffer);
  DisplayMenu(menu_category,client,MENU_TIME_FOREVER);
}

public GoldShopItemssHandle(Handle:menu_category,MenuAction:action,client,slot)
{
   if(action==MenuAction_Select)
   {
    decl String:category[255];
    GetMenuItem(menu_category,slot,category,sizeof(category));
    new cat=StringToInt(category);
    ItemsMenu(client,cat,true);
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack)
         GoldMenu(client);
   if(action==MenuAction_End)
      CloseHandle(menu_category);
}

public GoldShopPremium0(client) {
  SetMenuOff(client, 20.0);
  WCMessage(client, "menu is loading", client, 20.0);
  getPremiumData(client, "GoldShopPremium");
}

public GoldShopPremium(client, data[PremiumData])  {
  SetMenuOff(client, 0.0);
  new Handle:menu=CreateMenu(GoldShopPremiumHandle);
  SetMenuExitBackButton(menu,true);
  decl String:buffer[200], String:sgold[20];
  decl String:isactive[30];
  if(isClientPDModule(client))  {
    Format(isactive, 30, "%T", "active", client);
  }
  else  {
    Format(isactive, 30, "%T", "not active", client);
  }
  Format(buffer, sizeof(buffer), "%T", "premium account", client, isactive);
  SetMenuTitle(menu,buffer);
  IntToStringfloat(GetGold(client), sgold, 20);
  Format(buffer, sizeof(buffer), "You have %s Gold", sgold);


  AddMenuItem(menu,"",buffer, ITEMDRAW_DISABLED);

  //Expires (days left)
  if(data[PD_daysLeft] > 0)  {
    Format(buffer, sizeof(buffer), "Due: %s (%d days left)", data[PD_sExpires], data[PD_daysLeft]);
  }
  else  {
    new String:scost[20];
    IntToStringfloat(data[PD_costs], scost, 20);
    Format(buffer, sizeof(buffer), "%T", "Purchase Premium", client, scost);
  }
  if(data[PD_daysLeft] < 30)
    AddMenuItem(menu,"renew",buffer);
  else
    AddMenuItem(menu,"renew",buffer, ITEMDRAW_DISABLED);


  Format(buffer, sizeof(buffer), "%T", "short about premium", client);
  AddMenuItem(menu, "info", buffer, ITEMDRAW_DISABLED);

  Format(buffer, sizeof(buffer), "%T", "more about premium", client);
  AddMenuItem(menu, "info", buffer);

  //Subscribe
  /*if(data[PD_subscribed])*/
    /*buffer="Subscription is Enabled";*/
  /*else*/
    /*buffer="Subscription is Disabled";*/
  /*AddMenuItem(menu,"subscribe",buffer);*/



  //modules

  //Base module
  /*new String:scosts[10];*/
  /*IntToStringfloat(gPDModules[0], scosts, 10);*/

  /*Format(buffer, sizeof(buffer),*/
    /*"Default Module costs %s gold (can not be disabled)",*/
    /*scosts);*/
  /*AddMenuItem(menu,"",buffer,ITEMDRAW_DISABLED);*/

  /*for (new any:i = 1; i < iPDModules; i++) {*/
    /*if(isPDModuleEnabled(data, i))*/
      /*buffer="Enabled";*/
    /*else*/
      /*buffer="Disabled";*/
    /*new costs = gPDModules[i];*/
    /*IntToStringfloat(costs, scosts, 10);*/
    /*Format(buffer, sizeof(buffer), "Module \"%s\" is %s (%s gold)",*/
      /*sPDModuleNames[i], buffer, scosts);*/
    /*AddMenuItem(menu,"",buffer);*/
  /*}*/


  DisplayMenu(menu,client,MENU_TIME_FOREVER);
}

public GoldShopPremiumHandle(Handle:menu,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:chosen[255];
    GetMenuItem(menu,slot,chosen,sizeof(chosen));
    if(StrEqual("renew", chosen)) {
      new pd[PremiumData];
      getCachedPD(client, pd);
      askRenew(client, pd);
    }
    else if(StrEqual("subscribe", chosen))  {
      new pd[PremiumData];
      getCachedPD(client, pd);
      askSubscribe(client, pd);
      //triggerSubscribe(client, pd);
      //GoldShopPremium(client, pd);

    }
    else  {
      /*new any:id=slot-3;*/
      /*if(id<0)  {*/
      /*LogError("WC Error #gold.602: id is %d", id);*/
      /*}*/
      /*else  {*/
      /*GoldShopModule(client, id);*/
      /*}*/
      /*}*/
      moreAboutPremium(client);

    }
  }
  if(action==MenuAction_Cancel)
  if(slot==MenuCancel_ExitBack)
  GoldMenu(client);
  if(action==MenuAction_End)
  CloseHandle(menu);
}




moreAboutPremium(client) {

  new Handle:menu=CreateMenu(moreAboutPremiumHandler);
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","more about premium title",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[200];
  Format(buffer, sizeof(buffer), "%T", "about reserved", client);
  AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);

  AddMenuItem(menu, "", " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
  Format(buffer, sizeof(buffer), "%T", "about drop", client);
  AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);

  AddMenuItem(menu, "", " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);

  Format(buffer, sizeof(buffer), "%T", "about hp", client);
  AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);


  AddMenuItem(menu, "", " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);

  Format(buffer, sizeof(buffer), "%T", "about kpd", client);
  AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);

  AddMenuItem(menu, "", " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);

  Format(buffer, sizeof(buffer), "%T", "about xp", client);
  AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);

  AddMenuItem(menu, "", " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);

  Format(buffer, sizeof(buffer), "%T", "about dual", client);
  AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);



  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public moreAboutPremiumHandler(Handle:menu, //rename
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    GoldShopPremium0(client);
  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      GoldShopPremium0(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}




askRenew(client, pd[PremiumData]) {
  new Handle:menu=CreateMenu(RenewPremiumHandle);
  SetMenuExitBackButton(menu,true);
  SetMenuTitle(menu,"%T","Premium Account Renew",  client);
  decl String:buffer[200], String:sgold[20];
  IntToStringfloat(GetGold(client), sgold, 20);
  Format(buffer, sizeof(buffer), "%T", "you have gold", client,  sgold);
  AddMenuItem(menu,"",buffer,ITEMDRAW_DISABLED);
  //Subscribe
  /*if(pd[PD_subscribed])*/
    /*buffer="Subscription is Enabled";*/
  /*else*/
    /*buffer="Subscription is Disabled";*/
  new days = 30 - pd[PD_daysLeft];
  if(days > 30) days=30;
  Format(buffer, sizeof(buffer), "%T", "will be extended by days", client, days);
  AddMenuItem(menu,"",buffer, ITEMDRAW_DISABLED);

  new costs = RoundToCeil(float(pd[PD_costs]) / 30.0 * days);
  IntToStringfloat(costs,sgold,20);
  Format(buffer, sizeof(buffer),"%T", "renew premium final warning", client, sgold);
  AddMenuItem(menu,"",buffer, ITEMDRAW_DISABLED);

  Format(buffer, sizeof(buffer), "%T", "confirm", client);
  if(costs > GetGold(client))
    AddMenuItem(menu,"subscribe",buffer,ITEMDRAW_DISABLED);
  else
    AddMenuItem(menu,"subscribe",buffer);

  DisplayMenu(menu,client,MENU_TIME_FOREVER);
}

public RenewPremiumHandle(Handle:menu,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    new pd[PremiumData];
    getCachedPD(client, pd);
    decl String:chosen[255];
    GetMenuItem(menu,slot,chosen,sizeof(chosen));
    if(StrEqual("subscribe", chosen))  {
      renewSubscribe(client, pd);

     /*triggerSubscribe(client, pd);*/

      GoldShopPremium(client, pd);
    }

    GoldShopPremium(client, pd);

  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
       GoldShopPremium0(client);
  if(action==MenuAction_End)
    CloseHandle(menu);
}


askSubscribe(client, pd[PremiumData]) {
  new Handle:menu=CreateMenu(SubscribePremiumHandle);
  SetMenuExitBackButton(menu,true);
  SetMenuTitle(menu,"Premium Account Subscription");
  decl String:buffer[200], String:sgold[20];
  IntToStringfloat(GetGold(client), sgold, 20);
  Format(buffer, sizeof(buffer), "You have %s Gold", sgold);
  AddMenuItem(menu,"",buffer,ITEMDRAW_DISABLED);
  //Subscribe
  if(pd[PD_subscribed])
    buffer="Subscription is Enabled";
  else
    buffer="Subscription is Disabled";
  AddMenuItem(menu,"",buffer, ITEMDRAW_DISABLED);

  if( (pd[PD_daysLeft]>0) || pd[PD_subscribed] ) {
    Format(buffer, sizeof(buffer), "%d days left until renewal", pd[PD_daysLeft]);
    AddMenuItem(menu,"",buffer, ITEMDRAW_DISABLED);
  }
  else if(!pd[PD_subscribed]) {
    IntToStringfloat(pd[PD_costs],sgold,20);
    Format(buffer, sizeof(buffer),"WARNING!\nEnabling subscription will buy you\nPremium Account for %s gold for a month in the same moment!",sgold);
    AddMenuItem(menu,"",buffer, ITEMDRAW_DISABLED);
  }

  if(pd[PD_subscribed])
    buffer="Disable it";
  else
    buffer="Enable it";
  AddMenuItem(menu,"subscribe",buffer);

  DisplayMenu(menu,client,MENU_TIME_FOREVER);
}

public SubscribePremiumHandle(Handle:menu,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    new pd[PremiumData];
    getCachedPD(client, pd);
    decl String:chosen[255];
    GetMenuItem(menu,slot,chosen,sizeof(chosen));
    if(StrEqual("subscribe", chosen))  {


     triggerSubscribe(client, pd);

     GoldShopPremium(client, pd);
    }

    GoldShopPremium(client, pd);

  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
       GoldShopPremium0(client);
  if(action==MenuAction_End)
    CloseHandle(menu);
}

new PDModule:g_ff_lastmodule[65];
stock GoldShopModule(client, PDModule:module) {
  g_ff_lastmodule[client] = module;
  new pd[PremiumData];
  getCachedPD(client, pd);

  new Handle:menu=CreateMenu(ModuleShopMenuHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc pmodule menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[200];
  new String:sEnable[20];
  new String:descr[20];
  if(isPDModuleEnabled(pd, module)) {
    sEnable="Disable";
    descr="decrease";
  }
  else  {
    sEnable="Enable";
    descr="increase";
  }
  decl String:costs[10];
  decl String:monthly[10];
  IntToStringfloat( RoundToCeil( float(gPDModules[module]) / 30 * pd[PD_daysLeft]), costs, 10);
  IntToStringfloat(gPDModules[module], monthly, 10);
  Format(buffer, sizeof(buffer), "%T", "pmodule item", client,
    sEnable, sPDModules[module], costs, monthly, descr);
  AddMenuItem(menu, "accept", buffer);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}
public ModuleShopMenuHandler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {

    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));

    new data[PremiumData];
    getCachedPD(client, data);

    if(StrEqual(buffer, "accept"))  {
      new PDModule:module = g_ff_lastmodule[client];

      triggerPDModule(client, data, module);

    }

    GoldShopPremium(client, data);

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      GoldShopPremium0(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

getPremiumData(client, String:callback[],bool:preload=false)  {
  if((!preload) && (playerloaded[client]!=1) )
    return;
  new ML_object[MLoader];
  MLoader_Construct(ML_object, client, "getPremiumData2", "getPremiumError", "query"); //Construct class
  MLoader_generatePremiumQuery(ML_object, client, callback);
  //Execute ML_object
  MLoader_execute(ML_object);
}

public getPremiumError(ml[MLoader]) {
  ResetPack(ml[ML_sidePack]);
  new client = ReadPackCell(ml[ML_sidePack]);
  new String:callback[100];
  ReadPackString(ml[ML_sidePack], callback, 100);
  new pd[PremiumData];
  pd[PD_costs] = gPDModules[PDM_base];
  updateCachedPD(client, pd);
  //Call function
  new Function:func = GetFunctionByName(INVALID_HANDLE, callback);
  Call_StartFunction(INVALID_HANDLE, func);
  Call_PushCell(client);
  Call_PushArray(pd[0], sizeof(pd));
  if(Call_Finish() != SP_ERROR_NONE)  {
    decl String:err[100];
    Format(err,100,"Could not callback the function: %s",callback);
    Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
  }
}

public getPremiumData2(ml[MLoader]) {
  ResetPack(ml[ML_sidePack]);
  new client = ReadPackCell(ml[ML_sidePack]);
  new String:callback[100];
  ReadPackString(ml[ML_sidePack], callback, 100);

  new daysLeft = ReadPackCell(ml[ML_returns]);

  new String:due[40];
  ReadPackString(ml[ML_returns], due, 40);

  new modules = ReadPackCell(ml[ML_returns]);
  new subscribed = ReadPackCell(ml[ML_returns]);
  new costs = ReadPackCell(ml[ML_returns]);


  new pd[PremiumData];
  PD_Construct(pd, modules, due, (subscribed!=0), costs, daysLeft);

  //safer
  costs = 0;
  for (new any:i = 0; i < iPDModules; i++) {
    if(isPDModuleEnabled(pd, i))
      costs += gPDModules[i];
  }
  pd[PD_costs] = costs;

  updateCachedPD(client, pd);

  //Call function
  new Function:func = GetFunctionByName(INVALID_HANDLE, callback);
  Call_StartFunction(INVALID_HANDLE, func);
  Call_PushCell(client);
  Call_PushArray(pd[0], sizeof(pd));
  if(Call_Finish() != SP_ERROR_NONE)  {
    decl String:err[100];
    Format(err,100,"Could not callback the function: %s",callback);
    Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
  }

}

PD_Construct(pd[PremiumData], modules, String:expires[],
            bool:subscribed, costs, daysLeft) {
  pd[PD_modules] = modules;
  strcopy(pd[PD_sExpires],50,expires);
  pd[PD_subscribed]=subscribed;
  pd[PD_costs]=costs;
  pd[PD_daysLeft]=daysLeft;
}

//insert into wc_premium values (10, NOW(), 4, 1, 500);
stock MLoader_generatePremiumQuery(ml[MLoader], client, String:callback[]) {
  decl String:query[300];
  Format(query,300,"SELECT DATEDIFF(due, now()) AS `days_left`, wc_premium.* FROM wc_premium WHERE playerId = %d", playerids[client]);
  strcopy(ml[ML_query], 300, query);
  //ask for returns
  ml[ML_returns]=CreateDataPack();
  ml[ML_retClass]=CreateDataPack();
  ml[ML_sidePack]=CreateDataPack();
  WritePackCell(ml[ML_sidePack],client);
  WritePackString(ml[ML_sidePack],callback);

  //add returns requests
  WritePackString(ml[ML_returns],"days_left");
  WritePackCell(ml[ML_retClass],_:T_int);

  WritePackString(ml[ML_returns],"due");
  WritePackCell(ml[ML_retClass],_:T_str);

  WritePackString(ml[ML_returns],"modules");
  WritePackCell(ml[ML_retClass],_:T_int);

  WritePackString(ml[ML_returns],"subscribed");
  WritePackCell(ml[ML_retClass],_:T_int);

  WritePackString(ml[ML_returns],"costs");
  WritePackCell(ml[ML_retClass],_:T_int);

  new index = MLoader_FindIndex(ml[ML_Id]);
  SetArrayArray(g_ML, index, ml[0]);
}

bool:isPDModuleEnabled(pd[PremiumData], any:module) {
  if(module == PDM_base)
    return true;
  return ( (pd[PD_modules] & (1<<module))==(1<<module));
}

bool:renewSubscribe(client, pd[PremiumData]) {
  if(playerloaded[client]!=1) {
    LogError("error #773");
    return false;
  }



  new days = pd[PD_daysLeft];
  if(days < 0) days = 0;
  new costs = pd[PD_costs];
  costs = RoundToCeil( float(costs) / 30.0 * (30 - days));

  if(GetGold(client) < costs) {
    decl String:needed[10],String:has[10];
    IntToStringfloat(GetGold(client), has, 10);
    IntToStringfloat(costs, needed, 10);

    WCMessage(client, "no gold", client, has, needed);
    return false; //no resource

  }

  pd[PD_subscribed] = true;
  pd[PD_daysLeft] = 30;
  SpendGold(client, costs, "premium", "Purchased Premium");
  savePD(client, pd);


  decl String:format[300];
  Format(format, 300, "UPDATE wc_premium SET `due` = DATE_ADD(NOW(), INTERVAL 30 DAY) WHERE `playerId` = %d", playerids[client]);
  SQL_TQuery(TMSQL,TRepeating,format,AddQuery(Repeat, 3, format, 0 ));

  /*enablePD(client, pd);*/
  GoldShopPremium0(client);

  /*if(pd[PD_daysLeft]>0) {*/
    /*//is free*/
    /*pd[PD_subscribed]=true;*/
    /*WCMessage(client, "enabled subscription", client);*/
    /*savePD(client, pd);*/
  /*}*/
  /*else  {*/
    /*if(pd[PD_costs]<=gPDModules[PDM_base])  {*/
      /*WCMessage(client,"failed no modules selected", client);*/
      /*return false;*/
    /*}*/
    /*if(GetGold(client) < pd[PD_costs]) {*/
      /*decl String:needed[10],String:has[10];*/
      /*IntToStringfloat(GetGold(client), has, 10);*/
      /*IntToStringfloat(pd[PD_costs], needed, 10);*/

      /*WCMessage(client, "no gold", client, has, needed);*/
      /*return false; //no resource*/
    /*}*/
    /*WCMessage(client, "enabled subscription", client);*/
    /*pd[PD_subscribed]=true;*/
    /*savePD(client, pd);*/
    /*enablePD(client, pd);*/
  /*}*/


  return true; //success
}


bool:triggerSubscribe(client, pd[PremiumData]) {
  if(playerloaded[client]!=1) {
    LogError("error #773");
    return false;
  }

  new bool:isEnabled = pd[PD_subscribed];
  if(isEnabled) {
    pd[PD_subscribed] = false;
    WCMessage(client, "disabled subscription", client);
    savePD(client, pd);
  }
  else  {
    if(pd[PD_daysLeft]>0) {
      //is free
      pd[PD_subscribed]=true;
      WCMessage(client, "enabled subscription", client);
      savePD(client, pd);
    }
    else  {
      if(pd[PD_costs]<=gPDModules[PDM_base])  {
        WCMessage(client,"failed no modules selected", client);
        return false;
      }
      if(GetGold(client) < pd[PD_costs]) {
        decl String:needed[10],String:has[10];
        IntToStringfloat(GetGold(client), has, 10);
        IntToStringfloat(pd[PD_costs], needed, 10);

        WCMessage(client, "no gold", client, has, needed);
        return false; //no resource
      }
      WCMessage(client, "enabled subscription", client);
      pd[PD_subscribed]=true;
      savePD(client, pd);
      enablePD(client, pd);
    }
  }


  return true; //success
}

enablePD(client, pd[PremiumData])  {
  //update sql
  if(playerloaded[client]!=1) {
    LogError("Tried to enable PD of unloaded player");
    return;
  }
  SpendGold(client, pd[PD_costs], "premium", "Activated subscription");
  pd[PD_daysLeft]=30;

  new costs = 0;
  for (new any:i = 0; i < iPDModules; i++) {
    if(isPDModuleEnabled(pd, i))
      costs += gPDModules[i];
  }
  pd[PD_costs] = costs;
  decl String:format[300];
  Format(format, 300, "UPDATE wc_premium SET `due` = DATE_ADD(NOW(), INTERVAL 30 DAY) WHERE `playerId` = %d", playerids[client]);
  SQL_TQuery(TMSQL,TRepeating,format,AddQuery(Repeat, 3, format, 0 ));

  //update
  GoldShopPremium0(client);
}

bool:triggerPDModule(client, pd[PremiumData], any:module) {
  new bool:isEnabled = isPDModuleEnabled(pd, module);
  if(isEnabled) {
    pd[PD_modules] = pd[PD_modules] & ~(1 << module);
    WCMessage(client, "disabled pd module", client, sPDModules[module]);
  }
  else  {
    new costs = gPDModules[module];
    costs = RoundToCeil(float(costs) / 30 * pd[PD_daysLeft]);

    if(GetGold(client) < costs) {
      decl String:needed[10],String:has[10];
      IntToStringfloat(GetGold(client), has, 10);
      IntToStringfloat(costs, needed, 10);

      WCMessage(client, "no gold", client, has, needed);
      return false; //no resource
    }
    decl String:tmp[50];
    Format(tmp, 50, "Enabling module %d", module);
    SpendGold(client, costs, "premium", tmp);


    WCMessage(client, "enabled pd module", client, sPDModules[module]);
    pd[PD_modules] = pd[PD_modules] | 1 << module;
  }

  savePD(client, pd);

  return true; //success
}

savePD(client, pd[PremiumData]) {
  if(playerloaded[client]!=1) {
    LogError("Tried to save PD of unloaded player");
    return;
  }
  new costs = 0;
  for (new any:i = 0; i < iPDModules; i++) {
    if(isPDModuleEnabled(pd, i))
      costs += gPDModules[i];
  }
  pd[PD_costs] = costs;
  decl String:format[300];
  Format(format, 300, "INSERT INTO wc_premium (`playerId`,`due`,`modules`,`subscribed`,`costs`) VALUES (%d, NOW(), %d, %d, %d) ON DUPLICATE KEY UPDATE `modules` = '%d', `subscribed` = '%d', `costs` = '%d'", playerids[client],
      pd[PD_modules], pd[PD_subscribed], costs, pd[PD_modules], pd[PD_subscribed], costs);
  SQL_TQuery(TMSQL,TRepeating,format,AddQuery(Repeat, 3, format, 0 ));

  updateCachedPD(client, pd);
}

public loadPD(client, pd[PremiumData]) {
  //updateCachedPD(client, pd); //it is already included in algorithm

}

new bool:g_ff_hasCachedPD[65];
new g_ff_cachedPD[65][PremiumData];

resetCachedPD(client) {
  g_ff_hasCachedPD[client]=false;
}


updateCachedPD(client, pd[PremiumData]) {
  g_ff_hasCachedPD[client]=true;
  g_ff_cachedPD[client][PD_modules] = pd[PD_modules];
  g_ff_cachedPD[client][PD_sExpires] = pd[PD_sExpires];
  g_ff_cachedPD[client][PD_subscribed] = pd[PD_subscribed];

  new costs = 0;
  for (new any:i = 0; i < iPDModules; i++) {
    if(isPDModuleEnabled(pd, i))
      costs += gPDModules[i];
  }

  g_ff_cachedPD[client][PD_costs] = costs;
  g_ff_cachedPD[client][PD_daysLeft] = pd[PD_daysLeft];
}

bool:getCachedPD(client, pd[PremiumData]) {
  if(!g_ff_hasCachedPD[client])
    return false;
  pd[PD_modules] = g_ff_cachedPD[client][PD_modules];
  pd[PD_sExpires] = g_ff_cachedPD[client][PD_sExpires];
  pd[PD_subscribed] = g_ff_cachedPD[client][PD_subscribed];
  pd[PD_costs] = g_ff_cachedPD[client][PD_costs];
  pd[PD_daysLeft] = g_ff_cachedPD[client][PD_daysLeft];
  return true;
}

bool:isClientPDModule(client, PDModule:module=PDM_base)  {
  if(!IsClientConnected(client))
    return false;

  new pd[PremiumData];
  if(!getCachedPD(client, pd)) {
    getOutdatedPD(client, pd);
  }
  if(pd[PD_daysLeft] <= 0) {
    return false;
  }
  if(module)  { module=PDM_base; } //prevent warnings
  /*new bool:ret = isPDModuleEnabled(pd, module);*/
  //Now consider everything is enabled
  return true;
}

new Handle:outdatedPD=INVALID_HANDLE;

new g_ff_pds_tries=10;
getPDs()  {
  g_ff_pds_tries=10;
  getPDs1p5();
}

getPDs1p5() {
  if(g_ff_pds_tries<0) {
    LogError("wow fatal error #982");
    return;
  }
  g_ff_pds_tries--;
  decl String:query[400];
  Format(query, sizeof(query),"SELECT DATEDIFF(wc_premium.due, now()) AS `days_left`, wc_premium.*, `uniqueId` FROM wc_premium, wc_PlayerUniqueIds WHERE (DATEDIFF(due, now()) >= 0) AND (wc_PlayerUniqueIds.playerId = wc_premium.playerId)");
  SQL_TQuery(SSQL,getPDs2,query,0,DBPrio_High);
}

public getPDs2(Handle:db,Handle:query,String:error[],any:datapack)  {
  if(!query)  {
    LogError("Error obtaining Premium Datas, retrying. er: %s", error);
    getPDs1p5();
    return;
  }

  if(outdatedPD!=INVALID_HANDLE)  {
    CloseHandle(outdatedPD);
    outdatedPD = INVALID_HANDLE;
  }

  outdatedPD = CreateKeyValues("premium");

  while(SQL_FetchRow(query))  {
    new uniqueId = GetSQLDataInt(query, "uniqueId");
    new daysLeft = GetSQLDataInt(query, "daysLeft");
    new modules = GetSQLDataInt(query, "modules");
    new subscribed = GetSQLDataInt(query, "subscribed");
    new costs = GetSQLDataInt(query, "costs");

    //create entry
    KvRewind(outdatedPD);
    KvGotoFirstSubKey(outdatedPD);
    decl String:key[20];
    Format(key, 20, "%d", uniqueId);
    KvJumpToKey(outdatedPD, key, true);
    KvSetNum(outdatedPD, "days_left", daysLeft);
    KvSetNum(outdatedPD, "modules", modules);
    KvSetNum(outdatedPD, "subscribed", subscribed);
    KvSetNum(outdatedPD, "costs", costs);
  }

}

getOutdatedPD(client, pd[PremiumData])  {
  if(outdatedPD == INVALID_HANDLE)  {
    return;
  }
  new uniqueId = UniqueID(client);
  KvRewind(outdatedPD);
  decl String:key[20];
  Format(key, 20, "%d", uniqueId);
  if(!KvJumpToKey(outdatedPD, key, false))
    return;
  pd[PD_daysLeft] = KvGetNum(outdatedPD, "days_left");
  pd[PD_modules] = KvGetNum(outdatedPD, "modules");
  pd[PD_subscribed] = bool:KvGetNum(outdatedPD, "subscribed");
  pd[PD_costs] = KvGetNum(outdatedPD, "costs");
}

debugOutdatedPDs(client)  {
  if(outdatedPD == INVALID_HANDLE)  {
    ReplyToCommand(client, "Could not debug oudated pds, handle is invalid");
    return;
  }

  KeyValuesToFile(outdatedPD, "pds_debug.kv");
  ReplyToCommand(client, "Dumped file to cstrike/pds_debug.kv");
}

reloadPDs() {
  LogMessage("[wow] Reloading premium data cache from sql");
  getPDs();
}

bindingAccount(client, String:nick[], String:pass[])  {
  decl String:format[500];
  decl String:temp1[62];
  decl String:temp2[62];

  SQL_EscapeString(TMSQL, nick, temp1, 62);
  SQL_EscapeString(TMSQL, pass, temp2, 62);

  Format(format, 500, "SELECT SHA1(CONCAT(LOWER('%s'),'%s'))=passwd AS `equals` from mainsite.smf_members where passwd = SHA1(CONCAT(LOWER('%s'),'%s'))", temp1, temp2, temp1, temp2);

  SQL_TQuery(siteSQL,bindingAccount2,format,client);
}

public bindingAccount2(Handle:db,Handle:query,String:error[],any:client)  {
  if(!query)  {
    LogError("Error binding account: %s", error);
    bindingAccount3(client, false);
    return;
  }
  SQL_Rewind(query);
  if(SQL_FetchRow(query)) {
    bindingAccount3(client, true);
    return;
  }
  bindingAccount3(client, false);
}

bindingAccount3(client, bool:passed)  {
  new Handle:menu=CreateMenu(bindingAccountHandle);
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"Binding to Forum Account");
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[300];
  Format(buffer, sizeof(buffer), "By binding your wow mod to \nforum account, it secures your \nidentity and we will be able to\nprovide extra services like transfer\nof wow mod account to another steam.\nOh, and you get 2 gold for free!\nRight after pressing Confirm");
  AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);

  if(passed)  {
    Format(buffer, sizeof(buffer), "Click to Confirm binding");
    AddMenuItem(menu,"ConfirmBinding",buffer);
  }
  else  {
    Format(buffer, sizeof(buffer), "Your login/password are wrong");
    AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);
  }

  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public ConfirmBinding(client) {
  decl String:query[200];
  Format(query, 200, "UPDATE wc_Players SET `bound` = 1 WHERE playerId = %d",playerids[client]);
  SQL_TQuery(TMSQL,TRepeating,query,AddQuery(Repeat, 3, query, 0 ));
  WCMessage(client, "thanks for binding", client);
  AddGold(client, 200, "account", "Binding wowmod account to forum account");
  AccountMenu(client);
}

public bindingAccountHandle(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    //insert here


    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));

    //Call function
    new Function:func = GetFunctionByName(INVALID_HANDLE, buffer);
    Call_StartFunction(INVALID_HANDLE, func);
    Call_PushCell(client);
    if(Call_Finish() != SP_ERROR_NONE)  {
      decl String:err[100];
      Format(err,100,"Could not callback the function: %s",buffer);
      Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
    }

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      AccountMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

new g_clientExpansions[65];

_getClientExpansions(client)  {
  return g_clientExpansions[client];
}
_setClientExpansions(client, bitstring) {
  g_clientExpansions[client]=bitstring;
}

stock GetClientExpansions(client, Expansion:expansions[], max)  {
  new total = 0;
  for(new i=0; i<EXPANSIONS; i++)
    if(HasExpansion(client, g_expansions[i]))
      expansions[total++]=g_expansions[i];
}

bool:HasExpansion(client, any:expansion) {
  return ContainsExpansion(_getClientExpansions(client), expansion);
}

bool:ContainsExpansion(bitstring, any:expansion) {
//  if(expansion == E_None)
//    return true;
  return ( (bitstring & (1<<expansion))==(1<<expansion) );
}

_addExpansion(client, any:expansion)  {
  new bitstring = _getClientExpansions(client);
  bitstring = bitstring | 1 << expansion;
  _setClientExpansions(client, bitstring);
  return bitstring;
}


public ExpansionsMenu(client) {

  new Handle:menu=CreateMenu(ExpansionsMenuHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc expansions menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[300];
  decl String:sId[10];
  for(new i=0; i<EXPANSIONS; i++)  {
    IntToString(i, sId, 10);
    new bool:active = HasExpansion(client, g_expansions[i]);
    if(active)
      Format(buffer, sizeof(buffer), "(Activated)");
    else
      Format(buffer, sizeof(buffer), "(Not activated)");
    Format(buffer, sizeof(buffer), "%s %s", g_sExpansions[g_expansions[i]], buffer);
    AddMenuItem(menu, sId, buffer);
  }

  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public ExpansionsMenuHandler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {

    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));
    new id = StringToInt(buffer);

    ExpansionMenu(client, g_expansions[id]);

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      GoldMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

public ExpansionMenu(client, Expansion:expansion) {

  new Handle:menu=CreateMenu(ExpansionMenuHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc expansion menu",client, g_sExpansions[expansion]);
  SetMenuTitle(menu,title);

  /*** Menu Items ***/
  decl String:buffer[300];
  //Description
  Format(buffer,sizeof(buffer),"expansion_%d", expansion);
  Format(buffer,sizeof(buffer),"%T", buffer, client);
  AddMenuItem(menu, "", buffer, ITEMDRAW_DISABLED);

  //Have it?
  if(HasExpansion(client, expansion)) {
    AddMenuItem(menu, "", "This expansion is Activated", ITEMDRAW_DISABLED);
  }
  else  {
    decl String:sId[10];
    IntToString(_:expansion, sId, 10);
    Format(buffer, sizeof(buffer), "Buy %s in Special Items shop", g_sExpansions[expansion]);
    AddMenuItem(menu, sId, buffer);
    AddMenuItem(menu, "", "You have to USE item once you buy it to activate");
  }

  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public ExpansionMenuHandler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {

    ItemsMenu(client,27, true);
  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      ExpansionsMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

public GoldCharIdShop(client) {
  ChangeCharacterIdMenu(client);
}

new g_gg_goldId[65];
new Float:g_gg_goldIdTime[65];

stock Float:getCharIdSayTime(client)  {
  return g_gg_goldIdTime[client];
}
stock setGoldId(client, val)  {
  g_gg_goldId[client]=val;
  ChangeCharIdChecks(client);
}

stock ChangeCharacterIdMenu(client, String:errMsg[]="") {

  g_gg_goldId[client]=0;
  g_gg_goldIdTime[client]=GetEngineTime();

  new Handle:menu=CreateMenu(ChangeCharIdMenuHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","change char id title",client, characterids[client][0]);
  SetMenuTitle(menu,title);

  /*** Menu Items ***/
  decl String:buffer[300];
  //Description
  Format(buffer,sizeof(buffer), "%T", "type in new id", client);
  AddMenuItem(menu, "", buffer, ITEMDRAW_DISABLED);

  if(strlen(errMsg)>1)  {
    Format(buffer, sizeof(buffer), "Typed in ID was not accepted because: \n%s",errMsg);
    AddMenuItem(menu,"",buffer,ITEMDRAW_DISABLED);
  }

  /*** Send to client ***/
  DisplayMenu(menu,client,30);

}

public ChangeCharIdMenuHandler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      SpendGoldMenu(client);
    }
  if(action == MenuAction_End)  {
    CloseHandle(menu);
  }

}

stock ChangeCharIdChecks(client) {
  new id = g_gg_goldId[client];
  if(id<1 || id >999) {
    ChangeCharacterIdMenu(client,"New Id should be in range of 1 to 999!");
    return;
  }
  new String:query[255];
  Format(query, sizeof(query), "SELECT CharacterId FROM wc_Characters WHERE CharacterId = %d"
    ,id);
  //Check if old exists
  SQL_TQuery(TMSQL, TChangeCharId, query, client);
}

public TChangeCharId(Handle:db,Handle:query,String:error[],any:client)
{
  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      new bool:fetch=SQL_FetchRow(query);
      if(fetch)
      {
        //Already exists
        decl String:txt[100];
        Format(txt,100,"Selected Id (%s) already exists. Try another", g_gg_goldId[client]);
        ChangeCharacterIdMenu(client, txt);
      }
      else  {
        ChangeCharacterIdMenu2(client);
      }
    }
    else
    {
      LogError ("WC FAILED 1484 %s",error);
    }
  }
}

ChangeCharacterIdMenu2(client )  {
  new Handle:menu=CreateMenu(ChangeCharIdMenu2Handler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  new id = g_gg_goldId[client];
  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","change char id title2",client);
  SetMenuTitle(menu,title);

  //Costs
  new gold=3500;
  if(id>=10)
    gold=1500;
  if(id>=100)
    gold=800;

  /*** Menu Items ***/
  decl String:buffer[300];
  //Description
  decl String:sgold[25];
  IntToStringfloat(gold, sgold, 25);
  Format(buffer,sizeof(buffer), "%T", "changing char id from to", client, characterids[client][0], id, sgold);
  AddMenuItem(menu, "", buffer, ITEMDRAW_DISABLED);

  new bool:pass=true;
  if(gold > GetGold(client))  {
    pass=false;
    Format(buffer,sizeof(buffer),"%T","no gold2", client);
    AddMenuItem(menu,"",buffer,ITEMDRAW_DISABLED);
  }
  Format(buffer,sizeof(buffer), "%T", "change id costs", client);
  AddMenuItem(menu,"",buffer,ITEMDRAW_DISABLED);

  Format(buffer,sizeof(buffer),"%T","purchase",client,"Character Id");

  if(pass)
    AddMenuItem(menu,"",buffer);
  else
    AddMenuItem(menu,"",buffer,ITEMDRAW_DISABLED);

  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}


public ChangeCharIdMenu2Handler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {

    new id = g_gg_goldId[client];

    //Costs
    new gold=3500;
    if(id>=10)
      gold=1500;
    if(id>=100)
      gold=800;

    //Log and take gold
    decl String:buffer[50];
    Format(buffer,sizeof(buffer),"Bought Character Id: %d", id);
    if(!SpendGold(client, gold, "gold shop", buffer)) {
      WCMessage(client,"no gold2",client);
      return;
    }

    //Change data on server
    new oldId = characterids[client][0];
    new index=0;
    for(new i=0;i<=15;i++)  {
      if(characterids[client][i] == oldId)  {
        characterids[client][i]=id;
        index=i;
      }
    }

    //Change data on SQL
    new String:query[300];
    Format(query,sizeof(query),"UPDATE wc_Players SET character0 = %d, character%d = %d WHERE playerId = %d",
      id, index, id, playerids[client]);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));

    Format(query,sizeof(query), "update wc_Characters SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_CharacterInventory SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_CharacterInventory2 SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_CharacterMail SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_CharacterSpellbar SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_CharacterSpells SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_CItems SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_ignoredItems SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_Mail SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_Professions1 SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_Professions2 SET CharacterId = %d WHERE CharacterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));
    Format(query,sizeof(query), "update wc_Spells SET characterId = %d WHERE characterId = %d", id, oldId);
    SQL_TQuery(TSQL,TRepeating,query, AddQuery(Repeat, 3, query, 0 ));


    WCMessage(client, "success changing char id", client, oldId, id);


  }
  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      SpendGoldMenu(client);
    }
  if(action == MenuAction_End)  {
    CloseHandle(menu);
  }

}

new String:g_ffCPPath[65][200];

stock PrepareCP(client, String:path[]="")  {
  //Basic menu tests
  new bool:pass=true;
  strcopy(g_ffCPPath[client], 200, path);
  if(playerloaded[client]<1)
  {
    pass=false;
    WCMessage(client,"char not loaded",client);
  }
  if(GetMenuOff(client)>GetTickedTime())
  {
    pass=false;
    WCMessage(client,"menu is loading",client,GetMenuOff(client)-GetTickedTime());
  }
  if(!pass) return;

  //CP is cp.wowmod.eu  -> contorl panel of wowmod on website
  //It uses login/pass specified in account information
  //Let client know each step, should be useful for debugging and look 'proish' :D
  WCMessage(client, "cp stage 1",client);
  SetMenuOff(client,30.0);
  decl String:buffer[100];
  Format(buffer,sizeof(buffer),"SELECT login, pass, bound FROM wc_Players WHERE (playerId=%i)",playerids[client]);
  SQL_TQuery(TSQL,TPrepareCP2,buffer,client);
}

public TPrepareCP2(Handle:db,Handle:query,String:error[],any:client)
{
  if(! IsClientInGame(client))
    return;
  if(playerloaded[client] < 1)
    return;

  SetMenuOff(client,15.0);
  if(query)
  {
    decl String:sQuery[255];
    SQL_Rewind(query);
    new String:login[30], String:pass[30];
    new bool:found=false;
    if(SQL_FetchRow(query))
    {
      GetSQLDataStr(query, "login", login, sizeof(login));
      GetSQLDataStr(query, "pass", pass, sizeof(pass));
      if(strlen(login) > 1 && strlen(pass) > 2) {
        found = true;
        WCMessage(client, "cp stage 2 loaded", client, login);
      }
    }
    if(!found)  {
      if(strlen(login) < 3)
        Format(login, sizeof(login), "player_%d", playerids[client]);
      new String:listOfChars[] = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ0123456789";
      PrintToServer("pass now: %s", pass);
      for (new i=0; i<6;i++)  {
        new randomInt = GetRandomInt(0,sizeof(listOfChars)-1);
        Format(pass, sizeof(pass), "%s%c", pass, listOfChars[randomInt]);
      }
      PrintToServer("pass after: %s", pass);
      Format(sQuery, sizeof(sQuery), "UPDATE wc_Players set `login` = '%s', `pass` = '%s' WHERE playerId = %d",
        login, pass, playerids[client]);

      SQL_TQuery(TSQL,Tinserted,sQuery,1680);

      WCMessage(client, "cp stage 2 created", client, login, pass);

    }
    //Obtain SHA1 key
    Format(sQuery, sizeof(sQuery), "SELECT SHA1('%s') as `key`, '%s' as `login`", pass, login);
    SQL_TQuery(TSQL, TPrepareCP3, sQuery, client);

    return;

  }
  SetMenuOff(client, 0.0);
  WCMessage(client,"failed loading",client);
  LogError ("WC FAILED gold.1694 %s",error);
}


public TPrepareCP3(Handle:db,Handle:query,String:error[],any:client)
{
  if(! IsClientInGame(client))
    return;
  if(playerloaded[client] < 1)
    return;

  SetMenuOff(client,0.0);
  new bool:success=false;
  if(query)
  {
    SQL_Rewind(query);
    decl String:login[30], String:key[50];
    if(SQL_FetchRow(query))
    {
      success=true;
      GetSQLDataStr(query, "login", login, sizeof(login));
      GetSQLDataStr(query, "key", key, sizeof(key));

      //Not open control panel, not OpenCP you might see in the web :)
      OpenCP(client, login, key, g_ffCPPath[client]);
    }

  }
  if(!success)  {
    WCMessage(client,"failed loading",client);
    LogError ("WC FAILED gold.1723 %s",error);
  }
}

stock OpenCP(client, String:login[], String:key[], String:path[]) {

  decl String:fullPath[300];
  Format(fullPath, sizeof(fullPath), "http://cp.wowmod.eu/motd_login/%s/%s/%s", login, key, path);
  ShowMOTDPanel(client, "WoW Control Panel", fullPath, MOTDPANEL_TYPE_URL);
}


stock GetPremiumCost(client, data[PremiumData]) {
  return 500; //let's say it's 5g now / month. TODO unfinished
}
