/*
* File: wcmenus.inc
* Description: All menus and handling for mod
* Author(s): Stinkyfax
*/

#pragma semicolon 1

new lastMenu[65],lastMenu2[65],lastMenu3[65];
new lastProf[65][3];

new invSorting[MAXPLAYERS+1];
enum Sortable
{
  Sortable_NoSort = 0,

  Sortable_Gear,

  Sortable_Legendary,
  Sortable_Epic,
  Sortable_Rare,
  Sortable_Green,
  Sortable_PVPEpic,
  Sortable_PVPRare,

  Sortable_Reagents,
  Sortable_RPblacksmith,
  Sortable_RPjewelcrafting,
  Sortable_RPtailoring,
  Sortable_RPleatherworking,
  Sortable_RPenchanting,
  Sortable_Scrolls,
  Sortable_Inscription,
  Sortable_General,
  Sortable_Randomboxes,
  Sortable_Glyphs
}
enum SortableReal
{
  SortableReal_NoSort		=0,

  SortableRealNS_Legendary	=4,
  SortableRealNS_Epic		=3,
  SortableRealNS_Rare		=2,
  SortableRealNS_Green		=1,
  SortableRealNS_PVPEpic		=5,
  SortableRealNS_PVPRare		=6,

  SortableRealCS_Reagents		=8,
  SortableRealCS_RPblacksmith	=1,
  SortableRealCS_RPjewelcrafting	=2,
  SortableRealCS_RPtailoring	=3,
  SortableRealCS_RPleatherworking	=4,
  SortableRealCS_RPenchanting	=5,
  SortableRealCS_Scrolls = 25,
  SortableRealCS_Inscription = 9,
  SortableRealCS_General = 27,
  SortableRealCS_Randomboxes=24,
  SortableRealCS_Glyphs=30,
}

stock SetMenuOff(client,Float:time, String:phrase[]="")
{
   PlayerMenuOff[client]=GetTickedTime()+time;
   if(!StrEqual("", phrase))	{
   		WCMessage(client, phrase, client);
   }
}

public Float:GetMenuOff(client)
{
   return PlayerMenuOff[client];
}


DebugMenu(client)
{
  new bool:pass=true;
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
  if(pass)
  {
  SetMenuOff(client, 10.0);
  new Handle:menu_main=CreateMenu(DebugMenuHandle);
  decl String:title[128],String:line[128];
  Format(title,sizeof(title),"%T","wc debug menu",client);
  SetMenuTitle(menu_main,title);
  Format(line,sizeof(line),"%T","wc debug menu warning",client);
  AddMenuItem(menu_main,line,line,ITEMDRAW_DISABLED);

  Format(line,sizeof(line),"%T","wc debug menu mail",client);
  AddMenuItem(menu_main,"mail",line); //1

  DisplayMenu(menu_main,client,MENU_TIME_FOREVER);
  }
}

public DebugMenuHandle(Handle:menu_main,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    switch(slot)
    {
      case 1:
         MailDebugMenu(client);

    }
  }
  if(action==MenuAction_End)
    CloseHandle(menu_main);
}

MailDebugMenu(client) {
  ClearMailMenu(client);
  SetMenuOff(client,30.0);
  decl String:buffer[100];
  Format(buffer,sizeof(buffer),"SELECT * FROM wc_CharacterMail WHERE (CharacterId=%i)",characterids[client][0]);

  SQL_TQuery(TSQL,TMailDebugMenu,buffer,client);
}

public TMailDebugMenu(Handle:db,Handle:query,String:error[],any:client)
{

  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      new mail;
      if(SQL_FetchRow(query))
      {
        decl String:column[50];
        //PrintToServer("fetched 759");

        for(new x=1;x<=50;x++)
        {
          Format(column,sizeof(column),"mail_%i",x);
          //PrintToServer("mailid = %i at place of %i",GetSQLDataInt(query,column),x);
          ClientMailDataId[client][x]=GetSQLDataInt(query,column);
          if(ClientMailDataId[client][x]>0)
            mail++;
        }
      }
      charactersdata[client][CurrentCharacterIndex(client)][1]=mail;
      MailDebugMenu2(client);
    }
    else
    {
      WCMessage(client,"failed loading mail",client);
      LogError ("WC FAILED m.106 %s",error);
    }
  }
}

public MailDebugMenu2(client)
{
  decl String:buffer[200];
  for(new x=1;x<=50;x++)
  {
    new mailid=ClientMailDataId[client][x];
    ClientMailDebug[client][x]=0;
    //PrintToServer("checking mail with index %i and id %i",x,mailid);
    if(mailid>0)
    {
      ClientMailDebug[client][x]=1;
      //PrintToServer("getting mail %i",mailid);
      ClientMailQueue[client]--;
      Format(buffer,sizeof(buffer),"SELECT mailId FROM wc_Mail WHERE MailId = '%i'",mailid);

      SQL_TQuery(TSQL,PreloadMailDebug,buffer,client);
    }
  }
  if(ClientMailQueue[client]==0)
    MailDebugDisplay(client);
}


public PreloadMailDebug(Handle:db,Handle:query,String:error[],any:client)
{
  if(IsClientConnected(client))
  {
    ClientMailQueue[client]++;
    if(query)
    {
      SQL_Rewind(query);
      if(SQL_FetchRow(query))
      {
        new mailid=GetSQLDataInt(query,"mailId");
        new x=0;
        for (new y=1;y<=50;y++)
        {
          if (ClientMailDataId[client][y]==mailid)
          {
            x=y;
            break;
          }
        }
        ClientMailDebug[client][x]=0;
      }
    }
    if(ClientMailQueue[client]==0)
      MailDebugDisplay(client);
  }
}

MailDebugDisplay(client)
{

  SetMenuOff(client, 10.0);
  new Handle:menu_main=CreateMenu(MailDebugDispHandle);
  decl String:title[128],String:line[128];
  Format(title,sizeof(title),"%T","wc debug mail",client);
  SetMenuTitle(menu_main,title);
  Format(line,sizeof(line),"%T","wc debug mail1",client);
  AddMenuItem(menu_main,line,line,ITEMDRAW_DISABLED);

  //Display list
  new amount=0;
  for(new i=1;i<=50;i++) {
    if(ClientMailDebug[client][i]==1)  {
      amount++;
      Format(line,sizeof(line),"%T","wc debug menu mail3",client, ClientMailDataId[client][i]);
      AddMenuItem(menu_main,"mail",line,ITEMDRAW_DISABLED);
    }
  }

  Format(line,sizeof(line),"%T","wc debug menu mail2",client);
  if(amount>0)
    AddMenuItem(menu_main,"mail",line); //1
  else
    AddMenuItem(menu_main,"mail",line, ITEMDRAW_DISABLED); //1

  DisplayMenu(menu_main,client,MENU_TIME_FOREVER);
}

public MailDebugDispHandle(Handle:menu_main,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    WCMessage(client, "fixing mails", client);
    for(new i=1;i<=50;i++) {
      if(ClientMailDebug[client][i]==1)  {
        decl String:line[100];
        Format(line, sizeof(line), "mail_%i", i);
        wcUpdatePlayerInt(characterids[client][0],line,0,"CharacterId","wc_CharacterMail");
      }
    }
  }
  if(action==MenuAction_End)
    CloseHandle(menu_main);
}



WCMenu(client)
{
  new bool:pass=true;
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
  if(pass)
  {
    if(GetClass(client)==0) {
      wcClassChoose(client);
      return;
    }

    invSorting[client]=0;
    new Handle:menu_main=CreateMenu(WCMenuHandle);
    decl String:title[128],String:line[128];
    Format(title,sizeof(title),"%T","wc main menu",client,playerids[client], characterids[client][0]);
    SetMenuTitle(menu_main,title);
    Format(line, sizeof(line), "%T", "wc shops", client);
    AddMenuItem(menu_main, line, line);

    Format(line,sizeof(line),"%T","wc playerstats menu",client);
    AddMenuItem(menu_main,line,line);
    Format(line,sizeof(line),"%T","mclass",client);
    AddMenuItem(menu_main,line,line);
    Format(line,sizeof(line),"%T","wc gold menu",client);
    AddMenuItem(menu_main,line,line);
    Format(line,sizeof(line),"%T","wc other players",client);
    AddMenuItem(menu_main,line,line);
    Format(line,sizeof(line),"%T","wc help menu",client);
    AddMenuItem(menu_main,line,line);
    Format(line,sizeof(line),"%T","wc main arena",client);
    AddMenuItem(menu_main,line,line);
    /*Format(line,sizeof(line),"%T","wc ranking page",client);*/
    /*if(RANKING_ENABLE)*/
    /*AddMenuItem(menu_main,line,line);*/
    /*else*/
    /*AddMenuItem(menu_main,line,line,ITEMDRAW_DISABLED);*/
    /*Format(line,sizeof(line),"%T","wc account page",client);*/
    /*if(PASS_ENABLED)*/
    /*AddMenuItem(menu_main,line,line);*/
    /*else*/
    /*AddMenuItem(menu_main,line,line,ITEMDRAW_DISABLED);*/

    DisplayMenu(menu_main,client,MENU_TIME_FOREVER);
  }
}

public WCMenuHandle(Handle:menu_main,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    switch(slot)
    {
      case 0:
        ShopsMenu(client);
      /*case 1:*/
         /*if(ArenaAllowMenu(client))*/
            /*InvSortMenu(client);*/
         /*else*/
            /*WCMessage(client,"arena locked", client);*/
      /*case 2:*/
         /*if(ArenaAllowMenu(client))*/
            /*CharacterMenu(client);*/
         /*else*/
            /*WCMessage(client,"arena locked", client);*/

      case 1:
        wcPlayerStats(client);
      case 2:
      {
        if(GetClass(client)==0)
          wcClassChoose(client);
        else
          ClassMenu(client);
      }
      case 6:  {
         if(!bArenaActive)
            MainArenaMenu(client,true);
         else
            MainArenaMenu(client,false);
      }
      case 10:  {
         AccountMenu(client);
      }
      case 5:
      {
        new ids[20];
        HelpMenu(client,ids,0);
      }
      case 4:
         wcOtherPlayers(client);
      case 9:
         ShowMOTDPanel(client, "WoW Top 100", RANKING_PAGE, MOTDPANEL_TYPE_URL);
      case 3:
        GoldMenu(client);
    }
  }
  if(action==MenuAction_End)
    CloseHandle(menu_main);
}

