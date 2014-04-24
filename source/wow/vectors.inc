/*
* File: vectors.inc
* Description: Thil file contains all the vectors/spawn/stats functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1

// Vector position defines

/*
10:backs
11:chets
12:feet
13:hands
14:helmets
15:legs
16:necks
17:rings
18:shields
19:shoulders
20:trinkets
21:waists
22:weapons
23:wrists

Name To Index

ID             1
points         2
level          3
xp             4
reqxp          5
money          6
str            7
agi            8
int            9
sta            241
backs_cat      10
backs_item     11
chests_cat     12
chests_item    13
feet_cat       14
feet_item      15
hands_cat      16
hands_item     17
helmets_cat    18
helmets_item   19
legs_cat       20
legs_item      21
necks_cat      22
necks_item     23
rings_cat      24
rings_item     25
shields_cat    26
shields_item   27
shoulders_cat  28
shoulders_item 29
trinkets_cat   30
trinkets_item  31
waists_cat     32
waists_item    33
rightweapons_cat  34
rightweapons_item 35
leftweapons_cat   36
leftweapons_item  37
wrists_cat     38
wrists_item    39


ITEM POINTS SYSTEM
total points = 10 * item level
chests = total points * 1.0
backs = total points * 0.3
feet = total points * 0.65
hands = total points * 0.5
helmets = total points * 0.8
legs = total points * 0.9
necks = total points * 0.4
rings = total points * 0.4
shields = total points * 1.0
shoulders = total poitns * 0.6
trinkets = total points * 0.7
waists = total points * 0.5
weapons = total points * 1.0
2h weapons = total points * 2.0
wrists = total points * 0.4

1 str = 4 points
1 agi = 4 points
1 int = 4 points
1 sta = 4 points
1 armor = 0.5 points
1 attack = 1.5 points
1 spell damage = 1.8 points
1% attack speed = 1.8
1 point armor ignore = 2 points


item_1_cat  40
item_1_item 41
item_i_cat = 38 + 2i
item_i_item = 39 + 2i  max 39+ 200 = 239
hero class = 240


*/



public GetPlayerID(client)
{
  return client;
  //  for(new x=1;x<=64;x++)
  //  {
  //    if(client==wcUsersID[x])
  //      return x;
  //  }
  //  return 0;
}
// formula damage reduction: 1 - (armor / (armor + ((25*level)+20))
// formula armor ignore: total agi / 3.5
// formula attack speed: 1 + agi / 130
// formula bonus damage: damage + str/2 + agi/4
// formula armor: armor + agi/(2/3)
// formula bonus spell damage: total int
// damage 1, armor 2, str - 3, agi - 4, int - 5, AI - 6, bonus damage - 7, max/spawn hp - 8, max mana - 9, mana regen - 10, total armor - 11, attack speed - 12, current mana - 13, stamina - 14, spell dmg - 15, normal speed - 16 effects from 20
// client stat 24=haste, 25=parry.


public Action:SpawnAmmo(Handle:timer, any:client)
{
  if(!IsClientConnected(client))
  return;
  if(!IsClientInGame(client))
  return;
  if(!IsPlayerAlive(client))
  return;
  new weapon = GetEntDataEnt2(client, ACTIVEWEAPON);
  ClientClip[client][2]=1;
  if(IsValidEntity(weapon))
  {
    new ammo=GetEntData(weapon, CLIPOFFSET, 4);
    if(ammo>6)
    {
      decl String:Sweapon[50];
      GetEdictClassname(weapon, Sweapon, sizeof(Sweapon));
      //weapon = weapon_xm1014
      //weapon = weapon_m3
      if((!StrEqual("weapon_xm1014",Sweapon))&&(!StrEqual("weapon_m3",Sweapon))&&(!StrEqual("weapon_c4",Sweapon)))
      {
        new Float:speed=GetAttackSpeed(client);
        if(GetDualWieldSpecc(client)>0.0)
        {
          ammo+=RoundToNearest(float(ammo)*GetDualWieldSpecc(client));
        }
        if(speed>1.0)
        ammo=RoundToNearest(float(ammo)*speed);
        SetEntData(weapon, CLIPOFFSET, ammo, 4, true);
      }
    }
  }
  return;
}

public Float:wcCalculateStats(client)
{
  new id=GetPlayerID(client);
  //Rest float arrays
  ResetFloatArrays(client);


  new effect=30;
  //remove clip bonus
  ClientClip[client][0]=0;
  ClientClip[client][1]=0;
  ClientClip[client][2]=1;
  //end
  ClearTotems(client);
  //remove magic damage
  bMagicDamage[client]=false;
  //Remove combustion
  bCombustion[client]=false;
  //giving clip boost
  CreateTimer(0.5, SpawnAmmo, client);
  //end
  //remove possible single effect bugs
  PlayerSilence[client]=0.0;
  wcClientSparks[client]=false;
  wcClientAbsorb[client]=0;
  wcManaShield[client]=0;
  SetSealT(client,0.0);
  SetSeal(client,0);
  //SetSealP(client,0);
  SetHolyPower(client,0);
  SetHMT(client,0.0);
  SetWeaponSpeed(client,0.0);
  SetCasting(client,0,0.0);
  SetStealth(client, 0.0);
  SetThreat(client, 0);
  SetDoubleHand(client, false);
  //end of single effects
  ResetPoints(client);
  spreadPoints(client);
  for (new x=1;x<=70;x++) //shouldn't this be 0-70 and be in ResetFloatArrays??
  wcClientsStats[id][x]=float(0);
  for (new x=1;x<=30;x++) //shouldn't this be 0-31 and be in ResetFloatArrays??
  wcClientsBonusStats[id][x]=float(0);
  new Float:rating=0.0;
  for (new ID=10;ID<=23;ID++)
  {
    new item[UniqueItem];
    GetCharItem(item, client, ID);
    AddItemBonuses(client, item, effect, rating);
  }
  if(IsFakeClient(client))
  BotCalculateStats(client);
  //now add rating from base stats of person
  rating+= float(GetStr(client)) * RATING_STR;
  rating+= float(GetAgi(client)) * RATING_AGI;
  rating+= float(GetInt(client)) * RATING_INT;
  rating+= float(GetSta(client)) * RATING_STA;
  rating+= float(GetLevel(client)) * RATING_LEVEL;
  //end

  if(CheckRequirements(client, 8, 2, 1, 1) && GetClass(client)==8) {
    decl String:line[255];
    wcGetTalentInfo(8,2,1,1,client,"bonus",line);
    new bonus = StringToInt(line);
    wcClientsBonusStats[id][MAXMP] += float(bonus);
  }

  if(GetWeaponSpeed(client, false) == 0.0)
  SetWeaponSpeed(client,1.0);
  wcRecalculateStats(client);
  //rating for blocking
  switch(GetClass(client))  {
  case 1: rating+= GetBlockChance(client, false) * RATING_SHIELD_WAR;
  case 4: rating+= GetBlockChance(client, false) * RATING_SHIELD_PAL;
  case 6: rating+= GetBlockChance(client, false) * RATING_SHIELD_SHA;
  }
  RemoveLevelSpells(client);
  wcInstallEffects(client);
  return rating;
}

AddItemBonuses(id, item[UniqueItem], &effect, &Float:rating)  {
  if ((item[Item_id]>0) || (item[Item_defId] > 0))
  {
    if(item[Item_2hand]==1)
    SetDoubleHand(id, true);
    new Float:dmgMulti=1.0;
    if((item[Item_damage]>7.0) && (item[Item_spelldmg]>9.0))
    dmgMulti=0.7143;
    new Float:bufferf;
    bufferf=float(item[Item_damage]);
    wcClientsBonusStats[id][DAMAGE]+=bufferf;
    rating+= bufferf * dmgMulti * RATING_DAMAGE;

    bufferf=float(item[Item_spelldmg]);
    wcClientsBonusStats[id][SPELLDAMAGE]+=bufferf;
    rating+= bufferf * dmgMulti * RATING_SPELLDMG;

    bufferf=float(item[Item_weapon_ap]);
    wcClientsBonusStats[id][DAMAGE]+=bufferf;
    rating+= bufferf * dmgMulti * RATING_DAMAGE;

    bufferf=float(item[Item_weapon_sp]);
    wcClientsBonusStats[id][SPELLDAMAGE]+=bufferf;
    rating+= bufferf * dmgMulti * RATING_SPELLDMG;

    bufferf=item[Item_as];
    wcClientsBonusStats[id][ATTACKSPEED]+=bufferf;
    rating+= bufferf * RATING_AS;

    bufferf=float(item[Item_armor]);
    wcClientsBonusStats[id][ARMOR]+=bufferf;
    rating+= bufferf * RATING_ARMOR;

    bufferf=float(item[Item_str]);
    wcClientsStats[id][STR]+=bufferf;
    rating+= bufferf * RATING_STR;

    bufferf=float(item[Item_agi]);
    wcClientsStats[id][AGI]+=bufferf;
    rating+= bufferf * RATING_AGI;

    bufferf=float(item[Item_int]);
    wcClientsStats[id][INT]+=bufferf;
    rating+= bufferf * RATING_INT;

    bufferf=float(item[Item_sta]);
    wcClientsStats[id][STA]+=bufferf;
    rating+= bufferf * RATING_STA;

    bufferf=float(item[Item_ai]);
    wcClientsBonusStats[id][ARMORIGNORE]+=bufferf;
    rating+= bufferf * RATING_AI;

    bufferf=float(item[Item_regen]);
    wcClientsBonusStats[id][MPREGEN]+=bufferf;
    rating+= bufferf * RATING_REGEN;

    bufferf=item[Item_speed];
    wcClientsBonusStats[id][MOVESPEED]+=bufferf;
    rating+= bufferf * RATING_SPEED;

    bufferf=float(item[Item_res]);
    wcClientsBonusStats[id][RESISTANCE]+=bufferf;
    rating+= bufferf * RATING_RES;

    bufferf=float(item[Item_resilence]);
    wcClientsBonusStats[id][RESILIENCE]+=bufferf;  //resilence
    rating+= bufferf * RATING_RESILENCE;

    bufferf=item[Item_acc];
    wcClientsBonusStats[id][ACC]+=bufferf;  //accuracy
    rating+= bufferf * RATING_ACC;

    bufferf=item[Item_ws];
    if(bufferf > 0.0) {
      new Float:ws2 = GetWeaponSpeed(id, false);
      if (ws2 > 0.0)
      bufferf = (bufferf + ws2) / 2.0;
      SetWeaponSpeed(id, bufferf);
    }

    bufferf=item[Item_cs]; //Critical Strike
    wcClientsBonusStats[id][CRITCHANCE]+=bufferf;
    rating+= bufferf * RATING_CS;

    bufferf=float(item[Item_haste]); //Haste
    wcClientsStats[id][HASTE]+=bufferf;
    rating+= bufferf * RATING_HASTE;

    bufferf=item[Item_parry]; //parry
    wcClientsStats[id][PARRY]+=bufferf;
    rating+= bufferf * RATING_PARRY;

    bufferf=float(item[Item_effect]);
    if (bufferf != float(0))
    {
      wcClientsStats[id][effect]=bufferf;
      rating+= RATING_EFFECT; //we don't multiply RATING_EFFECT by it's id :D
      effect++;
    }
    new id2 = item[Item_EnchBit]; //Enchant is separate item, rating will be added recursively.
    if(id2>0)  {
      new recItem[UniqueItem];
      GetItemProperties(recItem, id2);
      AddItemBonuses(id, recItem, effect, rating);
    }
  }
}

Float:GetItemRating(item[UniqueItem])  {
  new Float:rating;
  if ((item[Item_id]>0) || (item[Item_defId] > 0))
  {
    //If Enchant/Scroll, then show rating of the enchant itself.
    if(item[Item_category] == 25) {
      decl String:info[30];
      IntToString(item[Item_defId], info, sizeof(info));
      KvRewind(items);
      KvJumpToKey(items,info);
      new createid=KvGetNum(items,"itemid");
      if(createid>0)  {
        GetItemProperties(item, createid);
      }
    }

    new Float:dmgMulti=1.0;
    if((item[Item_damage]>7.0) && (item[Item_spelldmg]>9.0))
    dmgMulti=0.7143;

    new Float:bufferf;
    bufferf=float(item[Item_damage]);
    rating+= bufferf * dmgMulti * RATING_DAMAGE;

    bufferf=float(item[Item_armor]);
    rating+= bufferf * RATING_ARMOR;

    bufferf=float(item[Item_str]);
    rating+= bufferf * RATING_STR;

    bufferf=float(item[Item_agi]);
    rating+= bufferf * RATING_AGI;

    bufferf=float(item[Item_int]);
    rating+= bufferf * RATING_INT;

    bufferf=float(item[Item_sta]);
    rating+= bufferf * RATING_STA;

    bufferf=float(item[Item_ai]);
    rating+= bufferf * RATING_AI;

    bufferf=float(item[Item_regen]);
    rating+= bufferf * RATING_REGEN;

    bufferf=item[Item_as];
    rating+= bufferf * RATING_AS;

    bufferf=float(item[Item_spelldmg]);
    rating+= bufferf * dmgMulti * RATING_SPELLDMG;

    bufferf=item[Item_speed];
    rating+= bufferf * RATING_SPEED;

    bufferf=float(item[Item_res]);
    rating+= bufferf * RATING_RES;

    bufferf=float(item[Item_resilence]);
    rating+= bufferf * RATING_RESILENCE;

    bufferf=item[Item_acc];
    rating+= bufferf * RATING_ACC;

    bufferf=item[Item_cs]; //Critical Strike
    rating+= bufferf * RATING_CS;

    bufferf=float(item[Item_haste]); //Haste
    rating+= bufferf * RATING_HASTE;

    bufferf=item[Item_parry]; //parry
    rating+= bufferf * RATING_PARRY;

    bufferf=float(item[Item_weapon_ap]); //weapon_ap
    rating+= bufferf * RATING_DAMAGE;

    bufferf=float(item[Item_weapon_sp]); //weapon_sp
    rating+= bufferf * RATING_SPELLDMG;

    bufferf=float(item[Item_effect]);
    if (bufferf != float(0))
    {
      rating+= RATING_EFFECT; //we don't multiply RATING_EFFECT by it's id :D
    }

    new id2 = item[Item_EnchBit]; //enchant is separate item, rating will be added recursively.
    if(id2>0)  {
      new recItem[UniqueItem];
      GetItemProperties(recItem, id2);
      rating += GetItemRating(recItem);
    }

  }
  return rating;
}

