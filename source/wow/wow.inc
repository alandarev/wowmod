/*
File: wow.inc
Description: Main include file for wow
Author(s): Stinkyfax
*/

#pragma semicolon 1

// Config
//Gold
#define GOLD_CYCLE_PERIOD FindSettingFloat("gold","GOLD_CYCLE_PERIOD")
#define GOLD_CYCLE_AMOUNT FindSettingInt("gold","GOLD_CYCLE_AMOUNT")
// General
#define SERVER_80 FindSettingBool("general","SERVER_80")
#define DONATIONS FindSettingInt("general", "DONATIONS")
#define ENABLE FindSettingBool("general","ENABLE")
#define CUSTOM_MODELS FindSettingBool("general", "CUSTOM_MODELS")
#define HEAL_BOT FindSettingBool("general", "HEAL_BOT")
#define FUN_SERVER FindSettingBool("general","FUN_SERVER")
#define MAIN_SERVER FindSettingBool("general","MAIN_SERVER")
#define SAFE_START FindSettingBool("general","SAFE_START")
#define SUPPORT_OLD_STEAM FindSettingBool("general","SUPPORT_OLD_STEAM")
#define ANNOUNCE_CHANGELVL FindSettingBool("general","ANNOUNCE_CHANGELVL")
#define BOT_MAXLEVEL FindSettingInt("general","BOT_MAXLEVEL")
#define BOT_DROP FindSettingBool("general","BOT_DROP")
#define BOT_HIGH_RATE FindSettingFloat("general","BOT_HIGH_RATE")
#define BOT_NORMAL_RATE FindSettingFloat("general","BOT_NORMAL_RATE")
#define STANCE_KNIFE FindSettingFloat("general","STANCE_KNIFE")
#define SAVEWORLD_DELAY FindSettingFloat("general","SAVEWORLD_DELAY")
#define GLOBAL_XP FindSettingFloat("general","GLOBAL_XP")
#define GLOBAL_DROP FindSettingFloat("general","GLOBAL_DROP")
#define PLAYER_REMOVE_DELAY FindSettingFloat("general","PLAYER_REMOVE_DELAY")
#define PLAYER_REMOVE_DAYS FindSettingInt("general","PLAYER_REMOVE_DAYS")
#define INIT_TIMEOUT FindSettingFloat("general","INIT_TIMEOUT")
#define INIT_DELAY FindSettingFloat("general","INIT_DELAY")
#define DOWNLOAD_SOUNDS FindSettingBool("general","DOWNLOAD_SOUNDS")
#define COMMUNICATION FindSettingBool("general","COMMUNICATION")
#define COMM_ID FindSettingInt("general","COMM_ID")

#define PASS_ENABLED FindSettingBool("general","PASSWORD")

#define RANKING_ENABLE FindSettingBool("general","RANKING_ENABLE")
#define LOWLVL_ARG FindSettingFloat("general","LOWLVL_ARG")
// XP
#define EXP_MONEY_RATIO FindSettingFloat("xp","EXP_MONEY_RATIO")
#define EXP_NOTICE FindSettingBool("xp","EXP_NOTICE")
#define EXP_START FindSettingInt("xp","EXP_START")
#define EXP_KILL FindSettingFloat("xp","EXP_KILL")
#define EXP_SPREE FindSettingFloat("xp","EXP_SPREE")
#define EXP_HEADSHOT FindSettingFloat("xp","EXP_HEADSHOT")
#define EXP_DAMAGE FindSettingFloat("xp","EXP_DAMAGE")
#define EXP_TEAMWIN FindSettingFloat("xp","EXP_TEAMWIN")
#define TEAMWIN_DROPRATE FindSettingFloat("xp","TEAMWIN_DROPRATE")
#define TEAMWIN_PLAYERS FindSettingFloat("xp","TEAMWIN_PLAYERS")
#define EXP_HEALING FindSettingFloat("xp","EXP_HEALING")
#define EXP_BOMB_PLANTED FindSettingInt("xp","EXP_BOMB_PLANTED")
#define EXP_BOMB_DEFUSE FindSettingInt("xp","EXP_BOMB_DEFUSE")
#define EXP_HOSTAGE_RESCUED FindSettingInt("xp","EXP_HOSTAGE_RESCUED")
#define RESTED_RATE FindSettingInt("xp","RESTED_RATE")
// mail
#define MAILCLEAN_DELAY FindSettingFloat("mail","MAILCLEAN_DELAY")
#define MAILCLEAN_DAYS FindSettingInt("mail","MAILCLEAN_DAYS")
#define MAILATTACH_DELAY FindSettingFloat("mail","MAILATTACH_DELAY")
//shop
#define SELL_PERCENT FindSettingFloat("shop","SELL_PERCENT")
#define RECIPE_LVL_MINUS FindSettingInt("shop","RECIPE_LVL_MINUS")
//auction
#define AUCTION_HOURS FindSettingInt("auction","AUCTION_HOURS")
#define AUCTION_BID_INC FindSettingFloat("auction","AUCTION_BID_INC")
#define AUCTION_TAX FindSettingFloat("auction","AUCTION_TAX")
#define AUCTION_DELAY FindSettingFloat("auction","AUCTION_DELAY")
//boss
#define HEAL_THREAT FindSettingFloat("boss","HEAL_THREAT")
#define DMG_THREAT FindSettingFloat("boss","DMG_THREAT")
#define MIN_THREAT FindSettingInt("boss","MIN_THREAT")
//pvp

