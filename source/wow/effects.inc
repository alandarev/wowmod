/*
* File: effects.inc
* Description: Effects
* Author(s): Stinkyfax
*/



#pragma semicolon 1

#include <sdktools>
#include <clients>
new precache_laser;

new const String:g_sTotemHorde[] = "models/props/wow/h_totem/totem.mdl";
new const String:g_sTotemAlliance[] = "models/props/wow/a_totem/totem.mdl";
public PrecacheModels()
{
   //end of quake sounds
  PrecacheSound("buttons/button11.wav", true);
  PrecacheSound("buttons/blip1.wav", true);
  PrecacheSound("quake/play.wav");
  PrecacheSound("wc/arena/countdown.wav");
  PrecacheSound("wc/arena/HordeWin.wav");
  PrecacheSound("wc/arena/AliianceWin.wav");
  PrecacheSound("wc/levelupv2.wav");
  PrecacheSound("wc/spells/thunderclap.wav");
  PrecacheSound("wc/spells/absorb.wav");
  PrecacheSound("wc/spells/mindblast.wav");
  PrecacheSound("wc/spells/manaburn.wav");
  PrecacheSound("wc/spells/heal.wav");
  PrecacheSound("wc/spells/sprint.wav");
  PrecacheSound("wc/spells/vanish.wav");
  PrecacheSound("wc/spells/strike.wav");
  PrecacheSound("wc/spells/whirlwind.wav");
  PrecacheSound("wc/spells/blink.wav");
  PrecacheSound("wc/spells/arcexpl.wav");
  PrecacheSound("wc/spells/shieldwall.wav");
  PrecacheSound("wc/spells/rend.wav");
  PrecacheSound("wc/spells/frostbolt.wav");
  PrecacheSound("wc/spells/warriorweaponswing.wav");
  PrecacheSound("wc/spells/fireball.wav");
  PrecacheSound("wc/spells/curse.wav");
  PrecacheSound("wc/spells/shadow.wav");
  PrecacheSound("wc/spells/fear.wav");
  PrecacheSound("wc/spells/resurrect.wav");
  //paladin sounds
  PrecacheSound("wc/spells/seal.wav");
  PrecacheSound("wc/spells/judge.wav");
  PrecacheSound("wc/spells/divine.wav");
  //80 lvl spells sounds
  PrecacheSound("wc/spells/hellfire.wav");
  PrecacheSound("wc/spells/hymn.wav");
  PrecacheSound("wc/spells/shadowtouch.wav");


  //2.25 version sounds
  PrecacheSound("wc/spells/divinestorm.wav");
  PrecacheSound("wc/spells/eyeforaneye.wav");
  PrecacheSound("wc/spells/avengersshield.wav");
  PrecacheSound("wc/spells/holyshock.wav");
  PrecacheSound("wc/spells/shamanisticrage.wav");
  PrecacheSound("wc/spells/chaosbolt.wav");
  PrecacheSound("wc/spells/metamorphosis.wav");
  PrecacheSound("wc/spells/painsuppression.wav");
  PrecacheSound("wc/spells/invisibility.wav");
  PrecacheSound("wc/spells/focusedmind.wav");
  PrecacheSound("wc/spells/livingbomb.wav");
  PrecacheSound("wc/spells/frostbite.wav");
  PrecacheSound("wc/spells/adrenalinerush.wav");
  PrecacheSound("wc/spells/hackandslash.wav");
  PrecacheSound("wc/spells/cheatdeath.wav");
  PrecacheSound("wc/spells/livingbomb2.wav");
  PrecacheSound("ambient/levels/prison/inside_battle_zombie1.wav");

  //2.50 version sounds
  PrecacheSound("wc/spells/presence_cast.wav"); //Presence Stances
  PrecacheSound("wc/spells/runes_regen.wav");
  PrecacheSound("wc/spells/dk_strike.wav");
  PrecacheSound("wc/spells/deathgrip.wav");
  PrecacheSound("wc/spells/icytouch.wav");
  PrecacheSound("wc/spells/deathcoil.wav");
  PrecacheSound("wc/spells/mindfreeze.wav");
  PrecacheSound("wc/spells/raiseally.wav");
  PrecacheSound("wc/spells/runetap.wav");
  PrecacheSound("wc/spells/vampiric.wav");
  PrecacheSound("wc/spells/hungeringcold.wav");
  PrecacheSound("wc/spells/butchery.wav");
  PrecacheSound("wc/spells/antimagic.wav");
  PrecacheSound("wc/spells/heartfire.wav");

  PrecacheSound("wc/spells/boss_enrage.wav");

  PrecacheSound("wc/spells/sheep.wav");

  //4.0 version precaches
  PrecacheSound("wc/spells/crusader.wav");
  PrecacheSound("wc/spells/iceblock.wav");
  PrecacheSound("wc/spells/flameorb.wav");
  PrecacheSound("wc/spells/flameorb2.wav");
  PrecacheSound("wc/spells/blind.wav");
  PrecacheSound("wc/spells/fanofknives.wav");
  PrecacheSound("wc/spells/infernal.wav");
  PrecacheSound("wc/spells/necroticstrike.wav");
  PrecacheSound("wc/spells/deathanddecay.wav");
  PrecacheSound("wc/spells/blizzard.wav");
  PrecacheSound("wc/spells/arcanemissile.wav");
  PrecacheSound("wc/spells/clearcast.wav");
  //PrecacheSound("wc/spells/aoeshield.wav");
  PrecacheSound("wc/spells/lightwell.wav");
  PrecacheSound("wc/spells/mindflay.wav");
  PrecacheSound("wc/spells/airshock.wav");
  PrecacheSound("wc/spells/lavasurge.wav");
  PrecacheSound("wc/spells/execute.wav");

  //Hani sounds
  //PrecacheSound("wc/spells/corpseparty.wav");
  PrecacheSound("wc/spells/shadowgate.wav");
  PrecacheSound("wc/spells/shadowstep.wav");
  PrecacheSound("wc/spells/shadowgate2.wav");
  PrecacheSound("wc/spells/colossussmash.wav");
  PrecacheSound("wc/spells/LightningBolt.wav");
  PrecacheSound("wc/spells/tidetotem.wav");
  PrecacheSound("wc/spells/obliterate.wav");
  PrecacheSound("wc/spells/corpseexplosionnew.wav");
  PrecacheSound("wc/spells/soulharvest.wav");
  PrecacheSound("wc/spells/taunt.wav");
  PrecacheSound("wc/spells/Shield_of_the_Righteous.wav");

  //Monk sounds
  PrecacheSound("wc/spells/expelharm1.wav");
  PrecacheSound("wc/spells/touchofkarma.wav");
  PrecacheSound("wc/spells/revival.wav");
  PrecacheSound("wc/spells/blossom1.wav");
  PrecacheSound("wc/spells/blossom2.wav");
  PrecacheSound("wc/spells/fireblossom1.wav");
  PrecacheSound("wc/spells/fireblossom2.wav");
  PrecacheSound("wc/spells/fireblossom3.wav");
  PrecacheSound("wc/spells/jab.wav");
  PrecacheSound("wc/spells/windwalker_stance.wav");
  PrecacheSound("wc/spells/brewmaster_stance.wav");
  PrecacheSound("wc/spells/mistweaver_stance.wav");
  PrecacheSound("wc/spells/stancehealmonk.wav");
  PrecacheSound("wc/spells/fistsoffury.wav");
  PrecacheSound("wc/spells/burb1.wav");
  PrecacheSound("wc/spells/burb2.wav");
  PrecacheSound("wc/spells/burb3.wav");
  PrecacheSound("wc/spells/burb4.wav");
  PrecacheSound("wc/spells/burb5.wav");
  PrecacheSound("wc/spells/chiexplosion.wav");
  PrecacheSound("wc/spells/serpentland.wav");
  PrecacheSound("wc/spells/serpentjump.wav");
  PrecacheSound("wc/spells/kegsmash.wav");
  PrecacheSound("wc/spells/tigerpalm.wav");
  PrecacheSound("wc/spells/chiburstheal.wav");
  PrecacheSound("wc/spells/chiburstdmg.wav");
  PrecacheSound("wc/spells/chiburstcast.wav");
  PrecacheSound("wc/spells/guard.wav");
  PrecacheSound("wc/spells/breathfire.wav");
  PrecacheSound("wc/spells/Parry.wav");
  PrecacheSound("wc/spells/dizzyinghaze.wav");
  PrecacheSound("wc/spells/cursedsoul.wav");
  PrecacheRanks();
  PrecacheHP();

  if(CSGO)  PrecacheModel("models/chicken/chicken.mdl");
  if(CSGO)  PrecacheCSGO();
  else      PrecacheCSS();
}



PrecacheHP()  {
  for(new i=0; i < sizeof(g_mHp); i++)  {
    PrecacheModel(g_mHp[i]);
    AddFileToDownloadsTable(g_mHp[i]);
    decl String:path2[255];
    strcopy(path2, sizeof(path2), g_mHp[i]);
    ReplaceString(path2, sizeof(path2), ".vmt", ".vtf");
    AddFileToDownloadsTable(path2);
  }
}

PrecacheCSS()	{

  PrecacheModel("particle/fire.vmt");
  SWD_sprite=PrecacheModel("models/effects/splodeglass.mdl"); //ShadowWord Death Priest
  ShieldSprite=PrecacheModel("sprites/strider_blackball.vmt"); //PowerWordShield Priest
  ChaosSprite=PrecacheModel("models/manhack/blur01.vmt"); // Chaos Bolt Sprite
  g_ef_playerredsmall = PrecacheModel("sprites/player_red_small.vmt");
  g_ef_tpbeam001 = PrecacheModel("sprites/tp_beam001.vmt");
  g_ef_bluecore = PrecacheModel("sprites/physcannon_bluecore2b.vmt");
  g_ef_hydragutbeampoison = PrecacheModel("sprites/hydragutbeam.vmt");
  g_ef_smoke = PrecacheModel("sprites/smoke.vmt");
  g_ef_tp_beam001 = PrecacheModel("sprites/tp_beam001.vmt");
  g_ef_old_xfire = PrecacheModel("sprites/old_xfire.vmt");
  g_ef_glow1 = PrecacheModel("sprites/glow1.vmt");
  g_ef_glow01 = PrecacheModel("sprites/glow01.vmt");
  g_ef_lgtning = PrecacheModel("sprites/lgtning.vmt");
  g_ef_airlock_laser = PrecacheModel("models/props_lab/airlock_laser.vmt");
  g_ef_ocean = PrecacheModel("sprites/blueshaft1.vmt");
  g_bloodModel = PrecacheModel("sprites/blood.vmt");
  g_ef_shellchrome = PrecacheModel("sprites/shellchrome.vmt");
  g_bloodsprayModel = PrecacheModel("sprites/bloodspray.vmt");
  //g_ar2_altfire1 = PrecacheModel("effects/ar2_altfire1.vmt"); - bugged in orange box
  g_redglow1 = PrecacheModel("sprites/redglow1.vmt");
  g_blueglow1 = PrecacheModel("sprites/blueglow1.vmt");
  g_plasmaember = PrecacheModel("sprites/plasmaember.vmt");
  g_purplelaser1 = PrecacheModel("sprites/purplelaser1.vmt");
  g_ef_yellowflare = PrecacheModel("effects/yellowflare.vmt");
  
  g_ef_physring1 = PrecacheModel("sprites/physring1.vmt");
  g_ef_flatflame = PrecacheModel("sprites/flatflame.vmt");
  g_mdl_totem_h = PrecacheModel("models/props/wow/h_totem/totem.mdl");
  g_mdl_totem_a = PrecacheModel("models/props/wow/a_totem/totem.mdl");
  g_plasmabeam = PrecacheModel ("materials/sprites/plasmabeam.vmt");
  g_bluelight1 = PrecacheModel ("materials/sprites/bluelight1.vmt");
  precache_laser=PrecacheModel("materials/sprites/laserbeam.vmt");
  g_ef_laserbeam=PrecacheModel("materials/sprites/laserbeam.vmt");

  g_ef_splodesprite = PrecacheModel("materials/sprites/splodesprite.vmt");
  g_ef_bluelight = PrecacheModel ("materials/sprites/physcannon_bluelight1b.vmt");
  g_ef_muzzleflashX = PrecacheModel ("materials/effects/muzzleflashX.vmt");
  g_ef_animglow = PrecacheModel ("materials/sprites/animglow01.vmt");
  g_ef_scanner = PrecacheModel ("materials/sprites/scanner_bottom.vmt");
  g_ef_fire1 = PrecacheModel ("materials/sprites/fire1.vmt");
  g_ef_fire2 = PrecacheModel ("materials/sprites/fire2.vmt");
  g_ef_train = PrecacheModel ("sprites/640_train.vmt");
  //g_ef_glow = PrecacheModel ("materials/sprites/glow");
  g_ef_rollermine = PrecacheModel ("materials/models/Roller/rollermine_glow.vmt");
  g_ef_hydraspinalcord = PrecacheModel ("materials/sprites/hydraspinalcord.vmt");
  g_ef_overlay = PrecacheModel ("sun/overlay.vmt");
  g_ef_striderbulgedudvdx60 = PrecacheModel("effects/strider_bulge_dudv_dx60.vmt");
  g_ef_rollerglow = PrecacheModel("effects/rollerglow.vmt");
  g_ef_hydragutbeam = PrecacheModel("effects/blueblacklargebeam.vmt");
  g_ef_blueblackflash = PrecacheModel("blueblackflash");
  g_ef_fire01 = PrecacheModel("sprites/sprite_fire01.vmt");
  g_ef_xbeam2 = PrecacheModel("sprites/xbeam2.vmt");
  g_ef_striderdarkflare = PrecacheModel("effects/strider_dark_flare.vmt");
  g_ef_particle_noisesphere = PrecacheModel ("materials/particle/particle_noisesphere.vmt");
  g_ef_trail_blue = PrecacheModel("materials/sprites/combineball_trail_blue_1.vmt");
  g_ef_combineball = PrecacheModel("models/Effects/combineball.mdl");
  g_ef_yellowflare1 = PrecacheModel("materials/sprites/yelflare1.vmt");
  g_ef_orangeflare1 = PrecacheModel("materials/sprites/orangeflare1.vmt");
  g_ef_orangelight1 = PrecacheModel("materials/sprites/orangelight1.vmt");
  g_ef_yellowglow1 = PrecacheModel("materials/sprites/yellowglow1.vmt");
  g_ef_WXplo1 = PrecacheModel("materials/sprites/WXplo1.vmt");
  g_ef_greenglow1 = PrecacheModel("materials/sprites/greenglow1.vmt");
  //g_ef_firecloud2 = PrecacheModel("materials/effects/fire_cloud2.vmt");
  PrecacheModel("effects/bluespark.vmt");

  //ctf
  g_ctf_horde    = PrecacheModel("models/cf_models_src/ctf_80rd3.mdl");
  g_ctf_alliance = PrecacheModel("models/cf_models_src/ctf_41114nc3.mdl");

  //CS:GO Chicken


  //DK
  AMSSprite=PrecacheModel("sprites/plasmaember.vmt");
  //HaloSprite=PrecacheModel("sprites/halo01.vmt");
  g_HaloSprite = PrecacheModel("materials/sprites/halo01.vmt");
}