public wcRecalculateStats(client) {
  new id=GetPlayerID(client);
  if(IsZombie(client))
  {
    RecalculatePetStats(client);
    return;
  }

  wcClientsStats[id][ARMORIGNORE]=(float(GetBonusAgi(client))+float(GetAgi(client)))/4.5+wcClientsBonusStats[id][ARMORIGNORE];
  wcClientsStats[id][ATTACKSPEED]=GetWeaponSpeed(client, false)-1+(float(1)+(float(GetAgi(client)+GetBonusAgi(client)))/(30.0 * float(GetLevel(client)) + 22.0)+wcClientsBonusStats[id][ATTACKSPEED]);
  if(GetStance(client)==3) //Unholy DK
  wcClientsStats[id][ATTACKSPEED]+=0.1;

  if(CheckRequirements(client,2,2,2,4))
  {
    decl String:line[255];
    wcGetTalentInfo(2,2,2,4,client,"increase",line);
    new Float:multi = 1.0 + StringToFloat(line);
    wcClientsStats[id][DAMAGE]=(float(GetStr(client)+GetBonusStr(client))*0.29714+wcClientsBonusStats[id][DAMAGE])+(float(GetAgi(client)+GetBonusAgi(client))*(0.08335*multi));
  }
  else
    wcClientsStats[id][DAMAGE]=(float(GetStr(client)+GetBonusStr(client))*0.29714+wcClientsBonusStats[id][DAMAGE])+float(GetAgi(client)+GetBonusAgi(client))*0.08335;

  wcClientsStats[id][MAXHP]=150.0+(float(GetSta(client))+float(GetBonusSta(client)))*0.95+float(GetLevel(client))*1.2+wcClientsBonusStats[id][MAXHP];
  if(IsHeroic())
  wcClientsStats[id][MAXHP]+=150.0;
  if(GetStance(client) == 1) //Blood DK
  wcClientsStats[id][MAXHP] *= 1.08;

  if(GetClass(client)!= 1 && GetClass(client)!= 2 && GetClass(client)!= 8 && GetClass(client) != 9)
  wcClientsStats[id][MAXMP]=30.0+float(GetLevel(client))*1.3+float(GetInt(client)+GetBonusInt(client))*5.0+wcClientsBonusStats[id][MAXMP];
  else
  wcClientsStats[id][MAXMP]=100.0+wcClientsBonusStats[id][MAXMP];


  wcClientsStats[id][MPREGEN]=1.0+float(GetInt(client)+GetBonusInt(client))/15.0+wcClientsBonusStats[id][MPREGEN];
  wcClientsStats[id][ARMOR]=float(GetAgi(client)+GetBonusAgi(client))/3+wcClientsBonusStats[id][ARMOR];
  if(GetStance(client) == 1) //Blood DK
  wcClientsStats[id][ARMOR] *= 1.2;

  if(GetClass(client) == 9)
    wcClientsStats[id][SPELLDAMAGE]=float(GetAgi(client)+GetBonusAgi(client))/5.0+wcClientsBonusStats[id][SPELLDAMAGE];
  else
    wcClientsStats[id][SPELLDAMAGE]=float(GetInt(client)+GetBonusInt(client))/5.0+wcClientsBonusStats[id][SPELLDAMAGE];
  //MS
  wcClientsStats[id][MOVESPEED]=1.0+float(GetAgi(client)+GetBonusAgi(client))/(32.625 * float(GetLevel(client)) + 150.0)+wcClientsBonusStats[id][MOVESPEED];
  if(GetStance(client)==3) //Unholy DK
  wcClientsStats[id][MOVESPEED]+=0.1;

  //resistance
  wcClientsStats[id][RESISTANCE]= float(GetLevel(client))*0.125 + wcClientsBonusStats[id][RESISTANCE];
  //cs
  wcClientsStats[id][CRITCHANCE]=wcClientsBonusStats[id][CRITCHANCE]+0.05+float(GetAgi(client)+GetBonusAgi(client))/( 0.125 * float(GetLevel(client)) + 0.8)/100.0 + float(GetInt(client)+GetBonusInt(client))/(0.25 * float(GetLevel ( client )) +  3.0) / 100.0;
  wcClientsStats[id][PARRY] = 0.05 + wcClientsBonusStats[id][PARRY];
  //resilence
  wcClientsStats[id][RESILIENCE] = wcClientsBonusStats[id][RESILIENCE];
  //accuracy
  wcClientsStats[id][ACC] = wcClientsBonusStats[id][ACC] + float(GetAgi(client)+GetBonusAgi(client)) / (10.0 + 0.188 * float(GetLevel(client))) / 100.0;
  //-crit chance
  wcClientsStats[id][MINUSCRITCHANCE] = (wcClientsStats[id][RESILIENCE] / 14.2857) * 0.06 + MinusCritChanceBonus(id);
  // -crit damage
  wcClientsStats[id][MINUSCRITDAMAGE] = (wcClientsStats[id][RESILIENCE] / 25.0) * 0.06 + MinusCritDamageBonus(id);
}

RecalculatePetStats(id) {
  //Default pet stats
  wcClientsStats[id][ARMORIGNORE]=     0.0;  //Armor Ignore
  wcClientsStats[id][DAMAGE]=        100.0;  //DPS
  wcClientsStats[id][MAXHP]=         200.0;  //Max HP
  wcClientsStats[id][MAXMP]=         100.0;  //Max Mana
  wcClientsStats[id][MPREGEN]=         0.0;  //Mana Regen
  wcClientsStats[id][ARMOR]=           250.0;  //Armor
  wcClientsStats[id][ATTACKSPEED]=     1.0;  //Attack Speed
  wcClientsStats[id][SPELLDAMAGE]=     0.0;  //Spell Dmg
  wcClientsStats[id][MOVESPEED]=       1.0;  //Movement Speed
  wcClientsStats[id][RESISTANCE]=      0.0;  //Resistance
  wcClientsStats[id][CRITCHANCE]=      0.25; //Critical Hit Chance
  wcClientsStats[id][ACC]=             0.0;  //Accuracy
  wcClientsStats[id][MINUSCRITCHANCE]= 0.0;  //-crit chance
  wcClientsStats[id][MINUSCRITDAMAGE]= 0.0;  //-crit damage
  wcClientsStats[id][RESILIENCE]=      0.0;  //Resilence
  new owner = GetZombie(id);

  if(_getZombieType(id)==1) //DK Zombie Pet
  {
    wcClientsStats[id][DAMAGE]+=50.0; //DPS
    wcClientsStats[id][MAXHP]+=150+GetLevel(owner)*2.5;  //Max HP
    wcClientsStats[id][ATTACKSPEED]+=0.25;  //Attack Speed
    wcClientsStats[id][MOVESPEED]+=0.15; //Movement Speed

    if(GetClass(owner)==8 && wcGetTalentLevel(owner,3,2,2)>0) {
      wcClientsStats[id][ATTACKSPEED]+= 0.25; //Attack Speed
      wcClientsStats[id][MOVESPEED]+= 0.15; //Movement Speed
    }
    if(GetClass(owner)==8 && wcGetTalentLevel(owner,3,4,2)>0) {
      wcClientsStats[id][ARMORIGNORE]+=RoundToNearest(float(GetIgnoreArmor(owner)+GetBonusIgnoreArmor(owner))*2.0);
      wcClientsStats[id][MAXHP]+=RoundToNearest(float(GetSta(owner)+GetBonusSta(owner))*2.25);
      wcClientsStats[id][RESISTANCE]+=RoundToNearest(float(GetResistance(owner))*3.25);
      new color3[4] = { 120, 255, 70, 255 };
      SetClientColor(id, color3);
    }
  }
  else if(_getZombieType(id) == 2) //Warlock Infernal Pet
  {
    if(wcGetTalentLevel(owner,2,1,3)>0) {
      decl String:line[255];
      wcGetTalentInfo(7,2,1,3,owner,"multi",line);
      new Float:talentbonus=StringToFloat(line);
      new bonus=RoundToNearest(float(170)*talentbonus);
      wcClientsStats[id][ARMOR]+=bonus;
    }
    wcClientsStats[id][DAMAGE]+=150.0; //DPS
    wcClientsStats[id][MAXHP]+=400.0 + GetLevel(owner)*6.0; //Max HP
    if(wcGetTalentLevel(owner,2,2,3)>0) {
      decl String:line[255];
      wcGetTalentInfo(7,2,2,3,owner,"multi",line);
      new Float:talentbonus=StringToFloat(line);
      new bonus=RoundToNearest(float(910)*talentbonus);
      wcClientsStats[id][MAXHP]+=bonus;
    }
    wcClientsStats[id][ARMOR]+=GetLevel(owner)*10.0; //Armor
    wcClientsStats[id][ATTACKSPEED]+=0.15;   //Attack Speed
    wcClientsStats[id][MOVESPEED]-=0.15;  //Movement Speed
  }
  if(IsHeroic())
  {
    wcClientsStats[id][DAMAGE]*=2.0;  //DPS
    wcClientsStats[id][MAXHP]*=4.0;  //Max HP
    wcClientsStats[id][MOVESPEED]+=0.25; //Movement Speed
  }
}

public Float:GetNormalSpeed(client) //movespeed without additional effects
{
  return wcClientsStats[GetPlayerID(client)][MOVESPEED];
}

public SetNormalSpeed(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][MOVESPEED]=amount;
}

public Float:GetNormalBonusSpeed(client)
{
  return wcClientsBonusStats[GetPlayerID(client)][MOVESPEED];
}

public SetNormalBonusSpeed(client,Float:amount)
{
  wcClientsBonusStats[GetPlayerID(client)][MOVESPEED]=amount;
}

stock Float:GetWeaponSpeed(client, bool:safe=false) {
  if( (wcClientsBonusStats[client][WEAPONSPEED]==0.0) && safe )
  return 1.0;
  return wcClientsBonusStats[client][WEAPONSPEED];
}

public Float:SetWeaponSpeed(client,Float:speed) {
  wcClientsBonusStats[client][WEAPONSPEED] = speed;
}

public GetBonusStr(client)
{
  return RoundToNearest(wcClientsStats[GetPlayerID(client)][STR]);
}

public GetBonusAgi(client)
{
  return  RoundToNearest(wcClientsStats[GetPlayerID(client)][AGI]);
}

public GetBonusInt(client)
{
  return  RoundToNearest(wcClientsStats[GetPlayerID(client)][INT]);
}

public GetBonusSta(client)
{
  return  RoundToNearest(wcClientsStats[GetPlayerID(client)][STA]);
}

public SetBonusStr(client,amount)
{
  wcClientsStats[GetPlayerID(client)][STR]=float(amount);
}

public SetBonusAgi(client,amount)
{
  wcClientsStats[GetPlayerID(client)][AGI]=float(amount);
}

public SetBonusInt(client,amount)
{
  wcClientsStats[GetPlayerID(client)][INT]=float(amount);
}

public SetBonusSta(client,amount)
{
  wcClientsStats[GetPlayerID(client)][STA]=float(amount);
}

public GetMaxHealth(client)
{
  new Float:hp = wcClientsStats[GetPlayerID(client)][MAXHP];
  if(wow_hasPremiumModule(client, PDModule:PDM_hp))
  hp*=1.05;
  return RoundToNearest(hp);
}

public GetBonusMaxHealth(client)
{
  return RoundToNearest(wcClientsBonusStats[GetPlayerID(client)][MAXHP]);
}

public SetBonusMaxHealth(client,amount)
{
  wcClientsBonusStats[GetPlayerID(client)][MAXHP]=float(amount);
}

public GetDamage(client)
{
  new dmg = RoundToNearest(wcClientsStats[GetPlayerID(client)][DAMAGE]);
  /*if(wow_hasPremiumModule(client, PDModule:PDM_dps))*/
  /*dmg = RoundToNearest(wcClientsStats[GetPlayerID(client)][DAMAGE]*1.1);*/
  return dmg;
}

public GetBonusDamage(client)
{
  return RoundToNearest(wcClientsBonusStats[GetPlayerID(client)][DAMAGE]);
}

public SetBonusDamage(client,Float:amount)
{
  wcClientsBonusStats[GetPlayerID(client)][DAMAGE]=amount;
}

public SetDamage(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][DAMAGE]=amount;
}

public GetSpellDmg(client,Float:multiplier)
{
  new Float:dmg = (wcClientsStats[GetPlayerID(client)][SPELLDAMAGE])*multiplier;
  /*if(wow_hasPremiumModule(client, PDModule:PDM_dps))*/
  /*dmg *= 1.1;*/
  return RoundToNearest(dmg);
}

public SetSpellDmg(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][SPELLDAMAGE]=amount;
}

public GetBonusSpellDmg(client,Float:multiplier)
{
  return RoundToNearest(wcClientsBonusStats[GetPlayerID(client)][SPELLDAMAGE]*multiplier);
}

public SetBonusSpellDmg(client,Float:amount)
{
  wcClientsBonusStats[GetPlayerID(client)][SPELLDAMAGE]=amount;
}

public GetArmor(client)
{
  return RoundToNearest(wcClientsStats[GetPlayerID(client)][ARMOR]);
}

public SetArmor(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][ARMOR]=amount;
}

public GetResistance(client)
{
  return RoundToNearest(wcClientsStats[GetPlayerID(client)][RESISTANCE]);
}

public AddResistance(client,Float:amount)
{
  wcClientsBonusStats[client][RESISTANCE] += amount;
}

public GetResilence(client)
{
  return RoundToNearest(wcClientsStats[GetPlayerID(client)][RESILIENCE]);
}

public AddResilence(client,Float:amount)
{
  wcClientsBonusStats[GetPlayerID(client)][RESILIENCE]+=amount;
}

