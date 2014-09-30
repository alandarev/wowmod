wowmod
======
wowmod is a [SourceMod] plugin developed for Counter-Strike:Source and Counter-Strike:Global Offensive games.
The plugin modifies the stated online shooter games to resemble to online RPG shooter.

## Features
Players are given an opportunity to become one of the nine classes available and grow stronger in endless amount of ways.

### Player

* Choose class to play (Warrior, Rogue, Mage, Paladin, Priest, Shaman, Warlock, Death Knight, Monk)
* Find, craft, win unique wearable gear
* Level up, gain new spells, use them on your foes or friends!
* Spend your talent points in a large talents tree to further make your character unique
* You can be supporter, damage dealer or even tank

### Environment

* Rich currency economy
* Auction houses for trading gear and goods
* Mailing system for more personal trade and deals

### Events

* Unite into a single team to fight bosses
* Answer the quiz questions and gain wowmod items or currency as rewards
* Play hide and seek, use spells of your class to hunt or stay undetected
* Treasure hunt. Yes, there are treasures you can fight for!

### Screenshots

![gameplay screenshot](http://wowmod.eu/images/deaztec0002j.jpg)
![gameplay screenshot2](http://wowmod.eu/images/2011052400002.jpg)

## Branches
* **Master** branch is for CS:S game
* **csgo** branch is for CS:GO game
* **development** branch is primarily used to develop future features

## Installation

### Dependencies
* [SourceMod]
* MySQL Server

### Compiling
You will need spcomp supplied by [SourceMod].

1. Copy library files from `./lib` to `path_to_spcomp/include/`
2. Open `./source` folder and run `spcomp wow.sp`
3. Run `spcomp visual.sp`, as it is required by wowmod plugin
*(rest plugins are optional)*

### Database
Prepare fresh database using `./db/womwod.sql` SQL file

### Files
1. Compiled `*.sp` become `*.smx`, and shall be placed to `SERVER/addons/sourcemod/plugins`.
2. File `./configs/wow/wow.ini` needs to be placed to `SERVER/addons/sourcemod/configs/`, do not forget to modify it.
3. Folder `./translations` is copied to `SERVER/addons/sourcemod` folder.
4. Copy folder `./data` to `SERVER/addons/sourcemod`
5. Copy content of `./data/effects/*` to `SERVER/addons/sourcemod/data`
6. Copy `./data/gamedata/*` to `SERVER/addons/sourcemod/gamedata/`
7. Copy `./data/items/*` to `SERVER/addons/sourcemod/data/`
8. Copy `./data/spells/*` to `SERVER/addons/sourcemod/data/`
8. Copy `./data/talents/*` to `SERVER/addons/sourcemod/data/`
8. Copy `./data/text/*` to `SERVER/addons/sourcemod/data/`
9. Copy `./translations/*` to `SERVER/addons/sourcemod/translations/`
10. Copy data files (mostly sounds) from `./data/heavy/*` to `SERVER`
11. (Optional) If you have HTTP download enabled, copy the same data files to your HTTP download server.

### Troubleshooting
If wowmod does not start, look into the error logs of sourcemod (`SERVER/addons/sourcemod/log/`), in most cases it accurately tells the reasons of failing to start the **wowmod** plugin.

## Further Information
[wowmod wiki] contains rich information on most features of the modification and is also used as a guide for players.

[wowmod forum] is a place where you can always reach our team, as well as players of the official wowmod server.

Write in CS:S game console `connect wowmod.eu` to try out the game live, on the official wowmod CS:S server.

## Contributing
Contributing is highly welcomed. The non-commercial project has grown too massive for a tiny team, so we are hoping to get more people involved as it is now public.

## Contributors
* alandarev
* Aurora
* Arioni
* Tristen
* Sirchipnsalsa
* Porta
* Koshyy
* upidstupid

## License
[GPLv3]


[GPLv3]:LICENSE
[SourceMod]:http://www.sourcemod.net
[wowmod wiki]:http://wiki.wowmod.eu
[wowmod forum]:http://wowmod.eu
