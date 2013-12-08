package clientPack;

import javax.swing.JCheckBox;

public class SpAndDmg extends JCheckBox {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	boolean twoHand=false;
	
	public SpAndDmg()	{
		super("Sp + Dmg");
		this.setToolTipText("70% Spell Damage and 70% Damage");
		
	}
	
	public boolean get()	{
		return !(this.getSelectedObjects()==null);
	}
	
	public double getMulti()	{
		return ((this.getSelectedObjects()==null)?1.0:0.7);
	}

}
