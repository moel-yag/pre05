#include "bigint.hpp"
#include <string>
#include <sstream>
#include <iostream>

// --- Constructors / Canonical form ---

bigint::bigint() : _value("0") {}
bigint::bigint(unsigned int n) {
    std::stringstream ss;
    ss << n;
    _value = ss.str();
}
bigint::bigint(const bigint& other) : _value(other._value) {}
bigint::~bigint() {}

bigint& bigint::operator=(const bigint& other) {
    if (this != &other)
        _value = other._value;
    return *this;
}

const std::string bigint::getValue() const {
    return _value;
}

// --- Arithmetic ---

bigint bigint::operator+(const bigint& other) const {
    bigint result;
    std::string res;
    int i = _value.size() - 1;
    int j = other._value.size() - 1;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry) {
        if (i >= 0) carry += _value[i--] - '0';
        if (j >= 0) carry += other._value[j--] - '0';
        res.insert(res.begin(), char((carry % 10) + '0'));
        carry /= 10;
    }
    result._value = res;
    return result;
}

bigint& bigint::operator+=(const bigint& other) {
    *this = *this + other;
    return *this;
}

bigint bigint::operator++(int) {
    bigint tmp(*this);
    *this = *this + bigint(1);
    return tmp;
}

bigint& bigint::operator++() {
    *this = *this + bigint(1);
    return *this;
}

// --- Decimal shift (unsigned int) ---

bigint bigint::operator<<(unsigned int shift) const {
    bigint tmp(*this);
    if (tmp._value == "0")
        return tmp;
    for (unsigned int i = 0; i < shift; i++)
        tmp._value.push_back('0');
    return tmp;
}

bigint& bigint::operator<<=(unsigned int shift) {
    *this = *this << shift;
    return *this;
}

bigint bigint::operator>>(unsigned int shift) const {
    bigint tmp(*this);
    if (tmp._value == "0")
        return tmp;
    if (shift >= tmp._value.size())
        tmp._value = "0";
    else
        tmp._value.erase(tmp._value.size() - shift);
    return tmp;
}

bigint& bigint::operator>>=(unsigned int shift) {
    *this = *this >> shift;
    return *this;
}

// --- Decimal shift (bigint) ---

bigint bigint::operator<<(const bigint& other) const {
    std::stringstream ss(other._value);
    unsigned int shift;
    ss >> shift;
    return *this << shift;
}

bigint& bigint::operator<<=(const bigint& other) {
    *this = *this << other;
    return *this;
}

bigint bigint::operator>>(const bigint& other) const {
    std::stringstream ss(other._value);
    unsigned int shift;
    ss >> shift;
    return *this >> shift;
}

bigint& bigint::operator>>=(const bigint& other) {
    *this = *this >> other;
    return *this;
}

// --- Comparison ---

bool bigint::operator<(const bigint& other) const {
    if (_value.size() != other._value.size())
        return _value.size() < other._value.size();
    return _value < other._value;
}

bool bigint::operator>(const bigint& other) const {
    return other < *this;
}

bool bigint::operator<=(const bigint& other) const {
    return !(other < *this);
}

bool bigint::operator>=(const bigint& other) const {
    return !(*this < other);
}

bool bigint::operator==(const bigint& other) const {
    return _value == other._value;
}

bool bigint::operator!=(const bigint& other) const {
    return _value != other._value;
}

// --- Stream output ---

std::ostream& operator<<(std::ostream& os, const bigint& obj) {
    os << obj.getValue();
    return os;
}
