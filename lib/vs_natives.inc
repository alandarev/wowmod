

/** Double-include prevention */
#if defined vs_natives_included
  #endinput
#endif
#define vs_natives_included

public SharedPlugin:__pl_visual = 
{
	name = "visual",
	file = "visual.smx",
	required = 1,
};

public __pl_visual_SetNTVOptional()
{
  MarkNativeAsOptional("VS_Spark");
  MarkNativeAsOptional("VS_Smoke");
  MarkNativeAsOptional("VS_Dust");
  MarkNativeAsOptional("VS_MuzzleFlash");
  MarkNativeAsOptional("VS_MetalSpark");
  MarkNativeAsOptional("VS_EnergySplash");
  MarkNativeAsOptional("VS_ArmorRicochet");
  MarkNativeAsOptional("VS_GlowSprite");
  MarkNativeAsOptional("VS_Explosion");
  MarkNativeAsOptional("VS_BloodSprite");
  MarkNativeAsOptional("VS_BeamRingPoint");
  MarkNativeAsOptional("VS_BeamPoints");
  MarkNativeAsOptional("VS_BeamLaser");
  MarkNativeAsOptional("VS_BeamRing");
  MarkNativeAsOptional("VS_BeamFollow");
  MarkNativeAsOptional("VS_Bubbles");
}
 
/**
 * Sparks
 *
 * @param Float:position[3]   Position
 * @param Float:direction[3]  Direction
 * @param magnitude           Magnitude of Sparks
 * @param trail_length        Length of Trails
 * @param Float:delay         Delay before effect launch
 * @return        No Return
 */
native VS_Spark(Float:poition[3], Float:direction[3], magnitude, trail_length, Float:delay=0.0);


native VS_Smoke(const Float:pos[3], Model, Float:Scale, FrameRate, Float:delay=0.0);

native VS_Dust(const Float:pos[3], const Float:dir[3], Float:Size, Float:Speed, Float:delay=0.0);

native VS_MuzzleFlash(const Float:pos[3], const Float:angles[3], Float:Scale, Type, Float:delay=0.0);

native VS_MetalSpark(const Float:pos[3], const Float:dir[3]);

native VS_EnergySplash(const Float:pos[3], const Float:dir[3], bool:Explosive, Float:delay=0.0);

native VS_ArmorRicochet(const Float:pos[3], const Float:dir[3], Float:delay=0.0);

native VS_GlowSprite(const Float:pos[3], Model, Float:Life, Float:Size, Brightness, Float:delay=0.0);

/**
 * MaterialType you can use:
 * '-'    "don't decal this surface"
 * 'C'   "Concrete"
 * 'M'   "Metal"
 * 'W'   "Wood"
 * 'Y'   "Glass"
 * 'F'   "Flesh"
 * 'B'   "BloodyFlesh"
 * 'H'   "AlienFlesh"
 * 'A'   "Antlion"
 * 'D'  "Dirt"
 * 'V'  "Vent"
 * 'G'  "Grate"
 * 'T'  "Tile"
 * 'S'  "Slosh"
 * 'P'  "Computer"
 * 'I'  "Clip"
 * 'O'  "Foliage"
 * 'N'  "Sand"
 * 'L'  "Plastic"
 */
native VS_Explosion(const Float:pos[3], Model, Float:Scale, Framerate, Flags, Radius, Magnitude, const Float:normal[3]={0.0, 0.0, 1.0}, MaterialType='C', Float:delay=0.0);


native VS_BloodSprite(const Float:pos[3], const Float:dir[3], const color[4], Size, SprayModel, BloodDropModel, Float:delay=0.0);

native VS_BeamRingPoint(const Float:center[3], Float:Start_Radius, Float:End_Radius, ModelIndex, HaloIndex, StartFrame, 
                FrameRate, Float:Life, Float:Width, Float:Amplitude, const Color[4], Speed, Flags, Float:delay=0.0);

native VS_BeamPoints(const Float:start[3], const Float:end[3], ModelIndex, HaloIndex, StartFrame, FrameRate, Float:Life, 
                Float:Width, Float:EndWidth, FadeLength, Float:Amplitude, const Color[4], Speed, Float:delay=0.0);

native VS_BeamLaser(StartEntity, EndEntity, ModelIndex, HaloIndex, StartFrame, FrameRate, Float:Life, 
                Float:Width, Float:EndWidth, FadeLength, Float:Amplitude, const Color[4], Speed, Float:delay=0.0);

native VS_BeamRing(StartEntity, EndEntity, ModelIndex, HaloIndex, StartFrame, FrameRate, Float:Life, Float:Width, Float:Amplitude, const Color[4], Speed, Flags, Float:delay=0.0);

native VS_BeamFollow(EntIndex, ModelIndex, HaloIndex, Float:Life, Float:Width, Float:EndWidth, FadeLength, const Color[4], Float:delay=0.0);

native VS_Bubbles(Model, Float:minXYZ[3], Float:maxXYZ[3], Float:height, count, Float:speed);

