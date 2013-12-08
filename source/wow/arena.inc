/* ========================================================================== */
/*                                                                            */
/*   arena.inc                                                                */
/*   (c) 2009 Stinkyfax                                                       */
/*                                                                            */
/*   Is used for functions which support arena map and events                 */
/*                                                                            */
/* ========================================================================== */
//mp time limit to 0
new g_ArenaSize;
new g_ArenaTeam[2][5];
new bool:g_ArenaConfirms[2][5];
new Handle:g_ArenaConfirmMenu=INVALID_HANDLE;

new g_OldTimeLimit;
new g_ArenaPoints[2];

new g_ArenaRound, g_ArenaTeamsWon[2];

ChangeTimeLimit(bool:restore) {
   new Handle:time=FindConVar("mp_timelimit");
   if(restore) {
      SetConVarInt(time,g_OldTimeLimit);
   }
   else
   {
      g_OldTimeLimit=GetConVarInt(time);
      SetConVarInt(time,0);
   }
   CloseHandle(time);
}

ArenaConfirmTeams(client)  {
   if(client!=g_ArenaLeader)  {
      WCMessage(client,"arena not leader",client);
      return;
   }
   ResetConfirms();
   if(g_ArenaSize==0)   {
      WCMessage(client,"wc arena size",client);
      return;
   }
   if(!CheckTeamsFull()) {
      WCMessage(client,"arena teams not full",client);
      return;
   }
   BuildConfirmMenu();
   g_ArenaLocked=true;
   ChangeTimeLimit(false);
   CreateTimer(15.5,ArenaConfirmTimer,0);
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))   {
         DisplayMenu(g_ArenaConfirmMenu,i,15);
         WCMessage(i,"arena confirm started",i);
      }
}

public Action:ArenaConfirmTimer(Handle:timer, any:trash) {
   if(!CheckConfirms()) {
      g_ArenaLocked=false;
      ChangeTimeLimit(true);
      for(new i=1;i<=GetMaxClients();i++)
         if(IsClientInGame(i))
            WCMessage(i,"arena confirm failed",i);
   }
   else  {
      if(!CheckTeamsFull())   {
         g_ArenaLocked=false;
         ChangeTimeLimit(true);
         for(new i=1;i<=GetMaxClients();i++)
            if(IsClientInGame(i))
               WCMessage(i,"arena confirm failed",i);

      }
      else  {
         for(new i=1;i<=GetMaxClients();i++)
            if(IsClientInGame(i))
               WCMessage(i,"arena confirm done",i);
         g_ArenaLocked=true;
         ChangeTimeLimit(true);
         StartCounting();
         //end
      }
   }
}

CheckConfirms()   {
   new bool:full=true;
   for(new y=0;y<2;y++)
      for(new x=0;x<g_ArenaSize;x++)   {
         if(!g_ArenaConfirms[y][x])  {
            full=false;
            break;
         }
      }
   return full;
}

CheckTeamsFull()  {
   new bool:full=true;
   for(new y=0;y<2;y++)
      for(new x=0;x<g_ArenaSize;x++)   {
         new user=g_ArenaTeam[y][x];
         if(user<=0)
            full=false;
         else  if(!IsClientInGame(user))  full=false;
      }
   return full;
}

ResetConfirms()   {
   for(new y=0;y<2;y++)
      for(new x=0;x<g_ArenaSize;x++)
         g_ArenaConfirms[y][x]=false;
}

BuildConfirmMenu()   {
   if(g_ArenaConfirmMenu!=INVALID_HANDLE)
      CloseHandle(g_ArenaConfirmMenu);
   g_ArenaConfirmMenu=CreateMenu(ArenaConfirmHandle);
   decl String:title[128],String:line[255];
   Format(title,sizeof(title),"%T","wc arena confirm",LANG_SERVER);
   SetMenuTitle(g_ArenaConfirmMenu,title);
   //horde
   Format(line,sizeof(line),"%T:","arena horde",LANG_SERVER);
   for(new i=0;i<g_ArenaSize;i++)   {
      Format(line,sizeof(line),"%s\n",line);
      decl String:name[30];
      GetArenaMemberName(2,i,name,sizeof(name));
      Format(line,sizeof(line),"%s %s",line,name);
   }
   AddMenuItem(g_ArenaConfirmMenu,"none",line,ITEMDRAW_DISABLED);
   //alliance
   Format(line,sizeof(line),"%T:","arena alliance",LANG_SERVER);
   for(new i=0;i<g_ArenaSize;i++)   {
      Format(line,sizeof(line),"%s\n",line);
      decl String:name[30];
      GetArenaMemberName(3,i,name,sizeof(name));
      Format(line,sizeof(line),"%s %s",line,name);
   }
   AddMenuItem(g_ArenaConfirmMenu,"none",line,ITEMDRAW_DISABLED);
   //end
   Format(line,sizeof(line),"%T", "arena press confirm",LANG_SERVER);
   AddMenuItem(g_ArenaConfirmMenu,"confirm",line);
   Format(line,sizeof(line),"%T", "arena specs",LANG_SERVER);
   AddMenuItem(g_ArenaConfirmMenu,"none",line,ITEMDRAW_DISABLED);
   SetMenuExitBackButton(g_ArenaConfirmMenu,false);
}

