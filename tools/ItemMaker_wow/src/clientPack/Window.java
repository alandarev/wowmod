package clientPack;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Scanner;
import java.util.Vector;

import javax.swing.*;


/**
 * @author Jevgenij Timosenko
 */
public class Window extends JFrame implements Runnable, ActionListener {
	private DecimalFormat twoDForm = new DecimalFormat("#.##");
	private Container primaryPanel;
	private JFileChooser fc;
	private JTextArea sLoad,sSave;
	private JButton bLoad,bSave,bLoadFile,bSaveFile;
	private Doc doc = new Doc();
	private JSpinner nextId;
  private JSpinner gold;
	private OneLine oneLine,oneLineExtra;
	public static final String STR_VERSION = "2.20";
	Cons console;
	
	private JButton addItem;
	
	//Item stats
	private StatsLeft statsLeft;
	private StatsCost	statsCost, statsHonor;
	private JTextArea statsName, statsDesc;
	JSpinner statsLevel;
	private JSpinner statsWS;
	private static final long serialVersionUID = 1L;
	Quality statsQuality;
	Slot statsSlot;
	Type statsType;
	private StatsClass statsClass;
	TwoHand twoHand;
	SpAndDmg spAndDmg;
	private Req required;
	StatsFormulas formula = new StatsFormulas(this);
	
	//Integer Stats
	private Stat statsStr, statsAgi, statsInt, statsSta, statsAS, statsAI, statsRes, statsRegen,
	statsSpellDmg, statsSpeed, statsCS, statsResilence, statsAcc;
	
	//Display stats
	private StatsDisplay statsDisplay;
	

	public Window(String string) {
		super(string);
		init();
	}
	
	private void init()	{
		DecimalFormatSymbols format = new DecimalFormatSymbols();
		format.setDecimalSeparator('.');
		twoDForm.setDecimalFormatSymbols(format);
		oneLine = new OneLine(false);
		oneLineExtra = new OneLine(true);
		this.setSize(new Dimension(600,510));
		this.setResizable(true);
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.setLocationRelativeTo(null);
		primaryPanel = this.getContentPane();
		primaryPanel.setLayout(null);
		AddComponents();
		
	}
	
