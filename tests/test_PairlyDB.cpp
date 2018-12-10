#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "PairlyDB lib tests mock"
#define BOOST_TEST_LOG_LEVEL "all"

#include <boost/test/unit_test.hpp>

#include "pairly.h"
#include "pairly-exceptions.h"
#include "data-factory-mock.h"
#include <ctime>
#include <iostream>


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

  time_t t = std::time(nullptr);

  pairly.addData(0, Data(1.0, t + 0));
  pairly.addData(0, Data(2.0, t + 1));
  pairly.addData(0, Data(3.0, t + 2));

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

  time_t t = std::time(nullptr);

  pairly.addData(0, Data(1.0, t + 0));
  pairly.addData(0, Data(2.0, t + 1));
  pairly.addData(0, Data(3.0, t + 2));
  pairly.addData(0, Data(4.0, t + 3));
  pairly.addData(0, Data(5.0, t + 4));

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).size(), 1);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).front().first, 3.0);
}

BOOST_AUTO_TEST_CASE(add_different_time_data)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  time_t t = 3600;

  pairly.addData(0, Data(1.0, t + 0));
  pairly.addData(0, Data(4.0, t + 1700));
  pairly.addData(0, Data(3.0, t + 1900));
  pairly.addData(0, Data(3.0, t + 3000));
  pairly.addData(0, Data(2.0, t + 5500));
  pairly.addData(0, Data(2.0, t + 5800));

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).size(), 3);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1)[0].first, 2.5);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1)[1].first, 3.0);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1)[2].first, 2.0);
}

BOOST_AUTO_TEST_CASE(add_different_time_data2)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  time_t t = 3620;

  pairly.addData(0, Data(1.0, t + 0));
  pairly.addData(0, Data(4.0, t + 1700));
  pairly.addData(0, Data(3.0, t + 1900));
  pairly.addData(0, Data(3.0, t + 3000));
  pairly.addData(0, Data(2.0, t + 5500));
  pairly.addData(0, Data(2.0, t + 5800));

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).size(), 3);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1)[0].first, 2.5);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1)[1].first, 3.0);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1)[2].first, 2.0);
}


BOOST_AUTO_TEST_CASE(add_different_time_data3)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  time_t t = 3600;

  pairly.addData(0, Data(1.0, t + 0));
  pairly.addData(0, Data(4.0, t + 1700));
  pairly.addData(0, Data(3.0, t + 3600 + 1900));
  pairly.addData(0, Data(3.0, t + 3600 + 3000));
  pairly.addData(0, Data(2.0, t + 7200 + 5500));
  pairly.addData(0, Data(2.0, t + 7200 + 5800));

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).size(), 3);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1)[0].first, 2.5);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1)[1].first, 3.0);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1)[2].first, 2.0);
}

BOOST_AUTO_TEST_CASE(add_data_filtered)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  time_t t = 3600;

  pairly.addData(0, Data(1.0, t + 0));
  pairly.addData(0, Data(4.0, t + 1700));
  pairly.addData(0, Data(3.0, t + 1900));
  pairly.addData(0, Data(3.0, t + 3000));
  pairly.addData(0, Data(2.0, t + 7200 + 5500));
  pairly.addData(0, Data(2.0, t + 7200 + 5800));
  pairly.addData(0, Data(2.0, t + 7200 + 6600));
  pairly.addData(0, Data(2.0, t + 7200 + 800));

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1, 0, 7200).size(), 2);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1, 0, 7200)[0].first, 2.5);
}


