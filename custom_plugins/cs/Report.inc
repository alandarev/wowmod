/*
name = "Report",
author = "CreativeFrags",
description = "makes you able to type !report and Report a Suggestion/Bug/Player",
version = "0.5"
*/

#include <sourcemod>
#include <sdktools>
#pragma semicolon 1

new bool:report[64] = false;
new bool:report_type[3] = false;
/*
0 = Suggestion
1 = Bug
2 = Player (not needet now)
*/

public LoadReport()
{
	RegConsoleCmd("sm_report", Command_Report);
	AddCommandListener(ReportAnyThing, "say");
}

public Action:Command_Report(client, args)
{
	ShowReportPanel(client);
	return Plugin_Handled;
}

ShowReportPanel(client)
{
	new Handle:reportpanel = CreatePanel();
	
	SetPanelTitle(reportpanel, "Report Menu");
	DrawPanelItem(reportpanel, "Suggestion");
	DrawPanelItem(reportpanel, "Bug");
	//DrawPanelItem(reportpanel, "Player (not ready!!)");
	
	SendPanelToClient(reportpanel, client, ReportPanelHandler, 20);
	
	CloseHandle(reportpanel);
}

public ReportPanelHandler(Handle:menu, MenuAction:action, client, param2)
{
	if(action == MenuAction_Select)
	{
		switch(param2)
		{
			case 1:
			{
				PrintToChat(client, "now you got 5 minutes to write your Suggestion into the Chat");
				report_type[0] = true;
				report[client] = true;
			}
			case 2:
			{
				PrintToChat(client, "now you got 5 minutes to write your Bug-Description into the Chat");
				report_type[1] = true;
				report[client] = true;
			}
			/*case 3:
			{
				PrintToChat(client, "now you got 5 minutes to write the reason for your playerreport into the Chat");
				CreateTimer(300.0,ReportTimer, client);
				report_type[2] = true;
				report[client] = true;
			}*/
		}
	}
}

// SQL REPORT EXAMPLE BY STINKY 
// SQL_AddReport(REPORT:reportType, String:steamID, String:title, String:text)
// MY
// SQL_AddReport(REPORT:reportType, String:steamID, String:text, String:targetID)    // cuz no need title?  but need target id (if report player)

public Action:ReportAnyThing(client, const String:command[], argc)
{
	if(report[client]==true)
	{
		static String:arg[64];
		GetCmdArgString(arg, sizeof(arg));
		StripQuotes(arg);
		
		decl String:id[64];
		GetClientAuthString(client, id, 63);
		
		//  ############################################################### SUGGESTION ###############################################################
		if(report_type[0]==true) 
		{
			PrintToServer("SUGGESTION: %s",arg);
			//SQL_AddReport("Suggestion",id,arg);
				
			report[client] = false;
			report_type[0] = false;
			return Plugin_Handled;
		}
		else if(report_type[1]==true)
		//  ###############################################################    BUG    ################################################################
		{
			PrintToServer("BUG: %s",arg);
			//SQL_AddReport("Bug",id,arg);
				
			report[client] = false;
			report_type[1] = false;
			return Plugin_Handled;
		}
		/*
		else if(report_type[1]==true)
		//  ###############################################################   PLAYERS   ################################################################
		{
			
			PrintToServer("PLAYER: %s",arg);
			//SQL_AddReport("Player",id,arg,tarid);
				
			report[client] = false;
			report_type[1] = false;
			return Plugin_Handled;
			KillTimer(ReportTimer);
		}
		*/
	}
	return Plugin_Continue;
}