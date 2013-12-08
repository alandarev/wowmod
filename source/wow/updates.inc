/*
* File: updates.inc
* Description: Updates Functions
* Author(s): Stinkyfax
*/
RunUpdate8() {
   PrintToServer("Update has started");
   new Handle:query=SQL_Query(hSQL,"SELECT CharacterId, level FROM wc_Characters");
   if(query!=INVALID_HANDLE)
   {
      SQL_Rewind(query);
      new total=SQL_GetRowCount(query);
      new cLevel;
      SQL_FieldNameToNum(query,"level",cLevel);

      new cCharId;
      SQL_FieldNameToNum(query,"CharacterId",cCharId);
      new now=0;
      new step=60;
      while(SQL_FetchRow(query)) {
         if ((now % step) == 0) {
            new perc=RoundToNearest(float(now)/float(total)*100.0);
            LogMessage("Done - %i percentage",perc);
         }
         new level = SQL_FetchInt(query,cLevel);
         new reqXp = GetReqXpByLevel(level*4);
         new charId = SQL_FetchInt(query,cCharId);
         level*=4;
         wcUpdatePlayerIntFast(charId,"reqxp",reqXp,"CharacterId","wc_Characters");
         wcUpdatePlayerIntFast(charId,"level",level,"CharacterId","wc_Characters");
         now++;
      }
      UpdateServerVersion(9);
      LogMessage("Database version is now 9, phew");
      return 9;
    }
   LogError("Update 0.96-0.98 has failed");
   return 8;
}

RunUpdate10() {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Characters ADD `hp` INTEGER(6) UNSIGNED NOT NULL DEFAULT '100', ADD `mn` INTEGER(6) UNSIGNED NOT NULL DEFAULT '5', ADD `armor` INTEGER(8) UNSIGNED NOT NULL DEFAULT '0', ADD `dps` INTEGER(4) UNSIGNED NOT NULL DEFAULT '0', ADD `res` INTEGER(4) UNSIGNED NOT NULL DEFAULT '0', ADD `sdmg` INTEGER(4) UNSIGNED NOT NULL DEFAULT '0', ADD `rank` VARCHAR(25) NOT NULL DEFAULT 'Copral'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      UpdateServerVersion(11);
      LogMessage("Database version is now 11");
      return 11;
    }
   LogError("Update 10-11 has failed");
   return 10;
}

RunUpdate11() {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `login` VARCHAR(30) NOT NULL DEFAULT '', ADD `pass` VARCHAR(30) NOT NULL DEFAULT '', ADD `email` VARCHAR(50) NOT NULL DEFAULT '',  ADD `userid` VARCHAR(32), ADD userlevel tinyint(2) unsigned not null, ADD `gold` INTEGER(8) UNSIGNED NOT NULL DEFAULT '0'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      UpdateServerVersion(12);
      LogMessage("Database version is now 12");
      return 12;
    }
   LogError("Update 11-12 has failed");
   return 11;
}

RunUpdate12() {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"SELECT CharacterId, points, (points + str + sta + agi + inte - 4) as total, level,  (ROUND(level/4.0 - 0.5, 0) + level + 19) as `must`, ((ROUND(level/4.0 - 0.5, 0) + level + 19)-(points + str + sta + agi + inte - 4)) as `dif` FROM wc_Characters where ((ROUND(level/4.0 - 0.5, 0) + level + 19) > (points + str + sta + agi + inte - 4)) AND (CharacterId > 1)");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      SQL_Rewind(query);
      new total=SQL_GetRowCount(query);

      //cols
      new cDiff;
      SQL_FieldNameToNum(query,"dif",cDiff);
      new cCharId;
      SQL_FieldNameToNum(query,"CharacterId",cCharId);
      new cPoints;
      SQL_FieldNameToNum(query,"points",cPoints);

      new now=0;
      new step=10;
      while(SQL_FetchRow(query)) {
         if ((now % step) == 0) {
            new perc=RoundToNearest(float(now)/float(total)*100.0);
            LogMessage("Done - %i percentage",perc);
         }
         new points = SQL_FetchInt(query,cDiff) + SQL_FetchInt(query,cPoints);
         new charId = SQL_FetchInt(query,cCharId);
         wcUpdatePlayerIntFast(charId,"points",points,"CharacterId","wc_Characters");
         now++;
      }
      UpdateServerVersion(13);
      LogMessage("Database version is now 13");
      return 13;
    }
   LogError("Update 12-13 has failed");
   return 12;
}

