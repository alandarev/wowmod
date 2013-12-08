/*
* File: profession.inc
* Description: Profession related functions
* Funcions you will find here are only 1.20 or higher version
* Lower version profession functions are residented in other files
* mostly in wcmenus.inc.
* Author(s): Stinkyfax
*/

#pragma semicolon 1


/*
* Hardcoded Profession constants, too lazy to create a data file for those
*/

new g_iCraftProfLevels[10][MAX_PROF_LEVELS][10];

DefineProfLevels()  {
  //Apprentice 1-75
  //Journeyman 75-125
  //Expert 125-200
  //Artisan 200-275
  //Master 275-350
  //Grand Master 350-425
  //Illustrious 425-500
  //Zen 500-600

  //Indexing:
  //[X][Y][Z]
  //X - profession ID (possible: 1, 2, 3, 4, 8, 9)
  //Y - ID of Profession Level - (Apprentice - 0, Journeyman - 1, etc)
  //Z - 0 - first reagent, 1 - amount of first reagent, 2 - 2nd reagent 3 - amount of 2nd reagent, 4 - 3rd reagent, 5 - amount of 3rd reagent
  //6 - minimal skill for this profession level, 7 - maximal skill
  //

  //BS
  //Apprentice
  g_iCraftProfLevels[1][0][0] = 184;  //Copper bar
  g_iCraftProfLevels[1][0][1] = 100;
  g_iCraftProfLevels[1][0][2] = 187;  //Bronze Bar
  g_iCraftProfLevels[1][0][3] = 100;  //100 of them
  g_iCraftProfLevels[1][0][4] = 0;  //No Third Reagent
  g_iCraftProfLevels[1][0][5] = 0;  //No third reagent
  g_iCraftProfLevels[1][0][6] = 0;  //Minimal Skill
  g_iCraftProfLevels[1][0][7] = 84; //Largest Skill

  //Journeyman
  g_iCraftProfLevels[1][1][0] = 188;  //Iron Bar
  g_iCraftProfLevels[1][1][1] = 200;
  g_iCraftProfLevels[1][1][2] = 0;  //No second reagent
  g_iCraftProfLevels[1][1][3] = 0;
  g_iCraftProfLevels[1][1][4] = 0;  //No Third Reagent
  g_iCraftProfLevels[1][1][5] = 0;
  g_iCraftProfLevels[1][1][6] = 85;  //Minimal Skill
  g_iCraftProfLevels[1][1][7] = 109; //Largest Skill

  //Expert
  g_iCraftProfLevels[1][2][0] = 190;  //Mithril Bar
  g_iCraftProfLevels[1][2][1] = 150;
  g_iCraftProfLevels[1][2][2] = 189;  //Gold Bar
  g_iCraftProfLevels[1][2][3] = 150;
  g_iCraftProfLevels[1][2][4] = 0;  //No Third Reagent
  g_iCraftProfLevels[1][2][5] = 0;  //No third reagent
  g_iCraftProfLevels[1][2][6] = 110;  //Minimal Skill
  g_iCraftProfLevels[1][2][7] = 149; //Largest Skill

  //Artisan
  g_iCraftProfLevels[1][3][0] = 192;  //Thorium Bar
  g_iCraftProfLevels[1][3][1] = 300;
  g_iCraftProfLevels[1][3][2] = 0;
  g_iCraftProfLevels[1][3][3] = 0;
  g_iCraftProfLevels[1][3][4] = 0;  //No Third Reagent
  g_iCraftProfLevels[1][3][5] = 0;  //No third reagent
  g_iCraftProfLevels[1][3][6] = 150;  //Minimal Skill
  g_iCraftProfLevels[1][3][7] = 179; //Largest Skill

  //Master
  g_iCraftProfLevels[1][4][0] = 194;  //Fel Iron Bar
  g_iCraftProfLevels[1][4][1] = 300;
  g_iCraftProfLevels[1][4][2] = 0;
  g_iCraftProfLevels[1][4][3] = 0;
  g_iCraftProfLevels[1][4][4] = 0;  //No Third Reagent
  g_iCraftProfLevels[1][4][5] = 0;  //No third reagent
  g_iCraftProfLevels[1][4][6] = 180;  //Minimal Skill
  g_iCraftProfLevels[1][4][7] = 209; //Largest Skill

  //Grand Master
  g_iCraftProfLevels[1][5][0] = 15006;  //Obsidium Bar
  g_iCraftProfLevels[1][5][1] = 200;
  g_iCraftProfLevels[1][5][2] = 0;
  g_iCraftProfLevels[1][5][3] = 0;
  g_iCraftProfLevels[1][5][4] = 0;  //No Third Reagent
  g_iCraftProfLevels[1][5][5] = 0;  //No third reagent
  g_iCraftProfLevels[1][5][6] = 210;  //Minimal Skill
  g_iCraftProfLevels[1][5][7] = 299; //Largest Skill

  //Illustrious
  g_iCraftProfLevels[1][6][0] = 15007;  //Elementium Bar
  g_iCraftProfLevels[1][6][1] = 100;
  g_iCraftProfLevels[1][6][2] = 15008; //Hardened Elementium
  g_iCraftProfLevels[1][6][3] = 100;
  g_iCraftProfLevels[1][6][4] = 0;
  g_iCraftProfLevels[1][6][5] = 0;
  g_iCraftProfLevels[1][6][6] = 300;  //Minimal Skill
  g_iCraftProfLevels[1][6][7] = 600; //Largest Skill


  //Leatherworking
  //Apprentice
  g_iCraftProfLevels[4][0][0] = 197;  //Light Leather
  g_iCraftProfLevels[4][0][1] = 100;
  g_iCraftProfLevels[4][0][2] = 200;  //Medium Leather
  g_iCraftProfLevels[4][0][3] = 100;
  g_iCraftProfLevels[4][0][4] = 0;
  g_iCraftProfLevels[4][0][5] = 0;
  g_iCraftProfLevels[4][0][6] = 0;  //Minimal Skill
  g_iCraftProfLevels[4][0][7] = 84; //Largest Skill

  //Journeyman
  g_iCraftProfLevels[4][1][0] = 201;  //Heavy Leather
  g_iCraftProfLevels[4][1][1] = 300;
  g_iCraftProfLevels[4][1][2] = 0;
  g_iCraftProfLevels[4][1][3] = 0;
  g_iCraftProfLevels[4][1][4] = 0;
  g_iCraftProfLevels[4][1][5] = 0;
  g_iCraftProfLevels[4][1][6] = 85;  //Minimal Skill
  g_iCraftProfLevels[4][1][7] = 109; //Largest Skill

  //Expert
  g_iCraftProfLevels[4][2][0] = 203;  //Thick Leather
  g_iCraftProfLevels[4][2][1] = 300;
  g_iCraftProfLevels[4][2][2] = 0;
  g_iCraftProfLevels[4][2][3] = 0;
  g_iCraftProfLevels[4][2][4] = 0;
  g_iCraftProfLevels[4][2][5] = 0;
  g_iCraftProfLevels[4][2][6] = 110;  //Minimal Skill
  g_iCraftProfLevels[4][2][7] = 144; //Largest Skill

  //Artisan
  g_iCraftProfLevels[4][3][0] = 205;  //Rugged Leather
  g_iCraftProfLevels[4][3][1] = 150;
  g_iCraftProfLevels[4][3][2] = 206; //Knothide Leather
  g_iCraftProfLevels[4][3][3] = 150;
  g_iCraftProfLevels[4][3][4] = 0;
  g_iCraftProfLevels[4][3][5] = 0;
  g_iCraftProfLevels[4][3][6] = 145;  //Minimal Skill
  g_iCraftProfLevels[4][3][7] = 179; //Largest Skill

  //Master
  g_iCraftProfLevels[4][4][0] = 208;  //Crystal Infused Leather
  g_iCraftProfLevels[4][4][1] = 300;
  g_iCraftProfLevels[4][4][2] = 0;
  g_iCraftProfLevels[4][4][3] = 0;
  g_iCraftProfLevels[4][4][4] = 0;
  g_iCraftProfLevels[4][4][5] = 0;
  g_iCraftProfLevels[4][4][6] = 180;  //Minimal Skill
  g_iCraftProfLevels[4][4][7] = 209; //Largest Skill

  //Grand Master
  g_iCraftProfLevels[4][5][0] = 15021;  //Savage Leather
  g_iCraftProfLevels[4][5][1] = 200;
  g_iCraftProfLevels[4][5][2] = 0;
  g_iCraftProfLevels[4][5][3] = 0;
  g_iCraftProfLevels[4][5][4] = 0;
  g_iCraftProfLevels[4][5][5] = 0;
  g_iCraftProfLevels[4][5][6] = 210;  //Minimal Skill
  g_iCraftProfLevels[4][5][7] = 299; //Largest Skill

  //Illustrious
  g_iCraftProfLevels[4][6][0] = 15022;  //Heavy Savage Leather
  g_iCraftProfLevels[4][6][1] = 100;
  g_iCraftProfLevels[4][6][2] = 15023; //Blackened Dragonscale
  g_iCraftProfLevels[4][6][3] = 100;
  g_iCraftProfLevels[4][6][4] = 0;
  g_iCraftProfLevels[4][6][5] = 0;
  g_iCraftProfLevels[4][6][6] = 300;  //Minimal Skill
  g_iCraftProfLevels[4][6][7] = 600; //Largest Skill


  //Enchanting
  //Apprentice
  g_iCraftProfLevels[8][0][0] = 6297;
  g_iCraftProfLevels[8][0][1] = 100;
  g_iCraftProfLevels[8][0][2] = 6304;
  g_iCraftProfLevels[8][0][3] = 100;
  g_iCraftProfLevels[8][0][4] = 0;
  g_iCraftProfLevels[8][0][5] = 0;
  g_iCraftProfLevels[8][0][6] = 0;  //Minimal Skill
  g_iCraftProfLevels[8][0][7] = 44; //Largest Skill

  //Journeyman
  g_iCraftProfLevels[8][1][0] = 6298;
  g_iCraftProfLevels[8][1][1] = 100;
  g_iCraftProfLevels[8][1][2] = 6305;
  g_iCraftProfLevels[8][1][3] = 100;
  g_iCraftProfLevels[8][1][4] = 0;
  g_iCraftProfLevels[8][1][5] = 0;
  g_iCraftProfLevels[8][1][6] = 45;  //Minimal Skill
  g_iCraftProfLevels[8][1][7] = 79; //Largest Skill

  //Expert
  g_iCraftProfLevels[8][2][0] = 6299;
  g_iCraftProfLevels[8][2][1] = 150;
  g_iCraftProfLevels[8][2][2] = 6306;
  g_iCraftProfLevels[8][2][3] = 150;
  g_iCraftProfLevels[8][2][4] = 0;
  g_iCraftProfLevels[8][2][5] = 0;
  g_iCraftProfLevels[8][2][6] = 80;  //Minimal Skill
  g_iCraftProfLevels[8][2][7] = 144; //Largest Skill

  //Artisan
  g_iCraftProfLevels[8][3][0] = 6300;
  g_iCraftProfLevels[8][3][1] = 120;
  g_iCraftProfLevels[8][3][2] = 6307;
  g_iCraftProfLevels[8][3][3] = 120;
  g_iCraftProfLevels[8][3][4] = 0;
  g_iCraftProfLevels[8][3][5] = 0;
  g_iCraftProfLevels[8][3][6] = 145;  //Minimal Skill
  g_iCraftProfLevels[8][3][7] = 209; //Largest Skill

  //Master
  g_iCraftProfLevels[8][4][0] = 6301;
  g_iCraftProfLevels[8][4][1] = 120;
  g_iCraftProfLevels[8][4][2] = 6307;
  g_iCraftProfLevels[8][4][3] = 120;
  g_iCraftProfLevels[8][4][4] = 0;
  g_iCraftProfLevels[8][4][5] = 0;
  g_iCraftProfLevels[8][4][6] = 210;  //Minimal Skill
  g_iCraftProfLevels[8][4][7] = 329; //Largest Skill

  //Grand Master
  g_iCraftProfLevels[8][5][0] = 15000;
  g_iCraftProfLevels[8][5][1] = 100;
  g_iCraftProfLevels[8][5][2] = 15001;
  g_iCraftProfLevels[8][5][3] = 100;
  g_iCraftProfLevels[8][5][4] = 0;
  g_iCraftProfLevels[8][5][5] = 0;
  g_iCraftProfLevels[8][5][6] = 330;  //Minimal Skill
  g_iCraftProfLevels[8][5][7] = 399; //Largest Skill

  //Illustrious
  g_iCraftProfLevels[8][6][0] = 6302;
  g_iCraftProfLevels[8][6][1] = 75;
  g_iCraftProfLevels[8][6][2] = 6309;
  g_iCraftProfLevels[8][6][3] = 75;
  g_iCraftProfLevels[8][6][4] = 0;
  g_iCraftProfLevels[8][6][5] = 0;
  g_iCraftProfLevels[8][6][6] = 400;  //Minimal Skill
  g_iCraftProfLevels[8][6][7] = 600; //Largest Skill


  //Inscription
  //Apprentice
  g_iCraftProfLevels[9][0][0] = 20000;
  g_iCraftProfLevels[9][0][1] = 100;
  g_iCraftProfLevels[9][0][2] = 20001;
  g_iCraftProfLevels[9][0][3] = 100;
  g_iCraftProfLevels[9][0][4] = 0;
  g_iCraftProfLevels[9][0][5] = 0;
  g_iCraftProfLevels[9][0][6] = 0;
  g_iCraftProfLevels[9][0][7] = 29;

  //Journeyman
  g_iCraftProfLevels[9][1][0] = 20002;
  g_iCraftProfLevels[9][1][1] = 100;
  g_iCraftProfLevels[9][1][2] = 20003;
  g_iCraftProfLevels[9][1][3] = 100;
  g_iCraftProfLevels[9][1][4] = 0;
  g_iCraftProfLevels[9][1][5] = 0;
  g_iCraftProfLevels[9][1][6] = 30;
  g_iCraftProfLevels[9][1][7] = 59;

  //Expert
  g_iCraftProfLevels[9][2][0] = 20004;
  g_iCraftProfLevels[9][2][1] = 200;
  g_iCraftProfLevels[9][2][2] = 0;
  g_iCraftProfLevels[9][2][3] = 0;
  g_iCraftProfLevels[9][2][4] = 0;
  g_iCraftProfLevels[9][2][5] = 0;
  g_iCraftProfLevels[9][2][6] = 60;
  g_iCraftProfLevels[9][2][7] = 74;

  //Artisan
  g_iCraftProfLevels[9][3][0] = 20007;
  g_iCraftProfLevels[9][3][1] = 40;
  g_iCraftProfLevels[9][3][2] = 0;
  g_iCraftProfLevels[9][3][3] = 0;
  g_iCraftProfLevels[9][3][4] = 0;
  g_iCraftProfLevels[9][3][5] = 0;
  g_iCraftProfLevels[9][3][6] = 75;
  g_iCraftProfLevels[9][3][7] = 89;

  //Master
  g_iCraftProfLevels[9][4][0] = 20008;
  g_iCraftProfLevels[9][4][1] = 100;
  g_iCraftProfLevels[9][4][2] = 0;
  g_iCraftProfLevels[9][4][3] = 0;
  g_iCraftProfLevels[9][4][4] = 0;
  g_iCraftProfLevels[9][4][5] = 0;
  g_iCraftProfLevels[9][4][6] = 90;
  g_iCraftProfLevels[9][4][7] = 104;

  //Grand Master
  g_iCraftProfLevels[9][5][0] = 20010;
  g_iCraftProfLevels[9][5][1] = 100;
  g_iCraftProfLevels[9][5][2] = 0;
  g_iCraftProfLevels[9][5][3] = 0;
  g_iCraftProfLevels[9][5][4] = 0;
  g_iCraftProfLevels[9][5][5] = 0;
  g_iCraftProfLevels[9][5][6] = 105;
  g_iCraftProfLevels[9][5][7] = 119;

  //Illustrious
  g_iCraftProfLevels[9][6][0] = 20011;
  g_iCraftProfLevels[9][6][1] = 100;
  g_iCraftProfLevels[9][6][2] = 0;
  g_iCraftProfLevels[9][6][3] = 0;
  g_iCraftProfLevels[9][6][4] = 0;
  g_iCraftProfLevels[9][6][5] = 0;
  g_iCraftProfLevels[9][6][6] = 120;
  g_iCraftProfLevels[9][6][7] = 600;

  //Jewelcrafting
  //NOTE: These might be a "bit" of a ripoff price, so they might need to be lowered, but I went with these as I'm not familiar with the current economic situations going on.
  //Apprentice
  g_iCraftProfLevels[2][0][0] = 184;
  g_iCraftProfLevels[2][0][1] = 40;
  g_iCraftProfLevels[2][0][2] = 187;
  g_iCraftProfLevels[2][0][3] = 40;
  g_iCraftProfLevels[2][0][4] = 185;
  g_iCraftProfLevels[2][0][5] = 20;
  g_iCraftProfLevels[2][0][6] = 0;
  g_iCraftProfLevels[2][0][7] = 49;

  //Journeyman
  g_iCraftProfLevels[2][1][0] = 188;
  g_iCraftProfLevels[2][1][1] = 70;
  g_iCraftProfLevels[2][1][2] = 172;
  g_iCraftProfLevels[2][1][3] = 20;
  g_iCraftProfLevels[2][1][4] = 186;
  g_iCraftProfLevels[2][1][5] = 20;
  g_iCraftProfLevels[2][1][6] = 50;
  g_iCraftProfLevels[2][1][7] = 74;

  //Expert
  g_iCraftProfLevels[2][2][0] = 189;
  g_iCraftProfLevels[2][2][1] = 50;
  g_iCraftProfLevels[2][2][2] = 190;
  g_iCraftProfLevels[2][2][3] = 50;
  g_iCraftProfLevels[2][2][4] = 192;
  g_iCraftProfLevels[2][2][5] = 50;
  g_iCraftProfLevels[2][2][6] = 75;
  g_iCraftProfLevels[2][2][7] = 94;

  //Artisan
  g_iCraftProfLevels[2][3][0] = 193;
  g_iCraftProfLevels[2][3][1] = 50;
  g_iCraftProfLevels[2][3][2] = 194;
  g_iCraftProfLevels[2][3][3] = 50;
  g_iCraftProfLevels[2][3][4] = 195;
  g_iCraftProfLevels[2][3][5] = 30;
  g_iCraftProfLevels[2][3][6] = 95;
  g_iCraftProfLevels[2][3][7] = 114;

  //Master
  g_iCraftProfLevels[2][4][0] = 196;
  g_iCraftProfLevels[2][4][1] = 30;
  g_iCraftProfLevels[2][4][2] = 180;
  g_iCraftProfLevels[2][4][3] = 30;
  g_iCraftProfLevels[2][4][4] = 195;
  g_iCraftProfLevels[2][4][5] = 80;
  g_iCraftProfLevels[2][4][6] = 115;
  g_iCraftProfLevels[2][4][7] = 124;

  //Grand Master
  g_iCraftProfLevels[2][5][0] = 15006;
  g_iCraftProfLevels[2][5][1] = 60;
  g_iCraftProfLevels[2][5][2] = 15007;
  g_iCraftProfLevels[2][5][3] = 60;
  g_iCraftProfLevels[2][5][4] = 0;
  g_iCraftProfLevels[2][5][5] = 0;
  g_iCraftProfLevels[2][5][6] = 125;
  g_iCraftProfLevels[2][5][7] = 219;

  //Illustrious
  g_iCraftProfLevels[2][6][0] = 15008;
  g_iCraftProfLevels[2][6][1] = 60;
  g_iCraftProfLevels[2][6][2] = 15009;
  g_iCraftProfLevels[2][6][3] = 20;
  g_iCraftProfLevels[2][6][4] = 15012;
  g_iCraftProfLevels[2][6][5] = 20;
  g_iCraftProfLevels[2][6][6] = 220;
  g_iCraftProfLevels[2][6][7] = 600;


  //Tailoring
  //Apprentice
  g_iCraftProfLevels[3][0][0] = 168;
  g_iCraftProfLevels[3][0][1] = 100;
  g_iCraftProfLevels[3][0][2] = 170;
  g_iCraftProfLevels[3][0][3] = 100;
  g_iCraftProfLevels[3][0][4] = 0;
  g_iCraftProfLevels[3][0][5] = 0;
  g_iCraftProfLevels[3][0][6] = 0;
  g_iCraftProfLevels[3][0][7] = 84;

  //Journeyman
  g_iCraftProfLevels[3][1][0] = 173;
  g_iCraftProfLevels[3][1][1] = 200;
  g_iCraftProfLevels[3][1][2] = 0;
  g_iCraftProfLevels[3][1][3] = 0;
  g_iCraftProfLevels[3][1][4] = 0;
  g_iCraftProfLevels[3][1][5] = 0;
  g_iCraftProfLevels[3][1][6] = 85;
  g_iCraftProfLevels[3][1][7] = 109;

  //Expert
  g_iCraftProfLevels[3][2][0] = 176;
  g_iCraftProfLevels[3][2][1] = 150;
  g_iCraftProfLevels[3][2][2] = 179;
  g_iCraftProfLevels[3][2][3] = 150;
  g_iCraftProfLevels[3][2][4] = 0;
  g_iCraftProfLevels[3][2][5] = 0;
  g_iCraftProfLevels[3][2][6] = 110;
  g_iCraftProfLevels[3][2][7] = 149;

  //Artisan
  g_iCraftProfLevels[3][3][0] = 181;
  g_iCraftProfLevels[3][3][1] = 300;
  g_iCraftProfLevels[3][3][2] = 0;
  g_iCraftProfLevels[3][3][3] = 0;
  g_iCraftProfLevels[3][3][4] = 0;
  g_iCraftProfLevels[3][3][5] = 0;
  g_iCraftProfLevels[3][3][6] = 150;
  g_iCraftProfLevels[3][3][7] = 179;

  //Master
  g_iCraftProfLevels[3][4][0] = 183;
  g_iCraftProfLevels[3][4][1] = 300;
  g_iCraftProfLevels[3][4][2] = 0;
  g_iCraftProfLevels[3][4][3] = 0;
  g_iCraftProfLevels[3][4][4] = 0;
  g_iCraftProfLevels[3][4][5] = 0;
  g_iCraftProfLevels[3][4][6] = 180;
  g_iCraftProfLevels[3][4][7] = 209;

  //Grand Master
  g_iCraftProfLevels[3][5][0] = 15019;
  g_iCraftProfLevels[3][5][1] = 200;
  g_iCraftProfLevels[3][5][2] = 0;
  g_iCraftProfLevels[3][5][3] = 0;
  g_iCraftProfLevels[3][5][4] = 0;
  g_iCraftProfLevels[3][5][5] = 0;
  g_iCraftProfLevels[3][5][6] = 210;
  g_iCraftProfLevels[3][5][7] = 299;

  //Illustrious
  g_iCraftProfLevels[3][6][0] = 15020;
  g_iCraftProfLevels[3][6][1] = 100;
  g_iCraftProfLevels[3][6][2] = 15017;
  g_iCraftProfLevels[3][6][3] = 100;
  g_iCraftProfLevels[3][6][4] = 0;
  g_iCraftProfLevels[3][6][5] = 0;
  g_iCraftProfLevels[3][6][6] = 300;
  g_iCraftProfLevels[3][6][7] = 600;


  //Silver Costs
  for(new p=0;p<10;p++) {
    g_iCraftProfLevels[p][0][8] = 100;
    g_iCraftProfLevels[p][1][8] = 500;
    g_iCraftProfLevels[p][2][8] = 2500;
    g_iCraftProfLevels[p][3][8] = 10000;
    g_iCraftProfLevels[p][4][8] = 70000;
    g_iCraftProfLevels[p][5][8] = 150000;
    g_iCraftProfLevels[p][6][8] = 1000000;//1 mil, still no Zen, so doesn't matter
  }

}

