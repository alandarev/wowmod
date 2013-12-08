/** Double-include prevention */
#if defined wow_natives_included
  #endinput
#endif
#define wow_natives_included

enum PDModule
{
  PDM_base=0,
  PDM_reserved=1,
  PDM_kpd=2,
  PDM_dual=3,
  PDM_xp=4,
  PDM_drop=5,
  PDM_dps=6,
  PDM_hp=7,
  PDM_models=8
}




/**
 * Gets player rating according to his gear
 *
 * @param client  Client
 * @param teamWon Team index which won.
 * @return        The float value of temporary player rating
 */
native Float:wow_getTempPlayerRating(client, teamWon=-1);

/**
 * Scrambles teams using neat  algorithm
 *
 * @return				void
 */
native wow_scrambleTeams();

/**
 * Tells whether client has specified premium module enabled
 * @param client           Client
 * @param PDModule:module  Module to check for
 * @return                 boolean
 */
native bool:wow_hasPremiumModule(client, PDModule:module);

/**
 * Returns the Silver player owns
 * @param client          Client
 */
native wow_getSilver(client);

/**
 * Returns the XP player owns (can range from 0 to Required XP - 1)
 * @param client          Client
 */
native wow_getXP(client);

/**
 * Set Silver of player (To add Silver do wow_setSilver (client, wow_getSilver + addAmount)
 * @param client          Client
 * @param amount          Amount to set
 */
native wow_setSilver(client, amount);

/**
 * Set XP of player (To add XP do wow_setXP (client, wow_getXP + addAmount)
 * @param client          Client
 * @param amount          Amount to set
 */
native wow_setXP(client, amount);
