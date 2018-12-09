#ifndef PAIRLY_H
#define PAIRLY_H

#include "data-factory.h"
#include <vector>
#include <limits>

/**
 *  @class PairlyDB
 * 
 *  @brief Main Pairlylib class. Used by the client to access remote data
 *  on devices and data. Implemented as a singleton.      
 */
class PairlyDB {
public:

    /**
     *      @brief Method implementing the singleton behaviour. It initializes
     *      PairlyDB instance on first call. It's not thread-safe.
     * 
     *      @return Reference to instance of PairlyDB class.
     */
    static PairlyDB& instance();

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
    
    /**
     *      @brief Method used to pass a pointer to DataFactory, which is an abstract class
     *      used to access the database. It is necessary to perform any database operation.
     * 
     *      @throws PairlyLibException If the dataFactory is not connected to a remote DB.
     *
     *      @param dataFactory A pointer to instance of the abstract DataFactory. The dataFactory
     *      must be already connected with a remote database.
     */
    void init(DataFactory *dataFactory);

    /**
     *      @brief Method used to obtain a device with a given id.
     * 
     *      @throws PairlyLibException if the dataFactory pointer is not present
     *      @throws DataBaseException if the device with a given id does not exist.
     * 
     *      @param deviceId Unique device id.
     *      
     *      @return Instance of the device.
     */
    Device getDevice(int deviceId) const;
    
    /**
     *      @brief Method used to obtain all devices belonging to the given user.
     *      If the user does not exist or does not have any devices, the method
     *      returns an empty vector.
     * 
     *      @throws PairlyLibException if the dataFactory pointer is not present\
     * 
     *      @param user User name whose devices we want to get.
     *      
     *      @return Vector of devices belonging to the user.
     */
    std::vector<Device> getDevices(const std::string &user) const;

    /**
     *      @brief Method used to obtain all devices.
     * 
     *      @throws PairlyLibException if the dataFactory pointer is not present
     *      
     *      @return Vector of all devices.
     */
    std::vector<Device> getDevices() const;

    std::vector<Device> getDevices(double latitude, double longitude,
                                   double kilometersRadius, DataType dataType) const;

    /**
     *      @brief Method used to get data from a single device. It is possible to
     *      specify time range of data samples.
     * 
     *      @throws PairlyLibException if the dataFactory pointer is not present
     *      @throws DataBaseException if the device with a given id does not exist.
     * 
     *      @param deviceId Unique id of the device, which data we would like to get.
     *      @param hourInterval Hour interval with which the data should be collected.
     *      The method takes the average of data samples if they are more frequent
     *      than the given interval or copies the samples if they are less frequent.
     *      Value 0 means "get all the data as it is in the db".
     * 
     *      @param after Time in Unix time format.
     *      @param before Time in Unix time format.
     *      
     *      @return Vector of data samples.
     */
    std::vector<Data> getDeviceData(int deviceId, int hourInterval, int after = 0, int before = std::numeric_limits<int>::max()) const;

    /**
     *      @brief Method used to add a new device to the database.
     * 
     *      @throws PairlyLibException if the dataFactory pointer is not present
     *      @throws DataBaseException if the device with a given id already exists
     *      
     *      @param reference to a new device
     */
    void addDevice(const Device &device);

    /**
     *      @brief Method used to remove a device from a database.
     * 
     *      @throws PairlyLibException if the dataFactory pointer is not present
     *      @throws DataBaseException if the device with a given id does not exist
     */
    void removeDevice(int deviceId);

    /**
     *      @brief Add a new data sample to a given device.
     * 
     *      @throws PairlyLibException if the dataFactory pointer is not present
     *      @throws DataBaseException if the device with a given id does not exist
     * 
     *      @param deviceId unique id of a device
     *      @param data new data sample
     */
    void addData(int deviceId, const Data &data);

    /**
     *      @brief Method used to obtain data from a given area with a given time interval
     *      and from a given time range.
     * 
     *      @throws PairlyLibException if the dataFactory pointer is not present
     *      
     *      @param longitude Geographical longitude
     *      @param latitude Geographical latitude
     *      @param kilometersRadius Radius in kilometers
     *      @param after Time in Unix time format.
     *      @param before Time in Unix time format.
     *      @param hourInterval Hour interval with which the data should be collected.
     *      The method takes the average of data samples if they are more frequent
     *      than the given interval or copies the samples if they are less frequent.
     *      Value 0 means "get all the data as it is in the db".
     *
     *      @param dataType Type of particles, which we want to measure.
     *
     *      @return A vector of data samples  
     */
    std::vector<Data> getAreaData(double latitude, double longitude,
                                  double kilometersRadius, int after,
                                  int before, int hoursInterval, DataType dataType) const;

private:
    PairlyDB() { }
    PairlyDB(const PairlyDB&) = delete;
    PairlyDB& operator=(const PairlyDB&) = delete;

    DataFactory *dataFactory = nullptr;
    
    void checkDataFactory() const;

    static int div_to_nearest(int n, int d);

    static double deg2rad(double);
    static double vincenty_distance(double latitude1, double longitude1, double latitude2,
                             double longitude2);

    static const int MINUTE = 60;
    static const int HOUR = 3600;
};

#endif // PAIRLY_H 
