/*
* File: cast_controller.inc
* Description: Is having main purpose to control and decide what to cast.
* this is used by rogues and warriors because their energy/rage pool is not as big
* as mana pool for casters
* Author(s): Stinkyfax
*/

#pragma semicolon 1

enum SpellSetting {
  bool:SS_isEnabled,
  SS_minRage  //Same for energy

}


new g_ff_spellSettings[65][25][SpellSetting];
new g_ff_talentSettings[65][MAX_TALENTS+30][SpellSetting];


clearSpellSettings(client)  {
  for(new i=0;i<25;i++) {
    g_ff_spellSettings[client][i][SS_isEnabled]=true;
    g_ff_spellSettings[client][i][SS_minRage]=0;
  }
  for(new i=0;i<MAX_TALENTS+30;i++) {
    g_ff_talentSettings[client][i][SS_isEnabled]=true;
    g_ff_talentSettings[client][i][SS_minRage]=0;
  }
}

stock loadSpellSettings(client, Handle:results) {
  if(!results)
    return;
  SQL_Rewind(results);
  while(SQL_FetchRow(results))  {
    new spellId = GetSQLDataInt(results, "spellId");
    new iIsEnabled = GetSQLDataInt(results, "isEnabled");
    new bool:isEnabled = (iIsEnabled != 0);
    new minRage = GetSQLDataInt(results, "minRage");

    if(spellId < 0) {
      spellId*=-1;
      g_ff_talentSettings[client][spellId][SS_isEnabled]=isEnabled;
      g_ff_talentSettings[client][spellId][SS_minRage]=minRage;
    }
    else  {
      g_ff_spellSettings[client][spellId][SS_isEnabled]=isEnabled;
      g_ff_spellSettings[client][spellId][SS_minRage]=minRage;
    }
  }
}