public ArenaConfirmHandle(Handle:menu,MenuAction:action,client,slot) {
   if(client<=0 || client > 65)
      return;
   if(action==MenuAction_Select)
   {
      decl String:buffer[50];
      GetMenuItem(menu,slot,buffer,sizeof(buffer));
      if(StrEqual(buffer,"confirm",false))   {
         ArenaClientConfirmed(client);
         return;
      }
   }
   if((action==MenuAction_Cancel) || (action==MenuAction_End) )   {
      decl String:name[40];
      if(IsClientInGame(client))
         GetClientName(client,name,sizeof(name));
      else
         name="Disconnected Player";
      new team;
      if(FindArenaMember(client,team)>=0)
         for(new i=1;i<=GetMaxClients();i++)
            if(IsClientInGame(i))
               WCMessage(i,"arena confirm declined",i,name);
   }
}

ArenaClientConfirmed(client)  {
   new team;
   new id=FindArenaMember(client,team);
   if(id>=0)
      g_ArenaConfirms[team-2][id]=true;
}

ArenaResetTeams(client) {
   if(g_ArenaLeader==client)  {
      for(new i=0;i<5;i++) {
         g_ArenaSize=0;
         g_ArenaTeam[0][i]=0;
         g_ArenaTeam[1][i]=0;
      }
   }
}

RandomizeTeams()  {
   if(g_ArenaSize==0)
      return;
   new players[65];
   new y=0;
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i) && (playerloaded[i]==1))
         players[y++]=i;
   decl bool:swapped;
   new size = y;
   do {
      swapped=false;
      size--;
      for(new i = 0 ; i<size; i++)  {
         if(GetPlayerRating(players[i]) < GetPlayerRating(players[i+1]))  {
              new first = players[i];
              players[i] = players[i+1];
              players[i+1] = first;
              swapped = true;
         }
      }
   } while (swapped);
   size = y;

   new left = g_ArenaSize * 2;
   new team = 0;
   new slots[2];
   new times = 1;
   for(new i=0;i<size;i++) {
      times++;
      if(slots[team]>=g_ArenaSize)
         team = (team+1) % 2;
      CleanArenaMember(players[i]);
      g_ArenaTeam[team][slots[team]++] = players[i];
      if(times==2)   {
         team = (team+1) % 2;
         times=0;
      }
      if((--left) <= 0)
         break;
   }
}

GetArenaMemberName(team,slot,String:name[],size) {
   new id=GetArenaMember(team,slot);
   Format(name,size,"No Player");
   if(id>0) if(IsClientInGame(id))
      GetClientName(id,name,size);
}

FindArenaMember(client,&team) {
   for(new x=0;x<2;x++)
      for(new y=0;y<g_ArenaSize;y++)
         if(g_ArenaTeam[x][y]==client) {
            team=x+2;
            return y;
         }
   return -1;
}

CleanArenaMember(client)   {
   for(new x=0;x<2;x++)
      for(new y=0;y<g_ArenaSize;y++)
         if(g_ArenaTeam[x][y]==client)
            g_ArenaTeam[x][y]=0;
   arenaTeams[client]=0;
}

SetArenaMember(team,slot,id)  {
   CleanArenaMember(id);
   g_ArenaTeam[team-2][slot]=id;
   arenaTeams[id]=team;
}

GetArenaMember(team,slot)  {
   return g_ArenaTeam[team-2][slot];
}

bool:TeamController(client, team)  {
   if(team==1)
      return false;
   if(!g_ArenaRunning)
      return false;
   if(team==0)
      return false;
   if(team==2 || team==3)  {
      if(arenaTeams[client]==team)
         return true;
      else
         return false;
   }
   return false;
}

