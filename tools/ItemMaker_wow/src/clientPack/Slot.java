package clientPack;

import javax.swing.JComboBox;

public class Slot extends JComboBox {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private Window main;
	
	private static String[] sSlot = { "Back", "Chest", "Feet", "Hands", "Helmet", "Legs", "Neck", "Ring",
		"Shield", "Shoulder", "Trinket", "Waist", "Weapon", "Wrist"
	};
	private static int[] iSlot =	{ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
	private static double[] dSlot = { 0.3, 1.0, 0.65, 0.5, 0.8, 0.9, 0.65, 0.6, 2.0, 0.6, 0.9, 0.5, 1.0, 0.4};
	
	public Slot(Window main)	{
		super(sSlot);
		this.main = main;
	}
	public int getInt()	{
		return iSlot[this.getSelectedIndex()];
	}
	public double getDouble()	{
		double multi = main.twoHand.getMulti();
		multi = (getInt()==22)?multi:1.0;

		return dSlot[getSelectedIndex()]*multi;
	}
	public boolean isWeapon()	{
		return (getInt()==22)?true:false;
	}
	public boolean isShield()	{
		return (getInt()==18)?true:false;
	}
	public boolean isJewel() {
		int slot = getInt();
		switch(slot)	{
		case 16: case 17: case 20: return true;
		}
		return false;
	}
}
