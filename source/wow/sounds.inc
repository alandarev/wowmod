/*
* File: sounds.inc
* Description: Sound functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1

PlayLevelupSound(client)
{
  decl String:soundpath[128];
  KvRewind(config);
  KvJumpToKey(config,"sounds");
  KvGetString(config,"levelup",soundpath,128);
  if(!StrEqual(soundpath,""))
    ClientCommand(client,"play %s",soundpath);
}