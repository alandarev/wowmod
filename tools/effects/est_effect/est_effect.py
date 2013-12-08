import es
#import wcs
import cmdlib
import playerlib
from string import zfill
from random import randint	

import log
reload(log)
        

try:
   import spe_effects
   spe0 = 1
except:
    log.log('!!! <<< To work effects requires spe and spe_effects! download and install it ' +
    'http://forums.eventscripts.com/viewtopic.php?f=125&t=44752 >>> and <<< '+
    'http://forums.eventscripts.com/viewtopic.php?f=27&t=45730&hilit=spe_effects >>>!!!')
    spe0 = 0

#################
###-> Load#######
#################
	
def load():
   if not spe0:
      log.log('Unloading spe_effect! Need install spe!!!')
      es.unload('wcs/addons/est_effect')
   #es.server.insertcmd('sm plugins unload wcs_effects')
   cmdlib.registerServerCommand('est_effect', regEffect, '')
   #cmdlib.registerServerCommand('est_Effect', regEffect, '')
   cmdlib.registerServerCommand('est_effect_01', regEffect1, '')
   cmdlib.registerServerCommand('est_effect_02', preRegEffect2, '')
   cmdlib.registerServerCommand('est_effect_03', preRegEffect3, '')
   cmdlib.registerServerCommand('est_effect_04', regEffect4, '')
   cmdlib.registerServerCommand('est_effect_05', preRegEffect3, '')
   cmdlib.registerServerCommand('est_effect_06', preRegEffect6, '')
   cmdlib.registerServerCommand('est_effect_07', preRegEffect7, '')
   cmdlib.registerServerCommand('est_effect_08', preRegEffect8, '')
   cmdlib.registerServerCommand('est_effect_09', preRegEffect9, '')
   cmdlib.registerServerCommand('est_effect_10', preRegEffect10, '')
   cmdlib.registerServerCommand('est_effect_11', preRegEffect11, '')
   cmdlib.registerServerCommand('est_effect_12', preRegEffect12, '')
   cmdlib.registerServerCommand('est_effect_13', preRegEffect13, '')
   cmdlib.registerServerCommand('est_effect_14', preRegEffect14, '')
   cmdlib.registerServerCommand('est_effect_15', preRegEffect15, '')
   cmdlib.registerServerCommand('est_effect_16', preRegEffect16, '')
   cmdlib.registerServerCommand('est_effect_17', preRegEffect17, '')
   cmdlib.registerServerCommand('est_effect_18', preRegEffect18, '')
   cmdlib.registerServerCommand('est_effect_19', preRegEffect19, '')
   cmdlib.registerServerCommand('est_effect_20', preRegEffect20, '')
   cmdlib.registerServerCommand('est_effect_21', preRegEffect21, '')
   cmdlib.registerServerCommand('est_effect_22', preRegEffect22, '')
   cmdlib.registerServerCommand('est_effect_23', preRegEffect23, '')
   cmdlib.registerServerCommand('est_effect_24', preRegEffect24, '')
   #es.ServerVar('eventscripts_noisy').set(1)

	  
def unload():
   cmdlib.unregisterServerCommand('est_effect')
   #cmdlib.unregisterServerCommand('est_Effect')
   for number in range(1, 25):
       cmdlib.unregisterServerCommand('est_effect_%s' % (zfill(number, 2)))
   """#cmdlib.unregisterServerCommand('est_effect_01')
   #cmdlib.unregisterServerCommand('est_effect_02')
   #cmdlib.unregisterServerCommand('est_effect_03')
   #cmdlib.unregisterServerCommand('est_effect_04')
   #cmdlib.unregisterServerCommand('est_effect_05')
   #cmdlib.unregisterServerCommand('est_effect_06')
   #cmdlib.unregisterServerCommand('est_effect_07')
   #cmdlib.unregisterServerCommand('est_effect_08')
   #cmdlib.unregisterServerCommand('est_effect_09')
   #cmdlib.unregisterServerCommand('est_effect_10')
   #cmdlib.unregisterServerCommand('est_effect_11')
   #cmdlib.unregisterServerCommand('est_effect_12')
   #cmdlib.unregisterServerCommand('est_effect_13')"""
   #es.ServerVar('eventscripts_noisy').set(0)

