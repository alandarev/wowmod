package clientPack;

public class Formula {
	private Window main=null;
	double d1,d2;
	
	public Formula(double d)	{
		d1=d;
	}
	
	public Formula(Window m, double d, double d1)	{
		main=m;
		this.d1=d;
		d2=d1;
	}

	public double get(Integer integer) {
		if(main==null)
			return d1 * integer;
		else
			return (d1 * 0.25 * (Integer)main.statsLevel.getValue() + d2) * integer;
	}
	
	public double get(Double doub) {
		if(main==null)
			return d1 * doub;
		else
			return (d1 * 0.25 * (Integer)main.statsLevel.getValue() + d2) * doub;
	}

}
