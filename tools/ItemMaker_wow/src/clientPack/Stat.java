package clientPack;

import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class Stat extends JPanel implements ChangeListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private boolean doubleStat=false;
	private String statName;
	private Window main;
	private JLabel label;
	JSpinner spinner;
	private Formula fWorth;
	
	public Stat(Window m, String name, boolean d, String tooltip, Formula worth)	{
		super();
		this.setLayout(null);
		doubleStat=d;
		statName=name;
		main=m;
		label = new JLabel(statName);
		label.setBounds(new Rectangle(new Point(0, 0), label.getPreferredSize()));
		this.add(label);
		spinner = new JSpinner();
		spinner.setToolTipText(tooltip);
		
		if(d)
			spinner.setModel(new SpinnerNumberModel(0.0, null, null, 0.01));
		else
			spinner.setModel(new SpinnerNumberModel(0, null, null, 1));
		spinner.addChangeListener(this);
		fWorth = worth;
		Dimension dim = spinner.getPreferredSize();
		dim.width+=30;
		spinner.setPreferredSize(dim);
		spinner.setBounds(new Rectangle(new Point(55, 0), spinner.getPreferredSize()));
		this.add(spinner);
		this.setPreferredSize(new Dimension(115,20));
	}
	
	public Stat(Window m, String name, String tooltip, Formula worth)	{
		super();
		this.setLayout(null);
		statName=name;
		main=m;
		label = new JLabel(statName);
		label.setBounds(new Rectangle(new Point(0, 0), label.getPreferredSize()));
		this.add(label);
		spinner = new JSpinner();
		spinner.setToolTipText(tooltip);
		
		spinner.setModel(new SpinnerNumberModel(0, null, null, 1));
		
		spinner.addChangeListener(this);
		fWorth = worth;
		Dimension dim = spinner.getPreferredSize();
		dim.width+=30;
		spinner.setPreferredSize(dim);
		spinner.setBounds(new Rectangle(new Point(55, 0), spinner.getPreferredSize()));
		this.add(spinner);
		this.setPreferredSize(new Dimension(115,20));
	}

	public Stat(Window m, String name, boolean d, String tooltip, Formula worth, boolean third)	{
		super();
		this.setLayout(null);
		doubleStat=d;
		statName=name;
		main=m;
		label = new JLabel(statName);
		label.setBounds(new Rectangle(new Point(0, 0), label.getPreferredSize()));
		this.add(label);
		spinner = new JSpinner();
		spinner.setToolTipText(tooltip);
		
		if(d)
			spinner.setModel(new SpinnerNumberModel(0.0, null, null, 0.01));
		else
			spinner.setModel(new SpinnerNumberModel(0, null, null, 1));
		spinner.addChangeListener(this);
		fWorth = worth;
		Dimension dim = spinner.getPreferredSize();
		dim.width+=30;
		spinner.setPreferredSize(dim);
		if(!third)
			spinner.setBounds(new Rectangle(new Point(55, 0), spinner.getPreferredSize()));
		else
			spinner.setBounds(new Rectangle(new Point(35, 0), spinner.getPreferredSize()));
		this.add(spinner);
		this.setPreferredSize(new Dimension(115,20));
	}

	public void stateChanged(ChangeEvent arg0) {
		main.Calculate();
	}
	
	public double Worth()	{
		if(doubleStat)
			return fWorth.get((Double) spinner.getValue());
		else
			return fWorth.get((Integer) spinner.getValue());
	}
	
	public int ValueInt()	{
		return (Integer) spinner.getValue();
	}
	
	public double ValueDouble()	{
		return (Double) spinner.getValue();
	}
	
	public void Set(int v)	{
		spinner.setValue(v);
	}
}
