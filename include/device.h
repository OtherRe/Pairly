#ifndef DEVICE_H
#define DEVICE_H

#include <string>

enum class DataType {
    CO, PM2_5, PM10
};

class Device {
public:
    int id;
    std::string user;
    std::string pubKey;
    double longitude;
    double latitude;
    DataType dataType;

    Device() { }
    Device(int id) : id(id) { 
        user = "";
        pubKey = "";
        longitude = 0.0;
        latitude = 0.0;
        dataType = DataType::CO;
    }

    Device(int id, const std::string &user, const std::string &pubKey, double longitude,
           double attitude, DataType dataType) : id(id), user(user), pubKey(pubKey), longitude(longitude),
           latitude(latitude),  dataType(dataType) { }


    bool operator==(const Device &rhs) {
        return (id == rhs.id && user == rhs.user && pubKey == rhs.pubKey &&
                longitude == rhs.longitude && latitude == rhs.latitude &&
                dataType == rhs.dataType);
    }
};

#endif