PlayerLoaded(client) {
  setLoadStage(client, 10);
  bindToServer(client);
  loadKpd(client);

  if(IsClientInGame(client) && IsFakeClient(client))
    BotPreloaded(client);

  //update his rating
  UpdatePlayerRating(client, wcCalculateStats(client));
  //
  NewbieMenu(client);
  if(!CSGO && !IsFakeClient(client) && !SawSlides(client, Slide:SLIDE_WELCOME))
    PrerunSlide(client, Slide:SLIDE_WELCOME);
  InitProfBonuses(client);
  /*resetAutoCasts(client);*/
  defineIsPlayerNew(client);

  if(!IsClientInGame(client))
      return;


  if(!bArenaActive)
      return;
  CheckForLeader();
}

AnnounceLeader()  {
   decl String:name[40];
   if(g_ArenaLeader<=0)
      name="No Leader";
   else  {
      if(!IsClientInGame(g_ArenaLeader))
         name="No Leader";
      else
         GetClientName(g_ArenaLeader,name,sizeof(name));
      }
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         WCMessage(i,"new arena leader",i,name);
}

stock CheckArenaChange(client, bool:disc = true)   {
   new team;
   if(FindArenaMember(client,team)>=0) {
      if(g_ArenaRunning) {
         new lost = ((team-2)+1) % 2;
         new lostPoints = g_ArenaPoints[lost];
         GiveArenaPoints(client,-lostPoints*2,true);
         CheckBreakArena(client,team);
      }
   }
   CleanArenaMember(client);
   if(client==g_ArenaLeader)
      FindNewLeader(client);
}

CheckBreakArena(client, team) {
   new bool:breakArena=true;
   for(new i=0;i<g_ArenaSize;i++)   {
      new c = g_ArenaTeam[team-2][i];
      if( (c > 0) && (c != client) && IsClientInGame(c) && (GetClientTeam(c) == team))
         breakArena=false;
   }
   if(breakArena)
      BreakArena(team);
}


public BreakArena(team)  {
   if(team==2)
      team=3;
   else  if(team==3)
      team=2;
   if(!g_ArenaLocked)
      return;
   g_ArenaLocked=false;
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         WCMessage(i,"arena broke",i);
   FinishArena(team);



}

FinishArena(team)  {
   if(!g_ArenaRunning)
      return;
   decl String:sTeam[30];
   Format(sTeam,sizeof(sTeam),"Unknown");
   if(team==2) {
      Format(sTeam,sizeof(sTeam),"Horde");
      for(new i=1;i<=GetMaxClients();i++)
         if(IsClientInGame(i))
            EmitSoundToClient(i, "wc/arena/HordeWin.wav");
   }
   if(team==3) {
      Format(sTeam,sizeof(sTeam),"Alliance");
      for(new i=1;i<=GetMaxClients();i++)
         if(IsClientInGame(i))
            EmitSoundToClient(i, "wc/arena/AliianceWin.wav");
   }
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         WCMessage(i,"arena team won",i,sTeam,g_ArenaPoints[team-2]);
   for(new i=0;i<2;i++)
      for(new y=0;y<g_ArenaSize;y++)   {
         new client = g_ArenaTeam[i][y];
         if(client>0)   if(IsClientInGame(client))
            if(playerloaded[client]==1)   {
               if(i==(team-2))   {
                  GiveArenaPoints(client,g_ArenaPoints[team-2]);
               }
               else  {
                  GiveArenaPoints(client,-g_ArenaPoints[team-2]);
               }
         }
   }

   StopArena();
}

stock GiveArenaPoints(client,points, bool:cast=false)   {
   if(cast) {
      decl String:name[30];
      GetClientName(client,name,sizeof(name));
      if(points < 0) {
         for(new i=1;i<=GetMaxClients();i++)
            if(IsClientInGame(i))
               WCMessage(i,"arena lost points", i, -points, name);
      }
      else  {
         for(new i=1;i<=GetMaxClients();i++)
            if(IsClientInGame(i))
               WCMessage(i,"arena gained points", i, points, name);
      }

   }
   ModifyPlayerPoints(client,points);
   AddArenasPlayed(client);
}

StopArena() {
   g_ArenaLocked=false;
   g_ArenaRunning=false;
   g_ArenaRound=-1;
   PutPlayersToSpec();
}

