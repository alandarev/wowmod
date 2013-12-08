package clientPack;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class Doc {
  
  Cons console;
  
	private Vector<Line> lines;
	Line end=null;
	public Doc()	{
		lines = new Vector<Line>(10000,1000);
	}

  Doc(Cons console) {
    this();
    this.console = console;
  }
	public void add(String nextLine) {
		lines.add(new Line(nextLine));
	}
	public int size()	{
		return lines.size();
	}
	public int GetLastId() {
		//Should be called asap in order to also find last }
                //Also check for duplicate id's!
                Map<Integer, Boolean> map = new HashMap<Integer, Boolean>(20000,10000);
		int max = 0;
		for(int i=0;i<lines.size()-1;i++)	{
			String line1= lines.get(i).get();
			String line2= lines.get(i+1).get();
			String args1[] = line1.split(" ");
			if((args1.length==1) && (line2.equals("{")))	{
				if(line1.startsWith("\"") && (line1.endsWith("\"")));	{
					line1=line1.substring(1, line1.length()-1);
					try	{
						int id = Integer.parseInt(line1);
						if(id>max)
							max=id;
                                                if(map.containsKey(new Integer(id)))
                                                  console.msg("WARNING: Duplicate ID: "+id);
                                                map.put(new Integer(id), Boolean.TRUE);
					}
					catch	(Exception e)	{}
				}
			}
			if(line2.equals("}"))
				end = lines.get(i+1);
		}
		return max;
	}
	
	public boolean SaveToFile(File file)	{
		try {
			FileWriter stream = new FileWriter(file);
			BufferedWriter out = new BufferedWriter(stream);
			for(int i=0;i<size();i++)	{
				out.write(lines.get(i).str+"\n");
			}
			out.close();
			stream.close();
			return true;
		} catch (IOException e) {
			return false;
		}
	}
	
	public void AddItem(Vector<Line> vec)	{
		if(end==null)	{
			GenerateFile();
			for(int i=0;i<lines.size();i++)
				if(lines.get(i).str.equals("}"))
					end = lines.get(i);
		}
		int index = lines.indexOf(end);
		System.out.println("index: "+index);
		for(int i=0;i<vec.size();i++)	{
			lines.insertElementAt(vec.get(i), index++);
		}
	}
	private void GenerateFile() {
		lines.clear();
		lines.add(new Line("//Auto Generated Items list"));
		lines.add(new Line("\"items\""));
		lines.add(new Line("{"));
		lines.add(new Line(""));
		lines.add(new Line("}"));
	}
}