public Float:GetAccuracy(client)
{
  if(wcClientsStats[GetPlayerID(client)][ACC]>1.0)
  return 1.0;
  return wcClientsStats[GetPlayerID(client)][ACC];
}

public SetBonusAccuracy(client, Float:amount)   {
  wcClientsBonusStats[client][ACC]=amount;
}

public Float:GetBonusAccuracy(client)
{
  if(wcClientsBonusStats[GetPlayerID(client)][ACC]>1.0)
  return 1.0;
  return wcClientsBonusStats[GetPlayerID(client)][ACC];
}

public SetAccuracy(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][ACC]=amount;
}

public Float:GetMinusCritChance(client)
{
  return wcClientsStats[GetPlayerID(client)][MINUSCRITCHANCE];
}

public SetMinusCritChance(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][MINUSCRITCHANCE]=amount;
}
// ff - for function
new Float:g_ff_minus_crit_bonus[65][2];
stock SetMinusCritChanceBonus(client, Float:val) {
  g_ff_minus_crit_bonus[client][0]=val;
}
stock SetMinusCritDamageBonus(client, Float:val) {
  g_ff_minus_crit_bonus[client][1]=val;
}

Float:MinusCritChanceBonus(client)  {
  return g_ff_minus_crit_bonus[client][0];
}

Float:MinusCritDamageBonus(client)  {
  return g_ff_minus_crit_bonus[client][1];
}

public Float:GetMinusCrit(client)
{
  return wcClientsStats[GetPlayerID(client)][MINUSCRITDAMAGE];
}

public SetMinusCrit(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][MINUSCRITDAMAGE]=amount;
}

public Float:GetCriticalStrike(client)
{
  if(GetBoss(client)>0)
  return 0.08;
  return wcClientsStats[GetPlayerID(client)][CRITCHANCE];
}

public SetCriticalStrike(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][CRITCHANCE]=amount;
}

public Float:GetCriticalStrikeBonus(client)
{
  return wcClientsBonusStats[GetPlayerID(client)][CRITCHANCE];
}

public SetCriticalStrikeBonus(client,Float:amount)
{
  wcClientsBonusStats[GetPlayerID(client)][CRITCHANCE]=amount;
}

public GetBonusArmor(client)
{
  return RoundToNearest(wcClientsBonusStats[GetPlayerID(client)][ARMOR]);
}

public SetBonusArmor(client,Float:amount)
{
  wcClientsBonusStats[GetPlayerID(client)][ARMOR]=amount;
}

public GetIgnoreArmor(client) //armor penetration
{
  return RoundToNearest(wcClientsStats[GetPlayerID(client)][ARMORIGNORE]);
}

public SetIgnoreArmor(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][ARMORIGNORE]=amount;
}

public GetBonusIgnoreArmor(client)
{
  return RoundToNearest(wcClientsBonusStats[GetPlayerID(client)][ARMORIGNORE]);
}

public SetBonusIgnoreArmor(client,Float:amount)
{
  wcClientsBonusStats[GetPlayerID(client)][ARMORIGNORE]=amount;
}

public Float:GetAttackSpeed(client)
{
  return wcClientsStats[GetPlayerID(client)][ATTACKSPEED];
}

public SetAttackSpeed(client,Float:amount)
{
  wcClientsStats[GetPlayerID(client)][ATTACKSPEED]=amount;
}

public Float:GetBonusAttackSpeed(client)
{
  return wcClientsBonusStats[GetPlayerID(client)][ATTACKSPEED];
}

public SetBonusAttackSpeed(client,Float:amount)
{
  wcClientsBonusStats[GetPlayerID(client)][ATTACKSPEED]=amount;
}

public GetMaxMana(client)
{
  return RoundToNearest(wcClientsStats[GetPlayerID(client)][MAXMP]);
}

public SetMaxMana(client,amount)
{
  wcClientsStats[GetPlayerID(client)][MAXMP]=float(amount);
}

public GetBonusMaxMana(client)
{
  return RoundToNearest(wcClientsBonusStats[GetPlayerID(client)][MAXMP]);
}

public SetBonusMaxMana(client,amount)
{
  wcClientsBonusStats[GetPlayerID(client)][MAXMP]=float(amount);
}

public GetMana(client)
{
  return RoundToNearest(wcClientsStats[GetPlayerID(client)][CURMP]);
}

public wcSetMana(client,amount)
{
  if(CheckRequirements(client,8,2,1,4))
  wcRunTalent_8_PowerOfFrost(client,amount);
  if((GetMaxMana(client) > 0) && (GetMaxMana(client) < amount))
  amount = GetMaxMana(client);
  wcClientsStats[GetPlayerID(client)][CURMP]=float(amount);
}

AddMana(client, amount) {
  if(GetStance(client)==2)
  amount = RoundToNearest(float(amount)*1.2);
  amount += GetMana(client);
  amount = GetMinInt(amount, GetMaxMana(client));
  wcSetMana(client, amount);
}

RestoreManaP(client,Float:p) {
  new max=GetMaxMana(client);
  new restore = RoundToCeil(float(max)*p);
  new old = GetMana(client);
  restore+=old;
  if(restore>GetMaxMana(client))
  restore=GetMaxMana(client);
  wcSetMana(client,restore);
  return restore-old;
}

stock RestoreHealthP(client,Float:p, bool:bRestore=true) {
  new max=GetMaxHealth(client);
  new restore = RoundToCeil(float(max)*p);
  restore+=wcGetHealth(client);
  if(restore>GetMaxHealth(client))
  restore=GetMaxHealth(client);
  if(restore < 1)
  restore=1;
  new restored = restore - wcGetHealth(client);
  if(bRestore)
  wcSetHealth(client,restore);
  return restored;
}

stock RestoreHealth(client, hp, bool:bRestore=true) {
  new restore = hp;
  restore+=wcGetHealth(client);
  if(restore>GetMaxHealth(client))
  restore=GetMaxHealth(client);
  if(restore < 1)
  restore=1;
  new restored = restore - wcGetHealth(client);
  if(bRestore)
  wcSetHealth(client,restore);
  return restored;
}

Float:GetFloatHealth(client)  {
  new max=GetMaxHealth(client);
  new hp=wcGetHealth(client);
  new Float:value=float(hp)/float(max);
  return value;
}

public GetManaReg(client)
{
  return RoundToNearest(wcClientsStats[GetPlayerID(client)][MPREGEN]);
}

public SetManaReg(client,amount)
{
  wcClientsStats[GetPlayerID(client)][MPREGEN]=float(amount);
}

stock SetBonusManaReg(client, amount) {
  wcClientsBonusStats[client][MPREGEN]=float(amount);
}

stock GetBonusManaReg(client) {
  return RoundToNearest(wcClientsBonusStats[client][MPREGEN]);
}
//effects= 30->70 (70=highest index in wcClientsStats)
public GetEffectsMax(client)
{
  new x=30;
  while (wcClientsStats[GetPlayerID(client)][x]!=float(0))
  x++;
  x--;
  if (x==29)
  return 0;
  return x;
}

public GetEffect(client,id)
{
  return RoundToNearest(wcClientsStats[GetPlayerID(client)][id]);
}

public GetFreeEffectVictim(client)
{
  new id=GetPlayerID(client);
  for (new i=1;i<=30;i++)
  if (wcEffectsVictim[id][i]==0)
  return i;
  return 0;
}

public GetFreeEffectSpellVic(client)
{
  new id=GetPlayerID(client);
  for (new i=1;i<=30;i++)
  if (wcEffectsSpellVic[id][i]==0)
  return i;
  return 0;
}

public GetFreeEffectAttacker(client)
{
  new id=GetPlayerID(client);
  for (new i=1;i<=30;i++)
  if (wcEffectsAttacker[id][i]==0)
  return i;
  return 0;
}

public GetFreeEffectSpellAtt(client)
{
  new id=GetPlayerID(client);
  for (new i=1;i<=30;i++)
  if (wcEffectsSpellAtt[id][i]==0)
  return i;
  return 0;
}

public GetFreeEffectDeath(client)
{
  new id=GetPlayerID(client);
  for (new i=1;i<=30;i++)
  if (wcEffectsDeath[id][i]==0)
  return i;
  return 0;
}

public SetEffectVictim(client,effid)
{
  new id=GetPlayerID(client);
  new free=GetFreeEffectVictim(client);
  wcEffectsVictim[id][free]=effid;
  return free;
}

public SetEffectSpellVic(client,effid)
{
  new id=GetPlayerID(client);
  new free=GetFreeEffectSpellVic(client);
  wcEffectsSpellVic[id][free]=effid;
  return free;
}

public SetEffectSpellAtt(client,effid)
{
  new id=GetPlayerID(client);
  new free=GetFreeEffectSpellAtt(client);
  wcEffectsSpellAtt[id][free]=effid;
  return free;
}

public SetEffectAttacker(client,effid)
{
  new id=GetPlayerID(client);
  new free=GetFreeEffectAttacker(client);
  wcEffectsAttacker[id][free]=effid;
  return free;
}

public SetEffectDeath(client,effid)
{
  new id=GetPlayerID(client);
  new free=GetFreeEffectDeath(client);
  wcEffectsDeath[id][free]=effid;
  return free;
}

stock bool:HasEffectVictim(client, effid)  {
  for(new x=1;x<=30;x++)
  if(wcEffectsVictim[GetPlayerID(client)][x]==effid)
  {
    return true;
  }
  return false;
}

public RemoveEffectVictim(client,effid)
{
  for(new x=1;x<=30;x++)
  if(wcEffectsVictim[GetPlayerID(client)][x]==effid)
  {
    wcEffectsVictim[GetPlayerID(client)][x]=0;
    return true;
  }
  return false;
}

public RemoveEffectSpellVic(client,effid)
{
  for(new x=1;x<=30;x++)
  if(wcEffectsSpellVic[GetPlayerID(client)][x]==effid)
  {
    wcEffectsSpellVic[GetPlayerID(client)][x]=0;
    return true;
  }
  return false;
}

public RemoveEffectSpellAtt(client,effid)
{
  for(new x=1;x<=30;x++)
  if(wcEffectsSpellAtt[GetPlayerID(client)][x]==effid)
  {
    wcEffectsSpellAtt[GetPlayerID(client)][x]=0;
    return true;
  }
  return false;
}

public RemoveEffectAttacker(client,effid)
{
  for(new x=1;x<=30;x++)
  if(wcEffectsAttacker[GetPlayerID(client)][x]==effid)
  {
    wcEffectsAttacker[GetPlayerID(client)][x]=0;
    return true;
  }
  return false;
}

public RemoveEffectDeath(client,effid)
{
  for(new x=1;x<=30;x++)
  if(wcEffectsDeath[GetPlayerID(client)][x]==effid)
  {
    wcEffectsDeath[GetPlayerID(client)][x]=0;
    return true;
  }
  return false;
}

public wcInstallEffects(client)
{
  new aa=1;
  new va=1;
  new da=1;
  new ua=1;
  new sv=1;
  new sa=1;
  new id=GetPlayerID(client);
  for (new i=1;i<=30;i++)
  {
    wcEffectsVictim[id][i]=0;
    wcEffectsAttacker[id][i]=0;
    wcEffectsSpellVic[id][i]=0;
    wcEffectsSpellAtt[id][i]=0;
    wcEffectsDeath[id][i]=0;
    wcEffectsUse[id][i]=0;
  }
  new y=GetEffectsMax(client);
  if (y!=0)
  for (new x=30;x<=y;x++)
  {
    new EffectId=GetEffect(client,x);
    new String:event[100];
    GetEffectEvent(EffectId,event);
    new bool:found=false;
    if(StrContains(event, "Victim", false) >= 0)  {

      wcEffectsVictim[id][va++]=EffectId;
      found=true;
    }
    if(StrContains(event, "Attacker", false) >= 0)  {

      wcEffectsAttacker[id][aa++]=EffectId;
      found=true;
    }
    if(StrContains(event, "Death", false) >= 0)  {

      wcEffectsDeath[id][da++]=EffectId;
      found=true;
    }
    if(StrContains(event, "SpellVic", false) >= 0)  {

      wcEffectsSpellVic[id][sv++]=EffectId;
      found=true;
    }
    if(StrContains(event, "SpellAtt", false) >= 0)  {

      wcEffectsSpellAtt[id][sa++]=EffectId;
      found=true;
    }
    if(StrContains(event, "Use", false) >= 0)  {

      wcEffectsUse[id][ua++]=EffectId;
      found=true;
    }
    if(!found)  {
      LogError("WC ERROR: could not find effect event match id = %i event = %s", EffectId, event);
    }
  }
}

public GetEffectEvent(id,String:event[100])
{
  KvRewind(effectsdb);
  decl String:ID[5];
  IntToString(id,ID,sizeof(ID));
  KvJumpToKey(effectsdb,ID);
  KvGetString(effectsdb,"effectevent",event,sizeof(event));
}

bool:TeleportPlayerView(client,Float:distance,Float:end[3]) {
  if(client>0)   if(IsPlayerAlive(client))  {
    new Float:angle[3];
    GetClientEyeAngles(client,angle);
    new Float:endpos[3];
    new Float:startpos[3];
    GetClientEyePosition(client,startpos);
    new Float:dir[3];
    GetAngleVectors(angle, dir, NULL_VECTOR, NULL_VECTOR);
    ScaleVector(dir, distance);
    AddVectors(startpos, dir, endpos);
    ClientTracer=client;
    //TR_TraceRayFilter(startpos,endpos,MASK_PLAYERSOLID,RayType_EndPoint,AimTargetFilter);
    TR_TraceRayFilter(startpos,endpos,MASK_ALL,RayType_EndPoint,AimTargetFilter);
    TR_GetEndPosition(endpos);
    distance = (GetVectorDistance(startpos, endpos) - 55.0);
    endpos[1] = (startpos[1] + (distance * Sine(DegToRad(angle[1]))));
    endpos[0] = (startpos[0] + (distance * Cosine(DegToRad(angle[1]))));
    //endpos[2]-=40;
    if(!CheckPlayerBox(endpos,startpos,client))
    return false;
    TeleportEntity(client, endpos, NULL_VECTOR, NULL_VECTOR);
    end=endpos;
    return true;
  }
  return false;
}