stock professionSkillToLevel(prof, skill) {
  for(new i=0;i<MAX_PROF_LEVELS;i++)  {
    new max = g_iCraftProfLevels[prof][i][7];
    if (max <= 0) {
      return i;
    }
    if(i == MAX_PROF_LEVELS - 1)  {
      return i;
    }
    if(skill <= max)  {
      return i;
    }
  }
  return 0; //actually should never get here
}

stock ProfessionLevelToString(level, String:buffer[], size, client=0)  {
  decl String:a[50];
  Format(a, 50, "profession_level_%d", level);
  Format(buffer, size, "%T", a, client);

}

stock GetClientProfessionLevel(client, slot)  {
  return professionSkillToLevel (GetProfession(client,slot), GetProfessionSk(client,slot));
}

stock GetClientProfessionLevelString(client, slot, String:buffer[], size) {
  new level = GetClientProfessionLevel(client, slot);
  ProfessionLevelToString(level, buffer, size, client);
}

stock GetProfessionLevelRequirements(profId, level, reagIds[], reagAmounts[]) {
  new amount = 0;
  for(new i=0; i<3; i++)  {
    new reagId = g_iCraftProfLevels[profId][level][i*2];
    new reagAmount = g_iCraftProfLevels[profId][level][i*2+1];
    if(reagId > 0 && reagAmount > 0)  {
      reagIds[amount] = reagId;
      reagAmounts[amount] = reagAmount;
      amount++;
    }
  }
  return amount;
}

