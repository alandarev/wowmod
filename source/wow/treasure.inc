/** The majority of spawn menu/location code is directly taken from Bailopan's CSSDM */

new Float:g_SpawnOrigins[MAX_TREASURE_SPAWNS][3];
new Float:g_SpawnAngles[MAX_TREASURE_SPAWNS][3];
new String:g_SpawnNames[MAX_TREASURE_SPAWNS][32];
new g_SpawnCount = 0;
new g_LastLocation[65];
new Float:g_fLastChestSpawn;

stock bool:SpawnChests(bool:safe=true)  {
  if (safe && !IsTreasureHunt()
      && (GetEngineTime() - 90.0 < g_fLastChestSpawn)) {
    return false;
  }
  new chance = 7;
  new times = 1;
  if(IsDmEnabled())
    chance = 1000;
  if(IsTreasureHunt()) {
    chance = 1000;
    if(PlayersInTeams(true)>8)
      times++;
    if(PlayersInTeams(true)>12)
      times++;
    if(PlayersInTeams(true)>16)
      times++;
    if(PlayersInTeams(true)>24)
      times++;
  }
  new bool:spawned = TreasureChestMap(chance, times) > 0;
  if (spawned)  {
    g_fLastChestSpawn = GetEngineTime();
  }
  return spawned;
}

bool:WriteTreasureMapConfig()
{
  new String:map[64];
  GetCurrentMap(map, sizeof(map));

  new String:path[255];
  Format(path, sizeof(path), "cfg/treasure/spawns/%s.txt", map);

  new Handle:file = OpenFile(path, "wt");
  if (file == INVALID_HANDLE)
  {
    LogError("Could not open spawn point file \"%s\" for writing.", path);
    return false;
  }

  for (new i=0; i<g_SpawnCount; i++)
  {
    //example: %f|%f|%f|pitch|yaw|roll|name
    //WriteFileLine(file, "%f|%f|%f|%f|%f|%f|%s",
    WriteFileLine(file, "%f|%f|%f|0.0|%f|0.0|%s",
      g_SpawnOrigins[i][0],
      g_SpawnOrigins[i][1],
      g_SpawnOrigins[i][2],
      //g_SpawnAngles[i][0],
      g_SpawnAngles[i][1],
      //g_SpawnAngles[i][2],
      g_SpawnNames[i]);
  }

  CloseHandle(file);

  return true;
}

