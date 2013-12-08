CREATE TABLE `wc_StaticItems` (
  `itemId` int(10) unsigned NOT NULL,
  `general_name` varchar(50) NOT NULL,
  `general_description` varchar(70) DEFAULT '',
  `general_level` int(4) NOT NULL DEFAULT '0',
  `general_cost` int(8) NOT NULL DEFAULT '-1',
  `general_gold` int(6),
  `general_honor` int(6),
  `general_quality` int(3) NOT NULL DEFAULT '0',
  `general_category` int(3) NOT NULL,
  `general_amount`  int(5) NOT NULL DEFAULT '1',
  `general_usable`  int(2),
  `general_notrade` int(2),
  `general_soulbound` int(2),
  `general_token`   int(10),
  `general_token_amount` int(6),

  `weapon_speed` DOUBLE,
  `weapon_attackPower` int(5),
  `weapon_spellPower` int(5),
  `weapon_twoHanded` int(2),

  `item_classes` int(10),
  `item_ilevel` int(4),
  `item_strength` int(5),
  `item_agility` int(5),
  `item_stamina` int(5),
  `item_intellect` int(5),
  `item_armorIgnore` int(5),
  `item_resistance` int(5),
  `item_armor` int(7),
  `item_spellDamage` int(5),
  `item_damage` int(5),
  `item_haste` int(5),
  `item_armorType` int(3),
  `item_effect` int(10),
  `item_attackSpeed` DOUBLE,
  `item_criticalStrike` DOUBLE,
  `item_movementSpeed` DOUBLE,

  `item_sound` int(5),

  `recipe_requiredSkill` int(4),
  `recipe_itemid` int(10),
  `recipe_grey` int(5),
  `recipe_green` int(5),
  `recipe_orange` int(5),

  `usable_message` VARCHAR(255),
  `usable_set`    int(5),
  `usable_category2` int(5),
  `usable_function` VARCHAR(50),
  `usable_args`     int(5),
  `usable_sets`   VARCHAR(255),



  PRIMARY KEY (`itemId`)
);