def regEffect(args):
   try:
      type = int(args[0])
   except:
      log.log('est_effect <1/2/3/6/8/7/10/4/5/9/11> <args>') 
      return	  
   if type == 1:
      regEffect1(args[1:])
   elif type == 2:
      regEffect2(args[1:])
   elif type == 3:
      regEffect3(args[1:])
   elif type == 10:
      regEffect10(args[1:])
   elif type == 4:
      regEffect4(args[1:])
   elif type == 5:
      regEffect5(args[1:])	
   elif type == 6:
      regEffect6(args[1:])	 	
   elif type == 7:
      regEffect7(args[1:])	
   elif type == 8:
      regEffect8(args[1:])	  
   elif type == 9:
      regEffect9(args[1:])	 
   elif type == 11:
      regEffect11(args[1:])	 
   elif type == 12:
      regEffect12(args[1:])	  	  
   else:
      log.log('est_effect: unkown type: %s' % type)

def preRegEffect2(args):
   if len(args) != 22 and len(args) != 18:
      log.log('est_effect 02 <player Filter> <delay> <model> <start ent> <start position "X Y Z"> <end ent> <end position "X Y Z"> '+
	  '<framerate> <life> <start width> <end width> <fade distance> <amplitude> <R> <G> <B> <A> <speed>')
      log.log('est_effect 02: get: '+' '.join(args))
      return
   if len(args) == 18:
      x, y, z = args[4].split(',')
      x1, y1, z1 = args[6].split(',')
      preRegEffect2([args[0], args[1], args[2], args[3], x, y, z, args[5], x1, y1, z1,
	  args[7], args[8], args[9], args[10], args[11], args[12], args[13], args[14], args[15],
	  args[16], args[17]]) 
      return
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])
   sid = int(args[3])
   start = (args[4], args[5], args[6])
   eid = int(args[7])
   end = (args[8], args[9], args[10])
   framerate = int(args[11])
   life = float(args[12])
   width = int(args[13])
   ewidth = int(args[14])
   fade = int(args[15])
   ampl = int(args[16])
   red = int(args[17])
   green = int(args[18])
   blue = int(args[19])
   alpha = int(args[20])
   speed = int(args[21])
   if sid != 0 and es.exists('userid', sid):
      sid = playerlib.getPlayer(sid).index
   else:
      log.log('est_effect_02: unkown id: %s !' % sid)
      return	  
   if eid != 0 and es.exists('userid', eid):
      eid = playerlib.getPlayer(eid).index
   else:
      log.log('est_effect_02: unkown id: %s !' % eid)
      return
   if ampl > 60: ampl = 60	  
   spe_effects.beamEntPoint(users, delay, sid, start, eid, end, model,
   0, 0, framerate, life, width, ewidth, fade, ampl, red, green, blue,
   alpha, speed)
   
def preRegEffect3(args):
   if len(args) != 20 and len(args) != 16:
      log.log('est_effect_03 <player Filter> <delay> <model> <start ent> <end ent> <framerate> <life> <start width> '+
	  '<end width> <fade distance> <amplitude> <R> <G> <B> <A> <speed>')
      log.log('est_effect_03: get: '+' '.join(args))
      return
   if len(args) == 16:
      x, y, z = args[3].split(',')
      x1, y1, z1 = args[4].split(',')
      preRegEffect3([args[0], args[1], args[2], x, y, z, x1, y1, z1,
	  args[5], args[6], args[7], args[8], args[9], args[10], args[11],
	  args[12], args[13], args[14], args[15]])
      return	  
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	  
   start = (args[3], args[4], args[5])
   end = (args[6], args[7], args[8])
   framerate = int(args[9])
   life = float(args[10])
   width = int(args[11])
   ewidth = int(args[12])
   fade = int(args[13])
   ampl = int(args[14])
   red = int(args[15])
   green = int(args[16])
   blue = int(args[17])
   alpha = int(args[18])
   speed = int(args[19])  
   if ampl > 60: ampl = 60   
   spe_effects.beamPoints(users, delay, start, end, model,
   0, 0, framerate, life, width, ewidth, fade, ampl, red, green, blue,
   alpha, speed)

