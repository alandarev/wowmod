package clientPack;

import javax.swing.JComboBox;

public class Type extends JComboBox {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private static String[] sSlot = { "Cloth/Spell Dmg", "Leather/Dmg", "Plate/Dmg"
	};
	private static int[] iSlot =	{ 1, 2, 3 };
	
	private static double[] dSlot = { 1.0, 0.8, 0.6 };
	
	private static double[] dmgRed = { 0.16, 0.28, 0.48 };
	
	public Type()	{
		super(sSlot);
	}
	public int getInt()	{
		return iSlot[this.getSelectedIndex()];
	}
	public double getDouble()	{
		return dSlot[this.getSelectedIndex()];
	}
	public double getReduction()	{
		return dmgRed[this.getSelectedIndex()];
	}
}
