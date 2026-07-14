#ifndef BIGINT_H
#define BIGINT_H
#include <string>
#include <iostream>

class bigint {
    std::string v;
public:
    bigint();
    bigint(unsigned int n);
    bigint(const bigint& o);
    std::string getValue() const;
    bigint& operator=(const bigint& o);
    bigint operator+(const bigint& o) const;
    bigint& operator+=(const bigint& o);
    bigint operator++(int);
    bigint& operator++();
    bigint operator<<(const bigint& o) const;
    bigint operator>>(const bigint& o) const;
    bigint& operator<<=(const bigint& o);
    bigint& operator>>=(const bigint& o);
    bigint operator<<(unsigned int s) const;
    bigint operator>>(unsigned int s) const;
    bigint& operator<<=(unsigned int s);
    bigint& operator>>=(unsigned int s);
    bool operator<(const bigint& o) const;
    bool operator<=(const bigint& o) const;
    bool operator>(const bigint& o) const;
    bool operator>=(const bigint& o) const;
    bool operator==(const bigint& o) const;
    bool operator!=(const bigint& o) const;
};

std::ostream& operator<<(std::ostream& os, const bigint& o);
#endif