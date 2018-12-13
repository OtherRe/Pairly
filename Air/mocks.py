import math

class DbMock():
    def __init__(self):
        self.devices = {}

    def init(self):
        pass

    def getDeviceData(self, deviceId, hourInterval, after = 0, before = math.inf):
        if deviceId not in self.devices:
            raise Exception

        data = [pair for pair in self.devices[deviceId].data if pair[0] > after and pair[1] < before]
        return data

    def addDevice(self, device):
        self.devices[device.id] = device


    def removeDevice(self,deviceId):
        self.devices.pop(deviceId, None)

    def addData(self, deviceId, data):
        device = self.devices[deviceId]
        device.data.append(data)

    @staticmethod
    def isInRadius(lat, lng, x, y, radius):
        return math.sqrt((lat - x)**2 + (lng - y)**2) < radius

    def getAreaData(self,latitude, longitude, kilometersRadius, after, before, hoursInterval, dataType):
        devices = [device for device in self.devices.values() if self.isInRadius(latitude, longitude, device.lat, device.lng, kilometersRadius)]
        data = [pair for device in devices for pair in device.data if pair[0] > after and pair[0] < before]

        return data