stock saveSpellSettings(client, Handle:thread)  {
  decl String:buffer[900];

  Format(buffer, sizeof(buffer), "DELETE FROM wc_Spells WHERE characterId = %d", characterids[client][0]);
  incQueue();
  SQL_TQuery(thread,TRepeating2,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);

  new point = Format(buffer, sizeof(buffer), "INSERT INTO wc_Spells (`characterId`, `spellId`, `isEnabled`, `minRage`) VALUES ");
  new bool:has=false;
  for(new i=0; i<25; i++) {
    new bool:pass=false;
    if(!g_ff_spellSettings[client][i][SS_isEnabled])
      pass=true;
    if(g_ff_spellSettings[client][i][SS_minRage]>0)
      pass=true;
    if(!pass)
      continue;
    if(has)
      point+=Format(buffer[point], sizeof(buffer)-point, ", ");
    has=true;
    point+=Format(buffer[point], sizeof(buffer)-point, "(%d, %d, %d, %d)", characterids[client][0], i, g_ff_spellSettings[client][i][SS_isEnabled], g_ff_spellSettings[client][i][SS_minRage]);
  }
  for(new i=0; i<MAX_TALENTS+30; i++) {
    new bool:pass=false;
    if(!g_ff_talentSettings[client][i][SS_isEnabled])
      pass=true;
    if(g_ff_talentSettings[client][i][SS_minRage]>0)
      pass=true;
    if(!pass)
      continue;
    if(has)
      point+=Format(buffer[point], sizeof(buffer)-point, ", ");
    has=true;
    point+=Format(buffer[point], sizeof(buffer)-point, "(%d, %d, %d, %d)", characterids[client][0], i*-1, g_ff_talentSettings[client][i][SS_isEnabled], g_ff_talentSettings[client][i][SS_minRage]);
  }

  if(!has)
    return;
  incQueue();
  SQL_TQuery(thread, TRepeating2,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
}



//Will return true/false depending on client settings like minimum rage
//and if spell is activated
bool:castIsRecommended(client, spellid) {

  //Handle talets, which are less than 0
  if(spellid < 0) {
    spellid *= -1; //normalize
    //We already know it is controlled, so we just check settings
    if(!g_ff_talentSettings[client][spellid][SS_isEnabled])
      return false;
    new rage = GetMana(client);
    if(rage < g_ff_talentSettings[client][spellid][SS_minRage])
      return false;
    return true;
  }


  //return true if it is not under controller scope
  if(wcGetSpellInfoInt(GetClass(client), spellid, "controlled")<=0)
    return true;

  if(!g_ff_spellSettings[client][spellid][SS_isEnabled])
    return false;
  new rage = GetMana(client);
  if(rage < g_ff_spellSettings[client][spellid][SS_minRage])
    return false;
  return true;
}

displaySealsMenu(client) {
  if(playerloaded[client]!=1)
    return;

  new Handle:menu=CreateMenu(SealsMenuHandler);
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[50];
  Format(title,sizeof(title),"%T","wc paladin seals",client);
  SetMenuTitle(menu,title);

  /*** Menu Items ***/
  decl String:line[255];
  new holy = wcGetTreeTotal(client, getActiveSpec(client),1);
  new protection = wcGetTreeTotal(client, getActiveSpec(client),2);
  new retribution = wcGetTreeTotal(client, getActiveSpec(client),3);
  Format(line, 255, "Holy %d / Protection %d / Retribution %d", holy, protection, retribution);
  AddMenuItem(menu, "", line);


  if(holy>=6)
    Format(line, 255, "Active");
  else
    Format(line, 255, "Requires 6 in Holy");
  Format(line, 255, "Seal of Light (%s)", line);
  AddMenuItem(menu, "", line);

  if(holy>=11)
    Format(line, 255, "Active");
  else
    Format(line, 255, "Requires 11 in Holy");
  Format(line, 255, "Seal of Wisdom (%s)", line);
  AddMenuItem(menu, "", line);

  if(holy>=16)
    Format(line, 255, "Active");
  else
    Format(line, 255, "Requires 16 in Holy");
  Format(line, 255, "Seal of Insight (%s)", line);
  AddMenuItem(menu, "", line);


  if(protection>=6)
    Format(line, 255, "Active");
  else
    Format(line, 255, "Requires 6 in Protection");
  Format(line, 255, "Seal of Justice (%s)", line);
  AddMenuItem(menu, "", line);

  if(protection>=11)
    Format(line, 255, "Active");
  else
    Format(line, 255, "Requires 11 in Protection");
  Format(line, 255, "Seal of Righteousness (%s)", line);
  AddMenuItem(menu, "", line);
  if(protection>=16)
    Format(line, 255, "Active");
  else
    Format(line, 255, "Requires 16 in Protection");
  Format(line, 255, "Seal of Truth (%s)", line);
  AddMenuItem(menu, "", line);



  if(retribution>=6)
    Format(line, 255, "Active");
  else
    Format(line, 255, "Requires 6 in Retribution");
  Format(line, 255, "Seal of Crusader (%s)", line);
  AddMenuItem(menu, "", line);

  if(retribution>=11)
    Format(line, 255, "Active");
  else
    Format(line, 255, "Requires 11 in Retribution");
  Format(line, 255, "Seal of Command (%s)", line);
  AddMenuItem(menu, "", line);
  if(retribution>=16)
    Format(line, 255, "Active");
  else
    Format(line, 255, "Requires 16 in Retribution");
  Format(line, 255, "Seal of Blood (%s)", line);
  AddMenuItem(menu, "", line);



  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

displayCastSettingsMenu(client) {
  if(playerloaded[client]!=1)
    return;

  new Handle:menu=CreateMenu(CastSettingsHandler);
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  Format(title,sizeof(title),"%T","wc cast settings menu",client);
  SetMenuTitle(menu,title);


  /*** Menu Items ***/
  //loop through spells
  for (new i=1;i<=50;i++)
  {
    new spellid=wcGetSpell(client,i);
    if(spellid!=0)
    {
      new race = GetClass(client);
      if(wcGetSpellInfoInt(race, spellid, "controlled")<=0)
        continue;

      decl String:line[255];
      wcGetSpellInfo(GetClass(client),spellid,"name",line, client);

      new costs = GetSpellCost(client, spellid);
      new minRage = g_ff_spellSettings[client][spellid][SS_minRage];
      new bool:enabled = g_ff_spellSettings[client][spellid][SS_isEnabled];
      decl String:sEnabled[20];
      if(enabled)
        sEnabled="Enabled";
      else
        sEnabled="Disabled";
      decl String:format[100];
      Format(format, sizeof(format), "%s (%d): %s, Threshold - %d", line, costs, sEnabled, minRage);

      decl String:spellstr[10];
      IntToString(spellid,spellstr,sizeof(spellstr));
      AddMenuItem(menu,spellstr,format);
    }
  }
  new race = GetClass(client);
  if(race>0)  for(new i=0; i<MAX_TALENTS+30;i++)  {
    new tree,tier,talent;
    if(getTalentPath(race,i,tree,tier,talent))  {
      decl String:tmp[255];
      wcGetTalentInfo(race,tree,tier,talent,client,"controlled",tmp);
      if(StringToInt(tmp) >= 1) {
        new spellid = i;
        new costs = GetTalentCost(client, tree, tier, talent);
        new minRage = g_ff_talentSettings[client][spellid][SS_minRage];
        new bool:enabled = g_ff_talentSettings[client][spellid][SS_isEnabled];
        decl String:sEnabled[20];
        if(enabled)
          sEnabled="Enabled";
        else
          sEnabled="Disabled";
        decl String:format[100];
        wcGetTalentInfo(race,tree,tier,talent,client,"name",tmp, client);
        Format(format, sizeof(format), "%s (%d): %s, Threshold - %d", tmp, costs, sEnabled, minRage);
        decl String:spellstr[10];
        IntToString(spellid*-1,spellstr,sizeof(spellstr));
        AddMenuItem(menu,spellstr,format);
      }
    }
  }


  new count=GetMenuItemCount(menu);
  if(count==0)
  {
    decl String:line[200];
    Format(line,sizeof(line),"%T","no spell",client);
    AddMenuItem(menu,line,line,ITEMDRAW_DISABLED);
  }

  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public SealsMenuHandler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    new race = GetClass(client);
    switch(slot)  {
      case 0: TalentsMenu(client);
      case 1: SpellInf(client, 14, GetSpellsHandle(race), false);
      case 2: SpellInf(client, 15, GetSpellsHandle(race), false);
      case 3: SpellInf(client, 21, GetSpellsHandle(race), false); //Seal of insight
      case 4: SpellInf(client, 12, GetSpellsHandle(race), false);
      case 5: SpellInf(client, 13, GetSpellsHandle(race), false);
      case 6: SpellInf(client, 19, GetSpellsHandle(race), false); //Seal of Truth
      case 7: SpellInf(client, 7, GetSpellsHandle(race), false);
      case 8: SpellInf(client, 6, GetSpellsHandle(race), false);
      case 9: SpellInf(client, 20, GetSpellsHandle(race), false); //Seal of Blood
    }
  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      ClassMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

public CastSettingsHandler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {

    decl String:buffer[10];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));

    ChooseSpellSetting(client, StringToInt(buffer));

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      ClassMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

new g_ff_spellsetting_spellid[65];
ChooseSpellSetting(client, spellid) {
  if(playerloaded[client]!=1)
    return;
  g_ff_spellsetting_spellid[client]=spellid;

  new Handle:menu=CreateMenu(ChooseSpellSettingHandler);
  SetMenuExitBackButton(menu,true);

  /*** Title ***/
  decl String:title[128];
  decl String:name[255];
  if(spellid>=0)
    wcGetSpellInfo(GetClass(client),spellid,"name",name, client);
  else  {
    new saveid = spellid *-1;
    new tree,tier,talent;
    if(!getTalentPath(GetClass(client), saveid, tree, tier, talent))  {
      LogError("[WC] Error in cast_controller logics: 341");
      return;
    }
    //TODO: finish
    wcGetTalentInfo(GetClass(client),tree,tier,talent,client,"name",name, client);
  }
  Format(title,sizeof(title),"%T","wc cast setting menu",client, name);
  SetMenuTitle(menu,title);

  decl String:manaName[10];
  GetManaName(client, manaName);


  /*** Menu Items ***/
  //loop through spells
  //AddMenuItem(menu,"","Turn Off");
  decl String:format[50];
  Format(format, 50, "Disable this ability");
  AddMenuItem(menu,"-1",format);
  Format(format, 50, "%d %s minimum", 0, manaName);
  AddMenuItem(menu,"0",format);
  Format(format, 50, "%d %s minimum", 10, manaName);
  AddMenuItem(menu,"10",format);
  Format(format, 50, "%d %s minimum", 25, manaName);
  AddMenuItem(menu,"25",format);
  Format(format, 50, "%d %s minimum", 40, manaName);
  AddMenuItem(menu,"40",format);
  Format(format, 50, "%d %s minimum", 60, manaName);
  AddMenuItem(menu,"60",format);
  Format(format, 50, "%d %s minimum", 90, manaName);
  AddMenuItem(menu,"90",format);


  /*** Send to client ***/
  DisplayMenu(menu,client,MENU_TIME_FOREVER);

}

public ChooseSpellSettingHandler(Handle:menu,
                        MenuAction:action,
                        client, slot) {
  if(action == MenuAction_Select) {
    decl String:buffer[10];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));
    new rage = StringToInt(buffer);
    if(rage < 0)
      ChangeSpellSetting(client, g_ff_spellsetting_spellid[client],
        false);
    else
      ChangeSpellSetting(client, g_ff_spellsetting_spellid[client],
        true, rage);

    displayCastSettingsMenu(client);

  }

  if(action == MenuAction_Cancel)
    if(slot == MenuCancel_ExitBack) {
      displayCastSettingsMenu(client);
    }
  if(action == MenuAction_End)
    CloseHandle(menu);

}

