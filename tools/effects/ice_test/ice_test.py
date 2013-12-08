import playerlib
import es
from vecmath import Vector
from gamethread import delayed

	
def terminator1(args):
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect 3 #a 0 sprites/bluelaser1.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' '+str(x1)+' '+str(y1)+' '+str(z1+20)+' .2 2 2 0 0 255 120')
	
def Shield(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+75)+' '+str(x1)+' '+str(y1)+' '+str(z1+70)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+10)+' '+str(y)+' '+str(z+75)+' '+str(x1)+' '+str(y1)+' '+str(z1+70)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+12)+' '+str(y)+' '+str(z+80)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+75)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-12)+' '+str(y)+' '+str(z+80)+' '+str(x1-10)+' '+str(y1)+' '+str(z1+75)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+14)+' '+str(y)+' '+str(z+85)+' '+str(x1+12)+' '+str(y1)+' '+str(z1+80)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-14)+' '+str(y)+' '+str(z+85)+' '+str(x1-12)+' '+str(y1)+' '+str(z1+80)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+16)+' '+str(y)+' '+str(z+90)+' '+str(x1+14)+' '+str(y1)+' '+str(z1+85)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-16)+' '+str(y)+' '+str(z+90)+' '+str(x1-14)+' '+str(y1)+' '+str(z1+85)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+18)+' '+str(y)+' '+str(z+105)+' '+str(x1+16)+' '+str(y1)+' '+str(z1+90)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-18)+' '+str(y)+' '+str(z+105)+' '+str(x1-16)+' '+str(y1)+' '+str(z1+90)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+14)+' '+str(y)+' '+str(z+107)+' '+str(x1+18)+' '+str(y1)+' '+str(z1+105)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-14)+' '+str(y)+' '+str(z+107)+' '+str(x1-18)+' '+str(y1)+' '+str(z1+105)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+12)+' '+str(y)+' '+str(z+112)+' '+str(x1+14)+' '+str(y1)+' '+str(z1+107)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-12)+' '+str(y)+' '+str(z+112)+' '+str(x1-14)+' '+str(y1)+' '+str(z1+107)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-12)+' '+str(y)+' '+str(z+112)+' '+str(x1+12)+' '+str(y1)+' '+str(z1+112)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-6)+' '+str(y)+' '+str(z+78)+' '+str(x1)+' '+str(y1)+' '+str(z1+75)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-12)+' '+str(y)+' '+str(z+91)+' '+str(x1-6)+' '+str(y1)+' '+str(z1+78)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-13)+' '+str(y)+' '+str(z+100)+' '+str(x1-12)+' '+str(y1)+' '+str(z1+91)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+102)+' '+str(x1-13)+' '+str(y1)+' '+str(z1+100)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-8)+' '+str(y)+' '+str(z+107)+' '+str(x1-10)+' '+str(y1)+' '+str(z1+102)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+8)+' '+str(y)+' '+str(z+107)+' '+str(x1-8)+' '+str(y1)+' '+str(z1+107)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+6)+' '+str(y)+' '+str(z+78)+' '+str(x1)+' '+str(y1)+' '+str(z1+75)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+12)+' '+str(y)+' '+str(z+91)+' '+str(x1+6)+' '+str(y1)+' '+str(z1+78)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+13)+' '+str(y)+' '+str(z+100)+' '+str(x1+12)+' '+str(y1)+' '+str(z1+91)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+10)+' '+str(y)+' '+str(z+102)+' '+str(x1+13)+' '+str(y1)+' '+str(z1+100)+' .2 2 2 255 255 255 120')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+8)+' '+str(y)+' '+str(z+107)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+102)+' .2 2 2 255 255 255 120')