BOOST_AUTO_TEST_CASE(add_data_filtered_dense)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  time_t t = 3600;

  pairly.addData(0, Data(4.0, t + 0));
  pairly.addData(0, Data(4.0, t + 1700));
  pairly.addData(0, Data(4.0, t + 1750));
  pairly.addData(0, Data(4.0, t + 1760));
  pairly.addData(0, Data(4.0, t + 1770));
  pairly.addData(0, Data(4.0, t + 1780));
  pairly.addData(0, Data(4.0, t + 1790));


  pairly.addData(0, Data(3.0, t + 1900));
  pairly.addData(0, Data(3.0, t + 1920));
  pairly.addData(0, Data(3.0, t + 1930));
  pairly.addData(0, Data(3.0, t + 1940));
  pairly.addData(0, Data(3.0, t + 1950));
  pairly.addData(0, Data(3.0, t + 1960));
  pairly.addData(0, Data(3.0, t + 3000));
  pairly.addData(0, Data(2.0, t + 7200 + 5500));
  pairly.addData(0, Data(2.0, t + 7200 + 5800));
  pairly.addData(0, Data(2.0, t + 7200 + 6600));
  pairly.addData(0, Data(2.0, t + 7200 + 800));

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1, 0, 7200).size(), 2);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1, 0, 7200)[0].first, 4.0);
  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1, 0, 7200)[1].first, 3.0);
}


BOOST_AUTO_TEST_CASE(add_sparse_data)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  time_t t = std::time(nullptr);

  for (int i = 0; i < 150; i++) {
    pairly.addData(0, Data(i, t + i * 3600));
  }

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).size(), 150);

  std::vector<Data> result = pairly.getDeviceData(0, 1);

  for (int i = 0; i < 150; i++) {
    BOOST_CHECK_EQUAL(result[i].first, i);
  }
}

BOOST_AUTO_TEST_CASE(add_data_two_on_one_sample_simple)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  time_t t = 3600;

  for (int i = 0; i < 4; i++) {
    pairly.addData(0, Data(i, t + i * 1800));
  }

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).size(), 2);
}


BOOST_AUTO_TEST_CASE(add_data_two_on_one_sample)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0));

  time_t t = 3600;

  // FIXME: undefined behaviour
  for (int i = 0; i < 36000; i++) {
    pairly.addData(0, Data(i, t + i * 1800));
  }

  BOOST_CHECK_EQUAL(pairly.getDeviceData(0, 1).size(), 18000);
}

BOOST_AUTO_TEST_CASE(get_devices_in_radius)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0, "a", "", 52.230055, 21.010606, DataType::CO)); // Warszawa cetnrum
  pairly.addDevice(Device(1, "a", "", 52.229444, 21.011357, DataType::CO)); // Warszawa centrum

  pairly.addDevice(Device(2, "a", "", 52.327085, 21.005208, DataType::CO)); // Warszawa - Bialoleka
  pairly.addDevice(Device(3, "a", "", 52.210512, 21.007089, DataType::CO)); // Warszawa - Mokotow


  BOOST_CHECK_EQUAL(pairly.getDevices(52.230250, 21.011985, 1, DataType::CO).size(), 2);
}

BOOST_AUTO_TEST_CASE(get_devices_in_radius2)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0, "a", "", 52.230055, 21.010606, DataType::CO)); // Warszawa cetnrum
  pairly.addDevice(Device(1, "a", "", 52.229444, 21.011357, DataType::CO)); // Warszawa centrum

  pairly.addDevice(Device(2, "a", "", 52.327085, 21.005208, DataType::CO)); // Warszawa - Bialoleka
  pairly.addDevice(Device(3, "a", "", 52.210512, 21.007089, DataType::CO)); // Warszawa - Mokotow


  BOOST_CHECK_EQUAL(pairly.getDevices(52.230250, 21.011985, 8.0, DataType::CO).size(), 3);
}

BOOST_AUTO_TEST_CASE(get_devices_in_radius3)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0, "a", "", 52.230055, 21.010606, DataType::CO)); // Warszawa cetnrum
  pairly.addDevice(Device(1, "a", "", 52.229444, 21.011357, DataType::CO)); // Warszawa centrum

  pairly.addDevice(Device(2, "a", "", 52.327085, 21.005208, DataType::CO)); // Warszawa - Bialoleka
  pairly.addDevice(Device(3, "a", "", 52.210512, 21.007089, DataType::CO)); // Warszawa - Mokotow


  BOOST_CHECK_EQUAL(pairly.getDevices(52.230250, 21.011985, 7.0, DataType::CO).size(), 3);
}

