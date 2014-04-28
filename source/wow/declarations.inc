/*
* File: declarations.inc
* Description: Declarations of variables, enums, etc..
* This file is first in include queue, so it's very safe to declare variables here
* Author(s): Stinkyfax
*/

#pragma semicolon 1

#define MAX_ANTIMAGIC wcGetSpellInfoInt(8,7,"max")
#define ANTIMAGIC_REGEN wcGetSpellInfoInt(8,7,"tick")
#define MAX_TALENTS 150
#define MAX_CLASS 9
#define MAX_PROF_LEVELS 8

#define SLIDE_HOOK_E 1
#define SLIDE_HOOK_PROFS 2

#define CSGO g_csgo

new bool:g_csgo=false;

new g_respawns[65]; //Respawns counter (is better than g_RoundCount)
new iPDModules = 8; //should be 9 to include player models
new g_iFlameShock[65];
new g_iCarry[65]; //To mark a target as the carry
new gPDModules[PDModule]  = { 900, 300, 400, 200, 400, 300, 1000, 1000, 900 };
new g_iShieldStack[65]=0;

new String:sPDModules[PDModule][] = { "Basic, is required to have premium account",
  "Reserved Slot for ALL servers",
  "Kills Per Round limit is increased by 50 percent", "Dual spec talents", "Plus 35 percent xp rate",
  "Plus 25 percent drop rate",
  "Increases your DPS and Spell Power by 10 percent",
  "Increases maximum Health by 15 percent",
  "Gives Access to Unique Models by typing !models"  };
/*new String:sPDModuleNames[PDModule][] = { "Basic", "Reserved Slot", "KPR increase", "Dual Spec", "+XP", "+Drop", "+DPS, +Spell Power", "+Health", "Player Models" };*/


#define EXPANSIONS 3
enum Expansion
{
  E_DK=1,  //Death Knight
  E_CATA,   //Cataclysm
  E_MONK    // Monk Class
}
new Expansion:g_expansions[] = { E_DK, E_CATA, E_MONK};
new String:g_sExpansions[Expansion][] = { "none", "Death Knight", "Cataclysm", "Monk"};

enum Seal
{
  SEAL_Any=0,
  SEAL_Light=1, //14 +healing
  SEAL_Wisdom=2,  // 15 +mana on hit
  SEAL_Justice=3, //12 -damage taken
  SEAL_Righteousness=4, // 13 +hp on dmg taken
  SEAL_Crusader=5, //7 +ammo chance
  SEAL_Command=6, //6 chance for +dmg
  SEAL_Truth=7, //19 Chance to absorb spell damage
  SEAL_Blood=8, //20 Extra crit damage
  SEAL_Insight=9 //21 Extra crit healing
}

enum PremiumData
{
	PD_modules,
	String:PD_sExpires[50],
	bool:PD_subscribed,
	PD_costs,
  PD_daysLeft
};

enum Slide  {
  SLIDE_NONE,
  SLIDE_WELCOME,
  SLIDE_30
};

new bool:g_noRadar=false;
new Slide:g_ffSlideType[65];
new g_ffSlideStep[65];
new String:g_ffSlidePath[65][63];
new g_ffSlideHook[65];


enum CachedPlayer {
  CP_id,
  Float:CP_time,

  CP_playerId, //playerids[client]
  CP_premiumData[PremiumData],  //?
  CP_arenaPoints, //GetPlayerPoints(client)
  CP_arenaPlayed, //GetArenaPlayed(client)
  CP_threat, //SetThreatSettingsInt
  CP_honor, //SetHonor
  CP_rank, //SetRank
  CP_newbie, //SetNewbie
  CP_newbie2, //SetNewbie2
  CP_usekeyspellbar, //SetUsekeySpellbar
  CP_chatVictim, //SetChat
  CP_chatTarget,
  CP_chatAttacker,
  CP_chatSpawn,
  CP_gold, //setGold
  CP_chars[16], //characterids[client][x] 0-15
  CP_restXp, //setRestXp
  //Stopped at line 1690 mysql.inc

  CP_data[100]
}


//wow orange imported
enum QueryType
{
   NoRepeat,
   Repeat
}


enum Query
{
	Q_Id,
	Q_Var,
	QueryType:Q_Type,
	Q_Repeats,
	String:Q_Line[4000]	
};


