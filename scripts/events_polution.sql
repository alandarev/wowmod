use wowmod;

delete from wc_events where planned is null;



/* 11:00 heroic event */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Heroic Event +400 percentage to all of your stats!', 'wc_heroic 1', '11:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'wc_heroic 0', '11:20:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'wc_heroic 0', '11:25:00');

/* 12:00 xp and drop event */
insert into wc_events (`msg`, `commands`, `rotated`) values ('plus 50% XP and 25% DROP rate for everything!', 'wc_temp_xp 1.5||wc_temp_drop 1.25', '12:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'wc_temp_xp 1.0||wc_temp_drop 1.0', '12:45:00');

/* 13:00 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '13:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '13:10:00');

/* 14:00 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '14:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '14:10:00');

/* 15:00 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '15:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '15:10:00');

/* 16:00 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '16:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '16:10:00');

/* 17:00 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '17:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '17:10:00');

/* 17:30 gambling */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Gambling event: type bet ct/t 0-300  to bet silver for the winning team', 'sm_bet 1', '17:30:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'sm_bet 0', '17:50:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'sm_bet 0', '17:55:00');

/* 18:00 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '18:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '18:10:00');

/* 21:30 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '21:30:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '21:40:00');

/* 22:00 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '22:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '22:10:00');

/* 22:30 gambling */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Gambling event: type bet ct/t 0-300  to bet silver for the winning team', 'sm_bet 1', '22:30:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'sm_bet 0', '23:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'sm_bet 0', '23:05:00');

/* 00:00 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '00:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '00:10:00');

/* 01:00 dm */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '01:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '01:10:00');

/* 14:00 GG event */
/*
insert into wc_events (`msg`, `commands`, `rotated`) values ('GunGame event', 'sm_map gg_blue_arena', '14:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'sm_map de_dust2', '14:50:00');
*/
/* 16:00 DM event */
/*
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1', '16:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0', '16:40:00');
*/
/* 18:00 VIP event */
/* test before enable


*/


/* 21:30 DM event */
/*
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1', '21:30:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0', '21:55:00');
*/

/* 22:20 GG event */
/*
insert into wc_events (`msg`, `commands`, `rotated`) values ('GunGame event', 'sm_map gg_dm_bloodarena3', '22:20:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'sm_map de_dust2', '22:50:00');
*/
/* 23:00 Drop Event */
insert into wc_events (`msg`, `commands`, `rotated`) values ('plus 35% DROP rate for everything!', 'wc_temp_drop 1.35', '23:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'wc_temp_drop 1.0', '23:50:00');

/* 01:00 VIP event */
/* test before enable


*/

/* 03:00 XP and Drop event */
insert into wc_events (`msg`, `commands`, `rotated`) values ('plus 50% XP and 50% DROP rate for everything!', 'wc_temp_xp 1.5||wc_temp_drop 1.5', '03:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'wc_temp_xp 1.0||wc_temp_drop 1.0', '04:00:00');

/* 05:00 DM event */
insert into wc_events (`msg`, `commands`, `rotated`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1', '05:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'mp_roundtime 3||cssdm_enabled 0', '05:50:00');

/* 07:00 GG event */
/*
insert into wc_events (`msg`, `commands`, `rotated`) values ('GunGame event', 'sm_map gg_dm_bloodarena3', '07:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'sm_map de_dust2', '07:40:00');
*/
/* 09:00 XP and Drop event */
insert into wc_events (`msg`, `commands`, `rotated`) values ('plus 50% XP and 50% DROP rate for everything!', 'wc_temp_xp 1.5||wc_temp_drop 1.5', '09:00:00');
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'wc_temp_xp 1.0||wc_temp_drop 1.0', '10:00:00');

/* Services */
/* is somehow broken
insert into wc_events (`msg`, `commands`, `rotated`) values ('', 'wc_generate_items', '04:50:00');
*/
/* End of services */


/* US server */
/* DEATH MATCH EVENTS*/
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '14:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '14:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '15:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '15:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '16:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '16:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '17:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '17:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '18:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '18:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '19:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '19:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '20:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '20:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '21:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '21:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '22:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '22:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '23:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '23:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '00:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '00:10:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('Death Match event', 'mp_roundtime 9||cssdm_enabled 1||mp_timelimit 50', '01:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'mp_roundtime 3||cssdm_enabled 0||mp_timelimit 30', '01:10:00', '3');


/* 13:00 Drop Event */
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('plus 35% DROP rate for everything!', 'wc_temp_drop 1.35||mp_timelimit 70', '08:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'wc_temp_drop 1.0||mp_timelimit 30', '08:50:00', '3');

/* 08:00 XP and Drop event */
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('plus 50% XP and 50% DROP rate for everything!', 'wc_temp_xp 1.5||wc_temp_drop 1.5||mp_timelimit 70', '03:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'wc_temp_xp 1.0||wc_temp_drop 1.0||mp_timelimit 30', '05:00:00', '3');

/* 17:00 xp and drop event */
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('plus 50% XP and 25% DROP rate for everything!', 'wc_temp_xp 1.5||wc_temp_drop 1.25||mp_timelimit 70', '12:00:00', '3');
insert into wc_events (`msg`, `commands`, `rotated`, `servId`) values ('', 'wc_temp_xp 1.0||wc_temp_drop 1.0||mp_timelimit 30', '13:00:00', '3');
