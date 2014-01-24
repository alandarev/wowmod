/*
* File: messages.inc
* Description: Message functions
* Author(s): Stinkyfax
*/

#pragma semicolon 1

// Colors
#define COLOR_DEFAULT 0x01
#define COLOR_LIGHTGREEN 0x03
#define COLOR_GREEN 0x04 // DOD = Red

public LoadTranslationFile()
{
  LoadTranslations("wow.phrases");
  LoadTranslations("core.phrases");
  LoadTranslations("common.phrases");
}
/** Coloring guide for messages printed to the chat box
 * @default    = Everything else
 * @lightgreen = Player names & Numbers
 * @green      = Skill/Spell/Talent names
 */
public WCMessage(target,any:...)
{
  return;
  if(IsClientInGame(target) || (target==0))
  {
    decl String:translation[350],String:buffer[350];
    VFormat(buffer,sizeof(buffer),"%T",2);
    Format(translation,sizeof(translation),"%c[WC] %c%s",COLOR_GREEN,COLOR_DEFAULT,buffer);
    ReplaceString(translation,350,"@default","\x01");
    ReplaceString(translation,350,"@lightgreen","\x03");
    ReplaceString(translation,350,"@green","\x04");
    ReplaceString(translation,sizeof(translation),"@perc","%%");
    PrintToChat(target,translation);
  }
}

public PlainMessage(client, String:m[])   {
   if(IsClientInGame(client) || (client==0))
   {
      decl String:msg[500];
      Format(msg,sizeof(msg),"%c[WC] %c%s",COLOR_GREEN,COLOR_DEFAULT,m);
      ReplaceString(msg,sizeof(msg),"@default","\x01");
      ReplaceString(msg,sizeof(msg),"@lightgreen","\x03");
      ReplaceString(msg,sizeof(msg),"@green","\x04");
      ReplaceString(msg,sizeof(msg),"@sp"," ");
      ReplaceString(msg,sizeof(msg),"@perc","%%");
      PrintToChat(client,msg);
   }
}

new g_ffDropId[65];
stock AnnounceDrop(client, String:name[], slot=-1) {
   if(!IsClientInGame(client))
      return;
   g_ffDropId[client] = slot;
   new Handle:panel = CreatePanel();
   decl String:temp[255];
   Format(temp, sizeof(temp), "%T", "panel an drop title", client);
   SetPanelTitle(panel, temp);
   Format(temp, sizeof(temp), "%T", "panel an drop item", client,name);
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   DrawPanelText(panel, temp);/*
   Format(temp,sizeof(temp),"  %s",name);
   DrawPanelItem(panel, name, ITEMDRAW_DISABLED);*/
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   SetPanelCurrentKey(panel, 2);
   Format(temp, sizeof(temp), "%T", "press to open drop", client);
   DrawPanelItem(panel, temp);
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   SetPanelCurrentKey(panel, 3);
   Format(temp, sizeof(temp), "%T", "exit", client);
   DrawPanelItem(panel, temp, ITEMDRAW_CONTROL);
   SendPanelToClient(panel, client, AnnounceDropHandler, 30);

   CloseHandle(panel);
}

stock getClientDropId(client) {
  return g_ffDropId[client];
}

AnnounceDropAll(client, String:iname[], rarity)  {
  decl String:name[30];
  GetClientName(client, name, 30);
  decl String:msg[200];
  for(new i=1; i <= GetMaxClients(); i++)
  {
    if(!IsClientInGame(i))
      continue;

    Format(msg, sizeof(msg), "%T", "panel global an drop item", i, name, iname);
    switch(rarity)
    {
      case 0: panelMessage(i, "Common", msg);
      case 1: panelMessage(i, "Uncommon", msg);
      case 2: panelMessage(i, "Rarez!", msg);
      case 3: panelMessage(i, "Epicz!!", msg);
      case 4: panelMessage(i, "Legendary!!", msg);
      case 5: panelMessage(i, "PVP - Rarez!!", msg);
      case 6: panelMessage(i, "PVP - Epicz!!", msg);
      default:panelMessage(i, "Item!!",msg);
    }
  }
}

/*
AnnounceDropAll(client, String:iname[])  {
  decl String:name[30];
  GetClientName(client, name, 30);
  decl String:msg[200];
  for(new i=1; i <= GetMaxClients(); i++)
    if(IsClientInGame(i)) {
      Format(msg, sizeof(msg), "%T", "panel global an drop item", i, name, iname);
      panelMessage(i, "Epicz!!", msg);
    }
} */

stock panelMessage(client, String:title[], String:msg[], seconds=0) {
   if(!IsClientInGame(client))
      return;
   new Handle:panel = CreatePanel();

   SetPanelTitle(panel, title);
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   DrawPanelText(panel, msg);
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   decl String:temp[50];
   Format(temp, sizeof(temp), "%T", "exit", client);
   SetPanelCurrentKey(panel, 2);
   DrawPanelItem(panel, temp, ITEMDRAW_CONTROL);
   SendPanelToClient(panel, client, AnnounceHandler, seconds);

   CloseHandle(panel);
}

