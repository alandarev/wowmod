/*
* File: mysql.inc
* Description: MySQL related functions (heart of the mod)
* Author(s): Stinkyfax
*/

#pragma semicolon 1

// Define vector handle
new Handle:kvDb=INVALID_HANDLE; // KeyValues storing DB info
new Handle:kvCheckDb=INVALID_HANDLE; //KV for connectint main db.

//new tmsql=0;



new Handle:g_queryArray;

new g_queryId=0;

Handle:CreateQueryArray()   {
   decl query[Query];
   query[Q_Id]=1;
   return CreateArray(sizeof(query));
}


bool:DeleteQueryById(id)   {
   for (new i=0; i<GetArraySize(g_queryArray); i++)   {
      if(GetArrayCell(g_queryArray, i, 0) == id)   {
         RemoveFromArray(g_queryArray, i);
         return true;
      }
   }
   return false;
}

bool:FindQueryById(query[Query], id) {
   query[Q_Id] = 0;
   for (new i=0; i<GetArraySize(g_queryArray); i++)   {
      if(GetArrayCell(g_queryArray, i, 0) == id)   {
         GetArrayArray(g_queryArray, i, query[0]);
         return true;
      }
   }
   return false;
}

stock AddQuery(QueryType:type, repeats, String:command[], var=0) {
   decl query[Query];
   strcopy(query[Q_Line], sizeof(query[Q_Line]), command);
   new id = GenerateQueryId();
   query[Q_Id] = id;
   query[Q_Repeats] = repeats;
   query[Q_Type] = type;
   query[Q_Var] = var;
   PushArrayArray(g_queryArray, query[0]);
   return id;
}

GenerateQueryId()  {
   g_queryId++;
   if(g_queryId>5000)
      g_queryId=1;
   return g_queryId;
}

public TRGetItem(Handle:db, Handle:hndl, String:error[], any:id)
{
   decl query[Query];
   new bool:bQuery=false;
   if(FindQueryById(query, id))
      bQuery=true;
   if (hndl == INVALID_HANDLE) {
      decl String:sError[300];
      Format(sError,sizeof(sError), "[WC] 1011 Error: %s", error);
      new bool:bError=true;
      if(StrContains(error, "Lost connection", false)>=0)   {
         if(bQuery)   {
            if( (query[Q_Type] == Repeat) && ((query[Q_Repeats]--) > 0))   {
               bError=false;
               //edit here
               SQL_TQuery(db, TRGetItem, query[Q_Line], id, DBPrio_High);
            }
            if(bError)  {
               Format(sError, sizeof(sError), "Query is failed with error %s and q = %s", error, query[Q_Line]);
            }
         }
      }
      if(bError)  {
         //Function specific code for error

         //end
         LogError(sError);
         if(bQuery)
            DeleteQueryById(id);
      }
   }

   else  {

    if(bQuery)  {
      LoadItemCode(hndl, query[Q_Var]);
      DeleteQueryById(id);
    }
   }
}

public TRepeating2(Handle:db, Handle:hndl, String:error[], any:id)
{
   decl query[Query];
   new bool:bQuery=false;
   if(FindQueryById(query, id))
      bQuery=true;
   if (hndl == INVALID_HANDLE) {
      decl String:sError[300];
      Format(sError,sizeof(sError), "[WC] 1012 Error: %s", error);
      new bool:bError=true;
      if(StrContains(error, "Lost connection", false)>=0)   {
         if(bQuery)   {
            if( (query[Q_Type] == Repeat) && ((query[Q_Repeats]--) > 0))   {
               bError=false;
               SQL_TQuery(db, TRepeating2, query[Q_Line], id, DBPrio_High);
            }
            if(bError)  {
               Format(sError, sizeof(sError), "Query is failed with error %s and q = %s", error, query[Q_Line]);
            }
         }
      }
      if(bError)  {
         //Function specific code for error
         g_T2Queue--;
         //end
         Format(sError, sizeof(sError), "%s Q = %s", sError, query[Q_Line]);
         LogError(sError);
         if(bQuery)
            DeleteQueryById(id);
      }
   }

   else  {
    //Insert query specific code here
    g_T2Queue--;
    //end
    if(bQuery)
      DeleteQueryById(id);
   }
}

public TRepeating3(Handle:db, Handle:hndl, String:error[], any:id)
{
   decl query[Query];
   new bool:bQuery=false;
   if(FindQueryById(query, id))
      bQuery=true;
   if (hndl == INVALID_HANDLE) {
      decl String:sError[300];
      Format(sError,sizeof(sError), "[WC] 1013 Error: %s", error);
      new bool:bError=true;
      if(StrContains(error, "Lost connection", false)>=0)   {
         if(bQuery)   {
            if( (query[Q_Type] == Repeat) && ((query[Q_Repeats]--) > 0))   {
               bError=false;
               SQL_TQuery(db, TRepeating3, query[Q_Line], id, DBPrio_High);
            }
            if(bError)  {
               Format(sError, sizeof(sError), "Query is failed with error %s and q = %s", error, query[Q_Line]);
            }
         }
      }
      if(bError)  {
         //Function specific code for error
         g_T2Queue--;
         if(G_RebootTotal>0)
            G_RebootQueue--;
         //end
         LogError(sError);
         if(bQuery)
            DeleteQueryById(id);
      }
   }

   else  {
    //Insert query specific code here
    g_T2Queue--;
    if(G_RebootTotal>0)
      G_RebootQueue--;
    //end
    if(bQuery)
      DeleteQueryById(id);
   }
}

public TRepeating(Handle:db, Handle:hndl, String:error[], any:id)
{
   decl query[Query];
   new bool:bQuery=false;
   if(FindQueryById(query, id))
      bQuery=true;
   if (hndl == INVALID_HANDLE) {
      decl String:sError[300];
      Format(sError,sizeof(sError), "[WC] 1014 Error: %s", error);
      new bool:bError=true;
      if(StrContains(error, "Lost connection", false)>=0)   {
         if(bQuery)   {
            if( (query[Q_Type] == Repeat) && ((query[Q_Repeats]--) > 0))   {
               bError=false;
               SQL_TQuery(db, TRepeating, query[Q_Line], id, DBPrio_High);
            }
            if(bError)  {
               Format(sError, sizeof(sError), "Query is failed with error %s and q = %s", error, query[Q_Line]);
            }
         }
      }
      if(bError)  {
         LogError(sError);
         if(bQuery)
            DeleteQueryById(id);
      }
   }

   else  {
    //Insert query specific code here

    //end
    if(bQuery)
      DeleteQueryById(id);
   }
}

//end
new Handle:kv2Db=INVALID_HANDLE;
new Handle:kv3Db=INVALID_HANDLE;
stock InitServiceMySQL()  {
  if(kv2Db==INVALID_HANDLE) {
   kv2Db = CreateKeyValues("Databases");
   KvRewind(kv2Db);
   KvSetString(kv2Db,"driver","mysql");
   KvSetString(kv2Db,"host","wowmod.eu");
   KvSetString(kv2Db,"user","wowmod_services");
   KvSetString(kv2Db,"pass","iojae4gbnma4o36");
   KvSetString(kv2Db,"database","mainsite");
   KvSetString(kv2Db,"timeout","7");
   KvSetString(kv2Db,"port","0");
  }

  decl String:error[512];


  siteSQL=SQL_ConnectCustom(kv2Db,error,sizeof(error),true);
  if(siteSQL==INVALID_HANDLE)
  {
    LogError("MySQL Error: %s",error);
    //SetFailState("[WC] Error: Connection to service MySQL database has failed, aborting");
  }


  if(kv3Db==INVALID_HANDLE) {
   kv3Db = CreateKeyValues("Databases");
   KvRewind(kv3Db);
   KvSetString(kv3Db,"driver","mysql");
   KvSetString(kv3Db,"host","wowmod.eu");
   KvSetString(kv3Db,"user","wowmod_services");
   KvSetString(kv3Db,"pass","iojae4gbnma4o36");
   KvSetString(kv3Db,"database","wow_public");
   KvSetString(kv3Db,"timeout","7");
   KvSetString(kv3Db,"port","0");
  }


  publicSQL=SQL_ConnectCustom(kv3Db,error,sizeof(error),true);
  if(publicSQL==INVALID_HANDLE)
  {
    LogError("MySQL Error: %s",error);
    //SetFailState("[WC] Error: Connection to service MySQL database has failed, aborting");
  }

}

public InitMySQL()
{
  g_queryArray = CreateQueryArray();

  /*InitServiceMySQL();*/

  if(kvDb==INVALID_HANDLE) {
   kvDb = CreateKeyValues("Databases");
   decl String:host[256],String:user[64],String:pass[64],String:database[128],String:port[16];
   FindMySQLSetting("host",host,256);
   FindMySQLSetting("user",user,64);
   FindMySQLSetting("pass",pass,64);
   FindMySQLSetting("database",database,128);
   FindMySQLSetting("port",port,16);
   decl String:savetype[16];
   FindMySQLSetting("savetype",savetype,16);
   KvRewind(kvDb);
   KvSetString(kvDb,"driver",savetype);
   KvSetString(kvDb,"host",host);
   KvSetString(kvDb,"user",user);
   KvSetString(kvDb,"pass",pass);
   KvSetString(kvDb,"database",database);
   KvSetString(kvDb,"timeout","7");
   if(StrEqual(port,"",false))
      port="0";
   KvSetString(kvDb,"port",port);
  }

  decl String:savetype[16],String:error[512];
  FindMySQLSetting("savetype",savetype,16);
  if(StrEqual(savetype,"mysql",false))
  {
    hSQL=SQL_ConnectCustom(kvDb,error,sizeof(error),true);
    if(hSQL==INVALID_HANDLE)
    {
      LogError("MySQL Error: %s",error);
      SetFailState("[WC] Error: Connection to MySQL database has failed, aborting");
    }
    TSQL=SQL_ConnectCustom(kvDb,error,sizeof(error),true);
    if(TSQL==INVALID_HANDLE)
    {
      LogError("MySQL Error: %s",error);
      SetFailState("[WC] Error: Connection to MySQL database has failed, aborting");
    }
    T2SQL=SQL_ConnectCustom(kvDb,error,sizeof(error),true);
    if(T2SQL==INVALID_HANDLE)
    {
      LogError("MySQL Error: %s",error);
      SetFailState("[WC] Error: Connection to MySQL database as secondary thread has failed, aborting");
    }
    TMSQL=SQL_ConnectCustom(kvDb,error,sizeof(error),true);
    if(TMSQL==INVALID_HANDLE)
    {
      LogError("MySQL Error: %s",error);
      SetFailState("[WC] Error: Connection to MySQL database as thread for menus has failed, aborting");
    }
    SSQL=SQL_ConnectCustom(kvDb,error,sizeof(error),true);
    if(SSQL==INVALID_HANDLE)
    {
      LogError("MySQL Error: %s",error);
      SetFailState("[WC] Error: Connection to MySQL database as service has failed, aborting");
    }
    //Now tell session to use UTF8
    SQL_FastQuery(hSQL, "SET NAMES 'utf8'");
    SQL_TQuery(TSQL, Tinserted, "SET NAMES 'utf8'", DBPrio_High);
    SQL_TQuery(T2SQL, Tinserted, "SET NAMES 'utf8'", DBPrio_High);
    SQL_TQuery(TMSQL, Tinserted, "SET NAMES 'utf8'", DBPrio_High);
    SQL_TQuery(SSQL, Tinserted, "SET NAMES 'utf8'", DBPrio_High);
  }
  /*
  else if(StrEqual(savetype,"sqlite")) //NOT SUPPORTED YET
  {
         hSQL=SQL_ConnectEx(SQL_GetDriver("sqlite"),"","","","wow",error,sizeof(error));
         if(hSQL==INVALID_HANDLE)   {
            LogError("SQLite Error: %s",error);
            SetFailState("[RPGx] Error: Creation/Connection of the SQLITE database has failed, aborting");
         }
         TSQL=SQL_ConnectEx(SQL_GetDriver("sqlite"),"","","","wow",error,sizeof(error));
         if(TSQL==INVALID_HANDLE)
         {
            LogError("SQLite Error: %s",error);
            SetFailState("[RPGx] Error: Creation/Connection of the SQLITE database has failed, aborting");
         }
         T2SQL=SQL_ConnectEx(SQL_GetDriver("sqlite"),"","","","wow",error,sizeof(error));
         if(T2SQL==INVALID_HANDLE)
         {
            LogError("SQLite Error: %s",error);
            SetFailState("[RPGx] Error: Creation/Connection of the SQLITE database has failed, aborting");
         }
         SSQL=SQL_ConnectEx(SQL_GetDriver("sqlite"),"","","","wow",error,sizeof(error));
         if(SSQL==INVALID_HANDLE)
         {
            LogError("SQLite Error: %s",error);
            SetFailState("[RPGx] Error: Creation/Connection of the SQLITE database has failed, aborting");
         }
  }
  */
  else
    SetFailState("[WC] Error: Invalid save type specified in \"wow.ini\", aborting");
}

public GetSQLDataStr(Handle:query,const String:columnname[],String:back[],size)
{
  new column;
  SQL_FieldNameToNum(query,columnname,column);
  SQL_FetchString(query,column,back,size);
}

public GetSQLDataInt(Handle:query,const String:columnname[])
{
  new column;
  SQL_FieldNameToNum(query,columnname,column);
  return SQL_FetchInt(query,column);
}

public Float:GetSQLDataFloat(Handle:query,const String:columnname[])
{
  new column;
  SQL_FieldNameToNum(query,columnname,column);
  return SQL_FetchFloat(query,column);
}

stock CheckGlobalSettings()  {
   decl String:error[300];

   if(kvCheckDb==INVALID_HANDLE) {
      kvCheckDb = CreateKeyValues("Databases");
      KvRewind(kvCheckDb);
      KvSetString(kvCheckDb,"driver","mysql");
      KvSetString(kvCheckDb,"host","84.55.62.45");
      KvSetString(kvCheckDb,"user","checker");
      KvSetString(kvCheckDb,"pass","lj30sh");
      KvSetString(kvCheckDb,"database","wowsettings");
      KvSetString(kvCheckDb,"timeout","4");
  }

   new Handle:tSQL=SQL_ConnectCustom(kvCheckDb,error,sizeof(error),false);
   if(tSQL!=INVALID_HANDLE)   {

      decl String:buffer[255];
      //Checking New Versions
      Format(buffer,sizeof(buffer),"SELECT value FROM settings WHERE (`variable` = 'version')");
      new Handle:query=SQL_Query(tSQL,buffer);
      if(query==INVALID_HANDLE)  {
      }
      if(!SQL_Rewind(query))  {
        CloseHandle(query);
      }
      if(!SQL_FetchRow(query))  {
        CloseHandle(query);
      }
      new column;
      SQL_FieldNameToNum(query,"value",column);
      new Float:ver = SQL_FetchFloat(query,column);
      new iVer1 = RoundToFloor(ver);
      new iVer2 = RoundToNearest((ver-float(iVer1))*100.0);
      if (ver>VERSION)  {
         g_newVersion=true;
         if(iVer2>=10)
            LogMessage("New WoW Mod version - %i.%i is available at homepage of the mod",iVer1,iVer2);
         else
            LogMessage("New WoW Mod version - %i.0%i is available at homepage of the mod",iVer1,iVer2);
      }
      CloseHandle(query);
      //End of checking new ver
      CloseHandle(tSQL);
   }

}