PrecacheCSGO()	{
  g_ef_physring1 = PrecacheModel("sprites/physring1.vmt");
  PrecacheModel("particle/star_ob_noz.vmt.vmt");
  SWD_sprite=PrecacheModel("models/effects/splodeglass.mdl"); //ShadowWord Death Priest
  ShieldSprite=PrecacheModel("sprites/strider_blackball.vmt"); //PowerWordShield Priest
  ChaosSprite=PrecacheModel("sprites/crystal_beam1.vmt"); // Chaos Bolt Sprite
  g_ef_playerredsmall = PrecacheModel("sprites/player_red_small.vmt");
  g_ef_tpbeam001 = PrecacheModel("sprites/tp_beam001.vmt");
  g_ef_bluecore = PrecacheModel("sprites/physcannon_bluecore1b.vmt");
  g_ef_hydragutbeampoison = PrecacheModel("sprites/hydragutbeam.vmt");
  g_ef_smoke = PrecacheModel("sprites/smoke.vmt");
  g_ef_tp_beam001 = PrecacheModel("sprites/tp_beam001.vmt");
  g_ef_old_xfire = PrecacheModel("sprites/xfireball3.vmt");
  g_ef_glow1 = PrecacheModel("sprites/glow.vmt");
  g_ef_glow01 = PrecacheModel("sprites/glow01.vmt");
  g_ef_lgtning = PrecacheModel("sprites/lgtning.vmt");
  g_ef_airlock_laser = PrecacheModel("sprites/laserbeam.vmt");
  g_ef_ocean = PrecacheModel("effect/splash4.vmt");
  g_bloodModel = PrecacheModel("sprites/zerogxplode.vmt");
  g_ef_shellchrome = PrecacheModel("sprites/shellchrome.vmt");
  g_bloodsprayModel = PrecacheModel("sprites/bloodspray.vmt");
  //g_ar2_altfire1 = PrecacheModel("effects/ar2_altfire1.vmt"); - bugged in orange box
  g_redglow1 = PrecacheModel("sprites/ledglow.vmt");
  g_blueglow1 = PrecacheModel("sprites/blueglow1.vmt");
  g_plasmaember = PrecacheModel("sprites/plasmaember.vmt");
  g_purplelaser1 = PrecacheModel("sprites/purplelaser1.vmt");
  g_ef_yellowflare = PrecacheModel("effects/yellowflare.vmt");

  g_ef_flatflame = PrecacheModel("sprites/flatflame.vmt");
  g_mdl_totem_h = PrecacheModel("models/props/wow/h_totem/totem.mdl");
  g_mdl_totem_a = PrecacheModel("models/props/wow/a_totem/totem.mdl");
  g_plasmabeam = PrecacheModel ("sprites/plasma1.vmt");
  g_bluelight1 = PrecacheModel ("sprites/bluelight1.vmt");
  precache_laser=PrecacheModel("sprites/laserbeam.vmt");
  g_ef_laserbeam=PrecacheModel("materials/sprites/laserbeam.vmt");

  g_ef_splodesprite = PrecacheModel("particle/particle_flares/particle_flare_004.vmt");
  g_ef_bluelight = PrecacheModel ("particle/beam_taser.vmt");
  g_ef_muzzleflashX = PrecacheModel ("effects/muzzleflashX.vmt");
  g_ef_animglow = PrecacheModel ("spritres/glow04.vmt");
  g_ef_scanner = PrecacheModel ("sprites/scanner_bottom.vmt");
  g_ef_fire1 = PrecacheModel ("sprites/fire.vmt");
  g_ef_fire2 = PrecacheModel ("sprites/fire1.vmt");
  g_ef_train = PrecacheModel ("vgui/hud/icons_arrow_plain.vmt");
  //g_ef_glow = PrecacheModel ("materials/sprites/glow");
  g_ef_rollermine = PrecacheModel ("particle/particle_glow_03.vmt");
  g_ef_hydraspinalcord = PrecacheModel ("sprites/cbbl_smoke.vmt");
  g_ef_overlay = PrecacheModel ("sun/overlay.vmt");
  g_ef_striderbulgedudvdx60 = PrecacheModel("effects/strider_bulge_dudv_dx60.vmt");
  g_ef_rollerglow = PrecacheModel("effects/rollerglow.vmt");
  g_ef_hydragutbeam = PrecacheModel("effects/blueblacklargebeam.vmt");
  g_ef_blueblackflash = PrecacheModel("sprites/blueflare1.vmt");
  g_ef_fire01 = PrecacheModel("sprites/fire1.vmt");
  g_ef_xbeam2 = PrecacheModel("sprites/tp_beam001.vmt");
  g_ef_striderdarkflare = PrecacheModel("particle/particle_flares/aircraft_white.vmt");
  g_ef_particle_noisesphere = PrecacheModel ("particle/particle_noisesphere.vmt");
  g_ef_trail_blue = PrecacheModel("effects/bluespark.vmt");
  g_ef_yellowflare1 = PrecacheModel("materials/sprites/yelflare1.vmt");
  g_ef_combineball = PrecacheModel("effects/fire_cloud1");
  g_ef_orangeflare1 = PrecacheModel("effects/yellowflare_noz.vmt");
  g_ef_orangelight1 = PrecacheModel("sprites/orangelight1.vmt");
  g_ef_yellowglow1 = PrecacheModel("sprites/glow03.vmt");
  g_ef_WXplo1 = PrecacheModel("prites/zerogxplode.vmt");
  g_ef_greenglow1 = PrecacheModel("particle/particle_flares/aircraft_green.vmt");
  //g_ef_firecloud2 = PrecacheModel("materials/effects/fire_cloud2.vmt");
  PrecacheModel("effects/bluespark.vmt");

  //ctf
  g_ctf_horde    = PrecacheModel("models/cf_models_src/ctf_80rd3.mdl");
  g_ctf_alliance = PrecacheModel("models/cf_models_src/ctf_41114nc3.mdl");

  //CS:GO Chicken


  //DK
  AMSSprite=PrecacheModel("sprites/plasmaember.vmt");
  g_HaloSprite = PrecacheModel("sprites/halo01.vmt");
}


/*
  {255,   0,   0, 255} //red
  {100, 100, 100, 255} //white
  {  0,   0, 255, 255} //blue
  {127, 255,   0, 255} //green
  {255, 255,   0, 255} //yellow
*/
stock DrawLine(Float:loc1[3], Float:loc2[3],color[4]={100, 100, 100, 255})  {
  BeamEffect("@all",loc1,loc2,g_ef_tp_beam001,5,20.0,7.0,10.0,color,0.0,5);
}

public bool:ExplosionCreate(const Float:origin[3],const Float:angles[3],owner,magnitude,radius,spawnflags,Float:force,inflicter)
{
  SDKCall(hExplosionCreate,origin,angles,owner,magnitude,radius,spawnflags,force,inflicter);
  return true;
}

public SetHMT(client,Float:silence)
{
   wcClientHMT[client]=GetTickedTime()+silence;
}

public Float:GetHMT(client)
{
   return wcClientHMT[client];
}

public SetHM(client,Float:multi)
{
   wcClientHM[client]=multi;
}

public Float:GetHM(client)
{
   return wcClientHM[client];
}

public SetSealT(client,Float:silence)
{
   wcClientSealT[client]=GetTickedTime()+silence;
}

public Float:GetSealT(client)
{
   return wcClientSealT[client];
}

public SetSeal(client,seal)
{
   wcClientSeal[client]=seal;
}

public getSealsString(client, String:buffer[], length) {
  Format(buffer, length, "NO SEALS - type 'seals' in chat for more info");
  new bool:first=true;
  if(HasSeal(client, Seal:SEAL_Light))  {
    if(first) {
      first=false;
      Format(buffer,length,"");
    }
    Format(buffer,length,"%s Light", buffer);
  }
  if(HasSeal(client, Seal:SEAL_Wisdom))  {
    if(first) {
      first=false;
      Format(buffer,length,"");
    }
    Format(buffer,length,"%s Wisdom", buffer);
  }
  if(HasSeal(client, Seal:SEAL_Justice))  {
    if(first) {
      first=false;
      Format(buffer,length,"");
    }
    Format(buffer,length,"%s Justice", buffer);
  }
  if(HasSeal(client, Seal:SEAL_Righteousness))  {
    if(first) {
      first=false;
      Format(buffer,length,"");
    }
    Format(buffer,length,"%s Righteous", buffer);
  }
  if(HasSeal(client, Seal:SEAL_Crusader))  {
    if(first) {
      first=false;
      Format(buffer,length,"");
    }
    Format(buffer,length,"%s Crusader", buffer);
  }
  if(HasSeal(client, Seal:SEAL_Command))  {
    if(first) {
      first=false;
      Format(buffer,length,"");
    }
    Format(buffer,length,"%s Command", buffer);
  }
  if(HasSeal(client, Seal:SEAL_Truth))  {
    if(first) {
      first=false;
      Format(buffer,length,"");
    }
    Format(buffer,length,"%s Truth", buffer);
  }
  if(HasSeal(client, Seal:SEAL_Blood))  {
    if(first) {
      first=false;
      Format(buffer,length,"");
    }
    Format(buffer,length,"%s Blood", buffer);
  }
  if(HasSeal(client, Seal:SEAL_Insight))  {
    if(first) {
      first=false;
      Format(buffer,length,"");
    }
    Format(buffer,length,"%s Insight", buffer);
  }
}

bool:HasSeal(client, any:module) {
  if(module == SEAL_Any)
    return _getSeal(client)>0 ;
  return ( (_getSeal(client) & (1<<module))==(1<<module));
}

public SetSeals(holy, protection, retribution)  {
  if(holy>0 && protection == 0 && retribution == 0)
    holy+=5;
  else if(protection > 0 && holy == 0 && retribution == 0)
    retribution+=5;
  else if(retribution >0 && protection == 0 && holy == 0)
    retribution+=5;
  new seals = 0;
  if(holy>=6)
    seals = seals | 1 << any:SEAL_Light;
  if(holy>=11)
    seals = seals | 1 << any:SEAL_Wisdom;
  if(holy>=16)
    seals = seals | 1 << any:SEAL_Insight;
  if(protection>=6)
    seals = seals | 1 << any:SEAL_Justice;
  if(protection>=11)
    seals = seals | 1 << any:SEAL_Righteousness;
  if(protection>=16)
    seals = seals | 1 << any:SEAL_Truth;
  if(retribution>=6)
    seals = seals | 1 << any:SEAL_Crusader;
  if(retribution>=11)
    seals = seals | 1 << any:SEAL_Command;
  if(retribution>=16)
    seals = seals | 1 << any:SEAL_Blood;
  return seals;
}

public _getSeal(client)
{
   return wcClientSeal[client];
}

//SetSealP(client, power)
//{
//  wcClientSealP[client]=power;
//}

//GetSealP(client)
//{
//  return wcClientSealP[client];
//}

stock SetSilence(client,Float:silence,bool:trueSilence=false)
{
  if(IsBoss() && (GetBoss(client)>0))
    return;
  if(!trueSilence)
    switch(GetClass(client))  {
      case 1: return;
      //case 2: return;
  }
  new bool:avoid=true;
  if(!trueSilence && GetClass(client)==3)
  {
    decl String:line[255];
    wcGetTalentInfo(3,3,1,3,client,"chance",line);
    new avoidchance = StringToInt(line);
    new random=GetRandomInt(1,100);
    if(avoidchance >= random)
    {
      avoid = false;
      WCMessage(client,"avoidsilence",client);
    }
  }
  else if(!trueSilence && GetClass(client)==6)
  {
    decl String:line[255];
    wcGetTalentInfo(6,1,2,4,client,"chance",line);
    new avoidchance = StringToInt(line);
    new random=GetRandomInt(1,100);
    if(avoidchance >= random)
    {
      avoid = false;
      WCMessage(client,"avoidsilence",client);
    }

  }
  else   if(!trueSilence && GetClass(client)==4)
  {
    decl String:line[255];
    wcGetTalentInfo(4,1,2,4,client,"chance",line);
    new avoidchance = StringToInt(line);
    new random=GetRandomInt(1,100);
    if(avoidchance >= random)
    {
      avoid = false;
      WCMessage(client,"avoidsilence",client);
    }
  }
  if(avoid)
    PlayerSilence[client]=GetTickedTime()+silence;
}

public Float:GetSilence(client)
{
   return PlayerSilence[client];
}

public bool:FadeEffect(client,type,time,duration,const color[4])
{
  if(client)
  {
    if(CSGO)
    {
      new Handle:hBf=StartMessageOne("Fade",client);
      //new Handle:hBf=StartMessageEx("Fade", client, 1);
      if(hBf!=INVALID_HANDLE)
      {
        PbSetInt(hBf, "duration", duration); //Time it takes to fade 
        PbSetInt(hBf, "hold_time", time); //How long it stays
        PbSetInt(hBf, "flags", (FFADE_PURGE|FFADE_IN));
        PbSetColor(hBf, "clr", color);
      }
      else
        return false;
    }
    else
    {
      new Handle:hBf=StartMessageOne("Fade",client);
      if(hBf!=INVALID_HANDLE)
      {
        BfWriteShort(hBf,duration);
        BfWriteShort(hBf,time);
        BfWriteShort(hBf,type);
        BfWriteByte(hBf,color[0]);
        BfWriteByte(hBf,color[1]);
        BfWriteByte(hBf,color[2]);
        BfWriteByte(hBf,color[3]);
      }
    }
    EndMessage();
    return true;
  }
  return false;
}

public bool:BeamEffect(const String:matchstr[],Float:startvec[3],Float:endvec[3],precache_model,framerate,Float:life,Float:width,Float:endwidth,const color[4],Float:amplitude,speed)
{
  new clients[MAXPLAYERS];
  new count=FindMatchingPlayers(matchstr,clients);
  TE_SetupBeamPoints(startvec,endvec,precache_model,0,0,framerate,life,width,endwidth,0,amplitude,color,speed);
  TE_Send(clients,count);
  return true;
}

public bool:MetalSparkEffect(const String:matchstr[], const Float:pos[3], const Float:dir[3])
{
  new clients[MAXPLAYERS];
  new count=FindMatchingPlayers(matchstr,clients);
  TE_SetupMetalSparks(pos, dir);
  TE_Send(clients,count);
  return true;
}

public bool:BeamRingEffect(const String:matchstr[],Float:centervec[3],Float:startradius,Float:endradius,precache_model,startframe,framerate,Float:life,Float:width,const color[4],Float:amplitude,speed)
{
  new clients[MAXPLAYERS];
  new count=FindMatchingPlayers(matchstr,clients);
  TE_SetupBeamRingPoint(centervec,startradius,endradius,precache_model,0,startframe,framerate,life,width,amplitude,color,speed,0);
  TE_Send(clients,count);
  return true;
}

stock bool:GlowSpriteEffect(const String:matchstr[],const Float:pos[3], Model, Float:Life, Float:Size, Brightness, String:sModel[]="")
{
  new clients[MAXPLAYERS];
  new count=FindMatchingPlayers(matchstr,clients);
  if(Model < 0)
    Model = PrecacheModel(sModel);
  TE_SetupGlowSprite(pos, Model, Life, Size, Brightness);
  TE_Send(clients,count);
  return true;
}

public bool:BubblesEffect(const String:matchstr[], const Float:m_vecMins[3], const Float:m_vecMaxs[3],
                           Model, Float:height, amount, Float:speed) {
   new clients[MAXPLAYERS];
   new count=FindMatchingPlayers(matchstr,clients);
   TE_Start("Bubbles");
   TE_WriteVector("m_vecMins", m_vecMins);
   TE_WriteVector("m_vecMaxs", m_vecMaxs);
   TE_WriteNum("m_nModelIndex", Model);
   TE_WriteFloat("m_fHeight", height);
   TE_WriteNum("m_nCount", amount);
   TE_WriteFloat("m_fSpeed", speed);
   TE_Send(clients, count, 0.0);
   return true;
}

public bool:SmokeEffect(const String:matchstr[], const Float:pos[3], Model, Float:Scale, FrameRate)
{
  new clients[MAXPLAYERS];
  new count=FindMatchingPlayers(matchstr,clients);
  TE_SetupSmoke(pos, Model, Scale, FrameRate);
  TE_Send(clients,count);
  return true;
}

