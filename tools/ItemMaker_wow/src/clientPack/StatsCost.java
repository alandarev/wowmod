package clientPack;



import javax.swing.JLabel;


public class StatsCost extends JLabel {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	double dStats=99999.0;
	String sStats;
	Window main;
	public StatsCost(String s)	{
		super();
		sStats=s;
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
		this.setText(String.format("%s%d", sStats, Math.round(dStats)));
	}
	
}
