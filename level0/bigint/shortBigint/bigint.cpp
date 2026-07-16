#include "bigint.hpp"
#include <algorithm>

bigint::bigint() : v("0") {}
bigint::bigint(unsigned int n) : v(std::to_string(n)) {}
bigint::bigint(const bigint& o) : v(o.v) {}

std::string bigint::getValue() const { return v; }
bigint& bigint::operator=(const bigint& o) {
    v = o.v;
    return *this;
}

bigint bigint::operator+(const bigint& o) const {
    std::string r;
    int i = v.size(), j = o.v.size(), c = 0;
    while (i || j || c) {
        c += (i ? v[--i] - '0' : 0) + (j ? o.v[--j] - '0' : 0);
        r += c % 10 + '0';
        c /= 10;
    }
    std::reverse(r.begin(), r.end());
    bigint res; res.v = r; return res;
}

bigint& bigint::operator+=(const bigint& o) {
    return *this = *this + o;
}
bigint bigint::operator++(int) {
    bigint t(*this);
    ++(*this);
    return t;
}
bigint& bigint::operator++() {
    return *this += bigint(1);
}

bigint bigint::operator<<(unsigned int s) const {
    bigint t(*this);
    if (t.v != "0")
        t.v.append(s, '0');
    return t;
}
bigint& bigint::operator<<=(unsigned int s) {
    return *this = *this << s;
}

bigint bigint::operator>>(unsigned int s) const {
    bigint t(*this);
    t.v = s >= t.v.size() ? "0" : t.v.substr(0, t.v.size() - s);
    return t;
}
bigint& bigint::operator>>=(unsigned int s) {
    return *this = *this >> s;
}

bigint bigint::operator<<(const bigint& o) const {
    return *this << std::stoul(o.v);
}
bigint& bigint::operator<<=(const bigint& o) {
    return *this = *this << o;
}
bigint bigint::operator>>(const bigint& o) const {
    return *this >> std::stoul(o.v);
}
bigint& bigint::operator>>=(const bigint& o) {
    return *this = *this >> o;
}

bool bigint::operator<(const bigint& o) const {
    return v.size() != o.v.size() ? v.size() < o.v.size() : v < o.v;
}
bool bigint::operator>(const bigint& o) const { return o < *this; }
bool bigint::operator<=(const bigint& o) const { return !(o < *this); }
bool bigint::operator>=(const bigint& o) const { return !(*this < o); }
bool bigint::operator==(const bigint& o) const { return v == o.v; }
bool bigint::operator!=(const bigint& o) const { return v != o.v; }

std::ostream& operator<<(std::ostream& os, const bigint& o) {
    return os << o.getValue();
}