stageMessage(client)  {
  new String:progress[30];
  new max = 15;
  new stages = 7 -1;
  new stage = getLoadStage(client);
  decl String:trans[40];
  translateLoadStage(stage, trans, 40);
  if(stage == 10)
    stage=7;
  stage-=1;
  new points = RoundToNearest( float(max) / stages * stage);
  progress[0]='|';
  for(new i=1 ; i<=points; i++)
    progress[i]='=';
  progress[points+1]='>';
  for(new i = points+2; i < max+2; i++)
    progress[i] = '_';
  progress[max+2]='|';
  progress[max+3]='\0';
  decl String: stageMsg[200];
  Format(stageMsg, sizeof(stageMsg), "%T", trans, client);
  decl String:msg[200];
  Format(msg, sizeof(msg), "%T", "Progress", client, stageMsg, progress);
  decl String:name[25];
  Format(name, sizeof(name), "%T", "Loading...", client);
  panelMessage(client, name, msg, 3);
}

public AnnounceDropHandler(Handle:menu, MenuAction:action, client, slot)   {
  if(action == MenuAction_Select) {
    if(slot == 2) {
      new id = getClientDropId(client);
      if(id >= 0) InvItemMenu(client,id);
    }
  }
}

public AnnounceHandler(Handle:menu, MenuAction:action, client, slot)   {
}


public HudHint(target,any:...)
{
  if(target)
  {
    decl String:buffer[192];
    VFormat(buffer,sizeof(buffer),"%T",2);
    new Handle:hBf;
    hBf=StartMessageOne("HintText",target);
    if(hBf!=INVALID_HANDLE)
    {
      if (GetUserMessageType() == UM_Protobuf)
      {
        PbSetString(hBf, "text", buffer);
      }
      else
      {
        //BfWriteByte(hBf,-1);
        BfWriteString(hBf,buffer);
      }

      EndMessage();
    }
  }
}

public HudHintStatic(target,String:message[])
{
  if(target)
  {
    new Handle:hBf;
    hBf=StartMessageOne("HintText",target);
    if(hBf!=INVALID_HANDLE)
    {
      if (GetUserMessageType() == UM_Protobuf)
      {
        PbSetString(hBf, "text", message);
      }
      else
      {
        //BfWriteByte(hBf,-1);
        BfWriteString(hBf,message);
      }

      EndMessage();
    }
  }
}

