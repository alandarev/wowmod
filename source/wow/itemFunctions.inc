/*
* File: itemFunctions.inc
* Description: Items which have functionality
* Author(s): Stinkyfax
*/



Handle:CreateItemsArray()   {
   decl temp[UniqueItem];
   temp[Item_id]=1;
   return CreateArray(sizeof(temp));
}

//Special items
SpecialUsed(client, slot)
{
  KvRewind(items);
  new item = GetSlotItem(client, slot);
  decl String:sitem[10];
  IntToString(item, sitem, 10);
  KvJumpToKey(items, sitem);
  //Get values
  decl String:function[100];
  KvGetString(items, "function", function, 100);
  //TODO properly handle arguments
  new arg1 = KvGetNum(items, "arg1");



  //Call function
  new Function:func = GetFunctionByName(INVALID_HANDLE, function);
  Call_StartFunction(INVALID_HANDLE, func);
  Call_PushCell(client);
  Call_PushCell(item);
  Call_PushCell(arg1);
  if(Call_Finish() != SP_ERROR_NONE)  {
    decl String:err[100];
    Format(err,100,"Could not callback the function: %s",function);
    Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
  }
  else  {
    wcSetStaticSlot(client, slot, 0);
    SetInvUnique(client, slot, 0);
    wcSetStaticSlotAmount(client, slot, 0);
  }
}

public AddExpansion(client, itemId, any:expansion) {
  /*if(expansion == E_DK)  {*/
    /*panelMessage(client, "Warning for Death Knights", "Please be aware that Death Knights \nare unable to use Tomes of XP for first 2 weeks!!!");*/
  /*}*/
  if(HasExpansion(client, expansion))
    ReturnItem(client, itemId, "tried to use known");
  else  {
    _addExpansion(client, expansion);
    decl String:name[50];
    GetClientName(client, name, 50);

    for(new i=1; i <= GetMaxClients(); i++)
      if(IsClientInGame(i))
        WCMessage(i, "player learned expansion", i, name, g_sExpansions[expansion]);
  }
}

public ReadTome(client, item, amount) {
//  if(GetClass(client)==8) {
//    ReturnItem(client, item, "dks tomes");
//    return;
//  }
  WCMessage(client, "read tome", client, amount);
  wcGiveXp(client, amount, false);
}

stock ReturnItem(client, itemId, String:msg[])  {
  new uitem[UniqueItem];
  GetItemProperties(uitem, itemId);
  GiveItem(client, uitem, 1, "refund" );

  WCMessage(client, msg, client);
}

// Random box
RandomBoxUsed(client, slot)
{
  new item = GetSlotItem(client, slot);
  decl Setitems[1000];
  new itemsAmount = GetItemsFromItem(item, Setitems, sizeof(Setitems));
  new newItem = BasicRoll(Setitems, itemsAmount);
  if (newItem > 0)  {
    wcSetStaticSlot(client, slot, 0);
    SetInvUnique(client, slot, 0);
    wcSetStaticSlotAmount(client, slot, 0);
    new uitem[UniqueItem];
    GetItemProperties(uitem, newItem);
    GiveItem(client, uitem, 1, "box");
    WCMessage(client,"item got",client,uitem[Item_Name]);
  }
}

GetItemsFromItem(item, Setitems[], size) {
  decl sets[64];
  new setsAmount = GetSets(item,sets, sizeof(sets));
  new itemsAmount=0;
  while ( (setsAmount >= 1) && (itemsAmount < size) ) {
    new curSet = sets[setsAmount];
    setsAmount--;
    new curItems[200];
    new curItemsAmount = GetItemsOfSet (curSet, curItems, sizeof(curItems));
    for (new x=curItemsAmount;x>0;x--) {
      Setitems[itemsAmount] = curItems [x];
      itemsAmount++;
      if (itemsAmount >= size) break;
    }
  }
  return itemsAmount;
}

GetSets(itemId, sets[], size) {
  KvRewind(items);
  decl String:Sitem[20];
  IntToString(itemId,Sitem,sizeof(Sitem));
  KvJumpToKey(items, Sitem);
  decl String:Ssets[200];
  KvGetString(items,"sets",Ssets,sizeof(Ssets),"0");
  new amount=0;
  if(!StrEqual(Ssets,"0",false))
  {
    decl String:Stemp[255];
    new StringID=BreakString(Ssets, Stemp, sizeof(Stemp));
    amount++;
    sets[1]=StringToInt(Stemp);
    while((StringID>=0) && (amount-1 < size))
    {
        amount++;
        new StringID2=BreakString(Ssets[StringID], Stemp, sizeof(Stemp));
        StringID+=StringID2;
        if(StringID2==-1)
          StringID=-1;
        sets[amount]=StringToInt(Stemp);
    }
  }
  return amount;
}

GetItemsOfSet(set, setitems[], size) {
  KvRewind(wcsets);
  decl String:Sset[500],String:Stemp[20];
  Format(Stemp,sizeof(Stemp),"set%i",set);
  KvGetString(wcsets,Stemp,Sset,sizeof(Sset),"0");
  new StringID=BreakString(Sset, Stemp, sizeof(Stemp));
  new Iamount = 1;
  setitems[Iamount]=StringToInt(Stemp);
  while((StringID>=0) && ((Iamount - 1) < size))
  {
    Iamount++;
    new StringID2=BreakString(Sset[StringID], Stemp, sizeof(Stemp));
    StringID+=StringID2;
    if(StringID2==-1)
      StringID=-1;
    setitems[Iamount]=StringToInt(Stemp);
  }
  return Iamount;
}

BasicRoll (setitems[], size) {
  new rolled=GetRandomInt(0,size-1);
  return setitems[rolled];
}

stock GetItemMessage(itemId,String:message[],size) {
  KvRewind(items);
  decl String:Sitem[20];
  IntToString(itemId,Sitem,sizeof(Sitem));
  KvJumpToKey(items, Sitem);
  KvGetString(items, "message", message, size);
}


//Unique System
new Handle:g_itemsArray;
new g_lastItemUpdate=0;

public PreLoadItems()  {
  g_itemsArray = CreateItemsArray();
  CreateTimer(1.5, ItemsUpdater, 0, TIMER_REPEAT);
}

public Action:ItemsUpdater(Handle:timer, any:trash)  {
  new size = GetArraySize(g_itemsArray);
  if(size <= g_lastItemUpdate)  {
    g_lastItemUpdate=0;
    return Plugin_Continue;
  }
  new item[UniqueItem];
  GetArrayArray(g_itemsArray, g_lastItemUpdate, item[0]);
  new Float:fTemp = item[Item_LastUsage];
  if( (fTemp+1200.0) < GetEngineTime() )  {
    RemoveFromArray(g_itemsArray, g_lastItemUpdate);
    return Plugin_Continue;
  }
  fTemp = item[Item_LastUpdate];
  if( (fTemp + 300.0) < GetEngineTime() ) {
    item[Item_LastUpdate] = GetEngineTime();
    ReloadUniqueItem(item[Item_id]);
  }

  g_lastItemUpdate++;
  return Plugin_Continue;
}

ReloadUniqueItem(itemId)  {
  LoadUniqueItem(itemId);
}

stock GetCharUnique(client, slot) {
  return wcUsers2[client][slot];
}

stock SetCharUnique(client, slot, value)  {
  CheckUnique(value, client);
  wcUsers2[client][slot]=value;
}

stock GetInvUnique(client, slot)  {
  return wcUsers2[client][slot+50];
}

stock SetInvUnique(client, slot, value) {
  if(value>3330000) {
    LogError("Ignoring high value unique, setting %i to 0", value);
    value=0;
  }
  CheckUnique(value, client);
  wcUsers2[client][slot+50]=value;
}

LoadUniqueItem(id)  {
  if(id<=0)
    return;
  decl String:format[300];
  Format(format,sizeof(format), "SELECT * FROM wc_Items WHERE id = %i", id);
  SQL_TQuery(SSQL,TRGetItem,format,AddQuery(Repeat, 4, format, id ));
}

/**
 * Has small chance of server crash/loop
 */
stock RISKY_SQLGetUniqueForced(id, client=0)  {
  new Handle:query=INVALID_HANDLE;
  if(id>0)  {
    decl String:buffer[255];
    Format(buffer,sizeof(buffer),"SELECT `id` FROM wc_Items WHERE `id` = %i",id);
    query=SQL_Query(hSQL, buffer);
    new tries=5;
    while( (query==INVALID_HANDLE) && (tries>0) )  {

      decl String:err[255];
      SQL_GetError(hSQL,err,255);
      query=SQL_Query(hSQL, buffer);
      tries--;
    }
    if(tries<=0)
      return -1;
    new id2=-1;
    if(SQL_FetchRow(query))  {
      id2=GetSQLDataInt(query, "id");
    }
    if(id2>0)
      return id2;
  }
  return InsertItem(client);
}

