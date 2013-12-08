/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package managerv2.item;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.DuplicateFormatFlagsException;
import java.util.HashMap;
import java.util.List;
import managerv2.All;
import managerv2.item.Classes;

/**
 *
 * @author alandarev
 */
public class Item {

  //Static stats

  private static String[] sSlot = { "Back", "Chest", "Feet", "Hands", "Helmet", "Legs", "Neck", "Ring",
          "Shield", "Shoulder", "Trinket", "Waist", "Weapon", "Wrist"
  };
  private static int[] iSlot =	{ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
  private static double[] dSlot = { 0.3, 1.0, 0.65, 0.5, 0.8, 0.9, 0.65, 0.6, 2.0, 0.6, 0.9, 0.5, 1.0, 0.4};
  private static final Double[] dQuality =	{ 1.0, 1.0, 1.2, 1.4, 1.7, 1.25, 1.45 };

  private HashMap<String, String> argsMap = new HashMap<String,String>(30);

  //Special
  private int itemId;
  private Classes item_classes=new Classes();
  private ItemType itemType = ItemType.SIMPLE;

  private int recipe_reagents_check=0;
  private Reagent[] recipe_reagents=null;

  private List<Integer> usable_sets = new ArrayList<Integer>();
  private Argument[] usable_args=null;

  //Standard
  private Stat<Double> weapon_speed = new Stat("ws","weaponSpeed");
  private Stat<Integer> weapon_attackPower = new Stat("weapon_ap", "weaponAP");
  private Stat<Integer> weapon_spellPower = new Stat("weapon_sp", "weaponSP");

  private Stat<String> general_name = new Stat("name");
  private Stat<String> general_description = new Stat("descr");
  private Stat<Double> general_version= new Stat("version").setValue(0.1d);
  private Stat<Integer> general_level = new Stat("level").setValue(1);
  private Stat<Integer> general_cost = new Stat("cost", "cost");
  private Stat<Integer> general_honor = new Stat("honor", "honor");
  private Stat<Integer> general_noshop = new Stat("noshop").setValue(0);
  private Stat<Integer> general_usable = new Stat("use").setValue(0);

  private Stat<Integer> item_ilevel = new Stat("ilevel");
  private Stat<Integer>  item_strength = new Stat("str", "1");
  private Stat<Integer>  item_agility = new Stat("agi", "1");
  private Stat<Integer>  item_stamina = new Stat("sta", "1");
  private Stat<Integer>  item_intellect = new Stat("int", "1");
  private Stat<Integer> general_category = new Stat("category");
  private Stat<Integer> item_armorIgnore = new Stat("ai", "0.50");
  private Stat<Integer> item_resistance = new Stat("res", "1");
  private Stat<Integer> item_manaRegeneration = new Stat("regen", "2");
  private Stat<Integer> item_resilence = new Stat("resilence", "2");
  private Stat<Integer> weapon_twoHanded = new Stat("2hand");
  private Stat<Integer> item_armor = new Stat("armor", "armor");
  private Stat<Integer> item_spellDamage = new Stat("spelldmg", "3.5");
  private Stat<Integer> item_damage = new Stat("damage", "3.5");
  private Stat<Integer> item_haste = new Stat("haste", "1");
  private Stat<Integer> general_amount = new Stat("amount");
  private Stat<Integer> item_armorType = new Stat("armor_type");
  private Stat<Integer> item_effect = new Stat("effect", "effect");

  private Stat<Double> item_attackSpeed = new Stat("as", "15 9");
  private Stat<Double> item_criticalStrike = new Stat("cs", "29.5 25");
  private Stat<Double> item_movementSpeed = new Stat("speed", "32 10");
  private Stat<Double> item_accuracy = new Stat("acc", "9.5 10");

  private Stat<Integer> recipe_requiredSkill = new Stat("reqskill");
  private Stat<Integer> recipe_itemid = new Stat("itemid");
  private Stat<Integer> recipe_grey = new Stat("grey");
  private Stat<Integer> recipe_green = new Stat("green");
  private Stat<Integer> recipe_orange = new Stat("orange");

  private Stat<Integer> general_gold = new Stat("gold");

  private Stat<String> usable_message = new Stat("message");



  private Stat<Integer> usable_set = new Stat("set");
  private Stat<Integer> usable_category2 = new Stat("category2");
  private Stat<String> usable_function = new Stat("function");
  private Stat<Integer> usable_args_check = new Stat("args");

  private Stat<Integer> general_notrade = new Stat("notrade");
  private Stat<Integer> general_soulbound = new Stat("soulbound");

  private Stat<Integer> item_sound = new Stat("sound");
  private Stat<Integer> general_token_amount = new Stat("token_amount");
  private Stat<Integer> general_token = new Stat("token");

  private List<Stat> itemStats = new ArrayList<Stat>();
  private List<Stat> standardWritableStats = new ArrayList<Stat>();

  public Item(Integer itemId) {
    this.itemId = itemId;

    itemStats.add(item_strength);
    itemStats.add(item_agility);
    itemStats.add(item_stamina);
    itemStats.add(item_intellect);
    itemStats.add(item_armorIgnore);
    itemStats.add(item_resistance);
    itemStats.add(item_manaRegeneration);
    itemStats.add(item_attackSpeed);
    itemStats.add(item_criticalStrike);
    itemStats.add(item_movementSpeed);
    itemStats.add(item_accuracy);
    itemStats.add(item_resilence);
    itemStats.add(item_haste);
    itemStats.add(item_effect);
    itemStats.add(item_spellDamage);
    itemStats.add(item_damage);

    constructStandardWritableStats();

  }

  public void compile(All all) throws IllegalStateException, IllegalArgumentException, IllegalAccessError {
    //throw new IllegalArgumentException("Not enough arguments to determine item type");
    defineItemType();

    //Fix Armor Type
    if(!item_armorType.isSet() && isWearable())  {
      fixArmorCategory(all);
    }
    
    //First thing: convert to latest version
    fixVersion();


    //Check messed up recipes
    if(itemType == ItemType.RECIPE) {
      fixRecipe(all);
    }

    //ilevel
    if(!item_ilevel.isSet())  {
      item_ilevel.setValue(general_level.getValue());
    }

    //Boost ilevel of end-game gear
    if(general_version.getValue() < 4.83d)  {
      if(general_level.getValue() > 80 && isWearable() && general_category.getValue() > 1)  {
        item_ilevel.setValue(item_ilevel.getValue() + 4);
        if(general_level.getValue() == 85)  {
          item_ilevel.setValue(item_ilevel.getValue() + 2);
        }
        fixItemPoints(all, true);
      }
    }

    //Take out stats if too many
    if(general_version.getValue() < 4.84d && isWearable())  {
      //Fix items points?
      if(general_noshop.isSet() && general_noshop.getValue() != 4)  {
        fixItemPoints(all);

        if(Math.abs(getSpentPoints() - getPreferredPoints()) > 2.5) {
          all.println(String.format("Item %d (%s) is NOT be balanced. Delta %.2f Preferred: %.2f Actual %.2f", itemId, general_name.getValue(), Math.abs(getSpentPoints() - getPreferredPoints()), getPreferredPoints(), getSpentPoints()));
        }
      }
    }

    //Check messed up recipes
    if(itemType == ItemType.RECIPE && general_version.getValue() < 4.82d) {
      nerfRecipe(all);
    }
    
    //Boost ilevel of end-game gear
    if(general_version.getValue() < 5.00d)  {
      if(general_level.getValue() == 85 && isWearable() && general_category.getValue() > 1)  {
        item_ilevel.setValue(item_ilevel.getValue() + 4);
        if(general_gold.isSet() && general_gold.getValue() > 0) {
          item_ilevel.setValue(item_ilevel.getValue() + 6);
        }
      }
    }
    
    //honor fix
    if(general_version.getValue() < 5.01d)  {
      if(general_honor.isSet() && general_honor.getValue() > 0)  {
        general_honor.setValue(general_honor.getValue() * 4);
      }
    }

    //Take out stats if too many
    if(isWearable())  {
      //Fix items points?
      if(general_noshop.isSet() && general_noshop.getValue() != 4)  {
        fixItemPoints(all);

        if(Math.abs(getSpentPoints() - getPreferredPoints()) > 2.5) {
          all.println(String.format("Item %d (%s) is NOT be balanced. Delta %.2f Preferred: %.2f Actual %.2f", itemId, general_name.getValue(), Math.abs(getSpentPoints() - getPreferredPoints()), getPreferredPoints(), getSpentPoints()));
        }
      }
    }
    
    //Weapon AP, SP
    if(general_category.isSet() && general_category.getValue() == 22)  {
      fixWeapon();
    }
    
    //Fix Armor Value
    if(item_armor.isSet() && isWearable())  {
      fixArmorValue(all);
    }

    general_version.setValue(5.02d);
    //System.out.println("Item "+ item_ilevel.getValue()+ " lvl Rating: "+ getSpentPoints() + " Supposed: "+ getPreferredPoints());
  }

  private void fixItemPoints(All all)  {
    fixItemPoints(all, false);
  }

  private void fixItemPoints(All all, boolean onlyPlus)  {
    double preferredPoints = getPreferredPoints();
    double spentPoints = getSpentPoints();
    double oldSpent = spentPoints;
    //System.out.println(String.format("Item %d preferred %.1f spent %.1f. Delta %.1f", itemId, preferredPoints, spentPoints, preferredPoints - spentPoints));
    double delta = preferredPoints - spentPoints;
    boolean stop=false;
    while (!onlyPlus && delta <= -1.0 && !stop) {
      stop=true;
      //We have overspent points, let's adjust it
      double factor = preferredPoints / getSpentPoints();
      for(Stat stat : itemStats)  {
        if(stat.isAdjustable())  {
          stop=false;
          stat.adjustFactor(factor);
          delta = preferredPoints - getSpentPoints();

          if(delta > -1.0)  {
            stop = true;
            break;
          }
        }
      }
    }

    //Now fix if it has too few stats :) rare case?
    delta = preferredPoints - getSpentPoints();
    stop=false;
    while (delta >= 0.8 && !stop) {
      stop=true;
      //We have overspent points, let's adjust it
      double factor = preferredPoints / getSpentPoints();
      for(Stat stat : itemStats)  {
        if(stat.isAdjustable())  {
          //Can we adjust it without going ouf the bounds?
          if(delta - stat.isSingleWorth(this) <= -1.0)  {
            continue;
          }

          stop=false;
          stat.adjustFactor(factor);
          delta = preferredPoints - getSpentPoints();

          if(delta < 1.0)  {
            stop = true;
            break;
          }
        }
      }
    }
    if(getSpentPoints() != oldSpent)  {
      all.println(String.format("Item %d %s was Changed by %.1f rating. and now Delta is %.1f", itemId, general_name.getValue(), getSpentPoints() - oldSpent, preferredPoints - getSpentPoints()));
    }
  }

  private void fixArmorCategory(All all) {
    //It is Equippable item but has no armor type, let's define then!

    //Special categories
    switch(general_category.getValue()) {
      case 22:  {
        item_armorType.setValue(0);
      }  break;
      case 16: case 17: case 20:  {
        item_armorType.setValue(0);
      } break;
      case 10:  {
        item_armorType.setValue(1);
      } break;
      case 18:  {
        //Shield or offhand?
        if(isForCaster()) {
          item_armorType.setValue(0);
        }
        else {
          item_armorType.setValue(3);
        }
      } break;
    }

    //Ok, if it's not yet defined then check which class it's for
    if(!item_armorType.isSet()) {
      //Check if Plate
      if(item_classes.hasClass(1) || item_classes.hasClass(4) || item_classes.hasClass(8))  {
        item_armorType.setValue(3);
      }

      //Leather?
      if(item_classes.hasClass(2) || item_classes.hasClass(6) || item_classes.hasClass(9))  {
        item_armorType.setValue(2);
      }

      //Cloth?
      if(item_classes.hasClass(3) || item_classes.hasClass(5) || item_classes.hasClass(7))  {
        //Has to be Cloth then
        item_armorType.setValue(1);
      }
    }
    //Not known yet? Let's try throwing error
    if(!item_armorType.isSet()) {
      all.println(String.format("WARN: Unknown armor type for item %d assuming cloth", itemId));
      item_armorType.setValue(1);
    }
  }

  private void fixWeapon()  {
    //Find preferred values
    weapon_attackPower.setValue(weapon_attackPower.getPreferredValue(this));
    weapon_spellPower.setValue(weapon_spellPower.getPreferredValue(this));
    if(item_spellDamage.isSet())  {
      item_spellDamage.setValue(0);
    }
    if(item_damage.isSet()) {
      item_damage.setValue(0);
    }
  }

  private void fixArmorValue(All all)  {
    //Find preferred values
    int old = item_armor.getValue();
    int n = item_armor.getPreferredValue(this);
    if(old != n)  {
      all.println(String.format("WARN: Changing armor of item %d by %d", itemId, n - old));
      item_armor.setValue(n);
    }
  }

  private void fixVersion() {
    if(general_version.getValue() <= 0.96d && general_level.isSet()) {
      general_level.setValue(general_level.getValue()*4);
      item_ilevel.setValue(general_level.getValue());
    }

    if(general_version.getValue() < 1.20d && general_category.getValue() == 22)  {
      if(item_damage.isSet()) {
        item_damage.setValue( (int)Math.round( (double)item_damage.getValue() *0.5d ));
      }
      if(item_spellDamage.isSet())  {
        item_spellDamage.setValue( (int)Math.round( (double)item_spellDamage.getValue() *0.5d ));
      }
    }


    if(general_version.getValue() < 2.0d && isWearable())  {
      if(item_classes.hasClass(1))  {
        if(item_intellect.isSet())  {
          if(!item_strength.isSet()) {
            item_strength.setValue(0);
          }
          item_strength.setValue(item_strength.getValue() + item_intellect.getValue());
          item_intellect.setValue(0);
        }
        if(item_manaRegeneration.isSet()) {
          if(!item_strength.isSet()) {
            item_strength.setValue(0);
          }
          item_strength.setValue(item_manaRegeneration.getValue()*2 + item_strength.getValue());
          item_manaRegeneration.setValue(0);
        }
      }
      else if(item_classes.hasClass(2)) {
        if(item_intellect.isSet())  {
          if(!item_agility.isSet()) {
            item_agility.setValue(0);
          }
          item_agility.setValue(item_agility.getValue() + item_intellect.getValue());
          item_intellect.setValue(0);
        }
        if(item_manaRegeneration.isSet()) {
          if(!item_agility.isSet()) {
            item_agility.setValue(0);
          }
          item_agility.setValue(item_manaRegeneration.getValue()*2 + item_agility.getValue());
          item_manaRegeneration.setValue(0);
        }
      }
    }
    if(general_version.getValue() < 2.0d)  {
      if(general_amount.isSet() && general_amount.getValue() > 5) {
        general_amount.setValue(general_amount.getValue()*5);
      }
    }
    if(general_version.getValue() < 2.5d) {
      if(item_armorIgnore.isSet())  {
        item_armorIgnore.setValue(item_armorIgnore.getValue()*2);
      }
    }

    if(general_version.getValue() < 2.40d && general_level.getValue() <= 80 && general_noshop.getValue() >= 3)  {
      if(item_armor.isSet())  {
        item_armor.setValue( (int) Math.round(item_armor.getValue()*0.8d) );
      }
    }

    if(item_armor.isSet() && general_version.getValue() < 4.0d)  {
      if(item_armor.isSet())  {
        item_armor.setValue( (int) Math.round(item_armor.getValue()*0.65d) );
      }
    }


  }

  private void fixRecipe(All all)  {
    boolean fixed=false;
    if(recipe_reagents_check == 0) {
      return; //Not crafting recipe
    }
    //Move id's to smallest possible
    int last=-1;
    for(int i=0;i<recipe_reagents.length;i++) {
      if(recipe_reagents[i].isSet())  {
        int slot = i;
        if(slot > last+1) {
          slot = last+1;
          recipe_reagents[slot] = recipe_reagents[i];
          recipe_reagents[i] = new Reagent();
          fixed=true;
        }
        last=slot;
      }
    }

    //Fix reagents amount
    if(recipe_reagents_check != last +1)  {
      recipe_reagents_check = last+1;
      fixed=true;
    }

    if(fixed) {
      all.println(String.format("WARN Item %d fixed reagents", this.getItemId()));
    }
  }

  private void nerfRecipe(All all)  {
    if(recipe_reagents_check == 0) {
      return; //Not crafting recipe
    }
    for(int i=0; i<recipe_reagents_check; i++)  {
      Reagent reagent = recipe_reagents[i];
      if(reagent.getAmount() > 1) {
        reagent.setAmount((int)Math.round(reagent.getAmount()*0.7));
      }
    }
  }



  public void addProperty(String sName, String sValue) throws IllegalArgumentException, NumberFormatException, DuplicateFormatFlagsException {
    //Anti double definition
    if(argsMap.containsKey(sName))  {
      throw new DuplicateFormatFlagsException(String.format("Key %s is already initialized for item %d", sName, itemId));
    }
    argsMap.put(sName, sValue);

    if(sName.equals("name"))  {
      general_name.setValue(sValue);
    } else if(sName.equals("descr"))  {
      general_description.setValue(sValue);
    } else if(sName.equals("version")) {
      general_version.setValue(Double.parseDouble(sValue));
    } else if(sName.equals("class")) {
      String[] scl = sValue.split(" ");
      for(String sClass : scl)  {
        item_classes.add(Integer.parseInt(sClass));
      }
    } else if(sName.equals("level")) {
      general_level.setValue(Integer.parseInt(sValue));

    } else if(sName.equals("cost")) {
      general_cost.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("honor")) {
      general_honor.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("noshop")) {
      general_noshop.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("category")) {
      general_category.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("reqstr")) {
      //Depricated
    }
    else if(sName.equals("reqsta")) {
      //Depricated
    }
    else if(sName.equals("reqint")) {
      //Depricated
    }
    else if(sName.equals("reqagi")) {
      //Depricated
    }
    else if(sName.equals("weapon_ap")) {
      weapon_attackPower.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("weapon_sp")) {
      weapon_spellPower.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("armor_type")) {
      item_armorType.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("ilevel")) {
      item_ilevel.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("str")) {
      item_strength.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("agi")) {
      item_agility.setValue( Integer.parseInt(sValue));
    }
    else if(sName.equals("sta")) {
      item_stamina.setValue( Integer.parseInt(sValue));
    }
    else if(sName.equals("int")) {
      item_intellect.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("ai")) {
      item_armorIgnore.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("res")) {
      item_resistance.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("regen")) {
      item_manaRegeneration.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("as")) {
      item_attackSpeed.setValue(Double.parseDouble(sValue));
    }
    else if(sName.equals("cs")) {
      item_criticalStrike.setValue(Double.parseDouble(sValue));
    }
    else if(sName.equals("speed")) {
      item_movementSpeed.setValue(Double.parseDouble(sValue));
    }
    else if(sName.equals("acc")) {
      item_accuracy.setValue(Double.parseDouble(sValue));
    }
    else if(sName.equals("resilence")) {
      item_resilence.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("ws")) {
      weapon_speed.setValue(Double.parseDouble(sValue));
    }
    else if(sName.equals("2hand")) {
      weapon_twoHanded.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("armor")) {
      item_armor.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("spelldmg")) {
      item_spellDamage.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("damage")) {
      item_damage.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("amount")) {
      general_amount.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("use")) {
      general_usable.setValue(Integer.parseInt(sValue));
      if(itemType == ItemType.SIMPLE) {
        itemType = ItemType.USABLE;
      }
    }
    else if(sName.equals("gold")) {
      general_gold.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("sets")) {
      String[] tokens = sValue.split(" ");
      for(String token : tokens)  {
        usable_sets.add(Integer.parseInt(token));
      }
    }
    else if (sName.equals("message")) {
      usable_message.setValue(sValue);
    }
    else if (sName.equals("effect")) {
      item_effect.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("set"))  {
      usable_set.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals(("category2")))  {
      //For which category
      usable_category2.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("notrade"))  {
      general_notrade.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("soulbound"))  {
      general_soulbound.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("haste"))  {
      item_haste.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("sound"))  {
      item_sound.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("token"))  {
      general_token.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("token_amount"))  {
      general_token_amount.setValue(Integer.parseInt(sValue));
    }

    //Recipes
    else if(sName.equals("reqskill")) {
      itemType = ItemType.RECIPE;
      recipe_requiredSkill.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("itemid")) {
      itemType = ItemType.RECIPE;
      recipe_itemid.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("reagents")) {
      itemType = ItemType.RECIPE;
      recipe_reagents_check = Integer.parseInt(sValue);
      recipe_reagents = new Reagent[10];
      for(int i=0; i < recipe_reagents.length; i++)  {
        recipe_reagents[i] = new Reagent();
      }
    }
    else if(sName.startsWith("reagent_")) {
      if(recipe_reagents_check <= 0)  {
        throw new IllegalArgumentException(String.format("Tried giving reagent %s (%s) before reagents value initialized!", sName, sValue));
      }
      String[] tokens = sName.split("_");
      if(tokens.length == 2)  {

        int index = Integer.parseInt(tokens[1])-1;
        if(recipe_reagents.length < index+1) {
          throw new IllegalArgumentException("Tried giving reagent higher than allowed "+sName+".");
        }
        recipe_reagents[index].setItemId(Integer.parseInt(sValue));
      }
      else if(tokens.length == 3 && tokens[2].equals("amount")) {
        int index = Integer.parseInt(tokens[1])-1;
        if(recipe_reagents.length < index+1) {
          throw new IllegalArgumentException("Tried giving reagent higher than allowed "+sName+".");
        }
        recipe_reagents[index].setAmount(Integer.parseInt(sValue));
      }
      else  {
        throw new IllegalArgumentException("Property "+sName+" is not recognized");
      }
    }
    else if(sName.equals("orange")) {
      itemType = ItemType.RECIPE;
      recipe_orange.setValue(Integer.parseInt(sValue));
    }
    else if(sName.equals("green")) {
      itemType = ItemType.RECIPE;
      recipe_green.setValue(Integer.parseInt(sValue));

    }
    else if(sName.equals("grey")) {
      itemType = ItemType.RECIPE;
      recipe_grey.setValue(Integer.parseInt(sValue));

    }

    //Functions
    else if(sName.equals("function")) {
      usable_function.setValue(sValue);
    }

    else if(sName.equals("args")) {
      usable_args_check.setValue(Integer.parseInt(sValue));
      usable_args = new Argument[usable_args_check.getValue()];
      for(int i=0; i < usable_args_check.getValue(); i++)  {
        usable_args[i] = new Argument();
      }
    }
    else if(sName.startsWith("arg")) {


      if(usable_args_check.getValue() <= 0)  {
        throw new IllegalArgumentException(String.format("Argument %s (%s) before arguments amount initialized!", sName, sValue));
      }
      String[] tokens = sName.split("arg");
      tokens = tokens[1].split("_");
      int index = Integer.parseInt(tokens[0])-1;
      if(usable_args.length < index+1) {
        throw new IllegalArgumentException("Argument higher than allowed "+sName+".");
      }

      if(tokens.length == 2 && tokens[1].equals("type"))  {
        usable_args[index].setArgType(sValue);
      }
      else if(tokens.length == 1) {
        usable_args[index].setArgValue(sValue);
      }
      else  {
        throw new IllegalArgumentException("Key "+sName+" is not recognized");
      }
    }

    else {
      throw new IllegalArgumentException("Property "+sName+" is not recognized");
    }
  }



  public int getItemId()  {
    return itemId;
  }

  public Stat getGeneralLevel() {
    return general_level;
  }

  public Stat getItemILevel()  {
    return item_ilevel;
  }


  public Double getQualityMulti() {
    if(!general_noshop.isSet()) {
      return dQuality[0];
    }
    else  {
      return dQuality[general_noshop.getValue()];
    }
  }

  public Double getSlotMulti() {
    if(!general_category.isSet()) {
      return 1.0d;
    }
    if(general_category.getValue() < 10 || general_category.getValue() > 23)  {
      return 1.0d;
    }
    double multi = 1.0d;
    if(general_category.getValue() == 22 && weapon_twoHanded.isSet() && weapon_twoHanded.getValue() == 1)  {
      multi *= 2.0d;
    }
    return multi * dSlot[general_category.getValue() - 10];
  }

  public boolean isForCaster() {
    if(item_classes.hasClass(3) || item_classes.hasClass(5) || item_classes.hasClass(7) ) {
      return true;
    }
    return false;
  }

  public boolean isWearable() {
    return itemType == ItemType.WEARABLE;
  }

  private double getPreferredPoints() {
    if(!general_category.isSet() || !item_ilevel.isSet()) {
      return 0d;
    }
    double points;
    double armorPointsModifier=1.0d;
    switch(item_armorType.getValue()) {
      case 0: case 1: {
        armorPointsModifier = 1.0d;
      } break;
      case 2: {
        armorPointsModifier = 0.8d;
      } break;
      case 3: {
        armorPointsModifier = 0.6d;
      } break;
    }
    points = item_ilevel.getValue() * 0.272277 * getSlotMulti() * (general_category.getValue()==22?0.7:1.0) * getQualityMulti() * armorPointsModifier;

    return points;
  }

  private double getSpentPoints() {
    double total=0d;
    if(itemType != ItemType.WEARABLE)  {
      return total;
    }

    for(Stat stat : itemStats)  {
      if(stat.isSet())  {
        total += stat.isWorth(this);
      }
    }
    return total;
  }

  private void defineItemType() {
    if(general_category.isSet() && general_category.getValue() >= 10 && general_category.getValue() <= 23)  {
      itemType = ItemType.WEARABLE;
    }
  }

  @Override
  public String toString() {
    StringBuilder sb = new StringBuilder();
    sb.append(String.format("\n\"%d\"\n{\n", itemId));

    for(Stat stat : standardWritableStats)  {
      if(stat.isSet())  {
        sb.append(stat.getWritableText()).append('\n');
      }
    }

    sb.append(item_classes.getWritableText()).append('\n');

    if(!usable_sets.isEmpty())  {
      sb.append("\"sets\" \"");
      for(Integer set : usable_sets)  {
        sb.append(String.format("%d ", set));
      }
      sb.deleteCharAt(sb.length()-1);
      sb.append("\"");
      sb.append('\n');
    }

    //Recipes
    if(recipe_reagents_check > 0) {
      sb.append(String.format("\"reagents\" \"%d\"\n", recipe_reagents_check));
      for(int i=0; i<recipe_reagents_check;i++) {
        sb.append(String.format("\"reagent_%d\" \"%d\"\n", i+1, recipe_reagents[i].getItemId()));
        sb.append(String.format("\"reagent_%d_amount\" \"%d\"\n", i+1, recipe_reagents[i].getAmount()));
      }
    }

    if(usable_args_check.isSet() && usable_args_check.getValue() > 0) {
      for(int i=0; i<usable_args_check.getValue(); i++)  {
        sb.append(String.format("\"arg%d_type\" \"%s\"\n", i+1, usable_args[i].getArgType()));
        sb.append(String.format("\"arg%d\" \"%s\"\n", i+1, usable_args[i].getArgValue()));
      }
    }

    sb.append("}");
    sb.append('\n');

    return sb.toString();
  }

  private void constructStandardWritableStats() {

    standardWritableStats.add(general_name);
    standardWritableStats.add(general_description);
    standardWritableStats.add(general_version);
    standardWritableStats.add(general_level);
    standardWritableStats.add(general_cost);
    standardWritableStats.add(general_gold);
    standardWritableStats.add(general_honor);
    standardWritableStats.add(general_noshop);
    standardWritableStats.add(general_category);
    standardWritableStats.add(general_amount);
    standardWritableStats.add(general_usable);
    standardWritableStats.add(general_notrade);
    standardWritableStats.add(general_soulbound);
    standardWritableStats.add(general_token_amount);
    standardWritableStats.add(general_token);

    standardWritableStats.add(weapon_speed);
    standardWritableStats.add(weapon_attackPower);
    standardWritableStats.add(weapon_spellPower);
    standardWritableStats.add(weapon_twoHanded);

    standardWritableStats.add(item_ilevel);
    standardWritableStats.add(item_strength);
    standardWritableStats.add(item_agility);
    standardWritableStats.add(item_stamina);
    standardWritableStats.add(item_intellect);
    standardWritableStats.add(item_armorIgnore);
    standardWritableStats.add(item_resistance);
    standardWritableStats.add(item_manaRegeneration);
    standardWritableStats.add(item_resilence);
    standardWritableStats.add(item_armor);
    standardWritableStats.add(item_spellDamage);
    standardWritableStats.add(item_damage);
    standardWritableStats.add(item_haste);
    standardWritableStats.add(item_armorType);
    standardWritableStats.add(item_effect);
    standardWritableStats.add(item_attackSpeed);
    standardWritableStats.add(item_criticalStrike);
    standardWritableStats.add(item_movementSpeed);
    standardWritableStats.add(item_accuracy);

    standardWritableStats.add(item_sound);

    standardWritableStats.add(recipe_requiredSkill);
    standardWritableStats.add(recipe_itemid);
    standardWritableStats.add(recipe_grey);
    standardWritableStats.add(recipe_green);
    standardWritableStats.add(recipe_orange);


    standardWritableStats.add(usable_message);
    standardWritableStats.add(usable_set);
    standardWritableStats.add(usable_category2);
    standardWritableStats.add(usable_function);
    standardWritableStats.add(usable_args_check);
  }

  public double getDamageReduction()  {
    if(!item_armorType.isSet()) {
      return 0d;
    }
    switch(item_armorType.getValue()) {
      case 0: {
        return 0d;
      }
      case 1: {
        return 0.16d;
      }
      case 2: {
        return 0.28d;
      }
      case 3: {
        return 0.48d;
      }
      default:  {
        return 0d;
      }
    }
  }

  public boolean IsRecipe() {
    return recipe_reagents_check > 0;
  }

  public int GetRecipeSkill()  {
      if(!IsRecipe())   return 0;
      return recipe_requiredSkill.getValue();
  }

  public Reagent[] GetRecipeReagents()   {
      return recipe_reagents;
  }

  public int GetRecipeProfession() {
    if(!IsRecipe()) return 0;
    return general_category.getValue();
  }

  public String GetName() {
    return general_name.getValue();
  }

  public int getQuality() {
    return general_noshop.getValue();
  }

  public int getLevel() {
    return general_level.getValue();
  }

}