public CancelArena() {
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         WCMessage(i,"arena cancelled",i);
   StopArena();
}

CheckForLeader()  { //recoded to compare ratings instead of level
   new Float:maxLevel = GetPlayerRating(g_ArenaLeader);
   new bool:changed=false;
   for(new i=1;i<=GetMaxClients();i++) {
      if((IsClientInGame(i)) && (playerloaded[i]==1) )   {
         new Float:level=GetPlayerRating(i);
         if(GetAdminFlag(GetUserAdmin(i), Admin_Custom6))
          level=10000.0;  //admins with flag 6 get exclusive arena leader
         if(level>maxLevel)   {
            changed=true;
            g_ArenaLeader=i;
            maxLevel=level;
         }
      }
   }
   if(changed)
      AnnounceLeader();
}

FindNewLeader(client)   {
   g_ArenaLeader=0;
   new Float:maxLevel = -1.0;
   new bool:changed=false;
   for(new i=1;i<=GetMaxClients();i++) {
      if((IsClientInGame(i)) && (playerloaded[i]==1) && (i!=client) )   {
         new Float:level=GetPlayerRating(i);
         if(GetAdminFlag(GetUserAdmin(i), Admin_Custom6))
          level=10000.0;  //admins with flag 6 get exclusive arena leader
         if(level>maxLevel)   {
            changed=true;
            g_ArenaLeader=i;
            maxLevel=level;
         }
      }
   }
   if(changed)
      AnnounceLeader();
}

//Arena menu for main server
MainArenaMenu(client,bool:mainServer)  {
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
  new Handle:menu_arena=CreateMenu(MainArenaMenuHandle);
  decl String:title[128],String:line[128];
  Format(title,sizeof(title),"%T","wc main arena",client);
  SetMenuTitle(menu_arena,title);
  //First button
  if(mainServer)  {
   Format(line,sizeof(line),"%T","transfer to arena",client,ARENA_SERVER);
   if(ARENA_REDIRECTION)
      AddMenuItem(menu_arena,ARENA_SERVER,line);
   else  {
      Format(line,sizeof(line),"%T","disabled redirection",client);
      AddMenuItem(menu_arena,ARENA_SERVER,line,ITEMDRAW_DISABLED);
   }
  }
  else   {
   Format(line,sizeof(line),"%T","transfer to main",client,MAIN_SERVER_ADRESS);
   if(ARENA_REDIRECTION)
      AddMenuItem(menu_arena,MAIN_SERVER_ADRESS,line);
   else  {
      Format(line,sizeof(line),"%T","disabled redirection",client);
      AddMenuItem(menu_arena,MAIN_SERVER_ADRESS,line,ITEMDRAW_DISABLED);
   }
  }
  // End of first button "Redirection"
  // Statistics button
  Format(line,sizeof(line),"%T","wc arena stats",client);
  AddMenuItem(menu_arena,line,line,ITEMDRAW_DISABLED);
  //Arena Control Button
  if(!mainServer) {
   Format(line,sizeof(line),"%T","wc arena control",client);
   AddMenuItem(menu_arena,line,line);
  }
  //end
  //Statistics
  Format(line,sizeof(line),"%T", "wc arena points", client, GetPlayerPoints(client), GetArenaPlayed(client));
  AddMenuItem(menu_arena,line,line,ITEMDRAW_DISABLED);
  if(GetArenaPlayed(client)>=10)
   Format(line,sizeof(line),"%T", "wc arena got games", client);
  else
   Format(line,sizeof(line),"%T", "wc arena more games", client, 10-GetArenaPlayed(client));
  AddMenuItem(menu_arena,line,line,ITEMDRAW_DISABLED);
  Format(line,sizeof(line),"%T", "wc honor", client, GetHonor(client));
  AddMenuItem(menu_arena,line,line,ITEMDRAW_DISABLED);
  //end
  SetMenuExitBackButton(menu_arena,true);
  DisplayMenu(menu_arena,client,MENU_TIME_FOREVER);
  }
}

public MainArenaMenuHandle(Handle:menu,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    switch(slot)
    {
      case 0:  {
         new String:ip[50];
         GetMenuItem(menu,slot,ip,sizeof(ip));
         RedirectClient(client,ip,"20");
      }
      case 2:  {
         DisplayArenaMenu(client);
      }
    }
  }
  if(action==MenuAction_Cancel)
    if(slot==MenuCancel_ExitBack)
      WCMenu(client);
  if(action==MenuAction_End)
    CloseHandle(menu);
}