bool:LoadItemCode(Handle:query, id, bool:noSQL=false)  {
  if(query)
  {
    if(!noSQL && !SQL_FetchRow(query))
    {
      LogError("[WC] Could not load item data with id = %i", id);
      return false;
    }
    new item[UniqueItem];
    item[Item_id] = GetSQLDataInt(query, "id");
    item[Item_defId] = GetSQLDataInt(query,"defId");
    item[Item_version] = float(GetSQLDataInt(query,"version"))/100.0;

    item[Item_class] = GetSQLDataInt(query,"class");
    item[Item_level] = GetSQLDataInt(query,"level");
    item[Item_ilevel] = GetSQLDataInt(query,"ilevel");
    item[Item_cost] = GetSQLDataInt(query,"cost");
    item[Item_honor] = GetSQLDataInt(query,"honor");
    item[Item_quality] = GetSQLDataInt(query,"quality");
    item[Item_category] = GetSQLDataInt(query,"category");
    item[Item_2hand] = GetSQLDataInt(query,"2hand");

    item[Item_reqstr] = GetSQLDataInt(query,"reqstr");
    item[Item_reqagi] = GetSQLDataInt(query,"reqagi");
    item[Item_reqsta] = GetSQLDataInt(query,"reqsta");
    item[Item_reqint] = GetSQLDataInt(query,"reqint");

    item[Item_str] = GetSQLDataInt(query,"str");
    item[Item_agi] = GetSQLDataInt(query,"agi");
    item[Item_sta] = GetSQLDataInt(query,"sta");
    item[Item_int] = GetSQLDataInt(query,"inte");
    item[Item_res] = GetSQLDataInt(query,"res");
    item[Item_regen] = GetSQLDataInt(query,"regen");
    item[Item_ai] = GetSQLDataInt(query,"ai");
    item[Item_haste] = GetSQLDataInt(query,"haste");

    item[Item_as] = float(GetSQLDataInt(query,"as")) / 100.0;
    item[Item_cs] = float(GetSQLDataInt(query,"cs")) / 100.0;
    item[Item_speed] = float(GetSQLDataInt(query,"speed")) / 100.0;
    item[Item_acc] = float(GetSQLDataInt(query,"acc")) / 100.0;
    item[Item_parry] = float(GetSQLDataInt(query,"parry")) / 100.0;

    item[Item_resilence] = GetSQLDataInt(query,"resilence");
    item[Item_armor] = GetSQLDataInt(query,"armor");
    item[Item_armor_type] = GetSQLDataInt(query,"armor_type"); // 1 Cloth, 2 Leather, 3 Plate, 4 None

    item[Item_ws] = float(GetSQLDataInt(query,"ws")) / 100.0;
    item[Item_weapon_ap] = GetSQLDataInt(query,"weapon_ap");
    item[Item_weapon_sp] = GetSQLDataInt(query,"weapon_sp");
    item[Item_damage] = GetSQLDataInt(query,"damage");
    item[Item_spelldmg] = GetSQLDataInt(query,"spelldmg");
    item[Item_effect] = GetSQLDataInt(query,"effect");

    item[Item_owner] = GetSQLDataInt(query,"owner");
    item[Item_EnchBit] = GetSQLDataInt(query,"enchbit");

    GetSQLDataStr(query, "name", item[Item_Name], 50);
    ReplaceString(item[Item_Name],50, "/z", "'");
    ReplaceString(item[Item_Name],50, "//", "/");

    GetSQLDataStr(query, "descr", item[Item_Descr], 70);
    ReplaceString(item[Item_Descr],70, "/z", "'");
    ReplaceString(item[Item_Descr],70, "//", "/");

    new olditem[UniqueItem];
    if(GetUniqueItem(olditem,item[Item_id],true)) {
      item[Item_LastUsage]=olditem[Item_LastUsage];
    }
    else
      item[Item_LastUsage]=GetEngineTime();
    item[Item_LastUpdate]=GetEngineTime();

    new index = GetUniqueInDB(item[Item_id]);
    if(index>=0)  {
      SetArrayArray(g_itemsArray, index, item[0]);
    }
    else
      PushArrayArray(g_itemsArray, item[0]);
    return true;
  }
  return false;
}

stock TInsertItem(Handle:datapack, owner=0) {
  decl String:buffer[255];
  if(owner<=0)
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Items (id) VALUES (NULL);");
  else
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Items (id, owner) VALUES (NULL, '%i');", characterids[owner][0]);

  SQL_TQuery(TSQL, TInsertedItem, buffer, datapack);
}

public TInsertedItem(Handle:db, Handle:hndl, String:error[], any:datapack)
{
  if(hndl == INVALID_HANDLE)  {
    if(StrContains(error, "Lost connection", false)>=0)   {
      LogError("Lost connection during item insert");
    }
    else  {
      LogError("Insert item error: %s", error);
    }
    CloseHandle(datapack);
    return;
  }
  new id = SQL_GetInsertId(hndl);
  ResetPack(datapack);
  new String:sfunc[50];
  ReadPackString(datapack, sfunc, sizeof(sfunc));
  new Function:func = GetFunctionByName(INVALID_HANDLE, sfunc);
  Call_StartFunction(INVALID_HANDLE, func);
  Call_PushCell(id);
  Call_PushCell(datapack);
  if(Call_Finish() != SP_ERROR_NONE)  {
    LogError("Could not callback the function: %s",sfunc);
  }

  //Clean up
  CloseHandle(datapack);
}


stock InsertItem(owner=0)  {
  new Handle:query=INVALID_HANDLE;
  decl String:buffer[255];
  if(owner<=0)
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Items (id) VALUES (NULL);");
  else
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Items (id, owner) VALUES (NULL, '%i');", characterids[owner][0]);

  while (!SQL_FastQuery(hSQL, buffer))  {
    LogMessage("[WC] Failing to Insert new item");
  }

  new attempts=5;
  while(query==INVALID_HANDLE && attempts > 0)  {
    query=SQL_Query(hSQL, "SELECT LAST_INSERT_ID() as `id`;");
    attempts--;
    if(attempts == 0) {
      return -1;
    }
  }
  new id=-1;
  if(SQL_FetchRow(query))  {
    id=GetSQLDataInt(query, "id");
  }
  CloseHandle(query);
  return id;
}

stock SaveItemCode(item[UniqueItem])  {
  new String:profbuffer[5000],len;
  new String:buffer[70];
  len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_Items SET");

  len+=Format(profbuffer[len],sizeof(profbuffer)-len," `defId` = '%i'", item[Item_defId]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `version` = '%i'", RoundToNearest(item[Item_version]*100.0));
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `class` = '%i'", item[Item_class]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `cost` = '%i'", item[Item_cost]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `level` = '%i'", item[Item_level]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `ilevel` = '%i'", item[Item_ilevel]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `honor` = '%i'", item[Item_honor]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `quality` = '%i'", item[Item_quality]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `category` = '%i'", item[Item_category]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `2hand` = '%i'", item[Item_2hand]);

  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `reqstr` = '%i'", item[Item_reqstr]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `reqagi` = '%i'", item[Item_reqagi]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `reqsta` = '%i'", item[Item_reqsta]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `reqint` = '%i'", item[Item_reqint]);

  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `str` = '%i'", item[Item_str]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `agi` = '%i'", item[Item_agi]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `sta` = '%i'", item[Item_sta]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `inte` = '%i'", item[Item_int]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `res` = '%i'", item[Item_res]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `regen` = '%i'", item[Item_regen]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `ai` = '%i'", item[Item_ai]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `haste` = '%i'", item[Item_haste]);

  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `as` = '%i'", RoundToNearest(item[Item_as]*100.0));
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `cs` = '%i'", RoundToNearest(item[Item_cs]*100.0));
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `speed` = '%i'", RoundToNearest(item[Item_speed]*100.0));
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `acc` = '%i'", RoundToNearest(item[Item_acc]*100.0));
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `parry` = '%i'", RoundToNearest(item[Item_parry]*100.0));

  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `resilence` = '%i'", item[Item_resilence]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `armor` = '%i'", item[Item_armor]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `armor_type` = '%i'", item[Item_armor_type]); //1 Cloth, 2 Leather, 3 Plate, 4 None

  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `ws` = '%i'", RoundToNearest(item[Item_ws]*100.0));
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `weapon_ap` = '%i'", item[Item_weapon_ap]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `weapon_sp` = '%i'", item[Item_weapon_sp]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `damage` = '%i'", item[Item_damage]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `spelldmg` = '%i'", item[Item_spelldmg]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `effect` = '%i'", item[Item_effect]);

  Format(buffer,sizeof(buffer),"%s",item[Item_Name]);
  ReplaceString(buffer,sizeof(buffer), "/", "//");
  ReplaceString(buffer,sizeof(buffer), "'", "/z");
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `name` = '%s'", buffer);

  Format(buffer,sizeof(buffer),"%s",item[Item_Descr]);
  ReplaceString(buffer,sizeof(buffer), "/", "//");
  ReplaceString(buffer,sizeof(buffer), "'", "/z");
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `descr` = '%s'", buffer);

  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `owner` = '%i'", item[Item_owner]);
  len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `enchbit` = '%i'", item[Item_EnchBit]);


  len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE id = %i",item[Item_id]);
  SQL_TQuery(SSQL,TRepeating,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_Normal);
}

stock bool:CheckUnique(value, client=0)  {
  if(value<=0)
    return false;
  if(!IsUniqueInDB(value))  {
    LoadUniqueItem(value);
    return false;
  }
  return true;
}

stock GetUniqueInDB(id) {
  for(new i=0; i < GetArraySize(g_itemsArray); i++) {
    if(GetArrayCell(g_itemsArray, i, 0) == id)
      return i;
  }
  return -1;
}

stock bool:IsUniqueInDB(id) {
  for(new i=0; i < GetArraySize(g_itemsArray); i++) {
    if(GetArrayCell(g_itemsArray, i, 0) == id)
      return true;
  }
  return false;
}

stock bool:GetUniqueItem(item[UniqueItem], id, bool:noload=false)  {
  item[Item_id] = 0;
  for (new i=0; i<GetArraySize(g_itemsArray); i++)   {
    if(GetArrayCell(g_itemsArray, i, 0) == id)   {
      GetArrayArray(g_itemsArray, i, item[0]);
      item[Item_LastUsage] = GetEngineTime();
      SetArrayArray(g_itemsArray, i, item[0]); //optimize later
      return true;
    }
  }
  if(!noload) {
    LoadUniqueItem(id);
  }
  return false;
}