	private void AddComponents()	{
		JScrollPane scrollTemp;
		JLabel lTemp = new JLabel("Item Creator by Stinkyfax");
		lTemp.setBounds(new Rectangle(new Point(170, 5), lTemp.getPreferredSize()));
		primaryPanel.add(lTemp);
		
		fc = new JFileChooser();
		
		//Load button/line
		lTemp = new JLabel("Load from:");
		lTemp.setBounds(new Rectangle(new Point(10, 22), lTemp.getPreferredSize()));
		primaryPanel.add(lTemp);
		sLoad = new JTextArea(1,30);
		sLoad.setBounds(new Rectangle(new Point(80, 22), sLoad.getPreferredSize()));
		sLoad.addKeyListener(oneLine);
		scrollTemp = new JScrollPane(sLoad);
		scrollTemp.setBounds(new Rectangle(new Point(80, 22), scrollTemp.getPreferredSize()));
		scrollTemp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
		scrollTemp.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		primaryPanel.add(scrollTemp);
		
		bLoad = new JButton("Browse");
		bLoad.setBounds(new Rectangle(new Point(430, 19), bLoad.getPreferredSize()));
		bSave = new JButton("Browse");
		bSave.setBounds(new Rectangle(new Point(430, 52), bSave.getPreferredSize()));
		primaryPanel.add(bLoad);
		primaryPanel.add(bSave);
		bLoad.addActionListener(this);
		bSave.addActionListener(this);
		//Save button/line
		lTemp = new JLabel("Save to:");
		lTemp.setBounds(new Rectangle(new Point(10, 55), lTemp.getPreferredSize()));
		primaryPanel.add(lTemp);
		sSave = new JTextArea(1,30);
		sSave.addKeyListener(oneLine);
		sSave.setBounds(new Rectangle(new Point(80, 55), sSave.getPreferredSize()));
		scrollTemp = new JScrollPane(sSave);
		scrollTemp.setBounds(new Rectangle(new Point(80, 55), scrollTemp.getPreferredSize()));
		scrollTemp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
		scrollTemp.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		primaryPanel.add(scrollTemp);
		
		bLoadFile = new JButton("Load File");
		bLoadFile.addActionListener(this);
		bSaveFile = new JButton("Save Changes");
		bSaveFile.addActionListener(this);
		bLoadFile.setBounds(new Rectangle(new Point(30, 80), bLoadFile.getPreferredSize()));
		bSaveFile.setBounds(new Rectangle(new Point(120, 80), bSaveFile.getPreferredSize()));
		primaryPanel.add(bSaveFile);
		primaryPanel.add(bLoadFile);
		
		//Next ID spinner
		lTemp = new JLabel("Next Item ID:");
		lTemp.setBounds(new Rectangle(new Point(250, 83), lTemp.getPreferredSize()));
		primaryPanel.add(lTemp);
		nextId = new JSpinner();
		nextId.setModel(new SpinnerNumberModel(6000, 1, 999999, 1));
		
		nextId.setToolTipText("Item ID which will go next");
		nextId.setBounds(new Rectangle(new Point(330, 83), nextId.getPreferredSize()));
		primaryPanel.add(nextId);

		//gold spinner
		lTemp = new JLabel("Gold:");
		lTemp.setBounds(new Rectangle(new Point(435, 83), lTemp.getPreferredSize()));
		primaryPanel.add(lTemp);
		gold = new JSpinner();
		gold.setModel(new SpinnerNumberModel(0, 0, 50000, 10));

		gold.setToolTipText("Costs gold divided by 100");
		gold.setBounds(new Rectangle(new Point(470, 83), gold.getPreferredSize()));
		primaryPanel.add(gold);
		
		//Stats left
		statsLeft = new StatsLeft("Points left: ",this);
		statsLeft.setFont(new Font("labelPoints", Font.ITALIC, 18));
		statsLeft.setBounds(new Rectangle(new Point(130, 110), statsLeft.getPreferredSize()));
		primaryPanel.add(statsLeft);
		statsLeft.modify(0);
		
		//Stats Cost
		statsCost = new StatsCost("Cost: ");
		statsCost.setFont(new Font("labelCost", Font.PLAIN, 18));
		statsCost.setBounds(new Rectangle(new Point(350, 110), statsCost.getPreferredSize()));
		primaryPanel.add(statsCost);
		statsCost.modify(0);
		
		//Stats cost honor
		statsHonor = new StatsCost("Honor: ");
		statsHonor.setFont(new Font("labelCost", Font.PLAIN, 18));
		statsHonor.setBounds(new Rectangle(new Point(470, 110), statsHonor.getPreferredSize()));
		primaryPanel.add(statsHonor);
		statsHonor.modify(0);
		
		
		//ItemName
		lTemp = new JLabel("Name:");
		lTemp.setBounds(new Rectangle(new Point(10, 140), lTemp.getPreferredSize()));
		primaryPanel.add(lTemp);
		statsName = new JTextArea(1,25);
		statsName.setBounds(new Rectangle(new Point(70, 140), statsName.getPreferredSize()));
		statsName.addKeyListener(oneLineExtra);
		scrollTemp = new JScrollPane(statsName);
		scrollTemp.setBounds(statsName.getBounds());
		scrollTemp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
		scrollTemp.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		primaryPanel.add(scrollTemp);
		
		//Item Desc
		//ItemName
		lTemp = new JLabel("Descrip.:");
		lTemp.setBounds(new Rectangle(new Point(10, 160), lTemp.getPreferredSize()));
		primaryPanel.add(lTemp);
		statsDesc = new JTextArea(1,25);
		statsDesc.setBounds(new Rectangle(new Point(70, 160), statsDesc.getPreferredSize()));
		statsDesc.addKeyListener(oneLineExtra);
		scrollTemp = new JScrollPane(statsDesc);
		scrollTemp.setBounds(statsDesc.getBounds());
		scrollTemp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
		scrollTemp.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		primaryPanel.add(scrollTemp);
		
		//Level
		lTemp = new JLabel("Level:");
		lTemp.setBounds(new Rectangle(new Point(350, 140), lTemp.getPreferredSize()));
		primaryPanel.add(lTemp);
		statsLevel = new JSpinner();
		statsLevel.setModel(new SpinnerNumberModel(10, 1, 85, 1));
		statsLevel.setToolTipText("Item Level");
		statsLevel.setBounds(new Rectangle(new Point(392, 137), statsLevel.getPreferredSize()));
		primaryPanel.add(statsLevel);
		statsLevel.addChangeListener(statsLeft);
		
		//Quality
		statsQuality = new Quality();
		statsQuality.setBounds(new Rectangle(new Point(350, 165), statsQuality.getPreferredSize()));
		primaryPanel.add(statsQuality);
		statsQuality.addActionListener(statsLeft);
		
		//Slot
		statsSlot = new Slot(this);
		statsSlot.setPreferredSize(statsQuality.getPreferredSize());
		statsSlot.setBounds(new Rectangle(new Point(350, 195), statsSlot.getPreferredSize()));
		primaryPanel.add(statsSlot);
		statsSlot.addActionListener(statsLeft);
		
		//Type
		statsType = new Type();
		statsType.setPreferredSize(statsQuality.getPreferredSize());
		statsType.setBounds(new Rectangle(new Point(350, 225), statsType.getPreferredSize()));
		primaryPanel.add(statsType);
		statsType.addActionListener(statsLeft);
		
		//Class list
		statsClass = new StatsClass();
		Dimension dTemp = statsClass.getPreferredSize();
		dTemp.width=100;
		statsClass.setBounds(new Rectangle(new Point(450, 140), dTemp));
		primaryPanel.add(statsClass);
		
		//Two hand checkbox
		twoHand = new TwoHand();
		twoHand.setBounds(new Rectangle(new Point(350, 255), twoHand.getPreferredSize()));
		primaryPanel.add(twoHand);
		twoHand.addActionListener(statsLeft);
		
		//SP/DMG checkbox
		spAndDmg = new SpAndDmg();
		spAndDmg.setBounds(new Rectangle(new Point(350, 305), spAndDmg.getPreferredSize()));
		primaryPanel.add(spAndDmg);
		spAndDmg.addActionListener(statsLeft);
		
		//weapon speed
		lTemp = new JLabel("WS:");
		lTemp.setBounds(new Rectangle(new Point(350, 285), lTemp.getPreferredSize()));
		primaryPanel.add(lTemp);
		
		statsWS = new JSpinner();
		statsWS.setModel(new SpinnerNumberModel(1.00, 0.01, 3.0, 0.05));
		statsWS.setToolTipText("Weapon Speed");
		statsWS.setBounds(new Rectangle(new Point(380, 283), statsWS.getPreferredSize()));
		primaryPanel.add(statsWS);
		
		//Bonus Stats
		statsStr = new Stat(this, "Strength: ", false, "Bonus Strength", new Formula(1.0));
		statsStr.setBounds(new Rectangle(new Point(3, 190), statsStr.getPreferredSize()));
		primaryPanel.add(statsStr);
		statsAgi = new Stat(this, "Agility: ", false, "Bonus Agility", new Formula(1.0));
		statsAgi.setBounds(new Rectangle(new Point(3, 215), statsAgi.getPreferredSize()));
		primaryPanel.add(statsAgi);
		statsInt = new Stat(this, "Intellect: ", false, "Bonus Intellect", new Formula(1.0));
		statsInt.setBounds(new Rectangle(new Point(3, 240), statsInt.getPreferredSize()));
		primaryPanel.add(statsInt);
		statsSta = new Stat(this, "Stamina: ", false, "Bonus Intellect", new Formula(1.0));
		statsSta.setBounds(new Rectangle(new Point(3, 265), statsSta.getPreferredSize()));
		primaryPanel.add(statsSta);
		
		statsAS = new Stat(this, "AS: ", true, "Attack Speed", new Formula(this, 15.0, 9.0));
		statsAS.setBounds(new Rectangle(new Point(3, 290), statsAS.getPreferredSize()));
		primaryPanel.add(statsAS);
		
		statsAI = new Stat(this, "A. Ignore: ", false, "Armor Ignore", new Formula(1.5));
		statsAI.setBounds(new Rectangle(new Point(123, 190), statsAI.getPreferredSize()));
		primaryPanel.add(statsAI);
		
		statsRes = new Stat(this, "Res: ", false, "Bonus Resistance", new Formula(1.0));
		statsRes.setBounds(new Rectangle(new Point(123, 215), statsRes.getPreferredSize()));
		primaryPanel.add(statsRes);
		
		statsRegen = new Stat(this, "M Regen: ", false, "Mana Regeneration", new Formula(2.0));
		statsRegen.setBounds(new Rectangle(new Point(123, 240), statsRegen.getPreferredSize()));
		primaryPanel.add(statsRegen);
		
		statsSpellDmg = new Stat(this, "Sp. Dmg: ", false, "Spell Damage", new Formula(3.5));
		statsSpellDmg.setBounds(new Rectangle(new Point(123, 265), statsSpellDmg.getPreferredSize()));
		primaryPanel.add(statsSpellDmg);
		
		statsSpeed = new Stat(this, "MS: ", true, "Movement Speed", new Formula(this, 32.0, 10.0));
		statsSpeed.setBounds(new Rectangle(new Point(123, 290), statsSpeed.getPreferredSize()));
		primaryPanel.add(statsSpeed);
		
		statsResilence = new Stat(this, "Reslnc: ", false, "Resilence - Reduces Critical effects", new Formula(2.0));
		statsResilence.setBounds(new Rectangle(new Point(123, 315), statsResilence.getPreferredSize()));
		primaryPanel.add(statsResilence);
		
		//Stats Accuracy
		statsAcc = new Stat(this, "Acc: ", true, "Accuracy", new Formula(this, 9.5, 10.0), true);
		statsAcc.setBounds(new Rectangle(new Point(243, 190), statsAcc.getPreferredSize()));
		primaryPanel.add(statsAcc);
		
		
		statsCS = new Stat(this, "CStrike: ", true, "Critical Strike", new Formula(this, 29.5, 25.0));
		statsCS.setBounds(new Rectangle(new Point(3, 315), statsCS.getPreferredSize()));
		primaryPanel.add(statsCS);
		
		required = new Req(this);
		required.setBounds(new Rectangle(new Point(3, 340), required.getPreferredSize()));
		primaryPanel.add(required);
		
		addItem = new JButton("Create");
		addItem.setPreferredSize(new Dimension(80,80));
		addItem.setBounds(new Rectangle(new Point(255, 340), addItem.getPreferredSize()));
		primaryPanel.add(addItem);
		addItem.addActionListener(this);
		
		statsDisplay = new StatsDisplay(this);
		statsDisplay.setBounds(new Rectangle(new Point(450, 310), statsDisplay.getPreferredSize()));
		primaryPanel.add(statsDisplay);
		
		//Cons
		console = new Cons();
		console.setBounds(new Rectangle(new Point(3, 430), console.getPreferredSize()));
		primaryPanel.add(console);
		
		Calculate();
	}