public wcUpdateNews()
{
  CreateTimer(float(420), UpdateNews, 0, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
}

public Action:UpdateNews(Handle:timer, any:nothing)
{
  KvRewind(wcnews);
  KvJumpToKey(wcnews, "news");
  new max=KvGetNum(wcnews,"amount");
  new rand=GetRandomInt(1,max);
  new String:Srand[3];
  IntToString(rand,Srand,sizeof(Srand));
  decl String:text[300];
  decl String:title[30];
  decl String:line[50];
  for(new i=1 ; i<=GetMaxClients(); i++)  {
    if (IsClientInGame(i))  {
      Format(title, sizeof(title), "%T", "Wc Tips", i);
      Format(line, sizeof(line), "Wc Tips Msg %s", Srand);
      Format(text,sizeof(text), "%T", line, i);
      Format(text,sizeof(text), "%c%s: %c%s", COLOR_GREEN, title, COLOR_DEFAULT, text);
      //ReplaceString(text,sizeof(text),"@'","\"");
      //ReplaceString(text,sizeof(text),"@p","%");
      ReplaceString(text,sizeof(text),"@default","\x01");
      ReplaceString(text,sizeof(text),"@lightgreen","\x03");
      ReplaceString(text,sizeof(text),"@green","\x04");
      ReplaceString(text,sizeof(text),"@perc","%%");
      PrintToChat(i,text);
    }
  }
  //KvGetString(wcnews, Srand, text, sizeof(text), "error");
  //Format(text,sizeof(text),"%c[WC Tips]: %c%s",COLOR_GREEN,COLOR_DEFAULT,text);
  //ReplaceString(text,sizeof(text),"@'","\"");
  //ReplaceString(text,sizeof(text),"@p","%");
  //PrintToChatAll(text);
  return Plugin_Continue;
}

public Action:ManaTimer(Handle:timer, any:temp)
{
  static counter=0;
  counter++;
  new bool:boss=IsBoss() || IsHNS();
  new bool:isDM=IsDmEnabled();
  new bosses[65],total;
  if(boss)
  {
    total=GetBosses(bosses);
  }
  for(new x=1;x<=GetMaxClients();x++)
  {
    if(IsClientConnected(x)) if(IsClientInGame(x))
    {
      decl String:buffer[255];
      if(IsPlayerAlive(x))
      {
        new mana=GetMana(x);
        new maxmana=GetMaxMana(x);
        new class = GetClass(x);

        if(GetCombat(x)>0)
         SetCombat(x,GetCombat(x)-1);

        if(class==2 || class == 9)
        {
          if(GetCombat(x) > 0)
          {
            decaySpar(x);
            if(!IsPlayerAlive(x))
              if(g_iCombatReady[x]>=1)
                RemoveReadiness(x);
          }
          else
          {
            resetSpar(x);
            if(g_iCombatReady[x]>=1)
              RemoveReadiness(x);
          }
          
          if(GetMonkStance(x)==2)
            mana+=1;
          mana+=5;
          if(mana>maxmana)
            mana=maxmana;
          wcSetMana(x,mana);
          if(CheckRequirements(x,2,1,2,1))
            wcRunTalent_2_LightFooted(x); //still used to refresh the lowgrav for rogues
        }
        else if(counter==10)
        {
          if(class==1 || class==8)
          {
            new bool:rageregen=false;
            new minmana=0;
            if(class==1 && CheckRequirements(x,1,3,2,4))
            {
              decl String:talent[255];
              wcGetTalentInfo(1,3,2,4,x,"limit",talent);
              minmana=StringToInt(talent);
              rageregen=true;
            }

            if(GetCombat(x)<=0)
            {
              mana -= 5;
              if(mana<minmana)
              {
                mana += 5;
                if(mana > minmana)
                  mana = minmana;
              }
              if(mana<0) // Just in case logics with minmana fail
                mana=0;
            }
            if(rageregen)
            {
              if(mana < minmana)
              {
                mana+=2;
                if(mana > minmana)
                {
                  mana = minmana;
                }
              }
            }
            wcSetMana(x,mana);

          }
          else
          {
            mana+=GetManaReg(x);
            if(isDM)
            mana+=GetManaReg(x);
            if(GetClass(x) == 6 && HasGlyph(x, GLYPH_SHAMAN_WATER) && CheckRequirements(x,6,3,2,4) )
            {
              mana += RoundToNearest(0.01 * maxmana);
            }
            if(GetClass(x) == 4 && HasGlyph(x, GLYPH_PALADIN_SPIRITUAL))
              mana += RoundToNearest(float(GetMaxMana(x))*0.01);
            if (mana>maxmana)
              mana=maxmana;

            wcSetMana(x,mana);
          }
        }
        if(class==1 && GetCombat(x)<=0)
          if(CheckRequirements(x,1,1,3,3) && (GetClass(x) == 1))
            wcRunTalent_1_RapidHealing(x);
        new hp=wcGetHealth(x);
        new maxhp=GetMaxHealth(x);
        if(hp>maxhp)
        {
          hp=maxhp;
          wcSetHealth(x,hp);
        }
        decl String:restype[10];
        GetManaName(x, restype);
        if(!boss || IsHNS())
          Format(buffer,sizeof(buffer),"%T","hp mp dmg taken",x,hp,maxhp,mana,maxmana,wcDamageDone[x],wcDamageTaken[x],restype);
        else
          Format(buffer,sizeof(buffer),"%T","hp mp dmg threat",x,hp,maxhp,mana,maxmana,wcDamageDone[x],GetThreat(x),restype);

        if(wcHealingDone[x]>0)
        {
          Format(buffer,sizeof(buffer),"%s%T",buffer,"hint healing done",x,wcHealingDone[x]);
        }
        if(GetClass(x)==4)
          Format(buffer,sizeof(buffer),"%s%T",buffer,"holy power hint",x,GetHolyPower(x));
        if(GetClass(x)==8)
          Format(buffer,sizeof(buffer),"%s%T",buffer,"runes hint",x,GetRunes(x));
        if(GetClass(x)==9)
          Format(buffer,sizeof(buffer),"%s%T",buffer,"chi hint",x,getChi(x));
        if(HasStealth(x))
        {
          new alpha=ModifyStealth(x);
          new perc= RoundToNearest( (1.0-float(alpha)/255.0)*100.0 );
          Format(buffer,sizeof(buffer),"%s%T",buffer,"hint stealth",x,perc);
        }
        else
        {
          g_alpha[x]=255;
        }
        if(GetEnrage(x) > 0)
          Format(buffer,sizeof(buffer),"%s%T",buffer,"enrage hint",x,GetEnrage(x));
        if(GetFlurry(x) > 0)
          Format(buffer,sizeof(buffer),"%s%T",buffer,"sflurry hint",x,GetFlurry(x));
        if(GetShadowform(x))
          Format(buffer,sizeof(buffer),"%s%T",buffer,"shadowform hint",x);
      }
      else
      {
        Format(buffer,sizeof(buffer),"%T", "dead", x);
        Format(buffer,sizeof(buffer),"%s%T", buffer, "dmg taken", x, wcDamageDone[x], wcDamageTaken[x]);
        if(wcHealingDone[x]>0)
        {
           Format(buffer,sizeof(buffer),"%s%T",buffer,"hint healing done",x,wcHealingDone[x]);
        }
      }

      if(boss)    for(new i=0;i<total;i++)
      {
        decl hp;
        decl String:name[25];
        GetClientName(bosses[i],name,sizeof(name));
        if(IsPlayerAlive(bosses[i]))
          hp=wcGetHealth(bosses[i]);
        else
          hp=0;
        new maxhp=GetMaxHealth(bosses[i]);
        Format(buffer,sizeof(buffer),"%s%T",buffer,"boss hp",x,hp,maxhp,name);
      }
      if((g_fCast[x] > GetEngineTime()) && (g_iCast[x] > 0) )
      {
        new Handle:spells = GetSpellsHandle(GetClass(x));
        if(spells != INVALID_HANDLE)
        {
          KvRewind(spells);
          decl String:s_spellid[5];
          IntToString(g_iCast[x],s_spellid,sizeof(s_spellid));
          KvJumpToKey(spells,s_spellid);
          decl String:name[255];
          KvGetString(spells, "name", name, sizeof(name), "0");
          decl String:title[100];
          Format(title,sizeof(title),"%T","hud spell cast",x,name,g_fCast[x] - GetEngineTime());
          Format(buffer,sizeof(buffer),"%s\n%s",buffer,title);
        }
      }
      if(GetSealT(x)>GetTickedTime())  if(HasSeal(x,Seal:SEAL_Any))
      {
        decl String:sealName[150];
        //Format(sealName, sizeof(sealName), "seal %i", GetSeal(x));
        //Format(sealName, sizeof(sealName), "%T", sealName, x);
        Format(sealName, sizeof(sealName), "Seal (%0.2f sec)", GetSealT(x) - GetTickedTime());
        Format(buffer,sizeof(buffer),"%s\n%s",buffer,sealName);
      }
      if(wcManaShield[x]>0 && HasEffectVictim(x, 11))
      {
        decl String:sealName[150];
        Format(sealName, sizeof(sealName), "%T", "mana shield", x, wcManaShield[x]);
        Format(buffer,sizeof(buffer),"%s\n%s",buffer,sealName);
      }

      if(g_fQuizTime>GetEngineTime())
      {
        Format(buffer,sizeof(buffer),"%s%T",buffer,"Q time",x,g_fQuizTime-GetEngineTime());
      }

      new String:cds[200];
      wcGetCooldownString(x, cds);
      if(strlen(cds)>1)
        Format(buffer, sizeof(buffer),"%s%s", buffer, cds);
      if(IsPlayerAlive(x) || wcDamageDone[x] > 0 || wcDamageTaken[x] > 0 || wcHealingDone[x] > 0)
        HudHintStatic(x,buffer);
    }
  }
  if(counter==10)
    counter=0;
  return Plugin_Continue;
}

new String:aMsg[5][400],String:vMsg[5][400];
new bool:affMsg[5]=false;
new bool:aMsgEx[5],bool:vMsgEx[5];
new aMsgClient[5],vMsgClient[5];
new EMsgId=0;

stock StartEMsg(String:amsg[], String:vmsg[], attacker, victim) {
   EMsgId++;
   if(EMsgId>=5)
      EMsgId=0;
   aMsgEx=true;
   vMsgEx=true;
   aMsgClient[EMsgId]=attacker;
   vMsgClient[EMsgId]=victim;
   Format(aMsg[EMsgId], 400, "%s", amsg);
   Format(vMsg[EMsgId], 400, "%s", vmsg);
   return EMsgId;
}

stock getMsgAttacker()  {
  return aMsgClient[EMsgId];
}
stock getMsgVictim()  {
  return vMsgClient[EMsgId];
}

stock MsgExist(id, bool:a=true, bool:v=true)  {
   aMsgEx[id]=a;
   vMsgEx[id]=v;
}

stock AddEMsgCaster(id, String:caster[]="") {
   decl String:sCaster[20];
   Format(sCaster,sizeof(sCaster),"%s",caster);
   decl String:buffer[50];
   Format(buffer,sizeof(buffer),"@green%s@default",sCaster);
   ReplaceString(aMsg[id],400,"@att",buffer);
   ReplaceString(vMsg[id],400,"@att",buffer);
}

stock AddEMsgDetails(id, String:victim[]="", dmg=0)   {
   decl String:sVictim[20];
   Format(sVictim,sizeof(sVictim),"%s",victim);
   decl String:buffer[50];

   Format(buffer,sizeof(buffer),"@green%s@default",sVictim);
   ReplaceString(aMsg[id],400,"@vic",buffer);
   ReplaceString(vMsg[id],400,"@vic",buffer);

   Format(buffer,sizeof(buffer),"@lightgreen%i@default",dmg);
   ReplaceString(vMsg[id],400,"@dmg",buffer);
   ReplaceString(aMsg[id],400,"@dmg",buffer);
}

stock AddEMsgAff( String:aff[], bool:translate)   {
   if(!affMsg[EMsgId]) {
      Format(aMsg[EMsgId],400, "%s (%T: @lightgreen",aMsg[EMsgId], "affected by", getMsgAttacker());
      Format(vMsg[EMsgId],400, "%s (%T: @lightgreen",vMsg[EMsgId], "affected by", getMsgVictim());
   }
   else  {
      Format(aMsg[EMsgId],400, "%s, ",aMsg[EMsgId]);
      Format(vMsg[EMsgId],400, "%s, ",vMsg[EMsgId]);
   }

   affMsg[EMsgId]=true;
   MsgExist(EMsgId);
   if(!translate) {
     Format(aMsg[EMsgId],400, "%s%s", aMsg[EMsgId], aff);
     Format(vMsg[EMsgId],400, "%s%s", vMsg[EMsgId], aff);
   }
   else {
     Format(aMsg[EMsgId],400, "%s%T", aMsg[EMsgId], aff, getMsgAttacker());
     Format(vMsg[EMsgId],400, "%s%T", vMsg[EMsgId], aff, getMsgVictim());

   }
    //4rdmg=RoundToNearest((armor / (armor + 400.0 + 85.0*(float(GetLevel(id)) + 4.5*(float(GetLevel(id)) - 59.0))))*100.0);
}

stock AddEMsgAffAttOnly( String:aff[], bool:translate) //attacker only messages through here
{
  if(!affMsg[EMsgId])
  {
    Format(aMsg[EMsgId],400, "%s (%T: @lightgreen",aMsg[EMsgId], "affected by", getMsgAttacker());
  }
  else
  {
    Format(aMsg[EMsgId],400, "%s, ",aMsg[EMsgId]);
  }

  affMsg[EMsgId]=true;
  MsgExist(EMsgId,true,false);
  if(!translate)
  {
    Format(aMsg[EMsgId],400, "%s%s", aMsg[EMsgId], aff);
  }
  else
  {
    Format(aMsg[EMsgId],400, "%s%T", aMsg[EMsgId], aff, getMsgAttacker());
  }
}

stock AddEMsgAffVicOnly( String:aff[], bool:translate) //victim only messages through here
{
  if(!affMsg[EMsgId])
  {
    Format(vMsg[EMsgId],400, "%s (%T: @lightgreen",vMsg[EMsgId], "affected by", getMsgVictim());
  }
  else
  {
    Format(vMsg[EMsgId],400, "%s, ",vMsg[EMsgId]);
  }

  affMsg[EMsgId]=true;
  MsgExist(EMsgId,false,true);
  if(!translate)
  {
    Format(vMsg[EMsgId],400, "%s%s", vMsg[EMsgId], aff);
  }
  else
  {
    Format(vMsg[EMsgId],400, "%s%T", vMsg[EMsgId], aff, getMsgVictim());
  }
}

stock CloseEMsgAff(id) {
   if(affMsg[id])  {
      Format(aMsg[id],400, "%s@default)",aMsg[id]);
      Format(vMsg[id],400, "%s@default)",vMsg[id]);
   }
}

stock CastEMsg(id, at, vic)   {
   CloseEMsgAff(id);
   if(aMsgEx[id]) {
      if(IsChat(at, Chat:Chat_Attacker))
        PlainMessage(at, aMsg[id]);
   }
   if(vMsgEx[id]) {
      if(IsChat(vic, Chat:Chat_Victim))
        PlainMessage(vic, vMsg[id]);
   }
   EndEMsg(id);
}

stock EndEMsg(id)   {
   aMsg[id]="";
   vMsg[id]="";
   affMsg[id]=false;
   EMsgId--;
   if(EMsgId<0)
      EMsgId=4;
}

new g_iChatSettings[65][Chat];

stock SetChat(client, Chat:type, value) {
  g_iChatSettings[client][type]=value;
}

stock GetChat(client, Chat:type) {
  return g_iChatSettings[client][type];
}

stock bool:TriggerChat(client, Chat:type)  {
  new value = (GetChat(client, type) + 1) % 2;
  SetChat(client, type, value);
  return IsChat(client, type);

}

stock bool:IsChat(client, Chat:type)  {
  new value = GetChat(client, type);
  if(value==0)
    return false;
  return true;
}

wcGetCooldownString(client, String:cds[200])  {
  Format(cds, 200, "");
  for(new type = 0; type < 20; type++)  {
    new Float:timeleft = GetCDSpell(client, type) - GetTickedTime();
    if(timeleft > 0)  {
      decl String:name[30];
      GetCDSpellName(client, type, name, 30);
      Format(cds, 200, "%s\n%s: %.1f", cds, name, timeleft);
    }
  }
}

dingPanel(client) {
   if(!IsClientInGame(client))
      return;
   new Handle:panel = CreatePanel();
   decl String:temp[255];
   Format(temp, sizeof(temp), "%T", "panel ding title", client, GetLevel(client));
   SetPanelTitle(panel, temp);

   Format(temp, sizeof(temp), "%T", "panel ding header", client);
   DrawPanelText(panel, temp);
   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);

   //TODO add auto stat selection
   //if(AutoSpreadPoints(

   Format(temp, sizeof(temp), "%T", "panel levelup points", client,GetMoney(client),
    getTPoints(client, getActiveSpec(client)));
   DrawPanelText(panel, temp);

   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);

   new race = GetClass(client);

   if(race>0) {

     new String:spellName[60];
     new bool:firstLearn=true;
     new spellid=0;
     while(  (spellid= learnSpell(client, spellName)) > 0 )  {
        if(firstLearn)  {

          Format(temp, sizeof(temp), "%T", "panel learned new spells", client);
          DrawPanelText(panel, temp);

          firstLearn=false;
        }
        if ( wcGetSpellInfoInt(race, spellid, "active") && (!wcGetSpellInfoInt(race, spellid, "autocast") ) ) {
          Format(spellName, sizeof(spellName), "%s (%T)", spellName, "requires casting", client);
          //Add to spellbar
          addToSpellbar(client, spellid);
        }
        DrawPanelText(panel, spellName);

     }

   }

   DrawPanelItem(panel, " \n", ITEMDRAW_RAWLINE|ITEMDRAW_SPACER);
   Format(temp, sizeof(temp), "%T", "panel ding footer", client);
   DrawPanelText(panel, temp);


   Format(temp, sizeof(temp), "%T", "exit", client);
   SetPanelCurrentKey(panel, 2);
   DrawPanelItem(panel, temp, ITEMDRAW_CONTROL);
   SendPanelToClient(panel, client, DingPanelHandle, 0);

   CloseHandle(panel);
}

