#include "data-factory-mock.h"
#include "pairly-exceptions.h"

bool DataFactoryMock::isConnected()
{
    return connected;
}
void DataFactoryMock::connect()
{
    connected = true;
}

void DataFactoryMock::disconnect()
{
    connected = false;
}

std::vector<Device> DataFactoryMock::getDevices()
{
    return devices;
}

void DataFactoryMock::addDevice(const Device &dev)
{
    for (auto it = devices.begin(); it != devices.end(); it++) {
        if (it->id == dev.id) {
            throw DataBaseException("Element with such already exists");
        }
    }
    devices.push_back(dev);
}

void DataFactoryMock::removeDevice(int deviceId)
{
    for (auto it = devices.begin(); it != devices.end(); it++) {
        if (it->id == deviceId) {
            devices.erase(it);
            break;
        }
    }
}

Device DataFactoryMock::getDeviceById(int id)
{
    for (auto it = devices.begin(); it != devices.end(); it++) {
        if (it->id == id) {
            return *it;
        }
    }

    throw DataBaseException("Element with such id does not exist");
}

std::vector<Data> DataFactoryMock::getDeviceData(int deviceId, int timeAfter,
                                                 int timeBefore)
{
    auto it = map.find(deviceId);

    if (it == map.end())
        throw DataBaseException("Element with such id does not exist");

    std::vector<Data> result;

    for (Data &d : it->second) {
        if (d.second >= timeAfter && d.second <= timeBefore) {
            result.push_back(d);
        }
    }

    return result;
}

void DataFactoryMock::addData(int deviceId, const Data &data)
{
    auto it = map.find(deviceId);

    if (it == map.end())
        throw DataBaseException("Element with such id does not exist");
    
    it->second.push_back(data);
}
