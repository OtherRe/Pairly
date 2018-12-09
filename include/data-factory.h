#ifndef DATA_FACTORY_H
#define DATA_FACTORY_H

#include <vector>
#include <limits>
#include "device.h"

/**
 *  @brief Pair of value and sample time.
 */
using Data = std::pair<double, int>;

/**
 *  @class DataFactory
 * 
 *  @brief An abstract factory class allowing clients to querry the database.
 *  Concrete class inheriting from that class shall implement connection with
 *  a particular DataBase system.
 */
class DataFactory {
public:

    /**
     *  @brief Determine if a connection with a data base is established.
     *  Connection method is data-base dependent.
     * 
     *  @return True if a connection is established.
     */
    virtual bool isConnected() = 0;

    /**
     *  @brief Get all devices in a database.
     * 
     *  @return A vector of all available devices.
     */
    virtual std::vector<Device> getDevices() = 0;

    /**
     *  @brief Add a new device to the database.
     * 
     *  @param dev A reference to a new device.
     */
    virtual void addDevice(const Device &dev) = 0;
    
    /**
     *  @brief Remove existing device.
     * 
     *  @param deviceId Unique id of device which should be removed.
     */
    virtual void removeDevice(int deviceId) = 0;

    /**
     *  @brief Get a single device
     * 
     *  @param id Unique id of device which should be removed.
     * 
     *  @return Device instance.
     */
    virtual Device getDeviceById(int id) = 0;


    /**
     *  @brief Get data samples from the device.
     *
     *  @param deviceId Unique id of the device, which data we would like to get.
     *
     *  @param timeAfter Time in Unix time format.
     *  @param timeBefore Time in Unix time format.
     *
     *  @param deviceId Unique id of device which should be removed.
     * 
     *  @return Vector of data samples.
     */
    virtual std::vector<Data> getDeviceData(int deviceId,
                                            int timeAfter = 0, int timeBefore = std::numeric_limits<int>::max()) = 0;

    /**
     *  @brief Add a new data sample.
     * 
     *  @param deviceId Unique device id.
     *  @param data A reference to new data sample.
     */
    virtual void addData(int deviceId, const Data &data) = 0;

    virtual std::vector<Device> getDevices(const std::string &user) = 0;
};

#endif
