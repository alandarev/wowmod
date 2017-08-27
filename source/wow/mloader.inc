/*
* File: mloader.inc
* Description: Menu Loader class
* Author(s): Stinkyfax
*/

/*
    Using MLoader
  Example will get information out of mysql about auction
  
  new object[MLoader];
  MLoader_Construct(object, client, "buy_bidCallback1", "bu_bidCallbackError", "query"); //Construct class
  //Now generation query for obtaining info from mysql, function has to be implemented if none suits
  MLoader_generateAuctionQuery(object, aucId);
  //Execute object
  MLoader_execute(object);
  //Then it will callback with data to specified callback function
*/

#pragma semicolon 1

#define MLOADER_RETRY_CELL 407


enum MType {
  T_int,
  T_str,
  T_float
}

//MenuLoader class
enum MLoader {
  ML_Id,
  ML_client,
  String:ML_query[300],
  String:ML_callback[100],
  String:ML_callbackError[100],
  Handle:ML_returns,
  Handle:ML_retClass,
  Handle:ML_sidePack, //Just to carry information through
  bool:ML_error,
  bool:ML_retry,
  String:ML_errorMsg[255]
  
};

new g_ML_id=1;
new Handle:g_ML=INVALID_HANDLE;

CreateAbstractArrays()  {
  g_ML = MLoader_CreateArray();
}

Handle:MLoader_CreateArray()   {
   decl ml[MLoader];
   ml[ML_Id]=1;
   return CreateArray(sizeof(ml));
}

stock MLoader_free(ml[MLoader]) {
  MLoader_DeleteById(ml[ML_Id]);
  if(ml[ML_returns]!=INVALID_HANDLE)
    CloseHandle(ml[ML_returns]);
  if(ml[ML_retClass]!=INVALID_HANDLE)
    CloseHandle(ml[ML_retClass]);
  if(ml[ML_sidePack]!=INVALID_HANDLE)
    CloseHandle(ml[ML_sidePack]);
}

stock MLoader_FindIndex(id) {
   for (new i=0; i<GetArraySize(g_ML); i++)   {
      if(GetArrayCell(g_ML, i, 0) == id)   {
         return i;
      }
   }
   return -1;
}

stock bool:MLoader_DeleteById(id)   {
   for (new i=0; i<GetArraySize(g_ML); i++)   {
      if(GetArrayCell(g_ML, i, 0) == id)   {
         RemoveFromArray(g_ML, i);
         return true;
      }
   }
   return false;
}

bool:MLoader_FindById(ml[MLoader], id) {
   ml[ML_Id] = 0;
   for (new i=0; i<GetArraySize(g_ML); i++)   {
      if(GetArrayCell(g_ML, i, 0) == id)   {
         GetArrayArray(g_ML, i, ml[0]);
         return true;
      }
   }
   return false;
}

stock MLoader_Construct(ml[MLoader], client, String:callback[], String:callbackError[], String:query[]) {
  if(GetArraySize(g_ML) > 500)  {
    decl String:err[500];
    Format(err, 500, "Leaking... Size: %d", GetArraySize(g_ML));
    Logger_log(LogType:LT_file, LogLevel:LL_warning, "MenuLoader", err);
  }
  ml[ML_client] = client;
  ml[ML_returns]=INVALID_HANDLE;
  ml[ML_retClass]=INVALID_HANDLE;
  ml[ML_sidePack]=INVALID_HANDLE;
  strcopy(ml[ML_query],300,query);
  strcopy(ml[ML_callback], 100, callback);
  strcopy(ml[ML_callbackError], 100, callbackError);
  ml[ML_error]=false;
  ml[ML_retry]=true;
  ml[ML_Id] = MLoader_GenerateId();
  PushArrayArray(g_ML, ml[0]);
  return ml[ML_Id];
}

MLoader_GenerateId()  {
   g_ML_id++;
   if(g_ML_id>5000) 
      g_ML_id=1;
   return g_ML_id;
}

MLoader_generateProcedureQuery(ml[MLoader]) {

  //ask for returns
  ml[ML_returns]=CreateDataPack();
  ml[ML_retClass]=CreateDataPack();

  //add returns requests
  WritePackString(ml[ML_returns],"status");
  WritePackCell(ml[ML_retClass],_:T_int);
  
  WritePackString(ml[ML_returns],"silver");
  WritePackCell(ml[ML_retClass],_:T_int);

  
  new index = MLoader_FindIndex(ml[ML_Id]);
  SetArrayArray(g_ML, index, ml[0]);
}

