//Capture the flag for WoWmod by Arioni
//TODO:
//- Make the flag follow you
//- Models for flags
//- Rewards
//- Testing

new g_iScore[2]; // 0 == T, 1 == CT
new g_iCaptures[65]; //How many flags you have captured during the event
new g_iFlagReturns[65]; //How many flags you have returned during the event
new g_iFlagCarrierKill[65]; //How many flag carriers you have killed during the event
new g_iFlag[2]; //0 is T, 1 is CT

new Float:g_fAllianceBase[3];
new Float:g_fHordeBase[3];
new g_iFlagHomeH=1; //Is the Horde flag home?
new g_iFlagHomeA=1; //Is the Alliance flag home?
new g_iVictories;
new g_iFollowingFlag[2];


stock CTFStart(Float:location[3],Float:location2[3]) //Begins CTF
{
  if(g_iVictories>=5)
  {
    SetCtf(false);
    g_iVictories=0;
    EndCTFEvent();
  }
  else if(IsCtf())
  {
    StartCTFEvent();
    //Horde Flag spawning
    g_fHordeBase[0]=location[0];
    g_fHordeBase[1]=location[1];
    g_fHordeBase[2]=location[2];
    SpawnHordeFlag(location);
    g_iFlagHomeH=1;

    //Alliance Flag spawning
    g_fAllianceBase[0]=location2[0];
    g_fAllianceBase[1]=location2[1];
    g_fAllianceBase[2]=location2[2];
    SpawnAllianceFlag(location2);
    g_iFlagHomeA=1;
  }
}