bool:CheckPlayerBox(Float:bottom[3],Float:start[3],client)  {
  new Float:edge[3][8];
  //point A
  edge[2][0]=bottom[2];
  edge[0][0]=bottom[0]+12.5;
  edge[1][0]=bottom[1]+12.5;
  //point B
  edge[2][1]=edge[2][0];
  edge[0][1]=edge[0][0]+25.0;
  edge[1][1]=edge[1][0];
  //point C
  edge[2][2]=edge[2][1];
  edge[0][2]=edge[0][1];
  edge[1][2]=edge[1][1]+25.0;
  //point D
  edge[2][3]=edge[2][2];
  edge[0][3]=edge[0][2]-25.0;
  edge[1][3]=edge[1][2];
  //other buttons
  for(new i=0;i<4;i++) {
    for(new x=0;x<2;x++) {
      edge[x][i+4]=edge[x][i];
    }
    edge[2][i+4]=edge[2][i]+67.0;
  }
  for(new i=0;i<4;i++) {
    decl Float:point[3];
    point[0]=edge[0][i];
    point[1]=edge[1][i];
    point[2]=edge[2][i];
    decl Float:endpoint[3];
    endpoint[0]=edge[0][i+4];
    endpoint[1]=edge[1][i+4];
    endpoint[2]=edge[2][i+4];
    TR_TraceRayFilter(point,endpoint,MASK_ALL,RayType_EndPoint,AimTargetFilter);
    //TR_TraceRayFilter(point,endpoint,MASK_PLAYERSOLID,RayType_EndPoint,AimTargetFilter);
    if(TR_DidHit()) {
      return false;
    }
  }
  for(new i=0;i<7;i++) {
    decl Float:point[3];
    point[0]=edge[0][i];
    point[1]=edge[1][i];
    point[2]=edge[2][i];
    decl Float:endpoint[3];
    endpoint[0]=edge[0][i+1];
    endpoint[1]=edge[1][i+1];
    endpoint[2]=edge[2][i+1];
    TR_TraceRayFilter(point,endpoint,MASK_ALL,RayType_EndPoint,AimTargetFilter);
    if(TR_DidHit()) {
      return false;
    }
  }
  new Float:top[3];
  top=bottom;
  top[2]+=90;
  ClientTracer=client;
  TR_TraceRayFilter(top,bottom,MASK_ALL,RayType_EndPoint,AimTargetFilter);
  new bool:temp;
  temp=TR_DidHit();
  if(temp)
  {
    return false;
  }
  ClientTracer=client;
  TR_TraceRayFilter(start,top,MASK_ALL,RayType_EndPoint,AimTargetFilter);
  temp=TR_DidHit();
  if(temp)
  {
    return false;
  }
  return true;
}


stock bool:PushPlayerTowardsPlayer(client, player, Float:power, Float:zModifier=0.0) {
  if(client <= 0 || client > GetMaxClients() || (!IsPlayerAlive(client)) )
  return false;
  new Float:clientLoc[3],Float:targetLoc[3];
  GetClientAbsOrigin(player,targetLoc);
  GetClientAbsOrigin(client, clientLoc);
  clientLoc[2]=targetLoc[2];
  targetLoc[2] += zModifier;
  new Float:velocity[3];
  GetForwardPushVec(clientLoc, targetLoc, velocity, power);
  TeleportEntity(client,NULL_VECTOR,NULL_VECTOR,velocity);
  return true;
}

stock bool:PushPlayerAwayPlayer(client, player, Float:power, Float:zModifier=0.0) {
  if(client <= 0 || client > GetMaxClients() || (!IsPlayerAlive(client)) )
  return false;
  new Float:clientLoc[3],Float:targetLoc[3];
  GetClientAbsOrigin(player,targetLoc);
  GetClientAbsOrigin(client, clientLoc);
  clientLoc[2]=targetLoc[2];
  targetLoc[2] += zModifier;
  new Float:velocity[3];
  GetForwardPushVec(clientLoc, targetLoc, velocity, power);
  velocity[0]*=-1;
  velocity[1]*=-1;
  TeleportEntity(client,NULL_VECTOR,NULL_VECTOR,velocity);
  return true;
}

stock PushPlayerView(client,Float:power, Float:zModifier=0.0)
{
  if(client>0)
  {
    if(IsPlayerAlive(client))
    {
      PrintToServer("power: %.2f Z %.2f", power, zModifier);
      new Float:velocity[3],Float:clientloc[3],Float:clientend[3];

      GetViewPoint(client, power, clientend);

      GetClientAbsOrigin(client,clientloc); // Get the position of the player's eyes

      clientend[2] = clientloc[2];

      GetForwardPushVec(clientloc,clientend,velocity,power); // Get how hard and where to push the client

      velocity[2]+=zModifier;
      TeleportEntity(client,NULL_VECTOR,NULL_VECTOR,velocity); // Push the client
    }
  }
}

public GetForwardPushVec(const Float:start[3],const Float:end[3],Float:output[3],Float:power)
{
  new Float:distance=GetDistanceBetween(start,end);
  CreateVectorFromPoints(start,end,output);
  output[0]*=power/distance;
  output[1]*=power/distance;
  output[2]*=power/distance;
}

public Float:CreateVectorFromPoints(const Float:vec1[3],const Float:vec2[3],Float:output[3])
{
  output[0]=vec2[0]-vec1[0];
  output[1]=vec2[1]-vec1[1];
  output[2]=vec2[2]-vec1[2];
}

public wcShootEffect(effectid,&tmp1,&tmp2,&tmp3,bool:crit,tmp5,tmp6,tmp7,tmp8,tmp9, owner)
{

  switch(effectid)
  {
  case 1:
    if(tmp9==1)
    Effect1(tmp1,tmp2);
  case 2:
    if(tmp9==1)
    Effect2(tmp2); // bloodthirst effect
  case 3:
    if(tmp9==2)
    Effect3(tmp1,tmp3,tmp2); //evade effect
  case 4:
    if(tmp9==1)
    Effect4(tmp1,tmp2,false); // deadly poison
  case 5:
    if(tmp9==3)
    Effect5(tmp1,tmp3,tmp2); // absorb effect
  case 6:
    if(tmp9==1)
    Effect6(tmp2,tmp3);
  case 7:
    if(tmp9==2)
    Effect7(tmp2,tmp3,tmp1);
  case 8:
    if(tmp9==1)
    Effect8(tmp1,tmp3);
  case 9:
    if(tmp9==2)
    Effect9(tmp1,tmp3);
  case 10:
    if(tmp9==2)
    Effect10(tmp3); //deff stance part 1
  case 11:
    if(tmp9==3)
    Effect11(tmp1, tmp3);
  case 12:
    if(tmp9==2)
    Effect12(tmp3, tmp1);
  case 13:
    if(tmp9==2)
    Effect13(tmp3, tmp1); //Shield Wall
  case 14:
    if(tmp9==1)
    Effect14(tmp2); //Unleash stealth
  case 15:
    if(tmp9==2)
    Effect15(tmp3); //deff stance part 2
  case 20:
    if(tmp9==1)
    Effect20(tmp1); // (victim)  -armor
  case 21:
    if(tmp9==2 && crit)
    Effect21(tmp3); //(dmg, crit) - spell manipulation
  case 23:
    if(tmp9==2)
    Effect23(tmp1, tmp3); // (victim, dmg) - increases dmg with conditions
    //case 24: //death wish
    //  if(tmp9==2)
    //    Effect24(tmp1, tmp3);
  case 25:
    if(tmp9==3)
    Effect25(tmp1, tmp2, tmp3);
  case 26:
    if(tmp9==2)
    Effect26(tmp1, tmp3);
  case 27:
    if(tmp9==2)
    Effect27(tmp2, tmp3);
  case 28:
    if(tmp9==2)
    Effect28(tmp1, tmp3);
  case 29:
    if(tmp9==3)
    Effect29(tmp1,tmp2,tmp3); // Earth Shield
  case 30:
    if(tmp9==3)
    Effect30(tmp2,tmp3); // Deaths Embrace
    //case 31:
    //  if(tmp9==2)
    //    Effect31(tmp2,tmp3); // Death Wish, Attacker
  case 32:
    if(tmp9==2)
    Effect32(tmp1,tmp3); // Divine Favor + Enveloping Shadows + Molten Skin + Master of Magic
  case 33:
    if(tmp9==2)
    Effect33(tmp2,tmp3); //Enrage + Shaman Flurry
  case 34:
    if(tmp9==3 && crit)
    Effect34(tmp1, tmp3, tmp2); //Eye for an Eye
  case 35:
    if(tmp9==1)
    Effect35(tmp2,tmp5,tmp1,tmp6); //Sweeeping Strikes
  case 36:
    if(tmp9==2)
    Effect36(tmp1,tmp3); //Inspiration
  case 37:
    if(tmp9==2)
    Effect37(tmp1, tmp3); //Shamanistic Rage, Victim
  case 38:
    if(tmp9==1)
    Effect38(tmp2, tmp3); //Shamanistic Rage, Attacker
    /*case 39:
    if(tmp9==2)
    Effect39(tmp2, tmp3);*/
  case 40:
    if(tmp9==1)
    Effect40(tmp1, tmp2);
  case 41:
    if(tmp9==1)
    Effect41(tmp2,tmp1,tmp5,tmp6);
  case 42:
    if(tmp9==2)
    Effect42(tmp1, tmp3);
  case 43:
    if(tmp9==2)
    Effect43(tmp2,tmp3);
  case 44:
    if(tmp9==2)
    Effect44(tmp2,tmp3);
  case 45:
    if(tmp9==2)
    Effect45(tmp2,tmp3);
  case 46:
    if(tmp9==3)
    Effect46(tmp1,tmp3);
  case 47:
    if(tmp9==3)
    Effect47(tmp1,tmp2,tmp3);
  case 48:
    if(tmp9==2)
    Effect48(tmp2,tmp3);
  case 49:
    if(tmp9==2)
    Effect49(tmp1,tmp3);
  case 50:
    if(tmp9==2)
    Effect50(tmp1,tmp2,tmp3);
  case 51:
    if(tmp9==1)
    Effect51(tmp2,tmp5,tmp1,tmp6);
  case 53:
    if(tmp9==2)
    Effect53(tmp2, tmp3);
  case 54:
    if(tmp9==2)
    Effect54(tmp1,tmp3,tmp2);
  /*case 55:
    if(tmp9==3)
    Effect55(tmp1, tmp2, tmp3);*/
  case 56:
    if(tmp9==2)
    Effect56(tmp2, tmp3);
  case 57:
    if(tmp9==1)
    Effect57(tmp2, tmp5);
  case 58:
    if(tmp9==1)
    Effect58(tmp2);
  default:  {
      //Multi effects
      if(effectid>100)  {
        new id=RoundToFloor(float(effectid)/100.0);
        switch(id)  {
        case 1:
          if(tmp9==1)
          Effect_m1(effectid, tmp1, tmp2, tmp3);  //effect m1
          //              (victim, attacker, damage)
        case 2:
          if(tmp9==1)
          Effect_m2(effectid, owner);
        }
      }
    }

  }
}

/*new bool:firstSpell[10][30];*/

