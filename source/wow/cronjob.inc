/*
* File: cronjob.inc
* Description: Is mainly used to handle planned and rotated events
* Author(s): Stinkyfax
*/

#pragma semicolon 1

//insert into wc_events (msg, rotated, commands) values ('Increased XP by 50%', '12:00', 'wc_temp_xp 1.5');
//insert into wc_events (planned, msg) values ('2010-10-20 17:00:00', 'dump event');
/*
finding events which need to be run:
select * from wc_events WHERE 
	( 
		( (TIMEDIFF(`rotated`, curtime() )) < '00:01:00' ) AND 
		( (TIMEDIFF(`rotated`, curtime() )) > '00:00:00' ) 
	) OR
	
	(
		( (TIMEDIFF(`planned`, NOW() )) > '00:00:00' ) AND
		( (TIMEDIFF(`planned`, NOW() )) < '00:01:00' )
	
	)
	AND (TIMEDIFF(TIMESTAMPADD(MINUTE, -1, NOW()), `lasttime`) > '00:00:00')

finding next rotated event:
SELECT 
		IF ( TIMEDIFF(`rotated`, curtime() ) > '00:00:00', 
			TIMEDIFF(`rotated`, curtime() ), 
			ADDTIME(TIMEDIFF(`rotated`, curtime() ) , '24:00:00' )
		) AS 'condition'
			
		, wc_events.* FROM wc_events WHERE `rotated` IS NOT NULL AND CHAR_LENGTH(`msg`) > 0
		ORDER BY `condition` LIMIT 1
		
finding next planned event:
SELECT 
		 TIMEDIFF(`planned`, NOW()) AS 'condition'
			
		, wc_events.* FROM wc_events WHERE `planned` IS NOT NULL AND (TIMEDIFF(`planned`, NOW()) > '00:00:00')
		ORDER BY `condition` limit 1

//INSERT INTO wc_events (`planned`, `msg`) VALUES ('2010-10-23 17:00:00', 'First planned event by stinkyfax!!');

*/
	

public Action:CronJob(Handle:timer, any:trash)
{
  decl String:buffer[800];
  Format(buffer,sizeof(buffer),"select * from wc_events WHERE (	( (TIMEDIFF(`rotated`, curtime() )) < '00:01:00' ) AND 	( (TIMEDIFF(`rotated`, curtime() )) > '00:00:00' ) 	) OR	(	( (TIMEDIFF(`planned`, NOW() )) > '00:00:00' ) AND	( (TIMEDIFF(`planned`, NOW() )) < '00:01:00' ) ) AND (TIMEDIFF(TIMESTAMPADD(MINUTE, -1, NOW()), `lasttime`) > '00:00:00') AND servId = %d", COMM_ID);

  SQL_TQuery(SSQL,TCronJob,buffer,0);
  return Plugin_Continue;
}

public TCronJob(Handle:owner, Handle:query, const String:error[], any:data)
{
  if(query)
  {
    SQL_Rewind(query);
    while(SQL_FetchRow(query))
    {
      //double check for server id
      new servId = GetSQLDataInt(query, "servId");
      if(servId != COMM_ID)
        continue;
      
      decl String:buffer[255];
      //tell sql that task is runned
      Format(buffer, sizeof(buffer),"UPDATE wc_events SET `lasttime` = NOW() WHERE id = %d", GetSQLDataInt(query, "id") );
      SQL_TQuery(SSQL, Tinserted, buffer, 65);
      
      //Run it
      decl String:msg[255];
      decl String:commands[1000];
      GetSQLDataStr(query, "msg", msg, sizeof(msg));
      GetSQLDataStr(query, "commands", commands, sizeof(commands));
      EventMessage(msg);
      //run commands if any
      decl String:buffers[20][200];
      new amount = ExplodeString(commands, "||", buffers, 20, 200);
      for(new i=0; i < amount; i++) {
        ServerCommand(buffers[i]);
        
      }
      if(amount>0)
        LogMessage("[WC] Running event: %s with %d commands, first command: %s", msg, amount, buffers[0]);
      else
        LogMessage("[WC] Running event: %s without commands", msg);      

    }
  }
  else  {
    LogError("SQL Error #60: %s", error);
  }
}

CronNextEvents()  {
  decl String:buffer[800];
  Format(buffer,sizeof(buffer),"SELECT  TIMEDIFF(`planned`, NOW()) AS 'condition'	, wc_events.* FROM wc_events WHERE `planned` IS NOT NULL AND TIMEDIFF(`planned`, NOW()) > '00:00:00' AND (`servId` = %d) ORDER BY `condition` limit 1", getServerId());

  SQL_TQuery(SSQL,TCronNext,buffer,0);
  
  Format(buffer,sizeof(buffer),"SELECT 	IF ( TIMEDIFF(`rotated`, curtime() ) > '00:00:00', 		TIMEDIFF(`rotated`, curtime() ), 		ADDTIME(TIMEDIFF(`rotated`, curtime() ) , '24:00:00' )	) AS 'condition'	, wc_events.* FROM wc_events WHERE `rotated` IS NOT NULL AND CHAR_LENGTH(`msg`) > 0 AND (`servId` = %d)	ORDER BY `condition` LIMIT 1", getServerId());

  SQL_TQuery(SSQL,TCronNext,buffer,1);
  
}

public TCronNext(Handle:owner, Handle:query, const String:error[], any:data)
{
  decl String:type[22];
  if(query)
  {
    SQL_Rewind(query);
    while(SQL_FetchRow(query))
    {
      
      //Run it
      decl String:msg[255];
      decl String:message[255];
      GetSQLDataStr(query, "msg", msg, sizeof(msg));
      decl String:time[50];
      if(data==1)
        GetSQLDataStr(query, "rotated", time, 50);
      else
        GetSQLDataStr(query, "planned", time, 50);
      decl String:timeleft[50];
      GetSQLDataStr(query, "condition", timeleft, 50);
      
      new servId = GetSQLDataInt(query, "servId");
      decl String:postfix[35];
      Format(postfix, 35, "");
      if(servId != COMM_ID)
        Format(postfix, 35, "(On Another server: ID %d )", servId);
      
      for(new i=1; i<= GetMaxClients(); i++)  {
        if(IsClientInGame(i))  {
          if(data==0)
            Format(type, 22, "%T", "Planned", i);
          else
            Format(type, 22, "%T", "Regular", i);
          Format(message, sizeof(message), "%T", "Next event is", i, type, msg, time, timeleft, postfix);
          
          PlainMessage(i, message);
        }  
      }
      
    }
  }
  else  {
    LogError("SQL Error #60: %s", error);
  }
}