stock SpawnHordeFlag(Float:location[3])
{
  SpawnCTFHorde(location);
  new Handle:dp=CreateDataPack();
  WritePackCell (dp,2);
  WritePackCell(dp,g_RoundCount);
  WritePackFloat(dp,location[0]);
  WritePackFloat(dp,location[1]);
  WritePackFloat(dp,location[2]);
  CreateTimer(0.1, CTFPick, dp, TIMER_REPEAT);
}
stock SpawnAllianceFlag(Float:location2[3])
{
  SpawnCTFAlliance(location2);
  new Handle:dp=CreateDataPack();
  WritePackCell (dp,3);
  WritePackCell(dp,g_RoundCount);
  WritePackFloat(dp,location2[0]);
  WritePackFloat(dp,location2[1]);
  WritePackFloat(dp,location2[2]);
  CreateTimer(0.1, CTFPick, dp, TIMER_REPEAT);
}
public Action:CTFPick(Handle:timer, Handle:datapack)
{
  decl Float:location[3];
  ResetPack(datapack);
  new team=ReadPackCell(datapack);
  new round=ReadPackCell(datapack);
  location[0]=ReadPackFloat(datapack);
  location[1]=ReadPackFloat(datapack);
  location[2]=ReadPackFloat(datapack);
  if(g_RoundCount!=round)
    return Plugin_Stop;
  new target;
  if(team==2)
    target = GetNearestClient("@ct",location,75.0);
  if(team==3)
    target = GetNearestClient("@t",location,75.0);
  if(target>0)
  {
    decl String:name[255];
    GetClientName(target,name,sizeof(name));
    g_iCarry[target]=1;
    new Handle:dp=CreateDataPack();
    WritePackCell (dp,target);
    CreateTimer(0.1, CTFCarry, dp, TIMER_REPEAT);
    g_iCarry[target]=1;
    for(new x=1;x<=GetMaxClients();x++)
    {
      if(IsClientInGame(x))
      {
        if(team==3)
        {
          WCMessage(x,"CTF Picked Alliance",x,name);
          g_iFlagHomeA=0;
        }
        if(team==2)
        {
          WCMessage(x,"CTF Picked Horde",x,name);
          g_iFlagHomeH=0;
        }
      }
    }
    if(team==2)
      KillFlag(g_iFlag[0]);
    if(team==3)
      KillFlag(g_iFlag[1]);
    return Plugin_Stop;
  }
  return Plugin_Continue;
}
public Action:CTFCarry(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client=ReadPackCell(datapack);
  decl Float:location[3];
  if(IsClientInGame(client))
    GetClientAbsOrigin(client,location);
  if(bVanish[client] || IsMageInvis(client) || !IsPlayerAlive(client) || UnableToVictim[client]==true)
  {
    CTFDrop(client,location); //Drops the flag
    //Add kill for following model

    return Plugin_Stop;
  }
  if(g_iCarry[client]==0)
    return Plugin_Stop;
  new team=GetClientTeam(client);
  //Add following model here
  if(!IsCtf())
  {
    //Add kill for following model
    return Plugin_Stop;
  }
  g_iCarry[client]=1;
  if(team==2)
  {
    new amount;
    new check;
    new targets[MAXPLAYERS];
    amount=GetNearClients("@t",g_fHordeBase,targets,75.0);
    for (new x=0;x<=(amount-1);x++)
    {
      if(client==targets[x])
        check=1;
    }
    if(check>0)
    {
      if(g_iFlagHomeH>0)
      {
        PrintToChatAll("Hflag was home");
        GiveScore(2);
        g_iFlagHomeA=1;
        g_iCaptures[client]+=1;
        g_iCarry[client]=0;
        //Add kill for following model here

        SpawnAllianceFlag(g_fAllianceBase);
        return Plugin_Stop;
      }
    }
  }
  if(team==3)
  {
    new amount;
    new check;
    new targets[MAXPLAYERS];
    amount=GetNearClients("@ct",g_fAllianceBase,targets,75.0);
    for (new x=0;x<=(amount-1);x++)
    {
      if(client==targets[x])
        check=1;
    }
    if(check>0)
    {
      if(g_iFlagHomeA>0)
      {
        PrintToChatAll("aflag was home");
        GiveScore(3);
        g_iFlagHomeH=1;
        g_iCaptures[client]+=1;
        g_iCarry[client]=0;
        //Add kill for following model here

        SpawnHordeFlag(g_fHordeBase);
        return Plugin_Stop;
      }
    }
  }
  new Handle:datapack2=CreateDataPack();
  WritePackFloat(datapack2,location[0]);
  WritePackFloat(datapack2,location[1]);
  WritePackFloat(datapack2,location[2]);
  WritePackCell(datapack2,team);
  CreateTimer(0.1,SpawnFollowingFlag,datapack2,TIMER_HNDL_CLOSE);
  return Plugin_Continue;
}
public Action:SpawnFollowingFlag(Handle:timer, Handle:datapack) //Only repeat this once!
{
  decl Float:location[3];
  ResetPack(datapack);
  location[0] = ReadPackFloat(datapack); 
  location[1] = ReadPackFloat(datapack); 
  location[2] = ReadPackFloat(datapack); 
  new team = ReadPackCell(datapack);
  FollowingFlag(location,team);
}

