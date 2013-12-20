// This file includes:
	// Determining how much experience a player is worth (GetTargetXPWorth) and (PlayerKillXp)
	// Low Level Damage Modifier I think? (LowLevelInfo)

/*
* File: xpgaining.inc
* Description: This file contains xp gained functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1

public TeamWinXP(team)
{
  if ((team==2) || (team==3))
  {
    new players = PlayersInTeams(true);
    new maxplayers=GetMaxClients();
    for(new x=1;x<=maxplayers;x++)
      if(IsClientInGame(x))
        if(GetClientTeam(x)==team)  {
          wcGiveXp(x,(RoundToNearest(TeamRatio(team)*float((2+GetLevel(x)))*EXP_TEAMWIN*GLOBAL_XP*GetTempVar(x, Temp_XPRate))),true);
          if(players >= TEAMWIN_PLAYERS)  {
            WCMessage(x, "triggered teamdrop", x, TEAMWIN_DROPRATE);
            TriggerDrop(x,x,true,TEAMWIN_DROPRATE);
          }
        }
  }
}

public LowLevelRound()
{
  new maxplayers=GetMaxClients();
  for(new x=1;x<=maxplayers;x++)
    if(IsClientInGame(x))
    {
      new team=GetClientTeam(x);
      if((team==2) || (team==3))
        if (GetLevel(x)<=52)
        {
          new give=RoundToNearest((10.0+float(GetLevel(x))*2.5)*GLOBAL_XP*GetTempVar(x, Temp_XPRate));
          wcGiveXp(x,give,false);
          WCMessage(x,"LowLevelRound",x,give);
        }
    }
}

public Action:HealingDoneXP(Handle:timer)
{
  new maxplayers=GetMaxClients();
  for (new x=1;x<=maxplayers;x++)
    if(IsClientInGame(x) && (wcHealingDone[x]>0))
    {
      new give=RoundToNearest(float(wcHealingDone[x])*EXP_HEALING*GLOBAL_XP*GetTempVar(x, Temp_XPRate));
      wcGiveXp(x,give,false);
      WCMessage(x,"XpForHealing",x,give,wcHealingDone[x]);
      wcTotalHealing[x]+=wcHealingDone[x];
      while(wcTotalHealing[x]>=6*(GetLevel(x)+1) && !IsBoss())  {
         WCMessage(x,"good healing",x);
         wcTotalHealing[x]-=6*(GetLevel(x)+1);
         TriggerDrop(x,x,true);
      }
    }
  for (new x=0;x<=64;x++)
    wcHealingDone[x]=0;
}

public Action:RoundStats(Handle:timer)
{
  new maxplayers=GetMaxClients();
  new dmg,hp,taken;
  for (new x=1;x<=maxplayers;x++)   {
   if(IsClientInGame(x) && (GetBoss(x)==0))   {
      if(wcDamageDone[dmg]<wcDamageDone[x]) {
        dmg=x;
        if(PlayersInTeams(true) >= 6 && !IsBoss())
        {
          new honorgain = GetHonor(dmg);
          honorgain+=RoundToNearest(float(wcDamageDone[dmg]) * 0.0033);
          SetHonor(dmg,honorgain);
          }
        }
      if(wcDamageTaken[taken]<wcDamageTaken[x]) {
         taken=x;
         if(PlayersInTeams(true) >= 6 && !IsBoss())
          {
            new honorgain = GetHonor(taken);
            honorgain+=RoundToNearest(float(wcDamageTaken[taken]) * 0.0033);
            SetHonor(taken,honorgain);
          }
         }
      if(wcHealingDone[hp]<wcHealingDone[x]) {
         hp=x;
         if(PlayersInTeams(true) >= 6 && !IsBoss())
          {
            new honorgain = GetHonor(hp);
            honorgain+=RoundToNearest(float(wcHealingDone[hp]) * 0.0066);
            SetHonor(hp,honorgain);
          }
         }
      }
  }
  new String:sDmg[20],String:sHp[20],String:sTaken[20];
  if(dmg>0)
   GetClientName(dmg,sDmg,sizeof(sDmg));
  else
   //Format(sDmg, sizeof(sDmg), "%T", "Nobody", i);
   sDmg="Nobody";
  if(hp>0)
   GetClientName(hp,sHp,sizeof(sHp));
  else
   //Format(sHp, sizeof(sDmg), "%T", "Nobody");
   sHp="Nobody";
  if(taken>0)
   GetClientName(taken,sTaken,sizeof(sTaken));
  else
   //Format(sTaken, sizeof(sDmg), "%T", "Nobody");
   sTaken="Nobody";
  decl String:msg[300];
  for(new i=1;i<=maxplayers;i++)
   if(IsClientInGame(i))  {
     if (StrEqual(sDmg, "Nobody"))
       Format(sDmg, sizeof(sDmg), "%T", "Nobody", i);
     if (StrEqual(sHp, "Nobody"))
       Format(sHp, sizeof(sHp), "%T", "Nobody", i);
     if (StrEqual(sTaken, "Nobody"))
       Format(sTaken, sizeof(sTaken), "%T", "Nobody", i);
     Format(msg,sizeof(msg),"%T","RoundStats",i,wcDamageDone[dmg],sDmg,wcDamageTaken[taken],sTaken,wcHealingDone[hp],sHp);
     PlainMessage(i,msg);
   }
}

public Action:DamageDoneXP(Handle:timer)
{
  new maxplayers=GetMaxClients();
  for (new x=1;x<=maxplayers;x++) {
    if(IsClientInGame(x) && (wcDamageDone[x]>0))
    {
      new give=RoundToNearest(float(wcDamageDone[x])*EXP_DAMAGE*GLOBAL_XP*GetTempVar(x, Temp_XPRate));
      wcGiveXp(x,give,false);
      WCMessage(x,"XpForDamage",x,give,wcDamageDone[x]);
    }
    if(IsClientInGame(x) && (GetClientTeam(x) == 2 || GetClientTeam(x) == 3)) {
      ItemDropBalancer(x);
    }

  }
  for (new x=0;x<=64;x++)  {
    wcDamageDone[x]=0;
    wcDamageTaken[x]=0;
   }
}

public PlayerKillXp(client,victim,bool:head)
{
  if(GetClientTeam(client)!=GetClientTeam(victim))
  {
   if((GetBoss(client)) == 0 || (!IsBoss() && !IsHNS()))  {
      new xp = GetTargetXPWorth(victim,client,head);
      GiveRestXp(client, RoundToNearest(float(xp)*0.5));
      wcGiveXp(client,xp,true);
      LowLevelInfo(victim, client);
      if(PlayersInTeams(true) >= 6)
      {
        new honorgain = GetHonor(client);
        new level = GetLevel(victim);
        if(level >= 85) {
          honorgain+=3;
          SetHonor(client,honorgain);
          }
        if(level >= 80 && level < 85) {
          honorgain+=2;
          SetHonor(client,honorgain);
          }
        if(level >= 70 && level < 80) {
          honorgain+=1;
          SetHonor(client,honorgain);
          }
        }
      }
  }
}

/*
LowLevelInfo(client, attacker)  {
  new lowered=100;
  new increased=100;
  LowLvlDmg(client,attacker,lowered,false);
  LowLvlDmg(attacker,client,increased,false);
  increased = KpdRule(attacker, increased, 1);
  lowered = KpdRule(client, lowered, 2);


  WCMessage(client, "lvl explanation victim", client, lowered, increased, GetLevel(attacker), GetAverageLevel());
  WCMessage(attacker, "lvl explanation attacker", attacker, lowered, increased, GetLevel(client), GetAverageLevel());
}
 */

