/*
* File: visual.sp
* Author: Stinkyfax
* Description: Visualization of the game, inspired by EST Effects
* Homepage: http://wowmod.eu/
* Plugin is private.
*/

#pragma semicolon 1

#define STR_VERSION "1.3"
#define VERSION 3

#include <sdktools>
#include <vs_natives>


public Plugin:myinfo =
{
  name = "visual",
  author = "Stinkyfax",
  description = "Creates visual effects mainly based on temp entities",
  version = STR_VERSION,
  url = "http://visual.wowmod.eu/"
};

public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
  RegisterNatives();
  RegPluginLibrary("visual");
  return APLRes_Success;
}

public OnPluginStart()  {

  PrintToServer("Visual Studio is loading");
  PrintToServer("Visual Studio commands is using late precaching algorithm, which is bad for non-testing server");
  PrintToServer("As a result, console commands for effects only for testing purposes");
  PrintToServer("Any plugin using Visual Studio should call natives instead of commands and precache models on map start");
  PrintToServer("You have been warned!");

  RegisterCommands();



  PrintToServer("Visual Studio is loaded");
}

public RegisterNatives()	{
  CreateNative("VS_Spark", Native_Spark);
  //TE_SetupSmoke(const Float:pos[3], Model, Float:Scale, FrameRate)
  CreateNative("VS_Smoke", Native_Smoke);
  //const Float:pos[3], const Float:dir[3], Float:Size, Float:Speed, Float:delay=0.0
  CreateNative("VS_Dust", Native_Dust);
  CreateNative("VS_MuzzleFlash", Native_MuzzleFlash);
  CreateNative("VS_MetalSpark", Native_MetalSpark);
  CreateNative("VS_EnergySplash", Native_EnergySplash);
  CreateNative("VS_ArmorRicochet", Native_ArmorRicochet);
  CreateNative("VS_GlowSprite", Native_GlowSprite);
  CreateNative("VS_Explosion", Native_Explosion);
  CreateNative("VS_BloodSprite", Native_BloodSprite);
  CreateNative("VS_BeamRingPoint", Native_BeamRingPoint);
  CreateNative("VS_BeamPoints", Native_BeamPoints);
  CreateNative("VS_BeamLaser", Native_BeamLaser);
  CreateNative("VS_BeamRing", Native_BeamRing);
  CreateNative("VS_BeamFollow", Native_BeamFollow);
  CreateNative("VS_Bubbles", Native_Bubbles);
}

public Native_BeamFollow(Handle:plugin, numParams)  {
//doBeamFollow(EntIndex, ModelIndex, HaloIndex, Float:Life, Float:Width, Float:EndWidth, FadeLength, const Color[4], Float:delay=0.0)
  new entity = GetNativeCell(1);
  new Model = GetNativeCell(2);
  new haloModel = GetNativeCell(3);
  new Float:life = GetNativeCell(4);
  new Float:width = GetNativeCell(5);
  new Float:end_width = GetNativeCell(6);
  new fade_length = GetNativeCell(7);
  new color[4];
  GetNativeArray(8,color,4);
  new Float:delay = GetNativeCell(9);
  doBeamFollow(entity, Model, haloModel, life, width, end_width, fade_length, color, delay);

}

public Native_BeamRing(Handle:plugin, numParams)
{
  new start_entity = GetNativeCell(1);
  new end_entity = GetNativeCell(2);
  new Model = GetNativeCell(3);
  new haloModel = GetNativeCell(4);
  new start_frame = GetNativeCell(5);
  new frame_rate = GetNativeCell(6);
  new Float:life = GetNativeCell(7);
  new Float:width = GetNativeCell(8);
  new Float:amplitude = GetNativeCell(9);

  new color[4];
  GetNativeArray(10, color, 4);

  new speed = GetNativeCell(11);
  new flags = GetNativeCell(12);

  new Float:delay = GetNativeCell(13);

  doBeamRing(start_entity, end_entity, Model, haloModel, start_frame, frame_rate, life, width,
    amplitude, color, speed, flags, delay);
}


public Native_Bubbles(Handle:plugin, numParams)
{
  //doBubbles(Float:effectVector1[3], Float:effectVector2[3], Float:height, modelIndex, count, Float:speed)   {
  //"VS_Bubbles(modelId(%s), Float:min[3], Float:max[3], %.2f, %d, %.2f);",
  //     path, height, count, speed);
  new Float:vec1[3], Float:vec2[3];
  GetNativeArray(2, vec1, 3);
  GetNativeArray(3, vec2, 3);
  new Model = GetNativeCell(1);

  new Float:height = GetNativeCell(4);
  new count = GetNativeCell(5);
  new Float:speed = GetNativeCell(6);

  doBubbles(vec1, vec2, height, Model, count, speed);

}

public Native_BeamLaser(Handle:plugin, numParams)
{

  new start = GetNativeCell(1);
  new end = GetNativeCell(2);

  new Model = GetNativeCell(3);
  new haloModel = GetNativeCell(4);
  new start_frame = GetNativeCell(5);
  new frame_rate = GetNativeCell(6);
  new Float:life = GetNativeCell(7);
  new Float:width = GetNativeCell(8);
  new Float:end_width = GetNativeCell(9);
  new fade_length = GetNativeCell(10);
  new Float:amplitude = GetNativeCell(11);

  new color[4];
  GetNativeArray(12, color, 4);

  new speed = GetNativeCell(13);
  new Float:delay = GetNativeCell(14);

  doBeamLaser(start, end, Model, haloModel, start_frame, frame_rate, life, width, end_width, fade_length,
    amplitude, color, speed, delay);
}