RunUpdate14() {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `arenaPoints` INTEGER(10) unsigned NOT NULL DEFAULT '1500', ADD `arenaPlayed` INTEGER(4) unsigned NOT NULL DEFAULT '0'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      UpdateServerVersion(14);
      LogMessage("Database version is now 14");
      return 14;
    }

   LogError("Update 13-14 has failed");
   return 13;
}

RunUpdate15() {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `honor` INTEGER(10) unsigned NOT NULL DEFAULT '0'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      UpdateServerVersion(15);
      LogMessage("Database version is now 15");
      return 15;
    }

   LogError("Update 14-15 has failed");
   return 14;
}

RunUpdate16() {
   PrintToServer("Update has started");
   wcCreateSQLTableChat();
   LogMessage("Update 15-16 has finished");
   UpdateServerVersion(16);
   return 16;
}

RunUpdate17() {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `threat` INTEGER(3) unsigned NOT NULL DEFAULT '0'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      UpdateServerVersion(17);
      LogMessage("Database version is now 17");
      return 17;
    }
   LogError("Update 16-17 has failed");
   return 16;
}

RunUpdate18(bool:change=true) {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `rank` INTEGER(3) unsigned NOT NULL DEFAULT '1', ADD `trophies` INTEGER(7) unsigned NOT NULL DEFAULT '0'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      if(change)
        UpdateServerVersion(18);
      LogMessage("Database version is now 18");
      return 18;
    }
   LogError("Update 17-18 has failed");
   return 17;
}

GetReqXpByLevel(level) {
   new d = level % 4;
   new d2 = RoundToFloor(float(level)/4.0);
   new Float:modifier = 0.2;
   switch(d)   {
      case 1:
         modifier = 0.22;
      case 2:
         modifier = 0.28;
      case 3:
         modifier = 0.3;
   }
   //amount = Roun
   new Float:reqxp=Pow(1.58,float(d2));
   return RoundToCeil(220.0*reqxp*modifier);
}


RunUpdate19() {
   PrintToServer("Update has started");
   if(!CreateUniInvTable()) {
    LogError("Couldn't create table for update 19");
    return 18;
   }

   new Handle:query=SQL_Query(hSQL,"SELECT CharacterId FROM wc_Characters");
   if(query!=INVALID_HANDLE)
   {
      SQL_Rewind(query);
      new total=SQL_GetRowCount(query);

      new cCharId;
      SQL_FieldNameToNum(query,"CharacterId",cCharId);
      new now=0;
      new step=60;
      while(SQL_FetchRow(query)) {
         if ((now % step) == 0) {
            new perc=RoundToNearest(float(now)/float(total)*100.0);
            LogMessage("Done - %i percentage",perc);
         }
         new charId = SQL_FetchInt(query,cCharId);
         decl String:temp[255];
         Format(temp,sizeof(temp),"INSERT INTO wc_CItems (`CharacterId`) VALUES ('%i')", charId);
         while(!SQL_FastQuery(hSQL,temp))   {
            PrintToServer("Retrying query: %s", temp);
         }
         now++;
      }
      UpdateServerVersion(19);
      LogMessage("Database version is now 19, what a stress test...");
      return 19;
    }
   LogError("Update 19 has failed");
   return 18;
}

RunUpdate20() {
   PrintToServer("Update has started");
   CreateItemsTable();
   PrintToServer("Update is finished");
   UpdateServerVersion(20);
   return 20;
}

RunUpdate21() {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Mail ADD `unique` INTEGER(10) NOT NULL DEFAULT '0'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      //UpdateServerVersion(21);
      LogMessage("Database version is now 20.5");
    }
   else
    return 20;
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Auction ADD `unique` INTEGER(10) NOT NULL DEFAULT '0'");
   query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      UpdateServerVersion(21);
      LogMessage("Database version is now 21");
      return 21;
    }
   LogError("Update 20-21 has failed");
   return 20;
}

RunUpdate22(bool:change=true) {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `newbie` INTEGER(3) unsigned NOT NULL DEFAULT '1'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      if(change)
        UpdateServerVersion(22);
      LogMessage("Database version is now 22");
      return 22;
    }
   LogError("Update 21-22 has failed");
   return 21;
}



RunUpdate23(bool:change=true) {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Characters ADD `rating` FLOAT(9,2) UNSIGNED NOT NULL DEFAULT '5.00'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      if(change)
        UpdateServerVersion(23);
      LogMessage("Database version is now 23");
      return 23;
    }
   LogError("Update 22-23 has failed");
   return 22;
}


