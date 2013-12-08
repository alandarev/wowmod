/*
* File: offsets.inc
* Description: Finds offsets
* Author(s): Stinkyfax
*/

#include <sdktools>

#pragma semicolon 1

#define IS_ALIVE IsClientAlive

new Handle:hGameConf;
new Handle:hExplosionCreate;
new Handle:hEyePosition;
//new Handle:hEyeAngles;
new Handle:hWeaponDrop;

//balancer handles
new Handle:switchTeam;
new Handle:setModel;
new Handle:roundRespawn;
new Handle:weaponDrop;

new g_MyWeaponsOffset;  //Thx to PimpinJuice for this one
new ColorOffset;
new RenderModeOffset;
new GetVelocityOffset_0;
new GetVelocityOffset_1;
new BaseVelocityOffset;
new MoveTypeOffset;
new LifeStateOffset;
new bool:UnableToAttack[65];
new bool:bVanish[65];
new bool:UnableToVictim[65];
new bool:FreedomClient[65];
new bool:KingsClient[65];
new bool:LegacyClient[65];
new bool:ArcaneClient[65];
new Feared[65];
new Float:g_fearspeed[65][2];
new g_CollisionGroup;
new offsArmor;
new g_minimapoffset;
new g_ff_ownsPet[65];


public InitOffsets()
{
  if(!CSGO)
    g_minimapoffset=FindSendPropOffs("CCSPlayerResource", "m_bPlayerSpotted");
  /*if(g_minimapoffset==-1)*/
    /*SetFailState("[WC] Error: Couldn't find minimap offset, aborting");*/

  g_CollisionGroup=FindSendPropOffs("CCSPlayer","m_CollisionGroup");
  if(g_CollisionGroup==-1)
    SetFailState("[WC] Error: Couldn't find collision group offset, aborting");

  g_MyWeaponsOffset=FindSendPropOffs("CBaseCombatCharacter","m_hMyWeapons");
  if(g_MyWeaponsOffset==-1)
    SetFailState("[WC] Error: Couldn't find m_hMyWeapons offset group offset, aborting");

  ACTIVEWEAPON=FindSendPropOffs("CAI_BaseNPC", "m_hActiveWeapon");
  CLIPOFFSET=FindSendPropOffs("CBaseCombatWeapon", "m_iClip1");
  if(ACTIVEWEAPON==-1 || CLIPOFFSET==-1)
    SetFailState("[WC] Error: Couldn't find the active weapon/clip offset, aborting");
  ColorOffset=FindSendPropOffs("CAI_BaseNPC","m_clrRender");
  if(ColorOffset==-1)
    SetFailState("[WC] Error: Couldn't find the Color offset, aborting");
  RenderModeOffset=FindSendPropOffs("CBaseAnimating","m_nRenderMode");
  if(RenderModeOffset==-1)
    SetFailState("[WC] Error: Couldn't find the RenderMode offset, aborting");
  GetVelocityOffset_0=FindSendPropOffs("CBasePlayer","m_vecVelocity[0]");
  if(GetVelocityOffset_0==-1)
    SetFailState("[WC] Error: Couldn't find the GetVelocity_0 offset, aborting");
  GetVelocityOffset_1=FindSendPropOffs("CBasePlayer","m_vecVelocity[1]");
  if(GetVelocityOffset_1==-1)
    SetFailState("[WC] Error: Couldn't find the GetVelocity_1 offset, aborting");
  BaseVelocityOffset=FindSendPropOffs("CBasePlayer","m_vecBaseVelocity");
  if(BaseVelocityOffset==-1)
    SetFailState("[WC] Error: Couldn't find the BaseVelocity offset, aborting");
  MoveTypeOffset=FindSendPropOffs("CAI_BaseNPC","movetype");
  if(MoveTypeOffset==-1)
    SetFailState("[WC] Error: Couldn't find the MoveType offset, aborting");
  LifeStateOffset=FindSendPropOffs("CAI_BaseNPC","m_lifeState");
  if(LifeStateOffset==-1)
    SetFailState("[WC] Error: Couldn't find the LifeState offset, aborting");
  offsArmor=FindSendPropInfo("CCSPlayer","m_ArmorValue");
  if(offsArmor==-1)
    SetFailState("[WC] Error: Couldn't find the Armor offset, aborting");
}

