#include "pairly.h"
#include "pairly-exceptions.h"
#include <cmath>

#include <algorithm>
#include <iostream>
#include <unordered_map>

PairlyDB& PairlyDB::instance()
{
    static PairlyDB pairlyInstance;

    return pairlyInstance;
}

void PairlyDB::init(DataFactory *dataFactory)
{
    if (!dataFactory->isConnected())
        throw PairlyLibException("Data Factory must be already connected");

    this->dataFactory = dataFactory;
}

void PairlyDB::checkDataFactory() const
{
    if (dataFactory == nullptr)
        throw PairlyLibException("Data Factory not present");

    if (!dataFactory->isConnected())
        throw PairlyLibException("Data Factory must be already connected");
}

double PairlyDB::deg2rad(double deg)
{
    return deg * (M_PI / 180.0);
}

double PairlyDB::vincenty_distance(double latitude1, double longitude1, double latitude2,
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

Device PairlyDB::getDevice(int deviceId) const
{
    checkDataFactory();

    return dataFactory->getDeviceById(deviceId);
}

std::vector<Device> PairlyDB::getDevices(const std::string &user) const
{
    checkDataFactory();

    return dataFactory->getDevices(user);
}

std::vector<Device> PairlyDB::getDevices() const
{
    checkDataFactory();

    return dataFactory->getDevices();
}

int PairlyDB::div_to_nearest(int n, int d)
{
    if (n < 0) {
        return (n - d/2 + 1) / d;
    } else {
        return (n + d/2) / d;
    }
}

bool PairlyDB::isInRadius(const Device &dev, double latitude, double longitude, double radius)
{
    double dist = vincenty_distance(latitude, longitude, dev.latitude, dev.longitude);

    return  (dist <= radius);
}

std::vector<Data> PairlyDB::getDeviceData(int deviceId, int hourInterval, int after, int before) const
{
    checkDataFactory();

    std::vector<Data> data = dataFactory->getDeviceData(deviceId, after, before);

    if (data.empty())
        return data;

    if (hourInterval == 0)
        return data;

    /* Key - result element time value
     * value - pair of sum of measuered values and number of measuered values
     */
    std::unordered_map<int, std::pair<double, int>> map;

    int interval = hourInterval * HOUR;

    for (Data &d : data) {
        int rounded = div_to_nearest(d.second, interval) * interval;

        auto it = map.find(rounded);

        if (it == map.end()) {
            map[rounded] = std::make_pair(d.first, 1);
        } else {
            it->second.first += d.first;
            it->second.second++;
        }
    }

    std::vector<Data> result;
    for (auto it = map.begin(); it != map.end(); it++) {
        result.push_back(std::make_pair(it->second.first / it->second.second, it->first));
    }

    std::sort(result.begin(), result.end(), [](auto &left, auto &right) {
                            return left.second < right.second;});

    return result;
}

void PairlyDB::addDevice(const Device &device)
{
    checkDataFactory();

    dataFactory->addDevice(device);
}

void PairlyDB::removeDevice(int deviceId)
{
    checkDataFactory();

    dataFactory->removeDevice(deviceId);
}

void PairlyDB::addData(int deviceId, const Data &data)
{
    checkDataFactory();

    dataFactory->addData(deviceId, data);
}

std::vector<Device> PairlyDB::getDevices(double longitude, double latitude,
                                         double kilometersRadius, DataType dataType) const
{
    checkDataFactory();

    std::vector<Device> devices = dataFactory->getDevices();
    std::vector<Device> filtered;

    for (Device &dev : devices) {
        if (dev.dataType == dataType && isInRadius(dev, longitude, latitude, kilometersRadius))
            filtered.push_back(dev);
    }

    return filtered;
}

std::vector<Data> PairlyDB::getAreaData(double longitude, double latitude,
                                        double kilometersRadius, int after,
                                        int before, int hoursInterval, DataType dataType) const
{

    std::vector<Data> data;

    int time = after;

    while (time < before) {
        data.emplace_back(0.0, time);
        time += hoursInterval * 60;
    }

    // TODO: calculate actual data
    return data;
}