RunUpdate24(bool:change=true) {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `chatVic` INTEGER(3) unsigned NOT NULL DEFAULT '1', ADD `chatAtt` INTEGER(3) unsigned NOT NULL DEFAULT '1', ADD `chatTar` INTEGER(3) unsigned NOT NULL DEFAULT '1'");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      if(change)
        UpdateServerVersion(24);
      LogMessage("Database version is now 24");
      return 24;
    }
   LogError("Update 23-24 has failed");
   return 23;
}

RunUpdate25() {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"SELECT CharacterId, level FROM wc_Characters");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      SQL_Rewind(query);
      new total=SQL_GetRowCount(query);

      //cols
      new cLevel;
      SQL_FieldNameToNum(query,"level",cLevel);
      new cCharId;
      SQL_FieldNameToNum(query,"CharacterId",cCharId);

      new now=0;
      new step=10;
      while(SQL_FetchRow(query)) {
         if ((now % step) == 0) {
            new perc=RoundToNearest(float(now)/float(total)*100.0);
            LogMessage("Done - %i percentage",perc);
         }
         new charId = SQL_FetchInt(query, cCharId);
         new needs = getMaxTalents(SQL_FetchInt(query, cLevel));
         wcUpdatePlayerIntFast(charId,"talentpoints",needs,"CharacterId","wc_Characters");
         wcUpdatePlayerStrFast(charId,"talents","","CharacterId","wc_Characters");
         now++;
      }
      UpdateServerVersion(25);
      LogMessage("Database version is now 25");
      return 25;
    }
   LogError("Update 24-25 has failed");
   return 24;
}

RunUpdate26() {
   PrintToServer("Update has started");
   decl String:buffer[900];
   Format(buffer,sizeof(buffer),"SELECT * FROM wc_Characters");
   new Handle:query=SQL_Query(hSQL,buffer);
   if(query!=INVALID_HANDLE)
   {
      SQL_Rewind(query);
      new total=SQL_GetRowCount(query);

      //cols
      new cLevel;
      SQL_FieldNameToNum(query,"level",cLevel);
      new cCharId;
      SQL_FieldNameToNum(query,"CharacterId",cCharId);
      new cClass;
      SQL_FieldNameToNum(query,"class",cClass);

      new now=0;
      new step=10;
      while(SQL_FetchRow(query)) {
        if ((now % step) == 0) {
          new perc=RoundToNearest(float(now)/float(total)*100.0);
          LogMessage("Done - %i percentage",perc);
        }

        new charId = SQL_FetchInt(query, cCharId);
        new level = SQL_FetchInt(query, cLevel);
        new race = SQL_FetchInt(query, cClass);

        if(race>0) {
          new str=1;
          new agi=1;
          new sta=1;
          new inte=1;


          //Get total points
          new points=20;
          new pLevel = 1;
          new amount=level;
          while (pLevel < amount) {
            pLevel++;
            if ((pLevel % 4) == 0)
              points++;
            if(pLevel <= 30)
              points++;
            points++;
          }

          for(new i=0;i<points;i++)  {
            new StatPoint:point = _getNextStat(race,str,agi,inte,sta);
            switch(point) {
              case SP_Strength: {
                str++;
              }
              case SP_Agility:  {
                agi++;
              }
              case SP_Intellect:  {
                inte++;
              }
              case SP_Stamina:  {
                sta++;
              }
            }
          }

          wcUpdatePlayerIntFast(charId,"str",str,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"agi",agi,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"inte",inte,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"sta",sta,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"points",0,"CharacterId","wc_Characters");

        }
        else  {
          //No class, reset him to 1 level
          wcUpdatePlayerIntFast(charId,"str",1,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"agi",1,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"inte",1,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"sta",1,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"points",0,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"level",1,"CharacterId","wc_Characters");
          wcUpdatePlayerIntFast(charId,"talentpoints",0,"CharacterId","wc_Characters");
        }

        now++;
      }
      UpdateServerVersion(26);
      LogMessage("Database version is now 26");
      return 26;
    }
   LogError("Update 25-26 has failed");
   return 25;
}

RunUpdate27(bool:change=true) {
  PrintToServer("Update has started");
  decl String:buffer[900];
  Format(buffer,sizeof(buffer),"ALTER TABLE wc_Players ADD `chatSpa` INTEGER(3) unsigned NOT NULL DEFAULT '1'");
  new Handle:query=SQL_Query(hSQL,buffer);
  if(query!=INVALID_HANDLE)
  {
    if(change)
      UpdateServerVersion(27);
    LogMessage("Database version is now 27");
    return 27;
  }
  LogError("Update 26-27 has failed");
  return 26;
}