public bool:BeamFollowEffect(const String:matchstr[],index,Float:life,Float:width,Float:endwidth,const color[4])
{
  new clients[MAXPLAYERS];
  new count=FindMatchingPlayers(matchstr,clients);
  TE_SetupBeamFollow(index,precache_laser,0,life,width,endwidth,1,color);
  TE_Send(clients,count);
  return true;
}
/*
public bool:FourBeamEffect(client,const String:matchstr[],Float:life,Float:height,Float:width,Float:endwidth,Float:beamwidth,color[4],Float:amplitude,speed)
{

 es es_xgetplayerlocation wcs_x1 wcs_y1 wcs_z1 event_var(userid)
es est_effect 10 #a 0 sprites/lgtning.vmt server_var(wcs_x1) server_var(wcs_y1) server_var(wcs_z1) 10 500 2 100 300 0 75 75 255 255 10
es est_effect 10 #a 0 sprites/lgtning.vmt server_var(wcs_x1) server_var(wcs_y1) server_var(wcs_z1) 10 500 2 100 300 0 75 75 255 255 10
es est_effect 10 #a 0.1 sprites/lgtning.vmt server_var(wcs_x1) server_var(wcs_y1) server_var(wcs_z1) 10 500 2 100 300 0 75 75 255 255 10
es est_effect 10 #a 0.2 sprites/lgtning.vmt server_var(wcs_x1) server_var(wcs_y1) server_var(wcs_z1) 10 500 2 100 300 0 75 75 255 255 1
es est_effect 10 #a 0.3 sprites/lgtning.vmt server_var(wcs_x1) server_var(wcs_y1) server_var(wcs_z1) 10 500 1 100 300 .8 175 175 255 255 1
es est_effect 10 #a 0.4 sprites/lgtning.vmt server_var(wcs_x1) server_var(wcs_y1) server_var(wcs_z1) 10 500 0.5 100 300 .8 75 75 255 255 1

  new Float:indexloc[3],Float:point1[3],Float:point2[3],Float:point3[3],Float:point4[3],Float:point5[3],Float:point6[3],Float:point7[3],Float:point8[3];
  GetClientAbsOrigin(client,indexloc);
  indexloc[2]-=30;
  point1[0]=indexloc[0]+width;
  point1[1]=indexloc[1];
  point1[2]=indexloc[2];
  point2[0]=indexloc[0]-width;
  point2[1]=indexloc[1];
  point2[2]=indexloc[2];
  point3[0]=indexloc[0];
  point3[1]=indexloc[1]+width;
  point3[2]=indexloc[2];
  point4[0]=indexloc[0];
  point4[1]=indexloc[1]-width;
  point4[2]=indexloc[2];
  point5[0]=indexloc[0]+endwidth;
  point5[1]=indexloc[1];
  point5[2]=indexloc[2]+height;
  point6[0]=indexloc[0]-endwidth;
  point6[1]=indexloc[1];
  point6[2]=indexloc[2]+height;
  point7[0]=indexloc[0];
  point7[1]=indexloc[1]+endwidth;
  point7[2]=indexloc[2]+height;
  point8[0]=indexloc[0];
  point8[1]=indexloc[1]-endwidth;
  point8[2]=indexloc[2]+height;
  BeamEffect(matchstr,point1,point5,life,beamwidth,beamwidth,color,amplitude,speed);
  BeamEffect(matchstr,point2,point6,life,beamwidth,beamwidth,color,amplitude,speed);
  BeamEffect(matchstr,point3,point7,life,beamwidth,beamwidth,color,amplitude,speed);
  BeamEffect(matchstr,point4,point8,life,beamwidth,beamwidth,color,amplitude,speed);
  return true;
}
*/
public Effect2(client)//bloodthirst
{
  if( (--g_iBT[client]) < 0)
    return;
  decl String:line[255];
  wcGetSpellInfo(1,3,"amount",line);
  new amount=StringToInt(line);
  wcGetSpellInfo(1,3,"multi",line);
  new Float:multi=StringToFloat(line);
  amount=RoundToNearest(float(GetStr(client)+GetBonusStr(client))*multi)+amount;
  //wcGetTalentInfo(1,1,2,1,client,"multi",line); //inaccurate
  amount+= RoundToNearest(amount*StringToFloat(line));
  new maxhp=GetMaxHealth(client);
  new hp=wcGetHealth(client);
  hp+=amount;
  if(maxhp<hp)
    hp=maxhp;
  wcSetHealth(client,hp);
  WCMessage(client,"bloodthirst used",client,amount);
}

public Effect3(client,&dmg, attacker)
{
  decl String:line[255];
  wcGetSpellInfo(2,1,"chance",line);
  new chance=StringToInt(line);
  if(GetAdrenaline(attacker))
    chance = 0;
  if(GetRandomInt(1,100)<=chance)
  {
    /*decl String:buffer[30];*/
    /*Format(buffer,sizeof(buffer),"%T", "evade used", client);*/
    AddEMsgAff("evade used", true);
    dmg=0;
  }
}
public Effect54(client,&dmg,victim) //Curse miss
{
  if(GetMiss(victim)>0)
  {
    AddEMsgAff("missed", true);
    dmg=0;
    SetMiss(victim, GetMiss(victim)-1);
  }
}

public Effect1(userid,attacker)
{
  new random=GetRandomInt(1,100);
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"1");
  new chance=KvGetNum(effectsdb,"chance",0);
  if(chance>=random)
    {
      new tickdelay=KvGetNum(effectsdb,"tickdelay",0);
      new ticks=KvGetNum(effectsdb,"ticks",0);
      new tickdamage=KvGetNum(effectsdb,"tickdamage",0);
      new effid=CreateEffectData(userid,attacker,ticks,tickdamage,0,0,0,0,0,0);
      if (tickdelay==0.0 || ticks == 0 || tickdamage == 0)
        LogError("ERROR: effects database corrupted.");
      else
        if(effid!=0)
          CreateTimer(float(tickdelay), Effect1Action, effid, TIMER_REPEAT);
    }
}

public Action:Effect1Action(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--;
  decl Float:vec[3],Float:rightVec[3],Float:upVec[3];
  GetClientAbsOrigin(effectsdata[effid][1], vec);
  vec[0] -= 30;
  vec[1] -= 30;
  GetVectorVectors(vec, rightVec, upVec);
  static const color[] = {255,15,15,255};
  if(!CSGO) {
    TE_SetupBloodSprite(vec, rightVec, color, 25, g_bloodsprayModel, g_bloodModel);
    TE_SendToAll();
  }
  wcPhysicalDamage(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][4]);
  return Plugin_Continue;
}

public Effect4(userid,attacker, bool:fok)
{
  decl String:line[255];
  new random=GetRandomInt(1,100);
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"4");
  wcGetTalentInfo(2,3,1,2,attacker,"increment",line);
  new talentbonus = StringToInt(line);
  new chance=KvGetNum(effectsdb,"chance",0)+talentbonus;
  chance = RoundToCeil(float(chance)/GetWeaponSpeed(attacker, false));
  if(chance>=random || fok)
  {
    wcGetTalentInfo(2,3,2,2,attacker,"chance",line);
    chance = StringToInt(line);
    random=GetRandomInt(1,100);
    if(chance>=random)
    {
      wcGetTalentInfo(2,3,2,2,attacker,"multi",line);
      new Float:slowmulti=StringToFloat(line);
      new Float:oldspeed=GetClientSpeed(userid);
      new Float:newspeed=GetNormalSpeed(userid);
      newspeed*=slowmulti;
      if(newspeed<oldspeed)
      {
        wcGetTalentInfo(2,3,2,2,attacker,"duration",line);
        new Float:duration=StringToFloat(line);
        WCMessage(attacker,"crippling poison user",attacker,duration);
        WCMessage(userid,"crippling poison victim",userid,duration);
        SetClientSpeed(userid,newspeed);
        new Handle:datapack2=CreateDataPack();
        WritePackCell(datapack2,userid);
        WritePackFloat(datapack2,newspeed);
        WritePackFloat(datapack2,slowmulti);
        CreateTimer(duration,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
      }
    }
    /*if(CheckRequirements(attacker,2,1,2,4))
    {
      wcGetTalentInfo(2,1,2,4,attacker,"increment",line);
      SetPoisoned(userid,StringToInt(line));
    }*/
    decl String:name[50],String:victimname[50];
    GetClientName(attacker,name,sizeof(name));
    GetClientName(userid,victimname,sizeof(victimname));
    new Float:tickdelay=KvGetFloat(effectsdb,"tickdelay",0.0);
    new ticks=KvGetNum(effectsdb,"ticks",0);
    ticks = ticks+RoundToFloor(float(ticks)*(GetHasteEffect(attacker)));
    tickdelay = tickdelay*(1.0-GetHasteEffect(attacker));
    new tickdamage=KvGetNum(effectsdb,"damage",0);
    new Float:multi=KvGetFloat(effectsdb,"multi",0.0);
    if(CheckRequirements(attacker,2,3,1,1))
    {
      decl Float:clienta[3],Float:targeta[3];
      GetClientEyeAngles(attacker, clienta);
      GetClientEyeAngles(userid, targeta);
      new Float:angle=clienta[1]-targeta[1];
      if(angle<0.0)
        angle*=-1.0;
      if(angle>180.0)
        angle = 360.0-angle;
      if(angle<=90.0 && IsValidShooting(attacker, userid))
      {
        wcGetTalentInfo(2,3,1,1,attacker,"multi",line);
        tickdamage+=RoundToNearest((float(GetBonusAgi(attacker)+GetAgi(attacker))*multi)*StringToFloat(line));
      }
      else
        tickdamage+=RoundToNearest(float(GetBonusAgi(attacker)+GetAgi(attacker))*multi);
    }
    else
      tickdamage+=RoundToNearest(float(GetBonusAgi(attacker)+GetAgi(attacker))*multi);

    decl String:vicMsg[300], String:attMsg[300];
    Format(attMsg,sizeof(attMsg),"%T","deadly poison casted",attacker,victimname,tickdamage);
    Format(vicMsg,sizeof(vicMsg),"%T","deadly poison casted victim",userid,name,tickdamage);
    decl Float:location[3],Float:location2[3];
    GetClientAbsOrigin(attacker,location);
    GetClientAbsOrigin(userid,location2);
    location[2]+=40;
    location2[2]+=40;
    new color[4]= {155,155,155,255};
    BeamEffect("@all",location,location2,g_ef_hydragutbeampoison,5,3.0,5.0,9.0,color,0.0,5);
    new effid=CreateEffectData(userid,attacker,ticks,tickdamage,0,0,0,0,g_respawns[userid],GetClientDeaths(userid));
    if (tickdelay==0.0 || ticks == 0 || tickdamage == 0)
    {
      LogError("ERROR: effects database corrupted.");
      DeleteEffectData(effid);
    }
    else
      if(effid!=0)
      {
        CreateTimer(tickdelay, Effect4Action, effid, TIMER_REPEAT);
      }
    /*if(CheckRequirements(attacker,2,2,3,3))
    {
      wcGetTalentInfo(2,2,3,3,attacker,"multi",line);
      SetPoisoned2(userid,StringToFloat(line));
      RemoveEffectVictim(userid,42);
      SetEffectVictim(userid,42);

      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,userid); // Write the client index of owner
      WritePackCell(datapack2,42);
      CreateTimer(tickdelay*float(ticks),DestroyEffectVictim,datapack2,TIMER_HNDL_CLOSE);
    }*/
  }
}

public Action:EffectFlameShockAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(effectsdata[effid][10]<GetClientDeaths(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--;
  decl Float:vec[3],Float:rightVec[3],Float:upVec[3];
  GetClientAbsOrigin(effectsdata[effid][1], vec);
  vec[0] -= 30;
  vec[1] -= 30;
  GetVectorVectors(vec, rightVec, upVec);
  static const color[] = {255,40,40,200};
  if(!CSGO) {
    TE_SetupBloodSprite(vec, rightVec, color, 25, g_bloodsprayModel, g_bloodModel);
    TE_SendToAll();
  }
  decl String:line[255];
  if(effectsdata[effid][8]==1)
    wcGetSpellInfo(6,3,"name",line);
  else if(effectsdata[effid][8]==2)
    wcGetTalentInfo(3,2,4,2,effectsdata[effid][1],"name",line);
  wcSpellDamage(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][4],"","",line);
  return Plugin_Continue;
}

public Action:EffectAoeDamage(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0) //empty effectdata
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2])) //victim/attacker
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1])) //victim
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(effectsdata[effid][10]<GetClientDeaths(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0) //ticks
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount) //dot from another round
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--; //expend one tick and continue
  decl Float:vec[3],Float:rightVec[3],Float:upVec[3];
  GetClientAbsOrigin(effectsdata[effid][1], vec); //victim
  vec[0] -= 30;
  vec[1] -= 30;
  GetVectorVectors(vec, rightVec, upVec);
  static const color[] = {255,40,40,200};
  if(!CSGO) {
    TE_SetupBloodSprite(vec, rightVec, color, 25, g_bloodsprayModel, g_bloodModel);
    TE_SendToAll();
  }
  decl String:line[255];
  new damage = effectsdata[effid][4]; //damage
  new aoe = effectsdata[effid][5]; //bool:aoe
  new swapTarget=0;
  if(effectsdata[effid][8]==1)  { //
    line="Living Bomb";

    //Tick effect - living bomb
    new Float:position_victim[3];
    GetClientAbsOrigin(effectsdata[effid][1], position_victim);
    position_victim[2]+=40;
    VS_BeamRingPoint(position_victim, 19.00, 20.00, g_ef_splodesprite, g_ef_splodesprite, 100, 300, 0.50, 20.00, 1.00, {255, 255, 255, 255}, 300, 0);
    //Explosion
    if( effectsdata[effid][3] <= 0) { //Check for latest tick

      //Increase last tick damage twice
      damage *= 4;
      //Sound
      EmitAmbientSound("wc/spells/livingbomb.wav",position_victim);
      //Push up
      if(!IsBoss()) {
        new Float:pushVec[3] = { 0.0, 0.0, 650.0 };
        TeleportEntity(effectsdata[effid][1], NULL_VECTOR, NULL_VECTOR, pushVec );
      }
      //Visual effect
      VS_BeamRingPoint(position_victim, 19.00, 20.00, g_ef_splodesprite, g_ef_splodesprite, 100, 300, 0.50, 60.00, 1.00, {255, 255, 255, 255}, 300, 0);
      VS_BeamRingPoint(position_victim, 19.00, 500.00, g_ef_splodesprite, g_ef_splodesprite, 100, 300, 0.50, 60.00, 1.00, {255, 255, 255, 255}, 300, 0);

      //Living Bomb Glyph
      if(HasGlyph(effectsdata[effid][2], GLYPH_MAGE_BOMB) && GetRandomInt(1,3)==1)  {
        new targets[2];
        new Float:pos[3];
        GetClientAbsOrigin(effectsdata[effid][1], pos);
        new amount = GetNearestPlayers(GetClientTeam(effectsdata[effid][1]),  pos, 500.0, targets, 2);
        if(amount >=2)  {
          new newTarget=targets[1];
          if(newTarget == effectsdata[effid][1])
            newTarget = targets[0];
          swapTarget=newTarget;
          effectsdata[effid][3]+=3;
        }

      }
    }
    else  {
      aoe = 1;  // non aoe
    }
  }
  else if(effectsdata[effid][8]==2)
    line="Immolation";
  else if(effectsdata[effid][8]==901) {
    line="Chi Explosion";
    new Float:position_victim[3];
    GetClientAbsOrigin(effectsdata[effid][1], position_victim);
    EmitAmbientSound("wc/spells/chiexplosion.wav",position_victim);

    }
  else if(effectsdata[effid][8]==3) {
    line="Corruption";
    if( effectsdata[effid][3] <= 0 && HasGlyph(effectsdata[effid][2], GLYPH_WARLOCK_CORRUPTION))  {
      damage*=2;
    }
    if(HasGlyph(effectsdata[effid][2], GLYPH_WARLOCK_SDOOM))
    {
      new client = effectsdata[effid][2];
      new targets[MAXPLAYERS];
      //new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,0,0,g_RoundCount); //This is just for reference
      decl Float:location[3];
      GetClientAbsOrigin(effectsdata[effid][1],location);
      new team = 0;
      if(effectsdata[effid][6] == 2)
        team = 3;
      if(effectsdata[effid][6] == 3)
        team = 2;
      aoe = effectsdata[effid][5];
      new amount = 0;
      new x = 0;
      if(team ==  2)
        amount=GetNearClients("@ct",location,targets,float(aoe));
      if(team == 3)
        amount=GetNearClients("@t",location,targets,float(aoe));
      decl String:name[255],String:name2[255];
      GetClientName(effectsdata[effid][2],name,sizeof(name));
      GetClientName(targets[x],name2,sizeof(name2));
      if(amount>=3)
      {
        new Float:oldspeed=GetClientSpeed(client);
        new Float:normalspeed=GetNormalSpeed(client);
        new Float:multi=1.03;
        new Float:newspeed=(oldspeed * multi);
        new Float:additionalspeed=newspeed-normalspeed;
        if((newspeed>normalspeed) && (additionalspeed < 0.300))
        {
        //  WCMessage(client,"swift doom glyph",client,RoundToNearest(newspeed*100));
          SetClientSpeed(client,newspeed);
          new Float:time=5.0; //High numbers for testing
          new Handle:datapack=CreateDataPack();
          WritePackCell(datapack,client); // Write the client index of owner
          WritePackFloat(datapack,multi);
          WritePackFloat(datapack,normalspeed);
          CreateTimer(time,RemoveDottedSpeed,datapack,TIMER_HNDL_CLOSE);
        }
      }
    }
  }
  else if(effectsdata[effid][8]==803)
    line="Plague Strike";
  else if(effectsdata[effid][8]==804)
    line="Icy Touch";
  new is_immolate = effectsdata[effid][7];
  if(is_immolate == IMMOLATE_UNIQUE)  {
    new Float:position_victim[3];
    GetClientAbsOrigin(effectsdata[effid][1], position_victim);
    VS_BeamRingPoint(position_victim, 40.00, 199.00, g_ef_fire1, g_ef_fire1, 100, 300, 2.00, 20.00, 0.80, {255, 255, 255, 255}, 20, 0);
    //VS_BeamRingPoint(position_victim, 50.00, 10.00, g_ef_fire1, g_ef_fire1, 100, 300, 1.00, 400.00, 0.00, {255, 255, 255, 255}, 20, 0);
    //TC
    VS_BeamRingPoint(position_victim, 50.00, 10.00, g_ef_fire1, g_ef_fire1, 100, 300, 1.00, 199.00, 0.00, {255, 255, 255, 255}, 20, 0);
    if(HasGlyph(effectsdata[effid][2], GLYPH_WARLOCK_SDOOM))
    {
      new client = effectsdata[effid][2];
      new targets[MAXPLAYERS];
      //new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,0,0,g_RoundCount); //This is just for reference
      decl Float:location[3];
      GetClientAbsOrigin(effectsdata[effid][1],location);
      new team = 0;
      if(effectsdata[effid][6] == 2)
        team = 3;
      if(effectsdata[effid][6] == 3)
        team = 2;
      aoe = effectsdata[effid][5];
      new amount = 0;
      new x = 0;
      if(team ==  2)
        amount=GetNearClients("@ct",location,targets,float(aoe));
      if(team == 3)
        amount=GetNearClients("@t",location,targets,float(aoe));
      decl String:name[255],String:name2[255];
      GetClientName(effectsdata[effid][2],name,sizeof(name));
      GetClientName(targets[x],name2,sizeof(name2));
      if(amount>=3)
      {
        new Float:oldspeed=GetClientSpeed(client);
        new Float:normalspeed=GetNormalSpeed(client);
        new Float:multi=1.03;
        new Float:newspeed=(oldspeed * multi);
        new Float:additionalspeed=newspeed-normalspeed;
        if((newspeed>normalspeed) && (additionalspeed < 0.300))
        {
       //   WCMessage(client,"swift doom glyph",client,RoundToNearest(newspeed*100));
          SetClientSpeed(client,newspeed);
          new Float:time=5.0; //High numbers for testing
          new Handle:datapack=CreateDataPack();
          WritePackCell(datapack,client); // Write the client index of owner
          WritePackFloat(datapack,multi);
          WritePackFloat(datapack,normalspeed);
          CreateTimer(time,RemoveDottedSpeed,datapack,TIMER_HNDL_CLOSE);
        }
      }
    }
  }
  wcSpellAoeDamage(effectsdata[effid][1],effectsdata[effid][2],damage,aoe,effectsdata[effid][6],line);
  if(swapTarget>0)  {
    effectsdata[effid][1] = swapTarget;
    WCMessage(swapTarget,"living bomb by glyph", swapTarget);
    decl String:name[30];
    GetClientName(swapTarget, name, 30);
    WCMessage(effectsdata[effid][2], "living bomb glyph attacker", effectsdata[effid][2], name);
  }
  return Plugin_Continue;
}

