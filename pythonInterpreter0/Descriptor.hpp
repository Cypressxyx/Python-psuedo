#ifndef __DESCRIPTOR_HPP
#define __DESCRIPTOR_HPP


#include <string>
#include <iostream>

#include "Token.hpp"
#include "Debug.hpp"


class TypeDescriptor {

public:
    enum types { INTEGER, DOUBLE, BOOL, STRING };
    TypeDescriptor(types type):
        _type{type}
    {}
    types &type() { return _type; }
    virtual ~TypeDescriptor() {
        if (destructor)
            std::cout << "~TypeDescriptor" << std::endl;
    }

private:
    types _type;
};

class NumberDescriptor: public TypeDescriptor {

public:
    NumberDescriptor(types descType):
        TypeDescriptor(descType)
    {}

    ~NumberDescriptor() {
        if (destructor)
            std::cout << "~NumberDescriptor" << std::endl;
    }

    union {
        int intValue;
        double doubleValue;
        int boolValue;
    } _value;
};

class StringDescriptor: public TypeDescriptor {

public:
    StringDescriptor(types descType):
        TypeDescriptor(descType)
    {}

    ~StringDescriptor() {
        if (destructor)
            std::cout << "~StringDescriptor" << std::endl;
    }

    std::string _stringValue;
};


#endif