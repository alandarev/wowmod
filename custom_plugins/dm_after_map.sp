#include <sourcemod>

new Handle: g_dmCheck = INVALID_HANDLE;
new bool: g_test;
new Float: g_time;
new Float: g_dmTimeStart;
new Handle: g_dmRoundTimeCheck = INVALID_HANDLE;
new Float: g_dmRoundTime;
new Float: g_dmRoundTimeOld;

public Plugin:myinfo =
{
  name = "DM after Map Change",
  author = "#Stinkyfax & Mr.Malina",
  description = "Re-enables DeathMatch after map change for up to 10 minutes.",
  version = "1.2",
  url = "http://wowmod.eu/"
};

public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
  return APLRes_Success;
}


public OnAllPluginsLoaded()
{
    g_dmCheck = FindConVar("cssdm_enabled");
    g_dmRoundTimeCheck = FindConVar("mp_roundtime");
    if (g_dmCheck != INVALID_HANDLE)
    {
        HookConVarChange(g_dmCheck, DmChangeVar);
    }
}

public DmChangeVar(Handle:cvar, const String:oldVal[], const String:newVal[])
{
    if (StrEqual(oldVal, "0") && StrEqual(newVal, "1"))
    {
        g_dmTimeStart = GetEngineTime();
    }
}

public Action: ChangeVarRoundTime(Handle: timer, any: foo)
{
    g_dmRoundTime = GetConVarFloat(g_dmRoundTimeCheck);
}

public OnMapEnd()
{
    if (g_dmCheck != INVALID_HANDLE)
    {
        g_test = GetConVarBool(g_dmCheck);
        CreateTimer(1.0, ChangeVarRoundTime, 0);
    }
}

public OnConfigsExecuted()
{
    if (g_test == true)
    {
        g_dmRoundTimeOld = GetConVarFloat(g_dmRoundTimeCheck);
        g_time = GetEngineTime() - g_dmTimeStart;
        SetConVarBool(g_dmCheck, true);
        SetConVarFloat(g_dmRoundTimeCheck, g_dmRoundTime);
        CreateTimer(30.0, TimerCmd, 0, TIMER_REPEAT);
    }
}

public Action: TimerCmd(Handle: timer, any: foo)
{
    g_time += 30.0;
    if (g_time >= 620.0 && GetConVarBool(g_dmCheck))
    {
        SetConVarBool(g_dmCheck, false);
        SetConVarFloat(g_dmRoundTimeCheck, g_dmRoundTimeOld);
        return Plugin_Stop;
    }
    return Plugin_Continue;
}
