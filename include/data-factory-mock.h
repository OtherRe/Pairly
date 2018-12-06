#ifndef DATA_FACTORY_MOCK_H
#define DATA_FACTORY_MOCK_H

#include <vector>
#include <limits>
#include <map>
#include "device.h"
#include "data-factory.h"

using Data = std::pair<double, int>;

class DataFactoryMock : public DataFactory {
public:
    virtual bool isConnected();
    virtual std::vector<Device> getDevices();
    virtual void addDevice(const Device &dev);
    virtual void removeDevice(int deviceId);
    virtual Device getDeviceById(int id);
    virtual std::vector<Data> getDeviceData(int deviceId,
                                            int timeAfter = 0, int timeBefore = std::numeric_limits<int>::max());
    virtual void addData(int deviceId, const Data &data);

    void connect();
    void disconnect();

private:
    using DeviceDataMap = std::map<int, std::vector<Data>>;

    bool connected = false;
    std::vector<Device> devices;
    DeviceDataMap map;
};

#endif