stock GetItemProperties(item[UniqueItem], itemId, uniqueId=-1)  {
  if(itemId>0)  {
    GetStandardItem(item, itemId);
  }
  else  {
    GiveItemUnknownProps(item);
  }
  item[Item_LastUsage] = GetEngineTime();
  item[Item_owner] = 0;
  item[Item_EnchBit] = 0;
  item[Item_defId]=itemId;
  item[Item_id]=uniqueId;
  if(uniqueId>0)  {
    new uniItem[UniqueItem];
    if(GetUniqueItem(uniItem, uniqueId))  {
      if(uniItem[Item_id]>0)
        item[Item_id]=uniItem[Item_id];
      if(uniItem[Item_defId]>0)
        item[Item_defId]=uniItem[Item_defId];
      if(uniItem[Item_version]>0.0)
        item[Item_version]=uniItem[Item_version];
      if(uniItem[Item_class]>=0)
        item[Item_class]=uniItem[Item_class];
      if(uniItem[Item_level]>=0)
        item[Item_level]=uniItem[Item_level];
      if(uniItem[Item_ilevel]>=0)
        item[Item_ilevel]=uniItem[Item_ilevel];
      if(uniItem[Item_cost]>=0)
        item[Item_cost]=uniItem[Item_cost];
      if(uniItem[Item_honor]>=0)
        item[Item_honor]=uniItem[Item_honor];
      if(uniItem[Item_quality]>=0)
        item[Item_quality]=uniItem[Item_quality];
      if(uniItem[Item_2hand]>=0)
        item[Item_2hand]=uniItem[Item_2hand];
      if(uniItem[Item_category]>=0)
        item[Item_category]=uniItem[Item_category];

      if(uniItem[Item_reqstr]>=0)
        item[Item_reqstr]=uniItem[Item_reqstr];
      if(uniItem[Item_reqagi]>=0)
        item[Item_reqagi]=uniItem[Item_reqagi];
      if(uniItem[Item_reqsta]>=0)
        item[Item_reqsta]=uniItem[Item_reqsta];
      if(uniItem[Item_reqint]>=0)
        item[Item_reqint]=uniItem[Item_reqint];

      if(uniItem[Item_str]>-100)
        item[Item_str]=uniItem[Item_str];
      if(uniItem[Item_agi]>-100)
        item[Item_agi]=uniItem[Item_agi];
      if(uniItem[Item_sta]>-100)
        item[Item_sta]=uniItem[Item_sta];
      if(uniItem[Item_int]>-100)
        item[Item_int]=uniItem[Item_int];
      if(uniItem[Item_res]>-100)
        item[Item_res]=uniItem[Item_res];
      if(uniItem[Item_regen]>-100)
        item[Item_regen]=uniItem[Item_regen];
      if(uniItem[Item_ai]>-100)
        item[Item_ai]=uniItem[Item_ai];
      if(uniItem[Item_weapon_ap]>-1)
        item[Item_weapon_ap]=uniItem[Item_weapon_ap];
      if(uniItem[Item_weapon_sp]>-1)
        item[Item_weapon_sp]=uniItem[Item_weapon_sp];

      if(uniItem[Item_as]>-1.0)
        item[Item_as]=uniItem[Item_as];
      if(uniItem[Item_cs]>-1.0)
        item[Item_cs]=uniItem[Item_cs];
      if(uniItem[Item_speed]>-1.0)
        item[Item_speed]=uniItem[Item_speed];
      if(uniItem[Item_acc]>-1.0)
        item[Item_acc]=uniItem[Item_acc];

      if(uniItem[Item_resilence]>-100)
        item[Item_resilence]=uniItem[Item_resilence];
      if(uniItem[Item_armor]>=0)
        item[Item_armor]=uniItem[Item_armor];
      if(uniItem[Item_armor_type]>=0)
        item[Item_armor_type]=uniItem[Item_armor_type];


      if(uniItem[Item_ws]>-1.0)
        item[Item_ws]=uniItem[Item_ws];
      if(uniItem[Item_damage]>=0)
        item[Item_damage]=uniItem[Item_damage];
      if(uniItem[Item_spelldmg]>=0)
        item[Item_spelldmg]=uniItem[Item_spelldmg];
      if(uniItem[Item_effect]>=0)
        item[Item_effect]=uniItem[Item_effect];

      if(!StrEqual(uniItem[Item_Name],""))
        strcopy(item[Item_Name],50,uniItem[Item_Name]);
      if(!StrEqual(uniItem[Item_Descr],""))
        strcopy(item[Item_Descr],70,uniItem[Item_Descr]);
      //StrEqual(uniItem[Item_message],"")?:strcopy(item[Item_message],50,uniItem[Item_message]);

      if(uniItem[Item_owner]>=0)
        item[Item_owner]=uniItem[Item_owner];
      if(uniItem[Item_EnchBit]>=0)
        item[Item_EnchBit]=uniItem[Item_EnchBit];	
    }
  }
  if( (item[Item_version]<1.20) && (item[Item_category]==22))  {
      item[Item_damage]=RoundToNearest( float(item[Item_damage])*0.5);
      item[Item_spelldmg]=RoundToNearest( float(item[Item_spelldmg])*0.75);
  }
  if( (item[Item_version]<2.0) )  {
    new classes[10];
    new amount=ConvertBitToClasses(item[Item_class], classes);
    for(new i=0;i<amount;i++)  {
      if(classes[i]==1) {
        item[Item_str] += item[Item_int];
        item[Item_int]=0;
        item[Item_str] += item[Item_regen] * 2;
        item[Item_regen] = 0;
      }
      if(classes[i]==2) {
        item[Item_agi] += item[Item_int];
        item[Item_int]=0;
        item[Item_agi] += item[Item_regen] * 2;
        item[Item_regen] = 0;
      }
    }
  }
  if( item[Item_version]<2.5 )  {
    item[Item_ai] *= 2;
  }
  if(item[Item_version] < 2.0 && item[Item_amount] > 5)  {
    item[Item_amount] *= 5;
  }
  if(item[Item_level] <= 80 && item[Item_quality] >= 3 && item[Item_version] < 2.40) {
    item[Item_armor] = RoundToNearest(float(item[Item_armor]) * 0.8);

  }
  if(item[Item_version] < 4.0 && item[Item_armor] > 0)  {
    item[Item_armor] = RoundToNearest(float(item[Item_armor]) * 0.65);
  }
}

stock GetStandardItem(item[UniqueItem], id) {
  decl String:key[10];
  KvRewind(items);
  IntToString(id,key,sizeof(key));
  if(!KvJumpToKey(items,key))  {
    GiveItemUnknownProps(item);
    LogError("Couldn't find props for default item = %i", id);
    item[Item_defId] = id;
    return;
  }
  item[Item_version] = KvGetFloat(items, "version", 0.96);

  //Finding classes
  new classes[12];
  decl String:Sclass[55];
  KvGetString(items,"class",Sclass,sizeof(Sclass),"0");
  decl String:Stemp[50];
  new StringID=BreakString(Sclass, Stemp, sizeof(Stemp));
  new camount=1;
  classes[0]=StringToInt(Stemp);
  while(StringID>=0)
  {
    new StringID2=BreakString(Sclass[StringID], Stemp, sizeof(Stemp));
    StringID+=StringID2;
    if(StringID2==-1)
      StringID=-1;
    classes[camount++]=StringToInt(Stemp);
  }
  //end of finding
  new bitclass = 0;
  for(new i=0;i<camount;i++)  {
    bitclass = bitclass | 1<<classes[i];
  }

  item[Item_class] = bitclass;

  item[Item_level] = KvGetNum(items, "level", 0);
  if (item[Item_version] <= 0.96)
    item[Item_level]*=4;
  item[Item_ilevel]= KvGetNum(items, "ilevel", 0);
  item[Item_cost] = KvGetNum(items, "cost", 0);
  item[Item_gold] = KvGetNum(items, "gold", 0);
  item[Item_honor] = KvGetNum(items, "honor", 0);
  item[Item_quality] = KvGetNum(items, "noshop", 0);
  item[Item_category] = KvGetNum(items, "category", 0);
  item[Item_2hand] = KvGetNum(items, "2hand", 0);
  item[Item_use] = KvGetNum(items, "use", 0);
  item[Item_amount] = KvGetNum(items, "amount", 1);

  item[Item_reqstr] = KvGetNum(items, "reqstr", 0);
  item[Item_reqagi] = KvGetNum(items, "reqagi", 0);
  item[Item_reqsta] = KvGetNum(items, "reqsta", 0);
  item[Item_reqint] = KvGetNum(items, "reqint", 0);

  item[Item_str] = KvGetNum(items, "str", 0);
  item[Item_agi] = KvGetNum(items, "agi", 0);
  item[Item_sta] = KvGetNum(items, "sta", 0);
  item[Item_int] = KvGetNum(items, "int", 0);
  item[Item_res] = KvGetNum(items, "res", 0);
  item[Item_regen] = KvGetNum(items, "regen", 0);
  item[Item_ai] = KvGetNum(items, "ai", 0);
  item[Item_haste] = KvGetNum(items, "haste", 0);
  item[Item_parry] = KvGetFloat(items, "parry", 0.0);

  item[Item_as] = KvGetFloat(items, "as", 0.0);
  item[Item_cs] = KvGetFloat(items, "cs", 0.0);
  item[Item_speed] = KvGetFloat(items, "speed", 0.0);
  item[Item_acc] = KvGetFloat(items, "acc", 0.0);

  item[Item_resilence] = KvGetNum(items, "resilence", 0);
  item[Item_armor] = KvGetNum(items, "armor", 0);
  item[Item_armor_type] = KvGetNum(items, "armor_type", 0); // 1 Cloth, 2 Leather, 3 Plate, 4 None.

  //Weapon settings
  item[Item_ws]        = KvGetFloat(items, "ws", 0.0);
  item[Item_weapon_ap] = KvGetNum(items, "weapon_ap", 0);
  item[Item_weapon_sp] = KvGetNum(items, "weapon_sp", 0);
  item[Item_damage]    = KvGetNum(items, "damage", 0);
  item[Item_spelldmg]  = KvGetNum(items, "spelldmg", 0);
  item[Item_effect]    = KvGetNum(items, "effect", 0);

  item[Item_forGold] = KvGetNum(items, "forgold", 0); //if 1, then no need to pay silver/honor

  item[Item_noTrade] = KvGetNum(items, "notrade", 0); //If item can be traded via mail/auction. being set to 1 means no but still can mail to other owned characters

  if(KvGetNum(items,"token", 0) == 13583)
    item[Item_valor] = KvGetNum(items, "token_amount", 0);
  if(KvGetNum(items,"token", 0 ) == 13584)
    item[Item_justice] = KvGetNum(items, "token_amount", 0);

  KvGetString(items, "descr", item[Item_Descr],70, "");
  KvGetString(items, "name", item[Item_Name],50, "Unknown");
  KvGetString(items, "message", item[Item_message],50, "");
}