BOOST_AUTO_TEST_CASE(get_devices_in_radius_south_hemisphere)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0, "a", "", 52.230055, 21.010606, DataType::CO)); // Warszawa cetnrum
  pairly.addDevice(Device(1, "a", "", 52.229444, 21.011357, DataType::CO)); // Warszawa centrum


  pairly.addDevice(Device(2, "a", "", 52.327085, 21.005208, DataType::CO)); // Warszawa - Bialoleka
  pairly.addDevice(Device(3, "a", "", 52.210512, 21.007089, DataType::CO)); // Warszawa - Mokotow

  pairly.addDevice(Device(4, "a", "", -52.229444, 21.011357, DataType::CO)); // south hemisphere
  pairly.addDevice(Device(5, "a", "", -52.229444, 21.011357, DataType::CO)); // south hemisphere



  BOOST_CHECK_EQUAL(pairly.getDevices(52.230250, 21.011985, 15.0, DataType::CO).size(), 4);
}

BOOST_AUTO_TEST_CASE(get_devices_in_radius_west_hemisphere)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0, "a", "", 52.230055, 21.010606, DataType::CO)); // Warszawa cetnrum
  pairly.addDevice(Device(1, "a", "", 52.229444, 21.011357, DataType::CO)); // Warszawa centrum

  pairly.addDevice(Device(2, "a", "", 52.327085, 21.005208, DataType::CO)); // Warszawa - Bialoleka
  pairly.addDevice(Device(3, "a", "", 52.210512, 21.007089, DataType::CO)); // Warszawa - Mokotow

  pairly.addDevice(Device(6, "a", "", 52.229444, -21.011357, DataType::CO)); // West hemisphere
  pairly.addDevice(Device(7, "a", "", 52.210512, -21.007089, DataType::CO)); // West hemisphere


  BOOST_CHECK_EQUAL(pairly.getDevices(52.230250, 21.011985, 15.0, DataType::CO).size(), 4);
}

BOOST_AUTO_TEST_CASE(get_devices_in_radius_west_south_hemisphere)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0, "a", "", 52.230055, 21.010606, DataType::CO)); // Warszawa cetnrum
  pairly.addDevice(Device(1, "a", "", 52.229444, 21.011357, DataType::CO)); // Warszawa centrum

  pairly.addDevice(Device(2, "a", "", 52.327085, 21.005208, DataType::CO)); // Warszawa - Bialoleka
  pairly.addDevice(Device(3, "a", "", 52.210512, 21.007089, DataType::CO)); // Warszawa - Mokotow

  pairly.addDevice(Device(6, "a", "", -52.229444, -21.011357, DataType::CO)); // West hemisphere
  pairly.addDevice(Device(7, "a", "", -52.210512, -21.007089, DataType::CO)); // West hemisphere


  BOOST_CHECK_EQUAL(pairly.getDevices(52.230250, 21.011985, 15.0, DataType::CO).size(), 4);
}


BOOST_AUTO_TEST_CASE(get_devices_in_radius_warsaw_cracow)
{
  DataFactoryMock db;

  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0, "a", "", 52.230055, 21.010606, DataType::CO)); // Warszawa cetnrum
  pairly.addDevice(Device(1, "a", "", 52.229444, 21.011357, DataType::CO)); // Warszawa centrum
  pairly.addDevice(Device(2, "a", "", 52.327085, 21.005208, DataType::CO)); // Warszawa - Bialoleka
  pairly.addDevice(Device(3, "a", "", 52.210512, 21.007089, DataType::CO)); // Warszawa - Mokotow
  pairly.addDevice(Device(4, "a", "", 52.238840, 20.987695, DataType::CO)); // Warszawa - Wola
  pairly.addDevice(Device(5, "a", "", 52.277088, 20.959614, DataType::CO)); // Warszawa - Bielany
  
  pairly.addDevice(Device(6, "a", "", 50.055479, 19.936357, DataType::CO)); // Krakow - Wawel
  pairly.addDevice(Device(7, "a", "", 50.047132, 19.946852, DataType::CO)); // Krakow - Kazimierz
  pairly.addDevice(Device(8, "a", "", 50.069877, 19.938436, DataType::CO)); // Kraków - Kleparz
  
  // Warszawa centrum
  BOOST_CHECK_EQUAL(pairly.getDevices(52.230250, 21.011985, 15.0, DataType::CO).size(), 6);
  // Krakow rynek
  BOOST_CHECK_EQUAL(pairly.getDevices(50.062603, 19.937196, 15.0, DataType::CO).size(), 3);
}