public Native_BeamPoints(Handle:plugin, numParams)
{
  new Float:vec1[3], Float:vec2[3];
  GetNativeArray(1, vec1, 3);
  GetNativeArray(2, vec2, 3);

  new Model = GetNativeCell(3);
  new haloModel = GetNativeCell(4);
  new start_frame = GetNativeCell(5);
  new frame_rate = GetNativeCell(6);
  new Float:life = GetNativeCell(7);
  new Float:width = GetNativeCell(8);
  new Float:end_width = GetNativeCell(9);
  new fade_length = GetNativeCell(10);
  new Float:amplitude = GetNativeCell(11);

  new color[4];
  GetNativeArray(12, color, 4);

  new speed = GetNativeCell(13);
  new Float:delay = GetNativeCell(14);

  doBeamPoints(vec1, vec2, Model, haloModel, start_frame, frame_rate, life, width, end_width, fade_length,
    amplitude, color, speed, delay);
}

public Native_BeamRingPoint(Handle:plugin, numParams)
{
  new Float:vec1[3];
  GetNativeArray(1, vec1, 3);

  new Float:start_radius = GetNativeCell(2);
  new Float:end_radius = GetNativeCell(3);
  new Model = GetNativeCell(4);
  new haloModel = GetNativeCell(5);
  new start_frame = GetNativeCell(6);
  new frame_rate = GetNativeCell(7);
  new Float:life = GetNativeCell(8);
  new Float:width = GetNativeCell(9);
  new Float:amplitude = GetNativeCell(10);

  new color[4];
  GetNativeArray(11, color, 4);

  new speed = GetNativeCell(12);
  new flags = GetNativeCell(13);

  new Float:delay = GetNativeCell(14);

  doBeamRingPoint(vec1, start_radius, end_radius, Model, haloModel, start_frame, frame_rate, life, width,
    amplitude, color, speed, flags, delay);
}


public Native_BloodSprite(Handle:plugin, numParams)
{
  new Float:vec1[3], Float:vec2[3];
  GetNativeArray(1, vec1, 3);
  GetNativeArray(2, vec2, 3);

  new color[4];
  GetNativeArray(3, color, 4);

  new size = GetNativeCell(4);
  new sprayModel = GetNativeCell(5);
  new dropModel = GetNativeCell(6);

  new Float:delay = GetNativeCell(7);

  doBloodSprite(vec1, vec2, color, size, sprayModel, dropModel, delay);
}

public Native_Explosion(Handle:plugin, numParams)
{
  new Float:vec1[3], Float:vec2[3];
  GetNativeArray(1, vec1, 3);
  new Model = GetNativeCell(2);
  new Float:scale = GetNativeCell(3);
  new framerate = GetNativeCell(4);
  new flags = GetNativeCell(5);
  new radius = GetNativeCell(6);
  new magnitude = GetNativeCell(7);
  GetNativeArray(8, vec2, 3);
  new type = GetNativeCell(9);

  new Float:delay = GetNativeCell(10);

  doExplosion(vec1, Model, scale, framerate, flags, radius, magnitude, vec2, type, delay);
}


public Native_GlowSprite(Handle:plugin, numParams)
{
  new Float:vec1[3];
  GetNativeArray(1, vec1, 3);
  new Model = GetNativeCell(2);
  new Float:life = GetNativeCell(3);
  new Float:size = GetNativeCell(4);
  new brightness = GetNativeCell(5);

  new Float:delay = GetNativeCell(6);

  doGlowSprite(vec1, Model, life, size, brightness, delay);
}

public Native_ArmorRicochet(Handle:plugin, numParams)
{
  new Float:vec1[3];
  GetNativeArray(1, vec1, 3);
  new Float:vec2[3];
  GetNativeArray(2, vec2, 3);

  new Float:delay = GetNativeCell(3);

  doArmorRicochet(vec1, vec2, delay);
}

public Native_EnergySplash(Handle:plugin, numParams)
{
  new Float:vec1[3];
  GetNativeArray(1, vec1, 3);
  new Float:vec2[3];
  GetNativeArray(2, vec2, 3);

  new bool:expl = GetNativeCell(3);
  new Float:delay = GetNativeCell(4);

  doEnergySplash(vec1, vec2, expl, delay);
}

public Native_MuzzleFlash(Handle:plugin, numParams)
{
  new Float:vec1[3];
  GetNativeArray(1, vec1, 3);
  new Float:vec2[3];
  GetNativeArray(2, vec2, 3);

  new Float:scale = GetNativeCell(3);
  new type = GetNativeCell(4);
  new Float:delay = GetNativeCell(5);

  doMuzzleFlash(vec1, vec2, scale, type, delay);
}

public Native_Dust(Handle:plugin, numParams)
{
  new Float:vec1[3];
  GetNativeArray(1, vec1, 3);
  new Float:vec2[3];
  GetNativeArray(2, vec2, 3);

  new Float:size = GetNativeCell(3);
  new Float:speed = GetNativeCell(4);
  new Float:delay = GetNativeCell(5);

  doDust(vec1, vec2, size, speed, delay);
}

