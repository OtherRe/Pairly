#ifndef DATA_FACTORY_MONGO_H
#define DATA_FACTORY_MONGO_H

#include <vector>
#include <limits>
#include <map>
#include <memory>

#include "device.h"
#include "data-factory.h"
#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

class DataFactoryMongo : public DataFactory {
public:
    virtual bool isConnected();
    virtual DeviceVec getDevices();
    virtual std::string addDevice(const Device &dev);
    virtual void removeDevice(const std::string &id);
    virtual Device getDeviceById(const std::string &id);
    virtual DataVec getDeviceData(const std::string &deviceId,
                                            int timeAfter = 0, int timeBefore = std::numeric_limits<int>::max());
    virtual void addData(const std::string &deviceId, const Data &data);
    virtual DeviceVec getDevices(const std::string &user);
    virtual DeviceVec getDevices(double latitude, double longitude,
                                 double kilometersRadius, DataType dataType) const;

    void connect(const std::string &address, const std::string &dbName);
    void disconnect();

private:
    using CollectionPtr = std::unique_ptr<mongocxx::collection>;

    bool connected = false;
    mongocxx::database db;

    static bsoncxx::oid stringToOid(const std::string &str);
    static Device parseDevice(const bsoncxx::document::view &view);
    static Data parseData(const bsoncxx::document::view &view);
};

#endif
