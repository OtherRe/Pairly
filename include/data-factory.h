#ifndef DATA_FACTORY_H
#define DATA_FACTORY_H

#include <vector>
#include <limits>
#include "device.h"

// value - time
using Data = std::pair<double, int>;

class DataFactory {
public:
    virtual bool isConnected() = 0;
    virtual std::vector<Device> getDevices() = 0;
    virtual void addDevice(const Device &dev) = 0;
    virtual void removeDevice(int deviceId) = 0;
    virtual Device getDeviceById(int id) = 0;
    virtual std::vector<Data> getDeviceData(int deviceId,
                                            int timeAfter = 0, int timeBefore = std::numeric_limits<int>::max()) = 0;
    virtual void addData(int deviceId, const Data &data) = 0;
};

#endif