def preRegEffect6(args):
   if len(args) != 20 and len(args) != 16:
      log.log('est_effect_06 <player Filter> <delay> <model> <start position "X Y Z"> <end position "X Y Z"> <framerate> <life> <start width>'+
	  ' <end width> <fade distance> <amplitude> <R> <G> <B> <A> <speed>')
      log.log('est_effect_06: get: '+' '.join(args))
      return	
   if len(args) == 16:
      x, y, z = args[3].split(',')
      x1, y1, z1 = args[4].split(',')
      preRegEffect6([args[0], args[1], args[2], x, y, z, x1, y1, z1,
	  args[5], args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13],
	  args[14], args[15]]) 
      return	  
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	  
   start = (args[3], args[4], args[5])
   end = (args[6], args[7], args[8])
   rate = int(args[9])
   life = float(args[10])
   width = int(args[11])
   ewidth = int(args[12])
   spread = int(args[13])
   ampl = int(args[14])
   red = int(args[15])
   green = int(args[16])
   blue = int(args[17])
   alpha = int(args[18])
   speed = int(args[19])  
   if ampl > 60: ampl = 60   
   spe_effects.beamPoints(users, delay, start, end, model,
   0, 0, rate, life, width, ewidth, spread, ampl, red, green, blue,
   alpha, speed)   

def preRegEffect7(args):
   if len(args) != 15:
      log.log('est_effect_07 <player Filter> <delay> <model> <start ent> <end ent> <framerate> <life> <width> <spread> <amplitude> <R> <G> <B> <A> <speed>')
      log.log('est_effect_07: get: '+' '.join(args))
      return	 
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])
   if not es.exists('userid', args[3]):
       log.log('est_effect_07: unkown id: %s !' % args[3])
       return	   
   StartEntityIndex = playerlib.getPlayer(args[3]).index
   if not es.exists('userid', args[4]):
       log.log('est_effect_07: unkown id: %s !' % args[4])
       return	  
   EndEntityIndex = playerlib.getPlayer(args[4]).index
   rate = int(args[5])
   life = float(args[6])
   width = int(args[7])
   spread = int(args[8])
   amplitude = int(args[9])
   red = int(args[10])
   green = int(args[11])
   blue = int(args[12])
   alpha = int(args[13])
   speed = int(args[14])
   if amplitude > 60: amplitude = 60
   spe_effects.beamRing(users, delay, StartEntityIndex, EndEntityIndex, model,
   0, 0, rate, life, width, spread, amplitude, red, green, blue, alpha, speed, 0)  

def preRegEffect8(args):
   if len(args) != 19 and len(args) != 17:
      log.log('est_effect_08 <player Filter> <delay> <model> <center "X Y Z"> <Start Radius> <End Radius> <framerate> <life> <width> <spread> <amplitude> <R> <G> <B> <A> <speed> <flags>')
      log.log('est_effect_08: get: '+' '.join(args))
      return
   if len(args) == 17:
      x, y, z = args[3].split(',')
      preRegEffect8([args[0], args[1], args[2], x, y, z, args[4], args[5], args[6], args[7],
	  args[8], args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16]]) 
      return	     
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	  
   start = (args[3], args[4], args[5])
   sradius = int(args[6])
   eradius = int(args[7])
   rate = int(args[8])
   life = float(args[9])
   width = int(args[10])
   spread = int(args[11])
   amplitude = int(args[12])
   red = int(args[13])
   green = int(args[14])
   blue = int(args[15])
   alpha = int(args[16])
   speed = int(args[17]) 
   flags = int(args[18])
   if amplitude > 60: amplitude = 60
   spe_effects.beamRingPoint(users, delay, start, sradius, eradius, model, 0, 0, rate,
   life, width, spread, amplitude, red, green, blue, alpha, speed, flags)   

def preRegEffect9(args):
   log.log('est_effect_09 - not work. Sorry...') 
   
   