#define ARENA_ENABLED FindSettingBool("pvp","ARENA_ENABLED")
#define ARENA_REDIRECTION FindSettingBool("pvp","ARENA_REDIRECTION")
#define ARENA_BASIC FindSettingInt("pvp","ARENA_BASIC")
#define ARENA_DIVIDER FindSettingFloat("pvp","ARENA_DIVIDER")
#define CTF_TIME FindSettingFloat("pvp","CTF_TIME")
#define CTF_SCORE FindSettingInt("pvp","CTF_SCORE")
//#define CTF_VICTORY FindSettingInt("pvp","CTF_VICTORY")
#define CTF_NERF FindSettingFloat("pvp","CTF_NERF")

//ratings
//defined name has same ending as UniqueItem enum
#define RATING_LEVEL FindSettingFloat("rating","level")
#define RATING_DAMAGE FindSettingFloat("rating","dps")
#define RATING_ARMOR FindSettingFloat("rating","armor")
#define RATING_SHIELD_WAR FindSettingFloat("rating","shield_warrior")
#define RATING_SHIELD_PAL FindSettingFloat("rating","shield_paladin")
#define RATING_SHIELD_SHA FindSettingFloat("rating","shield_shaman")
#define RATING_STR FindSettingFloat("rating","str")
#define RATING_AGI FindSettingFloat("rating","agi")
#define RATING_STA FindSettingFloat("rating","sta")
#define RATING_INT FindSettingFloat("rating","int")
#define RATING_AI FindSettingFloat("rating","ai")
#define RATING_REGEN FindSettingFloat("rating","regen")
#define RATING_AS FindSettingFloat("rating","as")
#define RATING_SPELLDMG FindSettingFloat("rating","spelldmg")
#define RATING_SPEED FindSettingFloat("rating","speed")
#define RATING_RES FindSettingFloat("rating","resistance")
#define RATING_RESILENCE FindSettingFloat("rating","resilence")
#define RATING_ACC FindSettingFloat("rating","accuracy")
#define RATING_CS FindSettingFloat("rating","critical")
#define RATING_EFFECT FindSettingFloat("rating","effect")
#define RATING_HASTE FindSettingFloat("rating","haste")
#define RATING_PARRY FindSettingFloat("rating","parry")
#define RATING_ILEVEL FindSettingFloat("rating","ilevel")
#define RATING_WEAPON_AP FindSettingFloat("rating","weapon_ap")
#define RATING_WEAPON_SP FindSettingFloat("rating","weapon_sp")
#define RATING_ARMOR_TYPE FindSettingFloat("rating","armor_type")

#define NEWBIE_UNTIL FindSettingInt("general","NEWBIE_UNTIL")

#define SERVER_LEVEL_POLICY FindSettingInt("general", "SERVER_LEVEL_POLICY")

//professions related
#define PROFESSION_BONUSES_AMOUNT 13
#define CRAFT_PROFS_AMOUNT 6
#define CLASSES 7