stock ShopsMenu(client) {

  new Handle:menu=CreateMenu(ShopsMenuHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc shops",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[200];
  Format(buffer, sizeof(buffer), "%T", "wc shop menu", client);
  AddMenuItem(menu,"CategoryMenu",buffer);

  Format(buffer,sizeof(buffer),"%T","wc Tinventory menu",client);
  AddMenuItem(menu,"InvSortMenu",buffer);

  Format(buffer,sizeof(buffer),"%T","wc character menu",client);
  AddMenuItem(menu,"CharacterMenu",buffer);

  Format(buffer,sizeof(buffer),"%T","wc auction",client);
  AddMenuItem(menu,"AuctionsMenu",buffer);

  Format(buffer,sizeof(buffer),"%T","wc mail menu",client);
  new mail=charactersdata[client][wcUsers[client][247]][1];
  if(mail>0)
    Format(buffer,sizeof(buffer),"%s %T",buffer,"mail inbox amount",client,mail);
  AddMenuItem(menu,"MailMenu",buffer);

  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public ShopsMenuHandler(Handle:menu, //rename
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    //insert here

    /*** Comment out to treat meta-data as functions with 'client' argument ***/

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



public wcOtherPlayers(client)
{
   new Handle:menu_op=CreateMenu(OtherPlayersHandle);
   SetMenuExitBackButton(menu_op,true);
   decl String:line[255];
   Format(line,sizeof(line),"%T","wc other players",client);
   SetMenuTitle(menu_op,line);
   decl String:buffer[5];
   new maxplayers=GetMaxClients();
   for (new i=1;i<=maxplayers;i++)
   {
      if(IsClientInGame(i))
         if(IsClientAuthorized(i))
         {
            decl String:Srace[255];
            new race=GetClass(i);
            decl String:Sname[20];
            GetClientName(i,Sname,sizeof(Sname));
            wcClassToName(i,race,Srace,sizeof(Srace), client);
            Format(line,sizeof(line),"%s (%s  %i)",Sname,Srace,GetLevel(i));
            IntToString(i,buffer,sizeof(buffer));
            AddMenuItem(menu_op,buffer,line);
         }
   }
   DisplayMenu(menu_op,client,MENU_TIME_FOREVER);
}

public OtherPlayersHandle(Handle:menu_op,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
   decl String:buffer[5];
   GetMenuItem(menu_op,slot,buffer,sizeof(buffer));
   new id=StringToInt(buffer);
   OtherPlayerMenu(client,id);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      WCMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_op);
}

public OtherPlayerMenu(client,id)
{
   if(IsClientInGame(id))
   if(IsClientAuthorized(id))
   {
   new Handle:menu_op=CreateMenu(OtherPlayerHandle);
   SetMenuExitBackButton(menu_op,true);
   decl String:line[255];
   new String:buffer[255];
   GetClientName(id,line,sizeof(line));
   Format(line,sizeof(line),"%s (ID = %i)",line,characterids[id][0]);
   SetMenuTitle(menu_op,line);
   new ibuffer=GetClass(id);
   wcClassToName(id,ibuffer,line,sizeof(line));
   Format(line,sizeof(line),"%T","class",client,line);
   new String:sid[10];
   IntToString(id, sid, sizeof(sid));
   AddMenuItem(menu_op,sid,line,ITEMDRAW_DISABLED);
   new bool:allowed=(GetAdminFlag(GetUserAdmin(client), Admin_Custom5));
   if(allowed)
   {
    Format(buffer,sizeof(buffer),"%T","money admin",client,GetMoney(id));
    AddMenuItem(menu_op,"money",buffer,ITEMDRAW_DISABLED);
    decl String:sgold[20];
    IntToStringfloat(GetGold(id), sgold, 20);
    Format(buffer,sizeof(buffer),"%T","gold admin",client,sgold);
    AddMenuItem(menu_op,"gold",buffer,ITEMDRAW_DISABLED);

   }
   new Float:rating = GetPlayerRating(id);
   Format(line,sizeof(line), "%T", "rating", client, rating);
   AddMenuItem(menu_op,"rating",line,ITEMDRAW_DISABLED);
   new prof1=GetProfession(id,1);
   new prof2=GetProfession(id,2);
   decl String:Sprof1[60],String:Sprof2[60];
   ProfToName(prof1,Sprof1,sizeof(Sprof1),id);
   ProfToName(prof2,Sprof2,sizeof(Sprof2),id);
   Format(Sprof1,sizeof(Sprof1),"%s (%i)",Sprof1,GetProfessionSk(id,1));
   Format(Sprof2,sizeof(Sprof2),"%s (%i)",Sprof2,GetProfessionSk(id,2));
   AddMenuItem(menu_op,"0",Sprof1,ITEMDRAW_DISABLED);
   AddMenuItem(menu_op,"0",Sprof2,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","xp/reqxp/lvl",client,GetXp(id),GetReqXp(id),GetLevel(id));
   AddMenuItem(menu_op,"xp",buffer,ITEMDRAW_DISABLED);
   Format(line,sizeof(line),"%T", "wc arena points", client, GetPlayerPoints(id), GetArenaPlayed(id));
   AddMenuItem(menu_op,line,line,ITEMDRAW_DISABLED);
   Format(line,sizeof(line),"%T", "wc honor", client, GetHonor(id));
   AddMenuItem(menu_op,line,line,ITEMDRAW_DISABLED);
   //PVP Rank
   TranslateRank(GetRank(id),line,sizeof(line));
   Format(line, sizeof(line),"%T", "wc rank", client, line);
   AddMenuItem(menu_op, "rank", line, ITEMDRAW_DISABLED);
   //end
   Format(buffer,sizeof(buffer),"%T","str",client,GetStr(id),RoundToNearest(wcClientsStats[id][STR]),(GetStr(id)+RoundToNearest(wcClientsStats[id][STR])));
   AddMenuItem(menu_op,"str",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","agi",client,GetAgi(id),RoundToNearest(wcClientsStats[id][AGI]),(GetAgi(id)+RoundToNearest(wcClientsStats[id][AGI])));
   AddMenuItem(menu_op,"agi",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","int",client,GetInt(id),RoundToNearest(wcClientsStats[id][INT]),(GetInt(id)+RoundToNearest(wcClientsStats[id][INT])));
   AddMenuItem(menu_op,"int",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","sta",client,GetSta(id),RoundToNearest(wcClientsStats[id][STA]),(GetSta(id)+RoundToNearest(wcClientsStats[id][STA])));
   AddMenuItem(menu_op,"sta",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","damage",client,GetDamage(id));
   AddMenuItem(menu_op,"damage",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","bonusspelldmg",client,GetSpellDmg(id,1.0));
   AddMenuItem(menu_op,"spelldmg",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","armor",client,RoundToNearest(wcClientsStats[id][ARMOR]));
   AddMenuItem(menu_op,"armor",buffer,ITEMDRAW_DISABLED);
   new Float:armor=wcClientsStats[id][ARMOR];
// formula damage reduction: damage reduction (armor / (armor + ((25*level)+20))
   new rdmg=RoundToNearest((1.0-GetDmgRed(RoundToNearest(armor), id))*100.0);
   Format(buffer,sizeof(buffer),"%T","red to lvl",client,rdmg);
   AddMenuItem(menu_op,"red to lvl",buffer,ITEMDRAW_DISABLED);
   // resistance
   Format(buffer,sizeof(buffer),"%T","resistance",client,RoundToNearest(wcClientsStats[id][RESISTANCE]));
   AddMenuItem(menu_op,"resistance",buffer,ITEMDRAW_DISABLED);
   //end
   // critical strike
   Format(buffer,sizeof(buffer),"%T","critical",client,RoundToNearest(GetCriticalStrike(id) * 100.0));
   AddMenuItem(menu_op,"critical",buffer,ITEMDRAW_DISABLED);
   //end
   //Accuracy
   Format(buffer,sizeof(buffer),"%T","accuracy",client,RoundToNearest(GetAccuracy(id) * 100.0));
   AddMenuItem(menu_op,"accuracy",buffer,ITEMDRAW_DISABLED);
   //end
   // resilence
   Format(buffer,sizeof(buffer),"%T","resilence",client,GetResilence(id));
   AddMenuItem(menu_op,"resilence",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","resilence c chance",client,GetMinusCritChance(id) * 100.0);
   AddMenuItem(menu_op,"resilence",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","resilence c dmg",client,GetMinusCrit(id) * 100.0);
   AddMenuItem(menu_op,"resilence",buffer,ITEMDRAW_DISABLED);
   //end
   // block chance
   Format(buffer,sizeof(buffer),"%T","block",client,RoundToNearest(GetBlockChance(id) * 100.0));
   AddMenuItem(menu_op,"block",buffer,ITEMDRAW_DISABLED);
   //end
   // Spell Reduction
   new sred;
   new Float:spred=GetSpellReduction(id,0.0);
   sred=RoundToNearest(spred * 100.0)-100;
   sred*=-1;
   Format(buffer,sizeof(buffer),"%T","red to res",client,sred);
   AddMenuItem(menu_op,"red to res",buffer,ITEMDRAW_DISABLED);
   //end
   Format(buffer,sizeof(buffer),"%T","haste",client,GetHasteEffect(id)*100,GetHaste(id));
   AddMenuItem(menu_op,"haste",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","parry",client,GetParryChance(id)*100.0,GetParryReduce(id)*100.0);
   AddMenuItem(menu_op,"parry",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","ai",client,RoundToNearest(wcClientsStats[id][ARMORIGNORE]));
   AddMenuItem(menu_op,"ai",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","asws",client,RoundToNearest(wcClientsStats[id][ATTACKSPEED]*100.0),RoundToNearest(GetWeaponSpeed(id)*100.0));
   AddMenuItem(menu_op,"as",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","speed",client,RoundToNearest(wcClientsStats[id][MOVESPEED]*100.0));
   AddMenuItem(menu_op,"speed",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","maxhp",client,GetMaxHealth(id));
   AddMenuItem(menu_op,"maxhp",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","maxmp",client,RoundToNearest(wcClientsStats[id][MAXMP]));
   AddMenuItem(menu_op,"maxmp",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","mregen",client,RoundToNearest(wcClientsStats[id][MPREGEN]));
   AddMenuItem(menu_op,"mregen",buffer,ITEMDRAW_DISABLED);
   //Talents
   if(allowed)
   {
    new treetotal = 0;
    for(new i=1; i<10;i++)
    {
      wcGetTalentInfo(GetClass(id),i,0,0,id,"name",line, client);
      if(!StrEqual(line, "0", false))
      {
        treetotal = wcGetTreeTotal(id, getActiveSpec(id), i);
        Format(line,sizeof(line),"%s (%d)",line, treetotal);
        IntToString((i+600),buffer,sizeof(buffer));
        AddMenuItem(menu_op,buffer,line);
      }
      else break;
    }
   }
   // items
   decl String:Scategory[255],String:Sitem[255];
   new item[UniqueItem];
   GetCharItem(item, id, 22);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line, sizeof(line), "%T", "Right Hand", client);
   Format(line,sizeof(line),"%s: %s (%s)",line,Sitem,Scategory);
   AddMenuItem(menu_op,"222",line); // 1

   GetCharItem(item, id, 18);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Left Hand: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"218",line); // 2

   GetCharItem(item, id, 14);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Helmet: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"214",line); // 3

   GetCharItem(item, id, 16);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Neck: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"216",line); // 4

   GetCharItem(item, id, 19);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Shoulders: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"219",line); // 5

   GetCharItem(item, id, 10);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Back: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"210",line); // 6

   GetCharItem(item, id, 11);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Chest: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"211",line); // 7

   GetCharItem(item, id, 23);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Wrist: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"223",line); // 8

   GetCharItem(item, id, 13);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Hands: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"213",line); // 9

   GetCharItem(item, id, 21);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Waist: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"221",line); // 21

   GetCharItem(item, id, 15);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Legs: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"215",line); // 11

   GetCharItem(item, id, 12);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Boots: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"212",line); // 12

   GetCharItem(item, id, 17);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Ring: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"217",line); // 13

   GetCharItem(item, id, 20);
   IdToCategory(item[Item_category],Scategory);
   if((item[Item_defId]==0) && (item[Item_id]==0))
    Format(Sitem,sizeof(Sitem),"%T","empty",client);
   else
    Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
   Format(line,sizeof(line),"Trinket: %s (%s)",Sitem,Scategory);
   AddMenuItem(menu_op,"220",line); // 14
   if(allowed)
   {
    Format(buffer,sizeof(buffer),"%T","slots admin",client,GetMoney(id));
    AddMenuItem(menu_op,"slots",buffer,ITEMDRAW_DISABLED);
    for (new x=1;x<=100;x++)
    {
      GetInvItem(item, id,x);
      IntToString(x,buffer,sizeof(buffer));
      if((item[Item_defId]==0) && (item[Item_id]==0))
        Format(Sitem,sizeof(Sitem),"%T","empty",client);
      else
        Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
      TranslateCategory(item[Item_category],Scategory, client);
      Format(line,sizeof(line),"slot %i: %s (%s)",x,Sitem,Scategory);
      AddMenuItem(menu_op,buffer,line);
    }
   }
   DisplayMenu(menu_op,client,MENU_TIME_FOREVER);
   }
   else
    WCMessage(client,"no player found",client);

}

public OtherPlayerHandle(Handle:menu_character,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
      new String:buffer[255];
      GetMenuItem(menu_character,slot,buffer,sizeof(buffer));
      new ID=StringToInt(buffer);
      GetMenuItem(menu_character,0,buffer,sizeof(buffer));
      new target=StringToInt(buffer);
      new item[UniqueItem];
      if(ID>=600)
      {
        ID-=600;
        wcViewOthersTalents(client,target,ID);
      }
      else
      {
        if(ID>=200) {
          GetCharItem(item, target, ID-200);
        }
        else  {
          GetInvItem(item, target, ID);
        }
        lastMenu[client]=1;
        lastMenu2[client]=target;
        ItemInfMenu(client,item,false);
      }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      wcOtherPlayers(client);
  if(action==MenuAction_End)
    CloseHandle(menu_character);
}

public wcClassChoose(client)
{
  new Handle:menu_classchoose=CreateMenu(ClassChooseHandle);
  SetMenuExitBackButton(menu_classchoose,true);
  decl String:title[128],String:line[128];
  Format(title,sizeof(title),"%T","wc class choose",client);
  SetMenuTitle(menu_classchoose,title);
  for (new i=1;i<=7;i++)
  {
    wcClassToName(client,i,line,sizeof(line));
    AddMenuItem(menu_classchoose,"class",line);
    //else
    //  AddMenuItem(menu_classchoose,"class",line,ITEMDRAW_DISABLED);
  }
  wcClassToName(client,8,line,sizeof(line));
  if(HasExpansion(client, Expansion:E_DK))
    AddMenuItem(menu_classchoose,"class",line);
  else  {
    Format(line, sizeof(line), "%s (%T)", line, "dk expansion required", client);
    AddMenuItem(menu_classchoose, "class", line, ITEMDRAW_DISABLED);
  }
  wcClassToName(client,9,line,sizeof(line));
  if(HasExpansion(client, Expansion:E_MONK))
    AddMenuItem(menu_classchoose,"class",line);
  else  {
    Format(line, sizeof(line), "%s (%T)", line, "monk expansion required", client);
    AddMenuItem(menu_classchoose, "class", line, ITEMDRAW_DISABLED);
  }

  DisplayMenu(menu_classchoose,client,MENU_TIME_FOREVER);
}

public ClassChooseHandle(Handle:menu_classchoose,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    wcSetClass(client,slot+1);
    decl String:race[128];
    wcClassToName(client,slot+1,race,sizeof(race));
    WCMessage(client,"your class",client,race);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      WCMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_classchoose);
}

public ClassMenu(client)
{
  new Handle:menu_class=CreateMenu(ClassMenuHandle);
  SetMenuExitBackButton(menu_class,true);
  decl String:title[128],String:line[128];
  wcClassToName(client,GetClass(client),line,sizeof(line));
  Format(title,sizeof(title),"%T","class menu",client,line);
  SetMenuTitle(menu_class,title);
  Format(line,sizeof(line),"%T","change character",client);
  AddMenuItem(menu_class,"change",line); // 0

  Format(line,sizeof(line),"%T","wc class spells",client);
  AddMenuItem(menu_class,line,line);
  Format(line,sizeof(line),"%T","wc manage spells",client);
  AddMenuItem(menu_class,line,line);
  Format(line,sizeof(line),"%T","wc spellbook",client);
  AddMenuItem(menu_class,line,line);
  if(GetClass(client)!=4) {
    Format(line,sizeof(line),"%T","wc casting manager",client);
    AddMenuItem(menu_class,"", line);
  }
  else  {
    Format(line,sizeof(line),"%T","wc seals",client);
    AddMenuItem(menu_class,"", line);
  }
  Format(line,sizeof(line),"%T","wc professions",client);
  AddMenuItem(menu_class,line,line);
  Format(line,sizeof(line),"%T","wc talents",client);
  AddMenuItem(menu_class,line,line);

  /*Format(line,sizeof(line),"%T","wc buy spells",client);*/
  /*AddMenuItem(menu_class,line,line);*/
  DisplayMenu(menu_class,client,MENU_TIME_FOREVER);
}

public ClassMenuHandle(Handle:menu_class,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    switch(slot)
    {
      case 0:
        ChangeCharacterMenu(client);
      case 1:
        SpellBar(client);
      case 2:
        ManageSpells(client);
      case 3:
        SpellBook(client);
      case 4:
        if(GetClass(client)!=4)
          displayCastSettingsMenu(client);
        else
          displaySealsMenu(client);
      case 5:
        ProfessionsMenu(client);
      case 6:
        TalentsMenu(client);
      /*case 7:*/
      /*{*/
        /*new race=GetClass(client);*/
        /*if (race != 0)*/
          /*BuySpells(client, GetSpellsHandle(race));*/
      /*}*/

    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      WCMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_class);
}

public TalentsMenu(client)
{
  new Handle:menu_talents=CreateMenu(TalentsMenuHandle);
  SetMenuExitBackButton(menu_talents,true);
  decl String:title[128],String:line[255],String:sId[10];
  Format(title,sizeof(title),"%T","wc talents",client,line);
  SetMenuTitle(menu_talents,title);
  Format(line, sizeof(line), "%T", "Unused Talents", client, getTPoints(client, getActiveSpec(client)));
  AddMenuItem(menu_talents,line, line, ITEMDRAW_DISABLED);

  if(isClientPDModule(client, PDModule:PDM_dual)) {
    Format(line, sizeof(line), "Dual talent: Switch spec to %d/%d/%d",
      wcGetTreeTotal(client, getInactiveSpec(client), 1),
      wcGetTreeTotal(client, getInactiveSpec(client), 2),
      wcGetTreeTotal(client, getInactiveSpec(client), 3) );
    AddMenuItem(menu_talents,"dual",line);
  }
  else  {
    Format(line, sizeof(line), "%T", "dual talent spec requires premium", client);
    AddMenuItem(menu_talents,"",line, ITEMDRAW_DISABLED);
  }

  new race = GetClass(client);
  new treetotal;
  for(new i=1; i<10;i++)
  {
    wcGetTalentInfo(race,i,0,0,client,"name",line, client);
    if(!StrEqual(line, "0", false))
    {
      treetotal = wcGetTreeTotal(client, getActiveSpec(client), i);
      Format(line,sizeof(line),"%s (%d)",line, treetotal);
      IntToString(i,sId,sizeof(sId));
      AddMenuItem(menu_talents,sId,line);
    }
    else break;
  }
  Format(line,sizeof(line),"%T","wc resettalents",client);
  AddMenuItem(menu_talents,"reset",line);
  DisplayMenu(menu_talents,client,MENU_TIME_FOREVER);
}

public TalentsMenuHandle(Handle:menu_talents,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:treeid[40];
    GetMenuItem(menu_talents,slot,treeid,sizeof(treeid));

    if(StrEqual(treeid,"reset", false))
    {
      ConfirmResetTalents(client);
    }
    else  if(StrEqual(treeid,"dual",false)) {
      DualTalentTrigger(client);
      TalentsMenu(client);
    }
    else  {
      new tree = StringToInt(treeid);
      SelectTalentsMenu(client, tree);

    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ClassMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_talents);
}

DualTalentTrigger(client) {
  if(IsPlayerAlive(client)) {
    ForcePlayerSuicide(client);
  }
  WCMessage(client, "changed talents spec", client);
  swapSpecs(client);

}

public wcViewOthersTalents(client,target, tree)
{
  new Handle:menu_otherstalents=CreateMenu(wcViewOthersTalentsHandle);
  SetMenuExitBackButton(menu_otherstalents,true);
  decl String:title[128],String:line[128], String:tier[255], String:name[255], String:talentID[10], String:max[255],String:buffer[255];
  new race = GetClass(target);
  wcGetTalentInfo(race, tree, 0, 0, target, "name", name, client);
  new treetotal = wcGetTreeTotal(target, getActiveSpec(target), tree);
  Format(title,sizeof(title),"%s - %d",name, treetotal);
  SetMenuTitle(menu_otherstalents,title);
  for(new p=1; p<100; p++)
  {
    wcGetTalentInfo(race,tree,p,0,target,"tier",tier);
    if (!StrEqual(tier, "0", false))
    {
      Format(line,sizeof(line),"%s",tier);
      IntToString(target,buffer,sizeof(buffer));
      AddMenuItem(menu_otherstalents,buffer, line, ITEMDRAW_DISABLED);
      for(new i=1; i<100; i++)
      {
        wcGetTalentInfo(race,tree,p,i,target,"name",name, client);
        if(!StrEqual(name, "0", false))
        {
         wcGetTalentInfo(race,tree,p,i,target,"max",max);
         new tmax = StringToInt(max);
         Format(line,sizeof(line),"%s -%d/%d",name, wcGetTalentLevel(target,tree,p,i), tmax);
         Format(talentID, sizeof(talentID), "%d %d %d %d", race, tree, p, i);
         AddMenuItem(menu_otherstalents,talentID, line,ITEMDRAW_DISABLED);
        }
        else break;
      }
    }
    else break;
  }
  DisplayMenu(menu_otherstalents,client,MENU_TIME_FOREVER);
}

public wcViewOthersTalentsHandle(Handle:menu_otherstalents,MenuAction:action,client,slot)
{
  decl String:buffer[255];
  GetMenuItem(menu_otherstalents,0,buffer,sizeof(buffer));
  new target=StringToInt(buffer);
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      OtherPlayerMenu(client,target);
  if(action==MenuAction_End)
    CloseHandle(menu_otherstalents);
}

public SelectTalentsMenu(client, tree)
{
  new Handle:menu_selecttalents=CreateMenu(SelectTalentsMenuHandle);
  SetMenuExitBackButton(menu_selecttalents,true);
  decl String:title[128],String:line[128], String:tier[255], String:name[255], String:talentID[10], String:max[255];
  new race = GetClass(client);
  wcGetTalentInfo(race, tree, 0, 0, client, "name", name, client);
  new treetotal = wcGetTreeTotal(client, getActiveSpec(client), tree);
  Format(title,sizeof(title),"%s - %d",name, treetotal);
  SetMenuTitle(menu_selecttalents,title);
  for(new p=1; p<100; p++)
  {
    wcGetTalentInfo(race,tree,p,0,client,"tier",tier);
    if (!StrEqual(tier, "0", false))
    {
      Format(line,sizeof(line),"%s",tier);
      AddMenuItem(menu_selecttalents,line, line, ITEMDRAW_DISABLED);
      for(new i=1; i<100; i++)
      {
        wcGetTalentInfo(race,tree,p,i,client,"name",name, client);
        if(!StrEqual(name, "0", false))
        {
         wcGetTalentInfo(race,tree,p,i,client,"max",max);
         new tmax = StringToInt(max);
         Format(line,sizeof(line),"%s -%d/%d",name, wcGetTalentLevel(client,tree,p,i), tmax);
         Format(talentID, sizeof(talentID), "%d %d %d %d", race, tree, p, i);
         AddMenuItem(menu_selecttalents,talentID, line);
        }
        else break;
      }
    }
    else break;
  }
  DisplayMenu(menu_selecttalents,client,MENU_TIME_FOREVER);
}

public SelectTalentsMenuHandle(Handle:menu_selecttalents,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:talentid[40],String:s_race[10], String:s_tree[10], String:s_tier[10], String: s_talent[10];
    GetMenuItem(menu_selecttalents,slot,talentid,sizeof(talentid));
    new StringID=BreakString(talentid, s_race, sizeof(s_race));
    StringID+=BreakString(talentid[StringID], s_tree, sizeof(s_tree));
    StringID+=BreakString(talentid[StringID], s_tier, sizeof(s_tier));
    BreakString(talentid[StringID], s_talent, sizeof(s_talent));

    new race=StringToInt(s_race);
    new tree=StringToInt(s_tree);
    new tier=StringToInt(s_tier);
    new talent=StringToInt(s_talent);
    TalentDetailMenu(client, race, tree, tier, talent);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      TalentsMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_selecttalents);
}

new g_ff_talentsmenu[65][3];
public TalentDetailMenu(client, race, tree, tier, talent)
{
  g_ff_talentsmenu[client][0]=tree;
  g_ff_talentsmenu[client][1]=tier;
  g_ff_talentsmenu[client][2]=talent;
  new Handle:menu_talentsdetail=CreateMenu(TalentsDetailMenuHandle);
  SetMenuExitBackButton(menu_talentsdetail,true);
  decl String:title[255], String:line[255], String:name[255], String:detail[255], String:ID[255];
  wcGetTalentInfo(race,tree,tier,talent,client,"name",name, client);
  wcGetTalentInfo(race,tree,tier,talent,client,"description",detail, client);
  ReplaceString(detail,sizeof(detail),"\\n","\n");
  wcGetTalentInfo(race,tree,tier,talent,client,"max",title);
  new tpoints=wcGetTreeTotal(client, getActiveSpec(client),tree);
  new max = StringToInt(title);
  wcGetTalentInfo(race,tree,tier,talent,client,"talentid",title);
  new id = StringToInt(title);
  Format(title,sizeof(title),"%s - %d/%d",name, wcGetTalentLevel(client, tree, tier, talent), max);
  SetMenuTitle(menu_talentsdetail,title);
  Format(line,sizeof(line),"%s",detail);
  AddMenuItem(menu_talentsdetail,line,line,ITEMDRAW_DISABLED);
  if(id>0)
  {
    Format(line,sizeof(line),"%T","Talent Id",client, id);
    AddMenuItem(menu_talentsdetail,line,line,ITEMDRAW_DISABLED);
    new mana = GetTalentCost(client,tree,tier,talent);
    Format(line,sizeof(line),"%T","Talent Mana Cost",client, mana);
    AddMenuItem(menu_talentsdetail,line,line,ITEMDRAW_DISABLED);
    wcGetTalentInfo(race,tree,tier,talent,client,"cooldown",line);
    new Float:cooldown = StringToFloat(line);
    Format(line,sizeof(line),"%T","Talent Cooldown",client, cooldown);
    AddMenuItem(menu_talentsdetail,line,line,ITEMDRAW_DISABLED);
    wcGetTalentInfo(race,tree,tier,talent,client,"base",line);
    new effect = StringToInt(line);
    wcGetTalentInfo(race,tree,tier,talent,client,"multi",line);
    effect += RoundToNearest(StringToFloat(line)*GetSpellDmg(client,1.0));
    if(effect > 0)
    {

      Format(line,sizeof(line),"%T", "Talent Effect",client, effect);
      AddMenuItem(menu_talentsdetail,line,line,ITEMDRAW_DISABLED);
    }
  }
  if(tier >= 4 && !HasExpansion(client, Expansion:E_CATA)) {
    Format(ID, sizeof(ID),"%d %d %d", tree, tier, talent);
    Format(line, sizeof(line), "%T", "talent require cata", client);
    AddMenuItem(menu_talentsdetail,ID, line, ITEMDRAW_DISABLED);
  }
  else if (tpoints <5*(tier-1))
  {
    new mpoints = (5*(tier-1))-tpoints;
    Format(ID, sizeof(ID),"%d %d %d", tree, tier, talent);
    Format(line, sizeof(line), "%T", "need more talents in tree", client, mpoints);
    AddMenuItem(menu_talentsdetail,ID,line, ITEMDRAW_DISABLED);
  }
  else if(tier >= 4 && !HasExpansion(client, Expansion:E_CATA)) {

  }
  else
  {
    Format(line,sizeof(line),"%T", "buy talent", client);
    Format(ID, sizeof(ID),"%d %d %d", tree, tier, talent);
    AddMenuItem(menu_talentsdetail,ID,line);
  }
  DisplayMenu(menu_talentsdetail,client,MENU_TIME_FOREVER);
}

public TalentsDetailMenuHandle(Handle:menu_talentsdetail,MenuAction:action,client,slot)
{

  decl String:buffer[255];
  if(action==MenuAction_Select)
  {
    new tree=g_ff_talentsmenu[client][0];
    new tier=g_ff_talentsmenu[client][1];
    new talent=g_ff_talentsmenu[client][2];
    new race = GetClass(client);
    new points = getTPoints(client, getActiveSpec(client));
    wcGetTalentInfo(race,tree,tier,talent,client,"max",buffer);
    new max = StringToInt(buffer);
    if(points > 0 && max > wcGetTalentLevel(client, tree, tier, talent))
    {
    new tpoints=wcGetTreeTotal(client, getActiveSpec(client), tree);
    if(tier >= 4 && !HasExpansion(client, Expansion:E_CATA))  {
      WCMessage(client, "you can't pick this talent", client);
      SelectTalentsMenu(client, tree);
    }
    if (tpoints >= 5*(tier-1))  {
      points-=1;
      wcAddTalent(tree,tier,talent,client);
      setTPoints(client, getActiveSpec(client), points);
      SelectTalentsMenu(client, tree);
    }
    else  {
      WCMessage(client, "you can't pick this talent", client);
      SelectTalentsMenu(client, tree);
    }
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      SelectTalentsMenu(client,g_ff_talentsmenu[client][0]);
  if(action==MenuAction_End)
    CloseHandle(menu_talentsdetail);
}

public ConfirmResetTalents(client)
{
  new Handle:menu_treset=CreateMenu(ResetTalentsHandle);
  SetMenuExitBackButton(menu_treset,true);
  decl String:title[255],String:line[255];
  new cost=0;
  for(new i=0;i<10;i++)
  {
    cost += wcGetTreeTotal(client, getActiveSpec(client), i);
  }
  cost *= 100;
  Format(title,sizeof(title),"%T","wc reset confirm",client);
  SetMenuTitle(menu_treset,title);
  Format(line,sizeof(line),"%T","reset confirm",client,cost);
  if(cost<=GetMoney(client))
   AddMenuItem(menu_treset,"confirm",line);
  else
   AddMenuItem(menu_treset,"confirm",line,ITEMDRAW_DISABLED);
  DisplayMenu(menu_treset,client,MENU_TIME_FOREVER);
}



public ResetTalentsHandle(Handle:menu_treset,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    new cost=0;
    for(new i=0;i<10;i++)
    {
      cost += wcGetTreeTotal(client, getActiveSpec(client), i);
    }
    cost *= 100;
    if(cost<=GetMoney(client))
    {
      wcSetMoney(client,GetMoney(client)-cost);
      wcResetTalents(client, getActiveSpec(client));
      WCMessage(client,"reset confirmed talents",client,cost,getTPoints(client, getActiveSpec(client)));
      TalentsMenu(client);
    }
    else
      WCMessage(client,"no money",client);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      TalentsMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_treset);
}

public ProfessionsMenu(client)
{
   decl String:title[255];
   new Handle:menu_prof=CreateMenu(MenuProfessionsHandle);
   SetMenuExitBackButton(menu_prof,true);
   Format(title,sizeof(title),"%T","wc professions",client);
   SetMenuTitle(menu_prof,title);
   new prof1=GetProfession(client,1);
   new prof2=GetProfession(client,2);
   decl String:Sprof1[60],String:Sprof2[60];
   ProfToName(prof1,Sprof1,sizeof(Sprof1),client);
   ProfToName(prof2,Sprof2,sizeof(Sprof2),client);
   if(isCraftingProfession(prof1))  {
     decl String:profName[30];
     GetClientProfessionLevelString(client, 1, profName, sizeof(profName));
     Format(Sprof1,sizeof(Sprof1),"%s (%s)",Sprof1,profName);
   }
   else {
     Format(Sprof1,sizeof(Sprof1),"%s (%i)",Sprof1,GetProfessionSk(client,1));
   }
   if(isCraftingProfession(prof2))  {
     decl String:profName[30];
     GetClientProfessionLevelString(client, 2, profName, sizeof(profName));
     Format(Sprof2,sizeof(Sprof2),"%s (%s)",Sprof2,profName);
   }
   else {
     Format(Sprof2,sizeof(Sprof2),"%s (%i)",Sprof2,GetProfessionSk(client,2));
   }
   if(prof1<=0)
    AddMenuItem(menu_prof,"0",Sprof1,ITEMDRAW_DISABLED);
   else
    AddMenuItem(menu_prof,"0",Sprof1);
   if(prof2<=0)
    AddMenuItem(menu_prof,"0",Sprof2,ITEMDRAW_DISABLED);
   else
    AddMenuItem(menu_prof,"0",Sprof2);
   decl String:buffer[255];
   Format(buffer,sizeof(buffer),"%T","change prof1",client);
   AddMenuItem(menu_prof,"0",buffer);
   Format(buffer,sizeof(buffer),"%T","change prof2",client);
   AddMenuItem(menu_prof,"0",buffer);
   Format(buffer, sizeof(buffer), "%T", "Bonuses of Professions you have", client);
   AddMenuItem(menu_prof,"0", buffer);
   Format(buffer, sizeof(buffer), "%T", "Bonuses of Professions you can get", client);
   AddMenuItem(menu_prof,"0", buffer);
   DisplayMenu(menu_prof,client,MENU_TIME_FOREVER);
   if((g_ffSlideHook[client] == SLIDE_HOOK_PROFS) && (prof1 > 0) && (prof2 > 0)) {
     SlideHooked(client);
   }

}

public MenuProfessionsHandle(Handle:menu_prof,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    switch(slot)
    {
      case 0:
        ProfessionMenuSort(client,1);
      case 1:
        ProfessionMenuSort(client,2);
      case 2: {
        new profId = GetProfession(client, 1);
        if(profId > 0)
          AskDeleteConfirmation(client, "ChangeProfessionMenu", 1,
            "You are about to override your profession with another one!");
        else
          ChangeProfessionMenu(client,1);
      }
      case 3: {
        new profId = GetProfession(client, 2);
        if(profId > 0)
          AskDeleteConfirmation(client, "ChangeProfessionMenu", 2,
            "You are about to override your profession with another one!");
        else
          ChangeProfessionMenu(client,2);
      }
      case 4:
        ProfessionBonusesMenu(client);
      case 5:
        ProfessionBonusesMenu(client, true);
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ClassMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_prof);
}

public ChangeProfessionMenu(client,which)
{
  decl String:title[255];
  new Handle:menu_prof=CreateMenu(MenuProfessionChangeHandle);
  SetMenuExitBackButton(menu_prof,true);
  decl String:Sprof[60];
  new prof=GetProfession(client,which);
  ProfToName(prof,Sprof,sizeof(Sprof),client);
  Format(title,sizeof(title),"%T","profession change to",client,Sprof);
  SetMenuTitle(menu_prof,title);
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"%T","prof warning",client);
  AddMenuItem(menu_prof,"0",buffer,ITEMDRAW_DISABLED);
  for (new x=1;x<=10;x++)
  {
    decl String:Sx[20];
    Format(Sx,sizeof(Sx),"%i %i",which,x);
    ProfToName(x,buffer,sizeof(buffer),client);
    if((x == 9 || x == 10) && !HasExpansion(client, Expansion:E_CATA)) {
      Format(buffer, sizeof(buffer), "%s (%T)", buffer, "cata expansion required", client);
      AddMenuItem(menu_prof, Sx, buffer, ITEMDRAW_DISABLED);
    }
    else
      AddMenuItem(menu_prof,Sx,buffer);
  }
  DisplayMenu(menu_prof,client,MENU_TIME_FOREVER);
}

public MenuProfessionChangeHandle(Handle:menu_prof,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[20];
    decl String:Swhich[10],String:Sid[10];
    GetMenuItem(menu_prof,slot,buffer,sizeof(buffer));
    new StringID=BreakString(buffer, Swhich, sizeof(Swhich));
    BreakString(buffer[StringID], Sid, sizeof(Sid));
    new which=StringToInt(Swhich);
    new id=StringToInt(Sid);
    if(id<=0 || id>10 || which>2 || which <1)
    {
      ProfessionsMenu(client);
    }
    else
    {
      for(new x=1;x<=200;x++)
      {
        SetClientProfessionSlot(client,which,x,0);
      }
      wcSetProfession(client,which,id);
      wcSetProfessionSk(client,which,0);
      decl String:profname[60];
      ProfToName(id,profname,sizeof(profname),client);
      WCMessage(client,"new prof",client,profname);
      if(g_ffSlideHook[client] == SLIDE_HOOK_PROFS) {
        SlideHooked(client);
      }
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ProfessionsMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_prof);
}


ProfessionMenuSort(client,which)
{
  if(!isCraftingProfession(GetProfession(client,which)))  {
    ProfessionMenu(client, which, 0, 9999);
    return;
  }
  decl String:title[255];
  new Handle:menu_prof=CreateMenu(MenuProfessionMenuSort);
  lastProf[client][2]=which;
  SetMenuExitBackButton(menu_prof,true);
  decl String:Sprof[60];
  new prof=GetProfession(client,which);
  ProfToName(prof,Sprof,sizeof(Sprof),client);
  Format(title,sizeof(title),"%T","profession select sort",client,Sprof);
  SetMenuTitle(menu_prof,title);
  decl String:line[255];
  new level = GetClientProfessionLevel(client, which);
  new bool:max = IsMaximumProfessionLevel(GetProfession(client, which), level);
  if(max)  {
    Format(line, sizeof(line), "%T", "maximum level", client);
    AddMenuItem(menu_prof, "levelup", line, ITEMDRAW_DISABLED);
  }
  else  {
    ProfessionLevelToString(level+1, line, sizeof(line), client);
    Format(line, sizeof(line), "%T", "level up profession", client, line);
    AddMenuItem(menu_prof, "levelup", line);
  }
  Format(line, sizeof(line), "%T", "All", client);
  AddMenuItem(menu_prof,"0 9999",line);
  for(new i=0; i<MAX_PROF_LEVELS;i++) {
    if(IsValidProfessionLevel(prof, i)) {
      decl String:temp[30];
      Format(temp, sizeof(temp), "%d %d", GetProfessionLevelLowerBound(prof, i), GetProfessionLevelHigherBound(prof,i));
      ProfessionLevelToString(i, line, sizeof(line), client);
      Format(line, sizeof(line), "%T", "show recipes of level", client, line);
      AddMenuItem(menu_prof, temp, line);
    }
  }
  DisplayMenu(menu_prof,client,MENU_TIME_FOREVER);
}

public MenuProfessionMenuSort(Handle:menu_prof,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[20];
    decl String:Smin[10],String:Smax[10];
    GetMenuItem(menu_prof,slot,buffer,sizeof(buffer));
    if(StrEqual(buffer, "levelup")) {
      ProfessionLevelUpMenu(client, lastProf[client][2]);
    }
    else  {
      new StringID=BreakString(buffer, Smin, sizeof(Smin));
      BreakString(buffer[StringID], Smax, sizeof(Smax));
      new min=StringToInt(Smin);
      new max=StringToInt(Smax);
      if(min== -1)  {
        ProfessionBonusesMenu(client);
      }
      else
        ProfessionMenu(client, lastProf[client][2], min, max);
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ProfessionsMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_prof);
}


ProfessionLevelUpMenu(client, which) {
  lastProf[client][2] = which;

  new level = GetClientProfessionLevel(client, which);

  if(IsMaximumProfessionLevel(GetProfession(client, which), level)) {
    LogError("[WC] Error logic m.1480");
    return;
  }

  new Handle:menu=CreateMenu(ProfessionLevelUpMenuHandle); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  decl String:profName[50], String:profLevel[50];
  ProfToName(GetProfession(client, which), profName, sizeof(profName), client);
  GetClientProfessionLevelString(client, which, profLevel, sizeof(profLevel));

  Format(title,sizeof(title),"%T","wc prof full title",client, profName, profLevel);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[200];

  ProfessionLevelToString(level+1, profLevel, sizeof(profLevel), client);
  Format(buffer, sizeof(buffer), "%T", "level up profession to", client, profLevel);
  AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);

  new reagents[5],reagents2[5];
  new amount = GetProfessionLevelRequirements(GetProfession(client, which), level, reagents, reagents2);

  new bool:canLevelUp=true;
  new silverNeeded=GetProfessionLevelRequiredSilver(GetProfession(client,which),level);
  new silverHas = GetMoney(client);
  Format(buffer, sizeof(buffer), "%T", "money reqhave", client, silverNeeded, silverHas);
  AddMenuItem(menu, "meta-data", buffer, ITEMDRAW_DISABLED);
  if(silverNeeded > silverHas)  canLevelUp=false;

  for(new i=0; i<amount; i++) {
    new item[UniqueItem];
    GetItemProperties(item, reagents[i]);
    new has = GetAmountOfItems(client, reagents[i]);
    Format(buffer,sizeof(buffer),"%T","reagent reqhave",client,item[Item_Name],reagents2[i],has);
    if(has < reagents2[i])  {
      canLevelUp=false;
    }
    AddMenuItem(menu, "meta-data", buffer, ITEMDRAW_DISABLED);
  }
  if(canLevelUp) {
    //Confirm leveling up
    Format(buffer, sizeof(buffer), "%T", "press confirm",  client);
    AddMenuItem(menu, "confirm", buffer);
  }
  else  {
    Format(buffer, sizeof(buffer), "%T", "not enough reagents", client);
    AddMenuItem(menu, "no", buffer, ITEMDRAW_DISABLED);

  }

  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public ProfessionLevelUpMenuHandle(Handle:menu, //rename
                        MenuAction:action,
                        client, slot) {

  if(action == MenuAction_Select) {
    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));
    if(StrEqual(buffer, "confirm")) {
      new which = lastProf[client][2];
      new level = GetClientProfessionLevel(client, which);
      new reagents[5],reagents2[5];
      new amount = GetProfessionLevelRequirements(GetProfession(client, which), level, reagents, reagents2);
      new silver = GetProfessionLevelRequiredSilver(GetProfession(client,which), level);

      wcSetMoney(client, GetMoney(client)-silver);

      for(new i=0; i<amount; i++) {
        if(!RemoveItem(client,reagents[i],reagents2[i]))
        LogError("[WC] Error removing item!! m.1534 ");
      }

      IncreaseProfessionLevel(client, which, level+1);
      ProfessionMenuSort(client, which);


    }

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      ProfessionMenuSort(client, lastProf[client][2]);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}



stock ProfessionBonusesMenu(client, bool:all=false)
{
   decl String:title[255];
   new Handle:menu_prof=CreateMenu(MenuProfessionHandle);
   SetMenuExitBackButton(menu_prof,true);

   if(!all)
    Format(title,sizeof(title),"%T","wc profession bonuses",client);
   else
    Format(title,sizeof(title),"%T","wc profession bonuses all",client);
   SetMenuTitle(menu_prof,title);
   new Profession:profs[PROFESSION_BONUSES_AMOUNT];
   new total=PROFESSION_BONUSES_AMOUNT;
   if(!all)
    total=GetAllProfBonuses(client, profs);
   else
    GetProfBonuses(profs);
   for(new i=0;i<total;i++) {
    decl String:temp[255];
    TranslateProfBonus(client, profs[i], temp, sizeof(temp));
    if(all) { //Add extra info
      decl String:profName[255], profSkill;
      new profId = GetProfRequirements(profs[i], profSkill);
      ProfToName(profId,profName,sizeof(profName),client);
      if(!isCraftingProfession(profId)) {
        Format(temp, sizeof(temp), "%s (%s - %d)", temp, profName, profSkill);
      }
      else  {
        decl String:profLevel[30];
        new level = professionSkillToLevel(profId, profSkill);
        ProfessionLevelToString(level, profLevel, sizeof(profLevel), client);
        Format(temp, sizeof(temp), "%s (%s - %s)", temp, profName, profLevel);
      }
    }
    AddMenuItem(menu_prof,"",temp,ITEMDRAW_DISABLED);
   }
   if(total<=0)
   {
    Format(title,sizeof(title),"%T","no bonuses",client);
    AddMenuItem(menu_prof,"0",title,ITEMDRAW_DISABLED);
   }
   DisplayMenu(menu_prof,client,MENU_TIME_FOREVER);
}

ProfessionMenu(client,which, iMin=0, iMax=9999)
{
   decl String:title[255];
   lastProf[client][0]=iMin;
   lastProf[client][1]=iMax;
   new Handle:menu_prof=CreateMenu(MenuProfessionHandle);
   SetMenuExitBackButton(menu_prof,true);
   decl String:Sprof[60];
   new skill=GetProfessionSk(client,which);
   new prof=GetProfession(client,which);
   ProfToName(prof,Sprof,sizeof(Sprof),client);
   if(!isCraftingProfession(prof))  {
     Format(title,sizeof(title),"%T","wc profession",client,Sprof,skill);
   }
   else {
     decl String:name[30];
     GetClientProfessionLevelString(client, which,name, sizeof(name));
     Format(title, sizeof(title), "%T", "wc profession 2", client, Sprof, name);
   }
   SetMenuTitle(menu_prof,title);
   new total;
   decl String:Sid[10];
   IntToString(which,Sid,sizeof(Sid));

   decl hRecipe[Recipe];
   hRecipe[Recipe_skill]=1;
   new Handle:array = CreateArray(sizeof(hRecipe));

   for(new x=1;x<=400;x++)
   {
    new recipe=GetClientProfessionSlot(client,which,x);
    if(recipe>0)
    {
      decl String:info[20];
      //decl String:Sname[255];
      decl item[UniqueItem];
      GetItemProperties(item, recipe);


      Format(info,sizeof(info),"%i %i",which,x);
      //AddMenuItem(menu_prof,info,Sname);

      hRecipe[Recipe_skill]=GetRequiredSkill(recipe);
      strcopy(hRecipe[Recipe_name], 50, item[Item_Name]);
      strcopy(hRecipe[Recipe_info], 20, info);
      if( (hRecipe[Recipe_skill] >= iMin) && (hRecipe[Recipe_skill] <= iMax))  {
        PushArrayArray(array, hRecipe[0]);
        total++;
      }
    }
    else
      break;
   }

   SortADTArrayCustom(array, SortFunction);

   for(new i=0; i < GetArraySize(array); i++) {
      GetArrayArray(array, i, hRecipe[0]);
      AddMenuItem(menu_prof,hRecipe[Recipe_info],hRecipe[Recipe_name]);
   }

   CloseHandle(array);
   new bool:gath=false;
   switch(prof)   {
      case 5: gath=true;
      case 6: gath=true;
      case 7: gath=true;
      case 10: gath=true;
   }
   if(gath) {
      //start of possible drops
      KvRewind(wcdrop);
      if (!KvGotoFirstSubKey(wcdrop))
      {
         LogError("[RPGx] Error: failed to open drop database");
      }
      do
      {
         new prof2=KvGetNum(wcdrop,"profession");
         if(prof2!=prof)
            continue;
         new minskill=KvGetNum(wcdrop,"minskill",0);
         if(minskill>skill)
            continue;
         new skillup=KvGetNum(wcdrop,"skillup",0);
         new itemid=KvGetNum(wcdrop,"id",0);
         if(itemid<=0)
            continue;
         decl String:itemName[255];
         GetItemName(itemid, itemName);
         Format(itemName,sizeof(itemName),"%s (%i-%i Skill)", itemName, minskill, skillup);
         AddMenuItem(menu_prof,"",itemName,ITEMDRAW_DISABLED);
         total++;
      } while(KvGotoNextKey(wcdrop));

      //end of possible drops list
   }
   if(total==0)
   {
    Format(title,sizeof(title),"%T","no recipes",client);
    AddMenuItem(menu_prof,"0",title,ITEMDRAW_DISABLED);
   }
   DisplayMenu(menu_prof,client,MENU_TIME_FOREVER);
}

// [Small, ...., Large]
public SortFunction(index1, index2, Handle:array, Handle:hndl)  {
  new val1 = GetArrayCell(array, index1);
  new val2 = GetArrayCell(array, index2);
  if(val1 > val2)
      return 1;
  if(val1 == val2)
      return 0;
  return -1;
}

public MenuProfessionHandle(Handle:menu_prof,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[20],String:Swhich[10],String:Sid[10];
    GetMenuItem(menu_prof,slot,buffer,sizeof(buffer));
    new StringID=BreakString(buffer, Swhich, sizeof(Swhich));
    BreakString(buffer[StringID], Sid, sizeof(Sid));
    new which=StringToInt(Swhich);
    new id=StringToInt(Sid);
    if(which<=0)
    {
      ProfessionsMenu(client);
    }
    else
    {
      ProfRecipeMenu(client,id,which);
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ProfessionsMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_prof);
}

ProfRecipeMenu(client,slotid,which)
{
   lastMenu2[client]=slotid;
   lastMenu3[client]=which;
   new id=GetClientProfessionSlot(client,which,slotid);
   KvRewind(items);
   decl String:Sid[20];
   IntToString(id,Sid,sizeof(Sid));
   KvJumpToKey(items,Sid);
   //title
   decl String:title[255];
   new Handle:menu_prof=CreateMenu(MenuProfRecipeHandle);
   SetMenuExitBackButton(menu_prof,true);
   Format(title,sizeof(title),"%T","wc make item",client);
   SetMenuTitle(menu_prof,title);
   //end of title
   new skill=GetProfessionSk(client,which);
   new createid=KvGetNum(items,"itemid");
   new amount=KvGetNum(items,"reagents");
   new orange=KvGetNum(items,"orange");
   new green=KvGetNum(items,"green");
   new grey=KvGetNum(items,"grey");
   new reagents[amount+1],nreagents[amount+1],hreagents[amount+1],String:Sreagents[amount+2][255];
   for(new x=1;x<=amount;x++)
   {
      decl String:buffer[60];
      Format(buffer,sizeof(buffer),"reagent_%i",x);
      reagents[x]=KvGetNum(items,buffer);
      Format(buffer,sizeof(buffer),"%s_amount",buffer);
      nreagents[x]=KvGetNum(items,buffer);
   }
   new bool:pass=true;
   for(new x=1;x<=amount;x++)
   {
    KvRewind(items);
    IntToString(reagents[x],Sid,sizeof(Sid));
    KvJumpToKey(items,Sid);
    KvGetString(items,"name",Sreagents[x],255);
    hreagents[x]=GetAmountOfItems(client,reagents[x]);
    decl String:buffer[150];
    if(hreagents[x]<nreagents[x])
      pass=false;
    Format(buffer,sizeof(buffer),"%T","reagent reqhave",client,Sreagents[x],nreagents[x],hreagents[x]);
    AddMenuItem(menu_prof,"0",buffer,ITEMDRAW_DISABLED);
   }
   new chance;
   if(skill>=grey)
    chance=0;
   else if (skill>=green)
    chance=30;
   else if (skill>=orange)
    chance=70;
   else
    chance=100;
   decl String:buffer[255];
   Format(buffer,sizeof(buffer),"%T","skillup chance",client,chance);
   InsertMenuItem(menu_prof,0,"0",buffer,ITEMDRAW_DISABLED);
   decl String:Sitem[255];
   GetItemName(createid,Sitem);
   Format(buffer,sizeof(buffer),"%T","create item",client,Sitem);
   Format(Sid,sizeof(Sid),"\"%i\" \"info\"",id);
   InsertMenuItem(menu_prof,0,Sid,buffer);
   Format(buffer,sizeof(buffer),"%T","press confirm",client);
   Format(Sid,sizeof(Sid),"\"%i\" \"%i\"",which,id);
   if(pass==true)
    AddMenuItem(menu_prof,Sid,buffer);
   else
    AddMenuItem(menu_prof,Sid,buffer,ITEMDRAW_DISABLED);
   DisplayMenu(menu_prof,client,MENU_TIME_FOREVER);
}

public MenuProfRecipeHandle(Handle:menu_prof,MenuAction:action,client,slot)
{
  new String:buffer[128],String:Swhich[10];
  new slot2=GetMenuItemCount(menu_prof)-1;
  GetMenuItem(menu_prof,slot2,buffer,sizeof(buffer));
  BreakString(buffer, Swhich, sizeof(Swhich));
  new which2=StringToInt(Swhich);
  if(action==MenuAction_Select)
  {
    new String:Sid[10];
    GetMenuItem(menu_prof,slot,buffer,sizeof(buffer));
    new StringID=BreakString(buffer, Swhich, sizeof(Swhich));
    BreakString(buffer[StringID], Sid, sizeof(Sid));
    new which=StringToInt(Swhich);
    new id=StringToInt(Sid);
    if(StrEqual(Sid,"info"))
    {
      KvRewind(items);
      IntToString(which,Sid,sizeof(Sid));
      KvJumpToKey(items,Sid);
      new createid=KvGetNum(items,"itemid");
      lastMenu[client]=2;
      new uitem[UniqueItem];
      GetItemProperties(uitem, createid);
      ItemInfMenu(client,uitem,false);
    }
    else if((id>0) && (which>0))
    {
      KvRewind(items);
      IntToString(id,Sid,sizeof(Sid));
      KvJumpToKey(items,Sid);
      new skill=GetProfessionSk(client,which);
      new createid=KvGetNum(items,"itemid");
      new amount=KvGetNum(items,"reagents");
      new orange=KvGetNum(items,"orange");
      new green=KvGetNum(items,"green");
      new grey=KvGetNum(items,"grey");
      new soulbound=KvGetNum(items,"soulbound", 0);
      new reagents[amount+1],nreagents[amount+1],hreagents[amount+1];
      for(new x=1;x<=amount;x++)
      {
        Format(buffer,sizeof(buffer),"reagent_%i",x);
        reagents[x]=KvGetNum(items,buffer);
        Format(buffer,sizeof(buffer),"%s_amount",buffer);
        nreagents[x]=KvGetNum(items,buffer);
      }
      new bool:pass=true;
      new freeslot=wcCheckFreeSlot2(client,createid);
      if(freeslot==0)
        pass=false;
      if(pass==true)  for(new x=1;x<=amount;x++)
      {
        hreagents[x]=GetAmountOfItems(client,reagents[x]);
        if(hreagents[x]<nreagents[x])
          pass=false;
      }
      new chance;
      if(skill>=grey)
        chance=0;
      else if (skill>=green)
        chance=30;
      else if (skill>=orange)
        chance=70;
      else
        chance=100;
      if(pass==false)
      WCMessage(client,"no free/no reagents",client);
      else
      {
      for(new x=1;x<=amount;x++)
      {
        if(!RemoveItem(client,reagents[x],nreagents[x]))
          LogError("[WC] Error removing item!!");
      }
      new uitem[UniqueItem];
      GetItemProperties(uitem, createid,0);
      new uslot;
      if(!(uslot=GiveItem(client, uitem, 1, "profession")))
        LogError("[WC] Error m.1481: item id = %d, char = %d", uitem[Item_defId], characterids[client][0]);

      soulbound*=-1;
      if(soulbound!=0)
        wcSetStaticSlotAmount(client,uslot,soulbound);
      if(chance>=GetRandomInt(1,100))
      {
        /*wcSetProfessionSk(client,which,GetProfessionSk(client,which)+2);*/
        /*InitProfBonuses(client);*/
        /*new profid=GetProfession(client,which);*/
        /*new String:Sprof[60];*/
        /*ProfToName(profid,Sprof,sizeof(Sprof),client);*/
        /*WCMessage(client,"increased prof skill2",client,Sprof, 2);*/
        /*[>if(GetRandomInt(0,1)==0) {<]*/
          /*[>wcSetProfessionSk(client,which,GetProfessionSk(client,which)+1);<]*/
          /*[>WCMessage(client,"increased prof skill",client,Sprof);<]*/
        /*[>}<]*/
      }
      decl String:itemname[255];
      GetItemName(createid,itemname);
      WCMessage(client,"created item",client,itemname);
      ProfessionMenu(client,which,lastProf[client][0],lastProf[client][1]);
      }
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
    {
      ProfessionMenu(client,which2,lastProf[client][0],lastProf[client][1]);
    }
  if(action==MenuAction_End)
    CloseHandle(menu_prof);
}

public AuctionsMenu(client)
{
   new Handle:menu_category=CreateMenu(AuctionsHandle);
   SetMenuExitBackButton(menu_category,true);
   decl String:buffer[255];
   Format(buffer, sizeof(buffer), "%T", "select category", client);
   SetMenuTitle(menu_category,buffer);
   Format(buffer,sizeof(buffer),"%T","create auction",client);
   AddMenuItem(menu_category,"-10",buffer);
   Format(buffer,sizeof(buffer),"%T","all",client);
   AddMenuItem(menu_category,"0",buffer);
   Format(buffer,sizeof(buffer),"%T","all silver items",client);
   AddMenuItem(menu_category,"-10", buffer);
   Format(buffer,sizeof(buffer),"%T","all gold items",client);
   AddMenuItem(menu_category,"-11", buffer);

   Format(buffer,sizeof(buffer),"%T","green item",client);
   AddMenuItem(menu_category,"-1",buffer);
   Format(buffer,sizeof(buffer),"%T","blue item",client);
   AddMenuItem(menu_category,"-2",buffer);
   Format(buffer,sizeof(buffer),"%T","epic item",client);
   AddMenuItem(menu_category,"-3",buffer);
   Format(buffer,sizeof(buffer),"%T","legendary item",client);
   AddMenuItem(menu_category,"-4",buffer);
   for(new i=1;i<=9;i++)   {
      decl String:sClass[50];
      Format(sClass,sizeof(sClass),"class_%i",i);
      Format(sClass,sizeof(sClass),"%T",sClass,client);
      decl String:sCat[10];
      Format(sCat,sizeof(sCat),"%i",i+100);
      AddMenuItem(menu_category,sCat,sClass);
   }

   Format(buffer,sizeof(buffer),"%T","weapons",client);
   AddMenuItem(menu_category,"22",buffer);
   Format(buffer,sizeof(buffer),"%T","shields",client);
   AddMenuItem(menu_category,"18",buffer);
   Format(buffer,sizeof(buffer),"%T","chests",client);
   AddMenuItem(menu_category,"11",buffer);
   Format(buffer,sizeof(buffer),"%T","backs",client);
   AddMenuItem(menu_category,"10",buffer);
   Format(buffer,sizeof(buffer),"%T","feet",client);
   AddMenuItem(menu_category,"12",buffer);
   Format(buffer,sizeof(buffer),"%T","hands",client);
   AddMenuItem(menu_category,"13",buffer);
   Format(buffer,sizeof(buffer),"%T","helmets",client);
   AddMenuItem(menu_category,"14",buffer);
   Format(buffer,sizeof(buffer),"%T","legs",client);
   AddMenuItem(menu_category,"15",buffer);
   Format(buffer,sizeof(buffer),"%T","necks",client);
   AddMenuItem(menu_category,"16",buffer);
   Format(buffer,sizeof(buffer),"%T","rings",client);
   AddMenuItem(menu_category,"17",buffer);
   Format(buffer,sizeof(buffer),"%T","shoulders",client);
   AddMenuItem(menu_category,"19",buffer);
   Format(buffer,sizeof(buffer),"%T","trinkets",client);
   AddMenuItem(menu_category,"20",buffer);
   Format(buffer,sizeof(buffer),"%T","waists",client);
   AddMenuItem(menu_category,"21",buffer);
   Format(buffer,sizeof(buffer),"%T","wrists",client);
   AddMenuItem(menu_category,"23",buffer);
   Format(buffer,sizeof(buffer),"%T","reagents",client);
   AddMenuItem(menu_category,"8",buffer);
   Format(buffer,sizeof(buffer),"%T","patterns for blacksmith",client);
   AddMenuItem(menu_category,"1",buffer);
   Format(buffer,sizeof(buffer),"%T","schemas for jewelcrafting",client);
   AddMenuItem(menu_category,"2",buffer);
   Format(buffer,sizeof(buffer),"%T","recipes for tailoring",client);
   AddMenuItem(menu_category,"3",buffer);
   Format(buffer,sizeof(buffer),"%T","patterns for leatherworking",client);
   AddMenuItem(menu_category,"4",buffer);
   Format(buffer,sizeof(buffer),"%T","formulas for enchanting",client);
   AddMenuItem(menu_category,"5",buffer);
   Format(buffer,sizeof(buffer),"%T","inscription books",client);
   AddMenuItem(menu_category,"9",buffer);
   Format(buffer,sizeof(buffer),"%T","scroll of enchantment",client);
   AddMenuItem(menu_category,"25",buffer);
   DisplayMenu(menu_category,client,MENU_TIME_FOREVER);
}

public AuctionsHandle(Handle:menu_category,MenuAction:action,client,slot)
{
   if(action==MenuAction_Select)
   {
    decl String:category[255];
    GetMenuItem(menu_category,slot,category,sizeof(category));
    new cat=StringToInt(category);
    if(slot==0)
    {
      RefreshAuctionData(client);
      MenuSendAuction(client);
    }
    else
      PrepareAuctionMenu(client,cat);
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack)
         ShopsMenu(client);
   if(action==MenuAction_End)
      CloseHandle(menu_category);
}



PrepareAuctionMenu(client,filter)
{
  SetMenuOff(client, 15.0, "preparing auctions page");
  GetAuctionsThread(client, filter);
  lastMenu3[client]=filter;
}

AuctionMenu(client, id[300],item[300],uni[300],bidid[300],timeleft[300],bid[300],buyout[300],iamount[300],
	          gold[300], goldBuyout[300], amount)	{
   uni[0]=uni[1];  //Suppress 'not used' symbol
   new filter = lastMenu3[client];
   SetMenuOff(client, 0.0, "auctions are loaded");
   decl String:title[255];
   new Handle:menu_auction=CreateMenu(MenuAuctionHandle);
   SetMenuExitBackButton(menu_auction,true);
   Format(title,sizeof(title),"%T","wc auction",client);
   SetMenuTitle(menu_auction,title);
   decl String:buffer[255];
   if(amount==0)
   {
    Format(buffer,sizeof(buffer),"%T","no items",client);
    AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
   }
   new String:Sid[20];
   for(new x=1;x<=amount;x++)
   {
    new uitem[UniqueItem];
    GetItemProperties(uitem, item[x], uni[x]);
    Format(Sid,sizeof(Sid),"%i %i",id[x],filter);
    if(gold[x]>0 || goldBuyout[x] > 0 )	{
      decl String:sGold[20],String:sGoldBuy[20];
      IntToStringfloat(gold[x], sGold, 20);
      IntToStringfloat(goldBuyout[x], sGoldBuy, 20);
      Format(buffer,sizeof(buffer),"%T","auction index gold",client,uitem[Item_Name],sGold,sGoldBuy,uitem[Item_level],timeleft[x],iamount[x]
           );
    }
    else
      Format(buffer,sizeof(buffer),"%T","auction index",client,uitem[Item_Name],bid[x],buyout[x],uitem[Item_level],timeleft[x],iamount[x]);
    if(bidid[x]==characterids[client][0])
      Format(buffer,sizeof(buffer),"%s %T",buffer,"your bid",client);
    AddMenuItem(menu_auction,Sid,buffer);
   }
   DisplayMenu(menu_auction,client,MENU_TIME_FOREVER);
}

public MenuAuctionHandle(Handle:menu_auction,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[20];
    GetMenuItem(menu_auction,slot,buffer,sizeof(buffer));
    decl String:Sfilter[10],String:Sid[10];
    new StringID=BreakString(buffer, Sid, sizeof(Sid));
    BreakString(buffer[StringID], Sfilter, sizeof(Sfilter));
    new id=StringToInt(Sid);
    new filter=StringToInt(Sfilter);
    if(id==0)
    {
      RefreshAuctionData(client);
      MenuSendAuction(client);
    }
    else
      if(id>0)
        PrepareMenuReadAuction(client,id,filter);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      AuctionsMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_auction);
}

public PrepareMenuReadAuction(client, id, filter)	{
  lastMenu2[client]=id;
  lastMenu3[client]=filter;
  SetMenuOff(client, 15.0, "preparing auction menu");
  GetAuctionThread(client, id, filter);

}

public MenuReadAuction(client, Id,fromId,time,bid,buyout,item[UniqueItem],String:fromname[],amount,bidId, gold, goldBuyout)
{
  decl String:title[255];
  new Handle:menu_auction=CreateMenu(MenuAuctionReadHandle);
  SetMenuExitBackButton(menu_auction,true);

  new itemlvl=item[Item_level];
  Format(title,sizeof(title),"%T","read auction title",client,item[Item_Name],amount,itemlvl);
  SetMenuTitle(menu_auction,title);
  decl String:SId[20];
  IntToString(Id,SId,sizeof(SId));
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"%T","hours left",client,time);
  decl String:Sfilter[10];
  IntToString(lastMenu3[client],Sfilter,sizeof(Sfilter));
  AddMenuItem(menu_auction,Sfilter,buffer,ITEMDRAW_DISABLED);
  Format(buffer,sizeof(buffer),"%T","from char",client,fromname,fromId);
  AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
  new bool:goldAuc = (gold > 0);
  new newbid=RoundToCeil(float(bid)*(1.0+AUCTION_BID_INC));
  if(goldAuc) {
    newbid = RoundToCeil(float(gold)*(1.0+AUCTION_BID_INC));
    decl String:sGold[20], String:sGold2[20];
    IntToStringfloat(gold, sGold, 20);
    IntToStringfloat(newbid, sGold2, 20);
    Format(buffer, sizeof(buffer), "%T", "read bid gold", client, sGold, sGold2);
  }
  else
    Format(buffer,sizeof(buffer),"%T","read bid",client,bid,newbid);
  if((time>=0) && (bidId!=characterids[client][0]) )
    AddMenuItem(menu_auction,SId,buffer);
  else
  {
    if(bidId==characterids[client][0])
      Format(buffer,sizeof(buffer),"%s %T",buffer,"your bid",client);
    if(time<0)
      Format(buffer,sizeof(buffer),"%s %T",buffer,"timeout",client);
    AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
  }
  if(goldAuc)	{
    Format(buffer, sizeof(buffer), "%T", "this is gold auction", client);
    AddMenuItem(menu_auction,"0", buffer, ITEMDRAW_DISABLED);
  }
  else	{
  	Format(buffer, sizeof(buffer), "%T", "read gold is 0", client);
  	AddMenuItem(menu_auction,"0", buffer, ITEMDRAW_DISABLED);
  }
  if(!goldAuc)
    Format(buffer,sizeof(buffer),"%T","read buyout",client,buyout);
  else  {
    decl String:sGold[20];
    IntToStringfloat(goldBuyout, sGold, 20);
    Format(buffer, sizeof(buffer), "%T", "read buyout gold", client, sGold);

  }
  if((time>=0) && (buyout>0 || goldBuyout > 0))
    AddMenuItem(menu_auction,SId,buffer);
  else
    AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
  Format(buffer,sizeof(buffer),"%T","information",client);
  Format(SId,sizeof(SId),"%i %i",item[Item_defId], item[Item_id]);
  AddMenuItem(menu_auction,SId,buffer);
  DisplayMenu(menu_auction,client,MENU_TIME_FOREVER);
}


public MenuAuctionReadHandle(Handle:menu_auction,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[50];
    GetMenuItem(menu_auction,slot,buffer,sizeof(buffer));
    new Id=-1, unique=-1;
    if(slot!=5)
      Id=StringToInt(buffer);
    else  {
      decl String:temp[10];
      new StringID=BreakString(buffer, temp, sizeof(temp));
      Id = StringToInt(temp);
      BreakString(buffer[StringID], temp, sizeof(temp));
      unique=StringToInt(temp);
    }
    GetMenuItem(menu_auction,0,buffer,sizeof(buffer));
    new filter=StringToInt(buffer);
    switch(slot)
    {
      case 2:
      {
        WCMessage(client, "your bid is proceeding", client);
      	AttemptToBuy(BuyType:Auction_Bid, client, Id);
      	PrepareAuctionMenu(client,filter);
      	/*

        */
      }
      case 4:
      {
        WCMessage(client, "your buyout is proceeding", client);
      	AttemptToBuy(BuyType:Auction_Buyout, client, Id);
      	PrepareAuctionMenu(client,filter);
      	/*
        new fromId,time,bid,buyout,item[UniqueItem],String:fromname[20],amount,bidId;
        if(GetAuction(Id,fromId,time,bid,buyout,item,fromname,amount,bidId))
        {
        if(buyout>GetMoney(client))
        {
          WCMessage(client,"no money",client);
          AuctionsMenu(client);
        }
        else
        if(time<0)
          WCMessage(client,"timeout",client);
        else
        {
          new oldclient=IsCharacterOnline(bidId);
          if(oldclient>0)
          {
            wcSetMoney(oldclient,GetMoney(oldclient)+bid);
            WCMessage(client,"not leading bet",client,item[Item_Name]);
          }
          else
          {
            decl String:squery[255];
            Format(squery,sizeof(squery),"SELECT money FROM wc_Characters WHERE CharacterId=%i",bidId);
            new Handle:query=SQL_Query(hSQL,squery);
            new oldmoney;
            if(query)
              if(SQL_FetchRow(query))
                oldmoney=GetSQLDataInt(query,"money");
            oldmoney+=bid;
            if(query!=INVALID_HANDLE)
              CloseHandle(query);
            wcUpdatePlayerInt(bidId,"money",oldmoney,"CharacterId","wc_Characters");
          }
          wcSetMoney(client,GetMoney(client)-buyout);
          decl String:text[255];
          Format(text,sizeof(text),"%T","bought out item",client,buyout);
          CreateMail(30,0,"Auction House","Item Buy Out",characterids[client][0],0,item,text,amount);
          Format(text,sizeof(text),"%T","your item is sold out",client,item[Item_Name]);
          new item2[UniqueItem];
          CreateMail(30,0,"Auction House","Your Item is Bought Out",fromId,buyout,item2,text,0);
          DeleteAuction(Id);
        }
        }
        */
      }
      case 5:  {

        lastMenu[client]=3;
        new uitem[UniqueItem];
        GetItemProperties(uitem, Id, unique);
        ItemInfMenu(client,uitem,false);
      }
    }
    if(Id==0)
      AuctionsMenu(client);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
    {
      decl String:buffer[10];
      GetMenuItem(menu_auction,0,buffer,sizeof(buffer));
      new Id=StringToInt(buffer);
      PrepareAuctionMenu(client,Id);
    }
  if(action==MenuAction_End)
    CloseHandle(menu_auction);
}
//auction

public ClearMailMenu(client)
{
  ClientMailQueue[client]=0;
  for(new x=0;x<=50;x++)
  {
    ClientMailDataId[client][x]=0;
    ClientMailTime[client][x]=0;
    ClientMailFromName[client][x]="No Name";
    ClientMailNew[client][x]=0;
  }
}

public MailMenu(client)
{
  ClearMailMenu(client);
  SetMenuOff(client,30.0);
  decl String:buffer[100];
  Format(buffer,sizeof(buffer),"SELECT * FROM wc_CharacterMail WHERE (CharacterId=%i)",characterids[client][0]);

  SQL_TQuery(TSQL,TMailMenu,buffer,client);
}

public TMailMenu(Handle:db,Handle:query,String:error[],any:client)
{

  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      new mail;
      if(SQL_FetchRow(query))
      {
        decl String:column[50];
        //PrintToServer("fetched 759");

        for(new x=1;x<=50;x++)
        {
          Format(column,sizeof(column),"mail_%i",x);
          //PrintToServer("mailid = %i at place of %i",GetSQLDataInt(query,column),x);
          ClientMailDataId[client][x]=GetSQLDataInt(query,column);
          if(ClientMailDataId[client][x]>0)
            mail++;
        }
      }
      charactersdata[client][CurrentCharacterIndex(client)][1]=mail;
      MailMenu2(client);
    }
    else
    {
      WCMessage(client,"failed loading mail",client);
      LogError ("WC FAILED m.1126 %s",error);
    }
  }
}