new Handle:hSQL; //instant queries
new Handle:TSQL; // for main threaded queries
new Handle:T2SQL; // for secondary threaded queries
//new Handle:TMSQL; //Thread for menus  - is defined in wow.inc
new Handle:SSQL; // for service threaded queries
new Handle:siteSQL; //access to site database
new Handle:publicSQL; //Access to public databases

new g_iTargetOnly[64]; // For !target

new g_SlideBits[65];
new g_iCombatReady[65];
new g_iHStone[65];
new g_ef_physring1;
new g_ef_splodesprite;
new g_ef_bluelight;
new g_ef_muzzleflashX;
new g_ef_animglow;
new g_plasmabeam;
new g_bluelight1;
new g_ef_hydraspinalcord;
new g_ef_particle_noisesphere;
new g_ef_laserbeam;
new g_HaloSprite;

//Shadow Word Death
new SWD_sprite;
//Power Word Shield Sprite
new ShieldSprite;
//ChaosBolt
new ChaosSprite;

//shadow bolt
new g_ef_scanner;
//new g_ef_glow;

//Immolate
#define IMMOLATE_UNIQUE 43287
new g_ef_fire1,g_ef_fire2;

// DK BLOOD
//new HaloSprite;
/*new g_iEnvBlood;*/

enum Buff_Type  { //Is important to support smooth overrides
  BT_General, //If others don't suit
  BT_Movement,  //TODO: Handle logics inside of buffs class
  BT_AttackSpeed, //TODO: Handle logics inside of buffs class
  BT_Damage //Damaging debuff
}

enum Buff_Faction  {
  BF_Positive,
  BF_Neutral, //dunno if it will be used
  BF_Negative
}

enum BuffCallReason {
  BCR_Fire, //Fire
  BCR_SilentFire, //Fire silent
  BCR_Kill //Kill
}


enum Buff {
  //Base Info
  Buff_id,  //id in array
  Buff_caster,
  Buff_target,

  //Tick info
  Float:Buff_began, //Buff_start will set Buff_began to engine time
  Float:Buff_nextTick,  //Defines when next time buff function should be fired
  Buff_totalTicks,  //will be '1' for non-tick buffs like sprint, evade.
  Buff_ticksLeft, //during last tick it will be equal to 0 at callback call
  //Essential
  Buff_uniqueId,  //Might be needed
  Buff_Faction:Buff_faction,
  Buff_Type:Buff_type,
  String:Buff_name[50],

  //Important for a function callback, but not used by buff class functions
  String:Buff_callback[50],
  Handle:Buff_datapack,

  //Fields used in maintaining/affecting buff life
  bool:Buff_dispellable,

  //Stacking policy
  Buff_priority,
  //bool:Buff_overridable, - aren't all going to be true?
  //bool:Buff_overrides,  - all must be true as well?
  String:Buff_overrideFunction[50]

}

new Handle:g_dmgArray=INVALID_HANDLE;
new Handle:StatsDB=INVALID_HANDLE;

enum StatPoint  {
  SP_None,
  SP_Strength,
  SP_Agility,
  SP_Stamina,
  SP_Intellect
}

enum CatType  {
  CT_Unknown,
  CT_Wearable,
  CT_Recipe
}

//For threat indicator
new g_ef_train;

//pain suppression
new g_ef_rollermine;

enum UniqueItem
{
  Item_id,

  Item_defId,
  Float:Item_version,
  Item_class,
  Item_level,  //Required level to equip
  Item_ilevel, //True item level
  Item_cost,
  Item_honor,
  Item_quality,
  Item_category,
  Item_2hand,

  Item_reqstr,
  Item_reqagi,
  Item_reqsta,
  Item_reqint,

  Item_str,
  Item_agi,
  Item_sta,
  Item_int,
  Item_res,
  Item_regen,
  Item_ai,
  Item_haste,

  Item_gold,
  Item_forGold,
  Item_noTrade,

  Float:Item_as,
  Float:Item_cs,
  Float:Item_speed,
  Float:Item_acc,
  Float:Item_parry,

  Item_resilence,
  Item_armor,      //Non weapon
  Item_armor_type, //1 Cloth, 2 Leather, 3 Plate, 0 None

  //Weapon settings
  Item_weapon_ap, //Weapon attack power
  Item_weapon_sp, //Weapon spell power
  Float:Item_ws,  //Weapon speed
  Item_damage,    //Bonus attack power
  Item_spelldmg,  //Bonus spell power
  Item_effect,    //On-hit effect