//tmp9=0 spells run on block, owner is victim (from wcRunSpell_1_1)
//tmp9=1 spells run on hit, owner can be victim or attacker
//tmp9=2 ???
//tmp9=3 ???
public wcRunSpell(spellid,race,&tmp1,&tmp2,&tmp3,&any:tmp4,&tmp5,&tmp6,&tmp7,&tmp8,&tmp9, owner)
{
  /*if(firstSpell[race][spellid])  {*/
  /*PrintToServer("Running Spell. Race %d Spell %d", race, spellid);*/
  /*}*/
  /*else  {*/
  /*firstSpell[race][spellid] = true;*/
  /*PrintToServer("Running FIRST time Spell. Race %d Spell %d", race, spellid);*/

  /*}*/
  if(IsZombie(owner))
    return 0; //No cast spells for Pets
  if(castIsRecommended(owner,spellid))
  {
    switch(race)
    {
      case 1: //Warrior
      {
        switch(spellid)
        {
          case 1:
            if(tmp9==2)
              return wcRunSpell_1_1(spellid,race,tmp1,tmp2,tmp3); //Shield Block
          case 2:
            wcRunSpell_1_2(spellid,race,tmp1); //Whirlwind
          case 3:
            wcRunSpell_1_3(spellid,race,tmp1); //Enrage Regeneration
          case 4:
            if(tmp9==1)
              wcRunSpell_1_4(spellid,race,tmp2,tmp1,tmp3); //Heroic Strike
          case 5:
            if(tmp9==1)
              wcRunSpell_1_5(spellid,race,tmp2,tmp1,tmp3); //Hamstring
          case 6:
            wcRunSpell_1_6(spellid,race,tmp1,tmp2,tmp3); //Revenge
          case 7:
            if(tmp9==1)
              wcRunSpell_1_7(spellid,race,tmp1,tmp2,tmp3,tmp4); //Spell Reflection
          case 8:
            if(tmp9==1)
              wcRunSpell_1_8(spellid,race,tmp2,tmp1,tmp3); //Mortal Strike
          case 9:
            wcRunSpell_1_9(spellid,race,tmp1); //Defensive Stance
          case 10:
            wcRunSpell_1_10(spellid,race,tmp1); //Thunder Clap
          case 11:
            wcRunSpell_1_11(spellid,race,tmp1); //Berserker Stance
          case 12:
            wcRunSpell_1_12(spellid,race,tmp1); //Shield Wall
          case 13:
            wcRunSpell_1_13(spellid,race,tmp1); //Taunt
          case 14:
            wcRunSpell_1_14(spellid,race,tmp1); //Rend
          case 15:
            wcRunSpell_1_15(spellid, race, tmp1); //Heroic Leap
        }
      }
      case 2: //Rogue
      {
        switch(spellid)
        {
          case 1:
            wcRunSpell_2_1(spellid,race,tmp1); //Evade
          case 2:
            if(tmp9==1)
              wcRunSpell_2_2(spellid,race,tmp2,tmp1,tmp3); //Sinister Strike
          case 3:
            wcRunSpell_2_3(spellid,race,tmp1); //Vanish
          case 4:
            if(tmp9==2)
              wcRunSpell_2_4(spellid,race,tmp2,tmp1,tmp3); //Backstab
          case 5:
            wcRunSpell_2_5(spellid,race,tmp1); //Deadly Poison
          case 6:
            wcRunSpell_2_6(spellid,race,tmp1); //Expose Armor
          case 7:
            wcRunSpell_2_7(spellid,race,tmp1); //Sprint
          case 8:
            if(tmp9==3)
              wcRunSpell_2_8(spellid,race,tmp1,tmp3); //Cheat Death
          case 9:
            wcRunSpell_2_9(spellid,race,tmp1); //Cloak of Shadow
          case 10:
            wcRunSpell_2_10(spellid,race,tmp1); //Slice & Dice
          case 11:
            wcRunSpell_2_11(spellid,race,tmp1); //Stealth
          case 12:
            wcRunSpell_2_12(spellid,race,tmp1); //Blind
          case 13:
            wcRunSpell_2_13(spellid, race, tmp1); //Fan of Knives
        }
      }
      case 3: //Mage
      {
        switch(spellid)
        {
          case 1:
            wcRunSpell_3_1(spellid,race,tmp2, tmp1); //Frostbolt
          case 2:
            wcRunSpell_3_2(spellid,race,tmp1); //Frost Nova
          case 3:
            wcRunSpell_3_3(spellid,race,tmp2, tmp1); //Fireball
          case 4:
            wcRunSpell_3_4(spellid,race,tmp2,tmp1); //Scorch
          case 5:
            wcRunSpell_3_5(spellid,race,tmp1); //Blink
          case 6:
            wcRunSpell_3_6(spellid,race,tmp1); //Arcane Intellect
          case 7:
            wcRunSpell_3_7(spellid,race,tmp1); //Arcane Explosion
          case 8:
            wcRunSpell_3_8(spellid,race,tmp1); //Combustion
          case 9:
            wcRunSpell_3_9(spellid,race,tmp1); //Mana Shield
          case 10:
            wcRunSpell_3_10(spellid,race,tmp1); //Polymorph
          case 11:
            if(tmp9==3)
              wcRunSpell_3_11(spellid,race,tmp1,tmp2,tmp3); //Ice Block
          case 12:
            wcRunSpell_3_12(spellid,race,tmp1); //Flame Orb
        }
      }
      case 4: //Paladin
      {
        switch(spellid)
        {
          case 1:
            if(tmp9==2)
              return wcRunSpell_4_1(spellid,race,tmp1,tmp2,tmp3); //Shield Block
          case 2:
            wcRunSpell_4_2(spellid,race,tmp1); //Divine Shield
          case 3:
            wcRunSpell_5_1(spellid,race,tmp1); //Flash Heal
          case 4:
            wcRunSpell_5_3(spellid,race,tmp1); //Greater Heal
          case 5:
            wcRunSpell_4_5(spellid,race,tmp1); //Blessing of Freedom
          case 6:
            wcRunSpell_4_6(spellid,race,tmp1); //Seal of Command
          case 7: // index is correct
            wcRunSpell_4_6(spellid,race,tmp1); //Seal of Command
          case 8:
            wcRunSpell_4_8(spellid,race,tmp1); //Judgement
          case 9:
            wcRunSpell_4_9(spellid,race,tmp1); //Blessing of Kings
          case 10:
            wcRunSpell_4_10(spellid,race,tmp1); //Spiritual Attunement
          case 11:
            wcRunSpell_4_11(tmp1); //Greater Blessing of Kings
          case 12:
            wcRunSpell_4_6(spellid,race,tmp1); //Seal of Command
          case 13:
            wcRunSpell_4_6(spellid,race,tmp1); //Seal of Command
          case 14:
            wcRunSpell_4_6(spellid,race,tmp1); //Seal of Command
          case 15:
            wcRunSpell_4_6(spellid,race,tmp1); //Seal of Command
          case 16:
            wcRunSpell_4_6(spellid,race,tmp1); //Seal of Command
          case 17:
            wcRunSpell_4_17(spellid, race, tmp1); //Word of Glory
          case 18:
            wcRunSpell_4_18(spellid, race, tmp1); //Holy Radiance
        }
      }
      case 5: //Priest
      {
        switch(spellid)
        {
          case 1:
            if(!GetShadowform(tmp1))
              wcRunSpell_5_1(spellid,race,tmp1); //Flash Heal
          case 2:
            if(!GetShadowform(tmp1))
              wcRunSpell_5_2(spellid,race,tmp1); //Prayer of Healing
          case 3:
            if(!GetShadowform(tmp1))
              wcRunSpell_5_3(spellid,race,tmp1); //Greater Heal
          case 4:
            wcRunSpell_5_4(spellid,race,tmp1); //Power Word: Shield
          case 5:
            wcRunSpell_5_5(spellid,race,tmp2, tmp1); //Mind Blast
          case 6:
            wcRunSpell_5_6(spellid,race,tmp1); //Mana Burn
          case 7:
            wcRunSpell_5_7(spellid,race,tmp1); //Power Word: Fortitude
          case 8:
            if(!GetShadowform(tmp1))
              wcRunSpell_5_8(spellid,race,tmp1); //Resurrection
          case 9:
            wcRunSpell_5_9(spellid,race,tmp1); //Hymn of Hope
          case 10:
            wcRunSpell_5_10(spellid, race, tmp1); //Inner Will
          case 11:
            wcRunSpell_5_11(spellid, race, tmp1); //Fade
        }
      }
      case 6: //Shaman
      {
        switch(spellid)
        {
          case 1:
            if(tmp9==2)
              wcRunSpell_6_1(spellid,race,tmp1,tmp2,tmp3); //Shield Block
          case 2:
            if(tmp9==2)
              wcRunSpell_6_2(spellid,race,tmp2,tmp1,tmp3); //Windfury
          case 3:
            wcRunSpell_6_3(spellid,race,tmp2,tmp1); //Flame Shock
          case 4:
            wcRunSpell_6_4(spellid,race,tmp2,tmp1); //Frost Shock
          case 5:
            wcRunSpell_6_5(spellid,race,tmp2,tmp1); //Earth Shock
          case 6:
            wcRunSpell_6_6(spellid,race,tmp1); //Healing Wave
          case 7:
            wcRunSpell_6_7(spellid,race,tmp1); //Rockbiter Weapon
          case 8:
            wcRunSpell_6_8(spellid,race,tmp1); //Reincarnation
          case 9:
            wcRunSpell_6_9(spellid,race,tmp1);  //Fire nova Totem
          case 10:
            wcRunSpell_6_10(spellid,race,tmp1);  //Earthbind Totem
          case 11:
            wcRunSpell_6_11(spellid, race, tmp1); //Healing Rain
          case 12:
            wcRunSpell_6_12(tmp1); //Agile Defence
        }
      }
      case 7: //Warlock
      {
        switch(spellid)
        {
          case 1:
            wcRunSpell_7_1(spellid,race,tmp2, tmp1, false); // shadow bolt
          case 2:
            wcRunSpell_7_2(spellid,race,tmp1); // Soul Harvest
          case 3:
            wcRunSpell_7_3(spellid,race,tmp2, tmp1); // corruption
          case 4:
            wcRunSpell_7_4(spellid,race,tmp1); // fear
          case 5:
            wcRunSpell_7_5(spellid,race,tmp1); // life tap
          case 6:
            wcRunSpell_7_6(spellid,race,tmp1); // demon skin
          case 7:
            wcRunSpell_7_7(spellid,race,tmp1); // corruption
          case 8:
            wcRunSpell_7_8(spellid,race,tmp1); // Metamorphosis
          case 9:
            wcRunSpell_7_9(spellid,race,tmp1); // Hellfire
          case 10:
            if(tmp9==2)
              wcRunSpell_7_10(spellid,race,tmp2,tmp1,tmp3); //Curse
          case 11:
            wcRunSpell_7_11(spellid,race,tmp1); // Infernal
        }
      }
      case 8: //Death Knight
      {
        switch(spellid)
        {
          case 1:
            if(tmp9==2)
              wcRunSpell_8_1(spellid,race,tmp2,tmp1,tmp3); //blood strike
          case 2:
            wcRunSpell_8_2(spellid,race,tmp1); //death grip
          case 3:
            if(tmp9==2)
              wcRunSpell_8_3(spellid,race,tmp2,tmp1,tmp3); //plague strike
          case 4:
            wcRunSpell_8_4(spellid,race,tmp1); //icy touch
          case 5:
            if(tmp9==2)
              wcRunSpell_8_5(spellid,race,tmp2,tmp1,tmp3); //death Strike
          case 6:
            wcRunSpell_8_6(spellid,race,tmp1); //death coil
          case 7:
            wcRunSpell_8_7(tmp1,tmp3); //Anti-Magic Shell
          case 8:
            wcRunSpell_8_8(spellid,race,tmp1); //Mind Freeze
          case 9:
            wcRunSpell_8_9(spellid,race,tmp1); //Raise Ally
          case 10:
            if(tmp9==1)
              wcRunSpell_8_10(spellid,race,tmp2,tmp1,tmp3); //Necrotic Strike
          case 11:
            wcRunSpell_8_11(spellid, race, tmp1); //Death and Decay
        }
      }
      case 9: // Monk
      {
        switch(spellid)
        {
          case 1:
            if(tmp9 == 1)
              wcRunSpell_9_1(spellid,race,tmp2,tmp1,tmp3); //Jab
          case 2:
            if(tmp9 == 1)
              wcRunSpell_9_2(spellid,race,tmp2,tmp1,tmp3); //Tiger Palm
          case 3:
            wcRunSpell_9_3(spellid,race,tmp1); //Expel Harm
          case 4:
            wcRunSpell_9_4(spellid,race,tmp1); //Revival
          case 5:
            wcRunSpell_9_5(spellid,race,tmp1); //Spinning Crane Kick
          case 6:
            wcRunSpell_9_6(spellid,race,tmp1); //Path of Blossoms
          case 7:
            wcRunSpell_9_7(spellid,race,tmp1); //Enhanced Parry
          //case 8: //Moved to spawn event
            //wcRunSpell_9_9(tmp1); //Legacy of the White Tiger
          case 10:
            wcRunSpell_9_10(spellid,race,tmp1); //Chi Burst
        }
      }
    }
  }
  return 0;
}