//Glyphs
#define MAX_GLYPHS 2
#define GLYPH_MAGE_INVIS 13774
#define GLYPH_MAGE_MISSLES 13776
#define GLYPH_MAGE_MOLTEN 13778
#define GLYPH_MAGE_BOMB 13780
#define GLYPH_MAGE_NOVA 13782
#define GLYPH_MAGE_ORB 13784
#define GLYPH_MAGE_ARCANEINT 20144
#define GLYPH_MAGE_FROSTBITE 20146
#define GLYPH_MAGE_SEARINGF 20148

#define GLYPH_PALADIN_RECKONING 13786
#define GLYPH_PALADIN_AVENGER 13788
#define GLYPH_PALADIN_STORM 13790
#define GLYPH_PALADIN_VERDICT 13792
#define GLYPH_PALADIN_SPIRITUAL 13794
#define GLYPH_PALADIN_RADIANCE 13796
#define GLYPH_PALADIN_SOC 20126
#define GLYPH_PALADIN_JUDGEMENT 20128
#define GLYPH_PALADIN_DIVINT 20130

#define GLYPH_WARLOCK_DRAIN 13798
#define GLYPH_WARLOCK_CORRUPTION 13800
#define GLYPH_WARLOCK_IMMOLATE 13802
#define GLYPH_WARLOCK_CHAOS 13804
#define GLYPH_WARLOCK_FEAR 13806
#define GLYPH_WARLOCK_META 13808
#define GLYPH_WARLOCK_SDOOM 20132
#define GLYPH_WARLOCK_SHADOWBOLT 20134
#define GLYPH_WARLOCK_DEMBRACE 20136

#define GLYPH_WARRIOR_WALL 13810
#define GLYPH_WARRIOR_REGEN 13812
#define GLYPH_WARRIOR_HEROIC 13814
#define GLYPH_WARRIOR_REND 13816
#define GLYPH_WARRIOR_LEAP 13818
#define GLYPH_WARRIOR_MORTAL 13820
#define GLYPH_WARRIOR_WHIRLWIND 20138
#define GLYPH_WARRIOR_TWOHWEP 20140
#define GLYPH_WARRIOR_HEAVYREP 20142


#define GLYPH_SHAMAN_WAVE 13822
#define GLYPH_SHAMAN_WATER 13824
#define GLYPH_SHAMAN_TOTEM 13826
#define GLYPH_SHAMAN_FROST 13828
#define GLYPH_SHAMAN_NOVA 13830
#define GLYPH_SHAMAN_RAGE 13832
#define GLYPH_SHAMAN_NPOWER 20114
#define GLYPH_SHAMAN_EARTHS 20116
#define GLYPH_SHAMAN_WINDFURY 20118

#define GLYPH_PRIEST_HYMN 13834
#define GLYPH_PRIEST_HEALS 13836
#define GLYPH_PRIEST_SHIELD 13838
#define GLYPH_PRIEST_INNER 13840
#define GLYPH_PRIEST_SHADOWFORM 13842
#define GLYPH_PRIEST_DEATH 13844
#define GLYPH_PRIEST_LIGHTWELL 20122
#define GLYPH_PRIEST_MINDBLAST 20120
#define GLYPH_PRIEST_PWSHIELD 20124

#define GLYPH_DK_HEARTFIRE 13846
#define GLYPH_DK_RUNETAP 13848
#define GLYPH_DK_HOWLING 13850
#define GLYPH_DK_FROST 13852
#define GLYPH_DK_NECROTIC 13854
#define GLYPH_DK_SCOURGE 13856
#define GLYPH_DK_AOF 20108
#define GLYPH_DK_VAMBLOOD 20110
#define GLYPH_DK_CORPSEPARTY 20112

#define GLYPH_ROGUE_RUSH 13858
#define GLYPH_ROGUE_SINISTER 13860
#define GLYPH_ROGUE_FAN 13862
#define GLYPH_ROGUE_POISON 13864
#define GLYPH_ROGUE_STEALTH 13866
#define GLYPH_ROGUE_AMBUSH 13868
#define GLYPH_ROGUE_SPRINT 20100
#define GLYPH_ROGUE_SSTEP 20102
#define GLYPH_ROGUE_MINTENT 20104
#define GLYPH_ROGUE_MASTEROS 20106


new ACTIVEWEAPON=1896;
new CLIPOFFSET=1204;

enum ShakeFlag {
	