LowLevelInfo(victim, attacker)  {

  new vlevel = GetLevel(victim);
  new alevel = GetLevel(attacker);
  if(alevel==vlevel && vlevel>=85) {
    new akpr = KpdRule(attacker, 100, 1, 1); //attacker dealing damage
    akpr = KpdRule(victim, 100, 2, 1); //victim taking attackers damage
    new vkpr = KpdRule(victim,   100, 1, 1); //victim dealing damage
    vkpr = KpdRule(attacker,   100, 2, 1); //attacker taking victims damage
    akpr -= 100; //remove the original 100 input, left with the +/- %
    vkpr -= 100; //remove the original 100 input, left with the +/- %

    new alow = 100,vlow = 100;
    LowLvlDmg(victim,attacker,alow,false);
    LowLvlDmg(attacker,victim,vlow,false);

    alow -= 100; //remove the original 100 input, left with the +/- %
    vlow -= 100; //remove the original 100 input, left with the +/- %

    new String:name[50];

    //attacker
    GetClientName(victim,name,sizeof(name));
    WCMessage(attacker,"your kpr lowlvl adj",attacker,
      100+akpr+alow,              //attacker combined changes
      akpr,                       //attacker amount of kpr
      alow);                      //attacker amount of low level


    WCMessage(attacker,"their kpr lowlvl adj",attacker,
      name,                       //victim name
      100+vkpr+vlow,              //victim combined changes
      vkpr,                       //victim amount of kpr
      vlow);                      //victim amount of low level

    WCMessage(attacker,"lvl average info",attacker,GetLevel(attacker),GetAverageLevel(),name,GetLevel(victim));
    //new apenalty = KpdRule(attacker, 100, 1);
    //new akills = getKills(attacker);
    //new arounds = getRounds(attacker);
    //WCMessage(attacker, "debug kpr info", attacker, akills, arounds, apenalty);

    //victim
    GetClientName(attacker,name,sizeof(name));
    WCMessage(victim,"your kpr lowlvl adj",victim,
      100+vkpr+vlow,              //victim combined changes
      vkpr,                       //victim amount of kpr
      vlow);                      //victim amount of low level

    WCMessage(victim,"their kpr lowlvl adj",victim,
      name,                       //attacker name
      100+akpr+alow,              //attacker combined changes+100
      akpr,                       //attacker amount of kpr
      alow);                      //attacker amount of low level

    WCMessage(victim,"lvl average info",victim,GetLevel(victim),GetAverageLevel(),name,GetLevel(attacker));
    //new vpenalty = KpdRule(victim, 100, 1);
    //new vkills = getKills(victim);
    //new vrounds = getRounds(victim);
    //WCMessage(victim, "debug kpr info", victim, vkills, vrounds, vpenalty);
  }
  else
  {
    new akpr = 100;
    new vkpr = 100;
    if(alevel >= vlevel)
      akpr = KpdRule(attacker, 100, 1); //attacker dealing damage
    if(vlevel >= alevel)
      akpr = KpdRule(victim, 100, 2); //victim taking attackers damage
    if(vlevel >= alevel) 
      vkpr = KpdRule(victim,   100, 1); //victim dealing damage
    if(alevel >= vlevel)
      vkpr = KpdRule(attacker,   100, 2); //attacker taking victims damage
    akpr -= 100; //remove the original 100 input, left with the +/- %
    vkpr -= 100; //remove the original 100 input, left with the +/- %

    new alow = 100,vlow = 100;
    LowLvlDmg(victim,attacker,alow,false);
    LowLvlDmg(attacker,victim,vlow,false);

    alow -= 100; //remove the original 100 input, left with the +/- %
    vlow -= 100; //remove the original 100 input, left with the +/- %

    new String:name[50];

    //attacker
    GetClientName(victim,name,sizeof(name));
    WCMessage(attacker,"your kpr lowlvl adj",attacker,
      100+akpr+alow,              //attacker combined changes
      akpr,                       //attacker amount of kpr
      alow);                      //attacker amount of low level


    WCMessage(attacker,"their kpr lowlvl adj",attacker,
      name,                       //victim name
      100+vkpr+vlow,              //victim combined changes
      vkpr,                       //victim amount of kpr
      vlow);                      //victim amount of low level

    WCMessage(attacker,"lvl average info",attacker,GetLevel(attacker),GetAverageLevel(),name,GetLevel(victim));
    //new apenalty = KpdRule(attacker, 100, 1);
    //new akills = getKills(attacker);
    //new arounds = getRounds(attacker);
    //WCMessage(attacker, "debug kpr info", attacker, akills, arounds, apenalty);

    //victim
    GetClientName(attacker,name,sizeof(name));
    WCMessage(victim,"your kpr lowlvl adj",victim,
      100+vkpr+vlow,              //victim combined changes
      vkpr,                       //victim amount of kpr
      vlow);                      //victim amount of low level

    WCMessage(victim,"their kpr lowlvl adj",victim,
      name,                       //attacker name
      100+akpr+alow,              //attacker combined changes+100
      akpr,                       //attacker amount of kpr
      alow);                      //attacker amount of low level

    WCMessage(victim,"lvl average info",victim,GetLevel(victim),GetAverageLevel(),name,GetLevel(attacker));
    //new vpenalty = KpdRule(victim, 100, 1);
    //new vkills = getKills(victim);
    //new vrounds = getRounds(victim);
    //WCMessage(victim, "debug kpr info", victim, vkills, vrounds, vpenalty);
    }

  //new akpr = KpdRule(attacker, 100, 1); //attacker dealing damage
  //akpr = KpdRule(victim, 100, 2); //victim taking attackers damage

  //new vkpr = KpdRule(victim,   100, 1); //victim dealing damage
  //vkpr = KpdRule(attacker,   100, 2); //attacker taking victims damage

}