def preRegEffect10(args):
   if len(args) != 14 and len(args) != 10:
      log.log('est_effect_10 <player Filter> <delay> <model> <origin "X Y Z"> <direction "X Y Z"> <R> <G> <B> <A> <Size>')
      log.log('est_effect_10: get: '+' '.join(args))
      return
   if len(args) == 10:
      x, y, z = args[3].split(',')
      x1, y1, z1 = args[4].split(',')
      preRegEffect10([args[0], args[1], args[2], x, y, z, x1, y1, z1, args[5], args[6], args[7],
	  args[8], args[9]]) 
      return	     
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	  
   start = (args[3], args[4], args[5])
   dir = (args[6], args[7], args[8])
   red = int(args[9])
   green = int(args[10])
   blue = int(args[11])
   alpha = int(args[12])
   size = int(args[13]) 
   if size < 1:
      log.log('est_effect_10: size > 1. get: %s' % size)
      return
   spe_effects.bloodSprite(users, delay, start, dir, red, green, blue, alpha, size)  
 
def preRegEffect11(args):
   if len(args) != 14 and len(args) != 10:
      log.log('est_effect_11 <player Filter> <delay> <model> <origin "X Y Z"> <direction "X Y Z"> <R> <G> <B> <A> <Amount>')
      log.log('est_effect_11: get: '+' '.join(args))
      return
   if len(args) == 10:
      x, y, z = args[3].split(',')
      x1, y1, z1 = args[4].split(',')
      preRegEffect11([args[0], args[1], args[2], x, y, z, x1, y1, z1, args[5], args[6], args[7],
	  args[8], args[9]]) 
      return	     
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	  
   start = (args[3], args[4], args[5])
   dir = (args[6], args[7], args[8])
   red = int(args[9])
   green = int(args[10])
   blue = int(args[11])
   alpha = int(args[12])
   amount = int(args[13]) 
   #if size < 1:
   #   log.log('est_effect_10: size > 1. get: %s' % size)
   #   return
   spe_effects.bloodStream(users, delay, start, dir, red, green, blue, alpha, amount)  
 
def preRegEffect12(args):
   log.log('est_effect_12 - not work. Sorry...') 
    
def preRegEffect13(args):
   if len(args) != 7 and len(args) != 5:
      log.log('est_effect_13 <player Filter> <delay> <decal> <origin "X Y Z"> <target entity index>')
      log.log('est_effect_13: get: '+' '.join(args))
      return
   if len(args) == 10:
      x, y, z = args[3].split(',')
      preRegEffect13([args[0], args[1], args[2], x, y, z, args[4]]) 
      return	     
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	  
   start = (args[3], args[4], args[5])
   index = int(args[6])
   spe_effects.worldDecal(users, delay, start, model)   

def preRegEffect14(args):
   if len(args) != 12 and len(args) != 8:
      log.log('est_effect_14 <player Filter> <delay> <model> <Min "X Y Z"> <Max "X Y Z"> <heigth> <count> <speed>')
      log.log('est_effect_14: get: '+' '.join(args))
      return
   if len(args) == 8:
      x, y, z = args[3].split(',')
      x1, y1, z1 = args[4].split(',')
      preRegEffect14([args[0], args[1], args[2], x, y, z, x1, y1, z1, args[5], args[6], args[7]]) 
      return	     
   users = str(args[0])	
   if users == '#a': users = '#all'
   fdelay = float(args[1])
   szModelPath = str(args[2])
   vStartOrigin = (args[3], args[4], args[5])
   vEndOrigin = (args[6], args[7], args[8])
   fHeight = float(args[9])
   iCount = int(args[10])
   fSpeed = float(args[11])
   spe_effects.bubbles(users, fdelay, vStartOrigin, vEndOrigin, fHeight,
   szModelPath, iCount, fSpeed)    
 
def preRegEffect15(args):
   if len(args) != 12 and len(args) != 8:
      log.log('est_effect_15 <player Filter> <delay> <model> <Min "X Y Z"> <Max "X Y Z"> <heigth> <count> <speed>')
      log.log('est_effect_15: get: '+' '.join(args))
      return
   if len(args) == 8:
      x, y, z = args[3].split(',')
      x1, y1, z1 = args[4].split(',')
      preRegEffect15([args[0], args[1], args[2], x, y, z, x1, y1, z1, args[5], args[6], args[7]]) 
      return	     
   users = str(args[0])	
   if users == '#a': users = '#all'
   fdelay = float(args[1])
   szModelPath = str(args[2])
   vStartOrigin = (args[3], args[4], args[5])
   vEndOrigin = (args[6], args[7], args[8])
   fHeight = float(args[9])
   iCount = int(args[10])
   fSpeed = float(args[11])
   spe_effects.bubbleTrail(users, fdelay, vStartOrigin, vEndOrigin, fHeight,
   szModelPath, iCount, fSpeed) 

