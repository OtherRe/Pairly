#ifndef PAIRLY_H
#define PAIRLY_H

#include "data-factory.h"
#include <vector>
#include <limits>

class PairlyDB {
public:
    static PairlyDB& instance();

    static bool isInRadius(const Device &dev, double longitude, double latitude, double radius);
    void init(DataFactory *dataFactory);
    Device getDevice(int deviceId) const;
    std::vector<Device> getDevices(const std::string &user) const;
    std::vector<Device> getDevices() const;
    std::vector<Data> getDeviceData(int deviceId, int hourInterval, int after = 0, int before = std::numeric_limits<int>::max()) const;
    void addDevice(const Device &device);
    void removeDevice(int deviceId);
    void addData(int deviceId, const Data &data);
    std::vector<Data> getAreaData(double longitude, double latitude,
                                  double kilometersRadius, int after,
                                  int before, int hoursInterval) const;
    
private:
    PairlyDB() { }
    PairlyDB(const PairlyDB&) = delete;
    PairlyDB& operator=(const PairlyDB&) = delete;

    DataFactory *dataFactory = nullptr;
    
    void checkDataFactory() const;


    static const int MINUTE = 60;
    static const int HOUR = 3600;
};

#endif // PAIRLY_H 
