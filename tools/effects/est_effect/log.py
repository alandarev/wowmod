import es
import os.path
import os
import time

def log(text):
   path = os.path.join(es.getAddonPath('wcs'), 'addons/est_effect' 'logs')
   if not os.path.isdir(path):
      os.mkdir(path)
   path = os.path.join(path, time.strftime('%m%d.log'))
   file = open(path, 'a+')
   file.write(time.strftime('%H:%M:%S: ')+str(text)+'\n')
   file.close()
   es.dbgmsg(0, str(text))