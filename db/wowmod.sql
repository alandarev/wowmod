/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `wc_Auction`
--

DROP TABLE IF EXISTS `wc_Auction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Auction` (
  `Id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `fromId` int(15) NOT NULL DEFAULT '0',
  `time` datetime DEFAULT NULL,
  `bid` int(7) NOT NULL DEFAULT '0',
  `bidId` int(11) NOT NULL DEFAULT '0',
  `buyout` int(7) NOT NULL DEFAULT '0',
  `item` int(7) NOT NULL DEFAULT '0',
  `unique` int(7) NOT NULL DEFAULT '0',
  `category` int(3) NOT NULL DEFAULT '0',
  `quality` int(3) NOT NULL DEFAULT '0',
  `fromname` char(20) COLLATE utf8_unicode_ci DEFAULT 'Administration',
  `amount` int(3) NOT NULL DEFAULT '0',
  `gold` int(10) NOT NULL DEFAULT '0',
  `goldBuyout` int(10) NOT NULL DEFAULT '0',
  `class` int(11) DEFAULT '0',
  `hExtend` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_CItems`
--

DROP TABLE IF EXISTS `wc_CItems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_CItems` (
  `CharacterId` int(11) unsigned NOT NULL,
  `citem_0` int(7) NOT NULL DEFAULT '0',
  `citem_1` int(7) NOT NULL DEFAULT '0',
  `citem_2` int(7) NOT NULL DEFAULT '0',
  `citem_3` int(7) NOT NULL DEFAULT '0',
  `citem_4` int(7) NOT NULL DEFAULT '0',
  `citem_5` int(7) NOT NULL DEFAULT '0',
  `citem_6` int(7) NOT NULL DEFAULT '0',
  `citem_7` int(7) NOT NULL DEFAULT '0',
  `citem_8` int(7) NOT NULL DEFAULT '0',
  `citem_9` int(7) NOT NULL DEFAULT '0',
  `citem_10` int(7) NOT NULL DEFAULT '0',
  `citem_11` int(7) NOT NULL DEFAULT '0',
  `citem_12` int(7) NOT NULL DEFAULT '0',
  `citem_13` int(7) NOT NULL DEFAULT '0',
  `u_item_1` int(7) NOT NULL DEFAULT '0',
  `u_item_2` int(7) NOT NULL DEFAULT '0',
  `u_item_3` int(7) NOT NULL DEFAULT '0',
  `u_item_4` int(7) NOT NULL DEFAULT '0',
  `u_item_5` int(7) NOT NULL DEFAULT '0',
  `u_item_6` int(7) NOT NULL DEFAULT '0',
  `u_item_7` int(7) NOT NULL DEFAULT '0',
  `u_item_8` int(7) NOT NULL DEFAULT '0',
  `u_item_9` int(7) NOT NULL DEFAULT '0',
  `u_item_10` int(7) NOT NULL DEFAULT '0',
  `u_item_11` int(7) NOT NULL DEFAULT '0',
  `u_item_12` int(7) NOT NULL DEFAULT '0',
  `u_item_13` int(7) NOT NULL DEFAULT '0',
  `u_item_14` int(7) NOT NULL DEFAULT '0',
  `u_item_15` int(7) NOT NULL DEFAULT '0',
  `u_item_16` int(7) NOT NULL DEFAULT '0',
  `u_item_17` int(7) NOT NULL DEFAULT '0',
  `u_item_18` int(7) NOT NULL DEFAULT '0',
  `u_item_19` int(7) NOT NULL DEFAULT '0',
  `u_item_20` int(7) NOT NULL DEFAULT '0',
  `u_item_21` int(7) NOT NULL DEFAULT '0',
  `u_item_22` int(7) NOT NULL DEFAULT '0',
  `u_item_23` int(7) NOT NULL DEFAULT '0',
  `u_item_24` int(7) NOT NULL DEFAULT '0',
  `u_item_25` int(7) NOT NULL DEFAULT '0',
  `u_item_26` int(7) NOT NULL DEFAULT '0',
  `u_item_27` int(7) NOT NULL DEFAULT '0',
  `u_item_28` int(7) NOT NULL DEFAULT '0',
  `u_item_29` int(7) NOT NULL DEFAULT '0',
  `u_item_30` int(7) NOT NULL DEFAULT '0',
  `u_item_31` int(7) NOT NULL DEFAULT '0',
  `u_item_32` int(7) NOT NULL DEFAULT '0',
  `u_item_33` int(7) NOT NULL DEFAULT '0',
  `u_item_34` int(7) NOT NULL DEFAULT '0',
  `u_item_35` int(7) NOT NULL DEFAULT '0',
  `u_item_36` int(7) NOT NULL DEFAULT '0',
  `u_item_37` int(7) NOT NULL DEFAULT '0',
  `u_item_38` int(7) NOT NULL DEFAULT '0',
  `u_item_39` int(7) NOT NULL DEFAULT '0',
  `u_item_40` int(7) NOT NULL DEFAULT '0',
  `u_item_41` int(7) NOT NULL DEFAULT '0',
  `u_item_42` int(7) NOT NULL DEFAULT '0',
  `u_item_43` int(7) NOT NULL DEFAULT '0',
  `u_item_44` int(7) NOT NULL DEFAULT '0',
  `u_item_45` int(7) NOT NULL DEFAULT '0',
  `u_item_46` int(7) NOT NULL DEFAULT '0',
  `u_item_47` int(7) NOT NULL DEFAULT '0',
  `u_item_48` int(7) NOT NULL DEFAULT '0',
  `u_item_49` int(7) NOT NULL DEFAULT '0',
  `u_item_50` int(7) NOT NULL DEFAULT '0',
  `u_item_51` int(7) NOT NULL DEFAULT '0',
  `u_item_52` int(7) NOT NULL DEFAULT '0',
  `u_item_53` int(7) NOT NULL DEFAULT '0',
  `u_item_54` int(7) NOT NULL DEFAULT '0',
  `u_item_55` int(7) NOT NULL DEFAULT '0',
  `u_item_56` int(7) NOT NULL DEFAULT '0',
  `u_item_57` int(7) NOT NULL DEFAULT '0',
  `u_item_58` int(7) NOT NULL DEFAULT '0',
  `u_item_59` int(7) NOT NULL DEFAULT '0',
  `u_item_60` int(7) NOT NULL DEFAULT '0',
  `u_item_61` int(7) NOT NULL DEFAULT '0',
  `u_item_62` int(7) NOT NULL DEFAULT '0',
  `u_item_63` int(7) NOT NULL DEFAULT '0',
  `u_item_64` int(7) NOT NULL DEFAULT '0',
  `u_item_65` int(7) NOT NULL DEFAULT '0',
  `u_item_66` int(7) NOT NULL DEFAULT '0',
  `u_item_67` int(7) NOT NULL DEFAULT '0',
  `u_item_68` int(7) NOT NULL DEFAULT '0',
  `u_item_69` int(7) NOT NULL DEFAULT '0',
  `u_item_70` int(7) NOT NULL DEFAULT '0',
  `u_item_71` int(7) NOT NULL DEFAULT '0',
  `u_item_72` int(7) NOT NULL DEFAULT '0',
  `u_item_73` int(7) NOT NULL DEFAULT '0',
  `u_item_74` int(7) NOT NULL DEFAULT '0',
  `u_item_75` int(7) NOT NULL DEFAULT '0',
  `u_item_76` int(7) NOT NULL DEFAULT '0',
  `u_item_77` int(7) NOT NULL DEFAULT '0',
  `u_item_78` int(7) NOT NULL DEFAULT '0',
  `u_item_79` int(7) NOT NULL DEFAULT '0',
  `u_item_80` int(7) NOT NULL DEFAULT '0',
  `u_item_81` int(7) NOT NULL DEFAULT '0',
  `u_item_82` int(7) NOT NULL DEFAULT '0',
  `u_item_83` int(7) NOT NULL DEFAULT '0',
  `u_item_84` int(7) NOT NULL DEFAULT '0',
  `u_item_85` int(7) NOT NULL DEFAULT '0',
  `u_item_86` int(7) NOT NULL DEFAULT '0',
  `u_item_87` int(7) NOT NULL DEFAULT '0',
  `u_item_88` int(7) NOT NULL DEFAULT '0',
  `u_item_89` int(7) NOT NULL DEFAULT '0',
  `u_item_90` int(7) NOT NULL DEFAULT '0',
  `u_item_91` int(7) NOT NULL DEFAULT '0',
  `u_item_92` int(7) NOT NULL DEFAULT '0',
  `u_item_93` int(7) NOT NULL DEFAULT '0',
  `u_item_94` int(7) NOT NULL DEFAULT '0',
  `u_item_95` int(7) NOT NULL DEFAULT '0',
  `u_item_96` int(7) NOT NULL DEFAULT '0',
  `u_item_97` int(7) NOT NULL DEFAULT '0',
  `u_item_98` int(7) NOT NULL DEFAULT '0',
  `u_item_99` int(7) NOT NULL DEFAULT '0',
  `u_item_100` int(7) NOT NULL DEFAULT '0',
  PRIMARY KEY (`CharacterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_CharacterInventory`
--

DROP TABLE IF EXISTS `wc_CharacterInventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_CharacterInventory` (
  `CharacterId` int(11) unsigned NOT NULL,
  `item_1` int(10) unsigned NOT NULL DEFAULT '0',
  `item_2` int(10) unsigned NOT NULL DEFAULT '0',
  `item_3` int(10) unsigned NOT NULL DEFAULT '0',
  `item_4` int(10) unsigned NOT NULL DEFAULT '0',
  `item_5` int(10) unsigned NOT NULL DEFAULT '0',
  `item_6` int(10) unsigned NOT NULL DEFAULT '0',
  `item_7` int(10) unsigned NOT NULL DEFAULT '0',
  `item_8` int(10) unsigned NOT NULL DEFAULT '0',
  `item_9` int(10) unsigned NOT NULL DEFAULT '0',
  `item_10` int(10) unsigned NOT NULL DEFAULT '0',
  `item_11` int(10) unsigned NOT NULL DEFAULT '0',
  `item_12` int(10) unsigned NOT NULL DEFAULT '0',
  `item_13` int(10) unsigned NOT NULL DEFAULT '0',
  `item_14` int(10) unsigned NOT NULL DEFAULT '0',
  `item_15` int(10) unsigned NOT NULL DEFAULT '0',
  `item_16` int(10) unsigned NOT NULL DEFAULT '0',
  `item_17` int(10) unsigned NOT NULL DEFAULT '0',
  `item_18` int(10) unsigned NOT NULL DEFAULT '0',
  `item_19` int(10) unsigned NOT NULL DEFAULT '0',
  `item_20` int(10) unsigned NOT NULL DEFAULT '0',
  `item_21` int(10) unsigned NOT NULL DEFAULT '0',
  `item_22` int(10) unsigned NOT NULL DEFAULT '0',
  `item_23` int(10) unsigned NOT NULL DEFAULT '0',
  `item_24` int(10) unsigned NOT NULL DEFAULT '0',
  `item_25` int(10) unsigned NOT NULL DEFAULT '0',
  `item_26` int(10) unsigned NOT NULL DEFAULT '0',
  `item_27` int(10) unsigned NOT NULL DEFAULT '0',
  `item_28` int(10) unsigned NOT NULL DEFAULT '0',
  `item_29` int(10) unsigned NOT NULL DEFAULT '0',
  `item_30` int(10) unsigned NOT NULL DEFAULT '0',
  `item_31` int(10) unsigned NOT NULL DEFAULT '0',
  `item_32` int(10) unsigned NOT NULL DEFAULT '0',
  `item_33` int(10) unsigned NOT NULL DEFAULT '0',
  `item_34` int(10) unsigned NOT NULL DEFAULT '0',
  `item_35` int(10) unsigned NOT NULL DEFAULT '0',
  `item_36` int(10) unsigned NOT NULL DEFAULT '0',
  `item_37` int(10) unsigned NOT NULL DEFAULT '0',
  `item_38` int(10) unsigned NOT NULL DEFAULT '0',
  `item_39` int(10) unsigned NOT NULL DEFAULT '0',
  `item_40` int(10) unsigned NOT NULL DEFAULT '0',
  `item_41` int(10) unsigned NOT NULL DEFAULT '0',
  `item_42` int(10) unsigned NOT NULL DEFAULT '0',
  `item_43` int(10) unsigned NOT NULL DEFAULT '0',
  `item_44` int(10) unsigned NOT NULL DEFAULT '0',
  `item_45` int(10) unsigned NOT NULL DEFAULT '0',
  `item_46` int(10) unsigned NOT NULL DEFAULT '0',
  `item_47` int(10) unsigned NOT NULL DEFAULT '0',
  `item_48` int(10) unsigned NOT NULL DEFAULT '0',
  `item_49` int(10) unsigned NOT NULL DEFAULT '0',
  `item_50` int(10) unsigned NOT NULL DEFAULT '0',
  `item_51` int(10) unsigned NOT NULL DEFAULT '0',
  `item_52` int(10) unsigned NOT NULL DEFAULT '0',
  `item_53` int(10) unsigned NOT NULL DEFAULT '0',
  `item_54` int(10) unsigned NOT NULL DEFAULT '0',
  `item_55` int(10) unsigned NOT NULL DEFAULT '0',
  `item_56` int(10) unsigned NOT NULL DEFAULT '0',
  `item_57` int(10) unsigned NOT NULL DEFAULT '0',
  `item_58` int(10) unsigned NOT NULL DEFAULT '0',
  `item_59` int(10) unsigned NOT NULL DEFAULT '0',
  `item_60` int(10) unsigned NOT NULL DEFAULT '0',
  `item_61` int(10) unsigned NOT NULL DEFAULT '0',
  `item_62` int(10) unsigned NOT NULL DEFAULT '0',
  `item_63` int(10) unsigned NOT NULL DEFAULT '0',
  `item_64` int(10) unsigned NOT NULL DEFAULT '0',
  `item_65` int(10) unsigned NOT NULL DEFAULT '0',
  `item_66` int(10) unsigned NOT NULL DEFAULT '0',
  `item_67` int(10) unsigned NOT NULL DEFAULT '0',
  `item_68` int(10) unsigned NOT NULL DEFAULT '0',
  `item_69` int(10) unsigned NOT NULL DEFAULT '0',
  `item_70` int(10) unsigned NOT NULL DEFAULT '0',
  `item_71` int(10) unsigned NOT NULL DEFAULT '0',
  `item_72` int(10) unsigned NOT NULL DEFAULT '0',
  `item_73` int(10) unsigned NOT NULL DEFAULT '0',
  `item_74` int(10) unsigned NOT NULL DEFAULT '0',
  `item_75` int(10) unsigned NOT NULL DEFAULT '0',
  `item_76` int(10) unsigned NOT NULL DEFAULT '0',
  `item_77` int(10) unsigned NOT NULL DEFAULT '0',
  `item_78` int(10) unsigned NOT NULL DEFAULT '0',
  `item_79` int(10) unsigned NOT NULL DEFAULT '0',
  `item_80` int(10) unsigned NOT NULL DEFAULT '0',
  `item_81` int(10) unsigned NOT NULL DEFAULT '0',
  `item_82` int(10) unsigned NOT NULL DEFAULT '0',
  `item_83` int(10) unsigned NOT NULL DEFAULT '0',
  `item_84` int(10) unsigned NOT NULL DEFAULT '0',
  `item_85` int(10) unsigned NOT NULL DEFAULT '0',
  `item_86` int(10) unsigned NOT NULL DEFAULT '0',
  `item_87` int(10) unsigned NOT NULL DEFAULT '0',
  `item_88` int(10) unsigned NOT NULL DEFAULT '0',
  `item_89` int(10) unsigned NOT NULL DEFAULT '0',
  `item_90` int(10) unsigned NOT NULL DEFAULT '0',
  `item_91` int(10) unsigned NOT NULL DEFAULT '0',
  `item_92` int(10) unsigned NOT NULL DEFAULT '0',
  `item_93` int(10) unsigned NOT NULL DEFAULT '0',
  `item_94` int(10) unsigned NOT NULL DEFAULT '0',
  `item_95` int(10) unsigned NOT NULL DEFAULT '0',
  `item_96` int(10) unsigned NOT NULL DEFAULT '0',
  `item_97` int(10) unsigned NOT NULL DEFAULT '0',
  `item_98` int(10) unsigned NOT NULL DEFAULT '0',
  `item_99` int(10) unsigned NOT NULL DEFAULT '0',
  `item_100` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`CharacterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_CharacterInventory2`
--

DROP TABLE IF EXISTS `wc_CharacterInventory2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_CharacterInventory2` (
  `CharacterId` int(11) unsigned NOT NULL,
  `amount_item_1` int(3) NOT NULL DEFAULT '0',
  `amount_item_2` int(3) NOT NULL DEFAULT '0',
  `amount_item_3` int(3) NOT NULL DEFAULT '0',
  `amount_item_4` int(3) NOT NULL DEFAULT '0',
  `amount_item_5` int(3) NOT NULL DEFAULT '0',
  `amount_item_6` int(3) NOT NULL DEFAULT '0',
  `amount_item_7` int(3) NOT NULL DEFAULT '0',
  `amount_item_8` int(3) NOT NULL DEFAULT '0',
  `amount_item_9` int(3) NOT NULL DEFAULT '0',
  `amount_item_10` int(3) NOT NULL DEFAULT '0',
  `amount_item_11` int(3) NOT NULL DEFAULT '0',
  `amount_item_12` int(3) NOT NULL DEFAULT '0',
  `amount_item_13` int(3) NOT NULL DEFAULT '0',
  `amount_item_14` int(3) NOT NULL DEFAULT '0',
  `amount_item_15` int(3) NOT NULL DEFAULT '0',
  `amount_item_16` int(3) NOT NULL DEFAULT '0',
  `amount_item_17` int(3) NOT NULL DEFAULT '0',
  `amount_item_18` int(3) NOT NULL DEFAULT '0',
  `amount_item_19` int(3) NOT NULL DEFAULT '0',
  `amount_item_20` int(3) NOT NULL DEFAULT '0',
  `amount_item_21` int(3) NOT NULL DEFAULT '0',
  `amount_item_22` int(3) NOT NULL DEFAULT '0',
  `amount_item_23` int(3) NOT NULL DEFAULT '0',
  `amount_item_24` int(3) NOT NULL DEFAULT '0',
  `amount_item_25` int(3) NOT NULL DEFAULT '0',
  `amount_item_26` int(3) NOT NULL DEFAULT '0',
  `amount_item_27` int(3) NOT NULL DEFAULT '0',
  `amount_item_28` int(3) NOT NULL DEFAULT '0',
  `amount_item_29` int(3) NOT NULL DEFAULT '0',
  `amount_item_30` int(3) NOT NULL DEFAULT '0',
  `amount_item_31` int(3) NOT NULL DEFAULT '0',
  `amount_item_32` int(3) NOT NULL DEFAULT '0',
  `amount_item_33` int(3) NOT NULL DEFAULT '0',
  `amount_item_34` int(3) NOT NULL DEFAULT '0',
  `amount_item_35` int(3) NOT NULL DEFAULT '0',
  `amount_item_36` int(3) NOT NULL DEFAULT '0',
  `amount_item_37` int(3) NOT NULL DEFAULT '0',
  `amount_item_38` int(3) NOT NULL DEFAULT '0',
  `amount_item_39` int(3) NOT NULL DEFAULT '0',
  `amount_item_40` int(3) NOT NULL DEFAULT '0',
  `amount_item_41` int(3) NOT NULL DEFAULT '0',
  `amount_item_42` int(3) NOT NULL DEFAULT '0',
  `amount_item_43` int(3) NOT NULL DEFAULT '0',
  `amount_item_44` int(3) NOT NULL DEFAULT '0',
  `amount_item_45` int(3) NOT NULL DEFAULT '0',
  `amount_item_46` int(3) NOT NULL DEFAULT '0',
  `amount_item_47` int(3) NOT NULL DEFAULT '0',
  `amount_item_48` int(3) NOT NULL DEFAULT '0',
  `amount_item_49` int(3) NOT NULL DEFAULT '0',
  `amount_item_50` int(3) NOT NULL DEFAULT '0',
  `amount_item_51` int(3) NOT NULL DEFAULT '0',
  `amount_item_52` int(3) NOT NULL DEFAULT '0',
  `amount_item_53` int(3) NOT NULL DEFAULT '0',
  `amount_item_54` int(3) NOT NULL DEFAULT '0',
  `amount_item_55` int(3) NOT NULL DEFAULT '0',
  `amount_item_56` int(3) NOT NULL DEFAULT '0',
  `amount_item_57` int(3) NOT NULL DEFAULT '0',
  `amount_item_58` int(3) NOT NULL DEFAULT '0',
  `amount_item_59` int(3) NOT NULL DEFAULT '0',
  `amount_item_60` int(3) NOT NULL DEFAULT '0',
  `amount_item_61` int(3) NOT NULL DEFAULT '0',
  `amount_item_62` int(3) NOT NULL DEFAULT '0',
  `amount_item_63` int(3) NOT NULL DEFAULT '0',
  `amount_item_64` int(3) NOT NULL DEFAULT '0',
  `amount_item_65` int(3) NOT NULL DEFAULT '0',
  `amount_item_66` int(3) NOT NULL DEFAULT '0',
  `amount_item_67` int(3) NOT NULL DEFAULT '0',
  `amount_item_68` int(3) NOT NULL DEFAULT '0',
  `amount_item_69` int(3) NOT NULL DEFAULT '0',
  `amount_item_70` int(3) NOT NULL DEFAULT '0',
  `amount_item_71` int(3) NOT NULL DEFAULT '0',
  `amount_item_72` int(3) NOT NULL DEFAULT '0',
  `amount_item_73` int(3) NOT NULL DEFAULT '0',
  `amount_item_74` int(3) NOT NULL DEFAULT '0',
  `amount_item_75` int(3) NOT NULL DEFAULT '0',
  `amount_item_76` int(3) NOT NULL DEFAULT '0',
  `amount_item_77` int(3) NOT NULL DEFAULT '0',
  `amount_item_78` int(3) NOT NULL DEFAULT '0',
  `amount_item_79` int(3) NOT NULL DEFAULT '0',
  `amount_item_80` int(3) NOT NULL DEFAULT '0',
  `amount_item_81` int(3) NOT NULL DEFAULT '0',
  `amount_item_82` int(3) NOT NULL DEFAULT '0',
  `amount_item_83` int(3) NOT NULL DEFAULT '0',
  `amount_item_84` int(3) NOT NULL DEFAULT '0',
  `amount_item_85` int(3) NOT NULL DEFAULT '0',
  `amount_item_86` int(3) NOT NULL DEFAULT '0',
  `amount_item_87` int(3) NOT NULL DEFAULT '0',
  `amount_item_88` int(3) NOT NULL DEFAULT '0',
  `amount_item_89` int(3) NOT NULL DEFAULT '0',
  `amount_item_90` int(3) NOT NULL DEFAULT '0',
  `amount_item_91` int(3) NOT NULL DEFAULT '0',
  `amount_item_92` int(3) NOT NULL DEFAULT '0',
  `amount_item_93` int(3) NOT NULL DEFAULT '0',
  `amount_item_94` int(3) NOT NULL DEFAULT '0',
  `amount_item_95` int(3) NOT NULL DEFAULT '0',
  `amount_item_96` int(3) NOT NULL DEFAULT '0',
  `amount_item_97` int(3) NOT NULL DEFAULT '0',
  `amount_item_98` int(3) NOT NULL DEFAULT '0',
  `amount_item_99` int(3) NOT NULL DEFAULT '0',
  `amount_item_100` int(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`CharacterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_CharacterMail`
--

DROP TABLE IF EXISTS `wc_CharacterMail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_CharacterMail` (
  `CharacterId` int(11) unsigned NOT NULL,
  `mail_1` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_2` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_3` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_4` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_5` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_6` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_7` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_8` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_9` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_10` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_11` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_12` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_13` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_14` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_15` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_16` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_17` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_18` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_19` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_20` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_21` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_22` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_23` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_24` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_25` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_26` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_27` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_28` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_29` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_30` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_31` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_32` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_33` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_34` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_35` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_36` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_37` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_38` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_39` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_40` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_41` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_42` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_43` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_44` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_45` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_46` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_47` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_48` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_49` int(10) unsigned NOT NULL DEFAULT '0',
  `mail_50` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`CharacterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_CharacterSpellbar`
--

DROP TABLE IF EXISTS `wc_CharacterSpellbar`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_CharacterSpellbar` (
  `CharacterId` int(11) unsigned NOT NULL,
  `spellbar_1` int(2) unsigned NOT NULL DEFAULT '0',
  `spellbar_2` int(2) unsigned NOT NULL DEFAULT '0',
  `spellbar_3` int(2) unsigned NOT NULL DEFAULT '0',
  `spellbar_4` int(2) unsigned NOT NULL DEFAULT '0',
  `spellbar_5` int(2) unsigned NOT NULL DEFAULT '0',
  `spellbar_6` int(2) unsigned NOT NULL DEFAULT '0',
  `spellbar_7` int(2) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`CharacterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_CharacterSpells`
--

DROP TABLE IF EXISTS `wc_CharacterSpells`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_CharacterSpells` (
  `CharacterId` int(11) unsigned NOT NULL,
  `spell_1` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_2` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_3` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_4` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_5` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_6` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_7` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_8` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_9` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_10` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_11` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_12` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_13` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_14` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_15` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_16` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_17` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_18` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_19` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_20` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_21` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_22` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_23` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_24` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_25` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_26` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_27` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_28` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_29` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_30` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_31` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_32` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_33` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_34` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_35` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_36` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_37` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_38` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_39` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_40` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_41` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_42` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_43` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_44` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_45` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_46` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_47` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_48` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_49` int(2) unsigned NOT NULL DEFAULT '0',
  `spell_50` int(2) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`CharacterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_Characters`
--

DROP TABLE IF EXISTS `wc_Characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Characters` (
  `CharacterId` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `class` int(2) unsigned NOT NULL DEFAULT '0',
  `points` int(5) unsigned NOT NULL DEFAULT '20',
  `level` int(5) unsigned NOT NULL DEFAULT '1',
  `xp` int(11) unsigned NOT NULL DEFAULT '0',
  `reqxp` int(11) unsigned NOT NULL DEFAULT '50',
  `money` int(10) unsigned NOT NULL DEFAULT '0',
  `str` int(5) unsigned NOT NULL DEFAULT '1',
  `agi` int(5) unsigned NOT NULL DEFAULT '1',
  `inte` int(5) unsigned NOT NULL DEFAULT '1',
  `sta` int(5) unsigned NOT NULL DEFAULT '1',
  `backs` int(10) unsigned NOT NULL DEFAULT '0',
  `chests` int(10) unsigned NOT NULL DEFAULT '0',
  `feet` int(10) unsigned NOT NULL DEFAULT '0',
  `hands` int(10) unsigned NOT NULL DEFAULT '0',
  `helmets` int(10) unsigned NOT NULL DEFAULT '0',
  `legs` int(10) unsigned NOT NULL DEFAULT '0',
  `necks` int(10) unsigned NOT NULL DEFAULT '0',
  `wrists` int(10) unsigned NOT NULL DEFAULT '0',
  `leftweapons` int(10) unsigned NOT NULL DEFAULT '0',
  `rightweapons` int(10) unsigned NOT NULL DEFAULT '0',
  `waists` int(10) unsigned NOT NULL DEFAULT '0',
  `trinkets` int(10) unsigned NOT NULL DEFAULT '0',
  `shoulders` int(10) unsigned NOT NULL DEFAULT '0',
  `rings` int(10) unsigned NOT NULL DEFAULT '0',
  `profession1` int(2) unsigned NOT NULL DEFAULT '0',
  `profession2` int(2) unsigned NOT NULL DEFAULT '0',
  `profession2sk` int(5) unsigned NOT NULL DEFAULT '0',
  `profession1sk` int(5) unsigned NOT NULL DEFAULT '0',
  `playerId` int(10) unsigned NOT NULL DEFAULT '0',
  `hide` int(3) unsigned NOT NULL DEFAULT '0',
  `rating` float(9,2) unsigned NOT NULL DEFAULT '5.00',
  `hp` int(6) unsigned NOT NULL DEFAULT '100',
  `mn` int(6) unsigned NOT NULL DEFAULT '5',
  `armor` int(8) unsigned NOT NULL DEFAULT '0',
  `dps` int(4) unsigned NOT NULL DEFAULT '0',
  `res` int(4) unsigned NOT NULL DEFAULT '0',
  `sdmg` int(4) unsigned NOT NULL DEFAULT '0',
  `rank` varchar(25) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'Copral',
  `talents` varchar(101) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `talentpoints` int(3) NOT NULL DEFAULT '0',
  `talents2` varchar(101) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `talentpoints2` int(3) NOT NULL DEFAULT '0',
  `spec` int(2) NOT NULL DEFAULT '0',
  `glyphs` smallint(6) DEFAULT '2',
  `glyph_1` int(11) DEFAULT '0',
  `glyph_0` int(11) DEFAULT '0',
  `glyph_2` int(11) DEFAULT '0',
  `dropped` int(11) DEFAULT '0',
  `autocast` int(11) DEFAULT '0',
  PRIMARY KEY (`CharacterId`)
) ENGINE=MyISAM AUTO_INCREMENT=1000 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_Chat`
--

DROP TABLE IF EXISTS `wc_Chat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Chat` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `servId` int(5) NOT NULL DEFAULT '0',
  `message` char(200) COLLATE utf8_unicode_ci NOT NULL DEFAULT '<no message>',
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `servId` (`servId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_Config`
--

DROP TABLE IF EXISTS `wc_Config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Config` (
  `variable` char(50) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'unknown',
  `value` char(15) COLLATE utf8_unicode_ci DEFAULT '0',
  PRIMARY KEY (`variable`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_Items`
--

DROP TABLE IF EXISTS `wc_Items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Items` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `defId` int(7) NOT NULL DEFAULT '-1',
  `version` int(5) NOT NULL DEFAULT '-1',
  `class` int(3) NOT NULL DEFAULT '-1',
  `level` int(4) NOT NULL DEFAULT '-1',
  `ilevel` smallint(6) DEFAULT '-1',
  `cost` int(8) NOT NULL DEFAULT '-1',
  `quality` int(3) NOT NULL DEFAULT '-1',
  `category` int(3) NOT NULL DEFAULT '-1',
  `reqstr` int(4) NOT NULL DEFAULT '-1',
  `reqagi` int(4) NOT NULL DEFAULT '-1',
  `reqsta` int(4) NOT NULL DEFAULT '-1',
  `reqint` int(4) NOT NULL DEFAULT '-1',
  `str` int(4) NOT NULL DEFAULT '-100',
  `agi` int(4) NOT NULL DEFAULT '-100',
  `sta` int(4) NOT NULL DEFAULT '-100',
  `inte` int(4) NOT NULL DEFAULT '-100',
  `res` int(4) NOT NULL DEFAULT '-100',
  `regen` int(4) NOT NULL DEFAULT '-100',
  `honor` int(7) NOT NULL DEFAULT '-100',
  `state` int(4) NOT NULL DEFAULT '0',
  `2hand` int(3) NOT NULL DEFAULT '-1',
  `as` int(4) NOT NULL DEFAULT '-100',
  `cs` int(4) NOT NULL DEFAULT '-100',
  `speed` int(4) NOT NULL DEFAULT '-100',
  `acc` int(4) NOT NULL DEFAULT '-100',
  `resilence` int(4) NOT NULL DEFAULT '-100',
  `armor` int(6) NOT NULL DEFAULT '-1',
  `armor_type` smallint(6) DEFAULT '-1',
  `ai` int(6) NOT NULL DEFAULT '-100',
  `ws` int(4) NOT NULL DEFAULT '-100',
  `weapon_ap` smallint(6) DEFAULT '-1',
  `weapon_sp` smallint(6) DEFAULT '-1',
  `damage` int(4) NOT NULL DEFAULT '-1',
  `spelldmg` int(4) NOT NULL DEFAULT '-1',
  `effect` int(4) NOT NULL DEFAULT '-1',
  `owner` int(11) NOT NULL DEFAULT '-1',
  `enchbit` int(10) NOT NULL DEFAULT '-1',
  `name` varchar(50) COLLATE utf8_unicode_ci DEFAULT '',
  `descr` varchar(70) COLLATE utf8_unicode_ci DEFAULT '',
  `haste` int(4) NOT NULL DEFAULT '-100',
  `parry` int(4) DEFAULT '0',
  `block` int(4) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_Mail`
--

DROP TABLE IF EXISTS `wc_Mail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Mail` (
  `mailId` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `CharacterId` int(15) NOT NULL DEFAULT '0',
  `fromId` int(15) NOT NULL DEFAULT '0',
  `time` datetime DEFAULT NULL,
  `gold` int(7) NOT NULL DEFAULT '0',
  `item` int(7) NOT NULL DEFAULT '0',
  `unique` int(7) NOT NULL DEFAULT '0',
  `text` char(255) COLLATE utf8_unicode_ci DEFAULT '0',
  `fromname` char(20) COLLATE utf8_unicode_ci DEFAULT 'Administration',
  `topic` char(50) COLLATE utf8_unicode_ci DEFAULT 'No Subject',
  `return` int(2) NOT NULL DEFAULT '1',
  `new` int(2) NOT NULL DEFAULT '1',
  `attached` int(2) NOT NULL DEFAULT '0',
  `amount` int(3) NOT NULL DEFAULT '0',
  `real` int(11) DEFAULT '0',
  PRIMARY KEY (`mailId`),
  KEY `CharacterId` (`CharacterId`),
  KEY `return` (`return`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_PlayerUniqueIds`
--

DROP TABLE IF EXISTS `wc_PlayerUniqueIds`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_PlayerUniqueIds` (
  `playerId` int(10) unsigned NOT NULL DEFAULT '0',
  `uniqueId` int(15) NOT NULL DEFAULT '0',
  PRIMARY KEY (`uniqueId`),
  KEY `playerId` (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_Players`
--

DROP TABLE IF EXISTS `wc_Players`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Players` (
  `playerId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `last_time` datetime NOT NULL DEFAULT '2010-10-14 00:00:00',
  `name` varchar(25) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'no name',
  `character0` int(10) unsigned NOT NULL DEFAULT '0',
  `character1` int(10) unsigned NOT NULL DEFAULT '0',
  `character2` int(10) unsigned NOT NULL DEFAULT '0',
  `character3` int(10) unsigned NOT NULL DEFAULT '0',
  `character4` int(10) unsigned NOT NULL DEFAULT '0',
  `character5` int(10) unsigned NOT NULL DEFAULT '0',
  `character6` int(10) unsigned NOT NULL DEFAULT '0',
  `character7` int(10) unsigned NOT NULL DEFAULT '0',
  `character8` int(10) unsigned NOT NULL DEFAULT '0',
  `character9` int(10) unsigned NOT NULL DEFAULT '0',
  `character10` int(10) unsigned NOT NULL DEFAULT '0',
  `character11` int(10) unsigned NOT NULL DEFAULT '0',
  `character12` int(10) unsigned NOT NULL DEFAULT '0',
  `character13` int(10) unsigned NOT NULL DEFAULT '0',
  `character14` int(10) unsigned NOT NULL DEFAULT '0',
  `character15` int(10) unsigned NOT NULL DEFAULT '0',
  `login` varchar(30) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `pass` varchar(30) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `email` varchar(50) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `userid` varchar(32) COLLATE utf8_unicode_ci DEFAULT NULL,
  `userlevel` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `gold` int(8) NOT NULL DEFAULT '0',
  `arenaPoints` int(10) unsigned NOT NULL DEFAULT '1500',
  `arenaPlayed` int(4) unsigned NOT NULL DEFAULT '0',
  `honor` int(10) unsigned NOT NULL DEFAULT '0',
  `threat` int(3) unsigned NOT NULL DEFAULT '0',
  `rank` int(3) unsigned NOT NULL DEFAULT '1',
  `trophies` int(7) unsigned NOT NULL DEFAULT '0',
  `newbie` int(3) unsigned NOT NULL DEFAULT '1',
  `newbie2` int(3) unsigned NOT NULL DEFAULT '1',
  `chatVic` int(3) unsigned NOT NULL DEFAULT '1',
  `chatAtt` int(3) unsigned NOT NULL DEFAULT '1',
  `chatTar` int(3) unsigned NOT NULL DEFAULT '1',
  `chatSpa` int(3) DEFAULT '1',
  `rested` int(11) DEFAULT '0',
  `first_time` datetime NOT NULL DEFAULT '2010-11-01 00:00:00',
  `referrer` int(11) DEFAULT NULL,
  `loaded` smallint(6) DEFAULT '0',
  `bound` smallint(6) DEFAULT '0',
  `expansions` int(11) DEFAULT '0',
  `add_honor` int(11) DEFAULT '0',
  `slides` int(11) DEFAULT '0',
  `usekeyspellbar` smallint(6) DEFAULT '1',
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM AUTO_INCREMENT=1000 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_Professions1`
--

DROP TABLE IF EXISTS `wc_Professions1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Professions1` (
  `CharacterId` int(11) unsigned NOT NULL,
  `prof_1` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_2` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_3` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_4` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_5` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_6` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_7` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_8` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_9` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_10` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_11` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_12` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_13` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_14` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_15` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_16` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_17` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_18` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_19` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_20` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_21` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_22` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_23` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_24` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_25` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_26` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_27` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_28` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_29` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_30` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_31` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_32` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_33` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_34` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_35` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_36` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_37` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_38` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_39` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_40` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_41` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_42` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_43` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_44` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_45` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_46` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_47` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_48` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_49` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_50` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_51` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_52` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_53` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_54` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_55` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_56` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_57` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_58` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_59` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_60` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_61` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_62` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_63` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_64` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_65` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_66` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_67` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_68` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_69` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_70` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_71` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_72` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_73` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_74` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_75` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_76` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_77` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_78` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_79` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_80` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_81` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_82` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_83` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_84` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_85` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_86` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_87` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_88` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_89` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_90` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_91` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_92` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_93` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_94` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_95` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_96` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_97` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_98` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_99` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_100` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_101` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_102` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_103` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_104` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_105` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_106` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_107` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_108` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_109` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_110` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_111` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_112` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_113` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_114` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_115` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_116` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_117` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_118` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_119` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_120` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_121` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_122` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_123` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_124` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_125` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_126` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_127` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_128` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_129` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_130` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_131` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_132` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_133` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_134` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_135` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_136` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_137` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_138` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_139` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_140` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_141` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_142` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_143` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_144` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_145` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_146` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_147` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_148` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_149` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_150` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_151` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_152` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_153` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_154` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_155` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_156` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_157` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_158` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_159` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_160` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_161` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_162` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_163` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_164` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_165` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_166` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_167` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_168` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_169` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_170` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_171` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_172` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_173` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_174` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_175` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_176` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_177` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_178` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_179` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_180` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_181` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_182` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_183` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_184` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_185` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_186` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_187` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_188` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_189` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_190` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_191` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_192` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_193` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_194` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_195` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_196` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_197` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_198` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_199` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_200` int(10) unsigned NOT NULL DEFAULT '0',
  `prof_201` int(11) DEFAULT '0',
  `prof_202` int(11) DEFAULT '0',
  `prof_203` int(11) DEFAULT '0',
  `prof_204` int(11) DEFAULT '0',
  `prof_205` int(11) DEFAULT '0',
  `prof_206` int(11) DEFAULT '0',
  `prof_207` int(11) DEFAULT '0',
  `prof_208` int(11) DEFAULT '0',
  `prof_209` int(11) DEFAULT '0',
  `prof_210` int(11) DEFAULT '0',
  `prof_211` int(11) DEFAULT '0',
  `prof_212` int(11) DEFAULT '0',
  `prof_213` int(11) DEFAULT '0',
  `prof_214` int(11) DEFAULT '0',
  `prof_215` int(11) DEFAULT '0',
  `prof_216` int(11) DEFAULT '0',
  `prof_217` int(11) DEFAULT '0',
  `prof_218` int(11) DEFAULT '0',
  `prof_219` int(11) DEFAULT '0',
  `prof_220` int(11) DEFAULT '0',
  `prof_221` int(11) DEFAULT '0',
  `prof_222` int(11) DEFAULT '0',
  `prof_223` int(11) DEFAULT '0',
  `prof_224` int(11) DEFAULT '0',
  `prof_225` int(11) DEFAULT '0',
  `prof_226` int(11) DEFAULT '0',
  `prof_227` int(11) DEFAULT '0',
  `prof_228` int(11) DEFAULT '0',
  `prof_229` int(11) DEFAULT '0',
  `prof_230` int(11) DEFAULT '0',
  `prof_231` int(11) DEFAULT '0',
  `prof_232` int(11) DEFAULT '0',
  `prof_233` int(11) DEFAULT '0',
  `prof_234` int(11) DEFAULT '0',
  `prof_235` int(11) DEFAULT '0',
  `prof_236` int(11) DEFAULT '0',
  `prof_237` int(11) DEFAULT '0',
  `prof_238` int(11) DEFAULT '0',
  `prof_239` int(11) DEFAULT '0',
  `prof_240` int(11) DEFAULT '0',
  `prof_241` int(11) DEFAULT '0',
  `prof_242` int(11) DEFAULT '0',
  `prof_243` int(11) DEFAULT '0',
  `prof_244` int(11) DEFAULT '0',
  `prof_245` int(11) DEFAULT '0',
  `prof_246` int(11) DEFAULT '0',
  `prof_247` int(11) DEFAULT '0',
  `prof_248` int(11) DEFAULT '0',
  `prof_249` int(11) DEFAULT '0',
  `prof_250` int(11) DEFAULT '0',
  `prof_251` int(11) DEFAULT '0',
  `prof_252` int(11) DEFAULT '0',
  `prof_253` int(11) DEFAULT '0',
  `prof_254` int(11) DEFAULT '0',
  `prof_255` int(11) DEFAULT '0',
  `prof_256` int(11) DEFAULT '0',
  `prof_257` int(11) DEFAULT '0',
  `prof_258` int(11) DEFAULT '0',
  `prof_259` int(11) DEFAULT '0',
  `prof_260` int(11) DEFAULT '0',
  `prof_261` int(11) DEFAULT '0',
  `prof_262` int(11) DEFAULT '0',
  `prof_263` int(11) DEFAULT '0',
  `prof_264` int(11) DEFAULT '0',
  `prof_265` int(11) DEFAULT '0',
  `prof_266` int(11) DEFAULT '0',
  `prof_267` int(11) DEFAULT '0',
  `prof_268` int(11) DEFAULT '0',
  `prof_269` int(11) DEFAULT '0',
  `prof_270` int(11) DEFAULT '0',
  `prof_271` int(11) DEFAULT '0',
  `prof_272` int(11) DEFAULT '0',
  `prof_273` int(11) DEFAULT '0',
  `prof_274` int(11) DEFAULT '0',
  `prof_275` int(11) DEFAULT '0',
  `prof_276` int(11) DEFAULT '0',
  `prof_277` int(11) DEFAULT '0',
  `prof_278` int(11) DEFAULT '0',
  `prof_279` int(11) DEFAULT '0',
  `prof_280` int(11) DEFAULT '0',
  `prof_281` int(11) DEFAULT '0',
  `prof_282` int(11) DEFAULT '0',
  `prof_283` int(11) DEFAULT '0',
  `prof_284` int(11) DEFAULT '0',
  `prof_285` int(11) DEFAULT '0',
  `prof_286` int(11) DEFAULT '0',
  `prof_287` int(11) DEFAULT '0',
  `prof_288` int(11) DEFAULT '0',
  `prof_289` int(11) DEFAULT '0',
  `prof_290` int(11) DEFAULT '0',
  `prof_291` int(11) DEFAULT '0',
  `prof_292` int(11) DEFAULT '0',
  `prof_293` int(11) DEFAULT '0',
  `prof_294` int(11) DEFAULT '0',
  `prof_295` int(11) DEFAULT '0',
  `prof_296` int(11) DEFAULT '0',
  `prof_297` int(11) DEFAULT '0',
  `prof_298` int(11) DEFAULT '0',
  `prof_299` int(11) DEFAULT '0',
  `prof_300` int(11) DEFAULT '0',
  `prof_301` int(11) DEFAULT '0',
  `prof_302` int(11) DEFAULT '0',
  `prof_303` int(11) DEFAULT '0',
  `prof_304` int(11) DEFAULT '0',
  `prof_305` int(11) DEFAULT '0',
  `prof_306` int(11) DEFAULT '0',
  `prof_307` int(11) DEFAULT '0',
  `prof_308` int(11) DEFAULT '0',
  `prof_309` int(11) DEFAULT '0',
  `prof_310` int(11) DEFAULT '0',
  `prof_311` int(11) DEFAULT '0',
  `prof_312` int(11) DEFAULT '0',
  `prof_313` int(11) DEFAULT '0',
  `prof_314` int(11) DEFAULT '0',
  `prof_315` int(11) DEFAULT '0',
  `prof_316` int(11) DEFAULT '0',
  `prof_317` int(11) DEFAULT '0',
  `prof_318` int(11) DEFAULT '0',
  `prof_319` int(11) DEFAULT '0',
  `prof_320` int(11) DEFAULT '0',
  `prof_321` int(11) DEFAULT '0',
  `prof_322` int(11) DEFAULT '0',
  `prof_323` int(11) DEFAULT '0',
  `prof_324` int(11) DEFAULT '0',
  `prof_325` int(11) DEFAULT '0',
  `prof_326` int(11) DEFAULT '0',
  `prof_327` int(11) DEFAULT '0',
  `prof_328` int(11) DEFAULT '0',
  `prof_329` int(11) DEFAULT '0',
  `prof_330` int(11) DEFAULT '0',
  `prof_331` int(11) DEFAULT '0',
  `prof_332` int(11) DEFAULT '0',
  `prof_333` int(11) DEFAULT '0',
  `prof_334` int(11) DEFAULT '0',
  `prof_335` int(11) DEFAULT '0',
  `prof_336` int(11) DEFAULT '0',
  `prof_337` int(11) DEFAULT '0',
  `prof_338` int(11) DEFAULT '0',
  `prof_339` int(11) DEFAULT '0',
  `prof_340` int(11) DEFAULT '0',
  `prof_341` int(11) DEFAULT '0',
  `prof_342` int(11) DEFAULT '0',
  `prof_343` int(11) DEFAULT '0',
  `prof_344` int(11) DEFAULT '0',
  `prof_345` int(11) DEFAULT '0',
  `prof_346` int(11) DEFAULT '0',
  `prof_347` int(11) DEFAULT '0',
  `prof_348` int(11) DEFAULT '0',
  `prof_349` int(11) DEFAULT '0',
  `prof_350` int(11) DEFAULT '0',
  `prof_351` int(11) DEFAULT '0',
  `prof_352` int(11) DEFAULT '0',
  `prof_353` int(11) DEFAULT '0',
  `prof_354` int(11) DEFAULT '0',
  `prof_355` int(11) DEFAULT '0',
  `prof_356` int(11) DEFAULT '0',
  `prof_357` int(11) DEFAULT '0',
  `prof_358` int(11) DEFAULT '0',
  `prof_359` int(11) DEFAULT '0',
  `prof_360` int(11) DEFAULT '0',
  `prof_361` int(11) DEFAULT '0',
  `prof_362` int(11) DEFAULT '0',
  `prof_363` int(11) DEFAULT '0',
  `prof_364` int(11) DEFAULT '0',
  `prof_365` int(11) DEFAULT '0',
  `prof_366` int(11) DEFAULT '0',
  `prof_367` int(11) DEFAULT '0',
  `prof_368` int(11) DEFAULT '0',
  `prof_369` int(11) DEFAULT '0',
  `prof_370` int(11) DEFAULT '0',
  `prof_371` int(11) DEFAULT '0',
  `prof_372` int(11) DEFAULT '0',
  `prof_373` int(11) DEFAULT '0',
  `prof_374` int(11) DEFAULT '0',
  `prof_375` int(11) DEFAULT '0',
  `prof_376` int(11) DEFAULT '0',
  `prof_377` int(11) DEFAULT '0',
  `prof_378` int(11) DEFAULT '0',
  `prof_379` int(11) DEFAULT '0',
  `prof_380` int(11) DEFAULT '0',
  `prof_381` int(11) DEFAULT '0',
  `prof_382` int(11) DEFAULT '0',
  `prof_383` int(11) DEFAULT '0',
  `prof_384` int(11) DEFAULT '0',
  `prof_385` int(11) DEFAULT '0',
  `prof_386` int(11) DEFAULT '0',
  `prof_387` int(11) DEFAULT '0',
  `prof_388` int(11) DEFAULT '0',
  `prof_389` int(11) DEFAULT '0',
  `prof_390` int(11) DEFAULT '0',
  `prof_391` int(11) DEFAULT '0',
  `prof_392` int(11) DEFAULT '0',
  `prof_393` int(11) DEFAULT '0',
  `prof_394` int(11) DEFAULT '0',
  `prof_395` int(11) DEFAULT '0',
  `prof_396` int(11) DEFAULT '0',
  `prof_397` int(11) DEFAULT '0',
  `prof_398` int(11) DEFAULT '0',
  `prof_399` int(11) DEFAULT '0',
  `prof_400` int(11) DEFAULT '0',
  PRIMARY KEY (`CharacterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_Professions2`
--

DROP TABLE IF EXISTS `wc_Professions2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Professions2` (
  `CharacterId` int(11) unsigned NOT NULL,
  `prof2_1` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_2` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_3` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_4` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_5` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_6` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_7` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_8` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_9` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_10` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_11` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_12` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_13` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_14` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_15` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_16` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_17` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_18` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_19` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_20` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_21` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_22` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_23` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_24` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_25` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_26` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_27` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_28` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_29` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_30` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_31` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_32` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_33` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_34` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_35` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_36` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_37` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_38` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_39` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_40` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_41` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_42` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_43` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_44` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_45` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_46` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_47` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_48` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_49` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_50` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_51` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_52` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_53` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_54` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_55` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_56` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_57` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_58` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_59` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_60` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_61` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_62` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_63` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_64` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_65` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_66` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_67` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_68` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_69` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_70` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_71` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_72` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_73` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_74` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_75` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_76` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_77` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_78` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_79` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_80` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_81` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_82` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_83` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_84` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_85` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_86` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_87` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_88` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_89` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_90` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_91` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_92` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_93` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_94` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_95` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_96` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_97` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_98` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_99` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_100` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_101` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_102` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_103` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_104` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_105` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_106` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_107` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_108` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_109` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_110` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_111` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_112` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_113` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_114` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_115` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_116` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_117` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_118` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_119` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_120` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_121` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_122` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_123` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_124` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_125` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_126` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_127` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_128` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_129` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_130` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_131` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_132` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_133` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_134` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_135` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_136` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_137` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_138` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_139` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_140` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_141` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_142` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_143` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_144` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_145` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_146` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_147` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_148` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_149` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_150` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_151` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_152` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_153` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_154` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_155` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_156` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_157` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_158` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_159` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_160` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_161` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_162` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_163` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_164` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_165` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_166` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_167` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_168` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_169` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_170` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_171` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_172` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_173` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_174` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_175` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_176` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_177` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_178` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_179` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_180` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_181` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_182` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_183` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_184` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_185` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_186` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_187` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_188` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_189` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_190` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_191` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_192` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_193` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_194` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_195` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_196` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_197` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_198` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_199` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_200` int(10) unsigned NOT NULL DEFAULT '0',
  `prof2_201` int(10) DEFAULT NULL,
  `prof2_202` int(10) DEFAULT NULL,
  `prof2_203` int(10) DEFAULT NULL,
  `prof2_204` int(10) DEFAULT NULL,
  `prof2_205` int(10) DEFAULT NULL,
  `prof2_206` int(10) DEFAULT NULL,
  `prof2_207` int(10) DEFAULT NULL,
  `prof2_208` int(10) DEFAULT NULL,
  `prof2_209` int(10) DEFAULT NULL,
  `prof2_210` int(10) DEFAULT NULL,
  `prof2_211` int(10) DEFAULT NULL,
  `prof2_212` int(10) DEFAULT NULL,
  `prof2_213` int(10) DEFAULT NULL,
  `prof2_214` int(10) DEFAULT NULL,
  `prof2_215` int(10) DEFAULT NULL,
  `prof2_216` int(10) DEFAULT NULL,
  `prof2_217` int(10) DEFAULT NULL,
  `prof2_218` int(10) DEFAULT NULL,
  `prof2_219` int(10) DEFAULT NULL,
  `prof2_220` int(10) DEFAULT NULL,
  `prof2_221` int(10) DEFAULT NULL,
  `prof2_222` int(10) DEFAULT NULL,
  `prof2_223` int(10) DEFAULT NULL,
  `prof2_224` int(10) DEFAULT NULL,
  `prof2_225` int(10) DEFAULT NULL,
  `prof2_226` int(10) DEFAULT NULL,
  `prof2_227` int(10) DEFAULT NULL,
  `prof2_228` int(10) DEFAULT NULL,
  `prof2_229` int(10) DEFAULT NULL,
  `prof2_230` int(10) DEFAULT NULL,
  `prof2_231` int(10) DEFAULT NULL,
  `prof2_232` int(10) DEFAULT NULL,
  `prof2_233` int(10) DEFAULT NULL,
  `prof2_234` int(10) DEFAULT NULL,
  `prof2_235` int(10) DEFAULT NULL,
  `prof2_236` int(10) DEFAULT NULL,
  `prof2_237` int(10) DEFAULT NULL,
  `prof2_238` int(10) DEFAULT NULL,
  `prof2_239` int(10) DEFAULT NULL,
  `prof2_240` int(10) DEFAULT NULL,
  `prof2_241` int(10) DEFAULT NULL,
  `prof2_242` int(10) DEFAULT NULL,
  `prof2_243` int(10) DEFAULT NULL,
  `prof2_244` int(10) DEFAULT NULL,
  `prof2_245` int(10) DEFAULT NULL,
  `prof2_246` int(10) DEFAULT NULL,
  `prof2_247` int(10) DEFAULT NULL,
  `prof2_248` int(10) DEFAULT NULL,
  `prof2_249` int(10) DEFAULT NULL,
  `prof2_250` int(10) DEFAULT NULL,
  `prof2_251` int(10) DEFAULT NULL,
  `prof2_252` int(10) DEFAULT NULL,
  `prof2_253` int(10) DEFAULT NULL,
  `prof2_254` int(10) DEFAULT NULL,
  `prof2_255` int(10) DEFAULT NULL,
  `prof2_256` int(10) DEFAULT NULL,
  `prof2_257` int(10) DEFAULT NULL,
  `prof2_258` int(10) DEFAULT NULL,
  `prof2_259` int(10) DEFAULT NULL,
  `prof2_260` int(10) DEFAULT NULL,
  `prof2_261` int(10) DEFAULT NULL,
  `prof2_262` int(10) DEFAULT NULL,
  `prof2_263` int(10) DEFAULT NULL,
  `prof2_264` int(10) DEFAULT NULL,
  `prof2_265` int(10) DEFAULT NULL,
  `prof2_266` int(10) DEFAULT NULL,
  `prof2_267` int(10) DEFAULT NULL,
  `prof2_268` int(10) DEFAULT NULL,
  `prof2_269` int(10) DEFAULT NULL,
  `prof2_270` int(10) DEFAULT NULL,
  `prof2_271` int(10) DEFAULT NULL,
  `prof2_272` int(10) DEFAULT NULL,
  `prof2_273` int(10) DEFAULT NULL,
  `prof2_274` int(10) DEFAULT NULL,
  `prof2_275` int(10) DEFAULT NULL,
  `prof2_276` int(10) DEFAULT NULL,
  `prof2_277` int(10) DEFAULT NULL,
  `prof2_278` int(10) DEFAULT NULL,
  `prof2_279` int(10) DEFAULT NULL,
  `prof2_280` int(10) DEFAULT NULL,
  `prof2_281` int(10) DEFAULT NULL,
  `prof2_282` int(10) DEFAULT NULL,
  `prof2_283` int(10) DEFAULT NULL,
  `prof2_284` int(10) DEFAULT NULL,
  `prof2_285` int(10) DEFAULT NULL,
  `prof2_286` int(10) DEFAULT NULL,
  `prof2_287` int(10) DEFAULT NULL,
  `prof2_288` int(10) DEFAULT NULL,
  `prof2_289` int(10) DEFAULT NULL,
  `prof2_290` int(10) DEFAULT NULL,
  `prof2_291` int(10) DEFAULT NULL,
  `prof2_292` int(10) DEFAULT NULL,
  `prof2_293` int(10) DEFAULT NULL,
  `prof2_294` int(10) DEFAULT NULL,
  `prof2_295` int(10) DEFAULT NULL,
  `prof2_296` int(10) DEFAULT NULL,
  `prof2_297` int(10) DEFAULT NULL,
  `prof2_298` int(10) DEFAULT NULL,
  `prof2_299` int(10) DEFAULT NULL,
  `prof2_300` int(10) DEFAULT NULL,
  `prof2_301` int(10) DEFAULT NULL,
  `prof2_302` int(10) DEFAULT NULL,
  `prof2_303` int(10) DEFAULT NULL,
  `prof2_304` int(10) DEFAULT NULL,
  `prof2_305` int(10) DEFAULT NULL,
  `prof2_306` int(10) DEFAULT NULL,
  `prof2_307` int(10) DEFAULT NULL,
  `prof2_308` int(10) DEFAULT NULL,
  `prof2_309` int(10) DEFAULT NULL,
  `prof2_310` int(10) DEFAULT NULL,
  `prof2_311` int(10) DEFAULT NULL,
  `prof2_312` int(10) DEFAULT NULL,
  `prof2_313` int(10) DEFAULT NULL,
  `prof2_314` int(10) DEFAULT NULL,
  `prof2_315` int(10) DEFAULT NULL,
  `prof2_316` int(10) DEFAULT NULL,
  `prof2_317` int(10) DEFAULT NULL,
  `prof2_318` int(10) DEFAULT NULL,
  `prof2_319` int(10) DEFAULT NULL,
  `prof2_320` int(10) DEFAULT NULL,
  `prof2_321` int(10) DEFAULT NULL,
  `prof2_322` int(10) DEFAULT NULL,
  `prof2_323` int(10) DEFAULT NULL,
  `prof2_324` int(10) DEFAULT NULL,
  `prof2_325` int(10) DEFAULT NULL,
  `prof2_326` int(10) DEFAULT NULL,
  `prof2_327` int(10) DEFAULT NULL,
  `prof2_328` int(10) DEFAULT NULL,
  `prof2_329` int(10) DEFAULT NULL,
  `prof2_330` int(10) DEFAULT NULL,
  `prof2_331` int(10) DEFAULT NULL,
  `prof2_332` int(10) DEFAULT NULL,
  `prof2_333` int(10) DEFAULT NULL,
  `prof2_334` int(10) DEFAULT NULL,
  `prof2_335` int(10) DEFAULT NULL,
  `prof2_336` int(10) DEFAULT NULL,
  `prof2_337` int(10) DEFAULT NULL,
  `prof2_338` int(10) DEFAULT NULL,
  `prof2_339` int(10) DEFAULT NULL,
  `prof2_340` int(10) DEFAULT NULL,
  `prof2_341` int(10) DEFAULT NULL,
  `prof2_342` int(10) DEFAULT NULL,
  `prof2_343` int(10) DEFAULT NULL,
  `prof2_344` int(10) DEFAULT NULL,
  `prof2_345` int(10) DEFAULT NULL,
  `prof2_346` int(10) DEFAULT NULL,
  `prof2_347` int(10) DEFAULT NULL,
  `prof2_348` int(10) DEFAULT NULL,
  `prof2_349` int(10) DEFAULT NULL,
  `prof2_350` int(10) DEFAULT NULL,
  `prof2_351` int(10) DEFAULT NULL,
  `prof2_352` int(10) DEFAULT NULL,
  `prof2_353` int(10) DEFAULT NULL,
  `prof2_354` int(10) DEFAULT NULL,
  `prof2_355` int(10) DEFAULT NULL,
  `prof2_356` int(10) DEFAULT NULL,
  `prof2_357` int(10) DEFAULT NULL,
  `prof2_358` int(10) DEFAULT NULL,
  `prof2_359` int(10) DEFAULT NULL,
  `prof2_360` int(10) DEFAULT NULL,
  `prof2_361` int(10) DEFAULT NULL,
  `prof2_362` int(10) DEFAULT NULL,
  `prof2_363` int(10) DEFAULT NULL,
  `prof2_364` int(10) DEFAULT NULL,
  `prof2_365` int(10) DEFAULT NULL,
  `prof2_366` int(10) DEFAULT NULL,
  `prof2_367` int(10) DEFAULT NULL,
  `prof2_368` int(10) DEFAULT NULL,
  `prof2_369` int(10) DEFAULT NULL,
  `prof2_370` int(10) DEFAULT NULL,
  `prof2_371` int(10) DEFAULT NULL,
  `prof2_372` int(10) DEFAULT NULL,
  `prof2_373` int(10) DEFAULT NULL,
  `prof2_374` int(10) DEFAULT NULL,
  `prof2_375` int(10) DEFAULT NULL,
  `prof2_376` int(10) DEFAULT NULL,
  `prof2_377` int(10) DEFAULT NULL,
  `prof2_378` int(10) DEFAULT NULL,
  `prof2_379` int(10) DEFAULT NULL,
  `prof2_380` int(10) DEFAULT NULL,
  `prof2_381` int(10) DEFAULT NULL,
  `prof2_382` int(10) DEFAULT NULL,
  `prof2_383` int(10) DEFAULT NULL,
  `prof2_384` int(10) DEFAULT NULL,
  `prof2_385` int(10) DEFAULT NULL,
  `prof2_386` int(10) DEFAULT NULL,
  `prof2_387` int(10) DEFAULT NULL,
  `prof2_388` int(10) DEFAULT NULL,
  `prof2_389` int(10) DEFAULT NULL,
  `prof2_390` int(10) DEFAULT NULL,
  `prof2_391` int(10) DEFAULT NULL,
  `prof2_392` int(10) DEFAULT NULL,
  `prof2_393` int(10) DEFAULT NULL,
  `prof2_394` int(10) DEFAULT NULL,
  `prof2_395` int(10) DEFAULT NULL,
  `prof2_396` int(10) DEFAULT NULL,
  `prof2_397` int(10) DEFAULT NULL,
  `prof2_398` int(10) DEFAULT NULL,
  `prof2_399` int(10) DEFAULT NULL,
  `prof2_400` int(10) DEFAULT NULL,
  PRIMARY KEY (`CharacterId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_Spells`
--

DROP TABLE IF EXISTS `wc_Spells`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_Spells` (
  `characterId` int(11) NOT NULL,
  `spellId` smallint(6) NOT NULL,
  `isEnabled` tinyint(4) DEFAULT '1',
  `minRage` smallint(6) DEFAULT '0',
  PRIMARY KEY (`characterId`,`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_events`
--

DROP TABLE IF EXISTS `wc_events`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_events` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `planned` datetime DEFAULT NULL,
  `msg` varchar(150) COLLATE utf8_unicode_ci DEFAULT '',
  `rotated` time DEFAULT NULL,
  `commands` varchar(1000) COLLATE utf8_unicode_ci DEFAULT '',
  `lasttime` datetime DEFAULT '2010-01-01 00:00:00',
  `servId` int(5) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_ignoredItems`
--

DROP TABLE IF EXISTS `wc_ignoredItems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_ignoredItems` (
  `CharacterId` int(10) NOT NULL,
  `itemId` int(11) NOT NULL,
  PRIMARY KEY (`CharacterId`,`itemId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_itemsCache`
--

DROP TABLE IF EXISTS `wc_itemsCache`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_itemsCache` (
  `itemid` int(11) NOT NULL,
  `cat` smallint(6) NOT NULL,
  `category` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
  `level` smallint(6) NOT NULL,
  `rating` float NOT NULL,
  `name` varchar(50) COLLATE utf8_unicode_ci NOT NULL,
  `class` int(11) DEFAULT NULL,
  `quality` smallint(6) DEFAULT NULL,
  PRIMARY KEY (`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_log`
--

DROP TABLE IF EXISTS `wc_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_log` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL DEFAULT '2010-01-01 00:00:00',
  `level` varchar(25) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cl` varchar(30) COLLATE utf8_unicode_ci DEFAULT NULL,
  `msg` varchar(500) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'no message',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_logGold`
--

DROP TABLE IF EXISTS `wc_logGold`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_logGold` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL DEFAULT '2010-01-01 00:00:00',
  `service` varchar(30) COLLATE utf8_unicode_ci DEFAULT NULL,
  `msg` varchar(500) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'no message',
  `playerId` int(11) NOT NULL,
  `value` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_logItems`
--

DROP TABLE IF EXISTS `wc_logItems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_logItems` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL DEFAULT '2010-01-01 00:00:00',
  `level` smallint(6) DEFAULT NULL,
  `ilevel` smallint(6) DEFAULT NULL,
  `quality` smallint(6) DEFAULT NULL,
  `item` int(11) DEFAULT '0',
  `unique` int(11) DEFAULT '0',
  `msg` varchar(50) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'no message',
  `amount` int(11) DEFAULT '0',
  `character` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wc_premium`
--

DROP TABLE IF EXISTS `wc_premium`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wc_premium` (
  `playerId` int(10) unsigned NOT NULL,
  `due` date NOT NULL DEFAULT '2010-01-01',
  `modules` int(10) DEFAULT NULL,
  `subscribed` tinyint(4) DEFAULT NULL,
  `costs` int(11) NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping routines for database 'wowmod_eu'
--
/*!50003 DROP FUNCTION IF EXISTS `maximum` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE FUNCTION `maximum`(n1 INT, n2 INT) RETURNS int(11)
    DETERMINISTIC
BEGIN
     DECLARE max INT;
     SET max = n1;
     IF (n2 > max) THEN
     	SET max = n2;
     END IF;
     RETURN max;
    END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `bid` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE PROCEDURE `bid`(IN aucId integer, IN charId integer, IN ingameMoney integer)
    MODIFIES SQL DATA
scope_main:BEGIN
DECLARE gold_has INTEGER;
DECLARE money_has INTEGER;
DECLARE gold_needed INTEGER;
DECLARE money_needed INTEGER;
DECLARE old_money INTEGER;
DECLARE old_gold INTEGER;
DECLARE old_bidder INTEGER;
DECLARE var_player INTEGER;
DECLARE h_Extend INTEGER;

DECLARE extend INTEGER;
DECLARE aLength INTEGER;



SELECT playerId INTO var_player FROM wc_Characters WHERE characterId = charId;
SELECT wc_Players.`gold` INTO gold_has FROM wc_Players, wc_Characters WHERE wc_Players.playerId = wc_Characters.playerId AND characterId = charId;
SELECT `money` INTO money_has FROM wc_Characters WHERE characterId = charId;
SELECT ROUND(`bid`*1.1), ROUND(`gold`*1.1), `bid`, `gold`, `bidId` INTO money_needed, gold_needed, old_money, old_gold, old_bidder FROM wc_Auction WHERE id = aucId;

IF (gold_has < gold_needed) OR (ingameMoney < money_needed) THEN
BEGIN
SELECT '1' AS `status`, 'Not Enough Silver/Gold' AS `message`, '0' AS `silver`;
LEAVE scope_main;
END;
END IF;

IF (old_bidder > 0) THEN
BEGIN
CALL mail(30, 0, 'Auction service', 'You were out-bidded', old_bidder, old_money, 0, 'Someone has placed higher bid or buyout on your bid',0, old_gold, 0);
END;
END IF;

UPDATE wc_Players SET `gold` = `gold` - gold_needed WHERE playerId = var_player;
UPDATE wc_Characters SET `money` = `money` - money_needed WHERE characterId = charId;
UPDATE wc_Auction SET `bid` = money_needed, `gold` = gold_needed, `bidId` = charId WHERE id = aucId;


SELECT `hExtend`, HOUR(TIMEDIFF(`time`, NOW())) INTO extend, aLength FROM wc_Auction WHERE id = aucId;

CASE
WHEN extend < 19 THEN

UPDATE wc_Auction
SET `hExtend` = extend +
IF((90 + extend) > (aLength + 12),
1,
12 - (90 + extend - aLength))
WHERE id = aucId;

ELSE

UPDATE wc_Auction
SET `hExtend` = extend + 1
WHERE id = aucId;
END CASE;



SELECT '0' AS `status`, gold_needed AS `gold`, money_needed AS `silver`;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `buyout` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE PROCEDURE `buyout`(IN aucId integer, IN charId integer, IN ingameMoney integer)
    MODIFIES SQL DATA
scope_main:BEGIN
  DECLARE  gold_has INTEGER;
  DECLARE  money_has INTEGER;
  DECLARE gold_needed INTEGER;
  DECLARE money_needed INTEGER;
  DECLARE old_money INTEGER;
  DECLARE old_gold INTEGER;
  DECLARE old_bidder INTEGER;
  DECLARE var_player INTEGER;

  DECLARE var_owner INTEGER;
  DECLARE var_itemId INTEGER;
  DECLARE var_itemAmount INTEGER;
  DECLARE var_itemUnique INTEGER;
  SELECT playerId INTO var_player FROM wc_Characters WHERE characterId = charId;
  SELECT wc_Players.`gold` INTO gold_has FROM wc_Players, wc_Characters  WHERE wc_Players.playerId = wc_Characters.playerId AND characterId = charId;
  SELECT `money` INTO money_has FROM wc_Characters WHERE characterId = charId;

  SELECT `buyout`, `goldBuyout`, `bid`, `gold`, `bidId` INTO money_needed, gold_needed, old_money, old_gold, old_bidder FROM wc_Auction WHERE id = aucId;
  IF (gold_has < gold_needed)  OR (ingameMoney < money_needed)  THEN
  BEGIN
    SELECT '1' AS `status`, 'Not Enough Silver/Gold' AS `message`, '0' AS `silver`;
    LEAVE scope_main;
  END;
  END IF;


  IF (old_bidder > 0) THEN
  BEGIN
    CALL mail(30, 0, 'Auction service', 'You were out-bidded', old_bidder, old_money, 0, 'Someone has placed higher bid or buyout on your bid', 0, old_gold, 0);
  END;
  END IF;

  UPDATE wc_Players SET `gold` = `gold` - gold_needed WHERE playerId = var_player;
  UPDATE wc_Characters SET `money` = `money` - money_needed WHERE characterId = charId;


  SELECT fromId, `item`, `unique`, `amount` INTO var_owner, var_itemId, var_itemUnique, var_itemAmount FROM wc_Auction WHERE id = aucId;
  CALL mail(30, 0, 'Auction service', 'Your item was bought-out', var_owner, money_needed, 0, 'Your item was bought out.', 0, gold_needed, 0);
  CALL mail(30, 0, 'Auction service', 'You have bought out item!', charId, 0, var_itemId, 'Congratulations, you have bought out the item.', var_itemAmount, 0, var_itemUnique);
  DELETE FROM wc_Auction WHERE id = aucId;
  SELECT '0' AS `status`, gold_needed AS `gold`, money_needed AS `silver`;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `mail` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE PROCEDURE `mail`(IN var_days integer,
                                    IN var_from integer,
                                    IN var_fromName VARCHAR(20),
                                    IN var_topic VARCHAR(50),
                                    IN var_to integer,
                                    IN var_silver integer,
                                    IN var_item integer,
                                    IN var_text VARCHAR(150),
                                    IN var_itemAmount integer,
                                    IN var_gold integer,
                                    IN var_unique integer)
    MODIFIES SQL DATA
scope_main:BEGIN
  INSERT INTO wc_Mail (mailId, CharacterId,
                      fromId, fromname, topic,
                      gold, item, `text`, `time`,
                      `amount`, `unique`, `real`) VALUES
                      (NULL, var_to,
                      var_from, var_fromName, var_topic,
                      var_silver, var_item, var_text, NOW(),
                      var_itemAmount, var_unique, var_gold);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

