#include "pairly.h"
#include "pairly-exceptions.h"

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

bool PairlyDB::isInRadius(const Device &dev, double longitude, double latitude, double radius)
{
    // TODO

    return false;
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

int PairlyDB::div_to_nearest(int n, int d) const
{
    if (n < 0) {
        return (n - d/2 + 1) / d;
    } else {
        return (n + d/2) / d;
    }
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

std::vector<Data> PairlyDB::getAreaData(double longitude, double latitude,
                                        double kilometersRadius, int after,
                                        int before, int hoursInterval, DataType dataType) const
{
    checkDataFactory();

    std::vector<Device> devices = dataFactory->getDevices();
    std::vector<Device> filtered;

    for (Device &dev : devices) {
        if (dev.dataType == dataType && isInRadius(dev, longitude, latitude, kilometersRadius))
            filtered.push_back(dev);
    }

    std::vector<Data> data;

    int time = after;

    while (time < before) {
        data.emplace_back(0.0, time);
        time += hoursInterval * 60;
    }

    // TODO: calculate actual data
    return data;
}