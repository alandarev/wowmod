package clientPack;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.border.BevelBorder;

public class StatsClass extends JList {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private DefaultListModel list;
	
	public StatsClass()	{
		super();
		this.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
		list = new DefaultListModel();
		this.setModel(list);
		
		list.addElement("All");
		list.addElement("Warrior");
		list.addElement("Rogue");
		list.addElement("Mage");
		list.addElement("Paladin");
		list.addElement("Priest");
		list.addElement("Shaman");
		list.addElement("Warlock");
                list.addElement("Death Knight");
	}
	
	public int[] GetInts()	{
		return this.getSelectedIndices();
	}
}