stock GiveItemUnknownProps(item[UniqueItem])  {
	item[Item_id]=0;
	item[Item_defId]=0;
	item[Item_version]=VERSION;
	item[Item_class]=0;
	item[Item_level]=0;
	item[Item_ilevel]=0;
	item[Item_cost]=0;
	item[Item_honor]=0;
	item[Item_quality]=0;
	item[Item_category]=0;
	item[Item_2hand]=0;
	item[Item_use]=0;
        item[Item_justice]=0;
        item[Item_valor]=0;
	
	item[Item_reqstr]=0;
	item[Item_reqagi]=0;
	item[Item_reqsta]=0;
	item[Item_reqint]=0;
	
	item[Item_str]=0;
	item[Item_agi]=0;
	item[Item_sta]=0;
	item[Item_int]=0;
	item[Item_res]=0;
	item[Item_regen]=0;
	item[Item_ai]=0;
	item[Item_haste]=0;
	item[Item_parry]=0.0;
	
	item[Item_as]=0.0;
	item[Item_cs]=0.0;
	item[Item_speed]=0.0;
	item[Item_acc]=0.0;

	item[Item_resilence]=0;
	item[Item_armor]=0;
	item[Item_armor_type]=0; //1 Cloth, 2 Leather, 3 Plate, 4 None.

	item[Item_ws]=0.0;
	item[Item_weapon_ap]=0;
	item[Item_weapon_sp]=0;
	item[Item_damage]=0;
	item[Item_spelldmg]=0;
	item[Item_effect]=0;

	Format(item[Item_Name], 50, "Unknown");
	Format(item[Item_Descr], 70, "");
	Format(item[Item_message], 50, "");
	
	item[Item_use]=0;
	item[Item_amount]=1;
        item[Item_noTrade]=0;
}

ConvertBitToClasses(bit, classes[]) {
  new amount=0;
  for(new i=0;i<=MAX_CLASS;i++)  {
    if(bit & (1<<i)==(1<<i)){
      classes[amount++]=i;
    }
  }
  return amount;
}

GetInvItem(item[UniqueItem], client, slot)  {
  new uniqueId = GetInvUnique(client, slot);
  GetItemProperties(item, GetSlotItem(client,slot), uniqueId);
}

GetCharItem(item[UniqueItem], client, slot) {
  GetItemProperties(item, GetCharItemId(client, slot), GetCharUnique(client, slot));
}

//end of unique items section

stock DebugItem(item[UniqueItem]) {
  PrintToServer("Debugging item");
  PrintToServer("def id = %i", item[Item_defId]);
  PrintToServer("costs = %i", item[Item_cost]);
  PrintToServer("honor = %i", item[Item_honor]);

  PrintToServer("End Debugging item");
}


stock UpdateUniqueState(id, stat)  {
  /**
   *  Updates the state of item which indicates
   *  what lately happened to it.
   *  0 - default
   *  2 - was sold to vendor
   *  3 - is sent to auction
   *  4 - disenchant
   *  6 - removed via standard RemoveItem func
   */
  if(id<=0)
    return;
  decl String:query[300];
  Format(query,sizeof(query),"UPDATE wc_Items SET `state` = '%i' WHERE id = %i", stat, id);
  SQL_TQuery(SSQL,TRepeating,query,AddQuery(Repeat, 3, query, 0 ),DBPrio_Normal);
}

stock UniqueOwner(id, owner)  {
  if(id<=0)
    return;
  decl String:query[300];
  Format(query,sizeof(query),"UPDATE wc_Items SET `owner` = '%i' WHERE id = %i", owner, id);
  SQL_TQuery(SSQL,TRepeating,query,AddQuery(Repeat, 3, query, 0 ),DBPrio_Normal);
}

RemoveInvItem(client, slot, stat=0) {
  wcSetStaticSlot(client,slot,0);
  wcSetStaticSlotAmount(client,slot,0);
  new uniqueid = GetInvUnique(client, slot);
  SetInvUnique(client, slot, 0);
  if(uniqueid>0)  {
    UpdateUniqueState(uniqueid, stat);
  }
}

public public_ApplyEnchant(id, Handle:pack) {
  ResetPack(pack);
  decl String:temp[50];
  ReadPackString(pack, temp, 50);
  new client = ReadPackCell(pack);
  new slot = ReadPackCell(pack);
  new enchid = ReadPackCell(pack);

  if(IsClientInGame(client)) {
    SetInvUnique(client, slot, id);
  }
  //Set unique Id of item

  decl String:query[255];
  Format(query,sizeof(query),"UPDATE wc_Items SET `enchbit` = '%i' WHERE id = %i", enchid, id);
  SQL_TQuery(SSQL,TRepeating,query,AddQuery(Repeat, 3, query, 0 ),DBPrio_High);


  if(CheckUnique(id, client)) {
    new uslot = GetUniqueInDB(id);
    new uitem[UniqueItem];
    GetArrayArray(g_itemsArray, uslot, uitem[0]);
    uitem[Item_LastUsage] = GetEngineTime();
    uitem[Item_EnchBit] = enchid;
    SetArrayArray(g_itemsArray, uslot, uitem[0]); //optimize later
  }


}

stock ApplyEnchant(client, slot, enchid, bool:insert=true)  {
  new item[UniqueItem];
  GetInvItem(item, client, slot);
  new id = item[Item_id];
  /*PrintToServer("Applying enchant %d to %d slot itemId %d", enchid, slot, id);*/
  if(id>0)  {
    SetInvUnique(client, slot, id);
    decl String:query[255];
    Format(query,sizeof(query),"UPDATE wc_Items SET `enchbit` = '%i' WHERE id = %i", enchid, id);
    /*PrintToServer("Running query update: %s", query);*/
    SQL_TQuery(SSQL,TRepeating,query,AddQuery(Repeat, 3, query, 0 ),DBPrio_High);
  }
  else {
    new Handle:pack = CreateDataPack();
    WritePackString(pack,"public_ApplyEnchant");
    WritePackCell(pack, client);
    WritePackCell(pack, slot);
    WritePackCell(pack, enchid);
    TInsertItem(pack, client);

    return;
  }
  if(CheckUnique(id, client)) {
    new uslot = GetUniqueInDB(id);

    /*PrintToServer("Saving info to cache. Uslot = %d", uslot);*/
    new uitem[UniqueItem];
    GetArrayArray(g_itemsArray, uslot, uitem[0]);
    uitem[Item_LastUsage] = GetEngineTime();
    uitem[Item_EnchBit] = enchid;
    SetArrayArray(g_itemsArray, uslot, uitem[0]); //optimize later
  }
}

bool:CanDisenchant(cat) {
  if(cat<10)
    return false;
  if(cat>23)
    return false;
  return true;
}

stock GiveItem(client, item[UniqueItem], amount, String:source[]="Unknown", bool:save=false, bool:announce=false )  {
  new space=0;
  new slot=0;
  new old = amount;
  while(amount>0) {
    slot=wcCheckFreeSlot3(client,item, space);
    if( (slot>0) && (space>0) ) {
      new give = GetMinInt(space, amount);
      amount-=give;
      if(give == 0) amount--; //If there is no slots for items then person is unlucky and he start loosing amount... - anti loop
      wcSetStaticSlot(client,slot,item[Item_defId]);
      SetInvUnique(client, slot, item[Item_id]);
      wcSetStaticSlotAmount(client,slot,GetSlotItemAmount(client,slot)+give);
      if(announce)  AnnounceDrop(client, item[Item_Name], slot);
      if(save)  {
        SQL_UpdateSlot(client, slot);
      }
    }
    else  {
      WCMessage(client, "failed drop", client, item[Item_Name]);
      return -1;
    }
  }
  //Log it
  sqlLogItem(client, item, old, source);
  return slot;
}

CatType:getCatType(cat)  {
  if( (cat>=10) && (cat <= 23) )
    return CatType:CT_Wearable;
  if( (cat >= 1)  && (cat <= 5) )
    return CatType:CT_Recipe;
  return CatType:CT_Unknown;
}

getCatTypeStr(CatType:cat, String:str[], max) {
  switch(cat) {
    case CT_Wearable: Format(str, max, "Wearable");
    case CT_Recipe: Format(str, max, "Recipe");
    case CT_Unknown: Format(str, max, "Unknown");
  }
}


prepareItemsQuery(client) {
  new cl = GetClass(client);

  //Get crafting profession
  //Check if person has any crafting profession
  new craftSlot=-1;
  for(new i=0;i<2;i++)  {
    if(isCraftingProfession(GetProfession(client,i+1)) )  {
      craftSlot = i;
    }
  }

  new craftId;
  new craftSk;
  if(craftSlot>=0)  {
    craftId=GetProfession(client,craftSlot+1);
    craftSk=GetProfessionSk(client,craftSlot+1);
  }

  decl String:query[2000];
  new pos=Format(query, sizeof(query), "select wc_itemsCache.* from wc_itemsCache where ( ( ( (class & 1 << %d ) = 1 << %d ) AND category = 'Wearable') OR (category = 'Recipe' AND rating <= %d AND cat = %d AND `itemid` NOT IN  (0",
    cl, cl, craftSk, craftId);
  //Add known recipes
  if(craftSlot >= 0) for(new x=1;x<=400;x++)
  {
    new recipe=GetClientProfessionSlot(client,craftSlot+1,x);
    if(recipe<=0)
      break;
    pos+=Format(query[pos],sizeof(query)-pos,", %d",recipe);

  }
  Format(query[pos], sizeof(query)-pos, ") ) ) AND `level` <= '%d' AND wc_itemsCache.`itemid` NOT IN (SELECT itemId FROM wc_ignoredItems WHERE CharacterId = '%d') AND quality = 0 ORDER BY category desc, rating desc", GetLevel(client), characterids[client][0] );

  SQL_TQuery(TSQL,PreloadTodoMenu,query,client);


}

public PreloadTodoMenu(Handle:db,Handle:query,String:error[],any:client)
{
  if(!IsClientConnected(client) || !IsClientInGame(client))
    return;
  if(!query)  {
    LogError("Error 816: %s", error);
    return;
  }
  SQL_Rewind(query);
  _displayTodoMenu(client, query);
}