stock GetProfessionLevelRequiredSilver(profId, level) {
  return g_iCraftProfLevels[profId][level][8];
}

stock bool:IsMaximumProfessionLevel(profId, level)  {
  if(level == MAX_PROF_LEVELS - 1)  {
    return true;
  }
  return g_iCraftProfLevels[profId][level+1][6] == 0;
}

stock IncreaseProfessionLevel(client, which, level) {
  new maximum = g_iCraftProfLevels[GetProfession(client,which)][level][7];
  decl String:profLevel[50],String:profName[50];
  ProfessionLevelToString(level, profLevel, sizeof(profLevel), client);
  ProfToName(GetProfession(client, which), profName, sizeof(profName), client);
  wcSetProfessionSk(client, which, maximum);
  WCMessage(client, "profession level increased", client, profName, profLevel);
}


stock GetProfessionMaximalSkill(profId, skill)  {
  new level = professionSkillToLevel(profId, skill);
  new maximal = g_iCraftProfLevels[profId][level][7];
  if(maximal < skill) return skill;
  return maximal;
}

stock GetProfessionLevelLowerBound(profId, level) {
  return g_iCraftProfLevels[profId][level][6];
}

stock GetProfessionLevelHigherBound(profId, level) {
  return g_iCraftProfLevels[profId][level][7];
}