DisplayArenaMenu(client)  {
   new Handle:menu_arena=CreateMenu(DisplayArenaMenuHandle);
   decl String:title[128],String:line[128];
   Format(title,sizeof(title),"%T","arena control",client);
   SetMenuTitle(menu_arena,title);
   new bool:leader=(g_ArenaLeader==client);
   //First button - leader
   decl String:name[40];
   GetClientName(g_ArenaLeader,name,sizeof(name));
   Format(line,sizeof(line),"%T","arena leader",client,name);
   AddMenuItem(menu_arena,"none",line,ITEMDRAW_DISABLED);
   //end
   //size format
   if(g_ArenaSize>0)
      Format(line,sizeof(line),"%T","teams size",client,g_ArenaSize,g_ArenaSize);
   else
      Format(line,sizeof(line),"%T","teams size undefined",client);
   if(leader)
      AddMenuItem(menu_arena,"size",line);
   else
      AddMenuItem(menu_arena,"size",line,ITEMDRAW_DISABLED);
   //end
   //Horde team
   Format(line,sizeof(line),"%T","arena horde",client);
   if(g_ArenaSize>0)
      AddMenuItem(menu_arena,"2",line);
   else
      AddMenuItem(menu_arena,"2",line,ITEMDRAW_DISABLED);
   //end
   //Alliance team
   Format(line,sizeof(line),"%T","arena alliance",client);
   if(g_ArenaSize>0)
      AddMenuItem(menu_arena,"3",line);
   else
      AddMenuItem(menu_arena,"3",line,ITEMDRAW_DISABLED);
   //end
   //Randomize teams
   Format(line,sizeof(line),"%T","arena randomize",client);
   if(g_ArenaSize>0)
      AddMenuItem(menu_arena,"random",line);
   else
      AddMenuItem(menu_arena,"random",line,ITEMDRAW_DISABLED);
   //end
   //Send confirmation
   Format(line,sizeof(line),"%T","arena send confirm",client);
   if(leader)
      AddMenuItem(menu_arena,"send",line);
   else
      AddMenuItem(menu_arena,"send",line,ITEMDRAW_DISABLED);
   //end
   SetMenuExitBackButton(menu_arena,true);
   DisplayMenu(menu_arena,client,MENU_TIME_FOREVER);
}

public DisplayArenaMenuHandle(Handle:menu,MenuAction:action,client,slot)
{
   new bool:leader=(g_ArenaLeader==client);
   if(action==MenuAction_Select)
   {
      decl String:buffer[50];
      GetMenuItem(menu,slot,buffer,sizeof(buffer));
      if(StrEqual(buffer,"size",false))   {
         if(!leader)
            WCMessage(client,"arena not leader",client);
         else  if(g_ArenaLocked) {
            WCMessage(client,"arena settings locked",client);
         }
         else
            ArenaChooseStyle(client);
      }
      else if(StrEqual(buffer,"2",false)) {
         ArenaManageTeam(client,2);
      }
      else if(StrEqual(buffer,"3",false)) {
         ArenaManageTeam(client,3);
      }
      else if(StrEqual(buffer,"send",false)) {
         if(leader)  {
         if(g_ArenaLocked) {
            WCMessage(client,"arena settings locked",client);
         }
         else  {
            ArenaConfirmTeams(client);
         }
         }
         else
            WCMessage(client,"arena not leader",client);
      }
      else if(StrEqual(buffer,"random",false)) {
         if(leader)  {
         if(g_ArenaLocked) {
            WCMessage(client,"arena settings locked",client);
         }
         else  {
            RandomizeTeams();
            DisplayArenaMenu(client);
         }
         }
         else
            WCMessage(client,"arena not leader",client);
      }
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack)
         MainArenaMenu(client,false);
   if(action==MenuAction_End)
      CloseHandle(menu);
}

public ArenaChooseStyle(client)  {
   new bool:leader=(client==g_ArenaLeader);
   if(!leader)
      return;
   new Handle:menu_arena=CreateMenu(ArenaChooseStyleHandle);
   decl String:title[128],String:line[128];
   Format(title,sizeof(title),"%T","wc arena size",client);
   SetMenuTitle(menu_arena,title);
   //Buttons
   Format(line,sizeof(line),"%T","teams size",client,2,2);
   AddMenuItem(menu_arena,"2",line);
   Format(line,sizeof(line),"%T","teams size",client,3,3);
   AddMenuItem(menu_arena,"3",line);
   Format(line,sizeof(line),"%T","teams size",client,4,4);
   AddMenuItem(menu_arena,"4",line);
   Format(line,sizeof(line),"%T","teams size",client,5,5);
   AddMenuItem(menu_arena,"5",line);
   //end
   SetMenuExitBackButton(menu_arena,true);
   DisplayMenu(menu_arena,client,MENU_TIME_FOREVER);
}