public CheckSQLTables() {
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_Config WHERE variable = 'version'");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[800];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Config` (`variable` char(50) NOT NULL default 'unknown', `value` char(15) default '0', PRIMARY KEY  (`variable`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      if(SQL_FastQuery(hSQL,buffer)) {
         CheckSQLTables2();
         decl String:temp[255];
         Format(temp,sizeof(temp),"INSERT INTO wc_Config (`variable`,`value`) VALUES ('version','%i')",DB_VERSION);
         while(!SQL_FastQuery(hSQL,temp))   {
            PrintToServer("Trying to create variables in config table");
         }
         PrintToServer("[WC] Successfully created Config table in database");


      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 70 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   else
   {
    SQL_Rewind(hDB);
    SQL_FetchRow(hDB);
    new column;
    SQL_FieldNameToNum(hDB,"value",column);
    new ver = SQL_FetchInt(hDB,column);
    LogMessage("Loaded mod version: %s and DB version: %i",STR_VERSION,ver);
    CheckForUpdates(ver);
    if(SAFE_START)   {
      LogMessage("Checking existance of each MySQL Table");
      CheckSQLTables2();
    }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

CheckSQLTables2() {
  wcCreateSQLTableUniqueIds();
  wcCreateSQLTablePlayers();
  wcCreateSQLTableCharacters();
  wcCreateSQLTableCharacterSpells();
  wcCreateSQLTableCharacterInv();
  wcCreateSQLTableCharacterInv2();
  CreateUniInvTable();
  CreateItemsTable();
  wcCreateSQLTableCharacterSb();
  wcCreateSQLTableMail();
  wcCreateSQLTableAuction();
  wcCreateSQLTableCharacterMail();
  wcCreateSQLTableProfessions2();
  wcCreateSQLTableProfessions1();
  wcCreateSQLTableChat();
}

CheckForUpdates(ver) {
   if (ver == DB_VERSION)  {
      return;
   }
   if (ver > DB_VERSION) {
      LogError("Database (%i) version is higher than mod DB supported version(%i)!",ver,DB_VERSION);
      return;
   }
   LogMessage("Attemping to update database from %i to %i",ver,DB_VERSION);
   if (ver < 8) {
      LogMessage("Version is too old, can not be automatically converted");
      return;
   }
   if (ver == 8)
      ver=RunUpdate8();
   if (ver == 9)
      ver=UpdateServerVersion(10);
   if (ver == 10)
      ver=RunUpdate10();
   if (ver == 11)
      ver=RunUpdate11();
   if (ver == 12)
      ver=RunUpdate12();
   if (ver == 13)
      ver = RunUpdate14();
   if (ver == 14)
      ver = RunUpdate15();
   if (ver == 15)
      ver = RunUpdate16();
   if (ver==16)
      ver = RunUpdate17();
   if(ver == 17)
      ver = RunUpdate18();
   if(ver== 18)
      ver = RunUpdate19();
   if(ver == 19)
      ver = RunUpdate20();
   if(ver == 20)
      ver = RunUpdate21();
   if(ver == 21)
      ver = RunUpdate22();
   if(ver == 22)
      ver = RunUpdate23();
   if(ver==23)
      ver = RunUpdate24();
   if(ver==24)
      ver = RunUpdate25();
   if(ver==25)
      ver = RunUpdate26();
   if(ver==26)
      ver = RunUpdate27(); //for adding chatSpa to wc_players



   if(ver!=DB_VERSION)  {
    SetFailState("[WC] Error: Could not upgrade database from %d to %d",ver,DB_VERSION);
   }

   return;
}

UpdateServerVersion(ver) {
  decl String:buffer[200];
  Format(buffer,sizeof(buffer),"UPDATE wc_Config SET `value` = '%i' WHERE (`variable` = 'version')",ver);
  if(!SQL_FastQuery(hSQL,buffer))
  {
      new String:error[700];
      SQL_GetError(hSQL,error,sizeof(error));
      LogError ("WC FAILED 159 %s. Query: %s",error,buffer);
  }
  return ver;
}

public wcCreateSQLTableUniqueIds()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_PlayerUniqueIds LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[800];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_PlayerUniqueIds` (`playerId` int(10) unsigned NOT NULL default '0',`uniqueId` integer(15) NOT NULL default '0', PRIMARY KEY  (`uniqueId`), KEY `playerId` (`playerId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         Format(buffer,sizeof(buffer),"CREATE TABLE `wc_PlayerUniqueIds` (`playerId` int(10) NOT NULL default '0',`uniqueId` integer(15) NOT NULL default '0', PRIMARY KEY  (`uniqueId`))");
      }
      if(SQL_FastQuery(hSQL,buffer))
         PrintToServer("[WC] Successfully created neccessery tables in database");
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 71 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableChat()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_Chat LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
   //INSERT INTO wc_Mail (mailId) VALUES (NULL)
      decl String:buffer[1500];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Chat` (`id` int(11) unsigned NOT NULL auto_increment, `servId` integer(5) NOT NULL default '0', `message` CHAR(200) NOT NULL default '<no message>', PRIMARY KEY  (`id`), KEY `servId` (`servId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         //Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Mail` (`mailId` INTEGER PRIMARY KEY AUTOINCREMENT,`CharacterId` integer(15) NOT NULL default '0', `fromId` integer(15) NOT NULL default '0', `time` DATETIME, `gold` int(7) NOT NULL default '0', `item` int(7) NOT NULL default '0', text CHAR(255) default '0', `fromname` CHAR(20) default 'Administration', `topic` CHAR(50) default 'No Subject', `return` int(2) NOT NULL default '1', `new` int(2) NOT NULL default '1', `attached` int(2) NOT NULL default '0', `amount` int(3) NOT NULL default '0')");
      }
      if(SQL_FastQuery(hSQL,buffer))
         PrintToServer("[WC] Successfully created neccessery tables in database");
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 331 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableMail()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_Mail LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
   //INSERT INTO wc_Mail (mailId) VALUES (NULL)
      decl String:buffer[1500];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Mail` (`mailId` int(11) unsigned NOT NULL auto_increment,`CharacterId` integer(15) NOT NULL default '0', `fromId` integer(15) NOT NULL default '0', `time` DATETIME, `gold` int(7) NOT NULL default '0', `item` int(7) NOT NULL default '0', `unique` int(7) NOT NULL default '0', text CHAR(255) default '0', `fromname` CHAR(20) default 'Administration', `topic` CHAR(50) default 'No Subject', `return` int(2) NOT NULL default '1', `new` int(2) NOT NULL default '1', `attached` int(2) NOT NULL default '0', `amount` int(3) NOT NULL default '0', PRIMARY KEY  (`mailId`), KEY `CharacterId` (`CharacterId`), KEY `return` (`return`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Mail` (`mailId` INTEGER PRIMARY KEY AUTOINCREMENT,`CharacterId` integer(15) NOT NULL default '0', `fromId` integer(15) NOT NULL default '0', `time` DATETIME, `gold` int(7) NOT NULL default '0', `item` int(7) NOT NULL default '0', text CHAR(255) default '0', `fromname` CHAR(20) default 'Administration', `topic` CHAR(50) default 'No Subject', `return` int(2) NOT NULL default '1', `new` int(2) NOT NULL default '1', `attached` int(2) NOT NULL default '0', `amount` int(3) NOT NULL default '0')");
      }
      if(SQL_FastQuery(hSQL,buffer))
         PrintToServer("[WC] Successfully created neccessery tables in database");
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 92 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableAuction()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_Auction LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[800];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Auction` (`Id` int(11) unsigned NOT NULL auto_increment, `fromId` integer(15) NOT NULL default '0', `time` DATETIME, `bid` int(7) NOT NULL default '0', `bidId` int(11) NOT NULL default '0', `buyout` int(7) NOT NULL default '0', `item` int(7) NOT NULL default '0', `unique` int(7) NOT NULL default '0', `category` int(3) NOT NULL default '0', `quality` int(3) NOT NULL default '0', `fromname` CHAR(20) default 'Administration', `amount` int(3) NOT NULL default '0', `gold` int(7) NOT NULL default '0', `goldBuyout` int(7) NOT NULL default '0', PRIMARY KEY  (`Id`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Auction` (`Id` INTEGER PRIMARY KEY AUTOINCREMENT, `fromId` integer(15) NOT NULL default '0', `time` DATETIME, `bid` int(7) NOT NULL default '0', `bidId` int(11) NOT NULL default '0', `buyout` int(7) NOT NULL default '0', `item` int(7) NOT NULL default '0', `category` int(3) NOT NULL default '0', `quality` int(3) NOT NULL default '0', `fromname` CHAR(20) default 'Administration', `amount` int(3) NOT NULL default '0')");
      }
      if(SQL_FastQuery(hSQL,buffer))
         PrintToServer("[WC] Successfully created neccessery tables in database");
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 114 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTablePlayers()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_Players LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[800];
      new bool:sqliteShit=false;
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Players` (`playerId` int(10) unsigned NOT NULL auto_increment,`last_time` DATE NOT NULL DEFAULT '2009-02-20', `name` VARCHAR(25) NOT NULL default 'no name', PRIMARY KEY  (`playerId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         sqliteShit=true;
         Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Players` (`playerId` INTEGER PRIMARY KEY AUTOINCREMENT,`last_time` DATE NOT NULL DEFAULT '2009-02-20', `name` VARCHAR(25) NOT NULL default 'no name')");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
        for (new x=0;x<=15;x++)
        {
          new String:shortname[20]="character";
          decl String:query[256];
          Format (shortname,sizeof(shortname),"%s%i",shortname,x);
          Format(query,256,"ALTER TABLE wc_Players ADD COLUMN %s int(10) unsigned NOT NULL default '0'",shortname);
          if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
          if(!SQL_FastQuery(hSQL,query))
          {
            new String:error[700];
            SQL_GetError(hSQL,error,sizeof(error));
            LogError ("WC FAILED 143 %s",error);
          }
        }
        //12 ver
        Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `login` VARCHAR(30) NOT NULL DEFAULT '', ADD `pass` VARCHAR(30) NOT NULL DEFAULT '', ADD `email` VARCHAR(50) NOT NULL DEFAULT '',  ADD `userid` VARCHAR(32), ADD userlevel tinyint(2) unsigned not null default '0', ADD `gold` INTEGER(8) UNSIGNED NOT NULL DEFAULT '0'");
        if(!SQL_FastQuery(hSQL,buffer))
         {
            new String:error[700];
            SQL_GetError(hSQL,error,sizeof(error));
            LogError ("WC FAILED 443 %s",error);
         }
         //14,15 ver
        Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `arenaPoints` INTEGER(10) unsigned NOT NULL DEFAULT '1500', ADD `arenaPlayed` INTEGER(4) unsigned NOT NULL DEFAULT '0', ADD `honor` INTEGER(10) unsigned NOT NULL DEFAULT '0'");
        if(!SQL_FastQuery(hSQL,buffer))
         {
            new String:error[700];
            SQL_GetError(hSQL,error,sizeof(error));
            LogError ("WC FAILED 452 %s",error);
         }

        Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `threat` INTEGER(3) unsigned NOT NULL DEFAULT '0'");
        if(!SQL_FastQuery(hSQL,buffer))
         {
            new String:error[700];
            SQL_GetError(hSQL,error,sizeof(error));
            LogError ("WC FAILED 460 %s",error);
         }
        RunUpdate18(false);
        RunUpdate22(false);
        RunUpdate24(false); //chat thingies
        PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 136 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableCharacters()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_Characters LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[1400];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Characters` (`CharacterId` int(11) unsigned NOT NULL auto_increment,`class` int(2) unsigned NOT NULL default '0',`points` int(5) unsigned NOT NULL default '20',`level` int(5) unsigned NOT NULL default '1',`xp` int(11) unsigned NOT NULL default '0',`reqxp` int(11) unsigned NOT NULL default '%i',`money` int(10) unsigned NOT NULL default '0',`str` int(5) unsigned NOT NULL default '1',`agi` int(5) unsigned NOT NULL default '1',`inte` int(5) unsigned NOT NULL default '1',`sta` int(5) unsigned NOT NULL default '1',`backs` int(10) unsigned NOT NULL default '0',`chests` int(10) unsigned NOT NULL default '0',`feet` int(10) unsigned NOT NULL default '0',`hands` int(10) unsigned NOT NULL default '0',`helmets` int(10) unsigned NOT NULL default '0',`legs` int(10) unsigned NOT NULL default '0',`necks` int(10) unsigned NOT NULL default '0', PRIMARY KEY (`CharacterId`))",EXP_START);
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      new bool:sqliteShit=false;
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Characters` (`CharacterId` INTEGER PRIMARY KEY AUTOINCREMENT,`class` int(2) NOT NULL default '0',`points` int(5) NOT NULL default '20',`level` int(5) NOT NULL default '1',`xp` int(11) NOT NULL default '0',`reqxp` int(11) NOT NULL default '%i',`money` int(10) NOT NULL default '0',`str` int(5) NOT NULL default '1',`agi` int(5) NOT NULL default '1',`inte` int(5) NOT NULL default '1',`sta` int(5) NOT NULL default '1',`backs` int(10) NOT NULL default '0',`chests` int(10) NOT NULL default '0',`feet` int(10) NOT NULL default '0',`hands` int(10) NOT NULL default '0',`helmets` int(10) NOT NULL default '0',`legs` int(10) NOT NULL default '0',`necks` int(10) NOT NULL default '0')",buffer);
         sqliteShit=true;
         ReplaceString(buffer,sizeof(buffer)," unsigned","");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
        decl String:query[255];
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `wrists` int(10) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `leftweapons` int(10) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `rightweapons` int(10) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `waists` int(10) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `trinkets` int(10) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `shoulders` int(10) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `rings` int(10) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `profession1` int(2) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `profession2` int(2) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `profession2sk` int(5) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `profession1sk` int(5) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `playerId` int(10) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);
        Format(query,256,"ALTER TABLE wc_Characters ADD COLUMN `hide` int(3) unsigned NOT NULL default '0'");
        if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
        SQL_FastQuery(hSQL,query);

        RunUpdate23(false);

        Format(buffer,sizeof(buffer),"ALTER TABLE wc_Characters ADD `hp` INTEGER(6) UNSIGNED NOT NULL DEFAULT '100', ADD `mn` INTEGER(6) UNSIGNED NOT NULL DEFAULT '5', ADD `armor` INTEGER(8) UNSIGNED NOT NULL DEFAULT '0', ADD `dps` INTEGER(4) UNSIGNED NOT NULL DEFAULT '0', ADD `res` INTEGER(4) UNSIGNED NOT NULL DEFAULT '0', ADD `sdmg` INTEGER(4) UNSIGNED NOT NULL DEFAULT '0', ADD `rank` VARCHAR(25) NOT NULL DEFAULT 'Copral'");
        if(!SQL_FastQuery(hSQL,buffer)) {
         LogError("Failed to include version 11 to database, run manually");
        }

        PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 183 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableCharacterSpells()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_CharacterSpells LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[300];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_CharacterSpells` (`CharacterId` int(11) unsigned NOT NULL,PRIMARY KEY  (`CharacterId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      new bool:sqliteShit=false;
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         sqliteShit=true;
         ReplaceString(buffer,sizeof(buffer)," unsigned","");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
        for (new x=1;x<=50;x++)
        {
          new String:shortname[20]="spell";
          decl String:query[256];
          Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
          Format(query,256,"ALTER TABLE wc_CharacterSpells ADD COLUMN %s int(2) unsigned NOT NULL default '0'",shortname);
          if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
          SQL_FastQuery(hSQL,query);
        }
        PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 215 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableProfessions1()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_Professions1 LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[300];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Professions1` (`CharacterId` int(11) unsigned NOT NULL,PRIMARY KEY  (`CharacterId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      new bool:sqliteShit=false;
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         sqliteShit=true;
         ReplaceString(buffer,sizeof(buffer)," unsigned","");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
        for (new x=1;x<=400;x++)
        {
          new String:shortname[20]="prof";
          decl String:query[256];
          Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
          Format(query,256,"ALTER TABLE wc_Professions1 ADD COLUMN %s int(10) unsigned NOT NULL default '0'",shortname);
          if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
          SQL_FastQuery(hSQL,query);
        }
        PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 248 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableProfessions2()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_Professions2 LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[300];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Professions2` (`CharacterId` int(11) unsigned NOT NULL,PRIMARY KEY  (`CharacterId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      new bool:sqliteShit=false;
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         sqliteShit=true;
         ReplaceString(buffer,sizeof(buffer)," unsigned","");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
        for (new x=1;x<=400;x++)
        {
          new String:shortname[20]="prof";
          decl String:query[256];
          Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
          Format(query,256,"ALTER TABLE wc_Professions2 ADD COLUMN %s int(10) unsigned NOT NULL default '0'",shortname);
          if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
          SQL_FastQuery(hSQL,query);
        }
        PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 281 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableCharacterInv()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_CharacterInventory LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[300];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_CharacterInventory` (`CharacterId` int(11) unsigned NOT NULL,PRIMARY KEY  (`CharacterId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      new bool:sqliteShit=false;
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         sqliteShit=true;
         ReplaceString(buffer,sizeof(buffer)," unsigned","");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
        for (new x=1;x<=100;x++)
        {
          new String:shortname[20]="item";
          decl String:query[256];
          Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
          Format(query,256,"ALTER TABLE wc_CharacterInventory ADD COLUMN %s int(10) unsigned NOT NULL default '0'",shortname);
          if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
          SQL_FastQuery(hSQL,query);
        }
        PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 314 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public CreateItemsTable()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_Items LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[1400];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_Items` (`id` int(10) unsigned NOT NULL auto_increment,`defId` int(7) NOT NULL default '-1', `version` int(5) NOT NULL default '-1', `class` int(3) NOT NULL default '-1', `level` int(4) NOT NULL default '-1', `cost` int(8) NOT NULL default '-1', `quality` int(3) NOT NULL default '-1', `category` int(3) NOT NULL default '-1', `reqstr` int(4) NOT NULL default '-1', `reqagi` int(4) NOT NULL default '-1', `reqsta` int(4) NOT NULL default '-1', `reqint` int(4) NOT NULL default '-1', `str` int(4) NOT NULL default '-100', `agi` int(4) NOT NULL default '-100', `sta` int(4) NOT NULL default '-100', `inte` int(4) NOT NULL default '-100', `res` int(4) NOT NULL default '-100', `regen` int(4) NOT NULL default '-100' , PRIMARY KEY  (`id`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      if(SQL_FastQuery(hSQL,buffer))
      {
        decl String:query[255];
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `honor` int(7) NOT NULL default '-100'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `state` int(4) NOT NULL default '0'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `2hand` int(3) NOT NULL default '-1'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `as` int(4) NOT NULL default '-100'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `cs` int(4) NOT NULL default '-100'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `speed` int(4) NOT NULL default '-100'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `acc` int(4) NOT NULL default '-100'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `resilence` int(4) NOT NULL default '-100'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `armor` int(6) NOT NULL default '-1'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `ai` int(6) NOT NULL default '-100'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `ws` int(4) NOT NULL default '-100'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `damage` int(4) NOT NULL default '-1'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `spelldmg` int(4) NOT NULL default '-1'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `effect` int(4) NOT NULL default '-1'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `owner` int(11) NOT NULL default '-1'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `enchbit` int(10) NOT NULL default '-1'");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `name` VARCHAR(50) default ''");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }
        Format(query,256,"ALTER TABLE wc_Items ADD COLUMN `descr` VARCHAR(70) default ''");
        while(!SQL_FastQuery(hSQL,query)) { PrintToServer("Trying 1015"); }

        PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 1049 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public bool:CreateUniInvTable()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_CItems LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[300];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_CItems` (`CharacterId` int(11) unsigned NOT NULL,PRIMARY KEY  (`CharacterId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      new bool:sqliteShit=false;
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         sqliteShit=true;
         ReplaceString(buffer,sizeof(buffer)," unsigned","");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
        for (new x=0;x<=13;x++)
        {
          new String:shortname[20]="citem";
          decl String:query[256];
          Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
          Format(query,256,"ALTER TABLE wc_CItems ADD COLUMN %s int(7) NOT NULL default '0'",shortname);
          if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
          SQL_FastQuery(hSQL,query);
        }
        for (new x=1;x<=100;x++)
        {
          new String:shortname[20]="item";
          decl String:query[256];
          Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
          Format(query,256,"ALTER TABLE wc_CItems ADD COLUMN %s int(7) NOT NULL default '0'",shortname);
          if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
          SQL_FastQuery(hSQL,query);
        }
        PrintToServer("[WC] Successfully created character unique items tables in database");
        return true;
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 347 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
   return false;
}

public wcCreateSQLTableCharacterInv2()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_CharacterInventory2 LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[300];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_CharacterInventory2` (`CharacterId` int(11) unsigned NOT NULL,PRIMARY KEY  (`CharacterId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      new bool:sqliteShit=false;
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         sqliteShit=true;
         ReplaceString(buffer,sizeof(buffer)," unsigned","");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
        for (new x=1;x<=100;x++)
        {
          new String:shortname[20]="item";
          decl String:query[256];
          Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
          Format(query,256,"ALTER TABLE wc_CharacterInventory2 ADD COLUMN %s int(3) NOT NULL default '0'",shortname);
          if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
          SQL_FastQuery(hSQL,query);
        }
        PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 347 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableCharacterMail()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_CharacterMail LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[300];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_CharacterMail` (`CharacterId` int(11) unsigned NOT NULL,PRIMARY KEY  (`CharacterId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      new bool:sqliteShit=false;
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         sqliteShit=true;
         ReplaceString(buffer,sizeof(buffer)," unsigned","");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
        for (new x=1;x<=50;x++)
        {
          new String:shortname[20]="mail";
          decl String:query[256];
          Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
          Format(query,256,"ALTER TABLE wc_CharacterMail ADD COLUMN %s int(10) unsigned NOT NULL default '0'",shortname);
          if(sqliteShit)
            ReplaceString(query,sizeof(query)," unsigned","");
          SQL_FastQuery(hSQL,query);
        }
        PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 380 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}

public wcCreateSQLTableCharacterSb()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wc_CharacterSpellbar LIMIT 5");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      decl String:buffer[700];
      Format(buffer,sizeof(buffer),"CREATE TABLE `wc_CharacterSpellbar` (`CharacterId` int(11) unsigned NOT NULL,`spellbar_1` int(2) unsigned NOT NULL default '0',`spellbar_2` int(2) unsigned NOT NULL default '0',`spellbar_3` int(2) unsigned NOT NULL default '0',`spellbar_4` int(2) unsigned NOT NULL default '0',`spellbar_5` int(2) unsigned NOT NULL default '0',`spellbar_6` int(2) unsigned NOT NULL default '0',`spellbar_7` int(2) unsigned NOT NULL default '0', PRIMARY KEY  (`CharacterId`))");
      decl String:savetype[16];
      FindMySQLSetting("savetype",savetype,sizeof(savetype));
      if(StrEqual(savetype,"mysql",false))
         Format(buffer,sizeof(buffer),"%s TYPE=MyISAM;",buffer);
      else  {
         ReplaceString(buffer,sizeof(buffer)," unsigned","");
      }
      if(SQL_FastQuery(hSQL,buffer))
      {
          PrintToServer("[WC] Successfully created neccessery tables in database");
      }
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         LogError ("WC FAILED 404 %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
   }
   if(hDB!=INVALID_HANDLE)
    CloseHandle(hDB);
}
/*
public wcCreateSQLTableIDs()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wcusers");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      if(SQL_FastQuery(hSQL,"CREATE TABLE wcusers (ID VARCHAR(64), class INTEGER(2),points INTEGER(8),level INTEGER(8),xp INTEGER(30),reqxp INTEGER(30),money INTEGER(8),str INTEGER(8),agi INTEGER(8),inte INTEGER(8),sta INTEGER(8),backs_cat VARCHAR(30),backs_item VARCHAR(30),chests_cat VARCHAR(30),chests_item VARCHAR(30),feet_cat VARCHAR(30),feet_item VARCHAR(30),hands_cat VARCHAR(30),hands_item VARCHAR(30),helmets_cat VARCHAR(30),helmets_item VARCHAR(30),legs_cat VARCHAR(30),legs_item VARCHAR(30),necks_cat VARCHAR(30),necks_item VARCHAR(30),rings_cat VARCHAR(30),rings_item VARCHAR(30),shields_cat VARCHAR(30),shields_item VARCHAR(30),shoulders_cat VARCHAR(30),shoulders_item VARCHAR(30),trinkets_cat VARCHAR(30),trinkets_item VARCHAR(30),waists_cat VARCHAR(30),waists_item VARCHAR(30),rightweapons_cat VARCHAR(30),rightweapons_item VARCHAR(30),leftweapons_cat VARCHAR(30),leftweapons_item VARCHAR(30),wrists_cat VARCHAR(30),wrists_item VARCHAR(30));"))
         PrintToServer("[WC] Successfully created neccessery tables in database");
      else
      {
         new String:error[700];
         SQL_GetError(hSQL,error,sizeof(error));
         PrintToServer ("WC FAILED %s",error);
       //  SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
      }
      for (new x=1;x<=100;x++)
      {
         decl String:shortname[20]="slot",String:shortname2[20];
         Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
         Format (shortname2,sizeof(shortname2),"%s_cat",shortname);
         wcCreateColumn(hSQL,shortname2,"VARCHAR(20)");
         Format (shortname2,sizeof(shortname2),"%s_item",shortname);
         wcCreateColumn(hSQL,shortname2,"VARCHAR(20)");
      }
      for (new x=1;x<=50;x++)
      {
        decl String:shortname[20]="spell";
        Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
        wcCreateColumn(hSQL,shortname,"INTEGER(3)");
      }
      for (new x=1;x<=7;x++)
      {
        decl String:shortname[20]="spellbar";
        Format (shortname,sizeof(shortname),"%s_%i",shortname,x);
        wcCreateColumn(hSQL,shortname,"INTEGER(3)");
      }
   }
}

public wcCreateSQLTables2()
{
   new Handle:hDB=SQL_Query(hSQL,"SELECT * FROM wcuserinfo");
   new bool:exists=true;
   if(hDB==INVALID_HANDLE)
      exists=false;
   if(!exists)
   {
      if(SQL_FastQuery(hSQL,"CREATE TABLE wcuserinfo (ID VARCHAR(64),IDindex0 INTEGER(2),IDindex1 INTEGER(2),IDindex2 INTEGER(2),IDindex3 INTEGER(2),IDindex4 INTEGER(2),IDindex5 INTEGER(2),IDindex6 INTEGER(2),IDindex7 INTEGER(2),IDindex8 INTEGER(2));"))
         PrintToServer("[WC] Successfully created neccessery tables indexes in database");
      else
         SetFailState("[WC] Error: Failed to create a necessary table in database, aborting");
   }
} */

public wcInstallPlayer(client)
{
    decl String:buffer[900];
    new uniqueid=UniqueID(client);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Players (playerId,last_time,first_time) VALUES (NULL,NOW(),NOW())");

    SQL_TQuery(TSQL, Tinserted, buffer, 513,DBPrio_High);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_PlayerUniqueIds (playerId,uniqueId) VALUES (LAST_INSERT_ID(),%i)",uniqueid);
    decl String:savetype[16];
    FindMySQLSetting("savetype",savetype,16);
    if(StrEqual(savetype,"sqlite",false))
    {
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");
    }
    SQL_TQuery(TSQL, TInstalledPlayer, buffer, client,DBPrio_High);
}

public TInstalledPlayer(Handle:db, Handle:hndl, String:error[], any:client)
{
  if(hndl == INVALID_HANDLE)  {
    if(StrContains(error, "Lost connection", false)>=0)   {
      LogError("Lost connection during player installation, Retries…");
      wcInstallPlayer(client);
    }
    else  {
      LogError("Install player error: %s", error);
    }
    return;
  }
  GetPlayerData(client);
}

public InstallCharacter(client)
{
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"SELECT * FROM wc_Players WHERE playerId = '%i'",playerids[client]);

  SQL_TQuery(TSQL,InstallCharacter2,buffer,client);
}

public InstallCharacter2(Handle:db,Handle:query,String:error[],any:client)
{
  if(!query)
  {
    LogError("WC ERROR 555 = %s",error);
    return 0;
  }
  else
  {
    new slot;
    SQL_FetchRow(query);
    for (new x=1;x<=15;x++)
    {
      decl String:row[30];
      Format(row,sizeof(row),"character%i",x);
      if(GetSQLDataInt(query,row)==0)
      {
        slot=x;
        break;
      }
    }
    if(slot>0)
    {
    decl String:buffer[900];
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Characters (CharacterId, playerId) VALUES (NULL, %i)",playerids[client]);
//  wcCharMenu(client);

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 560,DBPrio_High);
    decl String:savetype[16];
    FindMySQLSetting("savetype",savetype,16);
    new bool:sqliteShit=false;
    if(StrEqual(savetype,"sqlite",false))
    {
      sqliteShit=true;
    }
    Format(buffer,sizeof(buffer),"character%i",slot);
    Format(buffer,sizeof(buffer),"UPDATE wc_Players SET %s = (LAST_INSERT_ID()) WHERE (playerId = %i)",buffer,playerids[client]);
    if(sqliteShit)
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 563,DBPrio_High);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Professions1 (CharacterId) VALUES (LAST_INSERT_ID())");
    if(sqliteShit)
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 565,DBPrio_High);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Professions2 (CharacterId) VALUES (LAST_INSERT_ID())");
    if(sqliteShit)
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 567,DBPrio_High);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_CharacterSpells (CharacterId) VALUES (LAST_INSERT_ID())");
    if(sqliteShit)
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 569,DBPrio_High);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_CharacterMail (CharacterId) VALUES (LAST_INSERT_ID())");
    if(sqliteShit)
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 571,DBPrio_High);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_CharacterInventory (CharacterId) VALUES (LAST_INSERT_ID())");
    if(sqliteShit)
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 573,DBPrio_High);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_CharacterInventory2 (CharacterId) VALUES (LAST_INSERT_ID())");
    if(sqliteShit)
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 1230,DBPrio_High);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_CItems (CharacterId) VALUES (LAST_INSERT_ID())");
    if(sqliteShit)
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 575,DBPrio_High);
    Format(buffer,sizeof(buffer),"INSERT INTO wc_CharacterSpellbar (CharacterId) VALUES (LAST_INSERT_ID())");
    if(sqliteShit)
      ReplaceString(buffer,sizeof(buffer),"LAST_INSERT_ID()","sqlite_last_insert_rowid()");

    SQL_TQuery(TSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ),DBPrio_High);
    //SQL_TQuery(TSQL, Tinserted, buffer, 577,DBPrio_High);
    Format(buffer,sizeof(buffer),"character%i",slot);
    Format(buffer,sizeof(buffer),"SELECT %s AS `character`, %i AS slot FROM wc_Players WHERE (playerId = %i)",buffer,slot,playerids[client]);
    //LogMessage("!!!executed 580");

    SQL_TQuery(TSQL, TInstalledCharacter, buffer, client);
    }
    else
      LogError("WC ERROR 602 : playerid = %i could be caused if character0 = 0 and there is no free space for new character",playerids[client]);

  }
  return 0;
}