def streamtest01(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z)+' 0 150 .3 20 5 1 255 0 0 255 100')
	es.server.queuecmd('est_effect 10 #a .2 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z)+' 0 50 .3 4 10 1 255 0 0 255 100')
	es.server.queuecmd('est_effect 10 #a .4 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+15)+' 0 50 .3 4 10 1 255 0 0 255 100')
	es.server.queuecmd('est_effect 10 #a .6 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+25)+' 0 50 .3 4 10 1 255 0 0 255 100')
	es.server.queuecmd('est_effect 10 #a .8 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' 0 50 .3 4 10 1 255 0 0 255 100')
	es.server.queuecmd('est_effect 3 #a .9 sprites/lgtning.vmt '+str(x-40)+' '+str(y)+' '+str(z+70)+' '+str(x1+40)+' '+str(y1)+' '+str(z1+70)+' .2 2 2 255 0 0 120')
	es.server.queuecmd('est_effect 3 #a .9 sprites/lgtning.vmt '+str(x+40)+' '+str(y)+' '+str(z+70)+' '+str(x1)+' '+str(y1)+' '+str(z1+100)+' .2 2 2 255 0 0 120')
	es.server.queuecmd('est_effect 3 #a .9 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+100)+' '+str(x1-40)+' '+str(y1)+' '+str(z1+70)+' .2 2 2 255 0 0 120')
	es.server.queuecmd('est_effect 3 #a .9 sprites/lgtning.vmt '+str(x-40)+' '+str(y)+' '+str(z+70)+' '+str(x1+20)+' '+str(y1)+' '+str(z1+85)+' .2 2 2 255 0 0 120')
	es.server.queuecmd('est_effect 3 #a .9 sprites/lgtning.vmt '+str(x-20)+' '+str(y)+' '+str(z+85)+' '+str(x1+40)+' '+str(y1)+' '+str(z1+70)+' .2 2 2 255 0 0 120')
	
def streamtest02(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	x2,y2,z2 = args[0]
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1+20)+' '+str(y1)+' '+str(z1+40)+' .2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+20)+' '+str(y)+' '+str(z+40)+' '+str(x1)+' '+str(y1)+' '+str(z1+80)+' .2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1-20)+' '+str(y1)+' '+str(z1+40)+' .2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-20)+' '+str(y)+' '+str(z+40)+' '+str(x1)+' '+str(y1)+' '+str(z1+80)+' .2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+80)+' '+str(x2)+' '+str(y2)+' '+str(z2+60)+' .2 2 2 255 255 255 255')
	
def streamtest03(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 200 201 2 1 1 1 255 255 255 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 220 221 2 1 1 1 255 255 255 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1+20)+' 200 201 2 1 1 1 255 255 255 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1+20)+' 220 221 2 1 1 1 255 255 255 255 0')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1+100)+' '+str(y1)+' '+str(z1+20)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+100)+' '+str(y)+' '+str(z+20)+' '+str(x1+110)+' '+str(y1)+' '+str(z1+20)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+110)+' '+str(y)+' '+str(z+21)+' '+str(x1+110)+' '+str(y1)+' '+str(z1)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-110)+' '+str(y)+' '+str(z)+' '+str(x1+110)+' '+str(y1)+' '+str(z1)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1-100)+' '+str(y1)+' '+str(z1+20)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-100)+' '+str(y)+' '+str(z+20)+' '+str(x1-110)+' '+str(y1)+' '+str(z1+20)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-110)+' '+str(y)+' '+str(z+21)+' '+str(x1-110)+' '+str(y1)+' '+str(z1)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1+100)+' '+str(z1+20)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y+100)+' '+str(z+20)+' '+str(x1)+' '+str(y1+110)+' '+str(z1+20)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y+110)+' '+str(z+21)+' '+str(x1)+' '+str(y1+100)+' '+str(z1)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y-110)+' '+str(z)+' '+str(x1)+' '+str(y1+110)+' '+str(z1)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1-100)+' '+str(z1+20)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y-100)+' '+str(z+20)+' '+str(x1)+' '+str(y1-110)+' '+str(z1+20)+' 2 2 2 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y-110)+' '+str(z+21)+' '+str(x1)+' '+str(y1-110)+' '+str(z1)+' 2 2 2 255 255 255 255')
	
	
def streamtest04(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+320)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+360)+' .1 5 5 255 255 50 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+320)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+320)+' .1 5 5 255 255 50 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+10)+' '+str(y)+' '+str(z+320)+' '+str(x1-10)+' '+str(y1)+' '+str(z1+280)+' .1 5 5 255 255 50 255')
	delayed(.1, es.server.queuecmd, ('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+240)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+280)+' .1 5 5 255 255 50 255'))
	delayed(.1, es.server.queuecmd, ('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+240)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+240)+' .1 5 5 255 255 50 255'))
	delayed(.1, es.server.queuecmd, ('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+200)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+240)+' .1 5 5 255 255 50 255'))
	delayed(.2, es.server.queuecmd, ('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+160)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+200)+' .1 5 5 255 255 50 255'))
	delayed(.2, es.server.queuecmd, ('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+160)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+160)+' .1 5 5 255 255 50 255'))
	delayed(.2, es.server.queuecmd, ('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+120)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+160)+' .1 5 5 255 255 50 255'))
	delayed(.3, es.server.queuecmd, ('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+80)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+120)+' .1 5 5 255 255 50 255'))
	delayed(.3, es.server.queuecmd, ('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+80)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+80)+' .1 5 5 255 255 50 255'))
	delayed(.3, es.server.queuecmd, ('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-10)+' '+str(y)+' '+str(z+40)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+80)+' .1 5 5 255 255 50 255'))
	delayed(.4, es.server.queuecmd, ('est_effect 10 #a 0 sprites/glow_test02.vmt '+str(x)+' '+str(y)+' '+str(z)+' 10 201 .1 10 80 1 255 255 50 255 255'))
	delayed(.5, es.server.queuecmd, ('est_effect 10 #a 0 sprites/glow_test02.vmt '+str(x)+' '+str(y)+' '+str(z)+' 100 201 .1 30 80 1 255 255 50 255 255'))
	
def streamtest05(args):
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect_06 #a 0 sun/overlay.vmt '+str(Vector(x-30,y,z))+' '+str(Vector(x1-30,y1,z1+80))+' 10 .4 10 10 80 1 255 255 255 255 200')
	es.server.queuecmd('est_effect_06 #a 0 sun/overlay.vmt '+str(Vector(x+30,y,z))+' '+str(Vector(x1+30,y1,z1+80))+' 10 .4 10 10 80 1 255 255 255 255 200')
	es.server.queuecmd('est_effect 10 #a 0 sprites/glow_test02.vmt '+str(x)+' '+str(y)+' '+str(z)+' 10 201 .4 10 80 1 255 255 255 255 255')
	delayed(.3, es.server.queuecmd, ('est_effect_06 #a 0 sprites/glow_test02.vmt '+str(Vector(x-30,y,z+200))+' '+str(Vector(x-30,y,z))+' 10 .4 10 10 80 1 255 255 255 255 200'))
	delayed(.3, es.server.queuecmd, ('est_effect 10 #a 0 sprites/glow_test02.vmt '+str(x)+' '+str(y)+' '+str(z+150)+' 10 201 .4 10 80 1 255 255 255 255 255'))
	delayed(.3, es.server.queuecmd, ('est_effect_06 #a 0 sprites/glow_test02.vmt '+str(Vector(x+30,y,z+200))+' '+str(Vector(x+30,y,z))+' 10 .4 10 10 80 1 255 255 255 255 200'))
	
def streamtest06(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1-40)+' '+str(y1)+' '+str(z1+60)+' 1 .1 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 1 .1 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1+40)+' '+str(y1)+' '+str(z1+60)+' 1 .1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1+40)+' '+str(y1)+' '+str(z1+60)+' 1 5 5 255 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1-40)+' '+str(y1)+' '+str(z1+60)+' 1 5 5 255 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x-35)+' '+str(y)+' '+str(z+75)+' '+str(x1-40)+' '+str(y1)+' '+str(z1+60)+' 1 5 5 255 89 205 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1-35)+' '+str(y1)+' '+str(z1+75)+' 1 .1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x-20)+' '+str(y)+' '+str(z+85)+' '+str(x1-35)+' '+str(y1)+' '+str(z1+75)+' 1 5 5 255 89 205 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1-20)+' '+str(y1)+' '+str(z1+85)+' 1 .1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x-10)+' '+str(y)+' '+str(z+85)+' '+str(x1-20)+' '+str(y1)+' '+str(z1+85)+' 1 5 5 255 89 205 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1-10)+' '+str(y1)+' '+str(z1+85)+' 1 .1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+70)+' '+str(x1-10)+' '+str(y1)+' '+str(z1+85)+' 1 5 5 255 89 205 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1)+' '+str(y1)+' '+str(z1+70)+' 1 .1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x+35)+' '+str(y)+' '+str(z+75)+' '+str(x1+40)+' '+str(y1)+' '+str(z1+60)+' 1 5 5 255 89 205 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1+35)+' '+str(y1)+' '+str(z1+75)+' 1 .1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x+20)+' '+str(y)+' '+str(z+85)+' '+str(x1+35)+' '+str(y1)+' '+str(z1+75)+' 1 5 5 255 89 205 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1+20)+' '+str(y1)+' '+str(z1+85)+' 1 .1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x+10)+' '+str(y)+' '+str(z+85)+' '+str(x1+20)+' '+str(y1)+' '+str(z1+85)+' 1 5 5 255 89 205 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x1+10)+' '+str(y1)+' '+str(z1+85)+' 1 .1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+70)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+85)+' 1 5 5 255 89 205 255')

def streamtest07(args):
	x1,y1,z1 = args[0]
	x,y,z = args[1]
	x2,y2,z2 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 decals/blood1.vmt  '+str(x2)+' '+str(y2)+' '+str(z2+30)+' '+str(x)+' '+str(y)+' '+str(z)+' .3 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 decals/blood1.vmt  '+str(x2)+' '+str(y2+50)+' '+str(z2+50)+' '+str(x)+' '+str(y-50)+' '+str(z+80)+' .3 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 decals/blood1.vmt  '+str(x2)+' '+str(y2+50)+' '+str(z2+80)+' '+str(x)+' '+str(y-50)+' '+str(z+50)+' .3 10 10 255 255 255 255')
	es.server.queuecmd('est_effect_06 #a 0 decals/blood1.vmt '+str(Vector(x,y,z+20))+' '+str(Vector(x1,y1,z1+20))+' 10 .3 10 10 80 1 105 255 255 255 255')
	es.server.queuecmd('est_effect_18 #a 0 '+str(x)+' '+str(y)+' '+str(z)+' 255 0 0 20 90 2 400')

def streamtest08(args):
	x1,y1,z1 = args[1]
	x,y,z = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1)+' '+str(z1+280)+' '+str(x)+' '+str(y)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1+50)+' '+str(z1+200)+' '+str(x)+' '+str(y-50)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1+100)+' '+str(z1+280)+' '+str(x+40)+' '+str(y+100)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1+50)+' '+str(z1+200)+' '+str(x+40)+' '+str(y+150)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1-100)+' '+str(z1+280)+' '+str(x+40)+' '+str(y-100)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1-50)+' '+str(z1+200)+' '+str(x+40)+' '+str(y-150)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1)+' '+str(z1+280)+' '+str(x)+' '+str(y)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1+50)+' '+str(z1+200)+' '+str(x)+' '+str(y-50)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1+100)+' '+str(z1+280)+' '+str(x+40)+' '+str(y+100)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1+50)+' '+str(z1+200)+' '+str(x+40)+' '+str(y+150)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1-100)+' '+str(z1+280)+' '+str(x+40)+' '+str(y-100)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1-50)+' '+str(z1+200)+' '+str(x+40)+' '+str(y-150)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1)+' '+str(z1+280)+' '+str(x)+' '+str(y)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1+50)+' '+str(z1+200)+' '+str(x)+' '+str(y-50)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1+100)+' '+str(z1+280)+' '+str(x+40)+' '+str(y+100)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1+50)+' '+str(z1+200)+' '+str(x+40)+' '+str(y+150)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1-100)+' '+str(z1+280)+' '+str(x+40)+' '+str(y-100)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1-50)+' '+str(z1+200)+' '+str(x+40)+' '+str(y-150)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1)+' '+str(z1+280)+' '+str(x)+' '+str(y)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1+50)+' '+str(z1+200)+' '+str(x)+' '+str(y-50)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1+100)+' '+str(z1+280)+' '+str(x+40)+' '+str(y+100)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1+50)+' '+str(z1+200)+' '+str(x+40)+' '+str(y+150)+' '+str(z+200)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1-100)+' '+str(z1+280)+' '+str(x+40)+' '+str(y-100)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+40)+' '+str(y1-50)+' '+str(z1+200)+' '+str(x+40)+' '+str(y-150)+' '+str(z+200)+' 1 5 5 105 89 205 255')

def streamtest09(args):
	x1,y1,z1 = args[1]
	x,y,z = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1)+' '+str(z1+280)+' '+str(x)+' '+str(y)+' '+str(z)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1+40)+' '+str(z1+280)+' '+str(x)+' '+str(y-40)+' '+str(z+80)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1-40)+' '+str(z1+280)+' '+str(x)+' '+str(y+40)+' '+str(z+80)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1+40)+' '+str(z1+280)+' '+str(x)+' '+str(y+40)+' '+str(z+80)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1-40)+' '+str(z1+280)+' '+str(x)+' '+str(y-40)+' '+str(z+80)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1+40)+' '+str(z1+180)+' '+str(x)+' '+str(y)+' '+str(z+80)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1-40)+' '+str(z1+180)+' '+str(x)+' '+str(y)+' '+str(z+80)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1+40)+' '+str(z1+180)+' '+str(x)+' '+str(y-40)+' '+str(z+180)+' 1 5 5 105 89 205 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y+20)+' '+str(z+160)+' 1 .2 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y-20)+' '+str(z+160)+' 1 .2 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y+20)+' '+str(z+120)+' 1 .2 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y-20)+' '+str(z+120)+' 1 .2 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y)+' '+str(z+120)+' 1 .2 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y+15)+' '+str(z+112)+' 1 .2 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y-15)+' '+str(z+112)+' 1 .2 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y+10)+' '+str(z+120)+' 1 .2 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y-10)+' '+str(z+120)+' 1 .2 255')
	
