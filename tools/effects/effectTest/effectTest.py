#imports
from random import randint

from es import getplayerlocation
from es import ServerVar
from es import server
from es import msg
from playerlib import getPlayer
from weaponlib import getWeapon
from effectlib2 import *
from collections import defaultdict

import spe_effects
import ice_test

ServerVar('eventscripts_noisy').set(1)


_effects = ['oran9e', 'SPECTATE', 'Snake', 'SnakePoison', 'IceWdamage', 'IceWfroste', 'SHADEBOLT',
'bullet', 'deadbeam', 'ultcircle', 'toxic', 'fade', 'flashfreeze', 'vortattack', 'electrikwarrior', 
'eclipse', 'darkstrike', 'rage', 'darkmurk', 'darkpandor', 'Hoore2', 'bloody', 'Hoore', 'pyramid', 
'darkbubbles', 'just132541', 'fastdead', 'tornado', 'spider1', 'spider2', 'spider3', 'electrostrike',
'fire', 'flameattack', 'bloodyblades', 'bloodsuck', 'red', 'burningstrike', 'pinch', 'hydra',
'hydra2', 'purpleup', 'explodebomb', 'spider', 'kanibal', 'blackwidow1', 'blackwidow3', 'blackwidow4',
'spirit_of_raven', 'akayatsuku', 'woe', 'eli', 'toxins', 'spruce', 'spectron1', 'spectron2', 'spectron3',
'nortrom1', 'nortrom2', 'nortrom3', 'friz', 'frezult', 'oran9e2', 'fireplay', 'testfire', 'playdog1',
'playdog2', 'playdog3', 'poseidon1', 'poseidon2', 'poseidon3', 'ammobaron1', 'ammobaron2', 'ammobaron3',
'posya', 'posedon', 'assasinspider', 'assasinspider2', 'wafsehg', 'wfagagrhhrt', 'windrunner', 'kori',
'koral1', 'koral2', 'koral3', 'bofdar1', 'bofdar2', 'bofdar3', 'paladinaura', 'purplesmoke', 'purpleup2', 
'triple', 'soldier1', 'soldier2', 'hoore1', 'hoore2', 'jockomonkey1', 'jockomonkey2', 'jockomonkey3', 
'jockomonkey4', 'knife', 'lunabeam', 'iceice', 'vampire', 'scorpion', 'donator', 'deagleranger', 'vagabond2', 
'frostecristall', 'frostebite', 'deadflamer', 'greenlasers', 'crowwing', 'tooooooxic', 'physic', 'murk1', 
'murk2', 'razor1', 'razor2', 'rage1', 'rage2', 'fire', 'electros', 'electroaura', 'pulse', 'godstrengch', 
'sunlight', 'bluedarkattack', 'redlightattack', 'shade', 'tree', 'snake1', 'baal1', 'baal2', 'assassin1',
'assassin2', 'darkflame', 'watertotem', 'lasercross', 'laser', 'nebula1', 'holyfallen', 'holystrike', 'holyshield', 
'tester', 'ultimatewte', 'ultimatettt', 'brew1', 'brew2', 'fr', 'flame', 'firearrows', 'thunder', 'explode', 'blood', 
'leeched', 'poseidons_push', 'smoke', 'sparks', 'furion', 'deadwing', 'torch', 'earthquake', 'xepnya', 'zombieclaw', 
'healingct', 'healingt', 'marine', 'boyscout', 'sniperclass', 'flow', 'Punisher1', 'punisher2', 'punisher3', 'punisher4', 
'ing', 'mlow', 'evil', 'respawn', 'hybrid1', 'testert', 'werewolf', 'xrandom_rings', 'vagabond', 'necrolyt1', 'necrolyt2', 
'necrolyt3', 'necrolyt4', 'necrolyt5', 'necrolytult', 'traxex', 'traxex1', 'testerrr', 'swarm', 'teste', 'Ati', 'static', 
'ionforcefiled', 'testeeeeeeer', 'korch1', 'testeeee', 'testiol', 'korchspawn', 'korchvictim', 'orh', 'nogood', 'ion_cannon', 
'GAUSS', 'testeee', 'ravange', 'low', 'elem', 'nature', 'thornsaura', 'Transquility', 'mimicspawn', 'mimicattacker', 'slowdown', 
'azgalorspawn', 'azgalorattack', 'azgalorvictim', 'te', 'ion', 'ultimatettt', 'hunterspear', 'ambush', 'redring', 'ultimaterrrr', 
'shdem', 'grade1', 'rogue1', 'ex', 'khabrosu', 'bomb', 'terminator1', 'Shield', 'streamtest01', 'streamtest02', 'streamtest03',
'streamtest04', 'streamtest05', 'streamtest06', 'streamtest07', 'streamtest08', 'streamtest09', 'streamtest10', 'streamtest11',
'streamtest12', 'streamtest13', 'streamtest14', 'stream3_4', 'stream3_3', 'stream3_1', 'streamtest15', 'old3_2', 'streamtest16',
'old2_1', 'old1_3', 'old1_2', 'old1_1', 'streamtest17', 'streamtest18', 'juk1', 'juk3', 'Done', 'gu2', 'gu1', 'dru1', 'dru2',
'war2', 'freeze', 'poi1', 'poi2', 'poi4', 'poi6', 'zv', 'bg', 'ue3', 'ue4', 'ga1', 'something1', 'fl5', 'fl4', 'fl3', 'fl2',
'fl1', 'streamtest19', 'st1', 'error', 'heavy2', 'heavy', 'scout2', 'scout', 'beast1', 'st4', 'em1', 'em3', 'ga4', 'streamtest20',
'streamtest21', 'pusher2', 'pusher3', 'pusher4', 'streamtest22']