public Action:EffectRenewAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new caster = effectsdata[effid][1];
  new target = effectsdata[effid][2];
  new ticks = effectsdata[effid][3];
  new heal = effectsdata[effid][4];
  new bool:cancrit=false;
  if(effectsdata[effid][5]==1)
    cancrit=true;
  if (!IsClientInGame(caster) || !IsClientInGame(target))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(target))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(effectsdata[effid][10]<GetClientDeaths(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (ticks<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--;
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }

  wcHeal(caster, target, heal,0,cancrit);
  return Plugin_Continue;
}

public Action:EffectLifeDrainAction(Handle:timer, any:effid)
{
  new Float:talent=0.0;
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(effectsdata[effid][10]<GetClientDeaths(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--;
  decl Float:vec[3],Float:rightVec[3],Float:upVec[3];
  GetClientAbsOrigin(effectsdata[effid][1], vec);
  vec[0] -= 30;
  vec[1] -= 30;
  GetVectorVectors(vec, rightVec, upVec);
  static const color[] = {255,40,40,200};
  if(!CSGO) {
    TE_SetupBloodSprite(vec, rightVec, color, 25, g_bloodsprayModel, g_bloodModel);
    TE_SendToAll();
  }
  wcSpellAoeDamage(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][4],effectsdata[effid][5],effectsdata[effid][6]);
  if(CheckRequirements(effectsdata[effid][2],7,1,3,2))
  {
    decl String:line[255];
    wcGetTalentInfo(7,1,3,2,effectsdata[effid][2],"multi",line);
    talent = StringToFloat(line);
  }
  wcHeal(effectsdata[effid][2],effectsdata[effid][2],RoundToNearest(effectsdata[effid][4]*(1.0+talent)),0,false);
  if(HasGlyph(effectsdata[effid][2], GLYPH_WARLOCK_SDOOM))
  {
    new client = effectsdata[effid][2];
    new targets[MAXPLAYERS];
    //new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,0,0,g_RoundCount); //This is just for reference
    decl Float:location[3];
    GetClientAbsOrigin(effectsdata[effid][1],location);
    new team = 0;
    if(effectsdata[effid][6] == 2)
      team = 3;
    if(effectsdata[effid][6] == 3)
      team = 2;
    new aoe = effectsdata[effid][5];
    new amount = 0;
    new x = 0;
    if(team ==  2)
      amount=GetNearClients("@ct",location,targets,float(aoe));
    if(team == 3)
      amount=GetNearClients("@t",location,targets,float(aoe));
    decl String:name[255],String:name2[255];
    GetClientName(effectsdata[effid][2],name,sizeof(name));
    GetClientName(targets[x],name2,sizeof(name2));
    if(amount>=3)
    {
      new Float:oldspeed=GetClientSpeed(client);
      new Float:normalspeed=GetNormalSpeed(client);
      new Float:multi=1.03;
      new Float:newspeed=(oldspeed * multi);
      new Float:additionalspeed=newspeed-normalspeed;
      if((newspeed>normalspeed) && (additionalspeed < 0.300))
      {
       // WCMessage(client,"swift doom glyph",client,RoundToNearest(newspeed*100));
        SetClientSpeed(client,newspeed);
        new Float:time=5.0; //High numbers for testing
        new Handle:datapack=CreateDataPack();
        WritePackCell(datapack,client); // Write the client index of owner
        WritePackFloat(datapack,multi);
        WritePackFloat(datapack,normalspeed);
        CreateTimer(time,RemoveDottedSpeed,datapack,TIMER_HNDL_CLOSE);
      }
    }
  }
  //Glyph code
  if(HasGlyph(effectsdata[effid][2], GLYPH_WARLOCK_DRAIN))  {
    new dmg = effectsdata[effid][4];
    if(IsCaster(effectsdata[effid][1]))  {
      new mana = GetMana(effectsdata[effid][1]);
      new restore = GetMinInt(dmg, mana);
      if(restore > 0) {
        new setMana = GetMana(effectsdata[effid][2]);
        setMana+=restore;
        if(setMana > GetMaxMana(effectsdata[effid][2]))
          setMana = GetMaxMana(effectsdata[effid][2]);
        wcSetMana(effectsdata[effid][2], setMana);
      }
      mana -= dmg;
      if(mana < 0)
        mana = 0;
      wcSetMana(effectsdata[effid][1], mana);
    }
  }

  return Plugin_Continue;
}

public Action:EffectWall(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new eff = effectsdata[effid][7];
  if (!IsClientInGame(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  if(effectsdata[effid][2]!=12)   {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  new ticks = effectsdata[effid][3]--;
  if (ticks>0)
  {
    if(IsValidEntity(eff))  SyncEntityLocation(eff);
    return Plugin_Continue;
  }
  DeleteEffectData(effid);
  KillEntity(eff);
  return Plugin_Stop;
}

public Action:EffectIceBlock(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  new client = effectsdata[effid][1];
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    DestroyIceBlock(INVALID_HANDLE, client);
    return Plugin_Stop;
  }
  if(effectsdata[effid][2]!=2)   {
    DeleteEffectData(effid);
    DestroyIceBlock(INVALID_HANDLE, client);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    DestroyIceBlock(INVALID_HANDLE, client);
    return Plugin_Stop;
  }
  new ticks = effectsdata[effid][3]--;
  if (ticks>0)
  {
    return Plugin_Continue;
  }
  DeleteEffectData(effid);
  DestroyIceBlock(INVALID_HANDLE, client);
  return Plugin_Stop;
}


public Action:EffectBubble(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new eff = effectsdata[effid][7];
  if (!IsClientInGame(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  new client = effectsdata[effid][1];
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    DestroyShield(INVALID_HANDLE, client);
    return Plugin_Stop;
  }
  if(effectsdata[effid][2]!=2)   {
    DeleteEffectData(effid);
    KillEntity(eff);
    DestroyShield(INVALID_HANDLE, client);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    DestroyShield(INVALID_HANDLE, client);
    return Plugin_Stop;
  }
  new ticks = effectsdata[effid][3]--;
  if (ticks>0)
  {
    if(IsValidEntity(eff))  SyncEntityLocation(eff);
    return Plugin_Continue;
  }
  DeleteEffectData(effid);
  KillEntity(eff);
  DestroyShield(INVALID_HANDLE, client);
  return Plugin_Stop;
}

public Action:EffectHymnAction(Handle:timer, any:effid)
{
   if (effectsdata[effid][0]==0)
    return Plugin_Stop;
   new eff = effectsdata[effid][7];
   new eff2 = effectsdata[effid][8];
   if (effectsdata[effid][9]!=g_RoundCount    || //round changed
       effectsdata[effid][2]!=9               || //spellid not 9
       !IsClientInGame(effectsdata[effid][1]) || //player left
       !IsPlayerAlive(effectsdata[effid][1]))    //player died
   {
    DeleteEffectData(effid);
    KillEntity(eff);
    KillEntity(eff2);
    return Plugin_Stop;
   }
   new client = effectsdata[effid][1];
   if (GetEngineTime()>=g_fCast[client])
   {
    DeleteEffectData(effid);
    KillEntity(eff);
    KillEntity(eff2);
    return Plugin_Stop;
   }
   new team = effectsdata[effid][6];
   new aoe = effectsdata[effid][5];
   new Float:perc = float(effectsdata[effid][3])/1000.0;
   new targets[MAXPLAYERS];
   new amount;
   decl Float:location[3];
   GetClientAbsOrigin(client,location);
   new Float:distance = float(aoe);
   if(team==2)
      amount=GetNearClients("@t",location,targets,distance);
   if(team==3)
      amount=GetNearClients("@ct",location,targets,distance);
   decl String:name[255];
   GetClientName(client,name,sizeof(name));
   for (new x=0;x<=(amount-1);x++)  {
      new mana = RestoreManaP(targets[x],perc);
      if(mana > 0 && HasGlyph(client, GLYPH_PRIEST_HYMN)) {
        mana = RoundToNearest(float(mana)*0.25);
        wcHeal(client, targets[x], mana,0,false);
      }
      WCMessage(targets[x],"hymn target",targets[x],name,mana);
   }
   return Plugin_Continue;
}

public Action:EffectHellfireAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new eff = effectsdata[effid][7];
  if (effectsdata[effid][9]!=g_RoundCount    || //round changed
      effectsdata[effid][2]!=9               || //spellid not 9
      !IsClientInGame(effectsdata[effid][1]) || //player left
      !IsPlayerAlive(effectsdata[effid][1]))    //player died
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  new client = effectsdata[effid][1];
  if (GetEngineTime()>=g_fCast[client])
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }
  new selfDmg = effectsdata[effid][4];
  if(GetClientHealth(client) <= selfDmg)
  {
    DeleteEffectData(effid);
    KillEntity(eff);
    return Plugin_Stop;
  }

  decl String:line[255];
  wcGetSpellInfo(7,9,"name",line);
  wcSpellDamage(client, client, selfDmg, "", "",line);

  new dmg = effectsdata[effid][3];
  new aoe = effectsdata[effid][5];
  new team = effectsdata[effid][6];
  if (effectsdata[effid][8]>0 && CheckRequirements(client,7,3,4,2))
  {
    decl String:buf[255];
    wcGetTalentInfo(7,3,4,2,client,"multi",buf);
    dmg = RoundToNearest(dmg * StringToFloat(buf));
    wcHeal(client,effectsdata[effid][8],dmg,0,true);
    new Float:location[3];
    GetClientAbsOrigin(effectsdata[effid][8],location);
    wcSpellAoeDamage(client, client, dmg, aoe, team, line,location);
  }
  else
  {
    wcSpellAoeDamage(client, client, dmg, aoe, team, line);
  }
  return Plugin_Continue;
}


public Action:EffectLightWellAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];

  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  new team = effectsdata[effid][6];
  new aoe = effectsdata[effid][5];
  new heal = effectsdata[effid][3];
  new Float:location[3];
  location[0]=float(effectsdata[effid][4]);
  location[1]=float(effectsdata[effid][7]);
  location[2]=float(effectsdata[effid][8]);
  new targets[MAXPLAYERS];
  new amount;
  if(team==3)
    amount=GetNearClients("@ct",location,targets,float(aoe));
  if(team==2)
    amount=GetNearClients("@t",location,targets,float(aoe));
  location[2]+=50;
  EmitAmbientSound("wc/spells/lightwell.wav",location);
  for (new x=0;x<=(amount-1);x++)
  {
    if(HasGlyph(client, GLYPH_PRIEST_LIGHTWELL))
    {
      switch (GetClass(targets[x]))
      {
        case 3,4,5,6,7:
        {
          heal = RoundToNearest(heal*0.5);
          wcSetMana(targets[x],GetMana(targets[x]) + heal);
        }
      }
    }
    wcHeal(client,targets[x],heal,0,true);
    if(HasGlyph(client, GLYPH_PRIEST_LIGHTW))
    {
      new Float:bonus=1.15;
      new Float:duration=2.0;

      new Float:oldspeed=GetNormalSpeed(targets[x]);
      new Float:newspeed=oldspeed * bonus;
      if(newspeed>oldspeed)
      {
        WCMessage(client,"Battle Rush Used",client);
        new Float:time=duration;
        SetClientSpeed(client,newspeed);
        new Handle:datapack=CreateDataPack();
        WritePackCell(datapack,client); // Write the client index of owner
        WritePackFloat(datapack,newspeed);
        WritePackFloat(datapack,bonus);
        CreateTimer(time,RemoveSpeedEffect2,datapack,TIMER_HNDL_CLOSE);
      }
    }
  }
  EmitAmbientSound("wc/spells/lightwell.wav",location);
  return Plugin_Continue;
}

public Action:Effect4Action(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
  {
    SetPoisoned(effectsdata[effid][1],0);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    SetPoisoned(effectsdata[effid][1],0);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if(effectsdata[effid][10]<GetClientDeaths(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0)
  {
    SetPoisoned(effectsdata[effid][1],0);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_respawns[effectsdata[effid][1]])
  {
    SetPoisoned(effectsdata[effid][1],0);
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--;
  if((g_iDispatch[effectsdata[effid][1]]>0) && GetClass(effectsdata[effid][2])==2)
  {
    effectsdata[effid][3]=3;
    g_iDispatch[effectsdata[effid][1]]-=1;
  }
  decl Float:vec[3],Float:rightVec[3],Float:upVec[3];
  GetClientAbsOrigin(effectsdata[effid][1], vec);
  vec[0] -= 30;
  vec[1] -= 30;
  GetVectorVectors(vec, rightVec, upVec);
  static const color[] = {15,255,15,200};
  if(!CSGO) {
    TE_SetupBloodSprite(vec, rightVec, color, 25, g_bloodsprayModel, g_bloodModel);
    TE_SendToAll();
  }
  if(HasGlyph(effectsdata[effid][2], GLYPH_ROGUE_POISON))
    wcPhysicalDamage(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][4]);
  else
    {
      new class=GetClass(effectsdata[effid][2]);
      if(class == 1)
        wcPhysicalDamage(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][4],"","",true);
      else
      {
        decl String:line[255];
        wcSpellDamage(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][4], "", "",line);
      }
    }

  if(wcGetTalentLevel(effectsdata[effid][2],3,1,4)>0)
  {

    decl String:line[255];
    wcGetTalentInfo(2,3,1,4,effectsdata[effid][2],"multi",line);
    new Float:hm=GetHM(effectsdata[effid][1])-StringToFloat(line);
    if(hm<0.7)
      hm=0.7;
    SetHM(effectsdata[effid][1],hm);
    wcGetTalentInfo(2,3,1,4,effectsdata[effid][2],"duration",line);
    SetHMT(effectsdata[effid][1],StringToFloat(line));
  }

  return Plugin_Continue;
}


public CreateEffectData(tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8,tmp9,tmp10)
{
  for (new i=1;i<=299;i++)
  {
    if (effectsdata[i][0]==0)
    {

      effectsdata[i][0]=1;
      effectsdata[i][1]=tmp1;
      effectsdata[i][2]=tmp2;
      effectsdata[i][3]=tmp3;
      effectsdata[i][4]=tmp4;
      effectsdata[i][5]=tmp5;
      effectsdata[i][6]=tmp6;
      effectsdata[i][7]=tmp7;
      effectsdata[i][8]=tmp8;
      effectsdata[i][9]=tmp9;
      effectsdata[i][10]=tmp10;
      return i;
    }
  }
  return 0;
}

public Action:DestroyEffectVictim(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new effid=ReadPackCell(datapack); // Get effid
  if(effid==5)
  {
    g_iShieldStack[client]-=1;
    if(0>g_iShieldStack[client])
      g_iShieldStack[client]=0;
  }
  RemoveEffectVictim(client,effid);
}

public Action:DestroyEffectSpellVic(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new effid=ReadPackCell(datapack); // Get effid
  RemoveEffectSpellVic(client,effid);
}

public Action:DestroyEffectAttacker(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new effid=ReadPackCell(datapack); // Get effid
  RemoveEffectAttacker(client,effid);
}

public Action:DestroyEffectDeath(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new effid=ReadPackCell(datapack); // Get effid
  RemoveEffectDeath(client,effid);
}

public DeleteEffectData(id)
{
  effectsdata[id][0]=0;
}

stock BloodSpray(ent_to_bleed)
{
    if(CSGO)
      return;
    /* Create the entity */
    new ent = CreateEntityByName("env_blood");
    if (ent == -1)
    {
        return;
    }

    /* Set up the entity */
    DispatchSpawn(ent);
    DispatchKeyValue(ent, "spawnflags", "13");
    DispatchKeyValue(ent, "amount", "1000");

    /* Fire the effect */
    AcceptEntityInput(ent, "EmitBlood", ent_to_bleed);
    AcceptEntityInput(ent, "EmitBlood", ent_to_bleed);

    /* Remove the entity.
     * If this crashes or makes the blood not appear,
     * try removing it.  However, you don't want to create
     * tons of env_blood entities, so you may wish to
     * call FindEntityByClassname() and only create+
     * dispatchspawn if the entity does not exist.
     */
    RemoveEdict(ent);
}

public Action:RemoveAccuracy(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new round=ReadPackCell(datapack);
  new Float:restore=ReadPackFloat(datapack); // Get type
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (round!=g_RoundCount)
  {
    return Plugin_Stop;
  }
  SetBonusAccuracy(client,restore);
  wcRecalculateStats(client);
  return Plugin_Stop;
}

public Action:RemoveSlowAttack(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new round=ReadPackCell(datapack);
  new Float:restore=ReadPackFloat(datapack); // Get type
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (round!=g_respawns[client])
  {
    return Plugin_Stop;
  }
  wcClientsBonusStats[client][ATTACKSPEED]+=restore;
  wcRecalculateStats(client);
  return Plugin_Stop;
}

public Action:RemoveAFSpeed(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new round=ReadPackCell(datapack);
  new Float:oldspeed=ReadPackFloat(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (round!=g_respawns[client])
  {
    return Plugin_Stop;
  }
  SetClientSpeed(client,oldspeed);
  return Plugin_Stop;
}

public Action:RemoveAttackSpeed(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new round=ReadPackCell(datapack);
  new Float:restore=ReadPackFloat(datapack); // Get type
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (round!=g_respawns[client])
  {
    return Plugin_Stop;
  }
  wcClientsBonusStats[client][ATTACKSPEED]-=restore;
  wcRecalculateStats(client);
  return Plugin_Stop;
}

public Action:RemoveFreezeEffect(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:oldspeed=ReadPackFloat(datapack); // Get type
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (GetClientSpeed(client)!=oldspeed)
  {
    SetEntityMoveType(client, MOVETYPE_WALK);
    return Plugin_Stop;
  }
  SetEntityMoveType(client, MOVETYPE_WALK);
  SetClientSpeed(client,GetNormalSpeed(client));
  return Plugin_Stop;
}
public Action:PWSHaste(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  AddHaste(client, -25.0);
  wcRecalculateStats(client);
  return Plugin_Stop;
}
public Action:LTspd(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:spd=ReadPackFloat(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  spd = GetBonusSpellDmg(client,1.0)-spd;
  SetBonusSpellDmg(client,spd);
  wcRecalculateStats(client);
  return Plugin_Stop;
}
public Action:RemoveSpeedEffect(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:oldspeed=ReadPackFloat(datapack); //current speed
  new Float:multi=ReadPackFloat(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (GetClientSpeed(client)!=oldspeed)
  {
    return Plugin_Stop;
  }
  new Float:speed=GetClientSpeed(client);
  speed*=(1/multi);
  SetClientSpeed(client,speed);
  SetEntityMoveType(client, MOVETYPE_WALK);
  return Plugin_Stop;
}
public Action:RemoveDottedSpeed(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:multi=ReadPackFloat(datapack);
  new Float:normalspeed=ReadPackFloat(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  //if (GetClientSpeed(client)!=oldspeed)
  //{
  //  return Plugin_Stop;
  //}
  new Float:speed=GetClientSpeed(client);
  speed*=(1/multi);
  if (speed < normalspeed)
  {
    return Plugin_Stop;
  }
  SetClientSpeed(client,speed);
  return Plugin_Stop;
}
public Action:RemoveSpeedEffect2(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:oldspeed=ReadPackFloat(datapack); //current speed
  new Float:multi=ReadPackFloat(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (GetClientSpeed(client)!=oldspeed)
  {
    return Plugin_Stop;
  }
  new Float:speed=GetClientSpeed(client);
  speed*=(1/multi);
  SetEntityMoveType(client, MOVETYPE_WALK);
  SetClientSpeed(client,speed, true); // ignore anti-speed effects
  return Plugin_Stop;
}
public Action:RemoveSpeedEffectJ(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:oldspeed=ReadPackFloat(datapack); //current speed
  new Float:multi=ReadPackFloat(datapack);
  new Float:normalspeed=ReadPackFloat(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (GetClientSpeed(client)!=oldspeed)
  {
    return Plugin_Stop;
  }
  new Float:speed=GetClientSpeed(client);
  speed*=(1/multi);
  if(speed < normalspeed)
  {
    SetClientSpeed(client,normalspeed, true);
  }
  else
  {
  SetClientSpeed(client,speed, true);
  }
  return Plugin_Stop;
}
public Action:RemoveShadowWell(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  new Float:oldspeed=ReadPackFloat(datapack); // Get type
  new Float:multi=ReadPackFloat(datapack);
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  if (GetClientSpeed(client)!=oldspeed)
  {
    return Plugin_Stop;
  }
  new Float:speed=GetClientSpeed(client);
  speed*=(1/multi);
  if(speed > GetNormalSpeed(client))
    speed=GetNormalSpeed(client);
  SetClientSpeed(client,speed);
  return Plugin_Stop;
}

public Action:RemoveSparksEffect(Handle:timer, Handle:datapack)
{
  ResetPack(datapack); // Set the reading position to the first cell
  new client=ReadPackCell(datapack);  // Get the client index of owner
  if (!IsClientInGame(client))
  {
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    return Plugin_Stop;
  }
  wcClientSparks[client]=false;
  return Plugin_Stop;
}
new g_iShieldStackOld[65]=-1;
public Effect5(client,&damage,attacker)
{
  if(0>g_iShieldStackOld[client])
    g_iShieldStackOld[client] = g_iShieldStack[client];
  if(wcClientAbsorb[client]!=0)
  {
    decl Float:location[3];
    GetClientAbsOrigin(client,location);
    if(!IsBoss() && 0==g_iShieldStackOld[client])
      EmitAmbientSound("wc/spells/absorb.wav",location);
    new absorb=wcClientAbsorb[client];
    wcRunTalent_5_SoulWarding(client, damage);
    if(absorb<damage)
    {
      wcClientAbsorb[client]=0;
      /*decl String:buffer[30];*/
      /*Format(buffer,sizeof(buffer),"%T", "shield absorbed", client);*/
      if(!IsBoss() && 0==g_iShieldStackOld[client])
        AddEMsgAff("shield absorbed", true);
      damage-=absorb;
      if(!IsZombie(client))
        wcRunTalent_5_15(client,absorb,attacker);
      // effect
      new Float:pos[3]; // Stores position
      GetClientAbsOrigin(client,pos); // Get the position of the victim
      pos[2]+=35; // Offset for effect
      if(!IsBoss() && 0==g_iShieldStackOld[client])
      {
        TE_SetupGlowSprite(pos, ShieldSprite, 0.1, 1.0, 130); // Creates cool shield effect
        TE_SendToAll(); // everyone can see it
      }
      g_iShieldStackOld[client]-=1;
    }
    else
    {
      wcClientAbsorb[client]-=damage;
      /*decl String:buffer[30];*/
      /*Format(buffer,sizeof(buffer),"%T", "shield absorbed", client);*/
      if(!IsBoss() && 0==g_iShieldStackOld[client])
        AddEMsgAff("shield absorbed", true);
      if(!IsZombie(client))
        wcRunTalent_5_15(client,damage,attacker);
      damage=0;
      //effect
      new Float:pos[3]; // Stores position
      GetClientAbsOrigin(client,pos); // Get the position of the victim
      pos[2]+=35; // Offset for effect
      if(!IsBoss() && 0==g_iShieldStackOld[client])
      {
        TE_SetupGlowSprite(pos, ShieldSprite, 0.1, 1.0, 130); // Creates cool shield effect
        TE_SendToAll(); // everyone can see it
      }
      g_iShieldStackOld[client]-=1;
    }
  }
}

Effect6(client,&damage)
{
  new random=GetRandomInt(1,100);
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"6");
  new chance=KvGetNum(effectsdb,"chance",0);
  if(chance>=random)
   {
      new Float:restore=KvGetFloat(effectsdb,"restore",0.0);
      RestoreManaP(client,restore);
   }
}

Effect7(client,&damage,victim)
{
  new random=GetRandomInt(1,100);
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"7");
  new chance=KvGetNum(effectsdb,"chance",0);
  new Float:mindmg=KvGetFloat(effectsdb,"mindmg",0.0);
  new maxHp=GetMaxHealth(victim);
  new iMindmg=RoundToNearest(float(maxHp)*mindmg);
  if(iMindmg > damage)
      chance=RoundToNearest(float(chance)/4.0);
  if(chance>=random)
   {
      new Float:restore=KvGetFloat(effectsdb,"restore",0.0);
      RestoreManaP(client,restore);
   }
}

Effect8(client,&damage)
{
  new random=GetRandomInt(1,100);
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"8");
  new chance=KvGetNum(effectsdb,"chance",0);
  if(chance>=random)
   {
      new Float:restore=KvGetFloat(effectsdb,"restore",0.0);
      RestoreHealthP(client,restore);
   }
}

Effect9(client,&damage)
{
  new random=GetRandomInt(1,100);
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"9");
  new chance=KvGetNum(effectsdb,"chance",0);
  new Float:mindmg=KvGetFloat(effectsdb,"mindmg",0.0);
  new maxHp=GetMaxHealth(client);
  new iMindmg=RoundToNearest(float(maxHp)*mindmg);
  if(iMindmg > damage)
      chance=RoundToNearest(float(chance)/4.0);
  if(chance>=random)
   {
      new Float:restore=KvGetFloat(effectsdb,"restore",0.0);
      RestoreHealthP(client,restore);
   }
}

bool:Effect10(&damage)
{
   decl String:line[255];
   wcGetSpellInfo(1,9,"multi",line);
   new Float:multi = StringToFloat(line);
   damage = RoundToNearest (float(damage) * multi);
   return true;
}

bool:Effect15(&damage) //Defensive Stance
{
   decl String:line[255];
   wcGetSpellInfo(1,9,"multiatt",line);
   new Float:multi = StringToFloat(line);
//   wcGetTalentInfo(1,1,1,4,client,"increment",line); //talent no longer exists to buff defensive stance.
//   new Float:talentbonus = StringToFloat(line);
//   damage = RoundToNearest (float(damage) * (multi+talentbonus));
   damage = RoundToNearest(float(damage) * multi);
   return true;
}

bool:Effect12(&damage, client) //Berserker Stance
{
   decl String:line[255];
   wcGetSpellInfo(1,11,"multi",line);
   new Float:multi = StringToFloat(line);
   wcGetTalentInfo(1,3,1,3,client,"increment",line);
   new Float:talentbonus = StringToFloat(line);
   damage = RoundToNearest (float(damage) * (multi-talentbonus));
   return true;
}

bool:Effect13(client, &damage)
{
   decl String:line[255];
   if(GetClass(client)==1)
   {
     wcGetSpellInfo(1,12,"multi",line);
     new Float:multi = StringToFloat(line);
     damage = RoundToNearest (float(damage) * multi);
     return true;
   }
   else
   {
     wcGetTalentInfo(4,2,4,1,client,"reduction",line);
     new Float:multi = StringToFloat(line);
     damage = RoundToNearest (float(damage) * multi);
     return true;
   }
}

bool:Effect11(client, &damage)
{
  if(wcManaShield[client]>0)
  {
    decl Float:location[3];
    GetClientAbsOrigin(client,location);
    if(!IsBoss() || 2>g_iShieldStack[client])
      EmitAmbientSound("wc/spells/absorb.wav",location);
    new absorb=wcManaShield[client];
    decl String:line[255];
    wcGetSpellInfo(3,9,"multi",line);
    new Float:multi=StringToFloat(line);
    wcGetTalentInfo(3,3,2,1,client,"multi",line);
    new Float:talentbonus = StringToFloat(line);
    new max = RoundToFloor(float(GetMana(client))/multi);
    absorb = GetMinInt(absorb,max);
    if(absorb<=damage)
    {
      wcManaShield[client]=0;
      wcSetMana(client,GetMana(client)-RoundToFloor(absorb*multi*(1-talentbonus)));
      /*decl String:buffer[30];*/
      /*Format(buffer,sizeof(buffer),"%T", "shield absorbed", client);*/
      if(!IsBoss() || 2>g_iShieldStack[client])
        AddEMsgAff("shield absorbed", true);
      WCMessage(client,"mana shield end",client);
      damage-=absorb;

    }
    else
    {
      wcManaShield[client]-=damage;
      wcSetMana(client,GetMana(client)-RoundToFloor(damage*multi*(1-talentbonus)));
      /*decl String:buffer[30];*/
      /*Format(buffer,sizeof(buffer),"%T", "shield absorbed", client);*/
      if(!IsBoss() || 2>g_iShieldStack[client])
        AddEMsgAff("shield absorbed", true);
      damage=0;
    }
  }
}

Effect20(victim)
{
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  if(GetBoss(victim)>0)
    return;
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"20");
  new red=KvGetNum(effectsdb,"amount",0);
  if(red>0)
  {
    wcClientsBonusStats[victim][ARMOR]-=red;
    if(wcClientsBonusStats[victim][ARMOR] < 0.0)
      wcClientsBonusStats[victim][ARMOR] = 0.0;
    wcRecalculateStats(victim);
  }
}

Effect21(&dmg) {

  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"21");
  new Float:multi = KvGetFloat(effectsdb, "multi", 1.0);
  dmg = RoundToNearest(multi * float(dmg));
  /*decl String:buffer[30];*/
  /*Format(buffer,sizeof(buffer),"%T", "eff 21", LANG_SERVER);*/
  AddEMsgAff("eff 21", true);
}

bool:Effect22(client) {
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"22");
  new Float:chance = KvGetFloat(effectsdb, "chance", 0.0);
  if(chance >= GetRandomFloat(0.0, 1.0) ) {
    WCMessage(client, "avoided slowdown", client);
    return true;
  }
  return false;
}