public DingPanelHandle(Handle:menu, MenuAction:action, client, slot)   {
}

public Action:DisplayTodoMenu(Handle:timer, any:client)   {
  if(IsClientInGame(client) && GetNewbie2(client) && (!IsPlayerAlive(client)) )
    displayTodoMenu(client);
}

new g_ff_todo_priority[65];
stock resetTodoPriority(client) {
  g_ff_todo_priority[client]=0;
}

_getTodoPriority(client)  {
  return g_ff_todo_priority[client];
}

_incrTodoPriority(client) {
  return ++g_ff_todo_priority[client];
}

new bool:g_ff_todo_force[65];
stock displayTodoMenu(client, bool:force=false) {
  if(force)
    SetMenuOff(client, 2.0);
  g_ff_todo_force[client]=force;
  prepareItemsQuery(client);
}

stock _displayTodoMenu(client, Handle:query)
{
  new bool:pass=g_ff_todo_force[client];
  new bool:force = g_ff_todo_force[client];
  g_ff_todo_force[client]=false;

  if(playerloaded[client]!=1) //wait until loads
    return 0;

  decl String:slots[20][2][200];
  new bSlots[20];
  new islots=0;

  /**** Generate slots ****/

  //ask for class?
  if(GetClass(client)<=0)
  {
    //Priority check
    if(_getTodoPriority(client) % 1 == 0) //Means always unless auto-todo is disabled
      pass=true;

    new cur = islots++; //Current

    bSlots[cur] = true;
    Format(slots[cur][0], 200, "wcClassChoose");
    Format(slots[cur][1], 200, "%T", "wc todo class", client);
  }

  if(GetClass(client)>0)
  { //Other todo's should be disabled if no class

    //Talent points feature
    new tpoints = getTPoints(client, getActiveSpec(client));
    if(tpoints>0)
    {
      //Priority check
      if(_getTodoPriority(client) % 5 == 0) //Means always unless auto-todo is disabled
        pass=true;
      new cur = islots++; //Current

      bSlots[cur] = true;
      Format(slots[cur][0], 200, "TalentsMenu");
      Format(slots[cur][1], 200, "%T", "wc todo talents", client, tpoints);
    }

    //buyable shop items!
    new limit=4;
    while(SQL_FetchRow(query))
    {
      if(limit <=0 )
        break;

      new cat = GetSQLDataInt(query, "cat");
      new itemid = GetSQLDataInt(query,"itemid");

      if( getCatType(cat) == CT_Wearable )
      { //Check if we wear better then continue
        new itemid2 = GetCharItemId(client, cat);
        if(itemid2>0) {
          new uitemOld[UniqueItem];
          GetItemProperties(uitemOld, itemid2);

          if(GetItemRating(uitemOld) >= GetSQLDataFloat(query, "rating") ) //we have better
            continue;

          if(uitemOld[Item_level] >= GetSQLDataInt(query, "level") || uitemOld[Item_ilevel] >= GetSQLDataInt(query, "ilevel")) //we own higher level
            continue;

          if(uitemOld[Item_category] != cat)  //apparantly it's weapon slot and we don't suggest shield to if he has weapon and other way round
            continue;

          //Now check if it's 2h, we won't suggest shields
          if(cat == 18)
          {
            new mainhandId = GetCharItemId(client, 22);
            if(mainhandId > 0)
            {
              new mainhand[UniqueItem];
              GetItemProperties(mainhand, mainhandId);
              if(mainhand[Item_2hand])
                continue;
            }
          }
        }
      }
      else if (getCatType(cat) == CT_Recipe)
      {
        //Check if it's not other class
        new uitem[UniqueItem];
        new icreate = getItemCreates(itemid);
        GetItemProperties(uitem, icreate);
        new bitflags = uitem[Item_class];
        new suits=false;
        if( bitflags & (1 << 0) == (1 << 0) )
          suits=true;
        else if(bitflags & (1 << GetClass(client)) == (1 << GetClass(client)) )
          suits=true;
        if(!suits)
          continue;
      }

      new cur = islots++; //Current

      bSlots[cur] = true;
      Format(slots[cur][0], 200, "%d", itemid);
      decl String:itemName[50];
      GetSQLDataStr(query, "name", itemName, 50);
      Format(slots[cur][1], 200, "%T", "wc todo shopitem", client, itemName);

      if(_getTodoPriority(client) % 5 == 0) //Means always unless auto-todo is disabled
        pass=true;
      limit--;

    }

    //Profession chooser
    new prof1 = GetProfession(client, 1);
    new prof2 = GetProfession(client, 2);
    if(prof1==0 || prof2==0)
    {
      //Priority check
      if(_getTodoPriority(client) % 5 == 0) //Means always unless auto-todo is disabled
        pass=true;

      new cur = islots++; //Current

      bSlots[cur] = true;
      Format(slots[cur][0], 200, "ProfessionsMenu");
      Format(slots[cur][1], 200, "%T", "wc todo profession", client);
    }


    //Ask for new character
    new bool:charHas=false;
    for(new i=2; i<=15; i++)
    {
      if(characterids[client][i]>0)
      {
        charHas=true;
      }
    }
    if((!charHas) && (GetLevel(client)>60) )
    {
      //Priority check
      if(_getTodoPriority(client) % 5 == 0) //Means always unless auto-todo is disabled
        pass=true;

      new cur = islots++; //Current

      bSlots[cur] = true;
      Format(slots[cur][0], 200, "ChangeCharacterMenu");
      Format(slots[cur][1], 200, "%T", "wc todo new character", client);
    }

    //TODO add more useful features
  }

  /**** Process Slots ****/
  if(islots==0)
  {
    if(force)
      WCMessage(client,"wc todo empty",client);
    else
      return 0;
  }


  _incrTodoPriority(client);
  if(!pass)
    return 0;

  new Handle:menu=CreateMenu(TodoMenuHandle);
  decl String:title[250];
  Format(title,sizeof(title),"%T","wc todo menu",client);
  SetMenuTitle(menu,title);

  for(new i=0; i<islots; i++)
  {
    if(!bSlots[i])
      AddMenuItem(menu,slots[i][0],slots[i][1],ITEMDRAW_DISABLED);
    else
      AddMenuItem(menu,slots[i][0],slots[i][1]);
  }

  //Add footer
  Format(title, sizeof(title), "%T", "wc todo footer", client);
  AddMenuItem(menu, "", title, ITEMDRAW_DISABLED);

  DisplayMenu(menu,client,MENU_TIME_FOREVER);

  return 1;
}