public MailMenu2(client)
{
  decl String:buffer[200];
  for(new x=1;x<=50;x++)
  {
    new mailid=ClientMailDataId[client][x];
    //PrintToServer("checking mail with index %i and id %i",x,mailid);
    if(mailid>0)
    {
      //PrintToServer("getting mail %i",mailid);
      ClientMailQueue[client]--;
      Format(buffer,sizeof(buffer),"SELECT mailId, topic, fromname, DATEDIFF(NOW(), `time`) AS `time`, new FROM wc_Mail WHERE MailId = '%i'",mailid);

      SQL_TQuery(TSQL,PreloadMail,buffer,client);
    }
  }
  if(ClientMailQueue[client]==0)
    MailMenuDisplay(client);
}

public PreloadMail(Handle:db,Handle:query,String:error[],any:client)
{

  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      if(SQL_FetchRow(query))
      {
        new mailid=GetSQLDataInt(query,"mailId");
        new x=0;
        for (new y=1;y<=50;y++)
        {
          if (ClientMailDataId[client][y]==mailid)
          {
            x=y;
            break;
          }
        }
        ClientMailQueue[client]++;
        //PrintToServer("executed 1179 with x = %i . queue now %i",x,ClientMailQueue[client]);
        if(x>0)
        {
          ClientMailTime[client][x]=GetSQLDataInt(query,"time");
          GetSQLDataStr(query,"topic",ClientMailTopic[client][x],50);
          GetSQLDataStr(query,"fromname",ClientMailFromName[client][x],20);
          ClientMailNew[client][x]=GetSQLDataInt(query,"new");
          ReplaceString(ClientMailTopic[client][x],50,"@`","'");
          ReplaceString(ClientMailFromName[client][x],20,"@`","'");
        }
      }
      else
      {
        WCMessage(client,"failed loading mail",client);
        LogError("did not find mail from character %i",characterids[client][0]);
      }
    }
    else
    {
      WCMessage(client,"failed loading mail",client);
      LogError ("WC FAILED m.1190 %s",error);
    }
    if(ClientMailQueue[client]==0)
      MailMenuDisplay(client);
  }
}

MailMenuDisplay(client)
{
  SetMenuOff(client,0.0);
  if(IsClientConnected(client))  if(IsClientInGame(client) && (playerloaded[client]==1))
  {
   decl String:title[255];
   new Handle:menu_mail=CreateMenu(MenuMailHandle);
   SetMenuExitBackButton(menu_mail,true);
   Format(title,sizeof(title),"%T","wc mail menu",client);
   SetMenuTitle(menu_mail,title);
   decl String:buffer[255];
   Format(buffer,sizeof(buffer),"%T","send message",client);
   AddMenuItem(menu_mail,"0",buffer);
   new String:id[20];
   for(new x=0;x<=50;x++)  if(ClientMailDataId[client][x]>0)
   {
    //LogMessage("mail id = %i",mailIds[x]);
    IntToString(ClientMailDataId[client][x],id,sizeof(id));
    Format(buffer,sizeof(buffer),"%T","new mails index",client,ClientMailTopic[client][x],ClientMailFromName[client][x],MAILCLEAN_DAYS-ClientMailTime[client][x]);
    if(ClientMailNew[client][x]==1)
      Format(buffer,sizeof(buffer),"%s %T",buffer,"this new mail",client);
    AddMenuItem(menu_mail,id,buffer);
   }
   DisplayMenu(menu_mail,client,MENU_TIME_FOREVER);
  }
}

public MenuMailHandle(Handle:menu_mail,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[50];
    GetMenuItem(menu_mail,slot,buffer,sizeof(buffer));
    new id=StringToInt(buffer);
    if(id==0)
    {
      RefreshMailData(client);
      MenuSendMail(client);
    }
    else
      if(id>0)
      {
        //LogMessage("reading mail = %i",id);
        MenuReadMail(client,id);
      }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ShopsMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_mail);
}

public RefreshAuctionData(client)
{
  g_auction_char[client]=characterids[client][0];
  g_auction_pack_amount[client]=0;
  if(g_auction_pack[client]!=any:0)
    CloseHandle(g_auction_pack[client]);
  g_auction_pack[client]=CreateDataPack();
  g_auction_time[client]=0.0;
}

stock RefreshMailData(client)
{
  g_mail_char[client]=characterids[client][0];
  g_mail_pack_amount[client]=0;
  if(g_mail_pack[client]!=any:0)
    CloseHandle(g_mail_pack[client]);
  g_mail_pack[client]=CreateDataPack();
  g_mail_time[client]=0.0;
  g_mail_slot[client]=-1;
}