stock UseItem(client, islot, bool:nosuicide=false)  {
  new item[UniqueItem];
  GetInvItem(item, client, islot);
  new cat=item[Item_category];
  if(((cat>9) && (cat<24)))
  {
    new x=GetFilledInventory(client);
    if ((100-x)>=2)
    {
      GetItemOn(client,islot, nosuicide);
    }
    else
      WCMessage(client,"low free slots",client,100-x);
    if(!nosuicide)
      InventoryMenu(client);
  }
  else if(cat==30)  {
    GetGlyphOn(client, islot);
  }
  else if(cat==24) {
    WCMessage(client,item[Item_message],client);
    RandomBoxUsed(client,islot);
    if(!nosuicide)
      InventoryMenu(client);
  }
  else if(cat==25)  {
    lastMenu[client]=5;
    EnchantMenu(client, islot);
  }
  else if(cat == 27)  {
    SpecialUsed(client, islot);
    if(!nosuicide)
      InventoryMenu(client);
  }
  else
  {
    new prof=GetProfession(client,1);
    new profid=1;
    new profcat=cat;
    if(profcat==5)
      profcat=8;  //profession 8, recipe category is 5
    if(prof!=profcat)
    {
      prof=GetProfession(client,2);
      profid=2;
    }
    if(prof==profcat)
    {
      new skill=GetProfessionSk(client,profid);
      if(GetLevel(client)<GetRequiredLevel(item))
      {
        WCMessage(client,"too low lvl rec",client,GetRequiredLevel(item));
      }
      else if(skill<GetRequiredSkill(item[Item_defId]))
      {
        WCMessage(client,"too low skill",client);
      }
      else if(FindProfessionR(client,profid,item[Item_defId]))
      {
        WCMessage(client,"you already know",client);
      }
      else
      {
        wcSetStaticSlot(client,islot,0);
        wcSetStaticSlotAmount(client,islot,0);
        new unique = GetInvUnique(client, islot);
        SetInvUnique(client, islot, 0);
        if(unique >0)  {
          UpdateUniqueState(unique, 1);
        }
        new freeprof=GetFreeProfession(client,profid);
        if(freeprof<=0)
          LogError("could not find profession free slot");
        else
        {
          SetClientProfessionSlot(client,profid,freeprof,item[Item_defId]);
          WCMessage(client,"learned book",client,item[Item_Name]);
        }
      }
    }
    else
      WCMessage(client,"wrong profession",client);
    if(!nosuicide)
      InventoryMenu(client);
  }
}

stock GetItemOn(client,slot, nosuicide)
{
  /*if(IsPlayerAlive(client) && (!nosuicide))*/
  /*{*/
    /*ForcePlayerSuicide(client);*/
    /*WCMessage(client,"can not put items while alive",client);*/
  /*}*/
  new item[UniqueItem];
  GetInvItem(item, client, slot);
  new cat=item[Item_category];
  new compSum[UniqueItem];
  if(wcCheckStats(client,item)==1)
  {
    if ((cat==18) || (cat==22))
    {
      new ritem[UniqueItem];
      GetCharItem(ritem, client, 22);
      new hand=ritem[Item_2hand];
      if (hand == 1)
      {
        new freeslot=wcCheckFreeSlot(client);
        wcSetStaticSlot(client,freeslot,ritem[Item_defId]);
        wcSetStaticSlotAmount(client,freeslot,-1);
        SetInvUnique(client, freeslot, ritem[Item_id]);
        wcSetCharItem(client,22,0);
        SetCharUnique(client, 22, 0);
        AddItem(ritem, compSum);
      }
      //if (cat==18)
      //  categoryname="leftweapons";
      if (cat==22)
      {
        new hand2=item[Item_2hand];
        if (hand2==1)
        {
          new freeslot=wcCheckFreeSlot(client);
          new leftitem[UniqueItem];
          GetCharItem(leftitem, client,18);
          wcSetStaticSlot(client,freeslot,leftitem[Item_defId]);
          SetInvUnique(client, freeslot, leftitem[Item_id]);
          wcSetStaticSlotAmount(client,freeslot,-1);
          wcSetCharItem(client,18,0);
          SetCharUnique(client, 18, 0);
          AddItem(leftitem, compSum);
        }
        else

          if( (GetCharItemId(client,22)!=0) && CanDoubleHand(client) )  {
            cat=18;
          }
      }
    }
    new itemon[UniqueItem];
    GetCharItem(itemon, client, cat);
    AddItem(itemon, compSum);
    wcSetStaticSlot(client,slot,itemon[Item_defId]);
    SetInvUnique(client, slot, itemon[Item_id]);
    wcSetStaticSlotAmount(client,slot,-1);
    wcSetCharItem(client, cat, item[Item_defId]);

    SetCharUnique(client, cat, item[Item_id]);
    ItemCompare(item, compSum, client);
  }
  else
  {
   if(wcCheckStats(client,item)==0)
    WCMessage(client,"nostats",client);
   else if(wcCheckStats(client,item)==2)
    WCMessage(client,"wrong class",client);
   else if(wcCheckStats(client,item)==3)
    WCMessage(client,"low lvl",client);
  }
}


stock getItemCreates(id)  {
  KvRewind(items);
  decl String:Sid[20];
  IntToString(id,Sid,sizeof(Sid));
  KvJumpToKey(items,Sid);

  return KvGetNum(items,"itemid");
}


ignoreItemRecommended(client, id) {
  decl String:query[200];
  Format(query, sizeof(query), "INSERT INTO wc_ignoredItems (`CharacterId`, `itemId`) VALUES ('%d', '%d')",
    characterids[client][0], id);
  SQL_TQuery(TSQL, Tinserted, query, 915);
  WCMessage(client, "item added to ignore", client);
}


stock bool:AddTemporaryItem(client, item[UniqueItem], Float:duration, bool:stacks=false)  {
  if(!IsClientInGame(client) || !IsPlayerAlive(client) || (playerloaded[client]!= 1))
    return false;
  if((!stacks) && hasTempItem(client, item))
    return false;
  new Handle:datapack=CreateDataPack();
  WritePackCell(datapack,client);
  WritePackCell(datapack,item[Item_defId]);
  WritePackCell(datapack,g_respawns[client]);
  CreateTimer(duration,RemoveTemporaryItem,datapack,TIMER_HNDL_CLOSE);

  addTempItem(client, item);
  if(!stacks) pushTempItem(client, item);

  return true;

}

public Action:RemoveTemporaryItem(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new itemId = ReadPackCell(datapack);
  new spawn = ReadPackCell(datapack);
  if(!IsClientInGame(client) || !IsPlayerAlive(client) ||
    (playerloaded[client]!=1) || (spawn != g_respawns[client]) )
    return Plugin_Stop;

  new item[UniqueItem];
  GetItemProperties(item, itemId);
  removeTempItem(client, item);
  pullTempItem(client, item);
  return Plugin_Stop;
}

new g_tempItems[65][30];

flushTempItems(client)  {
  for(new i=0;i<30;i++)
    g_tempItems[client][i]=0;
}

pushTempItem(client, item[UniqueItem])  {
  for(new i=0;i<30;i++)
    if(g_tempItems[client][i]==0) {
      g_tempItems[client][i] = item[Item_defId];
      return;
    }
}

pullTempItem(client, item[UniqueItem])  {
  for(new i=0;i<30;i++)
    if(g_tempItems[client][i]==item[Item_defId])  {
      g_tempItems[client][i]=0;
      return;
    }
}

bool:hasTempItem(client, item[UniqueItem]) {
  for(new i=0;i<30;i++)
    if(g_tempItems[client][i]==item[Item_defId])  {
      return true;
    }
  return false;
}

/**
 * Adds item to the client's stats, does not support effects
 * also does not support enchants attached to those items
 * @return rating change
 */
Float:addTempItem(client, item[UniqueItem]) {
  new Float:rating=0.0;
  new id = client;
  if ((item[Item_id]>0) || (item[Item_defId] > 0))
  {
    new Float:dmgMulti=1.0;
    /*if(((item[Item_weapon_ap]+item[Item_damage])>7.0) && ((item[Item_weapon_sp]+item[Item_spelldmg])>9.0))*/
      /*dmgMulti=0.7143;*/
    new Float:bufferf;
    bufferf=float(item[Item_damage]);
    wcClientsBonusStats[id][DAMAGE]+=bufferf;
    rating+= bufferf * dmgMulti * RATING_DAMAGE;

    bufferf=float(item[Item_weapon_ap]);
    wcClientsBonusStats[id][DAMAGE]+=bufferf;
    rating+= bufferf * dmgMulti * RATING_DAMAGE;

    bufferf=float(item[Item_weapon_sp]);
    wcClientsBonusStats[id][SPELLDAMAGE]+=bufferf;
    rating+= bufferf * dmgMulti * RATING_SPELLDMG;

    bufferf=float(item[Item_armor]);
    wcClientsBonusStats[id][ARMOR]+=bufferf;
    rating+= bufferf * RATING_ARMOR;

    bufferf=float(item[Item_str]);
    wcClientsStats[id][STR]+=bufferf;
    rating+= bufferf * RATING_STR;

    bufferf=float(item[Item_agi]);
    wcClientsStats[id][AGI]+=bufferf;
    rating+= bufferf * RATING_AGI;

    bufferf=float(item[Item_int]);
    wcClientsStats[id][INT]+=bufferf;
    rating+= bufferf * RATING_INT;

    bufferf=float(item[Item_sta]);
    wcClientsStats[id][STA]+=bufferf;
    rating+= bufferf * RATING_STA;

    bufferf=float(item[Item_ai]);
    wcClientsBonusStats[id][ARMORIGNORE]+=bufferf;
    rating+= bufferf * RATING_AI;

    bufferf=float(item[Item_regen]);
    wcClientsBonusStats[id][MPREGEN]+=bufferf;
    rating+= bufferf * RATING_REGEN;

    bufferf=item[Item_as];
    wcClientsBonusStats[id][ATTACKSPEED]+=bufferf;
    rating+= bufferf * RATING_AS;

    bufferf=float(item[Item_spelldmg]);
    wcClientsBonusStats[id][SPELLDAMAGE]+=bufferf;
    rating+= bufferf * dmgMulti * RATING_SPELLDMG;

    bufferf=item[Item_speed];
    wcClientsBonusStats[id][MOVESPEED]+=bufferf;
    rating+= bufferf * RATING_SPEED;

    bufferf=float(item[Item_res]);
    wcClientsBonusStats[id][RESISTANCE]+=bufferf;
    rating+= bufferf * RATING_RES;

    bufferf=float(item[Item_resilence]);
    wcClientsBonusStats[id][RESILIENCE]+=bufferf;  //resilence
    rating+= bufferf * RATING_RESILENCE;

    bufferf=item[Item_acc];
    wcClientsBonusStats[id][ACC]+=bufferf;  //accuracy
    rating+= bufferf * RATING_ACC;

    bufferf=item[Item_cs]; //Critical Strike
    wcClientsBonusStats[id][CRITCHANCE]+=bufferf;
    rating+= bufferf * RATING_CS;

    bufferf=float(item[Item_haste]); //Haste
    wcClientsBonusStats[id][HASTE]+=bufferf;
    rating+= bufferf * RATING_HASTE;

    bufferf=item[Item_parry];
    wcClientsBonusStats[id][PARRY]+=bufferf;
    rating+= bufferf * RATING_PARRY;

    wcRecalculateStats(client);
  }
  return rating;
}