public Native_Smoke(Handle:plugin, numParams)
{
  new Float:vec1[3];
  GetNativeArray(1, vec1, 3);

  new modelId = GetNativeCell(2);
  new Float:scale = GetNativeCell(3);
  new rate = GetNativeCell(4);

  new Float:delay = GetNativeCell(5);

  doSmoke(vec1, modelId, scale, rate, delay);
}

public Native_Spark(Handle:plugin, numParams)
{
  new Float:vec1[3];
  GetNativeArray(1, vec1, 3);
  new Float:vec2[3];
  GetNativeArray(2, vec2, 3);

  new magnitude = GetNativeCell(3);
  new trail = GetNativeCell(4);
  new Float:delay = GetNativeCell(5);

  doSparks(vec1, vec2, magnitude, trail, delay);
}

public Native_MetalSpark(Handle:plugin, numParams)
{
  new Float:vec1[3];
  GetNativeArray(1, vec1, 3);
  new Float:vec2[3];
  GetNativeArray(2, vec2, 3);

  new Float:delay = GetNativeCell(5);

  doMetalSparks(vec1, vec2, delay);
}

RegisterCommands()  {
  RegConsoleCmd("vs_location", Cmd_Location, "Gets your current eyes location");
  RegAdminCmd("vs_sparks",Cmd_Sparks,ADMFLAG_CUSTOM1, "Creates sparks effect");
  RegAdminCmd("vs_smoke",Cmd_Smoke,ADMFLAG_CUSTOM1, "Creates smoke effect");
  RegAdminCmd("vs_dust",Cmd_Dust,ADMFLAG_CUSTOM1, "Creates dust effect");
  RegAdminCmd("vs_muzzle",Cmd_Muzzle,ADMFLAG_CUSTOM1, "Creates muzzle flash");
  RegAdminCmd("vs_metalsparks",Cmd_MetalSparks,ADMFLAG_CUSTOM1, "Creates metal sparks");
  RegAdminCmd("vs_energysplash",Cmd_EnergySplash,ADMFLAG_CUSTOM1, "Creates energy splash");
  RegAdminCmd("vs_armorricochet",Cmd_ArmorRicochet,ADMFLAG_CUSTOM1, "Creates armor ricochet");
  RegAdminCmd("vs_glowsprite",Cmd_GlowSprite,ADMFLAG_CUSTOM1, "Creates glow sprite");
  RegAdminCmd("vs_explosion",Cmd_Explosion,ADMFLAG_CUSTOM1, "Creates an explosion");
  RegAdminCmd("vs_bloodsprite",Cmd_BloodSprite,ADMFLAG_CUSTOM1, "Creates blood sprite");
  RegAdminCmd("vs_beampoints",Cmd_BeamPoints,ADMFLAG_CUSTOM1, "Creates beam points");
  RegAdminCmd("vs_beamringpoint",Cmd_BeamRingPoint,ADMFLAG_CUSTOM1, "Creates beam ring point");
  RegAdminCmd("vs_beamlaser",Cmd_BeamLaser,ADMFLAG_CUSTOM1, "Creates beam laser");
  RegAdminCmd("vs_beamring",Cmd_BeamRing,ADMFLAG_CUSTOM1, "Creates beam ring");
  RegAdminCmd("vs_beamfollow",Cmd_BeamFollow,ADMFLAG_CUSTOM1, "Creates beam follow");
  RegAdminCmd("vs_lightstyle",Cmd_LightStyle,ADMFLAG_CUSTOM1, "Sets a light style");
  RegAdminCmd("vs_bubbles",Cmd_Bubbles,ADMFLAG_CUSTOM1, "Creates Bubbles effect");
}

public Action:Cmd_Location(client, argc)  {
  new Float:loc[3];
  if(client==0) {
    ReplyToCommand(client, "Is client only command");
    return Plugin_Handled;
  }
  GetClientEyePosition(client, loc);
  decl String:buffer[255];
  Format(buffer, sizeof(buffer), "Location:   %.2f %.2f %.2f   You're Entity: %d", loc[0], loc[1], loc[2], client);
  ReplyToCommand(client, buffer);
  return Plugin_Handled;
}

public Action:Cmd_LightStyle(client,argc)  {
  if(argc!=1) {
    ReplyToCommand(client, "Syntax: <String:values>  where characters in value are a-z. max Length 64");
    return Plugin_Handled;
  }
  decl String:temp[64];
  GetCmdArg(1, temp, sizeof(temp));

  SetLightStyle(strlen(temp)-1, temp);

  return Plugin_Handled;
}