public InitGameData()
{
  hGameConf=LoadGameConfigFile("plugin.wow");
  if(DEBUG)
    PrintToServer("line 81");
  StartPrepSDKCall(SDKCall_Static);
  if(DEBUG)
    PrintToServer("line 84");
  PrepSDKCall_SetFromConf(hGameConf,SDKConf_Signature,"ExplosionCreate");
  /* The function prototype:
  void ExplosionCreate( const Vector &center, const QAngle &angles,
  CBaseEntity *pOwner, int magnitude, int radius, int nSpawnFlags,
  float flExplosionForce, CBaseEntity *pInflictor )
  */
  if(DEBUG)
    PrintToServer("line 92");
  PrepSDKCall_AddParameter(SDKType_Vector,SDKPass_Pointer);
  PrepSDKCall_AddParameter(SDKType_QAngle,SDKPass_Pointer);
  PrepSDKCall_AddParameter(SDKType_CBasePlayer,SDKPass_Pointer,VDECODE_FLAG_ALLOWWORLD);
  PrepSDKCall_AddParameter(SDKType_PlainOldData,SDKPass_Plain);
  PrepSDKCall_AddParameter(SDKType_PlainOldData,SDKPass_Plain);
  PrepSDKCall_AddParameter(SDKType_PlainOldData,SDKPass_Plain);
  PrepSDKCall_AddParameter(SDKType_Float,SDKPass_Plain);
  PrepSDKCall_AddParameter(SDKType_CBasePlayer,SDKPass_Pointer);
  if(DEBUG)
    PrintToServer("line 102");
  hExplosionCreate=EndPrepSDKCall();

  // EyePosition SDK call
  if(DEBUG)
    PrintToServer("line 107");
  StartPrepSDKCall(SDKCall_Player);
  PrepSDKCall_SetFromConf(hGameConf,SDKConf_Virtual,"EyePosition");
  if(DEBUG)
    PrintToServer("line 111");
  PrepSDKCall_SetReturnInfo(SDKType_QAngle,SDKPass_ByValue);
  hEyePosition=EndPrepSDKCall();

  if(DEBUG)
    PrintToServer("line 116");
  // EyeAngles SDK call
  //StartPrepSDKCall(SDKCall_Player);
  //PrepSDKCall_SetFromConf(hGameConf,SDKConf_Virtual,"EyeAngles");
  //PrepSDKCall_SetReturnInfo(SDKType_QAngle,SDKPass_ByValue);
  //hEyeAngles=EndPrepSDKCall();
  if(DEBUG)
    PrintToServer("line 123");
  //weapon drop offset
  StartPrepSDKCall(SDKCall_Entity);
  PrepSDKCall_SetFromConf(hGameConf,SDKConf_Signature,"Weapon_Drop");
  if(DEBUG)
    PrintToServer("line 128");
  PrepSDKCall_AddParameter(SDKType_CBaseEntity,SDKPass_Pointer);
  PrepSDKCall_AddParameter(SDKType_Vector,SDKPass_Pointer,VDECODE_FLAG_ALLOWNULL);
  PrepSDKCall_AddParameter(SDKType_Vector,SDKPass_Pointer,VDECODE_FLAG_ALLOWNULL);
  if(DEBUG)
    PrintToServer("line 133");
  hWeaponDrop=EndPrepSDKCall();
  if(DEBUG)
    PrintToServer("line 136");


  // SDK Calls for team switching and model setting
	// taken from SM Super Commands by pRED*

  StartPrepSDKCall(SDKCall_Player);
  PrepSDKCall_SetFromConf(hGameConf, SDKConf_Signature, "SwitchTeam");
  PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
  switchTeam = EndPrepSDKCall();

  StartPrepSDKCall(SDKCall_Player);
  PrepSDKCall_SetFromConf(hGameConf, SDKConf_Signature, "SetModel");
  PrepSDKCall_AddParameter(SDKType_String, SDKPass_Pointer);
  setModel = EndPrepSDKCall();

  StartPrepSDKCall(SDKCall_Player);
  PrepSDKCall_SetFromConf(hGameConf, SDKConf_Signature, "RoundRespawn");
  roundRespawn = EndPrepSDKCall();
	
	// This one was taken from GunGame:SM by team06
  StartPrepSDKCall(SDKCall_Player);
  PrepSDKCall_SetFromConf(hGameConf, SDKConf_Signature, "Weapon_Drop");
  PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
  PrepSDKCall_AddParameter(SDKType_Bool, SDKPass_Plain);
  PrepSDKCall_AddParameter(SDKType_Bool, SDKPass_Plain);
  weaponDrop = EndPrepSDKCall();
}


public GetEyePosition(client,Float:output[3])
{
  SDKCall(hEyePosition,client,output);
}