/**
 * Removes item from client's stats, does not support effects
 * also does not support enchants attached to those items
 * @return rating change
 */

Float:removeTempItem(client, item[UniqueItem])  {
  new Float:rating=0.0;
  new id = client;
  if ((item[Item_id]>0) || (item[Item_defId] > 0))
  {
    new Float:dmgMulti=1.0;
    /*if(((item[Item_weapon_ap]+item[Item_damage])>7.0) && ((item[Item_weapon_sp]+item[Item_spelldmg])>9.0))*/
      /*dmgMulti=0.7143;*/
    new Float:bufferf;
    bufferf=float(item[Item_damage]);
    wcClientsBonusStats[id][DAMAGE]-=bufferf;
    rating-= bufferf * dmgMulti * RATING_DAMAGE;

    bufferf=float(item[Item_weapon_ap]);
    wcClientsBonusStats[id][DAMAGE]-=bufferf;
    rating-= bufferf * dmgMulti * RATING_DAMAGE;

    bufferf=float(item[Item_weapon_sp]);
    wcClientsBonusStats[id][SPELLDAMAGE]-=bufferf;
    rating-= bufferf * dmgMulti * RATING_SPELLDMG;

    bufferf=float(item[Item_armor]);
    wcClientsBonusStats[id][ARMOR]-=bufferf;
    rating-= bufferf * RATING_ARMOR;

    bufferf=float(item[Item_str]);
    wcClientsStats[id][STR]-=bufferf;
    rating-= bufferf * RATING_STR;

    bufferf=float(item[Item_agi]);
    wcClientsStats[id][AGI]-=bufferf;
    rating-= bufferf * RATING_AGI;

    bufferf=float(item[Item_int]);
    wcClientsStats[id][INT]-=bufferf;
    rating-= bufferf * RATING_INT;

    bufferf=float(item[Item_sta]);
    wcClientsStats[id][STA]-=bufferf;
    rating-= bufferf * RATING_STA;

    bufferf=float(item[Item_ai]);
    wcClientsBonusStats[id][ARMORIGNORE]-=bufferf;
    rating-= bufferf * RATING_AI;

    bufferf=float(item[Item_regen]);
    wcClientsBonusStats[id][MPREGEN]-=bufferf;
    rating-= bufferf * RATING_REGEN;

    bufferf=item[Item_as];
    wcClientsBonusStats[id][ATTACKSPEED]-=bufferf;
    rating-= bufferf * RATING_AS;

    bufferf=float(item[Item_spelldmg]);
    wcClientsBonusStats[id][SPELLDAMAGE]-=bufferf;
    rating-= bufferf * dmgMulti * RATING_SPELLDMG;

    bufferf=item[Item_speed];
    wcClientsBonusStats[id][MOVESPEED]-=bufferf;
    rating-= bufferf * RATING_SPEED;

    bufferf=float(item[Item_res]);
    wcClientsBonusStats[id][RESISTANCE]-=bufferf;
    rating-= bufferf * RATING_RES;

    bufferf=float(item[Item_resilence]);
    wcClientsBonusStats[id][RESILIENCE]-=bufferf;
    rating-= bufferf * RATING_RESILENCE;

    bufferf=item[Item_acc];
    wcClientsBonusStats[id][ACC]-=bufferf;
    rating-= bufferf * RATING_ACC;

    bufferf=item[Item_cs];
    wcClientsBonusStats[id][CRITCHANCE]-=bufferf;
    rating-= bufferf * RATING_CS;

    bufferf=float(item[Item_haste]);
    wcClientsBonusStats[id][HASTE]-=bufferf;
    rating-= bufferf * RATING_HASTE;

    bufferf=item[Item_parry];
    wcClientsBonusStats[id][PARRY]-=bufferf;
    rating-= bufferf * RATING_PARRY;

    wcRecalculateStats(client);
  }
  return rating;

}

HasShield(client) {
  if(IsClientInGame(client) && IsFakeClient(client))
    return BotHasShield(client);
  new uitem[UniqueItem];
  GetCharItem(uitem, client, 18);
  return uitem[Item_category]==18;
}

HasDual(client) {
  new uitem[UniqueItem];
  GetCharItem(uitem, client, 18);
  return uitem[Item_category]==22;
}


new g_ffClientGlyphs[65][MAX_GLYPHS];
new g_ff_ClientGlyphSlotsAvail[65];
ClearGlyphs(client) {
  g_ff_ClientGlyphSlotsAvail[client]=2;
  for (new i=0;i<MAX_GLYPHS;i++)
    g_ffClientGlyphs[client][i]=0;
}

GetGlyphSlots(client) {
  return g_ff_ClientGlyphSlotsAvail[client];
}
SetGlyphSlots(client,amount) {
  g_ff_ClientGlyphSlotsAvail[client]=amount;
}

GetGlyphId(client, slot)  {
  return g_ffClientGlyphs[client][slot];
}

SetGlyphId(client, slot,id)  {
  g_ffClientGlyphs[client][slot]=id;
}

bool:GetGlyphFull(client, slot, item[UniqueItem])  {
  new id = GetGlyphId(client,slot);
  if(id>0)
    GetItemProperties(item, id);
  return id>0;
}

GetGlyphOnStatic(client, glyphSlot, inventorySlot)  {
  new old = GetGlyphId(client, glyphSlot);
  new item[UniqueItem];
  GetInvItem(item, client, inventorySlot);
  SetGlyphId(client, glyphSlot, item[Item_defId]);
  wcSetStaticSlot(client, inventorySlot, 0);
  wcSetStaticSlotAmount(client, inventorySlot, 0);
  if(old > 0) {
    wcSetStaticSlot(client, inventorySlot, old);
    wcSetStaticSlotAmount(client, inventorySlot, -1);
  }
}

stock bool:HasGlyph(client, any:glyph) {
  for (new i=0;i<GetGlyphSlots(client);i++) {
    if(any:GetGlyphId(client, i) == glyph)  {
      return true;
    }
  }
  return false;
}


stock GetJustice(client)  {
  return GetAmountOfItems(client,13584);
}
stock GetValor(client)  {
  return GetAmountOfItems(client, 13583);
}
stock SpendJustice(client, amount)  {
  if(!RemoveItem(client,13584,amount))
    LogError("[WC] Error removing Justice from Character id %d amount %d!!",characterids[client][0], amount);

}
stock SpendValor(client, amount)  {
  if(!RemoveItem(client,13583,amount))
    LogError("[WC] Error removing Valor from Character id %d amount %d!!",characterids[client][0], amount);

}

stock bool:CanWearShield(race)  {
  return (race == 1 || race == 4 || race == 6);
}
stock bool:CanWearOffhand(race) {
  return (race == 3 || race == 5 || race == 7);
}

stock bool:GivePlayerRandomItem(client, String:reason[]="drop", announce=-1, forceRarity=-1)  {
  //Prerequirements check
  if(!IsClientInGame(client) || (playerloaded[client] != 1))  {
    return false;
  }
  if(wcCheckFreeSlot(client) <= 0) {
    WCMessage(client, "failed drop", client, "Random");
    return false;
  }

  //Register item
  new Handle:pack=CreateDataPack();
  WritePackString(pack, "TGivePlayerRandomItem");
  WritePackCell(pack, client);
  WritePackString(pack, reason);
  WritePackCell(pack, announce);
  WritePackCell(pack, forceRarity);
  TInsertItem(pack, client);
  return true;
}

