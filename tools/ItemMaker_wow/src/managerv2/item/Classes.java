/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package managerv2.item;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

/**
 *
 * @author alandarev
 */
public class Classes implements WritableKey {
  Map<Integer,Boolean> map = new HashMap<Integer,Boolean>();
  
  public void add(Integer cl)  {
    if(cl <= 0) return;
    map.put(cl, Boolean.TRUE);
  }
  
  public boolean hasClass(Integer cl) {
    if(map.isEmpty()) {
      return true;
    }
    else {
      if(!map.containsKey(cl)) {
        return false;
      }
      return (Boolean)map.get(cl);
    }
  }

  @Override
  public String getWritableText() {
    StringBuilder sb = new StringBuilder();
    Set<Integer> cls = map.keySet();
    if(cls.isEmpty()) {
      sb.append("0");
    }
    else  {
      for(Integer cl : cls) {
        sb.append(String.format("%d ", cl));
      }
      sb.deleteCharAt(sb.length()-1);
    }
    return String.format("\"class\" \"%s\"", sb.toString());
  }
  
}
