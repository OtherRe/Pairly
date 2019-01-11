import sys, os
from Db import Db
sys.path.append(sys.path.append(os.path.join(os.path.dirname(__file__), "../PairlyDB/build")))
import pypairly as db
import random
import time

if __name__ == '__main__':
  fake_locations = [(21.00384, 52.24094), (21.02826, 52.22264), (21.05418, 52.236735), (21.003856, 52.256660), (21.160054, 51.400062)]
  devices = Db.mongo().getDevices()
  for index, location in enumerate(fake_locations):
    if not list(filter(lambda d: d.latitude == location[1], devices)):
      device = db.Device(
                '',
                'fake_user',
                'fake_public_key',
                f'test{index}',
                location[1],
                location[0],
                Db.get_data_type('CO')
              )
      Db.mongo().addDevice(device) 

  devices = Db.mongo().getDevices()
  for device in devices:
    for i in reversed(range(100)):
      data = db.Data(random.uniform(6,66), int(time.time()) - i * 3600)
      Db.mongo().addData(device.id, data)