public Action:Cmd_Muzzle(client,argc)  {
  if(argc!=8) {
    ReplyToCommand(client, "Syntax: <vector:position> <vector:angles> <float:scale> <integer:type>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3];
  new Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(5, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(6, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  GetCmdArg(7, temp, sizeof(temp));
  new Float:scale = StringToFloat(temp);

  GetCmdArg(8, temp, sizeof(temp));
  new type = StringToInt(temp);

  doMuzzleFlash(vec1, vec2, scale, type);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"VS_MuzzleFlash(position?, {%.2f, %.2f, %.2f}, %.2f, %d, [delay?]);"
          , vec2[0], vec2[1], vec2[2], scale, type);
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_Dust(client,argc)  {
  if(argc!=8) {
    ReplyToCommand(client, "Syntax: <vector:position> <vector:direction> <float:size> <float:speed>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3];
  new Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(5, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(6, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  GetCmdArg(7, temp, sizeof(temp));
  new Float:size = StringToFloat(temp);

  GetCmdArg(8, temp, sizeof(temp));
  new Float:speed = StringToFloat(temp);

  doDust(vec1, vec2, size, speed);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"VS_Dust(position?, direction?, %.2f, %.2f, [delay?]);", size, speed);
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_Smoke(client,argc)  {
  if(argc!=6) {
    ReplyToCommand(client, "Syntax: <vector:position> <path:model> <float:scale> <integer:frameRate>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  decl String:path[200];
  GetCmdArg(4, path, sizeof(path));

  GetCmdArg(5, temp, sizeof(temp));
  new Float:scale = StringToFloat(temp);

  GetCmdArg(6, temp,sizeof(temp));
  new rate = StringToInt(temp);

  new Model = PrecacheModel(path);
  doSmoke(vec1, Model, scale, rate);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"VS_Smoke(position?, modelId(%s), %.2f, %d, [delay?]);", path, scale, rate);
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_Sparks(client,argc)  {
  if(argc!=8) {
    ReplyToCommand(client, "Syntax: <vector:position> <vector:direction> <integer:magnitude> <integer:trail_length>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3];
  new Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(5, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(6, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  GetCmdArg(7, temp, sizeof(temp));
  new magnitude = StringToInt(temp);

  GetCmdArg(8, temp, sizeof(temp));
  new length = StringToInt(temp);

  doSparks(vec1, vec2, magnitude, length);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"VS_Sparks(position?, direction?, %d, %d, [delay?]);", magnitude, length);
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}




public Action:Cmd_MetalSparks(client,argc)  {
  if(argc!=6) {
    ReplyToCommand(client, "Syntax: <vector:position> <vector:direction>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3];
  new Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(5, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(6, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  doMetalSparks(vec1, vec2);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"VS_MetalSpark(position?, direction?, [delay?]);");
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_EnergySplash(client,argc)  {
  if(argc!=7) {
    ReplyToCommand(client, "Syntax: <vector:position> <vector:direction> <bool:explosive>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3];
  new Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(5, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(6, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  GetCmdArg(7, temp, sizeof(temp));
  new bool:explosive = bool:StringToInt(temp);

  doEnergySplash(vec1, vec2, explosive);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"VS_EnergySplash(position?, direction?, %s, [delay?]);", explosive?"true":"false" );
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_ArmorRicochet(client,argc)  {
  if(argc!=6) {
    ReplyToCommand(client, "Syntax: <vector:position> <vector:direction>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3];
  new Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(5, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(6, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  doArmorRicochet(vec1, vec2);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"VS_ArmorRicochet(position?, direction?, [delay?]);" );
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}


public Action:Cmd_GlowSprite(client,argc)  {
  if(argc!=7) {
    ReplyToCommand(client, "Syntax: <vector:position> <path:model> <float:life> <float:size> <integer:brightness>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  decl String:path[200];
  GetCmdArg(4, path, sizeof(path));

  GetCmdArg(5, temp, sizeof(temp));
  new Float:life = StringToFloat(temp);

  GetCmdArg(6, temp, sizeof(temp));
  new Float:size = StringToFloat(temp);

  GetCmdArg(7, temp,sizeof(temp));
  new brightness = StringToInt(temp);

  new Model = PrecacheModel(path);

  doGlowSprite(vec1, Model, life, size, brightness);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),"VS_GlowSprite(position?, modelId(%s), %.2f, %.2f, %d, [delay?]);", path, life, size, brightness);
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_Explosion(client,argc)  {
  if(argc!=13) {
    ReplyToCommand(client, "Syntax: <vector:position> <path:model> <float:scale> <int:framerate> <int:flags> <int:radius> <int:magnitude> <vector:normal> <char:materialType>");
    ReplyToCommand(client, "Example of arguments: 500.0 0.0 0.0 \"models/my_model.mdl\" 1.0 1 0 500 200 0.0 0.0 1.0 C");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3], Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  decl String:path[200];
  GetCmdArg(4, path, sizeof(path));

  GetCmdArg(5, temp, sizeof(temp));
  new Float:scale = StringToFloat(temp);

  GetCmdArg(6, temp, sizeof(temp));
  new framerate = StringToInt(temp);

  GetCmdArg(7, temp, sizeof(temp));
  new flags = StringToInt(temp);

  GetCmdArg(8, temp, sizeof(temp));
  new radius = StringToInt(temp);

  GetCmdArg(9, temp, sizeof(temp));
  new magnitude = StringToInt(temp);

  GetCmdArg(10, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(11, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(12, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  GetCmdArg(13, temp, sizeof(temp));
  new type = temp[0];

  new Model = PrecacheModel(path);

  doExplosion(vec1, Model, scale, framerate, flags, radius, magnitude, vec2, type);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),
          "VS_Explosion(position?, modelId(%s), %.2f, %d, %d, %d, %d, {%.2f, %.2f, %.2f}, '%c', [delay?]);",
          path, scale, framerate, flags, radius, magnitude, vec2[0], vec2[1], vec2[2], type);
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_BloodSprite(client,argc)  {
  if(argc!=13) {
    ReplyToCommand(client, "Syntax: <vector:position> <vector:direction> <RGBA> <int:size> <path:spray_model> <path:blood_drop_model>");
    ReplyToCommand(client, "Example arguments: 500 0 0  0 0 10  255 200 200 255 50 \"models/spray_model.mdl\" \"models/drop_model.mdl\"");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3], Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(5, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(6, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  new color[4];
  GetCmdArg(7, temp, sizeof(temp));
  color[0] = StringToInt(temp);
  GetCmdArg(8, temp, sizeof(temp));
  color[1] = StringToInt(temp);
  GetCmdArg(9, temp, sizeof(temp));
  color[2] = StringToInt(temp);
  GetCmdArg(10, temp, sizeof(temp));
  color[3] = StringToInt(temp);

  GetCmdArg(11, temp,sizeof(temp));
  new size = StringToInt(temp);

  decl String:path[200];
  GetCmdArg(12, path, sizeof(path));
  new sprayModel = PrecacheModel(path);

  GetCmdArg(13, path, sizeof(path));
  new dropModel = PrecacheModel(path);

  doBloodSprite(vec1, vec2, color, size, sprayModel, dropModel);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),
      "VS_BloodSprite(position?, {%.2f, %.2f, %.2f}, {%d, %d, %d, %d}, %d, modelId(%s), modelId(%s), [delay?]);",
       vec2[0], vec2[1], vec2[2], color[0], color[1], color[2], color[3], size, sprayModel, dropModel);
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_BeamRingPoint(client,argc)  {
  if(argc!=18) {
    ReplyToCommand(client, "Syntax: <vector:center> <float:start_radius> <float:end_radius> <path:model> <path:halo_model> <int:start_frame> <int:frame_rate>...");
    ReplyToCommand(client, "... <float:life> <float:width> <float:amplitude> <RGBA> <int:speed> <int:flags>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  new Float:start_radius = StringToFloat(temp);

  GetCmdArg(5, temp, sizeof(temp));
  new Float:end_radius = StringToFloat(temp);

  decl String:path[200];
  GetCmdArg(6, path, sizeof(path));
  new Model = PrecacheModel(path);

  decl String:path2[200];
  GetCmdArg(7, path2, sizeof(path2));
  new haloModel = PrecacheModel(path2);

  GetCmdArg(8, temp, sizeof(temp));
  new start_frame = StringToInt(temp);

  GetCmdArg(9, temp, sizeof(temp));
  new frame_rate = StringToInt(temp);

  GetCmdArg(10, temp, sizeof(temp));
  new Float:life = StringToFloat(temp);

  GetCmdArg(11, temp, sizeof(temp));
  new Float:width = StringToFloat(temp);

  GetCmdArg(12, temp, sizeof(temp));
  new Float:amplitude = StringToFloat(temp);

  new color[4];
  GetCmdArg(13, temp, sizeof(temp));
  color[0] = StringToInt(temp);
  GetCmdArg(14, temp, sizeof(temp));
  color[1] = StringToInt(temp);
  GetCmdArg(15, temp, sizeof(temp));
  color[2] = StringToInt(temp);
  GetCmdArg(16, temp, sizeof(temp));
  color[3] = StringToInt(temp);

  GetCmdArg(17, temp, sizeof(temp));
  new speed = StringToInt(temp);

  GetCmdArg(18, temp, sizeof(temp));
  new flags = StringToInt(temp);


  doBeamRingPoint(vec1, start_radius, end_radius, Model, haloModel, start_frame, frame_rate, life, width,
    amplitude, color, speed, flags);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),
      "VS_BeamRingPoint(center?, %.2f, %.2f, modelId(%s), modelId(%s), %d, %d, %.2f, %.2f, %.2f, {%d, %d, %d, %d}, %d, %d [delay?]);",
       start_radius, end_radius, path, path2, start_frame, frame_rate, life, width, amplitude,
       color[0], color[1], color[2], color[3], speed, flags);
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}


public Action:Cmd_BeamRing(client,argc)  {
  if(argc!=15) {
    ReplyToCommand(client, "Syntax: <int:start_entity> <int:end_entity> <path:model> <path:halo_model> <int:start_frame> <int:frame_rate>...");
    ReplyToCommand(client, "... <float:life> <float:width> <float:amplitude> <RGBA> <int:speed> <int:flags>");
    return Plugin_Handled;
  }
  decl String:temp[20];

  GetCmdArg(1, temp, sizeof(temp));
  new start_entity = StringToInt(temp);

  GetCmdArg(2, temp, sizeof(temp));
  new end_entity = StringToInt(temp);

  decl String:path[200];
  GetCmdArg(3, path, sizeof(path));
  new Model = PrecacheModel(path);

  decl String:path2[200];
  GetCmdArg(4, path2, sizeof(path2));
  new haloModel = PrecacheModel(path2);

  GetCmdArg(5, temp, sizeof(temp));
  new start_frame = StringToInt(temp);

  GetCmdArg(6, temp, sizeof(temp));
  new frame_rate = StringToInt(temp);

  GetCmdArg(7, temp, sizeof(temp));
  new Float:life = StringToFloat(temp);

  GetCmdArg(8, temp, sizeof(temp));
  new Float:width = StringToFloat(temp);

  GetCmdArg(9, temp, sizeof(temp));
  new Float:amplitude = StringToFloat(temp);

  new color[4];
  GetCmdArg(10, temp, sizeof(temp));
  color[0] = StringToInt(temp);
  GetCmdArg(11, temp, sizeof(temp));
  color[1] = StringToInt(temp);
  GetCmdArg(12, temp, sizeof(temp));
  color[2] = StringToInt(temp);
  GetCmdArg(13, temp, sizeof(temp));
  color[3] = StringToInt(temp);

  GetCmdArg(14, temp, sizeof(temp));
  new speed = StringToInt(temp);

  GetCmdArg(15, temp, sizeof(temp));
  new flags = StringToInt(temp);


  doBeamRing(start_entity, end_entity, Model, haloModel, start_frame, frame_rate, life, width,
    amplitude, color, speed, flags);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),
      "VS_BeamRing(entity, entity, modelId(%s), modelId(%s), %d, %d, %.2f, %.2f, %.2f, {%d, %d, %d, %d}, %d, %d [delay?]);",
       path, path2, start_frame, frame_rate, life, width, amplitude,
       color[0], color[1], color[2], color[3], speed, flags);
  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}


public Action:Cmd_BeamPoints(client,argc)  {
  if(argc!=20) {
    ReplyToCommand(client, "Syntax: <vector:start> <vector:end> <path:model> <path:halo_model> <int:start_frame> <int:frame_rate> ...");
    ReplyToCommand(client, "... <float:life> <float:width> <float:end_width> <int:fade_length> <float:amplitude> <RGBA> <int:speed>");
    return Plugin_Handled;
  }
  decl String:temp[20];
  new Float:vec1[3], Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(5, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(6, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  decl String:path[200];
  GetCmdArg(7, path, sizeof(path));
  new Model = PrecacheModel(path);

  decl String:path2[200];
  GetCmdArg(8, path2, sizeof(path2));
  new haloModel = PrecacheModel(path2);

  GetCmdArg(9, temp, sizeof(temp));
  new start_frame = StringToInt(temp);

  GetCmdArg(10, temp, sizeof(temp));
  new frame_rate = StringToInt(temp);

  GetCmdArg(11, temp, sizeof(temp));
  new Float:life = StringToFloat(temp);

  GetCmdArg(12, temp, sizeof(temp));
  new Float:width = StringToFloat(temp);

  GetCmdArg(13, temp, sizeof(temp));
  new Float:end_width = StringToFloat(temp);

  GetCmdArg(14, temp, sizeof(temp));
  new fade_length = StringToInt(temp);

  GetCmdArg(15, temp, sizeof(temp));
  new Float:amplitude = StringToFloat(temp);

  new color[4];
  GetCmdArg(16, temp, sizeof(temp));
  color[0] = StringToInt(temp);
  GetCmdArg(17, temp, sizeof(temp));
  color[1] = StringToInt(temp);
  GetCmdArg(18, temp, sizeof(temp));
  color[2] = StringToInt(temp);
  GetCmdArg(19, temp, sizeof(temp));
  color[3] = StringToInt(temp);

  GetCmdArg(20, temp, sizeof(temp));
  new speed = StringToInt(temp);

  doBeamPoints(vec1, vec2, Model, haloModel, start_frame, frame_rate, life, width, end_width, fade_length,
    amplitude, color, speed);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),
      "VS_BeamPoints(start?, end?, modelId(%s), modelId(%s), %d, %d, %.2f, %.2f, %.2f, %d, %.2f, {%d, %d, %d, %d}, %d,[delay?]);",
       path, path2, start_frame, frame_rate, life, width, end_width, fade_length, amplitude,
       color[0], color[1], color[2], color[3], speed);

  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_BeamLaser(client,argc)  {
  if(argc!=16) {
    ReplyToCommand(client, "Syntax: <int:start> <int:end> <path:model> <path:halo_model> <int:start_frame> <int:frame_rate> ...");
    ReplyToCommand(client, "... <float:life> <float:width> <float:end_width> <int:fade_length> <float:amplitude> <RGBA> <int:speed>");
    return Plugin_Handled;
  }
  decl String:temp[20];

  GetCmdArg(1, temp, sizeof(temp));
  new start = StringToInt(temp);
  GetCmdArg(2, temp, sizeof(temp));
  new end = StringToInt(temp);

  decl String:path[200];
  GetCmdArg(3, path, sizeof(path));
  new Model = PrecacheModel(path);

  decl String:path2[200];
  GetCmdArg(4, path2, sizeof(path2));
  new haloModel = PrecacheModel(path2);

  GetCmdArg(5, temp, sizeof(temp));
  new start_frame = StringToInt(temp);

  GetCmdArg(6, temp, sizeof(temp));
  new frame_rate = StringToInt(temp);

  GetCmdArg(7, temp, sizeof(temp));
  new Float:life = StringToFloat(temp);

  GetCmdArg(8, temp, sizeof(temp));
  new Float:width = StringToFloat(temp);

  GetCmdArg(9, temp, sizeof(temp));
  new Float:end_width = StringToFloat(temp);

  GetCmdArg(10, temp, sizeof(temp));
  new fade_length = StringToInt(temp);

  GetCmdArg(11, temp, sizeof(temp));
  new Float:amplitude = StringToFloat(temp);

  new color[4];
  GetCmdArg(12, temp, sizeof(temp));
  color[0] = StringToInt(temp);
  GetCmdArg(13, temp, sizeof(temp));
  color[1] = StringToInt(temp);
  GetCmdArg(14, temp, sizeof(temp));
  color[2] = StringToInt(temp);
  GetCmdArg(15, temp, sizeof(temp));
  color[3] = StringToInt(temp);

  GetCmdArg(16, temp, sizeof(temp));
  new speed = StringToInt(temp);

  doBeamLaser(start, end, Model, haloModel, start_frame, frame_rate, life, width, end_width, fade_length,
    amplitude, color, speed);

  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),
      "VS_BeamLaser(start?, end?, modelId(%s), modelId(%s), %d, %d, %.2f, %.2f, %.2f, %d, %.2f, {%d, %d, %d, %d}, %d,[delay?]);",
       path, path2, start_frame, frame_rate, life, width, end_width, fade_length, amplitude,
       color[0], color[1], color[2], color[3], speed);

  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_BeamFollow(client,argc)  {

  if(argc!=11) {
    ReplyToCommand(client, "Syntax: <int:entity> <path:model> <path:halo_model> <float:life> <float:width> <float:end_width> <int:fade_length> <RGBA>");
    return Plugin_Handled;
  }
  decl String:temp[20];

  GetCmdArg(1, temp, sizeof(temp));
  new entity = StringToInt(temp);

  decl String:path[200];
  GetCmdArg(2, path, sizeof(path));
  new Model = PrecacheModel(path);

  decl String:path2[200];
  GetCmdArg(3, path2, sizeof(path2));
  new haloModel = PrecacheModel(path2);

  GetCmdArg(4, temp, sizeof(temp));
  new Float:life = StringToFloat(temp);

  GetCmdArg(5, temp, sizeof(temp));
  new Float:width = StringToFloat(temp);

  GetCmdArg(6, temp, sizeof(temp));
  new Float:end_width = StringToFloat(temp);

  GetCmdArg(7, temp, sizeof(temp));
  new fade_length = StringToInt(temp);

  new color[4];
  GetCmdArg(8, temp, sizeof(temp));
  color[0] = StringToInt(temp);
  GetCmdArg(9, temp, sizeof(temp));
  color[1] = StringToInt(temp);
  GetCmdArg(10, temp, sizeof(temp));
  color[2] = StringToInt(temp);
  GetCmdArg(11, temp, sizeof(temp));
  color[3] = StringToInt(temp);

  doBeamFollow(entity, Model, haloModel, life, width, end_width, fade_length, color);
  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),
      "VS_BeamFollow(entity?, modelId(%s), modelId(%s), %.2f, %.2f, %.2f, %d, {%d, %d, %d, %d},[delay?]);",
       path, path2, life, width, end_width, fade_length,
       color[0], color[1], color[2], color[3]);

  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;
}

public Action:Cmd_Bubbles(client,argc)  {

  if(argc!=10) {
    ReplyToCommand(client, "Syntax: <vector:min> <vector:max> <path:model> <float:height> <int:count> <float:speed>");
    //<player Filter> <delay> <model> <Min "X Y Z"> <Max "X Y Z"> <heigth> <count> <speed>
    return Plugin_Handled;
  }

  decl String:temp[20];
  new Float:vec1[3];
  new Float:vec2[3];
  GetCmdArg(1, temp, sizeof(temp));
  vec1[0] = StringToFloat(temp);
  GetCmdArg(2, temp, sizeof(temp));
  vec1[1] = StringToFloat(temp);
  GetCmdArg(3, temp, sizeof(temp));
  vec1[2] = StringToFloat(temp);

  GetCmdArg(4, temp, sizeof(temp));
  vec2[0] = StringToFloat(temp);
  GetCmdArg(5, temp, sizeof(temp));
  vec2[1] = StringToFloat(temp);
  GetCmdArg(6, temp, sizeof(temp));
  vec2[2] = StringToFloat(temp);

  decl String:path[200];
  GetCmdArg(7, path, sizeof(path));
  new Model = PrecacheModel(path);

  GetCmdArg(8, temp, sizeof(temp));
  new Float:height = StringToFloat(temp);

  GetCmdArg(9, temp, sizeof(temp));
  new count = StringToInt(temp);

  GetCmdArg(10, temp, sizeof(temp));
  new Float:speed = StringToFloat(temp);

  doBubbles(vec1, vec2, height, Model, count, speed);
  ReplyToCommand(client,"Effect is translated to:");
  decl String:buffer[255];
  Format(buffer,sizeof(buffer),
      "VS_Bubbles(modelId(%s), Float:min[3], Float:max[3], %.2f, %d, %.2f);",
       path, height, count, speed);

  ReplyToCommand(client, buffer);
  ReplyToCommand(client, "Effect is running.");
  return Plugin_Handled;

}

doBubbles(Float:effectVector1[3], Float:effectVector2[3], Float:height, modelIndex, count, Float:speed)   {
  TE_Start("Bubbles");
  TE_WriteVector("m_vecMins", effectVector1);
  TE_WriteVector("m_vecMaxs", effectVector2);
  TE_WriteFloat("m_fHeight", height);
  TE_WriteNum("m_nModelIndex", modelIndex);
  TE_WriteNum("m_nCount", count);
  TE_WriteFloat("m_fSpeed", speed);
  TE_SendToAll();
}


//private Effect functions
stock doBeamFollow(EntIndex, ModelIndex, HaloIndex, Float:Life, Float:Width, Float:EndWidth, FadeLength, const Color[4], Float:delay=0.0) {
  TE_SetupBeamFollow(EntIndex, ModelIndex, HaloIndex, Life, Width, EndWidth, FadeLength, Color);
  TE_SendToAll(delay);
}

stock doBeamRing(StartEntity, EndEntity, ModelIndex, HaloIndex, StartFrame, FrameRate, Float:Life, Float:Width, Float:Amplitude, const Color[4], Speed, Flags, Float:delay=0.0) {
  TE_SetupBeamRing(StartEntity, EndEntity, ModelIndex, HaloIndex, StartFrame, FrameRate, Life, Width, Amplitude, Color, Speed, Flags);
  TE_SendToAll(delay);
}

stock doBeamLaser(StartEntity, EndEntity, ModelIndex, HaloIndex, StartFrame, FrameRate, Float:Life,
                Float:Width, Float:EndWidth, FadeLength, Float:Amplitude, const Color[4], Speed, Float:delay=0.0) {
  TE_SetupBeamLaser(StartEntity, EndEntity, ModelIndex, HaloIndex, StartFrame, FrameRate, Life,
                Width, EndWidth, FadeLength, Amplitude, Color, Speed);
}

stock doBeamPoints(const Float:start[3], const Float:end[3], ModelIndex, HaloIndex, StartFrame, FrameRate, Float:Life,
                Float:Width, Float:EndWidth, FadeLength, Float:Amplitude, const Color[4], Speed, Float:delay=0.0) {
  TE_SetupBeamPoints(start, end, ModelIndex, HaloIndex, StartFrame, FrameRate, Life,
                Width, EndWidth, FadeLength, Amplitude, Color, Speed);
  TE_SendToAll(delay);
}

stock doBeamRingPoint(const Float:center[3], Float:Start_Radius, Float:End_Radius, ModelIndex, HaloIndex, StartFrame,
                FrameRate, Float:Life, Float:Width, Float:Amplitude, const Color[4], Speed, Flags, Float:delay=0.0)  {
  TE_SetupBeamRingPoint(center, Start_Radius, End_Radius, ModelIndex, HaloIndex, StartFrame,
                FrameRate, Life, Width, Amplitude, Color, Speed, Flags);
  TE_SendToAll(delay);
}

stock doBloodSprite(const Float:pos[3], const Float:dir[3], const color[4], Size, SprayModel, BloodDropModel, Float:delay=0.0)  {
  TE_SetupBloodSprite(pos, dir, color, Size, SprayModel, BloodDropModel);
  TE_SendToAll(delay);
}

stock doExplosion(const Float:pos[3], Model, Float:Scale, Framerate, Flags, Radius, Magnitude, const Float:normal[3]={0.0, 0.0, 1.0}, MaterialType='C', Float:delay=0.0)  {
  TE_SetupExplosion(pos, Model, Scale, Framerate, Flags, Radius, Magnitude, normal, MaterialType);
  TE_SendToAll(delay);
}

stock doGlowSprite(const Float:pos[3], Model, Float:Life, Float:Size, Brightness, Float:delay=0.0)  {
  TE_SetupGlowSprite(pos, Model, Life, Size, Brightness);
  TE_SendToAll(delay);
}

stock doArmorRicochet(const Float:pos[3], const Float:dir[3], Float:delay=0.0) {
  TE_SetupArmorRicochet(pos, dir);
  TE_SendToAll(delay);
}

stock doEnergySplash(const Float:pos[3], const Float:dir[3], bool:Explosive, Float:delay=0.0) {
  TE_SetupEnergySplash(pos, dir, Explosive);
  TE_SendToAll(delay);
}

stock doMetalSparks(const Float:pos[3], const Float:dir[3], Float:delay=0.0) {
  TE_SetupMetalSparks(pos, dir);
  TE_SendToAll(delay);
}

stock doMuzzleFlash(const Float:pos[3], const Float:angles[3], Float:Scale, Type, Float:delay=0.0)  {
  TE_SetupMuzzleFlash(pos, angles, Scale, Type);
  TE_SendToAll(delay);
}

stock doSparks(const Float:pos[3], const Float:dir[3], Magnitude, TrailLength, Float:delay=0.0)  {

  TE_SetupSparks(pos, dir, Magnitude, TrailLength);
  TE_SendToAll(delay);
}

stock doSmoke(const Float:pos[3], Model, Float:Scale, FrameRate, Float:delay=0.0)  {

  TE_SetupSmoke(pos, Model, Scale, FrameRate);
  TE_SendToAll(delay);
}

stock doDust(const Float:pos[3], const Float:dir[3], Float:Size, Float:Speed, Float:delay=0.0)  {
  TE_SetupDust(pos, dir, Size, Speed);
  TE_SendToAll(delay);
}


stock GetAllClients(clients[])  {
  new c=0;
  for(new i=0; i < GetMaxClients(); i++)  {
    if(IsClientInGame(i))
      clients[c++]=i;
  }

  return c;

}