public gameSetHealth(client,amount)
{
  if(amount<1)
  {
//    decl String:time[200],String:name[100];
//    FormatTime(time, sizeof(time),NULL_STRING);
//    GetClientName(client,name,sizeof(name));
//    LogMessage("Killed Player %s With wcSetHealth in time %s",name,time);
    ForcePlayerSuicide(client);
  }
  else  {
    _gameSetHealth(client, amount);
  }
  updateModHealth(client,amount);
}

stock wcGetHealth(client) {
  return g_mHealth[client];
}

_gameSetHealth(client, amount)  {
  SetEntProp(client,Prop_Send,"m_iHealth",amount,1);
  SetEntProp(client,Prop_Data,"m_iHealth",amount,1);
}

updateModHealth(client, amount=-2)  {
  if(amount == -2)
    amount = GetClientHealth(client);
  new hp = RoundToFloor(float(GetMaxHealth(client)) / 100.0 * float(amount));
  _setModHealth(client, hp);
}

_setModHealth(client, hp) {
  CheckClientHpBar(client, RoundToNearest(100.0 * hp /  GetMaxHealth(client) ));
  g_mHealth[client]=hp;
}

public wcSetHealth(client, amount)  {
  _setModHealth(client, amount);
  if(amount < 1)
    ForcePlayerSuicide(client);
  else
    updateGameHealth(client);
}

updateGameHealth(client)  {
  new hp = RoundToCeil(float(wcGetHealth(client)) / float(GetMaxHealth(client)) * 100.0);
  new bool:changed = g_gHealth[client] != hp;
  g_gHealth[client]=hp;
  if(hp > 0 && changed)  {
    SetEntProp(client,Prop_Send,"m_iHealth",hp,1);
    SetEntProp(client,Prop_Data,"m_iHealth",hp,1);
  }
}

checkHpChanges() {
  for(new x=1; x<= GetMaxClients(); x++)  {
    if(IsClientInGame(x)) {
      new hp = GetClientHealth(x);
      if(hp != g_gHealth[x])  {
        ChangedHp(x);
      }
    }
  }
}

ChangedHp(client) {
  new hp = GetClientHealth(client);
  if (!IsPlayerAlive(client))
    hp = 0;
  updateModHealth(client, hp);
}

public SetColor(entity,red,green,blue,alpha)
{
  if(IsValidEntity(entity))
  {
  if(red!=-1)
    SetEntData(entity,ColorOffset,red,1,true);
  if(green!=-1)
    SetEntData(entity,ColorOffset+1,green,1,true);
  if(blue!=-1)
    SetEntData(entity,ColorOffset+2,blue,1,true);
  if(alpha!=-1)
    SetEntData(entity,ColorOffset+3,alpha,1,true);
  SetEntData(entity,RenderModeOffset,3,1,true);
  }
}

public SetVelocity(client,const Float:velocity[3])
{
  SetEntDataVector(client,BaseVelocityOffset,velocity,true);
}

public GetHorizontalVelocity(client,Float:output[3])
{
  output[0]=GetEntDataFloat(client,GetVelocityOffset_0);
  output[1]=GetEntDataFloat(client,GetVelocityOffset_1);
}

public Freeze(entity,bool:freeze)
{
  if(freeze)
    SetEntData(entity,MoveTypeOffset,0,1,true);
  else
    SetEntData(entity,MoveTypeOffset,2,1,true);
}

Float:MaxFloat(Float:val, Float:val2) {
   if (val>val2)
      return val;
   return val2;
}

public ConvertString(String:from[], fromSize, String:to[], toSize)   {
   new stage=0;
   for(new i=0;i<strlen(from);i++)   {
      new bool:pass=true;
      new c = from[i];
      if(IsCharAlpha(c))   {

      }
      else
         pass=false;
      if((!pass) && (IsCharNumeric(c)))
         pass=true;
      if(c==' ')
        pass=true;
      if(pass) {
         to[stage++]=c;
      }
      if(stage>=toSize)
         return;
   }
   to[stage]='\0';

   return;
}

public OnThinkPost(entity)
{
  for(new target = 1; target <= GetMaxClients(); target++) {

    if(IsClientInGame(target) && ( IsMageInvis(target) || (GetClass(target)==2 && (wcGetTalentLevel(target,1,3,3) > 0 && GetElusive(target))  ) ) )	{  //removed IsInvisible() in respect to rogues nerf
      if(!CSGO) SetEntData(entity, g_minimapoffset + target, false, 1, true);
    }
    if(NoRadar() && !CSGO) {
      SetEntData(entity, g_minimapoffset + target, false, 1, true);
    }
  }
}

stock bool:NoRadar()  {
  return g_noRadar;
}