stock bool:IsValidProfessionLevel(profId, level) {
  if(level > MAX_PROF_LEVELS - 1 || level < 0) {
    return false;
  }
  new maximal = g_iCraftProfLevels[profId][level][7];
  return maximal > 0;
}



new g_iCraftProfIds[]  = { 1, 2, 3, 4, 8, 9 };

new Profession:g_profAll[PROFESSION_BONUSES_AMOUNT] = { Prof_ExtraDrop, Prof_ExtraStats, Prof_ExtraDamage,
                  Prof_ExtraDef, Prof_ExtraRegen, Prof_ExtraSpeed,
                  Prof_EnchBonus2, Prof_LwBonus2, Prof_JcBonus2,
                  Prof_TailBonus2, Prof_BsBonus2,
                  Prof_InscrBonus, Prof_InscrBonus2};

new String:translations[Profession][100] = { "prof extra drop", "prof extra stats", "prof extra damage",
            "prof extra def", "prof extra regen", "prof extra speed",
            "prof extra ap spdmg sta", "prof extra agi", "prof extra jewel stats",
            "prof extra armor", "prof extra crit",
            "prof extra resilence", "prof extra haste" };

new bool:g_bProfBonuses[65][Profession];

InitProfBonuses(client) {
  for(new i=0;i<PROFESSION_BONUSES_AMOUNT;i++)
    g_bProfBonuses[client][i]=PlayerHasBonus(client, g_profAll[i]);
}