bool:Hani()
{
  new String:map[64];
  GetCurrentMap(map, sizeof(map));

  new String:path[255];
  Format(path, sizeof(path), "cfg/treasure/spawns/%s.txt", map);

  new Handle:file = OpenFile(path, "wt");
  if (file == INVALID_HANDLE)
  {
    LogError("Could not open spawn point file \"%s\" for writing.", path);
    return false;
  }

  if(StrEqual(map,"de_dust2",false)) {
    WriteFileLine(file,"733.5|680.0|96.0|0.0|0.0|0.0|Side Door");
    WriteFileLine(file,"495.9|1368.0|96.0|0.0|0.0|0.0|Middle");
    WriteFileLine(file,"-828.4|1362.2|16.0|0.0|0.0|0.0|Tunnel");
    WriteFileLine(file,"-572.7|649.6|160.0|0.0|0.0|0.0|Middle");
    WriteFileLine(file,"690.9|2763.8|224.0|0.0|0.0|0.0|Bombsite-A");
    WriteFileLine(file,"-1195.5|1263.9|48.9|0.0|0.0|0.0|Tunnel");
    WriteFileLine(file,"-272.0|1429.3|-31.9|0.0|0.0|0.0|Double Doors");
    WriteFileLine(file,"-860.1|2572.4|-46.8|0.0|0.0|0.0|Bombsite-B");
    WriteFileLine(file,"1438.5|188.8|-190.5|0.0|0.0|0.0|Side");
    WriteFileLine(file,"-2072.9|1301.2|154.2|0.0|0.0|0.0|Tunnel");
    WriteFileLine(file,"-2063.5|3119.9|160.0|0.0|0.0|0.0|Bombsite-B");
  }
  else if(StrEqual(map,"cs_compound",false)) {
    WriteFileLine(file,"2536.6|-548.4|-0.3|0.0|0.0|0.0|Side");
    WriteFileLine(file,"3504.7|-479.4|16.1|0.0|0.0|0.0|Side");
    WriteFileLine(file,"336.1|41.8|0.0|0.0|0.0|0.0|Far Side");
    WriteFileLine(file,"452.2|1103.3|0.0|0.0|0.0|0.0|Far side");
    WriteFileLine(file,"2161.3|-952.0|0.0|0.0|0.0|0.0|Back");
    WriteFileLine(file,"2378.6|-1820.6|56.6|0.0|0.0|0.0|Back");
    WriteFileLine(file,"1681.1|-1520.4|16.0|0.0|0.0|0.0|Storage Room");
    WriteFileLine(file,"1726.9|-1482.2|168.9|0.0|0.0|0.0|Storage Room");
    WriteFileLine(file,"1648.5|671.8|408.3|0.0|0.0|0.0|Inside");
    WriteFileLine(file,"2568.0|632.9|4.0|0.0|0.0|0.0|Gate house");
    WriteFileLine(file,"2487.5|708.5|2.6|0.0|0.0|0.0|Front");
    WriteFileLine(file,"1771.9|865.5|128.8|0.0|0.0|0.0|Front");
    WriteFileLine(file,"1130.5|1024.4|49.9|0.0|0.0|0.0|Front");
    WriteFileLine(file,"-3247.8|109.3|1.9|0.0|0.0|0.0|Gate");
    WriteFileLine(file,"3599.8|1507.6|1.8|0.0|0.0|0.0|Outside");
  }
  else if(StrEqual(map,"de_nuke",false)) {
    WriteFileLine(file,"42.1|-947.2|-767.9|0.0|0.0|0.0|Back Way");
    WriteFileLine(file,"91.0|-1669.7|-46.4|0.0|0.0|0.0|Back Way");
    WriteFileLine(file,"566.7|854.0|-479.9|0.0|0.0|0.0|Ramp");
    WriteFileLine(file,"975.9|-203.0|-415.9|0.0|0.0|0.0|Side Hall");
    WriteFileLine(file,"905.0|-1420.4|-415.9|0.0|0.0|0.0|Bombsite-A");
    WriteFileLine(file,"320.5|-1484.9|-127.8|0.0|0.0|0.0|Loft");
    WriteFileLine(file,"1324.6|-377.6|-639.9|0.0|0.0|0.0|Underground");
    WriteFileLine(file,"208.7|-1118.8|-383.9|0.0|0.0|0.0|Inside");
    WriteFileLine(file,"1507.7|-1481.1|-103.9|0.0|0.0|0.0|Roof");
    WriteFileLine(file,"1551.4|-2256.0|-415.9|0.0|0.0|0.0|Garage");
    WriteFileLine(file,"2082.5|-1930.6|-415.9|0.0|0.0|0.0|Garage");
    WriteFileLine(file,"2421.8|-1793.9|-415.9|0.0|0.0|0.0|CT Spawn");
  }
  else if(StrEqual(map,"de_inferno",false)) {
    WriteFileLine(file,"2465.7|-39.4|80.0|0.0|0.0|0.0|Bombsite-B");
    WriteFileLine(file,"653.9|3077.6|160.0|0.0|0.0|0.0|Bombsite-A ");
    WriteFileLine(file,"690.9|2763.8|224.0|0.0|0.0|0.0|Window");
    WriteFileLine(file,"-178.7|-696.6|64.0|0.0|0.0|0.0|Window");
    WriteFileLine(file,"-214.3|96.5|182.0|0.0|0.0|0.0|Upstairs");
    WriteFileLine(file,"-480.0|-564.4|192.0|0.0|0.0|0.0|Upstairs ");
    WriteFileLine(file,"75.7|181.0|182.0|0.0|0.0|0.0|Deck");
    WriteFileLine(file,"1059.8|-517.5|256.0|0.0|0.0|0.0|Bedroom");
    WriteFileLine(file,"1131.8|-245.4|256.0|0.0|0.0|0.0|Balcony");
    WriteFileLine(file,"-76.4|1412.2|106.0|0.0|0.0|0.0|Alley ");
    WriteFileLine(file,"863.9|1895.3|134.5|0.0|0.0|0.0|Garage");
    WriteFileLine(file,"292.3|521.3|21.9|0.0|0.0|0.0|Crawlspace");
    WriteFileLine(file,"943.8|136.3|256.0|0.0|0.0|0.0|Back Alley ");
  }
  else if(StrEqual(map,"de_train",false)) {
    WriteFileLine(file,"-887.5|-87.9|16.0|0.0|0.0|0.0|Bathroom");
    WriteFileLine(file,"-1047.9|-504.0|-95.9|0.0|0.0|0.0|Stairs");
    WriteFileLine(file,"-966.1|-452.3|16.0|0.0|0.0|0.0|Long Hall");
    WriteFileLine(file,"-336.0|-334.3|-217.9|0.0|0.0|0.0|Ladder");
    WriteFileLine(file,"257.8|751.9|40.0|0.0|0.0|0.0|Bombsite-A");
    WriteFileLine(file,"-48.0|776.9|-217.4|0.0|0.0|0.0|Back Alley");
    WriteFileLine(file,"1174.1|221.9|-66.7|0.0|0.0|0.0|Bombsite-A");
    WriteFileLine(file,"991.9|-156.4|-150.9|0.0|0.0|0.0|Bombsite-A");
    WriteFileLine(file,"1437.9|735.9|-215.9|0.0|0.0|0.0|Bombsite-A");
    WriteFileLine(file,"506.2|-1533.2|-359.9|0.0|0.0|0.0|Bombsite-B");
    WriteFileLine(file,"270.6|642.0|40.0|0.0|0.0|0.0|Bombsite-B");
    WriteFileLine(file,"-369.0|1232.0|-140.9|0.0|0.0|0.0|Back Alley");
  }
  else if((StrEqual(map,"de_dust",false)) && ((strlen(map)==7))) {
    WriteFileLine(file,"-304.6|3041.6|17.0|0.0|0.0|0.0|T-Spawn!");
    WriteFileLine(file,"-1730.8|2077.3|128.0|0.0|0.0|0.0|Back");
    WriteFileLine(file,"-1975.6|2881.2|32.0|0.0|0.0|0.0|Back");
    WriteFileLine(file,"-1232.7|1936.0|-127.9|0.0|0.0|0.0|Bridge");
    WriteFileLine(file,"462.8|852.4|128.0|0.0|0.0|0.0|Front");
    WriteFileLine(file,"717.7|437.0|128.0|0.0|0.0|0.0|Double Doors!");
    WriteFileLine(file,"1110.1|879.9|120.0|0.0|0.0|0.0|Bombsite-A");
    WriteFileLine(file,"-224.6|1184.4|128.0|0.0|0.0|0.0|Middle");
    WriteFileLine(file,"216.3|1539.2|128.0|0.0|0.0|0.0|Middle");
    WriteFileLine(file,"26.1|1335.9|32.0|0.0|0.0|0.0|Middle");
    WriteFileLine(file,"-1068.8|1284.7|32.0|0.0|0.0|0.0|Side");
    WriteFileLine(file,"-1215.9|768.7|-63.9|0.0|0.0|0.0|Bridge");
  }
  else if(StrEqual(map,"aim_ag_texture_mix",false)) {
    WriteFileLine(file,"-445.6|283.4|128.0|0.0|0.0|0.0|Far side");
    WriteFileLine(file,"515.8|281.5|128.0|0.0|0.0|0.0|Back");
    WriteFileLine(file,"6.1|-815.9|128.0|0.0|0.0|0.0|Storage Room");
    WriteFileLine(file,"-661.5|-430.9|384.0|0.0|0.0|0.0|Side");
    WriteFileLine(file,"1775.9|-313.3|640.0|0.0|0.0|0.0|Gate house");
    WriteFileLine(file,"61.6|-726.2|512.0|0.0|0.0|0.0|Inside");
    WriteFileLine(file,"-648.1|282.6|640.0|0.0|0.0|0.0|Front");
    WriteFileLine(file,"103.3|244.9|640.0|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"-667.6|559.9|448.0|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"1553.0|166.8|768.0|0.0|0.0|0.0|Gate");
    WriteFileLine(file,"1906.3|-317.2|960.0|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"845.3|-199.2|512.0|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"463.0|-168.6|128.0|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"1314.1|-164.7|128.0|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"791.8|-879.9|128.0|0.0|0.0|0.0|Somewhere");
  }
  else if(StrEqual(map,"de_cpl_mill",false)) {
    WriteFileLine(file,"2031.9|2082.4|208.0|0.0|0.0|0.0|Bombsite-B");
    WriteFileLine(file,"1375.6|2127.8|184.0|0.0|0.0|0.0|Courtyard");
    WriteFileLine(file,"420.1|2008.8|68.0|0.0|0.0|0.0|Gate");
    WriteFileLine(file,"574.6|1163.0|-43.9|0.0|0.0|0.0|Side");
    WriteFileLine(file,"-9.2|1607.1|-111.9|0.0|0.0|0.0|Gate house");
    WriteFileLine(file,"-1519.9|1551.1|-111.9|0.0|0.0|0.0|Inside");
    WriteFileLine(file,"-671.0|2271.9|207.1|0.0|0.0|0.0|Front");
    WriteFileLine(file,"-1092.0|2218.7|-287.9|0.0|0.0|0.0|Sewers");
    WriteFileLine(file,"-72.3|2371.8|-159.9|0.0|0.0|0.0|Sewers");
    WriteFileLine(file,"-375.4|1647.9|-111.9|0.0|0.0|0.0|Gate");
  }
  else if(StrEqual(map,"de_cpl_strike",false)) {
    WriteFileLine(file,"338.1|-691.8|-103.9|0.0|0.0|0.0|Far side");
    WriteFileLine(file,"173.7|98.0|-159.9|0.0|0.0|0.0|Back");
    WriteFileLine(file,"-774.2|-484.4|-167.9|0.0|0.0|0.0|Storage Room");
    WriteFileLine(file,"-1148.4|-1493.9|-81.5|0.0|0.0|0.0|Front Yard");
    WriteFileLine(file,"-23.9|-1920.0|-39.9|0.0|0.0|0.0|Gate house");
    WriteFileLine(file,"-446.6|-2248.3|8.0|0.0|0.0|0.0|Inside");
    WriteFileLine(file,"-448.8|-1591.8|-39.9|0.0|0.0|0.0|Front");
    WriteFileLine(file,"-446.6|-2248.3|8.0|0.0|0.0|0.0|Front");
    WriteFileLine(file,"-756.9|-991.2|-167.9|0.0|0.0|0.0|Middle");
    WriteFileLine(file,"472.0|-1453.9|-167.9|0.0|0.0|0.0|Gate");
    WriteFileLine(file,"363.3|-2086.0|-39.9|0.0|0.0|0.0|Apartment");
    WriteFileLine(file,"-1047.9|81.3|-367.9|0.0|0.0|0.0|Underground");
    WriteFileLine(file,"-1105.3|352.0|-79.9|0.0|0.0|0.0|Courtyard");
  }
  else if(StrEqual(map,"de_season",false)) {
    WriteFileLine(file,"3439.9|290.6|-79.9|0.0|0.0|0.0|Far side");
    WriteFileLine(file,"3507.1|704.4|48.1|0.0|0.0|0.0|Back");
    WriteFileLine(file,"3507.4|1450.5|-53.1|0.0|0.0|0.0|Storage Room");
    WriteFileLine(file,"1868.9|159.9|68.0|0.0|0.0|0.0|Front Yard");
    WriteFileLine(file,"2106.5|728.0|0.0|0.0|0.0|0.0|Gate house");
    WriteFileLine(file,"969.0|249.7|-42.2|0.0|0.0|0.0|Inside");
    WriteFileLine(file,"142.8|-676.0|-48.0|0.0|0.0|0.0|Front");
    WriteFileLine(file,"-765.1|-1085.8|-15.9|0.0|0.0|0.0|Long Hall");
    WriteFileLine(file,"146.2|-828.6|48.0|0.0|0.0|0.0|Tunnel!");
    WriteFileLine(file,"-445.0|215.8|128.0|0.0|0.0|0.0|Gate");
    WriteFileLine(file,"-397.5|-1276.2|-167.9|0.0|0.0|0.0|Long Hall");
  }
  else if(StrEqual(map,"cs_italy_tactik2",false)) {
    WriteFileLine(file,"-37.4|-152.0|-151.9|0.0|0.0|0.0|Far side");
    WriteFileLine(file,"-364.5|2191.9|-82.0|0.0|0.0|0.0|Back");
    WriteFileLine(file,"-1073.8|2124.3|-151.9|0.0|0.0|0.0|Storage Room");
    WriteFileLine(file,"-988.1|1247.7|-7.9|0.0|0.0|0.0|Front Yard");
    WriteFileLine(file,"-1417.5|56.4|8.0|0.0|0.0|0.0|Gate house");
    WriteFileLine(file,"-1327.9|1300.8|-143.6|0.0|0.0|0.0|Inside");
    WriteFileLine(file,"-777.7|189.0|8.0|0.0|0.0|0.0|Front");
    WriteFileLine(file,"-822.6|805.9|160.0|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"34.9|425.9|152.0|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"-410.7|506.9|32.0|0.0|0.0|0.0|Gate");
    WriteFileLine(file,"-172.4|501.3|-31.9|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"1181.9|768.6|-194.9|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"556.0|-391.6|-41.9|0.0|0.0|0.0|Somewhere");
  }
  else if(StrEqual(map,"knas_sandland_css",false)) {
    WriteFileLine(file,"384.0|-1832.0|-263.9|0.0|0.0|0.0|Far side");
    WriteFileLine(file,"-78.1|-343.0|-191.9|0.0|0.0|0.0|Back");
    WriteFileLine(file,"608.0|-547.9|32.0|0.0|0.0|0.0|Storage Room");
    WriteFileLine(file,"336.0|231.9|128.0|0.0|0.0|0.0|Front Yard");
    WriteFileLine(file,"-332.6|199.4|-175.9|0.0|0.0|0.0|Gate house");
    WriteFileLine(file,"-785.1|78.0|-415.9|0.0|0.0|0.0|Inside");
    WriteFileLine(file,"-1295.9|537.4|-399.9|0.0|0.0|0.0|Front");
    WriteFileLine(file,"-815.7|368.0|-191.9|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"-911.9|519.2|-127.9|0.0|0.0|0.0|Somewhere");
    WriteFileLine(file,"48.0|-512.0|32.0|0.0|0.0|0.0|Gate");
    WriteFileLine(file,"920.0|-79.9|32.0|0.0|0.0|0.0|Somewhere");
  }
  else if(StrEqual(map,"de_losttemple_pro",false)) {
    WriteFileLine(file,"1890.0|420.0|200.0|0.0|0.0|0.0|Far side");
    WriteFileLine(file,"1875.1|-465.6|232.0|0.0|0.0|0.0|Back");
    WriteFileLine(file,"1048.0|-434.2|184.0|0.0|0.0|0.0|Water");
    WriteFileLine(file,"392.1|-478.2|238.4|0.0|0.0|0.0|Main Hall");
    WriteFileLine(file,"1328.6|-350.9|102.0|0.0|0.0|0.0|Water");
    WriteFileLine(file,"-590.2|-510.2|195.8|0.0|0.0|0.0|Overpass");
    WriteFileLine(file,"-711.3|-646.2|64.0|0.0|0.0|0.0|Underpass");
    WriteFileLine(file,"383.9|-1003.6|184.0|0.0|0.0|0.0|Middle");
    WriteFileLine(file,"75.5|-1580.5|240.0|0.0|0.0|0.0|Ramp");
    WriteFileLine(file,"-99.9|587.4|2.3|0.0|0.0|0.0|Gate");
    WriteFileLine(file,"788.9|-424.9|6.5|0.0|0.0|0.0|Water");
    WriteFileLine(file,"-246.8|-751.9|0.0|0.0|0.0|0.0|Underpass");
  }
  else
  {
    LogError("Spawns MUST be defined for treasure chests to be available!");
  }

  CloseHandle(file);
  g_AreWeSpawningTreasure = LoadTreasureMapConfig(false);
  return true;
}

bool:LoadTreasureMapConfig(bool:emo=true)
{
  new String:map[64];
  GetCurrentMap(map, sizeof(map));

  new String:path[255];
  Format(path, sizeof(path), "cfg/treasure/spawns/%s.txt", map);

  g_SpawnCount = 0;

  new Handle:file = OpenFile(path, "rt");
  if (file == INVALID_HANDLE)
  {
    LogError("Could not find spawn point file \"%s\"", path);
    if(emo)
      Hani();
    return false;
  }

  new String:buffer[255];
  new String:parts[7][32];
  new partCount;
  while (!IsEndOfFile(file) && ReadFileLine(file, buffer, sizeof(buffer)))
  { //example: 0.0|0.0|0.0|0.0|0.0|0.0|Long-A Pit
    TrimString(buffer);
    partCount = ExplodeString(buffer, "|", parts, 7, 32);
    if (partCount < 6)
    {
      continue;
    }
    g_SpawnOrigins[g_SpawnCount][0] = StringToFloat(parts[0]);
    g_SpawnOrigins[g_SpawnCount][1] = StringToFloat(parts[1]);
    g_SpawnOrigins[g_SpawnCount][2] = StringToFloat(parts[2]);
    g_SpawnAngles[g_SpawnCount][0] = 0.0; //StringToFloat(parts[3]);
    g_SpawnAngles[g_SpawnCount][1] = StringToFloat(parts[4]);
    g_SpawnAngles[g_SpawnCount][2] = 0.0; //StringToFloat(parts[5]);
    g_SpawnNames[g_SpawnCount] = parts[6];
    g_SpawnCount++;
  }

  CloseHandle(file);

  LogMessage("Preset treasure spawn points loaded (number %d) (map %s)", g_SpawnCount, map);

  return true;
}

GetNearestSpawn(client)
{
  if (!g_SpawnCount)
  {
    return -1;
  }

  new Float:clorigin[3];
  GetClientAbsOrigin(client, clorigin);

  new Float:low_diff = GetDistanceBetween(g_SpawnOrigins[0], clorigin);
  new low_index = 0;
  for (new i=1; i<g_SpawnCount; i++)
  {
    new Float:diff = GetDistanceBetween(g_SpawnOrigins[i], clorigin);
    if (diff < low_diff)
    {
      low_diff = diff;
      low_index = i;
    }
  }

  return low_index;
}

AddSpawnFromClient(client)
{
  if (g_SpawnCount >= MAX_TREASURE_SPAWNS)
  {
    return -1;
  }

  GetClientAbsOrigin(client, g_SpawnOrigins[g_SpawnCount]);
  GetClientAbsAngles(client, g_SpawnAngles[g_SpawnCount]);
  g_SpawnNames[g_SpawnCount]=g_TreasureSpawnName;
  new old = g_SpawnCount++;

  return old;
}

bool:DeleteSpawn(index)
{
  if (index < 0 || index >= g_SpawnCount)
  {
    return false;
  }

  for (new i=index; i<g_SpawnCount-1; i++)
  {
    g_SpawnAngles[i] = g_SpawnAngles[i+1];
    g_SpawnOrigins[i] = g_SpawnOrigins[i+1];
    g_SpawnNames[i] = g_SpawnNames[i+1];
  }

  g_SpawnCount--;

  return true;
}

public Action:Command_TreasureSpawnMenu(client, args)
{
  if (client == 0)
  {
    ReplyToCommand(client, "[Treasure] This command is not available from the server console.");
    return Plugin_Handled;
  }
  PrepareTreasureSpawnMenu(client);
  return Plugin_Handled;
}

public Panel_VerifyDeleteSpawns(Handle:menu, MenuAction:action, param1, param2)
{
  if (action == MenuAction_Select)
  {
    if (param2 == 1)
    {
      g_SpawnCount = 0;
      if (!WriteTreasureMapConfig())
        PrintToChat(param1, "[Treasure] Could not write to spawn config file.");
      else
        PrintToChat(param1, "[Treasure] All spawn points have been deleted.");
    }
    PrepareTreasureSpawnMenu(param1);
  }
}

InsertSpawnFromClient(client, bool:pre, index)
{
  if (index == g_SpawnCount - 1 && !pre)
  {
    return AddSpawnFromClient(client);
  }

  if (g_SpawnCount >= MAX_TREASURE_SPAWNS)
  {
    return -1;
  }

  /* If this is a post-insertion, unmark the index for moving */
  if (!pre)
  {
    index++;
  }

  /* Move all of the slots down */
  for (new i=g_SpawnCount-1; i>=index; i--)
  {
    g_SpawnOrigins[i+1] = g_SpawnOrigins[i];
    g_SpawnAngles[i+1] = g_SpawnAngles[i];
    g_SpawnNames[i+1] = g_SpawnNames[i];
  }

  GetClientAbsOrigin(client, g_SpawnOrigins[index]);
  GetClientAbsAngles(client, g_SpawnAngles[index]);
  g_SpawnNames[index]=g_TreasureSpawnName;


  g_SpawnCount++;

  return index;
}

public Action:TimerKillEnt(Handle:timer, Handle:dp)
{
  ResetPack(dp);
  new ent = ReadPackCell(dp);
  new index = ReadPackCell(dp);
  g_occupiedTreasureSpawns[index]=false;
  if(IsValidEntity(ent))
    AcceptEntityInput(ent, "Kill");
}

PrepareTreasureSpawnMenu(client, String:title[32]="Treasure Spawn Point Editor")
{
  if (g_TreasureMenuOccupiedByClient == client ||
      g_TreasureMenuOccupiedByClient == 0)
  { //only one person at a time to keep it simple.
    WCMessage(client,"testmessage",client,"YES: g_TreasureMenuOccupiedByClient(client|0)",client);
    g_TreasureMenuOccupiedByClient=client;
    hTreasureSpawnMenu = CreateMenu(Menu_EditTreasureSpawns);
    SetMenuTitle(hTreasureSpawnMenu, title);
    AddMenuItem(hTreasureSpawnMenu, "nearest", "Teleport to nearest");    //0
    AddMenuItem(hTreasureSpawnMenu, "previous", "Teleport to previous");  //1
    AddMenuItem(hTreasureSpawnMenu, "next", "Teleport to next");          //2
    AddMenuItem(hTreasureSpawnMenu, "add", "Add position");               //3
    AddMenuItem(hTreasureSpawnMenu, "preinsert", "Insert position here"); //4
    AddMenuItem(hTreasureSpawnMenu, "delete", "Delete nearest");          //5
    AddMenuItem(hTreasureSpawnMenu, "spawn", "Spawn chest at Nearest");   //6
    AddMenuItem(hTreasureSpawnMenu, "clear", "Delete all");               //7
    DisplayMenu(hTreasureSpawnMenu, client, MENU_TIME_FOREVER);
  }
  else
  {
    WCMessage(client,"testmessage",client,"NO: g_TreasureMenuOccupiedByClient(client|0)",client);
    WCMessage(client,"testmessage",client,"Needed: g_TreasureMenuOccupiedByClient(client|0)",g_TreasureMenuOccupiedByClient);
  }
}

public Menu_EditTreasureSpawns(Handle:menu, MenuAction:action, param1, param2)
{
  //if (action == MenuAction_Start) //param1 & param2 are not set.
  if (action == MenuAction_Select) //param1 client, param2 slot
  {
    switch (param2)
    {
      case 0: //Teleport to Nearest
      {
        new index = GetNearestSpawn(param1);
        if (index == -1)
          PrintToChat(param1, "[Treasure] There are no spawn points.");
        else
        {
          TeleportEntity(param1, g_SpawnOrigins[index], g_SpawnAngles[index], NULL_VECTOR);
          PrintToChat(param1, "[Treasure] Teleported to spawn #%d (%d total).", index, g_SpawnCount);
          g_LastLocation[param1] = index;
        }
      }
      case 2: //Teleport to Next
      {
        if (g_SpawnCount == 0)
          PrintToChat(param1, "[Treasure] There are no spawn points.");
        else
        {
          new index = g_LastLocation[param1] + 1;
          if (index >= g_SpawnCount)
            index = 0;
          TeleportEntity(param1, g_SpawnOrigins[index], g_SpawnAngles[index], NULL_VECTOR);
          PrintToChat(param1, "[Treasure] Teleported to spawn #%d (%d total).", index, g_SpawnCount);
          g_LastLocation[param1] = index;
        }
      }
      case 1: //Teleport to Previous
      {
        if (g_SpawnCount == 0)
          PrintToChat(param1, "[Treasure] There are no spawn points.");
        else
        {
          new index = g_LastLocation[param1] - 1;
          if (index < 0)
            index = g_SpawnCount - 1;
          TeleportEntity(param1, g_SpawnOrigins[index], g_SpawnAngles[index], NULL_VECTOR);
          PrintToChat(param1, "[Treasure] Teleported to spawn #%d (%d total).", index, g_SpawnCount);
          g_LastLocation[param1] = index;
        }
      }
      case 5: //Delete Nearest
      {
        new index = GetNearestSpawn(param1);
        if (index == -1)
          PrintToChat(param1, "[Treasure] There are no spawn points.");
        else if (!DeleteSpawn(index))
          PrintToChat(param1, "[Treasure] Could not delete spawn #%d.", index);
        else if (!WriteTreasureMapConfig())
          PrintToChat(param1, "[Treasure] Could not write to spawn config file!");
        else
          PrintToChat(param1, "[Treasure] Deleted spawn #%d (%d total).", index, g_SpawnCount);
      }
      case 3: //Add current Position
      {
        new index;
        if ((index = AddSpawnFromClient(param1)) == -1)
          PrintToChat(param1, "[Treasure] Could not add spawn (max limit reached).");
        else if (!WriteTreasureMapConfig())
          PrintToChat(param1, "[Treasure] Could not write to spawn config file!");
        else
          PrintToChat(param1, "[Treasure] Added spawn #%d (%d total).", index, g_SpawnCount);
      }
      case 4: //Insert Position
      {
        new index = g_LastLocation[param1];
        new bool:pre = true;
        if (index == -1 || index >= g_SpawnCount)
        {
          index = g_SpawnCount - 1;
          pre = false;
        }
        if ((index = InsertSpawnFromClient(param1, pre, index)) == -1)
          PrintToChat(param1, "[Treasure] Could not add spawn (max limit reached).");
        else if (!WriteTreasureMapConfig())
          PrintToChat(param1, "[Treasure] Could not write to spawn config file!");
        else
        {
          PrintToChat(param1, "[Treasure] Inserted spawn at #%d (%d total).", index, g_SpawnCount);
        }
      }
      case 6: //Spawn a chest at nearest
      {
        new index = GetNearestSpawn(param1);
        if (index == -1)
          PrintToChat(param1, "[Treasure] There are no spawn points.");
        else if (g_occupiedTreasureSpawns[index])
          PrintToChat(param1, "[Treasure] Closest spawn point is already occupied.");
        else
        {
          g_occupiedTreasureSpawns[index]=true;
          new ent = SpawnChestModel(g_SpawnOrigins[index],g_SpawnAngles[index],1);
          new Handle:datapack=CreateDataPack();
          WritePackCell(datapack,ent);
          WritePackCell(datapack,index);
          CreateTimer(60.0, TimerKillEnt, datapack, TIMER_HNDL_CLOSE);
          //TeleportEntity(ent, g_SpawnOrigins[index], g_SpawnAngles[index], NULL_VECTOR);
          PrintToChat(param1, "[Treasure] Teleported to spawn #%d (%d total).", index, g_SpawnCount);
          g_LastLocation[param1] = index;
        }
      }
      case 7: //Delete All Locations!
      {
        /* Of course, we ask the user first. */
        new Handle:panel = CreatePanel();
        SetPanelTitle(panel, "Delete all spawn points?");
        DrawPanelItem(panel, "Yes");
        DrawPanelItem(panel, "No");
        SendPanelToClient(panel, param1, Panel_VerifyDeleteSpawns, MENU_TIME_FOREVER);
        CloseHandle(panel);
        return;
      }
    }
    /* Redraw the menu */
    PrepareTreasureSpawnMenu(param1,g_TreasureSpawnName);
  }
  //if (action == MenuAction_Cancel) //param1 client, param2 MenuCancel reason
  if (action == MenuAction_End) //param1 MenuEnd reason, param2 If param1 is MenuEnd_Cancelled, the MenuCancel reason
  { //this is called when all clients have closed this menu and
    g_TreasureMenuOccupiedByClient=0; //no longer in use
  }
}