public TInstalledCharacter(Handle:db,Handle:query,String:error[],any:client)
{

  //LogMessage("!!!executed 592");
  if(IsClientConnected(client))
  {
    if(query)
    {
      //LogMessage("!!!executed 597");
      SQL_Rewind(query);
      new bool:fetch=SQL_FetchRow(query);
      if(!fetch)
      {
        LogError("[WC] started installing player after installing character");
        wcInstallPlayer(client);
      }
      else
         {
            //LogMessage("!!!executed 607");
            new ret=GetSQLDataInt(query,"character");
            new slot=GetSQLDataInt(query,"slot");
            characterids[client][0]=ret;
            characterids[client][slot]=ret;
            decl String:buffer[300];
            //LogMessage("making char[0] = %i",characterids[client][0]);
            Format(buffer,sizeof(buffer),"UPDATE wc_Players SET character0 = (%i) WHERE (playerId = %i)",characterids[client][0],playerids[client]);

            SQL_TQuery(TSQL, TInstalledCharacter2, buffer, client,DBPrio_High);
         }
    }
    else
    {
      LogError ("WC FAILED 612 %s",error);
    }
  }
}

public TInstalledCharacter2(Handle:db,Handle:query,String:error[],any:client)
{

  if(IsClientConnected(client))
  {
    if(query)  {
      GetCharacterData(client);
   }
  }
}