GetProfBonuses(Profession:profs[PROFESSION_BONUSES_AMOUNT])  {
  profs = g_profAll;

}

stock GetProfRequirements(Profession:type, &skill)  {
  switch(type)  {
    case Prof_ExtraDrop:    { skill=120;  return 0; } //All
    case Prof_ExtraDef:     { skill=160;  return 1; } //BS tier 1
    case Prof_BsBonus2:     { skill=300;  return 1; } //BS tier 2
    case Prof_ExtraSpeed:   { skill=100;  return 2; } //JC tier 1
    case Prof_JcBonus2:     { skill=210;  return 2; } //JC tier 2
    case Prof_ExtraRegen:   { skill=160;  return 3; } //Tailoring tier 1
    case Prof_TailBonus2:   { skill=300;  return 3; } //Tailoring tier 2
    case Prof_ExtraDamage:  { skill=160;  return 4; } //LW tier 1
    case Prof_LwBonus2:     { skill=300;  return 4; } //LW tier 2
    case Prof_ExtraStats:   { skill=100;  return 8; } //Ench tier 1
    case Prof_EnchBonus2:   { skill=220;  return 8; } //Ench tier 2
    case Prof_InscrBonus:   { skill=100;  return 9; } //Insc tier 1
    case Prof_InscrBonus2:  { skill=150;  return 9; } //Insc tier 2
  }
  return -1;
}


