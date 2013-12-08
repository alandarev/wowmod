/*
File: checker.inc
Description: Is useless include for mod, but helps debugging
             gives more information on where source is broken on compilation.
Author(s): Stinkyfax
*/


public noCall()	{

  //admincmd's alive?
  SpellIdCmd(0,0);
  //fun alive?
  IsQuizInProgress();

  //spells alive?
  wcRunSpell_6_4(0,0,0,0);

  //vectors alive?
  SpawnAmmo(INVALID_HANDLE, 1);
  wcCalculateStats(1);
  new item[UniqueItem];
  new iTmp;
  new Float:fTmp;
  AddItemBonuses(1, item, iTmp, fTmp);

  wcRecalculateStats(1);
  GetMinusCrit(1);
  wcInstallEffects(1);

  GetBonusAccuracy(1);
  SetClientClipModifier(1, 0.0);

  //configuration alive?
  UpdateStringConfig();

  //sounds alive?

  //mysql alive?
  DeleteQueryById(1);
  GetPlayerData(1);
  CalcArenaForceSQL();
  wcCheckMail();
  CommSendMessage("");

  //menus alive?
  DebugMenu(1);
  OtherPlayerMenu(1,1);
  ProfRecipeMenu(1,1,1);
  MailMenu2(1);
  MenuReadMailDisplay(INVALID_HANDLE, INVALID_HANDLE, "", 1);
  SpellBar(1);
  ChangeCharacterMenu(1);
  ItemsMenu(1,1);
  ItemPurchase(1,1);
  ResetPoints(1);

  //rank alive?
  MaxRank(900);

  //damages
  LowLvlDmg(1,1,iTmp,true);
  GetDmgRed(100,1);

  //hooks
  SayCommand(1,1);
  OnGameFrame();
  Deny(1,"");

  //systems
  SetDoubleHand(1,true);
  IsDmEnabled();

  //effects
  Effect3(1,iTmp,1);
  VisualHP(1);
  TotemToPlayer(1,1);
  Effect35(1,iTmp,1,0);

  //Item functions
  CreateItemsArray();
  new uitem[UniqueItem];
  SetRandomValues(uitem);

  //talents
  SetShadowGateLocation(1,1,1.1);
  getActiveSpec(1);
  GetPoisoned(1);
  wcRunTalent_2_LightFooted(1);


  //natives
  InitNatives();


}
