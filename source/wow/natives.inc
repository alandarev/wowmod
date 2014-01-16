/*
* File: natives.inc
* Description: Natives
* Author(s): Stinkyfax
*/

#pragma semicolon 1


public InitNatives()	{
  CreateNative("wow_getTempPlayerRating", Native_TempPlayerRating);
  CreateNative("wow_scrambleTeams", Native_ScrambleTeams);
  CreateNative("wow_hasPremiumModule", Native_premiumModule);
  CreateNative("wow_getSilver", Native_getSilver);
  CreateNative("wow_setSilver", Native_setSilver);

  CreateNative("wow_getXP", Native_getXP);
  CreateNative("wow_setXP", Native_setXP);

  CreateNative("wow_getClass", Native_getClass);
  CreateNative("wow_getTalentTrees", Native_getTalentTrees);
}

public Native_premiumModule(Handle:plugin, numParams) {
  new client = GetNativeCell(1);
  if(!IsClientInGame(client))
    return 0;
  if(playerloaded[client]!=1)
    return 0;
  new PDModule:module = PDModule:GetNativeCell(2);
  return _:isClientPDModule(client, module);
}

public Native_getClass(Handle:plugin, numParams)  {
  new client = GetNativeCell(1);
  if(!IsClientInGame(client)) return 0;
  if(playerloaded[client]!=1) return 0;
  return GetClass(client);
}

public Native_getTalentTrees(Handle:plugin, numParams)  {
  new client = GetNativeCell(1);
  if(!IsClientInGame(client)) return 0;
  if(playerloaded[client]!=1) return 0;
  new spec = getActiveSpec(client);
  new bool:trees[3];
  for (new i=1; i<4; i++)
    trees = wcGetTreeTotal(client, spec,i) >= 6;
  new sum;
  for (new i=0; i<3; i++) {
    if(trees[i])  sum += 1 << i;
  }
  return sum;
}


public Native_getSilver(Handle:plugin, numParams)  {
  new client = GetNativeCell(1);
  if(!IsClientInGame(client))
    return 0;
  if(playerloaded[client]!=1)
    return 0;
  return GetMoney(client);
}


public Native_getXP(Handle:plugin, numParams)  {
  new client = GetNativeCell(1);
  if(!IsClientInGame(client))
    return 0;
  if(playerloaded[client]!=1)
    return 0;
  return GetXp(client);
}

public Native_setSilver(Handle:plugin, numParams) {
  new client = GetNativeCell(1);
  if(!IsClientInGame(client))
    return;
  if(playerloaded[client]!=1)
    return;
  new xp = GetNativeCell(2);
  if(xp < 0)
    return;
  wcSetMoney(client, xp);
}

public Native_setXP(Handle:plugin, numParams) {
  new client = GetNativeCell(1);
  if(!IsClientInGame(client))
    return;
  if(playerloaded[client]!=1)
    return;
  new xp = GetNativeCell(2);
  if(xp < 0)
    return;
  new modify = xp - GetXp(client);
  if(modify>0)
    wcGiveXp(client, modify, false);
  else
    wcSetStaticXp(client, xp);
}

public Native_TempPlayerRating(Handle:plugin, numParams)
{
  /* Retrieve the first parameter we receive */
  new client = GetNativeCell(1);
  /* Retrieve the second parameter we receive */
  new team = GetNativeCell(2);

  return _:getTempPlayerRating(client, team);
}

public Native_ScrambleTeams(Handle:plugin, numParams)	{
	scrambleTeams();
}

public Native_GetPremiumData(Handle:plugin, numParams)	{
	scrambleTeams(); //This shouldn't be scrambleTeams should it?
}

stock IntToStringfloat(val, String:string[], length) {
  new front = val / 100;
  new back = val % 100;
  decl String:mid[5];
  if(back < 10) {
    Format(mid, sizeof(mid), "0");
  }
  else
    Format(mid, sizeof(mid), "");
  Format(string, length, "%d.%s%d", front, mid, back);
}
