import sys, os
from Db import Db
sys.path.append(sys.path.append(os.path.join(os.path.dirname(__file__), "../PairlyDB/build")))
import pypairly as db
import random
import time

if __name__ == '__main__':
  devices = Db.mongo().getDevices()
  for device in devices:
    for i in range(100):
      data = db.Data(random.uniform(6,66), int(time.time()) - i * 3600)
      Db.mongo().addData(device.id, data)