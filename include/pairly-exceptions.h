#ifndef PAIRLY_EXCEPTIONS_H
#define PAIRLY_EXCEPTIONS_H

#include <stdexcept>

/**
 * @class DataBaseException
 * 
 * @brief Exception thrown by classes inheriting from DataFactory, to indicate
 * an in DataBase layer.
 */
class DataBaseException : public std::runtime_error {
public:
    DataBaseException(const std::string &what) : std::runtime_error(what) { }
};

/**
 * @class
 * 
 * @brief Exception thrown by the PairlyDB class
 */
class PairlyLibException : public std::runtime_error {
public:
    PairlyLibException(const std::string &what) : std::runtime_error(what) { }
};

#endif