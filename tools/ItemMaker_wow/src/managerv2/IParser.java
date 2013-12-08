/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package managerv2;

import managerv2.item.Item;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.DuplicateFormatFlagsException;
import java.util.HashMap;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.swing.SwingWorker;
import managerv2.item.Reagent;

/**
 *
 * @author alandarev
 */
class IParser extends SwingWorker {
  /**
   * 0 - initial
   * 
   */
  int state=0;
  int comments;
  int iline=1;
  final int START=0;
  final int START2=1; //Expect {
  final int NORMAL=2; //Now we can expect new item next line
  final int ITEM_START=3; //Expect { for item
  final int ITEM_NORMAL=4;  //Expect any item property now
  final int FINISH=10;  //Finished
  Item currentItem;
  private boolean stop=false;
  private boolean finished=false;
  
  private All all;
  private File file;
  IParser(All all, File file) {
    this.all = all;
    this.file = file;
  }
  
  
  @Override
  public String doInBackground() {
    

    Scanner sc=null;
    try {
      sc = new Scanner(file);
    } catch (FileNotFoundException e) {
      all.println("ERROR. File no found");
    }

    while(sc.hasNextLine()) {
      String line = sc.nextLine();
      try {
        if(iline % 50 == 0) {
          all.Header(String.format("Parsing %d - %d line", iline, iline+50));
        }
        this.parse(line);
        if(stop) {
          break;
        }
      } catch(IllegalStateException e)  {
        all.println("EXCEPTION: "+e.getMessage());
        this.failed();
        return "Error";
      }
    }
    if (!finished)  {
      this.finish();
    }
    return "Done";
  }
  
  private boolean parseAsk=true;
  boolean parse(String line) throws IllegalStateException  {
    if(isComment(line)) {
      comments++;
      iline++;
      return true;
    }
    
    //Let's remove spaces around
    line = line.trim();
    if(line.isEmpty()) {
      iline++;
      return true;
    }
    
    switch(state) {
      case START: {
        //search for "items" or BURN
        require(line, "\"[a-zA-Z]*\"");
        state=START2;
        break;
      }
      case START2:  {
        //Expect {
        require(line, "\\{");
        state=NORMAL;
        break;
      }
      case NORMAL:  {
        //Expect item ID
        if(!find(line, "\"[0-9]*\"")) {
          //Then finish it
          require(line, "\\}");
          state=FINISH;
          break;
        }
        String[] tokens = line.split("\"");
        Integer itemId = Integer.parseInt(tokens[1]);
        currentItem = new Item(itemId);
        state=ITEM_START;
        break;
      }
      case ITEM_START:  {
        require(line, "\\{");
        state=ITEM_NORMAL;
        break;
      }
      case ITEM_NORMAL: {
        //Check if it is }
        if(find(line, "\\}")) {
          try {
            currentItem.compile(all);
            all.items.add(currentItem);
          } catch(IllegalArgumentException e) {
            all.println(String.format("WARN L %d: Item %d has error: %s", iline, currentItem.getItemId(), e.getLocalizedMessage()));
          } catch(IllegalAccessError  e)  {
            all.println(String.format("WARN L %d: Illegal Access. %s",iline, e.getLocalizedMessage()));
            e.printStackTrace();
          }
          state = NORMAL;
          break;
        }
        String[] tokens = line.split("\"");
        
        if(tokens.length < 3) {
          throw new IllegalStateException(String.format("ERR L %d S %d. %s - is not item property!", iline, state, line));
        }
        String sName = tokens[1];
        String sValue="";
        if(tokens.length >= 4) {
          sValue=tokens[3];
        }
        try {
          currentItem.addProperty(sName, sValue);

        } catch(DuplicateFormatFlagsException e)  {
          all.println(String.format("WARN L %d: Duplicate! Message: %s", iline, e.getLocalizedMessage()));
        } catch(IllegalArgumentException e) {
          all.println(String.format("WARN L %d: %s", iline, e.getLocalizedMessage()));
          if(parseAsk)  {
            int answer = all.AskContinue("There are warnings in the file, Continue?");
            if(answer <=0)  {
              failed();
            }
            if(answer == 1) {
              parseAsk=false;
            }
          }
        }
        
        break;
      }
      case FINISH:  {
        //Found something after it should have finished!
        throw new IllegalStateException(String.format("State %d (FINISH). But found line %d (%s) rather then end of file.", state, iline, line));
      }
    }
    
    iline++;
    return true;
  }
  
  private void require(String line, String regex) throws IllegalStateException {
    if(!find(line, regex))  {
      this.failed();
      throw new IllegalStateException(String.format("State %d. Was expecting Regex %s on line %d (%s)", state, regex, iline, line ));
    }
  }

  private boolean isComment(String line) {
    return find(line, "^[ \\t]*\\/\\/");
    

  }
  
  private static boolean find(String line, String sPattern) {
    Pattern pattern = Pattern.compile(sPattern);
    Matcher matcher = pattern.matcher(line);

    return matcher.find();

  }

  void finish() {
    state=FINISH;
    stop=true;
    finished=true;
    all.setEnabled();
    all.println(String.format("%d Items loaded", all.items.size()));
    all.Header(String.format("Finished Parsing %d lines", iline));
    
    //Now some statistics :)
    temp();
  }
  

  void temp()   {
    HashMap<Integer, Temp> map = new HashMap<Integer, Temp>();
    for(Object oitem : all.items)    {
      Item item = (Item) oitem;
      if(!item.IsRecipe())  continue;
      Reagent[] reagents = item.GetRecipeReagents();
      for (Reagent reagent : reagents)  {
        if(reagent.isSet())   {
          if(!map.containsKey(reagent.getItemId())) {
            map.put(reagent.getItemId(), new Temp(reagent.getItemId(), item.GetRecipeProfession()));
          }
          
          Temp temp = map.get(reagent.getItemId());
          temp.bulk += reagent.getAmount();
          temp.amount ++;
          temp.skill += item.GetRecipeSkill();
          reagent.getAmount();
          reagent.getItemId();
        }
      }
    }
    
    List<Temp> list;
    list = new ArrayList<Temp>(map.values());
    java.util.Collections.sort(list);
    
    for (int i=1; i<10; i++) {
      for(Temp temp : list) {
        if(temp.prof != i)  continue;
              all.println(String.format("%d total %d skill, %d level, %d qual Prof %d Reagent %d (%s) %d times ", temp.bulk,  temp.skill / temp.amount,
                      ((Item)all.items.getById(temp.id)).getLevel(), ((Item)all.items.getById(temp.id)).getQuality(), temp.prof, temp.id, ((Item)all.items.getById(temp.id)).GetName(), temp.amount));
      }

    }
    
    all.println("Finished");
  }
  
  void failed() {
    stop=true;
    finished=true;
    all.println("ERROR. Parsing Aborted on line: "+iline);
    all.setEnabled();
    all.Header("Parsing Aborted");
  }
  
  

}