def streamtest10(args):
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 200 201 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+10)+' 190 191 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+20)+' 180 181 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+30)+' 170 171 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+40)+' 160 161 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' 150 151 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+60)+' 140 141 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+70)+' 130 131 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+80)+' 120 121 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+90)+' 110 111 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+100)+' 100 101 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+110)+' 90 91 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+120)+' 80 81 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+130)+' 70 71 3.7 16 1 0 139 69 0 255 0')
	es.server.queuecmd('est_effect 11 #a 0 sprites/fireburst.vmt '+str(x1)+' '+str(y1)+' '+str(z1+250)+' 2 1.5 255')
	delayed(1, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1+20)+' '+str(z1+250)+' .3 1.5 255'))
	delayed(1.3, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1+40)+' '+str(z1+210)+' .3 1.5 255'))
	delayed(1.6, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1+60)+' '+str(z1+170)+' .3 1.5 255'))
	delayed(1.9, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1+80)+' '+str(z1+130)+' .3 1.5 255'))
	delayed(2.2, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1+100)+' '+str(z1+90)+' .3 1.5 255'))
	delayed(2.5, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1+120)+' '+str(z1+50)+' .3 1.5 255'))
	delayed(2.8, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1+140)+' '+str(z1+10)+' .3 1.5 255'))
	delayed(2.8, es.server.queuecmd, ('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1+140)+' '+str(z1)+' 80 81 1 16 1 0 139 69 0 255 0'))
	delayed(1, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1-20)+' '+str(z1+250)+' .3 1.5 255'))
	delayed(1.3, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1-40)+' '+str(z1+210)+' .3 1.5 255'))
	delayed(1.6, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1-60)+' '+str(z1+170)+' .3 1.5 255'))
	delayed(1.9, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1-80)+' '+str(z1+130)+' .3 1.5 255'))
	delayed(2.2, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1-100)+' '+str(z1+90)+' .3 1.5 255'))
	delayed(2.5, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1-120)+' '+str(z1+50)+' .3 1.5 255'))
	delayed(2.8, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1)+' '+str(y1-140)+' '+str(z1+10)+' .3 1.5 255'))
	delayed(2.8, es.server.queuecmd, ('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1-140)+' '+str(z1)+' 80 81 1 16 1 0 139 69 0 255 0'))
	delayed(1, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1+20)+' '+str(y1)+' '+str(z1+250)+' .3 1.5 255'))
	delayed(1.3, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1+40)+' '+str(y1)+' '+str(z1+210)+' .3 1.5 255'))
	delayed(1.6, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1+60)+' '+str(y1)+' '+str(z1+170)+' .3 1.5 255'))
	delayed(1.9, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1+80)+' '+str(y1)+' '+str(z1+130)+' .3 1.5 255'))
	delayed(2.2, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1+100)+' '+str(y1)+' '+str(z1+90)+' .3 1.5 255'))
	delayed(2.5, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1+120)+' '+str(y1)+' '+str(z1+50)+' .3 1.5 255'))
	delayed(2.8, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1+140)+' '+str(y1)+' '+str(z1+10)+' .3 1.5 255'))
	delayed(2.8, es.server.queuecmd, ('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1+140)+' '+str(y1)+' '+str(z1)+' 80 81 1 16 1 0 139 69 0 255 0'))
	delayed(1, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1-20)+' '+str(y1)+' '+str(z1+250)+' .3 1.5 255'))
	delayed(1.3, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1-40)+' '+str(y1)+' '+str(z1+210)+' .3 1.5 255'))
	delayed(1.6, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1-60)+' '+str(y1)+' '+str(z1+170)+' .3 1.5 255'))
	delayed(1.9, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1-80)+' '+str(y1)+' '+str(z1+130)+' .3 1.5 255'))
	delayed(2.2, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1-100)+' '+str(y1)+' '+str(z1+90)+' .3 1.5 255'))
	delayed(2.5, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1-120)+' '+str(y1)+' '+str(z1+50)+' .3 1.5 255'))
	delayed(2.8, es.server.queuecmd, ('est_effect 11 #a 0 sprites/glow07.vmt '+str(x1-140)+' '+str(y1)+' '+str(z1+10)+' .3 1.5 255'))
	delayed(2.8, es.server.queuecmd, ('est_effect 10 #a 0 sprites/spectator_eye.vmt '+str(x1-140)+' '+str(y1)+' '+str(z1)+' 80 81 1 16 1 0 139 69 0 255 0'))
	
	
def streamtest11(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/spectator_eye.vmt '+str(x1)+' '+str(y1)+' '+str(z1+30)+' '+str(x)+' '+str(y)+' '+str(z+100)+' .3 15 15 0 0 255 255')
	
def streamtest12(args):
	x,y,z = args[0]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' '+str(x)+' '+str(y)+' '+str(z+50)+' .1 3 3 105 89 205 255')

def streamtest13(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect_06 #a 0 sprites/physbeam.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1,y1,z1+300))+' 0 .3 22 25 0 0 105 89 205 255 5')
	es.server.queuecmd('est_effect 10 #a 0 sprites/physbeam.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 10 100 .3 30 10 0 45 75 255 255 5')
	
def streamtest14(args):
	x1,y1,z1 = args[1]
	x,y,z = args[0]
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 30 1200 .5 5 10 0 45 75 255 255 255')
	delayed(.5, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 1200 1100 .1 110 110 0 45 75 255 255 255'))
	delayed(.6, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 1100 1000 .1 100 100 1 45 75 255 255 255'))
	delayed(.7, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 900 800 .1 90 90 2 45 75 255 255 255'))
	delayed(.8, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 800 700 .1 80 80 3 45 75 255 255 255'))
	delayed(.9, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 700 600 .1 70 70 4 45 75 255 255 255'))
	delayed(1, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 600 500 .1 60 60 5 45 75 255 255 255'))
	delayed(1.1, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 500 400 .1 50 50 6 45 75 255 255 255'))
	delayed(1.2, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 400 300 .1 40 40 7 45 75 255 255 255'))
	delayed(1.3, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 300 200 .1 30 30 8 45 75 255 255 255'))
	delayed(1.4, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 200 100 .1 20 20 9 45 75 255 255 255'))
	delayed(1.5, es.server.queuecmd, ('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+5)+' 100 30 .1 10 10 10 45 75 255 255 255'))
	
	
def stream3_4(args):
	x1,y1,z1 = args[1]
	x2,y2,z2 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+1000)+' '+str(y2+50)+' '+str(z2)+' 1 1 1 0 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+1000)+' '+str(y2+550)+' '+str(z2)+' 1 1 1 0 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+1000)+' '+str(y2+1050)+' '+str(z2)+' 1 1 1 0 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+1000)+' '+str(y2+1550)+' '+str(z2)+' 1 1 1 0 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-2000)+' '+str(y2+50)+' '+str(z2)+' 1 1 1 255 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-2000)+' '+str(y2+550)+' '+str(z2)+' 1 1 1 255 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-2000)+' '+str(y2+1050)+' '+str(z2)+' 1 1 1 255 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-2000)+' '+str(y2+1550)+' '+str(z2)+' 1 1 1 255 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-2000)+' '+str(y2-50)+' '+str(z2)+' 1 1 1 255 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-2000)+' '+str(y2-550)+' '+str(z2)+' 1 1 1 255 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-2000)+' '+str(y2-1050)+' '+str(z2)+' 1 1 1 255 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-2000)+' '+str(y2-1550)+' '+str(z2)+' 1 1 1 255 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+1000)+' '+str(y2-50)+' '+str(z2)+' 1 1 1 0 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+1000)+' '+str(y2-550)+' '+str(z2)+' 1 1 1 0 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+1000)+' '+str(y2-1050)+' '+str(z2)+' 1 1 1 0 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+1000)+' '+str(y2-1550)+' '+str(z2)+' 1 1 1 0 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-400)+' '+str(y2+500)+' '+str(z2+500)+' 1 1 1 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-300)+' '+str(y2+500)+' '+str(z2+500)+' 1 1 1 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-200)+' '+str(y2+500)+' '+str(z2+500)+' 1 1 1 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-100)+' '+str(y2+500)+' '+str(z2+500)+' 1 1 1 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2)+' '+str(y2+500)+' '+str(z2+500)+' 1 1 1 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+100)+' '+str(y2+500)+' '+str(z2+500)+' 1 1 1 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+200)+' '+str(y2+500)+' '+str(z2+500)+' 1 1 1 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+300)+' '+str(y2+500)+' '+str(z2+500)+' 1 1 1 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+400)+' '+str(y2+500)+' '+str(z2+500)+' 1 1 1 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-400)+' '+str(y2-500)+' '+str(z2+500)+' 1 1 1 0 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-300)+' '+str(y2-500)+' '+str(z2+500)+' 1 1 1 0 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-200)+' '+str(y2-500)+' '+str(z2+500)+' 1 1 1 0 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2-100)+' '+str(y2-500)+' '+str(z2+500)+' 1 1 1 0 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2)+' '+str(y2-500)+' '+str(z2+500)+' 1 1 1 0 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+100)+' '+str(y2-500)+' '+str(z2+500)+' 1 1 1 0 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+200)+' '+str(y2-500)+' '+str(z2+500)+' 1 1 1 0 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+300)+' '+str(y2-500)+' '+str(z2+500)+' 1 1 1 0 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x2+400)+' '+str(y2-500)+' '+str(z2+500)+' 1 1 1 0 255 0 255')

	