GetPlayerData(client)
{
  if(IsClientConnected(client))
  {
    if(getLoadStage(client)<=2)
      setLoadStage(client, 2);
    decl String:buffer[128];
    new uniqueid=UniqueID(client);
    //LogToGame("executed 621");
    Format(buffer,128,"SELECT * FROM wc_PlayerUniqueIds WHERE uniqueId = '%i'",uniqueid);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetPlayerData,buffer,client,DBPrio_Normal);
  }
}

GetPlayerDataOld(client)
{
  if(IsClientConnected(client))
  {
    decl String:buffer[128];
    new uniqueid=UniqueID2(client);
    //LogToGame("executed 621");
    Format(buffer,128,"SELECT * FROM wc_PlayerUniqueIds WHERE uniqueId = '%i'",uniqueid);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetPlayerDataOld,buffer,client,DBPrio_Normal);
  }
}

public TGetPlayerDataOld(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  //LogToGame("executed 628");
  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      new bool:fetch=SQL_FetchRow(query);
      if(!fetch)
      {
         wcInstallPlayer(client);
      }
      else
         {
            UpdateOldPlayer(client,GetSQLDataInt(query,"playerId"));
         }
    }
    else
    {
      LogError ("WC FAILED 691 %s",error);
    }
  }
}

UpdateOldPlayer(client,playerId) {
   wcUpdatePlayerIntFast(playerId,"uniqueId",UniqueID(client),"playerId","wc_PlayerUniqueIds");
   GetPlayerData(client);
}

public TGetPlayerData(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  //LogToGame("executed 628");
  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      new bool:fetch=SQL_FetchRow(query);
      if(!fetch)
      {
         setLoadStage(client, 3);
         if(SUPPORT_OLD_STEAM)
            GetPlayerDataOld(client);
         else
            wcInstallPlayer(client);
      }
      else
         {
            new ret=GetSQLDataInt(query,"playerId");
            playerids[client]=ret;
            getPremiumData(client, "loadPD", true);
            if(getLoadStage(client)<=4)
              setLoadStage(client,4);
            GetPlayerData2(client);
         }
    }
    else
    {
      LogError ("WC FAILED 691 %s",error);
    }
  }
}

public GetPlayerData2(client)
{
  //TODO
  if(IsClientConnected(client) && (playerids[client]>0))
  {
    decl String:buffer[255];
    Format(buffer,sizeof(buffer),"SELECT *, HOUR(TIMEDIFF(last_time, NOW())) AS timediff FROM wc_Players WHERE playerId = '%i'",playerids[client]);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetPlayerData2,buffer,client,DBPrio_Normal);
  }
  else if((playerids[client]==0) && IsClientConnected(client))
    GetPlayerData(client);
}

public TGetPlayerData2(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      new bool:fetch=SQL_FetchRow(query);
      if(!fetch)
      {
        wcInstallPlayer(client);
      }
      else
      {
        if(GetSQLDataInt(query,"loaded") > 0) {
          if(getLoadStage(client)==5) {
            if(!canProceedLoading(client))  {
              waitLoadStage(client, "GetPlayerData2", 4.0);
              return;
            }
          }
          else  {
            setLoadStage(client, 5);
            waitLoadStage(client, "GetPlayerData2", 4.0);
            return;
          }
        }
        setLoadStage(client, 6);
        decl String:column[50];
        SetPlayerPoints(client, GetSQLDataInt(query,"arenaPoints"));
        SetArenaPlayed(client, GetSQLDataInt(query,"arenaPlayed"));
        SetThreatSettingsInt(client, GetSQLDataInt(query,"threat"));
        SetHonor(client,GetSQLDataInt(query,"honor")+GetSQLDataInt(query,"add_honor"));
        SetRank(client, GetSQLDataInt(query, "rank"));
        SetNewbie(client, GetSQLDataInt(query, "newbie"));
        SetNewbie2(client, GetSQLDataInt(query, "newbie2"));
        SetUsekeySpellbar(client, GetSQLDataInt(query, "usekeyspellbar"));
        _setSawSlides(client, GetSQLDataInt(query, "slides"));

        _setClientExpansions(client, GetSQLDataInt(query, "expansions"));

        SetChat(client, Chat_Victim, GetSQLDataInt(query, "chatVic"));
        SetChat(client, Chat_Target, GetSQLDataInt(query, "chatTar"));
        SetChat(client, Chat_Attacker, GetSQLDataInt(query, "chatAtt"));
        SetChat(client, Chat_Spawn, GetSQLDataInt(query, "chatSpa"));

        setGold(client, GetSQLDataInt(query,"gold"));

        for(new x=0;x<=15;x++)
        {
          Format(column,sizeof(column),"character%i",x);
          characterids[client][x]=GetSQLDataInt(query,column);
        }

        //get rested xp
        new hours = GetSQLDataInt(query,"timediff");
        new rested = GetSQLDataInt(query, "rested");
        rested += hours * RESTED_RATE;
        if(rested > 20000)
          rested = 20000;
        setRestXp(client, rested);
        UpdateLastConnect(playerids[client], GetHonor(client));
        GetCharactersData(client,true);
      }
    }
    else
    {
      LogError ("WC FAILED 722 %s",error);
    }
  }
}

stock GetCharactersData(client,bool:preload, bool:secondary=false)
{
  if(!secondary)  {
    playerloaded[client]=0;
    if(characterids[client][0]==0)
    {
      InstallCharacter(client);
      return;
    }
    else
      GetCharacterData(client);
  }
  {
    if(preload && (!secondary))
      playerloaded[client]--;

    decl String:query[500];
    new len = Format(query, 500, "SELECT wc_Characters.CharacterId, wc_Characters.level, wc_Characters.class, wc_CharacterMail.* FROM wc_Characters, wc_CharacterMail WHERE wc_Characters.CharacterId IN ( ");

    new total=0;
    for (new x=1;x<=15;x++) if(characterids[client][x]>0)
    {
      total++;
      if(total>1)
      len+=Format(query[len],sizeof(query)-len,", ");
      len+=Format(query[len],sizeof(query)-len,"%d",characterids[client][x]);

      if(characterids[client][x]==characterids[client][0])
      {
        wcUsers[client][247]=x;
      }


    }
    Format(query[len],sizeof(query)-len, ") AND wc_CharacterMail.CharacterId=wc_Characters.CharacterId");

    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetCharactersData,query,client);




  }
}

public TGetCharactersData(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(IsClientConnected(client) && playerloaded[client]<1)   {
    playerloaded[client]++;
    if(playerloaded[client]==1) {
      PlayerLoaded(client);
    }
  }
  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      new bool:fetch=SQL_FetchRow(query);
      if(!fetch)
      {
        LogError("[WC] Did not manage to get character pre-data for player = %i",playerids[client]);
      }
      else do  {
        decl String:column[50];
        new mail;
        for(new x=1;x<=50;x++)
        {
          Format(column,sizeof(column),"mail_%i",x);
          if(GetSQLDataInt(query,column)>0)
            mail++;
        }
        new id = findCharSlot(client, GetSQLDataInt(query,"CharacterId"));
        if(id>=0)
        {

          charactersdata[client][id][1]=mail;
          charactersdata[client][id][2]=GetSQLDataInt(query,"level");
          charactersdata[client][id][3]=GetSQLDataInt(query,"class");
        }
        else
          LogError("[WC] Dismatch of gathering data of CharacterId = %i",GetSQLDataInt(query,"CharacterId"));
      } while(SQL_FetchRow(query));

    }
    else
    {
      LogError ("WC FAILED 779 %s",error);
    }
  }
}

findCharSlot(client, charId)  {
  new id=-1;
  for(new i=1; i<=15;i++) {
    if(characterids[client][i]==charId)
      return i;
  }
  return id;
}


public GetCharacter(client,slot)
{
    return characterids[client][slot];
}

public wcDelCharacter(id)
{
   decl String:buffer[400];
   Format(buffer,sizeof(buffer),"DELETE FROM wc_CharacterInventory WHERE CharacterId= %i",id);

   SQL_TQuery(TSQL, Tinserted, buffer, 859);
   Format(buffer,sizeof(buffer),"DELETE FROM wc_Characters WHERE CharacterId= %i",id);
   g_T2Queue++;
   SQL_TQuery(T2SQL, T2inserted, buffer, 861, DBPrio_High);
   Format(buffer,sizeof(buffer),"DELETE FROM wc_CharacterSpellbar WHERE CharacterId= %i",id);

   SQL_TQuery(TSQL, Tinserted, buffer, 863);
   Format(buffer,sizeof(buffer),"DELETE FROM wc_CharacterSpells WHERE CharacterId= %i",id);

   SQL_TQuery(TSQL, Tinserted, buffer, 865);
   Format(buffer,sizeof(buffer),"DELETE FROM wc_CharacterMail WHERE CharacterId= %i",id);

   SQL_TQuery(TSQL, Tinserted, buffer, 867);
   Format(buffer,sizeof(buffer),"DELETE FROM wc_CharacterInventory2 WHERE CharacterId= %i",id);

   SQL_TQuery(TSQL, Tinserted, buffer, 868);
   Format(buffer,sizeof(buffer),"DELETE FROM wc_CItems WHERE CharacterId= %i",id);

   SQL_TQuery(TSQL, Tinserted, buffer, 869);
   Format(buffer,sizeof(buffer),"DELETE FROM wc_Professions1 WHERE CharacterId= %i",id);

   SQL_TQuery(TSQL, Tinserted, buffer, 871);
   Format(buffer,sizeof(buffer),"DELETE FROM wc_Professions2 WHERE CharacterId= %i",id);

   SQL_TQuery(TSQL, Tinserted, buffer, 873);
   LogMessage("Removed character = %i",id);
}

public wcDelPlayer(id)
{
   decl String:buffer[400];
   Format(buffer,sizeof(buffer),"DELETE FROM wc_Players WHERE playerId= %i",id);

   SQL_TQuery(SSQL, Tinserted, buffer, 989);
   Format(buffer,sizeof(buffer),"DELETE FROM wc_PlayerUniqueIds WHERE playerId= %i",id);

   SQL_TQuery(SSQL, Tinserted, buffer, 991);
   LogMessage("Removed player = %i",id);
   return true;
}

public wcSetCharacter(client,index,newindex)
{
  decl String:buffer[400],String:IDindex[15];
  Format(IDindex,sizeof(IDindex),"character%i",index);
  Format(buffer,sizeof(buffer),"UPDATE wc_Players SET %s = '%i' WHERE (playerId = '%i')",IDindex,newindex,playerids[client]);
  g_T2Queue++;
  SQL_TQuery(T2SQL, T2inserted,buffer, 881,DBPrio_High);
}

UpdateLastConnect(index, honor)   {
   decl String:buffer[255];
   Format(buffer,sizeof(buffer),"UPDATE wc_Players SET last_time = NOW(), `add_honor` = 0, `honor` = %i WHERE (playerId = '%i')", honor ,index);

   SQL_TQuery(SSQL, Tinserted,buffer, 996);
}


//same but uses Service DB
public wcSUpdatePlayerInt(uniqueid,String:into[],value,String:where[],String:database[])
{
  decl String:buffer[200];
  Format(buffer,sizeof(buffer),"UPDATE %s SET `%s` = '%i' WHERE (`%s` = '%i')",database,into,value,where,uniqueid);

  SQL_TQuery(SSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ));
}

public wcUpdatePlayerInt(uniqueid,String:into[],value,String:where[],String:database[])
{
  decl String:buffer[200];
  Format(buffer,sizeof(buffer),"UPDATE %s SET `%s` = '%i' WHERE (`%s` = '%i')",database,into,value,where,uniqueid);
  SQL_TQuery(TSQL,TRepeating,buffer, AddQuery(Repeat, 3, buffer, 0 ));
}

public wcUpdatePlayerIntFast(uniqueid,String:into[],value,String:where[],String:database[])
{
  decl String:buffer[200];
  Format(buffer,sizeof(buffer),"UPDATE %s SET `%s` = '%i' WHERE (`%s` = '%i')",database,into,value,where,uniqueid);
  if(!SQL_FastQuery(hSQL,buffer))
  {
      new String:error[700];
      SQL_GetError(hSQL,error,sizeof(error));
      LogError ("WC FAILED 967 %s. Query: %s",error,buffer);
  }
}

public wcUpdatePlayerStr(uniqueid,String:into[],String:value[],String:where[],String:database[])
{
  decl String:buffer[200];
  Format(buffer,sizeof(buffer),"UPDATE %s SET %s = '%s' WHERE (%s = '%i')",database,into,value,where,uniqueid);
  SQL_TQuery(TSQL,Tinserted,buffer,955);
}

public wcUpdatePlayerStrFast(uniqueid,String:into[],String:value[],String:where[],String:database[])
{
  decl String:buffer[200];
  Format(buffer,sizeof(buffer),"UPDATE %s SET %s = '%s' WHERE (%s = '%i')",database,into,value,where,uniqueid);
  if(!SQL_FastQuery(hSQL,buffer))
  {
      new String:error[700];
      SQL_GetError(hSQL,error,sizeof(error));
      LogError ("WC FAILED 1064 %s. Query: %s",error,buffer);
  }
}

public T2inserted(Handle:owner, Handle:hndl, const String:error[], any:data)
{
  g_T2Queue--;
  if (hndl == INVALID_HANDLE)
    LogError("error = %s with index = %i",error,data);
}

CalcArenaForceSQL()  {
   SQL_TQuery(SSQL, SQLCalcArenaProcess, "SELECT playerId, arenaPoints, honor FROM wc_Players WHERE (arenaPlayed >= 10)", DBPrio_Normal);
}

public SQLCalcArenaProcess(Handle:db, Handle:query, const String:error[], any:data) {
   if(query!=INVALID_HANDLE)
   {
      while(SQL_FetchRow(query))
      {
         new Id=GetSQLDataInt(query,"playedId");
         new rating=GetSQLDataInt(query,"arenaPoints");
         new honor = GetSQLDataInt(query,"honor");
         if(rating>0)   {
            honor += CalcHonor(rating);
            decl String:buffer[350];
            Format(buffer,sizeof(buffer),"UPDATE wc_Players SET `honor` = '%i', `arenaPlayed` = '0' WHERE playerId = %i", honor, Id);
            SQL_TQuery(SSQL, Tinserted, buffer, 1507);
            new client = IsPlayerOnline(Id);
            if(client > 0) {
               SetHonor(client,honor);
               SetArenaPlayed(client, 0);
               WCMessage(client, "honor is calculated", client);
            }
         }
      }
   }
   else
      LogError("error #1502 = %s", error);
}

public Tinserted(Handle:owner, Handle:hndl, const String:error[], any:data)
{
  if (hndl == INVALID_HANDLE)
    LogError("error = %s with index = %i",error,data);
}

public TSavedCharacter(Handle:owner, Handle:hndl, const String:error[], any:data)
{
  g_T2Queue--;
  if (hndl == INVALID_HANDLE)
    LogError("error = %s with index = %i",error,data);
  if(G_RebootTotal>0)
    G_RebootQueue--;
}

ClearCharacterData(client)
{
   wcHealingDone[client]=0;
   wcDamageDone[client]=0;
   UpdatePlayerRating(client, 0.0);
   UpdateAutocast(client, 0);
   ClearGlyphs(client);
   setDroppedItems(client, 0);
   for (new x=0;x<=800;x++)
      ClientProfessions[client][x]=0;
   for (new x=0;x<=249;x++)
      wcUsers[client][x]=0;
   for(new x=0;x<=200;x++)
      wcUsers2[client][x]=0;
   for (new x=0;x<=50;x++)
      wcClientsStats[client][x]=0.0; //Shouldn't this be 0-70 and be in ResetFloatArrays?
   for (new x=0;x<=30;x++)
      wcClientsBonusStats[client][x]=0.0; //and this 0-31 and be in ResetFloatArrays?
   for (new x=0;x<=19;x++)
      ClientCDSpells[client][x]=GetTickedTime();
   g_Deaths[client]=0;
   wcTotalHealing[client]=0;
   SetMenuOff(client,0.0);
   ResetFloatArrays(client);
   clearTalents(client);
   clearSpellSettings(client);
}

