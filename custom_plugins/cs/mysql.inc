/*
* File: mysql.inc
* Description: MySQL related functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1

// Define vector handle

new Handle:kv2Db=INVALID_HANDLE;
new Handle:stinkySQL=INVALID_HANDLE;
stock _sqlInit()  {
  if(kv2Db==INVALID_HANDLE) {
   kv2Db = CreateKeyValues("Databases");
   KvRewind(kv2Db);
   KvSetString(kv2Db,"driver","mysql");
   KvSetString(kv2Db,"host","wowmod.eu");
   KvSetString(kv2Db,"user","cserverspub");
   KvSetString(kv2Db,"pass","Mewi5JSDFH93E0ZQ");
   KvSetString(kv2Db,"database","stinkypanel");
   KvSetString(kv2Db,"timeout","7");
   KvSetString(kv2Db,"port","0");
  }

  decl String:error[512];

  stinkySQL=SQL_ConnectCustom(kv2Db,error,sizeof(error),true);
  if(stinkySQL==INVALID_HANDLE)
  {
    LogError("MySQL Error: %s",error);
    SetFailState("[c-servers] Error: Connection to stinky panel MySQL database has failed, aborting");
  }

}

stock RequireRestart(id, String:key[])  {
  decl String:query[255];
  Format(query, sizeof(query), "INSERT INTO Requests (`servId`, `servKey`, `request`) VALUES  ('%d', '%s', 'restart');", id, key);
  SQL_TQuery(stinkySQL, Tinserted, query, 90);

}

public Tinserted(Handle:owner, Handle:hndl, const String:error[], any:data)
{
  if (hndl == INVALID_HANDLE)
    LogError("error = %s with index = %i",error,data);
}

stock sqlSendChat(String:name[], String:val2[]) {
  decl String:val[200],String:eName[50];
  decl String:query[400];
  SQL_EscapeString(stinkySQL, val2, val, sizeof(val));
  SQL_EscapeString(stinkySQL, name, eName, sizeof(eName));
  Format(query, sizeof(query), "INSERT INTO Chat (`servId`, `servKey`, `player`, `message`) VALUES (%d, '%s', '%s', '%s')",
    g_cId, g_cKey, eName, val);
  SQL_TQuery(stinkySQL, Tinserted, query, 59);
}

new g_chatLastId=0;
stock sqlGetChats() {
  decl String:query[500];
  Format(query, sizeof(query), "SELECT Chat.`id` AS `id`, `shortName`, `player`, `message` FROM Chat LEFT JOIN Server on Chat.servId = Server.id WHERE Chat.validated = 1 AND Chat.id > %d AND Chat.servId != %d ",
    g_chatLastId, g_cId);
  //Group support code
  for(new i=0; i< g_cGroupsAmount; i++) {
    if(i==0)  Format(query, sizeof(query), "%s AND (", query);
    if(i> 0) Format(query,sizeof(query), "%s OR ", query);
    Format(query, sizeof(query), "%s LOCATE('%s', Server.group) > 0 ", query, g_cGroups[i]);
    if(i==g_cGroupsAmount-1)  Format(query, sizeof(query), "%s )", query);
  }
  SQL_TQuery(stinkySQL, TGetChats, query);
}

public TGetChats(Handle:db,Handle:query,String:error[],any:foo)
{
  if(!query)
  {
    LogError("[c-servers] Error 74: Could not get Chat messages: %s", error);
    return;
  }
  SQL_Rewind(query);
  while(SQL_FetchRow(query))
  {
    decl String:text[255];
    decl String:servName[30],String:nick[30];
    GetSQLDataStr(query,"message",text,sizeof(text));
    GetSQLDataStr(query,"player",nick,sizeof(nick));

    GetSQLDataStr(query,"shortName",servName,sizeof(servName));

    new id = GetSQLDataInt(query,"id");
    if( (strlen(text)>1) && (StrContains(text, "no message") == -1) ) {
      Format(text, sizeof(text), "%c%s %c(%s)%s: %s",COLOR_LIGHTGREEN, nick, COLOR_GREEN, servName, COLOR_DEFAULT, text);
      PrintToChatAll(text);
    }
    if(id > g_chatLastId)
      g_chatLastId = id;
  }
}

stock sqlLoadConfig() {
  decl String:query[255];
  Format(query, sizeof(query), "SELECT `group` FROM Server WHERE `id` = %d", g_cId);
  SQL_TQuery(stinkySQL, TGetConfig, query);
}

public TGetConfig(Handle:db,Handle:query,String:error[],any:foo)
{
  if(!query)
  {
    LogError("[c-servers] Error 74: Could not get Chat messages: %s", error);
    return;
  }
  SQL_Rewind(query);
  if(!SQL_FetchRow(query))  {
    LogError("[c-servers] Error 76: Could not load config from SQL: %s", error);
    return;
  }
  decl String:text[255];
  GetSQLDataStr(query,"group",text,sizeof(text));
  g_cGroupsAmount = ExplodeString(text, ",", g_cGroups, 10, 30);
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


