/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package managerv2;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import managerv2.item.Item;

/**
 *
 * @author alandarev
 */
class Items<T> extends ArrayList {
  Map<Integer, Integer> items = new HashMap<Integer, Integer>();
  Items () {
    super(15000);
  }

  @Override
  public boolean add(Object e) {
    Item item = (Item) e;
    items.put(item.getItemId(), super.size());
    return super.add(e);
  }
  
  
  public Object getById(Integer id)  {
    return super.get(items.get(id));
  }
  
  
  
  
}
