package clientPack;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Point;
import java.awt.Rectangle;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.JSpinner.DefaultEditor;

public class StatsDisplay extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	Stat dmg, spellDmg, armor;
	Window main;
	
	public StatsDisplay(Window m)	{
		super();
		main=m;
		this.setLayout(null);
		this.setPreferredSize(new Dimension(120,130));
		this.setBorder(BorderFactory.createRaisedBevelBorder());
		
		armor = new Stat(m, "Armor", "Bonus Armor", new Formula(0.0));
		armor.setBounds(new Rectangle(new Point(3, 35), armor.getPreferredSize()));
		armor.spinner.setEnabled(false);
		DefaultEditor editor = (DefaultEditor) armor.spinner.getEditor();   
        JTextField tf = editor.getTextField();   
        editor.getTextField().setForeground(Color.BLACK);     
        tf.setDisabledTextColor(UIManager.getColor("TextField.foreground"));   
		
		this.add(armor);
		
		dmg = new Stat(m, "Damage", "Damage", new Formula(0.0));
		dmg.setBounds(new Rectangle(new Point(3, 60), dmg.getPreferredSize()));
		dmg.spinner.setEnabled(false);
		editor = (DefaultEditor) dmg.spinner.getEditor();   
        tf = editor.getTextField();   
        editor.getTextField().setForeground(Color.BLACK);     
        tf.setDisabledTextColor(UIManager.getColor("TextField.foreground"));
		this.add(dmg);
		
		spellDmg = new Stat(m, "Spell", "Spell Damage", new Formula(0.0));
		spellDmg.setBounds(new Rectangle(new Point(3, 85), spellDmg.getPreferredSize()));
		spellDmg.spinner.setEnabled(false);
		editor = (DefaultEditor) spellDmg.spinner.getEditor();   
        tf = editor.getTextField();   
        editor.getTextField().setForeground(Color.BLACK);     
        tf.setDisabledTextColor(UIManager.getColor("TextField.foreground"));
		this.add(spellDmg);
		
		
		JLabel text = new JLabel("Item Base Stats");
		text.setFont(new Font("labelReq", Font.ITALIC, 14));
		text.setBounds(new Rectangle(new Point(10, 5), new Dimension(120,20)));
		this.add(text);
	}
	
	public void Update()	{
		dmg.Set((int)Math.round(main.formula.GetDmg()));
		armor.Set(main.formula.GetArmor());
		spellDmg.Set((int)Math.round(main.formula.GetSpellDmg()));
	}

}