	public void run() {
		
	}




	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == bLoad) {
			console.msg("Choosing file to load");
            int returnVal = fc.showOpenDialog(this);

            if (returnVal == JFileChooser.APPROVE_OPTION) {
                File file = fc.getSelectedFile();
                sLoad.setText(file.getAbsolutePath());
            }

        //Handle save button action.
        } else if (e.getSource() == bSave) {
        	console.msg("Choosing location to save file");
            int returnVal = fc.showSaveDialog(this);
            if (returnVal == JFileChooser.APPROVE_OPTION) {
                File file = fc.getSelectedFile();
                sSave.setText(file.getAbsolutePath());
            }
        } else if (e.getSource() == bLoadFile)	{
        	console.msg("Loading file");
        	if(LoadFile())
        		console.msg("File is loaded");
        	else
        		console.msg("File could not be loaded");
        } else if (e.getSource() == bSaveFile)	{
        	console.msg("Saving file");
        	if(SaveFile())
        		console.msg("File is saved");
        	else
        		console.msg("File could not be saved");
        } else if (e.getSource() == addItem)	{
        	console.msg("Adding item");
        	if(AddItem())
        		console.msg("Item \"" + statsName.getText() + "\" is added");
        	else
        		console.msg("Failed adding item");
        }
		
	}
	
	private boolean AddItem() {
		Vector<Line> lines = new Vector<Line>();
		String name = statsName.getText();
		if(name.length()<1)	{
			JOptionPane.showMessageDialog( this, "No name is entered","Warning", JOptionPane.WARNING_MESSAGE );
			return false;
		}
		int[] classes = statsClass.getSelectedIndices();
		if(classes.length<1)	{
			JOptionPane.showMessageDialog( this, "Select Class(es)","Warning", JOptionPane.WARNING_MESSAGE );
			return false;
		}
		lines.add(GenLine(Integer.toString((Integer) nextId.getValue()), null));
		lines.add(new Line("{"));
		
		lines.add(GenLine("name", name));
    lines.add(GenLine("gold", Integer.toString((Integer)gold.getValue())) );
		lines.add(GenLine("descr", statsDesc.getText()));
		
		lines.add(GenLine("version", STR_VERSION));
		
		String sClass="";
		for(int i=0;i<classes.length;i++)	{
			sClass+= Integer.toString(classes[i]);
			if(i<(classes.length-1))
				sClass+=" ";
		}
		lines.add(GenLine("class", sClass));
		lines.add(GenLine("level",Integer.toString((Integer) statsLevel.getValue())));
		lines.add(GenLine("cost", Integer.toString((Integer) statsCost.get())));
		lines.add(GenLine("honor", Integer.toString((Integer) statsHonor.get())));
		
		lines.add(GenLine("noshop", Integer.toString(statsQuality.getInt())));
		lines.add(GenLine("category", Integer.toString(statsSlot.getInt())));
		
		lines.add(GenLine("reqstr", Integer.toString(required.reqStr.ValueInt())));
		lines.add(GenLine("reqagi", Integer.toString(required.reqAgi.ValueInt())));
		lines.add(GenLine("reqint", Integer.toString(required.reqInt.ValueInt())));
		lines.add(GenLine("reqsta", Integer.toString(required.reqSta.ValueInt())));
		
		lines.add(GenLine("str", Integer.toString(statsStr.ValueInt())));
		lines.add(GenLine("agi", Integer.toString(statsAgi.ValueInt())));
		lines.add(GenLine("int", Integer.toString(statsInt.ValueInt())));
		lines.add(GenLine("sta", Integer.toString(statsSta.ValueInt())));
		
		lines.add(GenLine("ai", Integer.toString(statsAI.ValueInt())));
		lines.add(GenLine("res", Integer.toString(statsRes.ValueInt())));
		lines.add(GenLine("regen", Integer.toString(statsRegen.ValueInt())));
		lines.add(GenLine("as", twoDForm.format(statsAS.ValueDouble())));
		lines.add(GenLine("cs", twoDForm.format(statsCS.ValueDouble())));
		lines.add(GenLine("speed", twoDForm.format(statsSpeed.ValueDouble())));
		lines.add(GenLine("acc", twoDForm.format(statsAcc.ValueDouble())));
		lines.add(GenLine("resilence", Integer.toString(statsResilence.ValueInt())));
		double spellDmg=0.0;
		if(statsSlot.isWeapon())	{
			lines.add(GenLine("ws", twoDForm.format((Double) statsWS.getValue())));
			if(spAndDmg.get())	{
				double dmg = formula.GetDmg();
				lines.add(GenLine("damage", Integer.toString((int)Math.round(dmg))));
				spellDmg += formula.GetSpellDmg();
			}
			else	if(statsType.getInt()==1)	{
				spellDmg += formula.GetSpellDmg();
				
			}
			else	{
				double dmg = formula.GetDmg();
				lines.add(GenLine("damage", Integer.toString((int)Math.round(dmg))));
			}
			if(twoHand.get())	{
				lines.add(GenLine("2hand", "1"));
			}
		}
		else	{
			lines.add(GenLine("armor", Integer.toString(formula.GetArmor())));
		}
		spellDmg += statsSpellDmg.ValueInt();
		lines.add(GenLine("spelldmg", Integer.toString((int)Math.round(spellDmg))));
			
		
		lines.add(new Line("}"));
		doc.AddItem(lines);
		nextId.setValue(((Integer)nextId.getValue() + 1));
		return true;
	}
	
	private Line GenLine(String name, String arg)	{
		if(arg!=null)
			return new Line("\""+name+"\"	"+"\""+arg+"\"");
		else
			return new Line("\""+name+"\"");
	}

	private boolean SaveFile() {
		String path = sSave.getText();
		File saveFile = new File(path);
		if(!saveFile.exists())
			try {
				saveFile.createNewFile();
			} catch (IOException e) {
			}
		if(saveFile.canWrite())	{
			return doc.SaveToFile(saveFile);
		}
		return false;
	}

	private boolean LoadFile()	{
		String path = sLoad.getText();
		File loadFile = new File(path);
		if(loadFile.exists())	{
			Scanner sc=null;
			try {
				sc = new Scanner(loadFile);
			} catch (FileNotFoundException e) {
				return false;
			}
			doc = new Doc(console);
			while(sc.hasNextLine())	{
				doc.add(sc.nextLine());
			}
			sc.close();

			int id = doc.GetLastId();
			nextId.setValue(id+1);
			return true;
		}
		else
			System.out.println("no fle");
		return false;
	}

	public void Calculate() {
		//Modify Cost
		double cost = Math.pow(1.4, ((double) (Integer) statsLevel.getValue()) / 4.0);
		cost *= 31.0* Math.pow(statsQuality.getDouble(), 2.0);
		cost = cost / 10.1 * statsSlot.getDouble() * 1.8;
		statsCost.modify(cost);
		
		//Modify honor
		if(statsQuality.isPvp())	{
			cost /= 14.46;
			if(statsSlot.isWeapon())
				cost *= 2.0;
			statsHonor.modify(cost);
		}
		else
			statsHonor.modify(0);
		//Modify Points
		double points = (double) (Integer)statsLevel.getValue();
		double type = statsType.getDouble();
		if(statsSlot.isJewel())
			type=1.0;
		if(statsSlot.isWeapon())
			type=0.7;
		points *= 0.272277 * statsSlot.getDouble() * type * statsQuality.getDouble();
		points -= SpentStats();
		statsLeft.modify(points);
		
		statsDisplay.Update();
		required.Update();
		
	}
	
	public double SpentStats()	{
		double spent=0.0;
		spent+=statsStr.Worth();
		spent+=statsAgi.Worth();
		spent+=statsInt.Worth();
		spent+=statsSta.Worth();
		spent+=statsAS.Worth();
		spent+=statsAI.Worth();
		spent+=statsRes.Worth();
		spent+=statsResilence.Worth();
		spent+=statsRegen.Worth();
		spent+=statsSpellDmg.Worth();
		spent+=statsSpeed.Worth();
		spent+=statsAcc.Worth();
		spent+=statsCS.Worth();
		return spent;
	}
}

