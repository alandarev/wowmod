/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package managerv2;

/**
 *
 * @author stink_000
 */
public   class Temp implements Comparable<Temp> {
    public int id, bulk=0, amount=0, skill=0, prof=0;
    Temp(int Id, int prof)  {
      id = Id;
      this.prof = prof;
    }
    
  @Override
	public int compareTo(Temp o2) {
 
		int compareQuantity = ((Temp) o2).skill / ((Temp)o2).amount;
 
		return -(this.skill/this.amount) + compareQuantity;
 
	}
}