public ReloadDBs()
{

  decl String:thepath[PLATFORM_MAX_PATH],String:path[PLATFORM_MAX_PATH];
  //loading sets table
  if(wcsets!=INVALID_HANDLE)
  CloseHandle(wcsets);
  wcsets=CreateKeyValues("sets");
  Format (thepath,sizeof(thepath),"data/wcsets.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(wcsets,path);
  //end

  //loading boss drop table
  if(wcbossdrop!=INVALID_HANDLE)
  CloseHandle(wcbossdrop);
  wcbossdrop=CreateKeyValues("drop");
  Format (thepath,sizeof(thepath),"data/wcbossdrop.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(wcbossdrop,path);
  //end
  //loading drop table
  if(wcdrop!=INVALID_HANDLE)
  CloseHandle(wcdrop);
  wcdrop=CreateKeyValues("drop");
  Format (thepath,sizeof(thepath),"data/wcdrop.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(wcdrop,path);
  //end
  //opening help menu
  if(wchelp!=INVALID_HANDLE)
  CloseHandle(wchelp);
  wchelp=CreateKeyValues("help");
  Format (thepath,sizeof(thepath),"data/wchelp.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(wchelp,path);
  //end
  //opening news
  if(wcnews!=INVALID_HANDLE)
  CloseHandle(wcnews);
  wcnews=CreateKeyValues("news");
  Format (thepath,sizeof(thepath),"data/wcnews.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(wcnews,path);
  //end of news
  //loading items
  if(items!=INVALID_HANDLE)
  CloseHandle(items);
  items=CreateKeyValues("items");
  Format (thepath,sizeof(thepath),"data/items.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(items,path);
  if (!KvGotoFirstSubKey(items))
  {
    LogError("[RPGx] Error: failed to open SubKey in %s path",thepath);
  }
  //end of loading items
  //effects
  if(effectsdb!=INVALID_HANDLE)
  CloseHandle(effectsdb);
  effectsdb=CreateKeyValues("effects");
  Format (thepath,sizeof(thepath),"data/effects.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(effectsdb,path);

  //Spells DB
  if(SpellsDB1!=INVALID_HANDLE)
  CloseHandle(SpellsDB1);
  SpellsDB1=CreateKeyValues("spells");
  Format (thepath,sizeof(thepath),"data/spells_1.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(SpellsDB1,path);
  if(SpellsDB2!=INVALID_HANDLE)
  CloseHandle(SpellsDB2);
  SpellsDB2=CreateKeyValues("spells");
  Format (thepath,sizeof(thepath),"data/spells_2.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(SpellsDB2,path);
  if(SpellsDB3!=INVALID_HANDLE)
  CloseHandle(SpellsDB3);
  SpellsDB3=CreateKeyValues("spells");
  Format (thepath,sizeof(thepath),"data/spells_3.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(SpellsDB3,path);
  if(SpellsDB4!=INVALID_HANDLE)
  CloseHandle(SpellsDB4);
  SpellsDB4=CreateKeyValues("spells");
  Format (thepath,sizeof(thepath),"data/spells_4.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(SpellsDB4,path);
  if(SpellsDB5!=INVALID_HANDLE)
  CloseHandle(SpellsDB5);
  SpellsDB5=CreateKeyValues("spells");
  Format (thepath,sizeof(thepath),"data/spells_5.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(SpellsDB5,path);
  if(SpellsDB6!=INVALID_HANDLE)
  CloseHandle(SpellsDB6);
  SpellsDB6=CreateKeyValues("spells");
  Format (thepath,sizeof(thepath),"data/spells_6.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(SpellsDB6,path);
  if(SpellsDB7!=INVALID_HANDLE)
  CloseHandle(SpellsDB7);
  SpellsDB7=CreateKeyValues("spells");
  Format (thepath,sizeof(thepath),"data/spells_7.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(SpellsDB7,path);
  if(SpellsDB8!=INVALID_HANDLE)
  CloseHandle(SpellsDB8);
  SpellsDB8=CreateKeyValues("spells");
  Format (thepath,sizeof(thepath),"data/spells_8.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(SpellsDB8,path);
  //Monk
  if(SpellsDB9!=INVALID_HANDLE)
  CloseHandle(SpellsDB9);
  SpellsDB9=CreateKeyValues("spells");
  Format (thepath,sizeof(thepath),"data/spells_9.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(SpellsDB9,path);


  if(StatsDB!=INVALID_HANDLE)
  CloseHandle(StatsDB);
  StatsDB=CreateKeyValues("stats");
  Format (thepath,sizeof(thepath),"data/stats.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(StatsDB,path);



  //end of reloading spells dbs
  //reloading talents dbs
  if(TalentsDB!=INVALID_HANDLE)
  CloseHandle(TalentsDB);
  TalentsDB=CreateKeyValues("talents");
  Format (thepath,sizeof(thepath),"data/talents.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(TalentsDB,path);
  talentsPreloadMaps();

  //loading item names
  if(wcitemnames!=INVALID_HANDLE)
  CloseHandle(wcitemnames);
  wcitemnames=CreateKeyValues("drop");
  Format (thepath,sizeof(thepath),"data/item_names.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(wcitemnames,path);
  //end

  //loading base item names
  if(wcbaseitemnames!=INVALID_HANDLE)
  CloseHandle(wcbaseitemnames);
  wcbaseitemnames=CreateKeyValues("base");
  Format (thepath,sizeof(thepath),"data/base_item_names.txt");
  BuildPath(Path_SM,path,PLATFORM_MAX_PATH,thepath);
  FileToKeyValues(wcbaseitemnames,path);
  //end


}

public InstallDownloads()
{
  if(DOWNLOAD_SOUNDS)  {
    //Play sound
    AddFileToDownloadsTable("sound/quake/play.wav");
    //end
    //Arena
    AddFileToDownloadsTable("sound/wc/arena/countdown.wav");
    AddFileToDownloadsTable("sound/wc/arena/HordeWin.wav");
    AddFileToDownloadsTable("sound/wc/arena/AliianceWin.wav");
    //end
    AddFileToDownloadsTable("sound/wc/levelupv2.wav");
    //priest files
    AddFileToDownloadsTable("sound/wc/spells/heal.wav");
    AddFileToDownloadsTable("sound/wc/spells/mindblast.wav");
    AddFileToDownloadsTable("sound/wc/spells/manaburn.wav");
    AddFileToDownloadsTable("sound/wc/spells/absorb.wav");
    //end of priest files
    //paladin files
    AddFileToDownloadsTable("sound/wc/spells/seal.wav");
    AddFileToDownloadsTable("sound/wc/spells/judge.wav");
    AddFileToDownloadsTable("sound/wc/spells/divine.wav");
    //end of paladin files
    AddFileToDownloadsTable("sound/wc/spells/sprint.wav");
    AddFileToDownloadsTable("sound/wc/spells/vanish.wav");
    AddFileToDownloadsTable("sound/wc/spells/strike.wav");
    AddFileToDownloadsTable("sound/wc/spells/whirlwind.wav");
    AddFileToDownloadsTable("sound/wc/spells/arcexpl.wav");
    AddFileToDownloadsTable("sound/wc/spells/blink.wav");
    AddFileToDownloadsTable("sound/wc/spells/frostbolt.wav");
    AddFileToDownloadsTable("sound/wc/spells/fireball.wav");
    AddFileToDownloadsTable("sound/wc/spells/curse.wav");
    AddFileToDownloadsTable("sound/wc/spells/shadow.wav");
    AddFileToDownloadsTable("sound/wc/spells/fear.wav");
    AddFileToDownloadsTable("sound/wc/spells/resurrect.wav");
    AddFileToDownloadsTable("sound/wc/spells/shieldwall.wav");
    AddFileToDownloadsTable("sound/wc/spells/rend.wav");
    AddFileToDownloadsTable("sound/wc/spells/thunderclap.wav");
    AddFileToDownloadsTable("sound/wc/spells/warriorweaponswing.wav");
    //80 lvl spells sounds
    AddFileToDownloadsTable("sound/wc/spells/hellfire.wav");
    AddFileToDownloadsTable("sound/wc/spells/hymn.wav");
    AddFileToDownloadsTable("sound/wc/spells/shadowtouch.wav");
    
    //Version 2.25 sounds
    AddFileToDownloadsTable("sound/wc/spells/divinestorm.wav");
    AddFileToDownloadsTable("sound/wc/spells/eyeforaneye.wav");
    AddFileToDownloadsTable("sound/wc/spells/avengersshield.wav");
    AddFileToDownloadsTable("sound/wc/spells/holyshock.wav");
    AddFileToDownloadsTable("sound/wc/spells/shamanisticrage.wav");
    AddFileToDownloadsTable("sound/wc/spells/chaosbolt.wav");
    AddFileToDownloadsTable("sound/wc/spells/metamorphosis.wav");
    AddFileToDownloadsTable("sound/wc/spells/painsuppression.wav");
    AddFileToDownloadsTable("sound/wc/spells/invisibility.wav");
    AddFileToDownloadsTable("sound/wc/spells/focusedmind.wav");
    AddFileToDownloadsTable("sound/wc/spells/livingbomb.wav");
    AddFileToDownloadsTable("sound/wc/spells/frostbite.wav");
    AddFileToDownloadsTable("sound/wc/spells/adrenalinerush.wav");
    AddFileToDownloadsTable("sound/wc/spells/hackandslash.wav");
    AddFileToDownloadsTable("sound/wc/spells/cheatdeath.wav");
    AddFileToDownloadsTable("sound/wc/spells/livingbomb2.wav");

    //Version 2.50 sounds
    AddFileToDownloadsTable("sound/wc/spells/presence_cast.wav"); //Presence Stances
    AddFileToDownloadsTable("sound/wc/spells/runes_regen.wav");
    AddFileToDownloadsTable("sound/wc/spells/dk_strike.wav");
    AddFileToDownloadsTable("sound/wc/spells/deathgrip.wav");
    AddFileToDownloadsTable("sound/wc/spells/icytouch.wav");
    AddFileToDownloadsTable("sound/wc/spells/deathcoil.wav");
    AddFileToDownloadsTable("sound/wc/spells/mindfreeze.wav");
    AddFileToDownloadsTable("sound/wc/spells/raiseally.wav");
    AddFileToDownloadsTable("sound/wc/spells/runetap.wav");
    AddFileToDownloadsTable("sound/wc/spells/vampiric.wav");
    AddFileToDownloadsTable("sound/wc/spells/hungeringcold.wav");
    AddFileToDownloadsTable("sound/wc/spells/butchery.wav");
    AddFileToDownloadsTable("sound/wc/spells/antimagic.wav");
    AddFileToDownloadsTable("sound/wc/spells/heartfire.wav");


    AddFileToDownloadsTable("sound/wc/spells/boss_enrage.wav");

    AddFileToDownloadsTable("sound/wc/spells/sheep.wav"); //Polymorph spell


    //4.0 version sounds
    AddFileToDownloadsTable("sound/wc/spells/crusader.wav");
    AddFileToDownloadsTable("sound/wc/spells/iceblock.wav");
    AddFileToDownloadsTable("sound/wc/spells/flameorb.wav");
    AddFileToDownloadsTable("sound/wc/spells/flameorb2.wav");
    AddFileToDownloadsTable("sound/wc/spells/blind.wav");
    AddFileToDownloadsTable("sound/wc/spells/fanofknives.wav");
    AddFileToDownloadsTable("sound/wc/spells/infernal.wav");
    AddFileToDownloadsTable("sound/wc/spells/necroticstrike.wav");
    AddFileToDownloadsTable("sound/wc/spells/deathanddecay.wav");
    AddFileToDownloadsTable("sound/wc/spells/blizzard.wav");
    AddFileToDownloadsTable("sound/wc/spells/arcanemissile.wav");
    AddFileToDownloadsTable("sound/wc/spells/clearcast.wav");
    //AddFileToDownloadsTable("sound/wc/spells/aoeshield.wav");
    AddFileToDownloadsTable("sound/wc/spells/lightwell.wav");
    AddFileToDownloadsTable("sound/wc/spells/mindflay.wav");
    AddFileToDownloadsTable("sound/wc/spells/airshock.wav");
    AddFileToDownloadsTable("sound/wc/spells/lavasurge.wav");
    AddFileToDownloadsTable("sound/wc/spells/execute.wav");

    //Hani sounds
    AddFileToDownloadsTable("sound/wc/spells/shadowstep.wav");
    //AddFileToDownloadsTable("sound/wc/spells/corpseparty.wav");
    AddFileToDownloadsTable("sound/wc/spells/shadowgate2.wav");
    AddFileToDownloadsTable("sound/wc/spells/shadowgate.wav");
    AddFileToDownloadsTable("sound/wc/spells/colossussmash.wav");
    AddFileToDownloadsTable("sound/wc/spells/LightningBolt.wav");
    AddFileToDownloadsTable("sound/wc/spells/tidetotem.wav");
    AddFileToDownloadsTable("sound/wc/spells/obliterate.wav");
    AddFileToDownloadsTable("sound/wc/spells/corpseexplosionnew.wav");
    AddFileToDownloadsTable("sound/wc/spells/soulharvest.wav");
    AddFileToDownloadsTable("sound/wc/spells/taunt.wav");
    AddFileToDownloadsTable("sound/wc/spells/Shield_of_the_Righteous.wav");
    

    //Monk sounds
    AddFileToDownloadsTable("sound/wc/spells/expelharm1.wav");
    AddFileToDownloadsTable("sound/wc/spells/touchofkarma.wav");
    AddFileToDownloadsTable("sound/wc/spells/revival.wav");
    AddFileToDownloadsTable("sound/wc/spells/blossom1.wav");
    AddFileToDownloadsTable("sound/wc/spells/blossom2.wav");
    AddFileToDownloadsTable("sound/wc/spells/fireblossom1.wav");
    AddFileToDownloadsTable("sound/wc/spells/fireblossom2.wav");
    AddFileToDownloadsTable("sound/wc/spells/fireblossom3.wav");
    AddFileToDownloadsTable("sound/wc/spells/jab.wav");
    AddFileToDownloadsTable("sound/wc/spells/mistweaver_stance.wav");
    AddFileToDownloadsTable("sound/wc/spells/windwalker_stance.wav");
    AddFileToDownloadsTable("sound/wc/spells/brewmaster_stance.wav");
    AddFileToDownloadsTable("sound/wc/spells/stancehealmonk.wav");
    AddFileToDownloadsTable("sound/wc/spells/fistsoffury.wav");
    AddFileToDownloadsTable("sound/wc/spells/burb1.wav");
    AddFileToDownloadsTable("sound/wc/spells/burb2.wav");
    AddFileToDownloadsTable("sound/wc/spells/burb3.wav");
    AddFileToDownloadsTable("sound/wc/spells/burb4.wav");
    AddFileToDownloadsTable("sound/wc/spells/burb5.wav");
    AddFileToDownloadsTable("sound/wc/spells/chiexplosion.wav");
    AddFileToDownloadsTable("sound/wc/spells/serpentland.wav");
    AddFileToDownloadsTable("sound/wc/spells/serpentjump.wav");
    AddFileToDownloadsTable("sound/wc/spells/kegsmash.wav");
    AddFileToDownloadsTable("sound/wc/spells/tigerpalm.wav");
    AddFileToDownloadsTable("sound/wc/spells/chiburstcast.wav");
    AddFileToDownloadsTable("sound/wc/spells/chiburstdmg.wav");
    AddFileToDownloadsTable("sound/wc/spells/chiburstheal.wav");
    AddFileToDownloadsTable("sound/wc/spells/guard.wav");
    AddFileToDownloadsTable("sound/wc/spells/breathfire.wav");
    AddFileToDownloadsTable("sound/wc/spells/Parry.wav");
    AddFileToDownloadsTable("sound/wc/spells/dizzyinghaze.wav");
    AddFileToDownloadsTable("sound/wc/spells/cursedsoul.wav");


  }
  //totems
  //Horde
  AddFileToDownloadsTable("materials/props/wow/h_totem/h_totem.vmt");
  AddFileToDownloadsTable("materials/props/wow/h_totem/h_totem.vtf");
  AddFileToDownloadsTable("materials/props/wow/h_totem/h_totem_n.vtf");

  AddFileToDownloadsTable("models/props/wow/h_totem/totem.dx80.vtx");
  AddFileToDownloadsTable("models/props/wow/h_totem/totem.dx90.vtx");
  AddFileToDownloadsTable("models/props/wow/h_totem/totem.mdl");
  AddFileToDownloadsTable("models/props/wow/h_totem/totem.phy");
  AddFileToDownloadsTable("models/props/wow/h_totem/totem.sw.vtx");
  AddFileToDownloadsTable("models/props/wow/h_totem/totem.vvd");

  //Alliance
  AddFileToDownloadsTable("materials/props/wow/a_totem/a_totem.vmt");
  AddFileToDownloadsTable("materials/props/wow/a_totem/a_totem.vtf");
  AddFileToDownloadsTable("materials/props/wow/a_totem/a_totem_n.vtf");

  AddFileToDownloadsTable("models/props/wow/a_totem/totem.dx80.vtx");
  AddFileToDownloadsTable("models/props/wow/a_totem/totem.dx90.vtx");
  AddFileToDownloadsTable("models/props/wow/a_totem/totem.mdl");
  AddFileToDownloadsTable("models/props/wow/a_totem/totem.phy");
  AddFileToDownloadsTable("models/props/wow/a_totem/totem.sw.vtx");
  AddFileToDownloadsTable("models/props/wow/a_totem/totem.vvd");

  //Slides
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1A.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1A.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1B.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1B.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1C.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1C.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2A.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2A.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2B.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2B.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2C.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2C.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/intro1.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/intro1.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/intro2.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/intro2.vtf");*/
  /*//RU slides*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1A_ru.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1A_ru.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1B_ru.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1B_ru.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1C_ru.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation1C_ru.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2A_ru.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2A_ru.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2B_ru.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2B_ru.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2C_ru.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/initiation2C_ru.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/intro1_ru.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/intro1_ru.vtf");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/intro2_ru.vmt");*/
  /*AddFileToDownloadsTable("materials/overlays/wow/intro2_ru.vtf");*/



  RankDownloads();
}

public PlaySound(client, String:sound[])
{
  decl String:buffer[150];
  Format(buffer, sizeof(buffer), "play %s", sound);
  ClientCommand(client, buffer);
}


public UnLoadDBs()
{
  CloseHandle(wcnews);
  CloseHandle(effectsdb);
}

public Float:GetDistanceBetween(const Float:startvec[3],const Float:endvec[3])
{
  return SquareRoot((startvec[0]-endvec[0])*(startvec[0]-endvec[0])+(startvec[1]-endvec[1])*(startvec[1]-endvec[1])+(startvec[2]-endvec[2])*(startvec[2]-endvec[2]));
}

stock GetNearClients(const String:matchstr[],Float:location[3],clients[],Float:distance, bool:ignoreWall=true )
{
  new count=0;
  new maxplayers=GetMaxClients();
  new Float:loc2[3];
  if(StrEqual(matchstr,"@all",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x))
      if(IsPlayerAlive(x))
      {
        GetClientAbsOrigin(x,loc2);
        loc2[2]+=20.0;
        if(GetDistanceBetween(location,loc2)<=distance)
        if(ignoreWall || IsClearPath(location, loc2,x))
        {

          clients[count]=x;
          count++;
        }
        else
        PrintToServer("Path is not clear");
      }
    }
  }
  else if(StrEqual(matchstr,"@t",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x)&&GetClientTeam(x)==2)
      if(IsPlayerAlive(x))
      {
        GetClientAbsOrigin(x,loc2);
        if(GetDistanceBetween(location,loc2)<=distance)
        if(ignoreWall || IsClearPath(location, loc2,x))
        {
          clients[count]=x;
          count++;
        }
      }
    }
  }
  else if(StrEqual(matchstr,"@ct",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x)&&GetClientTeam(x)==3)
      if(IsPlayerAlive(x))
      {
        GetClientAbsOrigin(x,loc2);
        if(GetDistanceBetween(location,loc2)<=distance)
        if(ignoreWall || IsClearPath(location, loc2,x))
        {
          clients[count]=x;
          count++;
        }
      }
    }
  }
  else if(matchstr[0]=='@')
  {
    new userid=StringToInt(matchstr[1]);
    if(userid)
    {
      new index=GetClientOfUserId(userid);
      if(index)
      {
        if(IsClientInGame(index))
        if(IsPlayerAlive(index))
        {
          GetClientAbsOrigin(index,loc2);
          if(GetDistanceBetween(location,loc2)<=distance)
          if(ignoreWall || IsClearPath(location, loc2,index))
          {
            clients[count]=index;
            count++;
          }
        }
      }
    }
  }
  else
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x))
      {
        decl String:name[64];
        GetClientName(x,name,64);
        if(StrContains(name,matchstr,false)!=-1)
        if(IsPlayerAlive(x))
        {
          GetClientAbsOrigin(x,loc2);
          if(GetDistanceBetween(location,loc2)<=distance)
          if(ignoreWall || IsClearPath(location, loc2,x))
          {
            clients[count]=x;
            count++;
          }
        }
      }
    }
  }
  return count;
}

GetNearestClient(const String:matchstr[],Float:location[3],Float:distance, self=0, tself=0)
{
  new target=0;
  new Float:closest=distance;
  new maxplayers=GetMaxClients();
  new Float:loc2[3];
  if(StrEqual(matchstr,"@all",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(x==self)
      continue;
      if(x==tself)
      continue;
      if(IsClientInGame(x))
      if(IsPlayerAlive(x))
      {
        GetClientAbsOrigin(x,loc2);
        if(GetDistanceBetween(location,loc2)<=closest)
        {
          target=x;
          closest=GetDistanceBetween(location,loc2);
        }
      }
    }
  }
  else if(StrEqual(matchstr,"@t",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(x==self)
      continue;
      if(x==tself)
      continue;
      if(IsClientInGame(x)&&GetClientTeam(x)==2)
      if(IsPlayerAlive(x))
      {
        GetClientAbsOrigin(x,loc2);
        if(GetDistanceBetween(location,loc2)<=closest)
        {
          target=x;
          closest=GetDistanceBetween(location,loc2);
        }
      }
    }
  }
  else if(StrEqual(matchstr,"@ct",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(x==self)
      continue;
      if(x==tself)
      continue;
      if(IsClientInGame(x)&&GetClientTeam(x)==3)
      if(IsPlayerAlive(x))
      {
        GetClientAbsOrigin(x,loc2);
        if(GetDistanceBetween(location,loc2)<=closest)
        {
          target=x;
          closest=GetDistanceBetween(location,loc2);
        }
      }
    }
  }
  else if(matchstr[0]=='@')
  {
    new userid=StringToInt(matchstr[1]);
    if(userid)
    {
      new index=GetClientOfUserId(userid);
      if(index)
      {
        if(IsClientInGame(index))
        if(IsPlayerAlive(index))
        {
          GetClientAbsOrigin(index,loc2);
          if(GetDistanceBetween(location,loc2)<=closest)
          {
            target=index;
            closest=GetDistanceBetween(location,loc2);
          }
        }
      }
    }
  }
  else
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x))
      {
        decl String:name[64];
        GetClientName(x,name,64);
        if(StrContains(name,matchstr,false)!=-1)
        if(IsPlayerAlive(x))
        {
          GetClientAbsOrigin(x,loc2);
          if(GetDistanceBetween(location,loc2)<=closest)
          {
            target=x;
            closest=GetDistanceBetween(location,loc2);
          }
        }
      }
    }
  }
  return target;
}

public GetNearestDead(const String:matchstr[],Float:location[3],Float:distance)
{
  new target=0;
  new Float:closest=distance;
  new maxplayers=GetMaxClients();
  new Float:loc2[3];
  if(StrEqual(matchstr,"@all",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x))
      if(!IsPlayerAlive(x))
      {
        loc2 = vClientCorpse[x];
        if(GetDistanceBetween(location,loc2)<=closest)
        {
          target=x;
          closest=GetDistanceBetween(location,loc2);
        }
      }
    }
  }
  else if(StrEqual(matchstr,"@t",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x)&&GetClientTeam(x)==2)
      if(!IsPlayerAlive(x))
      {
        loc2 = vClientCorpse[x];
        if(GetDistanceBetween(location,loc2)<=closest)
        {
          target=x;
          closest=GetDistanceBetween(location,loc2);
        }
      }
    }
  }
  else if(StrEqual(matchstr,"@ct",false))
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x)&&GetClientTeam(x)==3)
      if(!IsPlayerAlive(x))
      {
        loc2 = vClientCorpse[x];
        if(GetDistanceBetween(location,loc2)<=closest)
        {
          target=x;
          closest=GetDistanceBetween(location,loc2);
        }
      }
    }
  }
  else if(matchstr[0]=='@')
  {
    new userid=StringToInt(matchstr[1]);
    if(userid)
    {
      new index=GetClientOfUserId(userid);
      if(index)
      {
        if(IsClientInGame(index))
        if(!IsPlayerAlive(index))
        {
          loc2 = vClientCorpse[index];
          if(GetDistanceBetween(location,loc2)<=closest)
          {
            target=index;
            closest=GetDistanceBetween(location,loc2);
          }
        }
      }
    }
  }
  else
  {
    for(new x=1;x<=maxplayers;x++)
    {
      if(IsClientInGame(x))
      {
        decl String:name[64];
        GetClientName(x,name,64);
        if(StrContains(name,matchstr,false)!=-1)
        if(!IsPlayerAlive(x))
        {
          loc2 = vClientCorpse[x];
          if(GetDistanceBetween(location,loc2)<=closest)
          {
            target=x;
            closest=GetDistanceBetween(location,loc2);
          }
        }
      }
    }
  }
  return target;
}