stock MLoader_generateAuctionQuery(ml[MLoader], id) {
  decl String:query[300];
  Format(query,300,"SELECT *, HOUR(TIMEDIFF(time, NOW())) AS timediff FROM wc_Auction WHERE `Id`=%i",id);
  strcopy(ml[ML_query], 300, query);
  //ask for returns
  ml[ML_returns]=CreateDataPack();
  ml[ML_retClass]=CreateDataPack();

  //add returns requests
  WritePackString(ml[ML_returns],"Id");
  WritePackCell(ml[ML_retClass],_:T_int);
  
  WritePackString(ml[ML_returns],"fromId");
  WritePackCell(ml[ML_retClass],_:T_int);

  WritePackString(ml[ML_returns],"timediff");
  WritePackCell(ml[ML_retClass],_:T_int);

  WritePackString(ml[ML_returns],"bid");
  WritePackCell(ml[ML_retClass],_:T_int);

  WritePackString(ml[ML_returns],"buyout");
  WritePackCell(ml[ML_retClass],_:T_int);

  WritePackString(ml[ML_returns],"item");
  WritePackCell(ml[ML_retClass],_:T_int);

  WritePackString(ml[ML_returns],"unique");
  WritePackCell(ml[ML_retClass],_:T_int);
  
  WritePackString(ml[ML_returns],"amount");
  WritePackCell(ml[ML_retClass],_:T_int);
  
  WritePackString(ml[ML_returns],"bidId");
  WritePackCell(ml[ML_retClass],_:T_int);
  
  WritePackString(ml[ML_returns],"gold");
  WritePackCell(ml[ML_retClass],_:T_int);
  
  WritePackString(ml[ML_returns],"goldBuyout");
  WritePackCell(ml[ML_retClass],_:T_int);
  
  WritePackString(ml[ML_returns],"fromname");
  WritePackCell(ml[ML_retClass],_:T_str);
  
  new index = MLoader_FindIndex(ml[ML_Id]);
  SetArrayArray(g_ML, index, ml[0]);
}

MLoader_execute(ml[MLoader])  {
  if(DEBUG) {
    PrintToServer("[DEBUG] executing menu loader class");
    
  }
  if(ml[ML_Id]<=0)  {
    Logger_log(LogType:LT_file, LogLevel:LL_error, "MenuLoader", "Tried to execute INVALID MenuLoader class");
  }
  //consider it is valid then
  tmsql++;
  SQL_TQuery(TMSQL, MLoader_executorCallback, ml[ML_query], ml[ML_Id]);
}

public MLoader_executorCallback(Handle:db,Handle:query,String:error[],any:ml_id)
{
  new bool:cont=true;
  new bool:should_delete=true;
  //first of all get ML class
  new ml[MLoader];
  MLoader_FindById(ml,ml_id);
  if(ml[ML_Id]<=0)  {
    Logger_log(LogType:LT_file, LogLevel:LL_error, "MenuLoader", "MLoader class is lost after query callback");
    cont=false;
  }
  if(query==INVALID_HANDLE) {
    cont=false;
    if( (StrContains(error, "Lost connection", false)>=0) && ml[ML_retry] )   {
      //Update class
      new index = MLoader_FindIndex(ml[ML_Id]);
      SetArrayCell(g_ML, index, false, MLOADER_RETRY_CELL);
      
      //Reexecute class
      Logger_log(LogType:LT_file, LogLevel:LL_warning, "MenuLoader", "query lost connection, retrying…");
      should_delete=false;
      MLoader_execute(ml);
    }
    else  {
      decl String:err[500];
      Format(err, 500, "Got SQL Error: %s", error);
      Logger_log(LogType:LT_file, LogLevel:LL_error, "MenuLoader", err);
      MLoader_free(ml);
    }
         
  }
  if(!SQL_FetchRow(query))  {
    //Start callback
    new Function:func = GetFunctionByName(INVALID_HANDLE, ml[ML_callbackError]);
    Call_StartFunction(INVALID_HANDLE, func);
    Call_PushArray(ml[0], sizeof(ml));
    if(Call_Finish() != SP_ERROR_NONE)  {
      decl String:err[500];
      Format(err,500,"Could not callback the function: %s",ml[ML_callbackError]);
      Logger_log(LogType:LT_file, LogLevel:LL_warning, "MenuLoader", err);
    }
  
  
    MLoader_free(ml);
  
    //Logger_log(LogType:LT_file, LogLevel:LL_error, "MenuLoader", "no fetched rows. query: %s", ml[ML_query]);
    //MLoader_free(ml);
    return;
  }
  //Free array
  if(should_delete)  MLoader_DeleteById(ml_id);
  if(!cont)
    return;
    
  //Write information
  new Handle:results = CreateDataPack();
  new Handle:names = ml[ML_returns];
  new Handle:types = ml[ML_retClass];
  ResetPack(names);
  ResetPack(types);
  while(IsPackReadable(types,1))  {
    decl String:query_var[50];
    ReadPackString(names, query_var, 50);
    new MType:type = MType:ReadPackCell(types);
    
    switch(type)  {
      case T_int: {
        new ret = GetSQLDataInt(query,query_var);
        WritePackCell(results, ret);
      }
      case T_float: {
        new Float:ret = GetSQLDataFloat(query,query_var);
        WritePackFloat(results, ret);
      }
      case T_str: {
        new String:ret[100];
        GetSQLDataStr(query, query_var, ret, 100);
        WritePackString(results, ret);
      }
      
    }
  }
  ResetPack(results);
  CloseHandle(ml[ML_returns]);
  
  ml[ML_returns] = results;
  
  //Start callback
  new Function:func = GetFunctionByName(INVALID_HANDLE, ml[ML_callback]);
  Call_StartFunction(INVALID_HANDLE, func);
  Call_PushArray(ml[0], sizeof(ml));
  if(Call_Finish() != SP_ERROR_NONE)  {
    decl String:err[500];
    Format(err,500,"Could not callback the function: %s",ml[ML_callback]);
    Logger_log(LogType:LT_file, LogLevel:LL_warning, "MenuLoader", err);
  }
  MLoader_free(ml);
}