def preRegEffect16(args):
   log.log('est_effect_16 - not work. Sorry...') 
    
def preRegEffect17(args):
   if len(args) != 10 and len(args) != 6:
      log.log('est_effect_17 <player Filter> <delay> <Position "X Y Z"> <Direction "X Y Z"> <size> <speed>')
      log.log('est_effect_17: get: '+' '.join(args))
      return
   if len(args) == 6:
      x, y, z = args[2].split(',')
      x1, y1, z1 = args[3].split(',')
      preRegEffect17([args[0], args[1], x, y, z, x1, y1, z1, args[4], args[5]]) 
      return	     
   users = str(args[0])	
   if users == '#a': users = '#all'
   fdelay = float(args[1])
   vOrigin = (args[2], args[3], args[4])
   vDirection = (args[5], args[6], args[7])
   fSize = float(args[8])
   fSpeed = float(args[9])
   spe_effects.dust(users, fdelay, vOrigin, vDirection, fSize, fSpeed)   
   
def preRegEffect18(args):
   if len(args) != 12 and len(args) != 10:
      log.log('est_effect_18 <player Filter> <delay> <Position "X Y Z"> <R> <G> <B> <exponent> <radius> <time> <decay>')
      log.log('est_effect_18: get: '+' '.join(args))
      return
   if len(args) == 10:
      x, y, z = args[2].split(',')
      preRegEffect18([args[0], args[1], x, y, z, args[4], args[5], args[6], args[7],
	  args[8], args[9], args[10]]) 
      return	     
   users = str(args[0])	
   if users == '#a': users = '#all'
   fdelay = float(args[1])
   vOrigin = (args[2], args[3], args[4])
   iRed = int(args[5])
   iGreen = int(args[6])
   iBlue = int(args[7])
   iExponent = int(args[8])
   fRadius = float(args[9])
   fTime = float(args[10])
   fDecay = int(args[11])
   spe_effects.dynamicLight(users, fdelay, vOrigin, iRed, iGreen,
   iBlue, iExponent, fRadius, fTime, fDecay)   
 
def preRegEffect19(args):
   log.log('est_effect_19 - not work. Sorry...')   
   
def preRegEffect20(args):
   log.log('est_effect_20 - not work. Sorry...')  
   
def preRegEffect21(args):
   log.log('est_effect_21 - not work. Sorry...') 
   
def preRegEffect22(args):
   log.log('est_effect_22 - not work. Sorry...')      
 
def preRegEffect23(args):
   log.log('est_effect_23 - not work. Sorry...')   

def preRegEffect24(args):
   if len(args) != 7 and len(args) != 5:
      log.log('est_effect_24 <player Filter> <delay> <model> <Position "X Y Z"> <reversed>')
      log.log('est_effect_24: get: '+' '.join(args))
      return	  
   if len(args) == 5:
      x, y, z = args[3].split(',')
      preRegEffect24([args[0], args[1], args[2], x, y, z, args[4]]) 
      return	
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	    
   start = (args[3], args[4], args[5])
   reversed = int(args[6])
   spe_effects.largeFunnel(users, delay, start, model, reversed)
 
def regEffect1(args):
   if len(args) != 9:
      log.log('est_effect 1 <player Filter> <delay> <model> <position "X Y Z" or vector> <direction "X Y Z">')
      log.log('est_effect 1: get: '+' '.join(args))
      return
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])
   origin = (args[3], args[4], args[5]) 
   direction = (args[6], args[7], args[8])	  
   spe_effects.armorRicochet(users, delay, origin, direction)   
 
   