// New Experience per kill method, similar to WoW
 public GetTargetXPWorth(client,attacker,bool:head)
{
	new Float:base = float(GetLevel(client) * 5) + 45.0;
	new Float:worth = 0.0;
	new d = GetLevel(client) - GetLevel(attacker);
	new levelbetween = (d>0) ? RoundToFloor(d/10) ? RoundToCeil(d/10);
	switch(levelbetween)
	{
		case 1,2,3,4,5,6: //,7,8: //***
		worth=RoundToCeil((1 + 0.05 * d) * base);
		case 0:
			worth=base;
		case -1:
			worth=RoundToCeil((1 - ((-d) / 7)) * base);
		case -2:
			worth=RoundToCeil((1 - ((-d) / 11)) * base);
		case -3:
			worth=RoundToCeil((1 - ((-d) / 12)) * base);
		case -4:
			worth=RoundToCeil((1 - ((-d) / 13)) * base);
		case -5:
			worth=RoundToCeil((1 - ((-d) / 15)) * base);
		case -6:
			worth=RoundToCeil((1 - ((-d) / 17)) * base);
		// originally you only pasted to me from 1-6 but if you have more just uncomment and place here
		//case -7:
		//  worth=RoundToCeil((1 - ((-d) / 7)) * base);
		//case -8:
		//  worth=RoundToCeil((1 - ((-d) / 7)) * base);
		default:
			worth=base;
	}
   
	if(IsFakeClient(client))
	{
		if( SERVER_LEVEL_POLICY == 1)
			worth = RoundToNearest(BOT_HIGH_RATE*worth);
		else  
			worth = RoundToNearest(BOT_NORMAL_RATE*worth);
	}
	g_Deaths[attacker]=0;
	return worth;
}

