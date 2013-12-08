package clientPack;

import javax.swing.JComboBox;

public class Quality extends JComboBox {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private static String[] sQuality = { "Shop", "Green", "Rare", "Epic", "Legendary", "PVP Blue", "PVP Epic" };
	private static double[] dQuality =	{ 1.0, 1.0, 1.2, 1.7, 2.8, 1.25, 1.82 };
	private static int[] iQuality = { 0, 1, 2, 3, 4, 5, 6 };
	private static boolean[] bPvp = { false, false, false, false, false, true, true };
	
	public Quality()	{
		super(sQuality);
	}
	public double getDouble()	{
		return dQuality[this.getSelectedIndex()];
	}
	public int getInt()	{
		return iQuality[this.getSelectedIndex()];
	}
	public boolean isPvp()	{
		return bPvp[this.getSelectedIndex()];
	}
}