Effect23(victim, &dmg) {

  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"23");
  new Float:hp = KvGetFloat(effectsdb, "hp", 0.0);
  if(hp < GetFloatHealth(victim))
    return;
  new Float:multi = KvGetFloat(effectsdb, "inc", 1.0);
  dmg = RoundToNearest(multi * float(dmg));
  /*decl String:buffer[30];*/
  /*Format(buffer,sizeof(buffer),"%T", "eff 23", victim);*/
  AddEMsgAff("eff 23", true);
}


VisualHP(client)  {
   new team=GetClientTeam(client);
   new targets[MAXPLAYERS];
   new amount=FindMatchingHealer(team,targets,client);
   new Float:loc[3];
   GetClientAbsOrigin(client,loc);
   loc[2]+=110.0;
   //es est_effect 10 #a 0 sprites/physcannon_bluecore2b.vmt server_var(wcs_x1) server_var(wcs_y1) server_var(wcs_z1) 20 30 4 30 20 .6 255 255 255 255 10
   //01:01 - HOLLIDAY: est_effect [BeamRing] <player Filter> <delay> <model> <x> <y> <z> <start radius> <end radius> <life> <width> <spread> <amplitude> <Red> <Green> <Blue> <Alpha> <speed>
   // 255,0,0 red - 0%  255,255,0 50%    0,255,0 100%
   new color[4]={255,255,0,210};
   new Float:perc=GetFloatHealth(client);
   if(perc>=0.5)  {
      color[1] = RoundToNearest(-255.0 + 510.0 * perc);
      color[0] = RoundToNearest(400.0 - 400.0 * perc);
   }
   else  {
      color[1]=0;
      color[0] = RoundToNearest(255.0 - 110.0 * perc);
   }
   for(new i=0;i<2;i++) {
      if(color[i]<0)
         color[i]=0;
      if(color[i]>255)
         color[i]=255;
   }
   perc=-perc+1.0;
   new speed=RoundToNearest(9.0+perc*9.0);
   TE_SetupBeamRingPoint(loc, 20.0, 30.0, g_ef_bluecore, 0, 0, 20, 0.35, 30.0, 0.4, color, speed, 0);
   TE_Send(targets, amount);

}