_users = defaultdict(int)

def player_say(ev):
    if ev['text'].isdigit():
        _users[ev['userid']] = int(ev['text'])
    if ev['text'] == '+':
        _users[ev['userid']] += 1
    if ev['text'] == '-':
        _users[ev['userid']] -= 1
    if _users[ev['userid']] < 0:
        _users[ev['userid']] = 0
    if _users[ev['userid']] > len(_effects) - 1:
        _users[ev['userid']] = len(_effects) - 1
    
def bullet_impact(ev):
    x, y, z = getplayerlocation(ev['userid'])
    x1, y1, z1 = float(ev['x']), float(ev['y']), float(ev['z'])
    r, g, b = randint(0, 255), randint(0, 255), randint(0, 255)
    msg('%s (number %s/%s)' % (_effects[_users[ev['userid']]], _users[ev['userid']], len(_effects) - 1))
    ServerVar('est_userid').set(ev['userid'])
    ServerVar('wcs_x1').set(x)
    ServerVar('wcs_y1').set(y)
    ServerVar('wcs_z1').set(z)
    ServerVar('wcs_x2').set(x1)
    ServerVar('wcs_y2').set(y1)
    ServerVar('wcs_z2').set(z1)
    if int(_users[ev['userid']]) < 234:
        server.queuecmd('es_doblock effects/%s' % _effects[_users[ev['userid']]])
    else:
        exec("ice_test.ice_test.%s(%s)" % (_effects[_users[ev['userid']]], [(x, y, z), (x1, y1, z1)]))
    weapon = getWeapon(getPlayer(ev['userid']).weapon)
    if weapon.slot == 1:
        getPlayer(ev['userid']).clip.primary += 1
    elif weapon.slot==2:
        getPlayer(ev['userid']).clip.secondary += 1
    #createBall((x1, y1, z1 + 50), radius = 1000, colorGreen = g, colorBlue = b, colorRed = r, amplitude = 0)
    #createCone(vectorStart = (x1, y1, z1), height = 1000, radiusStart = 500, colorGreen = g, colorBlue = 0, colorRed = 0, model = 'models/police/pupil_r.vmt')
    #createBox(vectorStart = (x1, y1, z1), height = 1000, Range = 500, model = 'models/police/pupil_r.vmt', shaded = 1,
    #RangeLine = 50)
    #createBubbles(vectorStart = (x, y, z + 50), vectorEnd = (x1, y1, z1 + 50), count = 500, height = 50, speed = 100,
    #model = 'models/police/pupil_r.vmt')
    #createBall((x1, y1, z1 + 50), radius = randint(25, 75), colorGreen = g, colorBlue = b, colorRed = r, amplitude = 0)
    #createBox(vectorStart = (x1, y1, z1), height = 80, Range = 80, colorGreen = g, colorBlue = b, colorRed = r, shaded = 1)
    #createPyramid(vectorStart = (x1, y1, z1), height = 80, Range = 80, colorGreen = g, colorBlue = b, colorRed = r, shaded = 1)
    #createCrystal(vectorStart = (x1, y1, z1), height = 80, Range = 35, colorGreen = g, colorBlue = b, colorRed = r)
    #createCone(vectorStart = (x1, y1, z1), height = randint(25, 125), radiusStart = randint(25, 125), colorGreen = g, colorBlue = b, colorRed = r)
    #createPolygon(((x1, y1, z1), (x1 + 100, y1, z1), (x1 , y1, z1 + 600)), colorBlue = 255)
    #createLine(vectorStart = (x1, y1, z1 + 5), vectorEnd =(x1 - 50, y1, z1 + 80), colorGreen = 255, lifeTime = 5)
    #server.queuecmd('es_prop_dynamic_create %s props/de_nuke/cinderblock_stack.mdl' % ev['userid'])
    #global value
    #value += 1
    #msg(value)     
    