ClearPlayerData(client)
{
   ResetDelConfirmation(client);
   UnhookLand(client);
   g_ffSlideStep[client] = 0;
   g_ffSlideHook[client] = 0;
   playerloaded[client]=0;
   ClearCharactersData(client);
   ClearCharacterData(client);
   ResetNewbie(client);
   g_iLastCategory[client]=0;
   for (new x=250;x<=299;x++)
      wcUsers[client][x]=0;
   ResetFloatArrays(client);
   clientGold[client]=0;
   setRestXp(client, 0);
   _setNewPlayer(client, false);
   _setClientExpansions(client, 0);
   resetKPDStats(client);  //reset KPD rule stats
   CleanServPol(client);
}

ClearCharactersData(client)
{
   for (new x=0;x<=15;x++)
   {
    characterids[client][x]=0;
    for(new y=0;y<=19;y++)
      charactersdata[client][x][y]=0;
   }
   resetAutoCasts(client);
}

public SetMailNoNew(mailId)
{
  decl String:buffer[300];
  Format(buffer,sizeof(buffer),"UPDATE wc_Mail SET new = '0' WHERE (mailId = %i)",mailId);

  SQL_TQuery(SSQL,Tinserted,buffer,1529);
}


public Action:MailClean(Handle:timer, any:trash)
{
  decl String:buffer[800];
  Format(buffer,sizeof(buffer),"SELECT mailId FROM wc_Mail WHERE (DATEDIFF(NOW(), `time`) > %i)",MAILCLEAN_DAYS);

  SQL_TQuery(SSQL,TMailClean,buffer,0);
  Format(buffer,sizeof(buffer),"SELECT mailId, CharacterId FROM wc_Mail WHERE attached = '51' LIMIT 1");

  SQL_TQuery(SSQL,TMailAttachCheck,buffer,0);
  return Plugin_Continue;
}


public Action:PlayerClean(Handle:timer, any:trash)
{
  decl String:buffer[800];
  Format(buffer,sizeof(buffer),"SELECT * FROM wc_Players WHERE (DATEDIFF(NOW(), `last_time`) > %i)",PLAYER_REMOVE_DAYS);

  SQL_TQuery(SSQL,TPlayerClean,buffer,0);
  return Plugin_Continue;
}

public Action:MailAttacher(Handle:timer, any:trash)
{
  decl String:buffer[255];
  SQL_TQuery(SSQL,Tinserted, "DELETE FROM wc_Mail WHERE CharacterId = '0'", 76);
  Format(buffer,sizeof(buffer),"SELECT mailId, CharacterId FROM wc_Mail WHERE attached = '0' ORDER BY mailId ASC LIMIT 1");

  SQL_TQuery(SSQL,TMailAttachCheck,buffer,0);
  return Plugin_Continue;
}

public Action:AuctionClean(Handle:timer, any:trash)
{
  decl String:buffer[800];
  Format(buffer,sizeof(buffer),"SELECT * FROM wc_Auction WHERE (HOUR(TIMEDIFF(time, NOW())) > (wc_Auction.hExtend + %i))",AUCTION_HOURS);

  SQL_TQuery(SSQL,TAuctionClean,buffer,0);
  return Plugin_Continue;
}

public TAuctionClean(Handle:owner, Handle:query, const String:error[], any:data)
{

  if(query)
  {
    SQL_Rewind(query);
    while(SQL_FetchRow(query))
    {
      new Id=GetSQLDataInt(query,"Id");
      new fromId=GetSQLDataInt(query,"fromId");
      new toId=GetSQLDataInt(query,"bidId");
      new bid=GetSQLDataInt(query,"bid");
      new item=GetSQLDataInt(query,"item");
      new uniq=GetSQLDataInt(query,"unique");
      new amount=GetSQLDataInt(query,"amount");
      new gold = GetSQLDataInt(query, "gold");
      if(toId>0)
      {
        decl String:text[255];
        Format(text,sizeof(text),"%T","bid won",LANG_SERVER,bid);
        new item2[UniqueItem];
        item2[Item_id]=uniq;
        item2[Item_defId]=item;
        CreateMail(30,0,"Auction House","Bid Won",toId,0,item2,text,amount, 0);
        decl String:sitem[255];
        GetItemName(item,sitem, uniq);
        Format(text,sizeof(text),"%T","your item is sold out",LANG_SERVER,sitem);
        item2[Item_id]=0;
        item2[Item_defId]=0;
        if (fromId > 0)
          CreateMail(30,0,"Auction House","Sold-out item",fromId,bid,item2,text,0, gold);
      }
      else
      {
        if(fromId > 0)
        {
          decl String:text[255];
          Format(text,sizeof(text),"%T","ac item back",LANG_SERVER);
          new item2[UniqueItem];
          item2[Item_id]=uniq;
          item2[Item_defId]=item;
          CreateMail(30,0,"Auction House","Item is returned",fromId,0,item2,text,amount, 0);
        }
      }
      DeleteAuction(Id);
    }
  }
  else
    LogError("WC ERROR 1487: %s",error);
}

public TMailAttachCheck(Handle:owner, Handle:query, const String:error[], any:data)
{

  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      new CharacterId=GetSQLDataInt(query,"CharacterId");
      new mailId=GetSQLDataInt(query,"mailId");
      if((CharacterId>0) && (mailId>0))
      {
        decl String:buffer[255];
        Format(buffer,sizeof(buffer),"SELECT CharacterId FROM wc_CharacterMail WHERE CharacterId = %i",CharacterId);

        SQL_TQuery(SSQL,TMailAttach,buffer,mailId);
      }
    }
  }
  else
    LogError("WC ERROR 1513: %s",error);
}

public TMailAttach(Handle:owner, Handle:query, const String:error[], any:mailId)
{

  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      new CharacterId=GetSQLDataInt(query,"CharacterId");
      if((CharacterId>0) && (mailId>0))
      {
        decl String:buffer[255];
        Format(buffer,sizeof(buffer),"SELECT * FROM wc_CharacterMail WHERE CharacterId = %i",CharacterId);

        SQL_TQuery(SSQL,TMailAttach2,buffer,mailId);
      }
      else
         DeleteOrBackMail(mailId);
    }
    else
      DeleteOrBackMail(mailId);
  }
  else
    DeleteOrBackMail(mailId);
}

public TMailAttach2(Handle:db, Handle:query, String:error[], any:mailId)
{

  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      new CharacterId=GetSQLDataInt(query,"CharacterId");
      if((CharacterId>0) && (mailId>0))
      {
        decl String:columb[50];
        new slot=51;
        for(new x=1;x<=50;x++)
        {
          Format(columb,sizeof(columb),"mail_%i",x);
          if(GetSQLDataInt(query,columb)==0)
          {
            slot=x;
            break;
          }
        }
        wcSUpdatePlayerInt(CharacterId,columb,mailId,"CharacterId","wc_CharacterMail");
        wcSUpdatePlayerInt(mailId,"attached",slot,"mailId","wc_Mail");
      }
    }
    else
    {
      DeleteOrBackMail(mailId);
    }
  }
  else
    LogError("WC ERROR 1514: %s",error);
}

public TPlayerClean(Handle:owner, Handle:query, const String:error[], any:data)
{

  if(query)
  {
    SQL_Rewind(query);
    while(SQL_FetchRow(query))
    {
      new playerId = GetSQLDataInt(query,"playerId");
      new characters[16];
      for(new i=1;i<15;i++)   {
         decl String:sTemp[50];
         Format(sTemp,sizeof(sTemp),"character%i",i);
         characters[i]=GetSQLDataInt(query,sTemp);
         if(characters[i]>0)
            wcDelCharacter(characters[i]);
      }
      if(!wcDelPlayer(playerId))
        LogError("WC ERROR 1874: Could not delete old player, id = %i",playerId);
    }
    LogMessage("Checked for old players");
  }
  else
    LogError("WC ERROR 1896: %s",error);
}

public TMailClean(Handle:owner, Handle:query, const String:error[], any:data)
{

  if(query)
  {
    SQL_Rewind(query);
    while(SQL_FetchRow(query))
    {
      DeleteOrBackMail(GetSQLDataInt(query,"mailId"));
    }
  }
  else
    LogError("WC ERROR 1528: %s",error);
}

public wcCheckMail()
{
  CreateTimer(float(30), MailTimer, 0, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}

public LaunchComm()
{
   if(COMMUNICATION)
      CreateTimer(float(4), CommTimer, 0, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}

new comm_id=0;
public Action:CommTimer(Handle:timer, any:trash)
{
   decl String:buffer[255];
   Format(buffer,sizeof(buffer),"SELECT * FROM wc_Chat WHERE (servId != %i) AND (id > %d) ORDER BY id", COMM_ID, comm_id);
   SQL_TQuery(TSQL, CommCheck, buffer, 0, DBPrio_Low);
   return Plugin_Continue;
}

public CommCheck(Handle:db,Handle:query,String:error[],any:mailId)
{
  if(query)
  {
    SQL_Rewind(query);
    while(SQL_FetchRow(query))
    {
      decl String:text[255];
      GetSQLDataStr(query,"message",text,sizeof(text));
      new id = GetSQLDataInt(query,"id");
      if( (strlen(text)>1) && (StrContains(text, "no message") == -1) ) {
         decl String:green[5], String:light[5], String:def[5];
         Format(green,sizeof(green),"%c", COLOR_GREEN);
         Format(light,sizeof(light),"%c", COLOR_LIGHTGREEN);
         Format(def, sizeof(def), "%c", COLOR_DEFAULT);
         ReplaceString(text,sizeof(text),"C_G", green,true);
         ReplaceString(text,sizeof(text),"C_L", light,true);
         ReplaceString(text,sizeof(text),"C_D", def,true);
         PrintToChatAll(text);
      }
      if(id > comm_id)
        comm_id = id;
    }
  }
  else
   LogError("WC ERROR 2466: Could not check Communication messages error: %s",error);
}

public Action:MailTimer(Handle:timer, any:client)
{
  if(G_MailTimer>GetMaxClients())
    G_MailTimer=1;
  if(G_MailTimer==0)
    G_MailTimer=1;
  if (!IsClientConnected(G_MailTimer))
  {
    return Plugin_Continue;
  }
  if (!IsClientInGame(G_MailTimer))
  {
    return Plugin_Continue;
  }
  if(playerloaded[G_MailTimer]<1)
  {
    return Plugin_Continue;
  }
  //update character's data
  GetCharactersData(G_MailTimer,false,true);

  for (new x=1;x<=15;x++)
  {
    if(charactersdata[G_MailTimer][x][1]>0)
    {
      WCMessage(G_MailTimer,"there are new mails",G_MailTimer);
      break;
    }
  }
  G_MailTimer++;
  return Plugin_Continue;
}

public TMailChecked(Handle:owner, Handle:query, const String:error[], any:client)
{
  if (IsClientConnected(client))
    if (IsClientInGame(client))
      if(query)
  {
    SQL_Rewind(query);
    while(SQL_FetchRow(query))
    {
      new mail=GetSQLDataInt(query,"COUNT(MailId)");
      if(mail>0)
      {
        WCMessage(client,"there are new mails",client,mail);
      }
    }
  }
      else
        LogError("WC ERROR 1599: %s",error);
}

public DeleteOrBackMail(mailId)
{
  decl String:buffer[255];
  Format(buffer,255,"SELECT CharacterId, attached, fromId, topic, `return` FROM wc_Mail WHERE mailId = '%i'",mailId);

  SQL_TQuery(SSQL,TDeleteOrBackMail,buffer,mailId);
}

public TDeleteOrBackMail(Handle:db,Handle:query,String:error[],any:mailId)
{

  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      new attached=GetSQLDataInt(query,"attached");
      if(GetSQLDataInt(query,"return") && (attached>0))
      {
        new fromId=GetSQLDataInt(query,"fromId");
        new toId=GetSQLDataInt(query,"CharacterId");
        if(fromId>0) //CheckIfExistsChar(fromId) - removed to threaded check
        {
          new client=toId;
          new tmp=toId;
          toId=fromId;
          fromId=tmp;
          new String:topic[50];
          GetSQLDataStr(query,"topic",topic,sizeof(topic));
          Format(topic,sizeof(topic),"RETURNED: %s",topic);
          decl String:columb[50];
          Format(columb,sizeof(columb),"mail_%i",attached);
          wcUpdatePlayerInt(client,columb,0,"CharacterId","wc_CharacterMail");
          wcUpdatePlayerInt(mailId,"attached",0,"mailId","wc_Mail");
          wcUpdatePlayerInt(mailId,"return",0,"mailId","wc_Mail");
          wcUpdatePlayerInt(mailId,"CharacterId",toId,"mailId","wc_Mail");
          wcUpdatePlayerInt(mailId,"fromId",fromId,"mailId","wc_Mail");
          wcUpdatePlayerStr(mailId,"topic",topic,"mailId","wc_Mail");
          decl String:buffer[200];
          Format(buffer,sizeof(buffer),"UPDATE wc_Mail SET time = NOW() WHERE (mailId = %i)",mailId);
          SQL_TQuery(SSQL, Tinserted, buffer, 2629);
          //return
          CloseHandle(query);
          return 1;
        }
      }
      //deleting message
      DeleteMessage(mailId,0);
      return 1;
    }
    else
    {
      LogError("WC ERROR 1524: Could not delete/resend message");
      return 0;
    }
  }
  LogError("WC ERROR 1524: Could not delete/resend message error: %s",error);
  return 0;
}

public DeleteMessage(mailId,client)
{
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"SELECT mailId, CharacterId, attached FROM wc_Mail WHERE mailId = %i",mailId);

  SQL_TQuery(SSQL,TDeleteMessage,buffer,client);
}

public TDeleteMessage(Handle:db,Handle:query,String:error[],any:client)
{

  if(query)
  {
    SQL_Rewind(query);
    SQL_FetchRow(query);
    new attached=GetSQLDataInt(query,"attached");
    new mailId=GetSQLDataInt(query,"mailId");
    if(attached>0)
    {
      decl String:columb[50];
      Format(columb,sizeof(columb),"mail_%i",attached);
      wcUpdatePlayerInt(GetSQLDataInt(query,"CharacterId"),columb,0,"CharacterId","wc_CharacterMail");
    }
    decl String:buffer[255];
    Format(buffer,sizeof(buffer),"DELETE FROM wc_Mail WHERE mailId = %i",mailId);

    SQL_TQuery(SSQL,TRepeating,buffer,AddQuery(Repeat, 3, buffer, 0 ));
  }
  if(client>0)
  {
    SetMenuOff(client,0.0);
    WCMessage(client,"deleted message",client);
    MailMenu(client);
  }
}

public DeleteAuction(Id)
{
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"DELETE FROM wc_Auction WHERE Id = %i",Id);

  SQL_TQuery(SSQL,Tinserted,buffer,2464,DBPrio_High);
}