def stream3_3(args):
	x,y,z = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+1)+' 45 46 2 1 1 0 45 75 255 255 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+11)+' 85 86 2 1 1 0 45 75 255 255 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+27)+' 115 116 2 1 1 0 45 75 255 255 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+44)+' 135 136 2 1 1 0 45 75 255 255 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+62)+' 155 156 2 1 1 0 45 75 255 255 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+81)+' 135 136 2 1 1 0 45 75 255 255 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+101)+' 115 116 2 1 1 0 45 75 255 255 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+115)+' 85 86 2 1 1 0 45 75 255 255 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+129)+' 45 46 2 1 1 0 45 75 255 255 50')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y)+' '+str(z+65)+' 2 2 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/strider_blackball.vmt '+str(x)+' '+str(y)+' '+str(z+65)+' 2 1.5 255')

def stream3_1(args):
	x2,y2,z2 = args[0]
	x1,y1,z1 = args[1]
	delayed(.4, es.server.queuecmd, ('est_effect_06 #a 0 sprites/crystal_beam1.vmt '+str(Vector(x1,y1,z1))+' '+str(Vector(x2,y2,z2+300))+' 100 0.2 22 25 0 0 228 128 28 255 50'))
	delayed(.2, es.server.queuecmd, ('est_effect_06 #a 0 sprites/crystal_beam1.vmt '+str(Vector(x2,y2,z2+300))+' '+str(Vector(x2,y2,z2))+' 10 0.2 22 25 0 0 228 128 28 255 50'))
	es.server.queuecmd('est_effect_06 #a 0 sprites/crystal_beam1.vmt '+str(Vector(x1,y1,z1))+' '+str(Vector(x2,y2,z2))+' 10 0.2 22 25 0 0 228 128 28 255 50')
	delayed(.2, es.server.queuecmd, ('est_effect 10 #a 0 sprites\crystal_beam1.vmt '+str(x2)+' '+str(y2)+' '+str(z2+50)+' 139 140 0.2 30 0 1 228 128 28 255 255'))
	delayed(.2, es.server.queuecmd, ('est_effect 10 #a 0 sprites\crystal_beam1.vmt '+str(x2)+' '+str(y2)+' '+str(z2+105)+' 139 140 0.2 30 0 1 228 128 28 255 255'))
	delayed(.2, es.server.queuecmd, ('est_effect 10 #a 0 sprites\crystal_beam1.vmt '+str(x2)+' '+str(y2)+' '+str(z2+165)+' 139 140 0.2 30 0 1 228 128 28 255 255'))

def streamtest15(args):
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect_06 #a 0 sprites/glow.vmt '+str(Vector(x,y,z+40))+' '+str(Vector(x1,y1,z1))+' 100 .3 15 10 10 0 0 255 0 255 100')
	es.server.queuecmd('est_effect_14 #a 0 models\crow.mdl '+str(Vector(x,y,z+40))+' '+str(Vector(x,y,z+40))+' 50 10 300')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physbeam.vmt '+str(Vector(x,y,z+40))+' '+str(Vector(x1,y1,z1))+' 100 .3 15 10 10 0 255 255 255 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites\cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+70)+' 10 200 0.1 1 5 0 0 255 0 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites\cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+80)+' 10 200 0.1 1 5 0 0 255 0 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites\cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+90)+' 10 200 0.1 1 5 0 0 255 0 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites\cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 10 200 0.1 1 5 0 0 255 0 255 100')
	delayed(.2, es.server.queuecmd, ('est_effect 10 #a 0 sprites\cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+70)+' 10 200 0.1 1 5 0 0 255 0 255 100'))
	delayed(.2, es.server.queuecmd, ('est_effect 10 #a 0 sprites\cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+90)+' 10 200 0.1 1 5 0 0 255 0 255 100'))
	delayed(.2, es.server.queuecmd, ('est_effect 10 #a 0 sprites\cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+80)+' 10 200 0.1 1 5 0 0 255 0 255 100'))
	delayed(.2, es.server.queuecmd, ('est_effect 10 #a 0 sprites\cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 10 200 0.1 1 5 0 0 255 0 255 100'))
	
def old3_2(args):
	x,y,z = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+20)+' '+str(y)+' '+str(z)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+25)+' '+str(y)+' '+str(z+10)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+30)+' '+str(y)+' '+str(z+20)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+35)+' '+str(y)+' '+str(z+30)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+40)+' '+str(y)+' '+str(z+40)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+45)+' '+str(y)+' '+str(z+50)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+40)+' '+str(y)+' '+str(z+60)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+35)+' '+str(y)+' '+str(z+70)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+30)+' '+str(y)+' '+str(z+80)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+25)+' '+str(y)+' '+str(z+90)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x+20)+' '+str(y)+' '+str(z+100)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-20)+' '+str(y)+' '+str(z)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-25)+' '+str(y)+' '+str(z+10)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-30)+' '+str(y)+' '+str(z+20)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-35)+' '+str(y)+' '+str(z+30)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-40)+' '+str(y)+' '+str(z+40)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-45)+' '+str(y)+' '+str(z+50)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-40)+' '+str(y)+' '+str(z+60)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-35)+' '+str(y)+' '+str(z+70)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-30)+' '+str(y)+' '+str(z+80)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-25)+' '+str(y)+' '+str(z+90)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x-20)+' '+str(y)+' '+str(z+100)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+20)+' '+str(z)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+25)+' '+str(z+10)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+30)+' '+str(z+20)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+35)+' '+str(z+30)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+40)+' '+str(z+40)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+45)+' '+str(z+50)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+40)+' '+str(z+60)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+35)+' '+str(z+70)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+30)+' '+str(z+80)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+25)+' '+str(z+90)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y+20)+' '+str(z+100)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-20)+' '+str(z)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-25)+' '+str(z+10)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-30)+' '+str(z+20)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-35)+' '+str(z+30)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-40)+' '+str(z+40)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-45)+' '+str(z+50)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-40)+' '+str(z+60)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-35)+' '+str(z+70)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-30)+' '+str(z+80)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-25)+' '+str(z+90)+' 1 0.3 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueflare1.vmt '+str(x)+' '+str(y-20)+' '+str(z+100)+' 1 0.3 255')
	
def streamtest16(args):
	x,y,z = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' 50 51 2 20 20 0 255 80 25 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' 100 101 2 20 20 0 255 80 25 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+25)+' 100 101 2 20 20 0 255 80 25 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 100 101 2 20 20 0 255 80 25 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+75)+' 100 101 2 20 20 0 255 80 25 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+75)+' 100 101 2 20 20 0 255 80 25 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+75)+' 50 51 2 20 20 0 255 80 25 255 255 ')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 2 3 255')

def old2_1(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+10)+' 300 301 2 20 20 0 255 255 255 100 10')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+30)+' 250 251 2 20 20 0 255 255 255 100 10')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' 200 201 2 20 20 0 255 255 255 100 10')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+70)+' 150 151 2 20 20 0 255 255 255 100 10')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+90)+' 100 101 2 20 20 0 255 255 255 100 10')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+110)+' 50 51 2 80 20 0 255 255 255 100 10')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+10)+' 300 301 2 20 20 0 255 255 255 100 10')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' '+str(x1)+' '+str(y1)+' '+str(z1+1000)+' 2 50 50 177 177 177 50')
	
def old1_3(args):
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect 7 #a 0 sprites/smoke.vmt '+str(x)+' '+str(y)+' '+str(z+100)+' 100 12')
	es.server.queuecmd('est_effect_06 #a 0 sprites\lgtning.vmt '+str(Vector(x,y,z+100))+' '+str(Vector(x1,y1,z1+40))+' 80 0.2 15 1 10 10 210 255 255 255 255')
	
def old1_2(args):
	x1,y1,z1 = args[1]
	x,y,z = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+40)+' 1 800 0.8 20 20 0 65 80 255 255 10')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+40)+' 1 800 0.2 20 20 0 65 80 255 255 10')
	delayed(.1, es.server.queuecmd, ('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 1 1200 1 80 20 0 65 80 255 255 1'))
	es.server.queuecmd('est_effect_06 #a 0 sprites/plasmabeam.vmt '+str(Vector(x+100,y+500,z+800))+' '+str(Vector(x1,y1,z1))+' 80 1.1 15 1 10 0 65 80 255 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/plasmabeam.vmt '+str(Vector(x+200,y-500,z+800))+' '+str(Vector(x1,y1,z1))+' 80 1.1 15 1 10 0 65 80 255 255 255')
	