// Old method for determining players experience
//public GetTargetXPWorth(client,attacker,bool:head)
//{
//   new worth=RoundToCeil((200+Pow(float(GetLevel(client))*1.0,1.45))*EXP_KILL*(1.0+EXP_SPREE*float(g_Deaths[attacker]))*GetRandomFloat(0.85,1.2)*GLOBAL_XP*GetTempVar(attacker, Temp_XPRate));
//   if(IsFakeClient(client)) {
//      if( SERVER_LEVEL_POLICY == 1) worth = RoundToNearest(BOT_HIGH_RATE*worth);
//      else  worth = RoundToNearest(BOT_NORMAL_RATE*worth);
//   }
//   if(head)
//      worth=RoundToNearest(EXP_HEADSHOT*float(worth));
//   g_Deaths[attacker]=0;
//   //if(UniqueID(client)==1)
//   //   worth=RoundToNearest(0.33*float(worth));
//   new levelbetween = GetLevel(client) - GetLevel(attacker);
//   if(0>=levelbetween)
//    levelbetween = 1;
//   new Float:lowlvlworth = 1.0 + (float(levelbetween) / 20);
//   worth+= RoundToCeil(float(worth) * lowlvlworth);
//   return worth;
//}

public Float:TeamRatio(team)
{
  new ttotal;
  new cttotal;
  new maxplayers=GetMaxClients();
  for(new x=1;x<=maxplayers;x++)
  {
    if(IsClientInGame(x))
    {
      if(GetClientTeam(x)==2)
        ttotal++;
      else if(GetClientTeam(x)==3)
        cttotal++;
    }
  }
  if(team==2)
    return float(cttotal)/float(ttotal);
  else
    return float(ttotal)/float(cttotal);
}

stock PlayersInTeams(bool:ignoreBots=false)  {
  new total;
  for(new x=1;x<=GetMaxClients();x++)
  {
    if(IsClientInGame(x))
    {
      if(ignoreBots && IsFakeClient(x))
        continue;
      switch(GetClientTeam(x))  {
        case 2: total++;
        case 3: total++;
      }
    }
  }
  return total;
}

stock getActivePlayers(players[])  {
  new total;
  for(new x=1;x<=GetMaxClients();x++)
  {
    if(IsClientInGame(x))
    {
      if(IsFakeClient(x))
        continue;
      switch(GetClientTeam(x))  {
        case 2: players[total++]=x;
        case 3: players[total++]=x;
      }
    }
  }
  return total;
}

//Rest xp base
new g_ff_restPoints[65];

setRestXp(client, amount) {
  g_ff_restPoints[client]=amount;
}
getRestXp(client) {
  return g_ff_restPoints[client];
}





stock GiveRestXp(client, amount)  {
  new max = getRestXp(client);
  if(max<=0)
    return;
  new give = GetMinInt(max, amount);
  setRestXp(client, getRestXp(client) - give);
  WCMessage(client, "rest xp give", client, give, getRestXp(client));
  wcGiveXp(client,give,false);
}