def regEffect2(args):
   if len(args) != 12:
      log.log('est_effect 2 <player Filter> <delay> <model> <start userid> <end userid> <life> <width> <end width> <Red> <Green> <Blue> <Alpha>')
      log.log('est_effect 2: get: '+' '.join(args))
      return
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])
   sid = int(args[3])
   eid = int(args[4])
   life = float(args[5])
   width = int(args[6])
   ewidth = int(args[7])
   red = int(args[8])
   green = int(args[9])
   blue = int(args[10])
   alpha = int(args[11])
   if sid != 0 and es.exists('userid', sid):
      sid = playerlib.getPlayer(sid).index
   else:
      log.log('est_effect 2: unkown id: %s !' % sid)
      return	  
   if eid != 0 and es.exists('userid', eid):
      eid = playerlib.getPlayer(eid).index
   else:
      log.log('est_effect 2: unkown id: %s !' % eid)
      return
   spe_effects.beamEntPoint(users, delay, sid, 0, eid, 0, model,
   0, 0, 255, life, width, ewidth, 0, 0, red, green, blue,
   alpha, 0)

def regEffect3(args):
   if len(args) != 16:
      log.log('est_effect 3 <player Filter> <delay> <model> (start <X> <Y> <Z>) (end <X> <Y> <Z>) <life> <width> <end width> <Red> <Green> <Blue> <Alpha>')
      log.log('est_effect 3: get: '+' '.join(args))
      return	
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	  
   start = (args[3], args[4], args[5])
   end = (args[6], args[7], args[8])
   life = float(args[9])
   width = int(args[10])
   ewidth = int(args[11])
   red = int(args[12])
   green = int(args[13])
   blue = int(args[14])
   alpha = int(args[15])	  
   spe_effects.beamPoints(users, delay, start, end, model,
   0, 0, 255, life, width, ewidth, 0, 0, red, green, blue,
   alpha, 0)

def regEffect4(args):
   if len(args) != 11:
      log.log('est_effect 4 <player Filter> <delay> <model> <userid> <life> <width> <end width> <time to fade> <Red> <Green> <Blue> <Alpha>')
      log.log('est_effect 4: get: '+' '.join(args))
      return	
   users = str(args[0])
   if users == '#a': users = '#all'   
   fdelay = float(args[1])
   iEntityIndex = playerlib.getPlayer(args[3]).index
   szModelPath = str(args[2])
   fLife = float(args[4])
   fWidth = int(args[5])
   fEndWidth = int(args[6])
   iRed = int(args[7])
   iGreen = int(args[8])
   iBlue = int(args[9])
   iAlpha = int(args[10])
   spe_effects.beamFollow(users, fdelay, iEntityIndex, szModelPath, 0, fLife,
   fWidth, fEndWidth, 0, iRed, iGreen, iBlue, iAlpha)  

   
def regEffect5(args):
   if len(args) != 14:
      log.log('est_effect 5  <player Filter> <delay> <model> <userid> <end index> <life> <width> <spread> <amplitude> <Red> <Green> <Blue> <Alpha> <speed>')
      log.log('est_effect 5: get: '+' '.join(args))
      return	
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])
   if not es.exists('userid', args[3]):
       log.log('est_effect 5: unkown id: %s !' % args[3])
       return	   
   StartEntityIndex = playerlib.getPlayer(args[3]).index
   if not es.exists('userid', args[4]):
       log.log('est_effect 5: unkown id: %s !' % args[4])
       return	  
   EndEntityIndex = playerlib.getPlayer(args[4]).index
   life = float(args[5])
   width = int(args[6])
   spread = int(args[7])
   amplitude = int(args[8])
   red = int(args[9])
   green = int(args[10])
   blue = int(args[11])
   alpha = int(args[12])
   speed = int(args[13])
   if amplitude > 60: amplitude = 60
   spe_effects.beamRing(users, delay, StartEntityIndex, EndEntityIndex, model,
   0, 0, 255, life, width, spread, amplitude, red, green, blue, alpha, speed, 0)
  

def regEffect6(args):
   if len(args) != 7:
      log.log('est_effect 6 <player Filter> <delay> <model> <x> <y> <z> <reversed>')
      log.log('est_effect 6: get: '+' '.join(args))
      return	  
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	    
   start = (args[3], args[4], args[5])
   reversed = int(args[6])
   spe_effects.largeFunnel(users, delay, start, model, reversed)