  String:Item_Name[50],
  String:Item_Descr[70],

  //Unique specific
  Item_owner,
  Item_EnchBit,

  //Server specific
  Float:Item_LastUpdate,
  Float:Item_LastUsage,

  //Only for standard
  String:Item_message[50],
  Item_use,
  Item_amount,
  Item_justice,
  Item_valor
};

//DK effect models
new AMSSprite;

new g_ef_trail_blue;
new g_ef_combineball;
new g_ef_yellowflare1;
new g_ef_orangeflare1;
new g_ef_orangelight1;
new g_ef_yellowglow1;
new g_ef_WXplo1;
new g_ef_greenglow1;
new g_ef_energyball;
//new g_ef_firecloud2;
new g_mRanks[15];

//ctf
new g_ctf_horde;
new g_ctf_alliance;

//Hide&Seek
new bool:g_hns=false;
new g_hns_mode=0;
new g_hns_gold=0;
new g_hns_xp=0;
new g_hns_itemId=0;
new g_hns_itemAmount=0;

new g_BossEndXP=0;

new g_alpha[65];


new String:g_mHp[][] = { "materials/wow/hp_bar1.vmt", "materials/wow/hp_bar2.vmt", "materials/wow/hp_bar3.vmt", "materials/wow/hp_bar4.vmt", "materials/wow/hp_bar5.vmt", "materials/wow/hp_bar6.vmt" };

new g_eHp[65];



/* Some stock functions. The ones below are for "flying" objects */
stock bool:isClientConnectedIngameAlive(client){

  if(isClientConnectedIngame(client)){

    if(IsPlayerAlive(client) == true && IsClientObserver(client) == false){

      return true;

    }else{

      return false;

    }

  }else{

    return false;

  }

}
stock bool:isClientConnectedIngame(client){

  if(client > 0 && client <= MaxClients){

    if(IsClientInGame(client) == true){

      return true;

    }else{

      return false;

    }

  }else{

    return false;

  }

}

// settings for m_takedamage
#define DAMAGE_NO       0
#define DAMAGE_EVENTS_ONLY    1   // Call damage functions, but don't modify health
#define DAMAGE_YES        2
#define DAMAGE_AIM        3
stock setTakeDamage(entity, type){
  SetEntProp(entity, Prop_Data, "m_takedamage", type);

}

stock bool:IsEntityCollidable(entity, bool:includeplayer = true, bool:includehostage = true, bool:includeprojectile = true){

  decl String:classname[64];
  GetEdictClassname(entity, classname, 64);

  if((StrEqual(classname, "player", false) && includeplayer) || (StrEqual(classname, "hostage_entity", false) && includehostage)
      ||StrContains(classname, "physics", false) != -1 || StrContains(classname, "prop", false) != -1
      || StrContains(classname, "door", false)  != -1 || StrContains(classname, "weapon", false)  != -1
      || StrContains(classname, "break", false)  != -1 || ((StrContains(classname, "projectile", false)  != -1) && includeprojectile)
      || StrContains(classname, "brush", false)  != -1 || StrContains(classname, "button", false)  != -1
      || StrContains(classname, "physbox", false)  != -1 || StrContains(classname, "plat", false)  != -1
      || StrEqual(classname, "func_conveyor", false) || StrEqual(classname, "func_fish_pool", false)
      || StrEqual(classname, "func_guntarget", false) || StrEqual(classname, "func_lod", false)
      || StrEqual(classname, "func_monitor", false) || StrEqual(classname, "func_movelinear", false)
      || StrEqual(classname, "func_reflective_glass", false) || StrEqual(classname, "func_rotating", false)
      || StrEqual(classname, "func_tanktrain", false) || StrEqual(classname, "func_trackautochange", false)
      || StrEqual(classname, "func_trackchange", false) || StrEqual(classname, "func_tracktrain", false)
      || StrEqual(classname, "func_train", false) || StrEqual(classname, "func_traincontrols", false)
      || StrEqual(classname, "func_vehicleclip", false) || StrEqual(classname, "func_traincontrols", false)
      || StrEqual(classname, "func_water", false) || StrEqual(classname, "func_water_analog", false)){

    return true;

  }

  return false;

}

