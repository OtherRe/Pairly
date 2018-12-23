#include <boost/python.hpp>
#include "pairly.h"
#include "data-factory-mongo.h"


using namespace boost::python;

PairlyDB& pairlyMongoConnect(const std::string address,
                             const std::string dbName)
{
    PairlyDb pairly = PairlyDB::instance();
    static DataFactoryMongo dataFactory;

    dataFactory.connect(address, dbName);
    pairly.init(&DataFactoryMongo);

    return pairly;
}

BOOST_PYTHON_MODULE(pairly-db)
{
    enum_<DataType>("DataType")
        .value("CO", DataType::CO)
        .value("PM2_5", DataType::PM2_5)
        .value("PM10", DataType::PM10);

    class_<Device>("Device", init())
        .def(init<std::string>()),
        .def(init<std::string, std::string, std::string,
             std::string, double, double, DataType>());


    class_<PairlyDB>("PairlyDB")
        .def("instance", &PairlyDB::instance)
        .def("set", &World::set)
    ;
}