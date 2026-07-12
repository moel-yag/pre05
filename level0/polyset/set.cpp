#include "set.hpp"
#include <cstddef>

set::set() : bag_(NULL) {}

set::set(searchable_bag &s_bag) : bag_(&s_bag) {}

set::set(searchable_bag *s_bag) : bag_(s_bag) {}

set::set(const set &source) : bag_(source.bag_) {}

set &set::operator=(const set &source)
{
	if (this != &source)
		bag_ = source.bag_;
	return *this;
}

set::~set() {}

bool set::has(int value) const
{
	if (!bag_)
		return false;
	return bag_->has(value);
}

searchable_bag &set::get_bag() const
{
	return *bag_;
}

void set::insert(int value)
{
	if (!bag_)
		return;
	if (!bag_->has(value))
		bag_->insert(value);
}

void set::insert(int *data, int size)
{
	if (!bag_ || !data || size <= 0)
		return;
	for (int i = 0; i < size; i++)
		insert(data[i]);
}

void set::print() const
{
	if (!bag_)
		return;
	bag_->print();
}

void set::clear()
{
	if (!bag_)
		return;
	bag_->clear();
}