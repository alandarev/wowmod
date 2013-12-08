/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package managerv2.item;

/**
 *
 * @author alandarev
 */
public class Stat<StatValue> implements WritableKey {
  private StatValue value;
  private boolean initialized=false;
  
  private final String formula;
  private final String key;
  
  
  public Stat(String key, String formula)  {
    this.key = key;
    this.formula = formula;
  }
  
  public Stat(String key) {
    this.key = key;
    formula= "";
  }
  
  public boolean isAdjustable() {
    if(!isSet())  {
      return false;
    }
    String[] values = formula.split(" ");
    try {
      Double X = Double.parseDouble(values[0]);
    } catch( NumberFormatException e) {
      return false;
    }
    if(value instanceof Integer)  {
      if((Integer)value > 0) {
        return true;
      }
    }
    if(value instanceof Double) {
      if((Double)value > 0d)  {
        return true;
      }
    }
    return false;
  }
  
  public StatValue CalculateWorth(Item item) {
    /*
     * Possibilities:
     * Formula = "cost"
     * Formula = "honor"
     * "armor" -20% (< 2.40) for epics >=80 lvl
     * "weaponSpeed"
     * "effect"
     * X
     * X Y
     * "weaponAP"
     * "weaponSP"
     */
    
    return null;
  }
  
  public Stat setValue(StatValue val)  {
    value = val;
    initialized = true;
    return this;
  }
  
  public StatValue getValue() throws IllegalAccessError {
    if(!initialized)  {
      throw new IllegalAccessError(key + " has no value!");
    }
    return value;
  }
  
  public boolean isSet()  {
    return initialized;
  }

  @Override
  public String getWritableText() {
    if(isSet()) {
      return  String.format("\"%s\"   \"%s\"", key, value.toString());
    }
    else  {
      return  String.format("\"%s\"   \"\"", key);
    }
  }
  
  public double isWorth(Item item) {
    double worth=0d;
    if(formula.equals("effect"))  {
      if((Integer) value > 0) {
        return 25.0d;
      }
    }
    String[] tokens = formula.split(" ");
    double X = Double.parseDouble(tokens[0]);
    if (tokens.length == 1) {
      worth += X * (Integer) value;
    }
    else if(tokens.length == 2) {
      double Y = Double.parseDouble(tokens[1]);
      worth += (X * 0.25 * (Integer)item.getGeneralLevel().getValue() + Y) * (Double)value;
    }
    
    return worth;
  }
  
  public double isSingleWorth(Item item) {
    double worth=0d;
    if(formula.equals("effect"))  {
      if((Integer) value > 0) {
        return 25.0d;
      }
    }
    String[] tokens = formula.split(" ");
    double X = Double.parseDouble(tokens[0]);
    if (tokens.length == 1) {
      worth += X * 1;
    }
    else if(tokens.length == 2) {
      double Y = Double.parseDouble(tokens[1]);
      worth += (X * 0.25 * (Integer)item.getGeneralLevel().getValue() + Y) * 0.01;
    }
    
    return worth;
  }

  public StatValue getPreferredValue(Item item) {
    if(formula.equals("weaponAP"))  {
      Integer ilevel;
      ilevel = (Integer)item.getItemILevel().getValue();
      Double qualityMulti = item.getQualityMulti();
      Double slotMulti = item.getSlotMulti();
      Double dValue;
      dValue = 0.26*ilevel *  slotMulti * qualityMulti * 0.5;
      
      Integer iReturn;
      iReturn = ((int)Math.round(dValue));
      return (StatValue) iReturn;
    }
    if(formula.equals("weaponSP"))  {
      Integer ilevel;
      ilevel = (Integer)item.getItemILevel().getValue();
      Double qualityMulti = item.getQualityMulti();
      Double slotMulti = item.getSlotMulti();
      Double dValue;
      dValue = ilevel *  slotMulti * 0.275 *0.75 * Math.pow(qualityMulti, 0.5);
      
      Integer iReturn;
      iReturn = ((int)Math.round(dValue));
      return (StatValue) iReturn;
    }
    if(formula.equals("armor"))  {
      Integer ilevel;
      ilevel = (Integer)item.getItemILevel().getValue();
      Double qualityMulti = item.getQualityMulti();
      Double slotMulti = item.getSlotMulti();
      Double dValue;
      dValue = (ilevel * 6.25 + 20.0) * item.getDamageReduction() * slotMulti / ((1.0 - item.getDamageReduction()) * 7.65) * qualityMulti;
      Integer iReturn;
      iReturn = ((int)Math.round(dValue));
      return (StatValue) iReturn;
    }
    
    throw new AbstractMethodError("Formula does not support Preferred Value");
  }

  void adjustFactor(double factor) {
    if( value instanceof Integer) {
      Integer val = (Integer)value;
      Integer val2 = (int) Math.round(factor * val);
      if(val.equals(val2)) {
        if(factor < 1.0) {
          val2 = val - 1;
        }
        else {
          val2 = val + 1;
        }
      }
      value = (StatValue)val2;
    }
    if( value instanceof Double)  {
      Double old = (Math.round( (Double)value * 100.0)) / 100.0;
      Double val = (Double)value;
      Double val2 =  (Math.round(val * factor * 100.0)) / 100.0;
      if(old.equals(val2)) {
        if(factor < 1.0)  {
          val2 = val - 0.01;
        }
        else  {
          val2 = val + 0.01;
        }
      }
      value = (StatValue)val2;
      
    }
    
  }
  
}