public TodoMenuHandle(Handle:menu,MenuAction:action,client,slot)
{
  if(action==MenuAction_Select)
  {
    decl String:buffer[100];
    GetMenuItem(menu,slot,buffer,sizeof(buffer));
    //Check if it's suggested item
    new itemId = StringToInt(buffer);
    if(itemId == 0) {

      //Call function
      new Function:func = GetFunctionByName(INVALID_HANDLE, buffer);
      Call_StartFunction(INVALID_HANDLE, func);
      Call_PushCell(client);
      if(Call_Finish() != SP_ERROR_NONE)  {
        decl String:err[100];
        Format(err,100,"Could not callback the function: %s",buffer);
        Logger_log(LogType:LT_file, LogLevel:LL_error, "Messages", err);
      }
    }
    else  {
      //Open shop menu
      g_iLastCategory[client]=GetItemCat(itemId, 0);
      _enableAutoEquip(client);
      ItemMenu(client,itemId);
    }


  }
  if(action==MenuAction_End)
    CloseHandle(menu);
}

stock Client_HideRadar(client)
{
  SetEntProp(client, Prop_Send, "m_iHideHUD", 1<<2 );
}

stock Client_ShowRadar(client)
{
  SetEntProp(client, Prop_Send, "m_iHideHUD", 0);
}

