#include "data-factory-mongo.h"
#include "pairly-exceptions.h"

bool DataFactoryMongo::isConnected()
{
    return connected;
}

void DataFactoryMongo::connect(const std::string &address, const std::string &dbName)
{
    if (connected)
        throw DataBaseException("Database already connected already connected");

    static mongocxx::instance instance{}; // This should be done only once.
    mongocxx::uri uri(address);
    
    poolPtr = std::make_unique<mongocxx::pool>(uri);

    connected = true;
    this->dbName = dbName;
}

bsoncxx::oid DataFactoryMongo::stringToOid(const std::string &str)
{
    return bsoncxx::oid(bsoncxx::stdx::string_view({str}));
}

Device DataFactoryMongo::parseDevice(const bsoncxx::document::view &view)
{
    Device dev;

    dev.name = view["name"].get_utf8().value.to_string();
    dev.id = view["_id"].get_oid().value.to_string();
    dev.user = view["user"].get_utf8().value.to_string();
    dev.pubKey = view["pubKey"].get_utf8().value.to_string();
    dev.longitude = view["location"]["coordinates"][0].get_double();
    dev.latitude = view["location"]["coordinates"][1].get_double();
    dev.dataType = static_cast<DataType>(view["dataType"].get_int32().value);

    return dev;
}

Data DataFactoryMongo::parseData(const bsoncxx::document::view &view)
{
    Data data;

    data.first = view["value"].get_double();
    data.second = view["time"].get_int32();

    return data;
}

std::string DataFactoryMongo::addDevice(const Device &dev)
{
    auto builder = bsoncxx::builder::stream::document{};

    bsoncxx::document::value dev_value = builder
        << "name" << dev.name
        << "user" << dev.user
        << "pubKey" << dev.pubKey
        << "dataType" << (int32_t) dev.dataType
        << "location" << bsoncxx::builder::stream::open_document
            << "type" << "Point"
            << "coordinates" << bsoncxx::builder::stream::open_array
                << dev.longitude << dev.latitude << bsoncxx::builder::stream::close_array
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    bsoncxx::document::view view = dev_value.view();

    mongocxx::pool::entry client = poolPtr->acquire();
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
                (*client)[dbName]["devices"].insert_one(view);


    if (result)
        return result.value().inserted_id().get_oid().value.to_string();
    else
        throw DataBaseException("Adding device failed");
}

DeviceVec DataFactoryMongo::getDevices()
{
    DeviceVec result;

    mongocxx::pool::entry client = poolPtr->acquire();
    mongocxx::cursor cursor = (*client)[dbName]["devices"].find({});

    for (auto view : cursor) {
        result.push_back(parseDevice(view));
    }

    return result;
}

void DataFactoryMongo::removeDevice(const std::string &id)
{
    mongocxx::pool::entry client = poolPtr->acquire();

    (*client)[dbName]["devices"].delete_one(bsoncxx::builder::stream::document{} << "_id"
                            << stringToOid(id) << bsoncxx::builder::stream::finalize);

    (*client)[dbName]["data"].delete_many(bsoncxx::builder::stream::document{} << "device"
                            << stringToOid(id) << bsoncxx::builder::stream::finalize);
    
}

Device DataFactoryMongo::getDeviceById(const std::string &id)
{
    mongocxx::pool::entry client = poolPtr->acquire();
    
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
    (*client)[dbName]["devices"].find_one(bsoncxx::builder::stream::document{} << "_id"
                            << stringToOid(id) << bsoncxx::builder::stream::finalize);

    if (!maybe_result)
        throw DataBaseException("Device with id: " + id + " does not exist");

    return parseDevice(maybe_result->view());
}

DataVec DataFactoryMongo::getDeviceData(const std::string &deviceId,
                                        int timeAfter, int timeBefore)
{
    DataVec data;
    mongocxx::pool::entry client = poolPtr->acquire();

    mongocxx::cursor cursor = (*client)[dbName]["data"].find(
        bsoncxx::builder::stream::document{}
        << "device" << stringToOid(deviceId) << "time"
        << bsoncxx::builder::stream::open_document
        << "$gte" << timeAfter
        << "$lte" << timeBefore
        << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize);

    for (auto d : cursor) {
        data.push_back(parseData(d));
    }

    return data;
}

void DataFactoryMongo::addData(const std::string &deviceId, const Data &data)
{
    auto builder = bsoncxx::builder::stream::document{};
    mongocxx::pool::entry client = poolPtr->acquire();

    bsoncxx::document::value dev_value = builder
        << "device" << stringToOid(deviceId)
        << "value" << data.first
        << "time" << data.second
        << bsoncxx::builder::stream::finalize;

    bsoncxx::document::view view = dev_value.view();

    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
                (*client)[dbName]["data"].insert_one(view);
}

DeviceVec DataFactoryMongo::getDevices(const std::string &user)
{
    DeviceVec dv;
    mongocxx::pool::entry client = poolPtr->acquire();

    mongocxx::cursor cursor = (*client)[dbName]["devices"].find(
        bsoncxx::builder::stream::document{}
        << "user" << user
        << bsoncxx::builder::stream::finalize);

    for (auto dev : cursor) {
        dv.push_back(parseDevice(dev));
    }

    return dv;
}

DeviceVec DataFactoryMongo::getDevices(double latitude, double longitude,
                                       double kilometersRadius, DataType dataType) const
{
    DeviceVec dv;
    mongocxx::pool::entry client = poolPtr->acquire();

    mongocxx::cursor cursor = (*client)[dbName]["devices"].find(
        bsoncxx::builder::stream::document{}
        << "dataType" << static_cast<int>(dataType)
        << "location" << bsoncxx::builder::stream::open_document
        << "$nearSphere" << bsoncxx::builder::stream::open_document
            << "$maxDistance" << kilometersRadius * 1000
            << "$geometry"  << bsoncxx::builder::stream::open_document
                << "type" << "Point"
                    << "coordinates" << bsoncxx::builder::stream::open_array
                    << longitude << latitude << bsoncxx::builder::stream::close_array
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize);

    for (auto d : cursor) {
        dv.push_back(parseDevice(d));
    }

    return dv;
}

void DataFactoryMongo::disconnect()
{
    connected = false;
}