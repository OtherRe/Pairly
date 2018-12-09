#include "pairly.h"
#include "pairly-exceptions.h"

#include <algorithm>
#include <iostream>

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

std::vector<Data> PairlyDB::getDeviceData(int deviceId, int hourInterval, int after, int before) const
{
    checkDataFactory();

    std::vector<Data> data = dataFactory->getDeviceData(deviceId, after, before);

    /* sort by time */
    std::sort(data.begin(), data.end(), [](auto &left, auto &right) {
                                return left.second < right.second;});

    int currentInterval = std::max(0, after - (hourInterval * 60 / 2));

    return averageData(data, currentInterval, hourInterval);
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
    devices.erase(std::remove_if(devices.begin(), devices.end(), [&](Device& d){
        return !(d.dataType == dataType || isInRadius(d, longitude, latitude, kilometersRadius));
    }));


    std::vector<Data> data;
    int time = after - hoursInterval * 60 / 2;

    while (time < before) {
        // double sum

        data.emplace_back(0.0, time);
        time += hoursInterval * 60;
    }

    // TODO: calculate actual data
    return data;
}

std::vector<Data> PairlyDB::averageData(const std::vector<Data>& data, int currentInterval, int hourInterval) const
{
    std::vector<Data> averagedData;

    Data current = {0.0, currentInterval};
    int nElements = 0;

    for (const Data &d : data) {
        if (d.second < currentInterval + hourInterval * 60) {
            current.first += d.first;
            nElements++;
        } else {
            current.first = current.first / nElements;
            averagedData.push_back(current);
            current.first = 0;
            current.second += hourInterval * 60;
            currentInterval += hourInterval * 60;
            nElements = 0;
        }
    }

    if (current.second < currentInterval + hourInterval * 60) {
        current.first = current.first / nElements;
        averagedData.push_back(current);
    }
 
    return averagedData; 
}
