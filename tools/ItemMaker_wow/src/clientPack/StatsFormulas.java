package clientPack;

public class StatsFormulas {
	
	Window main;

	public StatsFormulas(Window window) {
		main = window;
	}
	
	public int GetArmor()	{
		double multi = main.statsSlot.getDouble();

		double dred = main.statsType.getReduction();
		double qmulti = main.statsQuality.getDouble();
		double armor = ((Integer)main.statsLevel.getValue()*6.25+20.0)*dred*multi/((1-dred)*7.65)*qmulti;
		if(main.statsSlot.isJewel() || main.statsSlot.isWeapon())
			armor=0.0;
		return (int)Math.round(armor);
	}
	
	public double GetSpellDmg()	{
		Double ret = (Integer)main.statsLevel.getValue()*main.statsSlot.getDouble()*0.275*main.spAndDmg.getMulti()*0.75 * Math.pow(main.statsQuality.getDouble(), 0.5);
    return ret;
	}
	
	public double GetDmg()	{
		Double ret = 0.26*(Integer)main.statsLevel.getValue()*main.statsSlot.getDouble()*main.statsQuality.getDouble()*main.spAndDmg.getMulti()*0.5;
    return ret;
	}

}