DisplaySlides(client, Slide:type) {
  if(client && type)  {}
  return;

  /*g_ffSlideType[client] = type;*/
  /*g_ffSlideStep[client] = 1;*/

  /*ChangeClientTeam(client, 1);*/
  /*SlidesControl(client);*/

  /*CreateTimer(0.1, TimerKeepSlideUp, client, TIMER_REPEAT);*/
}

stock bool:IsFirstSlide(client)  {
  return (g_ffSlideStep[client] == 1);
}

stock SlideHooked(client) {
  g_ffSlideStep[client]++;
  g_ffSlideHook[client] = 0;
  SlidesControl(client);
}

stock TurnOffSlides(client) {
  g_ffSlideStep[client]=0;
  SetSawSlides(client, Slide:SLIDE_NONE);
}

stock IsRussian(client) {
  new lang = GetClientLanguage(client);
  decl String:sLang[5];
  GetLanguageInfo(lang, sLang, 5);
  return StrEqual(sLang,"ru",false);
}

stock SlidesControl(client) {
  //Controller. Init Paths, Init Hooks

  if(!IsRussian(client)) {
    switch(g_ffSlideType[client])  {
      case SLIDE_WELCOME: {
        switch(g_ffSlideStep[client]) {
          case 1: {
            g_ffSlidePath[client] =  "overlays/wow/intro1.vmt";
            g_ffSlideHook[client] =  SLIDE_HOOK_E;

          }
          case 2: {
            g_ffSlidePath[client] =  "overlays/wow/initiation1A.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;

          }
          case 3: {
            g_ffSlidePath[client] = "overlays/wow/initiation1B.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_PROFS;
          }
          case 4: {
            g_ffSlidePath[client] = "overlays/wow/initiation1C.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;
          }
          default: {
            g_ffSlideStep[client] = 0;
            SetSawSlides(client, g_ffSlideType[client]);
          }

        }

      }
      case SLIDE_30:  {
        switch(g_ffSlideStep[client]) {
          case 1: {
            g_ffSlidePath[client] =  "overlays/wow/intro2.vmt";
            g_ffSlideHook[client] =  SLIDE_HOOK_E;

          }
          case 2: {
            g_ffSlidePath[client] =  "overlays/wow/initiation2A.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;

          }
          case 3: {
            g_ffSlidePath[client] = "overlays/wow/initiation2B.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;
          }
          case 4: {
            g_ffSlidePath[client] = "overlays/wow/initiation2C.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;
          }
          default: {
            g_ffSlideStep[client] = 0;
            SetSawSlides(client, g_ffSlideType[client]);
          }
        }

      }
    }
  }
  //Russian slides
  else  {
    switch(g_ffSlideType[client])  {
      case SLIDE_WELCOME: {
        switch(g_ffSlideStep[client]) {
          case 1: {
            g_ffSlidePath[client] =  "overlays/wow/intro1_ru.vmt";
            g_ffSlideHook[client] =  SLIDE_HOOK_E;

          }
          case 2: {
            g_ffSlidePath[client] =  "overlays/wow/initiation1A_ru.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;

          }
          case 3: {
            g_ffSlidePath[client] = "overlays/wow/initiation1B_ru.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_PROFS;
          }
          case 4: {
            g_ffSlidePath[client] = "overlays/wow/initiation1C_ru.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;
          }
          case 5: {
            g_ffSlideStep[client] = 0;
            SetSawSlides(client, g_ffSlideType[client]);
          }

        }

      }
      case SLIDE_30:  {
        switch(g_ffSlideStep[client]) {
          case 1: {
            g_ffSlidePath[client] =  "overlays/wow/intro2_ru.vmt";
            g_ffSlideHook[client] =  SLIDE_HOOK_E;

          }
          case 2: {
            g_ffSlidePath[client] =  "overlays/wow/initiation2A_ru.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;

          }
          case 3: {
            g_ffSlidePath[client] = "overlays/wow/initiation2B_ru.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;
          }
          case 4: {
            g_ffSlidePath[client] = "overlays/wow/initiation2C_ru.vmt";
            g_ffSlideHook[client] = SLIDE_HOOK_E;
          }
          case 5: {
            g_ffSlideStep[client] = 0;
            SetSawSlides(client, g_ffSlideType[client]);
          }
          default:
          g_ffSlideStep[client] = 0;
        }

      }
    }
  }

}