public ArenaChooseStyleHandle(Handle:menu,MenuAction:action,client,slot)
{
   new bool:leader=(g_ArenaLeader==client);
   if(action==MenuAction_Select) {
   if(leader)
   {
      decl String:buffer[50];
      GetMenuItem(menu,slot,buffer,sizeof(buffer));
      new size=StringToInt(buffer);
      if(size>1)  {
         ArenaResetTeams(client);
         g_ArenaSize=size;
         WCMessage(client,"arena size modified",client);
         DisplayArenaMenu(client);
      }
   }
   else
      WCMessage(client,"arena not leader",client);
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack)
         DisplayArenaMenu(client);
   if(action==MenuAction_End)
      CloseHandle(menu);
}

public ArenaManageTeam(client,team)  {
   new bool:leader=(client==g_ArenaLeader);
   new Handle:menu_arena=CreateMenu(ArenaManageTeamHandle);
   decl String:title[128],String:line[128];
   if(g_ArenaSize<2)
      return;
   new String:sTeam[20];
   if(team==2)
      sTeam="Horde";
   else
      sTeam="Alliance";
   Format(title,sizeof(title),"%T","wc arena manage team",client,sTeam);
   SetMenuTitle(menu_arena,title);
   //Buttons
   for(new i=0;i<g_ArenaSize;i++)   {
      new String:name[40];
      GetArenaMemberName(team,i,name,sizeof(name));
      Format(line,sizeof(line),"%T","arena manage team slot",client,i,name);
      decl String:sId[30];
      Format(sId,sizeof(sId),"%i %i",team,i);
      if(leader)
         AddMenuItem(menu_arena,sId,line);
      else
         AddMenuItem(menu_arena,sId,line,ITEMDRAW_DISABLED);
   }
   //end
   SetMenuExitBackButton(menu_arena,true);
   DisplayMenu(menu_arena,client,MENU_TIME_FOREVER);
}

public ArenaManageTeamHandle(Handle:menu,MenuAction:action,client,slot)
{
   new bool:leader=(g_ArenaLeader==client);
   if(action==MenuAction_Select) {
   if(leader)
   {
      decl String:buffer[50];
      GetMenuItem(menu,slot,buffer,sizeof(buffer));
      decl String:Steam[10],String:Sid[10];
      new StringID=BreakString(buffer, Steam, sizeof(Steam));
      BreakString(buffer[StringID], Sid, sizeof(Sid));
      new team=StringToInt(Steam);
      new cell=StringToInt(Sid);
      if(cell>=0)  {
         ArenaManageTeamSlot(client,team,cell);
      }
   }
   else
      WCMessage(client,"arena not leader",client);
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack)
         DisplayArenaMenu(client);
   if(action==MenuAction_End)
      CloseHandle(menu);
}

public ArenaManageTeamSlot(client,team,slot)  {
   new bool:leader=(client==g_ArenaLeader);
   new Handle:menu_arena=CreateMenu(ArenaManageTeamSlotHandle);
   decl String:title[128],String:line[128];
   if(!leader)
      return;
   if(g_ArenaSize<2)
      return;
   new String:sTeam[20];
   if(team==2)
      sTeam="Horde";
   else
      sTeam="Alliance";
   new String:name[50];
   GetArenaMemberName(team,slot,name,sizeof(name));
   Format(title,sizeof(title),"%T","wc arena manage team slot",client,slot,name,sTeam);
   SetMenuTitle(menu_arena,title);
   //Buttons
   Format(line,sizeof(line),"%T","empty",client);
   decl String:buffer[255];
   Format(buffer,sizeof(buffer),"%i %i 0",team,slot);
   AddMenuItem(menu_arena,buffer,line);
   for(new i=1;i<=GetMaxClients();i++)   {
      if(IsClientInGame(i))
         if(IsClientAuthorized(i))
         {
            decl String:Srace[255];
            new race=GetClass(i);
            decl String:Sname[20];
            GetClientName(i,Sname,sizeof(Sname));
            wcClassToName(i,race,Srace,sizeof(Srace));
            new targetTeam;

            if(FindArenaMember(i,targetTeam)>=0)   {
               new String:stargetTeam[15];
               if(targetTeam==2)
                  Format(stargetTeam,sizeof(stargetTeam),"Horde");
               else  if(targetTeam==3)
                  Format(stargetTeam,sizeof(stargetTeam),"Alliance");
               else
                  Format(stargetTeam,sizeof(stargetTeam),"Unknown");
               Format(line,sizeof(line),"%s (%s  %i) (%s)",Sname,Srace,GetLevel(i), stargetTeam);
            }
            else
               Format(line,sizeof(line),"%s (%s  %i)",Sname,Srace,GetLevel(i));
            Format(buffer,sizeof(buffer),"%i %i %i",team,slot,i);
            AddMenuItem(menu_arena,buffer,line);
         }
   }
   //end
   SetMenuExitBackButton(menu_arena,true);
   DisplayMenu(menu_arena,client,MENU_TIME_FOREVER);
}

