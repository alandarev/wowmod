/*
* File: buy_controller.inc
* Description: Makes sure everything is checked and
*              handled when it's about currency
* Author(s): Stinkyfax
*/

#pragma semicolon 1

enum BuyType  {
  None,
  Auction_Bid,
  Auction_Buyout
};

//Structure Id,fromId,time,bid,buyout,item[UniqueItem],String:fromname[],amount,bidId, gold, goldBuyout
enum Auction  {
  Auc_id,
  Auc_fromId, //owner
  Auc_time,
  Auc_bid,
  Auc_buyout,
  Auc_item[UniqueItem],
  String:Auc_fromName[50],
  Auc_amount,
  Auc_bidder,
  Auc_gold,
  Auc_goldBuyout
};

stock setGold(client, amount) {
  clientGold[client] = amount;
}

stock GetGold(client) {
  return clientGold[client];
}


stock bool:SpendGold(client, value, String:service[], String:msg[]) {
  if(value == 0)
    return false;
  if(value < 0) {
    Logger_log(LogType:LT_sql, LogLevel:LL_error, "gold",
      "Tried to spend negative gold, details: playerid %d value %d/100 service %s msg %s",
      playerids[client], value, service, msg);
    return false;
  }
  setGold(client, GetGold(client)-value);

  goldModifySql(client, playerids[client], -1*value);

  Logger_log(LogType:LT_sql, LogLevel:LL_log, service, msg);
  Logger_logGold(playerids[client], service, msg, -1*value);
  return true;
}

stock AddGold(client, value, String:service[], String:msg[]) {
  if(value == 0)
    return;
  if(value < 0) {
    Logger_log(LogType:LT_sql, LogLevel:LL_error, "gold",
      "Tried to get negative gold, details: playerid %d value %d/100 service %s msg %s",
      playerids[client], value, service, msg);
    return;
  }

  goldModifySql(client, playerids[client], value);

  Logger_log(LogType:LT_sql, LogLevel:LL_log, service, msg);
  Logger_logGold(playerids[client], service, msg, value);
}

addGoldToPlayer(playerId, value, String:service[], String:msg[])  {
  if(value < 0.0) {
    Logger_log(LogType:LT_sql, LogLevel:LL_error, "gold",
      "Tried to get negative gold, details: playerid %d value %d/100 service %s msg %s",
      playerId, value, service, msg);
    return;
  }
  goldModifySql(0, playerId, value);

  Logger_log(LogType:LT_sql, LogLevel:LL_log, service, msg);
  Logger_logGold(playerId, service, msg, value);
}

stock AttemptToBuy(BuyType:type, client, id)  {

  switch(type)  {
    case Auction_Bid: AttemptBid(client, id);
    case Auction_Buyout: AttemptBuyout(client, id);
    default: {
      decl String:name[50];
      GetClientName(client, name, sizeof(name));
      LogError("[WC] Error: %s tried to use unsupported buy", name);
    }
  }


}

stock AttemptBid(client, aucId) {

  new object[MLoader];
  decl String:call[100];
  Format(call,sizeof(call), "CALL bid(%d, %d, %d);", aucId, characterids[client][0], GetMoney(client) );
  MLoader_Construct(object, client, "buy_bidCallback1", "", call);
  MLoader_generateProcedureQuery(object);
  MLoader_execute(object);
  /*
        new fromId,time,bid,buyout,item[UniqueItem],String:fromname[20],amount,bidId;
        if(GetAuction(Id,fromId,time,bid,buyout,item,fromname,amount,bidId))
        {
        new newbid=RoundToCeil(float(bid)*(1.0+AUCTION_BID_INC));
        if(newbid>GetMoney(client))
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
            WCMessage(oldclient,"not leading bet",oldclient,item[Item_Name]);
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
            wcUpdatePlayerIntFast(bidId,"money",oldmoney,"CharacterId","wc_Characters");
          }
          wcSetMoney(client,GetMoney(client)-newbid);
          wcUpdatePlayerIntFast(Id,"bidId",characterids[client][0],"Id","wc_Auction");
          wcUpdatePlayerIntFast(Id,"bid",newbid,"Id","wc_Auction");
          MenuReadAuction(client,Id,filter);
        }
        } */
}

public buy_bidCallback1(object[MLoader])  {

  if(object[ML_error])  {
    LogError("Error in bid query: %s", object[ML_errorMsg]);
    return;
  }
  new status = ReadPackCell(object[ML_returns]);
  if(status==0) {
    new silver = ReadPackCell(object[ML_returns]);
    wcSetMoney(object[ML_client], GetMoney(object[ML_client])-silver);
    WCMessage(object[ML_client], "bid placed", object[ML_client]);
  }
  else  {
    WCMessage(object[ML_client], "buying error", object[ML_client]);
  }
  updateGold(object[ML_client]);
}

stock AttemptBuyout(client, aucId)  {
  new object[MLoader];
  decl String:call[100];
  Format(call,sizeof(call), "CALL buyout(%d, %d, %d);", aucId, characterids[client][0], GetMoney(client) );
  MLoader_Construct(object, client, "buy_buyoutCallback1", "", call);
  MLoader_generateProcedureQuery(object);
  MLoader_execute(object);
}

public buy_buyoutCallback1(object[MLoader])  {

  if(object[ML_error])  {
    LogError("Error in buyout query: %s", object[ML_errorMsg]);
    return;
  }
  new status = ReadPackCell(object[ML_returns]);
  if(status==0) {
    new silver = ReadPackCell(object[ML_returns]);
    wcSetMoney(object[ML_client], GetMoney(object[ML_client])-silver);
    WCMessage(object[ML_client], "buyout placed", object[ML_client]);
  }
  else  {
    WCMessage(object[ML_client], "buying error", object[ML_client]);
  }
  updateGold(object[ML_client]);
}