public MenuSendAuction(client)
{
  if(g_auction_char[client]!=characterids[client][0])
    RefreshAuctionData(client);
  new x=g_auction_pack_amount[client];
  new Handle:menu_auction=CreateMenu(MenuSendAuctionHandle);
  SetMenuExitBackButton(menu_auction,false);
  decl String:title[255];
  new String:buffer[255];
  Format(title,sizeof(title),"%T","wc auction send mod",client);
  SetMenuTitle(menu_auction,title);
  switch(x)
  {
    case 0:
    {
      ResetPack(g_auction_pack[client]);
      Format(buffer,sizeof(buffer),"%T","mail choose slot",client);
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      decl String:Sx[4];
      for (new y=1;y<=100;y++)
      {
        decl String:Scategory[255];
        IntToString(y,Sx,sizeof(Sx));
        new item[UniqueItem];
        GetInvItem(item, client, y);
        new iamount=GetSlotItemAmount(client,y);
        TranslateCategory(item[Item_category],Scategory, client);
        if(iamount>0)
          Format(buffer,sizeof(buffer),"%s (%s) x%i",item[Item_Name],Scategory,iamount);
        else  if(iamount==-1)
          Format(buffer,sizeof(buffer),"%s (%s) (Soulbound)",item[Item_Name],Scategory);
        else
          Format(buffer,sizeof(buffer),"%s (%s) (Player Bound)",item[Item_Name],Scategory);
        if(iamount>0)
          AddMenuItem(menu_auction,Sx,buffer);
        else
          AddMenuItem(menu_auction,Sx,buffer,ITEMDRAW_DISABLED);
      }
    }
    case 1:
    {
      ResetPack(g_auction_pack[client]);
      //item write
      new slot=ReadPackCell(g_auction_pack[client]);
      new item[UniqueItem];
      GetInvItem(item,client,slot);
      decl String:Scategory[255];
      TranslateCategory(item[Item_category],Scategory, client);
      new iamount=GetSlotItemAmount(client,slot);
      if(iamount<=0)
      {
        WCMessage(client,"can't send this item",client);
        RefreshAuctionData(client);
        AuctionsMenu(client);
        return;
      }
      new willcost=item[Item_cost];
      willcost=RoundToNearest(float(willcost)*AUCTION_TAX)*iamount;
      Format(buffer,sizeof(buffer),"%T","auction item",client,item[Item_Name],Scategory,iamount,willcost);
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      //end of item write
      Format(buffer,sizeof(buffer),"%T","choose silver",client);
      AddMenuItem(menu_auction,"silver",buffer);
      Format(buffer, sizeof(buffer), "%T", "choose gold", client);
      AddMenuItem(menu_auction,"gold",buffer);
      g_auction_time[client]=GetEngineTime()+40.0;
    }
    case 2:
    {
      ResetPack(g_auction_pack[client]);
      //item write
      new slot=ReadPackCell(g_auction_pack[client]);
      new bool:gold = bool:ReadPackCell(g_auction_pack[client]);
      new item[UniqueItem];
      GetInvItem(item,client,slot);
      decl String:Scategory[255];
      TranslateCategory(item[Item_category],Scategory, client);
      new iamount=GetSlotItemAmount(client,slot);
      if(iamount<=0)
      {
        WCMessage(client,"can't send this item",client);
        RefreshAuctionData(client);
        AuctionsMenu(client);
        return;
      }
      new willcost=item[Item_cost];
      willcost=RoundToNearest(float(willcost)*AUCTION_TAX)*iamount;
      Format(buffer,sizeof(buffer),"%T","auction item",client,item[Item_Name],Scategory,iamount,willcost);
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      if(!gold)
        Format(buffer, sizeof(buffer), "%T", "using silver", client);
      else
        Format(buffer, sizeof(buffer), "%T", "using gold", client);
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      //end of item write
      Format(buffer,sizeof(buffer),"%T","type bid",client);
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      g_auction_time[client]=GetEngineTime()+40.0;
    }
    case 3:
    {
      ResetPack(g_auction_pack[client]);

      //item write
      new slot=ReadPackCell(g_auction_pack[client]);
      new bool:gold = bool:ReadPackCell(g_auction_pack[client]);
      //case 2
      new bid=ReadPackCell(g_auction_pack[client]);
      //end
      new item[UniqueItem];
      GetInvItem(item,client,slot);
      decl String:Scategory[255];
      TranslateCategory(item[Item_category],Scategory, client);
      new iamount=GetSlotItemAmount(client,slot);
      if(iamount<=0)
      {
        WCMessage(client,"can't send this item",client);
        RefreshAuctionData(client);
        AuctionsMenu(client);
        return;
      }
      new willcost=item[Item_cost];
      willcost=RoundToNearest(float(willcost)*AUCTION_TAX)*iamount;
      Format(buffer,sizeof(buffer),"%T","auction item",client,item[Item_Name],Scategory,iamount,willcost);
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      if(!gold)
        Format(buffer, sizeof(buffer), "%T", "using silver", client);
      else
        Format(buffer, sizeof(buffer), "%T", "using gold", client);
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      //end of item write
      if(!gold)
        Format(buffer,sizeof(buffer),"%T","bid",client,bid);
      else  {
        decl String:sGold[20];
        IntToStringfloat(bid, sGold, 20);
        Format(buffer, sizeof(buffer), "%T", "bid gold", client, sGold);
      }
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","type buyout",client);
      AddMenuItem(menu_auction,"buyout",buffer);
      g_auction_time[client]=GetEngineTime()+40.0;
    }
    case 4:
    {
      ResetPack(g_auction_pack[client]);
      //item write
      new slot=ReadPackCell(g_auction_pack[client]);
      new bool:gold = bool:ReadPackCell(g_auction_pack[client]);
      //case 3
      new bid=ReadPackCell(g_auction_pack[client]);
      new buyout=ReadPackCell(g_auction_pack[client]);
      //end
      new item[UniqueItem];
      GetInvItem(item,client,slot);
      decl String:Scategory[255];
      TranslateCategory(item[Item_category],Scategory, client);
      new iamount=GetSlotItemAmount(client,slot);
      if(iamount<=0)
      {
        WCMessage(client,"can't send this item",client);
        RefreshAuctionData(client);
        AuctionsMenu(client);
        return;
      }
      new willcost=item[Item_cost];
      willcost=RoundToNearest(float(willcost)*AUCTION_TAX)*iamount;
      Format(buffer,sizeof(buffer),"%T","auction item",client,item[Item_Name],Scategory,iamount,willcost);
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      if(!gold)
        Format(buffer, sizeof(buffer), "%T", "using silver", client);
      else
        Format(buffer, sizeof(buffer), "%T", "using gold", client);
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      //end of item write
      if(!gold)
        Format(buffer,sizeof(buffer),"%T","bid",client,bid);
      else  {
        decl String:sGold[20];
        IntToStringfloat(bid, sGold, 20);
        Format(buffer, sizeof(buffer), "%T", "bid gold", client, sGold);
      }
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      if(buyout>0)  {
        if(!gold)
          Format(buffer,sizeof(buffer),"%T","buyout",client,buyout);
          else  {
            decl String:sGold[20];
            IntToStringfloat(buyout, sGold, 20);
            Format(buffer, sizeof(buffer), "%T", "buyout gold", client, sGold);
          }
      }
      else
      {
        Format(buffer,sizeof(buffer),"%T","buyout disabled",client);
      }
      AddMenuItem(menu_auction,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","auction confirm send",client);
      AddMenuItem(menu_auction,"send",buffer);
    }
  }
  Format(buffer,sizeof(buffer),"%T","cancel auction mod",client);
  AddMenuItem(menu_auction,"cancel",buffer);
  DisplayMenu(menu_auction,client,MENU_TIME_FOREVER);
  return;
}

public MenuSendAuctionHandle(Handle:menu_auction,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[20];
    GetMenuItem(menu_auction,slot,buffer,sizeof(buffer));
    new id=StringToInt(buffer);
    if(StrEqual(buffer,"0"))
      MenuSendAuction(client);
    else if(StrEqual(buffer,"silver"))  {
      ResetPack(g_auction_pack[client]);
      ReadPackCell(g_auction_pack[client]);
      WritePackCell(g_auction_pack[client], _:false);
      g_auction_pack_amount[client]++;
      if(g_auction_pack_amount[client]==2)
        MenuSendAuction(client);
    }
    else if(StrEqual(buffer,"gold"))  {
      ResetPack(g_auction_pack[client]);
      ReadPackCell(g_auction_pack[client]);
      WritePackCell(g_auction_pack[client], _:true);
      g_auction_pack_amount[client]++;
      if(g_auction_pack_amount[client]==2)
        MenuSendAuction(client);
    }

    if(StrEqual(buffer,"buyout"))
    {
      ResetPack(g_auction_pack[client]);
      ReadPackCell(g_auction_pack[client]);
      ReadPackCell(g_auction_pack[client]);
      ReadPackCell(g_auction_pack[client]);
      WritePackCell(g_auction_pack[client],0);
      g_auction_pack_amount[client]++;
      if(g_auction_pack_amount[client]==4)
        MenuSendAuction(client);
    }
    if(StrEqual(buffer,"cancel"))
    {
      RefreshAuctionData(client);
      AuctionsMenu(client);
    }
    if(StrEqual(buffer,"send"))
    {
      if((g_auction_pack_amount[client]==4) && (g_auction_char[client]==characterids[client][0]))
      {
        ResetPack(g_auction_pack[client]);
        new slot2=ReadPackCell(g_auction_pack[client]);
        new bool:gold = bool:ReadPackCell(g_auction_pack[client]);
        new bid=ReadPackCell(g_auction_pack[client]);
        new buyout=ReadPackCell(g_auction_pack[client]);
        new item[UniqueItem];
        new amount;
        if((slot2>=0) && (slot2 <=100))
        {
          GetInvItem(item,client,slot2);
          amount=GetSlotItemAmount(client,slot2);
          if(amount<=0 || item[Item_noTrade] == 1)
          {
            WCMessage(client,"can't send this item",client);
            RefreshAuctionData(client);
            AuctionsMenu(client);
          }
        }
        if(amount>0 && item[Item_noTrade] == 0)
        {
        new String:clientname[20];
        GetClientName(client,clientname,sizeof(clientname));
        new willcost=item[Item_cost];
        willcost=RoundToNearest(float(willcost)*AUCTION_TAX)*amount;
        if(GetMoney(client)>=willcost)
        {
          wcSetMoney(client,GetMoney(client)-willcost);
          new unique = GetInvUnique(client, slot2);
          SetInvUnique(client, slot2, 0);
          if(unique >0)  {
            UpdateUniqueState(unique, 3);
          }
          wcSetStaticSlot(client,slot2,0);
          wcSetStaticSlotAmount(client,slot2,0);
          if(slot2 > 0)
            SQL_UpdateSlot(client, slot2);
          CreateAuction(g_auction_char[client],clientname,bid,buyout,item,amount,gold);
          WCMessage(client,"auction sent",client);
          RefreshAuctionData(client);
          AuctionsMenu(client);
        }
        else
          WCMessage(client,"no money",client);
        }
      }
    }
    if(id>0)
    {
      ResetPack(g_auction_pack[client]);
      WritePackCell(g_auction_pack[client],id);
      g_auction_pack_amount[client]++;
      if(g_auction_pack_amount[client]==1)
        MenuSendAuction(client);
      else
      {
        WCMessage(client,"no item for inf",client);
        RefreshAuctionData(client);
        MenuSendAuction(client);
      }
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ClassMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_auction);
}

public MenuSendMail(client)
{
  if(IsClientConnected(client))
  {
  if(g_mail_char[client]!=characterids[client][0])
    RefreshMailData(client);
  //Support from inventory menu
  if(g_mail_slot[client] >= 0 && g_mail_pack_amount[client] == 4) {
    ResetPack(g_mail_pack[client]);
    ReadPackCell(g_mail_pack[client]);
    //writing character
    decl String:buffer[255];
    ReadPackString(g_mail_pack[client],buffer,sizeof(buffer));
    ReadPackString(g_mail_pack[client],buffer,sizeof(buffer));
    ReadPackCell(g_mail_pack[client]);
    ReadPackCell(g_mail_pack[client]);
    WritePackCell(g_mail_pack[client], g_mail_slot[client]);
    g_mail_pack_amount[client] = 5;
  }
  new x=g_mail_pack_amount[client];
  new Handle:menu_mail=CreateMenu(MenuSendMailHandle);
  SetMenuExitBackButton(menu_mail,false);
  decl String:title[255];
  new String:buffer[255];
  new String:buffer2[255]; //bb
  Format(title,sizeof(title),"%T","wc mail send mod",client);
  SetMenuTitle(menu_mail,title);
  switch(x)
  {
    case 0:
    {
      Format(buffer,sizeof(buffer),"%T","type adresser",client);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      g_mail_time[client]=GetEngineTime()+60.0;
    }
    case 1:
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      //writing character
      ReadPackString(g_mail_pack[client],buffer,sizeof(buffer));
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      //end of writing character
      Format(buffer,sizeof(buffer),"%T","type topic",client);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      g_mail_time[client]=GetEngineTime()+120.0;
    }
    case 2:
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      //writing character
      ReadPackString(g_mail_pack[client],buffer,sizeof(buffer));
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      //end of writing character
      new String:topic[50];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      Format(buffer,sizeof(buffer),"%T","mail topic name",client,topic);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","type gold",client);
      Format(buffer2, sizeof(buffer2), "%T", "your silver", GetMoney(client)); //bb
      StrCat(buffer, sizeof(buffer), buffer2);
      AddMenuItem(menu_mail,"gold",buffer);
    }
    case 3:
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      //writing character
      ReadPackString(g_mail_pack[client],buffer,sizeof(buffer));
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      //end of writing character
      new String:topic[50];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      new gold=ReadPackCell(g_mail_pack[client]);
      Format(buffer,sizeof(buffer),"%T","mail topic name",client,topic);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","mail gold",client,gold);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","type real",client);
      decl String:sReal[20];
      IntToStringfloat(GetGold(client), sReal, sizeof(sReal));
      Format(buffer2, sizeof(buffer2), "\nYour gold: %s.", sReal); //bb
      StrCat(buffer, sizeof(buffer), buffer2);
      AddMenuItem(menu_mail,"real",buffer);
    }
    case 4:
    {
      ResetPack(g_mail_pack[client]);
      new receiver = ReadPackCell(g_mail_pack[client]);
      //writing character
      ReadPackString(g_mail_pack[client],buffer,sizeof(buffer));
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      //end of writing character
      new String:topic[50];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      new gold=ReadPackCell(g_mail_pack[client]);
      Format(buffer,sizeof(buffer),"%T","mail topic name",client,topic);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","mail gold",client,gold);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      new real = ReadPackCell(g_mail_pack[client]);
      decl String:sReal[20];
      IntToStringfloat(real, sReal, 20);
      Format(buffer,sizeof(buffer),"%T","mail real",client, sReal);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);


      Format(buffer,sizeof(buffer),"%T","mail choose slot",client,gold);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","mail no item",client,gold);
      AddMenuItem(menu_mail,"item",buffer);
      new bool:sendingToOwnCharacter=false;
      for(new i=0;i<=15;i++)  {
        if(receiver == characterids[client][i])
          sendingToOwnCharacter=true;
      }
      decl String:Sx[4];
      for (new y=1;y<=100;y++)
      {
        decl String:Scategory[255];
        IntToString(y,Sx,sizeof(Sx));
        new uitem[UniqueItem];
        GetInvItem(uitem, client, y);
        new category=uitem[Item_category];
        new iamount=GetSlotItemAmount(client,y);
        TranslateCategory(category,Scategory, client);
        if(iamount>0)
          Format(buffer,sizeof(buffer),"%s (%s) x%i",uitem[Item_Name],Scategory,iamount);
        else if(iamount==-1)  {
           Format(buffer, sizeof(buffer), "%T", "Item Soulbound", client);
           Format(buffer,sizeof(buffer),"%s (%s) (%s)",uitem[Item_Name],Scategory,buffer);
         }
        else  {
          Format(buffer, sizeof(buffer), "%T", "Item Player Bound", client);
          Format(buffer,sizeof(buffer),"%s (%s) (%s)",uitem[Item_Name],Scategory,buffer);
          }
        if( (iamount>0) || (iamount==-2) )  {
         if(uitem[Item_noTrade] == 1) {
           Format(buffer, sizeof(buffer), "%s (Not Tradable)", buffer);
         }
         if(uitem[Item_noTrade] == 1 && (!sendingToOwnCharacter))
           AddMenuItem(menu_mail,Sx,buffer,ITEMDRAW_DISABLED);
         else
           AddMenuItem(menu_mail,Sx,buffer);
        }
        else
          AddMenuItem(menu_mail,Sx,buffer,ITEMDRAW_DISABLED);
      }
    }
    case 5:
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      //writing character
      ReadPackString(g_mail_pack[client],buffer,sizeof(buffer));
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      //end of writing character
      new String:topic[50];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      new gold=ReadPackCell(g_mail_pack[client]);
      new real = ReadPackCell(g_mail_pack[client]);
      decl String:sReal[20];
      IntToStringfloat(real, sReal, 20);
      new slot=ReadPackCell(g_mail_pack[client]);
      Format(buffer,sizeof(buffer),"%T","mail topic name",client,topic);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","mail gold",client,gold);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","mail real",client,sReal);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      if(slot>0)
      {
        decl String:Scategory[255];
        new item[UniqueItem];
        GetInvItem(item, client, slot);
        new category=item[Item_category];
        TranslateCategory(category,Scategory, client);
        new iamount=GetSlotItemAmount(client,slot);
        if(iamount<=0)
        {
          WCMessage(client,"can't send this item",client);
          RefreshMailData(client);
          MenuSendMail(client);
          return;
        }
        Format(buffer,sizeof(buffer),"%T","mail item",client,item[Item_Name],Scategory,iamount);
        AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      }
      else
      {
        Format(buffer,sizeof(buffer),"%T","no items",client);
        AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      }
      Format(buffer,sizeof(buffer),"%T","mail write text",client);
      AddMenuItem(menu_mail,"text",buffer);
      g_mail_time[client]=GetEngineTime()+120.0;
    }
    case 6:
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      //writing character
      ReadPackString(g_mail_pack[client],buffer,sizeof(buffer));
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      //end of writing character
      new String:topic[50],String:text[255];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      new gold=ReadPackCell(g_mail_pack[client]);
      new real = ReadPackCell(g_mail_pack[client]);
      decl String:sReal[20];
      IntToStringfloat(real, sReal, 20);
      new slot=ReadPackCell(g_mail_pack[client]);
      ReadPackString(g_mail_pack[client],text,sizeof(text));
      Format(buffer,sizeof(buffer),"%T","mail topic name",client,topic);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","mail gold",client,gold);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","mail real",client,sReal);
      AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      if(slot>0)
      {
        decl String:Scategory[255];
        new item[UniqueItem];
        GetInvItem(item,client,slot);
        new category=item[Item_category];
        TranslateCategory(category,Scategory, client);
        new iamount=GetSlotItemAmount(client,slot);
        if(iamount<=0)
        {
          WCMessage(client,"can't send this item",client);
          RefreshMailData(client);
          MenuSendMail(client);
          return;
        }
        Format(buffer,sizeof(buffer),"%T","mail item",client,item[Item_Name],Scategory,iamount);
        AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      }
      else
      {
        Format(buffer,sizeof(buffer),"%T","no items",client);
        AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
      }
      AddMenuItem(menu_mail,"0",text,ITEMDRAW_DISABLED);
      Format(buffer,sizeof(buffer),"%T","mail confirm send",client);
      AddMenuItem(menu_mail,"send",buffer);
    }
  }
  Format(buffer,sizeof(buffer),"%T","cancel mail mod",client);
  AddMenuItem(menu_mail,"cancel",buffer);
  DisplayMenu(menu_mail,client,MENU_TIME_FOREVER);
  }
  return;
}

public MenuSendMailHandle(Handle:menu_mail,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[20],String:Stemp[255];
    GetMenuItem(menu_mail,slot,buffer,sizeof(buffer));
    new id=StringToInt(buffer);
    if(StrEqual(buffer,"0"))
      MenuSendMail(client);
    if(StrEqual(buffer,"gold"))
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      ReadPackString(g_mail_pack[client],Stemp,sizeof(Stemp));
      new String:topic[50];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      WritePackCell(g_mail_pack[client],0);
      g_mail_pack_amount[client]++;
      if(g_mail_pack_amount[client]==3)
        MenuSendMail(client);
    }
    if(StrEqual(buffer,"real"))
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      ReadPackString(g_mail_pack[client],Stemp,sizeof(Stemp));
      new String:topic[50];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      ReadPackCell(g_mail_pack[client]);
      WritePackFloat(g_mail_pack[client],0.0);
      g_mail_pack_amount[client]++;
      if(g_mail_pack_amount[client]==4)
        MenuSendMail(client);
    }
    if(StrEqual(buffer,"item"))
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      ReadPackString(g_mail_pack[client],Stemp,sizeof(Stemp));
      new String:topic[50];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      ReadPackCell(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      WritePackCell(g_mail_pack[client],0);
      g_mail_pack_amount[client]++;
      if(g_mail_pack_amount[client]==5)
        MenuSendMail(client);
    }
    if(StrEqual(buffer,"text"))
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      ReadPackString(g_mail_pack[client],Stemp,sizeof(Stemp));
      new String:topic[50];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      ReadPackCell(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      decl String:tmp[50];
      Format(tmp,sizeof(tmp),"%T","Mail No Text", client);
      WritePackString(g_mail_pack[client],tmp);
      g_mail_pack_amount[client]++;
      if(g_mail_pack_amount[client]==6)
        MenuSendMail(client);
    }
    if(StrEqual(buffer,"cancel"))
    {
      RefreshMailData(client);
      MailMenu(client);
    }
    if(StrEqual(buffer,"send"))
    {
      if((g_mail_pack_amount[client]==6) && (g_mail_char[client]==characterids[client][0]))
      {
        ResetPack(g_mail_pack[client]);
        new adresser=ReadPackCell(g_mail_pack[client]);
        ReadPackString(g_mail_pack[client],Stemp,sizeof(Stemp));
        new String:topic[50],String:text[255];
        ReadPackString(g_mail_pack[client],topic,sizeof(topic));
        new gold=ReadPackCell(g_mail_pack[client]);
        new real=ReadPackCell(g_mail_pack[client]);
        new slot2=ReadPackCell(g_mail_pack[client]);
        ReadPackString(g_mail_pack[client],text,sizeof(text));
        new item[UniqueItem];
        new amount;
        if((slot2>=0) && (slot2 <=100))
        {
          GetInvItem(item, client,slot2);
          amount=GetSlotItemAmount(client,slot2);
        }
        if((slot2>0) && (amount<=0) && (!HasCharacter(client, adresser)) )
        {
          WCMessage(client,"can't send this item",client);
          RefreshMailData(client);
          MenuSendMail(client);
        }
        else
        {
        new String:clientname[20];
        GetClientName(client,clientname,sizeof(clientname));
        if(GetMoney(client)>=gold)
        {
        if(GetGold(client)>=real) {
          wcSetMoney(client,GetMoney(client)-gold);
          SQL_UpdateMoney(client);
          wcSetStaticSlot(client,slot2,0);
          SetInvUnique(client, slot2, 0);
          UpdateUniqueState(item[Item_id], 3);
          wcSetStaticSlotAmount(client,slot2,0);
          if(slot2 > 0 )
            SQL_UpdateSlot(client,slot2);
          if(real>0)  {
            decl String:reason[255];
            Format(reason,sizeof(reason),"Sent %d / 100 gold from %d to %d characters", real, g_mail_char[client], adresser);
            SpendGold(client, real, "mail", reason);
          }
          CreateMail(30,g_mail_char[client],clientname,topic,adresser,gold,item,text,amount,real);

          RefreshMailData(client);
          WCMessage(client,"message sent",client);
        }
        else  {
          decl String:tmp1[20], String:tmp2[20];
          IntToStringfloat(GetGold(client), tmp1, 20);
          IntToStringfloat(real, tmp2, 20);
          WCMessage(client, "no gold", client, tmp1, tmp2);
        }

        }

        else
          WCMessage(client,"no money",client);
        }
      }
    }
    if(id>0)
    {
      ResetPack(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      ReadPackString(g_mail_pack[client],Stemp,sizeof(Stemp));
      new String:topic[50];
      ReadPackString(g_mail_pack[client],topic,sizeof(topic));
      ReadPackCell(g_mail_pack[client]);
      ReadPackCell(g_mail_pack[client]);
      WritePackCell(g_mail_pack[client],id);
      g_mail_pack_amount[client]++;
      if(g_mail_pack_amount[client]==5)
        MenuSendMail(client);
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ClassMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_mail);
}

public MenuReadMail(client,mailId)
{
   decl String:buffer[500];
   Format(buffer,sizeof(buffer),"SELECT mailId, CharacterId, amount, fromId, fromname, topic, gold, item, text, DATEDIFF(NOW(), `time`) AS `time`, `new`, `real`, `unique`, wc_Items.* FROM wc_Mail LEFT JOIN wc_Items ON `unique` = id WHERE mailId = '%d'",mailId);

   SQL_TQuery(TSQL,MenuReadMailDisplay,buffer,client);
   SetMenuOff(client,20.0);
}

public MenuReadMailDisplay(Handle:db,Handle:query,String:error[],any:client)
{

   new mailId,days,fromId,String:fromname[20],String:topic[50],gold,item,String:text[255],amount,real,unique;
   new isNew=0;
   if(query)
   {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      mailId=GetSQLDataInt(query,"mailId");
      days=GetSQLDataInt(query,"time");
      fromId=GetSQLDataInt(query,"fromId");
      GetSQLDataStr(query,"fromname",fromname,sizeof(fromname));
      GetSQLDataStr(query,"topic",topic,sizeof(topic));
      gold=GetSQLDataInt(query,"gold");
      item=GetSQLDataInt(query,"item");
      amount=GetSQLDataInt(query,"amount");
      isNew = GetSQLDataInt(query, "new");
      real = GetSQLDataInt(query,"real");
      GetSQLDataStr(query,"text",text,sizeof(text));
      ReplaceString(fromname,sizeof(fromname),"@`","'");
      ReplaceString(topic,sizeof(topic),"@`","'");
      ReplaceString(text,sizeof(text),"@`","'");
      unique = GetSQLDataInt(query, "unique");
      if(unique > 0)  {
        LoadItemCode(query, unique, true);
      }
    }
    else
      WCMessage(client,"failed loading mail",client);
   }
   SetMenuOff(client,0.0);
   new Handle:menu_mail=CreateMenu(MenuMailReadHandle);
   SetMenuExitBackButton(menu_mail,true);
   new String:title[255];
   if(isNew==1) {
    LogMessage("[WC] Client opened mail: id %i; from %i; topic %s gold %d /100 silver %i item %i x%i", mailId, fromId, topic, real, gold, item, amount);
    SetMailNoNew(mailId);
   }
   Format(title,sizeof(title),"%s",topic);
   SetMenuTitle(menu_mail,title);
   decl String:SmailId[50];
   IntToString(mailId,SmailId,sizeof(SmailId));
   decl String:buffer[255];
   Format(buffer,sizeof(buffer),"%T","days left",client,MAILCLEAN_DAYS-days);
   AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","from char",client,fromname,fromId);
   AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","money attached",client,gold);
   if(gold>0)
   {
    AddMenuItem(menu_mail,SmailId,buffer);
   }
   else
    AddMenuItem(menu_mail,SmailId,buffer,ITEMDRAW_DISABLED);
   decl String:tmp1[20];
   IntToStringfloat(real, tmp1, 20);
   Format(buffer,sizeof(buffer),"%T","gold attached",client,tmp1);
   if(real>0)
   {
    AddMenuItem(menu_mail,SmailId,buffer);
   }
   else
    AddMenuItem(menu_mail,SmailId,buffer,ITEMDRAW_DISABLED);
   if(item>0)
   {
     new uitem[UniqueItem];
     GetItemProperties(uitem, item, unique);
     Format(buffer,sizeof(buffer),"%T","item attached",client,uitem[Item_Name],amount);
     AddMenuItem(menu_mail,SmailId,buffer);
   }
   else
   {
    Format(buffer,sizeof(buffer),"%T","no item",client);
    AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
   }
   if(!StrEqual(text,"0"))
   {
    AddMenuItem(menu_mail,"0",text,ITEMDRAW_DISABLED);
   }
   else
   {
    Format(buffer,sizeof(buffer),"%T","no text",client);
    AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
   }
   Format(buffer,sizeof(buffer),"%T","delete message",client);
   if((item>0) || (gold > 0) || (real > 0))
    AddMenuItem(menu_mail,"0",buffer,ITEMDRAW_DISABLED);
   else
    AddMenuItem(menu_mail,SmailId,buffer);
   Format(buffer,sizeof(buffer),"%T","reply",client);
   decl String:SfromId[12];
   IntToString(fromId,SfromId,sizeof(SfromId));
   AddMenuItem(menu_mail,SfromId,buffer);
   DisplayMenu(menu_mail,client,MENU_TIME_FOREVER);
}

