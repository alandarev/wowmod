package clientPack;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.Point;
import java.awt.Rectangle;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class Req extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JLabel text;
	Stat reqStr, reqAgi, reqInt, reqSta;
	Window main;
	
	public Req(Window m)	{
		super();
		main=m;
		this.setLayout(null);
		this.setPreferredSize(new Dimension(250,90));
		this.setBorder(BorderFactory.createLoweredBevelBorder());
		
		reqStr = new Stat(m, "Strength", "Required Strength", new Formula(1.0));
		reqStr.setBounds(new Rectangle(new Point(3, 35), reqStr.getPreferredSize()));
		this.add(reqStr);
		
		reqAgi = new Stat(m, "Agility", "Required Agility", new Formula(1.0));
		reqAgi.setBounds(new Rectangle(new Point(3, 60), reqAgi.getPreferredSize()));
		this.add(reqAgi);
		
		reqInt = new Stat(m, "Intellect", "Required Intellect", new Formula(1.0));
		reqInt.setBounds(new Rectangle(new Point(123, 35), reqInt.getPreferredSize()));
		this.add(reqInt);
		
		reqSta = new Stat(m, "Stamina", "Required Stamina", new Formula(1.0));
		reqSta.setBounds(new Rectangle(new Point(123, 60), reqSta.getPreferredSize()));
		this.add(reqSta);
		
		text = new JLabel();
		text.setFont(new Font("labelReq", Font.ITALIC, 14));
		text.setBounds(new Rectangle(new Point(33, 5), new Dimension(200,20)));
		this.add(text);
	}
	
	public void Update()	{
		String s = String.format("Required Stats: %d/%d", getAlready(), getTotal());
		text.setText(s);
	}

	public int getAlready() {
		int sum=0;
		sum+=reqStr.ValueInt();
		sum+=reqAgi.ValueInt();
		sum+=reqInt.ValueInt();
		sum+=reqSta.ValueInt();
		return sum;
	}

	public int getTotal() {
		int lvl = (Integer) main.statsLevel.getValue();
		return (int) Math.round(15.0+0.8125*lvl);
	}

}
