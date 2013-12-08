/*
* File: rank.inc
* Description: This file holds functions for ranking
* Author(s): Stinkyfax
*/

new Float:g_fClientRating[65];

RankDownloads() {
  AddFileToDownloadsTable("materials/wow/rank1.vtf");
  AddFileToDownloadsTable("materials/wow/rank1.vmt");
  AddFileToDownloadsTable("materials/wow/rank2.vtf");
  AddFileToDownloadsTable("materials/wow/rank2.vmt");
  AddFileToDownloadsTable("materials/wow/rank3.vtf");
  AddFileToDownloadsTable("materials/wow/rank3.vmt");
  AddFileToDownloadsTable("materials/wow/rank4.vtf");
  AddFileToDownloadsTable("materials/wow/rank4.vmt");
  AddFileToDownloadsTable("materials/wow/rank5.vtf");
  AddFileToDownloadsTable("materials/wow/rank5.vmt");
  AddFileToDownloadsTable("materials/wow/rank6.vtf");
  AddFileToDownloadsTable("materials/wow/rank6.vmt");
  AddFileToDownloadsTable("materials/wow/rank7.vtf");
  AddFileToDownloadsTable("materials/wow/rank7.vmt");
  AddFileToDownloadsTable("materials/wow/rank8.vtf");
  AddFileToDownloadsTable("materials/wow/rank8.vmt");
  AddFileToDownloadsTable("materials/wow/rank9.vtf");
  AddFileToDownloadsTable("materials/wow/rank9.vmt");
  AddFileToDownloadsTable("materials/wow/rank10.vtf");
  AddFileToDownloadsTable("materials/wow/rank10.vmt");
  AddFileToDownloadsTable("materials/wow/rank11.vtf");
  AddFileToDownloadsTable("materials/wow/rank11.vmt");
  AddFileToDownloadsTable("materials/wow/rank12.vtf");
  AddFileToDownloadsTable("materials/wow/rank12.vmt");
  AddFileToDownloadsTable("materials/wow/rank13.vtf");
  AddFileToDownloadsTable("materials/wow/rank13.vmt");
  AddFileToDownloadsTable("materials/wow/rank14.vtf");
  AddFileToDownloadsTable("materials/wow/rank14.vmt");
  AddFileToDownloadsTable("materials/wow/rank15.vtf");
  AddFileToDownloadsTable("materials/wow/rank15.vmt");
}



PrecacheRanks() {
  decl String:path[255];
  for(new i=0; i<15; i++) {
    Format(path, sizeof(path), "materials/wow/rank%i.vmt", i+1);
    g_mRanks[i] = PrecacheModel(path);
  }
}

public Action:ShowRank(Handle:tmr, any:client)  {
  DisplayRank(client, GetRank(client));
  return Plugin_Handled;
}

DisplayRank(target, rank) {
  if(rank>15)
    rank=15;
  if(rank<1)
    rank=1;

  if(!IsClientInGame(target))
    return;
  if(!IsPlayerAlive(target))
    return;

  decl String:path[255];
  Format(path, sizeof(path), "materials/wow/rank%i.vmt", rank);

  new ent = spawnPlayerAttachedEntity(target, path);
  if(ent <= 0 ) return;
  LiveEntity(ent, 5.5);

}

stock spawnPlayerAttachedEntity(client, String:path[]) {

  if(!IsClientInGame(client) || !IsPlayerAlive(client)) return -1;

  decl String:iTarget[16];
  Format(iTarget, 16, "client%d", client);
  DispatchKeyValue(client, "targetname", iTarget);

  decl Float:Origin[3];
  GetClientEyePosition(client,Origin);
  Origin[2] += 38.0;

  new Ent = CreateEntityByName("env_sprite");
  if(!Ent) return -1;

  SetEntityModel(Ent, path);
  /*DispatchKeyValue(Ent, "model", sprite);*/
  DispatchKeyValue(Ent, "classname", "env_sprite");
  DispatchKeyValue(Ent, "spawnflags", "1");
  DispatchKeyValue(Ent, "scale", "0.9");
  DispatchKeyValue(Ent, "rendermode", "1");
  DispatchKeyValue(Ent, "rendercolor", "255 255 255");
  DispatchSpawn(Ent);
  TeleportEntity(Ent, Origin, NULL_VECTOR, NULL_VECTOR);
  SetVariantString(iTarget);
  AcceptEntityInput(Ent, "SetParent", Ent, Ent, 0);

  return Ent;
}

stock LiveEntity(ent, Float:time=-1.0) {
  new Handle:pack = CreateDataPack();
  WritePackCell(pack, ent);
  if(time > 0)  {
    WritePackFloat(pack, GetEngineTime() + time);
  }
  else  {
    WritePackFloat(pack, -1.0);
  }
  CreateTimer(0.1, SyncEntity, pack, TIMER_REPEAT|TIMER_HNDL_CLOSE);
  if(time > 0.0)
    KillEntityDelay(time, ent);
}

