/*
* File: bg.inc
* Description: Battle ground support
* Author(s): Stinkyfax
*/

#pragma semicolon 1

enum BG_Style {
  BGS_none=0,
  BGS_standard,
  BGS_wsg
}

#define BGMAPS 1
new BG_Style:g_ff_bgstyles[] = { BGS_wsg };
new String:g_ff_bgmaps[][30] = { "bg_warsonggulch_v1" };
new String:g_ff_bgLoads[BG_Style][30] = { "_bgNone", "_bgLoadDefault", "_bgLoadWSG" };
new String:g_ff_bgUnLoads[BG_Style][30] = { "_bgNone", "_bgUnLoadDefault", "_bgUnLoadWSG" };

new BG_Style:g_BGStyle=BGS_none;

bool:CheckBgStyle()  {
  decl String:sMap[30];
  GetCurrentMap(sMap,sizeof(sMap));
  new count=StrContains(sMap,"bg_",false);
  if (count<0)  {
    SwitchBGStyle(BGS_none);
    return false;
  }
  for(new i=0; i<BGMAPS; i++) {
    if(StrEqual(g_ff_bgmaps[i], sMap))  {
      return SwitchBGStyle(g_ff_bgstyles[i]);
    }
  }
  return SwitchBGStyle(BGS_standard);
}

bool:SwitchBGStyle(BG_Style:style) {
  if((g_BGStyle == style) && (style == BGS_none))
    return false;
  //g_BGStyle = style;
  
  //call the appropriate functions
  
  //Unload old
  //Load
  new Function:func = GetFunctionByName(INVALID_HANDLE, g_ff_bgUnLoads[style]);
  Call_StartFunction(INVALID_HANDLE, func);
  if(Call_Finish() != SP_ERROR_NONE)  {
    decl String:err[100];
    decl String:buffer[100];
    Format(err,100,"Could not callback the function: %s",buffer);
    Logger_log(LogType:LT_file, LogLevel:LL_error, "BG", err);
    return false;
  }
  PrintToServer("switchBgStyle.50");
  
  //Load
  func = GetFunctionByName(INVALID_HANDLE, g_ff_bgLoads[style]);
  Call_StartFunction(INVALID_HANDLE, func);
  new bool:ret;
  if(Call_Finish(ret) != SP_ERROR_NONE)  {
    decl String:err[100];
    decl String:buffer[100];
    Format(err,100,"Could not callback the function: %s",buffer);
    Logger_log(LogType:LT_file, LogLevel:LL_error, "BG", err);
    return false;
  }
  PrintToServer("switchBgStyle.51");
  return ret;
}

public bool:_bgUnload()  {
  PrintToServer("unloading bg");
  if(g_BGStyle == BGS_none)
    return true;
  g_BGStyle = BGS_none;
  return true;
}

public bool:_bgLoadDefault() {
  PrintToServer("loading default bg");
  g_BGStyle = BGS_standard;
  return true;
}

public bool:_bgUnLoadDefault() {
  PrintToServer("loading default bg");
  g_BGStyle = BGS_standard;
  return true;
}

















