SetClientSpeed(client, Float:newspeed, bool:ignore=false,bool:fear=false)
{
  if(IsBoss() && (GetBoss(client) > 0) )
  return 0;
  new Float:curspeed=GetClientSpeed(client);
  new bool:slower = curspeed>newspeed;
  if(FreedomClient[client] && (!ignore))
  if(slower)
  return 0;
  if(HasGlyph(client, GLYPH_SHAMAN_RAGE) && HasShamanisticRage(client))
  {
    if(slower)
    return 0;
  }
  if((!ignore) && (slower) )
  {
    if(FindEffect(client, 22))
    {
      if(Effect22(client))
      return 0;
    }
    if(GetClass(client)==8 && wcGetTalentLevel(client,2,2,1)>0 && newspeed < 0.1) //Lichborne
    return 0;
    else if(GetClass(client)==1 && wcGetTalentLevel(client,1,1,4)>0) //Unstoppable
    {
      decl String:line[255];
      wcGetTalentInfo(1,1,1,4,client,"chance",line); //Unstoppable
      new chance = StringToInt(line);
      new random = GetRandomInt(1,100);
      if (chance >= random)
      {
        return 0;
      }
    }
    else if (GetClass(client)==7 && wcGetTalentLevel(client,2,1,4)>0) //Demonic Stride
    {

      decl String:line[255];
      wcGetTalentInfo(7,2,1,4,client,"chance",line); //Demonic Stride
      new chance = StringToInt(line);
      new random = GetRandomInt(1,100);
      if (chance >= random)
      {
        return 0;
      }
    }
    else if (GetClass(client)==8 && wcGetTalentLevel(client,1,1,4)>0) //Bloodtrail
    {

      decl String:line[255];
      wcGetTalentInfo(8,1,1,4,client,"chance",line); //Bloodtrail
      new chance = StringToInt(line);
      new random = GetRandomInt(1,100);
      if (chance >= random)
      {
        return 0;
      }
    }
  }
  if(newspeed >= 0.1)
  {
    SetEntDataFloat(client, offsSpeed, newspeed);
    if(!fear)
    {
      if(slower)
      g_fearspeed[client][0]*=(newspeed/g_fearspeed[client][1]);
      else
      g_fearspeed[client][0]*=(g_fearspeed[client][1]/newspeed);
      g_fearspeed[client][1]=newspeed;
    }
  }
  else
  {
    SetEntityMoveType(client, MOVETYPE_NONE);
  }
  if(!slower && GetEntityMoveType(client) == MOVETYPE_NONE)
  {
    SetEntityMoveType(client, MOVETYPE_WALK);
  }
  return 1;
}

public Float:GetClientSpeed(client) //Current movespeed
{
  if(GetEntityMoveType(client) == MOVETYPE_NONE)  {
    return 0.0;
  }
  return GetEntDataFloat(client, offsSpeed);
}


public Action:SetClientColorT(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new color[4];
  color[0]=ReadPackCell(datapack);
  color[1]=ReadPackCell(datapack);
  color[2]=ReadPackCell(datapack);
  color[3]=ReadPackCell(datapack);
  if(IsClientInGame(client))
  if(IsPlayerAlive(client))
  SetClientColor(client,color);
}

public SetClientColor(client, color[4])
{
  SetColor(client, color[0], color[1], color[2], color[3]);
  new weapon =  GetEntDataEnt2(client, ACTIVEWEAPON);
  SetColor(weapon, color[0], color[1], color[2], color[3]);
}

stock bool:IsClearPath(Float:loc[3],Float:loc2[3],target=-1)
{
  decl Float:vector[3],Float:angles[3];
  MakeVectorFromPoints(loc, loc2, vector);
  GetVectorAngles(vector, angles);

  TR_TraceRayFilter(loc, angles, MASK_SOLID, RayType_Infinite,AimTargetFilter);
  new Float:endvec[3];
  TR_GetEndPosition(endvec);
  new endEntity = TR_GetEntityIndex();
  if((target >= 0) && (endEntity == target))
  return true;
  //now check if distance of ray is more or equal to distance between players
  //if so then we can say there is no wall between
  //That's because of the bug on linux servers :(
  new Float:rayDistance = GetDistanceBetween(loc, endvec);
  new Float:targetDistance = GetDistanceBetween(loc, loc2);
  if(rayDistance < (targetDistance-35.0))
  return false; //Then there is a solid object between
  return true;
}

