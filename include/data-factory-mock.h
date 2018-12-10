#ifndef DATA_FACTORY_MOCK_H
#define DATA_FACTORY_MOCK_H

#include <vector>
#include <limits>
#include <map>
#include "device.h"
#include "data-factory.h"

class DataFactoryMock : public DataFactory {
public:
    virtual bool isConnected();
    virtual DeviceVec getDevices();
    virtual void addDevice(const Device &dev);
    virtual void removeDevice(int deviceId);
    virtual Device getDeviceById(int id);
    virtual DataVec getDeviceData(int deviceId,
                                            int timeAfter = 0, int timeBefore = std::numeric_limits<int>::max());
    virtual void addData(int deviceId, const Data &data);
    virtual DeviceVec getDevices(const std::string &user);

    void connect();
    void disconnect();

private:
    using DeviceDataMap = std::map<int, DataVec>;

    bool connected = false;
    DeviceVec devices;
    DeviceDataMap map;
};

#endif
