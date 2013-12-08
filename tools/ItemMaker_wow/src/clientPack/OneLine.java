package clientPack;

import java.awt.Component;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JTextArea;


public class OneLine implements KeyListener {
	private boolean noExtra;
	
	public OneLine(boolean extra)	{
		noExtra=extra;
	}

	public void keyPressed(KeyEvent arg0) {
	}

	public void keyReleased(KeyEvent arg0) {
	}

	@Override
	public void keyTyped(KeyEvent e) {
		if(e.getKeyChar() == '\n')	{
			Component c = e.getComponent();
			
			if(c instanceof javax.swing.JTextArea)	{
				javax.swing.JTextArea area = (JTextArea) c;
				String text = area.getText();
				area.setText(text.substring(0, text.length()-1));
			}
		}
		if(noExtra && ((e.getKeyChar() == '"' ) || (e.getKeyChar() == '\\' )))	{
			e.setKeyChar((char) (0));
		}
	}

}