	SHAKE_START = 0,		// Starts the screen shake for all players within the radius.
	SHAKE_STOP,				// Stops the screen shake for all players within the radius.
	SHAKE_AMPLITUDE,		// Modifies the amplitude of an active screen shake for all players within the radius.
	SHAKE_FREQUENCY,		// Modifies the frequency of an active screen shake for all players within the radius.
	SHAKE_START_RUMBLEONLY,	// Starts a shake effect that only rumbles the controller, no screen effect.
	SHAKE_START_NORUMBLE,	// Starts a shake that does NOT rumble the controller.
	
};

enum CharacterType  {
  Character_Healer,
  Character_Tank,
  Character_DPS
};

enum Chat {
  Chat_Victim,
  Chat_Attacker,
  Chat_Target,
  Chat_Spawn
};

enum Temp {
  Float:Temp_None,
  Float:Temp_DropRate,
  Float:Temp_XPRate
};

enum Profession {
  Prof_ExtraDrop,
  Prof_ExtraStats,
  Prof_ExtraDamage,
  Prof_ExtraDef,
  Prof_ExtraRegen,
  Prof_ExtraSpeed,
  Prof_EnchBonus2,
  Prof_LwBonus2,
  Prof_JcBonus2,
  Prof_TailBonus2,
  Prof_BsBonus2,
  Prof_InscrBonus,
  Prof_InscrBonus2
};

new Float:UserTemps[65][Temp];

new String:g_sEventMessage[255]="";

enum Recipe {
  Recipe_skill,
  String:Recipe_name[255],
  String:Recipe_info[20]
};

//string variables
new String:RANKING_PAGE[355], String:ACCOUNT_PAGE[355], String:WIKI_PAGE[355],
      String:ARENA_SERVER[70], String:MAIN_SERVER_ADRESS[70],
      String:ARENA_FORCE_MAP[70],
      String:COMM_NICK[25],
      String:SERVER_LEVEL_POLICY_REDIRECT[50];

//new version variable
new bool:g_newVersion;

//gold stuff
new clientGold[65];

//Health Tracker
new g_mHealth[65]; //Mod Health tracker
new g_gHealth[65]; //Game Health tracker

//Arena variables
new bool:bArenaActive=false;
new arenaTeams[65]=0;
new g_ArenaLeader;

new g_RoundCount=-2;
// effect indexes
new g_bloodModel,g_bloodsprayModel,g_ef_ocean,g_ef_airlock_laser,g_ef_lgtning,g_ef_glow1,g_ef_glow01,g_ef_tp_beam001,g_ef_old_xfire,g_ef_smoke,g_ef_hydragutbeam,g_purplelaser1,g_ef_hydragutbeampoison,
      g_ef_yellowflare;
//Totems
new ClientTracer;
new g_mdl_totem_h, g_mdl_totem_a, g_ef_flatflame;
new g_iTotems[65][10];
//Effects since 1.06
new g_ef_bluecore;
//shaman effects
new g_redglow1,g_blueglow1,g_plasmaember;
//Monk Effects
new g_ef_tpbeam001, g_ef_playerredsmall, g_ef_shellchrome, g_ef_overlay;
new g_ef_striderbulgedudvdx60, g_ef_fire01, g_ef_rollerglow;
new g_ef_blueblackflash, g_ef_xbeam2, g_ef_striderdarkflare;



//Stores sm_immunity_mode for boss event
new g_oldImmunity;
//teambalance command string
new String:TEAMBALANCE_COMMAND[100];
new String:FUN_MESSAGE[100];

//clip based on speed
new ClientClip[65][3];

//vectors
//242 - honor 243 - rank 244 - trophies
new wcUsers[65][300];
new wcUsers2[65][201];
#define USERS_FLOAT_0 30
#define USERS_FLOAT_1 30
new Float:wcUsersFloat0[65][USERS_FLOAT_0];
new Float:wcUsersFloat1[65][USERS_FLOAT_1];

// effects data
new effectsdata[300][10];

//values for reboot
new G_RebootTotal,G_RebootQueue;

new g_T2Queue,g_T2QueueTotal;
new bool:g_ServerPause=false;
new g_ServerBots;

//characters data of client
// 1 - total mails
// 2- level
// 3 - class
new charactersdata[65][16][20];

new Float:PlayerMenuOff[65];
new Float:PlayerSilence[65];