stock ChangeSpellSetting(client, spellId, bool:isEnabled, rage=-1)   {
  if(spellId>0) {
    g_ff_spellSettings[client][spellId][SS_isEnabled]=isEnabled;
    if(rage>=0)
      g_ff_spellSettings[client][spellId][SS_minRage]=rage;
  }
  else  {
    spellId *= -1;
    g_ff_talentSettings[client][spellId][SS_isEnabled]=isEnabled;
    if(rage>=0)
      g_ff_talentSettings[client][spellId][SS_minRage]=rage;
  }


}

stock bool:triggerControlledSpell(client, spellId, bool:msg=true) {
  new bool:now = false;
  if(spellId >= 0 ) now = g_ff_spellSettings[client][spellId][SS_isEnabled];
  else
    now = g_ff_talentSettings[client][spellId*-1][SS_isEnabled];
  if(now) {
    ChangeSpellSetting(client, spellId, false);
    if(msg) {
      decl String:spellname[255];
      if(spellId > 0)
        wcGetSpellInfo(GetClass(client), spellId, "name", spellname, client);
      else  {
        new saveid = spellId *-1;
        new tree,tier,talent;
        if(!getTalentPath(GetClass(client), saveid, tree, tier, talent))  {
          LogError("[WC] Error in cast_controller logics: 341");
          return false;
        }
        wcGetTalentInfo(GetClass(client),tree,tier,talent,client,"name",spellname, client);
      }
      WCMessage(client, "auto cast off", client, spellname);
    }
    return false;
  }
  else  {
    ChangeSpellSetting(client, spellId, true);
    if(msg) {
      decl String:spellname[255];
      if(spellId > 0)
        wcGetSpellInfo(GetClass(client), spellId, "name", spellname, client);
      else  {
        new saveid = spellId *-1;
        new tree,tier,talent;
        if(!getTalentPath(GetClass(client), saveid, tree, tier, talent))  {
          LogError("[WC] Error in cast_controller logics: 456");
          return false;
        }
        wcGetTalentInfo(GetClass(client),tree,tier,talent,client,"name",spellname, client);
      }
      WCMessage(client, "auto cast on", client, spellname);
    }
    return true;
  }
}