BOOST_AUTO_TEST_CASE(get_data_from_radius)
{
  DataFactoryMock db;
  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0, "a", "", 52.230055, 21.010606, DataType::CO)); // Warszawa cetnrum
  pairly.addDevice(Device(1, "a", "", 52.229444, 21.011357, DataType::CO)); // Warszawa centrum
  pairly.addDevice(Device(6, "a", "", 50.055479, 19.936357, DataType::CO)); // Krakow - Wawel

  time_t t = 3600;

  pairly.addData(0, Data(1.0, t + 0));
  pairly.addData(0, Data(4.0, t + 1700));
  pairly.addData(0, Data(3.0, t + 3600 + 1900));
  pairly.addData(0, Data(3.0, t + 3600 + 3000));
  pairly.addData(0, Data(2.0, t + 7200 + 5500));
  pairly.addData(0, Data(2.0, t + 7200 + 5800));

  pairly.addData(1, Data(1.0, t + 0));
  pairly.addData(1, Data(4.0, t + 1700));
  pairly.addData(1, Data(3.0, t + 3600 + 1900));
  pairly.addData(1, Data(3.0, t + 3600 + 3000));
  pairly.addData(1, Data(2.0, t + 7200 + 5500));
  pairly.addData(1, Data(2.0, t + 7200 + 5800));

  DataVec data = pairly.getAreaData(52.230250, 21.011985, 5.0, 0, 100000, 1, DataType::CO);
  BOOST_CHECK_EQUAL(data.size(), 3);
  BOOST_CHECK_EQUAL(data[0].first, 2.5);
  BOOST_CHECK_EQUAL(data[1].first, 3.0);
  BOOST_CHECK_EQUAL(data[2].first, 2.0);
}

BOOST_AUTO_TEST_CASE(get_data_from_radius2)
{
  DataFactoryMock db;
  PairlyDB &pairly = PairlyDB::instance();
  db.connect();
  pairly.init(&db);

  pairly.addDevice(Device(0, "a", "", 52.230055, 21.010606, DataType::CO)); // Warszawa cetnrum
  pairly.addDevice(Device(1, "a", "", 52.229444, 21.011357, DataType::CO)); // Warszawa centrum
  pairly.addDevice(Device(6, "a", "", 50.055479, 19.936357, DataType::CO)); // Krakow - Wawel

  time_t t = 3600;

  pairly.addData(0, Data(1.0, t + 0));
  pairly.addData(0, Data(1.0, t + 1700));
  pairly.addData(0, Data(3.0, t + 3600 + 1900));
  pairly.addData(0, Data(3.0, t + 3600 + 3000));
  pairly.addData(0, Data(5.0, t + 7200 + 5500));
  pairly.addData(0, Data(5.0, t + 7200 + 5800));

  pairly.addData(1, Data(2.0, t + 0));
  pairly.addData(1, Data(2.0, t + 1700));
  pairly.addData(1, Data(4.0, t + 3600 + 1900));
  pairly.addData(1, Data(4.0, t + 3600 + 3000));
  pairly.addData(1, Data(6.0, t + 7200 + 5500));
  pairly.addData(1, Data(6.0, t + 7200 + 5800));

  DataVec data = pairly.getAreaData(52.230250, 21.011985, 5.0, 0, 100000, 1, DataType::CO);
  BOOST_CHECK_EQUAL(data.size(), 3);
  BOOST_CHECK_EQUAL(data[0].first, 1.5);
  BOOST_CHECK_EQUAL(data[1].first, 3.5);
  BOOST_CHECK_EQUAL(data[2].first, 5.5);
}

BOOST_AUTO_TEST_SUITE_END()