/** DEFINESS FOR BOTH wcClientsStats & BonusStats */
#define STR 3
#define AGI 4
#define INT 5
#define ARMORIGNORE 6
#define DAMAGE 7
#define MAXHP 8
#define MAXMP 9
#define MPREGEN 10
#define ARMOR 11
#define ATTACKSPEED 12
#define CURMP 13
#define STA 14
#define SPELLDAMAGE 15
#define MOVESPEED 16
#define RESISTANCE 17
#define CRITCHANCE 18
#define WEAPONSPEED 19
#define ACC 20
#define MINUSCRITCHANCE 21
#define MINUSCRITDAMAGE 22
#define RESILIENCE 23
#define HASTE 24
#define PARRY 25
//UNUSED stat numbers: 1,2,26-29 //these are unconfirmed as of yet.
//EFFECTS occupy wcClientsStats[][X]: 30->70
new Float:wcClientsStats[65][71];
new Float:wcClientsBonusStats[65][31];
/** DEFINES FOR BOTH wcClientsStats & BonusStats */

new wcEffectsVictim[65][31],wcEffectsSpellVic[65][31];
new wcEffectsAttacker[65][31],wcEffectsSpellAtt[65][31];
new wcEffectsDeath[65][31];
new wcEffectsUse[65][31];

new playerids[65],playerloaded[65],characterids[65][20];
//end of vectors
new wcHealingDone[65],wcDamageDone[65],wcDamageTaken[65],wcTotalHealing[65];
// Boss dmg/healing done min for drops
new g_iBossDropDmgMin=2000;
new g_iBossDropHealMin=500;
// absorb shields
new wcClientAbsorb[65],wcManaShield[65];
// seal store

//Magic damage bullets
new bool:bMagicDamage[65]=false;

// Stores location of client's corpse
new Float:vClientCorpse[65][3];

// Combustion
new bool:bCombustion[65];

// Parry
new Float:g_fParryReduce[65]=0.0;
new Float:g_iSpar[65][65];
new bool:g_bSpar[65]=false;


new Handle:g_BossMod;
new Handle:g_Heroic;
new Handle:g_TreasureHunt;
new Handle:g_Ctf;
// sparks run effect
new bool:wcClientSparks[65];

//professions
new ClientProfessions[65][801];

new Handle:convar_friendlyfire;

//mail read/preload variables
new ClientMailDataId[65][51],ClientMailTime[65][51],String:ClientMailTopic[65][51][50],String:ClientMailFromName[65][51][20],ClientMailNew[65][51],ClientMailQueue[65];
new ClientMailDebug[65][51];
// mail mod send variables
new Handle:g_mail_pack[65];
new g_mail_slot[65];
new g_mail_char[65];
new Float:g_mail_time[65];
new g_mail_pack_amount[65];
new G_MailTimer=1;
// end of mail mod
// auction variables
new Handle:g_auction_pack[65];
new g_auction_char[65];
new Float:g_auction_time[65];
new g_auction_pack_amount[65];
// end of auction mod
//split timer
new Float:g_split_time[65];
new g_split_item[65];
//end
// anti bubble defuse
new Defuser;
//end
// death spree xp
new g_Deaths[65];
//end

//Channeling
new Float:g_fCast[65],Float:g_fStealth[65];
new g_iCast[65];
//end

//temporary for test

//end of temporary

//Account
new g_iAccountTyping[65], Float:g_fAccountTime[65];
//end

//Anti fast weapon
//new Float:g_fLastMulti[65];
//end
new g_iLastCategory[65];
//accurency
new bool:afterShoot[65];

//Global arena vars
new bool:g_ArenaLocked=false;
new bool:g_ArenaRunning=false;
//end

//Threat
new wc_iPlayerHurtEvent;
new wc_aPlayerHurtEvent;
new bool:g_bThreat[65],g_iThreat[65];

//end
//new Handle:g_hPrimaryMenu=INVALID_HANDLE;
new Handle:TalentsDB=INVALID_HANDLE;

//sql
new Handle:TMSQL; //Menu loader handle
new tmsql=0;  //queue


// wow Include files
#include <wow_natives>
#include "wow/protobuf" //added 29/6/2013

#include "wow/declarations"
#include "wow/checker"

