/*
 * =====================================================================================
 *
 *       Filename:  keg.sp
 *
 *    Description:  An attempt to create flying objects, later to be used in wowmod.
 *
 *        Version:  1.0
 *        Created:  28/10/2013 20:24:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jevgenij Timosenko (jozh), stinkyfax@gmail.com
 *        Company:  sTeam
 *
 * =====================================================================================
 */
#pragma semicolon 1

#include <sourcemod>
#include <sdktools>
#include <cstrike>
#include "sdkhooks"

/*
 * Some Stock functions
 */
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


/*
 * Plugin Code
 */
public OnPluginStart()  {
  RegConsoleCmd("sm_keg", Command_Keg);
  RegConsoleCmd("sm_rocket", Command_Rocket);

}

public Action:Command_Rocket(client, args) {
  PrintToServer("rocket start");
  createRocket(client);
  PrintToServer("rocket end");
  return Plugin_Handled;
}

public Action:Command_Keg(client, args)  {
  PrintToServer("keg start");
  createKeg(client);
  PrintToServer("keg end");
  return Plugin_Handled;
}

public Action:GrenadeTouchHook(entity, other){
  if(other != 0){
    if(other == GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity")){
      return Plugin_Continue;
    }else{
      if(!IsEntityCollidable(other, true, true, true)){
        return Plugin_Continue;
      }
    }
  }

  GrenadeActive(entity);

  return Plugin_Continue;

}

public Action:GrenadeDamageHook(entity, &attacker, &inflictor, &Float:damage, &damagetype){
  if(GetEntProp(entity, Prop_Data, "m_takedamage") == DAMAGE_YES){
    GrenadeActive(entity);
  }

  return Plugin_Continue;
}

stock GrenadeActive(entity){

  SDKUnhook(entity, SDKHook_StartTouch, GrenadeTouchHook);
  SDKUnhook(entity, SDKHook_OnTakeDamage, GrenadeDamageHook);

  if(GetEntProp(entity, Prop_Data, "m_takedamage") == DAMAGE_YES){

    setTakeDamage(entity, DAMAGE_NO);//더이상 데미지를 안 입게 한다
    decl Float:entityposition[3];
    GetEntPropVector(entity, Prop_Send, "m_vecOrigin", entityposition);
    new client = GetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity");
    new gasentity = GetEntPropEnt(entity, Prop_Send, "m_hEffectEntity");//가스엔티티를 구한다
    AcceptEntityInput(gasentity, "Kill");
    entityposition[2] = entityposition[2] + 10.0;//폭발을 약간 위쪽으로, 데미지를 제대로 주기 위한 수단
    AcceptEntityInput(entity, "Kill");

  }

}