public ArenaManageTeamSlotHandle(Handle:menu,MenuAction:action,client,slot)
{
   new bool:leader=(g_ArenaLeader==client);
   if(action==MenuAction_Select) {
   if(leader)
   {
      decl String:buffer[50];
      GetMenuItem(menu,slot,buffer,sizeof(buffer));
      decl String:Steam[10],String:Sid[10];
      new StringID=BreakString(buffer, Steam, sizeof(Steam));
      StringID+=BreakString(buffer[StringID], Sid, sizeof(Sid));
      new team=StringToInt(Steam);
      new cell=StringToInt(Sid);
      BreakString(buffer[StringID],Sid,sizeof(Sid));
      new user=StringToInt(Sid);
      if((cell>=0) && (team > 1) && (user >= 0))  {
         SetArenaMember(team,cell,user);
         ArenaManageTeam(client,team);
      }
   }
   else
      WCMessage(client,"arena not leader",client);
   }
   if(action==MenuAction_Cancel)
      if(slot==MenuCancel_ExitBack) {
         decl String:buffer[50];
         GetMenuItem(menu,0,buffer,sizeof(buffer));
         decl String:Steam[10];
         BreakString(buffer, Steam, sizeof(Steam));
         new team=StringToInt(Steam);
         ArenaManageTeam(client,team);
      }
   if(action==MenuAction_End)
      CloseHandle(menu);
}

StartCounting()   {
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         EmitSoundToClient(i, "wc/arena/countdown.wav");
   CreateTimer(20.0, CountingEnd, 0);
}

public Action:CountingEnd(Handle:timer, any:trash) {
   if(!CheckTeamsFull())   {
      CancelArena();
   }
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         EmitSoundToClient(i, "quake/play.wav");
   StartArena();
}

StartArena()   {
   g_ArenaRunning=true;
   CalculateArenaPoints();
   PutPlayersToTeams();
   g_ArenaRound=0;
   g_ArenaTeamsWon[0]=0;
   g_ArenaTeamsWon[1]=0;
   CreateTimer(4.0, RestartRound, 0);
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         WCMessage(i,"arena is started",i);
}

public Action:RestartRound(Handle:timer, any:trash)   {
   if(!CheckTeamsFull())
      CancelArena();
   RespawnPlayers();
   ServerCommand("mp_restartgame 1");
   return Plugin_Stop;
}

ArenaRoundEnd(team)  {

   if(!g_ArenaRunning)   {
      return;
   }
   decl String:faction[50];
   new bool:pass=true;
   if(team==2)
      Format(faction,sizeof(faction),"Horde");
   else  if(team==3)
      Format(faction,sizeof(faction),"Alliance");
   else  {
      pass=false;
      Format(faction,sizeof(faction),"Unknown");
   }
   if (g_ArenaRound>0)  if((team>=2) && (team<4))
      g_ArenaTeamsWon[team-2]++;
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i) && (!StrEqual(faction, "Unknown")))   {
         WCMessage(i,"arena round end",i,g_ArenaRound, faction);
         WCMessage(i,"arena scores",i, g_ArenaTeamsWon[0],g_ArenaTeamsWon[1]);
      }
   if(pass) {
      if(g_ArenaRound==0)
         for(new i=1;i<=GetMaxClients();i++)
            if(IsClientInGame(i))
               WCMessage(i,"arena round finish 0",i);
      g_ArenaRound++;
   }
   else
      if(g_ArenaRound==0)
         for(new i=1;i<=GetMaxClients();i++)
            if(IsClientInGame(i))
               WCMessage(i,"arena round still 0",i);
   for(new i=0;i<2;i++) {
      if(g_ArenaTeamsWon[i]>=2)  {
         FinishArena(i+2);
      }
   }
}