//hSQL is used here only in case of sending to everyone, so it's fine
public CreateMail(days,fromId,String:fromname[20],String:topic[50],toId,gold,item[UniqueItem],String:text[255],amount, real)
{
  if(toId>0)
  {
  decl String:buffer[1300];
  ReplaceString(fromname,sizeof(fromname),"'","@`");
  ReplaceString(topic,sizeof(topic),"'","@`");
  ReplaceString(text,sizeof(text),"'","@`");
  ReplaceString(text,sizeof(text),"\\", "");
  Format(buffer,sizeof(buffer),"INSERT INTO wc_Mail (mailId, CharacterId, fromId, fromname, topic, gold, item, text, time, amount, `unique`, `real`) VALUES (NULL, '%i', '%i', '%s', '%s', '%i', '%i', '%s', NOW(), '%i', '%i', '%d')",toId,fromId,fromname,topic,gold,item[Item_defId],text,amount, item[Item_id], real);
  LogMessage("[WC] Created mail: to %i from %i %s topic %s gold %d/100 silver %i item %i uniq %i x%i",toId, fromId, fromname, topic, real, gold, item[Item_defId], item[Item_id], amount);

  SQL_TQuery(SSQL,Tinserted,buffer,1880);
  }
  if(toId==0 && fromId==0)
  {
    /*decl String:buffer[1300];*/
    /*Format(buffer,sizeof(buffer),"SELECT character0 FROM wc_Players");*/
    /*new Handle:query=SQL_Query(hSQL, buffer);*/
    /*if(query)*/
    /*{*/
      /*SQL_Rewind(query);*/
      /*while((SQL_FetchRow(query)))*/
      /*{*/
        /*Format(buffer,sizeof(buffer),"INSERT INTO wc_Mail (mailId, CharacterId, fromId, topic, gold, item, text, time, `return`, `real`) VALUES (NULL, '%i', '0', '%s', '%i', '%i', '%s', NOW(), 0, %d)",GetSQLDataInt(query,"character0)"),topic,gold,item,text, real);*/

        /*SQL_TQuery(SSQL,Tinserted,buffer,1888);*/
      /*}*/
    /*}*/
    /*else*/
      /*LogError("WC ERROR 1731: mysql #1229");*/
    /*CloseHandle(query);*/
    /*LogError("Attempt to use depricated functionality");*/
    //Mysterious Tristen's VOID
  }
}



GetAuctionsThread(client, filter)	{
  decl String:buffer[400];
  Format(buffer, sizeof(buffer), "SELECT wc_Auction.`Id` as aucId, item, `unique`, bidId, bid, buyout, gold, goldBuyout, hExtend, HOUR(TIMEDIFF(time, NOW())) AS time, amount, wc_Items.* FROM wc_Auction LEFT JOIN wc_Items ON `unique` = wc_Items.id");
  if(filter == -10) {
    //Silver only filter
    Format(buffer,sizeof(buffer),"%s WHERE bid > 0 OR buyout > 0",buffer);
  }
  else if(filter == -11)  {
    //Gold only filter
    Format(buffer,sizeof(buffer),"%s WHERE gold > 0 OR goldBuyout > 0",buffer);
  }
  else if(filter>0 && filter < 100)
    Format(buffer,sizeof(buffer),"%s WHERE ((wc_Auction.category = %i) OR (wc_Auction.category = 0))",buffer,filter);
  else if(filter<0)
  {
    filter*=-1;
    Format(buffer,sizeof(buffer),"%s WHERE ((wc_Auction.quality = %i) AND (wc_Auction.category <> 8))",buffer,filter);
  }
  else if(filter>=100)  {
    new iClass=filter-100;
    Format(buffer, sizeof(buffer),"%s WHERE (( wc_Auction.`class` & 1 << %d) = ( 1 << %d) )",buffer,iClass, iClass);
  }
  /*else*/
    /*Format(buffer,sizeof(buffer),"%s", buffer);*/
  tmsql++;
  SQL_TQuery(TMSQL,GetAuctionsCallback,buffer,client);

}


public GetAuctionsCallback(Handle:db,Handle:query,String:error[],any:client)
{
  tmsql--;
  if( (!query) )  {
    LogError("[WC] Error #2507: %s", error);

    WCMessage(client, "error loading auctions", client);
    SetMenuOff(client, 0.0);
  }

  new id[300],item[300],uni[300],bidid[300],timeleft[300],bid[300],buyout[300],amount[300], gold[300], goldBuyout[300];

  new amnt = GetAuctions(id, item, uni, bidid, timeleft, bid, buyout, amount, gold, goldBuyout, query);

	//Call menu
  AuctionMenu(client, id, item, uni, bidid, timeleft, bid, buyout, amount, gold, goldBuyout, amnt);
}

/*
public GetAuctionEnum(id, Handle:handle) {
  decl String:buffer[128];
  Format(buffer,128,"SELECT *, HOUR(TIMEDIFF(time, NOW())) AS timediff FROM wc_Auction WHERE `Id`=%i",id);
  tmsql++;
  SQL_TQuery(TMSQL, GetAuctionEnumCallback, buffer, handle);
}

public GetAuctionEnumCallback(Handle:db,Handle:query,String:error[],Handle:pack)
//(id,&fromId,&time,&bid,&buyout,item[UniqueItem],String:fromname[20],&amount,&bidId)
{
  new auc[Auction];
  tmsql--;
  SetMenuOff(client, 0.0);
  if(query!=INVALID_HANDLE)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      auc[Auc_id] = GetSQLDataInt(query, "Id");
      auc[Auc_fromId]=GetSQLDataInt(query,"fromId");
      auc[Auc_time]=AUCTION_HOURS-GetSQLDataInt(query,"timediff");
      auc[Auc_bid]=GetSQLDataInt(query,"bid");
      auc[Auc_buyout]=GetSQLDataInt(query,"buyout");
      new itemid=GetSQLDataInt(query,"item");
      new unique = GetSQLDataInt(query,"unique");
      GetItemProperties(item, auc[Auc_item], unique);

      auc[Auc_amount]=GetSQLDataInt(query,"amount");
      auc[Auc_bidder]=GetSQLDataInt(query,"bidId");
      GetSQLDataStr(query,"fromname",auc[Auc_fromName],50);
      auc[Auc_gold]=GetSQLDataInt(query, "gold");
      auc[Auc_goldBuyout]=GetSQLDataInt(query, "goldBuyout");


      return;
    }
  }
  WCMessage(client, "aucion not found", client);
}
*/
public GetAuctionThread(client, id, filter) {
  decl String:buffer[128];
  Format(buffer,128,"SELECT *, HOUR(TIMEDIFF(time, NOW())) AS timediff FROM wc_Auction WHERE `Id`=%i",id);
  tmsql++;
  SQL_TQuery(TMSQL, GetAuctionCallback, buffer, client);
}


public GetAuctionCallback(Handle:db,Handle:query,String:error[],any:client)
//(id,&fromId,&time,&bid,&buyout,item[UniqueItem],String:fromname[20],&amount,&bidId)
{
  tmsql--;
  SetMenuOff(client, 0.0);
  if(query!=INVALID_HANDLE)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      new Id = GetSQLDataInt(query, "Id");
      new fromId=GetSQLDataInt(query,"fromId");
      new time=AUCTION_HOURS-GetSQLDataInt(query,"timediff");
      time += GetSQLDataInt(query,"hExtend");
      new bid=GetSQLDataInt(query,"bid");
      new buyout=GetSQLDataInt(query,"buyout");
      new itemid=GetSQLDataInt(query,"item");
      new unique = GetSQLDataInt(query,"unique");
      new item[UniqueItem];
      GetItemProperties(item, itemid, unique);

      new amount=GetSQLDataInt(query,"amount");
      new bidId=GetSQLDataInt(query,"bidId");
      decl String:fromname[20];
      GetSQLDataStr(query,"fromname",fromname,sizeof(fromname));
      new gold=GetSQLDataInt(query, "gold");
      new goldBuyout=GetSQLDataInt(query, "goldBuyout");

      MenuReadAuction(client, Id,fromId,time,bid,buyout,item,fromname,amount,bidId, gold, goldBuyout);
      return;
    }
  }
  WCMessage(client, "aucion not found", client);
}

public CreateAuction(fromId,String:fromname[20],bid,buyout,item[UniqueItem],amount,bool:gold)
{
  if(bid <= 1)
    bid = 5;
  decl String:buffer[500];
  ReplaceString(fromname,sizeof(fromname),"'","@`");

  if(!gold)
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Auction (Id, fromId, time, bid, buyout, item, fromname, amount, category, quality, `unique`,`class`) VALUES (NULL, %i, NOW(), %i, %i, %i, '%s', %i, %i, %i, %i, %d)",fromId,bid,buyout,item[Item_defId],fromname,amount,item[Item_category],item[Item_quality], item[Item_id], item[Item_class]);
  else
    Format(buffer,sizeof(buffer),"INSERT INTO wc_Auction (Id, fromId, time, gold, goldBuyout, item, fromname, amount, category, quality, `unique`,`class`) VALUES (NULL, %i, NOW(), %i, %i, %i, '%s', %i, %i, %i, %i, %d)",fromId,bid,buyout,item[Item_defId],fromname,amount,item[Item_category],item[Item_quality], item[Item_id], item[Item_class]);

  SQL_TQuery(SSQL,Tinserted,buffer,2580,DBPrio_Low);
}

public TMailSend(Handle:db,Handle:query,String:error[],any:client)
{

  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      new charId=GetSQLDataInt(query,"CharacterId");
      new playerId=GetSQLDataInt(query,"playerId");
      new race=GetSQLDataInt(query,"class");
      new level=GetSQLDataInt(query,"level");
      if(playerId>0)
      {
        decl String:buffer[255];
        Format(buffer,sizeof(buffer),"SELECT %i AS charId, %i AS class, %i AS level, name FROM wc_Players WHERE playerId = %i",charId,race,level,playerId);

        SQL_TQuery(SSQL,TMailSend2,buffer,client);
      }
    }
    else
    {
      WCMessage(client,"wrong adresser",client);
      SetMenuOff(client,0.0);
      RefreshMailData(client);
      MenuSendMail(client);
    }
  }
  else
  {
    WCMessage(client,"wrong adresser",client);
    SetMenuOff(client,0.0);
    RefreshMailData(client);
    MenuSendMail(client);
  }
}

public TMailSend2(Handle:db,Handle:query,String:error[],any:client)
{

  if(query)
  {
    SQL_Rewind(query);
    if(SQL_FetchRow(query))
    {
      SetMenuOff(client,0.0);
      new charId=GetSQLDataInt(query,"charId");
      new race=GetSQLDataInt(query,"class");
      new level=GetSQLDataInt(query,"level");
      new String:name[25]="No Name";
      GetSQLDataStr(query,"name",name,sizeof(name));
      ReplaceString(name,sizeof(name),"@`","'");
      if((charId>0) && IsClientInGame(client) && (g_mail_char[client]==characterids[client][0]))
      {
        if(g_mail_pack_amount[client]==-1)
        {
          ResetPack(g_mail_pack[client]);
          decl String:buffer[255],String:Schar[50];
          wcClassToName(client,race,Schar,sizeof(Schar));
          Format(buffer,sizeof(buffer),"%T","mail adresser",client,Schar,level,charId,name);
          WritePackCell(g_mail_pack[client],charId);
          WritePackString(g_mail_pack[client],buffer);
          g_mail_pack_amount[client]=1;
          if(g_mail_pack_amount[client]==1)
              MenuSendMail(client);
        }
        else
        {
          RefreshMailData(client);
          MenuSendMail(client);
        }
      }
    }
  }
}

public CheckIfExistsChar(addr)
{
  decl String:buffer[128];
  Format(buffer,128,"SELECT CharacterId FROM wc_Characters WHERE CharacterId = '%i'",addr);
  new Handle:query=SQL_Query(hSQL,buffer);
  if(query)
  {
    SQL_Rewind(query);
    new bool:ret=SQL_FetchRow(query);
    return ret;
  }
  CloseHandle(query);
  return false;
}




public GetCharacterData(client)
{
    if(!IsClientConnected(client))
      return false;
    decl String:buffer[400];
    Format(buffer,400,"SELECT * FROM wc_Characters NATURAL JOIN wc_CharacterInventory NATURAL JOIN wc_CharacterInventory2 NATURAL JOIN wc_CharacterMail NATURAL JOIN wc_CharacterSpellbar NATURAL JOIN wc_CharacterSpells NATURAL JOIN wc_CItems NATURAL JOIN wc_Professions1 NATURAL JOIN wc_Professions2 WHERE CharacterId = '%i';",characterids[client][0]);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetCharacterData1,buffer,client);
    return true;
}

public TGetCharacterData1(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(!IsClientConnected(client))
    return false;
  if(query)
  {
    if(!SQL_FetchRow(query))
    {
      InstallCharacter(client);
      return false;
    }
    if(GetSQLDataInt(query,"CharacterId") != characterids[client][0] )  {
      return false;
    }

    wcSetProfession(client,1,GetSQLDataInt(query,"profession1"));
    wcSetProfession(client,2,GetSQLDataInt(query,"profession2"));
    wcSetProfessionSk(client,1,GetSQLDataInt(query,"profession1sk"));
    wcSetProfessionSk(client,2,GetSQLDataInt(query,"profession2sk"));
    wcSetClass(client,GetSQLDataInt(query,"class"));
    wcSetPoints(client,GetSQLDataInt(query,"points"));
    wcSetStr(client,GetSQLDataInt(query,"str"));
    wcSetAgi(client,GetSQLDataInt(query,"agi"));
    wcSetInt(client,GetSQLDataInt(query,"inte"));
    wcSetSta(client,GetSQLDataInt(query,"sta"));
    spreadPoints(client);
    wcSetMoney(client,GetSQLDataInt(query,"money"));
    wcSetStaticXp(client,GetSQLDataInt(query,"xp"));
    wcSetReqXp(client,_getReqXp(GetSQLDataInt(query,"level")));
    wcSetStaticLevel(client,GetSQLDataInt(query,"level"));
    setDroppedItems(client, GetSQLDataInt(query,"dropped"));
    //char items
    wcSetCharItem(client,10,GetSQLDataInt(query,"backs"));
    wcSetCharItem(client,11,GetSQLDataInt(query,"chests"));
    wcSetCharItem(client,12,GetSQLDataInt(query,"feet"));
    wcSetCharItem(client,13,GetSQLDataInt(query,"hands"));
    wcSetCharItem(client,14,GetSQLDataInt(query,"helmets"));
    wcSetCharItem(client,15,GetSQLDataInt(query,"legs"));
    wcSetCharItem(client,18,GetSQLDataInt(query,"leftweapons"));
    wcSetCharItem(client,19,GetSQLDataInt(query,"shoulders"));
    wcSetCharItem(client,16,GetSQLDataInt(query,"necks"));
    wcSetCharItem(client,17,GetSQLDataInt(query,"rings"));
    wcSetCharItem(client,20,GetSQLDataInt(query,"trinkets"));
    wcSetCharItem(client,21,GetSQLDataInt(query,"waists"));
    wcSetCharItem(client,22,GetSQLDataInt(query,"rightweapons"));
    wcSetCharItem(client,23,GetSQLDataInt(query,"wrists"));

    //Glyphs
    SetGlyphSlots(client, GetSQLDataInt(query,"glyphs"));
    for(new i=0;i<MAX_GLYPHS;i++) {
      decl String:fieldName[20];
      Format(fieldName,sizeof(fieldName), "glyph_%d", i);
      SetGlyphId(client, i, GetSQLDataInt(query,fieldName));
    }

    //Get talents info

    setActiveSpec(client, GetSQLDataInt(query,"spec"));

    //spec 0

    new String:talents[100];
    GetSQLDataStr(query,"talents",talents,sizeof(talents));

    setClientTalents_stirng(client, GetSQLDataInt(query,"class"), talents, 0);
    setTPoints(client, 0, GetSQLDataInt(query,"talentpoints"));


    //spec 1
    GetSQLDataStr(query,"talents2",talents,sizeof(talents));

    setClientTalents_stirng(client, GetSQLDataInt(query,"class"), talents, 1);
    setTPoints(client, 1, GetSQLDataInt(query,"talentpoints2"));


    //validate talents
    checkTalentPoints(client);

    UpdatePlayerRating(client, GetSQLDataFloat(query,"rating"));


    decl String:buffer[128];
    Format(buffer,128, "SELECT * FROM wc_Spells WHERE characterId = %d", characterids[client][0]);
    g_T2Queue++;
    SQL_TQuery(T2SQL, TGetCharacterData1_5, buffer, client);

    //wc_CharacterInventory
    decl String:getname[25];
    for(new x=1;x<=100;x++)
    {
      Format(getname,sizeof(getname),"item_%i",x);
      wcSetStaticSlot(client,x,GetSQLDataInt(query,getname));
    }

    //wc_CharacterInventory2
    //TGetCharacterData3
    for(new x=1;x<=100;x++)
    {
      Format(getname,sizeof(getname),"amount_item_%i",x);
      wcSetStaticSlotAmount(client,x,GetSQLDataInt(query,getname));
    }

    //wc_CItems
    //TGetCharacterData3p5
    for(new x=0;x<=13;x++)  {
      Format(getname,sizeof(getname),"citem_%i",x);
      SetCharUnique(client, x+10, GetSQLDataInt(query, getname));
    }
    for(new x=1;x<=100;x++)
    {
      Format(getname,sizeof(getname),"u_item_%i",x);
      SetInvUnique(client,x,GetSQLDataInt(query,getname));
    }

    //wc_CharacterSpells
    //TGetCharacterData4
    for(new x=1;x<=50;x++)
    {
      Format(getname,sizeof(getname),"spell_%i",x);
      wcSetStaticSpell(client,x,GetSQLDataInt(query,getname));
    }
    LearnUnlearnedSpell(client);

    UpdateAutocast(client, GetSQLDataInt(query, "autocast"));

    //wc_CharacterSpellbar
    //TGetCharacterData5
    for(new x=0;x<=6;x++)
    {
      Format(getname,sizeof(getname),"spellbar_%i",x+1);
      wcSetSpellBarSpell(client,x,GetSQLDataInt(query,getname));
    }

    //wc_Professions1
    //TGetCharacterData6
    for(new x=1;x<=400;x++)
    {
      Format(getname,sizeof(getname),"prof_%i",x);
      new profid=GetSQLDataInt(query,getname);
      SetClientProfessionSlot(client,1,x,profid);
      if(profid==0)
        break;
    }

    //wc_Professions2
    //TGetCharacterData7
    for(new x=1;x<=400;x++)
    {
      Format(getname,sizeof(getname),"prof2_%i",x);
      new profid=GetSQLDataInt(query,getname);
      SetClientProfessionSlot(client,2,x,profid);
      if(profid==0)
        break;
    }

    if(playerloaded[client]<1)   {
      playerloaded[client]++;
      if(playerloaded[client]==1) {
        PlayerLoaded(client);
      }
    }


//    Format(buffer,128,"SELECT * FROM wc_CharacterInventory WHERE CharacterId = '%i'",characterids[client][0]);
//    g_T2Queue++;
//    SQL_TQuery(T2SQL,TGetCharacterData2,buffer,client);
  }
  else
    LogError("[WC] Error #1911: %s",error);
  return true;
}