bool:HealEffect(client) {
   if(IsClientInGame(client) && IsPlayerAlive(client))   {
      decl Float:vec[3];
      GetClientAbsOrigin(client,vec);
      BubblesEffect("@all", vec, vec, g_ef_yellowflare, 100.0, 16, 40.0);
   }
}

bool:FindEffect(client, id)  {
   for(new i=1;i<=30;i++)  {
      if(wcEffectsVictim[client][i]==id)  {
         return true;
      }
      if(wcEffectsAttacker[client][i]==id)  {
         return true;
      }
      if(wcEffectsSpellVic[client][i]==id)  {
         return true;
      }
      if(wcEffectsSpellAtt[client][i]==id)  {
         return true;
      }
      if(wcEffectsDeath[client][i]==id)  {
         return true;
      }
      if(wcEffectsUse[client][i]==id)  {
         return true;
      }
   }
   return false;
}

stock SpawnTotem(Float:location[3], bool:horde, hp, type=1) {
  new ent = CreateEntityByName( "prop_physics_override" );
  if( !IsValidEntity( ent ) )
  {
    return -1;
  }
  if(horde)   {
    SetEntProp( ent, Prop_Data, "m_nModelIndex", g_mdl_totem_h );
    if(!CSGO) {
      SetEntityModel( ent, g_sTotemHorde );
    }
  }
  else  {
    SetEntProp( ent, Prop_Data, "m_nModelIndex", g_mdl_totem_a );
    if(!CSGO) {
      SetEntityModel( ent, g_sTotemAlliance );
    }
  }

  switch(type) {
    case 2:  {
      SetColor(ent, 30, 255, 30, 255);
    }
    case 3:  {
      SetColor(ent, 30, 30, 255, 255);
    }
    case 4:  {
      SetColor(ent, 150, 220, 255, 255);
    }
  }
  SetEntityMoveType( ent, MOVETYPE_NONE );

  DispatchSpawn( ent );
  SetEntProp( ent, Prop_Send, "m_nSolidType", 2 );
  SetEntProp( ent, Prop_Data, "m_takedamage", 2 );
  SetEntProp( ent, Prop_Data, "m_iHealth", hp * 3 );
  SetEntProp( ent, Prop_Data, "m_CollisionGroup", 2, 4);
  AcceptEntityInput(ent, "DisableMotion");

  TeleportEntity( ent, location, NULL_VECTOR, NULL_VECTOR );
  //SetEntPropFloat( ent, Prop_Data, "m_flNextThink", GetGameTime() + 0.1 );

  return ent;
}

public Action:EffectNovaTotemAction0(Handle:timer, any:effid)  {

   CreateTimer(0.5, EffectNovaTotemAction, effid, TIMER_REPEAT);
}

public Action:EffectEarthbindTotemAction0(Handle:timer, any:effid)  {

   CreateTimer(0.5, EffectEarthbindTotemAction, effid, TIMER_REPEAT);
}

public Action:EffectEarthbindTotemAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];
  new spellid = effectsdata[effid][2];
  new Float:multi = any:effectsdata[effid][3];
  new totem = effectsdata[effid][4];
  new Float:distance = float(effectsdata[effid][5]);
  new team = effectsdata[effid][6];
  new ticks = effectsdata[effid][7];
  new round = effectsdata[effid][9];
  if (!IsClientInGame(client))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if ((ticks<=0) || (spellid!=10))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  effectsdata[effid][7]--;
  if (round!=g_RoundCount)
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if(!IsValidEntity(totem))   {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    return Plugin_Stop;
  }

  new Float:location[3];
  GetEntPropVector(totem, Prop_Send, "m_vecOrigin", location);

  new targets[MAXPLAYERS];
  new amount;
  if(team==2)
   amount=GetNearClients("@t",location,targets,distance*0.6);
  if(team==3)
   amount=GetNearClients("@ct",location,targets,distance*0.6);
  if(amount==0)   {
   return Plugin_Continue;
  }
  if(team==2)
   amount=GetNearClients("@t",location,targets,distance);
  if(team==3)
   amount=GetNearClients("@ct",location,targets,distance);

  decl String:name[255];
  GetClientName(client, name, 255);
  decl String:name2[255];
  new String:stargets[255];
  for (new x=0;x<amount;x++)
  {
    GetClientName(targets[x],name2,sizeof(name2));
    if(x>0)
      Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
    else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
    WCMessage(targets[x], "totem earthbind target", targets[x], name);
    //Slodown effect
    new Float:oldspeed=GetClientSpeed(targets[x]);
    new Float:newspeed=GetNormalSpeed(targets[x]);
    newspeed*=multi;
    if(newspeed<oldspeed)
    {
      SetClientSpeed(targets[x],newspeed);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,targets[x]); // Write the client index of owner
      WritePackFloat(datapack2,newspeed);
      WritePackFloat(datapack2,multi);
      CreateTimer(0.49,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
    }

  }
  WCMessage(client,"totem earthbind",client,stargets);
  //TODO: Effect

  //end
  return Plugin_Continue;
}

public Action:EffectNovaTotemAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];
  new spellid = effectsdata[effid][2];
  new dmg = effectsdata[effid][3];
  new totem = effectsdata[effid][4];
  new Float:distance = float(effectsdata[effid][5]);
  new team = effectsdata[effid][6];
  new ticks = effectsdata[effid][7];
  new round = effectsdata[effid][9];
  if (!IsClientInGame(client))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if ((ticks<=0) || (spellid!=9))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  effectsdata[effid][7]--;
  if (round!=g_RoundCount)
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if(!IsValidEntity(totem))   {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    return Plugin_Stop;
  }

  new Float:location[3];
  GetEntPropVector(totem, Prop_Send, "m_vecOrigin", location);

  new targets[MAXPLAYERS];
  new amount;
  if(team==2)
   amount=GetNearClients("@t",location,targets,distance*0.6);
  if(team==3)
   amount=GetNearClients("@ct",location,targets,distance*0.6);
  if(amount==0)   {
   return Plugin_Continue;
  }
  if(team==2)
   amount=GetNearClients("@t",location,targets,distance);
  if(team==3)
   amount=GetNearClients("@ct",location,targets,distance);
  new bool:glyph = HasGlyph(client, GLYPH_SHAMAN_NOVA);
  if(glyph)
    dmg = RoundToNearest(0.15 * dmg);

  decl String:name2[255];
  new String:stargets[255];
  decl String:line[255];
  wcGetSpellInfo(6,9,"name",line);
  for (new x=0;x<amount;x++)
  {
   GetClientName(targets[x],name2,sizeof(name2));
   if(x>0)
     Format(stargets,sizeof(stargets),"%s, %s",stargets,name2);
   else
      Format(stargets,sizeof(stargets),"%s %s",stargets,name2);
   decl String:vicMsg[300];
   Format(vicMsg,sizeof(vicMsg),"%T","totem nova target", targets[x]);
   wcSpellDamage(targets[x],client,dmg, "", vicMsg,line);
  }
  WCMessage(client,"totem nova",client,stargets,dmg);
  EmitAmbientSound("wc/spells/fireball.wav",location);
  if(!glyph)  {
    TotemFromPlayer(client,totem);
    KillTotem(totem);
  }
  //Effect
  new Float:life = glyph?1.0:2.5;
  BeamRingEffect("@all",location,50.0,350.0,g_ef_flatflame,0,2,life,90.0,{255, 255, 255, 200},0.0,2);
  location[2]+=15;
  GlowSpriteEffect("@all",location, g_ef_flatflame, 3.0, 3.0, 255);
  //end
  if(glyph)
    return Plugin_Continue;
  else
    return Plugin_Stop;
}
public Action:EffectLightningNovaTotemAction0(Handle:timer, any:effid)  {
  CreateTimer(0.5, EffectLightningNovaTotemAction, effid, TIMER_REPEAT);
}
public Action:EffectLightningNovaTotemAction(Handle:timer, any:effid)
{
  new client = effectsdata[effid][1];
  new dmg = effectsdata[effid][3];
  new totem = effectsdata[effid][4];
  new Float:distance = float(effectsdata[effid][5]);
  new team = effectsdata[effid][6];
  //new ticks = effectsdata[effid][7];
  new round = effectsdata[effid][9];
  if (!IsClientInGame(client))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(client))
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if (effectsdata[effid][7]<=0)//ticks
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  effectsdata[effid][7]--;
  if (round!=g_RoundCount)
  {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    KillTotem(totem);
    return Plugin_Stop;
  }
  if(!IsValidEntity(totem))   {
    DeleteEffectData(effid);
    TotemFromPlayer(client,totem);
    return Plugin_Stop;
  }

  new Float:location[3];
  GetEntPropVector(totem, Prop_Send, "m_vecOrigin", location);

  new target;
  if(team==2)
    target=GetNearestClient("@t",location,distance);
  if(team==3)
    target=GetNearestClient("@ct",location,distance);
  if(target==0)   {
   return Plugin_Continue;
  }
  decl String:vicMsg[300];
  Format(vicMsg,sizeof(vicMsg),"%T","lightning nova totem target", target);
  //decl String:line[255];
  wcSpellDamage(target,client,dmg, "", vicMsg,"Lightning Totem");
  //Effect
  decl Float:clientpos[3];
  GetClientAbsOrigin(target,clientpos);
  clientpos[2]+=40;
  EmitAmbientSound("wc/spells/lightningbolt.wav",clientpos);
  location[2]+=20.0;
  //new color[4] = {255, 255, 255, 255};
  //TE_SetupBeamPoints(location, clientpos, g_LightningSprite, 0, 0, 0, 0.2, 20.0, 10.0, 0, 1.0, color, 3);
  //TE_SendToAll();;
  VS_BeamPoints(location, clientpos, g_ef_hydraspinalcord, g_ef_hydraspinalcord, 1, 5, 0.5, 5.00, 1.00, 1, 0.00, {25, 25, 255, 255}, 20);
  return Plugin_Continue;
}
bool:TotemToPlayer(client, totem)  {
   for(new i=0;i<10;i++)
      if(g_iTotems[client][i]<=0)   {
         g_iTotems[client][i]=totem;
         return true;
      }
   return false;
}

bool:TotemFromPlayer(client, totem) {
   for(new i=0;i<10;i++)
      if(g_iTotems[client][i]==totem)  {
         g_iTotems[client][i]=-1;
         return true;
      }
   return false;
}

public Effect14(client)
{
  SetStealth(client, 0.0);
}

stock Effect_m1(effid, victim, attacker, damage)
{
  new random=GetRandomInt(1,100);
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  decl String:sTemp[100];
  IntToString(effid, sTemp, sizeof(sTemp));
  KvJumpToKey(effectsdb,sTemp);
  new chance=KvGetNum(effectsdb,"chance",0);
  if(chance>=random)
  {
    //Fireball like spell
    new bool:sound= (KvGetNum(effectsdb,"sound",0)!=0);
    new dmg = KvGetNum(effectsdb,"damage",0);
    new type = KvGetNum(effectsdb,"type",2);
    if(sound)  {
      decl Float:location[3];
      GetClientAbsOrigin(victim,location);
      location[2]+=20;
      EmitAmbientSound("wc/spells/fireball.wav",location);
    }
    KvGetString(effectsdb, "attMsg", sTemp, sizeof(sTemp), "");
    decl String:sTemp2[100];
    KvGetString(effectsdb, "vicMsg", sTemp2, sizeof(sTemp2), "");
    if(damage>0)  {
      switch(type)  {
        case 0:
          wcPhysicalDamage(victim,attacker,dmg, sTemp, sTemp2, true);
        case 1:
          wcPhysicalDamage(victim,attacker,dmg, sTemp, sTemp2, false);
        default:
          wcSpellDamage(victim,attacker,dmg, sTemp, sTemp2, "Fireblast");
      }
    }
  }
}


stock Effect_m2(effid, client)
{
  new random=GetRandomInt(1,100);

  KvRewind(effectsdb);
  decl String:sTemp[100];
  IntToString(effid, sTemp, sizeof(sTemp));
  KvJumpToKey(effectsdb,sTemp);
  new chance=KvGetNum(effectsdb,"chance",0);
  if(chance>=random)
  {
    //Crusader like spell

    //Sound
    new bool:sound= (KvGetNum(effectsdb,"sound",0)!=0);
    if(sound)  {
      decl Float:location[3];
      GetClientAbsOrigin(client,location);
      location[2]+=20;
      EmitAmbientSound("wc/spells/crusader.wav",location);
    }

    //Modifier message
    decl String:sModifier[20];
    KvGetString(effectsdb, "modifierName", sModifier, 20, "");
    if(strlen(sModifier)>1)
      AddEMsgAff(sModifier, false);

    //Heal effect
    new heal = KvGetNum(effectsdb,"heal", 0);
    if(heal>0)  {
      new bool:crithealallowed=true;
      if(KvGetNum(effectsdb,"cha",1) != 1)
        crithealallowed=false;
      wcHeal(client, client, heal,0,crithealallowed);
    }

    new stacks = KvGetNum(effectsdb,"stacks",0);

    //Stats bonus
    new itemId = KvGetNum(effectsdb,"bonuses", 0);
    new Float:duration = KvGetFloat(effectsdb,"duration", 0.0);
    if(itemId > 0 && duration > 0.0)  {
      new item[UniqueItem];
      GetItemProperties(item, itemId, 0);
      if(item[Item_defId] > 0)  { //Run only if item exists
        AddTemporaryItem(client, item, duration, (stacks>0));
      }
    }
  }
}

//public Effect24(client, &damage)                                                //Death Wish
//{
//   decl String:line[255];
//   wcGetTalentInfo(1,2,3,1,client,"multi2",line);
//   new Float:multi = StringToFloat(line) +1.0;
//   damage = RoundToNearest (float(damage) * multi);
//   return true;
//}