stock GetAllProfBonuses(client, Profession:bonuses[])    {
  new amount=0;
  for(new i=0;i<PROFESSION_BONUSES_AMOUNT;i++)  {
    new Profession:type = g_profAll[i];
    if(PlayerHasBonusFast(client, type))
      bonuses[amount++]=type;
  }
  return amount;
}

stock TranslateProfBonus(client, Profession:prof, String:back[], size)  {
  Format(back, size, "%T", translations[prof], client);
}

stock bool:PlayerHasBonusFast(client, Profession:type)  {
  return g_bProfBonuses[client][type];
}

//do not use, is private function
//use PlayerHasBonusFast instead
stock bool:PlayerHasBonus(client, Profession:type) {
  new playerProfs[2][2];  // first index  - prof index, 2nd index:
                          // 0 - profession id
                          // 1 - profession skill
  for(new i=0;i<2;i++)  {
    playerProfs[i][0]=GetProfession(client,i+1);
    playerProfs[i][1]=GetProfessionSk(client,i+1);
  }

  //Check if person has any crafting profession
  new bool:pass=false;
  for(new i=0;i<2;i++)  {
    new bool:isCrafting=false;
    new profid = playerProfs[i][0];
    for(new y=0;y<CRAFT_PROFS_AMOUNT;y++) {
      if(g_iCraftProfIds[y]==profid)  {
        isCrafting=true;
        pass=true;
        break;
      }
    }
    if(!isCrafting) {
      //Erase it's skill so that gathering profession doesn't
      //get bonuses which are for all crafting professions
      playerProfs[i][1]=0;
    }
  }
  if(!pass) //No crafting professions == no bonuses
    return false;

  new skill;
  new prof = GetProfRequirements(type, skill);
  if(prof==0) { //Don't check for prof id match
    for(new i=0;i<2;i++)
      if(playerProfs[i][1]>=skill)
        return true;
  }
  else  {
    for(new i=0;i<2;i++)
      if((playerProfs[i][0]==prof) && (playerProfs[i][1]>=skill))
        return true;
  }

  return false;

}

stock bool:isCraftingProfession(profid)  {
  new bool:bcrafting=false;
  for(new y=0;y<CRAFT_PROFS_AMOUNT;y++) {
    if(g_iCraftProfIds[y]==profid)  {
      bcrafting=true;
      break;
    }
  }
  return bcrafting;
}

