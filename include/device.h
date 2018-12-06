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
    char *pubKey;
    double longitude;
    double attitude;
    DataType dataType;
};

#endif