PutPlayersToSpec()   {
   for(new t=0;t<2;t++)
      for(new p=0;p<g_ArenaSize;p++)   {
         new client = g_ArenaTeam[t][p];
         if(client>0)  {
            if(IsClientInGame(client))
               ChangeClientTeam(client,1);
            else
               CleanArenaMember(client);
         }
      }
}

RespawnPlayers()  {
   for(new t=0;t<2;t++)
      for(new p=0;p<g_ArenaSize;p++)   {
         new client = g_ArenaTeam[t][p];
         if(client>0)
            CS_RespawnPlayer(client);
      }
}

PutPlayersToTeams()  {
   for(new t=0;t<2;t++)
      for(new p=0;p<g_ArenaSize;p++)   {
         new client = g_ArenaTeam[t][p];
         if(client>0)
            ChangeClientTeam(client,t+2);
      }
}

new g_ff_playerPoints[65];


GetPlayerPoints(client) {
   return g_ff_playerPoints[client];
}

ModifyPlayerPoints(client, value) {
   value += GetPlayerPoints(client);
   SetPlayerPoints(client,value);
}

SetPlayerPoints(client, value)   {
   if(value <= 0 )
      value = 1;
   g_ff_playerPoints[client] = value;
}

CalculateArenaPoints()  {
   new teamPoints[2];
   for(new t=0;t<2;t++)
      for(new p=0;p<g_ArenaSize;p++)
         teamPoints[t]+=GetPlayerPoints(g_ArenaTeam[t][p]);

   for(new i=0;i<2;i++)
      teamPoints[i] =  RoundToNearest ( float(teamPoints[i]) / g_ArenaSize);

   new diffPoints = teamPoints[0] - teamPoints[1];
   diffPoints = RoundToNearest( float(diffPoints) / 55.0 );

   g_ArenaPoints[0] = 15 - diffPoints;
   g_ArenaPoints[1] = 15 + diffPoints;
   new Float:fMulti = 1.0;
   if(g_ArenaSize == 3)
      fMulti = 1.25;
   else if (g_ArenaSize == 5)
      fMulti = 1.7;
   for(new i=0;i<2;i++)
      g_ArenaPoints[i] = RoundToNearest(float(g_ArenaPoints[i])*fMulti);

   for(new i=0;i<2;i++) {
      if(g_ArenaPoints[i]<2)
         g_ArenaPoints[i]=2;
   }
}


new g_ff_arenaPlayed[65];
SetArenaPlayed(client, amount)  {
  g_ff_arenaPlayed[client]=amount;
}

GetArenaPlayed(client)  {
  return g_ff_arenaPlayed[client];
}

AddArenasPlayed(client) {
   decl String:query[255];
   Format(query, 255, "UPDATE wc_Players SET arenaPlayed = arenaPlayed + 1 WHERE playerId = %d", playerids[client]);
   SQL_TQuery(TSQL, Tinserted, query, 1038);
   g_ff_arenaPlayed[client]++;
}

public RedirectClient(client,String:ip[],String:time[])  {
   /*
   This function was partically copied from:
   name = "Advanced Client Redirect",
	author = "Olly/Tobi",
	description = "Redirect client to a new server via a menu.",
	version = "1.0",
	url = "http://www.steamfriends.com"
	P.S. Thanks chief ;-)
   */
	new Handle:top_values = CreateKeyValues("msg");
	KvSetString(top_values, "title", "Connect to this WoW mod Server");
	KvSetNum(top_values, "level", 1);
	KvSetString(top_values, "time", time);
	CreateDialog(client, top_values, DialogType_Msg);
	CloseHandle(top_values);
   		
	new Handle:values = CreateKeyValues("msg");
	KvSetString(values, "time", time);
	KvSetString(values, "title", ip);
	CreateDialog(client, values, DialogType_AskConnect);
	CloseHandle(values);
}

ArenaAllowMenu(client)  {
   if(!g_ArenaLocked)
      return true;
   new team;
   if(FindArenaMember(client,team)>=0)
      return false;
   return true;

}