public ReplyMessage(client,fromId)
{
  if(fromId>0)
  {
      CancelClientMenu(client);
      decl String:buffer[255];
      Format(buffer,sizeof(buffer),"SELECT CharacterId, playerId, level, class FROM wc_Characters WHERE CharacterId = '%i'",fromId);
      SetMenuOff(client,20.0);
      g_mail_pack_amount[client]=-1;
      SQL_TQuery(TSQL,TMailSend,buffer,client);
   }
   else
  {
    WCMessage(client,"no char",client);
    RefreshMailData(client);
    MenuSendMail(client);
  }
}

public MenuMailReadHandle(Handle:menu_mail,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[50];
    GetMenuItem(menu_mail,slot,buffer,sizeof(buffer));
    new mailId=StringToInt(buffer);
    switch(slot)
    {
      case 2:
      {
        GetMoneyMail(client,mailId);
        MenuReadMail(client,mailId);
      }
      case 3:
      {
        GetGoldMail(client, mailId);
        MenuReadMail(client,mailId);
      }
      case 4:
      {
        GetItemMail(client,mailId);
      }
      case 6:
      {
        SetMenuOff(client,20.0);
        DeleteMessage(mailId,client);
      }
      case 7:
      {
      GetMenuItem(menu_mail,6,buffer,sizeof(buffer));
      new fromId=StringToInt(buffer);
      if((mailId>0) && (fromId>0))
        ReplyMessage(client,fromId);
      else
        WCMessage(client,"no reply",client);
      }
    }
    if(mailId==0)
      MailMenu(client);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      MailMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_mail);
}

public GetMoneyMail(client,mailId)
{
  decl String:buffer[128];
  Format(buffer,128,"SELECT gold FROM wc_Mail WHERE mailId = '%i'",mailId);

  SQL_TQuery(TSQL,TGetMoneyMail,buffer,client);
  wcUpdatePlayerInt(mailId,"gold",0,"mailId","wc_Mail");
}

public TGetMoneyMail(Handle:db,Handle:query,String:error[],any:client)
{

  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      new gold=GetSQLDataInt(query,"gold");
      if(gold>0)
      {
        wcSetMoney(client,(GetMoney(client)+gold));
        SQL_UpdateMoney(client);
        WCMessage(client,"obtained money",client,gold);
      }
    }
   }
}

public GetGoldMail(client,mailId)
{
  decl String:buffer[128];
  Format(buffer,128,"SELECT `real`, mailId, fromId FROM wc_Mail WHERE mailId = '%i'",mailId);

  SQL_TQuery(TSQL,TGetGoldMail,buffer,client);
  wcUpdatePlayerInt(mailId,"real",0,"mailId","wc_Mail");
}

public TGetGoldMail(Handle:db,Handle:query,String:error[],any:client)
{
  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      new gold=GetSQLDataInt(query,"real");
      //PrintToServer("debug msg: %d/100", gold);
      if(gold>0)
      {
        decl String:reason[200];
        Format(reason, sizeof(reason), "Received %d/100 gold from %d to %d characters. mailId %d", gold, characterids[client][0], GetSQLDataInt(query,"fromId"), GetSQLDataInt(query,"mailId"));
        AddGold(client, gold, "mail", reason);
        decl String:sgold[20];
        IntToStringfloat(gold, sgold, 20);
        WCMessage(client,"obtained gold",client,sgold);
      }
    }
    else
      LogError("[WC] Error 2803: received empty! get gold mail query");
  }
  else
    LogError("[WC] Error 2804: %s", error);
}

public TGetItemMail(Handle:db,Handle:query,String:error[],any:client)
{

  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      new item=GetSQLDataInt(query,"item");
      new unique = GetSQLDataInt(query,"unique");
      new amount=GetSQLDataInt(query,"amount");
      new mailId=GetSQLDataInt(query,"mailId");
      if(item>0)
      {
        new uitem[UniqueItem];
        GetItemProperties(uitem, item, unique);
        if(GiveItem(client, uitem, amount, "mail",true)>=0)
        {
            decl String:category[255];
            IdToCategory(uitem[Item_category], category);
            Format(category, sizeof(category), "%T", category, client);
            WCMessage(client,"obtained item",client,uitem[Item_Name],category,amount);
            wcUpdatePlayerInt(mailId,"item",0,"mailId","wc_Mail");
            wcUpdatePlayerInt(mailId,"unique",0,"mailId","wc_Mail");
            MenuReadMail(client,mailId);
            if(unique>0)  {
              UpdateUniqueState(unique, 0);
              UniqueOwner(unique, characterids[client][0]);
            }
        }
        else
          WCMessage(client,"no free slots",client);
      }


    }
   }
}

public GetItemMail(client,mailId)
{
  decl String:buffer[128];
  Format(buffer,128,"SELECT mailId, item, amount, `unique` FROM wc_Mail WHERE mailId = '%i'",mailId);

  SQL_TQuery(TSQL,TGetItemMail,buffer,client);
}

public InsertStringInString(String:orig[], String:dest[], destlen, freq)
{
    new origlen = strlen(orig);
    new count;
    new y;
    for(new x=0;x<origlen;x++)
    {
      if(count==freq)
      {
        dest[y]='\n';
        y++;
        count=0;
      }
      dest[y]=orig[x];
      y++;
      count++;
    }
    dest[y]=0;
}

public BuySpells(client,Handle:SpellsDB)
{
  decl String:title[255];
  new Handle:menu_buyspells=CreateMenu(MenuBuySpellsHandle);
  SetMenuExitBackButton(menu_buyspells,true);
  Format(title,sizeof(title),"%T","wc buy spells",client);
  SetMenuTitle(menu_buyspells,title);
  new level=GetLevel(client);
  KvRewind(SpellsDB);
  if (!KvGotoFirstSubKey(SpellsDB))
  {
    LogError("[WC] Error: failed to open Spells database");
  }
  decl String:buffer[5],String:name[255];
  do
  {
    KvGetSectionName(SpellsDB, buffer, sizeof(buffer));
    new spellid=StringToInt(buffer);
    KvGetString(SpellsDB, "name", name, sizeof(name), "0");
    new ilevel=KvGetNum(SpellsDB, "level", 0);
    if(ilevel>level)
      Format(name,sizeof(name),"%s (%T)",name,"low lvl",client);
    new foundspell=wcSearchSpell(client,spellid);
    if(foundspell==0)
      AddMenuItem(menu_buyspells,buffer,name);
   }  while (KvGotoNextKey(SpellsDB));
  new count=GetMenuItemCount(menu_buyspells);
  if(count==0)
   {
    Format(name,sizeof(name),"%T","no spell",client);
    AddMenuItem(menu_buyspells,name,name,ITEMDRAW_DISABLED);
   }
  DisplayMenu(menu_buyspells,client,MENU_TIME_FOREVER);
}

public MenuBuySpellsHandle(Handle:menu_buyspells,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[5];
    GetMenuItem(menu_buyspells,slot,buffer,sizeof(buffer));
    new spellid=StringToInt(buffer);
    SpellMenu(client,spellid);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ClassMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_buyspells);
}

public SpellMenu(client,spellid)
{
  new Handle:menu_spellmenu=CreateMenu(SpellMenuHandle);
  SetMenuExitBackButton(menu_spellmenu,true);
  decl String:title[128],String:line[255];
  new race=GetClass(client);
  wcGetSpellInfo(race,spellid,"name",line, client);
  Format(title,sizeof(title),"%T","spell menu",client,line);
  SetMenuTitle(menu_spellmenu,title);
  decl String:s_spellid[5];
  IntToString(spellid,s_spellid,sizeof(s_spellid));
  Format(line,sizeof(line),"%T","information",client);
  AddMenuItem(menu_spellmenu,s_spellid,line);
  Format(line,sizeof(line),"%T","buy spell",client);
  AddMenuItem(menu_spellmenu,s_spellid,line);
  DisplayMenu(menu_spellmenu,client,MENU_TIME_FOREVER);
}

public SpellMenuHandle(Handle:menu_spellmenu,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[5];
    GetMenuItem(menu_spellmenu,slot,buffer,sizeof(buffer));
    new spellid=StringToInt(buffer);
    switch(slot)
    {
      case 0:
      {
        new race=GetClass(client);
        if (race != 0)
          SpellInf(client, spellid, GetSpellsHandle(race), true);
      }
      case 1:
      {
        new race=GetClass(client);
        if (race != 0)
          BuySpell(client, spellid, GetSpellsHandle(race));
      }
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
    {
      new race=GetClass(client);
      if (race != 0)
        BuySpells(client, GetSpellsHandle(race));
    }
  if(action==MenuAction_End)
    CloseHandle(menu_spellmenu);
}


public SpellInf(client,spellid,Handle:SpellsDB,bool:buy)
{
   KvRewind(SpellsDB);
   decl String:info[255],String:title[255],String:text[1000],String:s_spellid[5];
   new Handle:menu_spellinf;
   if(buy)
    menu_spellinf=CreateMenu(MenuSpellInfHandle);
   else
    menu_spellinf=CreateMenu(MenuSpellInf2Handle);
   new tmp;
   IntToString(spellid,s_spellid,sizeof(s_spellid));
   SetMenuExitBackButton(menu_spellinf,true);
   KvJumpToKey(SpellsDB,s_spellid);
   decl String:name[255],String:transName[255];
   KvGetString(SpellsDB, "name", name, sizeof(name), "0");
   //KvGetString(SpellsDB, "name", transName, sizeof(transName), "0");
   Format(transName, sizeof(transName), "%s name", name);
   Format(transName, sizeof(transName), "%T", transName, client);
   Format(title,sizeof(title),"%T","wc spell inf",client,transName);
   SetMenuTitle(menu_spellinf,title);
   new level=GetLevel(client);
   new splevel=KvGetNum(SpellsDB, "level", 0);
   Format(info,sizeof(info),"%T","spell lvl",client,splevel,level);
   AddMenuItem(menu_spellinf,s_spellid,info,ITEMDRAW_DISABLED);
   tmp=KvGetNum(SpellsDB, "cost", 0);
   Format(info,sizeof(info),"%T","cost",client,tmp);
   AddMenuItem(menu_spellinf,info,info,ITEMDRAW_DISABLED);
   Format(info,sizeof(info),"%T","book spellid",client,spellid);
   AddMenuItem(menu_spellinf,info,info,ITEMDRAW_DISABLED);
   tmp=KvGetNum(SpellsDB, "mana", 0);
   if(tmp>0)
   {
    new Float:manainc=KvGetFloat(SpellsDB, "manainc", 0.0);
    if(manainc>0.0)
      tmp+=RoundToNearest(float(GetLevel(client))*manainc);
    Format(info,sizeof(info),"%T","mana usage",client,tmp);
    AddMenuItem(menu_spellinf,info,info,ITEMDRAW_DISABLED);
   }
   new Float:f_tmp=KvGetFloat(SpellsDB, "cooldown", 0.0);
   if(f_tmp>0.0)
   {
    Format(info,sizeof(info),"%T","cooldown time",client,f_tmp);
    AddMenuItem(menu_spellinf,info,info,ITEMDRAW_DISABLED);
   }
   f_tmp=KvGetFloat(SpellsDB, "cast", 0.0);
   if(f_tmp>0.0)
   {
    Format(info,sizeof(info),"%T","cast time",client,f_tmp);
    AddMenuItem(menu_spellinf,info,info,ITEMDRAW_DISABLED);
   }
   tmp=KvGetNum(SpellsDB, "threat", 0);
   if(tmp!=0)
   {
    Format(info,sizeof(info),"%T","threat spell",client,tmp);
    AddMenuItem(menu_spellinf,info,info,ITEMDRAW_DISABLED);
   }
   f_tmp=KvGetFloat(SpellsDB, "distance", 0.0);
   if(f_tmp>0.0)
   {
    Format(info,sizeof(info),"%T","spell distance",client,f_tmp);
    AddMenuItem(menu_spellinf,info,info,ITEMDRAW_DISABLED);
   }
   new Float:ftmp=KvGetFloat(SpellsDB, "multi", 0.0);
   if(ftmp>0.0)
   {
    tmp=RoundToNearest(ftmp*100.0);
    Format(info,sizeof(info),"%T","spell multi",client,tmp);
    AddMenuItem(menu_spellinf,info,info,ITEMDRAW_DISABLED);
   }
   Format(name,sizeof(name),"%s info",name);
   Format(text,sizeof(text),"%T",name,client);
   AddMenuItem(menu_spellinf,"info",text,ITEMDRAW_DISABLED);
   DisplayMenu(menu_spellinf,client,MENU_TIME_FOREVER);
}



public MenuSpellInfHandle(Handle:menu_spellinf,MenuAction:action,client,slot)
{
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
    {
      decl String:buffer[5];
      GetMenuItem(menu_spellinf,0,buffer,sizeof(buffer));
      new spellid=StringToInt(buffer);
      SpellMenu(client,spellid);
    }
  if(action==MenuAction_End)
    CloseHandle(menu_spellinf);
}

public MenuSpellInf2Handle(Handle:menu_spellinf,MenuAction:action,client,slot)
{
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
    {
      SpellBook(client);
    }
  if(action==MenuAction_End)
    CloseHandle(menu_spellinf);
}

public BuySpell(client,spellid,Handle:SpellsDB)
{
   decl String:info[255],String:title[255],String:line[255],String:s_spellid[5];
   new Handle:menu_buyspell=CreateMenu(MenuBuySpellHandle);
   IntToString(spellid,s_spellid,sizeof(s_spellid));
   SetMenuExitBackButton(menu_buyspell,true);
   KvRewind(SpellsDB);
   KvJumpToKey(SpellsDB,s_spellid);
   KvGetString(SpellsDB, "name", line, sizeof(line), "0");
   Format(title,sizeof(title),"%T","confirm spell",client,line);
   SetMenuTitle(menu_buyspell,title);
   new level=GetLevel(client);
   new splevel=KvGetNum(SpellsDB, "level", 0);
   Format(info,sizeof(info),"%T","spell lvl",client,splevel,level);
   AddMenuItem(menu_buyspell,s_spellid,info,ITEMDRAW_DISABLED);
   new cost=KvGetNum(SpellsDB, "cost", 0);
   new money=GetMoney(client);
   Format(info,sizeof(info),"%T","cost",client,cost);
   AddMenuItem(menu_buyspell,info,info,ITEMDRAW_DISABLED);
   Format(info,sizeof(info),"%T","current money",client,money);
   AddMenuItem(menu_buyspell,info,info,ITEMDRAW_DISABLED);
   money-=cost;
   Format(info,sizeof(info),"%T","will be money",client,money);
   AddMenuItem(menu_buyspell,info,info,ITEMDRAW_DISABLED);
   if(!KvGetNum(SpellsDB, "buyable", 0))  {
    Format(info,sizeof(info),"%T","auto learn",client);
    AddMenuItem(menu_buyspell,info,info,ITEMDRAW_DISABLED);
   }
   else   if(level<splevel)
   {
    Format(info,sizeof(info),"%T","low lvl",client);
    AddMenuItem(menu_buyspell,info,info,ITEMDRAW_DISABLED);
   }
   else
    if(money<0)
    {
      Format(info,sizeof(info),"%T","no money",client);
      AddMenuItem(menu_buyspell,info,info,ITEMDRAW_DISABLED);
    }
    else
    {
      Format(info,sizeof(info),"%T","press confirm",client);
      AddMenuItem(menu_buyspell,s_spellid,info);
    }
   DisplayMenu(menu_buyspell,client,MENU_TIME_FOREVER);
}

public MenuBuySpellHandle(Handle:menu_buyspell,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[255];
    GetMenuItem(menu_buyspell,slot,buffer,sizeof(buffer));
    new spellid=StringToInt(buffer);
    if(spellid>0)
    {
      decl String:s_cost[255];
      new race=GetClass(client);
      wcGetSpellInfo(race,spellid,"cost",s_cost);
      new String:name[255];
      wcGetSpellInfo(race,spellid,"name",name, client);
      new cost=StringToInt(s_cost);
      if(GetMoney(client)>=cost)
      {
        wcSetSpell(client,spellid);
        wcSetMoney(client,GetMoney(client)-cost);
        WCMessage(client,"bought spell",client,name);
        if (race != 0)
          BuySpells(client, GetSpellsHandle(race));
      }
      else
        WCMessage(client,"no money",client);
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
    {
      decl String:buffer[5];
      GetMenuItem(menu_buyspell,0,buffer,sizeof(buffer));
      new spellid=StringToInt(buffer);
      SpellMenu(client,spellid);
    }
  if(action==MenuAction_End)
    CloseHandle(menu_buyspell);
}

public SpellBar(client)
{
  new bool:pass=true;
  if(playerloaded[client]<1)
  {
    pass=false;
    WCMessage(client,"char not loaded",client);
  }
  if(pass)
  {
  new Handle:menu_spellbar=CreateMenu(SpellBarHandle),spellid;
  SetMenuExitBackButton(menu_spellbar,false);
  decl String:title[128],String:line[255],String:spellstr[128],String:line2[255];
  Format(title,sizeof(title),"%T","spellbar",client);
  SetMenuTitle(menu_spellbar,title);
  for (new i=0;i<=6;i++)
  {
    spellid=wcGetSpellBarSpell(client,i);
    if(spellid!=0)
    {
      new active = wcGetSpellInfoInt(GetClass(client),spellid,"active");
      if(active == 0)
        active = wcGetSpellInfoInt(GetClass(client),spellid,"controlled");
      if(active!=0)
      {
        wcGetSpellInfo(GetClass(client),spellid,"name",line, client);
        IntToString(spellid,spellstr,sizeof(spellstr));
        wcGetSpellInfo(GetClass(client),spellid,"mana",line2);
        new mana = GetSpellCost(client, spellid);
        Format(line,sizeof(line),"%s (%i)",line,mana);
        if(wcGetSpellInfoInt(GetClass(client), spellid, "autocast")!=0) {
          if(isAutoCastEnabled(client, spellid))
            Format(line, sizeof(line), "%s (%T)", line, "activated", client);
          else
            Format(line, sizeof(line), "%s (%T)", line, "deactivated", client);
        }
        AddMenuItem(menu_spellbar,spellstr,line);
      }
      else
      {
        wcGetSpellInfo(GetClass(client),spellid,"name",line, client);
        Format(line,sizeof(line),"%s (%T)",line,"passive",client);
        IntToString(spellid,spellstr,sizeof(spellstr));
        AddMenuItem(menu_spellbar,spellstr,line,ITEMDRAW_DISABLED);
      }
    }
    else
    {
      Format(line,sizeof(line),"%T","no spell",client);
      AddMenuItem(menu_spellbar,"0",line,ITEMDRAW_DISABLED);
    }

  }
  DisplayMenu(menu_spellbar,client,MENU_TIME_FOREVER);
  }
}

public SpellBarHandle(Handle:menu_spellbar,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    if(IsPlayerAlive(client))
    {
      new spellid=wcGetSpellBarSpell(client,slot);
      TryCast(client, spellid);
      if(!((GetClass(client)==5 && spellid == 8) || (GetClass(client)==7 && spellid == 11) || (GetClass(client)==8 && spellid == 9)))
        SpellBar(client);
      else
        SetReopenSpellbar(client,true);
    }
    else
      WCMessage(client,"you are dead",client);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ClassMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_spellbar);
}

public ManageSpells(client)
{
  new Handle:menu_managespells=CreateMenu(ManageSpellsHandle);
  new spellid;
  SetMenuExitBackButton(menu_managespells,true);
  decl String:title[128],String:line[255],String:spellstr[128];
  Format(title,sizeof(title),"%T","manage spells",client);
  SetMenuTitle(menu_managespells,title);
  for (new i=0;i<=6;i++)
  {
    spellid=wcGetSpellBarSpell(client,i);
    if(spellid!=0)
    {
      wcGetSpellInfo(GetClass(client),spellid,"name",line, client);
      IntToString(spellid,spellstr,sizeof(spellstr));
      AddMenuItem(menu_managespells,spellstr,line);
    }
    else
    {
      Format(line,sizeof(line),"%T","no spell",client);
      AddMenuItem(menu_managespells,"0",line);
    }

  }
  DisplayMenu(menu_managespells,client,MENU_TIME_FOREVER);
}

public ManageSpellsHandle(Handle:menu_managespells,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    ManageSpellSlot(client,slot);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ClassMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_managespells);
}

public ManageSpellSlot(client,slot)
{
  new Handle:menu_managespellslot=CreateMenu(ManageSpellHandle),spellid;
  SetMenuExitBackButton(menu_managespellslot,true);
  decl String:title[128],String:line[255],String:spellstr[128];
  Format(title,sizeof(title),"%T","manage spells",client);
  SetMenuTitle(menu_managespellslot,title);
  Format(spellstr,sizeof(spellstr),"\"%i\" \"0\"",slot);
  Format(line,sizeof(line),"%T","no spell",client);
  AddMenuItem(menu_managespellslot,spellstr,line);
  for (new i=1;i<=50;i++)
  {
    spellid=wcGetSpell(client,i);
    if(spellid!=0)
    {
      new active = wcGetSpellInfoInt(GetClass(client),spellid,"active");
      if(active == 0)
        active = wcGetSpellInfoInt(GetClass(client),spellid,"controlled");
      if(active!=0)
      {
        wcGetSpellInfo(GetClass(client),spellid,"name",line, client);
        Format(spellstr,sizeof(spellstr),"\"%i\" \"%i\"",slot,spellid);
        AddMenuItem(menu_managespellslot,spellstr,line);
      }
      else
      {
        wcGetSpellInfo(GetClass(client),spellid,"name",line, client);
        Format(line,sizeof(line),"%s (%T)",line,"passive",client);
        Format(spellstr,sizeof(spellstr),"\"%i\" \"%i\"",slot,spellid);
        AddMenuItem(menu_managespellslot,spellstr,line,ITEMDRAW_DISABLED);
      }
    }
  }
  DisplayMenu(menu_managespellslot,client,MENU_TIME_FOREVER);
}

public ManageSpellHandle(Handle:menu_managespellslot,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    new String:buffer[128],String:Sslotid[5],String:Sspellid[5];
    GetMenuItem(menu_managespellslot,slot,buffer,sizeof(buffer));
    new StringID=BreakString(buffer, Sslotid, sizeof(Sslotid));
    BreakString(buffer[StringID], Sspellid, sizeof(Sspellid));
    new slotid=StringToInt(Sslotid);
    new spellid=StringToInt(Sspellid);
    wcSetSpellBarSpell(client,slotid,spellid);
    ManageSpells(client);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ManageSpells(client);
  if(action==MenuAction_End)
    CloseHandle(menu_managespellslot);
}

public SpellBook(client)
{
  new Handle:menu_spellbook=CreateMenu(MenuSpellBookHandle),spellid;
  SetMenuExitBackButton(menu_spellbook,true);
  decl String:title[128],String:line[255],String:spellstr[128];
  Format(title,sizeof(title),"%T","wc spellbook2",client, GetLevel(client));
  SetMenuTitle(menu_spellbook,title);
  for (new i=1;i<=50;i++)
  {
    spellid=wcGetSpell(client,i);
    if(spellid!=0)
    {
      wcGetSpellInfo(GetClass(client),spellid,"name",line, client);
      IntToString(spellid,spellstr,sizeof(spellstr));
      AddMenuItem(menu_spellbook,spellstr,line);
    }
  }
  new count=GetMenuItemCount(menu_spellbook);
  if(count==0)
  {
    Format(line,sizeof(line),"%T","no spell",client);
    AddMenuItem(menu_spellbook,line,line,ITEMDRAW_DISABLED);
  }
  //Unlearned spells
  Format(line, sizeof(line), "%T", "unlearned spells line", client);
  AddMenuItem(menu_spellbook,line,line,ITEMDRAW_DISABLED);

  new Handle:SpellsDB = GetSpellsHandle(GetClass(client));
  KvRewind(SpellsDB);
  if (!KvGotoFirstSubKey(SpellsDB))
  {
    LogError("[WC] Error: failed to open Spells database");
  }
  decl String:buffer[5],String:name[255];
  count = 0;
  do
  {
    KvGetSectionName(SpellsDB, buffer, sizeof(buffer));
    spellid=StringToInt(buffer);
    if(wcSearchSpell(client, spellid) > 0)  continue;

    wcGetSpellInfo(GetClass(client) ,spellid,"name",name, client);
    new ilevel=KvGetNum(SpellsDB, "level", 0);

    Format(name,sizeof(name),"%s (%T)",name, "level", client ,ilevel);
    IntToString(spellid, spellstr, sizeof(spellstr));
    AddMenuItem(menu_spellbook, spellstr, name);
    count++;
  }  while (KvGotoNextKey(SpellsDB));
  if(count==0)
  {
    Format(name,sizeof(name),"%T","no spell",client);
    AddMenuItem(menu_spellbook,name,name,ITEMDRAW_DISABLED);
  }

  DisplayMenu(menu_spellbook,client,MENU_TIME_FOREVER);
}

public MenuSpellBookHandle(Handle:menu_spellbook,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    new String:buffer[128],spellid;
    GetMenuItem(menu_spellbook,slot,buffer,sizeof(buffer));
    spellid=StringToInt(buffer);
    new race=GetClass(client);
    if(race>0)
      SpellInf(client, spellid, GetSpellsHandle(race), false);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ClassMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_spellbook);
}

public InvSortMenu(client)
{
  if(!ArenaAllowMenu(client))  {
    WCMessage(client,"arena locked", client);
    return;
  }
  new Handle: menu = CreateMenu(InvSortMenuHandle);
  SetMenuExitBackButton(menu, true);
  decl String: title[128],
  String: line[256];
  Format(title, sizeof(title), "%T", "TrSorting", client);
  SetMenuTitle(menu, title);
  Format(line, sizeof(line), "%T", "TrSortable_NoSort", client);
  AddMenuItem(menu, "", line);

  Format(line, sizeof(line), "%T", "gear item", client);
  AddMenuItem(menu, "", line);

  Format(line, sizeof(line), "%T", "legendary item", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "epic item", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "blue item", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "green item", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "pvp epic item", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "pvp blue item", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "reagents", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "patterns for blacksmith", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "schemas for jewelcrafting", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "recipes for tailoring", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "patterns for leatherworking", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "formulas for enchanting", client);
  AddMenuItem(menu, "", line);
  Format(line, sizeof(line), "%T", "scroll of enchantment",client);
  AddMenuItem(menu,"",line);
  Format(line, sizeof(line), "%T", "inscription books", client);
  AddMenuItem(menu,"",line);
  Format(line,sizeof(line),"%T", "general usable items",client);
  AddMenuItem(menu,"",line);
  Format(line,sizeof(line),"%T","random boxes",client);
  AddMenuItem(menu,"",line);
  Format(line, sizeof(line), "%T", "glyphs", client);
  AddMenuItem(menu,"",line);
  DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public InvSortMenuHandle(Handle:menu,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    invSorting[client]=slot;
    RefreshInventory(client);
    InventoryMenu(client);
  }
  if(action==MenuAction_Cancel)
  if(slot==MenuCancel_ExitBack)
  ShopsMenu(client);
  if(action==MenuAction_End)
  CloseHandle(menu);
}

