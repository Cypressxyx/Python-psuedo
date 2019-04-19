#include <stack>
#include <iostream>
#include "SymTab.hpp"

// https://stackoverflow.com/questions/41871115/why-would-i-stdmove-an-stdshared-ptr

/* void SymTab::addDescriptor(std::string vName, TypeDescriptor descriptor) {
    if (symTab.size() > 0)
        (stack.top())[vName] = std::move(descriptor);
    else
        globalSymTab[vName] = std::move(descriptor);
}*/


void SymTab::addDescriptor(std::string vName, std::shared_ptr<NumberDescriptor> descriptor) {
    if ( symTab.size() > 0 )
        (symTab.top())[vName] = std::move(descriptor);
    else
        globalSymTab[vName] = std::move(descriptor);
}

void SymTab::createEntryFor(std::string vName, int value) {

    if (debug)
        std::cout << "SymTab::createEntryFor(INT) ->" << value << "<-" << std::endl;
    auto descriptor = std::make_shared<NumberDescriptor>(TypeDescriptor::INTEGER);
    descriptor->_value.intValue = value;
    addDescriptor(vName, descriptor);
    //globalSymTab[vName] = std::move(descriptor);
}

void SymTab::createEntryFor(std::string vName, double value) {

    if (debug)
        std::cout << "SymTab::createEntryFor(DOUBLE) ->" << value << "<-" << std::endl;

    auto descriptor = std::make_shared<NumberDescriptor>(TypeDescriptor::DOUBLE);
    descriptor->_value.doubleValue = value;
    addDescriptor(vName, descriptor);
}

void SymTab::createEntryFor(std::string vName, bool value) {

    if (debug)
        std::cout << "SymTab::createEntryFor(BOOL) ->" << value << "<-" << std::endl;

    auto descriptor = std::make_shared<NumberDescriptor>(TypeDescriptor::BOOL);
    descriptor->_value.boolValue = (int) value;
    addDescriptor(vName, descriptor);
}

void SymTab::createEntryFor(std::string vName, std::string value) {
    if (debug)
        std::cout << "SymTab::createEntryFor(STRING) ->" << value << "<-" << std::endl;

    auto descriptor = std::make_shared<StringDescriptor>(TypeDescriptor::STRING);
    descriptor->_stringValue = value;
    if ( symTab.size() > 0 )
        (symTab.top())[vName] = std::move(descriptor);
    else
        globalSymTab[vName] = std::move(descriptor);
}

void SymTab::setValueFor(std::string vName, std::shared_ptr<TypeDescriptor> sp) {
    
    if ( symTab.size() > 0 )
        (symTab.top())[vName] = /*std::move(*/sp/*)*/;
    else
        globalSymTab[vName] = /*std::move(*/sp/*)*/;

    
    
    // globalSymTab[vName] = std::move(sp);
}

bool SymTab::isDefined(std::string vName) {
    
    if ( symTab.size() > 0 )
        return (symTab.top()).find(vName) != (symTab.top()).end();
    return globalSymTab.find(vName) != globalSymTab.end();
}

bool SymTab::erase(std::string vName) {
    if (isDefined(vName)) {

        if ( symTab.size() > 0 ) {
            auto iterator = (symTab.top()).find(vName);
            (symTab.top()).erase(iterator);
        } else {
            auto iterator = globalSymTab.find(vName);
            globalSymTab.erase(iterator);
        }
        return true;
    }
    return false;
}

TypeDescriptor *SymTab::getValueFor(std::string vName) {

    if ( !isDefined(vName) ) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }

    if (debug)
        std::cout << "SymTab::getValueFor: " << vName << "\n";

    return globalSymTab.find(vName)->second.get();

}

void SymTab::openScope() {

    std::map<std::string, std::shared_ptr<TypeDescriptor>> newScope;
    symTab.push(newScope);
}

void SymTab::closeScope() {

    if ( symTab.size() == 0 ) {
        std::cout << "SymTab::closeScope() -> can't close scope - stack size is zero" << std::endl;
        exit(1);
    }
    symTab.pop();
}