public Effect25(client,attacker,&damage)                                        //Ice Barrier
{
  if(wcClientAbsorb[client]!=0)
  {
    decl Float:location[3];
    decl String:line[255];
    GetClientAbsOrigin(client,location);
    if(!IsBoss() || 2>g_iShieldStack[client])
      EmitAmbientSound("wc/spells/absorb.wav",location);
    new absorb=wcClientAbsorb[client];

    new bool:bSlow=(damage>=20);

    if(absorb<damage)
    {
      if(HasGlyph(client, GLYPH_MAGE_ICEBARRIER))
      {
        new hp = wcGetHealth(client);
        new maxhp = GetMaxHealth(client);
        hp+=RoundToNearest(maxhp*0.03);
        if(hp>maxhp)
          hp = maxhp;
        wcSetHealth(client,hp);
      }
      wcClientAbsorb[client]=0;
      /*decl String:buffer[30];*/
      /*Format(buffer,sizeof(buffer),"%T", "ice barrier absorb", client);*/
      if(!IsBoss() || 2>g_iShieldStack[client])
        AddEMsgAff("ice barrier absorb", true);
      damage-=absorb;

      if(bSlow) {

        wcGetTalentInfo(3,1,3,1,client,"slowmulti",line);
        new Float:slowmulti=StringToFloat(line);
        new Float:oldspeed=GetClientSpeed(attacker);
        new Float:newspeed=GetNormalSpeed(attacker);
        newspeed*=slowmulti;
        if(newspeed<oldspeed)
        {
          wcGetTalentInfo(3,1,3,1,client,"slowtime",line);
          new Float:slowtime=StringToFloat(line);
          SetClientSpeed(attacker,newspeed);
          new Handle:datapack2=CreateDataPack();
          WritePackCell(datapack2,attacker);
          WritePackFloat(datapack2,newspeed);
          WritePackFloat(datapack2,slowmulti);
          CreateTimer(slowtime,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
        }
      }
      if(CheckRequirements(client,3,1,3,2))
        wcRunTalent_3_ShatteredBarrier(client);
    }
    else
    {
      wcClientAbsorb[client]-=damage;
      /*decl String:buffer[30];*/
      /*Format(buffer,sizeof(buffer),"%T", "ice barrier absorb", client);*/
      if(!IsBoss() || 2>g_iShieldStack[client])
        AddEMsgAff("ice barrier absorb", true);
      damage=0;

      if(bSlow) {
        wcGetTalentInfo(3,1,3,1,client,"slowmulti",line);
        new Float:slowmulti=StringToFloat(line);
        new Float:oldspeed=GetClientSpeed(attacker);
        new Float:newspeed=GetNormalSpeed(attacker);
        newspeed*=slowmulti;
        if(newspeed<oldspeed)
        {
          wcGetTalentInfo(3,1,3,1,client,"slowtime",line);
          new Float:slowtime=StringToFloat(line);
          SetClientSpeed(attacker,newspeed);
          new Handle:datapack2=CreateDataPack();
          WritePackCell(datapack2,attacker);
          WritePackFloat(datapack2,newspeed);
          WritePackFloat(datapack2,slowmulti);
          CreateTimer(slowtime,RemoveSpeedEffect,datapack2,TIMER_HNDL_CLOSE);
        }
      }
    }
  }
}

public Effect26(victim, &damage)                                                //Pain Suppresion
{
   decl String:line[255];
   wcGetTalentInfo(5,1,3,1,victim,"multi",line);
   new Float:multi = StringToFloat(line);
   damage = RoundToNearest (float(damage) * multi);
   /*Format(line,sizeof(line),"%T", "pain suppresion msg", victim);*/
   AddEMsgAff("pain suppresion msg", true);
   return true;
}
public Effect53(target, &damage)                                                //Pain Suppresion
{
   decl String:line[255];
   wcGetTalentInfo(5,1,3,1,target,"multi",line);
   new Float:multi = StringToFloat(line);
   damage = RoundToNearest (float(damage) * multi);
   /*Format(line,sizeof(line),"%T", "pain suppresion msg", target);*/
   AddEMsgAff("pain suppresion msg", true);
   return true;
}

public Effect27(target, &damage)                                                //Shadow Form, increased damage done
{
   decl String:line[255];
   wcGetTalentInfo(5,3,3,1,target,"multi",line);
   new Float:multi = StringToFloat(line);
   if(HasGlyph(target, GLYPH_PRIEST_SHADOWF))
      multi*=1.50;
   damage = RoundToNearest (float(damage) * (1+multi));
   return true;
}

public Effect28(victim, &damage)                                                //Shadow Form, decreased damage taken
{
   decl String:line[255];
   wcGetTalentInfo(5,3,3,1,victim,"multi",line);
   new Float:multi = StringToFloat(line)*0.5;
   if(HasGlyph(victim, GLYPH_PRIEST_SHADOWF))
    multi*=1.50;
   damage = RoundToNearest (float(damage) * (1-multi));
   return true;
}

public Effect29(client,attacker,&damage) //Earth Shield
{                                                                               //Earth Shield
  decl String:line[255];
  new caster = GetEarthShield(client);
  new count = GetEarthShieldCount(client);
  if(count > 0)
  {
    /*Format(line,sizeof(line),"%T", "earth shield absorb", client);*/
    AddEMsgAff("earth shield absorb", true);
    wcGetTalentInfo(6,3,3,1,caster,"heal",line);
    new Float:heal=StringToFloat(line);
    if(HasGlyph(client, GLYPH_SHAMAN_EARTHS))
      heal*=1.20;
    new healing = GetSpellDmg(caster,heal);
    wcHeal(caster,client,healing,0,true);
    SetEarthShieldCount(client,count-1);
  }
  else
  {
    RemoveEffectVictim(client,29);
    RemoveEffectSpellVic(client,29);
  }
}

public Effect30(client, &damage) //Improved Metamorphosis
{
  decl String:line[255];
  wcGetTalentInfo(7,2,2,2,client,"multi",line);
  new Float:multi = StringToFloat(line);
  new heal = RoundToNearest(damage*multi);
  wcHeal(client,client,heal,0,false);
}

//public Effect31(client, &damage)                                                //Death Wish, attacker
//{
//   decl String:line[255];
//   wcGetTalentInfo(1,2,3,1,client,"multi",line);
//   new Float:multi = StringToFloat(line);
//   damage = RoundToNearest (float(damage) * (1+multi));
//   /*Format(line,sizeof(line),"%T", "death wish attack", client);*/
//   AddEMsgAff("death wish attack", true);
//   return true;
//}


public Effect32(client, &damage)                                                //Divine Favor + Enveloping Shadows + Molten Skin + Master of Magic
{
   decl String:line[255];
   if(GetClass(client)==2)
      wcGetTalentInfo(2,1,1,3,client,"increment",line);
   else if(GetClass(client)==3)
      wcGetTalentInfo(3,3,2,4,client,"multi",line);
   else if(GetClass(client)==4)
      wcGetTalentInfo(4,2,2,2,client,"multi",line);
   else if(GetClass(client)==7)
      wcGetTalentInfo(7,3,2,3,client,"increment",line);
   new Float:multi = StringToFloat(line);
   damage = RoundToNearest (float(damage) * (1-multi));
   return true;
}

public Effect33(client, &damage)                                                //Enrage + Shaman Flurry
{
   decl String:line[255];
   new Float:multi=0.0;
   if(GetClass(client)==1)
   {
     wcGetTalentInfo(1,2,2,1,client,"multi",line);
     multi = StringToFloat(line)*GetEnrage(client);
   }
   if(GetClass(client)==6)
   {
     wcGetTalentInfo(6,2,2,1,client,"multi",line);
     multi = StringToFloat(line)*GetFlurry(client);
   }
   if(GetClass(client)==9)
   {
     wcGetTalentInfo(9,2,2,2,client,"multi",line);
     multi = StringToFloat(line)*GetSSS(client);
   }
   damage += RoundToNearest (float(damage) * multi);
   return true;
}

public Effect34(client, damage, attacker)                                       //Eye for an Eye
{
  decl String:line[255];
  wcGetTalentInfo(4,3,1,2,client,"multi",line);
  new Float:multi=StringToFloat(line);
  new dmg = RoundToNearest(damage*multi);
  decl String:vicMsg[300], String:attMsg[300];
  Format(vicMsg,sizeof(vicMsg),"%T","eye for an eye attacker", attacker);
  Format(attMsg,sizeof(attMsg),"%T","eye for an eye paladin", client);
  wcGetTalentInfo(4,3,1,2,client,"name",line);
  wcSpellDamage(attacker,client,dmg, attMsg, vicMsg,line);

  new Float:center[3];  //We declare vector where values will be stored
  //center[0] - X,  center[1] - Y, center[2] - Z
  GetClientAbsOrigin(attacker, center); //We tell it who's origin we need and where to store it
  center[2] += 20;  //Same as center[2] = center[2] + 20;    We add 20 points to Z.
  VS_BeamRingPoint(center, 19.00, 20.00, g_ef_splodesprite, // We can add some comments here
   g_ef_bluelight, // Halo model :)
   100, 300, 3.00, 20.00, 1.00, {255, 255, 255, 255}, 300, 0);
  EmitAmbientSound("wc/spells/eyeforaneye.wav",center);
}

public Effect35(client,damage,victim,bonushit) //Sweeping Strikes
{
  if(GetSweepingStrikes(client)>0 && bonushit==0)
  {
    decl String:line[255];
    wcGetTalentInfo(1,3,3,1,client,"range",line);
    new Float:range = StringToFloat(line);
    wcGetTalentInfo(1,3,3,1,client,"multi",line);
    damage = RoundToNearest(damage*StringToFloat(line));
    new Float:location[3];
    GetClientAbsOrigin(victim,location);
    new target[2];
    new number = GetNearestPlayers(GetClientTeam(victim),location,range, target, 2);
    SetSweepingStrikes(client, GetSweepingStrikes(client)-1);
    if(target[1]!=0 && number > 1)
    {
      dealExtraHit(client,target[1],damage,"Sweeping Strikes","Sweeping Strikes");
    }
    new Float:locationSound[3];
    GetClientAbsOrigin(victim,locationSound);
    EmitAmbientSound("wc/spells/hackandslash.wav",locationSound);
  }
}

public Effect36(client, &damage) //Inspiration
{
  decl String:line[255];
  new caster = GetInspiration(client);
  wcGetTalentInfo(5,2,1,2,caster,"increment",line); //Inspiration
  new Float:multi = StringToFloat(line);
  damage = RoundToNearest (float(damage) * (1-multi));
  AddEMsgAff( "Inspiration name", true);
  return true;
}

public Effect37(client, &damage)                                                //Shamanistic Rage, victim
{
  decl String:line[255];
  wcGetTalentInfo(6,2,3,1,client,"increment",line);
  new Float:reduction=StringToFloat(line);
  damage = RoundToNearest(float(damage) * (1-reduction));
  return true;
}

public Effect38(client, &damage)                                                //Shamanistic Rage, attacker
{
  decl String:line[255];
  wcGetTalentInfo(6,2,3,1,client,"chance",line);
  new chance = StringToInt(line);
  new random = GetRandomInt(1,100);
  if (chance >= random && SpendShamanisticProc(client))
  {
    wcGetTalentInfo(6,2,3,1,client,"multi",line);
    new Float:mana=StringToFloat(line);
    wcSetMana(client,RoundToFloor(GetMana(client)+GetMaxMana(client)*mana));
    if(GetMana(client)>GetMaxMana(client))
      wcSetMana(client,GetMaxMana(client));
  }
}

/*public Effect39(attacker, &damage) //Improved Metamorphis
{
  decl String:line[255];
  wcGetTalentInfo(7,2,2,3,attacker,"multi",line);
  new Float:multi=StringToFloat(line);
  damage = RoundToNearest(float(damage) * (1.0+multi));
  AddEMsgAff("imp meta", true);
  return true;
}*/
public Action:EndChaosBolt(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new victim=ReadPackCell(datapack);
  new effect=ReadPackCell(datapack);
  AddResistance(victim,float(effect));
  wcRecalculateStats(victim);
}
public Effect40(userid,attacker)
{
  new random=GetRandomInt(1,100);
  if (effectsdb==INVALID_HANDLE)
    LogError("INVALID HANDLE - effectsdb");
  KvRewind(effectsdb);
  KvJumpToKey(effectsdb,"40");
  new chance=KvGetNum(effectsdb,"chance",0);
  if(IsDoubleHand(attacker))
    chance = RoundToNearest(chance*1.5);
  if(chance>=random)
  {
    decl String:name[50],String:victimname[50];
    GetClientName(attacker,name,sizeof(name));
    GetClientName(userid,victimname,sizeof(victimname));
    new tickdelay=KvGetNum(effectsdb,"tickdelay",0);
    new ticks=KvGetNum(effectsdb,"ticks",0);
    new tickdamage=KvGetNum(effectsdb,"damage",0);
    new Float:multi=KvGetFloat(effectsdb,"multi",0.0);
    tickdamage+=RoundToNearest(float(GetDamage(attacker))*multi);
    WCMessage(attacker,"rend casted",attacker,victimname,tickdamage);
    WCMessage(userid,"rend casted victim",userid,name,tickdamage);
    //creating visual effect
    decl Float:location[3],Float:location2[3];
    GetClientAbsOrigin(attacker,location);
    GetClientAbsOrigin(userid,location2);
    location[2]+=40;
    location2[2]+=40;
    new color[4]= {196,16,6,95};
    BeamEffect("@all",location,location2,g_ef_hydragutbeam,5,3.0,5.0,9.0,color,5.0,0);
    // end of visual effect
    new effid=CreateEffectData(userid,attacker,ticks,tickdamage,0,0,0,0,g_respawns[userid],GetClientDeaths(userid));
    if (tickdelay==0.0 || ticks == 0 || tickdamage == 0)
    {
      LogError("ERROR: effects database corrupted.");
      DeleteEffectData(effid);
    }
    else if(effid!=0)
    {
      if(HasGlyph(attacker, GLYPH_WARRIOR_REND))
        wcPhysicalDamage(userid, attacker, RoundToCeil(0.15*tickdamage*ticks), "", "", true);
      CreateTimer(float(tickdelay), Effect4Action, effid, TIMER_REPEAT);
    }
  }
}

stock sendshakemsg(client, all:shakeflag, Float:amplitude, Float:frequency, Float:duration){

	new Handle:shakemsg;
	
	if (client == 0){
		
		shakemsg = StartMessageAll("Shake");
		
	}else{
		
		shakemsg = StartMessageOne("Shake", client);
		
	}

	if(CSGO){
    PbSetInt(shakemsg, "command", shakeflag);
    PbSetFloat(shakemsg, "local_amplitude", amplitude);
    PbSetFloat(shakemsg, "frequency", frequency);
    PbSetFloat(shakemsg, "duration", duration);
    EndMessage();
  }
  else{
    BfWriteByte(shakemsg, shakeflag);
    BfWriteFloat(shakemsg, amplitude);
    BfWriteFloat(shakemsg, frequency);
    BfWriteFloat(shakemsg, duration);
    EndMessage();
  }
	
}

Effect41(client,victim,damage,bonushit) //Bladestorm
{
  if(bonushit==0 && GetSweepingStrikes(client)>0)
  {
    decl String:line[255];
    new bool:bs=false;
    wcGetTalentInfo(1,2,4,1,client,"distance",line); //BladeStorm
    new Float:distance = StringToFloat(line);
    wcGetTalentInfo(1,2,4,1,client,"multi",line); //BladeStorm
    new Float:multi= StringToFloat(line);
    new Float:location[3];
    GetClientAbsOrigin(client,location);

    new targets[MAXPLAYERS];
    new team=GetClientTeam(client); //1 Spectator, 2 Terrorist, 3 Counter-Terrorist
    new amount;
    if(team==2)
      amount=GetNearClients("@ct",location,targets,distance);
    else if(team==3)
      amount=GetNearClients("@t",location,targets,distance);
    for (new x=0;x<amount;x++)
    {
      if(targets[x] != victim && targets[x] != 0)
      {
        dealExtraHit(client,targets[x],RoundToNearest(damage*multi),"Blade Storm","Blade Storm");
        bs=true;
      }
    }
    if(bs)
    {
      SetSweepingStrikes(client,GetSweepingStrikes(client)-1);
      if(GetSweepingStrikes(client)<1)
        WCMessage(client,"Bladestorm Ends",client);
    }
  }
}

Effect42(victim,&damage)
{
  damage=RoundToNearest(damage*(1+GetPoisoned2(victim)));
}

Effect43(client,&damage)
{
  decl String:line[255];
  wcGetTalentInfo(3,3,3,2,client,"multi",line);
  damage=RoundToNearest(damage*(1+StringToFloat(line)));
}

/*Effect44(client,&damage)
{
  decl String:line[255];
  wcGetTalentInfo(4,2,4,1,client,"multi",line);
  damage=RoundToNearest(damage*(1+StringToFloat(line)));
}*/

Effect45(client,&damage)
{
  damage=RoundToNearest(damage*(1-GetNBlessing(client)));
}

public Action:EffectCursedSoulDamage(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--;

  decl String:line[255];
  wcGetTalentInfo(7,1,4,1,effectsdata[effid][2],"multi",line);
  new Float:multi=StringToFloat(line);
  new client = effectsdata[effid][2];
  new victim = effectsdata[effid][1];
  new healthp = RoundToNearest((float(wcGetHealth(victim)) / float(GetMaxHealth(victim))) * 100.0);
  if(healthp >= 70)
    multi = (0.8 - float(wcGetHealth(victim)) / float(GetMaxHealth(victim))) * multi;
  if(healthp < 70 && healthp >= 50)
    multi = (0.9 - float(wcGetHealth(victim)) / float(GetMaxHealth(victim))) * multi;
  if(healthp < 50 && healthp >= 30)
    multi = (1.00 - float(wcGetHealth(victim)) / float(GetMaxHealth(victim))) * multi;
  if(healthp < 30 && healthp >= 15)
    multi = (1.10 - float(wcGetHealth(victim)) / float(GetMaxHealth(victim))) * multi;
  if(healthp < 15)
    multi = (1.30 - float(wcGetHealth(victim)) / float(GetMaxHealth(victim))) * multi;
  if(HasGlyph(client, GLYPH_WARLOCK_CURSEDSOUL))
  {
    new Float:percentage = (1.5 - float(wcGetHealth(client)) / float(GetMaxHealth(victim)));
    if(1.0>percentage || percentage > 1.5)
      percentage=1.0;
    multi*=percentage;
  
  }
  new damage=effectsdata[effid][4] + GetSpellDmg(client,multi);
  new aoe = effectsdata[effid][5];

  wcSpellAoeDamage(effectsdata[effid][1],effectsdata[effid][2],damage,aoe,effectsdata[effid][6],line);
  new Float:distance=float(aoe);
  new Float:location[3];
  GetClientAbsOrigin(effectsdata[effid][2], location);
  new targets[MAXPLAYERS];
  new amount;
  new team = effectsdata[effid][6];
  if(team==2)
    amount=GetNearClients("@t",location,targets,distance);
  else if(team==3)
    amount=GetNearClients("@ct",location,targets,distance);
  for (new x=0;x<amount;x++)
  {
    if(GetHM(targets[x] < 0.40))
      continue;
    SetHM(targets[x],0.40);
    SetHMT(targets[x],3.0);
  }
  if(HasGlyph(effectsdata[effid][2], GLYPH_WARLOCK_SDOOM))
  {
    //new effid=CreateEffectData(target,client,ticks,dmgef,aoe,team,0,0,g_RoundCount); //This is just for reference
    GetClientAbsOrigin(effectsdata[effid][1],location);
    if(effectsdata[effid][6] == 2)
      team = 3;
    if(effectsdata[effid][6] == 3)
      team = 2;
    new x = 0;
    if(team ==  2)
      amount=GetNearClients("@ct",location,targets,float(aoe));
    if(team == 3)
      amount=GetNearClients("@t",location,targets,float(aoe));
    decl String:name[255],String:name2[255];
    GetClientName(effectsdata[effid][2],name,sizeof(name));
    GetClientName(targets[x],name2,sizeof(name2));
    if(amount>=3)
    {
      new Float:oldspeed=GetClientSpeed(client);
      new Float:normalspeed=GetNormalSpeed(client);
      multi=1.03;
      new Float:newspeed=(oldspeed * multi);
      new Float:additionalspeed=newspeed-normalspeed;
      if((newspeed>normalspeed) && (additionalspeed < 0.300))
      {
       // WCMessage(client,"swift doom glyph",client,RoundToNearest(newspeed*100));
        SetClientSpeed(client,newspeed);
        new Float:time=5.0; //High numbers for testing
        new Handle:datapack=CreateDataPack();
        WritePackCell(datapack,client); // Write the client index of owner
        WritePackFloat(datapack,multi);
        WritePackFloat(datapack,normalspeed);
        CreateTimer(time,RemoveDottedSpeed,datapack,TIMER_HNDL_CLOSE);
      }
    }
  }
  return Plugin_Continue;
}

Effect46(client,damage)
{
  decl String:vicMsg[300];
  Format(vicMsg,sizeof(vicMsg),"%T","spirit link victim", GetSpiritLink(client));
  wcSpellDamage(GetSpiritLink(client),client,damage,"", vicMsg,"Spirit Link");
}

/*public Action:EffectShadowWellAction(Handle:timer, any:effid)
{
  decl String:line[255];
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  new client = effectsdata[effid][1];
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }

  new team = effectsdata[effid][2];
  new aoe = effectsdata[effid][3];
  new Float:location[3];
  location[0]=float(effectsdata[effid][4]);
  location[1]=float(effectsdata[effid][5]);
  location[2]=float(effectsdata[effid][6]);
  new targets[MAXPLAYERS];
  new amount;
  if(team==2)
    amount=GetNearClients("@ct",location,targets,float(aoe));
  if(team==3)
    amount=GetNearClients("@t",location,targets,float(aoe));
  wcGetTalentInfo(7,3,3,1,client,"slowmulti",line);
  new Float:slowmulti=StringToFloat(line);
  wcGetTalentInfo(7,3,3,1,client,"healingmulti",line);
  new Float:healmulti=StringToFloat(line);
  wcGetTalentInfo(7,3,3,1,client,"tickdelay",line);
  new Float:duration = StringToFloat(line)+0.02;
  new Float:distance;
  new Float:loc2[3];
  for (new x=0;x<=(amount-1);x++)
  {
    GetClientAbsOrigin(targets[x],loc2);
    distance = GetDistanceBetween(location,loc2);
    new Float:oldspeed=GetClientSpeed(targets[x]);
    new Float:newspeed=GetNormalSpeed(targets[x]);
    newspeed*=(1-slowmulti*(1-distance/aoe*0.8));
    if(newspeed<oldspeed)
    {
      SetClientSpeed(targets[x],newspeed);
      new Handle:datapack2=CreateDataPack();
      WritePackCell(datapack2,targets[x]); // Write the client index of owner
      WritePackFloat(datapack2,newspeed);
      WritePackFloat(datapack2,slowmulti);
      CreateTimer(duration,RemoveShadowWell,datapack2,TIMER_HNDL_CLOSE);
    }
    SetHM(targets[x],1-healmulti*(1-distance/aoe*0.8));
    SetHMT(targets[x],duration);
  }
  return Plugin_Continue;
}*/

Effect47(client,attacker,&damage)
{
  decl String:line[255];
  wcGetTalentInfo(8,1,3,1,client,"multi",line);
  new dmg = RoundToNearest(damage*StringToFloat(line));
  wcPhysicalDamage(attacker,client,dmg,"","",true);
  WCMessage(attacker,"heartfire victim",attacker,dmg);
  if(HasGlyph(client, GLYPH_DK_HEARTFIRE))
    damage = RoundToNearest(damage * 0.9);
}

Effect48(client,&damage)
{
  decl String:line[255];
  wcGetTalentInfo(8,3,2,1,GetPlagued(client),"multi",line);
  damage = RoundToNearest(damage*(1+StringToFloat(line)));
}

public Action:EffectAMZoneAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }

  new team = effectsdata[effid][2];
  new aoe = effectsdata[effid][3];
  new Float:location[3];
  location[0]=float(effectsdata[effid][4]);
  location[1]=float(effectsdata[effid][5]);
  location[2]=float(effectsdata[effid][6]);
  new targets[MAXPLAYERS];
  new amount;
  if(team==3)
    amount=GetNearClients("@ct",location,targets,float(aoe));
  if(team==2)
    amount=GetNearClients("@t",location,targets,float(aoe));

  for (new x=0;x<=(amount-1);x++)
  {
    RemoveEffectSpellVic(targets[x],49);
    SetEffectSpellVic(targets[x],49);
    SetZoned(targets[x],effid);
  }
  return Plugin_Continue;
}