public Action:SyncEntity(Handle:timer, Handle:pack) {
  ResetPack(pack);
  new ent = ReadPackCell(pack);
  new Float:time = ReadPackFloat(pack);
  if(time > 0.0 && GetEngineTime() > time)  {
    return Plugin_Stop;
  }
  if(!IsValidEntity(ent)) {
    return Plugin_Stop;
  }
  new client = GetEntPropEnt(ent, Prop_Data, "m_pParent");
  if(client <= 0 )  {
    KillEntity(ent);
    return Plugin_Stop;
  }
  if(!IsClientConnected(client) || !IsClientInGame(client) || !IsPlayerAlive(client)) {
    KillEntity(ent);
    return Plugin_Stop;
  }

  SyncEntityLocation(ent);


  return Plugin_Continue;
}

stock SyncEntityLocation(ent) {
  new Float:pos[3];
  GetEntPropVector(ent, Prop_Send, "m_vecOrigin", pos);
  TeleportEntity(ent, pos, NULL_VECTOR, NULL_VECTOR); //Yes, we really teleport entities to the same place. Hug Valve for that amazing bug
}

new g_ff_rank[65];
GetRank(client)  {
  return g_ff_rank[client];
}

SetRank(client,amount) {
   g_ff_rank[client]=amount;
}

stock TranslateRank(rank, String:back[], size=25)  {
  Format(back, size, "None");
  switch(rank)  {
    case 1: Format(back, size, "Private");
    case 2: Format(back, size, "Corporal");
    case 3: Format(back, size, "Sergeant");
    case 4: Format(back, size, "Master Sergeant");
    case 5: Format(back, size, "Sergeant Major");
    case 6: Format(back, size, "Knight");
    case 7: Format(back, size, "Knight-Lieutenant");
    case 8: Format(back, size, "Knight-Captain");
    case 9: Format(back, size, "Knight-Champion");
    case 10: Format(back, size, "Lieutenant Commander");
    case 11: Format(back, size, "Commander");
    case 12: Format(back, size, "Marshal");
    case 13: Format(back, size, "Field Marshal");
    case 14: Format(back, size, "Grand Marshal");
    case 15: Format(back, size, "High Warlord");
  }
}




CalcRankForceSQL()  {
   //SQL_TQuery(SSQL, SQLCalcRankProcess, "SELECT playerId, trophies FROM wc_Players WHERE (rating >= 100) ORDER BY rating DESC", DBPrio_Normal);
}

public SQLCalcRankProcess(Handle:db, Handle:query, const String:error[], any:data) {
   if(query!=INVALID_HANDLE)
   {
      new curRank=14;
      new curPlayers=0;
      while(SQL_FetchRow(query))
      {
        new Id=GetSQLDataInt(query,"playedId");
        new trophies=GetSQLDataInt(query,"rating");
        UpdateCurRank(curRank,curPlayers, trophies);
        decl String:buffer[450];
        Format(buffer,sizeof(buffer),"UPDATE wc_Players SET `rank` = '%i' WHERE playerId = %i", curRank, Id);
        SQL_TQuery(SSQL, Tinserted, buffer, 1507);
        new client = IsPlayerOnline(Id);
        if(client > 0) {
          //SetTrophies(client,trophies);
        }
      }
   }
   else
      LogError("error #rank.142 = %s", error);
}

UpdateCurRank(&rank, &amount, &trophies)  {
  if(rank>=10)  {
    if(amount > (14-rank))  {
      amount=0;
      rank--;
    }
  }
  new max = MaxRank(trophies);
  if(max < rank)
    rank=max;
  //trophies = RoundToFloor(float(trophies)*0.96);
  amount++;
}

MaxRank(trophies) {
  if(trophies >= 1025)
    return 14;
  if(trophies >= 980)
    return 13;
  if(trophies >= 920)
    return 12;
  if(trophies >= 870)
    return 11;
  if(trophies >= 820)
    return 10;
  if(trophies >= 750)
    return 9;
  if(trophies >= 700)
    return 8;
  if(trophies >= 600)
    return 7;
  if(trophies >= 500)
    return 6;
  if(trophies >= 400)
    return 5;
  if(trophies >= 300)
    return 4;
  if(trophies >= 200)
    return 3;
  if(trophies >= 100)
    return 2;
  return 1;
}


UpdatePlayerRating(client, Float:rating)  {
  g_fClientRating[client]=rating;
}

Float:GetPlayerRating(client) {
  if(client==0)
    return 0.0;
  return g_fClientRating[client];
}