stock GetAimTarget(client,Float:value, bool:smartHeal=false, bool:ignoreHP=false, Float:MAXDIST=0.0)
{

  decl Float:loc[3],Float:view[3];
  GetClientEyePosition(client,loc);
  GetClientEyeAngles(client, view);
  ClientTracer=client;
  new team = GetClientTeam(client);
  TR_TraceRayFilter(loc, view, MASK_SHOT, RayType_Infinite,AimTargetFilter);
  new target=TR_GetEntityIndex(INVALID_HANDLE);
  new targetPriority=0;
  if(smartHeal)
  target=0;
  if((target>0)&&(target<=GetMaxClients()))
  if(IsClientConnected(target))
  if(IsClientInGame(target) && IsPlayerAlive(target) && (! (IsMageInvis(target) || IsInvisible(target))))
  if(!HasStealth(target) || GetStealth(target) > 50 || smartHeal)
  return target;
  new Float:angelrec[2],Closest;
  angelrec[0]=181.0;
  angelrec[1]=181.0;
  for(new x=1;x<=GetMaxClients();x++)
  if(IsClientConnected(x) && IsClientInGame(x) && IsPlayerAlive(x)) if(x!=client)
  {
    //check for same teams for smartheal only
    if(smartHeal && (GetClientTeam(x)!=team))
    continue;
    //can't target invisible guys
    if( (GetClientTeam(x)!=team) && (IsMageInvis(x) || IsInvisible(x) ))
    continue;
    //Can't target high stealth
    if(!smartHeal && HasStealth(x) && GetStealth(x) < 50) continue;
    decl Float:loc2[3];
    decl Float:vector[3],Float:angles[3];
    GetClientEyePosition(x,loc2);
    MakeVectorFromPoints(loc, loc2, vector);
    GetVectorAngles(vector, angles);
    new Float:dif[2];
    dif[0]=angles[0]-view[0];
    dif[1]=angles[1]-view[1];
    if(dif[0]<0.0)
    dif[0]*=-1.0;
    if(dif[1]<0.0)
    dif[1]*=-1.0;
    if(dif[0]>180)
    dif[0]=360.0-dif[0];
    if(dif[1]>180)
    dif[1]=360.0-dif[1];

    if( (dif[0]>value) || (dif[1]>value) )
    continue;
    if(smartHeal) {
      if(targetPriority > GetHealPriority(x, ignoreHP))
      continue;
    }
    else  {
      if((dif[0]>angelrec[0]) && (dif[1]>angelrec[1]))
      continue; //if not smart heal, choose the one who is closer to center
    }


    TR_TraceRayFilter(loc, angles, MASK_SOLID, RayType_Infinite,AimTargetFilter);
    //new target2=TR_GetEntityIndex(INVALID_HANDLE);  - no longer used
    new Float:endvec[3];
    TR_GetEndPosition(endvec);
    //now check if distance of ray is more or equal to distance between players
    //if so then we can say there is no wall between
    //That's because of the bug on linux servers :(
    new Float:rayDistance = GetDistanceBetween(loc, endvec);
    new Float:targetDistance = GetDistanceBetween(loc, loc2);
    if(rayDistance < (targetDistance-13.0))
    continue; //Then there is a solid object between

    if(MAXDIST > 0.0 && MAXDIST < targetDistance)
    continue; //Target too far

    if(smartHeal) {
      targetPriority = GetHealPriority(x, ignoreHP);
      angelrec[0]=dif[0];
      angelrec[1]=dif[1];
      Closest=x;
    }
    else  {
      angelrec[0]=dif[0];
      angelrec[1]=dif[1];
      Closest=x;
    }
  }


  return Closest;
}

public bool:AimTargetFilter(entity,mask)
{
  return !(entity==ClientTracer);
}

public bool:WallTargetFilter(entity,mask) {
  return !(entity>0 && entity <= GetMaxClients());
}


public bool:NoPlayerFilter(entity,mask)
{
  return !(entity <= GetMaxClients() && entity > 0);
}

public bool:AimTargetFilter2(entity,mask)
{
  ReplyToCommand(dAimRunner,"filter found entity %i", entity);
  return !(entity==ClientTracer);
}

Float:GetSpellReduction(client,Float:pen)
{
  new Float:res=float(GetResistance(client))-pen;
  if(res>0.0)
  return (float(1) - (res / (res + ((1.6*float(GetLevel(client)))+6.0))));
  else if(res<0.0)
  return (float(1) + ( (-1.0*res) / ( (-1.0*res) + ((1.6*float(GetLevel(client)))+6.0))));
  return 1.0;
}

CalcHonor(rating) {
  rating-=1300;
  if(rating<0)
  rating=0;
  new points = ARENA_BASIC + RoundToNearest(Pow( float(rating) / ARENA_DIVIDER, 0.7));
  return points;
}

new g_ff_playerHonor[65];
GetHonor(client)  {
  return g_ff_playerHonor[client];
}

SetHonor(client,amount) {
  g_ff_playerHonor[client]=amount;
}
/*
%=res/res+3.5*lvl+6
%*(res+3.5*lvl+6)=res
res*% + 3.5*lvl*% + 6*% = res
(%-1)*res + 3.5*lvl*% + 6*%=0
res = (3.5*lvl*%+6*%)/(1-%)
*/

//The bigger - the higher
GetHealPriority(client, bool:ignoreHP=false) {
  new prio=1;
  new Float:hp =GetFloatHealth(client);
  if((!ignoreHP) && (hp>=0.98))
  return 0;
  if(HasShield(client))  //shield
  prio+=2;
  if(hp<0.8)  // <80%
  prio+=2;
  if(GetLevel(client)>=70)  // >= 70 level
  prio++;
  new bool:boss=IsBoss();
  if(boss)  {
    if(TopThreat(client, 2))
    prio++;
  }
  return prio;
}


ResetFloatArrays(client)  {
  for(new i=0; i<USERS_FLOAT_0; i++)
    wcUsersFloat0[client][i]=0.0;
  for(new i=0; i<USERS_FLOAT_1; i++)
    wcUsersFloat1[client][i]=1.0;

  g_fParryReduce[client] =0.0;
  //added since 2.0
  SetMinusCritDamageBonus(client, 0.0);
  SetMinusCritChanceBonus(client, 0.0);
}

stock Float:GetClientClipModifier(client) {
  return wcUsersFloat1[client][0];

}

stock SetClientClipModifier(client, Float:value)  {
  if(value<0.1)
  value=0.1;
  wcUsersFloat1[client][0]=value;
}


stock CharacterType:getCharacterType(client)  {
  //TODO make it smart ;-)
  switch(GetClass(client))  {
  case 1: if(GetBlockChance(client)>10.0) return Character_Tank;
  case 5: return Character_Healer;
  }
  return Character_DPS;
}

stock bool:IsInvisible(client) {
  return bVanish[client];
}

stock bool:CanDoubleHand(client)  {
  switch(GetClass(client))
  {
    case 2: return true;
    case 1: return true;
    case 6: 
    {
      new elemental = wcGetTreeTotal(client, getActiveSpec(client),1);
      new enhancement = wcGetTreeTotal(client, getActiveSpec(client),2);
      new restoration = wcGetTreeTotal(client, getActiveSpec(client),3);
      if(enhancement>elemental+restoration)
        return true;
    }
    case 8:
    {
      if(GetStance(client)==2)
        return true;
    }
    case 9:
    {
      //if(GetMonkStance(client)==2)
      return true;
    }
  }

  return false;
}

stock GetSpellCost(client, spellid) {
  new race = GetClass(client);
  new mana = wcGetSpellInfoInt(race, spellid, "mana");
  new Float:manainc = wcGetSpellInfoFloat(race, spellid, "manainc");
  new lvl = GetLevel(client);
  mana = RoundToNearest(float(mana) + manainc * lvl);

  switch(race)
  {
  case 3:
    {
      mana = wcRunTalent_3_9(client, mana);
      mana = wcRunTalent_3_ArcanePower2(client,mana);
    }
  case 5:
    {
      switch(spellid)
      {
      case 1:
        mana = wcRunTalent_5_13(client, mana);
      case 3:
        mana = wcRunTalent_5_13(client,mana);
      case 5:
        mana = wcRunTalent_5_14(client,mana);
      case 6:
        mana = wcRunTalent_5_14(client,mana);
      }
      wcRunTalent_5_FocusedWillEffect(client,mana);
    }
  case 6:
    switch(spellid)
    {
    case 3:
      {
        mana = wcRunTalent_6_13(client,mana);
      }
    case 4:
      {
        mana = wcRunTalent_6_13(client,mana);
      }
    case 5:
      {
        mana = wcRunTalent_6_13(client,mana);
      }
    case 6:
      mana = wcRunTalent_6_14(client,mana);
    case 9:
      mana = wcRunTalent_6_14(client,mana);
    case 10:
      mana = wcRunTalent_6_14(client,mana);
    case 11:
      mana = wcRunTalent_6_14(client,mana);
    }
  case 7:
    {
      switch(spellid)
      {
      case 3:
        {
          mana = wcRunTalent_7_LurkingShadows(client,mana);
        }
      case 7:
        {
          mana = wcRunTalent_7_LurkingShadows(client,mana);
        }
      }
    }
  }

  return mana;
}

stock GetTalentCost(client, tree,tier,cell) {
  new race = GetClass(client);
  decl String:line[255];
  wcGetTalentInfo(race,tree,tier,cell,client,"mana",line);
  new mana = StringToInt(line);
  wcGetTalentInfo(race,tree,tier,cell,client,"manainc",line);
  new Float:manainc=StringToFloat(line);
  new lvl = GetLevel(client);
  mana = RoundToNearest(float(mana) + manainc * lvl);

  if(race==5 && tree==3 && tier ==2 && cell == 1)
  mana = wcRunTalent_5_14(client, mana);
  else if(race==6 && tree==2 && tier ==2 && cell == 3)
  mana = wcRunTalent_6_14(client, mana);
  else if(race==6 && tree==3 && tier ==2 && cell == 2)
  mana = wcRunTalent_5_14(client, mana);
  else if(race==3)
  {
    mana = wcRunTalent_3_9(client,mana);
    mana = wcRunTalent_3_ArcanePower2(client,mana);
  }
  else if(race==5)
  {
    wcRunTalent_5_FocusedWillEffect(client,mana);
  }
  else if(race==7 && tree == 1 && tier==1 && cell==1)
  {
    mana = wcRunTalent_7_LurkingShadows(client,mana);
  }

  return mana;
}

bool:GetViewPoint(client,Float:distance,Float:end[3]) {
  if(client>0)   if(IsClientInGame(client))  {
    new Float:angle[3];
    GetClientEyeAngles(client,angle);
    new Float:endpos[3];
    new Float:startpos[3];
    GetClientEyePosition(client,startpos);
    new Float:dir[3];
    GetAngleVectors(angle, dir, NULL_VECTOR, NULL_VECTOR);
    ScaleVector(dir, distance);
    AddVectors(startpos, dir, endpos);
    new Float:endpos2[3];
    for(new i=0;i<3;i++)  {
      endpos2[i] = endpos[i];
    }
    ClientTracer=client;
    //TR_TraceRayFilter(startpos,endpos,MASK_PLAYERSOLID,RayType_EndPoint,AimTargetFilter);
    TR_TraceRayFilter(startpos,endpos,MASK_ALL,RayType_EndPoint,NoPlayerFilter);
    TR_GetEndPosition(endpos);
    new bool:same = true;
    for(new i=0; i<3; i++)
    if(endpos2[i] != endpos[i])
    same = false;
    distance = (GetVectorDistance(startpos, endpos) - 55.0);
    endpos[1] = (startpos[1] + (distance * Sine(DegToRad(angle[1]))));
    endpos[0] = (startpos[0] + (distance * Cosine(DegToRad(angle[1]))));
    end=endpos;
    return same;
  }
  return false;
}

Float:GetHasteEffect(client)
{
  //new level = GetLevel(client);
  //new Float:haste = wcClientsStats[client][HASTE] + wcClientsBonusStats[client][HASTE];
  //new Float:reduction = haste/(haste + (level*5) + 25);

  //  22haste = 10% cooldown, 100haste=33% cooldown
  new Float:cap = 0.35;
  new Float:haste = wcClientsStats[client][HASTE] + wcClientsBonusStats[client][HASTE];
  if (GetClass(client) == 3)
  {
    decl String:line[255];
    wcGetTalentInfo(3,3,2,3,client,"cap",line); //Mage Focused Mind
    cap   += StringToFloat(line);
  }

  new Float:reduction = haste/(haste + 198);
  if(GetStance(client)==3)
    reduction+=0.15;
  if (reduction>cap)
  {
    reduction=cap;
  }
  return reduction;
}

GetHaste(client)  {
  return RoundToNearest(wcClientsStats[client][HASTE] + wcClientsBonusStats[client][HASTE]);
}

AddHaste(client, Float:amount)  {
  wcClientsBonusStats[client][HASTE] += amount;
}

stock bool:IsValidShooting(attacker, victim)  {
  //Check if attacker is watching at victim, otherwise it's not
  if(!IsClientInGame(attacker) || !IsClientInGame(victim) || !IsPlayerAlive(attacker))  {
    return false;
  }
  new Float:angle[3];
  GetClientEyeAngles(attacker, angle);
  new Float:attPos[3],Float:vicPos[3],Float:attVector[3];
  GetClientEyePosition(attacker,attPos);
  GetClientEyePosition(victim,vicPos);
  new Float:dir[3];
  GetAngleVectors(angle, dir, NULL_VECTOR, NULL_VECTOR);
  ScaleVector(dir, 10.0);
  AddVectors(attPos, dir, attVector);

  return (GetDistanceBetween(attVector, vicPos) < GetDistanceBetween(attPos, vicPos));
}

GetTargetFOV(client, target, Float:value=180.0, bool:HorozontalOnly=false)
{
  if(!IsClientConnected(target) || !IsClientInGame(target) || !IsPlayerAlive(target))
  return false;

  if(target==client)
  return false;

  decl Float:loc[3], Float:loc2[3],Float:view[3];
  decl Float:vector[3],Float:angles[3];

  GetClientEyePosition(client,loc); //location of looker head
  GetClientEyeAngles(client, view); //viewing angles
  GetClientEyePosition(target,loc2);//location of victim head
  MakeVectorFromPoints(loc, loc2, vector); //vect between looker & victim
  GetVectorAngles(vector, angles);
  ClientTracer=client;
  TR_TraceRayFilter(loc, angles, MASK_SOLID, RayType_Infinite,WallTargetFilter);
  new Float:dif[2];
  dif[0]=angles[0]-view[0];
  dif[1]=angles[1]-view[1];
  if(dif[0]<0.0)
  dif[0]*=-1.0;
  if(dif[1]<0.0)
  dif[1]*=-1.0;
  if(dif[0]>180)
  dif[0]=360.0-dif[0];
  if(dif[1]>180)
  dif[1]=360.0-dif[1];
  if(HorozontalOnly) //no pitch needed.
  dif[0]=0.0;

  if( (dif[0]>(value/2.0)) || (dif[1]>(value/2.0)) )
  return false;

  new Float:endvec[3];
  TR_GetEndPosition(endvec);
  //now check if distance of ray is more or equal to distance between players
  //if so then we can say there is no wall between
  //That's because of the bug on linux servers :(
  new Float:rayDistance = GetDistanceBetween(loc, endvec);
  new Float:targetDistance = GetDistanceBetween(loc, loc2);
  if(rayDistance < (targetDistance-5.0))
  return false; //Then there is a solid object between
  return true; //must be able to see target
}


