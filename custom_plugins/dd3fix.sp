/* ========================================================================== */
/*                                                                            */
/*   dd3fix.sp                                                                */
/*   (c) 2011 Stinkyfax                                                       */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#define VERSION "1.0"


#pragma semicolon 1


#include <sourcemod>
#include <sdktools>


public Plugin:myinfo =
{
  name = "dd3fix",
  author = "Stinkyfax",
  description = "Fix of DD3 bugged skybox not downloading from http",
  version = VERSION,
  url = "http://sourcemod.net/"
};

public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
  return APLRes_Success;
}


public OnPluginStart()  {
  AddDls();
}

public OnMapStart()  {
  AddDls();
}

public OnConfigsExecuted() {
}

AddDls()  {
  //Check if its' dd3
  decl String:map[50];
  GetCurrentMap(map, 50);
  if(!StrEqual(map,"de_dust3"))
    return;

  AddFileToDownloadsTable("sound/ambient/music/HansZimmer.wav");
  AddFileToDownloadsTable("sound/ambient/atmosphere/hole_amb3.wav");
  AddFileToDownloadsTable("materials/SignatureAlex2.vtf");
  AddFileToDownloadsTable("materials/SignatureAlex2.vmt");
  AddFileToDownloadsTable("materials/SignatureAlex.vtf");
  AddFileToDownloadsTable("materials/SignatureAlex.vmt");
  AddFileToDownloadsTable("materials/skybox/sky_dust3rt.vmt");
  AddFileToDownloadsTable("materials/skybox/sky_dust3bk.vmt");
  AddFileToDownloadsTable("materials/skybox/sky_dust3dn.vmt");
  AddFileToDownloadsTable("materials/skybox/sky_dust3ft.vmt");
  AddFileToDownloadsTable("materials/skybox/sky_dust3lf.vmt");
  AddFileToDownloadsTable("materials/skybox/sky_dust3up.vmt");
  AddFileToDownloadsTable("materials/skybox/sky_dust3rt.vtf");
  AddFileToDownloadsTable("materials/skybox/sky_dust3bk.vtf");
  AddFileToDownloadsTable("materials/skybox/sky_dust3dn.vtf");
  AddFileToDownloadsTable("materials/skybox/sky_dust3ft.vtf");
  AddFileToDownloadsTable("materials/skybox/sky_dust3lf.vtf");
  AddFileToDownloadsTable("materials/skybox/sky_dust3up.vtf");

}
