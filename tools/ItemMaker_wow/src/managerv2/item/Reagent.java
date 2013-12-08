/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package managerv2.item;

/**
 *
 * @author alandarev
 */
public class Reagent {
  private int itemId, amount=1;
  private boolean initialized=false;

  public int getAmount() {
    return amount;
  }

  public int getItemId() {
    return itemId;
  }

  public void setAmount(int amount) {
    this.amount = amount;
  }

  public void setItemId(int itemId) {
    initialized = true;
    this.itemId = itemId;
  }
  
  public boolean isSet() {
    return initialized;
  }
  
  
}
