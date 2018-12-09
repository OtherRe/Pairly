#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "PairlyDB lib tests mock"
#define BOOST_TEST_LOG_LEVEL "all"

#include <boost/test/unit_test.hpp>

#include "pairly.h"
#include "pairly-exceptions.h"
#include "data-factory-mock.h"


BOOST_AUTO_TEST_SUITE( PAIRLYDB_TEST_SUITE )

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE( connection_not_connected )
{
  DataFactoryMock db;

  BOOST_CHECK_EQUAL(db.isConnected(), false);
}


BOOST_AUTO_TEST_CASE( connection_connected )
{
  DataFactoryMock db;

  db.connect();

  BOOST_CHECK_EQUAL(db.isConnected(), true);
}


BOOST_AUTO_TEST_CASE( raise_exception_when_db_not_connected1 )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();

  BOOST_CHECK_THROW(pairly.getDevices(), PairlyLibException);
}

BOOST_AUTO_TEST_CASE( raise_exception_when_db_not_connected2 )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();

  BOOST_CHECK_THROW(pairly.init(&db), PairlyLibException);
}

BOOST_AUTO_TEST_CASE( dont_raise_except_when_connected )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();

  pairly.init(&db);

  Device dev;

  BOOST_CHECK_NO_THROW(pairly.addDevice(dev));
}

BOOST_AUTO_TEST_CASE( device_added )
{
 DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  Device dev;
  pairly.addDevice(dev);

  BOOST_CHECK_EQUAL(pairly.getDevices().size(), 1);
}

BOOST_AUTO_TEST_CASE( device_added_and_found)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  Device dev(0);
  pairly.addDevice(dev);

  BOOST_CHECK(pairly.getDevice(0) == dev);
}

BOOST_AUTO_TEST_CASE( add_same_device_twice )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  Device dev(0);
  pairly.addDevice(dev);

  BOOST_CHECK_THROW(pairly.addDevice(dev), DataBaseException);
}

BOOST_AUTO_TEST_CASE( add_few_devices )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  Device dev(0);
  pairly.addDevice(dev);

  Device dev2(1);
  pairly.addDevice(dev2);

  BOOST_CHECK_EQUAL(pairly.getDevices().size(), 2);
}

BOOST_AUTO_TEST_CASE( add_different_user_devices )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(66, "a", "00", 0.0, 0.0, DataType::CO));
  pairly.addDevice(Device(1, "a", "", 0.0, 0.0, DataType::CO));

  pairly.addDevice(Device(2, "b", "", 0.0, 0.0, DataType::CO));

  BOOST_CHECK_EQUAL(pairly.getDevices("a").size(), 2);
  BOOST_CHECK_EQUAL(pairly.getDevices("b").size(), 1);

  BOOST_CHECK_EQUAL(pairly.getDevices("c").size(), 0);
}

BOOST_AUTO_TEST_CASE( add_data_samples_to_not_existing_device )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);


  BOOST_CHECK_THROW(pairly.addData(0, Data(0.0, 0)), DataBaseException);
}

BOOST_AUTO_TEST_CASE( add_some_data )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  pairly.addData(0, Data(0.0, 0));
  pairly.addData(0, Data(0.0, 1));
  pairly.addData(0, Data(0.0, 2));

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 0).size(), 3);
}

BOOST_AUTO_TEST_CASE( add_some_data_with_no_zero_interval )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  pairly.addData(0, Data(1.0, 0));
  pairly.addData(0, Data(2.0, 1));
  pairly.addData(0, Data(3.0, 2));

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).size(), 1);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).front().first, 2.0);
}

BOOST_AUTO_TEST_CASE( add_more_data_with_no_zero_interval )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  pairly.addData(0, Data(1.0, 0));
  pairly.addData(0, Data(2.0, 1));
  pairly.addData(0, Data(3.0, 2));
  pairly.addData(0, Data(4.0, 3));
  pairly.addData(0, Data(5.0, 4));

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).size(), 1);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).front().first, 3.0);
}



BOOST_AUTO_TEST_SUITE_END()