def old1_1(args):
	x1,y1,z1 = args[1]
	x,y,z = args[0]
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+20)+' '+str(x1)+' '+str(y1)+' '+str(z1+20)+' .3 2 5 65 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 800 1 .3 20 20 0 65 80 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1+40)+' 1 800 .3 20 20 0 65 255 255 80 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1+80)+' 800 1 .3 20 20 0 65 80 255 255 255')
	delayed(.3, es.server.queuecmd, ('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+20)+' '+str(x1)+' '+str(y1)+' '+str(z1+20)+' .3 2 5 65 255 255 255'))
	delayed(.3, es.server.queuecmd, ('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 1 800 .3 20 20 0 65 80 255 255 255'))
	delayed(.3, es.server.queuecmd, ('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1+40)+' 800 1 .3 20 20 0 65 255 255 80 255'))
	delayed(.3, es.server.queuecmd, ('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1+80)+' 1 800 .3 20 20 0 65 80 255 255 255'))

def streamtest17(args):
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	x2,y2,z2 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+40)+' '+str(y)+' '+str(z+50)+' '+str(x1)+' '+str(y1)+' '+str(z1+20)+' 1 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-40)+' '+str(y)+' '+str(z+50)+' '+str(x1)+' '+str(y1)+' '+str(z1+20)+' 1 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 1 80 1 10 2 0 255 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x2+42)+' '+str(y2)+' '+str(z2+48)+' '+str(x-2)+' '+str(y)+' '+str(z+80)+' 1 10 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x2-42)+' '+str(y2)+' '+str(z2+48)+' '+str(x+2)+' '+str(y)+' '+str(z+80)+' 1 10 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x2)+' '+str(y2)+' '+str(z2+50)+' '+str(x)+' '+str(y)+' '+str(z+80)+' 1 10 5 255 255 255 255')

def streamtest18(args):
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect_14 #a 0 effects/fire_cloud2.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1,y1,z1))+' 150 20 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/fire2.vmt '+str(x)+' '+str(y)+' '+str(z)+' .8 2 1000')
	es.server.queuecmd('est_effect 10 #a 0 sprites/sprite_fire01.vmt '+str(x)+' '+str(y)+' '+str(z+40)+' 400 0 1 10 4 0 255 255 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/sprite_fire01.vmt '+str(x)+' '+str(y)+' '+str(z+40)+' 0 400 1 10 4 0 255 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/plasmabeam.vmt '+str(x1)+' '+str(y1)+' '+str(z1+20)+' '+str(x)+' '+str(y)+' '+str(z+20)+' .5 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/plasmabeam.vmt '+str(x1)+' '+str(y1)+' '+str(z1+20)+' '+str(x)+' '+str(y)+' '+str(z+20)+' .5 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/plasmabeam.vmt '+str(x1)+' '+str(y1)+' '+str(z1+20)+' '+str(x)+' '+str(y)+' '+str(z+20)+' .5 1 1 255 255 255 255')
	
def juk1(args):
	x1,y1,z1 = args[1]
	x,y,z = args[0]
	es.server.queuecmd('est_effect 3 #a 0 effects/blueblacklargebeam.vmt '+str(x1)+' '+str(y1)+' '+str(z1+30)+' '+str(x)+' '+str(y)+' '+str(z+50)+' .5 20 20 255 255 255 150')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 400 450 .5 80 50 0 255 20 147 255 255')
	es.server.queuecmd('est_effect 11 #a 0 effects/muzzleflash2_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1+45)+' .5 1 255')
	es.server.queuecmd('est_effect 11 #a 0 effects/muzzleflash2_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1+45)+' .5 1 255')
	es.server.queuecmd('est_effect 11 #a 0 effects/muzzleflash2_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1+45)+' .5 1 255')
	
def juk3(args):
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect 3 #a 0 sprites/physcannon_blueglow.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' '+str(x)+' '+str(y)+' '+str(z+50)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physcannon_blueglow.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' '+str(x)+' '+str(y+50)+' '+str(z+90)+' 2 5 5 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physcannon_blueglow.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' '+str(x)+' '+str(y-50)+' '+str(z+30)+' 2 5 5 0 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physcannon_blueglow.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' '+str(x+90)+' '+str(y+10)+' '+str(z+50)+' 2 5 5 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/physcannon_blueglow.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' '+str(x+80)+' '+str(y+80)+' '+str(z+50)+' 2 5 5 0 255 255 255')
	es.server.queuecmd('est_effect_14 #a 0 effects/blueflare1.vmt '+str(Vector(x,y, z))+' '+str(Vector(x1,y1,z1))+' 80 200 100')
	es.server.queuecmd('est_effect_14 #a 0 effects\yellowflare_noz.vmt '+str(Vector(x,y, z))+' '+str(Vector(x1,y1,z1))+' 80 200 20')
	es.server.queuecmd('est_effect_14 #a 0 effects\redflare.vmt '+str(Vector(x1,y1,z1))+' '+str(Vector(x,y,z))+' 80 200 50')
	
def Done(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1+50)+' '+str(z1+70)+' '+str(x-50)+' '+str(y+50)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1-50)+' '+str(z1+70)+' '+str(x+50)+' '+str(y-50)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1+50)+' '+str(z1+70)+' '+str(x+50)+' '+str(y-50)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1+50)+' '+str(z1+70)+' '+str(x-50)+' '+str(y-50)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1+50)+' '+str(z1)+' '+str(x-50)+' '+str(y+50)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1+50)+' '+str(z1)+' '+str(x+50)+' '+str(y+50)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1-50)+' '+str(z1)+' '+str(x-50)+' '+str(y-50)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1-50)+' '+str(z1)+' '+str(x+50)+' '+str(y-50)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1+50)+' '+str(z1+70)+' '+str(x)+' '+str(y)+' '+str(z)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1-50)+' '+str(z1+70)+' '+str(x)+' '+str(y)+' '+str(z)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1-50)+' '+str(z1+70)+' '+str(x)+' '+str(y)+' '+str(z)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1+50)+' '+str(z1+70)+' '+str(x)+' '+str(y)+' '+str(z)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1+50)+' '+str(z1)+' '+str(x)+' '+str(y)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1-50)+' '+str(z1)+' '+str(x)+' '+str(y)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1-50)+' '+str(z1)+' '+str(x)+' '+str(y)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1+50)+' '+str(z1)+' '+str(x)+' '+str(y)+' '+str(z+70)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1+50)+' '+str(z1)+' '+str(x-50)+' '+str(y+50)+' '+str(z)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1-50)+' '+str(z1)+' '+str(x+50)+' '+str(y-50)+' '+str(z)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1+50)+' '+str(y1+50)+' '+str(z1)+' '+str(x+50)+' '+str(y-50)+' '+str(z)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1-50)+' '+str(y1+50)+' '+str(z1)+' '+str(x-50)+' '+str(y-50)+' '+str(z)+' 2 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 11 #a 0 vgui/hud/icon_complete.vmt '+str(x)+' '+str(y)+' '+str(z+45)+' 1 .5 255')
	

def gu2(args):
	x,y,z = args[0]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/spectator_freecam.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x)+' '+str(y)+' '+str(z+50)+' .5 5 5 255 255 255 255')
	
def gu1(args):
	x,y,z = args[1]
	es.server.queuecmd('est_effect 11 #a 0 vgui/hud/icon_complete.vmt '+str(x)+' '+str(y)+' '+str(z+90)+' 1 .5 255')
	
def dru1(args):
	x,y,z =args[0]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueglow2.vmt '+str(x1)+' '+str(y1)+' '+str(z1+100)+' .5 1 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueglow2.vmt '+str(x1)+' '+str(y1)+' '+str(z1+100)+' .5 1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1)+' '+str(z1+10)+' '+str(x)+' '+str(y)+' '+str(z)+' .5 5 1 55 55 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1+30)+' '+str(z1+70)+' '+str(x)+' '+str(y)+' '+str(z)+' .5 5 1 55 55 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1-30)+' '+str(z1+70)+' '+str(x)+' '+str(y)+' '+str(z)+' .5 5 1 55 55 255 255')
	
def dru2(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueglow2.vmt '+str(x1)+' '+str(y1)+' '+str(z1+100)+' .2 1 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueglow2.vmt '+str(x1)+' '+str(y1)+' '+str(z1+100)+' .2 1 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1)+' '+str(z1+100)+' '+str(x)+' '+str(y)+' '+str(z)+' .2 5 5 55 55 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 0 200 .2 20 10 20 55 55 255 255 255')
	
