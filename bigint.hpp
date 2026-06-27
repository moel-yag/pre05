#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <string>

class bigint {
private:
    std::string _value;
public:
    bigint();
    bigint(unsigned int n);
    bigint(const bigint& other);
    ~bigint();

    const std::string getValue() const;

    bigint &operator=(const bigint& other);

    bigint operator+(const bigint& other) const;
    bigint &operator+=(const bigint& other);

    bigint operator++(int);
    bigint &operator++();

    bigint operator<<(const bigint& other) const;
    bigint operator>>(const bigint& other) const;
    bigint &operator<<=(const bigint& other);
    bigint &operator>>=(const bigint& other);

    bigint operator<<(unsigned int shift) const;
    bigint operator>>(unsigned int shift) const;
    bigint &operator<<=(unsigned int shift);
    bigint &operator>>=(unsigned int shift);

    bool   operator<(const bigint& other) const;
    bool   operator<=(const bigint& other) const;
    bool   operator>(const bigint& other) const;
    bool   operator>=(const bigint& other) const;
    bool   operator==(const bigint& other) const;
    bool   operator!=(const bigint& other) const;
};

std::ostream &operator<<(std::ostream& os, const bigint& obj);

#endif