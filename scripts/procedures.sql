DELIMITER $$

DROP PROCEDURE IF EXISTS `wowmod`.`bid`$$
CREATE DEFINER=`jozh`@`%` PROCEDURE  `wowmod`.`bid`(IN aucId integer, IN charId integer, IN ingameMoney integer)
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

  SELECT playerId INTO var_player FROM wc_Characters WHERE characterId = charId;

  SELECT wc_Players.`gold` INTO gold_has FROM wc_Players, wc_Characters  WHERE wc_Players.playerId = wc_Characters.playerId AND characterId = charId;
  SELECT `money` INTO money_has FROM wc_Characters WHERE characterId = charId;

  /* this line is bid/buyout sensitive */
  SELECT ROUND(`bid`*1.1), ROUND(`gold`*1.1), `bid`, `gold`, `bidId` INTO money_needed, gold_needed, old_money, old_gold, old_bidder FROM wc_Auction WHERE id = aucId;


  IF (gold_has < gold_needed) OR (ingameMoney < money_needed)  THEN
  BEGIN
    SELECT '1' AS `status`, 'Not Enough Silver/Gold' AS `message`, '0' AS `silver`;
    LEAVE scope_main;
  END;
  END IF;

  /* we made checks, now do the processing */

  /* return money to old owner */
  IF (old_bidder > 0) THEN
  BEGIN
    CALL mail(30, 0, 'Auction service', 'You were out-bidded', old_bidder, old_money, 0, 'Someone has placed higher bid or buyout on your bid', 0, old_gold, 0);
  END;
  END IF;

  /* take new bidder's money, be aware that we still need to take silver off in wowmod */
  UPDATE wc_Players SET `gold` = `gold` - gold_needed WHERE playerId = var_player;

  UPDATE wc_Characters SET `money` = `money` - money_needed WHERE characterId = charId;

  /* update auction */
  /* this line is bid/buyout sensitive */
  UPDATE wc_Auction SET `bid` = money_needed, `gold` = gold_needed, `bidId` = charId WHERE id = aucId;


  SELECT '0' AS `status`, gold_needed AS `gold`, money_needed AS `silver`;



END $$

DELIMITER ;



DELIMITER $$

DROP PROCEDURE IF EXISTS `wowmod`.`buyout`$$
CREATE DEFINER=`jozh`@`%` PROCEDURE  `wowmod`.`buyout`(IN aucId integer, IN charId integer, IN ingameMoney integer)
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
  /* declares for buyout */
  DECLARE var_owner INTEGER;
  DECLARE var_itemId INTEGER;
  DECLARE var_itemAmount INTEGER;
  DECLARE var_itemUnique INTEGER;

  SELECT playerId INTO var_player FROM wc_Characters WHERE characterId = charId;

  SELECT wc_Players.`gold` INTO gold_has FROM wc_Players, wc_Characters  WHERE wc_Players.playerId = wc_Characters.playerId AND characterId = charId;
  SELECT `money` INTO money_has FROM wc_Characters WHERE characterId = charId;

  /* this line is bid/buyout sensitive */
  SELECT `buyout`, `goldBuyout`, `bid`, `gold`, `bidId` INTO money_needed, gold_needed, old_money, old_gold, old_bidder FROM wc_Auction WHERE id = aucId;


  IF (gold_has < gold_needed)  OR (ingameMoney < money_needed)  THEN
  BEGIN
    SELECT '1' AS `status`, 'Not Enough Silver/Gold' AS `message`, '0' AS `silver`;
    LEAVE scope_main;
  END;
  END IF;

  /* we made checks, now do the processing */

  /* return money to old owner */
  IF (old_bidder > 0) THEN
  BEGIN
    CALL mail(30, 0, 'Auction service', 'You were out-bidded', old_bidder, old_money, 0, 'Someone has placed higher bid or buyout on your bid', 0, old_gold, 0);
  END;
  END IF;

  /* take new bidder's money, be aware that we still need to take silver off in wowmod */
  UPDATE wc_Players SET `gold` = `gold` - gold_needed WHERE playerId = var_player;
  UPDATE wc_Characters SET `money` = `money` - money_needed WHERE characterId = charId;

  /* update auction */
  /* this line is bid/buyout sensitive */
  SELECT fromId, `item`, `unique`, `amount` INTO var_owner, var_itemId, var_itemUnique, var_itemAmount FROM wc_Auction WHERE id = aucId;
  CALL mail(30, 0, 'Auction service', 'Your item was bought-out', var_owner, money_needed, 0, 'Your item was bought out.', 0, gold_needed, 0);
  CALL mail(30, 0, 'Auction service', 'You have bought out item!', charId, 0, var_itemId, 'Congratulations, you have bought out the item.', var_itemAmount, 0, var_itemUnique);

  DELETE FROM wc_Auction WHERE id = aucId;


  SELECT '0' AS `status`, gold_needed AS `gold`, money_needed AS `silver`;



END $$

DELIMITER ;


DELIMITER $$

DROP PROCEDURE IF EXISTS `wowmod`.`mail`$$
CREATE DEFINER=`jozh`@`%` PROCEDURE  `wowmod`.`mail`(IN var_days integer,
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



END $$

DELIMITER ;