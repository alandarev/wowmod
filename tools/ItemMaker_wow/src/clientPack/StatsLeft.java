package clientPack;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JLabel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class StatsLeft extends JLabel implements ActionListener, ChangeListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	double dStats=99999.0;
	String sStats;
	Window main;
	public StatsLeft(String s, Window main)	{
		super();
		sStats=s;
		this.main=main;
		Update();
	}
	
	public void modify(int stats)	{
		this.dStats= (double)stats;
		Update();
	}
	
	public void modify(double stats)	{
		this.dStats= stats;
		Update();
	}
	
	public void add(double stats)	{
		this.dStats += stats;
		Update();
	}
	
	public int get()	{
		return (int) Math.round(dStats);
	}
	
	private void Update()	{
		this.setText(String.format("%s%.2f", sStats, dStats));
	}

	public void actionPerformed(ActionEvent arg0) {
		main.Calculate();
	}

	@Override
	public void stateChanged(ChangeEvent arg0) {
		main.Calculate();
	}
	
}
