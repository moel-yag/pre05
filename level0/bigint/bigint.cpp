#include "bigint.hpp"
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <ostream>

bigint::bigint() : _value("0") {} 
bigint::bigint(unsigned int n) {
    std::stringstream ss;
    ss << n;
    _value = ss.str();
}
bigint::bigint(const bigint& other) : _value(other._value) {} 

bigint::~bigint() {} 

bigint &bigint::operator=(const bigint& other) {
    if (this != &other)
        _value = other._value;
    return *this;
}

const std::string bigint::getValue() const {
    return _value;
}

bigint bigint::operator+(const bigint& other) const { // "231" + "48"
    bigint result;
    std::string res;
    int n1 = this->_value.size() - 1;
    int n2 = other._value.size() - 1;
    int rest = 0; // carry

    while(n1 >= 0 || n2 >= 0 || rest) {
        if (n1 >= 0) {
            rest = rest + _value[n1] - '0'; // 0 + 1 = 1
            n1--;
        }
        if (n2 >= 0)
            rest += other._value[n2--] - '0'; // 1 + 2 = 3

        res.insert(res.begin(), char((rest % 10)+'0'));

        rest = rest / 10;
    }
    result._value = res;
    return result;
}
bigint &bigint::operator+=(const bigint& other) {
    *this = *this + other;
    return *this;
}
bigint bigint::operator++(int) { // a++;
    bigint tmp(*this);
    *this = *this + bigint(1);
    return tmp;
}
bigint &bigint::operator++() { // ++a;
    bigint tmp(1);
    *this = *this + tmp;
    return *this;
}
//---
bigint bigint::operator<<(unsigned int shift) const { // 42 << 3 == 42000
    bigint tmp(*this);
    if (tmp._value == "0")
        return tmp;
    for (unsigned int i = 0; i < shift; i++) {
        tmp._value.push_back('0');
    }
    return tmp;
}
bigint &bigint::operator<<=(unsigned int shift) {
    *this = *this << shift;
    return *this;
}
bigint bigint::operator>>(unsigned int shift) const {
    bigint tmp(*this);
    if (_value == "0")
        return tmp;
    if (shift > _value.size())
        tmp._value = "0";
    else
        tmp._value.erase(_value.size() - shift);
    return tmp;
}
bigint &bigint::operator>>=(unsigned int shift) {
    *this = *this >> shift;
    return *this;
}
//--
bigint bigint::operator<<(const bigint& other) const {
    std::stringstream ss(other._value);
    unsigned int shift;
    ss >> shift;
    return *this << shift;
}
bigint &bigint::operator<<=(const bigint& other) {
    *this = *this << other;
    return *this;
}
bigint bigint::operator>>(const bigint& other) const {
    std::stringstream ss(other._value);
    unsigned int shift;
    ss >> shift;
    return *this >> shift;
}
bigint &bigint::operator>>=(const bigint& other) {
    std::stringstream ss(other._value);
    unsigned int shift;
    ss >> shift;
    *this = *this >> shift;
    return *this;
}
bool   bigint::operator<(const bigint& other) const {
    if (_value.size() != other._value.size())
        return _value.size() < other._value.size();
    return _value < other._value;
}
//
bool   bigint::operator>(const bigint& other) const { // < >=
    return !(other < *this) ; // a > b
}
bool   bigint::operator<=(const bigint& other) const {
    return !(*this > other);
}
bool   bigint::operator>=(const bigint& other) const {
    return !(*this < other);
}
bool   bigint::operator==(const bigint& other) const {
    return _value == other._value;
}
bool   bigint::operator!=(const bigint& other) const {
    return _value != other._value;
}
std::ostream &operator<<(std::ostream& os, const bigint& obj) {
    os << obj.getValue();
    return os;
}
