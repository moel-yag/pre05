#include "vect2.hpp"

vect2::vect2() : x(0), y(0) {}
vect2::vect2(int x, int y) : x(x), y(y) {}
vect2::vect2(const vect2 &copy) x(copy.x), y(copy.y) {}
vect2::~vect2() {}

vect2	&vect2::operator=(const vect2 &other) {
	if (*this != other) {
		x = other.x;
		y = other.y;
	}
	return *this;
}
bool	&vect2::operator==(const vect2 &other) const {
	if (x == &other.x && y == &other.y)
		return true;
	return false;
}
bool	&vect2::operator!=(const vect2 &other) const {
	if (*this == other)
		return false;
	return true;
}
int	&vect2::operator[](int i) {
	if (i == 0)
		return x;
	else if (i == 1)
		return y;
	else
		throw std::out_of_range("out of range..");
}
const int	&vect2::operator[](int i) const {
	if (i == 0)
                return x;
        else if (i == 1)
                return y;
        else
		throw std::out_of_range("out of range..");
}
std::ostream &operator<<(std::ostream &os, const vect2 &v) {
	os << "{" << v[0] << ", " << v[1] << "}";
	return os;
}
vect2	vect2::operator+(const vect2 &other) const {
	int xr = x + other.x;
	int yr = y + other.y;
	return vect2(xr, yr);
}
vect2	vect2::operator-(const vect2 &other) const {
	int xr = x - other.x;
	int yr = y - other.y;
	return vect2(xr, yr);
}
vect2	vect2::operator*(const int scalar) const {
	int xr = x * scalar;
	int yr = y * scalar;
	return vect2(xr, yr);
}

vect2	operator*(const int scalar, const vect2 &other)
{
	return (other * scalar);
}

vect2	&vect2::operator+=(const vect2 &other) {
	*this = *this + other;
	return *this;
}
vect2	&vect2::operator-=(const vect2 &other) {
	*this = *this - other;
	return *this;
}
vect2	&vect2::operator*=(const vect2 &other) {
	*this = *this * other;
	return *this;
}

vect2	&vect2::operator++() {
	*this = *this + 1;
	return *this;
}
vect2	&vect2::operator++(int) {
	vect2 tmp(*this);
	*this  = *this + 1;
	return tmp;
}
vect2	&vect2::operator--() {
	*this = *this - 1;
	return *this;
}
vect2	&vect2::operator--(int) {
	vect2 tmp(*this);
	*this = *this + 1;
	return tmp;
}
vect2	&vect2::operator+() const {
	return *this;
}
vect2	&vect2::operator-() const {
	return vect2(-x, -y);
}

