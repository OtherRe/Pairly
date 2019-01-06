from  pypairly import *


PairlyDB.connect("mongodb://127.0.0.1:27017", "pairlyDB")
pdb = PairlyDB.instance()

id1 = pdb.addDevice(Device("", "user", "key", "devname1", 10, 20, DataType.CO))
id2 = pdb.addDevice(Device("", "user", "key", "devname2", 10, 20, DataType.CO))


pdb.addData(id1, Data(10.0, 0))
pdb.addData(id1, Data(10.0, 3600))
pdb.addData(id1, Data(10.0, 10000))
pdb.addData(id1, Data(10.0, 20000))
pdb.addData(id1, Data(10.0, 30000))
pdb.addData(id1, Data(10.0, 40000))
pdb.addData(id1, Data(10.0, 50000))
pdb.addData(id1, Data(10.0, 60000))


for d in pdb.getDevices():
    print(d.id + " name: " + d.name + ":\n")
    for data in pdb.getDeviceData(d.id, 1):
        print("     {} {}".format(data.first, data.second))


