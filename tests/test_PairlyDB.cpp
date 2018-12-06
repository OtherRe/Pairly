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

  BOOST_CHECK(db.isConnected() == false);
}


BOOST_AUTO_TEST_CASE( connection_connected )
{
  DataFactoryMock db;

  db.connect();

  BOOST_CHECK(db.isConnected() == true);
}


BOOST_AUTO_TEST_CASE( raise_exception_when_db_not_connected1 )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();

  BOOST_CHECK_THROW (pairly.getDevices(), PairlyLibException);
}

BOOST_AUTO_TEST_CASE( raise_exception_when_db_not_connected2 )
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();

  BOOST_CHECK_THROW (pairly.init(&db), PairlyLibException);
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

  BOOST_CHECK(pairly.getDevices().size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