#include "wow/logger.inc"
#include "wow/mloader.inc"
#include "wow/offsets"
#include "wow/buy_controller.inc"
#include "wow/cast_controller.inc"
#include "wow/clock.inc"
#include "wow/admincmds"
#include "wow/fun"
#include "wow/spells"
#include "wow/vectors"
#include "wow/configuration"
#include "wow/sounds"
#include "wow/mysql"
#include "wow/wcmenus"
#include "wow/gold"
#include "wow/rank"
#include "wow/damages"
#include "wow/hooks"
#include "wow/messages"
#include "wow/systems"
#include "wow/effects"

#include "wow/eventhooks"
#include "wow/xpgaining"
#include "wow/itemFunctions"
#include "wow/updates"
#include "wow/arena"
#include "wow/tempSettings"
#include "wow/profession"
#include "wow/talents"

#include "wow/cronjob"
#include "wow/balancer.inc"
/*#include "wow/caching.inc"*/

#include "wow/dk"
#include "wow/monk"
#include "wow/treasure"

#include "wow/bg"
#include "wow/wsg"
#include "wow/bots"

#include "wow/natives"
#include "wow/ctf"

public SetCurrentVersion(const String:version[])
{
  CreateConVar("wow_version",version,"[WC] Current version of World of Warcraft plugin",FCVAR_PLUGIN|FCVAR_NOTIFY);
}

CheckHLStatsX()   {
   new Handle:plug=FindPluginByFile("hlstatsx.smx");
   if(plug!=INVALID_HANDLE)   {
      LogMessage("[WC] Reloaded HLStatsX plugin to avoid Pre_Hook issues");
      ServerCommand("sm plugins reload hlstatsx");
   }
   plug=FindPluginByFile("gameme.smx");
   if(plug!=INVALID_HANDLE)   {
      LogMessage("[WC] Reloaded GameMe plugin to avoid Pre_Hook issues");
      ServerCommand("sm plugins reload gameme");
   }
   plug=FindPluginByFile("advcommands.smx");
   if(plug!=INVALID_HANDLE)   {
      LogMessage("[WC] Reloaded Advanced Comands plugin to avoid Pre_Hook issues");
      ServerCommand("sm plugins reload advcommands");
   }
}

BlockServer()  {
   new Handle:tb=FindConVar(TEAMBALANCE_COMMAND);
   if(tb!=INVALID_HANDLE)
      SetConVarBool(tb, false);
   CloseHandle(tb);
   new Handle:quota=FindConVar("bot_quota");
   if(quota!=INVALID_HANDLE)  {
      g_ServerBots=GetConVarInt(quota);
      CloseHandle(quota);
   }
   else
      g_ServerBots=0;
   g_ServerPause=true;
   g_T2QueueTotal=g_T2Queue;
   CreateTimer(2.0,CheckBlock,0,TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}

public Action:CheckBlock(Handle:timer, any:nothing)   {
   new Handle:quota=FindConVar("bot_quota");
   if(quota!=INVALID_HANDLE)  {
      new bots = GetConVarInt(quota);
      if(bots > 0)   {
         g_ServerBots=bots;
         SetConVarInt(quota, 0);
      }
      CloseHandle(quota);
   }

   if(!g_ServerPause)   {
      return Plugin_Stop;
   }
   if(g_T2Queue<=2) {
      UnblockServer();
      for(new i=1;i<=GetMaxClients();i++)
         if(IsClientInGame(i))
            WCMessage(i,"wc db loaded",i);
      return Plugin_Stop;
   }
   new perc=100-RoundToNearest(float(g_T2Queue)/float(g_T2QueueTotal)*70.0);
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         WCMessage(i,"wc db loading",i,perc);
   return Plugin_Continue;
}

UnblockServer()   {
   new Handle:tb=FindConVar(TEAMBALANCE_COMMAND);
   if(tb!=INVALID_HANDLE)
      SetConVarBool(tb, true);
   CloseHandle(tb);
   for(new i=1;i<=GetMaxClients();i++)
      if(IsClientInGame(i))
         EmitSoundToClient(i, "quake/play.wav");
   g_ServerPause=false;
   new Handle:quota=FindConVar("bot_quota");
   if(quota!=INVALID_HANDLE)  {
      decl String:sMap[50];
      GetCurrentMap(sMap,sizeof(sMap));
      new count=StrContains(sMap,"surf_",false);

      if(count < 0) {
        SetConVarInt(quota, g_ServerBots);
      }
      CloseHandle(quota);
   }
}