def regEffect7(args):
   if len(args) != 8:
      log.log('est_effect 7 <player Filter> <delay> <model> <x> <y> <z> <scale> <framerate>')
      log.log('est_effect 7: get: '+' '.join(args))
      return	  
   users = str(args[0])
   if users == '#a': users = '#all'   
   fdelay = float(args[1])
   vOrigin = (args[3], args[4], args[5])
   szModelPath = str(args[2])
   fScale = float(args[6])
   iRate = int(args[7])
   spe_effects.smoke(users, fdelay, vOrigin, szModelPath, fScale, iRate) 

def regEffect8(args):
   if len(args) != 9:
      log.log('est_effect 8 <player Filter> <delay> <model> <x> <y> <z> (towards <x> <y> <z>)')
      log.log('est_effect 8: get: '+' '.join(args))
      return	  
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	 
   start = (args[3], args[4], args[5])
   end = (args[6], args[7], args[8])
   spe_effects.metalSparks(users, delay, start, end) 
 
def regEffect9(args):
   if len(args) != 10:
      log.log('est_effect 9 <player Filter> <delay> <model> <x> <y> <z> (towards <x> <y> <z>) <type>')
      log.log('est_effect 9: get: '+' '.join(args))
      return	  
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	 
   start = (args[3], args[4], args[5])
   end = (args[6], args[7], args[8])
   type = int(args[9])
   spe_effects.gaussExplosion(users, delay, start, end, type) 
    
def regEffect10(args):
   if len(args) != 17:
      log.log('est_effect 10 <player Filter> <delay> <model> <x> <y> <z> <start radius> <end radius> <life> <width> <spread> <amplitude> <Red> <Green> <Blue> <Alpha> <speed>')
      log.log('est_effect 10: get: '+' '.join(args))
      return	
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	  
   start = (args[3], args[4], args[5])
   sradius = int(args[6])
   eradius = int(args[7])
   life = float(args[8])
   width = int(args[9])
   spread = int(args[10])
   # amplitude = int(args[11])
   amplitude = float(args[11])
   red = int(args[12])
   green = int(args[13])
   blue = int(args[14])
   alpha = int(args[15])
   speed = int(args[16]) 
   if amplitude > 60: amplitude = 60
   spe_effects.beamRingPoint(users, delay, start, sradius, eradius, model, 0, 0, 255,
   life, width, spread, amplitude, red, green, blue, alpha, speed, 0)   

def regEffect11(args):
   if len(args) != 9:
      log.log('est_effect 11 <player Filter> <delay> <model> <x> <y> <z> <life> <size> <brightness>')
      log.log('est_effect 11: get: '+' '.join(args))
      return	
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	 
   start = (args[3], args[4], args[5])
   life = float(args[6])
   size = float(args[7])
   brightness = int(args[8])
   spe_effects.glowSprite(users, delay, start, model, life, size, brightness) 

def regEffect12(args):
   if len(args) != 15:
      log.log('est_effect 12 <player Filter> <delay> <model> (pos <x> <y> <z>) (angle <x> <y> <z>) (velocity <x> <y> <z>) <subtype> <flags> <randomize> ')
      log.log('est_effect 12: get: '+' '.join(args))
      return  
   users = str(args[0])
   if users == '#a': users = '#all'
   delay = float(args[1])
   model = str(args[2])	 
   start = (args[3], args[4], args[5])
   angle = (args[6], args[7], args[8])
   veloc = (args[9], args[10], args[11])
   flags = int(args[13])
   random = int(args[14])
   spe_effects.physicsProp(users, delay, model, 1, start, angle, veloc, flags, random)
   
"""
#def player_jump(ev):
  # x, y, z = playerlib.getPlayer(ev['userid']).getViewCoord().split(',')
   #r = randint(0,255)
   #g = randint(0,255)
   #b = randint(0,255)
   #es.server.insertcmd('es est_effect 4 #all 0 sprites/laser.wmt %s 1 1 2 3 255 255 0 255' % (ev['userid']))"""
 