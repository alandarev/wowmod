/*
* File: logger.inc
* Description: Logging functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1

#define LOGGER "Logger"

enum LogType  {
  LT_file,
  LT_sql
}

enum LogLevel {
  LL_error,
  LL_warning,
  LL_debug,
  LL_log
}



//not used yet
enum Log  {
  LogType:Log_type,
  LogLevel:Log_level,
  String:Log_cl[50],
  String:Log_msg[500]
}

/*
stock Logger_Construct(Log:log, LogType:type, LogLevel:level, String:cl[], String:msg[]) {
  log[Log_type] = type;
  log[Log_level] = level;
  strcopy(log[Log_cl], 50, cl);
  strcopy(log[Log_msg], 500, msg);

}*/

stock Logger_log(LogType:type, LogLevel:level, String:cl[], String:msg[], any:...) {
  //new log[Log];
  //Logger_Construct(log, type, level, cl, msg);
  decl String:format[400];
  VFormat(format, sizeof(format), msg, 5);
  switch(type) {
    case LT_file:  Logger_logFile(level, cl, format);
    case LT_sql:  Logger_logSql(level, cl, format);
    default:  {
      decl String:err[500];
      Format(err, 500, "Invalid logger type is called by: %s", format);
      Logger_log(LogType:LT_file, LogLevel:LL_error, LOGGER, err);
    }
  }
}

stock Logger_logSql(LogLevel:lvl, String:cl[], String:msg[])  {
  decl String:level[20];
  switch(lvl) {
    case LL_error:  {
      Format(level, sizeof(level), "error");
    }
    case LL_warning:  {
      Format(level, sizeof(level), "warning");
    }
    case LL_debug:  {
      Format(level, sizeof(level), "debug");
    }
    case LL_log:  {
      Format(level, sizeof(level), "log");
    }
    default:  {
      Format(level, sizeof(level), "unknown");
    }
  }
  sqlLog(level, cl, msg);
}

stock Logger_logGold(playerId, String:service[], String:cMsg[], value) {
  decl String:query[400];
  decl String:msg[100];
  SQL_EscapeString(SSQL, cMsg, msg, sizeof(msg));
  Format(query, sizeof(query),
    "INSERT INTO wc_logGold (`date`, `service`, `msg`, `playerId`, `value`) VALUES (NOW(), '%s', '%s', %d, %d)",
    service, msg, playerId, value);
  SQL_TQuery(SSQL, Tinserted, query, 87);
}



stock Logger_logFile(LogLevel:lvl, String:cl[], String:msg[]) {
  decl String:formed[550];
  switch(lvl) {
    case LL_error:  {
      Format(formed, 550, "WoW Error [%s]: %s", cl, msg);
    }
    case LL_warning:  {
      Format(formed, 550, "WoW [%s]: %s", cl, msg);
    }
    case LL_debug:  {
      if(DEBUG) Format(formed, 550, "WoW DEBUG [%s]: %s", cl, msg);
    }
  }

  //TODO
  //only for now
  LogError(formed);

}
