#include "data-factory-mock.h"
#include "pairly-exceptions.h"
#include <cmath>

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
            throw DataBaseException("Element with such id already exists");
        }
    }

    map.insert(std::make_pair(dev.id, std::vector<Data>(0)));
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

std::vector<Device> DataFactoryMock::getDevices(const std::string &user)
{
    std::vector<Device> result;

    for (Device &dev : this->devices) {
        if (dev.user == user)
            result.push_back(dev);
    }

    return result;
}

double DataFactoryMock::deg2rad(double deg)
{
    return deg * (M_PI / 180.0);
}

double DataFactoryMock::vincenty_distance(double latitude1, double longitude1, double latitude2,
                         double longitude2)
{
    static double earth_radius_km = 6371.0;

    double lat1 = deg2rad(latitude1);
    double lon1 = deg2rad(longitude1);
    double lat2 = deg2rad(latitude2);
    double lon2 = deg2rad(longitude2);

    double d_lon = std::abs(lon1 - lon2);

    // Numerator
    double a = pow(cos(lat2) * sin(d_lon), 2);

    double b = cos(lat1) * sin(lat2);
    double c = sin(lat1) * cos(lat2) * cos(d_lon);
    double d = pow(b - c, 2);

    double e = sqrt(a + d);

    // Denominator
    double f = sin(lat1) * sin(lat2);
    double g = cos(lat1) * cos(lat2) * cos(d_lon);

    double h = f + g;

    double d_sigma = atan2(e, h);

    return earth_radius_km * d_sigma;
}

bool DataFactoryMock::isInRadius(const Device &dev, double latitude, double longitude, double radius)
{
    double dist = vincenty_distance(latitude, longitude, dev.latitude, dev.longitude);

    return  (dist <= radius);
}

DeviceVec DataFactoryMock::getDevices(double latitude, double longitude,
                                      double kilometersRadius, DataType dataType) const
{
    DeviceVec filtered;

    for (const Device &dev : devices) {
        if (dev.dataType == dataType && isInRadius(dev, latitude, longitude, kilometersRadius))
            filtered.push_back(dev);
    }

    return filtered;
}
