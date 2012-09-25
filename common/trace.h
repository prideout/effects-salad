#pragma once

#include <string>

class Trace {

};

class TraceScope {
    std::string _scopeName;
    float _start;
public:

    TraceScope(std::string scopeName);
    ~TraceScope();
};