def war2(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' '+str(x)+' '+str(y+50)+' '+str(z+100)+' 2 5 1 255 155 155 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x1)+' '+str(y1)+' '+str(z1+50)+' '+str(x)+' '+str(y-50)+' '+str(z)+' 2 5 1 255 155 155 255')
	
def freeze(args):
	x,y,z = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites\lgtning_noz.vmt '+str(x)+' '+str(y)+' '+str(z+40)+' 1 1.5 200')

def poi1(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites/greenglow1.vmt '+str(x1)+' '+str(y1)+' '+str(z1+40)+' .5 .5 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/greenglow1.vmt '+str(x1)+' '+str(y1)+' '+str(z1+40)+' .5 .5 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/greenglow1.vmt '+str(x1)+' '+str(y1)+' '+str(z1+40)+' .5 .5 255')

def poi2(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 60 61 .5 20 10 0 56 176 72 255 255')
	
def poi4(args):
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	x2,y2,z2 = args[0]
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1)+' '+str(z1)+' .5 5 5 56 176 72 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1+80)+' 60 61 .5 20 10 0 56 176 72 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x2)+' '+str(y2)+' '+str(z2)+' '+str(x1+30)+' '+str(y1+30)+' '+str(z1+80)+' .5 5 5 56 176 72 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x2)+' '+str(y2)+' '+str(z2)+' '+str(x1+30)+' '+str(y1-30)+' '+str(z1+80)+' .5 5 5 56 176 72 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x2)+' '+str(y2)+' '+str(z2)+' '+str(x1-30)+' '+str(y1-30)+' '+str(z1+80)+' .5 5 5 56 176 72 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x2)+' '+str(y2)+' '+str(z2)+' '+str(x1-30)+' '+str(y1+30)+' '+str(z1+80)+' .5 5 5 56 176 72 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x2)+' '+str(y2)+' '+str(z2+80)+' '+str(x1+30)+' '+str(y1+30)+' '+str(z1+80)+' .5 5 5 56 176 72 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x2)+' '+str(y2)+' '+str(z2+80)+' '+str(x1-30)+' '+str(y1-30)+' '+str(z1+80)+' .5 5 5 56 176 72 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x2)+' '+str(y2)+' '+str(z2+80)+' '+str(x1+30)+' '+str(y1-30)+' '+str(z1+80)+' .5 5 5 56 176 72 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x2)+' '+str(y2)+' '+str(z2+80)+' '+str(x1-30)+' '+str(y1+30)+' '+str(z1+80)+' .5 5 5 56 176 72 255')
	
def poi5(args):
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1)+' '+str(z1)+' .5 5 5 56 176 72 255')

def zv(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	x2,y2,z2 = args[0]
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' '+str(x)+' '+str(y)+' '+str(z+70)+' .5 10 10 255 69 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x2)+' '+str(y2)+' '+str(z2)+' '+str(x)+' '+str(y)+' '+str(z+70)+' .5 10 10 255 69 0 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites\lgtning_noz.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 60 61 .5 20 10 0 255 69 0 255 255')
	
def vg(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1+60)+' '+str(y1+60)+' '+str(z1+70)+' 1.5 5 5 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1+80)+' '+str(y1+120)+' '+str(z1+10)+' 1.8 5 5 255 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1+70)+' '+str(y1-60)+' '+str(z1+50)+' 2.1 5 5 255 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1-70)+' '+str(y1-120)+' '+str(z1+50)+' 2.4 5 5 0 255 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1-60)+' '+str(y1+180)+' '+str(z1)+' 2.7 5 5 0 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1-80)+' '+str(y1-180)+' '+str(z1+120)+' 3 5 5 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1-350)+' '+str(y1+200)+' '+str(z1+35)+' 3.3 5 5 175 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1+100)+' '+str(y1+180)+' '+str(z1+35)+' 3.6 5 5 175 175 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1+150)+' '+str(y1+120)+' '+str(z1+15)+' 3.9 5 5 175 0 175 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1-250)+' '+str(y1+15)+' '+str(z1+25)+' 4.2 5 5 0 175 175 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1-150)+' '+str(y1+95)+' '+str(z1+15)+' 4.5 5 5 175 175 175 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' '+str(x1-100)+' '+str(y1+500)+' '+str(z1+25)+' 4.8 5 5 58 255 60 255')
	
def ue3(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 150 151 2 3 255 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 150 151 2 3 255 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 180 181 2 3 255 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 180 181 2 3 255 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-20)+' '+str(y-74)+' '+str(z+10)+' '+str(x1+60)+' '+str(y1+48)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-20)+' '+str(y+74)+' '+str(z+10)+' '+str(x1+60)+' '+str(y1-48)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-20)+' '+str(y-74)+' '+str(z+10)+' '+str(x1-20)+' '+str(y1+74)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-20)+' '+str(y-74)+' '+str(z+10)+' '+str(x1+60)+' '+str(y1+48)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-20)+' '+str(y+74)+' '+str(z+10)+' '+str(x1+60)+' '+str(y1-48)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-20)+' '+str(y-74)+' '+str(z+10)+' '+str(x1-20)+' '+str(y1+74)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-75)+' '+str(y)+' '+str(z+10)+' '+str(x1+60)+' '+str(y1+48)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-75)+' '+str(y)+' '+str(z+10)+' '+str(x1+60)+' '+str(y1+48)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-75)+' '+str(y)+' '+str(z+10)+' '+str(x1+60)+' '+str(y1-48)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-75)+' '+str(y)+' '+str(z+10)+' '+str(x1+60)+' '+str(y1-48)+' '+str(z1+10)+' 2 2 2 255 0 0 255')
	
def ue4(args):
	x,y,z = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 110 109 2 3 1 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 140 139 2 3 1 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+120)+' 40 41 2 3 1 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+120)+' 20 21 2 3 1 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 40 41 2 3 1 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/laser.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 20 21 2 3 1 0 255 0 0 255 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/ledglow.vmt '+str(x)+' '+str(y)+' '+str(z+110)+' 2 1 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/orangecore2.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 2 1.3 255')

	
def ga1(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites/hud/320v_crosshair1.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 1 5 255')

def something1(args):
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueglow2.vmt '+str(x)+' '+str(y)+' '+str(z+190)+' 2 0.4 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueglow2.vmt '+str(x)+' '+str(y)+' '+str(z+170)+' 2 0.4 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueglow2.vmt '+str(x)+' '+str(y)+' '+str(z+150)+' 2 0.4 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/blueglow2.vmt '+str(x)+' '+str(y)+' '+str(z+130)+' 2 0.4 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-10)+' '+str(y)+' '+str(z+200)+' '+str(x1-10)+' '+str(y1)+' '+str(z1+100)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x+10)+' '+str(y)+' '+str(z+200)+' '+str(x1)+' '+str(y1)+' '+str(z1+210)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-10)+' '+str(y)+' '+str(z+200)+' '+str(x1)+' '+str(y1)+' '+str(z1+210)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x+10)+' '+str(y)+' '+str(z+200)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+100)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-12)+' '+str(y)+' '+str(z+100)+' '+str(x1+12)+' '+str(y1)+' '+str(z1+100)+' 2 10 10 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-12)+' '+str(y)+' '+str(z+100)+' '+str(x1+12)+' '+str(y1)+' '+str(z1+100)+' 2 10 10 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-12)+' '+str(y)+' '+str(z+100)+' '+str(x1+12)+' '+str(y1)+' '+str(z1+100)+' 2 10 10 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-10)+' '+str(y)+' '+str(z+200)+' '+str(x1-10)+' '+str(y1)+' '+str(z1+100)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x+10)+' '+str(y)+' '+str(z+200)+' '+str(x1)+' '+str(y1)+' '+str(z1+210)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-10)+' '+str(y)+' '+str(z+200)+' '+str(x1)+' '+str(y1)+' '+str(z1+210)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x+10)+' '+str(y)+' '+str(z+200)+' '+str(x1+10)+' '+str(y1)+' '+str(z1+100)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-12)+' '+str(y)+' '+str(z+100)+' '+str(x1+12)+' '+str(y1)+' '+str(z1+100)+' 2 10 10 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-12)+' '+str(y)+' '+str(z+100)+' '+str(x1+12)+' '+str(y1)+' '+str(z1+100)+' 2 10 10 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-12)+' '+str(y)+' '+str(z+100)+' '+str(x1+12)+' '+str(y1)+' '+str(z1+100)+' 2 10 10 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z+60)+' '+str(x1)+' '+str(y1)+' '+str(z1+100)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x+1)+' '+str(y)+' '+str(z+60)+' '+str(x1+1)+' '+str(y1)+' '+str(z1+100)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-1)+' '+str(y)+' '+str(z+60)+' '+str(x1-1)+' '+str(y1)+' '+str(z1+100)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x+1)+' '+str(y)+' '+str(z+60)+' '+str(x1+1)+' '+str(y1)+' '+str(z1+100)+' 2 2 2 0 51 102 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x-1)+' '+str(y)+' '+str(z+60)+' '+str(x1-1)+' '+str(y1)+' '+str(z1+100)+' 2 2 2 0 51 102 255')
	
def fl5(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 80 79 .5 5 255 0 227 38 54 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+15)+' 80 79 .5 5 255 0 0 127 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+20)+' 80 79 .5 5 255 0 240 248 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+30)+' 80 79 .5 10 255 0 59 68 75 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' 80 79 .5 10 255 0 59 68 75 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+45)+' 80 79 .5 5 255 0 227 38 54 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 80 79 .5 5 255 0 0 127 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+55)+' 80 79 .5 5 255 0 240 248 255 255 255')

