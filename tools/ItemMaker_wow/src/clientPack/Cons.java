package clientPack;

import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

public class Cons extends JScrollPane {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	JTextArea log;
	
	public Cons()	{
		super();
        log = new JTextArea(3,30);
        //log.setMargin(new Insets(5,5,5,5));
        log.setEditable(false);
        //this.setPreferredSize(log.getPreferredSize());
        //this.setSize(log.getPreferredSize());
        this.getViewport().add(log);
        log.append("Program is initialized");
        this.setBorder(BorderFactory.createLineBorder(Color.blue));
        this.setHorizontalScrollBarPolicy(HORIZONTAL_SCROLLBAR_NEVER);
	}
	
	public void msg(String msg)	{
		log.append("\n"+msg);
		log.setCaretPosition(log.getDocument().getLength());
	}

}