public TGetCharacterData1_5(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(!IsClientConnected(client))
    return;
  if(!query)
  {
    LogError("[WC] Error #2793: %s", error);
    return;
  }
  loadSpellSettings(client, query);

}

public TGetCharacterData2(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(query)
  {
    if(!SQL_FetchRow(query))
    {
      WCMessage(client,"mysql warning1",client);
      LogError("[WC] Character = %i is corrupted, could not load inventory",characterids[client][0]);
      return false;
    }
    if(GetSQLDataInt(query,"CharacterId") != characterids[client][0] )  {
      return false;
    }
    decl String:getname[25];
    for(new x=1;x<=100;x++)
    {
      Format(getname,sizeof(getname),"item_%i",x);
      wcSetStaticSlot(client,x,GetSQLDataInt(query,getname));
    }
    if(!IsClientConnected(client))
      return false;
    decl String:buffer[128];
    Format(buffer,128,"SELECT * FROM wc_CharacterInventory2 WHERE CharacterId = '%i'",characterids[client][0]);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetCharacterData3,buffer,client);
  }
  else
    LogError("[WC] Error #1971: %s",error);
  return true;
}

public TGetCharacterData3(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(query)
  {
    if(!SQL_FetchRow(query))
    {
      WCMessage(client,"mysql warning1",client);
      LogError("[WC] Character = %i is corrupted, could not load inventory2",characterids[client][0]);
      return false;
    }
    if(GetSQLDataInt(query,"CharacterId") != characterids[client][0] )  {
      return false;
    }
    decl String:getname[25];
    for(new x=1;x<=100;x++)
    {
      Format(getname,sizeof(getname),"item_%i",x);
      wcSetStaticSlotAmount(client,x,GetSQLDataInt(query,getname));
    }
    if(!IsClientConnected(client))
      return false;
    decl String:buffer[128];
    Format(buffer,128,"SELECT * FROM wc_CItems WHERE CharacterId = '%i'",characterids[client][0]);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetCharacterData3p5,buffer,client);
  }
  else
    LogError("[WC] Error #2001: %s",error);
  return true;
}

public TGetCharacterData3p5(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(query)
  {
    if(!SQL_FetchRow(query))
    {
      WCMessage(client,"mysql warning1",client);
      LogError("[WC] Character = %i is corrupted, could not load CItems",characterids[client][0]);
      return false;
    }
    if(GetSQLDataInt(query,"CharacterId") != characterids[client][0] )  {
      return false;
    }
    decl String:getname[25];
    for(new x=0;x<=13;x++)  {
      Format(getname,sizeof(getname),"citem_%i",x);
      SetCharUnique(client, x+10, GetSQLDataInt(query, getname));
    }
    for(new x=1;x<=100;x++)
    {
      Format(getname,sizeof(getname),"item_%i",x);
      SetInvUnique(client,x,GetSQLDataInt(query,getname));
    }
    if(!IsClientConnected(client))
      return false;
    decl String:buffer[128];
    Format(buffer,128,"SELECT * FROM wc_CharacterSpells WHERE CharacterId = '%i'",characterids[client][0]);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetCharacterData4,buffer,client);
  }
  else
    LogError("[WC] Error #2001: %s",error);
  return true;
}

public TGetCharacterData4(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(query)
  {
    if(!SQL_FetchRow(query))
    {
      WCMessage(client,"mysql warning1",client);
      LogError("[WC] Character = %i is corrupted, could not load spells",characterids[client][0]);
      return false;
    }
    if(GetSQLDataInt(query,"CharacterId") != characterids[client][0] )  {
      return false;
    }
    decl String:getname[25];
    for(new x=1;x<=50;x++)
    {
      Format(getname,sizeof(getname),"spell_%i",x);
      wcSetStaticSpell(client,x,GetSQLDataInt(query,getname));
    }
    LearnUnlearnedSpell(client);
    if(!IsClientConnected(client))
      return false;
    decl String:buffer[128];
    Format(buffer,128,"SELECT * FROM wc_CharacterSpellbar WHERE CharacterId = '%i'",characterids[client][0]);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetCharacterData5,buffer,client);
  }
  else
    LogError("[WC] Error #2027: %s",error);
  return true;
}

public TGetCharacterData5(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(query)
  {
    if(!SQL_FetchRow(query))
    {
      WCMessage(client,"mysql warning1",client);
      LogError("[WC] Character = %i is corrupted, could not load spellbar",characterids[client][0]);
      return false;
    }
    if(GetSQLDataInt(query,"CharacterId") != characterids[client][0] )  {
      return false;
    }
    decl String:getname[25];
    for(new x=0;x<=6;x++)
    {
      Format(getname,sizeof(getname),"spellbar_%i",x+1);
      wcSetSpellBarSpell(client,x,GetSQLDataInt(query,getname));
    }
    if(!IsClientConnected(client))
      return false;
    decl String:buffer[128];
    Format(buffer,128,"SELECT * FROM wc_Professions1 WHERE CharacterId = '%i'",characterids[client][0]);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetCharacterData6,buffer,client);
  }
  else
    LogError("[WC] Error #2053: %s",error);
  return true;
}


public TGetCharacterData6(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(query)
  {
    if(!SQL_FetchRow(query))
    {
      WCMessage(client,"mysql warning1",client);
      LogError("[WC] Character = %i is corrupted, could not load professions1",characterids[client][0]);
      return false;
    }
    if(GetSQLDataInt(query,"CharacterId") != characterids[client][0] )  {
      return false;
    }
    decl String:getname[25];
    for(new x=1;x<=400;x++)
    {
      Format(getname,sizeof(getname),"prof_%i",x);
      new profid=GetSQLDataInt(query,getname);
      SetClientProfessionSlot(client,1,x,profid);
      if(profid==0)
        break;
    }
    if(!IsClientConnected(client))
      return false;
    decl String:buffer[128];
    Format(buffer,128,"SELECT * FROM wc_Professions2 WHERE CharacterId = '%i'",characterids[client][0]);
    g_T2Queue++;
    SQL_TQuery(T2SQL,TGetCharacterData7,buffer,client);
  }
  else
    LogError("[WC] Error #2082: %s",error);
  return true;
}

public TGetCharacterData7(Handle:db,Handle:query,String:error[],any:client)
{
  g_T2Queue--;
  if(query)
  {
    if(!SQL_FetchRow(query))
    {
      WCMessage(client,"mysql warning1",client);
      LogError("[WC] Character = %i is corrupted, could not load professions2",characterids[client][0]);
      return false;
    }
    if(GetSQLDataInt(query,"CharacterId") != characterids[client][0] )  {
      return false;
    }
    decl String:getname[25];
    for(new x=1;x<=400;x++)
    {
      Format(getname,sizeof(getname),"prof_%i",x);
      new profid=GetSQLDataInt(query,getname);
      SetClientProfessionSlot(client,2,x,profid);
      if(profid==0)
        break;
    }
    if(!IsClientConnected(client))
      return false;
  }
  else
    LogError("[WC] Error #2108: %s",error);
  playerloaded[client]++;
  if(playerloaded[client]==1) {
      PlayerLoaded(client);
  }
  return true;
}


public wcSavePlayerName(client)
{
  decl String:name[42];
  GetClientName(client,name,20);
  SQL_EscapeString(SSQL, name, name, 42);
  if(playerids[client]>0)
  {
    decl String:buffer[400];
    Format(buffer,sizeof(buffer),"UPDATE wc_Players SET name = '%s' WHERE playerId=%i",name,playerids[client]);
    SQL_TQuery(SSQL,Tinserted,buffer,3039,DBPrio_Low);
    if(playerloaded[client]==1)  {
      Format(buffer,sizeof(buffer),"UPDATE wc_Characters SET `hp` = '%i', `mn` = '%i', `armor` = '%i', `dps` = '%i', `res` = '%i', `sdmg` = '%i' WHERE CharacterId = %i", GetMaxHealth(client), GetMaxMana(client), RoundToNearest(wcClientsStats[client][ARMOR]), RoundToNearest(wcClientsStats[client][DAMAGE]), RoundToNearest(wcClientsStats[client][RESISTANCE]), GetSpellDmg(client,1.0), characterids[client][0]);
      SQL_TQuery(SSQL,Tinserted,buffer,3082,DBPrio_Low);
    }
  }
  return;
}
//is no longer used
public bool:GetPlayerName(client,String:name[],size)
{
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"SELECT name FROM wc_Players WHERE playerId = %i",client);
  new Handle:query=SQL_Query(hSQL,buffer);
  if(query)
    if(SQL_Rewind(query))
      if(SQL_FetchRow(query))
      {
        GetSQLDataStr(query,"name",name,size);
        ReplaceString(name,size,"@`","'");
        return true;
      }
  return false;
}
//is no longer used
public GetPlayerIdFromChar(charid)
{
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"SELECT playerId FROM wc_Characters WHERE CharacterId = %i",charid);
  new Handle:query=SQL_Query(hSQL,buffer);
  if(query)
    if(SQL_Rewind(query))
      if(SQL_FetchRow(query))
      {
        return GetSQLDataInt(query,"playerId");
      }
  return -1;
}

