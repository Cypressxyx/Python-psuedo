#ifndef __DESCRIPTOR_HPP
#define __DESCRIPTOR_HPP


#include <string>
#include <iostream>
#include <vector>

#include "Token.hpp"
#include "Debug.hpp"

template <typename T>
class BaseArrayClass {
  public:
  BaseArrayClass(){
    _vector.reserve(100);
  }

  void printItem() {
    for (auto &item: _vector) {
      std::cout << item << std::endl;
    }
  }

  void getSize() { return _vector.size(); }

  std::vector<T> _vector;
};

class IntArray: public BaseArrayClass<int> {
  public:
    IntArray() {}
    void addItem(int i) { _vector.push_back(i); }
};

class StringArray: public BaseArrayClass<std::string> {
  public:
    StringArray() {}
    void addItem(std::string s) { _vector.push_back(s); }
};

class DoubleArray: public BaseArrayClass<double> {
  public:
    DoubleArray() {}
    void addItem(double d) { _vector.push_back(d); }
};

class BoolArray: public BaseArrayClass<bool> {
public:
    BoolArray() {}
    void addItem(bool b) { _vector.push_back(b); }
};

class TypeDescriptor {

public:
    enum types { INTEGER, DOUBLE, BOOL, STRING, ARRAY_INT, ARRAY_DOUBLE, ARRAY_BOOL, ARRAY_STRING };
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

class ArrayDescriptor: public TypeDescriptor {

public:
    ArrayDescriptor(types descType):
        TypeDescriptor(descType)
    {}

    ~ArrayDescriptor() {
        if ( destructor )
            std::cout << "~ArrayDescriptor" << std::endl;
    }

    //lawl
    IntArray     _integerArray;
    StringArray  _stringArray;
    DoubleArray  _doubleArray;
    BoolArray    _boolArray;
};



#endif