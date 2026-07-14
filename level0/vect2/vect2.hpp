#pragma once
#include <ostream>

class vect2 {
private:
	int x;
	int y;
public:
	vect2();
	vect2(int x, int y);
	vect2(const vect2 &other);
	~vect2();
	vect2 &operator=(const vect2 &other);

	int &operator[](int index);
	const int &operator[](int index) const;

	vect2 operator-() const;

	vect2 operator+(const vect2 &other) const;
	vect2 operator-(const vect2 &other) const;
	vect2 operator*(int scalar) const;

	vect2 &operator+=(const vect2 &other);
	vect2 &operator-=(const vect2 &other);
	vect2 &operator*=(int scalar);

	vect2 &operator++();   // prefix
	vect2 operator++(int); // postfix
	vect2 &operator--();   // prefix
	vect2 operator--(int); // postfix

	bool operator==(const vect2 &other) const;
	bool operator!=(const vect2 &other) const;
};

vect2 operator*(int scalar, const vect2 &v);
std::ostream &operator<<(std::ostream &os, const vect2 &v);