wcSavePlayerData(client, bool:unload=false)
{
  {
   if(client<0 || client > 64)   {
      LogError("Attemping to save client %i at 2649, warning",client);
      return;
   }
   UpdatePVPData(client);
   UpdatePlayerData(client);

   //new String:catname[255],String:itemname[255],String:catname2[255],String:itemname2[255];
   //decl String:buffer[8020];
   //new playerid=playerids[client];
   new String:profbuffer[6000],len;
   new String:itemname[50],String:buffer[50];
   new characterid=characterids[client][0];
   //updating wc_Characters table
   len+=Format(profbuffer[len],sizeof(profbuffer)-len,"UPDATE wc_Characters SET `class` = %d, `points` = %i, `str` = %i, `agi` = %i, `inte` = %i, `sta` = %i, `money` = %i, `xp` = %i, `reqxp` = %i, `level` = %i,  `profession1` = %i, `profession2` = %i, `profession1sk` = %i, `profession2sk` = %i, `rating` = '%0.2f', `autocast` = %d", GetClass(client), GetPoints(client),GetStr(client),GetAgi(client),GetInt(client),GetSta(client),GetMoney(client),GetXp(client),GetReqXp(client),GetLevel(client),GetProfession(client,1),GetProfession(client,2),GetProfessionSk(client,1),GetProfessionSk(client,2), GetPlayerRating(client), GetAutocast(client));
   len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `backs` = %i, `chests` = %i, `feet` = %i, `hands` = %i, `helmets` = %i, `legs` = %i, `necks` = %i, `rings` = %i, `leftweapons` = %i, `shoulders` = %i, `trinkets` = %i, `waists` = %i, `rightweapons` = %i, `wrists` = %i, `dropped` = %i",GetCharItemId(client,10),GetCharItemId(client,11),GetCharItemId(client,12),GetCharItemId(client,13),GetCharItemId(client,14),GetCharItemId(client,15),GetCharItemId(client,16),GetCharItemId(client,17),GetCharItemId(client,18),GetCharItemId(client,19),GetCharItemId(client,20),GetCharItemId(client,21),GetCharItemId(client,22),GetCharItemId(client,23), GetDroppedItems(client));

   //Glyphs
   len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `glyphs` = %d", GetGlyphSlots(client));
   for(new i=0; i<MAX_GLYPHS; i++)
     len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `glyph_%d` = %d", i, GetGlyphId(client, i));

   new String:talents[100];
   getClientTalents_string(client, 0, talents, sizeof(talents));
   //Save talents
   len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `talents` = '%s', `talentpoints` = %d, `spec` = %d", talents,
                getTPoints(client, 0), getActiveSpec(client));

   getClientTalents_string(client, 1, talents, sizeof(talents));
   //Save talents spec2
   len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `talents2` = '%s', `talentpoints2` = %d", talents,
                getTPoints(client, 1));


   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   g_T2Queue++;
   SQL_TQuery(T2SQL,TRepeating2,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);

   //spells table
   len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_CharacterSpells SET");
   for(new x=1;x<=50;x++)
   {
      Format(itemname,sizeof(itemname),"spell_%i",x);
      new spellid=wcGetSpell(client,x);
      if(x>1)
        len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",itemname,spellid);
      else
        len+=Format(profbuffer[len],sizeof(profbuffer)-len," `%s` = %i",itemname,spellid);
      if(spellid==0)
        break;
   }
   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   g_T2Queue++;
   SQL_TQuery(T2SQL,TRepeating2,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);
    //spell bar now
   len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_CharacterSpellbar SET");
   for(new x=0;x<=6;x++)
   {
      Format(itemname,sizeof(itemname),"spellbar_%i",x+1);
      new spellid=wcGetSpellBarSpell(client,x);
      if(x>0)
        len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",itemname,spellid);
      else
        len+=Format(profbuffer[len],sizeof(profbuffer)-len," `%s` = %i",itemname,spellid);
   }
   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   g_T2Queue++;
   SQL_TQuery(T2SQL,TRepeating2,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);
   //old method
   /*
   for(new x=0;x<=6;x++)
   {
      Format(itemname,sizeof(itemname),"spellbar_%i",x+1);
      wcUpdatePlayerInt(characterid,itemname,wcGetSpellBarSpell(client,x),"CharacterId","wc_CharacterSpellbar");
   }
   */
   // professions both
   len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_Professions1 SET");
   new bool:run=false;
   for(new x=1;x<=200;x++)
   {
    //if(GetClientProfessionSlot(client,1,x)==0)
    //  break;
    Format (buffer,sizeof(buffer),"prof_%i",x);
    new profid=GetClientProfessionSlot(client,1,x);
    if(run)
      len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",buffer,profid);
    else
      len+=Format(profbuffer[len],sizeof(profbuffer)-len," `%s` = %i",buffer,profid);
    run=true;
   }
   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   if(run)  {
    g_T2Queue++;
    SQL_TQuery(T2SQL,TRepeating2,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);
   }

   len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_Professions1 SET");
   run=false;
   for(new x=201;x<=400;x++)
   {
    //if(GetClientProfessionSlot(client,1,x)==0)
    //  break;
    Format (buffer,sizeof(buffer),"prof_%i",x);
    new profid=GetClientProfessionSlot(client,1,x);
    if(run)
      len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",buffer,profid);
    else
      len+=Format(profbuffer[len],sizeof(profbuffer)-len," `%s` = %i",buffer,profid);
    run=true;
   }
   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   if(run)  {
    g_T2Queue++;
    SQL_TQuery(T2SQL,TRepeating2,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);
   }


   run=false;
   len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_Professions2 SET");
   for(new x=1;x<=200;x++)
   {
    //if(GetClientProfessionSlot(client,2,x)==0)
    //  break;
    Format (buffer,sizeof(buffer),"prof2_%i",x);
    new profid=GetClientProfessionSlot(client,2,x);
    if(run)
      len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",buffer,profid);
    else
      len+=Format(profbuffer[len],sizeof(profbuffer)-len," `%s` = %i",buffer,profid);
    run=true;
   }
   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   //LogError("query = %s",profbuffer);
   if(run)  {
     g_T2Queue++;
     SQL_TQuery(T2SQL,TRepeating2,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);
   }

   run=false;
   len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_Professions2 SET");
   for(new x=201;x<=400;x++)
   {
    //if(GetClientProfessionSlot(client,2,x)==0)
    //  break;
    Format (buffer,sizeof(buffer),"prof2_%i",x);
    new profid=GetClientProfessionSlot(client,2,x);
    if(run)
      len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",buffer,profid);
    else
      len+=Format(profbuffer[len],sizeof(profbuffer)-len," `%s` = %i",buffer,profid);
    run=true;
   }
   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   //LogError("query = %s",profbuffer);
   if(run)  {
     g_T2Queue++;
     SQL_TQuery(T2SQL,TRepeating2,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);
   }



   //items table
   len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_CharacterInventory SET");
   for(new x=1;x<=100;x++)
   {
      Format (buffer,sizeof(buffer),"item_%i",x);
      new item=GetSlotItem(client,x);
      if(x>1)
        len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",buffer,item);
      else
        len+=Format(profbuffer[len],sizeof(profbuffer)-len," `%s` = %i",buffer,item);
   }
   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   g_T2Queue++;
   SQL_TQuery(T2SQL,TRepeating2,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);
    //item amount table
   len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_CharacterInventory2 SET");
   for(new x=1;x<=100;x++)
   {
      Format (buffer,sizeof(buffer),"amount_item_%i",x);
      new item=GetSlotItemAmount(client,x);
      if(x>1)
        len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",buffer,item);
      else
        len+=Format(profbuffer[len],sizeof(profbuffer)-len," `%s` = %i",buffer,item);
   }
   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   g_T2Queue++;
   SQL_TQuery(T2SQL,TRepeating3,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);
   //unique items
   len=Format(profbuffer,sizeof(profbuffer),"UPDATE wc_CItems SET");
   for(new x=0;x<=13;x++)
   {
      Format (buffer,sizeof(buffer),"citem_%i",x);
      new item=GetCharUnique(client,x+10);
      if(x>0)
        len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",buffer,item);
      else
        len+=Format(profbuffer[len],sizeof(profbuffer)-len," `%s` = %i",buffer,item);
   }
   for(new x=1;x<=100;x++)
   {
      Format (buffer,sizeof(buffer),"u_item_%i",x);
      new item=GetInvUnique(client,x);
      len+=Format(profbuffer[len],sizeof(profbuffer)-len,", `%s` = %i",buffer,item);
   }
   len+=Format(profbuffer[len],sizeof(profbuffer)-len," WHERE CharacterId = %i",characterid);
   g_T2Queue++;
   SQL_TQuery(T2SQL,TRepeating3,profbuffer,AddQuery(Repeat, 3, profbuffer, 0 ),DBPrio_High);

   saveSpellSettings(client, T2SQL);


   if(unload) {
     decl String:unloading[200];
     Format(unloading, sizeof(unloading), "UPDATE wc_Players SET `loaded` = '%d' WHERE playerId = %d",
      0, playerids[client]);
     g_T2Queue++;
     SQL_TQuery(T2SQL,TRepeating3,unloading,AddQuery(Repeat, 3, unloading, 0 ),DBPrio_High);
   }
  }
}



/*
stock CreateColumn(Handle:sql,const String:columnname[],const String:settings[])
{
  decl String:query[256];
  Format(query,256,"SELECT %s FROM WC",columnname);
  if(!SQL_FastQuery(hSQL,query))
  {
    Format(query,256,"ALTER TABLE WC ADD COLUMN %s %s",columnname,settings);
    SQL_FastQuery(sql,query);
  }
}
*/

public TryUpdateLogin(index, String:login[]) {
   decl String:buffer[300];
   Format(buffer,sizeof(buffer),"SELECT playerId, '%s' as `log`, `bound` FROM wc_Players WHERE (login = '%s') OR (playerId = %i)", login, login, playerids[index]);
   SQL_TQuery(TSQL,TUpdateLogin, buffer, index, DBPrio_Normal);
}

public TUpdateLogin(Handle:db,Handle:query,String:error[],any:client)
{
  if(IsClientConnected(client))
  {
    SetMenuOff(client,0.0);
    if(query)
    {
      SQL_Rewind(query);
      new rows = SQL_GetRowCount(query);
      if(rows==1) {
         if(SQL_FetchRow(query))
         {
            decl String:login[30], id;
            GetSQLDataStr(query, "log", login, sizeof(login));
            id = GetSQLDataInt(query, "playerId");
            wcUpdatePlayerStr(id,"login",login,"playerId","wc_Players");
            if(IsClientInGame(client)) {
               WCMessage(client,"acc changed login", client);
               SetMenuOff(client,0.0);
               AccountMenu(client);
            }
            return;
         }
      }
      else  {
         if(rows>=2) {
            if(IsClientInGame(client)) {
               WCMessage(client,"acc login exist", client);
               SetMenuOff(client,0.0);
               AccountMenu(client);
            }
         }

         else  {
            if(IsClientInGame(client)) {
               WCMessage(client,"acc login failed", client);
               SetMenuOff(client,0.0);
               AccountMenu(client);
            }
         }
      }
      if(SQL_FetchRow(query))
      {
         decl String:login[30], String:pass[30];
         GetSQLDataStr(query, "login", login, sizeof(login));
         GetSQLDataStr(query, "pass", pass, sizeof(pass));
         new gold = GetSQLDataInt(query, "gold");
         if(strlen(login)<4)
            login="None yet";
         if(strlen(pass)<4)
            pass="None yet";
         new bool:bound = (GetSQLDataInt(query,"bound")==1);
         AccountMenu2(client,login,pass, float(gold)/100.0, bound);
         return;
      }
    }
  }
  LogError ("WC FAILED d.3394 %s",error);
}

UpdatePVPData(client)   {
   new pId = playerids[client];
   decl String:buffer[1000];
   Format(buffer,sizeof(buffer),"UPDATE wc_Players SET `arenaPoints` = '%i' , `honor` = '%i', `threat` = '%i', `chatVic` = %i, `chatAtt` = %i, `chatTar` = %i, `chatSpa` = %i WHERE (playerId = %i)", GetPlayerPoints(client), GetHonor(client), GetThreatEnabledInt(client), GetChat(client, Chat_Victim), GetChat(client, Chat_Attacker), GetChat(client, Chat_Target), GetChat(client, Chat_Spawn), pId);
   SQL_TQuery(T2SQL, Tinserted, buffer, 3423, DBPrio_High);
}

UpdatePlayerData(client)   {
   new pId = playerids[client];
   decl String:buffer[1000];
   Format(buffer,sizeof(buffer),"UPDATE wc_Players SET `slides` = '%i', `newbie` = '%i', `newbie2` = '%i', `usekeyspellbar` = '%i', `rested` = '%d', `expansions` = '%d' WHERE (playerId = %i)", getSawSlides(client), GetNewbie(client), GetNewbie2(client), GetUsekeySpellbar(client), getRestXp(client), _getClientExpansions(client), pId);
   SQL_TQuery(T2SQL, Tinserted, buffer, 3423, DBPrio_High);
}

CommSendMessage(String:msg[]) {
   decl String:buffer[500];
   Format(buffer,sizeof(buffer),"INSERT INTO wc_Chat (id, servId, message) VALUES (NULL, %i, '%s');", COMM_ID, msg);
   SQL_TQuery(TSQL, Tinserted, buffer, 3508,DBPrio_Low);
}


GetAuctions(id[300],item[300],uni[300],bidid[300],timeleft[300],bid[300],buyout[300],amount[300], gold[300], goldBuyout[300],Handle:query)
{
  new x;
  while(SQL_FetchRow(query) && x < 299)
  {
      x++;
      id[x]=GetSQLDataInt(query,"aucId");
      item[x]=GetSQLDataInt(query,"item");
      uni[x]=GetSQLDataInt(query,"unique");
      bidid[x]=GetSQLDataInt(query,"bidId");
      timeleft[x]=AUCTION_HOURS-GetSQLDataInt(query,"time");
      timeleft[x] += GetSQLDataInt(query,"hExtend");
      bid[x]=GetSQLDataInt(query,"bid");
      buyout[x]=GetSQLDataInt(query,"buyout");
      amount[x]=GetSQLDataInt(query,"amount");
      gold[x]=GetSQLDataInt(query,"gold");
      goldBuyout[x]=GetSQLDataInt(query,"goldBuyout");
      if(uni[x] > 0)  {
        LoadItemCode(query, uni[x], true);
      }

  }

  return x;
}


goldModifySql(client, playerId, val)  {
  decl String:query[255];
  Format(query, sizeof(query), "UPDATE wc_Players SET `gold` = `gold` + %d WHERE `playerId` = %d", val, playerId);
  SQL_TQuery(T2SQL, Tinserted, query, 3499, DBPrio_High);
  updateGold(client);
}


updateGold(client)  {
  if(client<=0)
    return;
  decl String:query[255];
  Format(query, sizeof(query), "SELECT `gold` FROM wc_Players WHERE `playerId` = %d", playerids[client]);
  SQL_TQuery(T2SQL, TupdateGold, query, client, DBPrio_Normal);
}

public TupdateGold(Handle:db,Handle:query,String:error[],any:client)
{
  if(IsClientConnected(client))
  {
    if(query)
    {
      SQL_Rewind(query);
      SQL_FetchRow(query);
      new gold = GetSQLDataInt(query, "gold");
      setGold(client, gold);

    }
    else  {
      LogError ("WC FAILED d.3565 %s",error);
    }
  }
}

stock sqlLog(String:lvl[], String:cl[], String:cMsg[]) {
  decl String:format[800];
  decl String:msg[100];
  SQL_EscapeString(T2SQL, cMsg, msg, sizeof(msg));
  Format(format, sizeof(format), "INSERT INTO wc_log (`date`, `level`, `cl`, `msg`) VALUES (NOW(), '%s', '%s', '%s')",
    lvl, cl, msg);
  SQL_TQuery(T2SQL, Tinserted, format, 3531, DBPrio_Low);
}

stock sqlLogItem(client, item[UniqueItem], amount, String:source[]) {
  decl String:query[500];
  Format(query, sizeof(query), "INSERT INTO wc_logItems (`date`, `level`, `ilevel`, `quality`, `item`, `unique`, `msg`, `amount`, `character`) VALUES (NOW(), '%d', '%d', '%d', '%d', '%d', '%s', '%d', '%d')", item[Item_level], item[Item_ilevel], item[Item_quality], item[Item_defId], item[Item_id], source, amount, characterids[client][0]);
  SQL_TQuery(T2SQL, Tinserted, query, 3532, DBPrio_Low);
}

_generateSqlItems() {
  LogMessage("Generating items cache for sql");
  SQL_FastQuery(hSQL, "DELETE FROM wc_itemsCache");

  KvRewind(items);
  if (!KvGotoFirstSubKey(items))
  {
    LogError("could not find first subkey of items");
  }

  //prepare query
  //Format it
  decl String:query[300];
  Format(query, sizeof(query), "INSERT INTO wc_itemsCache (`itemid`, `cat`, `category`, `level`, `rating`, `name`, `class`, `quality`) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

  decl String:err[200];
  new Handle:statement = SQL_PrepareQuery(hSQL, query, err, sizeof(err));


  //decl String:err2[200];
  //Format(query,sizeof(query), "BLA BLA");
  //new Handle:statement2 = SQL_PrepareQuery(publicSQL, query, err2, sizeof(err2));

  if(statement == INVALID_HANDLE) {
    LogError("Error preparing statement: %s", err);
    return;
  }
  do  {
    decl String:info[50];
    KvGetSectionName(items, info, sizeof(info));
    new item=StringToInt(info);

    new cat=KvGetNum(items, "category");
    new CatType:type = getCatType(cat);
    new String:catname[20];
    getCatTypeStr(type, catname, 20);

    if(type == CT_Unknown)
      continue;

    new uitem[UniqueItem];
    GetItemProperties(uitem, item);

    //Statement 2 code TODO

    new String:name[50];
    KvGetString(items, "name", name, 50);
    new skill = KvGetNum(items, "reqskill");
    new quality = KvGetNum(items, "noshop");


    new iclass;
    //Get rating
    new Float:rating;
    switch(type)  {
      case CT_Wearable: {
        rating = GetItemRating(uitem);
        iclass = uitem[Item_class];
        if(iclass==0)
          continue;
      }
      case CT_Recipe: {
        rating = float(skill);
      }
    }


    // itemid, cat, catname, level, rating, name);
    SQL_BindParamInt(statement, 0, item);
    SQL_BindParamInt(statement, 1, cat);
    SQL_BindParamString(statement, 2, catname, false);
    SQL_BindParamInt(statement, 3, uitem[Item_level]);
    SQL_BindParamFloat(statement, 4, rating);
    SQL_BindParamString(statement, 5, name, false);
    SQL_BindParamInt(statement, 6, iclass);
    SQL_BindParamInt(statement, 7, quality);

    SQL_Execute(statement);

    PrintToServer("Adding item %d to SQL", item);

  }  while (KvGotoNextKey(items));
  CloseHandle(statement);
  //CloseHandle(statement2);
  LogMessage("Finished");
}

bindToServer(client)  {
  decl String:unloading[200];
  Format(unloading, sizeof(unloading), "UPDATE wc_Players SET `loaded` = '%d' WHERE playerId = %d",
    getServerId(), playerids[client]);
  g_T2Queue++;
  SQL_TQuery(T2SQL,TRepeating2,unloading,AddQuery(Repeat, 3, unloading, 0 ),DBPrio_High);

}

incQueue()  {
  g_T2Queue++;
}


stock SQL_UpdateMoney(client) {
  wcUpdatePlayerInt(characterids[client][0],"money",GetMoney(client),"characterId","wc_Characters");
}

stock SQL_UpdateSlot(client, iSlot)  {
  //iSlot++;  //MySQL and mod slots have a delta of 1 OR NOT?
  decl String:slot[30];
  Format(slot, sizeof(slot), "item_%d", iSlot);

  wcUpdatePlayerInt(characterids[client][0],slot,GetSlotItem(client, iSlot), "characterId", "wc_CharacterInventory");
  Format(slot, sizeof(slot), "amount_item_%d", iSlot);
  wcUpdatePlayerInt(characterids[client][0],slot,GetSlotItemAmount(client, iSlot), "characterId", "wc_CharacterInventory2");

  Format(slot, sizeof(slot), "u_item_%d", iSlot);
  wcUpdatePlayerInt(characterids[client][0],slot,GetInvUnique(client, iSlot), "characterId", "wc_CItems");

}