new g_ff_glyphSlot[65];
public ChooseGlyphSlot(client, slot, item[UniqueItem])  {
  g_ff_glyphSlot[client] = slot;
  new Handle:menu = CreateMenu(ChooseGlyphSlotHandle);
  SetMenuExitBackButton(menu, true);
  decl String:title[128],String:line[128];
  Format(title, sizeof(title), "%T", "wc choose glyph slot", client, item[Item_Name]);
  SetMenuTitle(menu, title);

  for(new i=0; i<GetGlyphSlots(client); i++) {
    decl slotItem[UniqueItem];
    if(GetGlyphFull(client, i, slotItem))
      Format(line, sizeof(line), "%T", "glyph slot", client, i+1, slotItem[Item_Name]);
    else
      Format(line, sizeof(line), "%T", "glyph slot", client, i+1, "Empty");
    AddMenuItem(menu, "equip", line);
  }
  if(GetGlyphSlots(client) < MAX_GLYPHS) {
    //Offer to buy one
    Format(line, sizeof(line), "%T", "buy glyph slot", client, 0.5*(GetGlyphSlots(client)+1), GetGlyphSlots(client), MAX_GLYPHS);
    if(GetGold(client) > 50+50*GetGlyphSlots(client))
      AddMenuItem(menu, "buy", line);
    else
      AddMenuItem(menu, "buy", line, ITEMDRAW_DISABLED);
  }
  DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public ChooseGlyphSlotHandle(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    //insert here
    new glyphSlot = slot;
    new invSlot = g_ff_glyphSlot[client];
    if(GetGlyphSlots(client) <= glyphSlot) {
      //Buy Slot
      SpendGold(client, 50*GetGlyphSlots(client)+50, "gold shop", "Bought Glyph Slot");
      SetGlyphSlots(client, GetGlyphSlots(client)+1);
    }
    //Place Glyph
    new item[UniqueItem];
    GetInvItem(item, client, invSlot);
    WCMessage(client, "equipped glyph", client, item[Item_Name]);
    GetGlyphOnStatic(client, glyphSlot, invSlot);
    InventoryMenu(client);


  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      InventoryMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}


public InventoryMenu(client)
{
	new Handle: menu_inventory = CreateMenu(InventoryMenuHandle);
	SetMenuExitBackButton(menu_inventory, true);
	decl String: title[128],
	String: line[128],
	String: Scategory[255];
	new category;
	Format(title, sizeof(title), "%T", "wc inventory menu", client, GetFilledInventory(client));
	SetMenuTitle(menu_inventory, title);
	Format(line, sizeof(line), "%T", "move empty slots", client);
	AddMenuItem(menu_inventory, "empty slots", line);
	Format(line, sizeof(line), "%T", "TrSorting", client);
	switch (invSorting[client])
        {
          case Sortable_NoSort:
          {
            Format(line, sizeof(line), "%s [%T]", line, "TrSortable_NoSort", client);
          }
          case Sortable_Gear:
          {
            Format(line, sizeof(line), "%s [%T]", line, "gear item", client);
          }
          case Sortable_Legendary:
          {
            Format(line, sizeof(line), "%s [%T]", line, "legendary item", client);
          }
          case Sortable_Epic:
          {
            Format(line, sizeof(line), "%s [%T]", line, "epic item", client);
          }
          case Sortable_Rare:
          {
            Format(line, sizeof(line), "%s [%T]", line, "blue item", client);
          }
          case Sortable_Green:
          {
            Format(line, sizeof(line), "%s [%T]", line, "green item", client);
          }
          case Sortable_PVPEpic:
          {
            Format(line, sizeof(line), "%s [%T]", line, "pvp epic item", client);
          }
          case Sortable_PVPRare:
          {
            Format(line, sizeof(line), "%s [%T]", line, "pvp blue item", client);
          }
          case Sortable_Reagents:
          {
            Format(line, sizeof(line), "%s [%T]", line, "reagents", client);
          }
          case Sortable_RPblacksmith:
          {
            Format(line, sizeof(line), "%s [%T]", line, "patterns for blacksmith", client);
          }
          case Sortable_RPjewelcrafting:
          {
            Format(line, sizeof(line), "%s [%T]", line, "schemas for jewelcrafting", client);
          }
          case Sortable_RPtailoring:
          {
            Format(line, sizeof(line), "%s [%T]", line, "recipes for tailoring", client);
          }
          case Sortable_RPleatherworking:
          {
            Format(line, sizeof(line), "%s [%T]", line, "patterns for leatherworking", client);
          }
          case Sortable_RPenchanting:
          {
            Format(line, sizeof(line), "%s [%T]", line, "formulas for enchanting", client);
          }
          case Sortable_Scrolls:
          {
            Format(line, sizeof(line), "%T", "scroll of enchantment",client);
          }
          case Sortable_Inscription:
          {
            Format(line, sizeof(line), "%T", "inscription books",client);
          }
          case Sortable_General:
          {
            Format(line,sizeof(line),"%T", "general usable items",client);
          }
          case Sortable_Randomboxes:
          {
            Format(line,sizeof(line),"%T","random boxes",client);
          }
          case Sortable_Glyphs:
          {
            Format(line, sizeof(line), "%T", "glyphs", client);
          }
        }
	AddMenuItem(menu_inventory, "empty slots", line);
	new String:sslot[8];
	new ix=0;
	for (new x = 1; x <= 100; x++)
	{
                new item[UniqueItem];
                GetInvItem(item, client, x);
                category = item[Item_category];

		if (invSorting[client] > 0)
                {
                  new quality = item[Item_quality];
                  switch (invSorting[client])
                  {
                    /*case Sortable_NoSort:
                    {
                  }*/
                    case Sortable_Gear:
                    {
                      if(category < 10 || (category > 23 && category != 30))  continue;
                    }
                    case Sortable_Legendary:
                    {
                      if (_:SortableRealNS_Legendary != quality) continue;
                    }
                    case Sortable_Epic:
                    {
                      if (_:SortableRealNS_Epic != quality) continue;
                    }
                    case Sortable_Rare:
                    {
                      if (_:SortableRealNS_Rare != quality) continue;
                    }
                    case Sortable_Green:
                    {
                      if (_:SortableRealNS_Green != quality) continue;
                    }
                    case Sortable_PVPEpic:
                    {
                      if (_:SortableRealNS_PVPEpic != quality) continue;
                    }
                    case Sortable_PVPRare:
                    {
                      if (_:SortableRealNS_PVPRare != quality) continue;
                    }
                    case Sortable_Reagents:
                    {
                      if (_:SortableRealCS_Reagents != category) continue;
                    }
                    case Sortable_RPblacksmith:
                    {
                      if (_:SortableRealCS_RPblacksmith != category) continue;
                    }
                    case Sortable_RPjewelcrafting:
                    {
                      if (_:SortableRealCS_RPjewelcrafting != category) continue;
                    }
                    case Sortable_RPtailoring:
                    {
                      if (_:SortableRealCS_RPtailoring != category) continue;
                    }
                    case Sortable_RPleatherworking:
                    {
                      if (_:SortableRealCS_RPleatherworking != category) continue;
                    }
                    case Sortable_RPenchanting:
                    {
                      if (_:SortableRealCS_RPenchanting != category) continue;
                    }
                    case Sortable_Scrolls:
                    {
                      if (_:SortableRealCS_Scrolls != category) continue;
                    }
                    case Sortable_Inscription:
                    {
                      if (_:SortableRealCS_Inscription != category) continue;
                    }

                    case Sortable_General:
                    {
                      if (_:SortableRealCS_General != category) continue;
                    }
                    case Sortable_Randomboxes:
                    {
                      if (_:SortableRealCS_Randomboxes != category) continue;
                    }				
                    case Sortable_Glyphs:
                    {
                      if(_:SortableRealCS_Glyphs != category) continue;
                    }
                  }
                }
		IdToCategory(category, Scategory);
		if (StrEqual(Scategory, "no category"))
		  Format(Scategory, sizeof(Scategory), "%T", "Item No Category", client);
		if (item[Item_defId] == 0) {
                  Format(item[Item_Name], 50, "%T", "empty", client);
                }
		new amount = GetSlotItemAmount(client, x);
		decl String: buffer[255];
		if (amount > 0)
		{
			if (category > 9 && category < 24)  {
				Format(buffer, sizeof(buffer), "%T", "Item Level", client, item[Item_level]);
				Format(line, sizeof(line), "%s (%s) x%i %s", item[Item_Name], Scategory, amount, buffer);
				}
			else
				Format(line, sizeof(line), "%s (%s) x%i", item[Item_Name], Scategory, amount);
		}
		else
			if (amount == -1)  {
				Format(buffer, sizeof(buffer), "%T", "Item Soulbound", client);
				Format(line, sizeof(line), "%s (%s) (%s)", item[Item_Name], Scategory, buffer);
                }
			else  {
				Format(buffer, sizeof(buffer), "%T", "Item Player Bound", client);
				Format(line, sizeof(line), "%s (%s) (%s)", item[Item_Name], Scategory, buffer);
                }
		IntToString(x, sslot, 8);
		AddMenuItem(menu_inventory, sslot, line);
		ix++;
	}
	if(!ix)
	{
		Format(line, sizeof(line), "%T", "no items", client);
		AddMenuItem(menu_inventory, sslot, line, ITEMDRAW_DISABLED);
	}
	DisplayMenu(menu_inventory, client, MENU_TIME_FOREVER);
}

public InventoryMenuHandle(Handle:menu_inventory,MenuAction:action,client,slot)
{
	if(action==MenuAction_Select)
	{
		if (slot==0)
		{
			RefreshInventory(client);
			InventoryMenu(client);			
		}
		else
		{
			if(slot>1)
			{
				if(invSorting[client]>0)
				{
					new String:sslot[8];
					GetMenuItem(menu_inventory,slot,sslot,8);
					InvItemMenu(client,StringToInt(sslot));
				}else{ InvItemMenu(client,slot-1); }
			}
			else
			{
				InvSortMenu(client);
			}
		}
	}
	if(action==MenuAction_Cancel)
		if(slot==MenuCancel_ExitBack)
			ShopsMenu(client);
	if(action==MenuAction_End)
		CloseHandle(menu_inventory);
}

public ChangeCharacterMenu(client)
{
  new Handle:menu_characters=CreateMenu(ChangeCharacterMenuHandle);
  SetMenuExitBackButton(menu_characters,true);
  decl level,String:line[255],String:charID[255];
  Format(line,sizeof(line),"%T","wc character menu",client);
  SetMenuTitle(menu_characters,line);
  new y=0;
  for (new x=1;x<=15;x++)
  if(characterids[client][x]!=0)
  {
    level=charactersdata[client][x][2];
    new id=charactersdata[client][x][3];
    wcClassToName(client,id,line,sizeof(line));
    Format(line,sizeof(line),"%T","character name w id",client,level,line,
      characterids[client][x]);
    new mail=charactersdata[client][x][1];
    if(mail>0)
    {
      Format(line,sizeof(line),"%s %T",line,"mail exists",client,mail);
    }
    /*else*/
      /*Format(line,sizeof(line),"%s %T",line,"no new mail",client);*/
    IntToString(x,charID,sizeof(charID));
    AddMenuItem(menu_characters,charID,line);
    y++;
  }
  Format(line,sizeof(line),"%T","insert new char",client);
  if (y<15)
    InsertMenuItem(menu_characters,0,"new",line);
  else
    InsertMenuItem(menu_characters,0,"new",line,ITEMDRAW_DISABLED);
  DisplayMenu(menu_characters,client,MENU_TIME_FOREVER);
}

public ChangeCharacterMenuHandle(Handle:menu_characters,MenuAction:action,client,slot)
{

  if(action==MenuAction_Select)
  {
    if (slot==0)
    {
      if(playerloaded[client]==1)
        wcSavePlayerData(client, false);
      ForcePlayerSuicide(client);
      ClearCharacterData(client);
      playerloaded[client]=0;
      InstallCharacter(client);
    }
    else
    {
      decl String:buffer[255];
      GetMenuItem(menu_characters,slot,buffer,sizeof(buffer));
      new charID=StringToInt(buffer);
      if (charID >= 1)
      {
         wcLoadDelCharacter(client,charID);
      }
      else
        LogError("WC: Error, could not find index in loading another character");
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ClassMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_characters);
}

public wcLoadDelCharacter(client,idc)
{
   new Handle:menu_loaddel=CreateMenu(LoadDelMenuHandle);
   SetMenuExitBackButton(menu_loaddel,true);
   decl String:title[255],String:line[255];
   wcClassToName(client,charactersdata[client][idc][3],title,sizeof(title));
   Format(title,sizeof(title),"%T","wc loaddel menu",client,title,charactersdata[client][idc][2]);
   decl String:ScharID[10];
   IntToString(idc,ScharID,sizeof(ScharID));
   SetMenuTitle(menu_loaddel,title);
   Format(line,sizeof(line),"%T","load character",client);
   AddMenuItem(menu_loaddel,ScharID,line);
   Format(line,sizeof(line),"%T","del character",client);
   AddMenuItem(menu_loaddel,ScharID,line);
   DisplayMenu(menu_loaddel,client,MENU_TIME_FOREVER);
}

public LoadDelMenuHandle(Handle:menu_loaddel,MenuAction:action,client,slot)
{
  if((client>0) && (client <= GetMaxClients()))
  {
  new String:buffer[255];
  GetMenuItem(menu_loaddel,0,buffer,sizeof(buffer));
  new idc=StringToInt(buffer);
  if((action==MenuAction_Select) && (characterids[client][idc]>0))
  {
   switch(slot)
   {
      case 0:
      {
        if(characterids[client][idc]==characterids[client][0])
        {
          WCMessage(client,"can not load same",client);
          ChangeCharacterMenu(client);
        }
        else
        {
        ForcePlayerSuicide(client);
        if(playerloaded[client]==1)
          wcSavePlayerData(client, false);
        ClearCharacterData(client);
        playerloaded[client]=0;
        characterids[client][0]=characterids[client][idc];
        wcSetCharacter(client,0,characterids[client][idc]);
        GetCharacterData(client);
        }
      }
      case 1:
      {
         //ConfirmDelCharacter(client,idc);
        AskDeleteConfirmation(client, "ConfirmDelCharacter", idc,
          "You are about to delete one of your characters!");
      }
   }
  }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ChangeCharacterMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_loaddel);
}


/**
 * Using delete confirmation:
 * call: AskDeleteConfirmation(client, String:callback[], argument,
 * String:text[])
 */

new g_ff_delconfirmation[65];
new String:g_ff_delCallback[65][255];
new g_ff_delArgument[65];

stock ResetDelConfirmation(client)  {
  g_ff_delconfirmation[client] = 0;
}

stock bool:IsWaitingDelConfirmation(client)  {
  if(g_ff_delconfirmation[client] <= 0)
    return false;
  return true;
}

stock DelConfirmed(client)  {
  ResetDelConfirmation(client);

  //Call function
  new Function:func = GetFunctionByName(INVALID_HANDLE, g_ff_delCallback[client]);
  Call_StartFunction(INVALID_HANDLE, func);
  Call_PushCell(client);
  Call_PushCell(g_ff_delArgument[client]);
  if(Call_Finish() != SP_ERROR_NONE)  {
    decl String:err[100];
    Format(err,100,"Could not callback the function: %s",g_ff_delCallback[client]);
    Logger_log(LogType:LT_file, LogLevel:LL_error, "Menus", err);
  }

}

stock AskDeleteConfirmation(client, String:callback[], argument, String:text[])  {
  ResetDelConfirmation(client);
  g_ff_delconfirmation[client] = 1;
  g_ff_delArgument[client] = argument;
  strcopy(g_ff_delCallback[client], 255, callback);
  _displayDeleteMenu(client, text);
}

stock _displayDeleteMenu(client, String:text[]) {
  new Handle:menu = CreateMenu(DeleteMenuHandle);
  decl String:buffer[80];
  SetMenuExitBackButton(menu, false);
  Format(buffer,sizeof(buffer), "%T", "confirm deletion", client);
  SetMenuTitle(menu, buffer);
  AddMenuItem(menu,"", "-----------------------------", ITEMDRAW_DISABLED);
  AddMenuItem(menu, "", text, ITEMDRAW_DISABLED);
  AddMenuItem(menu, "", "-----------------------------", ITEMDRAW_DISABLED);
  Format(buffer,sizeof(buffer), "%T", "type confirm", client);
  AddMenuItem(menu, "", buffer, ITEMDRAW_DISABLED);
  AddMenuItem(menu, "", "-----------------------------", ITEMDRAW_DISABLED);
  DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public DeleteMenuHandle(Handle:menu,MenuAction:action,client,slot)
{
  if(action==MenuAction_End)  {
    if(client > 0 && client < 65) {
      ResetDelConfirmation(client);
    }
    CloseHandle(menu);
  }
}


public ConfirmDelCharacter(client,idc)
{
   new Handle:menu_confirmdel=CreateMenu(ConfirmDelMenuHandle);
   SetMenuExitBackButton(menu_confirmdel,true);
   decl String:title[255],String:buffer[255],String:line[255];
   new charID=GetCharacter(client,idc);
   Format(buffer,sizeof(buffer),"SELECT level, class FROM wc_Characters WHERE CharacterId = '%i'",charID);
   new Handle:query=SQL_Query(hSQL,buffer);
   if (query)
   {
      SQL_Rewind(query);
      new bool:fetch=SQL_FetchRow(query);
      if(fetch)
      {
        new level=GetSQLDataInt(query,"level");
        new id=GetSQLDataInt(query,"class");
        wcClassToName(client,id,title,sizeof(title));
        Format(title,sizeof(title),"%T","wc confirmdel menu",client,title,level);
      }
   }
   if (query == INVALID_HANDLE)  {
      WCMessage(client,"failed loading",client);
      LogError("Could not query character %i for deletion",idc);
      return;
   }
   CloseHandle(query);
   decl String:ScharID[10];
   IntToString(idc,ScharID,sizeof(ScharID));
   SetMenuTitle(menu_confirmdel,title);
   Format(line,sizeof(line),"%T","del attention1",client);
   AddMenuItem(menu_confirmdel,ScharID,line,ITEMDRAW_DISABLED);
   Format(line,sizeof(line),"%T","del attention2",client);
   AddMenuItem(menu_confirmdel,ScharID,line,ITEMDRAW_DISABLED);
   Format(line,sizeof(line),"%T","del attention3",client);
   AddMenuItem(menu_confirmdel,ScharID,line);
   DisplayMenu(menu_confirmdel,client,MENU_TIME_FOREVER);
}

public ConfirmDelMenuHandle(Handle:menu_confirmdel,MenuAction:action,client,slot)
{
  new String:buffer[255];
  GetMenuItem(menu_confirmdel,0,buffer,sizeof(buffer));
  new id=StringToInt(buffer);
  if((action==MenuAction_Select) && (id>0))
  {
   switch(slot)
   {
      case 2:
      {
         new charid=GetCharacter(client,id);
         if(charid==GetCharacter(client,0))
         {
            WCMessage(client,"character active",client);
         }
         else
         {
            wcSetCharacter(client,id,0);
            wcDelCharacter(charid);
            WCMessage(client,"character removed",client);
         }
      }
   }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ChangeCharacterMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_confirmdel);
}

public CharacterMenu(client)
{
  new Handle:menu_character=CreateMenu(CharacterMenuHandle);
  SetMenuExitBackButton(menu_character,true);
  decl String:title[128],String:line[128],String:Sitem[255],String:Scategory[255];
  Format(title,sizeof(title),"%T","wc character menu",client);
  SetMenuTitle(menu_character,title);

  // items
  new id = client;
  new item[UniqueItem];
  GetCharItem(item, id, 22);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Right Hand: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"22",line); // 1

  GetCharItem(item, id, 18);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Left Hand: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"18",line); // 2

  GetCharItem(item, id, 14);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Helmet: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"14",line); // 3

  GetCharItem(item, id, 16);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Neck: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"16",line); // 4

  GetCharItem(item, id, 19);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Shoulders: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"19",line); // 5

  GetCharItem(item, id, 10);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Back: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"10",line); // 6

  GetCharItem(item, id, 11);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Chest: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"11",line); // 7

  GetCharItem(item, id, 23);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Wrist: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"23",line); // 8
  GetCharItem(item, id, 13);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Hands: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"13",line); // 9

  GetCharItem(item, id, 21);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Waist: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"21",line); // 10

  GetCharItem(item, id, 15);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Legs: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"15",line); // 11

  GetCharItem(item, id, 12);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Boots: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"12",line); // 12

  GetCharItem(item, id, 17);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Ring: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"17",line); // 13

  GetCharItem(item, id, 20);
  IdToCategory(item[Item_category],Scategory);
  if((item[Item_defId]==0) && (item[Item_id]==0))
  Format(Sitem,sizeof(Sitem),"%T","empty",client);
  else
  Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
  Format(line,sizeof(line),"Trinket: %s (%s)",Sitem,Scategory);
  AddMenuItem(menu_character,"20",line); // 14

  for(new i=0;i<MAX_GLYPHS;i++) {
    new bool:has=false;
    if(GetGlyphFull(client, i, item)) {
      has=true;
      Format(line, sizeof(line), "Glyph Slot %d: %s", i+1, item[Item_Name]);
    }
    else
      Format(line, sizeof(line), "Glyph Slot %d: Empty", i+1);
    decl String:counter[5];
    Format(counter, 5, "%d", 30+i);
    if(has)
      AddMenuItem(menu_character, counter, line);
    else
      AddMenuItem(menu_character, counter, line, ITEMDRAW_DISABLED);

  }
  DisplayMenu(menu_character,client,MENU_TIME_FOREVER);
}


public CharacterMenuHandle(Handle:menu_character,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    /*if (slot==0)*/
    /*{*/
      /*ChangeCharacterMenu(client);*/
    /*}*/
    /*else*/
    {
      new String:buffer[255];
      GetMenuItem(menu_character,slot,buffer,sizeof(buffer));
      new ID=StringToInt(buffer);
      CharItemOpt(client,ID);
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      ShopsMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_character);
}

public CharItemOpt(client,ID)
{
  new Handle:menu_charitem=CreateMenu(CharItemMenuHandle);
  SetMenuExitBackButton(menu_charitem,true);
  decl String:title[255],String:line[255],String:Scategory[255],String:IDstr[10];
  new item[UniqueItem];
  GetCharItem(item,client,ID);
  IdToCategory(ID,Scategory);
  IntToString(ID,IDstr,sizeof(IDstr));
  if(item[Item_defId]==0)
    Format(title,sizeof(title),"%T","empty",client);
  else
    Format(title,sizeof(title),"%T","charitem menu",client,item[Item_Name],Scategory);
  SetMenuTitle(menu_charitem,title);
  if (item[Item_defId]==0)
  {
    Format(line,sizeof(line),"%T","put into inventory",client,0);
    AddMenuItem(menu_charitem,IDstr,line,ITEMDRAW_DISABLED);
  }
  else
  {
    Format(line,sizeof(line),"%T","put into inventory",client,0);
    AddMenuItem(menu_charitem,IDstr,line);
  }
  if (item[Item_defId]==0)
  {
    Format(line,sizeof(line),"%T","information",client);
    AddMenuItem(menu_charitem,IDstr,line,ITEMDRAW_DISABLED);
  }
  else
  {
    Format(line,sizeof(line),"%T","information",client);
    AddMenuItem(menu_charitem,IDstr,line);
  }
  DisplayMenu(menu_charitem,client,MENU_TIME_FOREVER);
}

public CharItemMenuHandle(Handle:menu_charitem,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    new String:buffer[255];
    GetMenuItem(menu_charitem,slot,buffer,sizeof(buffer));
    new ID=StringToInt(buffer);
    switch(slot)
    {
      case 0:
      {
        new x=GetFilledInventory(client);
        if ((100-x)>=2)
        {
          PutIntoInventory(client,ID);
          CharacterMenu(client);
        }
        else
        {
          WCMessage(client,"low free slots, clear your inventory.",client,x);
          CharacterMenu(client);
        }
      }
      case 1:
      {
        lastMenu[client]=4;
        lastMenu2[client]=ID;
        new item[UniqueItem];
        GetCharItem(item, client, ID);
        ItemInfMenu(client,item,false);
      }
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      CharacterMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_charitem);
}

public InvItemMenu(client,slot)
{
  if( (slot>100) || (slot <= 0) ) {
    InventoryMenu(client);
    return;
  }
  lastMenu2[client]=slot;
  new Handle:menu_invitem=CreateMenu(InvItemMenuHandle);
  SetMenuExitBackButton(menu_invitem,true);
  decl String:title[255],String:line[255],String:Scategory[255],String:slotname[5];
  new item[UniqueItem];

  GetInvItem(item, client, slot);
  new cat=item[Item_category];
  //DebugItem(item);  //For testing
  //new uniqueId = item[Item_id];
  IdToCategory(cat,Scategory);
  if (StrEqual(Scategory, "no category"))
    Format(Scategory, sizeof(Scategory), "%T", "Item No Category", client);
  new iamount=GetSlotItemAmount(client,slot);
  switch(iamount) {
    case -1:  Format(title,sizeof(title),"%T","wc invitem menu noamount",client,slot,item[Item_Name],Scategory);
    case -2:  Format(title,sizeof(title),"%T","wc invitem menu pl bound",client,slot,item[Item_Name],Scategory);
    default:  Format(title,sizeof(title),"%T","wc invitem menu",client,slot,item[Item_Name],Scategory,iamount);
  }
  SetMenuTitle(menu_invitem,title);
  IntToString(slot,slotname,sizeof(slotname));
  if ( (item[Item_defId]==0) && (item[Item_id]==0))
  {
    Format(line,sizeof(line),"%T","sell item",client,0);
    AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);
  }
  else
  {
    new amount=RoundToNearest(float(RoundToNearest(float(item[Item_cost])*SELL_PERCENT))*float(iamount));
    if(amount<0)
      amount*=-1;
    Format(line,sizeof(line),"%T","sell item",client,amount);
    AddMenuItem(menu_invitem,slotname,line);
  }
  new bool:dEnch=false;
  if ( (item[Item_defId]==0) && (item[Item_id]==0))
  {
    Format(line,sizeof(line),"%T","get item on",client, 0.0);
    AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);
  }
  else
  {
    if ((cat>9) && (cat <24))
    {
      dEnch=CanDisenchant(cat);
      new equippeditem[UniqueItem];
      GetCharItem(equippeditem,client,cat);
      new Float:oldrating=GetItemRating(equippeditem);
      new Float:ratingdiff=GetItemRating(item);
      if(oldrating>0.0)
      {
        ratingdiff-=oldrating;
      }
      Format(line,sizeof(line),"%T","get item on",client,ratingdiff);
      AddMenuItem(menu_invitem,slotname,line);

    }
    else if(cat == 30)  {
      if(HasExpansion(client, Expansion:E_CATA))  {
        Format(line, sizeof(line), "%T", "get glyph on", client);
        AddMenuItem(menu_invitem,slotname,line);
      }
      else  {
        Format(line, sizeof(line), "%T (%T)", "get glyph on", client, "cata expansion required", client);
        AddMenuItem(menu_invitem,slotname, line, ITEMDRAW_DISABLED);
      }
    }
    else
    {
      Format(line,sizeof(line),"%T","use item",client);
      if(item[Item_use]!=0)
        AddMenuItem(menu_invitem,slotname,line);
      else
        AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);
    }
  }
  if ( (item[Item_defId]==0) && (item[Item_id]==0))
  {
    Format(line,sizeof(line),"%T","information",client);
    AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);
  }
  else
  {
    Format(line,sizeof(line),"%T","information",client);
    AddMenuItem(menu_invitem,slotname,line);
  }
  new max=GetItemMaxAmount(item[Item_defId]);
  if (max==0)
  {
    Format(line,sizeof(line),"%T","merge item",client,max);
    AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);
  }
  else
  {
    Format(line,sizeof(line),"%T","merge item",client,max);
    AddMenuItem(menu_invitem,slotname,line);
  }
  if ((max>0) && (iamount>0) && (wcCheckFreeSlot(client)>0))
  {
    Format(line,sizeof(line),"%T","split item",client);
    AddMenuItem(menu_invitem,slotname,line);
  }
  else
  {
    Format(line,sizeof(line),"%T","split item",client);
    AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);
  }

  Format(line, sizeof(line), "%T", "create auction item", client);
  if(iamount>0) {
    AddMenuItem(menu_invitem,slotname,line);
  }
  else
    AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);
  Format(line, sizeof(line), "%T", "create mail item", client);
  if(iamount>0) {
    AddMenuItem(menu_invitem,slotname,line);
  }
  else
    AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);

  //No disenchant of shop items
  if(dEnch && item[Item_quality] == 0)  {
    dEnch=false;
  }
  if(dEnch) {
    new enchanting = 0;
    for(new i=1;i<=2;i++) {
      if(GetProfession(client, i) == 8) {
        enchanting = GetProfessionSk(client, i);
      }
    }
    new Float:chance = GetDESkillUp(enchanting, item);
    Format(line,sizeof(line),"%T","disenchant w warning+chance",client, RoundToNearest(chance * 100));
    AddMenuItem(menu_invitem,slotname,line);
  }
  else
  {
    Format(line,sizeof(line),"%T","disenchant w warning",client);
    AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);
  }

  if ( (item[Item_defId]<=1) || (item[Item_id]>0))
  {
    Format(line,sizeof(line),"%T","sell all",client,0);
    AddMenuItem(menu_invitem,slotname,line,ITEMDRAW_DISABLED);
  }
  else
  {

    new has = GetAmountOfItems(client,item[Item_defId]);
    new amount=RoundToNearest(float(item[Item_cost])*SELL_PERCENT)*has;
    Format(line,sizeof(line),"%T","sell all",client,amount);
    AddMenuItem(menu_invitem,slotname,line);
  }


  DisplayMenu(menu_invitem,client,MENU_TIME_FOREVER);
}

public InvItemMenuHandle(Handle:menu_invitem,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    new String:buffer[255];
    switch(slot)
    {
      case 0:
      {
        GetMenuItem(menu_invitem,slot,buffer,sizeof(buffer));
        SellItem(client,StringToInt(buffer));
      }
      case 1:
      {
        GetMenuItem(menu_invitem,slot,buffer,sizeof(buffer));
        new islot=StringToInt(buffer);
        UseItem(client, islot);
      }
      case 2:
      {
        GetMenuItem(menu_invitem,slot,buffer,sizeof(buffer));
        new slot2=StringToInt(buffer);
        new item[UniqueItem];
        GetInvItem(item, client, slot2);
        lastMenu[client]=5;
        ItemInfMenu(client,item,false);
      }
      case 3:
      {
        GetMenuItem(menu_invitem,slot,buffer,sizeof(buffer));
        MergeItem(client,StringToInt(buffer));
        InventoryMenu(client);
      }
      case 4:
      {
        GetMenuItem(menu_invitem,slot,buffer,sizeof(buffer));
        WCMessage(client,"write for split",client);
        InventoryMenu(client);
        g_split_time[client]=GetEngineTime()+20.0;
        g_split_item[client]=StringToInt(buffer);
      }
      case 7:
      {
        GetMenuItem(menu_invitem,slot,buffer,sizeof(buffer));
        new slot2=StringToInt(buffer);
        new item[UniqueItem];
        GetInvItem(item, client, slot2);
        if(CanDisenchant(item[Item_category])) {
          TriggerDisEnch(client, item, slot2);
          InventoryMenu(client);
        }
        else  {
          WCMessage(client,"no disenchant", client);
        }
        InventoryMenu(client);
      }

      case 5:
      {
        GetMenuItem(menu_invitem,slot,buffer,sizeof(buffer));
        new id = StringToInt(buffer);

        RefreshAuctionData(client);
        ResetPack(g_auction_pack[client]);
        WritePackCell(g_auction_pack[client],id);
        g_auction_pack_amount[client]=1;
        MenuSendAuction(client);
      }
      case 6:
      {
        GetMenuItem(menu_invitem,slot,buffer,sizeof(buffer));
        new id = StringToInt(buffer);

        RefreshMailData(client);
        g_mail_slot[client]=id;

        MenuSendMail(client);
      }
      case 8:
      {
        GetMenuItem(menu_invitem,slot,buffer,sizeof(buffer));
        SellItem(client,StringToInt(buffer),true);
      }
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      InventoryMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_invitem);
}

