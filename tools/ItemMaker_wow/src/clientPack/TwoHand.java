package clientPack;

import javax.swing.JCheckBox;

public class TwoHand extends JCheckBox {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	boolean twoHand=false;
	
	public TwoHand()	{
		super("Two Hand");
		
	}
	
	public boolean get()	{
		return !(this.getSelectedObjects()==null);
	}
	
	public double getMulti()	{
		return ((this.getSelectedObjects()==null)?1.0:2.0);
	}

}
