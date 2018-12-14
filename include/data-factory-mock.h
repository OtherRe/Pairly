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
    virtual DeviceVec getDevices(double latitude, double longitude,
                                 double kilometersRadius, DataType dataType) const;

    void connect();
    void disconnect();

private:
    using DeviceDataMap = std::map<int, DataVec>;

    /**
     *      @brief Helper method used to determine, if a given device lies within a
     *      specified area. The area is specified as a geographical point and a radius
     *      from that point.
     * 
     *      @param dev Device reference
     *      @param latitude Geographical latitude
     *      @param longitude Geographical longitude
     *      @param radius Radius in kilometers
     * 
     *      @return Check if the device lies in the area
     */
    static bool isInRadius(const Device &dev, double latitude, double longitude, double radius);

    static double deg2rad(double);
    static double vincenty_distance(double latitude1, double longitude1, double latitude2,
                             double longitude2);

    bool connected = false;
    DeviceVec devices;
    DeviceDataMap map;
};

#endif