stock FollowingFlag(Float:location[3], team=3) {
  if(team==3)   { 
    SetEntProp( g_iFollowingFlag[0], Prop_Data, "m_nModelIndex", g_ctf_horde ); //Checks for the carrier's team; if the carrier is alliance, it spawns the horde flag
    //SetEntProp( g_iFollowingFlag[0], Prop_Data, "m_nModelIndex", "models/cf_models_src/ctf_80rd3.mdl" ); //Checks for the carrier's team; if the carrier is alliance, it spawns the horde flag
    g_iFollowingFlag[0] = CreateEntityByName( "prop_physics_override" );
    SetEntityMoveType( g_iFollowingFlag[0], MOVETYPE_NONE );

    DispatchSpawn( g_iFollowingFlag[0] );
    SetEntProp( g_iFollowingFlag[0], Prop_Send, "m_nSolidType", 2 );
    SetEntProp( g_iFollowingFlag[0], Prop_Data, "m_takedamage", 2 );
    SetEntProp( g_iFollowingFlag[0], Prop_Data, "m_iHealth", 30000000 );
    SetEntProp( g_iFollowingFlag[0], Prop_Data, "m_CollisionGroup", 2, 4);
    AcceptEntityInput(g_iFollowingFlag[0], "DisableMotion");

    TeleportEntity( g_iFollowingFlag[0], location, NULL_VECTOR, NULL_VECTOR );
    
    new Handle:datapack=CreateDataPack();
    WritePackCell(datapack,g_iFollowingFlag[0]);
    CreateTimer(0.1,DestroyFlag,datapack,TIMER_HNDL_CLOSE);

    return g_iFollowingFlag[0];
  }
  else  {
    SetEntProp( g_iFollowingFlag[1], Prop_Data, "m_nModelIndex", g_ctf_alliance );
    //SetEntProp( g_iFollowingFlag[1], Prop_Data, "m_nModelIndex", "models/cf_models_src/ctf_41114nc3.mdl" );
    g_iFollowingFlag[1] = CreateEntityByName( "prop_physics_override" );
    SetEntityMoveType( g_iFollowingFlag[1], MOVETYPE_NONE );

    DispatchSpawn( g_iFollowingFlag[1] );
    SetEntProp( g_iFollowingFlag[1], Prop_Send, "m_nSolidType", 2 );
    SetEntProp( g_iFollowingFlag[1], Prop_Data, "m_takedamage", 2 );
    SetEntProp( g_iFollowingFlag[1], Prop_Data, "m_iHealth", 30000000 );
    SetEntProp( g_iFollowingFlag[1], Prop_Data, "m_CollisionGroup", 2, 4);
    AcceptEntityInput(g_iFollowingFlag[1], "DisableMotion");

    TeleportEntity( g_iFollowingFlag[1], location, NULL_VECTOR, NULL_VECTOR );
    
    new Handle:datapack2=CreateDataPack();
    WritePackCell(datapack2,g_iFollowingFlag[1]);
    CreateTimer(0.1,DestroyFlag,datapack2,TIMER_HNDL_CLOSE);
   
    return g_iFollowingFlag[1];  
  }
}
public Action:DestroyFlag(Handle:timer, Handle:datapack) //Only repeat this once!
{
  ResetPack(datapack);
  new flag = ReadPackCell(datapack);
  KillFollowingFlag(flag);
}
stock bool:KillFollowingFlag(ent) {
  if(ent<=0) {
    return false;
  }
  if(IsValidEntity(ent)) {
    RemoveEdict(ent);
    return true;
  }
  return false;
}
stock CTFDrop(client,Float:location[3])
{
  g_iCarry[client]=0;
  decl String:name[255];
  GetClientName(client,name,sizeof(name));
  new team=GetClientTeam(client);
  for(new x=1;x<=GetMaxClients();x++)
    {
    if(IsClientInGame(x))
    {
      if(team==3)
        WCMessage(x,"Horde Flag Drop",x,name);
      if(team==2)
        WCMessage(x,"Alliance Flag Drop",x,name);
    }
  }
  if(team==3)
    SpawnCTFHorde(location);
  if(team==2)
    SpawnCTFAlliance(location);
  new Handle:dp=CreateDataPack();
  WritePackCell (dp,team);
  WritePackCell(dp,g_RoundCount);
  WritePackFloat(dp,location[0]);
  WritePackFloat(dp,location[1]);
  WritePackFloat(dp,location[2]);
  CreateTimer(0.1, CTFDropTick, dp, TIMER_REPEAT);
}
public Action:CTFDropTick(Handle:timer, Handle:datapack)
{
  decl Float:location[3];
  ResetPack(datapack);
  new team=ReadPackCell(datapack);
  new round=ReadPackCell(datapack);
  location[0]=ReadPackFloat(datapack);
  location[1]=ReadPackFloat(datapack);
  location[2]=ReadPackFloat(datapack);
  if(g_RoundCount!=round)
    return Plugin_Stop;
  new target = GetNearestClient("@all",location,75.0);
  if(target>0)
  {
    decl String:name[255];
    GetClientName(target,name,sizeof(name));
    new targetteam=GetClientTeam(target);
    if(targetteam==team)
    {
      g_iCarry[target]=1;
      new Handle:dp=CreateDataPack();
      WritePackCell (dp,target);
      CreateTimer(0.1, CTFCarry, dp, TIMER_REPEAT);
      for(new x=1;x<=GetMaxClients();x++)
        if(IsClientInGame(x))
        {
          if(targetteam==2)
            WCMessage(x,"CTF Carry Alliance Flag",x,name); //Carries the alliance flag
          if(targetteam==3)
            WCMessage(x,"CTF Carry Horde Flag",x,name); //Carries the horde flag
        }
    }
    if(targetteam!=team)
    {
      CTFReturn(targetteam,target);
    }
    return Plugin_Stop;
  }
  return Plugin_Continue;
}
stock CTFReturn(team,client)
{
  decl String:name[255];
  GetClientName(client,name,sizeof(name));
  for(new x=1;x<=GetMaxClients();x++)
    if(IsClientInGame(x))
    {
      if(team==2)
      {
        WCMessage(x,"CTF Return Horde Flag",x,name);
      }
      if(team==3)
      {
        WCMessage(x,"CTF Return Alliance Flag",x,name);
      }
    }
  g_iFlagReturns[client]+=1;
  if(team==2)
  {
    KillFlag(g_iFlag[0]);
    SpawnHordeFlag(g_fHordeBase);
    g_iFlagHomeH=1;
  }
  if(team==3)
  {
    KillFlag(g_iFlag[1]);
    SpawnAllianceFlag(g_fAllianceBase);
    g_iFlagHomeA=1;
  }

}
stock SpawnCTFHorde(Float:location[3])
{
  g_iFlag[0] = CreateEntityByName( "prop_physics_override" );
  if(!IsValidEntity( g_iFlag[0]))
  {
    PrintToChatAll("Failed at SpawnCTFHorde");
    return;
  }
  SetEntityModel(g_iFlag[0], "models/cf_models_src/ctf_80rd3.mdl");
  for(new x=0;x<1;x++)
  {
    SetEntityMoveType( g_iFlag[x], MOVETYPE_NONE );
    DispatchSpawn( g_iFlag[x] );
    SetEntProp( g_iFlag[x], Prop_Send, "m_nSolidType", 2 );
    SetEntProp( g_iFlag[x], Prop_Data, "m_iHealth", 1000 );
    AcceptEntityInput( g_iFlag[x], "DisableMotion");
    if(x==0)
      TeleportEntity( g_iFlag[x], location, NULL_VECTOR, NULL_VECTOR );
  }
  return;
}
stock SpawnCTFAlliance(Float:location[3])
{
  g_iFlag[1] = CreateEntityByName( "prop_physics_override" );
  if(!IsValidEntity( g_iFlag[1]))
  {
    PrintToChatAll("Failed at SpawnCTFAlliance");
    return;
  }
  SetEntityModel(g_iFlag[1], "models/cf_models_src/ctf_41114nc3.mdl");
  for(new x=1;x<2;x++)
  {
    SetEntityMoveType( g_iFlag[x], MOVETYPE_NONE );
    DispatchSpawn( g_iFlag[x] );
    SetEntProp( g_iFlag[x], Prop_Send, "m_nSolidType", 2 );
    SetEntProp( g_iFlag[x], Prop_Data, "m_iHealth", 1000 );
    AcceptEntityInput( g_iFlag[x], "DisableMotion");
    if(x==1)
      TeleportEntity( g_iFlag[x], location, NULL_VECTOR, NULL_VECTOR );
  }
  return;
}
stock bool:KillFlag(ent) {
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
stock StartCTFEvent() //Runs required commands, e.g roundtime, deathmatch, teambalance
{
  new Handle:round=FindConVar("mp_roundtime");
  g_RoundTime=GetConVarFloat(round);
  SetConVarFloat(round, 6.0);
  //for(new x=1;x<=GetMaxClients();x++)
    //if(IsClientInGame(x))
      //WCMessage(x,"CTF Start",x);
  CloseHandle(round);
}
stock EndCTFEvent() //Ends required commands, e.g roundtime, deathmatch, teambalance
{
  new Handle:round=FindConVar("mp_roundtime");
  SetConVarFloat(round, g_RoundTime);
  //for(new x=1;x<=GetMaxClients();x++)
    //if(IsClientInGame(x))
      //WCMessage(x,"CTF Over",x);
  CloseHandle(round);
}
stock GiveScore(team)
{
  if(team==2)
  {
    g_iScore[0]+=1;
    for(new x=1;x<=GetMaxClients();x++)
      if(IsClientInGame(x) && GetClientTeam(x)==2)
      {
        if(g_iScore[0]==g_iScore[1])
          WCMessage(x,"Horde Score Equal",x,g_iScore[1],g_iScore[0]);
        if(g_iScore[0]>g_iScore[1])
          WCMessage(x,"Horde Score Winning",x,g_iScore[1],g_iScore[0]);
        if(g_iScore[0]<g_iScore[1])
          WCMessage(x,"Horde Score Losing",x,g_iScore[1],g_iScore[0]);
      }
    if(g_iScore[0]>=3)
    {
      for(new x=1;x<=GetMaxClients();x++)
        if(IsClientInGame(x))
          WCMessage(x,"Horde Victory",x,g_iScore[0],g_iScore[1]);
       //Add round ending etc. here
      GiveReward();
    }
  }
  if(team==3)
  {
    g_iScore[1]+=1;
    for(new x=1;x<=GetMaxClients();x++)
      if(IsClientInGame(x) && GetClientTeam(x)==3)
      {
        if(g_iScore[0]==g_iScore[1])
          WCMessage(x,"Alliance Score Equal",x,g_iScore[0],g_iScore[1]);
        if(g_iScore[0]>g_iScore[1])
          WCMessage(x,"Alliance Score Winning",x,g_iScore[0],g_iScore[1]);
        if(g_iScore[0]<g_iScore[1])
          WCMessage(x,"Alliance Score Losing",x,g_iScore[0],g_iScore[1]);
      }
    if(g_iScore[0]>=3)
    {
      for(new x=1;x<=GetMaxClients();x++)
        if(IsClientInGame(x))
          WCMessage(x,"Alliance Victory",x,g_iScore[1],g_iScore[0]);
      //Add round ending etc. here
      g_iScore[0]=0;
      g_iScore[1]=0;
      GiveReward();
    }
  }
  if(g_iVictories>=5)
  {
    if(g_iScore[0]>g_iScore[1])
      for(new x=1;x<=GetMaxClients();x++)
        if(IsClientInGame(x))
          WCMessage(x,"Horde End Victory",x,g_iScore[0],g_iScore[1]);
    if(g_iScore[1]>=g_iScore[0])
      for(new x=1;x<=GetMaxClients();x++)
        if(IsClientInGame(x))
          WCMessage(x,"Alliance End Victory",x,g_iScore[1],g_iScore[0]);
  }
}
stock GiveReward() //Gives a reward based on your efforts throughout the match and how many points your team had
{
  for(new x=1;x<=GetMaxClients();x++)
  {
    if(IsClientInGame(x))
    {
      new team=GetClientTeam(x);
      new score = 20; //Base
      if(team==2)
        score+=g_iScore[0]*15;
      if(team==3)
        score+=g_iScore[1]*15;
      score+=g_iFlagReturns[x]*5;
      score+=g_iCaptures[x]*15;
      score+=g_iFlagCarrierKill[x]*5;
      new silver = score * 15; //Just for testing
      wcSetMoney(x,(GetMoney(x)+silver));
    }
  }
}
