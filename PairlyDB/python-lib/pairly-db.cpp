#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "pairly.h"
#include "data-factory-mongo.h"


using namespace boost::python;

void pairlyMongoConnect(const std::string address,
                        const std::string dbName)
{
    PairlyDB &pairly = PairlyDB::instance();
    static DataFactoryMongo dataFactory;

    dataFactory.connect(address, dbName);
    pairly.init(&dataFactory);
}

typedef DeviceVec (PairlyDB::*GetDeviceStr)(const std::string&) const;
GetDeviceStr getDevStr = &PairlyDB::getDevices;

typedef DeviceVec (PairlyDB::*GetDeviceNone)() const;
GetDeviceNone getDevNone = &PairlyDB::getDevices;

typedef DeviceVec (PairlyDB::*GetDeviceGeo)(double, double, double, DataType) const;
GetDeviceGeo getDevGeo = &PairlyDB::getDevices;




BOOST_PYTHON_MODULE(pypairly)
{
    class_<Data>("Data")
        .def(init<double, int>())
        .def_readwrite("first", &Data::first)
        .def_readwrite("second", &Data::second);


    enum_<DataType>("DataType")
        .value("CO", DataType::CO)
        .value("PM2_5", DataType::PM2_5)
        .value("PM10", DataType::PM10);

    class_<Device>("Device")
        .def(init<std::string>())
        .def(init<std::string, std::string, std::string,
             std::string, double, double, DataType>())
        .def_readwrite("id", &Device::id)
        .def_readwrite("user", &Device::user)
        .def_readwrite("pubKey", &Device::pubKey)
        .def_readwrite("name", &Device::name)
        .def_readwrite("latitude", &Device::latitude)
        .def_readwrite("longitude", &Device::longitude)
        .def_readwrite("dataType", &Device::dataType);

    class_<std::vector<Device> >("DeviceVec")
        .def(vector_indexing_suite<std::vector<Device> >());

    class_<std::vector<Data> >("DataVec")
        .def(vector_indexing_suite<std::vector<Data> >());
    
    class_<PairlyDB, boost::noncopyable>("PairlyDB", no_init)
        .def("instance", &PairlyDB::instance,
         return_value_policy<reference_existing_object>())
        .def("connect", pairlyMongoConnect)
        .def("getDevice", &PairlyDB::getDevice)
        .def("getDevices", getDevStr)
        .def("getDevices", getDevNone)
        .def("getDevices", getDevGeo)
        .def("addDevice", &PairlyDB::addDevice)
        .def("removeDevice", &PairlyDB::removeDevice)
        .def("addData", &PairlyDB::addData)
        .def("getAreaData", &PairlyDB::getAreaData)
        .def("getDeviceData", &PairlyDB::getDeviceData2)
        .def("getDeviceData", &PairlyDB::getDeviceData3)
        .def("getDeviceData", &PairlyDB::getDeviceData4);
}