def fl4(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites/640hud9.vmt '+str(x)+' '+str(y)+' '+str(z+90)+' 2 .5 255')

def fl3(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[0]
	es.server.queuecmd('est_effect 11 #a 0 sprites/640_pain_up.vmt '+str(x)+' '+str(y)+' '+str(z+70)+' .5 .5 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/640_pain_right.vmt '+str(x+50)+' '+str(y)+' '+str(z+50)+' .5 .5 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/640_pain_left.vmt '+str(x-50)+' '+str(y)+' '+str(z+50)+' .5 .5 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/640_pain_down.vmt '+str(x)+' '+str(y)+' '+str(z+30)+' .5 .5 255')
	
def fl2(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 80 79 .5 5 255 0 227 38 54 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+15)+' 80 79 .5 5 255 0 0 127 255 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+20)+' 80 79 .5 5 255 0 240 248 255 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+30)+' 80 79 .5 10 255 0 59 68 75 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' 80 79 .5 10 255 0 59 68 75 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+45)+' 80 79 .5 5 255 0 227 38 54 255 100')
	es.server.queuecmd('est_effect 10 #a 0 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 80 79 .5 5 255 0 0 127 255 255 100')
	es.server.queuecmd('est_effect 11 #a 0 sprites/640_pain.vmt '+str(x)+' '+str(y)+' '+str(z+55)+' .5 .5 255')

def fl1(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 80 79 .5 5 255 0 227 38 54 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+15)+' 80 79 .5 5 255 0 0 127 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+20)+' 80 79 .5 5 255 0 240 248 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+30)+' 80 79 .5 10 255 0 59 68 75 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+35)+' 80 79 .5 10 255 0 59 68 75 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+45)+' 80 79 .5 5 255 0 227 38 54 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 80 79 .5 5 255 0 0 127 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 vgui/gfx/vgui/cs_logo.vmt '+str(x)+' '+str(y)+' '+str(z+55)+' 80 79 .5 5 255 0 240 248 255 255 255')

def streamtest19(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 11 #a 0 models\crow.mdl '+str(x)+' '+str(y)+' '+str(z+80)+' 2 0.4 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+200)+' '+str(x1+200)+' '+str(y1)+' '+str(z1)+' 2 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+200)+' '+str(x1-200)+' '+str(y1)+' '+str(z1)+' 2 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x+200)+' '+str(y)+' '+str(z+200)+' '+str(x1)+' '+str(y1)+' '+str(z1)+' 2 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x-200)+' '+str(y)+' '+str(z+200)+' '+str(x1)+' '+str(y1)+' '+str(z1)+' 2 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y+200)+' '+str(z+200)+' '+str(x1)+' '+str(y1)+' '+str(z1)+' 2 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y-200)+' '+str(z+200)+' '+str(x1)+' '+str(y1)+' '+str(z1)+' 2 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+200)+' '+str(x1)+' '+str(y1+200)+' '+str(z1)+' 2 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+200)+' '+str(x1)+' '+str(y1-200)+' '+str(z1)+' 2 10 10 255 255 255 255')
	
def st1(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+20)+' 80 79 .5 50 255 0 236 25 3 255 255')
	es.server.queuecmd('est_effect 11 #a 0 models\crow.mdl '+str(x1)+' '+str(y1)+' '+str(z1+80)+' .5 0.4 255')
	es.server.queuecmd('est_effect_14 #a 0 models\crow.mdl '+str(Vector(x,y,z))+' '+str(Vector(x1,y1,z1))+' 30 200 800')
	
def error(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect_14 #a 0 error.mdl '+str(Vector(x,y,z))+' '+str(Vector(x1,y1,z1))+' 30 200 800')

def heavy2(args):	
	userid = str(args[0])
	x,y,z = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+120)+' 80 79 1 255 255 0 236 84 0 255 255')

def heavy(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1+30)+' 1 400 .5 25 1 0 236 84 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+30)+' 400 1 .5 25 1 0 0 84 236 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x1)+' '+str(y1)+' '+str(z1)+' 1 400 .5 25 1 0 236 84 0 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' 400 1 .5 25 1 0 0 84 236 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+30)+' '+str(x1)+' '+str(y1)+' '+str(z1)+' .5 5 5 236 84 0 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1)+' '+str(z1+30)+' .5 5 5 0 84 236 255')
	
def scout2(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[0]
	x1,y1,z1 = args[0]
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x,y,z+200))+' '+str(Vector(x1,y1+150,z1+200))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x,y,z+200))+' '+str(Vector(x1,y1-150,z1+200))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x,y,z+200))+' '+str(Vector(x1-150,y1,z1+200))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x,y,z+200))+' '+str(Vector(x1+150,y1,z1+200))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x,y+100,z+250))+' '+str(Vector(x1,y1+150,z1+200))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x,y+150,z+200))+' '+str(Vector(x1,y1+100,z1+150))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x,y-100,z+250))+' '+str(Vector(x1,y1-150,z1+200))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x,y-150,z+200))+' '+str(Vector(x1,y1-100,z1+150))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x+100,y,z+250))+' '+str(Vector(x1+150,y1,z1+200))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x+150,y,z+200))+' '+str(Vector(x1+100,y1,z1+150))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x-100,y,z+250))+' '+str(Vector(x1-150,y1,z1+200))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/cbbl_smoke.vmt '+str(Vector(x-150,y,z+200))+' '+str(Vector(x1-100,y1,z1+150))+' 80 .5 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/cbbl_smoke.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1)+' '+str(z1+200)+' .5 10 10 255 255 102 255')

def scout(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' 1 631 .5 20 20 0 207 0 255 255 255')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z)+' 631 1 .5 20 20 0 255 50 80 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+150)+' '+str(x1+305)+' '+str(y1)+' '+str(z1)+' .5 10 10 255 255 255 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+150)+' '+str(x1-305)+' '+str(y1)+' '+str(z1)+' .5 10 10 255 255 255 255')

def beast1(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 250 251 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+30)+' 230 231 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 210 211 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+70)+' 190 191 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+90)+' 170 171 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+110)+' 170 171 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+130)+' 190 191 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+150)+' 210 211 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+170)+' 230 231 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 10 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+190)+' 250 251 .3 20 20 0 166 29 9 255 10')
	es.server.queuecmd('est_effect 3 #a 0 decals/bloodstain_003.vmt  '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1)+' '+str(z1+250)+' .4 50 50 255 255 255 255')

def st4(args):	
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 250 631 1 20 20 0 207 35 35 255 10')
	es.server.queuecmd('est_effect 10 #a 0 sprites/lgtning.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 631 1 .5 20 20 0 255 50 80 255 10')
	es.server.queuecmd('est_effect 3 #a 0 sprites/reticle.vmt  '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1)+' '+str(z1+100)+' 1 100 100 255 35 35 255')
	delayed(.1, es.server.queuecmd, ('est_effect 3 #a 0 sprites/physcannon_blueflare1.vmt '+str(x)+' '+str(y)+' '+str(z+300)+' '+str(x1)+' '+str(y1)+' '+str(z1+500)+' .2 100 100 207 35 35 255'))
	delayed(.3, es.server.queuecmd, ('est_effect 3 #a 0 sprites/physcannon_blueflare1.vmt '+str(x)+' '+str(y)+' '+str(z+100)+' '+str(x1)+' '+str(y1)+' '+str(z1+500)+' .1 100 100 207 35 35 255'))
	delayed(.4, es.server.queuecmd, ('est_effect 3 #a 0 sprites/physcannon_blueflare1.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1)+' '+str(z1+500)+' .4 100 100 207 35 35 255'))
	delayed(.9, es.server.queuecmd, ('est_effect 3 #a 0 decals/blood1.vmt  '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1+150)+' '+str(z1+200)+' .4 10 10 255 255 255 255'))
	delayed(.9, es.server.queuecmd, ('est_effect 3 #a 0 decals/blood1.vmt  '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1+150)+' '+str(y1)+' '+str(z1+120)+' .4 10 10 255 255 255 255'))
	delayed(.9, es.server.queuecmd, ('est_effect 11 #a 0 decals/bloodstain_003.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 0.5 0.4 255'))

def em1(args):	
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 10 #a 0 sprites/physring1.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 300 301 1.5 20 20 0 255 255 255 100 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/physring1.vmt '+str(x)+' '+str(y)+' '+str(z+30)+' 250 251 1.5 20 20 0 255 255 255 100 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/physring1.vmt '+str(x)+' '+str(y)+' '+str(z+50)+' 200 201 1.5 20 20 0 255 255 255 100 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/physring1.vmt '+str(x)+' '+str(y)+' '+str(z+70)+' 150 151 1.5 20 20 0 255 255 255 100 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/physring1.vmt '+str(x)+' '+str(y)+' '+str(z+90)+' 100 101 1.5 20 20 0 255 255 255 100 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/physring1.vmt '+str(x)+' '+str(y)+' '+str(z+110)+' 50 51 1.5 80 20 0 255 255 255 100 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/physring1.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 300 301 1.5 20 20 0 255 255 255 100 50')
	es.server.queuecmd('est_effect 10 #a 0 sprites/physring1.vmt '+str(x)+' '+str(y)+' '+str(z+10)+' 300 301 1.5 20 20 0 255 255 255 100 50')
	es.server.queuecmd('est_effect 3 #a 0 effects/blueblacklargebeam.vmt '+str(x)+' '+str(y)+' '+str(z)+' '+str(x1)+' '+str(y1)+' '+str(z1+1000)+' 1.5 50 50 255 255 255 255')
	