stock _setSawSlides(client, value) {
  g_SlideBits[client] = value;
}

stock getSawSlides(client)  {
  return g_SlideBits[client];
}

stock bool:SawSlides(client, Slide:type)  {
  return (( 1 << _:type) & g_SlideBits[client] ) > 0;
}

stock SetSawSlides(client, Slide:type) {
  if(type == Slide:SLIDE_NONE)  {

    for(new i=0; i < 10; i++) {
      g_SlideBits[client] = 1 << i | g_SlideBits[client];
    }
  }
  else
    g_SlideBits[client] = 1 << _:type | g_SlideBits[client];

}

stock bool:IsSlideOn(client)  {
  return g_ffSlideStep[client] > 0;
}

public Action:TimerKeepSlideUp(Handle:timer, any:client)  {
  if(!IsClientInGame(client))
    return Plugin_Stop;
  if(g_ffSlideStep[client] == 0)  {
    SetOverlay(client, "0");
    /*Client_ShowRadar(client);*/
    return Plugin_Stop;
  }
  /*new MenuSource:menu = GetClientMenu(client);*/
  /*if(menu == MenuSource_None)*/
    /*Client_HideRadar(client);*/
  /*else*/
    /*Client_ShowRadar(client);*/
  if(IsFirstSlide(client))  {
    static count[65];
    count[client]++;
    if(count[client] % 20 == 0)  {
      count[client] = 0;
      WCMessage(client, "exit slide", client);
    }
  }
  SetOverlay(client, g_ffSlidePath[client]);

  return Plugin_Continue;
}

stock SetOverlay(client, String:path[]) {
  decl String:fullPath[100];
  Format(fullPath, sizeof(fullPath), "r_screenoverlay %s", path);
  ClientCommand(client, fullPath);
}

