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

  void printItems() {
    for (auto &item: _vector) {
      std::cout << item << std::endl;
    }
  }

  int getSize() { return _vector.size(); }

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
    enum types { INTEGER, DOUBLE, BOOL, STRING, ARRAY_INT, ARRAY_DOUBLE, ARRAY_BOOL, ARRAY_STRING, NOTY_ARRAY };
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

    void print() {

        if ( type() == ARRAY_INT)  
            _integerArray.printItems();
        else if ( type() == ARRAY_DOUBLE)
            _doubleArray.printItems();
        else if ( type() == ARRAY_BOOL)
            _doubleArray.printItems();
        else if ( type() == ARRAY_STRING)
            _stringArray.printItems();
        
    }

    int getLength() {
        if ( type() ==  ARRAY_INT) {
            return _integerArray.getSize();
        }
        else if ( type() == ARRAY_DOUBLE)
            return _doubleArray.getSize();
        else if ( type() == ARRAY_BOOL)
            return _doubleArray.getSize();
        else if ( type() == ARRAY_STRING)
            return _stringArray.getSize();
        return 0;
    }

    void setInitialized() { initialized = true; }

    void addItem(int i) { _integerArray.addItem(i); }
    void addItem(double d) { _doubleArray.addItem(d); }
    void addItem(bool b) { _doubleArray.addItem(b); }
    void addItem(const char *s) { addItem(std::string(s)); }
    void addItem(std::string s) { _stringArray.addItem(s); }

    bool initialized{false};
    //lawl
    IntArray     _integerArray;
    StringArray  _stringArray;
    DoubleArray  _doubleArray;
    BoolArray    _boolArray;
};



#endif