def em3(args):	
	x,y,z = args[0]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x-60,y,z+90))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255')
	delayed(.1, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x-180,y,z+120))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(.2, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x-30,y,z+50))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(.3, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x+60,y,z+90))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(.4, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x+180,y,z+120))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(.5, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x+30,y,z+50))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(.6, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y+60,z+90))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(.7, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y+180,z+120))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(.8, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y+30,z+50))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(.9, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y-60,z+120))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(1, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y-180,z+90))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(1.1, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y-30,z+50))+' '+str(Vector(x1,y1,z1+50))+' 80 .2 5 5 100 0 255 255 102 255 255'))
	delayed(1.2, es.server.queuecmd, ('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+50))+' '+str(Vector(x1,y1,z1+50))+' 80 0.5 5 5 100 0 255 255 102 255 255'))
	
def ga4(args):	
	x,y,z = args[1]
	es.server.queuecmd('est_effect 11 #a 0 sprites/reticle.vmt '+str(x)+' '+str(y)+' '+str(z+20)+' 1.8 0.2 255')
	delayed(1.8, es.server.queuecmd, ('est_effect 11 #a 0 sprites/reticle.vmt '+str(x)+' '+str(y)+' '+str(z+40)+' 1.8 0.2 255'))
	delayed(3.6, es.server.queuecmd, ('est_effect 11 #a 0 sprites/reticle.vmt '+str(x)+' '+str(y)+' '+str(z+60)+' 1.8 0.2 255'))
	delayed(5.4, es.server.queuecmd, ('est_effect 11 #a 0 sprites/reticle.vmt '+str(x)+' '+str(y)+' '+str(z+80)+' 1.8 0.2 255'))
	delayed(7.2, es.server.queuecmd, ('est_effect 11 #a 0 sprites/spectator_eye.vmt '+str(x)+' '+str(y)+' '+str(z+80)+' .5 0.4 255'))
	delayed(7.2, es.server.queuecmd, ('est_effect 11 #a 0 sprites/radio.vmt '+str(x)+' '+str(y)+' '+str(z+100)+' .5 0.4 255'))
	delayed(7.2, es.server.queuecmd, ('est_effect_08 #a 0 sprites/scanner_dots2.vmt '+str(Vector(x,y,z+50))+' 0 100 255 .5 255 25 0 0 255 255 255 2 1'))
	delayed(7.2, es.server.queuecmd, ('est_effect_08 #a 0 sprites/scanner_dots2.vmt '+str(Vector(x,y,z+50))+' 50 101 255 1 255 25 0 0 255 255 255 2 1'))
	delayed(7.2, es.server.queuecmd, ('est_effect_08 #a 0 sprites/scanner_dots2.vmt '+str(Vector(x,y,z+50))+' 800 100 255 .5 255 25 0 0 255 255 255 2 1'))

def streamtest20(args):	
	x,y,z = args[1]
	x1,y1,z1 = args[0]
	x2,y2,z2 = args[1]
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+250))+' '+str(Vector(x1,y1,z1+30))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+250))+' '+str(Vector(x2,y2,z2+200))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+200))+' '+str(Vector(x2,y2+50,z2+150))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+200))+' '+str(Vector(x2,y2-50,z2+150))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x)+' '+str(y)+' '+str(z+150)+' 1 2 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y+50,z+150))+' '+str(Vector(x2,y2,z2+100))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y-50,z+150))+' '+str(Vector(x2,y2,z2+100))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x2,y2,z2+100))+' 80 1 5 5 100 0 255 255 102 255 255')

def streamtest21(args):	
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1-100,y1,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1+100,y1,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1+100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1-100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1+100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1-100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1+100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1+100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1-100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1-100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1-100,y1,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1+100,y1,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1+100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1-100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1+100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1-100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1+100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1+100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1-100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1-100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z+90))+' '+str(Vector(x1,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x-100,y-100,z))+' '+str(Vector(x1-100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x+100,y+100,z))+' '+str(Vector(x1-100,y1+100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x+100,y-100,z))+' '+str(Vector(x1-100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x+100,y+100,z))+' '+str(Vector(x1+100,y1-100,z1))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physring1.vmt '+str(Vector(x,y,z))+' '+str(Vector(x1,y1,z1+90))+' 80 1 5 5 100 0 255 255 102 255 255')
	
def pusher2(args):	
	x,y,z = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+60))+' '+str(Vector(x1+50,y1,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+60))+' '+str(Vector(x1+100,y1,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+60))+' '+str(Vector(x1-50,y1,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+60))+' '+str(Vector(x1-100,y1,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+60))+' '+str(Vector(x1,y1-50,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+60))+' '+str(Vector(x1,y1-100,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+60))+' '+str(Vector(x1,y1+100,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+60))+' '+str(Vector(x1,y1+50,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y,z+60))+' '+str(Vector(x1,y1,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y+100,z+60))+' '+str(Vector(x1,y1,z1+60))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y-100,z+60))+' '+str(Vector(x1,y1,z1+60))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x+100,y,z+60))+' '+str(Vector(x1,y1,z1+60))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x-100,y,z+60))+' '+str(Vector(x1,y1,z1+60))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y+100,z+60))+' '+str(Vector(x1,y1,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x,y-100,z+60))+' '+str(Vector(x1,y1,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x+100,y,z+60))+' '+str(Vector(x1,y1,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')
	es.server.queuecmd('est_effect_06 #a 0 sprites/physcannon_blueflare1.vmt '+str(Vector(x-100,y,z+60))+' '+str(Vector(x1,y1,z1+600))+' 80 1 5 5 100 0 255 255 102 255 255')

def pusher3(args):	
	x,y,z = args[1]
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z))+' 100 101 255 .2 25 25 0 0 255 52 255 2 1')
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z))+' 50 51 255 .2 25 25 0 0 255 52 255 2 1')
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z+10))+' 100 101 255 .2 25 25 0 0 255 52 255 2 1')
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z+20))+' 100 101 255 .2 25 25 0 0 255 52 255 2 1')
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z+30))+' 100 101 255 .2 25 25 0 0 255 52 255 2 1')
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z+40))+' 100 101 255 .2 25 25 0 0 255 52 255 2 1')
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z+50))+' 100 101 255 .2 25 25 0 0 255 52 255 2 1')
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z+60))+' 100 101 255 .2 25 25 0 0 255 52 255 2 1')
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z+70))+' 100 101 255 .2 25 25 0 0 255 52 255 2 1')
	es.server.queuecmd('est_effect_08 #a 0 cs_italy/black.vmt '+str(Vector(x,y,z+70))+' 50 51 255 .2 25 25 0 0 255 52 255 2 1')
	
def pusher4(args):
	x,y,z = args[1]
	x2,y2,z2 = args[1]
	x1,y1,z1 = args[1]
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x)+' '+str(y)+' '+str(z+1000)+' '+str(x2)+' '+str(y2)+' '+str(z2)+' 1 25 25 255 255 255 100')
	es.server.queuecmd('est_effect 11 #a 0 holliday\eye.vmt '+str(x)+' '+str(y)+' '+str(z+500)+' 1 5 255')
	es.server.queuecmd('est_effect 11 #a 0 sprites/redglow2.vmt '+str(x2)+' '+str(y2)+' '+str(z2+500)+' 1 9 255')
	es.server.queuecmd('est_effect 3 #a 0 sprites\lgtning_noz.vmt '+str(x)+' '+str(y)+' '+str(z+500)+' '+str(x1)+' '+str(y1)+' '+str(z1+40)+' 1 25 25 255 0 0 255')
	
def streamtest22(args):
	userid = str(args[0])
	attacker = str(args[1])
	x,y,z = args[1]
	v0 = Vector(x,y,z+40)
	v = Vector(x,y,z)
	v1 = Vector(args[0])
	es.server.queuecmd('est_effect_06 #a 0 effects/bluelaser1.vmt '+str(v1)+' '+str(v0)+' 100 1.1 10 10 0 0 228 128 28 255 255')
	es.server.queuecmd('est_effect 10 #a 0 effects/tvscreen_noise002a.vmt '+str(x)+' '+str(y)+' '+str(z)+' 1 500 1.1 50 10 1 0 255 255 255 255')
	es.server.queuecmd('est_effect_18 #a 0 '+str(x)+' '+str(y)+' '+str(z)+' 255 20 255 85 50 10 20')
	es.server.queuecmd('est_effect_06 #a 0 sprites/plasmabeam.vmt '+str(v1)+' '+str(v0)+' 80 1.1 15 1 10 0 65 80 255 255 255')