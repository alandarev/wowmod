package clientPack;

public class Main {
	private final static String STR_VER = "2.7";
	public static void main(String[] args)	{
		System.out.println("launching program");
		Window w = new Window("Item Manager");
    //w.setSize(600, 600);
		w.setTitle("Item Manager "+STR_VER);
		w.setVisible(true);
	}
}