public bool:TGivePlayerRandomItem(id, Handle:pack) {
  new uitem[UniqueItem];
  decl String:reason[50];
  ResetPack(pack);
  ReadPackString(pack, reason, 50);
  new client = ReadPackCell(pack);
  ReadPackString(pack, reason, 50);
  new announce = ReadPackCell(pack);
  new forceRarity = ReadPackCell(pack);
  //Re-test requirements
  if(!IsClientInGame(client) || (playerloaded[client] != 1))  {
    return false;
  }
  if(wcCheckFreeSlot(client) <= 0) {
    WCMessage(client, "failed drop", client, "Random");
    return false;
  }
  if(1 > GetLevel(client))
    return false;



  if(id <= 0 )  {
    LogError("could not give player unique id, id returned = %d", id);
    return false;
  }
  GiveItemUnknownProps(uitem);
  uitem[Item_id] = id;
  uitem[Item_defId] = 1;  //Barebone ID

  //Give item properties

  //Category?
  new cat = GetRandomInt(10, 23);
  //Reduce chances of weapons and shields
  if(cat == 18 || cat == 23)  {
    if(GetRandomInt(0,1) == 0)  {
      cat = GetRandomInt(10,23);
    }
  }
  //Quality?
  new quality = GetRandomInt(1,100);
  if(quality > 99 && isClientPDModule(client))  quality = 4; //Legendaries, 1% with premium, 0% otherwise
  else if(quality > 93)  quality = 3; //7% epic
  else if(quality > 70)  quality = 2; //23% rare
  else quality = 1; //Common

  if (forceRarity>=0 && forceRarity <= 4)
    quality = forceRarity;
  //Class?
  new race = GetClass(client);
  if(GetRandomInt(0,1) == 0 || race == 0)  race = GetRandomInt(1,MAX_CLASS);

  while(cat == 18 && (!CanWearShield(race)) && (!CanWearOffhand(race)) ) {
    cat = GetRandomInt(10,23);
  }


  //Level?
  new level = GetLevel(client);
  level = GetRandomInt(level-6, level);
  if(level == GetLevel(client) && level < 85-1) {
    level += GetRandomInt(0,2);
  }
  if(level <  1) level =  1;
  if(level > 85) level = 85;

  // If legendary, set level 80-85
  if (quality == 4)
  {
    level = GetRandomInt(81,85);
  }

  //ilevel
  new ilevel = level;
  if(GetRandomInt(1,3)==3)  {
    ilevel += GetRandomInt(1,3);
  }

  //Save some settings into item
  uitem[Item_level] = level;
  uitem[Item_ilevel] = ilevel;
  uitem[Item_quality] = quality;
  uitem[Item_category] = cat;
  uitem[Item_class] = 1 << race;

  uitem[Item_owner] = characterids[client][0];

  //2handed?
  if(cat == 22) {
    switch(race)  {
      // Monks don't have offhands
      // 2hand is later set to 0 for Rogues and speed increased to match dagger speeds
      // so 2hand = dagger if rogue
      case 1,2,8,9: { //Rogues will have 1h set in next section, this' to control naming.
        if(GetRandomInt(0,2) == 0)  uitem[Item_2hand] = 1;
      }
      case 3,4,5,7:
        if(GetRandomInt(0,1)) uitem[Item_2hand] = 1;
    }
  }

  //Give item values
  SetRandomValues(uitem);

  //Random Name
  new a=uitem[Item_agi];//1
  new b=uitem[Item_int];//2
  new c=uitem[Item_sta];//3
  new d=uitem[Item_str];//4
  new max = d;
  new pStat=4;
  if(a > max) {
    max = a;
    pStat = 1;
  }
  if(b > max) {
    max = b;
    pStat = 2;
  }
  if(c > max) {
    max = c;
    pStat = 3;
  }

  /*new pStat = (a>b)?((a>c)?((a>d)?1:4):((c>d)?3:4)):((b>c)?((b>d)?2:4):((c>d)?3:4));//gah this' ugly*/  //Tristen, you are a troll!

  GetRandomName(race, cat, quality, level, uitem[Item_Name], 50, pStat, uitem[Item_2hand]);

  //Save item
  SaveItemCode(uitem);

  uitem[Item_LastUpdate]=GetEngineTime();
  uitem[Item_LastUsage]=GetEngineTime();

  new index = GetUniqueInDB(uitem[Item_id]);
  if(index>=0)  {
    SetArrayArray(g_itemsArray, index, uitem[0]);
  }
  else  {
    PushArrayArray(g_itemsArray, uitem[0]);
  }


  //Give item to player
  new iSlot = GiveItem(client, uitem, 1, reason);
  WCMessage(client,"item dropped",client,uitem[Item_Name]);
  if (announce < 0) //-1=decide here, 0=default, 1=self, 2=all
  {
    if (uitem[Item_quality] >= 4) //legendary, any level.
      announce = 2;
    else if (uitem[Item_level] >= 82 && uitem[Item_quality] >= 3) //epic, level 82+
      announce = 2;
    else
      announce = 1;
  }
  switch(announce)
  {
    case 1:  AnnounceDrop(client,uitem[Item_Name],iSlot);
    case 2:  AnnounceDropAll(client, uitem[Item_Name], uitem[Item_quality]);
    default: AnnounceDrop(client,uitem[Item_Name],iSlot);
  }

  return true;
}




stock bool:GetRandomName(race, cat, quality, level, String:name[], size, pStat, handedness) {
  handedness++;
  decl String:names[50][50];
  new found=0;
  KvRewind(wcitemnames);
  KvGotoFirstSubKey(wcitemnames);
  do {
    if(KvGetNum(wcitemnames, "minlevel") > level) continue;
    if(KvGetNum(wcitemnames, "maxlevel") < level) continue;
    if(KvGetNum(wcitemnames, "quality") != quality) continue;

    KvGetString(wcitemnames, "name", names[found], 50, "Error");
    found++;
  }  while(KvGotoNextKey(wcitemnames));

  if(found == 0)  {
    Format(name, size, "Unique");
  }
  else  {
    new index = GetRandomInt(0,found-1);
    Format(name, size, "%s", names[index]);
  }

  KvRewind(wcbaseitemnames);
  new String:key[10];
  Format(key, sizeof(key), "%d", cat);
  if(!KvJumpToKey(wcbaseitemnames,key,false))
    LogError("Random name generator asked for unknown category (%s)", key);
  new String:part[30];
  new num;
  switch(cat) {
    case 10,11,12,13,14,15,16,17,19,20,21,23:  //armor items
    {
      switch (cat)
      {
        case 10:{part="Back";}
        case 11:{part="Chest";}
        case 12:{part="Boots";}
        case 13:{part="Hands";}
        case 14:{part="Helm";}
        case 15:{part="Leggings";}
        case 16:{part="Necklace";}
        case 17:{part="Ring";}
        case 18:{if(CanWearShield(race)){part="Shield";}else{part="Off-hand";}}
        case 19:{part="Shoulder";}
        case 20:{part="Trinket";}
        case 21:{part="Belt";}
        case 22:{part="Weapon";}
        case 23:{part="Wrists";}
        default:{part="Unknown";}
      }
      Format(key,sizeof(key),"%d",pStat);
      if(!KvJumpToKey(wcbaseitemnames,key,false))
        LogError("Random name generator asked for unknown stat in cat (%d) for %s equipment (key = %s)", cat,part,key);
      else
      {
        num=KvGetNum(wcbaseitemnames, "num");
        Format(key, sizeof(key),"%d",GetRandomInt(1,num));
        KvGetString(wcbaseitemnames,key,part, sizeof(part));
      }
    }
    case 18: //shield or offhand
    {
      if(CanWearShield(race)) //Shield
      {
        Format(key, sizeof(key),"%d",pStat);
        if(!KvJumpToKey(wcbaseitemnames,key,false))
          {
            part="Shield";
            LogError("Random name generator asked for unknown stat in Shield equipment (%s)",  key);
          }
        else
        {
          num=KvGetNum(wcbaseitemnames, "num");
          Format(key,sizeof(key),"%d",GetRandomInt(1,num));
          KvGetString(wcbaseitemnames,key, part, sizeof(part), "Shield");
        }
      }
      else //offhand
      {
        if(!KvJumpToKey(wcbaseitemnames,"5",false))
          {
            part="Off-hand";
            LogError("Random name generator asked for unknown stat in Off-hand equipment (5)");
          }
        else
        {
          if(race==0)
          {
            part="Off-hand";
          }
          else
          {
            Format(key,sizeof(key),"%d",race*100);
            num=KvGetNum(wcbaseitemnames, key);
            Format(key,sizeof(key),"%d",GetRandomInt(1,num)+race*100);
            KvGetString(wcbaseitemnames,key, part, sizeof(part), "Off-Hand");
          }
        }
      }
    }
    case 22: {
      part="Weapon";

      Format(key,sizeof(key),"%d",handedness);
      if(!KvJumpToKey(wcbaseitemnames,key,false))
      {
        part="Weapon";
        LogError("Random name generator asked for handedness other than one or two (%s)", key);
      }
      else
      {
        if(race==0)
        {
          part="Weapon";
        }
        else
        {
          Format(key,sizeof(key),"%d",race*100);
          num=KvGetNum(wcbaseitemnames, key);
          Format(key,sizeof(key),"%d",GetRandomInt(1,num)+race*100);
          KvGetString(wcbaseitemnames,key, part, sizeof(part), "Weapon");
        }
      }
    }
    default: //pretty sure we won't get here.
    {
      part="Thing???";
      LogError("Random name generator switch made it to default with (%s) category", key);
    }
  }
  decl String:sc[50];
  Format(sc,sizeof(sc),"class_%i",race);
  Format(sc,sizeof(sc),"%T",sc,LANG_SERVER);

  Format(name, size, "%s %s (%s)", name, part, sc);
  return true;
}

