#ifndef DATA_FACTORY_H
#define DATA_FACTORY_H

#include <vector>
#include <limits>
#include "device.h"

/**
 *  @brief Pair of value and sample time.
 */
using Data = std::pair<double, int>;
using DeviceVec = std::vector<Device>;
using DataVec = std::vector<Data>;

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
    virtual DeviceVec getDevices() = 0;

    /**
     *  @brief Add a new device to the database.
     * 
     *  @param dev A reference to a new device.
     * 
     *  @return actual id of the device
     */
    virtual std::string addDevice(const Device &dev) = 0;
    
    /**
     *  @brief Remove existing device.
     * 
     *  @param deviceId Unique id of device which should be removed.
     */
    virtual void removeDevice(const std::string &deviceId) = 0;

    /**
     *  @brief Get a single device
     * 
     *  @param id Unique id of device which should be removed.
     * 
     *  @return Device instance.
     */
    virtual Device getDeviceById(const std::string &id) = 0;

    /**
     *      @brief Method used to obtain devices in a given radius and with a given type
     * 
     *      @throws PairlyLibException if the dataFactory pointer is not present
     *      
     *      @param longitude Geographical longitude
     *      @param latitude Geographical latitude
     *      @param kilometersRadius Radius in kilometers
     *      @dataType type of data measured by devices
     * 
     *      @return Vector of data samples.
     */
    virtual DeviceVec getDevices(double latitude, double longitude,
                                 double kilometersRadius, DataType dataType) const = 0;


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
    virtual DataVec getDeviceData(const std::string &deviceId, int timeAfter = 0,
                                  int timeBefore = std::numeric_limits<int>::max()) = 0;

    /**
     *  @brief Add a new data sample.
     * 
     *  @param deviceId Unique device id.
     *  @param data A reference to new data sample.
     */
    virtual void addData(const std::string &deviceId, const Data &data) = 0;

    /**
     *  @brief Get devices connected with a user
     * 
     *  @param user Username whose devices we'd like to obtain
     * 
     *  @return A vector of user's devices
     */
    virtual DeviceVec getDevices(const std::string &user) = 0;
};

#endif