stock createRocket(client)  {

  decl Float:clienteyeangle[3], Float:anglevector[3], Float:clienteyeposition[3], Float:resultposition[3], entity;
  GetClientEyeAngles(client, clienteyeangle);
  GetClientEyePosition(client, clienteyeposition);
  GetAngleVectors(clienteyeangle, anglevector, NULL_VECTOR, NULL_VECTOR);
  NormalizeVector(anglevector, anglevector);
  //ScaleVector(anglevector, 10.0);
  AddVectors(clienteyeposition, anglevector, resultposition);
  NormalizeVector(anglevector, anglevector);
  ScaleVector(anglevector, 500.0); // speed

  entity = CreateEntityByName("hegrenade_projectile");
  SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", client);
  setTakeDamage(entity, DAMAGE_NO);
  DispatchSpawn(entity);
  new Float:vecmax[3] = {4.0, 4.0, 4.0};
  new Float:vecmin[3] = {-4.0, -4.0, -4.0};
  SetEntPropVector(entity, Prop_Send, "m_vecMins", vecmin);
  SetEntPropVector(entity, Prop_Send, "m_vecMaxs", vecmax);
  SetEntityMoveType(entity, MOVETYPE_FLY);
  SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", client);
  SetEntityModel(entity, "models/effects/combineball.mdl");  // flying chicken? :D
  TeleportEntity(entity, resultposition, clienteyeangle, anglevector);

  new gascloud = CreateEntityByName("env_rockettrail");
  DispatchKeyValueVector(gascloud,"Origin", resultposition);
  DispatchKeyValueVector(gascloud,"Angles", clienteyeangle);
  new Float:smokecolor[3] = {0.2, 0.0, 0.4};
  SetEntPropVector(gascloud, Prop_Send, "m_StartColor", smokecolor);
  SetEntPropFloat(gascloud, Prop_Send, "m_Opacity", 0.8);
  SetEntPropFloat(gascloud, Prop_Send, "m_SpawnRate", 150.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_ParticleLifetime", 1.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_StartSize", 5.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_EndSize", 30.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_SpawnRadius", 0.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_MinSpeed", 0.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_MaxSpeed", 20.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_flFlareScale", 1.0);
  DispatchSpawn(gascloud);
  decl String:target[64];
  Format(target, 64, "target%d_%f", client, GetGameTime());
  DispatchKeyValue(entity, "targetname", target);
  SetVariantString(target);
  AcceptEntityInput(gascloud, "SetParent");
  SetEntPropEnt(entity, Prop_Send, "m_hEffectEntity", gascloud);

  /*playsound(client, SOUNDTYPEROCKETFIRE1, clienteyeposition);*/
  /*playingrocketsound[entity] = true;*/
  /*playentitysound(entity, SOUNDROCKET1, resultposition);*/
  SDKHook(entity, SDKHook_StartTouch, GrenadeTouchHook);
  SDKHook(entity, SDKHook_OnTakeDamage, GrenadeDamageHook);

  /*new Handle:datapack = CreateDataPack();*/
  /*WritePackCell(datapack, EntIndexToEntRef(entity));*/
  /*CreateTimer(0.1, RocketSeekThink, datapack, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);*/
  setTakeDamage(entity, DAMAGE_YES);


}



stock createKeg(client) {
  decl Float:clienteyeangle[3], Float:anglevector[3], Float:clienteyeposition[3], Float:resultposition[3], entity;
  GetClientEyeAngles(client, clienteyeangle);
  GetClientEyePosition(client, clienteyeposition);
  GetAngleVectors(clienteyeangle, anglevector, NULL_VECTOR, NULL_VECTOR);
  NormalizeVector(anglevector, anglevector);
  //ScaleVector(anglevector, 10.0);
  AddVectors(clienteyeposition, anglevector, resultposition);
  NormalizeVector(anglevector, anglevector);
  ScaleVector(anglevector, 350.0); // Speed

  //이제 중요한 것 하나, 플레이어의 이동속도를 유탄의 속도에 더해야한다.
  //현실성은 중요한것
  decl Float:playerspeed[3];
  GetEntPropVector(client, Prop_Data, "m_vecVelocity", playerspeed);
  AddVectors(anglevector, playerspeed, anglevector);

  entity = CreateEntityByName("hegrenade_projectile");
  SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", client);
  setTakeDamage(entity, DAMAGE_NO);
  DispatchSpawn(entity);
  SetEntPropEnt(entity, Prop_Send, "m_hOwnerEntity", client);
  SetEntPropFloat(entity, Prop_Data, "m_flGravity", 0.15);
  /*SetEntityModel(entity, "models/Items/ar2_grenade.mdl");//유탄*/
  SetEntityModel(entity, "models/lduke/chicken/chicken3.mdl");  // flying chicken? :D
  // Play sound

  SDKHook(entity, SDKHook_StartTouch, GrenadeTouchHook);
  SDKHook(entity, SDKHook_OnTakeDamage, GrenadeDamageHook);

  setTakeDamage(entity, DAMAGE_YES);
  TeleportEntity(entity, resultposition, clienteyeangle, anglevector);

  new gascloud = CreateEntityByName("env_smoketrail");
  DispatchKeyValueVector(gascloud,"Origin", resultposition);
  DispatchKeyValueVector(gascloud,"Angles", clienteyeangle);
  new Float:smokecolor[3] = {1.0, 1.0, 1.0};
  new Float:endcolor[3] = {0.0, 0.0, 0.0};
  SetEntPropVector(gascloud, Prop_Send, "m_StartColor", smokecolor);
  SetEntPropVector(gascloud, Prop_Send, "m_EndColor", endcolor);
  SetEntPropFloat(gascloud, Prop_Send, "m_Opacity", 0.2);
  SetEntPropFloat(gascloud, Prop_Send, "m_SpawnRate", 48.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_ParticleLifetime", 1.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_StartSize", 5.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_EndSize", 30.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_SpawnRadius", 0.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_MinSpeed", 0.0);
  SetEntPropFloat(gascloud, Prop_Send, "m_MaxSpeed", 10.0);
  DispatchSpawn(gascloud);
  SetVariantString("!activator");
  AcceptEntityInput(gascloud, "SetParent", entity);
  SetEntPropEnt(entity, Prop_Send, "m_hEffectEntity", gascloud);



}