new bool:g_ffSellItem[65];
stock SellItem(client,slot, bool:all=false)
{
  g_ffSellItem[client] = all;
  new Handle:menu_sellitem=CreateMenu(SellItemMenuHandle);
  SetMenuExitBackButton(menu_sellitem,true);
  decl String:title[255],String:line[255],String:Scategory[255],String:slotname[255];
  new item[UniqueItem];
  GetInvItem(item, client,slot);
  new itemam=GetSlotItemAmount(client,slot);
  if(itemam==-1)
    itemam=1;
  IdToCategory(item[Item_category], Scategory);
  Format(title,sizeof(title),"%T","wc sellitem menu",client,slot,item[Item_Name],Scategory,itemam);
  SetMenuTitle(menu_sellitem,title);
  IntToString(slot,slotname,sizeof(slotname));
  if ((item[Item_id]==0) && (item[Item_defId]==0))
  {
    Format(line,sizeof(line),"%T","sell item",client,0);
    AddMenuItem(menu_sellitem,"0",line,ITEMDRAW_DISABLED);
  }
  else
  {

    new amount = GetSlotItemAmount(client, slot);
    if (amount<0)
      amount*=-1;
    if(all) {
      amount = GetAmountOfItems(client,item[Item_defId]);
    }

    amount=RoundToNearest(float(item[Item_cost]*amount)*SELL_PERCENT);
    if(all) {
      Format(line,sizeof(line),"%T","sell all items confirm",client,amount);
    }
    else
      Format(line,sizeof(line),"%T","sell item confirm",client,amount);
    AddMenuItem(menu_sellitem,slotname,line);
  }
  DisplayMenu(menu_sellitem,client,MENU_TIME_FOREVER);
}

public SellItemMenuHandle(Handle:menu_sellitem,MenuAction:action,client,slot)
{
  new String:buffer[255];
  GetMenuItem(menu_sellitem,0,buffer,sizeof(buffer));
  new itemslot=StringToInt(buffer);
  if(action==MenuAction_Select)
  {
    new all = g_ffSellItem[client];
    new item[UniqueItem];
    GetInvItem(item, client, itemslot);
    if ((item[Item_id]==0) && (item[Item_defId]==0))
    {
      WCMessage(client,"cant sell",client);
    }
    else  if(!all)
    {
      new amount=RoundToNearest(float(item[Item_cost])*SELL_PERCENT)*GetSlotItemAmount(client,itemslot);
      if (amount<0)
        amount*=-1;
      if (amount>=0)
      {
        ItemSold(client,itemslot,amount);
        InventoryMenu(client);
      }
      else
      {
        WCMessage(client,"cant sell",client);
      }
    }
    else  {
      new  amount = GetAmountOfItems(client,item[Item_defId]);
      amount=RoundToNearest(float(item[Item_cost])*SELL_PERCENT)*amount;

      AllItemsSold(client, item[Item_defId], amount);
      InventoryMenu(client);
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      InvItemMenu(client,itemslot);
  if(action==MenuAction_End)
    CloseHandle(menu_sellitem);
}
new bool:g_ff_shop_autoequip[65];
_enableAutoEquip(client)  {
  g_ff_shop_autoequip[client]=true;
}
getAutoEquip(client)  {
  return g_ff_shop_autoequip[client];
}
public CategoryMenu(client)
{
   new Handle:menu_category=CreateMenu(CategoryHandle);
   decl String:buffer[80];
   SetMenuExitBackButton(menu_category,true);
   Format(buffer, sizeof(buffer), "%T", "select category", client);
   SetMenuTitle(menu_category,buffer);
   //Screw that overcomplicated shop menu
   /*AddMenuItem(menu_category,"22","Weapons");*/
   /*AddMenuItem(menu_category,"18","Shields");*/
   /*AddMenuItem(menu_category,"11","Chests");*/
   /*AddMenuItem(menu_category,"10","Backs");*/
   /*AddMenuItem(menu_category,"12","Feet");*/
   /*AddMenuItem(menu_category,"13","Hands");*/
   /*AddMenuItem(menu_category,"14","Helmets");*/
   /*AddMenuItem(menu_category,"15","Legs");*/
   /*AddMenuItem(menu_category,"16","Necks");*/
   /*AddMenuItem(menu_category,"17","Rings");*/
   /*AddMenuItem(menu_category,"19","Shoulders");*/
   /*AddMenuItem(menu_category,"20","Trinkets");*/
   /*AddMenuItem(menu_category,"21","Waists");*/
   /*AddMenuItem(menu_category,"23","Wrists");*/
   //Add person's class item first!
   if(GetClass(client)>0) {
      decl String:sClass[50];
      Format(sClass,sizeof(sClass),"class_%i",GetClass(client));
      Format(sClass,sizeof(sClass),"%T %T",sClass,client, "for you", client);
      decl String:sCat[10];
      Format(sCat,sizeof(sCat),"%i",100+GetClass(client));
      AddMenuItem(menu_category,sCat,sClass);

   }
   Format(buffer,sizeof(buffer),"%T","reagents",client);
   AddMenuItem(menu_category,"8",buffer);
   for(new i=1;i<=8;i++)   {
      if(GetClass(client) == i) continue;
      decl String:sClass[50];
      Format(sClass,sizeof(sClass),"class_%i",i);
      Format(sClass,sizeof(sClass),"%T",sClass,client);
      decl String:sCat[10];
      Format(sCat,sizeof(sCat),"%i",i+100);
      AddMenuItem(menu_category,sCat,sClass);
   }
   Format(buffer,sizeof(buffer),"%T","patterns for blacksmith",client);
   AddMenuItem(menu_category,"1",buffer);
   Format(buffer,sizeof(buffer),"%T","schemas for jewelcrafting",client);
   AddMenuItem(menu_category,"2",buffer);
   Format(buffer,sizeof(buffer),"%T","recipes for tailoring",client);
   AddMenuItem(menu_category,"3",buffer);
   Format(buffer,sizeof(buffer),"%T","patterns for leatherworking",client);
   AddMenuItem(menu_category,"4",buffer);
   Format(buffer,sizeof(buffer),"%T","formulas for enchanting",client);
   AddMenuItem(menu_category,"5",buffer);
   Format(buffer,sizeof(buffer),"%T","inscription books",client);
   AddMenuItem(menu_category,"9",buffer);

   DisplayMenu(menu_category,client,MENU_TIME_FOREVER);
}

new g_ffPreItems[65][5];

public CategoryHandle(Handle:menu_category,MenuAction:action,client,slot)
{
   if(action==MenuAction_Select)
   {
    decl String:category[255];
    GetMenuItem(menu_category,slot,category,sizeof(category));
    new cat=StringToInt(category);
    for(new i=0; i<5 ;i++)
      g_ffPreItems[client][i]=0;

    if(cat >=100) {
      //Pick type
      g_ffPreItems[client][0]=cat;

      PreItemsMenu1(client);
    }
    else
      ItemsMenu(client,cat);
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack)
         ShopsMenu(client);
   if(action==MenuAction_End)
      CloseHandle(menu_category);
}

PreItemsMenu1(client) {

  new Handle:menu=CreateMenu(PreItemsMenu1Handler);
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc preitems1 menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[200];
  Format(buffer, sizeof(buffer), "%T", "item_shop", client, GetMoney(client));
  AddMenuItem(menu,"1",buffer);
  Format(buffer, sizeof(buffer), "%T", "item_honor", client, GetHonor(client));
  AddMenuItem(menu,"2",buffer);
  decl String:sgold[20];
  IntToStringfloat(GetGold(client), sgold, 20);
  Format(buffer, sizeof(buffer), "%T", "item_gold", client, sgold);
  AddMenuItem(menu,"3",buffer);
  Format(buffer, sizeof(buffer), "%T", "item_justice", client, GetJustice(client));
  AddMenuItem(menu,"4",buffer);
  Format(buffer, sizeof(buffer), "%T", "item_valor", client, GetValor(client));
  AddMenuItem(menu,"5",buffer);



  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public PreItemsMenu1Handler(Handle:menu, //rename
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    decl String:id[20];
    GetMenuItem(menu,slot,id,sizeof(id));
    g_ffPreItems[client][1]=StringToInt(id);
    ItemsMenu(client, g_ffPreItems[client][0], false, g_ffPreItems[client][1]);
  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      //Go previous menu?
      CategoryMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}




new bool:g_ff_last_gold[65];
stock ItemsMenu(client,category, bool:goldshop=false, special=0)
{
   g_ff_last_gold[client]=goldshop;
   g_ff_shop_autoequip[client]=false;
   g_iLastCategory[client]=category;
   if(special == 0 && !goldshop) special = g_ffPreItems[client][1];
   decl String:info[255],String:title[255];
   decl String:name[255];
   decl level;
   new itemar[1000],itemlvlar[1000];
   new amount=0;
   new cat=category;
   new bClass=false;
   new iClass=0;

   if(cat>=100) {
      Format(info,sizeof(info),"class_%i",cat-100);
      Format(title,sizeof(title),"%T", info, client);
      bClass=true;
      iClass=cat-100;
   }
   else  {
      IdToCategory(cat,title);
      Format(title,sizeof(title),"%T",title,client);
   }
   new Handle:menu_items=CreateMenu(MenuItemsHandle);
   SetMenuExitBackButton(menu_items,true);

   SetMenuTitle(menu_items,title);
   KvRewind(items);
   if (!KvGotoFirstSubKey(items))
   {
      LogError("could not find first subkey of items");
   }
   decl String:buffer[255];

   do
   {
     //Special cases:
     //1 - simple shop
     //2 - honor
     //3 - gold
     //4 - justice
     //5 - valor
     //
     new noshop = KvGetNum(items,"noshop",0);
     if(special == 1 && noshop>0)  continue;
     if(special==0 && (!goldshop) && (!FUN_SERVER) && (noshop > 0) && (noshop < 5  ) )
     continue;

     //Check if it's not 'gold shop'
     new gold = KvGetNum(items, "gold");
     if(special!=3 && ( gold > 0 ) && (!goldshop) )
     continue;
     if ( (gold ==0) && (goldshop || special==3) )
     continue;

     new token = KvGetNum(items,"token",0);
     new honor = KvGetNum(items,"honor",0);
     if(special == 2)  {
      if(honor == 0)  continue;
     }
     if(special == 4)
      if(token != 13584)  continue;
     if(special == 5)
      if(token != 13583)  continue;

     if(!bClass) {
       if(KvGetNum(items,"category",0)!=cat)
       continue;
     }
     else  {
       new bool:pass=false;
       //Finding classes
       new classes[12];
       decl String:Sclass[55];
       KvGetString(items,"class",Sclass,sizeof(Sclass),"0");
       decl String:Stemp[50];
       new StringID=BreakString(Sclass, Stemp, sizeof(Stemp));
       new camount=1;
       classes[1]=StringToInt(Stemp);
       while(StringID>=0)
       {
         camount++;
         new StringID2=BreakString(Sclass[StringID], Stemp, sizeof(Stemp));
         StringID+=StringID2;
         if(StringID2==-1)
         StringID=-1;
         classes[camount]=StringToInt(Stemp);
       }
       //end of finding
       for(new x=1;x<=camount;x++)
       {
         if(classes[x]==iClass)
         pass=true;

       }
       if(!pass)
       continue;
     }
     {
       new Float:ver = KvGetFloat(items,"version",0.96);
       new lMulti = 1;
       if (ver <= 0.96)
       lMulti = 4;
       level=KvGetNum(items,"level")*lMulti;
       new reqskill=KvGetNum(items,"reqskill",-1);
       if(reqskill >= 0) level = reqskill;
       KvGetSectionName(items, info, sizeof(info));
       /*PrintToServer("Adding item: %s", info);*/
       new item=StringToInt(info);
       //KvGetString(items, "name", name[amount],sizeof(name[amount]));
       //KvGetString(items,"descr",descr[amount],sizeof(descr[amount]));
       //if (cl>0)
       //{
       //   decl String:cl_n[255];
       //   wcClassToName(client,cl,cl_n,sizeof(cl_n));
       //   Format(buffer,sizeof(buffer),"%s %i (%s) (%s)",name,level,cl_n,descr);
       //}
       //else
       //{
       //   Format(buffer,sizeof(buffer),"%s %i (%s)",name,level,descr);
       //}
       for(new x=0;x<=amount;x++)
       {
         if (itemlvlar[x]>level)
         {
           for(new y=amount;y>=x;y--)
           {
             itemlvlar[y+1]=itemlvlar[y];
             itemar[y+1]=itemar[y];
           }
           itemlvlar[x]=level;
           itemar[x]=item;
           amount++;
           break;
         }
         if(itemar[x]==0)
         {
           itemlvlar[x]=level;
           itemar[x]=item;
           amount++;
           break;
         }
       }
     }
   }  while (KvGotoNextKey(items));
   {
      for(new x=0;x<amount;x++)
      {
         KvRewind(items);
         IntToString(itemar[x],info,sizeof(info));
         KvJumpToKey(items,info);
         KvGetString(items, "name", name,sizeof(name));
         new classes[10];
         new clamount=GetClasses(itemar[x],classes);
         if (classes[1]>0)
         {
            decl String:cl_n[255];
            Format(cl_n,sizeof(cl_n),"(");
            for(new y=1;y<=clamount;y++)
            {
              decl String:Scl[255];
              wcClassToName(client,classes[y],Scl,sizeof(Scl));
              Format(cl_n,sizeof(cl_n),"%s%s",cl_n,Scl);
              if(y<clamount)
                Format(cl_n,sizeof(cl_n),"%s ",cl_n);
            }
            Format(cl_n,sizeof(cl_n),"%s)",cl_n);
            Format(buffer,sizeof(buffer),"%s %i %s",name,itemlvlar[x],cl_n);
         }
         else
         {
            new reqskill=KvGetNum(items,"reqskill");
            new profId = KvGetNum(items, "category");
            if(profId == 5) profId = 8;
            if(reqskill > 0 && isCraftingProfession(profId))  {
              decl String:slevel[30];
              new plevel = professionSkillToLevel(profId, reqskill);
              ProfessionLevelToString(plevel, slevel, sizeof(slevel), client);
              Format(buffer,sizeof(buffer),"%s   (%s)",name,slevel);

            }
            else  {
              Format(buffer,sizeof(buffer),"%s %i",name,itemlvlar[x]);
            }
         }
         AddMenuItem(menu_items,info,buffer);
      }
      if(amount==0)  {
         Format(buffer,sizeof(buffer),"%T","no items", client);
         AddMenuItem(menu_items,buffer,buffer,ITEMDRAW_DISABLED);
      }
   }
   DisplayMenu(menu_items,client,MENU_TIME_FOREVER);
}



public MenuItemsHandle(Handle:menu_items,MenuAction:action,client,slot)
{
   if(action==MenuAction_Select)
   {
    new String:buffer[10];
    GetMenuItem(menu_items,slot,buffer,sizeof(buffer));
    new item=StringToInt(buffer);
    ItemMenu(client,item);
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack) {
        if(!g_ff_last_gold[client])
          CategoryMenu(client);
        else  {
          GoldShopItems(client);
        }
      }
   if(action==MenuAction_End)
      CloseHandle(menu_items);
}

public ItemMenu(client,item)
{
   new Handle:menu_item=CreateMenu(ItemMenuHandle);
   SetMenuExitBackButton(menu_item,true);
   decl String:title[255];
   GetItemName(item,title);
   SetMenuTitle(menu_item,title);
   decl String:info[255];
   decl String:buffer[255];
   IntToString(item,info,sizeof(info));
   Format(buffer, sizeof(buffer), "%T", "Item Information", client);
   AddMenuItem(menu_item,info,buffer);
   Format(buffer, sizeof(buffer), "%T", "Item Purchase", client);
   AddMenuItem(menu_item,info,buffer);
   if(getAutoEquip(client)) {
    AddMenuItem(menu_item,info,"Do not recommend this item in todo");
   }
   DisplayMenu(menu_item,client,MENU_TIME_FOREVER);
}

public ItemMenuHandle(Handle:menu_item,MenuAction:action,client,slot)
{
   decl String:info[255];
   GetMenuItem(menu_item,1,info,sizeof(info));
   new item=StringToInt(info);
   if(action==MenuAction_Select)
   {
    switch(slot)
    {
      case 0:
      {
        lastMenu[client]=6;
        lastMenu2[client]=item;
        new uitem[UniqueItem];
        GetItemProperties(uitem, item);
        ItemInfMenu(client,uitem,true);
      }
      case 1:
      {
        ItemPurchase(client,item);
      }
      case 2:
      {
        ignoreItemRecommended(client, item);
        displayTodoMenu(client, true);
      }
    }
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack)
         ItemsMenu(client,g_iLastCategory[client],g_ff_last_gold[client]);
   if(action==MenuAction_End)
      CloseHandle(menu_item);
}

public ItemInfMenu(client,item[UniqueItem],bool:back)
{
   /*PrintToServer("Item ID: %d", item[Item_id]);*/
   if((item[Item_id]>0) || (item[Item_defId]>0))
   {
   new Handle:menu_iteminf=CreateMenu(ItemInfHandle);
   SetMenuExitBackButton(menu_iteminf,true);
   //if(back)
   //   SetMenuExitBackButton(menu_iteminf,true);
   decl String:iteminftitle[255],String:info[10];
   IntToString(item[Item_defId], info, sizeof(info));
   IntToString(item[Item_defId],info,sizeof(info));
   new String:inf[30]="information";
   Format(inf,sizeof(inf),"%T",inf,client);
   Format(iteminftitle,sizeof(iteminftitle),"%s %s",item[Item_Name],inf);
   SetMenuTitle(menu_iteminf,iteminftitle);

   new bufferi,Float:bufferf,String:fbuffer[255],String:trans[255];

   new Float:rating=GetItemRating(item);
   Format(trans, sizeof(trans), "%T", "rating", client, rating);
   AddMenuItem(menu_iteminf, info, trans, ITEMDRAW_DISABLED);

   new category=item[Item_category];
   new classes[10];
   new clamount=ConvertBitToClasses(item[Item_class], classes);
   if (classes[0]>0)
   {
      decl String:cl_n[255];
      Format(cl_n,sizeof(cl_n),"(");
      for(new y=0;y<clamount;y++)
      {
        decl String:Scl[255];
        wcClassToName(client,classes[y],Scl,sizeof(Scl));
        Format(cl_n,sizeof(cl_n),"%s%s",cl_n,Scl);
        if(y<clamount)
          Format(cl_n,sizeof(cl_n),"%s ",cl_n);
      }
      Format(cl_n,sizeof(cl_n),"%s)",cl_n);
      AddMenuItem(menu_iteminf,info,cl_n,ITEMDRAW_DISABLED);
   }
   if(!StrEqual(item[Item_Descr],""))
      AddMenuItem(menu_iteminf,info,item[Item_Descr],ITEMDRAW_DISABLED);
   bufferi=item[Item_quality];
   decl String:catname[255];
   IdToCategory(category,catname);
   Format(catname,sizeof(catname),"%T",catname,client);
   AddMenuItem(menu_iteminf,info,catname,ITEMDRAW_DISABLED);
   if(item[Item_EnchBit]>0) {
    new enchItem[UniqueItem];
    GetItemProperties(enchItem, item[Item_EnchBit]);
    if(enchItem[Item_defId]>0)  {
      Format(fbuffer, sizeof(fbuffer), "%s", enchItem[Item_Name]);
      IntToString(enchItem[Item_defId], info, sizeof(info));
      AddMenuItem(menu_iteminf,info,fbuffer);
    }

   }
   if( (category<=9) || (category==25) )
   {
    //writing reagents
    KvRewind(items);
    KvJumpToKey(items,info);
    new createid=KvGetNum(items,"itemid");
    if(createid>0)
    {
    new String:Sitem[255];
    GetItemName(createid,Sitem);
    if(category!=25)
      Format(fbuffer,sizeof(fbuffer),"%T","create item",client,Sitem);
    else
      Format(fbuffer,sizeof(fbuffer),"%T","ench item",client,Sitem);
    new soulbound = KvGetNum(items,"soulbound");
    if(soulbound!=0)  {
      AddMenuItem(menu_iteminf,info,"Creates Soulbound item!", ITEMDRAW_DISABLED);
    }
    IntToString(createid,Sitem,sizeof(Sitem));
    AddMenuItem(menu_iteminf,Sitem,fbuffer);
    KvRewind(items);
    KvJumpToKey(items,info);
    new reqskill=KvGetNum(items,"reqskill");
    new profId = KvGetNum(items, "category");
    decl String:slevel[30];
    if(profId == 5) profId = 8;
    new plevel = professionSkillToLevel(profId, reqskill);
    ProfessionLevelToString( plevel, slevel, sizeof(slevel), client);

    Format(fbuffer,sizeof(fbuffer),"%T","req prof level",client,slevel);
    if(category!=25)
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
    new amount=KvGetNum(items,"reagents",0);
    new reagents[amount+1],nreagents[amount+1],String:Sreagents[amount+2][255];
    for(new x=1;x<=amount;x++)
    {
      decl String:buffer[60];
      Format(buffer,sizeof(buffer),"reagent_%i",x);
      reagents[x]=KvGetNum(items,buffer);
      Format(buffer,sizeof(buffer),"%s_amount",buffer);
      nreagents[x]=KvGetNum(items,buffer);
    }
    for(new x=1;x<=amount;x++)
    {
      KvRewind(items);
      decl String:Sid[20];
      IntToString(reagents[x],Sid,sizeof(Sid));
      KvJumpToKey(items,Sid);
      KvGetString(items,"name",Sreagents[x],255);
      Format(fbuffer,sizeof(fbuffer),"%T","reagent req",client,Sreagents[x],nreagents[x]);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
    }
   }
   }
   if((bufferi>0))
   {
      switch(bufferi)
      {
        case 1:
          Format(fbuffer, sizeof(fbuffer), "%T", "green item", client);
        case 2:
          Format(fbuffer, sizeof(fbuffer), "%T", "blue item", client);
        case 3:
          Format(fbuffer, sizeof(fbuffer), "%T", "epic item", client);
        case 4:
          Format(fbuffer, sizeof(fbuffer), "%T", "legendary item", client);
        case 5:
          Format(fbuffer, sizeof(fbuffer), "%T", "pvp blue item", client);
        case 6:
          Format(fbuffer, sizeof(fbuffer), "%T", "pvp epic item", client);
      }
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_level];
   Format(fbuffer, sizeof(fbuffer), "%T", "level", client, bufferi);
   AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   bufferi=item[Item_ilevel];
   if(bufferi > 0)  {
     Format(fbuffer, sizeof(fbuffer), "%T", "ilevel", client, bufferi);
     AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }


   bufferi=item[Item_cost];
   if(item[Item_gold]<=0)
    Format(fbuffer, sizeof(fbuffer), "%T", "cost", client, bufferi);
   else {
    decl String:sgold[20];
    IntToStringfloat(item[Item_gold], sgold, 20);
    Format(fbuffer, sizeof(fbuffer), "%T", "cost gold2", client, sgold);
   }
   AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);


   bufferi=item[Item_honor];
   if(bufferi>0) {
      Format(fbuffer, sizeof(fbuffer), "%T", "cost honor", client, bufferi, GetHonor(client));
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   if(item[Item_justice]>0) {
     Format(fbuffer, sizeof(fbuffer), "%T", "cost justice", client, item[Item_justice], GetJustice(client));
     AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   if(item[Item_valor]>0) {
     Format(fbuffer, sizeof(fbuffer), "%T", "cost valor", client, item[Item_valor], GetValor(client));
     AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }

   if(category>9)
   {
   bufferi=item[Item_2hand];
   if (bufferi==1)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "2hand", client);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   else if (bufferi==0 && item[Item_category] == 22)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "1hand", client);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }

   bufferi=item[Item_weapon_ap];

   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "weapon ap", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }

   bufferi=item[Item_weapon_sp];

   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "weapon sp", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }



   bufferi=item[Item_damage];

   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "damage", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferf=item[Item_ws];
   if (bufferf>0.0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "weapon speed", client, RoundToNearest(bufferf*100.0));
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferf=item[Item_cs];
   if (bufferf>0.0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "critical", client, RoundToNearest(bufferf*100.0));
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_resilence];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "resilence no comm", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferf=item[Item_acc];
   if (bufferf>0.0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "accuracy no comm", client, RoundToNearest(bufferf*100.0));
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_armor_type];
   if (bufferi>0)
   {
     decl String:armorType[30];
     Format(armorType, sizeof(armorType), "armor_type_%d", bufferi);
     Format(armorType, sizeof(armorType), "%T", armorType, client);
     Format(fbuffer, sizeof(fbuffer), "%T", "armor type", client, armorType);
     AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }

   bufferi=item[Item_armor];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "armor", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_res];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "resistance", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_str];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "bonus str", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_agi];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "bonus agi", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_int];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "bonus int", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_sta];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "bonus sta", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_ai];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "ai", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_regen];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "mregen", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferf=item[Item_as];
   if (bufferf!=0.0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "bonus as", client, RoundToNearest(bufferf*100.0));
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_spelldmg];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "bonus spelldmg", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferf=item[Item_speed];
   if (bufferf!=0.0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "bonus speed", client, RoundToNearest(bufferf*100.0));
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_haste];
   if (bufferi>0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "bonus haste", client, bufferi);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferf=item[Item_parry];
   if (bufferf>0.0)
   {
      Format(fbuffer, sizeof(fbuffer), "%T", "bonus parry", client, RoundToNearest(bufferf*100.0));
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   bufferi=item[Item_effect];
   if (bufferi>0)
   {
            //Firstly check what effects.txt has
      KvRewind(effectsdb);
      decl String:sTemp[100];
      new String:effect[255];
      IntToString(bufferi, sTemp, sizeof(sTemp));
      KvJumpToKey(effectsdb,sTemp);
      KvGetString(effectsdb, "translate", trans, sizeof(trans), "");
      //end
      if(StrEqual(trans, ""))  {
        trans="effect_";
        Format (effect,sizeof(effect),"%s%i",trans,bufferi);
        Format (trans,sizeof(trans),"%T",effect,client);
      }
      Format (fbuffer,sizeof(fbuffer),"%T","effect",client,trans);
      AddMenuItem(menu_iteminf,info,fbuffer,ITEMDRAW_DISABLED);
   }
   }
   //AddMenuItem(menu_iteminf,item_category,"Purchase");
   DisplayMenu(menu_iteminf,client,MENU_TIME_FOREVER);
   }
   else
      WCMessage(client,"no item for inf",client);
}

public ItemInfHandle(Handle:menu_iteminf,MenuAction:action,client,slot)
{
   decl String:Sitem[255];
   GetMenuItem(menu_iteminf,0,Sitem,sizeof(Sitem));
   new item=StringToInt(Sitem);
   if(action==MenuAction_Select)
   {
    GetMenuItem(menu_iteminf,slot,Sitem,sizeof(Sitem));
    item=StringToInt(Sitem);
    lastMenu[client]=-1;
    new uitem[UniqueItem];
    GetItemProperties(uitem, item);
    ItemInfMenu(client,uitem,false);
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack) {
         switch(lastMenu[client])   {
            case 1: OtherPlayerMenu(client, lastMenu2[client]);
            case 2: ProfRecipeMenu(client,lastMenu2[client],lastMenu3[client]);
            case 3: PrepareMenuReadAuction(client,lastMenu2[client],lastMenu3[client]);
            case 4: CharItemOpt(client,lastMenu2[client]);
            case 5: InvItemMenu(client,lastMenu2[client]);
            case 6: ItemMenu(client,lastMenu2[client]);
         }
         //ItemMenu(client,item);
      }
   if(action==MenuAction_End)
      CloseHandle(menu_iteminf);
}