stock SetRandomValues(item[UniqueItem]) {
  //Find Slot, Quality, dred ...
  new Float:slots[] = { 0.3, 1.0, 0.65, 0.5, 0.8, 0.9, 0.65, 0.6, 2.0, 0.6, 0.9, 0.5, 1.0, 0.4 };
  new Float:qualities[] = { 1.0, 1.0, 1.2, 1.4, 1.7, 1.25, 1.45 };
  new Float:dreds[] = { 0.16, 0.28, 0.48 };
  new Float:slot, Float:quality;
  slot = slots[item[Item_category]-10];
  quality = qualities[item[Item_quality]];
  new Float:level = float(item[Item_ilevel]);
  new armorType=0;


  //Give item dred, dmg type
  new Float:dred = 0.48;
  new Float:dmg= 0.0;
  new Float:sp = 0.0;
  new Float:armor = 0.0;

  new classes[10];
  ConvertBitToClasses(item[Item_class], classes);
  new race = classes[0];
  new Float:pointMulti=1.0;

  if(item[Item_category] == 22) {
    //Give random Weapon Speed
    new Float:ws = GetRandomFloat(0.95, 1.14);
    if(item[Item_2hand] == 1)
    {
      ws = GetRandomFloat(0.8, 1.0);
    }
    if(race == 2 && item[Item_2hand] == 1)
    {
      ws = GetRandomFloat(1.20, 1.35);
      item[Item_2hand] = 0; //Removed 2handedness after naming has determined it was a dagger.
    }
    item[Item_ws] = ws;
  }

  switch(race)  {
    case 0: {
      dred = dreds[0];
      dmg = 1.0;
      sp = 0.0;
      armorType = 1;
    }
    case 1: {
      dred = dreds[2];
      dmg = 1.0;
      sp = 0.0;
      pointMulti = 0.6;
      armorType = 3;
    }
    case 2: {
      dred = dreds[1];
      dmg = 1.0;
      sp = 0.0;
      pointMulti = 0.8;
      armorType = 2;
    }
    case 3: {
      dred = dreds[0];
      dmg = 0.0;
      sp = 1.0;
      armorType = 1;
    }
    case 4: {
      dred = dreds[2];
      dmg = 0.7;
      sp = 0.7;
      pointMulti = 0.6;
      armorType = 3;
    }
    case 5: {
      dred = dreds[0];
      dmg = 0.0;
      sp = 1.0;
      armorType = 1;
    }
    case 6: {
      dred = dreds[1];
      dmg = 0.7;
      sp = 0.7;
      pointMulti = 0.8;
      armorType = 2;
    }
    case 7: {
      dred = dreds[0];
      dmg = 0.0;
      sp = 1.0;
      armorType = 1;
    }
    case 8: {
      dred = dreds[2];
      dmg = 1.0;
      sp = 0.0;
      pointMulti = 0.6;
      armorType = 3;
    }
    case 9: {
      dred = dreds[1];
      dmg = 0.7;
      sp= 0.7;
      pointMulti = 0.8;
      armorType = 2;
    }
  }
  item[Item_armor_type] = armorType;

  //Valuate item
  new Float:points = float(item[Item_ilevel]) * 0.272277 * slot * (item[Item_category] == 22?0.7:1.0) * quality * pointMulti;

  dmg = 0.26 * level * slot * quality * 0.5;
  sp = level * slot * 0.275 * 0.75 * Pow(quality, 0.5);
  armor = (level * 6.25 + 20.0) * dred * slot / ((1.0 - dred) * 7.65) * quality;

  if(item[Item_category] != 22) {
    dmg = 0.0;
    sp = 0.0;
  }
  if(item[Item_category] == 16 || item[Item_category] == 17 || item[Item_category] == 20 || item[Item_category] == 22)  {
    armor = 0.0;
  }
  //Offhand support
  if(item[Item_category] == 18 && CanWearOffhand(race)) {
    armor = 0.0;
  }

  if(item[Item_2hand])  {
    dmg *= 2.0;
    sp *= 2.0;
    points *= 2.0;
  }

  //Support for caster 1h + offhand
  if(!item[Item_2hand] && CanWearOffhand(race)) {
    sp *= 1.9;
  }

  item[Item_weapon_ap] = RoundToCeil(dmg);
  item[Item_weapon_sp] = RoundToCeil(sp);
  item[Item_armor] = RoundToCeil(armor);


  item[Item_cost] = RoundToCeil( Pow(1.3, level / 4.0) * 31.0 * Pow(quality, 2.0) / 10.1  * slot * 1.8 * (item[Item_2hand] == 1?2.0:1.0) );

  //Stat - secondary stats balance
  new Float:division = GetRandomFloat(0.0,1.0);
  new Float:statPoints = points * division;
  new Float:secPoints = points * (1.0 - division);

  //Give stat points based on stats.txt
  new str,agi,inte,sta;
  new Float:oldPoints = statPoints;
  while(statPoints > 0.0) {
    statPoints = statPoints - 1.0;

    new StatPoint:point = _getNextStat(race, str, agi, inte, sta);
    switch(point) {
      case SP_Strength: str++;
      case SP_Agility: agi++;
      case SP_Intellect: inte++;
      case SP_Stamina: sta++;
    }
  }
  //Now randomize stat points we got
  new total = str+agi+inte+sta;
  if(total > 0)  {
    new Float:qStr = float(str) / total;
    new Float:qAgi = float(agi) / total;
    new Float:qInt = float(inte) / total;
    new Float:qSta = float(sta) / total;
    qStr *= GetRandomFloat(0.0, 2.0);
    qAgi *= GetRandomFloat(0.0, 2.0);
    qInt *= GetRandomFloat(0.0, 2.0);
    qSta *= GetRandomFloat(0.0, 2.0);
    new Float:norm = qStr + qAgi + qInt + qSta;
    qStr /= norm;
    qAgi /= norm;
    qSta /= norm;
    qInt /= norm;
    str = RoundToNearest(qStr * oldPoints);
    agi = RoundToNearest(qAgi * oldPoints);
    sta = RoundToNearest(qSta * oldPoints);
    inte = RoundToNearest(qInt * oldPoints);
  }

  item[Item_str] = str;
  item[Item_agi] = agi;
  item[Item_sta] = sta;
  item[Item_int] = inte;

  while(secPoints > 0.0)  {
    //AS, resis, MS, haste CS

    //Regen, SP
    //Accu, AI
    new q = GetRandomInt(1,20);

    if(q<=17) //85% chance (78% originally)
    { //'common' stats, AP,SP,AI,Res,Regen & Haste
      new stat = GetRandomInt(0,6);

      switch(stat)
      {
        case 0:
        {
          if(race != 1 && race != 2 && race != 8 && race != 9)
          {
            item[Item_spelldmg] += 1;
            secPoints -= 3.5;
          }
        }
        case 1:
        {
          if(race != 3 && race != 5 && race != 7 && race != 9)
          {
            item[Item_damage] += 1;
            secPoints -= 3.5;
          }
        }
        case 2:
        {
          if(race != 1 && race != 2 && race != 8 && race != 9)
          {
            item[Item_regen] += 1;
            secPoints -= 2.0;
          }
        }
        case 3:
        {
          if(race != 3 && race != 4 && race != 5 && race != 7)
          {
            item[Item_ai] += 1;
            secPoints -= 0.5;
          }
        }
        case 4:
        {
          item[Item_res] += 1;
          secPoints -= 1;
        }
        case 5:
        {
          item[Item_haste] += 1;
          secPoints -= 1;
        }
        case 6:
        {
          if (item[Item_category] == 13 || //Gloves
              item[Item_category] == 17 || //Ring
              item[Item_category] == 20 || //Trinket
              item[Item_category] == 22 || //Weapon
              item[Item_category] == 23 )  //Wrist
          {
            item[Item_parry] += 0.01;
            secPoints -= 3;
          }
        }
      }
    }
    else    //'uncommon' stats, Crit,MS,AS,Acc.
    {
      new stat = GetRandomInt(0,3);

      switch(stat)
      {
        case 0:
        {
          if(race != 9 && race != 8)
          {
          item[Item_as] += 0.01;
          secPoints -= 0.01 * 0.25 * 15.0 * (9.0 + level);
          }
        }
        case 1:
        {
          item[Item_speed] += 0.01;
          secPoints -= 0.01 * 0.25 * 32.0 * (10.0 + level);
        }
        case 2:
        {
          item[Item_cs] += 0.01;
          secPoints -= 0.01 * 0.25 * 29.0 * (25.0 + level);
        }
        case 3:
        {
          if(race != 3 && race != 5 && race != 7 && race != 9)
          {
            item[Item_acc] += 0.01;
            secPoints -= 0.01 * 0.25 * 9.5 * (10.0 + level);
          }
        }
      }
    }
  }
}

AddItem(adding[UniqueItem],ret[UniqueItem]) {
  ret[Item_armor] += adding[Item_armor];
  ret[Item_str] += adding[Item_str];
  ret[Item_agi] += adding[Item_agi];
  ret[Item_sta] += adding[Item_sta];
  ret[Item_int] += adding[Item_int];
  ret[Item_res] += adding[Item_res];
  ret[Item_regen] += adding[Item_regen];
  ret[Item_ai] += adding[Item_ai];
  ret[Item_haste] += adding[Item_haste];
  ret[Item_parry] += adding[Item_parry];
  ret[Item_as] += adding[Item_as];
  ret[Item_cs] += adding[Item_cs];
  ret[Item_speed] += adding[Item_speed];
  ret[Item_acc] += adding[Item_acc];
  ret[Item_resilence] += adding[Item_resilence];
  if(adding[Item_ws] > 0.0)
    ret[Item_ws] = adding[Item_ws];
  ret[Item_damage] += adding[Item_damage];
  ret[Item_weapon_ap] += adding[Item_weapon_ap];
  ret[Item_weapon_sp] += adding[Item_weapon_sp];
  ret[Item_spelldmg] += adding[Item_spelldmg];
}

ItemCompare(newitem[UniqueItem], olditem[UniqueItem],client)
{

  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"%T","item compare", client);

  if((newitem[Item_armor]-olditem[Item_armor])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name armor",client,newitem[Item_armor]-olditem[Item_armor]);
  }
  if((newitem[Item_str]-olditem[Item_str])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name str",client,newitem[Item_str]-olditem[Item_str]);
  }
  if((newitem[Item_agi]-olditem[Item_agi])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name agi",client,newitem[Item_agi]-olditem[Item_agi]);
  }
  if((newitem[Item_sta]-olditem[Item_sta])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name sta",client,newitem[Item_sta]-olditem[Item_sta]);
  }
  if((newitem[Item_int]-olditem[Item_int])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name int",client,newitem[Item_int]-olditem[Item_int]);
  }
  if((newitem[Item_res]-olditem[Item_res])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name res",client,newitem[Item_res]-olditem[Item_res]);
  }
  if((newitem[Item_regen]-olditem[Item_regen])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name regen",client,newitem[Item_regen]-olditem[Item_regen]);
  }
  if((newitem[Item_ai]-olditem[Item_ai])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name ai",client,newitem[Item_ai]-olditem[Item_ai]);
  }
  if((newitem[Item_haste]-olditem[Item_haste])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name haste",client,newitem[Item_haste]-olditem[Item_haste]);
  }
  if((newitem[Item_haste]-olditem[Item_parry])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name parry",client,newitem[Item_parry]-olditem[Item_parry]);
  }
  if((newitem[Item_as]-olditem[Item_as])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %d\%,",buffer,"name as",client,RoundToNearest((newitem[Item_as]-olditem[Item_as])*100.0));
  }
  if((newitem[Item_cs]-olditem[Item_cs])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %d\%,",buffer,"name cs",client,RoundToNearest((newitem[Item_cs]-olditem[Item_cs])*100.0));
  }
  if((newitem[Item_speed]-olditem[Item_speed])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %d\%,",buffer,"name speed",client,RoundToNearest((newitem[Item_speed]-olditem[Item_speed])*100.0));
  }
  if((newitem[Item_acc]-olditem[Item_acc])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %d\%,",buffer,"name acc",client,RoundToNearest((newitem[Item_acc]-olditem[Item_acc])*100.0));
  }
  if((newitem[Item_resilence]-olditem[Item_resilence])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name resilience",client,newitem[Item_resilence]-olditem[Item_resilence]);
  }
  if((newitem[Item_ws]-olditem[Item_ws])!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %d\%,",buffer,"name ws",client,RoundToNearest((newitem[Item_ws]-olditem[Item_ws])*100.0));
  }
  if(((newitem[Item_damage] + newitem[Item_weapon_ap])-(olditem[Item_damage]+olditem[Item_weapon_ap]))!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name damage",client,((newitem[Item_damage] + newitem[Item_weapon_ap])-(olditem[Item_damage]+olditem[Item_weapon_ap])));
  }
  if(((newitem[Item_spelldmg] + newitem[Item_weapon_sp])-(olditem[Item_spelldmg]+olditem[Item_weapon_sp]))!=0)
  {
    Format(buffer,sizeof(buffer),"%s %T %i,",buffer,"name spelldmg",client,((newitem[Item_spelldmg] + newitem[Item_weapon_sp])-(olditem[Item_spelldmg]+olditem[Item_weapon_sp])));
  }
  buffer[strlen(buffer)-1] = '.';
  PlainMessage(client,buffer);
}