stock bool:IsCaster(client)  {
  if(IsZombie(client))
    return false;
  return _isCaster(GetClass(client));
}

stock bool:_isCaster(race)  {
  switch(race)  {
    case 1: return false;
    case 2: return false;
    case 8: return false;
    case 9: return false;
  }
  return true;

}

stock bool:CanSilence(client) {
  if(IsZombie(client))  return false;
  switch(GetClass(client))  {
    case 1: return false;
    case 2: return false;
  }
  return true;
}

stock GetManaName(client, String:name[])  {
  switch(GetClass(client))  {
    case 1: Format(name, 9, "Rage");
    case 2: Format(name, 9, "NRG");
    case 8: Format(name, 9, "Power");
    case 9: Format(name, 9, "NRG");
    default: Format(name, 9, "Mana");
  }
}

new g_ffAutoCast[65];
UpdateAutocast(client, value) {
  g_ffAutoCast[client] = value;
  for(new i=1;i<32; i++)  {
    if(value & (1 << i) > 0)  {
      setAutoCast(client,i, false);
    }
  }
}

_addAutocast(client, val, bool:disable) {
  if(disable) {
    g_ffAutoCast[client] = g_ffAutoCast[client] | 1 << val;
  }
  else  {
    g_ffAutoCast[client] &= ~(1 << val);
  }
}

GetAutocast(client) {
  return g_ffAutoCast[client];
}