public ItemPurchase(client,itemid)
{
   decl String:buffer[255],String:key[10];
   IntToString(itemid, key, sizeof(key));
   new item[UniqueItem];
   GetItemProperties(item, itemid);
   new Handle:menu_itempurchase=CreateMenu(MenuItemPurchaseHandle);
   SetMenuExitBackButton(menu_itempurchase,true);
   SetMenuTitle(menu_itempurchase,"%T (%s)","purchase",client,item[Item_Name],item[Item_Descr]);
   new money=GetMoney(client);
   new bufferi,String:fbuffer[255];

   bufferi=item[Item_cost];
   Format(fbuffer, sizeof(fbuffer), "%T", "cost", client, bufferi);
   if(item[Item_gold]<=0)
    AddMenuItem(menu_itempurchase,key,fbuffer,ITEMDRAW_DISABLED);
   Format (buffer,sizeof(buffer),"%T","current money",client,money);
   AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
   new moneyleft=money-bufferi;
   new honor = item[Item_honor];
   if(honor>0)  {
      Format(fbuffer, sizeof(fbuffer), "%T", "cost honor", client, honor, GetHonor(client));
      AddMenuItem(menu_itempurchase,key,fbuffer,ITEMDRAW_DISABLED);
   }
   if(item[Item_justice]>0) {
     Format(fbuffer, sizeof(fbuffer), "%T", "cost justice", client, item[Item_justice], GetJustice(client));
     AddMenuItem(menu_itempurchase,key,fbuffer,ITEMDRAW_DISABLED);
   }
   if(item[Item_valor]>0) {
     Format(fbuffer, sizeof(fbuffer), "%T", "cost valor", client, item[Item_valor], GetValor(client));
     AddMenuItem(menu_itempurchase,key,fbuffer,ITEMDRAW_DISABLED);
   }
   new gold = item[Item_gold];
   if(gold>0)  {
      moneyleft=0;
      decl String:sgold[20];
      decl String:sgold2[20];
      IntToStringfloat(item[Item_gold], sgold, sizeof(sgold));
      IntToStringfloat(GetGold(client), sgold2, sizeof(sgold2));
      Format(fbuffer, sizeof(fbuffer), "%T", "cost gold", client, sgold, sgold2);
      AddMenuItem(menu_itempurchase,key,fbuffer,ITEMDRAW_DISABLED);
   }
   if ( (moneyleft>=0) || FUN_SERVER )
   {
     Format (buffer,sizeof(buffer),"%T","will be money",client,moneyleft);
     if(item[Item_gold]<=0)
     AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
     /*PrintToServer("J c %d i %d  V c %d i %d",GetJustice(client), item[Item_justice], GetValor(client), item[Item_valor]);*/
     if(GetJustice(client) >= item[Item_justice]) {
       if(GetValor(client) >= item[Item_valor]) {
         if( (GetHonor(client)-honor) >= 0)  {

           if( GetGold(client)-gold >= 0 ) {

             if(wcCheckStats(client,item)==1)
             {
               if((wcCheckFreeSlot(client)>0))
               {
                 Format (buffer,sizeof(buffer),"%T","purchase",client,item[Item_Name]);
                 AddMenuItem(menu_itempurchase,"purchase",buffer);
                 if(item[Item_category] == 8) {
                   //Reagent, add purchase multiple options
                   new stack = 5;
                   if((GetGold(client) - gold * stack) >= 0 && (money - item[Item_cost] * stack) >= 0 && GetHonor(client) - honor * stack >= 0)  {
                     Format(buffer,sizeof(buffer), "%T", "purchase stack", client, item[Item_Name], stack);
                     AddMenuItem(menu_itempurchase,"purchase 5",buffer);
                   }
                   stack = 10;
                   if((GetGold(client) - gold * stack) >= 0 && (money - item[Item_cost] * stack) >= 0 && GetHonor(client) - honor * stack >= 0)  {
                     Format(buffer,sizeof(buffer), "%T", "purchase stack", client, item[Item_Name], stack);
                     AddMenuItem(menu_itempurchase,"purchase 10",buffer);
                   }
                   stack = 20;
                   if((GetGold(client) - gold * stack) >= 0 && (money - item[Item_cost] * stack) >= 0 && GetHonor(client) - honor * stack >= 0)  {
                     Format(buffer,sizeof(buffer), "%T", "purchase stack", client, item[Item_Name], stack);
                     AddMenuItem(menu_itempurchase,"purchase 20",buffer);
                   }
                   stack = 50;
                   if((GetGold(client) - gold * stack) >= 0 && (money - item[Item_cost] * stack) >= 0 && GetHonor(client) - honor * stack >= 0)  {
                     Format(buffer,sizeof(buffer), "%T", "purchase stack", client, item[Item_Name], stack);
                     AddMenuItem(menu_itempurchase,"purchase 50",buffer);
                   }
                   stack = 100;
                   if((GetGold(client) - gold * stack) >= 0 && (money - item[Item_cost] * stack) >= 0 && GetHonor(client) - honor * stack >= 0)  {
                     Format(buffer,sizeof(buffer), "%T", "purchase stack", client, item[Item_Name], stack);
                     AddMenuItem(menu_itempurchase,"purchase 100",buffer);
                   }


                 }
               }
             }
           }
           else  {
             Format (buffer,sizeof(buffer),"%T","no gold2",client);
             AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
           }
         }
         else  {
           Format (buffer,sizeof(buffer),"%T","no honor",client);
           AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
         }
       }
       else  {
         Format(buffer, sizeof(buffer), "%T", "no valor", client);
         AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
       }
     }
     else  {
       Format(buffer, sizeof(buffer), "%T", "no justice", client);
       AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
     }
     //AddMenuItem(menu_itempurchase,item);
   }
   else
   {
      Format (buffer,sizeof(buffer),"%T","no money",client);
      AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
   }
   if (wcCheckStats(client,item)==2)
   {
      Format (buffer,sizeof(buffer),"%T","wrong class",client);
      AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
   }
   if (wcCheckStats(client,item)==3)
   {
      Format (buffer,sizeof(buffer),"%T","low lvl item",client,GetRequiredLevel(item));
      AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
   }
   if (wcCheckStats(client,item)==0)
   {
      Format (buffer,sizeof(buffer),"%T","nostats",client);
      AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
   }
   if (wcCheckFreeSlot(client)==0)
   {
      Format(buffer,sizeof(buffer),"%T","no free slot",client);
      AddMenuItem(menu_itempurchase,key,buffer,ITEMDRAW_DISABLED);
   }
   Format(buffer,sizeof(buffer),"%T","Personal Statistics",client);
   AddMenuItem(menu_itempurchase,"stats",buffer);
   DisplayMenu(menu_itempurchase,client,MENU_TIME_FOREVER);
}

public MenuItemPurchaseHandle(Handle:menu_iteminf,MenuAction:action,client,slot)
{
  decl String:key[10];
  GetMenuItem(menu_iteminf,1,key,sizeof(key));
  new item=StringToInt(key);
  if(action==MenuAction_Select)
  {
    decl String:purch[255];
    GetMenuItem(menu_iteminf,slot,purch,sizeof(purch));
    if(StrContains(purch,"purchase",false)>=0)
    {
      //Check if it's stack
      new amount = 1;
      if(ReplaceString(purch, sizeof(purch), "purchase ", "") > 0)  {
        amount = StringToInt(purch);
      }
      if(amount <=0)  amount = 1;
      PurchaseItemConfirmed(client,item, amount);
      ItemsMenu(client,g_iLastCategory[client], g_ff_last_gold[client]);
    }
    else
    {
      wcPlayerStats(client);
    }
  }
  if(action==MenuAction_Cancel)
  if(slot==MenuCancel_ExitBack)
  {
   ItemMenu(client,item);
  }
  if(action==MenuAction_End)
  CloseHandle(menu_iteminf);
}

public wcPlayerStats(client)
{
   decl String:buffer[255];
   new Handle:menu_playerstats=CreateMenu(MenuPlayerStatsHandle);
   SetMenuExitBackButton(menu_playerstats,true);
   SetMenuTitle(menu_playerstats,"%T","player stats",client,characterids[client][0]);
   Format(buffer,sizeof(buffer),"%T","xp/reqxp/lvl",client,GetXp(client),GetReqXp(client),GetLevel(client));
   AddMenuItem(menu_playerstats,"xp",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","money",client,GetMoney(client));
   AddMenuItem(menu_playerstats,"money",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","str",client,GetStr(client),RoundToNearest(wcClientsStats[client][STR]),(GetStr(client)+RoundToNearest(wcClientsStats[client][STR])));
   AddMenuItem(menu_playerstats,"str",buffer, ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","agi",client,GetAgi(client),RoundToNearest(wcClientsStats[client][AGI]),(GetAgi(client)+RoundToNearest(wcClientsStats[client][AGI])));
   AddMenuItem(menu_playerstats,"agi",buffer, ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","int",client,GetInt(client),RoundToNearest(wcClientsStats[client][INT]),(GetInt(client)+RoundToNearest(wcClientsStats[client][INT])));
   AddMenuItem(menu_playerstats,"int",buffer, ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","sta",client,GetSta(client),RoundToNearest(wcClientsStats[client][STA]),(GetSta(client)+RoundToNearest(wcClientsStats[client][STA])));
   AddMenuItem(menu_playerstats,"sta",buffer, ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","points",client,GetPoints(client));
   AddMenuItem(menu_playerstats,"points",buffer,ITEMDRAW_DISABLED);
   //PVP Rank
   TranslateRank(GetRank(client),buffer,sizeof(buffer));
   Format(buffer, sizeof(buffer),"%T", "wc rank", client, buffer);
   AddMenuItem(menu_playerstats, "rank", buffer, ITEMDRAW_DISABLED);
   //end
   new Float:rating = GetPlayerRating(client);
   Format(buffer,sizeof(buffer), "%T", "rating", client, rating);
   AddMenuItem(menu_playerstats,"rating",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","damage",client,GetDamage(client));
   AddMenuItem(menu_playerstats,"damage",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","bonusspelldmg",client,GetSpellDmg(client,1.0));
   AddMenuItem(menu_playerstats,"spelldmg",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","armor",client,RoundToNearest(wcClientsStats[client][ARMOR]));
   AddMenuItem(menu_playerstats,"armor",buffer,ITEMDRAW_DISABLED);
   new Float:armor=wcClientsStats[client][ARMOR];
   // formula damage reduction
   new rdmg=RoundToNearest((1.0-GetDmgRed(RoundToNearest(armor), client))*100.0);
   Format(buffer,sizeof(buffer),"%T","red to lvl",client,rdmg);
   AddMenuItem(menu_playerstats,"red to lvl",buffer,ITEMDRAW_DISABLED);
   // resistance
   Format(buffer,sizeof(buffer),"%T","resistance",client,RoundToNearest(wcClientsStats[client][RESISTANCE]));
   AddMenuItem(menu_playerstats,"resistance",buffer,ITEMDRAW_DISABLED);
   //end
   // critical strike
   Format(buffer,sizeof(buffer),"%T","critical",client,RoundToNearest(GetCriticalStrike(client) * 100.0));
   AddMenuItem(menu_playerstats,"critical",buffer,ITEMDRAW_DISABLED);
   //end
   //Accuracy
   Format(buffer,sizeof(buffer),"%T","accuracy",client,RoundToNearest(GetAccuracy(client) * 100.0));
   AddMenuItem(menu_playerstats,"accuracy",buffer,ITEMDRAW_DISABLED);
   //end
   // resilence
   Format(buffer,sizeof(buffer),"%T","resilence",client,GetResilence(client));
   AddMenuItem(menu_playerstats,"resilence",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","resilence c chance",client,GetMinusCritChance(client) * 100.0);
   AddMenuItem(menu_playerstats,"resilence",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","resilence c dmg",client,GetMinusCrit(client) * 100.0);
   AddMenuItem(menu_playerstats,"resilence",buffer,ITEMDRAW_DISABLED);
   //end

   // block chance
   Format(buffer,sizeof(buffer),"%T","block",client,RoundToNearest(GetBlockChance(client) * 100.0));
   AddMenuItem(menu_playerstats,"block",buffer,ITEMDRAW_DISABLED);
   //end
   // Spell Reduction
   new sred;
   new Float:spred=GetSpellReduction(client,0.0);
   sred=RoundToNearest(spred * 100.0)-100;
   sred*=-1;
   Format(buffer,sizeof(buffer),"%T","red to res",client,sred);
   AddMenuItem(menu_playerstats,"red to res",buffer,ITEMDRAW_DISABLED);
   //end
   Format(buffer,sizeof(buffer),"%T","haste",client,GetHasteEffect(client)*100,GetHaste(client));
   AddMenuItem(menu_playerstats,"haste",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","parry",client,GetParryChance(client)*100.0,GetParryReduce(client)*100.0);
   AddMenuItem(menu_playerstats,"parry",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","ai",client,RoundToNearest(wcClientsStats[client][ARMORIGNORE]));
   AddMenuItem(menu_playerstats,"ai",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","asws",client,RoundToNearest(wcClientsStats[client][ATTACKSPEED]*100.0),RoundToNearest(GetWeaponSpeed(client)*100.0));
   AddMenuItem(menu_playerstats,"as",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","speed",client,RoundToNearest(wcClientsStats[client][MOVESPEED]*100.0));
   AddMenuItem(menu_playerstats,"speed",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","maxhp",client,GetMaxHealth(client));
   AddMenuItem(menu_playerstats,"maxhp",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","maxmp",client,RoundToNearest(wcClientsStats[client][MAXMP]));
   AddMenuItem(menu_playerstats,"maxmp",buffer,ITEMDRAW_DISABLED);
   Format(buffer,sizeof(buffer),"%T","mregen",client,RoundToNearest(wcClientsStats[client][MPREGEN]));
   AddMenuItem(menu_playerstats,"mregen",buffer,ITEMDRAW_DISABLED);
   //new resc=RoundToNearest(Pow(1.4,(float(GetLevel(client)))/3.1));
   //Format(buffer,sizeof(buffer),"%T","reset stats",client,resc);
   //if(GetMoney(client)>=resc)
   //   AddMenuItem(menu_playerstats,"reset",buffer);
   //else
   //   AddMenuItem(menu_playerstats,"reset",buffer,ITEMDRAW_DISABLED);
   DisplayMenu(menu_playerstats,client,MENU_TIME_FOREVER);
}

public MenuPlayerStatsHandle(Handle:menu_playerstats,MenuAction:action,client,slot)
{
  /*if(action==MenuAction_Select)
  {
    switch(slot)
    {
      case 2:
      {
        if(GetPoints(client)>0)
        {
          wcSetStr(client,GetStr(client)+1);
          wcSetPoints(client,GetPoints(client)-1);
          wcPlayerStats(client);
        }
        else
        {
          WCMessage(client,"Tell player no points",client);
          wcPlayerStats(client);
        }
      }
      case 3:
      {
        if(GetPoints(client)>0)
        {
          wcSetAgi(client,GetAgi(client)+1);
          wcSetPoints(client,GetPoints(client)-1);
          wcPlayerStats(client);
        }
        else
        {
          WCMessage(client,"Tell player no points",client);
          wcPlayerStats(client);
        }
      }
      case 4:
      {
        if(GetPoints(client)>0)
        {
          wcSetInt(client,GetInt(client)+1);
          wcSetPoints(client,GetPoints(client)-1);
          wcPlayerStats(client);
        }
        else
        {
          WCMessage(client,"Tell player no points",client);
          wcPlayerStats(client);
        }
      }
      case 5:
      {
        if(GetPoints(client)>0)
        {
          wcSetSta(client,GetSta(client)+1);
          wcSetPoints(client,GetPoints(client)-1);
          wcPlayerStats(client);
        }
        else
        {
          WCMessage(client,"Tell player no points",client);
          wcPlayerStats(client);
        }
      }
    }
    decl String:line[25];
    GetMenuItem(menu_playerstats,slot,line,sizeof(line));
    if(!strcmp(line,"reset"))
    {
      ConfirmResetStats(client);
    }
  }
  */
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      WCMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_playerstats);
}

/*public ConfirmResetStats(client)
{
  new Handle:menu_reset=CreateMenu(ResetStatsHandle);
  SetMenuExitBackButton(menu_reset,true);
  decl String:title[255],String:line[255];
  new cost=RoundToNearest(Pow(1.4,(float(GetLevel(client)))/3.1));
  Format(title,sizeof(title),"%T","wc reset confirm",client);
  SetMenuTitle(menu_reset,title);
  Format(line,sizeof(line),"%T","reset confirm",client,cost);
  if(cost<=GetMoney(client))
   AddMenuItem(menu_reset,"confirm",line);
  else
   AddMenuItem(menu_reset,"confirm",line,ITEMDRAW_DISABLED);
  DisplayMenu(menu_reset,client,MENU_TIME_FOREVER);
}

public ResetStatsHandle(Handle:menu_reset,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    new cost=RoundToNearest(Pow(1.4,(float(GetLevel(client)))/3.1));
    new slots=GetFilledInventory(client);
    if ((100-slots)>=14)
    if(cost<=GetMoney(client))
    {
      for (new x=10;x<=23;x++)
         PutIntoInventory(client,x);
      ResetPoints(client);
      WCMessage(client,"reset confirmed",client,cost,GetPoints(client));
      wcSetMoney(client,GetMoney(client)-cost);
    }
    else
      WCMessage(client,"no money",client);
    else
      WCMessage(client,"low free slots",client,(100-slots));
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      wcPlayerStats(client);
  if(action==MenuAction_End)
    CloseHandle(menu_reset);
}
*/

ResetPoints(client) {
  new points;
  points+=GetStr(client)-1;
  points+=GetAgi(client)-1;
  points+=GetInt(client)-1;
  points+=GetSta(client)-1;
  wcSetStr(client,1);
  wcSetAgi(client,1);
  wcSetSta(client,1);
  wcSetInt(client,1);
  wcSetPoints(client,GetPoints(client)+points);
}

public HelpMenu(client,ids[],amount)
{
  ShowMOTDPanel(client, "WoW Help Wiki", WIKI_PAGE, MOTDPANEL_TYPE_URL);
  /*
  new Handle:menu_help=CreateMenu(HelpMenuHandle);
  SetMenuExitBackButton(menu_help,true);
  new String:Sid[5];
  new String:menutree[255],String:menutree2[255];
  decl String:Smenuid[6];
  Format(menutree,sizeof(menutree),"%i",amount+1);
  for (new i=1;i<=(amount);i++)
  {
    Format(menutree,sizeof(menutree),"%s %i",menutree,ids[i]);
  }
  KvRewind(wchelp);
  for(new i=1;i<=amount;i++)
  {
    IntToString(ids[i],Sid,sizeof(Sid));
    KvJumpToKey(wchelp,Sid);
  }
  decl String:title[255],String:text[700],String:Smenu[255];
  KvGetString(wchelp,"title",title,sizeof(title),"No Title");
  SetMenuTitle(menu_help,title);
  new menus=KvGetNum(wchelp,"menu",0);
  KvGetString(wchelp,"text",text,sizeof(text),"0");
  if(!StrEqual(text,"0"))
  {
    ReplaceString(text,sizeof(text),"@n","\n");
    ReplaceString(text,sizeof(text),"@'","\"");
    Format(menutree2,sizeof(menutree2),"%s 0",menutree);
    AddMenuItem(menu_help,menutree2,text,ITEMDRAW_DISABLED);
  }
  for(new i=1;i<=menus;i++)
  {
    Format(Smenuid,sizeof(Smenuid),"%i m",i);
    KvGetString(wchelp,Smenuid,Smenu,sizeof(Smenu));
    ReplaceString(Smenu,sizeof(Smenu),"@n","\n");
    ReplaceString(Smenu,sizeof(Smenu),"@'","\"");
    Format(menutree2,sizeof(menutree2),"%s %i",menutree,i);
    AddMenuItem(menu_help,menutree2,Smenu);
  }
  DisplayMenu(menu_help,client,MENU_TIME_FOREVER);
  */
}

public HelpMenuHandle(Handle:menu_help,MenuAction:action,client,slot)
{
  new ids[20];
  if(action==MenuAction_Select)
  {
    decl String:buffer[255],String:Stemp[5];
    GetMenuItem(menu_help,slot,buffer,sizeof(buffer));
    new StringID=BreakString(buffer, Stemp, sizeof(Stemp));
    new amount=StringToInt(Stemp);
    for (new i=1;i<=amount;i++)
    {
      new old=StringID;
      StringID=BreakString(buffer[StringID],Stemp,sizeof(Stemp));
      StringID+=old;
      ids[i]=StringToInt(Stemp);
    }
    HelpMenu(client,ids,amount);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
    {
      decl String:buffer[255],String:Stemp[5];
      GetMenuItem(menu_help,0,buffer,sizeof(buffer));
      new StringID=BreakString(buffer, Stemp, sizeof(Stemp));
      new amount=StringToInt(Stemp);
      for (new i=1;i<=amount;i++)
      {
        new old=StringID;
        StringID=BreakString(buffer[StringID],Stemp,sizeof(Stemp));
        StringID+=old;
        ids[i]=StringToInt(Stemp);
      }
      if((amount-2)>=0)
        HelpMenu(client,ids,(amount-2));
      else
        WCMenu(client);
    }
  if(action==MenuAction_End)
    CloseHandle(menu_help);
}

public AccountMenu(client)
{
   SetMenuOff(client,30.0);
   decl String:buffer[100];
   Format(buffer,sizeof(buffer),"SELECT login, pass, gold, bound FROM wc_Players WHERE (playerId=%i)",playerids[client]);
   SQL_TQuery(TSQL,TAccMenu,buffer,client,DBPrio_Low);
}

public TAccMenu(Handle:db,Handle:query,String:error[],any:client)
{
  if(IsClientConnected(client))
  {
    SetMenuOff(client,0.0);
    if(query)
    {
      SQL_Rewind(query);
      if(SQL_FetchRow(query))
      {
         decl String:login[30], String:pass[30];
         GetSQLDataStr(query, "login", login, sizeof(login));
         GetSQLDataStr(query, "pass", pass, sizeof(pass));
         new Float:gold = float(GetSQLDataInt(query, "gold"))/100.0;
         new iGold = GetSQLDataInt(query,"gold");
         setGold(client, iGold);
         if(strlen(login)<2)
            login="None yet";
         if(strlen(pass)<2)
            pass="None yet";
         new bool:bound=false;
         if(GetSQLDataInt(query,"bound")==1)
          bound=true;
         AccountMenu2(client,login,pass, gold, bound);
         return;
      }
    }
  }
  WCMessage(client,"failed loading",client);
  LogError ("WC FAILED menus.4286 %s",error);
}

new String:cachedAccData[65][2][30];

public AccountMenu2(client, String:login[30], String:pass[30], Float:gold,
          bool:bound)   {
  strcopy(cachedAccData[client][0],30,login);
  strcopy(cachedAccData[client][1],30,pass);
  new Handle:menu_acc=CreateMenu(AccountMenuHandle);
  SetMenuExitBackButton(menu_acc,true);
  decl String:title[128],String:line[128];
  Format(title,sizeof(title),"%T","account menu",client,line);
  SetMenuTitle(menu_acc,title);
  Format(line,sizeof(line),"%T","wc load page",client);
  AddMenuItem(menu_acc,line,line);
  Format(line,sizeof(line),"%T","gold balance",client,gold);
  AddMenuItem(menu_acc,line,line,ITEMDRAW_DISABLED);
  Format(line,sizeof(line),"%T","wc set login",client,login);
  if(!bound)
    AddMenuItem(menu_acc,line,line);
  else
    AddMenuItem(menu_acc,line,line,ITEMDRAW_DISABLED);
  Format(line,sizeof(line),"%T","wc set pass",client,pass);
  if(!bound)
    AddMenuItem(menu_acc,line,line);
  else
    AddMenuItem(menu_acc,line,line,ITEMDRAW_DISABLED);
  if(!bound)  {
    Format(line, sizeof(line), "%T", "Binding to Forum Account (off)", client);
    AddMenuItem(menu_acc,"",line,ITEMDRAW_DISABLED);
  }
  else  {
    AddMenuItem(menu_acc,"","Your account is already bound to Forum account",
      ITEMDRAW_DISABLED);
  }
  if(IsThreatEnabled(client)) {
   Format(line,sizeof(line),"%T","wc threat is on",client);
  }
  else   {
   Format(line,sizeof(line),"%T","wc threat is off",client);
  }
  AddMenuItem(menu_acc,"threat",line);
  DisplayMenu(menu_acc,client,MENU_TIME_FOREVER);
}

public AccountMenuHandle(Handle:menu_acc,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    switch(slot)
    {
      case 0:
        ShowMOTDPanel(client, "WoW Account Page", ACCOUNT_PAGE, MOTDPANEL_TYPE_URL);
      case 2:  {
         g_iAccountTyping[client]=1;
         g_fAccountTime[client]=GetEngineTime()+30.0;
         WCMessage(client,"acc type login",client);
      }
      case 3:  {
         g_iAccountTyping[client]=2;
         g_fAccountTime[client]=GetEngineTime()+30.0;
         WCMessage(client,"acc type pass",client);
      }
      case 4:  {
        bindingAccount(client, cachedAccData[client][0], cachedAccData[client][1]);
      }
      case 5:  {
         TriggerThreat(client);
         AccountMenu(client);
      }
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      WCMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu_acc);
}

public RefreshAccData(client) {
   g_iAccountTyping[client]=0;
   g_fAccountTime[client]=0.0;
}

public EnchantMenu(client, slot)  {
  new item[UniqueItem];
  GetInvItem(item, client, slot);
  KvRewind(items);
  decl String:Sid[20];
  IntToString(item[Item_defId],Sid,sizeof(Sid));
  KvJumpToKey(items,Sid);


  if(item[Item_category]!=25) {
    WCMessage(client,"this item is not enchant",client, item[Item_Name]);
    InvItemMenu(client, lastMenu2[client]);
    return;
  }
  new cat2=KvGetNum(items,"category2",0);
  new hand = KvGetNum(items,"2hand",0);
  new bool:twohand=(hand==1);
  new createid=KvGetNum(items,"itemid",0);
  if( (cat2<=0) || (createid<=0) )  {
    WCMessage(client,"this item is not enchant",client, item[Item_Name]);
    InvItemMenu(client, lastMenu2[client]);
    return;
  }
  WCMessage(client,"choose to enchant",client, item[Item_Name]);

  new Handle:menu=CreateMenu(EnchantMenuHandler);
  SetMenuExitBackButton(menu,true);
  decl String:title[128],String:line[128];
  Format(title,sizeof(title),"%T","enchant menu",client,line);
  SetMenuTitle(menu,title);
  decl String:Sslot[7];
  IntToString(slot, Sslot, sizeof(Sslot));

  for (new x=1;x<=100;x++)
  {
    GetInvItem(item, client,x);
    decl String:Sitem[50];
    if((item[Item_defId]==0) && (item[Item_id]==0))
      Format(Sitem,sizeof(Sitem),"%T","empty",client);
    else
      Format(Sitem,sizeof(Sitem),"%s",item[Item_Name]);
    decl String:Scategory[255];
    TranslateCategory(item[Item_category],Scategory, client);
    Format(line,sizeof(line),"slot %i: %s (%s)",x,Sitem,Scategory);
    if(cat2==item[Item_category]) {
      if(!twohand)
        AddMenuItem(menu,Sslot,line);
      else  {
        if(item[Item_2hand]==1) {
          AddMenuItem(menu,Sslot,line);
        }
        else
          AddMenuItem(menu,Sslot,line, ITEMDRAW_DISABLED);
      }
    }
    else
      AddMenuItem(menu,Sslot,line, ITEMDRAW_DISABLED);
  }
  DisplayMenu(menu,client,MENU_TIME_FOREVER);
}

public EnchantMenuHandler(Handle:menu,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));
    new enchSlot = StringToInt(buffer);
    slot++;
    new enchant[UniqueItem], item[UniqueItem];
    GetInvItem(enchant, client, enchSlot);
    GetInvItem(item, client, slot);
    KvRewind(items);
    decl String:Sid[20];
    IntToString(enchant[Item_defId],Sid,sizeof(Sid));
    KvJumpToKey(items,Sid);
    if(enchant[Item_category]!=25) {
      WCMessage(client,"this item is not enchant",client, item[Item_Name]);
      InvItemMenu(client, lastMenu2[client]);
      return;
    }
    new cat2=KvGetNum(items,"category2",0);
    new enchid=KvGetNum(items,"itemid",0);
    if( (cat2!=item[Item_category]) || (cat2==0) )  {
      WCMessage(client,"wrong ench category",client, enchant[Item_Name], item[Item_Name]);
      InvItemMenu(client, lastMenu2[client]);
      return;
    }

    RemoveInvItem(client, enchSlot, 1);
    wcSetStaticSlotAmount(client,slot,-1);
    ApplyEnchant(client, slot, enchid);
    InventoryMenu(client);
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      InvItemMenu(client, lastMenu2[client]);
  if(action==MenuAction_End)
    CloseHandle(menu);
}





stock ExampleMenu(client) { //remove stock modifier and rename

  new Handle:menu=CreateMenu(ExampleMenuHandler); //EDIT !!
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc todo menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  decl String:buffer[200];
  Format(buffer, sizeof(buffer), "%T", "My menu item", client);
  AddMenuItem(menu,"meta-data",buffer,ITEMDRAW_DISABLED);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public ExampleMenuHandler(Handle:menu, //rename
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    //insert here

    /*** Comment out to treat meta-data as functions with 'client' argument ***/
    /*

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

    */
  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      //Go previous menu?
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}