Effect49(client,&damage)
{
  new effid = GetZoned(client);
  if (effectsdata[effid][0]==0)
  {
    RemoveEffectSpellVic(client,49);
  }
  else
  {
    decl String:line[255];
    new Float:location[3];
    location[0]=float(effectsdata[effid][4]);
    location[1]=float(effectsdata[effid][5]);
    location[2]=float(effectsdata[effid][6]);
    new Float:loc2[3];
    GetClientAbsOrigin(client,loc2);

    wcGetTalentInfo(8,3,4,1,client,"distance",line);
    new Float:distance = GetDistanceBetween(loc2,location)/float(StringToInt(line));
    if(distance<=1.0)
    {
      wcGetTalentInfo(8,3,4,1,client,"multi",line);
      new Float:multi=StringToFloat(line);
      damage = RoundToNearest(damage*multi);
    }
    else
    {
      RemoveEffectSpellVic(client,49);
    }
  }
}

public Action:EffectMindFlayAction(Handle:timer, any:effid)
{
  if (effectsdata[effid][0]==0)
    return Plugin_Stop;
  if (!IsClientInGame(effectsdata[effid][1]) || !IsClientInGame(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (!IsPlayerAlive(effectsdata[effid][1]) || !IsPlayerAlive(effectsdata[effid][2]))
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][3]<=0)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  if (effectsdata[effid][9]!=g_RoundCount)
  {
    DeleteEffectData(effid);
    return Plugin_Stop;
  }
  effectsdata[effid][3]--;
  decl Float:vec[3],Float:rightVec[3],Float:upVec[3];
  GetClientAbsOrigin(effectsdata[effid][1], vec);
  vec[0] -= 30;
  vec[1] -= 30;
  GetVectorVectors(vec, rightVec, upVec);
  static const color[] = {255,40,40,200};
  if(!CSGO) {
    TE_SetupBloodSprite(vec, rightVec, color, 25, g_bloodsprayModel, g_bloodModel);
    TE_SendToAll();
  }

  new Float:location[3], Float:locationatt[3];
  GetClientAbsOrigin(effectsdata[effid][2], locationatt);
  GetClientAbsOrigin(effectsdata[effid][1], location);
  location[2]+=40;
  locationatt[2]+=40;
  VS_BeamPoints(location, locationatt, g_ef_scanner,g_ef_scanner, 1, 5, 1.00, 15.00, 15.00, 1, 1.00, {80, 0, 80, 255}, 9);
  VS_BeamPoints(location, locationatt, g_ef_scanner, g_ef_scanner, 1, 5, 1.00, 15.00, 15.00, 1, 0.00, {80, 0, 80, 255}, 9);
  VS_BeamPoints(locationatt, location, g_ef_glow1, g_ef_glow1, 1, 5, 1.00, 20.00, 10.00, 10, 0.00, {80, 0, 80, 255}, 8);
  VS_BeamPoints(locationatt, location, g_ef_lgtning, g_ef_glow1, 1, 5, 1.00, 20.00, 10.00, 0, 0.00, {80, 0, 80, 255}, 8);

  wcSpellDamage(effectsdata[effid][1],effectsdata[effid][2],effectsdata[effid][4],"","","Mind Flay");
  if(HasGlyph(effectsdata[effid][2], GLYPH_PRIEST_MINDBLAST))
    {
      new random = GetRandomInt(1,100);
      if (15 >= random)
      {
        new client = effectsdata[effid][2];
        decl String:line[255];
        wcGetSpellInfo(5,1,"cooldown",line);
        new Float:cd = StringToFloat(line);
        cd *= 0.8;
        SetCdSpell(client, 5, GetCDSpell(client, 5) - GetTickedTime() -cd, 2);
      }
    }
  return Plugin_Continue;
}

Effect50(client,attacker,&damage)
{
  damage=RoundToNearest(float(damage)*0.5);
  wcPhysicalDamage(GetBastion(client),attacker,damage, "", "", true);
  AddEMsgAff("unbreakable bastion",true);
}


public Effect51(client,damage,victim,bonushit) //Adrenaline Strike with Glyph
{
  if(!HasGlyph(client, GLYPH_ROGUE_RUSH))
    return;
  if(bonushit==0)
  {
    new Float:range = 225.0;
    damage = RoundToNearest(damage*0.20);
    new Float:location[3];
    GetClientAbsOrigin(victim,location);
    new target[2];
    new number = GetNearestPlayers(GetClientTeam(victim),location,range, target, 2);
    SetSweepingStrikes(client, GetSweepingStrikes(client)-1);
    if(target[1]!=0 && number > 1)
    {
      decl String:vicMsg[300];
      Format(vicMsg,sizeof(vicMsg),"%T","adrenaline target glyph", target[1]);
      decl String:attMsg[300];
      Format(attMsg,sizeof(attMsg),"%T","adrenaline attacker glyph", target[1]);
      dealExtraHit(client,target[1],damage,"Adrenaline Rush","Adrenaline Rush");
    }
  }
}
public Effect52(client, &damage)                                                //MoS, victim
{
  new Float:reduction=(0.05);
  damage = RoundToNearest(float(damage) * (1-reduction));
  return true;
}

//HP Bars functions
stock GetClientHpBarModel(hp, String:model[], size) {
  if(hp >= 100) {
    strcopy(model, size, g_mHp[0]);
  }
  else if(hp >= 80) {
    strcopy(model, size, g_mHp[1]);
  }
  else if(hp >= 60) {
    strcopy(model, size, g_mHp[2]);
  }
  else if(hp >= 40) {
    strcopy(model, size, g_mHp[3]);
  }
  else if(hp > 20 ) {
    strcopy(model, size, g_mHp[4]);
  }
  else {
    strcopy(model, size, g_mHp[5]);
  }

}

stock _spawnClientHpBar(client) {

  if(!IsClientInGame(client) || !IsPlayerAlive(client)) return -1;

  decl String:iTarget[16];
  Format(iTarget, 16, "client%d", client);
  DispatchKeyValue(client, "targetname", iTarget);

  decl Float:Origin[3];
  GetClientEyePosition(client,Origin);
  Origin[2] += 18.0;

  new Ent = CreateEntityByName("env_sprite");
  if(!Ent) return -1;

  decl String:model[255];
  GetClientHpBarModel(GetClientHealth(client), model, sizeof(model));
  SetEntityModel(Ent, model);
  /*DispatchKeyValue(Ent, "model", sprite);*/
  DispatchKeyValue(Ent, "classname", "env_sprite");
  DispatchKeyValue(Ent, "spawnflags", "1");
  DispatchKeyValue(Ent, "scale", "0.08");
  DispatchKeyValue(Ent, "rendermode", "1");
  DispatchKeyValue(Ent, "rendercolor", "255 255 255");
  DispatchSpawn(Ent);
  TeleportEntity(Ent, Origin, NULL_VECTOR, NULL_VECTOR);
  SetVariantString(iTarget);
  AcceptEntityInput(Ent, "SetParent", Ent, Ent, 0);

  return Ent;
}

stock SyncClientHpBar(client) {
  new ent = g_eHp[client];
  if(ent <= 0)  return;
  if(!IsValidEntity(ent)) return;
  new Float:pos[3];
  GetEntPropVector(ent, Prop_Send, "m_vecOrigin", pos);
  TeleportEntity(ent, pos, NULL_VECTOR, NULL_VECTOR); //Yes, we really teleport entities to the same place. Hug Valve for that amazing bug
}

public Action:RefreshClientHpBar(Handle:timer, any:client)  {
  RemoveClientHpBar(client);
  CheckClientHpBar(client);

  return Plugin_Handled;
}

stock CheckClientHpBar(client, hp=-1)  {
  new bool:exists = g_eHp[client] > 0;
  if(exists)  {
    exists = IsValidEntity(g_eHp[client]);
    if(!exists) {
      RemoveClientHpBar(client);
    }
  }

  new bool:should = (IsClientInGame(client) && IsPlayerAlive(client));

  if(exists && !should) {
    //Remove it
    RemoveClientHpBar(client);
  }
  if(exists && should)  {
    UpdateClientHpBar(client, hp);
  }
  if((!exists || !IsValidEntity(g_eHp[client]) ) && should) {
    g_eHp[client] = _spawnClientHpBar(client);
    if(g_eHp[client] > 0 )  {
      SDKHook(g_eHp[client], SDKHook_SetTransmit, TransmitHpBarHook);
    }
  }
}

stock UpdateClientHpBar(client, hp=-1) {
  decl String:model[255];
  if(hp == -1)  {
    hp = GetClientHealth(client);
  }
  GetClientHpBarModel(hp, model, sizeof(model));
  SetEntityModel(g_eHp[client], model);
}

stock RemoveClientHpBar(client) {
  if(g_eHp[client] > 0) {
    if(IsValidEntity(g_eHp[client])) SDKUnhook(g_eHp[client], SDKHook_SetTransmit, TransmitHpBarHook);
  }
  KillEntity(g_eHp[client]);
  g_eHp[client] = -1;
}
new g_iUnchaining[65];
new Float:g_fUnchaining[65];
stock Effect58(client)
{
  g_fUnchaining[client] = GetEngineTime() + 10.0;
  if(10>=g_iUnchaining[client])
  {
    g_iUnchaining[client]+=1;
    SetBonusAgi(client, (GetBonusAgi(client)+10*g_iUnchaining[client]));
  }
  new Handle:datapack=CreateDataPack();
  WritePackCell(datapack,client);
  WritePackCell(datapack,g_RoundCount);
  CreateTimer(10.0,EffectUnchaining,datapack,TIMER_HNDL_CLOSE);
}
public Action:EffectUnchaining(Handle:timer, Handle:datapack)
{
  ResetPack(datapack);
  new client = ReadPackCell(datapack);
  new round = ReadPackCell(datapack);
  if(g_fUnchaining[client]<GetEngineTime() || round != g_RoundCount)
  {
    SetBonusAgi(client, (GetBonusAgi(client)-10*g_iUnchaining[client]));
    g_iUnchaining[client]=0;
    g_fUnchaining[client]=0.0;
    WCMessage(client,"Unchaining end",client);
  }
}
public Effect59(client) //Energetic Recovery
{
  decl String:line[255];
  new Float:accuracy = GetBonusAccuracy(client)+GetAccuracy(client);
  new Float:random = GetRandomFloat(0.0,1.0);
  if(accuracy>random)
  {
    wcGetTalentInfo(2,2,2,2,client,"restore",line);
    new heal = RoundToNearest(StringToFloat(line)*float(GetMaxHealth(client)));
    wcHeal(client,client,heal,0,true);
  }
}










