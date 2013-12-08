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

public Plugin:myinfo = 
{
	name = "Report",
	author = "CreativeFrags",
	description = "makes you able to type !report and Report a Suggestion/Bug/Player",
	version = "0.5",
	url = "http://www.wowmod.eu"
}

public OnPluginStart()
{
	RegConsoleCmd("sm_report", Command_Report);
	AddCommandListener(ReportAnyThing, "say");
}

public Action:Command_Report(client, args)
{
	ShowPanel(client);
	return Plugin_Handled;
}

ShowPanel(client)
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
				CreateTimer(300.0,ReportTimer, client);
				report_type[0] = true;
				report[client] = true;
			}
			case 2:
			{
				PrintToChat(client, "now you got 5 minutes to write your Bug-Description into the Chat");
				CreateTimer(300.0,ReportTimer, client);
				report_type[1] = true;
				report[client] = true;
			}
			/*case 3:
			{
				PrintToChat(client,"its not ready :D");
				report_type[2] = true;
			}*/
		}
	}
}

public Action:ReportAnyThing(client, const String:command[], argc)
{
	if(report[client]==true)
	{
		static String:arg[64];
		GetCmdArgString(arg, sizeof(arg));
		StripQuotes(arg);
		
		//  ############################################################### SUGGESTION ###############################################################
		if(report_type[0]==true) 
		{
			PrintToChat(client, "SUGGESTION: %s",arg);

			report[client] = false;
			report_type[0] = false;
			return Plugin_Handled;
			KillTimer(ReportTimer);
		}
		else if(report_type[1]==true)
		//  ###############################################################    BUG    ################################################################
		{
			PrintToChat(client, "BUG: %s",arg);
				
			report[client] = false;
			report_type[1] = false;
			return Plugin_Handled;
			KillTimer(ReportTimer);
		}
	}
	return Plugin_Continue;
}

public Action:ReportTimer(Handle:timer, any:client)
{
	report_type[0] = false;
	report_type[1] = false;
	report_type[2] = false;
	report[client] = false;
}