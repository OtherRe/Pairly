#ifndef PAIRLY_EXCEPTIONS_H
#define PAIRLY_EXCEPTIONS_H

#include <stdexcept>


class DataBaseException : public std::runtime_error {
public:
    DataBaseException(const std::string &what) : std::runtime_error(what) { }
};

class PairlyLibException : public std::runtime_error {
public:
    PairlyLibException(const std::string &what) : std::runtime_error(what) { }
};

#endif