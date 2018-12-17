#ifndef DEVICE_H
#define DEVICE_H

#include <string>

enum class DataType {
    CO, PM2_5, PM10
};

class Device {
public:
    std::string id;
    std::string user;
    std::string pubKey;
    std::string name;
    double latitude;
    double longitude;
    DataType dataType;

    Device() { }
    Device(const std::string &id) : id(id) {
        user = "";
        pubKey = "";
        longitude = 0.0;
        latitude = 0.0;
        dataType = DataType::CO;
    }

    Device(const std::string &id, const std::string &user, const std::string &pubKey,
           const std::string &name, double latitude,
           double longitude, DataType dataType) : id(id), user(user), pubKey(pubKey),  name(name),
           latitude(latitude), longitude(longitude), dataType(dataType) { }


    bool operator==(const Device &rhs) {
        return (id == rhs.id && user == rhs.user && pubKey == rhs.pubKey &&
                longitude == rhs.longitude && latitude == rhs.latitude &&
                dataType == rhs.dataType);
    }

    std::string devTypeStr() {
        switch (dataType) {
        case DataType::CO:
            return "CO";
        case DataType::PM2_5:
            return "PM2_5";
        case DataType::PM10:
            return "PM10";
        }
    }
};

#endif
